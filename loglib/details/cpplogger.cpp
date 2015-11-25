#include <Windows.h>
#include <common/path_helpers.h>
#include <common/date_helpers.h>
#include <common/sync_helpers.h>
#include <common/on_exit.h>
#include <common/fs_utils.h>
#include <sstream>
#include <cstdio>
#include <common\pugixml.hpp>
#include <set>
#include <regex>
#include <map>
#include <array>
#include <mutex>
#include "cpplogger_details.h"
#include "zipper.h"

namespace cpplogger
{
	/*Defined constants*/
	static const log_level DEFAULT_LOG_LEVEL = log_level::trace;

	auto const use_developer_log_key = "use_developer_log";
	auto const level_key = "level";
	auto const max_backup_index_key = "max_backup_index";
	auto const max_file_size_key = "max_file_size";
	auto const max_buffer_size_key = "max_buffer_size";
	auto const config_file_root_tag = "log_settings";
	auto const developer_log_tag = "developer_log";
	auto const history_log_tag = "history_log";

	auto const max_config_file_size = 1024 * 1024;
	auto const max_backup_index_up_limit = 100;
	auto const max_backup_index_down_limit = 1;
	auto const max_file_size_up_limit = 1024 * 1024 * 100;
	auto const max_file_size_down_limit = 1024 * 512;
	auto const max_buffer_size_up_limit = 1024 * 1024 * 2;
	auto const max_buffer_size_down_limit = 1024 * 512;
	static const logger_settings default_logger_settings = { false, DEFAULT_LOG_LEVEL, 4, 1048576, 1048576 };

	namespace
	{
		using scanning_func_t = std::function < void(const wstring_t &) > ;
		using dirs_to_skip_t = std::vector < wstring_t > ;

		template <typename handle_t>
		class generic_handle_holder
		{
		public:
			generic_handle_holder(std::function<void(handle_t)> close_func) : _handle(0), _close_func(close_func) {}
			generic_handle_holder(handle_t handle, std::function<void(handle_t)> close_func) : _handle(handle), _close_func(close_func) {}
			~generic_handle_holder()
			{
				if (this->operator bool())
				{
					if (_close_func)
					{
						_close_func(_handle);
					}
					_handle = 0;
				}
			}

			handle_t get()
			{
				return _handle;
			}

			const handle_t get() const
			{
				return _handle;
			}

			explicit operator bool() const
			{
				return !(_handle == INVALID_HANDLE_VALUE || _handle == 0);
			}

			bool operator==(const generic_handle_holder& t) const
			{
				return _handle == t._handle;
			}

			bool operator!=(const generic_handle_holder& t) const
			{
				return !this->operator==(t);
			}

			void swap(generic_handle_holder & x)
			{
				std::swap(x._handle, _handle);
				std::swap(x._close_func, _close_func);
			}

			generic_handle_holder(generic_handle_holder && x) : _handle(0)
			{
				this->swap(x);
			}

			generic_handle_holder & operator = (generic_handle_holder && x)
			{
				this->swap(x);
				return *this;
			}

			generic_handle_holder(const generic_handle_holder &) = delete;
			generic_handle_holder & operator = (const generic_handle_holder &) = delete;
			bool operator<(const generic_handle_holder&) = delete;
			bool operator>(const generic_handle_holder&) = delete;
			bool operator<=(const generic_handle_holder&) = delete;
			bool operator>=(const generic_handle_holder&) = delete;

		private:
			handle_t _handle;
			std::function<void(handle_t)> _close_func;
		};

		class find_file_handle_holder final : public generic_handle_holder<HANDLE>
		{
		public:
			explicit find_file_handle_holder(HANDLE h) : generic_handle_holder(h, [](const HANDLE& handle) { ::FindClose(handle); }) {}
		};

		/*help methods*/
		static void output_debug_print(const wstring_t &message)
		{
			OutputDebugStringW((message + L'\n').c_str());
		}

		static std::uint64_t get_file_size(const wstring_t &full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;

			if (!GetFileAttributesExW(full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"GetFileAttributesExW failed with error:" + std::to_wstring(last_error));
				throw std::runtime_error("cpplogger get_file_size: GetFileAttributesExW failed.");
			}

			LARGE_INTEGER file_size;
			file_size.LowPart = fad.nFileSizeLow;
			file_size.HighPart = fad.nFileSizeHigh;

			if (file_size.QuadPart < 0)
			{
				throw std::runtime_error("cpplogger get_file_size: Negative file size exception.");
			}

			return file_size.QuadPart;

		}

		template <typename T>
		T lexical_cast(const wstring_t &str)
		{
			T value;
			std::wistringstream ss;
			ss.str(str);

			if (ss.fail())
			{
				std::stringstream error;
				error << "Can't convert string " << " to " << typeid(T).name();
				throw std::bad_cast(error.str().c_str());
			}

			ss >> value;

			if (ss.fail())
			{
				std::stringstream error;
				error << "Can't convert string " << " to " << typeid(T).name();
				throw std::bad_cast(error.str().c_str());
			}

			return value;
		}

		bool dir_to_skip(const wstring_t & path, const dirs_to_skip_t& dirs_to_skip)
		{
			return std::find(std::cbegin(dirs_to_skip), std::cend(dirs_to_skip), path) != std::cend(dirs_to_skip);
		}

		static void for_each_fs_object(const wstring_t & path, scanning_func_t func, const dirs_to_skip_t& dirs_to_skip = {})
		{
			const auto search_path = path_helpers::concatenate_paths(path, L"*");
			auto all_dirs_to_skip = dirs_to_skip_t{ L".", L".." };
			all_dirs_to_skip.insert(std::end(all_dirs_to_skip), std::cbegin(dirs_to_skip), std::cend(dirs_to_skip));

			WIN32_FIND_DATAW find_data;
			auto find_handle = find_file_handle_holder{ FindFirstFileW(search_path.c_str(), &find_data) };

			if (find_handle.get() == INVALID_HANDLE_VALUE)
			{
				auto last_error = GetLastError();
				if (last_error == ERROR_FILE_NOT_FOUND)
				{
					return;
				}
				else
				{
					std::wostringstream error;
					error << "FindFirstFile failed with error: " << std::hex << std::showbase << last_error;
					output_debug_print(error.str());
					return;
				}
			}

			if (!dir_to_skip(find_data.cFileName, all_dirs_to_skip))
			{
				const auto &full_path = path_helpers::concatenate_paths(path, find_data.cFileName);
				func(full_path);
			}

			for (;;)
			{
				if (FindNextFileW(find_handle.get(), &find_data) == 0)
				{
					auto last_error = GetLastError();
					if (last_error == ERROR_NO_MORE_FILES)
					{
						return;
					}
					else
					{
						std::wostringstream error;
						error << "FindNextFile failed with error: " << std::hex << std::showbase << last_error;
						output_debug_print(error.str());
						return;
					}
				}
				else if (!dir_to_skip(find_data.cFileName, all_dirs_to_skip))
				{
					const auto &full_path = path_helpers::concatenate_paths(path, find_data.cFileName);
					func(full_path);
				}
			}
		}

		std::tuple <wstring_t, wstring_t, wstring_t, wstring_t> split_path(wstring_t file_full_path)
		{
			const auto long_path_prefix = std::wstring{ L"\\\\?\\" };
			const bool is_long_path = file_full_path.compare(0, long_path_prefix.size(), long_path_prefix) == 0;

			if (is_long_path)
				file_full_path.erase(std::begin(file_full_path), std::begin(file_full_path) + long_path_prefix.size());

			const auto buffer_size = std::size_t{ 32768 };

			auto drive_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(drive_name_buf.get(), buffer_size*sizeof(drive_name_buf[0]));
			auto dir_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(dir_name_buf.get(), buffer_size*sizeof(dir_name_buf[0]));
			auto file_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(file_name_buf.get(), buffer_size*sizeof(file_name_buf[0]));
			auto extension_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(extension_buf.get(), buffer_size*sizeof(extension_buf[0]));

			auto const split_result = _wsplitpath_s(
				file_full_path.c_str(),
				drive_name_buf.get(),
				buffer_size,
				dir_name_buf.get(),
				buffer_size,
				file_name_buf.get(),
				buffer_size,
				extension_buf.get(),
				buffer_size);

			if (split_result != 0)
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path));

			auto drive_name = std::wstring{ drive_name_buf.get() };
			auto dir_name = std::wstring{ dir_name_buf.get() };
			auto file_name = std::wstring{ file_name_buf.get() };
			auto file_ext = std::wstring{ extension_buf.get() };

			if (drive_name.empty() && dir_name.empty() && file_name.empty() && file_ext.empty())
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path) + ";result is empty");

			if (is_long_path && !drive_name.empty())
				drive_name = long_path_prefix + drive_name;

			return std::make_tuple(std::move(drive_name), std::move(dir_name), std::move(file_name), std::move(file_ext));
		}

		static bool remove_log_file(const std::wstring &filename)
		{
			int result = _wremove(filename.c_str());

			return result != 0 ? false : true;
		}

		static bool rename_log_file(const std::wstring &old_filename, const std::wstring &new_filename)
		{
			int result = _wrename(old_filename.c_str(), new_filename.c_str());

			return result != 0 ? false : true;
		}

		bool file_exists(const wstring_t &file_full_path)
		{
			if (file_full_path.empty())
			{
				return false;
			}

			DWORD attributes = GetFileAttributesW(file_full_path.c_str());

			return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		SYSTEMTIME get_file_system_datetime(const wstring_t &file_full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			SYSTEMTIME sys_time;

			if (!GetFileAttributesExW(file_full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"GetFileAttributesExW failed with error:" + std::to_wstring(last_error));
				throw std::runtime_error("GetFileAttributesExW failed in cpplogger::get_file_system_datetime.");
			}

			if (!FileTimeToSystemTime(&fad.ftLastWriteTime, &sys_time))
			{
				const auto last_error = GetLastError();
				output_debug_print(L"FileTimeToSystemTime failed with error:" + std::to_wstring(last_error));
				throw std::runtime_error("FileTimeToSystemTime failed in cpplogger::get_file_system_datetime.");
			}

			return sys_time;
		}
	} //END namespace

	void initialize(const wstring_t &config,
		const wstring_t &log_path,
		const wstring_t &log_file_prefix,
		bool watch_config_changes,
		const notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func)
	{
		details::get_logger_instance()->initialize(config,log_path, log_file_prefix, watch_config_changes, notify_developers_logging_enabled_func);
	}

	void reload_settings_from_config(const wstring_t & config)
	{
		details::get_logger_instance()->reload_settings_from_config(config);
	}

	void reload_settings_from_config()
	{
		details::get_logger_instance()->reload_settings_from_config();
	}


	void log_message(const log_level &level, const wstring_t &message)
	{
		details::get_logger_instance()->log_message(level, message);
	}

	wstring_t get_config()
	{
		return details::get_logger_instance()->get_config();
	}

	wstring_t get_log_path()
	{
		return details::get_logger_instance()->get_log_path();
	}

	wstring_t get_log_prefix()
	{
		return details::get_logger_instance()->get_log_prefix();
	}

	logger_settings get_current_logger_settings()
	{
		return details::get_logger_instance()->get_current_logger_settings();
	}

	logger_settings get_default_logger_settings()
	{
		return default_logger_settings;
	}

	logger_settings read_logger_settings(const wstring_t &config)
	{
		logger_settings tmp_settings = default_logger_settings;
		pugi::xml_document doc;

		if (config.size() > max_config_file_size)
		{
			throw details::internal_logger_exception(L"Very big config size: " + std::to_wstring(config.size()) + L", apply default logger settings.");
		}

		pugi::xml_parse_result parse_result = doc.load_buffer(config.c_str(), config.size()*sizeof(wchar_t), pugi::parse_default, pugi::encoding_utf16);

		if (!parse_result)
		{
			throw details::internal_logger_exception(string_utils::convert_utf8_to_wchar(parse_result.description()) + L",status: " + std::to_wstring(parse_result.status)
				+ L",offset: " + std::to_wstring(parse_result.offset) + L", apply default logger settings.");
		}

		pugi::xml_node node_log_settings = doc.child(config_file_root_tag);
		pugi::xml_node node_developer_log = node_log_settings.child(developer_log_tag);
		pugi::xml_node node_history_log = node_log_settings.child(history_log_tag);

		/*Parse developer log tag*/
		if (node_developer_log.empty() && node_history_log.empty())
		{
			return default_logger_settings;
		}

		if (!node_developer_log.attribute(use_developer_log_key).empty())
		{
			tmp_settings.use_developer_log = node_developer_log.attribute(use_developer_log_key).as_bool();
		}

		if (!node_developer_log.attribute(level_key).empty())
		{
			tmp_settings.level = get_log_level_by_config_value(node_developer_log.attribute(level_key).value());
		}

		if (!node_developer_log.attribute(max_backup_index_key).empty())
		{
			tmp_settings.max_backup_index = node_developer_log.attribute(max_backup_index_key).as_uint();
		}

		if (!node_developer_log.attribute(max_file_size_key).empty())
		{
			tmp_settings.max_file_size = node_developer_log.attribute(max_file_size_key).as_ullong();
		}

		/*Parse history log tag*/

		if (!node_history_log.attribute(max_buffer_size_key).empty())
		{
			tmp_settings.max_buffer_size = node_history_log.attribute(max_buffer_size_key).as_ullong();
		}

		return tmp_settings;
	}

	void write_logger_settings(const std::wstring &full_file_path, const logger_settings &settings)
	{
		pugi::xml_document doc;
		pugi::xml_node log_settings = doc.append_child(config_file_root_tag);
		pugi::xml_node developer_log = log_settings.append_child(developer_log_tag);
		developer_log.append_attribute(use_developer_log_key) = settings.use_developer_log;
		developer_log.append_attribute(level_key) = get_config_value_by_log_level(settings.level).c_str();
		developer_log.append_attribute(max_backup_index_key) = settings.max_backup_index;
		developer_log.append_attribute(max_file_size_key) = settings.max_file_size;

		pugi::xml_node history_log = log_settings.append_child(history_log_tag);
		history_log.append_attribute(max_buffer_size_key) = settings.max_buffer_size;

		if (!doc.save_file(full_file_path.c_str())) {
			throw details::internal_logger_exception(L"Config file save error.");
		}
	}

	void validate_logger_settings(logger_settings & settings, const validate_report_func_t & validate_report_func)
	{
		/*Check max backup index*/
		if (settings.max_backup_index < max_backup_index_down_limit)
		{
			validate_report_func(L"max_backup_index", std::to_wstring(settings.max_backup_index), std::to_wstring(max_backup_index_down_limit));
			settings.max_backup_index = max_backup_index_down_limit;
		}
		else if (settings.max_backup_index > max_backup_index_up_limit)
		{
			validate_report_func(L"max_backup_index", std::to_wstring(settings.max_backup_index), std::to_wstring(max_backup_index_up_limit));
			settings.max_backup_index = max_backup_index_up_limit;
		}

		/*Check max buffer size*/
		if (settings.max_buffer_size < max_buffer_size_down_limit)
		{
			validate_report_func(L"max_buffer_size", std::to_wstring(settings.max_buffer_size), std::to_wstring(max_buffer_size_down_limit));
			settings.max_buffer_size = max_buffer_size_down_limit;
		}
		else if (settings.max_buffer_size > max_buffer_size_up_limit)
		{
			validate_report_func(L"max_buffer_size", std::to_wstring(settings.max_buffer_size), std::to_wstring(max_buffer_size_up_limit));
			settings.max_buffer_size = max_buffer_size_up_limit;
		}

		/*Check max file size*/
		if (settings.max_file_size < max_file_size_down_limit)
		{
			validate_report_func(L"max_file_size", std::to_wstring(settings.max_file_size), std::to_wstring(max_file_size_down_limit));
			settings.max_file_size = max_file_size_down_limit;
		}
		else if (settings.max_file_size > max_file_size_up_limit)
		{
			validate_report_func(L"max_file_size", std::to_wstring(settings.max_file_size), std::to_wstring(max_file_size_up_limit));
			settings.max_file_size = max_file_size_up_limit;
		}
	}

	void validate_logger_settings(logger_settings &settings, const wstring_t &full_config_path)
	{
		const auto validate_report_func = [&full_config_path](const std::wstring & param_name, const std::wstring & original_value, const std::wstring & modified_value)
		{
			output_debug_print(L"Replace current '" + param_name + L"' for file: " + full_config_path + L" with value = " + modified_value + L". Original value was = " + original_value + L'.');
		};

		validate_logger_settings(settings, validate_report_func);
	}

	wstring_t create_unique_zip_file_name(const wstring_t &log_path, const wstring_t &log_file_prefix_narrow)
	{
		time_t current_time;
		time(&current_time);
		auto zip_backup_index = 0;

		std::wostringstream ss;
		ss << log_file_prefix_narrow << date_helpers::local_time_to_str(current_time, "%Y-%m-%d_%H-%M-%S").c_str();
		const auto zip_base_name = ss.str();
		ss << ".zip";
		auto zip_file_name = ss.str();

		while (file_exists(path_helpers::concatenate_paths(log_path, zip_file_name)))
		{
			++zip_backup_index;
			const auto temp = zip_base_name + L"_(" + std::to_wstring(zip_backup_index) + L")" + L".zip";
			zip_file_name = temp;
		}

		return zip_file_name;
	}

	void check_and_compress_history_logs(const wstring_t &log_path, const wstring_t &log_file_prefix, const logger_settings &settings)
	{
		std::vector <wstring_t> history_log_files;
		std::vector <wstring_t> zip_files;
		std::uint64_t summary_zip_files_size = 0;
		wstring_t log_file_prefix_narrow = log_file_prefix;

		for_each_fs_object(log_path, [&](const wstring_t &full_file_path)
		{
			/*Don't pack zip archives*/
			if (full_file_path.find(L".zip") == wstring_t::npos)
			{
				wstring_t base_name;
				wstring_t file_ext;
				std::tie(std::ignore, std::ignore, base_name, file_ext) = split_path(full_file_path);
				wstring_t file_name = base_name + file_ext;

				auto const process_id = std::to_wstring(GetCurrentProcessId());
				auto const process_id_prefix = wstring_t(L"_") + process_id;
				auto const regex_date_pattern = L"[0-9]{4}-[0-9]{2}-[0-9]{2}";
				auto const find_pos = log_file_prefix_narrow.find(process_id_prefix);

				if (find_pos != wstring_t::npos)
				{
					log_file_prefix_narrow = log_file_prefix_narrow.substr(0, find_pos);
				}

				if (file_name.find(log_file_prefix_narrow) != wstring_t::npos && std::regex_search(file_name, std::wregex(regex_date_pattern)))
				{
					history_log_files.push_back(full_file_path);
				}
			}
			else
			{
				zip_files.push_back(full_file_path);
				summary_zip_files_size += get_file_size(full_file_path);
			}
		});

		/*process stale zip files*/
		if (!zip_files.empty() && summary_zip_files_size > settings.max_file_size)
		{
			std::sort(zip_files.begin(), zip_files.end(), [](const wstring_t& file1, const wstring_t& file2)
			{
				SYSTEMTIME st1 = get_file_system_datetime(file1);
				SYSTEMTIME st2 = get_file_system_datetime(file2);
				FILETIME ft1;
				FILETIME ft2;

				if (!SystemTimeToFileTime(&st1, &ft1) || !SystemTimeToFileTime(&st2, &ft2))
				{
					throw std::runtime_error("Exception: SystemTimeToFileTime failed.");
				}

				LONG result = CompareFileTime(&ft1, &ft2);

				return result == -1 ? true : false;
			});

			for (const auto &it : zip_files)
			{
				auto const file_size = get_file_size(it);

				if (summary_zip_files_size > settings.max_file_size)
				{
					remove_log_file(it);
					summary_zip_files_size -= file_size;
				}
				else
				{
					break;
				}
			}
		}

		/*process history log files*/
		if (history_log_files.empty())
		{
			return;
		}

		bool zipper_exception_error = true;
		const auto zip_file_name = create_unique_zip_file_name(log_path, log_file_prefix_narrow);

		try {
			/*compression_level = 1 - Z_BEST_SPEED, 0 - Z_NO_COMPRESSION, 9 - Z_BEST_COMPRESSION, (-1) - Z_DEFAULT_COMPRESSION*/
			zip::create_archive(history_log_files, log_path, zip_file_name, 1);

			/*remove compressed history logs*/
			for (const auto &it : history_log_files)
			{
				//without checking, it's not critical. 
				remove_log_file(it);
			}

			zipper_exception_error = false;
		}
		catch (const std::ios_base::failure &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}
		catch (const zip::compression_exception &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}
		catch (const std::exception &e)
		{
			output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
		}

		/*remove zip archive in exception case*/
		if (zipper_exception_error)
		{
			wstring_t full_zip_path = path_helpers::concatenate_paths(log_path, zip_file_name);

			if (file_exists(full_zip_path))
			{
				//without checking, it's not critical. 
				remove_log_file(full_zip_path);
			}
		}
	}

	wstring_t get_config_value_by_log_level(log_level level)
	{
		switch (level)
		{
		case cpplogger::log_level::trace:
			return L"TRACE";
		case cpplogger::log_level::debug:
			return L"DEBUG";
		case cpplogger::log_level::info:
			return L"INFO";
		case cpplogger::log_level::warn:
			return L"WARN";
		case cpplogger::log_level::error:
			return L"ERROR";
		case cpplogger::log_level::fatal:
			return L"FATAL";
		}

		throw details::internal_logger_exception(L"Invalid log level value.");
	}

	log_level get_log_level_by_config_value(const string_t &val)
	{
		std::map <string_t, log_level> log_level_string_map;
		string_t tmp = string_utils::trim(val);

		log_level_string_map.emplace("TRACE", log_level::trace);
		log_level_string_map.emplace("DEBUG", log_level::debug);
		log_level_string_map.emplace("INFO", log_level::info);
		log_level_string_map.emplace("WARN", log_level::warn);
		log_level_string_map.emplace("ERROR", log_level::error);
		log_level_string_map.emplace("FATAL", log_level::fatal);

		if (tmp.empty())
		{
			throw details::internal_logger_exception(L"Empty log level config value.");
		}

		auto it = log_level_string_map.find(tmp);

		if (it != log_level_string_map.end())
		{
			return it->second;
		}

		throw details::internal_logger_exception(L"Incorrect log level config value.");
	}

	namespace details 
	{
		const DWORD config_watch_period = 3000; // 3 sec

		class initial_logger final : public logger_interface
		{
		public:
			initial_logger() {}

			void initialize(const wstring_t &config,
				const wstring_t &log_path,
				const wstring_t &log_file_prefix,
				bool,
				const notify_developers_logging_enabled_func_t &) override
			{
				std::lock_guard<decltype(_config_mtx)> config_lock(_config_mtx);

				_config = config;
				log_path_ = log_path;
				log_file_prefix_ = log_file_prefix;
			}

			void reload_settings_from_config(const wstring_t &) override
			{
				// do nothing
			}

			void reload_settings_from_config() override
			{
				// do nothing
			}

			logger_settings get_current_logger_settings() const override
			{
				return default_logger_settings;
			}

			wstring_t get_config() const override
			{
				std::lock_guard<decltype(_config_mtx)> config_lock(_config_mtx);
				return _config;
			}

			wstring_t get_log_path() const override
			{
				std::lock_guard<decltype(_config_mtx)> config_lock(_config_mtx);
				return log_path_;
			}

			wstring_t get_log_prefix() const override
			{
				std::lock_guard<decltype(_config_mtx)> config_lock(_config_mtx);
				return log_file_prefix_;
			}
			
			void log_message(const log_level &, const wstring_t &message) override
			{
				output_debug_print(message);
			}

			initial_logger(const initial_logger &) = delete;
			initial_logger & operator = (const initial_logger &) = delete;
		private:
			wstring_t log_path_;
			wstring_t log_file_prefix_;
			wstring_t _config;

			mutable std::recursive_mutex _config_mtx;
		};

		namespace logger_instance
		{
			using logger_ptr = std::shared_ptr < logger_interface > ;
			
			volatile LONG finalization_ = 0;
			sync_helpers::rw_lock _instance_lock;

			class logger_instance_wrapper final
			{
			public:
				logger_instance_wrapper() : instance_(logger_ptr(new initial_logger)) {}

				~logger_instance_wrapper()
				{
					finalization_ = 1;

					sync_helpers::rw_lock_guard_exclusive lock(_instance_lock);
					instance_.reset();
				}

				friend void set_instance(logger_ptr new_instance);
				friend logger_ptr get_instance();
			private:
				logger_ptr instance_;
			};

			logger_instance_wrapper logger_instance_wrapper_;

			void set_instance(logger_ptr new_instance)
			{
				if (finalization_) return;
				
				sync_helpers::rw_lock_guard_exclusive lock(_instance_lock);

				if (finalization_) return;

				std::swap(logger_instance_wrapper_.instance_, new_instance);
			}

			logger_ptr get_instance()
			{
				if (finalization_) return std::make_shared<initial_logger>();

				sync_helpers::rw_lock_guard_shared lock(_instance_lock);

				if (finalization_) return std::make_shared<initial_logger>();

				return logger_instance_wrapper_.instance_;
			}
		}

		using logger_instance::logger_ptr;

		sync_helpers::once_flag init_once_;

		logger_ptr get_logger_instance()
		{
			sync_helpers::call_once(init_once_, []()
			{
				logger_ptr new_instance = std::make_shared<Logger>();
				logger_instance::set_instance(new_instance);
			});

			return logger_instance::get_instance();
		}

		struct log_file_comp
		{
			bool operator()(const log_file_info &info1, const log_file_info &info2)
			{
				return info1.backup_index < info2.backup_index;
			}
		};

		logger_interface::~logger_interface() {}

		Logger::Logger()
			: current_logger_settings_(default_logger_settings), flag_internal_error_(false), flag_io_error_(false),
			_watch_config_changes(false), _last_config_write_time(),
			_stop_watching_event(sync_helpers::create_basic_event_object(true))
		{
		}

		Logger::~Logger()
		{
			sync_helpers::set_event(_stop_watching_event);

			if (_config_watch_thread.joinable())
			{
				_config_watch_thread.join();
			}
		}

		void Logger::initialize(const wstring_t &config,
			const wstring_t &log_path,
			const wstring_t &log_file_prefix,
			bool watch_config_changes,
			const logger::notify_developers_logging_enabled_func_t & notify_developers_logging_enabled_func)
		{
			auto current_logger_settings = default_logger_settings;
			decltype(_last_config_write_time) last_config_write_time{};

			try
			{
				current_logger_settings = read_logger_settings(config);
			}
			catch (const std::exception & exc)
			{
				output_debug_print(L"Could not read logger settings. Error: " + string_utils::convert_utf8_to_wchar(exc.what()));
			}

			if (current_logger_settings != default_logger_settings)
			{
				validate_logger_settings(current_logger_settings, config);
			}

			check_and_compress_history_logs(log_path, log_file_prefix, current_logger_settings);

			std::thread config_watch_thread;

			if (watch_config_changes)
			{
				config_watch_thread = std::thread([this]
				{
					try
					{
						this->watch_config_change();
					}
					catch (const std::exception & exc)
					{
						output_debug_print(string_utils::convert_utf8_to_wchar(exc.what()));
					}
					catch (...)
					{
						output_debug_print(L"An unknown exception was caught.");
					}
				});
			}

			{
				sync_helpers::rw_lock_guard_exclusive lock(_config_lock);
				_config_watch_thread = std::move(config_watch_thread);
				log_path_ = log_path;
				log_file_prefix_ = log_file_prefix;
				_config = config;
				_watch_config_changes = watch_config_changes;
				current_logger_settings_ = std::move(current_logger_settings);
				_last_config_write_time = last_config_write_time;
				_notify_developers_logging_enabled_func = notify_developers_logging_enabled_func;
			}

			{
				sync_helpers::rw_lock_guard_shared lock(_config_lock);
				if (_notify_developers_logging_enabled_func && current_logger_settings_.use_developer_log)
				{
					_notify_developers_logging_enabled_func(true);
				}
			}
		}

		void Logger::reload_settings_from_config()
		{
			decltype(_config) config;
			{
				sync_helpers::rw_lock_guard_shared lock(_config_lock);
				config = _config;
			}
			reload_settings_from_config(config);
		}

		void Logger::reload_settings_from_config(const wstring_t & config)
		{
			decltype(current_logger_settings_) current_logger_settings;
			
			{
				sync_helpers::rw_lock_guard_shared lock(_config_lock);
				current_logger_settings = current_logger_settings_;
			}

			decltype(current_logger_settings_) new_logger_settings;

			try
			{
				new_logger_settings = read_logger_settings(config);
			}
			catch (const details::internal_logger_exception & exc)
			{
				output_debug_print(L"Could not read logger settings. Error: " + string_utils::convert_utf8_to_wchar(exc.what()));
				return;
			}
			
			if (new_logger_settings == current_logger_settings)
			{
				output_debug_print(L"Settings read from configuration file does not differ from the current settings.");
				return;
			}

			if (new_logger_settings != default_logger_settings)
			{
				validate_logger_settings(new_logger_settings, config);
			}

			bool developers_logs_flag_changed = false;
			
			{
				sync_helpers::rw_lock_guard_exclusive lock(_config_lock);
				developers_logs_flag_changed = new_logger_settings.use_developer_log != current_logger_settings_.use_developer_log;
				_config = config;
				current_logger_settings_ = std::move(new_logger_settings);
			}
		
			{
				sync_helpers::rw_lock_guard_shared lock(_config_lock);
				if (developers_logs_flag_changed && _notify_developers_logging_enabled_func)
				{
					_notify_developers_logging_enabled_func(current_logger_settings_.use_developer_log);
				}
			}
		}
		
		logger_settings Logger::get_current_logger_settings() const
		{
			sync_helpers::rw_lock_guard_shared lock(_config_lock);
			return current_logger_settings_;
		}

		wstring_t Logger::get_config() const
		{
			sync_helpers::rw_lock_guard_shared lock(_config_lock);
			return _config;
		}

		wstring_t Logger::get_log_path() const
		{
			sync_helpers::rw_lock_guard_shared lock(_config_lock);
			return log_path_;
		}

		wstring_t Logger::get_log_prefix() const
		{
			sync_helpers::rw_lock_guard_shared lock(_config_lock);
			return log_file_prefix_;
		}

		bool Logger::log_file_exist(std::size_t backup_index) const
		{
			auto it = std::find_if(log_files_.begin(), log_files_.end(), [&](const log_file_info &info)
			{
				return info.backup_index == backup_index;
			});

			return it != log_files_.end();
		}

		void Logger::synchronize_log_file_directory()
		{
			std::vector <wstring_t> directory_log_files;
			wstring_t match_pattern(log_file_prefix_ + L".log");

			for_each_fs_object(log_path_, [&](const wstring_t &file_path)
			{
				if (file_path.find(match_pattern) != wstring_t::npos)
				{
					directory_log_files.emplace(directory_log_files.end(), file_path);
					log_file_info file_info;
					file_info.full_path = file_path;
					wstring_t tmp_index;

					for (auto it = file_path.rbegin(); it != file_path.rend(); ++it)
					{
						if (iswdigit(*it))
						{
							tmp_index.insert(tmp_index.begin(), *it);
						}
						else
						{
							break;
						}
					}

					if (tmp_index.empty())
					{
						file_info.backup_index = 0;
					}
					else
					{
						file_info.backup_index = lexical_cast<std::size_t>(tmp_index);
					}

					if (!log_file_exist(file_info.backup_index)) 
					{
						log_files_.insert(log_files_.begin(), file_info);
					}
				}
			});
		
			/*Checking when user delete some logs in file system, we need sync it with log_files container inside the program*/

			log_files_.erase(std::remove_if(log_files_.begin(), log_files_.end(), [&](const log_file_info &info)
			{
				return std::find(directory_log_files.begin(), directory_log_files.end(),info.full_path) == directory_log_files.end();
			}), log_files_.end());

			/*In case when user change 'max_backup_index' in configuration file and restart logger
			For example, user reduced 'max_backup_index' from 5 till 3, then
			logger manually delete logs '<log_prefix>.log.4' and '<log_prefix>.log.5'
			*/

			if (log_files_.empty()) return;

			std::sort(log_files_.begin(), log_files_.end(), log_file_comp());

			while (log_files_.back().backup_index > get_current_logger_settings().max_backup_index)
			{
				cleanup_stale_log();

				if (log_files_.empty()) return;
			}
		}

		std::wstring Logger::create_file_path(std::size_t backup_index)
		{
			std::wostringstream ss;

			if (!log_path_.empty()) {
				ss << path_helpers::concatenate_paths(log_path_, L"\\");
			}

			ss << log_file_prefix_;
			ss << ".log";

			if (backup_index != 0)
			{
				ss << ".";
				ss << backup_index;
			}

			return ss.str();
		}

		void Logger::create_log_file()
		{
			log_file_info file_info{ wstring_t(), 0 };
			file_info.full_path = create_file_path(file_info.backup_index);

			if (!log_file_exist(file_info.backup_index))
			{
				log_files_.insert(log_files_.begin(), file_info);
			}

			log_writer_.open(file_info.full_path, logger::open_disposition::file_open_normal_mode);
		}

		void Logger::cleanup_stale_log()
		{
			if (!remove_log_file(log_files_.back().full_path))
			{
				throw internal_logger_exception(L"Can't remove file" + log_files_.back().full_path);
			}

			log_files_.pop_back();
		}

		void Logger::rolling_files()
		{
			log_writer_.close();
			synchronize_log_file_directory();

			const auto & current_logger_settings = get_current_logger_settings();

			if (log_files_.back().backup_index == current_logger_settings.max_backup_index)
			{
				cleanup_stale_log();
			}

			for (auto it = log_files_.rbegin(); it != log_files_.rend(); ++it)
			{
				it->backup_index++;
				wstring_t target_path = create_file_path(it->backup_index);

				if (!rename_log_file(it->full_path, target_path))
				{
					throw internal_logger_exception(L"Can't rename log file from: " + it->full_path + L" to: " + target_path);
				}
				else
				{
					it->full_path = target_path;
				}
			}

			create_log_file();
		}

		void Logger::clear_log_file_data()
		{
			log_writer_.close();

			if (!log_files_.empty())
			{
				log_writer_.open(log_files_.front().full_path, logger::open_disposition::file_open_clear_mode);
				log_writer_.close();
			}
		}

		void Logger::write_message_to_file(const std::wstring &message)
		{
			if (!log_writer_.is_open())
			{
				create_log_file();
			}

			log_writer_.write(message + L"\r\n");
		}

		void Logger::log_message(const log_level &level, const std::wstring &message)
		{
			const auto current_logger_settings = get_current_logger_settings();
			
			if (current_logger_settings.level <= level)
			{
				std::lock_guard<decltype(log_write_mutex_)> lock(log_write_mutex_);

				try
				{
					if (log_files_.empty() || flag_io_error_)
					{
						create_log_file();
						flag_io_error_ = false;
					}

					if (!flag_io_error_)
					{
						while (!cached_exception_log_messages_.empty())
						{
							write_message_to_file(cached_exception_log_messages_.front());
							cached_exception_log_messages_.pop();
						}

						write_message_to_file(message);

						std::uint64_t file_size = get_file_size(log_writer_.current_log_file_path());

						if (!flag_internal_error_)
						{
							if (file_size >= current_logger_settings.max_file_size)
							{
								rolling_files();
							}
						}
						else
						{
							clear_log_file_data();
							create_log_file();
						}

						flag_internal_error_ = false;
					}
				}
				catch (const logger::log_file_writer_exception &e)
				{
					output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
					cached_exception_log_messages_.push(message);
					flag_io_error_ = true;
				}
				catch (const internal_logger_exception &e)
				{
					output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
					cached_exception_log_messages_.push(message);
					flag_internal_error_ = true;
				}
				catch (const std::exception &e)
				{
					output_debug_print(string_utils::convert_utf8_to_wchar(e.what()));
				}
				catch (...)
				{
					output_debug_print(L"Unknown exception.");
				}
			}
		}

		void Logger::watch_config_change()
		{
		}


	} //END details

	void free_logger_instance()
	{
		auto logger = std::make_shared<details::initial_logger>();
		details::logger_instance::set_instance(logger);
	}

} //END cpplogger
#include "CppUnitTest.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <loglib\log.h>
#include <loglib\details\cpplogger_details.h>
#include <loglib\details\zipper.h>
#include <common\unit_tests_common.h>
#include <common\fs_helpers.h>
#include <common\fs_utils.h>
#include <common\path_helpers.h>
#include <common\string_utils.h>
#include <common\date_helpers.h>
#include <common\on_exit.h>

#pragma warning(push)
#pragma warning(disable:4244) //conversion from 'int' to 'char', possible loss of data, poco-1.6.0\zip\include\poco\zip\ziplocalfileheader.h(321), poco-1.6.0\zip\include\poco\zip\zipfileinfo.h(405)
#include <Poco/Zip/Decompress.h>
#pragma warning(pop)

#include <ShlDisp.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	/*for logging by singletone instance*/
	void run_logging_in_cycle()
	{
		for (int n = 0; n < 1000; ++n)
		{
			LOG_DEBUG() << n;
		}

		LOG_ERROR() << "Flush all messages.";
	}

	void init_logger_instance()
	{
		const auto current_dir = fs_utils::get_full_path_name(L".");
		
		const std::wstring configs_dir(current_dir);
		const std::wstring config_file_name(L"log.xml");
		const std::wstring logs_dir(current_dir);
		const std::wstring log_file_prefix(L"скан");

		logger::initialize(configs_dir, config_file_name, logs_dir, log_file_prefix, false, {});
	}

	/*for logging by different instances*/
	const std::wstring test_message = L"This is testing log message";
	const std::wstring trace_test_message = L"TRACE: this is testing log message";
	const std::wstring debug_test_message = L"DEBUG: this is testing log message";
	const std::wstring info_test_message = L"INFO: this is testing log message";
	const std::wstring warn_test_message = L"WARN: this is testing log message";
	const std::wstring error_test_message = L"ERROR: this is testing log message";
	const std::wstring fatal_test_message = L"FATAL: this is testing log message";

	const std::uint64_t test_message_mem_size = test_message.size() * sizeof(std::wstring::value_type);

	const cpplogger::logger_settings default_logger_settings_use_dev_log = { true, cpplogger::log_level::trace, 4, 1048576, 1048576 };
	const cpplogger::logger_settings default_logger_settings_no_use_dev_log = { false, cpplogger::log_level::trace, 4, 1048576, 1048576 };

	std::vector <std::wstring> unexpectable_debug_messages = {L"TRACE"};
	std::vector <std::wstring> unexpectable_info_messages = {L"TRACE",L"DEBUG"};
	std::vector <std::wstring> unexpectable_warn_messages = {L"TRACE",L"DEBUG",L"INFO"};
	std::vector <std::wstring> unexpectable_error_messages = {L"TRACE",L"DEBUG",L"INFO",L"WARN"};
	std::vector <std::wstring> unexpectable_fatal_messages = {L"TRACE",L"DEBUG",L"INFO",L"WARN",L"ERROR"};

	std::vector <std::wstring> share_modes = { L"FILE_SHARE_READ", L"FILE_SHARE_WRITE", L"FILE_SHARE_DELETE" };
	std::vector <std::uint8_t> empty_zip_hex_mask = { 0x50, 0x4B, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
													  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	std::size_t get_file_lines_count(const std::wstring &file_name)
	{
		std::size_t counter = 0;
		std::wifstream file(file_name);
		std::wstring line;
		file.exceptions(std::ios_base::badbit);

		while (file >> std::ws && std::getline(file, line)) ++counter;

		return counter;
	}

	std::wstring get_last_file_line(const std::wstring &file_name)
	{
		std::wifstream file(file_name);
		file.exceptions(std::ios_base::badbit);
		std::wstring line;

		while (file >> std::ws && std::getline(file, line));

		return line;
	}

	std::wstring get_first_file_line(const std::wstring &file_name)
	{
		std::wifstream file(file_name);
		file.exceptions(std::ios_base::badbit);

		std::wstring line;
		file.seekg(0, std::ios::beg);
		std::getline(file, line);

		return line;
	}

	void generate_test_level_messages(cpplogger::details::Logger &log)
	{
		for (size_t i = 0; i < 20; ++i)
		{
			log.log_message(cpplogger::log_level::trace, trace_test_message);
			log.log_message(cpplogger::log_level::debug, debug_test_message);
			log.log_message(cpplogger::log_level::info, info_test_message);
			log.log_message(cpplogger::log_level::warn, warn_test_message);
			log.log_message(cpplogger::log_level::error, error_test_message);
			log.log_message(cpplogger::log_level::fatal, fatal_test_message);
		}
	}

	cpplogger::logger_settings generate_test_logger_settings(std::size_t max_backup_index, std::uint64_t max_file_size)
	{
		cpplogger::logger_settings settings = default_logger_settings_use_dev_log;
		settings.max_backup_index = max_backup_index;
		settings.max_file_size = max_file_size;
		return settings;
	}

	void log_level_testing(cpplogger::log_level level,
						   const std::wstring &configs_dir,
						   const std::wstring &config_file_name,
						   const std::wstring &logs_dir,
						   const std::wstring &log_file_prefix)
	{
		cpplogger::details::Logger log;
		cpplogger::logger_settings settings = default_logger_settings_use_dev_log;
		settings.level = level;
		write_logger_settings(path_helpers::concatenate_paths(configs_dir,config_file_name),settings);
		log.initialize(configs_dir,config_file_name,logs_dir,log_file_prefix);
		generate_test_level_messages(log);
	}

	bool unexpectable_messages_exist(const std::wstring &file_full_path, const std::vector<std::wstring> &patterns)
	{
		if (!fs_utils::file_exists(file_full_path))
		{
			return true;
		}

		std::wifstream in(file_full_path);
		in.exceptions(std::ios_base::badbit);
		std::wstring line;

		for (const auto &it : patterns)
		{
			while (in >> std::ws && std::getline(in, line))
			{
				if (line.find(it) != std::string::npos)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::wstring get_log_file_full_path(const std::wstring &logs_dir,const std::wstring &log_file_prefix)
	{
		std::wostringstream ss;
		ss << path_helpers::concatenate_paths(logs_dir,log_file_prefix);
		ss << ".log";
		return ss.str();
	}
	
	void clean_directory(const std::wstring &path)
	{
		fs_helpers::for_all_fs_objects(path, [&](const std::wstring& file_path)
		{
			fs_utils::remove_read_only_attribute(file_path);
			fs_utils::delete_file(file_path);
		});
	}

	void remove_directory(const std::wstring &path)
	{
		fs_utils::remove_read_only_attribute(path);

		clean_directory(path);

		if (fs_utils::check_access_rights(path, DELETE))
		{
			fs_utils::remove_directory(path);
		}
	}

	void prepare_test_directory(const std::wstring &path)
	{
		if (fs_utils::directory_exists(path))
		{
			clean_directory(path);
		}
		else
		{
			fs_utils::create_directory(path);
		}
	}

	void run_test_process(PPROCESS_INFORMATION processInfo, const std::wstring & exe_name, const std::wstring & params)
	{
		const std::size_t params_buffer_size = 32768;
		const std::size_t max_chars_num = params_buffer_size - 1;

		const std::wstring full_params = params;
		wchar_t params_buffer[params_buffer_size];
		std::fill_n(params_buffer, params_buffer_size, 0);
		std::copy_n(full_params.begin(), std::min(full_params.length(), max_chars_num), params_buffer);

		STARTUPINFOW startup_info{};
		if (!CreateProcessW(
			exe_name.c_str(),
			params_buffer,
			0,
			0,
			TRUE,
			0,
			0,
			0,
			&startup_info,
			processInfo))
		{
			Assert::Fail(L"Can't run test process");
		}
	}

	void create_fictive_log_file(const std::wstring &logs_dir, 
								 const std::wstring &log_file_prefix, 
								 std::size_t max_backup_index, 
							     DWORD dwSharedMode, 
								 DWORD dwFlagsAndAttributes)
	{
		std::wostringstream full_file_name;
		full_file_name << path_helpers::concatenate_paths(logs_dir, log_file_prefix);
		full_file_name << ".log";
		full_file_name << max_backup_index;

		HANDLE hfile = CreateFileW(full_file_name.str().c_str(), GENERIC_READ | GENERIC_WRITE, dwSharedMode, 0, CREATE_ALWAYS, dwFlagsAndAttributes, NULL);

		if (hfile == INVALID_HANDLE_VALUE)
		{
			Assert::Fail(L"Error in creation fictive log file");
		}

		CloseHandle(hfile);
	}

	std::wstring create_history_log_file_path(const std::wstring &logs_dir, const std::wstring &log_prefix, std::size_t index)
	{
		time_t current_time;
		time(&current_time);
		std::wostringstream ss;
		ss << path_helpers::concatenate_paths(logs_dir, log_prefix) << date_helpers::local_time_to_str(current_time, "%Y-%m-%d_%H-%M-%S").c_str() 
		   << L"_" << std::to_wstring(index) << L".log";
		return ss.str();
	}

	std::wstring get_full_path(const std::wstring &relative_path)
	{
		if (relative_path.empty())
		{
			Assert::Fail(L"Relative path is empty");
		}

		auto full_path = std::unique_ptr < wchar_t[] > {new wchar_t[MAX_PATH]};

		if (!_wfullpath(full_path.get(), relative_path.c_str(), MAX_PATH))
		{
			Assert::Fail(L"_wfullpath failed assertion");
		}

		return std::wstring(full_path.get());
	}

	void unzip_using_shell(const std::wstring &source_path, const std::wstring &destination_path)
	{
		HRESULT hResult = S_FALSE;
		IShellDispatch *pIShellDispatch = NULL;
		Folder *pToFolder;
		Folder *pFromFolder;
		VARIANT variantDir, variantFile, variantOpt;

		CoInitialize(NULL);
		hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pIShellDispatch);

		if (FAILED(hResult))
		{
			Assert::Fail(L"Initialize COM failed");
		}

		utils::on_exit uninitialize_com_on_exit([&]
		{
			pFromFolder->Release();
			pToFolder->Release();
			pIShellDispatch->Release();
			CoUninitialize();
		});

		VariantInit(&variantDir);
		variantDir.vt = VT_BSTR;
		variantDir.bstrVal = const_cast<BSTR>(destination_path.c_str());
		hResult = pIShellDispatch->NameSpace(variantDir, &pToFolder);

		if (FAILED(hResult))
		{
			Assert::Fail(L"IShellDispatch: Namespace() failed");
		}

		VariantInit(&variantFile);
		variantFile.vt = VT_BSTR;
		variantFile.bstrVal = const_cast<BSTR>(source_path.c_str());

		if (FAILED(pIShellDispatch->NameSpace(variantFile, &pFromFolder)))
		{
			Assert::Fail(L"IShellDispatch: Namespace() failed");
		}

		if (pFromFolder == nullptr)
		{
			Assert::Fail(L"pFromFolder is null");
		}

		FolderItems *fi = NULL;
		pFromFolder->Items(&fi);

		VariantInit(&variantOpt);
		variantOpt.vt = VT_I4;
		variantOpt.lVal = FOF_NO_UI;

		VARIANT newV;
		VariantInit(&newV);
		newV.vt = VT_DISPATCH;
		newV.pdispVal = fi;
		hResult = pToFolder->CopyHere(newV, variantOpt);

		if (FAILED(hResult))
		{
			Assert::Fail(L"Folder interface: CopyHere() failed");
		}
	}
}

namespace logger_proto_test
{	

	using namespace unit_tests_common;
	using namespace cpplogger;

	TEST_CLASS(base_test_singletone)
	{
	public:
		TEST_METHOD(initialize_logger)
		{
			checked_execute([] 
			{
				init_logger_instance();
				LOG_STACK();
			});
		}

		TEST_METHOD(test_logging_for_types)
		{
			checked_execute([]
			{
				init_logger_instance();
				LOG_STACK();

				LOG_DEBUG() << 'a';
				LOG_DEBUG() << "Test message ASCII";
				LOG_DEBUG() << L'a';
				LOG_DEBUG() << "abc";
				LOG_DEBUG() << L"abc";
				LOG_DEBUG() << 10;
				LOG_DEBUG() << -10;
				LOG_DEBUG() << 10U;
				LOG_DEBUG() << 10L;
				LOG_DEBUG() << -10L;
				LOG_DEBUG() << 10UL;
				LOG_DEBUG() << true;
				LOG_DEBUG() << false;
				LOG_DEBUG() << 10.10;
				LOG_DEBUG() << -10.10;
				LOG_DEBUG() << LLONG_MIN;
				LOG_DEBUG() << LLONG_MAX;
				LOG_DEBUG() << ULLONG_MAX;
				LOG_DEBUG() << L"Привет";
				LOG_DEBUG() << "Привет";

				for (std::size_t i = 0; i < 65535; ++i)
				{
					LOG_DEBUG() << wchar_t(i);
				}

				LOG_INFO() << L"Leave My test function";
				LOG_DEBUG() << L"Привет мир";
				LOG_DEBUG() << "ПРивет мир";
 			});
		}

		TEST_METHOD(test_logging)
		{
			checked_execute([]
			{
				init_logger_instance();
				LOG_STACK();

				LOG_TRACE() << "Trace";
				LOG_DEBUG() << "Debug";
				LOG_INFO() << "Info";
				LOG_ERROR() << "Error";
				LOG_FATAL() << "Fatal";

				logger::severity severities[] = {
					logger::severity::trace,
					logger::severity::debug,
					logger::severity::info,
					logger::severity::warn,
					logger::severity::error,
					logger::severity::fatal
				};

				for (auto sev : severities)
				{
					LOG(sev) << "Test for severity: " << static_cast<int>(sev);
				}
			});
		}

		TEST_METHOD(test_logging_mth)
		{
			checked_execute([]
			{
				init_logger_instance();
				LOG_STACK();

				const unsigned int number_of_threads = 10;
				std::vector<std::thread> threads;
				for (unsigned int n = 0; n < number_of_threads; ++n)
				{
					threads.push_back(std::thread([]
					{
						LOG_STACK();
						init_logger_instance();
						run_logging_in_cycle();
					}));
				}

				for (auto & th : threads)
				{
					th.join();
				}
			});
		}

		TEST_METHOD(test_logging_in_cycle)
		{
			checked_execute([]
			{
				init_logger_instance();
				LOG_STACK();
				run_logging_in_cycle();
			});
		}
	};

	TEST_CLASS(base_test)
	{
	public:
		
		TEST_METHOD(test_check_file_size_and_backup_index)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_check_file_size_and_backup_index_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_check_file_size_and_backup_index_logs");
				prepare_test_directory(logs_dir);

				const std::wstring log_file_prefix(L"serv");
				logger_settings cur_log_settings;

				for (size_t i = 0; i < 3; ++i)
				{
					logger_settings settings = generate_test_logger_settings(rand() % 4 + 2, default_logger_settings_use_dev_log.max_file_size + i * 200000);

					{
						details::Logger log;
						write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
						log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

						cur_log_settings = log.get_current_logger_settings();
						std::uint64_t max_log_files_mem_size = (cur_log_settings.max_backup_index + 1) * cur_log_settings.max_file_size;
						std::uint64_t mem_counter = 0;

						while (mem_counter <= max_log_files_mem_size * 2)
						{
							log.log_message(log_level::trace, test_message);
							mem_counter += test_message_mem_size;
						}
					}

					std::vector<std::wstring> files;
					std::vector<std::int64_t> file_sizes;

					std::wstring match_pattern;
					match_pattern.append(log_file_prefix);
					match_pattern.append(L".log");

					fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring& file_path)
					{
						if (file_path.find(match_pattern) != std::wstring::npos)
						{
							files.push_back(file_path);
							file_sizes.push_back(fs_utils::get_file_size(file_path));
						}
					});

					/*check write/read config operations*/
					Assert::AreEqual(static_cast<std::size_t>(cur_log_settings.level), static_cast<std::size_t>(settings.level));
					Assert::AreEqual(cur_log_settings.max_backup_index, settings.max_backup_index);
					Assert::AreEqual(cur_log_settings.max_buffer_size, settings.max_buffer_size);
					Assert::AreEqual(cur_log_settings.max_file_size, settings.max_file_size);
					Assert::AreEqual(cur_log_settings.use_developer_log, settings.use_developer_log);

					/*check max_backup_index value*/
					Assert::AreEqual(files.size(), cur_log_settings.max_backup_index + 1);

					/*check max_file_size value*/
#pragma warning(suppress: 4018) //signed/unsigned mismatch
					Assert::IsFalse(*std::max_element(file_sizes.begin(), file_sizes.end()) > cur_log_settings.max_file_size + test_message_mem_size * 2);

					/*check first and last lines from file*/
					for (auto &it : files)
					{
						Assert::AreEqual(get_first_file_line(it), test_message);
						Assert::AreEqual(get_last_file_line(it), test_message);
					}

					clean_directory(logs_dir);
				}

				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_control_use_developer_log)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_control_use_developer_log_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_control_use_developer_log_logs");
				prepare_test_directory(logs_dir);

				const std::wstring log_file_prefix(L"serv");

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_no_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					std::uint64_t max_log_file_mem_size = log.get_current_logger_settings().max_file_size;
					std::uint64_t mem_counter = 0;

					if (log.get_current_logger_settings().use_developer_log)
					{
						while (mem_counter <= max_log_file_mem_size)
						{
							log.log_message(log_level::trace, test_message);
							mem_counter += test_message_mem_size;
						}
					}

					std::vector<std::wstring> files;
					std::wstring match_pattern;
					match_pattern.append(log_file_prefix);
					match_pattern.append(L".log");

					fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring& file_path)
					{
						if (file_path.find(match_pattern) != std::wstring::npos)
						{
							files.push_back(file_path);
						}
					});

					Assert::IsTrue(files.empty());
				}

				remove_directory(logs_dir);
				remove_directory(configs_dir);
			}); 
		}

		TEST_METHOD(test_control_log_level)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_control_log_level_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_control_log_level_logs");
				prepare_test_directory(logs_dir);

				const std::wstring dbg_log_file_prefix(L"serv_dbg");
				const std::wstring inf_log_file_prefix(L"serv_inf");
				const std::wstring warn_log_file_prefix(L"serv_warn");
				const std::wstring err_log_file_prefix(L"serv_err");
				const std::wstring fat_log_file_prefix(L"serv_fat");

				/*Don't test TRACE level because it is lowest log level and log may contains all messages*/
				/*Test DEBUG logging*/
				log_level_testing(log_level::debug, configs_dir, config_file_name, logs_dir, dbg_log_file_prefix);
				std::wstring dbg_log_file = get_log_file_full_path(logs_dir, dbg_log_file_prefix);
				Assert::IsFalse(unexpectable_messages_exist(dbg_log_file, unexpectable_debug_messages));

				/*Test INFO logging*/
				log_level_testing(log_level::info, configs_dir, config_file_name, logs_dir, inf_log_file_prefix);
				std::wstring info_log_file = get_log_file_full_path(logs_dir, inf_log_file_prefix);
				Assert::IsFalse(unexpectable_messages_exist(info_log_file, unexpectable_info_messages));

				/*Test WARN logging*/
				log_level_testing(log_level::warn, configs_dir, config_file_name, logs_dir, warn_log_file_prefix);
				std::wstring warn_log_file = get_log_file_full_path(logs_dir, warn_log_file_prefix);
				Assert::IsFalse(unexpectable_messages_exist(warn_log_file, unexpectable_warn_messages));

				/*Test ERROR logging*/
				log_level_testing(log_level::error, configs_dir, config_file_name, logs_dir, err_log_file_prefix);
				std::wstring err_log_file = get_log_file_full_path(logs_dir, err_log_file_prefix);
				Assert::IsFalse(unexpectable_messages_exist(err_log_file, unexpectable_error_messages));

				/*Test FATAL logging*/
				log_level_testing(log_level::fatal, configs_dir, config_file_name, logs_dir, fat_log_file_prefix);
				std::wstring fatal_log_file = get_log_file_full_path(logs_dir, fat_log_file_prefix);
				Assert::IsFalse(unexpectable_messages_exist(fatal_log_file, unexpectable_fatal_messages));
				
				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_control_mth_logging)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_control_mth_logging_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_control_mth_logging_logs");
				prepare_test_directory(logs_dir);

				const std::wstring log_file_prefix(L"serv");

				std::list <std::wstring> in_messages;
				std::vector <std::size_t> in_messages_hash;
				std::list <std::wstring> out_messages;
				std::hash <std::wstring> hash_func;
				const unsigned int messages_count = 10000;

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					for (size_t i = 0; i < messages_count; ++i)
					{
						std::wostringstream ss;
						ss << "This is a test message" << " " << i;
						in_messages.push_back(ss.str());
					}

					const unsigned int number_of_threads = std::thread::hardware_concurrency();
					std::vector<std::thread> threads;
					std::mutex mx;
					std::atomic <bool> enable = true;

					for (unsigned int n = 0; n < number_of_threads; ++n)
					{
						threads.push_back(std::thread([&]
						{
							while (enable)
							{
								std::wstring message;

								{
									std::unique_lock <std::mutex> lock(mx);

									if (!in_messages.empty())
									{
										message = in_messages.front();
										in_messages_hash.push_back(hash_func(message));
										in_messages.pop_front();
									}
									else
									{
										enable = false;
									}
								}

								log.log_message(log_level::trace, message);
							}
						}));
					}

					for (auto & th : threads)
					{
						th.join();
					}
				}

				{
					std::wifstream in(get_log_file_full_path(logs_dir, log_file_prefix));
					in.exceptions(std::ios_base::badbit);
					std::wstring line;

					while (in >> std::ws && std::getline(in,line))
					{
						out_messages.push_back(line);
						
						if (std::find(in_messages_hash.begin(), in_messages_hash.end(), hash_func(line)) == in_messages_hash.end())
						{
							Assert::Fail(L"Can't find line from log file in reference container");
						}
					}
				}

				const unsigned int compare_size = static_cast<unsigned int>(out_messages.size());
				Assert::AreEqual(compare_size,messages_count);

				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_log_file_readonly)
		{
			checked_execute([] 
			{
				const std::wstring configs_dir(L"../test_log_file_readonly_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_log_file_readonly_logs");
				prepare_test_directory(logs_dir);

				logger_settings current_log_settings;
				const std::wstring log_file_prefix(L"serv");

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					current_log_settings = log.get_current_logger_settings();
					std::uint64_t max_log_files_mem_size = (current_log_settings.max_backup_index + 1) * current_log_settings.max_file_size;
					std::uint64_t mem_counter = 0;
					std::size_t max_backup_index = current_log_settings.max_backup_index;

					create_fictive_log_file(logs_dir, log_file_prefix, max_backup_index + 1, 0, FILE_ATTRIBUTE_READONLY);

					if (log.get_current_logger_settings().use_developer_log)
					{
						while (mem_counter <= max_log_files_mem_size)
						{
							log.log_message(log_level::trace, test_message);
							mem_counter += test_message_mem_size;
						}
					}
				}

				std::vector<std::wstring> files;
				std::wstring match_pattern;
				match_pattern.append(log_file_prefix);
				match_pattern.append(L".log");

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring& file_path)
				{
					files.push_back(file_path);
				});

				Assert::AreEqual(files.size(), std::size_t(2));
				
				
				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_log_file_shared_mode)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_log_file_shared_mode_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_log_file_shared_mode_logs");
				prepare_test_directory(logs_dir);

				logger_settings current_log_settings;
				const std::wstring log_file_prefix(L"serv");

				for (auto it : share_modes)
				{
					{
						PROCESS_INFORMATION pi;
						HANDLE hEvent = CreateEventW(NULL, TRUE, FALSE, L"my_logger_test_event");

						details::Logger log;
						logger_settings settings = default_logger_settings_use_dev_log;
						write_logger_settings(path_helpers::concatenate_paths(configs_dir,config_file_name), settings);
						log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

						current_log_settings = log.get_current_logger_settings();
						std::uint64_t max_log_files_mem_size = current_log_settings.max_file_size;
						std::uint64_t mem_counter = 0;
						std::size_t max_backup_index = current_log_settings.max_backup_index;

						/*run test create file util*/
						std::wostringstream ss;
						ss << path_helpers::concatenate_paths(logs_dir, log_file_prefix) << ".log";
						ss << "." << max_backup_index + 1 << " ";
						run_test_process(&pi, L"create_shared_file_util.exe", ss.str() + it);

						if (log.get_current_logger_settings().use_developer_log)
						{
							while (mem_counter <= max_log_files_mem_size * 2)
							{
								log.log_message(log_level::trace, test_message);
								mem_counter += test_message_mem_size;
							}
						}

						Assert::IsTrue(SetEvent(hEvent) == TRUE);
						Assert::IsFalse(WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_OBJECT_0);
						CloseHandle(hEvent);
						CloseHandle(pi.hThread);
						CloseHandle(pi.hProcess);
					}

					std::vector<std::wstring> files;
					std::wstring match_pattern;
					match_pattern.append(log_file_prefix);
					match_pattern.append(L".log");

					fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring& file_path)
					{
						files.push_back(file_path);
					});

					Assert::AreEqual(files.size(), std::size_t(2));

					clean_directory(logs_dir);
				}

				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_log_file_sync_directory)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_log_file_sync_directory_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_log_file_sync_directory_logs");
				prepare_test_directory(logs_dir);

				logger_settings current_log_settings;
				const std::wstring log_file_prefix(L"serv");

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					current_log_settings = log.get_current_logger_settings();
					std::uint64_t max_log_files_mem_size = (current_log_settings.max_backup_index + 1) * current_log_settings.max_file_size;
					std::uint64_t mem_counter = 0;
					std::size_t max_backup_index = current_log_settings.max_backup_index;

					create_fictive_log_file(logs_dir, log_file_prefix, max_backup_index + 1, 0, FILE_ATTRIBUTE_NORMAL);
					create_fictive_log_file(logs_dir, log_file_prefix, max_backup_index + 2, 0, FILE_ATTRIBUTE_NORMAL);

					if (log.get_current_logger_settings().use_developer_log)
					{
						while (mem_counter <= max_log_files_mem_size * 2)
						{
							log.log_message(log_level::trace, test_message);
							mem_counter += test_message_mem_size;
						}
					}
				}

				std::vector<std::wstring> files;
				std::wstring match_pattern;
				match_pattern.append(log_file_prefix);
				match_pattern.append(L".log");

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring& file_path)
				{
					files.push_back(file_path);
				});

				/*Check that logger deletes two fictive files with indexes greater than max_backup_index*/
				Assert::AreEqual(files.size(), current_log_settings.max_backup_index + 1);

				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_log_file_write_new)
		{
			checked_execute([]
			{
				const auto configs_dir(L"../test_log_file_write_new_configs");
				const auto config_file_name(L"mylog.xml");
				const auto logs_dir(L"../test_log_file_write_new_logs");
				const auto log_file_prefix(L"scan_service");
				const auto lines_written = std::size_t(10);
				std::vector <std::wstring> log_files;

				prepare_test_directory(configs_dir);

				prepare_test_directory(logs_dir);

				utils::on_exit remove_dirs_on_exit_guard([&]
				{
					remove_directory(configs_dir);
					remove_directory(logs_dir);
				});

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					if (log.get_current_logger_settings().use_developer_log)
					{
						for (std::size_t i = 0; i < lines_written; ++i)
						{
							log.log_message(log_level::trace, test_message);
						}
					}
				}

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_path)
				{
					log_files.push_back(file_path);
				});

				if (log_files.empty())
				{
					Assert::Fail(L"log_files is empty assertion");
				}

				Assert::AreEqual(lines_written, get_file_lines_count(log_files.at(0)));
			});
		}

		TEST_METHOD(test_log_file_append_data)
		{
			checked_execute([]
			{
				const auto configs_dir(L"../test_log_file_append_data_configs");
				const auto config_file_name(L"mylog.xml");
				const auto logs_dir(L"../test_log_file_append_data_logs");
				const auto log_file_prefix(L"scan_service");
				const auto lines_written = std::size_t(10);
				const auto iteration_count = std::size_t(2);
				std::vector <std::wstring> log_files;

				prepare_test_directory(configs_dir);

				prepare_test_directory(logs_dir);

				utils::on_exit remove_dirs_on_exit_guard([&]
				{
					remove_directory(configs_dir);
					remove_directory(logs_dir);
				});

				for (std::size_t i = 0; i < iteration_count; ++i)
				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					if (log.get_current_logger_settings().use_developer_log)
					{
						for (std::size_t i = 0; i < lines_written; ++i)
						{
							log.log_message(log_level::trace, test_message);
						}
					}
				}

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_path)
				{
					log_files.push_back(file_path);
				});

				if (log_files.empty())
				{
					Assert::Fail(L"log_files is empty assertion");
				}

				Assert::AreEqual(lines_written * iteration_count, get_file_lines_count(log_files.at(0)));
			});
		}

		TEST_METHOD(test_in_log_directory_file_names)
		{
			checked_execute([]
			{
				const auto log_file_prefix(L"scan_service");
				const auto config_file_name(L"mylog.xml");
				const auto configs_dir(L"../test_in_log_directory_file_names_configs");
				const auto logs_dir(L"../test_in_log_directory_file_names_scan_service_logs");
				const auto decompress_dir(L"../test_in_log_directory_file_names_decompress");
				const auto fictive_log_files_count = 5U;
				std::set <std::wstring> files_in_logs_dir{ L"scan_service.log", L"scan_service.info.log", L"info.scan_service.log", 
															  L"scan_service2015-9-14_14-11-15.log"};
				prepare_test_directory(configs_dir);

				prepare_test_directory(logs_dir);

				utils::on_exit remove_dirs_exit_guard([&]
				{
					remove_directory(logs_dir);
					remove_directory(configs_dir);
					remove_directory(decompress_dir);
				});

				/*Create additional files in logs dir*/
				for (const auto &it : files_in_logs_dir)
				{
					const auto file_full_path = path_helpers::concatenate_paths(logs_dir, it);
					handle_holder{ CreateFileW(file_full_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) };
				}

				/*Create fictive history log files*/
				for (std::size_t i = 0; i < fictive_log_files_count; ++i)
				{
					const auto history_log_file_path = create_history_log_file_path(logs_dir, log_file_prefix, i);
					handle_holder{ CreateFileW(history_log_file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) };
				}

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					for (std::size_t i = 0; i < 20000; ++i)
					{
						log.log_message(log_level::debug, test_message);
					}

					log.log_message(log_level::error, test_message);
				}

				/*decompress with poco*/
				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_full_path)
				{
					std::ifstream in(string_utils::convert_wchar_to_utf8(file_full_path), std::ios_base::in | std::ios_base::binary);
					Assert::IsTrue(in.good());

					if (file_full_path.find(log_file_prefix) != std::wstring::npos && string_utils::ends_with(file_full_path, L".zip"))
					{
						Poco::Zip::Decompress d(in, string_utils::convert_wchar_to_utf8(decompress_dir), false, true);
						d.decompressAllFiles();
					}
				});

				fs_helpers::for_all_fs_objects(decompress_dir, [&](const std::wstring &file_full_path)
				{
					if (files_in_logs_dir.find(path_helpers::get_file_name(file_full_path)) != files_in_logs_dir.end())
					{
						Assert::Fail(L"Unexpected files in zip archive");
					}
				});
			}); 
		}
	};

	TEST_CLASS(zipper_test)
	{
		TEST_METHOD(test_zipper_with_poco)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_zipper_with_poco_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");
				const std::wstring zip_file_name(L"test.zip");
				const std::wstring poco_decompress_dir(L"../poco_decompress");

				const std::wstring logs_dir(L"../test_zipper_with_poco_logs");
				prepare_test_directory(logs_dir);

				std::vector <std::wstring> files_to_compress;
				std::vector <std::wstring> decompressed_files;
				const std::wstring log_file_prefix(L"serv");

				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);

					generate_test_level_messages(log);
				}

				{
					std::wofstream empty_file_stream;
					const auto full_path = path_helpers::concatenate_paths(logs_dir, L"empty.log");
					empty_file_stream.open(full_path, std::fstream::binary | std::fstream::out | std::fstream::trunc);
					Assert::IsTrue(empty_file_stream.is_open());
					empty_file_stream.close();
					Assert::IsTrue(fs_utils::get_file_size(full_path) == 0ULL);
				}

				{
					/*compress with zlib*/
					fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_path)
					{
						files_to_compress.push_back(file_path);
					});

					Assert::IsFalse(files_to_compress.empty());

					zip::create_archive(files_to_compress, logs_dir, zip_file_name, 1);
				}

				{
					/*decompress with poco*/
					std::wstring decompress_file_full_path = path_helpers::concatenate_paths(logs_dir, zip_file_name);
					std::ifstream in(string_utils::convert_wchar_to_utf8(decompress_file_full_path), std::ios_base::in | std::ios_base::binary);
					Assert::IsTrue(in.good());
					Poco::Zip::Decompress d(in, string_utils::convert_wchar_to_utf8(poco_decompress_dir), false, true);
					d.decompressAllFiles();
				}

				fs_helpers::for_all_fs_objects(poco_decompress_dir, [&](const std::wstring &file_path)
				{
					decompressed_files.push_back(file_path);
				});

				Assert::IsFalse(decompressed_files.empty());

				Assert::AreEqual(files_to_compress.size(), decompressed_files.size());

				for (std::size_t n = 0; n != files_to_compress.size(); ++n)
				{
					Assert::IsTrue(fs_utils::compare_files(files_to_compress.at(n), decompressed_files.at(n)));
				}

				remove_directory(poco_decompress_dir);
				remove_directory(logs_dir);
				remove_directory(configs_dir);
			});
		}

		TEST_METHOD(test_zipper_empty_archive)
		{
			const std::wstring zip_file_name(L"test.zip");
			const std::wstring logs_dir(L"../test_zipper_empty_archive_logs");
			const std::wstring full_zip_file_path = path_helpers::concatenate_paths(logs_dir, zip_file_name);
			std::vector <std::uint8_t> zip_file_data;

			prepare_test_directory(logs_dir);

			{
				zip::create_archive(std::vector<std::wstring>(), logs_dir, zip_file_name, 1);
			}

			{
				zip_file_data.resize(static_cast<unsigned int>(fs_utils::get_file_size(full_zip_file_path)));
				std::ifstream in(string_utils::convert_wchar_to_utf8(full_zip_file_path), std::ios_base::in | std::ios_base::binary);
				in.exceptions(std::ios_base::badbit | std::ios_base::failbit);
				in.read(reinterpret_cast<char*>(zip_file_data.data()), zip_file_data.size());
			}

			Assert::IsTrue(std::equal(zip_file_data.begin(), zip_file_data.end(), empty_zip_hex_mask.begin()));

			remove_directory(logs_dir);
		}

		TEST_METHOD(test_cleanup_stale_zip)
		{
			checked_execute([]
			{
				const std::wstring configs_dir(L"../test_cleanup_stale_zip_configs");
				prepare_test_directory(configs_dir);

				const std::wstring config_file_name(L"mylog.xml");

				const std::wstring logs_dir(L"../test_cleanup_stale_zip_logs");
				prepare_test_directory(logs_dir);

				const std::wstring log_file_prefix(L"serv");

				utils::on_exit remove_dirs_exit_guard([&]
				{
					remove_directory(configs_dir);
					remove_directory(logs_dir);
				});

				/*Creates some empty zip samples*/
				for (std::size_t i = 0; i < 10; ++i)
				{
					std::wstring zip_file_full_path = path_helpers::concatenate_paths(logs_dir, log_file_prefix) + L"_" + std::to_wstring(i) + L".zip";
					auto zip_file_handle = handle_holder{ CreateFileW(zip_file_full_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, HANDLE()) };
					fs_utils::set_file_pointer(zip_file_handle, default_logger_settings_use_dev_log.max_file_size / 5);
					SetEndOfFile(zip_file_handle.get());
				}
				
				/*Initialize logger. Logger must deletes excess zip archives*/
				logger_settings current_logger_settings;
				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_file_prefix);
					current_logger_settings = log.get_current_logger_settings();
				}

				std::int64_t summary_zip_files_size = 0;

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_full_path)
				{
					summary_zip_files_size += fs_utils::get_file_size(file_full_path);
				});
#pragma warning(suppress: 4018) //signed/unsigned mismatch
				Assert::IsTrue(summary_zip_files_size < current_logger_settings.max_file_size);
			});
		}

		TEST_METHOD(test_zip_pack_for_different_processes)
		{
			checked_execute([]
			{
				const auto log_file_prefix_scan_worker(L"scan_worker");
				const auto log_file_prefix_scan_service(L"scan_service");
				const auto config_file_name(L"mylog.xml");
				const auto configs_dir(L"../test_zip_pack_for_different_processes_configs");
				const auto logs_dir(L"../test_zip_pack_for_different_processes_logs");
				const auto scan_worker_decompress_dir(L"../scan_worker_decompress");
				const auto scan_service_decompress_dir(L"../scan_service_decompress");
				const std::size_t fictive_log_files_count = 5;
				std::vector <std::wstring> log_prefixes{ log_file_prefix_scan_service, log_file_prefix_scan_worker };

				prepare_test_directory(configs_dir);

				prepare_test_directory(logs_dir);

				prepare_test_directory(scan_service_decompress_dir);

				prepare_test_directory(scan_worker_decompress_dir);

				utils::on_exit remove_dirs_exit_guard([&]
				{
					remove_directory(configs_dir);
					remove_directory(logs_dir);
					remove_directory(scan_service_decompress_dir);
					remove_directory(scan_worker_decompress_dir);
				});

				/*create fictive log files for scan_service and scan_worker*/
				for (std::size_t i = 0; i < fictive_log_files_count; ++i)
				{
					const auto scan_service_log_file_path = create_history_log_file_path(logs_dir, log_file_prefix_scan_service, i);
					const auto scan_worker_log_file_path = create_history_log_file_path(logs_dir, log_file_prefix_scan_worker, i);
					auto scan_service_handle_holder = handle_holder{ CreateFileW(scan_service_log_file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) };
					auto scan_worker_handle_holder = handle_holder{ CreateFileW(scan_worker_log_file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) };
				}

				for (const auto &log_prefix : log_prefixes)
				{
					details::Logger log;
					logger_settings settings = default_logger_settings_use_dev_log;
					write_logger_settings(path_helpers::concatenate_paths(configs_dir, config_file_name), settings);
					log.initialize(configs_dir, config_file_name, logs_dir, log_prefix);
				}

				fs_helpers::for_all_fs_objects(logs_dir, [&](const std::wstring &file_full_path)
				{
					std::ifstream in(string_utils::convert_wchar_to_utf8(file_full_path), std::ios_base::in | std::ios_base::binary);
					Assert::IsTrue(in.good());

					if (file_full_path.find(log_file_prefix_scan_service) != std::wstring::npos && string_utils::ends_with(file_full_path, L".zip"))
					{
						Poco::Zip::Decompress d(in, string_utils::convert_wchar_to_utf8(scan_service_decompress_dir), false, true);
						d.decompressAllFiles();
					}
					else if (file_full_path.find(log_file_prefix_scan_worker) != std::wstring::npos && string_utils::ends_with(file_full_path, L".zip"))
					{
						Poco::Zip::Decompress d(in, string_utils::convert_wchar_to_utf8(scan_worker_decompress_dir), false, true);
						d.decompressAllFiles();
					}
				});

				/*check decompress dirs*/
				std::vector <std::wstring> scan_service_files;
				std::vector <std::wstring> scan_worker_files;

				fs_helpers::for_all_fs_objects(scan_service_decompress_dir, [&](const std::wstring &file_full_path)
				{
					if (file_full_path.find(log_file_prefix_scan_worker) != std::wstring::npos)
					{
						Assert::Fail(L"Unexpectable log files by 'scan_worker' in scan_service decompress directory");
					}

					scan_service_files.push_back(file_full_path);
				});
				
				Assert::AreEqual(scan_service_files.size(), fictive_log_files_count);

				fs_helpers::for_all_fs_objects(scan_worker_decompress_dir, [&](const std::wstring &file_full_path)
				{
					if (file_full_path.find(log_file_prefix_scan_service) != std::wstring::npos)
					{
						Assert::Fail(L"Unexpectable log files by 'scan_service' in scan_worker decompress directory");
					}

					scan_worker_files.push_back(file_full_path);
				});

				Assert::AreEqual(scan_worker_files.size(), fictive_log_files_count);

			});
		}
	};
}
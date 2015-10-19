#include <common/string_utils.h>

#include "log_file_writer.h"

namespace logger
{
	log_file_writer_exception::log_file_writer_exception(const wstring_t & message)
		: std::exception(string_utils::convert_wchar_to_utf8(message).c_str())
	{
	}
	
	log_file_writer::log_file_writer()
		: log_file_handle_(INVALID_HANDLE_VALUE)
	{
	}

	log_file_writer::~log_file_writer()
	{
		close();
	}

	void log_file_writer::close()
	{
		decltype(log_file_handle_) log_file_handle = INVALID_HANDLE_VALUE;
		decltype(current_log_file_path_) current_log_file_path;

		std::swap(log_file_handle, log_file_handle_);
		std::swap(current_log_file_path_, current_log_file_path);

		if (log_file_handle != INVALID_HANDLE_VALUE)
		{
			auto const result = ::CloseHandle(log_file_handle);
			if (!result)
			{
				OutputDebugStringW(L"Log file writer error: could not close log file handle.\n");
			}
		}
	}

	bool log_file_writer::is_open() const
	{
		return log_file_handle_ != INVALID_HANDLE_VALUE;
	}

	void log_file_writer::open(const wstring_t& full_file_path, open_disposition permission)
	{
		DWORD log_file_open_disposition = 0;
		DWORD log_file_access = 0;

		switch (permission)
		{
		case open_disposition::file_open_normal_mode:
			log_file_open_disposition = OPEN_ALWAYS;
			log_file_access = GENERIC_READ | FILE_APPEND_DATA;
			break;
		case open_disposition::file_open_clear_mode:
			log_file_open_disposition = TRUNCATE_EXISTING;
			log_file_access = GENERIC_WRITE;
			break;
		default:
			throw std::invalid_argument("Log file writer error: wrong file open permission.");
		}

		auto log_file_handle = CreateFileW(
			full_file_path.c_str(),
			log_file_access,
			FILE_SHARE_READ,
			NULL,
			log_file_open_disposition,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (log_file_handle == INVALID_HANDLE_VALUE)
		{
			throw log_file_writer_exception(L"Could not open log file.");
		}

		current_log_file_path_ = full_file_path;
		std::swap(log_file_handle_, log_file_handle);
	}

	void log_file_writer::write(const wstring_t & message)
	{
		if (message.empty()) return;

		const auto log_file_handle = log_file_handle_;
		if (log_file_handle == INVALID_HANDLE_VALUE)
		{
			OutputDebugStringW((message + L'\n').c_str());
			throw log_file_writer_exception(L"Could not write to a closed log file.");
		}

		auto const result_msg = string_utils::convert_wchar_to_utf8(message);
		DWORD bytes_written;
		if (!WriteFile(log_file_handle_, result_msg.c_str(), static_cast<DWORD>(result_msg.size()), &bytes_written, NULL))
		{
			throw log_file_writer_exception(L"Could not write to the log file.");
		}
	}

	wstring_t log_file_writer::current_log_file_path() const
	{
		return current_log_file_path_;
	}
}
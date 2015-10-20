#pragma once

#include <string>
#include <exception>
#include <Windows.h>

namespace logger
{
	using wstring_t = std::wstring;

	/*log file writer*/
	enum class open_disposition : unsigned int
	{
		file_open_normal_mode = 0,
		file_open_clear_mode = 1
	};

	class log_file_writer_exception final : public std::exception
	{
	public:
		log_file_writer_exception(const wstring_t & message);
	};

	class log_file_writer final
	{

	public:
		log_file_writer();
		~log_file_writer();

		log_file_writer(const log_file_writer &) = delete;
		log_file_writer & operator = (const log_file_writer &) = delete;

		void close();
		void open(const wstring_t& full_file_path, open_disposition permission);
		void write(const wstring_t& message);
		wstring_t current_log_file_path() const;

		bool is_open() const;
	private:
		HANDLE log_file_handle_;
		wstring_t current_log_file_path_;
	};
}
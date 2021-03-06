/////////////////////////////////////
// (C) 2014 ��� "��� ������������"
// ������: SNES-AV eset
// �����: ������ �����
// ������: 22.01.2015
// ������� ��������: ��������� � ���������� ������������ � ����������� ����������
/////////////////////////////////////
#pragma once

#include <loglib/log.h>
#include <common/string_utils.h>
//#include <error_lib/application_exception.h>
#include <windows.h>
#include <type_traits>

namespace logger
{
	class log_and_throw_stream final
	{
	public:
		template<typename exception_t>
		log_and_throw_stream(const exception_t& exception, const char* file_name, int line):
			exception_(std::make_exception_ptr(exception)),
			exception_what_(string_utils::convert_utf8_to_wchar(exception.what())),
			name_(typeid(exception_t).name()),
			file_name_(file_name),
			line_(line)
		{
		}
		
		__declspec(noreturn) ~log_and_throw_stream() /* noexcept(false) */
		{
			try
			{
				flush();
			}
			catch(const std::exception & exc)
			{
				OutputDebugStringA(exc.what());
				std::terminate();
			}
			catch(...)
			{
				OutputDebugStringA("An unknown exception was caught");
				std::terminate();
			}
			std::rethrow_exception(std::move(exception_));
		}

		template <typename T>
		log_and_throw_stream & operator << (const T & ref)
		{
			stream_ << ref;
			return *this;
		}

		log_and_throw_stream(const log_and_throw_stream &) = delete;
		log_and_throw_stream operator = (const log_and_throw_stream &) = delete;
	private:
		void flush()
		{
			bool empty = stream_.str().empty();
			auto add_delimiter = [&empty, this]
			{
				if(!empty)
					stream_ << "; ";
			};
		
			if(!exception_what_.empty())
			{
				add_delimiter();
				stream_ << exception_what_;
				empty = false;
			}

			add_delimiter();
			stream_ << name_ << "; file: " << file_name_ << '(' << line_ << ')';

			log_message(log_severity, L"EXCEPTION " + stream_.str());
		}

		std::exception_ptr exception_;
		std::wstring exception_what_;
		std::wostringstream stream_;
		const char* name_;
		const char* file_name_;
		int line_;

		const severity log_severity = severity::debug;
	};
}

#define LOG_AND_THROW(exception) logger::log_and_throw_stream(exception, __FILE__, __LINE__)

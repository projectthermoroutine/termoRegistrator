#pragma once

#include <loglib/log.h>
#include <common/string_utils.h>
#include <type_traits>

namespace logger
{
	class log_and_throw_stream final
	{
	public:

		template<
			class exception_t,
			std::enable_if_t<std::is_base_of_v<std::exception, std::decay_t<exception_t>>, int> = 0
		>
			log_and_throw_stream(exception_t&& exception, const char* file_name, int line) :
			exception_(nullptr),
			exception_rtti_name_(typeid(exception_t).name()),
			exception_what_(string_utils::convert_utf8_to_wchar(exception.what())),
			file_name_(file_name),
			line_(line)
		{
			if (std::current_exception())
			{
				try
				{
					std::throw_with_nested(std::forward<exception_t>(exception));
				}
				catch (...)
				{
					exception_ = std::current_exception();
				}
			}
			else
			{
				exception_ = std::make_exception_ptr(std::forward<exception_t>(exception));
			}
		}

		template<
			class exception_t,
			std::enable_if_t<!std::is_base_of_v<std::exception, std::decay_t<exception_t>>, int> = 0
		>
			log_and_throw_stream(exception_t&& exception, const char* file_name, int line) :
			exception_(nullptr),
			exception_rtti_name_(typeid(exception_t).name()),
			exception_what_(std::wstring(L"<not-stl-exception:").append(string_utils::convert_utf8_to_wchar(exception_rtti_name_)).append(L'>', 1)),
			file_name_(file_name),
			line_(line)
		{
			if (std::current_exception())
			{
				try
				{
					std::throw_with_nested(std::forward<exception_t>(exception));
				}
				catch (...)
				{
					exception_ = std::current_exception();
				}
			}
			else
			{
				exception_ = std::make_exception_ptr(std::forward<exception_t>(exception));
			}
		}

		[[noreturn]] ~log_and_throw_stream() noexcept(false)
		{
			try
			{
				flush();
			}
			catch (...)
			{
				std::terminate();
			}

			if (exception_) // disable warning C4722: destructor never returns, potential memory leak
				std::rethrow_exception(std::move(exception_));
		}

		template <typename T>
		log_and_throw_stream & operator << (const T & ref)
		{
			stream_ << ref;
			return *this;
		}

		log_and_throw_stream & operator << (const std::string& ref)
		{
			stream_ << string_utils::convert_utf8_to_wchar(ref);
			return *this;
		}

		log_and_throw_stream & operator << (const char* ref)
		{
			if (ref)
				stream_ << string_utils::convert_utf8_to_wchar(std::string_view(ref));
			return *this;
		}

		log_and_throw_stream & operator << (char ref)
		{
			stream_ << string_utils::convert_utf8_to_wchar(std::string_view(&ref, 1));
			return *this;
		}

		log_and_throw_stream & operator << (const wchar_t* ref)
		{
			stream_ << ref;
			return *this;
		}

	public:

		log_and_throw_stream() = delete;
		log_and_throw_stream(log_and_throw_stream&&) = delete;
		log_and_throw_stream(const log_and_throw_stream&) = delete;
		log_and_throw_stream& operator=(log_and_throw_stream&&) = delete;
		log_and_throw_stream& operator=(const log_and_throw_stream&) = delete;

	private:

		void flush()
		{
			bool empty = stream_.str().empty();
			const auto add_delimiter = [&empty, this]
			{
				if (!empty)
					stream_ << "; ";
			};

			if (!exception_what_.empty())
			{
				add_delimiter();
				stream_ << exception_what_;
				empty = false;
			}

			add_delimiter();
			stream_ << exception_rtti_name_ << "; file: " << file_name_ << '(' << line_ << ')';

			log_message(log_level, L"EXCEPTION " + stream_.str());
		}

		std::exception_ptr exception_;
		const char* exception_rtti_name_;
		std::wstring exception_what_;
		std::wostringstream stream_;
		const char* file_name_;
		int line_;

		const level log_level = level::debug;
	};
}

#define LOG_AND_THROW(exception) ::logger::log_and_throw_stream(exception, __FILE__, __LINE__)

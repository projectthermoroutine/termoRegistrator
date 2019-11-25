#pragma once

#ifdef min
#undef min
#endif // #ifdef min

#ifdef max
#undef max
#endif // #ifdef max

#ifndef NOMINMAX
#define NOMINMAX
#endif // #ifndef NOMINMAX

#include "CppUnitTest.h"
#include <Windows.h>
#include <codecvt>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <thread>
#include <atomic>
#include <mutex>

#include <common/string_utils.h>
#include <common/on_exit.h>
#include <common/locale.hpp>
#include <common/std_string_view_extended_operators.hpp>

namespace unit_tests_common
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	template <typename T>
	auto convert_string(T str) -> decltype(common::wstring_convert<wchar_t>().from_bytes(T()))
	{
		return common::wstring_convert<wchar_t>().from_bytes(str);
	}

	template <typename TFunc>
	void checked_execute(TFunc func)
	{
		try
		{
			func();
		}
		catch (const std::exception & err)
		{
			Assert::Fail(convert_string(err.what()).c_str());
		}
		catch (...)
		{
			Assert::Fail(L"Unknown exception.");
		}
	}

	template <typename Tfunc>
	void multiple_creation(Tfunc func, std::size_t num_of_items = 10)
	{
		std::vector<decltype(func())> results;
		for (decltype(num_of_items) n = 0; n < num_of_items; ++n)
		{
			results.push_back(func());
		}
	}

	template <typename T, typename TOutIter>
	void generate_test_numbers_range(T min, T max, TOutIter out)
	{
		*(out++) = min;
		auto value = min;
		for (unsigned int n = 0; (n != (sizeof(T) * 8)) && (value < max); value *= 2, ++n)
		{
			if (value == 0) ++value;
			*(out++) = value;
			if (value > 2 && value != max)
			{
				*(out++) = value + 1;
			}
			if (value > 2 && value != min)
			{
				*(out++) = value - 1;
			}
		}
		*(out++) = max;
	}

	template<typename TOutInter>
	void fill_buffer_with_random(unsigned int buffer_size, TOutInter output)
	{
		std::generate_n(
			output,
			buffer_size,
			[]() { return static_cast<BYTE>(rand()); });
	}

	template <typename value_t>
	typename std::enable_if<(!std::is_integral<value_t>::value), value_t >::type
	get_random()
	{
		return value_t();
	}

	template <typename value_t>
	typename std::enable_if<(!(std::is_same<value_t, bool>::value) && std::is_integral<value_t>::value), value_t >::type
	get_random()
	{
		return static_cast<value_t>(rand());
	}

	template <typename value_t>
	typename std::enable_if<(std::is_same<value_t, bool>::value && std::is_integral<value_t>::value), value_t>::type
	get_random()
	{
		return (rand() % 2) == 0;
	}

	template <typename value_t, typename iter_t>
	void generate_random_values(iter_t iter, std::size_t num_of_elements)
	{
		std::generate_n(
			iter,
			num_of_elements,
			[]() { return get_random<value_t>(); });
	}

	inline std::wstring get_system_directory()
	{
		WCHAR dir_name[MAX_PATH];
		auto result = GetSystemDirectoryW(dir_name, MAX_PATH);
		if (result == 0)
		{
			Assert::Fail(L"Failed to get system directory.");
		}
		return std::wstring(dir_name);
	}

	inline void run_test_process(PPROCESS_INFORMATION processInfo, const std::wstring & params)
	{
		Assert::IsNotNull(processInfo, L"Invalid parameter.");

		const std::wstring command(get_system_directory() + L"\\cmd.exe");

		const unsigned int params_buffer_size = 32768;
		const unsigned int max_chars_num = params_buffer_size - 1;

		const std::wstring full_params = L"/c " + params;
		wchar_t params_buffer[params_buffer_size];
		std::fill_n(params_buffer, params_buffer_size, 0);
		std::copy_n(full_params.begin(), std::min(full_params.length(), max_chars_num), params_buffer);

		STARTUPINFOW startup_info{};
		if (!CreateProcessW(
			command.c_str(),
			params_buffer,
			0,
			0,
			FALSE,
			0,
			0,
			0,
			&startup_info,
			processInfo))
		{
			auto last_error = GetLastError();
			std::wstringstream sstream;
			sstream << L"Error " << last_error << L" during command execution: " << command << L" " << params_buffer;
			Assert::Fail(sstream.str().c_str());
		}
	}

	inline void run_command(const std::wstring & params)
	{
		const std::wstring command(get_system_directory() + L"\\cmd.exe");

		const unsigned int params_buffer_size = 32768;
		const unsigned int max_chars_num = params_buffer_size - 1;

		const std::wstring full_params = L"/c " + params;
		wchar_t params_buffer[params_buffer_size];
		std::fill_n(params_buffer, params_buffer_size, 0);
		std::copy_n(full_params.begin(), std::min(full_params.length(), max_chars_num), params_buffer);

		STARTUPINFOW startup_info{};
		PROCESS_INFORMATION pi;
		if (!CreateProcessW(
			command.c_str(),
			params_buffer,
			0,
			0,
			FALSE,
			0,
			0,
			0,
			&startup_info,
			&pi))
		{
			auto last_error = GetLastError();
			std::wstringstream sstream;
			sstream << L"Error " << last_error << L" during command execution: " << command << L" " << params_buffer;
			Assert::Fail(sstream.str().c_str());
		}
		else
		{
			auto wait_result = WaitForSingleObject(pi.hProcess, INFINITE);

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			Assert::IsTrue(wait_result == WAIT_OBJECT_0, L"Waiting for external command failed.");
		}
	}

	inline void run_cmd(PPROCESS_INFORMATION processInfo)
	{
		Assert::IsNotNull(processInfo, L"Invalid parameter.");

		const std::wstring command(get_system_directory() + L"\\cmd.exe");

		STARTUPINFOW startup_info{};
		if (!CreateProcessW(
			command.c_str(),
			0,
			0,
			0,
			FALSE,
			0,
			0,
			0,
			&startup_info,
			processInfo))
		{
			auto last_error = GetLastError();
			std::wstringstream sstream;
			sstream << L"Error " << last_error << L" during command execution: " << command;
			Assert::Fail(sstream.str().c_str());
		}
	}

	inline void join_all_threads(std::vector<std::thread> & threads)
	{
		for (auto & thread : threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	inline unsigned int get_number_of_threads_for_test()
	{
		auto th_num = std::max(1u, std::thread::hardware_concurrency());

		return th_num * 2;
	}

	inline void mth_run_n(unsigned int num, std::function<void()> func)
	{
		std::atomic<bool> fail;
		std::exception_ptr exception_ptr;
		{
			std::vector<std::thread> threads;
			utils::on_exit exit_guard([&threads] { join_all_threads(threads); });

			for (unsigned int n = 0; n != num; ++n)
			{
				threads.emplace_back(
					[&fail, &exception_ptr, func]
				{
					try
					{
						func();
					}
					catch (...)
					{
						bool expected = false;
						if (fail.compare_exchange_strong(expected, true))
						{
							exception_ptr = std::current_exception();
						}
					}
				}
				);
			}
		}

		if (exception_ptr) {
			std::rethrow_exception(exception_ptr);
		}
	}
}

#include <unhandled_exception.h>
#include <date_helpers.h>
#include <handle_holder.h>
#include <path_helpers.h>
#include <sync_helpers.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <DbgHelp.h>

#include <sstream>
#include <csignal>

namespace unhandled_exception_handler
{
	namespace details
	{
		std::wstring _path_for_dumps;
		dump_message_func_t _dump_message_func;

		void dump_modules(std::wostream & out, HANDLE snapshot)
		{
			MODULEENTRY32 me32{};
			me32.dwSize = sizeof(me32);

			if (!Module32First(snapshot, &me32))
			{
				out << "Module32First failed." << std::endl;
				return;
			}

			do
			{
				out << "Module name: " << me32.szModule << std::endl;
				out << "Executable: " << me32.szExePath << std::endl;
				out << std::hex;
				out << "Base address: " << static_cast<void*>(me32.modBaseAddr) << std::endl;
				out << std::dec;
				out << "Base size: " << me32.modBaseSize << std::endl;
			} while (Module32Next(snapshot, &me32));
		}

		void dump_process_memory_info(std::wostream & out)
		{
			PROCESS_MEMORY_COUNTERS mem_cnt{};
			mem_cnt.cb = sizeof(mem_cnt);

			if (!GetProcessMemoryInfo(
				GetCurrentProcess(),
				&mem_cnt,
				mem_cnt.cb))
			{
				out << "GetProcessMemoryInfo failed." << std::endl;
				return;
			}

			out << std::dec;
			//  V128 The 'mem_cnt.PeakWorkingSetSize' variable of the memsize type is written to a stream. Consider verifying the compatibility of 32 and 64 bit versions of the application in the context of a stored data. unhandled_exception.cpp 63
			out << "PeakWorkingSetSize: " << mem_cnt.PeakWorkingSetSize << std::endl; //-V128
			out << "WorkingSetSize: " << mem_cnt.WorkingSetSize << std::endl; //-V128
			out << "PeakPagefileUsage: " << mem_cnt.PeakPagefileUsage << std::endl; //-V128
			out << "PagefileUsage: " << mem_cnt.PagefileUsage << std::endl; //-V128
			out << "QuotaPeakPagedPoolUsage: " << mem_cnt.QuotaPeakPagedPoolUsage << std::endl; //-V128
			out << "QuotaPagedPoolUsage: " << mem_cnt.QuotaPagedPoolUsage << std::endl; //-V128
			out << "QuotaPeakNonPagedPoolUsage: " << mem_cnt.QuotaPeakNonPagedPoolUsage << std::endl; //-V128
			out << "QuotaNonPagedPoolUsage: " << mem_cnt.QuotaNonPagedPoolUsage << std::endl; //-V128
		}

		void dump_exception(std::wostream & out, PEXCEPTION_RECORD record)
		{
			out << std::hex;
			out << "ExceptionCode: " << record->ExceptionCode << std::endl;
			out << "ExceptionAddress: " << record->ExceptionAddress << std::endl;
			out << "ExceptionFlags: " << record->ExceptionFlags << std::endl;

			if (record->ExceptionRecord != 0)
			{
				dump_exception(out, record->ExceptionRecord);
			}
		}

		std::wstring create_mini_dump(std::wostream & out, LPEXCEPTION_POINTERS exceptionInfo)
		{
			auto const pid = GetCurrentProcessId();

			std::wstring dump_file_name;

			{
				time_t current_time;
				time(&current_time);

				std::wostringstream ss;
				ss << date_helpers::local_time_to_str(current_time, "%Y-%m-%d_%H-%M-%S").c_str() << '_' << pid;
				ss << ".dmp";
				dump_file_name = ss.str();
			}

			const auto full_dump_path = path_helpers::concatenate_paths(_path_for_dumps, dump_file_name);
			const handle_holder file(CreateFileW(
				full_dump_path.c_str(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				0,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				0));

			if (file.get() == INVALID_HANDLE_VALUE)
			{
				auto const last_error = GetLastError();
				out << "Could not create minidump file. Error: " << last_error << std::endl;
				return std::wstring();
			}

			MINIDUMP_EXCEPTION_INFORMATION mdei{};
			const MINIDUMP_TYPE minidump_type = MINIDUMP_TYPE(DWORD(MiniDumpWithFullMemory));//| */DWORD(MiniDumpWithHandleData) | DWORD(MiniDumpWithUnloadedModules));

			mdei.ThreadId = GetCurrentThreadId();
			mdei.ExceptionPointers = exceptionInfo;
			mdei.ClientPointers = FALSE;

			auto const dump_result = MiniDumpWriteDump(
				GetCurrentProcess(),
				pid,
				file.get(),
				minidump_type,
				(exceptionInfo != 0) ? &mdei : 0,
				0,
				0);

			if (!dump_result)
			{
				auto const last_error = GetLastError();
				out << "Could not create minidump. Error: " << last_error << std::endl;
			}

			return dump_file_name;
		}

		LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS exceptionInfo)
		{
			try
			{
				std::wostringstream out_stream;

				std::wstring dump_file_name;

				if (exceptionInfo != 0)
				{
					dump_exception(out_stream, exceptionInfo->ExceptionRecord);
					dump_file_name = create_mini_dump(out_stream, exceptionInfo);
				}

				handle_holder const snapshot(
					CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId()));

				if (snapshot.get() != INVALID_HANDLE_VALUE)
				{
					dump_modules(out_stream, snapshot.get());
				}

				dump_process_memory_info(out_stream);

				if (!dump_file_name.empty())
				{
					out_stream << "Minidump was created: " << dump_file_name.c_str() << std::endl;
				}

				_dump_message_func(out_stream.str());
			}
			catch (const std::exception & exc)
			{
				std::wostringstream out_stream;
				out_stream << "Error occurred in CustomUnhandledExceptionFilter: " << exc.what() << std::endl;

				_dump_message_func(out_stream.str());
			}
			catch (...)
			{
				_dump_message_func(L"Unknown error occurred in CustomUnhandledExceptionFilter.");
			}

			return EXCEPTION_EXECUTE_HANDLER;
		}

		void signal_handler(int signal)
		{
			if (signal != SIGABRT) return;
			_dump_message_func(L"SIGABRT was caught.");
		}

		sync_helpers::once_flag init_once;
		void do_init(const std::wstring & path_for_dumps, dump_message_func_t dump_message_func)
		{
			_path_for_dumps = path_for_dumps;
			_dump_message_func = dump_message_func;

			SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
			signal(SIGABRT, signal_handler);
		}
	}

	void initialize(const std::wstring & path_for_dumps, dump_message_func_t dump_message_func)
	{
		using namespace details;
		sync_helpers::call_once(init_once, do_init, path_for_dumps, dump_message_func);
	}
}
#pragma once

#include <functional>

#ifndef DISABLE_ON_EXIT_LOGS
#include <loglib/log.h>
#include <exception>
#endif // DISABLE_ON_EXIT_LOGS

namespace utils
{
	class on_exit final
	{
	public:
		using func_t = std::function<void(void)>;
		on_exit(func_t func) : _func(func) {}

		~on_exit()
		{
			try
			{
				_func();
			}
#ifdef DISABLE_ON_EXIT_LOGS
			catch (const std::exception&)
			{
				std::abort();
#else
			catch (const std::exception& e)
			{
				LOG_FATAL() << "Fatal error in on_exit handler: " << e.what();
				std::terminate();
#endif // DISABLE_ON_EXIT_LOGS
			}
			catch (...)
			{
#ifdef DISABLE_ON_EXIT_LOGS
				std::abort();
#else
				LOG_FATAL() << "Unknown error in on_exit handler. Terminating...";
				std::terminate();
#endif // DISABLE_ON_EXIT_LOGS
			}
			
		}

		on_exit(const on_exit &) = delete;
		on_exit & operator = (const on_exit &) = delete;
	private:
		func_t _func;
	};
}
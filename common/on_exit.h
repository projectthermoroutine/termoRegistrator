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
		on_exit(func_t&& func) : _func(std::move(func)) {}
        on_exit(const func_t& func): _func(func) {}

		~on_exit()
		{
            apply_now();
		}

		void cancel()
		{
            _func = func_t();
		}

        bool apply_now()
        {
            try
            {
                if (func_t func = std::move(_func))
                {
                    /* tmp test */ if (func_t tmp_test_func = std::move(_func)) std::terminate();

                    func();
                    return true;
                }

                return false;
            }
#ifdef DISABLE_ON_EXIT_LOGS
            catch (...)
            {
                std::abort();
            }
#else
            catch (const std::exception& e)
            {
                LOG_FATAL() << "Fatal error in on_exit handler: " << e.what();
                std::terminate();
            }
            catch (...)
            {
                LOG_FATAL() << "Unknown error in on_exit handler. Terminating...";
                std::terminate();
            }
#endif // DISABLE_ON_EXIT_LOGS
        }

		on_exit(const on_exit &) = delete;
		on_exit & operator = (const on_exit &) = delete;
	private:
		func_t _func;
	};
}


#define __ON_EXIT_OF_SCOPE_JOIN2(param1, param2) param1 ## param2
#define __ON_EXIT_OF_SCOPE_JOIN(param1, param2) __ON_EXIT_OF_SCOPE_JOIN2(param1, param2)
#define ON_EXIT_OF_SCOPE(func) const ::utils::on_exit __ON_EXIT_OF_SCOPE_JOIN(guard_exit_of_scope_on_line_, __LINE__)(func)

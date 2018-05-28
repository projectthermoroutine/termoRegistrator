/////////////////////////////////////
// (C) 2015 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 22.09.2015
// Краткое описание: реализация таймера
/////////////////////////////////////


#include <thread>

#include <common/periodic_timer.h>
#include <common/log_and_throw.h>
#include <common/sync_helpers.h>
#include <common/handle_holder.h>

#include <error_lib/error_codes.h>
#include <error_lib/application_exception.h>


namespace management
{
	const std::size_t infinitely_caller(0);

	struct periodic_timer::data_t
	{
		handle_holder wake_up_event;
		std::chrono::milliseconds time_between_calls;
		std::function<void()> functor;
		std::thread worker;
	};

	periodic_timer::periodic_timer() noexcept
		: m_data(nullptr)
	{}

	periodic_timer::periodic_timer(const std::chrono::milliseconds& time_between_calls, std::function<void()>&& functor)
		: periodic_timer(infinitely_caller, time_between_calls, std::move(functor))
	{}

	periodic_timer::periodic_timer(std::size_t caller_count, const std::chrono::milliseconds& time_between_calls, std::function<void()>&& functor)
		: m_data(nullptr)
	{
		LOG_STACK();

		if (!functor)
			LOG_AND_THROW(::common::application_exception(::common::result_code::empty_function_was_passed_to_periodic_timer));

		if (time_between_calls <= time_between_calls.zero())
			LOG_AND_THROW(::common::application_exception(::common::result_code::invalid_argument_period_was_passed_to_periodic_timer));

		m_data = std::make_shared<data_t>();
		m_data->wake_up_event = sync_helpers::create_basic_manual_reset_event();
		m_data->time_between_calls = time_between_calls;
		m_data->functor = std::move(functor);
		m_data->worker = std::thread(&periodic_timer::thread_func, std::ref(*m_data), caller_count);
	}

	periodic_timer::periodic_timer(periodic_timer&& other) noexcept
		: m_data(std::move(other.m_data))
	{ }
	
	periodic_timer& periodic_timer::operator=(periodic_timer&& other)
	{
		periodic_timer(std::move(other)).swap(*this);
		return *this;
	}

	periodic_timer::operator bool() const noexcept
	{
		return static_cast<bool>(m_data) && m_data->worker.joinable();
	}

	void periodic_timer::swap(periodic_timer& other) noexcept
	{
		m_data.swap(other.m_data);
	}

	void periodic_timer::shutdown()
	{
		LOG_STACK();

		if (this->operator bool())
		{
			sync_helpers::set_event(m_data->wake_up_event);
			m_data->worker.join();
		}
	}

	std::chrono::milliseconds periodic_timer::time_between_calls() const
	{
		return (this->operator bool())
			? m_data->time_between_calls
			: std::chrono::milliseconds::zero();
	}

	periodic_timer::~periodic_timer()
	{
		LOG_STACK();

		shutdown();
	}

	void periodic_timer::thread_func(data_t& data, std::size_t caller_count)
	{
		LOG_STACK();

		do 
		{
			try
			{
				if (sync_helpers::wait(data.wake_up_event, static_cast<DWORD>(data.time_between_calls.count())))
					break;
			}
			catch (const std::system_error& e)
			{
				LOG_ERROR() << "An '" << e.code().category().name() << "' exception was caught inside periodic timer loop: " << e.what();
				break;
			}
			catch (const std::exception& e)
			{
				LOG_ERROR() << "An STL exception was caught inside periodic timer loop: " << e.what();
				break;
			}
			catch (...)
			{
				LOG_ERROR() << "An unknown exception was caught inside periodic timer loop.";
				break;
			}

			try
			{
				data.functor();
			}
			catch (const std::system_error& e)
			{
				LOG_WARN() << "An '" << e.code().category().name() << "' exception was caught inside the function timer handler: " << e.what();
			}
			catch (const std::exception& e)
			{
				LOG_WARN() << "An STL exception was caught inside the function timer handler: " << e.what();
			}
			catch (...)
			{
				LOG_WARN() << "An unknown exception was caught inside the function timer handler";
			}
		}
		while (caller_count == infinitely_caller || (--caller_count) > 0);
	}
}

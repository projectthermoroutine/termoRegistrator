/////////////////////////////////////
// (C) 2015 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 22.09.2015
// Краткое описание: интерфейс таймера
/////////////////////////////////////

#pragma once

#include <chrono>
#include <memory>
#include <functional>


namespace management
{
	extern const std::size_t infinitely_caller;

	class periodic_timer final
	{
	public:

		periodic_timer() noexcept;
		periodic_timer(const std::chrono::milliseconds& time_between_calls, std::function<void()>&& functor);
		periodic_timer(std::size_t caller_count, const std::chrono::milliseconds& time_between_calls, std::function<void()>&& functor);

		periodic_timer(periodic_timer&& other) noexcept;
		periodic_timer& operator=(periodic_timer&& other);

		~periodic_timer();

		periodic_timer(const periodic_timer&) = delete;
		periodic_timer& operator=(const periodic_timer&) = delete;

		operator bool() const noexcept;

	public:

		void swap(periodic_timer& other) noexcept;

		void shutdown();

		std::chrono::milliseconds time_between_calls() const;

	private:

		struct data_t;

		static void thread_func(data_t& data, std::size_t caller_count);

	private:

		std::shared_ptr<data_t> m_data;
	};

}

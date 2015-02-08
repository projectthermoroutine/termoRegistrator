#pragma once

#include <functional>
#include <chrono>

namespace thread_pool
{
	class thread_pool final
	{
	public:
		using work_item_t = std::function<void()>;

		explicit thread_pool(
			unsigned int max_num_of_threads,
			const std::chrono::duration<uint64_t> thread_keep_alive = std::chrono::minutes(1));

		~thread_pool();

		void queue_work_item(work_item_t work_item);
		void wait_for_completion();

		thread_pool(const thread_pool &) = delete;
		thread_pool & operator = (const thread_pool &) = delete;
	private:
		void  * _p_impl;
	};
}
#pragma once

#include <functional>
#include <memory>
#include <future>
#include <mutex>

#include <common\thread_exception.h>
#include "position_detector_connector.h"

namespace position_detector
{

	using message_processing_func_t = std::function<void(const BYTE *  message, unsigned int message_size)>;
	using create_process_packet_func_t = std::function<message_processing_func_t()>;

	struct process_packets_factory
	{
		create_process_packet_func_t create_process_event_packet_func;
		create_process_packet_func_t create_process_synchro_packet_func;
	};


	using connector_ptr_t = std::shared_ptr<position_detector_connector>;

	using create_connector_func_t = std::function<connector_ptr_t(const std::vector<std::string>&)>;

	class position_detector_dispatcher
	{
	public:
		using settings_func_t = std::function<std::vector<std::string>(const std::string &)>;
		using active_state_callback_func_t = std::function<void(bool)>;

		position_detector_dispatcher(process_packets_factory, create_connector_func_t, active_state_callback_func_t);

		~position_detector_dispatcher();

		position_detector_dispatcher(const position_detector_dispatcher &) = delete;
		position_detector_dispatcher & operator = (const position_detector_dispatcher &) = delete;

		void run_processing_loop(settings_func_t settings_func, const exception_queue_ptr_t& exc_queue);
		void stop_processing_loop();
	private:
		void * _p_impl;
	};
} // namespace position_detector

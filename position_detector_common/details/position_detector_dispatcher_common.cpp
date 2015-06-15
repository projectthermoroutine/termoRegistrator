#define NOMINMAX

#include <Windows.h>

#include "position_detector_dispatcher_common.h"

#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <array>

#include <common\handle_holder.h>
#include <loglib\log.h>
#include <common\sync_helpers.h>
#include <common\thread_pool.h>

#include "position_detector_connector.h"
#include "position_detector_packet.h"

#define CATCH_ALL_TERMINATE \
catch (const std::exception & exc) \
{ \
	LOG_FATAL() << "Error: " << exc.what(); \
	terminate(); \
} \
catch (...) \
{ \
	LOG_FATAL() << "Unknown exception."; \
	terminate(); \
}

namespace position_detector
{
	namespace details
	{
		using namespace events;
		using namespace synchronization;

		class position_detector_dispatcher_impl final
		{
		public:
			position_detector_dispatcher_impl(process_packets_factory process_packets_factory, create_connector_func_t create_connector_func, position_detector_dispatcher::active_state_callback_func_t);

			position_detector_dispatcher_impl(const position_detector_dispatcher_impl &) = delete;
			position_detector_dispatcher_impl & operator = (const position_detector_dispatcher_impl &) = delete;

			void run_processing_loop(position_detector_dispatcher::settings_func_t settings_func, const exception_queue_ptr_t& exc_queue);
			void stop_processing_loop();

			bool connection_active() const;
		private:
			void run_message_processing(connector_ptr_t & connector, message_processing_func_t message_processing_func);

			void processing_loop(connector_ptr_t connector, std::vector<std::string> settings, message_processing_func_t message_processing_func);

			bool sleep_for(const DWORD timeout) const;
			bool wait_for(const handle_holder & event) const;

			bool stop_requested() const;

			bool create_connector(connector_ptr_t & connector, std::vector<std::string> settings);
			void close_connector(connector_ptr_t & connector);

			template<typename TMessage>
			void dispatch_message(const BYTE *  message, unsigned int message_size);
			void dispatch_synchro_packet(const BYTE *  message, unsigned int message_size);
			void dispatch_event_packet(const BYTE *  message, unsigned int message_size);

			connector_ptr_t & get_connector_events_src();
			connector_ptr_t & get_connector_synchro_packets();

			void set_connection_active(bool active);

			handle_holder _stop_event;
			std::atomic<bool> _stop_requested;
			std::array<std::thread, 2> _processing_loop_threads;

			std::atomic<bool> _connection_closing_requested;
			create_connector_func_t _create_connector_func;
			connector_ptr_t _connector_events_src;
			connector_ptr_t _connector_synchro_packets;
			sync_helpers::rw_lock _connector_events_src_lock;
			sync_helpers::rw_lock _connector_synchro_packets_lock;

			message_processing_func_t _process_event_packet_func;
			message_processing_func_t _process_synchro_packet_func;

			std::atomic<bool> _connection_active;
			position_detector_dispatcher::active_state_callback_func_t _active_state_callback_func;


			std::map<timestamp_t, event_packet_ptr_t> _event_packets_container;
			std::map<timestamp_t, sync_packet_ptr_t> _synchro_packets_container;

			exception_queue_ptr_t _exc_queue;
		};
		position_detector_dispatcher_impl::position_detector_dispatcher_impl(process_packets_factory process_packets_factory, create_connector_func_t create_connector_func, position_detector_dispatcher::active_state_callback_func_t active_state_callback_func) :
			_connection_active(false),
			_connection_closing_requested(false),
			_create_connector_func(create_connector_func),
			_active_state_callback_func(active_state_callback_func),
			_stop_event(sync_helpers::create_basic_event_object(true)),
			_stop_requested(false)
		{
			LOG_STACK();
			
			_process_event_packet_func = process_packets_factory.create_process_event_packet_func();
			_process_synchro_packet_func = process_packets_factory.create_process_synchro_packet_func();
		}

		bool position_detector_dispatcher_impl::connection_active() const
		{
			return _connection_active;
		}

		void position_detector_dispatcher_impl::run_processing_loop(position_detector_dispatcher::settings_func_t settings_func, const exception_queue_ptr_t& exc_queue)
		{
			LOG_STACK();
			for (auto & thread : _processing_loop_threads)
			if (thread.joinable())
			{
				LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
				throw std::logic_error("The processing loop must not be running at this point.");
			}

			_exc_queue = exc_queue;
			
			LOG_TRACE() << "Creating processing loop threads.";
			LOG_TRACE() << "Creating processing loop thread for synchronization packets.";

			message_processing_func_t message_processing_func(_process_synchro_packet_func);
			auto connector_settings = settings_func("Syncronizer device");
			connector_ptr_t connector = _connector_synchro_packets;
			std::thread processing_loop_thread1(
				[this, connector, connector_settings, message_processing_func]()
			{ this->processing_loop(connector, connector_settings, message_processing_func); }
			);

			LOG_TRACE() << "Thread was created.";

			LOG_TRACE() << "Creating processing loop thread for events packets.";
			message_processing_func = _process_event_packet_func;
			
			connector_settings = settings_func("Syncronizer events device"); 
			connector = _connector_events_src;
			std::thread processing_loop_thread2(
				[this, connector, connector_settings, message_processing_func]()
			{ this->processing_loop(connector, connector_settings, message_processing_func); }
			);

			LOG_TRACE() << "Thread was created.";
			_processing_loop_threads[0].swap(processing_loop_thread1);
			_processing_loop_threads[1].swap(processing_loop_thread2);

		}

		void position_detector_dispatcher_impl::stop_processing_loop()
		{
			LOG_STACK();

			LOG_TRACE() << "Requesting stopping.";
			_stop_requested = true;
			sync_helpers::set_event(_stop_event);
			LOG_TRACE() << "Stop flag was set.";
			for (auto & thread : _processing_loop_threads)
			if (thread.joinable())
			{
				LOG_TRACE() << "Waiting for completion of processing loop thread.";
				thread.join();
				LOG_TRACE() << "Processing loop thread finished.";
			}

		}

		bool position_detector_dispatcher_impl::create_connector(connector_ptr_t & connector, std::vector<std::string> settings)
		{
			LOG_STACK();

			connector_ptr_t tmp_connector;
			LOG_TRACE() << "Trying to connect to position detector";
			try{
				tmp_connector = _create_connector_func(settings);
			}
			catch (const position_detector_connector_exception&)
			{
				_exc_queue->raise_exception();
				return false;
			}
			catch (...)
			{
				_exc_queue->raise_exception();
				return false;
			}
			LOG_TRACE() << "Connection established.";
			{
				connector.swap(tmp_connector);
			}
			return true;
		}

		void position_detector_dispatcher_impl::close_connector(connector_ptr_t & connector)
		{
			LOG_STACK();
			connector->close();
		}
		void position_detector_dispatcher_impl::processing_loop(connector_ptr_t connector, std::vector<std::string> settings, message_processing_func_t message_processing_func)
		{
			LOG_STACK();

			try
			{
				LOG_TRACE() << "Running processing loop.";

				_active_state_callback_func(false);

				while (!stop_requested())
				{
					if (create_connector(connector, settings))
					{
						{
							utils::on_exit exit_guard([this] { set_connection_active(false); });
							set_connection_active(true);
							run_message_processing(connector, message_processing_func);
						}
						close_connector(connector);
					}
					else
					{
						_stop_requested = true;
						sync_helpers::set_event(_stop_event);
					}
				}

				LOG_TRACE() << "Stop was requested.";
			}
			catch (const position_detector_connector_exception&)
			{
				_exc_queue->raise_exception();
			}
			catch (...)
			{
				_stop_requested = true;
				sync_helpers::set_event(_stop_event);
				_exc_queue->raise_exception();

			}
			//CATCH_ALL_TERMINATE
		}

		bool position_detector_dispatcher_impl::wait_for(const handle_holder & event) const
		{
			LOG_STACK();

			std::array<HANDLE, 2> handles{ { event.get(), _stop_event.get() } };
			const auto wait_result = sync_helpers::wait_any(handles.data(), handles.size());
			if (!wait_result.event_raised)
			{
				return false;
			}

			if (wait_result.event_index == 0)
			{
				return true;
			}

			return false;
		}

		bool position_detector_dispatcher_impl::stop_requested() const
		{
			LOG_STACK();

			return _stop_requested.load();
		}

		void position_detector_dispatcher_impl::run_message_processing(connector_ptr_t & connector, message_processing_func_t message_processing_func)
		{
			LOG_STACK();

			LOG_TRACE() << "Running message processing loop.";

			_connection_closing_requested = false;

			auto stop_requested_func = std::bind(&position_detector_dispatcher_impl::stop_requested, this);

			while (!stop_requested())
			{
				LOG_TRACE() << "Waiting for a message";

				try
				{
					connector->process_incoming_message(stop_requested_func, _stop_event.get(), message_processing_func);
				}
				catch (const position_detector_connector_exception & exc)
				{
					LOG_WARN() << exc.what();
					if (exc.get_error_code() == E_HANDLE)
					{
						LOG_DEBUG() << "Stopping processing due invalid handle of communication channel.";
						_connection_closing_requested = true;

						_exc_queue->raise_exception();
					}
				}

				if (_connection_closing_requested)
				{
					LOG_TRACE() << "Connection closing was requested.";
					return;
				}
			}

			LOG_TRACE() << "Stop was requested.";
		}



		inline bool is_message_well_size(unsigned int message_size)
		{
			return message_size >= min_synchro_paket_size;
		}

		void position_detector_dispatcher_impl::set_connection_active(bool active)
		{
			bool expected = !active;
			if (_connection_active.compare_exchange_strong(expected, active))
			{
				_active_state_callback_func(active);
			}
		}

		inline position_detector_dispatcher_impl* cast_to_position_detector_dispatcher_impl(void * p_impl)
		{
			if (p_impl == nullptr)
				throw std::invalid_argument("The passed argument p_impl can't be equal to nullptr");

			return static_cast<position_detector_dispatcher_impl*>(p_impl);
		}

	} // namespace details


position_detector_dispatcher::position_detector_dispatcher(process_packets_factory _process_packets_factory, create_connector_func_t create_connector_func, position_detector_dispatcher::active_state_callback_func_t active_state_callback_func) :
_p_impl(new details::position_detector_dispatcher_impl(_process_packets_factory, create_connector_func, active_state_callback_func))
{	
}

position_detector_dispatcher::~position_detector_dispatcher()
{
	auto const p_impl = _p_impl;
	_p_impl = 0;

	delete details::cast_to_position_detector_dispatcher_impl(p_impl);
}


void position_detector_dispatcher::run_processing_loop(settings_func_t settings_func, const exception_queue_ptr_t& exc_queue)
{
	details::cast_to_position_detector_dispatcher_impl(_p_impl)->run_processing_loop(settings_func, exc_queue);
}

void position_detector_dispatcher::stop_processing_loop()
{
	details::cast_to_position_detector_dispatcher_impl(_p_impl)->stop_processing_loop();
}

} // namespace position_detector
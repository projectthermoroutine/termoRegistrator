#include <CppUnitTest.h>
#include <common\unit_tests_common.h>

#include <vector>
#include <algorithm>
#include <fstream>
#include "server_proxy_pd_connector.h"
#include "client_pd_dispatcher.h"

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include "position_detector_dispatcher.h"

#include "udp_packets_test_source.h"
#include "packets_helpers.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace client_pd_dispatcher_test_project
{

	using namespace unit_tests_common;
	using namespace position_detector;

	static struct test_synchro_packet_t g_synchro_packet{ 0, 0, 0 };
	test_synchro_packet_t gen_next_syncro_packet()
	{
		g_synchro_packet.counter++;
		g_synchro_packet.speed++;
		g_synchro_packet.timestamp++;
		return g_synchro_packet;
	}

	void reset_syncro_packet()
	{
		g_synchro_packet.counter = 0;
		g_synchro_packet.speed = 0;
		g_synchro_packet.timestamp = 0;
	}

#define SIZE_TEST_EVENT_PACKET 4096

	struct test_event_packet_t
	{
		BYTE data[SIZE_TEST_EVENT_PACKET];
	};


	const unsigned int g_event_packet_max_index = 5;
	static unsigned int g_event_packet_index = 1;
	test_event_packet_t gen_next_event_packet()
	{

		std::string filename("../../packets/packet");
		filename += std::to_string(g_event_packet_index++) + ".xml";
		if (g_event_packet_index > g_event_packet_max_index) g_event_packet_index = 1;
		std::string test_packet(
			(std::istreambuf_iterator<char>(
			*(std::unique_ptr<std::ifstream>(
			new std::ifstream(filename)
			)).get()
			)),
			std::istreambuf_iterator<char>()
			);

		test_event_packet_t packet = { 0 };
		std::memcpy(packet.data, test_packet.c_str(), std::min(SIZE_TEST_EVENT_PACKET, (int)test_packet.size()));
		return packet;
	}

	void reset_event_packet()
	{
		g_event_packet_index = 1;
	}


	template<typename TMessage>
	using append_packet_func_t = std::function<void(const TMessage &)>;

	using append_event_packet_func_t = append_packet_func_t<events::event_packet_ptr_t>;
	using append_synchro_packet_func_t = append_packet_func_t<synchronization::sync_packet_ptr_t>;

	struct packets_stream
	{
		append_event_packet_func_t append_event_packet_func;
		append_synchro_packet_func_t append_synchro_packet_func;

		//template<typename TMessage>
		//void operator<<(const TMessage & packet)
		inline void operator<<(const events::event_packet_ptr_t & packet)
		{
			append_event_packet_func(packet);
		}
		inline void operator<<(const synchronization::sync_packet_ptr_t & packet)
		{
			append_synchro_packet_func(packet);
		}
		template<typename TMessage>
		void dispatch_message(const BYTE * message, unsigned int message_size)
		{
			LOG_STACK();

			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			/*	if (is_message_well_size(message_size)){
			LOG_WARN() << "Invalide message size: " << message_size;
			return;
			}*/
			TMessage packet;
			try{

				packet =
					parce_packet_from_message<TMessage>(
					message, message_size);

				*this << packet;

			}
			catch (const deserialization_error & exc)
			{
				LOG_WARN() << exc.what();
				return;
			}

			//_event_packets_container.emplace(event_message->id, event_message);
		}

	};


	class test_packets_manager
	{
	public:

		template<typename TMessage> void add_packet(const TMessage &);
		template<typename TMessage> TMessage get_packet(const timestamp_t &);
		size_t get_synchro_size() { return _synchro_packets_container.size(); }
		size_t get_events_size(){ return _event_packets_container.size(); }

	private:

		std::vector<event_packet_ptr_t> _event_packets_container;
		std::map<timestamp_t, sync_packet_ptr_t> _synchro_packets_container;

	};

	template<>
	void test_packets_manager::add_packet(const sync_packet_ptr_t & packet)
	{
		_synchro_packets_container.emplace(packet->timestamp, packet);
	}

	template<>
	void test_packets_manager::add_packet(const event_packet_ptr_t & packet)
	{
		_event_packets_container.emplace_back(packet);
	}

	template<>
	sync_packet_ptr_t test_packets_manager::get_packet(const timestamp_t & timestamp)
	{
		return _synchro_packets_container[timestamp];
	}

	template<>
	event_packet_ptr_t test_packets_manager::get_packet(const timestamp_t & timestamp)
	{
		return _event_packets_container[(unsigned int)timestamp];
	}

	void notify_dispatch_error(const std::string& msg)
	{

		std::string tmp = msg;
		tmp += "Recieved OK.";
	}


	std::wstring to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}


	using namespace position_detector::events;
	using namespace position_detector::synchronization;



	TEST_CLASS(test_position_detector_connector)
	{
	public:
		TEST_METHOD(test_server_proxy_pd_connector_creation)
		{
			checked_execute([]
			{
				server_proxy_pd_connector connector(notify_dispatch_error, notify_dispatch_error, notify_dispatch_error);
			});
		}

		TEST_METHOD(test_set_config_proxy_server_pd)
		{
			checked_execute([]
			{
				server_proxy_pd_connector connector(notify_dispatch_error, notify_dispatch_error, notify_dispatch_error);

				connection_address pd_address{ "127.0.0.1", "0.0.0.0", 32222 };
				connection_address pd_events_address{ "127.0.0.1", "0.0.0.0", 32223 };

				std::vector<std::string> config{ "pd_ip", pd_address.ip, "pd_i_ip", pd_address.i_ip, "pd_port", std::to_string(pd_address.port),
					"pd_events_ip", pd_events_address.ip, "pd_i_events_ip", pd_events_address.i_ip, "pd_events_port", std::to_string(pd_events_address.port)
				};

				connector.setConfig(config);
			});
		}
		TEST_METHOD(test_get_config_proxy_server_pd)
		{
			checked_execute([]
			{
				server_proxy_pd_connector connector(notify_dispatch_error, notify_dispatch_error, notify_dispatch_error);

				connection_address pd_address{ "127.0.0.1", "0.0.0.0", 32222 };
				connection_address pd_events_address{ "127.0.0.1", "0.0.0.0", 32223 };
				std::vector<std::string> config{ "pd_ip", pd_address.ip, "pd_i_ip", pd_address.i_ip, "pd_port", std::to_string(pd_address.port),
					"pd_events_ip", pd_events_address.ip, "pd_i_events_ip", pd_events_address.i_ip, "pd_events_port", std::to_string(pd_events_address.port)
				};
				connector.setConfig(config);

				auto result_config = connector.getConfig();

			});
		}


		TEST_METHOD(test_proxy_server_pd_process_packets)
		{
			checked_execute([]
			{
				const char sync_ip[] = "127.0.0.1";
				const char events_ip[] = "127.0.0.1";
				const unsigned short sync_port = 32222;
				const unsigned short events_port = 32223;

				handle_holder closing_event(sync_helpers::create_basic_event_object(true));

				test_packets_manager packets_manager;
				packets_stream packets_ostream;
				packets_ostream.append_event_packet_func = std::bind(&test_packets_manager::add_packet<event_packet_ptr_t>, &packets_manager, std::placeholders::_1);
				packets_ostream.append_synchro_packet_func = std::bind(&test_packets_manager::add_packet<sync_packet_ptr_t>, &packets_manager, std::placeholders::_1);

				process_packets_factory factory;
				factory.create_process_synchro_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<sync_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });
				factory.create_process_event_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<event_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });


				position_detector_dispatcher::active_state_callback_func_t active_state_callback_func([](bool){});

				scoped_WSA WSA_startup;

				test_udp_server synchro_server(sync_ip, sync_port);
				test_udp_server events_server(events_ip, events_port);

				server_proxy_pd_connector connector(notify_dispatch_error, notify_dispatch_error, notify_dispatch_error);

				connection_address pd_address{ "127.0.0.1", "0.0.0.0", 32222 };
				connection_address pd_events_address{ "127.0.0.1", "0.0.0.0", 32223 };
				std::vector<std::string> config{ "pd_ip", pd_address.ip, "pd_i_ip", pd_address.i_ip, "pd_port", std::to_string(pd_address.port),
					"pd_events_ip", pd_events_address.ip, "pd_i_events_ip", pd_events_address.i_ip, "pd_events_port", std::to_string(pd_events_address.port)
				};

				Assert::IsTrue(connector.setConfig(config),L"proxy server pd was already configured.");

				auto result_config = connector.getConfig();

				auto settings_func = [result_config](const std::string &key)->std::vector<std::string>
				{
					std::vector<std::string> result;
					if (key == "Syncronizer device")
					{
						result.push_back(result_config[0].share_memory_name);
						result.push_back(std::to_string(result_config[0].share_memory_size));
						result.push_back(result_config[0].read_event_name);
					}
					if (key == "Syncronizer events device")
					{
						result.push_back(result_config[1].share_memory_name);
						result.push_back(std::to_string(result_config[1].share_memory_size));
						result.push_back(result_config[1].read_event_name);
					}

					return result;
				};

				exception_queue_ptr_t exc_queue(new exception_queue);
				bool is_exception_occurred = false;
				std::string exc_occurred;
				thread_exception_handler thread_exception_handler(exc_queue, [&is_exception_occurred, &exc_occurred](const std::exception_ptr &exc_ptr)
				{
					try{
						std::rethrow_exception(exc_ptr);
					}
					catch (const std::exception& exc)
					{
						exc_occurred = exc.what();
					}
					catch (...)
					{
						exc_occurred = "Some exception";
					}

					is_exception_occurred = true;
				});

				thread_exception_handler.start_processing();

				position_synchronizer_dispatcher packets_dispatcher(factory, active_state_callback_func);
				packets_dispatcher.run_processing_loop(settings_func, exc_queue);
				std::this_thread::sleep_for(std::chrono::seconds(3));

				std::thread syncro_server_thread([&synchro_server](){synchro_server.start_server<test_synchro_packet_t>(gen_next_syncro_packet, count_messages, 50000); });
				std::thread events_server_thread([&events_server](){events_server.start_server<test_event_packet_t>(gen_next_event_packet, count_messages >> 5, 100000); });

				if (syncro_server_thread.joinable())
					syncro_server_thread.join();
				if (events_server_thread.joinable())
					events_server_thread.join();

				std::this_thread::sleep_for(std::chrono::seconds(1));
				sync_helpers::set_event(closing_event);

				packets_dispatcher.stop_processing_loop();
				auto exc_text = to_wstring(exc_occurred);
				Assert::IsFalse(is_exception_occurred, exc_text.c_str());

				reset_syncro_packet();
				test_synchro_packet_t right_messages_buffer[count_messages];
				for (auto i = 0; i < count_messages; ++i)
					right_messages_buffer[i] = gen_next_syncro_packet();

				Assert::AreEqual(packets_manager.get_synchro_size(), count_messages, L"Invalid count synchro packets");
				for (auto i = 0; i < count_messages; ++i)
				{
					auto right_packet = right_messages_buffer[i];
					auto packet = packets_manager.get_packet<sync_packet_ptr_t>(right_packet.timestamp);
					if (!compare_synchro_packets(right_packet, *(packet.get())))
						Assert::Fail(L"Error data in the messages buffer.");
				}

				reset_event_packet();

				std::vector<event_packet_ptr_t> right_packets_buffer;
				for (auto i = 0; i < (count_messages >> 5); ++i){
					auto raw_data = gen_next_event_packet();
					event_packet_ptr_t packet;
					try{
						packet = parce_packet_from_message<event_packet_ptr_t>((const BYTE*)(&raw_data), sizeof(test_event_packet_t));
					}
					catch (const deserialization_error &)
					{
						continue;
					}

					if (packet)
						right_packets_buffer.emplace_back(packet);
				}

				Assert::AreEqual(packets_manager.get_events_size(), right_packets_buffer.size(), L"Invalid count event packets");
				ULONG64 i = 0;
				for (auto cur_packet : right_packets_buffer)
				{
					auto packet = packets_manager.get_packet<event_packet_ptr_t>(i);
					if (!compare_event_packets(cur_packet.get(), packet.get()))
						Assert::Fail(L"Error data in the messages buffer.");
					i++;
				}

				std::this_thread::sleep_for(std::chrono::seconds(1));
			});
		}


	};
}
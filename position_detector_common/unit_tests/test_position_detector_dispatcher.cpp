#include <CppUnitTest.h>

#include <common\unit_tests_common.h>
#include <common\sync_helpers.h>

#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <functional>
#include <fstream>

#include <position_detector_dispatcher_common.h>
#include <position_detector_packet.h>
#include "position_detector_packet_details.h"

#include "packets_helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace position_detector_test_project
{
	
	using namespace unit_tests_common;
	using namespace position_detector;

	static struct test_synchro_packet_t g_synchro_packet{ 0, 0, 0 };
	void gen_next_syncro_packet(BYTE * dst)
	{
		std::memcpy(dst, &g_synchro_packet, sizeof(test_synchro_packet_t));
		g_synchro_packet.counter++;
		g_synchro_packet.speed++;
		g_synchro_packet.timestamp++;
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
	void gen_next_event_packet(BYTE * dst)
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
		std::memcpy(dst, test_packet.c_str(), std::min(SIZE_TEST_EVENT_PACKET, (int)test_packet.size()));
	}

	void reset_event_packet()
	{
		g_event_packet_index = 1;
	}

	class fake_connector_api : public connector_api
	{
		using gen_data_func_t =void(*)(BYTE *);
		using get_data_func_t = std::function<int(BYTE *, packet_size_t &)>;
	public:
		fake_connector_api(const std::vector<std::string>& settings)
		{
			count_gens = 0;
			max_count_gens = std::stoul(settings[1]);
			hStopEvent = (HANDLE)std::stoul(settings[2]);
			if (settings[0] == "synchro")
			{
				get_data_func = std::bind(&fake_connector_api::gen_data<test_synchro_packet_t>, this, std::placeholders::_1, std::placeholders::_2);
				gen_data_func = gen_next_syncro_packet;
				return;
			}
			if (settings[0] == "events")
			{
				get_data_func = std::bind(&fake_connector_api::gen_data<test_event_packet_t>, this, std::placeholders::_1, std::placeholders::_2);
				gen_data_func = gen_next_event_packet;
				return;
			}

			throw std::invalid_argument("Invalid argument settings passed.");
		}

		int get_message(get_message_struct * const buffer, const packet_size_t buffer_size, const HANDLE)
		{
			auto data_size = buffer_size;
			return get_data_func(buffer->data, data_size);
		}
		~fake_connector_api() {}
		void close() {}
		fake_connector_api(const fake_connector_api &) = delete;
		fake_connector_api & operator = (const fake_connector_api &) = delete;
	private:
		template<typename TMessage>
		int gen_data(BYTE *dstData, packet_size_t & data_size)
		{
			if (max_count_gens == count_gens){
				data_size = 0;
				if (!SetEvent(hStopEvent))
				{
					throw std::runtime_error("Could not trigger event.");
				}
				return 0;

			}
			if (data_size < sizeof(TMessage))
				throw std::invalid_argument("Invalid argument data size passed.");
			
			data_size = sizeof(TMessage);
			gen_data_func(dstData);
			count_gens++;
			return data_size;
		}

		gen_data_func_t gen_data_func;
		get_data_func_t get_data_func;
		ULONG32 count_gens;
		ULONG32 max_count_gens;
		HANDLE hStopEvent;

	};


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


	connector_ptr_t create_connector(const std::vector<std::string>& settings)
	{
		return std::make_shared<position_detector_connector>(std::make_shared<fake_connector_api>(settings));
	}


	using namespace position_detector::events;
	using namespace position_detector::synchronization;

	TEST_CLASS(test_position_detector_common_dispatcher)
	{
	public:
		TEST_METHOD(test_dispatcher_common_creation)
		{
			checked_execute([]
			{
				{
					position_detector_dispatcher::active_state_callback_func_t active_state_callback_func([](bool){});
					position_detector_dispatcher::settings_func_t settings_func([](const std::string &)->std::vector<std::string>
					{
						return std::vector<std::string>();
					}
					);
					packets_stream packets_ostream;
					process_packets_factory factory;
					factory.create_process_synchro_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<sync_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });
					factory.create_process_event_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<event_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });
					position_detector_dispatcher packets_dispatcher(factory, create_connector_func_t(create_connector), active_state_callback_func);
				}

			});
		}

		TEST_METHOD(test_dispatcher_common_process_incoming_message)
		{
			checked_execute([]
			{
				handle_holder closing_event1(sync_helpers::create_basic_event_object(true));
				handle_holder closing_event2(sync_helpers::create_basic_event_object(true));
				auto hEvent1 = closing_event1.get();
				auto hEvent2 = closing_event2.get();

				test_packets_manager packets_manager;
				packets_stream packets_ostream;
				packets_ostream.append_event_packet_func = std::bind(&test_packets_manager::add_packet<event_packet_ptr_t>, &packets_manager, std::placeholders::_1);
				packets_ostream.append_synchro_packet_func = std::bind(&test_packets_manager::add_packet<sync_packet_ptr_t>, &packets_manager, std::placeholders::_1);

				process_packets_factory factory;
				factory.create_process_synchro_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<sync_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });
				factory.create_process_event_packet_func = create_process_packet_func_t([&packets_ostream]()->std::function<void(const BYTE *, unsigned int)>{return std::bind(&packets_stream::dispatch_message<event_packet_ptr_t>, &packets_ostream, std::placeholders::_1, std::placeholders::_2); });

				position_detector_dispatcher::active_state_callback_func_t active_state_callback_func([](bool){});
				
				const ULONG32 count_packets = 1000;
				position_detector_dispatcher::settings_func_t settings_func([count_packets, hEvent1, hEvent2](const std::string &key)->std::vector<std::string>
																				{
																					std::vector<std::string> res;
																					if (key == "Syncronizer device"){
																						res.emplace_back(std::string("synchro"));
																						res.emplace_back(std::to_string(count_packets));
																						res.emplace_back(std::to_string((ULONG32)hEvent1));
																					}
																					if (key == "Syncronizer events device"){
																						res.emplace_back(std::string("events"));
																						res.emplace_back(std::to_string(count_packets>>5));
																						res.emplace_back(std::to_string((ULONG32)hEvent2));
																					}
																					return res;
																				}
																		);

				position_detector_dispatcher packets_dispatcher(factory, create_connector_func_t(create_connector), active_state_callback_func);
				packets_dispatcher.run_processing_loop(settings_func, std::make_shared<exception_queue>());
				
				sync_helpers::wait(closing_event1);
				sync_helpers::wait(closing_event2);
				packets_dispatcher.stop_processing_loop();

				reset_syncro_packet();
				test_synchro_packet_t right_messages_buffer[count_packets];
				for (auto i = 0; i < count_packets; ++i){
					gen_next_syncro_packet((BYTE*)(&right_messages_buffer[i]));
				}

				Assert::AreEqual(packets_manager.get_synchro_size(), count_packets, L"Invalid count synchro packets");
				for (auto i = 0; i < count_packets; ++i)
				{
					auto right_packet = right_messages_buffer[i];
					auto packet = packets_manager.get_packet<sync_packet_ptr_t>(right_packet.timestamp);
					if (!compare_synchro_packets(right_packet, *(packet.get())))
						Assert::Fail(L"Error data in the messages buffer.");
				}

				reset_event_packet();

				std::vector<event_packet_ptr_t> right_packets_buffer;
				for (auto i = 0; i < (count_packets >> 5); ++i)
				{
					test_event_packet_t raw_data = { 0 };
					//std::fill(&raw_data, &raw_data + sizeof(test_event_packet_t), 0);
					gen_next_event_packet(raw_data.data);
					event_packet_ptr_t packet;
					try{
						packet = parce_packet_from_message<event_packet_ptr_t>((const BYTE*)(&raw_data), sizeof(test_event_packet_t));
					}
					catch (const deserialization_error & exc)
					{
						auto text = exc.what();
						text = "";
						continue;
					}

					if (packet)
						right_packets_buffer.push_back(packet);
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
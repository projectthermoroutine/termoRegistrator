#include <CppUnitTest.h>
#include <common\unit_tests_common.h>

#include <vector>
#include <algorithm>
#include <fstream>
#include "client_pd_dispatcher.h"

#include "udp_packets_test_source.h"
#include "packets_helpers.h"

#include "position_detector_packets_manager.h"
#include <position_detector_common\details\position_detector_packet_details.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace position_detector
{
	using namespace events;
	using namespace synchronization;

	void
		calculate_picket_offset(
		coordinate_t coordinate,
		const nonstandard_kms_map_t & nonstandard_kms,
		picket_t & picket,
		offset_t & offset
		);
}

namespace client_pd_manager_packets
{

	using namespace unit_tests_common;
	using namespace position_detector;
	using namespace client_pd_dispatcher_test_project;

	class sync_packet_generator
	{
	public:
		sync_packet_generator(int checkpont_period) :
			_checkpont_period(checkpont_period), 
			_counter(1),
			tick_count0(::GetTickCount64()),
			timestamp0(std::time(nullptr) * 1000)
		{
			std::memset(&_synchro_packet, 0, sizeof(struct test_synchro_packet_t));
		}

		test_synchro_packet_t gen_next_syncro_packet()
		{
			time_t timestamp = timestamp0++;// ::GetTickCount64();
			if (_counter % _checkpont_period == 0){
				_checkpoint_timestamps.push_back(timestamp);
			}

			_synchro_packet.counter++;
			_synchro_packet.speed++;
			_synchro_packet.timestamp = timestamp;
			_counter++;
			return _synchro_packet;
		}

		const std::vector<time_t> & get_checkpoint_timestamps() const { return _checkpoint_timestamps; }
	private:
		int _checkpont_period;
		int _counter;

		time_t tick_count0;
		time_t timestamp0;

		struct test_synchro_packet_t _synchro_packet;

		std::vector<time_t> _checkpoint_timestamps;

	};

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

	std::wstring to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	void errors_callback_stub(const std::string&)
	{

	}

	TEST_CLASS(test_client_pd_packets_manager)
	{
	public:
		TEST_METHOD(test_packets_manager)
		{
			checked_execute([]
			{
				const char sync_ip[] = "127.0.0.1";
				const char events_ip[] = "127.0.0.1";
				const unsigned short sync_port = 32222;
				const unsigned short events_port = 32223;
				const int max_test_exceptions = 10;

				packets_manager_ptr_t _packets_manager(std::make_shared<packets_manager>(5));

				scoped_WSA WSA_startup;

				client_pd_dispatcher _client_pd_dispatcher(_packets_manager, errors_callback_stub);

				test_udp_server synchro_server(sync_ip, sync_port);
				test_udp_server events_server(events_ip, events_port);

				connection_address pd_address{ "127.0.0.1", "0.0.0.0", 32222 };
				connection_address pd_events_address{ "127.0.0.1", "0.0.0.0", 32223 };

				exception_queue_ptr_t exc_queue(std::make_shared<exception_queue>());
				bool is_exception_occurred = false;
				std::string exc_occurred;
				int exceptions_counter = 0;
				thread_exception_handler thread_exception_handler(exc_queue, [&is_exception_occurred, &exc_occurred, &exceptions_counter](const std::exception_ptr &exc_ptr)
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
					exceptions_counter++;
					is_exception_occurred = true;
				});

				thread_exception_handler.start_processing();

				_client_pd_dispatcher.run_processing_loop(pd_address, pd_events_address, exc_queue);

				std::this_thread::sleep_for(std::chrono::seconds(3));

				sync_packet_generator _sync_packet_generator(6);

				std::thread syncro_server_thread([&synchro_server, &_sync_packet_generator]()
				{
					synchro_server.start_server<test_synchro_packet_t>(
										std::bind(&sync_packet_generator::gen_next_syncro_packet, &_sync_packet_generator),
										100,
										10000); 
				});

				std::this_thread::sleep_for(std::chrono::seconds(1));

				std::thread events_server_thread([&events_server](){events_server.start_server<test_event_packet_t>(gen_next_event_packet, 1, 100000); });

				std::this_thread::sleep_for(std::chrono::seconds(3));

				std::thread exceptions_thread([&exc_queue, max_test_exceptions]()
				{
					for (int i = 0; i < max_test_exceptions; i++){
						try{
							throw std::runtime_error("test exception.");
						}
						catch (...)
						{
							exc_queue->raise_exception();
						}
					}
				});

				if (syncro_server_thread.joinable())
					syncro_server_thread.join();
				if (events_server_thread.joinable())
					events_server_thread.join();

				if (exceptions_thread.joinable())
					exceptions_thread.join();

				_client_pd_dispatcher.stop_processing_loop();

				auto checkpoint_timestamps = _sync_packet_generator.get_checkpoint_timestamps();
				std::wstring err = L"can't get point info. checkpoint_timestamps size: " + std::to_wstring(checkpoint_timestamps.size());

				track_point_info point_info;
				for (auto & timestamp : checkpoint_timestamps)
				{
					//auto res = _packets_manager->get_point_info_by_time(timestamp, point_info);
					auto res = _packets_manager->get_last_point_info(point_info);
					Assert::IsTrue(res, err.c_str());
				}

				auto exc_text = to_wstring(exc_occurred);
				Assert::IsFalse(is_exception_occurred, exc_text.c_str());


			});
		}


		TEST_METHOD(test_picket_offset_calculation)
		{
			checked_execute([]
			{
				coordinate_t test_coord = -24 * 100 * 10;
				picket_t picket = 0;
				offset_t offset = 0;

				nonstandard_kms_t nonstandard_kms;
				calculate_picket_offset(test_coord, nonstandard_kms, picket,offset);
				Assert::AreEqual(0,picket);
				Assert::AreEqual(-24000, offset);

				test_coord = -31 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(0, picket);
				Assert::AreEqual(-31000, offset);

				test_coord = -3 * 1000 * 100 * 10 - 30 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(-30, picket);
				Assert::AreEqual(-30000, offset);

				nonstandard_kms = nonstandard_kms_t{ { 1, 125 }, { 0, 25 }, { 3, 75 } };
				test_coord = -24 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(0, picket);
				Assert::AreEqual(-24000, offset);

				test_coord = -31 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(-1, picket);
				Assert::AreEqual(-6000, offset);

				test_coord = -3 * 1000 * 100 * 10 - 30 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(-30, picket);
				Assert::AreEqual(-5000, offset);


				nonstandard_kms = nonstandard_kms_t{ { 2, 82 }, { 16, 107 }, { 21, 105 }, { 26, 82 } };
				test_coord = 2 * 1000 * 100 * 10 + 273 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(22, picket);
				Assert::AreEqual(79000, offset);

				test_coord = 2 * 1000 * 100 * 10 + 193 * 100 * 10;
				calculate_picket_offset(test_coord, nonstandard_kms, picket, offset);
				Assert::AreEqual(21, picket);
				Assert::AreEqual(104000, offset);


			});
		}


	};
}
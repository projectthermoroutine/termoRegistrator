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


#include <position_detector_packet.h>
#include "position_detector_packet_details.h"
#include "packets_helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace position_detector_test_project
{
	using namespace unit_tests_common;

	using namespace position_detector::events;

	bool check_event_packet1(const position_detector::events::event_packet_ptr_t & packet)
	{
		position_detector::events::StartCommandEvent_packet right_packet;
		right_packet.type = event_type::EvStartCommandEvent;
		right_packet.counter = 96040;
		right_packet.dataTime = "2014-10-20T17:18:20.5713208+04:00";
		right_packet.source = L"POLOVODOV3";
		right_packet.guid = "1c222581-9f25-4810-86e9-cf8d360478c4";

		right_packet.data_time.year = 2014;
		right_packet.data_time.month = 10;
		right_packet.data_time.day = 20;
		right_packet.data_time.hour = 17;
		right_packet.data_time.minute = 18;
		right_packet.data_time.second = 20;
		right_packet.data_time.millisecond = 5713208;
		right_packet.data_time.tzd = 240;

		return  compare_event_packets(&right_packet, packet.get());
	}

	TEST_CLASS(test_position_detector_common_packet)
	{
	public:

		void test_event_packet(const char * filename, check_event_packet_func_t check_event_packet_func)
		{
			std::string test_packet(
				(std::istreambuf_iterator<char>(
				*(std::unique_ptr<std::ifstream>(
				new std::ifstream(filename)
				)).get()
				)),
				std::istreambuf_iterator<char>()
				);

			auto packet =
				position_detector::parce_packet_from_message<position_detector::events::event_packet_ptr_t>(
				(const BYTE *)test_packet.c_str(),
				(unsigned int)test_packet.size());

			auto res = check_event_packet_func(packet);

			Assert::IsTrue(res, L"Incorrect event packet data.");

		}

		void test_synchro_packet()
		{
			const counter_t test_counter = 12345UL;
			const timestamp_t test_timestamp = 123456789101112ULL;
			const speed_t test_speed = 0x92345678UL;

			test_synchro_packet_t test_packet{ test_counter, test_timestamp, test_speed };

			auto packet =
				position_detector::parce_packet_from_message<position_detector::synchronization::sync_packet_ptr_t>(
				(const BYTE *)&test_packet,
				(unsigned int)sizeof(test_synchro_packet_t));

			auto res = compare_synchro_packets(test_packet, *(packet.get()));
			Assert::IsTrue(res, L"Incorrect synchronization packet data.");

		}

		TEST_METHOD(test_position_detector_packet_parser)
		{
			checked_execute([this]
			{
				test_event_packet("../../packets/packet1.xml", check_event_packet1);
				test_synchro_packet();

			});

		}


		TEST_METHOD(test_packet_parser)
		{
			checked_execute([this]
			{
				std::string test_packet(
					(std::istreambuf_iterator<char>(
					*(std::unique_ptr<std::ifstream>(
					new std::ifstream("../../packets/baku/event1.xml")
					)).get()
					)),
					std::istreambuf_iterator<char>()
					);

				try{
					auto packet =
						position_detector::parce_packet_from_message<position_detector::events::event_packet_ptr_t>(
						(const BYTE *)test_packet.c_str(),
						(unsigned int)test_packet.size());
				}
				catch (const deserialization_error&){

					Assert::Fail(L"deserialization_error");
				}

			});


		}


	};
}
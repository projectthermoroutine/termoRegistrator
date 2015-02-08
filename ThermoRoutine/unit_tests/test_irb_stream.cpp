#include <CppUnitTest.h>
#include <common\unit_tests_common.h>

#include <vector>
#include <algorithm>
#include <fstream>

#include "packets_helpers.h"

#include "irb_file_helper.h"

#include <ctime>
#include <ratio>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ThermoRoutine_test_project
{

	using namespace unit_tests_common;
	using namespace position_detector;
	using namespace client_pd_dispatcher_test_project;

	static struct test_synchro_packet_t g_synchro_packet{ 0, 0, 0 };

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
			auto tick_count = ::GetTickCount64() - tick_count0;
			time_t timestamp = timestamp0 + tick_count % 1000 + tick_count / 1000;
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

	using namespace irb_file_helper;

	TEST_CLASS(test_irb_stream)
	{
	public:
		TEST_METHOD(test_irb_stream_read)
		{
			checked_execute([]
			{

				IRBFile irb_file0("../../stream_data/irmetro1_0000.irb");
				auto frame0 = irb_file0.read_frame(1);

				//IRBFile irb_file("../../stream_data/copy_irb_file.irb");
				//auto frame1 = irb_file.read_frame(1);

			});
		}

		TEST_METHOD(test_irb_stream_write)
		{
			checked_execute([]
			{
				using namespace std::chrono;
				IRBFile irb_file("../../stream_data/irmetro1_0000.irb");

				auto frames_count = irb_file.count_frames();

				auto copy_file = create_irb_file("../../stream_data/copy_irb_file.irb", irb_file_version::original, frames_count);

				auto frames_id_list = irb_file.get_frames_id_list();
				std::vector<irb_frame_shared_ptr_t> frames;

				frame_id_t new_frame_id = frames_id_list.back() + 1;
				for (auto & frame_id : frames_id_list)
				{
					irb_frame_shared_ptr_t frame(irb_file.read_frame(frame_id).release());

					auto frame_time = frame->get_frame_time_in_sec();

					int days = (int)frame_time;
					auto milli = frame_time - (double)days;
					int mssec = milli * 1000000;

					duration<double> ttt = duration<double>(milli);


					//duration<int> time_span = duration_cast<int>(ttt);

					frames.emplace_back(irb_frame_shared_ptr_t(irb_file.read_frame(frame_id).release()));
					frames.back()->id = new_frame_id++;
				}

				IRBFile copy_irb_file(copy_file);

				copy_irb_file.append_frames(frames);

			});
		}

	};
}
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

#include <position_detector_connector.h>
#include <position_detector_connector_details.h>
#include "udp_packets_test_source.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace position_detector_test_project
{

	typedef unsigned long message_t;
	const unsigned int message_size = sizeof(message_t);

	static message_t cur_message = 0;
	message_t gen_next_message()
	{
		return cur_message++;
	}

	void reset_message()
	{
		cur_message = 0;
	}



	using namespace unit_tests_common;
	using namespace position_detector;

	TEST_CLASS(test_position_detector_connector)
	{
	public:
		TEST_METHOD(test_connector_creation)
		{
			checked_execute([]
			{
				{
					scoped_WSA WSA_startup;
					position_detector_connector connector("127.0.0.1", 32220);
					connector.close();
				}

			});
		}

		TEST_METHOD(test_connector_process_incoming_message)
		{
			checked_execute([]
			{
				const char ip[] = "127.0.0.1";
				const unsigned short port = 32222;
				message_t messages_buffer[count_messages];

				std::fill(messages_buffer, &messages_buffer[count_messages], 0);

				handle_holder closing_event(sync_helpers::create_basic_event_object(true));

				scoped_WSA WSA_startup;

				test_udp_server server(ip, port);
				position_detector_connector client(ip, port);

				std::thread client_thread([&client, &closing_event](message_t * messages_buffer)
				{
					while (!sync_helpers::is_event_set(closing_event))
					{
						client.process_incoming_message(
							closing_event.get(),
							[messages_buffer](const BYTE * message, unsigned int message_size)
						{

							static int count_messages_recieved = 0;
							if (message_size != position_detector_test_project::message_size)
							{
								Assert::Fail(L"Invalide message size.");
							}
							if (count_messages_recieved >= position_detector_test_project::count_messages)
							{
								Assert::Fail(L"Too many messages recieved.");
							}

							std::memcpy(const_cast<message_t*>(&messages_buffer[count_messages_recieved++]), message, message_size);
						});
					}
				}, messages_buffer
				);

				std::thread server_thread([&server](){server.start_server<message_t>(gen_next_message, count_messages,1000); });

				if (server_thread.joinable())
					server_thread.join();

				std::this_thread::sleep_for(std::chrono::seconds(5));
				sync_helpers::set_event(closing_event);

				if (client_thread.joinable())
					client_thread.join();

				reset_message();
				message_t right_messages_buffer[count_messages];
				for (auto i = 0; i < count_messages; ++i)
					right_messages_buffer[i] = gen_next_message();

				for (auto i = 0; i < count_messages; ++i)
				{
					if (right_messages_buffer[i] != messages_buffer[i])
						Assert::Fail(L"Error data in the messages buffer.");
				}

			});
		}
	};
}
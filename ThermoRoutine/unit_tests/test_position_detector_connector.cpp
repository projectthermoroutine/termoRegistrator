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

#include <position_detector_connector_api.h>
#include "position_detector_dispatcher.h"
#include "udp_packets_test_source.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace client_pd_dispatcher_test_project
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
				}

			});
		}

		TEST_METHOD(test_connector_process_incoming_message)
		{
			checked_execute([]
			{

			});
		}
	};
}
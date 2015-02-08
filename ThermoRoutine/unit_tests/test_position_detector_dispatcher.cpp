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

#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include "position_detector_dispatcher.h"

#include "udp_packets_test_source.h"
#include "packets_helpers.h"
#include "server_proxy_pd_connector.h"
#include "client_pd_dispatcher.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace client_pd_dispatcher_test_project
{

	using namespace unit_tests_common;
	using namespace position_detector;



	using namespace position_detector::events;
	using namespace position_detector::synchronization;


	TEST_CLASS(test_position_detector_dispatcher)
	{
	public:
		TEST_METHOD(test_dispatcher_creation)
		{
			checked_execute([]
			{

			});
		}

		TEST_METHOD(test_dispatcher_process_incoming_message)
		{
			checked_execute([]
			{


			});
		}
	};
}
#pragma once

#include <functional>
#include <memory>
#include <position_detector_common\position_detector_connector.h>
#include <position_detector_common\position_detector_dispatcher_common.h>
#include <position_detector_common\position_detector_packet.h>

namespace position_detector
{
	class position_synchronizer_dispatcher final : public position_detector_dispatcher
	{
	public:
		position_synchronizer_dispatcher(process_packets_factory, active_state_callback_func_t);

		position_synchronizer_dispatcher(const position_synchronizer_dispatcher &) = delete;
		position_synchronizer_dispatcher & operator = (const position_synchronizer_dispatcher &) = delete;
	};

	class scoped_WSA final
	{
	public:
		scoped_WSA();
		~scoped_WSA();

		scoped_WSA(const scoped_WSA &) = delete;
		scoped_WSA & operator = (const scoped_WSA &) = delete;
	};
} // namespace position_detector

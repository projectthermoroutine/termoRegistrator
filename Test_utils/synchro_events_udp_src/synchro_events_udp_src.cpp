#include <memory>
#include <vector>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <thread>             // std::thread
#include <condition_variable>
#include "udp_packets_test_source.h"
#include <common\sync_helpers.h>


typedef ULONG32 counter_t;
typedef ULONG64 timestamp_t;
typedef ULONG32 speed_t;

#pragma pack (push)
#pragma pack (1)


#define DIRECTION_MASK 0x80000000UL
struct test_synchro_packet_t
{
	counter_t counter;
	timestamp_t timestamp;
	speed_t speed;
};

#pragma pack (pop)

using namespace test_packets_udp_source;

enum class POSITON_STRATEGY{
	DECREASE,
	UNCHANGE,
	INCREASE
};


static struct test_synchro_packet_t g_synchro_packet{ 10, 0, 0 };
test_synchro_packet_t gen_next_syncro_packet(POSITON_STRATEGY strategy);


class sync_packet_generator
{
public:
	sync_packet_generator(POSITON_STRATEGY strategy) :_strategy(strategy) {}

	test_synchro_packet_t operator()() { return gen_next_syncro_packet(_strategy); }
private:

	POSITON_STRATEGY _strategy;

};


class sync_packet_generator_from_file
{
public:
	sync_packet_generator_from_file(const std::string& file_name) :
		_current_packet_pointer(0),
		_read_data_block_size_b(4 * 1024 * 1024),
		_read_error(false),
		_stream_end(false),
		_reading(false),
		_buffer(std::make_unique<BYTE[]>(_read_data_block_size_b)),
		_data(std::make_unique<BYTE[]>(_read_data_block_size_b))
	{
		_stream.open(file_name.c_str(), std::ios::in | std::ios::binary);
		if (_stream.rdstate() == std::ios::failbit)
		{
			throw std::invalid_argument("Can't open stream: '" + file_name + "'");
		}

		auto event = sync_helpers::create_basic_event_object(false);
		read_event.swap(event);

		std::thread _thread([this](){ this->reading_loop(); });

		_reading_thread.swap(_thread);


	}

	sync_packet_generator_from_file(const sync_packet_generator_from_file&) = delete;
	sync_packet_generator_from_file(sync_packet_generator_from_file&& other)
	{
		
		_reading_thread.swap(other._reading_thread);
		_stream.swap(other._stream);
	}

	~sync_packet_generator_from_file()
	{
		_stream_end = true;
		sync_helpers::set_event(read_event);

		if (_reading_thread.joinable())
			_reading_thread.join();

		_stream.close();
	}

	test_synchro_packet_t operator()() { return this->gen_next_syncro_packet(); }
private:
	using data_t = std::unique_ptr < BYTE[] > ;

	std::fstream _stream;
	data_t _data;
	uint32_t _current_packet_pointer;
	uint32_t _read_data_block_size_b;
	volatile bool _read_error;
	volatile bool _stream_end;
	volatile bool _reading;

	std::thread _reading_thread;
	std::mutex _mtx;
	std::condition_variable _cv;
	std::mutex _buffer_mtx;
	data_t _buffer;

	handle_holder read_event;

	void reading_loop()
	{
		while (!_read_error && !_stream_end)
		{
			_reading = true;
			read_next_block_data();
			_buffer_mtx.lock();
			_data.swap(_buffer);
			_reading = false;
			_buffer_mtx.unlock();
			{
				sync_helpers::wait(read_event);
			}
		}
	}

	test_synchro_packet_t gen_next_syncro_packet()
	{
		if (_stream_end || _read_error)
			return g_synchro_packet;

		std::unique_lock<std::mutex> lck(_buffer_mtx);

		auto packet = reinterpret_cast<test_synchro_packet_t*>(_data.get() + _current_packet_pointer);
		if (packet == nullptr)
			return g_synchro_packet;
		_current_packet_pointer += sizeof(test_synchro_packet_t);
		if (_current_packet_pointer > _read_data_block_size_b - 1024){
			sync_helpers::set_event(read_event);
		}
		return *packet;
	}



	void read_next_block_data()
	{
		_stream.read(reinterpret_cast<char*>(_buffer.get()), _read_data_block_size_b);

		auto stream_state = _stream.rdstate();
		if (stream_state == std::ios::failbit || stream_state == std::ios::badbit)
		{
			_read_error = true;
		}
		if (stream_state & std::ios::eofbit)
		{
			_stream_end = true;
		}

	}

};



test_synchro_packet_t gen_next_syncro_packet(POSITON_STRATEGY strategy)
{

	g_synchro_packet.timestamp++;

	if (strategy == POSITON_STRATEGY::UNCHANGE)
		return g_synchro_packet;

	if (strategy == POSITON_STRATEGY::INCREASE){
		g_synchro_packet.counter++;
		g_synchro_packet.speed++;
	}

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


const unsigned int g_event_packet_max_index = 1;
static unsigned int g_event_packet_index = 1;
test_event_packet_t gen_next_event_packet()
{
	std::string filename("../../packets/event");
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


class IncrementEventsStrategy
{
	int _cur_index;
public:
	IncrementEventsStrategy() :_cur_index(1){}
	std::string operator()()
	{
		return std::to_string(_cur_index++);
	}
};


template<typename TEventsStrategy = IncrementEventsStrategy>
class event_packet_generator
{
	using _TEvS = TEventsStrategy;
public:
	event_packet_generator(const std::string& file_name_pattern, const _TEvS &event_policy) :
		_file_name_pattern(file_name_pattern),
		_next_event(event_policy)
	{
		get_next_file_name();
	}

	event_packet_generator(const event_packet_generator&) = delete;

	~event_packet_generator(){}

	test_event_packet_t operator()() { return this->gen_next_event_packet(); }
private:
	_TEvS _next_event;

	std::string _current_file_name;
	std::string _file_name_pattern;

	test_event_packet_t gen_next_event_packet()
	{

		get_next_file_name();

		std::string test_packet(
			(std::istreambuf_iterator<char>(
			*(std::unique_ptr<std::ifstream>(
			new std::ifstream(_current_file_name)
			)).get()
			)),
			std::istreambuf_iterator<char>()
			);

		test_event_packet_t packet = { 0 };
		std::memcpy(packet.data, test_packet.c_str(), std::min(SIZE_TEST_EVENT_PACKET, (int)test_packet.size()));

		return packet;
	}


	void get_next_file_name()
	{
		_current_file_name = _file_name_pattern + _next_event() + ".xml";
	}

};



class StartStopEventsStrategy
{
	std::chrono::seconds _stop_event_span;
	std::chrono::steady_clock::time_point deadline;
	bool _not_started;
	bool _stoped;

public:
	StartStopEventsStrategy(size_t stop_delay) :
		_stop_event_span(stop_delay),
		_not_started(true),
		_stoped(false)
	{
	}
	std::string operator()()
	{
		if (_not_started)
		{
			_not_started = false;
			deadline = std::chrono::steady_clock::now();
		}
		else
		{
			if (!_stoped)
			{
				if (std::chrono::steady_clock::now() - deadline > _stop_event_span)
				{
					_stoped = true;
				}
				else
					return "start";

			}

			return "stop";

		}

		return "start";
	}
};









using connection_address = std::pair<std::string, unsigned short>;
void
start(
	const connection_address& sync_addr, unsigned int sync_delay,
	const connection_address& events_addr, unsigned int events_delay,
	POSITON_STRATEGY position_strategy
)
{
	scoped_WSA WSA_startup;

	test_udp_server synchro_server(sync_addr.first.c_str(), sync_addr.second);
	test_udp_server events_server(events_addr.first.c_str(), events_addr.second);

	std::thread syncro_server_thread([&synchro_server, sync_delay, position_strategy]()
									{
										auto gen = std::make_unique<sync_packet_generator_from_file>("../../packets/Synchro.src");
										auto &&func = std::bind(&sync_packet_generator_from_file::operator(), gen.get());

										synchro_server.start_server<test_synchro_packet_t>(
																func,
																0,
																sync_delay
																); 
									});
	std::thread events_server_thread([&events_server, events_delay]()
	{
		StartStopEventsStrategy events_policy(40);
		auto gen = std::make_unique<event_packet_generator<StartStopEventsStrategy>>("../../packets/event_", events_policy);
		auto &&func = std::bind(&event_packet_generator<StartStopEventsStrategy>::operator(), gen.get());

		events_server.start_server<test_event_packet_t>(
			func,
			0,
			events_delay
			);
	});

	_getch();

	synchro_server.stop_server();
	events_server.stop_server();

	if (syncro_server_thread.joinable())
		syncro_server_thread.join();
	if (events_server_thread.joinable())
		events_server_thread.join();

}


template <typename TIter>
std::map<std::wstring, std::wstring> parse_parameters(TIter begin, TIter end)
{
	std::map<std::wstring, std::wstring> parameters;
	for (auto iter = begin; iter != end; ++iter)
	{
		const std::wstring param_name(*iter);

		if (++iter == end)
		{
			throw std::invalid_argument("Invalid parameters list.");
		}

		const std::wstring param_value(*iter);
		printf("param: %ws, value: %ws\n", param_name.c_str(), param_value.c_str());

		parameters.emplace(param_name, param_value);
	}

	return parameters;
}

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		int args_num = 0;    // Default is no line numbers.

		const int min_num_of_args = 15;
		const int max_num_of_args = min_num_of_args;

		args_num = argc - 1;

		if (args_num != 0 && args_num != 2 && !(argc >= min_num_of_args && argc <= max_num_of_args))
		{
			const std::wstring exe_name_w(argv[0]);
			const std::string exe_name(exe_name_w.cbegin(), exe_name_w.cend());
			std::cout << "Usage:" << std::endl;
			std::cout << exe_name << " sync_ip ip sync_port port sync_delay delay events_ip ip events_port port events_delay delay" << std::endl;
			std::cout << "sync_ip ip - ip address synchronization packet destination." << std::endl;
			std::cout << "sync_port port - ip port synchronization packet destination." << std::endl;
			std::cout << "sync_delay delay - delay synchronization packet generation." << std::endl;
			std::cout << "events_ip ip - ip address events packet destination." << std::endl;
			std::cout << "events_port port - ip port events packet destination." << std::endl;
			std::cout << "events_delay delay - delay events packet generation." << std::endl;
			std::cout << "position_strategy number(0,1,2) - (0: position decreases 1: position no changing, 2: position increases)." << std::endl;
			return -1;
		}

		std::cout << "Count arguments: " << args_num << std::endl;

		std::wstring w_sync_ip = L"224.5.6.1";
		std::wstring w_sync_port = L"32300";
		std::wstring w_sync_delay = L"10";
		std::wstring w_events_ip = L"224.5.6.98";
		std::wstring w_events_port = L"32298";
		std::wstring w_events_delay = L"1000";
		std::wstring w_position_strategy = L"1";

		if (args_num > 0)
		{
			if (args_num != 2 && (argc < min_num_of_args || argc > max_num_of_args))
			{
				throw std::invalid_argument("No parameters were passed.");
			}

			auto const parameters = parse_parameters(
				&argv[1],
				&argv[argc]);

			if (args_num > 2){
				w_sync_ip = parameters.at(L"sync_ip");
				w_sync_port = parameters.at(L"sync_port");
				w_sync_delay = parameters.at(L"sync_delay");
				w_events_ip = parameters.at(L"events_ip");
				w_events_port = parameters.at(L"events_port");
				w_events_delay = parameters.at(L"events_delay");
			}

			w_position_strategy = parameters.at(L"position_strategy");
		}
		const std::string sync_ip(w_sync_ip.cbegin(), w_sync_ip.cend());
		const std::string events_ip(w_events_ip.cbegin(), w_events_ip.cend());
		const auto sync_port = (unsigned short)std::stoul(w_sync_port);
		const auto events_port = (unsigned short)std::stoul(w_events_port);
		const auto sync_delay = std::stoul(w_sync_delay);
		const auto events_delay = std::stoul(w_events_delay);
		auto positon_strategy = static_cast<POSITON_STRATEGY>(std::stoul(w_position_strategy));

		if (positon_strategy > POSITON_STRATEGY::INCREASE){
			positon_strategy = POSITON_STRATEGY::INCREASE;
		}

		connection_address sync_addr{ sync_ip, sync_port };
		connection_address events_addr{ events_ip, events_port };

		start(sync_addr, sync_delay, events_addr, events_delay, positon_strategy);

	}
	catch (const std::exception & exc)
	{
		std::cout << "Error: " << exc.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cout << "Unknown exception." << std::endl;
		return -1;
	}


	return 0;
}


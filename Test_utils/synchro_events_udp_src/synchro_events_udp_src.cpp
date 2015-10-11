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
#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>
#include <future>



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


using sync_packet_notify_func_t = std::function<void(const test_synchro_packet_t&)>;

class sync_packet_generator_from_file
{
public:
	sync_packet_generator_from_file(const std::string& file_name, const sync_packet_notify_func_t& packet_notify_func) :
		_packet_notify_func(packet_notify_func),
		_current_packet_pointer(0),
		_current_size_b(0),
		_new_data_size_b(0),
		_read_data_block_size_b(1 * 1024),
		_read_error(false),
		_stream_end(false),
		_reading(false),
		_new_data(false),
		_last_data(false)
	{

		_stream.open(file_name, std::ios::in | std::ios::binary);
		if (_stream.rdstate() == std::ios::failbit)
		{
			auto error = ::GetLastError();
			throw std::invalid_argument("Can't open stream: '" + file_name + "' Error: " + std::to_string(error));
		}

		_buffer = std::make_unique<BYTE[]>(_read_data_block_size_b);
		_data = std::make_unique<BYTE[]>(_read_data_block_size_b);

		auto event = sync_helpers::create_basic_event_object(false);
		read_event.swap(event);

		ZeroMemory(_data.get(), sizeof(test_synchro_packet_t)*10);
		std::thread _thread([this](){ this->reading_loop(); });

		_reading_thread.swap(_thread);

	}

	sync_packet_generator_from_file& operator=(sync_packet_generator_from_file&) = delete;

	sync_packet_generator_from_file(const sync_packet_generator_from_file&) = delete;
	sync_packet_generator_from_file(sync_packet_generator_from_file&& other)
	{
		
		_reading_thread.swap(other._reading_thread);
		_stream.swap(other._stream);
	}

	~sync_packet_generator_from_file()
	{
		_stream_end = true;
		_last_data = true;
		sync_helpers::set_event(read_event);

		if (_reading_thread.joinable())
			_reading_thread.join();

		//_stream.close();
	}

	bool operator()(test_synchro_packet_t& data_packet) { return this->gen_next_syncro_packet(data_packet); }
private:
	using data_t = std::unique_ptr < BYTE[] > ;
	sync_packet_notify_func_t _packet_notify_func;

	std::fstream _stream;
	data_t _data;
	uint32_t _current_packet_pointer;
	uint32_t _read_data_block_size_b;
	uint32_t _current_size_b;
	uint32_t _new_data_size_b;
	volatile bool _read_error;
	volatile bool _stream_end;
	volatile bool _reading;
	volatile bool _new_data;
	volatile bool _last_data;


	std::thread _reading_thread;
	std::mutex _buffer_mtx;
	data_t _buffer;

	handle_holder read_event;

	void reading_loop()
	{
		while (!_read_error && !_last_data)
		{
			_reading = true;
			read_next_block_data();
			_new_data = true;
			_reading = false;
			sync_helpers::wait(read_event);
		}
	}

	bool gen_next_syncro_packet(test_synchro_packet_t& data_packet)
	{
		if (_stream_end || _read_error)
			return false;

		if (_current_size_b == 0 && !_new_data)
			return false;

		if (_current_size_b > 0 && _current_packet_pointer >= _current_size_b){
			return false;
		}

		auto packet = reinterpret_cast<test_synchro_packet_t*>(_data.get() + _current_packet_pointer);
		if (packet == nullptr)
			return false;

		data_packet = *packet;

		if (_packet_notify_func)
			_packet_notify_func(data_packet);

		_current_packet_pointer += sizeof(test_synchro_packet_t);

		if (_current_packet_pointer >= _current_size_b && _new_data)
		{
			_current_packet_pointer = 0;
			_buffer_mtx.lock();
			_data.swap(_buffer);
			_new_data = false;
			_current_size_b = _new_data_size_b;
			_new_data_size_b = 0;
			_buffer_mtx.unlock();
		}

		if (_current_packet_pointer >= _current_size_b && _last_data){
			_stream_end = true;
		}

		if (!_new_data && !_last_data){
			sync_helpers::set_event(read_event);
		}

		return true;
	}



	void read_next_block_data()
	{
		_buffer_mtx.lock();

		_stream.read(reinterpret_cast<char*>(_buffer.get()), _read_data_block_size_b);

		auto stream_state = _stream.rdstate();
		if (stream_state == std::ios::failbit || stream_state == std::ios::badbit)
		{
			_read_error = true;
		}
		_new_data_size_b = _read_data_block_size_b;
		if (stream_state & std::ios::eofbit)
		{
			auto last_data_count = _stream.gcount();
			_new_data_size_b = static_cast<uint32_t>(last_data_count);
			
			_last_data = true;
		}
		_buffer_mtx.unlock();

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

	void sync_packet_callback(const test_synchro_packet_t& packet)
	{
		_next_event.sync_packet_callback(packet);
	}

	bool operator()(test_event_packet_t& data_packet) { return this->gen_next_event_packet(data_packet); }
private:
	_TEvS _next_event;

	std::string _current_file_name;
	std::string _file_name_pattern;

	bool gen_next_event_packet(test_event_packet_t& data_packet)
	{
		if(!get_next_file_name())
			return false;

		data_packet = { 0 };
		std::string test_packet(
			(std::istreambuf_iterator<char>(
			*(std::unique_ptr<std::ifstream>(
			new std::ifstream(_current_file_name)
			)).get()
			)),
			std::istreambuf_iterator<char>()
			);

		std::memcpy(data_packet.data, test_packet.c_str(), std::min(SIZE_TEST_EVENT_PACKET, (int)test_packet.size()));

		return true;
	}


	bool get_next_file_name()
	{
		std::string file_name_part;
		if (!_next_event(file_name_part))
			return false;
		_current_file_name = _file_name_pattern + file_name_part + ".log";
		return true;
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
	void sync_packet_callback(const test_synchro_packet_t &)
	{
	}

	bool operator()(std::string& file_name)
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
					file_name = "start";

			}

			file_name = "stop";

		}

		file_name = "start";
		return true;
	}
};


class CustomEventsStrategy
{

	std::chrono::seconds _stop_event_span;
	std::chrono::steady_clock::time_point deadline;
	bool _not_started;
	bool _stoped;


	struct event_info{
		counter_t counter;
		int index;
	};

	std::vector<event_info> events_info;
	counter_t _current_counter;
	counter_t _next_packet_counter;
	int _last_file_index;
	int _current_file_index;
	int _current_read_file_index;
	int _last_readed_file_index;

	std::string _file_name_pattern;

	int _retrieved_counter;

	int _current_index_index;

public:
	CustomEventsStrategy(const std::string& file_name_pattern, int last_file_index) :
		_current_file_index(-1),
		_last_file_index(last_file_index),
		_file_name_pattern(file_name_pattern),
		_not_started(true),
		_stoped(false),
		_next_packet_counter(-1),
		_retrieved_counter(0),
		_current_index_index(0),
		_current_read_file_index(-1),
		_last_readed_file_index(-1)
	{
		get_next_event_data();
	}


	void sync_packet_callback(const test_synchro_packet_t& packet)
	{
		if (packet.counter >= _next_packet_counter)
		{
			get_next_event_data();
		}

		_current_counter = packet.counter;
	}

	void get_next_event_data()
	{
		if (_current_read_file_index == -2 || _current_read_file_index == _last_file_index)
			return;

		if (_current_read_file_index == -1){
			_current_read_file_index++;
		}
		else{
			if (_current_read_file_index == 1)
				_current_file_index = 0;
		}

		std::string test_packet(
			(std::istreambuf_iterator<char>(
			*(std::unique_ptr<std::ifstream>(
			new std::ifstream(_file_name_pattern + std::to_string(_current_read_file_index++) + ".log")
			)).get()
			)),
			std::istreambuf_iterator<char>()
			);

		if (test_packet.empty()){
			_current_read_file_index = -2;
			return;
		}

		try{
			auto packet =
				position_detector::parce_packet_from_message<position_detector::events::event_packet_ptr_t>(
				(const BYTE *)test_packet.c_str(),
				(unsigned int)test_packet.size());

			_next_packet_counter = packet->counter;
			_last_readed_file_index = _current_read_file_index-1;
			events_info.push_back({ _next_packet_counter, _last_readed_file_index });
			std::cout << "file parsed, index: " << _last_readed_file_index << " counter: "<<_next_packet_counter << std::endl;

		}
		catch (const position_detector::deserialization_error&)
		{
			_next_packet_counter = 0;
		}

	}

	bool operator()(std::string& file_name)
	{
		if (_current_file_index == -1 || _current_file_index == -2)
			return false;

		if (_retrieved_counter > 4)
		{
			if (_current_file_index == _last_file_index ||
				(_current_read_file_index == -2 &&
				_last_readed_file_index == _current_file_index)
				)
			{
				std::cout << "reached max file index: " << _current_file_index << std::endl;
				_current_file_index = -2;
				return false;
			}
			if (_current_file_index == _last_readed_file_index){
				return false;
			}

			_current_file_index++;
			_retrieved_counter = 0;

			
		}

		const auto & event_info = events_info[_current_file_index];
		if (event_info.counter > _current_counter)
			return false;

		if (_retrieved_counter == 0)
			std::cout << "current file index: " << _current_file_index << std::endl;
		_retrieved_counter++;
		file_name = std::to_string(_current_file_index);
		return true;
	}
};




using connection_address = std::pair<std::string, unsigned short>;
void
start(
	const connection_address& sync_addr, unsigned int sync_delay,
	const connection_address& events_addr, unsigned int events_delay,
	const std::string& sync_file_name,
	const std::string& events_name,
	uint32_t events_name_last_indx
)
{
	scoped_WSA WSA_startup;

	test_udp_server synchro_server(sync_addr.first.c_str(), sync_addr.second);
	test_udp_server events_server(events_addr.first.c_str(), events_addr.second);

	CustomEventsStrategy events_policy("../../packets/" + events_name, events_name_last_indx);
	auto events_generator_ptr = std::make_unique<event_packet_generator<CustomEventsStrategy>>("../../packets/" + events_name, events_policy);

	std::promise<void> exception;
	auto exception_future = exception.get_future();
	bool has_error = false;
	std::string error_str;

	std::thread exception_thread([&exception_future,&has_error,&error_str]()
	{
		try{
			exception_future.get();
		}
		catch (const std::invalid_argument& exc)
		{
			has_error = true;
			error_str = exc.what();
		}
		catch (...)
		{
			has_error = true;
		}

	});


		std::thread events_server_thread([&events_server, events_delay, &events_generator_ptr, &exception]()
		{
			auto &&func = std::bind(&event_packet_generator<CustomEventsStrategy>::operator(), events_generator_ptr.get(), std::placeholders::_1);

			try{
				events_server.start_server<test_event_packet_t>(
					func,
					0,
					events_delay
					);
			}
			catch (...)
			{
				exception.set_exception(std::current_exception());
			}
		});

		auto sync_connector = std::bind(&event_packet_generator<CustomEventsStrategy>::sync_packet_callback, events_generator_ptr.get(), std::placeholders::_1);

		std::unique_ptr<sync_packet_generator_from_file> gen;
		try{
			gen = std::make_unique<sync_packet_generator_from_file>("../../packets/" + sync_file_name, sync_connector);
		}
		catch (const std::bad_alloc& exc){

			events_server.stop_server();
			if (events_server_thread.joinable())
				events_server_thread.join();

			std::cout << "Error: " << exc.what() << std::endl;

			if (exception_thread.joinable()){
				exception.set_value();
				exception_thread.join();
			}
			return;
		}
		catch (const std::invalid_argument& exc){

			events_server.stop_server();
			if (events_server_thread.joinable())
				events_server_thread.join();

			std::cout << "Error: " << exc.what() << std::endl;

			if (exception_thread.joinable()){
				exception.set_value();
				exception_thread.join();
			}

			return;
		}

		std::thread syncro_server_thread([&synchro_server, sync_delay, &sync_file_name, &sync_connector, &gen, &exception]()
		{
			try{
				auto &&func = std::bind(&sync_packet_generator_from_file::operator(), gen.get(), std::placeholders::_1);

				synchro_server.start_server<test_synchro_packet_t>(
					func,
					0,
					sync_delay
					);
			}
			catch (...)
			{
				exception.set_exception(std::current_exception());
			}
		});

		if (has_error){
			std::cout << "Error: " << error_str << std::endl;
		}
		else
			_getch();

		synchro_server.stop_server();
	if (syncro_server_thread.joinable())
		syncro_server_thread.join();

	events_server.stop_server();
	if (events_server_thread.joinable())
		events_server_thread.join();


	if (exception_thread.joinable()){
		exception.set_value();
		exception_thread.join();
	}


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

		const int min_num_of_args = 19;
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
			std::cout << "sync_name - synchro pakets file name" << std::endl;
			std::cout << "events_name - events pakets file name pattern" << std::endl;
			std::cout << "events_name_last_indx - events pakets file name last index" << std::endl;
			return -1;
		}

		std::cout << "Count arguments: " << args_num << std::endl;

		std::wstring w_sync_ip = L"224.5.6.1";
		std::wstring w_sync_port = L"32300";
		std::wstring w_sync_delay = L"10";
		std::wstring w_events_ip = L"224.5.6.98";
		std::wstring w_events_port = L"32298";
		std::wstring w_events_delay = L"1000";
		std::wstring w_sync_file_name = L"Moscow/test/2/Synchro.src";
		std::wstring w_events_file_name = L"Moscow/test/2/event_";
		std::wstring w_events_file_last_indx = L"1000";

		if (args_num > 0)
		{
			if (argc < min_num_of_args || argc > max_num_of_args)
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
				w_sync_file_name = parameters.at(L"sync_name");
				w_events_file_name = parameters.at(L"events_name");
				w_events_file_last_indx = parameters.at(L"events_name_last_indx");
			}

		}

		std::cout << "Actual parameters:" << std::endl;
		std::cout << "sync_ip ip: " << std::string(w_sync_ip.cbegin(), w_sync_ip.cend()) << std::endl;
		std::cout << "sync_port: " << std::string(w_sync_port.cbegin(), w_sync_port.cend()) << std::endl;
		std::cout << "sync_delay:: " << std::string(w_sync_delay.cbegin(), w_sync_delay.cend()) << std::endl;
		std::cout << "events_ip ip: " << std::string(w_events_ip.cbegin(), w_events_ip.cend()) << std::endl;
		std::cout << "events_port: " << std::string(w_events_port.cbegin(), w_events_port.cend()) << std::endl;
		std::cout << "events_delay delay: " << std::string(w_events_delay.cbegin(), w_events_delay.cend()) << std::endl;
		std::cout << "sync packets file name: " << std::string(w_sync_file_name.cbegin(), w_sync_file_name.cend()) << std::endl;
		std::cout << "events packets file name: " << std::string(w_events_file_name.cbegin(), w_events_file_name.cend()) << std::endl;
		std::cout << "events packets file name last index: " << std::string(w_events_file_last_indx.cbegin(), w_events_file_last_indx.cend()) << std::endl;


		const std::string sync_ip(w_sync_ip.cbegin(), w_sync_ip.cend());
		const std::string events_ip(w_events_ip.cbegin(), w_events_ip.cend());
		const std::string events_name(w_events_file_name.cbegin(), w_events_file_name.cend());
		const std::string sync_name(w_sync_file_name.cbegin(), w_sync_file_name.cend());


		const auto sync_port = (unsigned short)std::stoul(w_sync_port);
		const auto events_port = (unsigned short)std::stoul(w_events_port);
		const auto sync_delay = std::stoul(w_sync_delay);
		const auto events_delay = std::stoul(w_events_delay);
		const auto events_file_last_indx = std::stoul(w_events_file_last_indx);

		connection_address sync_addr{ sync_ip, sync_port };
		connection_address events_addr{ events_ip, events_port };

		start(sync_addr, sync_delay, events_addr, events_delay, sync_name, events_name, events_file_last_indx);

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


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

#include <common\string_utils.h>
#include <common\path_helpers.h>
#include <common\on_exit.h>

#include <list>
#include <fcntl.h>
#include <io.h>


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
	sync_packet_generator_from_file(const std::wstring& file_name, const sync_packet_notify_func_t& packet_notify_func) :
		_packet_notify_func(packet_notify_func),
		_current_packet_pointer(0),
		_current_size_b(0),
		_new_data_size_b(0),
		_read_data_block_size_b(10 * 1024 * 1024),
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
			throw std::invalid_argument("Can't open stream: '" + string_utils::convert_wchar_to_utf8(file_name) + "' Error: " + std::to_string(error));
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


static int max_size_event_packet = SIZE_TEST_EVENT_PACKET;

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
	event_packet_generator(const std::wstring& file_name_pattern, const std::wstring& file_name_extention, const _TEvS &event_policy) :
		_file_name_pattern(file_name_pattern),
		_file_name_extention(file_name_extention),
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

	std::wstring _current_file_name;
	std::wstring _file_name_pattern;
	std::wstring _file_name_extention;
	

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
		std::wstring file_name_part;
		if (!_next_event(file_name_part))
			return false;
		_current_file_name = _file_name_pattern + file_name_part + _file_name_extention;
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

	std::wstring _file_name_pattern;
	std::wstring _file_name_extention;
	

	int _retrieved_counter;

	int _current_index_index;

public:
	CustomEventsStrategy(const std::wstring& file_name_pattern, const std::wstring& file_name_extention, int last_file_index) :
		_current_file_index(-1),
		_last_file_index(last_file_index),
		_file_name_pattern(file_name_pattern),
		_file_name_extention(file_name_extention),
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
			new std::ifstream(_file_name_pattern + std::to_wstring(_current_read_file_index++) + _file_name_extention)
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

	bool operator()(std::wstring& file_name)
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
		file_name = std::to_wstring(_current_file_index);
		return true;
	}
};



template<typename TEventsStrategy = FileEventsStrategy>
class event_packet_generator2
{
	using _TEvS = TEventsStrategy;
public:
	event_packet_generator2(_TEvS &&event_policy) :
		_next_event(event_policy)
	{
	}

	event_packet_generator2(const event_packet_generator2&) = delete;

	~event_packet_generator2(){}

	void sync_packet_callback(const test_synchro_packet_t& packet)
	{
		_next_event.sync_packet_callback(packet);
	}

	bool operator()(test_event_packet_t& data_packet) { return this->gen_next_event_packet(data_packet); }
private:
	_TEvS _next_event;

	bool gen_next_event_packet(test_event_packet_t& data_packet)
	{
		return _next_event(data_packet);
	}
};


class FileEventsStrategy
{
	struct event_info
	{
		counter_t counter;
		std::string event_data;
		position_detector::events::event_packet_ptr_t event;
	};

	std::vector<event_info> events;
	int _current_index;
	counter_t _current_counter;
public:
	FileEventsStrategy(const std::wstring& events_src_file_name) :
		_current_index(0),
		_current_counter(0)
	{

		std::ifstream stream;
		//stream.open(events_src_file_name, std::ios::binary);
		stream.open(events_src_file_name);
		read_events_data(stream);

		std::wcout << L"Found " << events.size() <<
			L" events in the events source file '" << events_src_file_name << L"'" << std::endl;

	}

	void read_events_data(std::ifstream& stream)
	{

		std::string stream_data(
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>()
			);

		if (stream_data.empty()){
			return;
		}

		int data_index = 0;
		auto data = stream_data.c_str();
		int data_size = stream_data.size();

		for (;;)
		{
			auto res = skip_nulls_data(data, data_index,data_size);
			if (!res)
				return;

			std::string test_packet((data + data_index));
			if (test_packet.empty()){
				return;
			}

			data_index += test_packet.size() + 1;

			if (max_size_event_packet < (int)test_packet.size())
				continue;

			try{
				auto packet =
					position_detector::parce_packet_from_message<position_detector::events::event_packet_ptr_t>(
					(const BYTE *)test_packet.c_str(),
					(unsigned int)test_packet.size());

				events.push_back({ packet->counter, test_packet, packet});

			}
			catch (const position_detector::deserialization_error& exc)
			{
				auto err = exc.what();
			}
		}
	}

	bool skip_nulls_data(const char* data, int & index, int data_size)
	{
		while (index < data_size)
		{
			char buffer = *(data + index);

			if (buffer != 0){

				return true;
			}
			index++;
		}

		return false;
	}



	void sync_packet_callback(const test_synchro_packet_t& packet)
	{
		//if (packet.counter >= _next_packet_counter)
		//{
		//	get_next_event_data();
		//}

		_current_counter = packet.counter;
	}

	bool operator()(test_event_packet_t& data_packet)
	{
		if (events.empty())
			return false;

		if (_current_index == events.size()){
			std::cout << "No more events." << std::endl;
			events.clear();
			return false;
		}

		const auto & event_info = events[_current_index];
		if (event_info.counter > _current_counter)
			return false;

		counter_t next_counter = 0;
		std::wstring next_event_name;
		if (_current_index + 1 < (int)events.size()){
			next_counter = events[_current_index + 1].counter;
			next_event_name = events[_current_index + 1].event->event_name;
		}

		std::memcpy(&data_packet, event_info.event_data.c_str(), event_info.event_data.size());

		std::wcout << event_info.event.get() << std::endl;
		std::wcout << L"current event index: " << _current_index 	<< 
			L" counter: " << event_info.counter << L" event name: " << event_info.event->event_name <<
			L" next counter: " << next_counter << L" event name: " << next_event_name << 
			std::endl << std::endl;

		_current_index++;
		return true;
	}
};



using connection_address = std::pair<std::wstring, unsigned short>;

void
start(
	const connection_address& sync_addr, unsigned int sync_delay,
	const connection_address& events_addr, unsigned int events_delay,
	const std::wstring& sync_file_name,
	const std::wstring& events_name
)
{
	scoped_WSA WSA_startup;

	test_udp_server synchro_server(sync_addr.first.c_str(), sync_addr.second);
	test_udp_server events_server(events_addr.first.c_str(), events_addr.second);

	event_packet_generator2<FileEventsStrategy> events_generator({ events_name });

	std::promise<void> exception;
	auto exception_future = exception.get_future();
	std::string error_str;

	std::thread exception_thread([&]()
	{
		try
		{
			exception_future.get();
		}
		catch (const std::invalid_argument& exc)
		{
			error_str = exc.what();
		}
		catch (...)
		{
			error_str = "Unknown exception";
		}
	});

	ON_EXIT_OF_SCOPE([&]
	{
		if (exception_thread.joinable()){
			exception.set_value();
			exception_thread.join();
		}
	});


	std::thread events_server_thread([&]()
	{
		auto func = std::bind(&event_packet_generator2<FileEventsStrategy>::operator(), &events_generator, std::placeholders::_1);

		try
		{
			events_server.start_server<test_event_packet_t>(func, 0, events_delay);
		}
		catch (...)
		{
			exception.set_exception(std::current_exception());
		}
	});

	ON_EXIT_OF_SCOPE([&]
	{
		events_server.stop_server();
		if (events_server_thread.joinable())
			events_server_thread.join();
	});

	try
	{
		sync_packet_generator_from_file gen(sync_file_name, std::bind(&event_packet_generator2<FileEventsStrategy>::sync_packet_callback, &events_generator, std::placeholders::_1));

		std::thread syncro_server_thread([&]()
		{
			try
			{
				synchro_server.start_server<test_synchro_packet_t>(std::bind(&sync_packet_generator_from_file::operator(), &gen, std::placeholders::_1), 0, sync_delay);
			}
			catch (...)
			{
				exception.set_exception(std::current_exception());
			}
		});

		ON_EXIT_OF_SCOPE([&]
		{
			synchro_server.stop_server();
			if (syncro_server_thread.joinable())
				syncro_server_thread.join();
		});


		if (error_str.empty())
			_getch();
	}
	catch (const std::bad_alloc& exc)
	{
		error_str = exc.what();
	}
	catch (const std::invalid_argument& exc)
	{
		error_str = exc.what();
	}

	if (!error_str.empty())
	{
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << "Error: " << error_str << std::endl;
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
	_setmode(_fileno(stdout), _O_TEXT);

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
			std::cout << "events_name - events pakets file name" << std::endl;
			return -1;
		}

		std::cout << "Count arguments: " << args_num << std::endl;


		const std::wstring counters_filename = L"Synchro.src";
		const std::wstring events_filename = L"Events.src";

		std::wstring w_sync_ip = L"224.5.6.1";
		std::wstring w_sync_port = L"32300";
		std::wstring w_sync_delay = L"1";
		std::wstring w_events_ip = L"224.5.6.98";
		std::wstring w_events_port = L"32298";
		std::wstring w_events_delay = L"1000";
		std::wstring w_sync_file_name = L"../../packets/Moscow/test/6/" + counters_filename;
		std::wstring w_events_file_name = L"../../packets/Moscow/test/6/" + events_filename;

		if (args_num > 0)
		{
			if (args_num != 2 && (argc < min_num_of_args || argc > max_num_of_args))
			{
				throw std::invalid_argument("No parameters were passed.");
			}

			auto const parameters = parse_parameters(
				&argv[1],
				&argv[argc]);

			if (args_num == 2){

				auto w_events_dir_path = parameters.at(L"p");

				w_sync_file_name = path_helpers::concatenate_paths(w_events_dir_path, counters_filename);
				w_events_file_name = path_helpers::concatenate_paths(w_events_dir_path, events_filename);
			}
			else{
				w_sync_ip = parameters.at(L"sync_ip");
				w_sync_port = parameters.at(L"sync_port");
				w_sync_delay = parameters.at(L"sync_delay");
				w_events_ip = parameters.at(L"events_ip");
				w_events_port = parameters.at(L"events_port");
				w_events_delay = parameters.at(L"events_delay");
				w_sync_file_name = parameters.at(L"sync_name");
				w_events_file_name = parameters.at(L"events_name");
			}

		}

		std::wcout << "Actual parameters:" << std::endl;
		std::wcout << "sync_ip ip: " << w_sync_ip << std::endl;
		std::wcout << "sync_port: " << w_sync_port << std::endl;
		std::wcout << "sync_delay:: " << w_sync_delay << std::endl;
		std::wcout << "events_ip ip: " << w_events_ip << std::endl;
		std::wcout << "events_port: " << w_events_port << std::endl;
		std::wcout << "events_delay delay: " << w_events_delay << std::endl;
		std::wcout << "sync packets file name: " << w_sync_file_name << std::endl;
		std::wcout << "events packets file name: " << w_events_file_name << std::endl;


		const auto sync_port = (unsigned short)std::stoul(w_sync_port);
		const auto events_port = (unsigned short)std::stoul(w_events_port);
		const auto sync_delay = std::stoul(w_sync_delay);
		const auto events_delay = std::stoul(w_events_delay);

		connection_address sync_addr{ w_sync_ip, sync_port };
		connection_address events_addr{ w_events_ip, events_port };

		_setmode(_fileno(stdout), _O_U16TEXT);

		start(sync_addr, sync_delay, 
			events_addr, events_delay, 
			w_sync_file_name, 
			w_events_file_name
			);

	}
	catch (const std::exception & exc)
	{
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << "Error: " << exc.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << "Unknown exception." << std::endl;
		return -1;
	}


	return 0;
}


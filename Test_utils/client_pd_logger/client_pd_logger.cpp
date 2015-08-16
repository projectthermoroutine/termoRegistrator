#include <common\sync_helpers.h>
#include <common\socket_holder.h>

#include "client_pd_dispatcher\position_detector_packets_manager.h"
#include "client_pd_dispatcher\client_pd_dispatcher.h"
#include "client_pd_dispatcher\position_detector_dispatcher.h"
#include "client_pd_dispatcher\server_proxy_pd_connector.h"
#include <map>
#include <vector>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <conio.h>

using namespace position_detector;
using namespace position_detector::synchronization;
using namespace position_detector::events;

template<typename TMessage>
using append_packet_func_t = std::function<void(const TMessage &)>;

using append_event_packet_func_t = append_packet_func_t<events::event_packet_ptr_t>;
using append_synchro_packet_func_t = append_packet_func_t<synchronization::sync_packet_ptr_t>;


std::ofstream file_Events;
std::ofstream file_Synchro;


struct packets_stream
{
	static void dispatch_message_Events(const BYTE * message, unsigned int message_size)
	{
		
		file_Events.write((char*)message, message_size);

	}

	static void dispatch_message_Synchro(const BYTE * message, unsigned int message_size)
	{

		file_Synchro.write((char*)message, message_size);

	}
};


std::string g_exception_string;
handle_holder g_stop_event;

void notify_dispatch_error(const std::string& msg)
{
	g_exception_string = msg;
	sync_helpers::set_event(g_stop_event);
}

void active_state_func(bool)
{
}

void 
start(
const connection_address& sync_addr, 
const connection_address& events_addr
)
{
	time_t t = time(0);   // get time now
	struct tm  now ;
	localtime_s(&now, &t);
	
	std::string hour_str = std::to_string(now.tm_hour);
	std::string min_str = std::to_string(now.tm_min);

	std::string last_part_name = "_" + hour_str + "_" + min_str + "_" + std::to_string(now.tm_sec) + ".log";

	// create filename for Events
	std::string fileName_Events = "Events" + last_part_name;
	file_Events.open(fileName_Events, std::ios::binary);

	// create filename for Synchro
	std::string fileName_Synchro = "Synchro" + last_part_name;
	file_Synchro.open(fileName_Synchro, std::ios::binary);

	handle_holder closing_event(sync_helpers::create_basic_event_object(true));

	process_packets_factory factory;
	factory.create_process_synchro_packet_func = create_process_packet_func_t([]()->std::function<void(const BYTE *, unsigned int)>{return &packets_stream::dispatch_message_Synchro; });
	factory.create_process_event_packet_func = create_process_packet_func_t([]()->std::function<void(const BYTE *, unsigned int)>{return &packets_stream::dispatch_message_Events; });


	//position_detector_dispatcher::active_state_callback_func_t active_state_callback_func([](bool){});

	server_proxy_pd_connector connector(notify_dispatch_error, notify_dispatch_error, notify_dispatch_error);

	std::vector<std::string> config{ "pd_ip", sync_addr.ip, "pd_i_ip", sync_addr.i_ip, "pd_port", std::to_string(sync_addr.port),
		"pd_events_ip", events_addr.ip, "pd_i_events_ip", events_addr.i_ip, "pd_events_port", std::to_string(events_addr.port)
	};



	exception_queue_ptr_t exc_queue(new exception_queue);
	bool is_exception_occurred = false;
	thread_exception_handler thread_exception_handler(exc_queue, [](const std::exception_ptr &exc_ptr)
	{
		try{
			std::rethrow_exception(exc_ptr);
		}
		catch (const std::exception& exc)
		{
			g_exception_string = exc.what();
		}
		catch (...)
		{
			g_exception_string = "Some exception";
		}

		sync_helpers::set_event(g_stop_event);
		//_putch(' ');

	});

	connector.setConfig(config);

	auto client_settings = connector.getConfig();

	auto settings_func = [client_settings](const std::string &key)->std::vector<std::string>
	{
		std::vector<std::string> result;
		if (key == "Syncronizer device")
		{
			result.push_back(client_settings[0].share_memory_name);
			result.push_back(std::to_string(client_settings[0].share_memory_size));
			result.push_back(client_settings[0].read_event_name);
			result.push_back(std::to_string(client_settings[0].id));
		}
		if (key == "Syncronizer events device")
		{
			result.push_back(client_settings[1].share_memory_name);
			result.push_back(std::to_string(client_settings[1].share_memory_size));
			result.push_back(client_settings[1].read_event_name);
			result.push_back(std::to_string(client_settings[1].id));
		}

		return result;
	};


	thread_exception_handler.start_processing();
	position_synchronizer_dispatcher packets_dispatcher(factory, active_state_func);
	packets_dispatcher.run_processing_loop(settings_func, exc_queue);

	sync_helpers::wait(g_stop_event);

	packets_dispatcher.stop_processing_loop();

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

		const int min_num_of_args = 9;
		const int max_num_of_args = min_num_of_args;

		args_num = argc - 1;

		if (args_num != 0 && 
			!(argc >= min_num_of_args && argc <= max_num_of_args) &&
			args_num != 2
			)
		{
			const std::wstring exe_name_w(argv[0]);
			const std::string exe_name(exe_name_w.cbegin(), exe_name_w.cend());
			std::cout << "Usage:" << std::endl;
			std::cout << exe_name << " sync_ip ip sync_port port events_ip ip events_port port " << std::endl;
			std::cout << "sync_ip ip - ip address synchronization packet source." << std::endl;
			std::cout << "sync_i_ip ip - interface ip address synchronization packet source." << std::endl;
			std::cout << "sync_port port - ip port synchronization packet source." << std::endl;
			std::cout << "events_ip ip - ip address events packet source." << std::endl;
			std::cout << "events_i_ip ip - interface ip address events packet source." << std::endl;
			std::cout << "events_port port - ip port events packet source." << std::endl;
			return -1;
		}


		std::cout << "Count arguments: " << args_num << std::endl;

		std::wstring w_sync_ip = L"224.5.6.1";
		std::wstring w_sync_i_ip = L"192.168.3.105";
		std::wstring w_sync_port = L"32300";
		std::wstring w_events_ip = L"224.5.6.98";
		std::wstring w_events_i_ip = L"192.168.2.105";
		std::wstring w_events_port = L"32298";

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
				auto w_profile_id = parameters.at(L"p");

				if (w_profile_id == L"2"){
					w_sync_i_ip = L"172.16.0.42";
					w_events_i_ip = L"172.16.0.42";
				}

				if (w_profile_id == L"3"){
					w_sync_i_ip = L"192.168.3.109";
					w_events_i_ip = L"192.168.2.109";
				}
				if (w_profile_id == L"4"){
					//w_sync_i_ip = L"127.0.0.1";
					//w_events_i_ip = L"127.0.0.1";
					w_sync_i_ip = L"192.168.2.15";
					w_events_i_ip = L"192.168.2.15";
				}

			}
			else{
				w_sync_ip = parameters.at(L"sync_ip");
				w_sync_i_ip = parameters.at(L"sync_i_ip");
				w_sync_port = parameters.at(L"sync_port");
				w_events_ip = parameters.at(L"events_ip");
				w_events_i_ip = parameters.at(L"events_i_ip");
				w_events_port = parameters.at(L"events_port");
			}
		}


		std::cout << "Actual parameters:" << std::endl;
		std::cout << "sync_ip ip: " << std::string(w_sync_ip.cbegin(), w_sync_ip.cend()) << std::endl;
		std::cout << "sync_i_ip: " << std::string(w_sync_i_ip.cbegin(), w_sync_i_ip.cend()) << std::endl;
		std::cout << "sync_port:: " << std::string(w_sync_port.cbegin(), w_sync_port.cend()) << std::endl;
		std::cout << "events_ip ip: " << std::string(w_events_ip.cbegin(), w_events_ip.cend()) << std::endl;
		std::cout << "events_i_ip: " << std::string(w_events_i_ip.cbegin(), w_events_i_ip.cend()) << std::endl;
		std::cout << "events_port: " << std::string(w_events_port.cbegin(), w_events_port.cend()) << std::endl;


		const std::string sync_ip(w_sync_ip.cbegin(), w_sync_ip.cend());
		const std::string events_ip(w_events_ip.cbegin(), w_events_ip.cend());
		const std::string sync_i_ip(w_sync_i_ip.cbegin(), w_sync_i_ip.cend());
		const std::string events_i_ip(w_events_i_ip.cbegin(), w_events_i_ip.cend());

		const auto sync_port = (unsigned short)std::stoul(w_sync_port);
		const auto events_port = (unsigned short)std::stoul(w_events_port);
		connection_address sync_addr{ sync_ip, sync_i_ip, sync_port };
		connection_address events_addr{ events_ip, events_i_ip, events_port };

		g_stop_event = sync_helpers::create_basic_event_object(true);
		
		std::thread _thread(start, sync_addr, events_addr);

		std::thread _thread_getch([](){_getch(); sync_helpers::set_event(g_stop_event); });

		sync_helpers::wait(g_stop_event);
		
		//_getch();

		//sync_helpers::set_event(g_stop_event);

		if (_thread.joinable())
			_thread.join();

		_putch(' ');

		if (_thread_getch.joinable())
			_thread_getch.join();

		if (!g_exception_string.empty())
		{
			throw std::exception(g_exception_string.c_str());
		}

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


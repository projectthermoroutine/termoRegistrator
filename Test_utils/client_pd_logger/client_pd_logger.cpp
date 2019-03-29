#include <common\sync_helpers.h>
#include <common\socket_holder.h>
#include <common\date_helpers.h>

#include <map>
#include <vector>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <position_detector_common\position_detector_packet.h>
#include <position_detector_common\details\position_detector_packet_details.h>

#include <position_detector_dispatcher\position_detector_dispatcher.h>
#include <position_detector_dispatcher\pd_settings.h>



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
	static void dispatch_message_Events(const BYTE * message, unsigned int /*message_size*/)
	{
		std::string event((char*)(message));
		if (event.size() > 0){

			std::ostringstream ss;
			time_t current_time;
			time(&current_time);
			ss << date_helpers::local_time_to_str(current_time, "%Y/%m/%d %H:%M:%S") << " " << _current_counter << " ";

			file_Events.write(ss.str().c_str(), ss.str().size());
			file_Events.write(event.c_str(), event.size());
			file_Events.write("\n",sizeof("\n"));
		}

	}

	static void dispatch_message_Synchro(const BYTE * message, unsigned int message_size)
	{

		auto packet = position_detector::parce_packet_from_message<position_detector::synchronization::sync_packet_ptr_t>(message,message_size);
		if (packet)
		{
			_current_counter = packet->counter;
			file_Synchro.write((char*)message, message_size);
		}

	}

	static position_detector::synchronization::counter_t _current_counter;
};


position_detector::synchronization::counter_t packets_stream::_current_counter = 0;

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


	auto thread_exception_handler(std::make_shared<::thread_exception_handler>([](const std::exception_ptr &exc_ptr)
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

	}));


	auto settings_func = [sync_addr, events_addr](const std::string &key)->std::vector<std::string>
	{
		std::vector<std::string> result;
		if (key == "Syncronizer device")
		{
			result.push_back(sync_addr.ip);
			result.push_back(sync_addr.i_ip);
			result.push_back(std::to_string(sync_addr.port));
		}
		if (key == "Syncronizer events device")
		{
			result.push_back(events_addr.ip);
			result.push_back(events_addr.i_ip);
			result.push_back(std::to_string(events_addr.port));
		}

		return result;
	};


	scoped_WSA WSA;
	position_synchronizer_dispatcher packets_dispatcher(factory, active_state_func);
	packets_dispatcher.run_processing_loop(settings_func, thread_exception_handler);

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

struct profile_info {
	std::wstring sync_i_ip;
	std::wstring events_i_ip;
	std::wstring description;
};

static profile_info profiles_info[] = { { L"192.168.3.105", L"192.168.2.105" , L"Baku1"},
{ L"172.16.0.110", L"172.16.0.110", L"My" },
{ L"192.168.3.109", L"192.168.2.109", L"" },
{ L"192.168.2.15", L"192.168.2.15", L"" },
{ L"192.168.3.121", L"192.168.2.14", L"" },
{ L"192.168.2.1", L"192.168.2.1", L"" }
};


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		int args_num = 0;    // Default is no line numbers.

		const int min_num_of_args = 13;
		const int max_num_of_args = min_num_of_args;


		std::wstring w_sync_ip = L"224.5.6.1";
		std::wstring w_sync_i_ip = L"192.168.3.105";
		std::wstring w_sync_port = L"32300";
		std::wstring w_events_ip = L"224.5.6.98";
		std::wstring w_events_i_ip = L"192.168.2.105";
		std::wstring w_events_port = L"32298";

		int max_profile_index = sizeof(profiles_info) / sizeof(profile_info) - 1;

		args_num = argc - 1;

		if (args_num != 0 && 
			!(argc >= min_num_of_args && argc <= max_num_of_args) &&
			args_num != 2
			)
		{
			const std::wstring exe_name_w(argv[0]);
			const std::string exe_name(exe_name_w.cbegin(), exe_name_w.cend());
			std::cout << "Usage:" << std::endl;
			std::cout << exe_name << " sync_ip ip sync_port port sync_i_ip i_ip events_ip ip events_port port events_i_ip i_ip_events\n" << std::endl;
			std::cout << "sync_ip ip - ip address synchronization packet source." << std::endl;
			std::cout << "sync_i_ip ip - interface ip address synchronization packet source." << std::endl;
			std::cout << "sync_port port - ip port synchronization packet source." << std::endl;
			std::cout << "events_ip ip - ip address events packet source." << std::endl;
			std::cout << "events_i_ip ip - interface ip address events packet source." << std::endl;
			std::cout << "events_port port - ip port events packet source." << std::endl;
			std::cout << "Example: client_pd_logger.exe sync_ip 224.5.6.1 sync_port 32300 sync_i_ip 192.168.3.142 events_ip 224.5.6.98 events_port 32298 events_i_ip 192.168.2.1" << std::endl;
			std::cout << "Or" << std::endl;
			std::cout << exe_name << " p index\n" << std::endl;
			std::cout << "Avaliable profiles:\n" << std::endl;

			std::vector<std::wstring> lines(9);

			for (int i = 0; i <= max_profile_index; i++)
			{
				lines[0] += L" " + profiles_info[i].description + L" Index: " + std::to_wstring(i + 1);
				lines[2] += L" sync_ip: " + w_sync_ip;
				lines[3] += L" sync_port: " + w_sync_port;
				lines[4] += L" sync_i_ip: " + profiles_info[i].sync_i_ip;
				lines[6] += L" events_ip: " + w_events_ip;
				lines[7] += L" events_port: " + w_events_port;
				lines[8] += L" events_i_ip:" + profiles_info[i].events_i_ip;

				const auto max_line_size = std::max_element(lines.cbegin(), lines.cend(), 
					[&](const auto & first, const auto & second)
				{ 
					return first.size() < second.size(); 
				})->size();

				for (auto & line : lines)
				{
					std::wstring spacer = L"   ";
					for (auto j = line.size(); j < max_line_size; ++j)
						spacer += L" ";
					line += spacer + L"|";
				}

			}

			for(const auto & line : lines)
			std::wcout << line << std::endl;

			return -1;
		}


		std::cout << "Count arguments: " << args_num << std::endl;

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

				auto profile_index = std::stol(w_profile_id) - 1;
				if (profile_index < 0 || profile_index > max_profile_index)
					profile_index = 0;

				w_sync_i_ip = profiles_info[profile_index].sync_i_ip;
				w_events_i_ip = profiles_info[profile_index].events_i_ip;

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


		std::wcout << "Actual parameters:" << std::endl;
		std::wcout << "sync_ip ip: " << w_sync_ip << std::endl;
		std::wcout << "sync_i_ip: " << w_sync_i_ip<< std::endl;
		std::wcout << "sync_port:: " << w_sync_port<< std::endl;
		std::wcout << "events_ip ip: " << w_events_ip<< std::endl;
		std::wcout << "events_i_ip: " << w_events_i_ip<< std::endl;
		std::wcout << "events_port: " << w_events_port<< std::endl;


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


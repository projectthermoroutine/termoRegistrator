#include <windows.h>
#ifdef _WINGDI_
#undef _WINGDI_
#endif

#include <atlsafe.h>
#include <vector>
#include <map>
#include <sstream>
#include <thread>

#include <common\handle_holder.h>
#include <loglib\log.h>
#include <common\sync_helpers.h>

#include <position_detector_com_server/midl/PD_COMServer_i.h>
#include <position_detector_com_server/midl/PD_COMServer_i.c>

#include "server_proxy_pd_connector.h"
#include "details\position_detector_connector_api.h"

#include <comutil.h>
#include <atlcom.h>

using namespace std;

#pragma comment(lib, "comsuppw.lib")

#define CATCH_ALL_TERMINATE \
	catch (const std::exception & exc) \
{ \
	LOG_FATAL() << "Error: " << exc.what(); \
	terminate(); \
} \
	catch (...) \
{ \
	LOG_FATAL() << "Unknown exception."; \
	terminate(); \
}

namespace position_detector
{

	class sink_server_proxy_events final
	{
	public:
		sink_server_proxy_events(details::shared_memory_connector_api *p_connector, proxy_server_pd_events_callback_func_t dispatch_error_func,
			proxy_server_pd_events_callback_func_t connection_error_func,
			proxy_server_pd_events_callback_func_t runtime_error_func):
			_dispatch_error_func(dispatch_error_func),
			_connection_error_func(connection_error_func),
			_runtime_error_func(runtime_error_func),
			_stop_event(sync_helpers::create_basic_event_object(true)),
			_p_connector(nullptr)
		{
			_p_connector = p_connector;
		}
		~sink_server_proxy_events()
		{
			stop_processing_loop();
			delete _p_connector;

		}
		void stop_processing_loop()
		{
			LOG_STACK();

			LOG_TRACE() << "Requesting stopping.";
			sync_helpers::set_event(_stop_event);
			LOG_TRACE() << "Stop flag was set.";

			if (_events_listener_thread.get_id() == std::this_thread::get_id())
				return;
			if (_events_listener_thread.joinable())
			{
				LOG_TRACE() << "Waiting for completion of processing loop thread.";
				_events_listener_thread.join();
				LOG_TRACE() << "Processing loop thread finished.";
			}
		}

		void run_processing_loop()
		{
			LOG_STACK();
			if (_events_listener_thread.joinable())
			{
				LOG_DEBUG() << "Looks like run_processing_loop was called twice.";
				throw std::logic_error("The processing loop must not be running at this point.");
			}

			LOG_TRACE() << "Creating processing loop thread for proxy server events.";

			std::thread processing_loop_thread(
				[this]()
			{ this->processing_loop(); }
			);

			LOG_TRACE() << "Thread was created.";

			_events_listener_thread.swap(processing_loop_thread);
		}
	private:
		void processing_loop()
		{
			LOG_STACK();

			try
			{
				LOG_TRACE() << "Running processing loop.";

				while (!stop_requested())
				{
					run_message_processing();
				}

				LOG_TRACE() << "Stop was requested.";
			}
			catch (const std::exception & exc) 
			{ 
			LOG_FATAL() << "Error: " << exc.what(); 
			terminate(); 
			} 
			catch (...) 
			{ 
			LOG_FATAL() << "Unknown exception."; 
			terminate(); 
			}
			//CATCH_ALL_TERMINATE
		}

		void run_message_processing()
		{
			LOG_STACK();

			LOG_TRACE() << "Running message processing loop.";

			bool _connection_closing_requested = false;

			while (!stop_requested())
			{
				LOG_TRACE() << "Waiting for a message";

				try
				{
					get_message_struct message_buffer;

					LOG_TRACE() << "Getting message.";
					const packet_size_t packet_size = get_message(&message_buffer, sizeof(message_buffer));
					if (packet_size)
					{
						errorInfo * p_error_info = reinterpret_cast<errorInfo *>(&message_buffer);

						proxy_server_pd_events_callback_func_t message_sink_func;
						switch (p_error_info->err_source)
						{
							case errorSource::dispatch_error:
								message_sink_func = _dispatch_error_func;
								break;
							case errorSource::connection_error:
								message_sink_func = _connection_error_func;
								break;
							case errorSource::runtime_error:
								message_sink_func = _runtime_error_func;
								break;
							default:
							break;
						}

						if (message_sink_func){
							std::string msg(reinterpret_cast<char*>(&p_error_info->err_str));
							message_sink_func(msg);
						}

					}
				}
				catch (...)
				{
					_connection_closing_requested = true;
				}

				if (_connection_closing_requested)
				{
					LOG_TRACE() << "Connection closing was requested.";
					return;
				}
			}

			LOG_TRACE() << "Stop was requested.";
		}
		unsigned int get_message(
			get_message_struct * const buffer,
			const packet_size_t buffer_size)
		{
			LOG_STACK();

			for (;;)
			{
				auto result = _p_connector->get_message(buffer, buffer_size);
				if (result > 0){
					return result;
				}
				if (stop_requested())
				{
					LOG_TRACE() << "Stop was requested.";
					return 0;
				}
			}
			return 0;
		}


		bool sleep_for(const DWORD timeout) const
		{
			LOG_STACK();

			auto wait_result = WaitForSingleObject(_stop_event.get(), timeout);
			if (wait_result == WAIT_OBJECT_0)
			{
				return false;
			}
			else if (wait_result == WAIT_FAILED)
			{
				auto last_error = GetLastError();
				LOG_DEBUG() << "WaitForSingleObject failed on stop event: " << last_error;
				throw std::runtime_error("Checking state of stop event failed.");
			}
			else if (wait_result == WAIT_ABANDONED)
			{
				LOG_TRACE() << "WaitForSingleObject returned unexpected result for stop event: WAIT_ABANDONED. Treat as a stop request.";
				return false;
			}
			else
			{
				LOG_DEBUG() << "Unexpected wait result was returned from WaitForSingleObject: " << wait_result;
			}

			return true;
		}

		bool stop_requested() const
		{
			LOG_STACK();

			return !sleep_for(0);
		}

	private:
		proxy_server_pd_events_callback_func_t _dispatch_error_func;
		proxy_server_pd_events_callback_func_t _connection_error_func;
		proxy_server_pd_events_callback_func_t _runtime_error_func;

		details::shared_memory_connector_api *_p_connector;

		std::thread _events_listener_thread;
		handle_holder _stop_event;

	};


	server_proxy_pd_connector_exception::server_proxy_pd_connector_exception(HRESULT error_code, const std::string & message) :
		std::runtime_error(message), _error_code(error_code)
	{
			std::ostringstream ss;
			ss << message << " Error: " << std::hex << std::showbase << error_code;
			_message = ss.str();
		}

	const char * server_proxy_pd_connector_exception::what() const
	{
		return _message.c_str();
	}

	HRESULT server_proxy_pd_connector_exception::get_error_code() const
	{
		return _error_code;
	}

	namespace details
	{
		class server_proxy_pd_connector_impl final
		{

		public:
			server_proxy_pd_connector_impl(proxy_server_pd_events_callback_func_t dispatch_error_func,
				proxy_server_pd_events_callback_func_t connection_error_func,
				proxy_server_pd_events_callback_func_t runtime_error_func);
			~server_proxy_pd_connector_impl();
			server_proxy_pd_connector_impl(const server_proxy_pd_connector_impl &) = delete;
			server_proxy_pd_connector_impl & operator = (const server_proxy_pd_connector_impl &) = delete;

			bool setConfig(const std::vector<std::string>& config) const;
			std::vector<client_settings> getConfig() const;
		private:
			details::shared_memory_connector_api * connect_to_events_stream();

		private:
			IClassFactory* pICF;
			IProxyPD_Dispatcher* pIProxy_pd_dispatcher;
			sink_server_proxy_events *_p_sink_server_proxy_events;
			mutable uint32_t _client_id;
			uint32_t _errors_client_id;
		};


		server_proxy_pd_connector_impl::server_proxy_pd_connector_impl(proxy_server_pd_events_callback_func_t dispatch_error_func,
			proxy_server_pd_events_callback_func_t connection_error_func,
			proxy_server_pd_events_callback_func_t runtime_error_func) :pICF(nullptr), pIProxy_pd_dispatcher(nullptr), _p_sink_server_proxy_events(nullptr)
		{
			_client_id = _errors_client_id = 0;
			auto result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			if (FAILED(result))	{
				throw 	server_proxy_pd_connector_exception(result, "Can't initialize Com library");
			}

			result = CoGetClassObject(CLSID_ProxyPD_Dispatcher, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&pICF);

			if (FAILED(result))	{
				CoUninitialize();
				throw 	server_proxy_pd_connector_exception(result, "Can't get Com class object");
			}

			result = pICF->CreateInstance(NULL, IID_IProxyPD_Dispatcher, (void**)&pIProxy_pd_dispatcher);

			if (FAILED(result))
			{
				pICF->Release();
				CoUninitialize();
				throw 	server_proxy_pd_connector_exception(result, "Can't get Com interface for proxy position dispatcher");
			}

			auto events_connector = connect_to_events_stream();
			_p_sink_server_proxy_events = new sink_server_proxy_events(events_connector, dispatch_error_func, connection_error_func, runtime_error_func);
			_p_sink_server_proxy_events->run_processing_loop();
		}

		server_proxy_pd_connector_impl::~server_proxy_pd_connector_impl()
		{
			delete _p_sink_server_proxy_events;
			pIProxy_pd_dispatcher->disconnectClient(_client_id, _errors_client_id);
			pIProxy_pd_dispatcher->Release();
			pICF->Release();
			CoUninitialize();
		}

		bool server_proxy_pd_connector_impl::setConfig(const std::vector<std::string>& config) const
		{

			CComSafeArray<BSTR> out_sa((ULONG)config.size());

			// Copy all the contents into SAFEARRAY
			for (size_t i = 0; i < config.size(); ++i)
			{
				out_sa.SetAt((LONG)i, _com_util::ConvertStringToBSTR(config[i].c_str()),FALSE);
			}

			CComVariant variantOut_sa(out_sa);

			auto result = pIProxy_pd_dispatcher->setConfig(variantOut_sa);

			if (FAILED(result))
			{
				throw 	server_proxy_pd_connector_exception(result, "Can't set configuration to the proxy server position dispatcher");
			}

			if (result == S_FALSE){
				return false;
			}
			return true;

		}

		std::vector<client_settings> server_proxy_pd_connector_impl::getConfig() const
		{
			if (_client_id > 0)
			{
				pIProxy_pd_dispatcher->disconnectClient(_client_id,0);
			}

			ShareMemorySettings sync_settings, event_settings;
			_client_id = 0;
			auto result = pIProxy_pd_dispatcher->getConfig(&sync_settings, &event_settings, &_client_id);
			if (FAILED(result))
			{
				throw 	server_proxy_pd_connector_exception(result, "Can't get client configuration from the proxy server position dispatcher.");
			}

			std::vector<client_settings> config{ client_settings(), client_settings() };

			if (sync_settings.read_event_name == nullptr ||
				sync_settings.share_memory_name == nullptr ||
				event_settings.read_event_name == nullptr ||
				event_settings.share_memory_name == nullptr
				)
			{
				throw server_proxy_pd_connector_exception(result, "null name was recieve from proxy server.");
			}

			auto read_event_name = _com_util::ConvertBSTRToString(sync_settings.read_event_name);
			auto share_memory_name = _com_util::ConvertBSTRToString(sync_settings.share_memory_name);

			config[0].read_event_name = read_event_name;
			config[0].share_memory_name = share_memory_name;
			config[0].share_memory_size = (unsigned int)sync_settings.share_memory_size;

			delete read_event_name;
			delete share_memory_name;

			read_event_name = _com_util::ConvertBSTRToString(event_settings.read_event_name);
			share_memory_name = _com_util::ConvertBSTRToString(event_settings.share_memory_name);

			config[1].read_event_name = read_event_name;
			config[1].share_memory_name = share_memory_name;
			config[1].share_memory_size = (unsigned int)event_settings.share_memory_size;

			delete read_event_name;
			delete share_memory_name;

			SysFreeString(sync_settings.read_event_name);
			SysFreeString(sync_settings.share_memory_name);
			SysFreeString(event_settings.read_event_name);
			SysFreeString(event_settings.share_memory_name);

			
			return config;
		}
		details::shared_memory_connector_api * server_proxy_pd_connector_impl::connect_to_events_stream()
		{

			ShareMemorySettings events_stream;
			_errors_client_id = 0;
			auto result = pIProxy_pd_dispatcher->connectToErrorsStream(&events_stream, &_errors_client_id);
			if (FAILED(result))
			{
				throw 	server_proxy_pd_connector_exception(result, "Can't get events stream from the proxy server position dispatcher");
			}

			if (events_stream.read_event_name == nullptr ||
				events_stream.share_memory_name == nullptr)
			{
				throw server_proxy_pd_connector_exception(result, "null name was recieve from proxy server.");
			}

			auto read_event_name = _com_util::ConvertBSTRToString(events_stream.read_event_name);
			auto share_memory_name = _com_util::ConvertBSTRToString(events_stream.share_memory_name);

			client_settings settings{ (unsigned int)events_stream.share_memory_size, share_memory_name, read_event_name };

			delete read_event_name;
			delete share_memory_name;

			SysFreeString(events_stream.share_memory_name);
			SysFreeString(events_stream.read_event_name);

			std::vector<std::string> config{ settings.share_memory_name,
				std::to_string(settings.share_memory_size),
				settings.read_event_name
			};

			return new details::shared_memory_connector_api(config);
		}

		inline server_proxy_pd_connector_impl* cast_to_server_proxy_pd_connector_impl(void * p_impl)
		{
			if (p_impl == nullptr)
				throw std::invalid_argument("The passed argument p_impl can't be equal to nullptr");

			return static_cast<server_proxy_pd_connector_impl*>(p_impl);
		}


	}

	server_proxy_pd_connector::server_proxy_pd_connector(proxy_server_pd_events_callback_func_t dispatch_error_func,
		proxy_server_pd_events_callback_func_t connection_error_func,
		proxy_server_pd_events_callback_func_t runtime_error_func) :
		_p_impl(new details::server_proxy_pd_connector_impl(dispatch_error_func, connection_error_func, runtime_error_func))
	{
	}

	server_proxy_pd_connector::~server_proxy_pd_connector()
	{
		auto const p_impl = _p_impl;
		_p_impl = 0;

		delete details::cast_to_server_proxy_pd_connector_impl(p_impl);
	}

	bool server_proxy_pd_connector::setConfig(const std::vector<std::string>& config) const
	{
		return details::cast_to_server_proxy_pd_connector_impl(_p_impl)->setConfig(config);
	}

	std::vector<client_settings> server_proxy_pd_connector::getConfig() const
	{
		return details::cast_to_server_proxy_pd_connector_impl(_p_impl)->getConfig();
	}


}
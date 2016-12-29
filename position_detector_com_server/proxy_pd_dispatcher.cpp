#include "stdafx.h"
#include "proxy_pd_dispatcher.h"
#include <atlsafe.h>
#include <vector>
#include <common\sync_helpers.h>
#include <common\path_helpers.h>
#include <common\shared_memory_channel.h>
#include "proxy_server_events_dispatcher.h"

#include <position_detector_dispatcher\proxy_server_pd.h>
#include <position_detector_dispatcher\details\shared_memory_channel.h>

#include <common\thread_exception.h>
#include <common\unhandled_exception.h>
#include <loglib\log.h>
#include <fstream>

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


using namespace position_detector;
using namespace proxy_server_pd_ns;


using pd_dispatcher_ptr = std::unique_ptr<proxy_server_pd>;
struct CProxyPD_Dispatcher::Impl
{
	connection_address pd_address;
	connection_address pd_events_address;
	pd_dispatcher_ptr  pd_dispatcher;
	events_manager_ptr_t events_manager;
	thread_exception_handler_ptr thread_exception_handler;
	uint32_t last_client_id;
	uint32_t last_errors_client_id;
	uint32_t clients_counter;
	uint32_t errors_clients_counter;

	void connection_active_state(bool)
	{
	}

	void exception_handler(const std::exception_ptr& exc_ptr)
	{
		LOG_STACK()
		try{
			pd_dispatcher->stop();
			std::rethrow_exception(exc_ptr);
		}

		catch (const position_detector_connector_exception& exc)
		{
			events_manager->send_error(errorSource::connection_error, exc.what());
		}
		catch (const channels::shared_memory_channel_exception& exc)
		{
			events_manager->send_error(errorSource::dispatch_error, exc.what());
		}
		catch (const std::exception& exc)
		{
			events_manager->send_error(errorSource::runtime_error, exc.what());
		}
		catch (...)
		{
			events_manager->send_error(errorSource::runtime_error, "Unexpected exception.");
		}

	}

};

std::wstring get_module_file_name(HINSTANCE instance)
{
	LOG_STACK();

	const auto buffer_size = DWORD{ UNICODE_STRING_MAX_CHARS };
	auto path_buffer = std::unique_ptr<wchar_t[]> { new wchar_t[static_cast<std::size_t>(buffer_size)] };

	const auto get_module_file_name_result = GetModuleFileNameW(instance, path_buffer.get(), buffer_size);
	if (get_module_file_name_result == 0){
		auto last_error = GetLastError();
		std::ostringstream ss;
		ss << "GetModuleFileNameW failed." << " Error: " << std::hex << std::showbase << last_error;
		LOG_DEBUG() << ss.str().c_str();
		throw std::runtime_error(ss.str());
	}

	return{ path_buffer.get() };
}

std::wstring get_current_module_path()
{
	auto module = HMODULE{};

	if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCWSTR)&get_current_module_path,
		&module))
	{
		auto last_error = GetLastError();
		std::ostringstream ss;
		ss << "GetModuleHandleExW failed." << " Error: " << std::hex << std::showbase << last_error;
		LOG_DEBUG() << ss.str().c_str();
		throw std::runtime_error(ss.str());
	}

	return get_module_file_name(module);
}

static const std::wstring g_log_default_config = L"<log_settings><developer_log use_developer_log = \"false\" level = \"TRACE\" max_backup_index = \"5\" max_file_size = \"52428800\"/><history_log max_buffer_size = \"1048576\" /></log_settings>";

void initialize_log()
{
	std::wstring module_path = L"C:\\";
	std::wstring base_name = L"PD_COMServer";
	std::string err;

	try{
		const auto full_path = get_current_module_path();
		module_path = path_helpers::get_directory_path(full_path);
		base_name = path_helpers::get_base_name(full_path);
	}
	catch (const std::runtime_error& exc)
	{
		err = exc.what();
	}
	std::wstring log_config_file_name = module_path + L"\\" + base_name + L".log.config";
	std::wstring log_config(
		(std::istreambuf_iterator<char>(
		*(std::unique_ptr<std::ifstream>(
		new std::ifstream(log_config_file_name)
		)).get()
		)),
		std::istreambuf_iterator<char>()
		);

	if (log_config.empty())
		log_config = g_log_default_config;

	auto const pid = GetCurrentProcessId();

	std::wstring log_name = base_name + L"_" + std::to_wstring(pid);

	try{
		unhandled_exception_handler::initialize(module_path, [](const std::wstring & message) { LOG_FATAL() << message; });

		logger::initialize(
			log_config,
			module_path,
			log_name,
			false,
			[](bool){});
	}
	catch (const std::exception&){}

	LOG_DEBUG() << "log path: " << module_path;

	if (!err.empty())
		LOG_DEBUG() << "get_current_module_path failed: " << err.c_str();

}

CProxyPD_Dispatcher::CProxyPD_Dispatcher()
{
	initialize_log();

	LOG_STACK();

	decltype(_p_impl) impl = std::make_unique<CProxyPD_Dispatcher::Impl>();
	impl->clients_counter = 0;
	impl->errors_clients_counter = 0;
	impl->last_client_id = 0;
	impl->last_errors_client_id = 0;

	impl->pd_dispatcher = std::make_unique<proxy_server_pd>(std::bind(&CProxyPD_Dispatcher::Impl::connection_active_state, impl.get(), std::placeholders::_1));
	impl->events_manager = std::make_unique<events_manager>();
//	impl->exc_queue = std::make_shared<exception_queue>();

	impl->thread_exception_handler = 
		std::make_shared<thread_exception_handler>(
		std::bind(&CProxyPD_Dispatcher::Impl::exception_handler, impl.get(), std::placeholders::_1)
		);

	_p_impl.swap(impl);
}
CProxyPD_Dispatcher::~CProxyPD_Dispatcher()
{
	logger::deinitialize();
}

STDMETHODIMP CProxyPD_Dispatcher::getConfig(ShareMemorySettings* syncSettings, ShareMemorySettings* eventSettings, ULONG32* clientId)
{
	LOG_STACK()
	client_context_ptr_t context_synchro, context_events;
	auto new_client_id = _InterlockedIncrement(&_p_impl->last_client_id);
	try{
		context_synchro = _p_impl->pd_dispatcher->create_client_context(new_client_id,packet_type::synchronization_packet);
		context_events = _p_impl->pd_dispatcher->create_client_context(new_client_id,packet_type::event_packet);
	}
	catch (const client_context_exception& exc)
	{
		return exc.get_error_code();
	}

	{
		USES_CONVERSION;
		auto w_param = context_synchro->get_shared_memory_name();
		auto param = W2A(w_param.c_str());
		syncSettings->share_memory_name = _com_util::ConvertStringToBSTR(param);
		
		syncSettings->share_memory_size = context_synchro->get_shared_memory_size();

		w_param = context_synchro->get_event_name();
		param = W2A(w_param.c_str());
		syncSettings->read_event_name = _com_util::ConvertStringToBSTR(param);

		w_param = context_events->get_shared_memory_name();
		param = W2A(w_param.c_str());
		eventSettings->share_memory_name = _com_util::ConvertStringToBSTR(param);

		eventSettings->share_memory_size = context_events->get_shared_memory_size();

		w_param = context_events->get_event_name();
		param = W2A(w_param.c_str());
		eventSettings->read_event_name = _com_util::ConvertStringToBSTR(param);
		*clientId = new_client_id;
	}

	_p_impl->pd_dispatcher->add_client(context_synchro);
	_p_impl->pd_dispatcher->add_client(context_events, packet_type::event_packet);

	_InterlockedIncrement(&_p_impl->clients_counter);


	return S_OK;
}

STDMETHODIMP CProxyPD_Dispatcher::setConfig(VARIANT Arr)
{
	LOG_STACK()
	BSTR* pDest;
	std::map<std::string, std::string> settings;

	if (_p_impl->pd_dispatcher->State() == proxy_server_pd::state::TurnOn)
	{
		return S_FALSE;
	}
	USES_CONVERSION;
	//if (Arr.vt == (VT_ARRAY | VT_BSTR))
	{
		SafeArrayAccessData(Arr.parray, (void**)&pDest);	// Get the data

		CComBSTR bstr1 = pDest[0];
		for (ULONG i = 0; i < Arr.parray->rgsabound->cElements; i = i + 2){
			settings.insert({ _com_util::ConvertBSTRToString(pDest[i]), _com_util::ConvertBSTRToString(pDest[i + 1]) });
		}
	
		SafeArrayUnaccessData(Arr.parray);
	}

	try
	{
		auto map_iter = settings.find("pd_ip");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}
		_p_impl->pd_address.ip = map_iter->second;

		map_iter = settings.find("pd_i_ip");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}
		_p_impl->pd_address.i_ip = map_iter->second;

		map_iter = settings.find("pd_port");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}

		_p_impl->pd_address.port = (unsigned short)std::stoul(map_iter->second);

		map_iter = settings.find("pd_events_ip");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}
		_p_impl->pd_events_address.ip = map_iter->second;

		map_iter = settings.find("pd_i_events_ip");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}
		_p_impl->pd_events_address.i_ip = map_iter->second;


		map_iter = settings.find("pd_events_port");
		if (map_iter == settings.end()){
			return E_INVALIDARG;
		}

		_p_impl->pd_events_address.port = (unsigned short)std::stoul(map_iter->second);
	}
	catch (const std::invalid_argument&)
	{
		return E_INVALIDARG;
	}
	catch (const std::out_of_range&)
	{
		return E_ATL_VALUE_TOO_LARGE;
	}

	_p_impl->pd_dispatcher->start(_p_impl->pd_address, _p_impl->pd_events_address, _p_impl->thread_exception_handler);

	return S_OK;
}

STDMETHODIMP CProxyPD_Dispatcher::connectToErrorsStream(ShareMemorySettings* errStream, ULONG32* clientId)
{
	LOG_STACK()
	const unsigned int memory_size = 4096 - 4*sizeof(long);
	std::wstring shared_memory_name;
	sync_helpers::create_random_name(shared_memory_name,false);

	auto new_client_id = _InterlockedIncrement(&_p_impl->last_errors_client_id);

	events_stream_ptr_t p_channel;
	try{
		p_channel = std::make_shared<channels::shared_memory_channel>(new_client_id, shared_memory_name, memory_size);
	}
	catch (const channels::shared_memory_channel_exception&)
	{
		return E_FAIL; //exc.get_error_code();
	}

	{
		USES_CONVERSION;
		auto w_param = p_channel->shared_memory_name();
		auto param = W2A(w_param.c_str());
		errStream->share_memory_name = _com_util::ConvertStringToBSTR(param);

		errStream->share_memory_size = p_channel->shared_memory_size();

		w_param = p_channel->event_name();
		param = W2A(w_param.c_str());
		errStream->read_event_name = _com_util::ConvertStringToBSTR(param);
		*clientId = new_client_id;
	}

	_p_impl->events_manager->add_client(p_channel);
	_InterlockedIncrement(&_p_impl->errors_clients_counter);

	return S_OK;
}
STDMETHODIMP
CProxyPD_Dispatcher::disconnectClient(ULONG32 clientId, ULONG32 errorsClientId)
{
	LOG_STACK()
	auto res = _p_impl->pd_dispatcher->remove_client(clientId);
	res = _p_impl->pd_dispatcher->remove_client(clientId, packet_type::event_packet);

	auto clients_number = _InterlockedDecrement(&_p_impl->clients_counter);

	_p_impl->events_manager->remove_client(errorsClientId);

	if (clients_number == 0)
	{
		_p_impl->pd_dispatcher->stop();
	}


	return S_OK;
}
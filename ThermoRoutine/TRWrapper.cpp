// TRWrapper.cpp: реализация CTRWrapper

#include "stdafx.h"
#include <string>
#include "TRWrapper.h"
#include "_ITRWrapperEvents_CP.h"

#include <comutil.h>
#include <memory>
#include <client_pd_dispatcher\client_pd_dispatcher.h>

#include "irb_frame_image_dispatcher.h"
#include "irb_frame_helper.h"
//#include "structures.h"

#define _1cm 10


using namespace video_grabber;
using namespace position_detector;
using namespace irb_frame_helper;

CTRWrapper::CTRWrapper() :
_notify_grab_frame_span(0),
disable_events(false),
_is_grabbing(false),
_cur_frame_id(0)
{
	_extern_irb_frames_cache.set_cache_size(10);
	grabber_state = IRB_GRABBER_STATE::NONE;
	_coordinates_manager = std::make_shared<packets_manager>();
	_client_pd_dispatcher = std::make_unique<client_pd_dispatcher>(_coordinates_manager, std::bind(&CTRWrapper::pd_proxy_error_handler, this, std::placeholders::_1));

	_exception_queue = std::make_shared<exception_queue>();
	_thread_exception_handler = std::make_unique<thread_exception_handler>(
		_exception_queue,
		std::bind(&CTRWrapper::client_pd_dispatcher_error_handler,
		this, std::placeholders::_1));

	_thread_exception_handler->start_processing();

	_irb_frames_cache = std::make_unique<irb_frame_delegates::irb_frames_cache>(
		(uint16_t)irb_frame_delegates::default_frames_per_file,
		std::bind(&CTRWrapper::process_grabbed_frame, this, std::placeholders::_1),
		0
		);

	_image_dispatcher.set_areas_mask_size(1024, 768);

	grabberReady = 0;
	_notify_grab_frame_counter = 0;
	_cached_frame_ids.resize(_notify_grab_frame_span + 1, 0);

	_grab_frames_file_pattern = "ir_metro_";
}

CTRWrapper::~CTRWrapper()
{
	FinishAll();
}

void CTRWrapper::init_pd_objects()
{
	if (_client_pd_dispatcher)
		return;
	_client_pd_dispatcher = std::make_unique<client_pd_dispatcher>(_coordinates_manager, std::bind(&CTRWrapper::pd_proxy_error_handler, this, std::placeholders::_1));
}
void CTRWrapper::close_pd_objects()
{
	_client_pd_dispatcher.reset();
}

void CTRWrapper::new_irb_file(const std::string & filename)
{
	auto bstr_filename = _com_util::ConvertStringToBSTR(filename.c_str());
	Fire_FileFromGrabber(bstr_filename);

	SysFreeString(bstr_filename);
}


void CTRWrapper::init_grabber_dispatcher()
{
	if (_grab_frames_dispatcher)
		return;
	
	USES_CONVERSION;
	_grab_frames_dispatcher = std::make_unique<irb_grab_frames_dispatcher::frames_dispatcher>();
	*_grab_frames_dispatcher += std::bind(&irb_frame_delegates::irb_frames_cache::process_frame_non_cache, _irb_frames_cache.get(), std::placeholders::_1);

	auto last_error = _grab_frames_dispatcher->last_error();
	if (!last_error.empty())
	{
		auto bstr_text = _com_util::ConvertStringToBSTR(last_error.c_str());
		Fire_grabberDispatcherError(bstr_text);
		SysFreeString(bstr_text);
	}
}

STDMETHODIMP CTRWrapper::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ITRWrapper
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTRWrapper::StartRecieveCoordinates(BSTR pd_ip, USHORT pd_port, BSTR events_pd_ip, USHORT events_pd_port)
{
	disable_events = false;

	if (pd_ip == nullptr || events_pd_ip == nullptr)
		return S_FALSE;

	if (_client_pd_dispatcher){
		_client_pd_dispatcher->stop_processing_loop();
	}
	else
	{
		_client_pd_dispatcher = std::make_unique<client_pd_dispatcher>(_coordinates_manager, std::bind(&CTRWrapper::pd_proxy_error_handler, this, std::placeholders::_1));
	}


	connection_address pd_address{ _com_util::ConvertBSTRToString(pd_ip), pd_port };
	connection_address pd_events_address{ _com_util::ConvertBSTRToString(events_pd_ip), events_pd_port };

	
	Fire_coordinatesDispatcherState(TRUE);
	_client_pd_dispatcher->run_processing_loop(pd_address, pd_events_address, _exception_queue);

	//SysFreeString(pd_ip);
	//SysFreeString(events_pd_ip);

	return S_OK;
}
STDMETHODIMP CTRWrapper::StopRecieveCoordinates()
{
	disable_events = true;
//	Fire_coordinatesDispatcherState(FALSE);

	if (_client_pd_dispatcher){
		_client_pd_dispatcher->stop_processing_loop();
	//	_thread_exception_handler->stop_processing();
	}
	disable_events = false;
	close_pd_objects();

	return S_OK;
}

void CTRWrapper::client_pd_dispatcher_error_handler(const std::exception_ptr &exc_ptr)
{
	USES_CONVERSION;
	BSTR bstr_text = nullptr;
	try{
		std::rethrow_exception(exc_ptr);
	}
	catch (const std::exception& exc)
	{
		auto error = exc.what();
		if (!disable_events){
			bstr_text = _com_util::ConvertStringToBSTR(error);
			Fire_coordinatesDispatcherError(bstr_text);
		}
	}
	catch (...)
	{
		if (!disable_events){
			bstr_text = _com_util::ConvertStringToBSTR("Unexpected ERROR.");
			Fire_coordinatesDispatcherError(bstr_text);
		}
	}

	if (bstr_text != nullptr){
		SysFreeString(bstr_text);
	}

	Fire_coordinatesDispatcherState(FALSE);
	StopRecieveCoordinates();

}
void CTRWrapper::pd_proxy_error_handler(const std::string &error)
{
	if (!disable_events){
		auto bstr_text = _com_util::ConvertStringToBSTR(error.c_str());
		Fire_coordinatesDispatcherError(bstr_text);
		SysFreeString(bstr_text);
		Fire_coordinatesDispatcherState(FALSE);
	}
	//StopRecieveCoordinates();
}

bool CTRWrapper::process_grabbed_frame(const irb_grab_frames_dispatcher::irb_frame_shared_ptr_t& frame)
{
	track_point_info point_info;
#ifdef TIMESTAMP_SYNCH_PACKET_ON
	time_t frame_time = (time_t)frame->get_frame_time_in_msec();
	auto res = _coordinates_manager->get_point_info_by_time(frame_time, point_info);
#else
	auto res = _coordinates_manager->get_last_point_info(point_info);
#endif
	if (res)
	{
		frame->coords.coordinate = point_info._movment_info.coordinate;
		frame->coords.path = point_info._path_info->path;
		frame->coords.line = point_info._path_info->line;
	}
	_cached_frame_ids[_notify_grab_frame_counter] = frame->id;

	if (_notify_grab_frame_counter++ == _notify_grab_frame_span)
	{
		_notify_grab_frame_counter = 0;

		_extern_irb_frames_cache.push_frame(frame);

		//if (!disable_events)
		//	Fire_FrameFromCam(_cached_frame_ids[0]);
	}

	return true;

}

void CTRWrapper::process_new_frame(irb_frame_helper::frame_id_t frame_id)
{
	//Fire_FrameFromCam(frame_id);
}

STDMETHODIMP CTRWrapper::GetGrabberSources(SAFEARRAY **sourcesList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	init_grabber_dispatcher();
//		if (_grab_frames_dispatcher)

	auto sources_list = _grab_frames_dispatcher->get_grabber_sources();


	SAFEARRAYBOUND bounds = { (ULONG)sources_list.size(),0 };
	*sourcesList = SafeArrayCreate(VT_BSTR, 1, &bounds);
	long i = 0;
	for (auto & source : sources_list)
	{
		auto bstr_source = _com_util::ConvertStringToBSTR(source.c_str());
		SafeArrayPutElement(*sourcesList, &i, bstr_source);
		i++;
	}

	return S_OK;
}

void CTRWrapper::grabbing_state(bool active)
{
	_is_grabbing = active;
	grabber_state = IRB_GRABBER_STATE::NONE;
	if (active)
	{
		grabber_state = IRB_GRABBER_STATE::GRABBING;
	}
	else{
		grabber_state = IRB_GRABBER_STATE::SRC_CONNECTED;
	}
	if (!disable_events)
		Fire_grabberDispatcherState((BYTE)grabber_state);
}

STDMETHODIMP CTRWrapper::GetRealTimeFrameRaster(
	DWORD frameId, 
	irb_frame_info* frame_info, 
	VARIANT* pixels, 
	VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = FALSE;

	auto frame = _extern_irb_frames_cache.get_frame_by_id(frameId);
	if (!frame)
	{
		return S_FALSE;
	}

	SAFEARRAY *pSA = pixels->parray;
	BYTE *pxls;
	SafeArrayAccessData(pSA, (void**)&pxls);
	irb_frame_image_dispatcher::temperature_span_t calibration_interval;
	auto result = 
		_image_dispatcher.get_formated_frame_raster(
				frame,
				reinterpret_cast<irb_frame_image_dispatcher::irb_frame_raster_ptr_t>(pxls),
				calibration_interval
				);
	SafeArrayUnaccessData(pSA);
	if (!result){
		return E_FAIL;
	}


	frame_info->image_info.width = frame->header.geometry.imgWidth;
	frame_info->image_info.height = frame->header.geometry.imgHeight;

	frame_info->measure.tmin = frame->min_temperature;
	frame_info->measure.tavr = frame->avr_temperature;
	frame_info->measure.tmax = frame->max_temperature;
	frame_info->measure.object_tmin = frame->spec.IRBmin;
	frame_info->measure.object_tmax = frame->spec.IRBmax;

	frame_info->measure.calibration_min = frame->header.calibration.tmin - 273.15f;
	frame_info->measure.calibration_max = frame->header.calibration.tmax - 273.15f;

	frame_info->coordinate.coordinate = frame->coords.coordinate;
	frame_info->coordinate.line = frame->coords.line;
	frame_info->coordinate.path = frame->coords.path;
	frame_info->timestamp = frame->get_frame_time_in_sec();
	*res = TRUE;
	return S_OK;
}


STDMETHODIMP 
CTRWrapper::GetNextRealTimeFrameRaster(
	DWORD* frameId, 
	irb_frame_info* frame_info,
	VARIANT* pixels,
	VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = FALSE;

	auto frame = _extern_irb_frames_cache.get_next_frame();
	if (!frame)
	{
		return S_FALSE;
	}

	SAFEARRAY *pSA = pixels->parray;
	BYTE *pxls;
	SafeArrayAccessData(pSA, (void**)&pxls);
	irb_frame_image_dispatcher::temperature_span_t calibration_interval;
	auto result =
		_image_dispatcher.get_formated_frame_raster(
		frame,
		reinterpret_cast<irb_frame_image_dispatcher::irb_frame_raster_ptr_t>(pxls),
		calibration_interval
		);
	SafeArrayUnaccessData(pSA);
	if (!result){
		return E_FAIL;
	}

	frame_info->image_info.width = frame->header.geometry.imgWidth;
	frame_info->image_info.height = frame->header.geometry.imgHeight;

	frame_info->measure.tmin = frame->min_temperature;
	frame_info->measure.tavr = frame->avr_temperature;
	frame_info->measure.tmax = frame->max_temperature;
	frame_info->measure.object_tmin = frame->spec.IRBmin;
	frame_info->measure.object_tmax = frame->spec.IRBmax;

	frame_info->measure.calibration_min = frame->header.calibration.tmin - 273.15f;
	frame_info->measure.calibration_max = frame->header.calibration.tmax - 273.15f;

	frame_info->coordinate.coordinate = frame->coords.coordinate;
	frame_info->coordinate.line = frame->coords.line;
	frame_info->coordinate.path = frame->coords.path;
	frame_info->timestamp = frame->get_frame_time_in_sec();

	*frameId = frame->id;

	*res = TRUE;
	return S_OK;
}

STDMETHODIMP
CTRWrapper::GetRealTimeFrame(
DWORD frameId,
irb_frame_info* frame_info,
VARIANT* pixels,
VARIANT* temp_values,
VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = FALSE;

	auto frame = _extern_irb_frames_cache.get_frame_by_id(frameId);
	if (!frame)
	{
		return S_FALSE;
	}

	SAFEARRAY *pSA = pixels->parray;
	SAFEARRAY *pSA2 = temp_values->parray;
	BYTE *pxls, *tempValues;
	SafeArrayAccessData(pSA, (void**)&pxls);
	std::memcpy(pxls, frame->pixels.get(), frame->get_pixels_data_size());
	SafeArrayUnaccessData(pSA);
	SafeArrayAccessData(pSA2, (void**)&tempValues);
	std::memcpy(tempValues, frame->header.calibration.tempvals, sizeof(frame->header.calibration.tempvals));
	SafeArrayUnaccessData(pSA2);

	frame_info->image_info.width = frame->header.geometry.imgWidth;
	frame_info->image_info.height = frame->header.geometry.imgHeight;

	//frame_info->image_info.firstValidX = frame->header.geometry.firstValidX;
	//frame_info->image_info.lastValidX = frame->header.geometry.lastValidX;
	//frame_info->image_info.firstValidY = frame->header.geometry.firstValidY;
	//frame_info->image_info.lastValidY = frame->header.geometry.lastValidY;

	frame_info->measure.tmin = 0;
	frame_info->measure.tavr = 0;
	frame_info->measure.tmax = 0;

	frame_info->measure.object_tmin = frame->spec.IRBmin;
	frame_info->measure.object_tmax = frame->spec.IRBmax;

	frame_info->measure.calibration_min = frame->header.calibration.tmin - 273.15f;
	frame_info->measure.calibration_max = frame->header.calibration.tmax - 273.15f;

	frame_info->coordinate.coordinate = frame->coords.coordinate;
	frame_info->coordinate.line = frame->coords.line;
	frame_info->coordinate.path = frame->coords.path;
	frame_info->timestamp = frame->get_frame_time_in_sec();

	*res = TRUE;
	return S_OK;
}


STDMETHODIMP
CTRWrapper::GetNextRealTimeFrame(
DWORD* frameId,
irb_frame_info* frame_info,
VARIANT* pixels,
VARIANT* temp_values,
VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = FALSE;

	auto frame = _extern_irb_frames_cache.get_next_frame();
	if (!frame)
	{
		return S_FALSE;
	}

	SAFEARRAY *pSA = pixels->parray;
	SAFEARRAY *pSA2 = temp_values->parray;
	BYTE *pxls,*tempValues;
	SafeArrayAccessData(pSA, (void**)&pxls);
	std::memcpy(pxls, frame->pixels.get(), frame->get_pixels_data_size());
	SafeArrayUnaccessData(pSA);
	SafeArrayAccessData(pSA2, (void**)&tempValues);
	std::memcpy(tempValues, frame->header.calibration.tempvals, sizeof(frame->header.calibration.tempvals));
	SafeArrayUnaccessData(pSA2);

	frame_info->image_info.width = frame->header.geometry.imgWidth;
	frame_info->image_info.height = frame->header.geometry.imgHeight;

	//frame_info->image_info.firstValidX = frame->header.geometry.firstValidX;
	//frame_info->image_info.lastValidX = frame->header.geometry.lastValidX;
	//frame_info->image_info.firstValidY = frame->header.geometry.firstValidY;
	//frame_info->image_info.lastValidY = frame->header.geometry.lastValidY;

	frame_info->measure.tmin = 0;
	frame_info->measure.tavr = 0;
	frame_info->measure.tmax = 0;

	frame_info->measure.object_tmin = frame->spec.IRBmin - 273.15f;
	frame_info->measure.object_tmax = frame->spec.IRBmax - 273.15f;

	frame_info->measure.calibration_min = frame->header.calibration.tmin - 273.15f;
	frame_info->measure.calibration_max = frame->header.calibration.tmax - 273.15f;

	frame_info->coordinate.coordinate = frame->coords.coordinate;
	frame_info->coordinate.line = frame->coords.line;
	frame_info->coordinate.path = frame->coords.path;
	frame_info->timestamp = frame->get_frame_time_in_sec();

	*frameId = frame->getFrameNum();


	*res = TRUE;
	return S_OK;
}


STDMETHODIMP CTRWrapper::get_pixel_temperature(DWORD frameId, USHORT x, USHORT y, FLOAT* tempToReturn, VARIANT_BOOL* res)
{
	*res = FALSE;

	auto frame = _extern_irb_frames_cache.get_frame_by_id(frameId);
	if (!frame)
	{
		return S_FALSE;
	}

	FLOAT temp;
	*res = frame->GetPixelTemp(x, y, &temp);

	if (*res)
		*tempToReturn = temp - 273.15f;

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetPallete(BSTR palleteFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	auto pallete_filename = W2A(palleteFileName);
	_image_dispatcher.set_palette(pallete_filename);

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetDefaultPallete(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_image_dispatcher.set_default_palette();

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetPaletteCalibrationMode(calibration_mode mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_image_dispatcher.set_calibration_type(static_cast<irb_frame_image_dispatcher::IMAGE_CALIBRATION_TYPE>(mode));
	return S_OK;
}

STDMETHODIMP CTRWrapper::SetPaletteCalibration(float min, float max)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_image_dispatcher.set_calibration_interval(min, max);

	return S_OK;

}

STDMETHODIMP CTRWrapper::StartRecord(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_irb_frames_cache->set_writer(
		std::make_unique<irb_frame_delegates::irb_frames_writer>
		(_grab_frames_dir,
		_grab_frames_file_pattern,
		std::bind(&CTRWrapper::new_irb_file, this, std::placeholders::_1)));

	return S_OK;
}

STDMETHODIMP CTRWrapper::StopRecord(BYTE unload, BYTE save)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!_is_grabbing)
	{
		if (unload == 1)
		{
			if (_grab_frames_dispatcher)
				_grab_frames_dispatcher->stop_grabbing(true);
		}
		return S_OK;
	}

	_irb_frames_cache->set_writer(std::unique_ptr<irb_frame_delegates::irb_frames_writer>());

	//Fire_grabberDispatcherState((BYTE)grabber_state);
	//close_pd_objects();

	return S_OK;
}
STDMETHODIMP CTRWrapper::StopGrabbing(BYTE unload, BYTE save)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!_is_grabbing)
	{
		if (unload == 1)
		{
			if (_grab_frames_dispatcher)
				_grab_frames_dispatcher->stop_grabbing(true);
		}
		return S_OK;
	}

	disable_events = true;
	if (_grab_frames_dispatcher)
		_grab_frames_dispatcher->stop_grabbing(unload == 1 ? true : false);
	_irb_frames_cache->reset();
	_irb_frames_cache->set_writer(std::unique_ptr<irb_frame_delegates::irb_frames_writer>());
	_extern_irb_frames_cache.stop_cache();
	disable_events = false;

	//Fire_grabberDispatcherState((BYTE)grabber_state);
	//close_pd_objects();

	return S_OK;
}
STDMETHODIMP CTRWrapper::ClearGrabbingCache()
{
	_irb_frames_cache->reset();

	return S_OK;
}

STDMETHODIMP CTRWrapper::FinishAll(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	disable_events = true;
	StopGrabbing(0,1);
	VARIANT_BOOL res;
	DisconnectCamera(&res);

	StopRecieveCoordinates();

	_thread_exception_handler->stop_processing();

	disable_events = false;

	return S_OK;
}

STDMETHODIMP CTRWrapper::StartGrabbing(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	disable_events = false;

	if (_is_grabbing)
		return S_FALSE;

	_extern_irb_frames_cache.stop_cache(); 
	_extern_irb_frames_cache.start_cache(
		irb_frames_cache::new_irb_frame_process_func_t()
		//std::bind(&CTRWrapper::process_new_frame, this, std::placeholders::_1)
		);
	_grab_frames_dispatcher->start_grabbing(std::bind(&CTRWrapper::grabbing_state, this, std::placeholders::_1));
	
	return S_OK;
}


STDMETHODIMP CTRWrapper::ShowCamSettings(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (_grab_frames_dispatcher)
		_grab_frames_dispatcher->show_settigs(true);

	return S_OK;
}


STDMETHODIMP CTRWrapper::HideCamSettings(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (_grab_frames_dispatcher)
		_grab_frames_dispatcher->show_settigs(false);

	return S_OK;
}


STDMETHODIMP CTRWrapper::ConnectCamera(BYTE initMode, VARIANT_BOOL* res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = TRUE;
	if (_is_grabbing)
		return S_FALSE;

	init_pd_objects();
	init_grabber_dispatcher();
	if (_grab_frames_dispatcher)
	{
		auto result = _grab_frames_dispatcher->init_grabber_connection((int)initMode);
		auto last_error = _grab_frames_dispatcher->last_error();
		if (!result)
		{
			*res = FALSE;
			Fire_grabberDispatcherError(_com_util::ConvertStringToBSTR(last_error.c_str()));
		}
		else
		{
			if (!disable_events)
				Fire_grabberDispatcherState((BYTE)IRB_GRABBER_STATE::SRC_CONNECTED);
		}
	}

	return S_OK;
}
STDMETHODIMP CTRWrapper::DisconnectCamera(VARIANT_BOOL* res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*res = TRUE;
	if (_grab_frames_dispatcher)
	{
		auto result = _grab_frames_dispatcher->close_grabber_connection();
		auto last_error = _grab_frames_dispatcher->last_error();
		if (!result)
		{
			*res = FALSE;
			Fire_grabberDispatcherError(_com_util::ConvertStringToBSTR(last_error.c_str()));
		}
		else
		{
			if (!disable_events)
				Fire_grabberDispatcherState((BYTE)IRB_GRABBER_STATE::INITED);
		}
	}

	return S_OK;
}

STDMETHODIMP CTRWrapper::GetPallete(VARIANT* pallete)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(pallete == NULL)
		return S_OK;

	auto safe_array = pallete->parray;
	auto pallete_size = _image_dispatcher.get_palette_size();
	if ((ULONG)pallete_size > safe_array->rgsabound->cElements)
		return E_BOUNDS;

	auto pallete_image = reinterpret_cast<const DWORD *>(_image_dispatcher.get_palette_image());
	DWORD *data;

	SafeArrayAccessData(safe_array, (void**)&data);
	std::memcpy(data, pallete_image, pallete_size*sizeof(DWORD));
	SafeArrayUnaccessData(pallete->parray);

	return S_OK;
}


STDMETHODIMP CTRWrapper::GetPalleteLength(ULONG32* number_colors, SHORT* len)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*len = _image_dispatcher.get_palette_size();
	*number_colors = _image_dispatcher.get_palette().numI;

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetGrabberPath(BSTR grabberPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	_grab_frames_dir = _com_util::ConvertBSTRToString(grabberPath);

	return S_OK;
}


STDMETHODIMP CTRWrapper::AddArea(SHORT id, area_info* area)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (area->width == 0 || area->heigth == 0)
		return E_INVALIDARG;

	switch (area->type){
	case RECTANGLE:
	{
					  AreaRect rect;
					  rect.id = id;
					  rect.m_x = area->x0;
					  rect.m_y = area->y0;
					  rect.m_width = area->width;
					  rect.m_height = area->heigth;

					  _image_dispatcher.AddAreaRect(rect);

					  break;
	}
	case ELLIPSE:
	{
					AreaEllips rect(area->x0, area->y0, area->width, area->heigth);
					rect.id = id;

					_image_dispatcher.AddAreaEllips(rect);

					break;
	}
	default:
		return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CTRWrapper::AreaChanged(SHORT id, area_info* area)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (area->width == 0 || area->heigth == 0)
		return E_INVALIDARG;

	switch (area->type){
	case RECTANGLE:
	{
					  AreaRect rect;
					  rect.id = id;
					  rect.m_x = area->x0;
					  rect.m_y = area->y0;
					  rect.m_width = area->width;
					  rect.m_height = area->heigth;

					  _image_dispatcher.ChangeRectArea(id, rect);

					  break;
	}
	case ELLIPSE:
	{
					AreaEllips rect(area->x0, area->y0, area->width, area->heigth);
					rect.id = id;

					_image_dispatcher.ChangeEllipsArea(id, rect);

					break;
	}
	default:
		return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CTRWrapper::RemoveArea(SHORT id, area_type* type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_image_dispatcher.DelArea(id);

	return S_OK;
}

STDMETHODIMP CTRWrapper::RemoveAllAreas(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_image_dispatcher.clear_areas();

	return S_OK;
}


STDMETHODIMP CTRWrapper::GetAreaInfo(ULONG aid, area_temperature_measure* measure, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto res = _image_dispatcher.get_area_temperature_measure(aid, *measure);

	*result = FALSE;
	if (res){
		*result = TRUE;
	}

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetMaxFramesInIRBFile(USHORT frames_number)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_irb_frames_cache->set_max_frames_in_cache(frames_number);

	return S_OK;
}


STDMETHODIMP CTRWrapper::SetBlockCamFrame(BYTE blockFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return S_OK;
}

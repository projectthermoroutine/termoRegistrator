// TRWrapper.cpp: реализация CTRWrapper

#include "stdafx.h"
#include "CMovieTransit.h"
#include "_IThermoLibErrorEvents_CP.h"

#include <comutil.h>
#include <memory>
#include "movie_transit.h"
#include "irb_frame_filter.h"
#include "structures.h"

#include <loglib\log.h>
#include <common\string_utils.h>
#include <common\on_exit.h>

using namespace movie_transit_ns;

CMovieTransit::CMovieTransit() :
disable_events(false),
_cur_frame_id(0),
_camera_offset(0)
{
	_movie_transit = std::make_unique<movie_transit>();
	_movie_transit->set_default_palette();
}

CMovieTransit::~CMovieTransit()
{
	Close();
}


STDMETHODIMP CMovieTransit::ClearMovieTransitCache()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->clear_cache();
	return S_OK;
}
STDMETHODIMP CMovieTransit::Reset()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->reset();
	return S_OK;
}


STDMETHODIMP 
CMovieTransit::GetFramePositionInfo(
	ULONG frame_id, 
	frame_coordinate *frameCoordinate, 
	double* timestamp,
	VARIANT_BOOL* result
)
{
	*timestamp = 0.0;
	*result = FALSE;
	auto frame = _movie_transit->get_frame_by_index(frame_id);
	if (!frame)
		return S_FALSE;

	const auto & frame_coords = frame->coords;
	frameCoordinate->coordinate = frame_coords.coordinate;

	frameCoordinate->path = ::SysAllocString(frame_coords.path.c_str());
	frameCoordinate->line = ::SysAllocString(frame_coords.line.c_str());

	frameCoordinate->direction = frame_coords.direction;
	*timestamp = frame->get_frame_time_in_sec();

	*result = TRUE;
	return S_OK;
}
STDMETHODIMP CMovieTransit::GetCurrentFramePositionInfo(frame_coordinate *frameCoordinate, double* timestamp)
{
	*timestamp = 0;

	auto frame = _movie_transit->current_irb_frame();
	if (!frame)
		return S_FALSE;

	const auto & frame_coords = frame->coords;
	frameCoordinate->coordinate = frame_coords.coordinate;
	frameCoordinate->path = ::SysAllocString(frame_coords.path.c_str());
	frameCoordinate->line = ::SysAllocString(frame_coords.line.c_str());
	frameCoordinate->direction = frame_coords.direction;

	*timestamp = frame->get_frame_time_in_sec();

	return S_OK;
}

STDMETHODIMP CMovieTransit::SetIRBFiles(VARIANT filesNames, SAFEARRAY **errors, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*result = FALSE;
	if (filesNames.vt != (VT_ARRAY | VT_BSTR))
	{
		return E_INVALIDARG;
	}
	USES_CONVERSION;
	
	SAFEARRAY *pSA = filesNames.parray;

	LONG lBound;
	LONG uBound;

	SafeArrayGetLBound(pSA, 1, &lBound);
	SafeArrayGetUBound(pSA, 1, &uBound);

	std::vector<std::wstring> file_names;
	for (long i = lBound; i <= uBound; i++)
	{
		BSTR str;
		SafeArrayGetElement(pSA, &i, &str);
		file_names.push_back(std::wstring(str));
	}

	if (!file_names.empty())
	{
		_movie_transit->reset();
	}


	SAFEARRAYBOUND bounds = { (ULONG)file_names.size(), 0 };
	*errors = SafeArrayCreate(VT_BSTR, 1, &bounds);
	long i = -1;
	*result = TRUE;
	HRESULT hresult = S_OK;
	_camera_offset = 0;
	irb_files_list_t irb_files_list;
	for (auto &file_name : file_names)
	{
		i++;
		try{
			irb_files_list.emplace_back(std::make_shared<IRBFile>(file_name));
			SafeArrayPutElement(*errors, &i, _com_util::ConvertStringToBSTR("OK"));

		}
		catch (const irb_file_exception& exc)
		{
			SafeArrayPutElement(*errors, &i, _com_util::ConvertStringToBSTR(exc.what()));
			*result = FALSE;
			hresult = S_FALSE;
			continue;
		}
		catch (...)
		{
			SafeArrayPutElement(*errors, &i, _com_util::ConvertStringToBSTR("Unexpected ERROR while irb files adding to the movie project."));
			hresult = S_FALSE;
			*result = FALSE;
			continue;
		}
	}
	if (irb_files_list.empty())
	{
		return S_FALSE;
	}

	_camera_offset = read_camera_offset_from_file(*irb_files_list[0]);

	_movie_transit->add_irb_files(irb_files_list);

	if (_movie_transit->Open())
		return hresult;

	return E_FAIL;
}


STDMETHODIMP CMovieTransit::FilesCount(SHORT* filesCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*filesCount = (short)_movie_transit->number_irb_files();

	return S_OK;
}


STDMETHODIMP CMovieTransit::FramesCount(LONG* framesCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*framesCount = _movie_transit->number_frames_in_files();

	return S_OK;
}

STDMETHODIMP
CMovieTransit::GetFrameRaster(
	ULONG32 frameNum,
	irb_frame_info* frame_info,
	VARIANT* frameRaster,
	VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*res = FALSE;

	try{
		_movie_transit->Go_to_frame_by_index((DWORD)frameNum, FILTER_SEARCH_TYPE::FILTER_NO);


		SAFEARRAY *pSA = frameRaster->parray;
		BYTE *pxls;
		SafeArrayAccessData(pSA, (void**)&pxls);
		irb_frame_image_dispatcher::temperature_span_t calibration_interval;
		auto result =
			_movie_transit->get_formated_frame_raster_by_index(
			frameNum,
			reinterpret_cast<irb_frame_image_dispatcher::irb_frame_raster_ptr_t>(pxls),
			calibration_interval
			);
		SafeArrayUnaccessData(pSA);
		if (!result){
			return E_FAIL;
		}
	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}
	auto frame = _movie_transit->current_irb_frame();
	if (frame)
	{
		fill_frame_info(*frame_info, *frame);
		frame_info->coordinate.camera_offset = _camera_offset;

		*res = TRUE;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP
CMovieTransit::GetFrame(
ULONG32 frameNum,
irb_frame_info* frame_info,
VARIANT* frameRaster, 
VARIANT* temp_values,
VARIANT_BOOL* res
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*res = FALSE;
	try{
	_movie_transit->Go_to_frame_by_index((DWORD)frameNum, FILTER_SEARCH_TYPE::FILTER_NO);

	auto frame = _movie_transit->current_irb_frame();
	if (!frame)
		return S_FALSE;

	SAFEARRAY *pSA = frameRaster->parray;
	SAFEARRAY *pSA2 = temp_values->parray;
	BYTE *pxls, *tempValues;
	SafeArrayAccessData(pSA, (void**)&pxls);
	std::memcpy(pxls, frame->pixels.get(), frame->get_pixels_data_size());
	SafeArrayUnaccessData(pSA);
	SafeArrayAccessData(pSA2, (void**)&tempValues);
	std::memcpy(tempValues, frame->header.calibration.tempvals, sizeof(frame->header.calibration.tempvals));
	SafeArrayUnaccessData(pSA2);

	fill_frame_info(*frame_info, *frame);

	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}



	frame_info->coordinate.camera_offset = _camera_offset;

	*res = TRUE;
	return S_OK;
}

STDMETHODIMP CMovieTransit::get_pixel_temperature(DWORD frameIndex, USHORT x, USHORT y, FLOAT* tempToReturn, VARIANT_BOOL* res)
{
	*res = FALSE;
	
	try{
		auto frame = _movie_transit->get_frame_by_index(frameIndex);
		if (!frame)
		{
			return S_FALSE;
		}

		FLOAT temp;
		*res = frame->GetPixelTemp(x, y, &temp);


		if (*res)
			*tempToReturn = temp - 273.15f;

	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}



	return S_OK;
}



STDMETHODIMP CMovieTransit::SetPallete(BSTR palleteFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	auto pallete_filename = W2A(palleteFileName);
	if (!_movie_transit->set_palette(pallete_filename))
		return S_FALSE;

	return S_OK;
}


STDMETHODIMP CMovieTransit::SetDefaultPallete(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->set_default_palette();

	return S_OK;
}
STDMETHODIMP CMovieTransit::SaveCurrentFrame(BSTR path, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	*result = _movie_transit->SaveCurr(path);
	return S_OK;
}


STDMETHODIMP CMovieTransit::SaveFrame(ULONG index, BSTR deviceName, ULONG picket, ULONG offset, BSTR filename, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
	try{
		*result = _movie_transit->save_frame(index,
			std::string(std::unique_ptr<char>(_com_util::ConvertBSTRToString(deviceName)).get()),
			picket, offset,
			filename
			);
	}
	catch (const irb_file_helper::irb_file_exception&)
	{
		*result = FALSE;
	}
	return S_OK;
}

STDMETHODIMP CMovieTransit::SaveOneFrame(ULONG index, BSTR filename, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
try{
	*result = _movie_transit->save_frame(index,	filename);
}
catch (const irb_file_helper::irb_file_exception&)
{
	*result = FALSE;
}

	return S_OK;
}


STDMETHODIMP CMovieTransit::GetCurrentFrameRaster(VARIANT* raster, irb_frame_info* frame_info, VARIANT_BOOL* res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*res = FALSE;

	try{
		SAFEARRAY *pSA = raster->parray;
		BYTE *pxls;
		SafeArrayAccessData(pSA, (void**)&pxls);
		irb_frame_image_dispatcher::temperature_span_t calibration_interval;
		auto result =
			_movie_transit->get_formated_current_frame_raster(
			reinterpret_cast<irb_frame_image_dispatcher::irb_frame_raster_ptr_t>(pxls),
			calibration_interval);
		SafeArrayUnaccessData(pSA);
		if (!result){
			return E_FAIL;
		}

		auto frame = _movie_transit->current_irb_frame();
		if (frame)
		{
			fill_frame_info(*frame_info, *frame);
			frame_info->coordinate.camera_offset = _camera_offset;

			*res = TRUE;
			return S_OK;
		}
	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}

	return S_FALSE;
}

STDMETHODIMP CMovieTransit::AddArea(SHORT id, area_info* area)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (area->width == 0 || area->heigth == 0)
		return E_INVALIDARG;

	try{
		bool res = false;
		switch (area->type){
		case RECTANGLE:
		{
			AreaRect rect;
			rect.id = id;
			rect.m_x = area->x0;
			rect.m_y = area->y0;
			rect.m_width = area->width;
			rect.m_height = area->heigth;

			res = _movie_transit->AddAreaRect(rect);

			break;
		}
		case ELLIPSE:
		{
			AreaEllips rect(area->x0, area->y0, area->width, area->heigth);
			rect.id = id;

			res = _movie_transit->AddAreaEllips(rect);

			break;
		}
		}

		if (res)
			return S_OK;
	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}


	return E_INVALIDARG;
}

STDMETHODIMP CMovieTransit::AreaChanged(SHORT id, area_info* area)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (area->width == 0 || area->heigth == 0)
		return E_INVALIDARG;

	try{
	switch (area->type){
	case RECTANGLE:
	{
					  AreaRect rect;
					  rect.id = id;
					  rect.m_x = area->x0;
					  rect.m_y = area->y0;
					  rect.m_width = area->width;
					  rect.m_height = area->heigth;

					  _movie_transit->ChangeRectArea(id,rect);

					  break;
	}
	case ELLIPSE:
	{
					AreaEllips rect(area->x0, area->y0, area->width, area->heigth);
					rect.id = id;

					_movie_transit->ChangeEllipsArea(id,rect);

					break;
	}
	default:
		return S_FALSE;
	}
	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CMovieTransit::RemoveArea(SHORT id, area_type* type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->DelArea(id);

	return S_OK;
}

STDMETHODIMP CMovieTransit::GetAreasInfo(area_temperature_measure_result **results, ULONG size)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	area_temperature_measure_result *result = reinterpret_cast<area_temperature_measure_result *>(results);

	for (ULONG i = 0; i < size; i++)
	{
		if (result == nullptr)
			return E_POINTER;
		auto res = _movie_transit->get_area_temperature_measure(result->area_id, result->measure);
		result->result_is_valid = res;
		result++;
	}

	return S_OK;
}

STDMETHODIMP CMovieTransit::GetAreaInfo(ULONG aid, area_temperature_measure* measure, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto res = _movie_transit->get_area_temperature_measure(aid, *measure);

	*result = FALSE;
	if (res){
		*result = TRUE;
	}

	return S_OK;
}


STDMETHODIMP CMovieTransit::GetCurFrameTemperatures(temperature_measure* measure)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->CurFrameTemperaturesCompute();

	auto current_irb_frame = _movie_transit->current_irb_frame();
	if (current_irb_frame){

		measure->tmin = current_irb_frame->min_temperature;
		measure->tavr = current_irb_frame->avr_temperature;
		measure->tmax = current_irb_frame->max_temperature;
		measure->object_tmin = current_irb_frame->spec.IRBmin;
		measure->object_tmax = current_irb_frame->spec.IRBmax;

		measure->calibration_min = current_irb_frame->header.calibration.tmin - 273.15f;
		measure->calibration_max = current_irb_frame->header.calibration.tmax - 273.15f;
	}


	return S_OK;
}

STDMETHODIMP CMovieTransit::RemoveAllAreas(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->remove_all_areas();

	return S_OK;
}


STDMETHODIMP CMovieTransit::GetTimeString(BSTR* timeString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	//*timeString = SysAllocString(_movie_transit->GetStartTime().Format("%d %m %Y"));

	return S_OK;
}

STDMETHODIMP CMovieTransit::GetDateTimeString(BSTR* dateTimeString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	//*dateTimeString = SysAllocString(_movie_transit->GetStartTime().Format("%d %m %Y  %H : %M"));

	return S_OK;
}

STDMETHODIMP CMovieTransit::InitFrameFilter(FLOAT* timeFrom, FLOAT* timeTo, FLOAT* tempObj, FLOAT* tempArea, SHORT* pickFrom, SHORT* pickTo, LONG* flags, VARIANT_BOOL* checkedOnly, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*result = FALSE;

	auto filter_flags = _movie_transit->get_filter_flags();
	irb_frame_filter::FILTER filter;

	_movie_transit->get_filter_params(filter);

	*flags = filter_flags;

	if (filter_flags & FILTER_TIME0)
		*timeFrom = (float)filter.time.first;

	if (filter_flags & FILTER_TIME1)
		*timeTo = (float)filter.time.second;

	if (filter_flags & FILTER_TEMPFULL)
		*tempObj = filter.T.first;

	if (filter_flags & FILTER_TEMPREGION)
		*tempArea = filter.T.second;

	if (filter.flags & FILTER_METKA)
		*checkedOnly = TRUE;

	*result = TRUE;

	return S_OK;
}


STDMETHODIMP CMovieTransit::SetFrameFilter(FLOAT timeFrom, FLOAT timeTo, FLOAT tempObj, FLOAT tempArea, SHORT picketFrom, SHORT picketTo, LONG flags, VARIANT_BOOL* res)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	irb_frame_filter::FILTER filter;

	filter.flags = flags;

	if (timeFrom != 0.0f && timeTo != 0.0f &&
		timeFrom <= timeTo)
		filter.time = { timeFrom, timeTo };
	else
		filter.time = { timeTo, timeFrom };


	filter.T = { tempObj, tempArea };

	if (picketFrom <= picketTo)
		filter.picket = { picketFrom, picketTo };
	else
		filter.picket = { picketTo, picketFrom };


	_movie_transit->SetFilter(filter);

	return S_OK;
}

STDMETHODIMP CMovieTransit::IsFrameMeetFilter(ULONG32 frameNum, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//	_movie_transit->mark_frame_in_filter(frameNum, false);
	*result = FALSE;
	try{
		auto res = _movie_transit->IsFilterYes(frameNum);

		//if (res)
		//	_movie_transit->mark_frame_in_filter(frameNum, true);
		*result = res;
	}
	catch (const CMemoryException&)
	{
		_movie_transit->clear_cache();
		return S_FALSE;
	}


	return S_OK;
}

STDMETHODIMP CMovieTransit::GetPallete(VARIANT* pallete)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pallete == NULL)
		return S_OK;

	auto safe_array = pallete->parray;
	auto pallete_size = _movie_transit->get_palette_size();
	if ((ULONG)pallete_size > safe_array->rgsabound->cElements)
		return E_BOUNDS;

	auto pallete_image = reinterpret_cast<const DWORD *>(_movie_transit->get_palette_image());
	DWORD *data;

	SafeArrayAccessData(safe_array, (void**)&data);
	std::memcpy(data, pallete_image, pallete_size*sizeof(DWORD));
	SafeArrayUnaccessData(pallete->parray);

	return S_OK;
}


STDMETHODIMP CMovieTransit::GetPalleteLength(ULONG32* number_colors, SHORT* len)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*len = _movie_transit->get_palette_size();
	*number_colors = _movie_transit->get_palette_colors_number();

	return S_OK;
}

STDMETHODIMP CMovieTransit::SetPaletteCalibrationMode(calibration_mode mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->set_palette_calibration_mode(mode);
	return S_OK;
}

STDMETHODIMP CMovieTransit::SetPaletteCalibration(float min, float max)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->set_palette_calibration(min,max);

	return S_OK;

}
STDMETHODIMP CMovieTransit::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return S_OK;
}

STDMETHODIMP CMovieTransit::WriteCameraOffset(LONG32 offset)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_movie_transit->write_camera_offset(offset);

	return S_OK;
}

STDMETHODIMP CMovieTransit::SaveIrbFrames(VARIANT framesIndexes, BSTR fileNamePattern, USHORT framesPerFile, VARIANT_BOOL* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*result = FALSE;
	if (framesIndexes.vt != (VT_ARRAY | VT_UINT))
	{
		return E_INVALIDARG;
	}
	USES_CONVERSION;

	SAFEARRAY *pSA = framesIndexes.parray;

	LONG lBound;
	LONG uBound;

	SafeArrayGetLBound(pSA, 1, &lBound);
	SafeArrayGetUBound(pSA, 1, &uBound);

	int number_frames = uBound - lBound + 1;

	UINT *values;
	SafeArrayAccessData(pSA, (void**)&values);
	utils::on_exit exit_guard([pSA](){ SafeArrayUnaccessData(pSA); });

	std::vector<uint32_t> frames_indexes(number_frames);
	for (long i = lBound; i <= uBound; i++)
	{
		frames_indexes.push_back(values[i]);
	}

	try{
		if (_movie_transit->SaveFrames(frames_indexes, fileNamePattern, framesPerFile))
			*result = TRUE;
	}
	catch (const irb_file_exception&)
	{
		*result = FALSE;
		return E_FAIL;
	}

	return S_OK;
}



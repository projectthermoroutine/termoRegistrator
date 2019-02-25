#define NOMINMAX
#include "irb_frame_helper.h"
#include <iostream>
#include <tuple>
#include <common\string_utils.h>
#include <ppl.h>
#include <mutex>

#include <loglib\log.h>


namespace irb_frame_helper
{


#define EVENT_CODE_PAGE	1251
#define CONVERT_TO_UTF16_FROM_ANSI(_ansi_str) string_utils::ConvertToUTF16((_ansi_str),EVENT_CODE_PAGE)


#define MAX_NAME_LENGTH_CB  64
#define CONVERT_TO_UTF8(_utf16_str)  string_utils::convert_wchar_to_utf8((_utf16_str))
#define CONVERT_TO_UTF16(_utf8_str)  string_utils::convert_utf8_to_wchar((_utf8_str))


#pragma pack(push,1)


	typedef struct _FrameCoordPresentation_v1
	{
		coordinate_t coordinate;
		char line[MAX_NAME_LENGTH_CB];
		char path[MAX_NAME_LENGTH_CB];
		direction_t direction;

	}FrameCoordPresentation_v1;



	typedef struct _FrameCoordPresentation_v3
	{
		coordinate_t coordinate;
		char line[MAX_NAME_LENGTH_CB];
		char path[MAX_NAME_LENGTH_CB];
		char railway[MAX_NAME_LENGTH_CB];
		direction_t direction;
		picket_t picket;
		offset_t offset;
		camera_offset_t camera_offset;
		counter_t counter;

	}FrameCoordPresentation_v3;

	typedef struct _FrameCoordPresentation_v2
	{
		coordinate_t coordinate;
		char line[MAX_NAME_LENGTH_CB];
		char path[MAX_NAME_LENGTH_CB];
		char railway[MAX_NAME_LENGTH_CB];
		direction_t direction;
		picket_t picket;
		offset_t offset;
		camera_offset_t camera_offset;
		counter_t counter;

	}FrameCoordPresentation_v2;

	typedef struct _FrameCoordPresentation_v4
	{
		coordinate_t coordinate;
		char line[MAX_NAME_LENGTH_CB];
		char path[MAX_NAME_LENGTH_CB];
		char railway[MAX_NAME_LENGTH_CB];
		direction_t direction;
		picket_t picket;
		offset_t offset;
		camera_offset_t camera_offset;
		counter_t counter;
		counter_size_t counter_size;
		path_type_t path_type;

	}FrameCoordPresentation_v4;


	typedef FrameCoordPresentation_v4 FrameCoordPresentation;


#pragma pack(pop)

	uint32_t get_size_frame_coordinates()
	{
		return (uint32_t)sizeof(FrameCoordPresentation);
	}

	uint32_t get_frame_time_offset()
	{
		return sizeof(IRBFrameGeometry)+sizeof(IRBFrameObjectParametrs)+sizeof(IRBFrameCallibration)+FIELD_OFFSET(IRBFramePresentation, imgTime);
	}

	uint32_t get_frame_coordinate_type_offset()
	{
		return 0ul;
	}

#pragma pack(push,1)
	typedef struct _pixel_data
	{
		uint8_t number;
		uint8_t value;
	}pixel_data, *pixel_data_ptr;
#pragma pack(pop)

	bool LHDecode(ULONG pixels_number, BYTE *pixelsToDecode, ULONG dataSize, irb_frame_pixels_t &decodedPixels)
	{

		if (pixelsToDecode == nullptr || dataSize == 0 || pixels_number == 0)
			return false;

		if (pixels_number >= dataSize)
			return false;

		decodedPixels = std::make_unique<irb_pixel_t[]>(pixels_number);

		ULONG offset = 0;
		ULONG i = 0;
		//		std::memcpy((void*)decodedPixels.data(), (void*)(pixelsToDecode), pixels_number);
		for (i = 0; i < pixels_number; ++i)
		{
			decodedPixels[i] = (irb_pixel_t)pixelsToDecode[i];
		}

		offset = 1;
		for (i = pixels_number; i < dataSize - 1; i += 2)
		{
			pixel_data_ptr pixel_data = reinterpret_cast<pixel_data_ptr>(&pixelsToDecode[i]);
			for (int j = 0; j < pixel_data->number; ++j, ++offset)
			{
				if (offset < pixels_number)
					decodedPixels[offset] = pixel_data->value;
			}
		}

		return true;
	}

	bool Decode8Bit(BYTE *pixelsToDecode, ULONG dataSize, irb_frame_pixels_t &decodedPixels)
	{
		/*if (decodedPixels.size() != dataSize)
			return false;*/

		if (pixelsToDecode == nullptr || dataSize == 0)
			return false;

		ULONG offset = 0;
		while (offset < dataSize)
		{
			std::memcpy((void*)(&decodedPixels[offset]), (void*)(pixelsToDecode + offset), 1);
			++offset;
		}

		return true;
	}

	bool Decode16Bit(const irb_pixel_t *pixelsToDecode, ULONG dataSize, irb_frame_pixels_t &decodedPixels)
	{
		/*if (decodedPixels.size() != dataSize)
			return false;*/

		if (pixelsToDecode == nullptr || dataSize == 0)
			return false;

		std::memcpy((void*)decodedPixels.get(), (void*)pixelsToDecode, dataSize);

		return true;
	}


	std::istream & operator>>(std::istream & in, IRBFrame &irb_frame)
	{
		auto begin = in.tellg();

		in.read(reinterpret_cast<char*>(&irb_frame.header), sizeof(IRBFrameHeader));

		ULONG32 pixels_data_size = 0;
	//	in.read(reinterpret_cast<char*>(&pixels_data_size), sizeof(pixels_data_size));

		auto pixelFormat = irb_frame.header.geometry.pixelFormat;
		auto compression = irb_frame.header.geometry.compression;

		if (pixelFormat != 1 && pixelFormat != 2){
			in.seekg(begin, std::ios::beg);
			return in;
		}
		if (compression != 0 && compression != 1){
			in.seekg(begin, std::ios::beg);
			return in;
		}

		auto decode_type = pixelFormat + compression;

		if (decode_type == 2)
		{
			irb_frame.pixels = std::make_unique<irb_pixel_t[]>(irb_frame.get_pixels_count());
			in.read(reinterpret_cast<char*>(irb_frame.pixels.get()), irb_frame.get_pixels_count()*decode_type);
		}
		else
		{
			pixels_data_size = irb_frame.get_pixels_count()*pixelFormat;

			std::unique_ptr<BYTE[]> p_pixels(std::make_unique<BYTE[]>(pixels_data_size));
			in.read(reinterpret_cast<char*>(p_pixels.get()), pixels_data_size);

			bool res = false;
			switch (decode_type){
			case 1:
				irb_frame.pixels = std::make_unique<irb_pixel_t[]>(irb_frame.get_pixels_count());
				//irb_frame.pixels.resize(irb_frame.get_pixels_count());
				res = Decode8Bit(p_pixels.get(), pixels_data_size, irb_frame.pixels);
				break;
			case 3:
				res = LHDecode(irb_frame.get_pixels_count(), p_pixels.get(), pixels_data_size, irb_frame.pixels);
				break;

			};
		}

		return in;
	}

	std::ostream & operator<<(std::ostream & out,const IRBFrame &irb_frame)
	{
		out.write(reinterpret_cast<const char*>(&irb_frame.header), sizeof(IRBFrameHeader));
		ULONG32 pixels_data_size = irb_frame.header.geometry.pixelFormat*irb_frame.get_pixels_count();
		out.write(reinterpret_cast<const char*>(irb_frame.pixels.get()), pixels_data_size);

		return out;
	}

	std::istream & operator>>(std::istream & in, FrameCoord_v1 &frame_coordinate)
	{

		FrameCoordPresentation_v1 coords;
		in.read(reinterpret_cast<char*>(&coords), sizeof(FrameCoordPresentation_v1));
		frame_coordinate.coord.coordinate = coords.coordinate;
		frame_coordinate.coord.direction = coords.direction;
		bool is_ansi_str = false;
		try{
			frame_coordinate.coord.path = CONVERT_TO_UTF16(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16(coords.line);
		}
		catch (const std::range_error&)
		{
			is_ansi_str = true;
		}
		if (is_ansi_str){
			frame_coordinate.coord.path = CONVERT_TO_UTF16_FROM_ANSI(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16_FROM_ANSI(coords.line);
		}
		return in;
	}

	std::istream & operator>>(std::istream & in, FrameCoord_v2 &frame_coordinate)
	{

		FrameCoordPresentation_v2 coords;
		in.read(reinterpret_cast<char*>(&coords), sizeof(FrameCoordPresentation_v2));
		frame_coordinate.coord.coordinate = coords.coordinate;
		frame_coordinate.coord.direction = coords.direction;
		frame_coordinate.coord.camera_offset = coords.camera_offset;
		frame_coordinate.coord.counter = coords.counter;
		bool is_ansi_str = false;
		try{
			frame_coordinate.coord.path = CONVERT_TO_UTF16(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16(coords.line);
			frame_coordinate.coord.railway = CONVERT_TO_UTF16(coords.railway);
		}
		catch (const std::range_error&)
		{
			is_ansi_str = true;
		}
		if (is_ansi_str){
			frame_coordinate.coord.path = CONVERT_TO_UTF16_FROM_ANSI(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16_FROM_ANSI(coords.line);
			frame_coordinate.coord.railway = CONVERT_TO_UTF16_FROM_ANSI(coords.railway);
		}


		return in;
	}

	std::istream & operator>>(std::istream & in, FrameCoord_v3 &frame_coordinate)
	{

		FrameCoordPresentation_v3 coords;
		in.read(reinterpret_cast<char*>(&coords), sizeof(FrameCoordPresentation_v3));
		frame_coordinate.coord.coordinate = coords.coordinate;
		frame_coordinate.coord.direction = coords.direction;
		frame_coordinate.coord.camera_offset = coords.camera_offset;
		frame_coordinate.coord.picket = coords.picket;
		frame_coordinate.coord.offset = coords.offset;
		frame_coordinate.coord.counter = coords.counter;
		bool is_ansi_str = false;
		try{
			frame_coordinate.coord.path = CONVERT_TO_UTF16(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16(coords.line);
			frame_coordinate.coord.railway = CONVERT_TO_UTF16(coords.railway);
		}
		catch (const std::range_error&)
		{
			is_ansi_str = true;
		}

		if (is_ansi_str){
			frame_coordinate.coord.path = CONVERT_TO_UTF16_FROM_ANSI(coords.path);
			frame_coordinate.coord.line = CONVERT_TO_UTF16_FROM_ANSI(coords.line);
			frame_coordinate.coord.railway = CONVERT_TO_UTF16_FROM_ANSI(coords.railway);

		}

		return in;
	}


	std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate)
	{

		FrameCoordPresentation coords;
		in.read(reinterpret_cast<char*>(&coords), sizeof(FrameCoordPresentation));
		frame_coordinate.coordinate = coords.coordinate;
		frame_coordinate.direction = coords.direction;
		frame_coordinate.camera_offset = coords.camera_offset;
		frame_coordinate.picket = coords.picket;
		frame_coordinate.offset = coords.offset;
		frame_coordinate.counter = coords.counter;
		frame_coordinate.counter_size = coords.counter_size;
		frame_coordinate.path_type = coords.path_type;
		bool is_ansi_str = false;
		try{
			frame_coordinate.path = CONVERT_TO_UTF16(coords.path);
			frame_coordinate.line = CONVERT_TO_UTF16(coords.line);
			frame_coordinate.railway = CONVERT_TO_UTF16(coords.railway);
		}
		catch (const std::range_error&)
		{
			is_ansi_str = true;
		}

		if (is_ansi_str){
			frame_coordinate.path = CONVERT_TO_UTF16_FROM_ANSI(coords.path);
			frame_coordinate.line = CONVERT_TO_UTF16_FROM_ANSI(coords.line);
			frame_coordinate.railway = CONVERT_TO_UTF16_FROM_ANSI(coords.railway);

		}

		return in;
	}

	std::ostream & operator<<(std::ostream & out, const FrameCoord &frame_coordinate)
	{
		FrameCoordPresentation coords;
		ZeroMemory(&coords, sizeof(FrameCoordPresentation));
		coords.coordinate = frame_coordinate.coordinate;
		coords.direction = frame_coordinate.direction;
		coords.camera_offset = frame_coordinate.camera_offset;
		coords.picket = frame_coordinate.picket;
		coords.offset = frame_coordinate.offset;
		coords.counter = frame_coordinate.counter;
		coords.counter_size = frame_coordinate.counter_size;
		coords.path_type = frame_coordinate.path_type;
		coords.railway[0] = (char)0;
		coords.path[0] = (char)0;
		coords.line[0] = (char)0;
		strncpy_s(reinterpret_cast<char*>(&coords.railway), MAX_NAME_LENGTH_CB, CONVERT_TO_UTF8(frame_coordinate.railway).c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&coords.path), MAX_NAME_LENGTH_CB, CONVERT_TO_UTF8(frame_coordinate.path).c_str(), _TRUNCATE);
		strncpy_s(reinterpret_cast<char*>(&coords.line), MAX_NAME_LENGTH_CB, CONVERT_TO_UTF8(frame_coordinate.line).c_str(), _TRUNCATE);

		out.write(reinterpret_cast<const char*>(&coords), sizeof(FrameCoordPresentation));
		return out;
	}


	std::istream & operator>>(std::istream & in, IRBSpec & irb_spec)
	{
		in.read(reinterpret_cast<char*>(&irb_spec), sizeof(IRBSpec));
		return in;

	}

	std::ostream & operator<<(std::ostream & out, const IRBSpec & irb_spec)
	{
		out.write(reinterpret_cast<const char*>(&irb_spec), sizeof(IRBSpec));
		return out;
	}

	std::vector<char> get_frame_raw_data(const IRBFrame &irb_frame)
	{
		std::ostringstream data_stream;
		data_stream << irb_frame << irb_frame.coords << irb_frame.spec;
		auto data = data_stream.str();
		std::vector<char> raw_data(data.size());
		std::copy_n(data.cbegin(), data.size(), stdext::make_checked_array_iterator(raw_data.data(), data.size()));
		return raw_data;
	}

	irb_frame_ptr_t create_frame_by_raw_data(const std::vector<char>& frame_raw_data)
	{
		if (frame_raw_data.empty())
			return irb_frame_ptr_t();

		std::stringstream data_stream;
		std::copy(frame_raw_data.cbegin(), frame_raw_data.cend(), std::ostream_iterator<char>(data_stream));
		irb_frame_ptr_t frame(std::make_unique<IRBFrame>());
		data_stream >> *frame >> frame->coords;
		IRBSpec spec;
		data_stream >> spec;
		frame->set_spec(spec);

		return frame;
	}

	IRBFrame::IRBFrame() : _T_measured(false), _is_spec_set(false), _correction_T_enable(false)
	{
		min_temperature = max_temperature = avr_temperature = 0.0f;
		std::memset(&this->header, 0, sizeof(IRBFrameHeader));
	}

	IRBFrame::IRBFrame(const IRBFrameHeader & header) :IRBFrame()
	{
		std::memcpy(&this->header, &header, sizeof(IRBFrameHeader));
		auto count_pixels = get_pixels_count();
		if (count_pixels > 0){
			pixels = std::make_unique<irb_pixel_t[]>(count_pixels);
			std::memset(pixels.get(), 0, sizeof(irb_pixel_t)* count_pixels);
		}
	}

	IRBFrame::IRBFrame(const IRBFrame & frame) :IRBFrame()
	{
		_is_spec_set = frame._is_spec_set;
		std::memcpy(&this->header, &frame.header, sizeof(IRBFrameHeader));
		auto count_pixels = get_pixels_count();
		if (count_pixels > 0){
			pixels = std::make_unique<irb_pixel_t[]>(count_pixels);
			std::memcpy(pixels.get(), frame.pixels.get(), sizeof(irb_pixel_t)* count_pixels);
		}
		this->spec = frame.spec;
		this->coords = frame.coords;
		this->id = frame.id;
		this->_correction_T_enable = frame._correction_T_enable;
		this->_correction_T_params = frame._correction_T_params;
	}

	IRBFrame::~IRBFrame()
	{
	}

	const irb_frame_pixels_t& IRBFrame::getPixels()
	{
		return pixels;
	}

	irb_pixel_t IRBFrame::getPixel(int x, int y)
	{
		if (x < 0 || y < 0) return 0;
		if (x >= header.geometry.imgWidth || y >= header.geometry.imgHeight) return 0;

		return pixels[header.geometry.imgWidth*y + x];
	}

	BOOL IRBFrame::GetPixelTemp(uint16_t x, uint16_t y, float *tempToReturn)
	{
		if (x >= header.geometry.imgWidth || y >= header.geometry.imgHeight) return false;

		irb_pixel_t pixel = pixels[header.geometry.imgWidth*y + x];

		BYTE hiByte = pixel >> 8;
		BYTE loByte = pixel & 0xFF;

		FLOAT Temp1 = header.calibration.tempvals[hiByte];
		FLOAT Temp2 = header.calibration.tempvals[hiByte + 1];

		*tempToReturn = (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256));

		if (_correction_T_enable)
			*tempToReturn = *tempToReturn * _correction_T_params.factor + _correction_T_params.offset;

		return true;
	}

	float IRBFrame::retrieve_pixel_temperature(irb_pixel_t pixel)
	{
		byte hiByte = pixel >> 8;
		byte loByte = pixel & 0xFF;

		float Temp1 = header.calibration.tempvals[hiByte];
		float Temp2 = header.calibration.tempvals[hiByte + 1];

		float result = (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256));
		if (_correction_T_enable)
			result = result * _correction_T_params.factor + _correction_T_params.offset;

		return result;

	}

#define RETRIEVE_PIXEL_TEMPERATURE(_value,_pixel) \
	{\
		byte hiByte = _pixel >> 8; \
		byte loByte = _pixel & 0xFF; \
		float Temp1 = header.calibration.tempvals[hiByte];\
		float Temp2 = header.calibration.tempvals[hiByte + 1];\
		_value = (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256)); \
		if (_correction_T_enable)\
			_value = _value * _correction_T_params.factor + _correction_T_params.offset;\
	}



	bool IRBFrame::ComputeMinMaxAvr()
	{
		if (_is_spec_set || _T_measured)
			return true;
#ifdef USE_PPL
		return Extremum_parallel();
#else
		return Extremum();
#endif
	}

	
	void IRBFrame::foreach_T_value_parallel(const process_temperature_point_func_t& process_func)
	{
		LOG_STACK();

		_T_measured = false;

		max_temperature = -500.0f;
		min_temperature = 500.0f;
		avr_temperature = 0.0f;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;

		double avg_T = 0.0f;

		const WORD imgWidth = header.geometry.imgWidth;

		std::mutex data_mtx;

		concurrency::parallel_for(firstY, lastY + 1, [&, firstX, lastX, imgWidth](int y)
		{
			pixel_point_t max_T_point{ (std::uint16_t)firstX , (std::uint16_t)y};
			pixel_point_t min_T_point{ max_T_point };

			float max_T = -500.0f;
			float min_T = 500.0f;

			double avg_temp = 0.0f;

			irb_pixel_t max_T_pixel = std::numeric_limits<irb_pixel_t>::min();
			irb_pixel_t min_T_pixel = std::numeric_limits<irb_pixel_t>::max();

			float point_temp = 0.0f;
			irb_pixel_t *cur_pixel = &pixels[imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; ++x, ++cur_pixel)
			{
				irb_pixel_t pixel = *cur_pixel;

				RETRIEVE_PIXEL_TEMPERATURE(point_temp, pixel);

				process_func(x, y, point_temp);

				avg_temp += point_temp;
				if (max_T < point_temp)
				{
					max_T_point.first = (std::uint16_t)x;
					max_T = point_temp;
					max_T_pixel = pixel;
				}

				if (min_T > point_temp)
				{
					min_T_point.first = (std::uint16_t)x;
					min_T = point_temp;
					min_T_pixel = pixel;
				}
			}

			std::lock_guard<decltype(data_mtx)> lock(data_mtx);
			avg_T += avg_temp;
			if (max_temperature < max_T)
			{
				_max_temperature_point = max_T_point;
				max_temperature = max_T;
				_max_temperature_pixel = max_T_pixel;
			}

			if (min_temperature > min_T)
			{
				_min_temperature_point = min_T_point;
				min_temperature = min_T;
				_min_temperature_pixel = min_T_pixel;
			}

		});

		avr_temperature = (float)(avg_T / ((lastX - firstX + 1)*(lastY - firstY + 1)));

		_T_measured = true;

		if (!_is_spec_set)
		{
			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
	}

	void IRBFrame::foreach_T_value(const process_temperature_point_func_t& process_func)
	{
		LOG_STACK();

		_T_measured = false;

		max_temperature = -500.0f;
		min_temperature = 500.0f;
		avr_temperature = 0.0f;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;
		irb_pixel_t *cur_pixel = nullptr;
		double avg_temp = 0.0f;
		float point_temp = 0.0f;

		for (int y = firstY; y <= lastY; ++y/*, cur_raster_line = cur_raster_line + header.geometry.imgWidth*/)
		{
			cur_pixel = &pixels[header.geometry.imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; ++x, ++cur_pixel)
			{
				irb_pixel_t pixel = *cur_pixel;

				RETRIEVE_PIXEL_TEMPERATURE(point_temp, pixel);
				process_func(x, y, point_temp);

				avg_temp += point_temp;
				if (max_temperature < point_temp) 
				{
					_max_temperature_point.first = (std::uint16_t)x;
					_max_temperature_point.second = (std::uint16_t)y;
					max_temperature = point_temp;
					_max_temperature_pixel = pixel;
				}

				if (min_temperature > point_temp) 
				{
					_min_temperature_point.first = (std::uint16_t)x;
					_min_temperature_point.second = (std::uint16_t)y;

					min_temperature = point_temp;
					_min_temperature_pixel = pixel;
				}
			}
		}

		avr_temperature = (float)(avg_temp / ((lastX - firstX + 1)*(lastY - firstY + 1)));

		_T_measured = true;

		if (!_is_spec_set)
		{
			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
	}


	bool IRBFrame::Extremum_parallel(float * temp_vals)
	{
		LOG_STACK();
		_T_measured = false;

		max_temperature = -500.0f;
		min_temperature = 500.0f;
		avr_temperature = 0.0f;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;

		double avg_T = 0.0f;

		const WORD imgWidth = header.geometry.imgWidth;

		//concurrency::combinable<float> avg_T_s;

		std::mutex data_mtx;

		concurrency::parallel_for(firstY, lastY + 1, [&, firstX, lastX, imgWidth](int y)
		{
			pixel_point_t max_T_point{ (std::uint16_t)firstX , (std::uint16_t)y };
			pixel_point_t min_T_point{ max_T_point };

			float max_T = -500.0f;
			float min_T = 500.0f;
			//float avr_T = 0.0f;

			double avg_temp = 0.0f;

			irb_pixel_t max_T_pixel = std::numeric_limits<irb_pixel_t>::min();
			irb_pixel_t min_T_pixel = std::numeric_limits<irb_pixel_t>::max();

			float point_temp = 0.0f;
			float *cur_temp = nullptr;
			irb_pixel_t *cur_pixel = &pixels[imgWidth*y + firstX];
			
			if (temp_vals != nullptr)
				cur_temp = &temp_vals[imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; ++x, ++cur_pixel, ++cur_temp)
			{
				irb_pixel_t pixel = *cur_pixel;

				RETRIEVE_PIXEL_TEMPERATURE(point_temp, pixel);
				avg_temp += point_temp;
				if (max_T < point_temp)
				{
					max_T_point.first = (std::uint16_t)x;
					max_T = point_temp;
					max_T_pixel = pixel;
				}

				if (min_T > point_temp)
				{
					min_T_point.first = (std::uint16_t)x;
					min_T = point_temp;
					min_T_pixel = pixel;
				}

				if (temp_vals != nullptr)
				{
					*cur_temp = point_temp;
				}
			}

			std::lock_guard<decltype(data_mtx)> lock(data_mtx);
			avg_T += avg_temp;
			if (max_temperature < max_T)
			{
				_max_temperature_point = max_T_point;
				max_temperature = max_T;
				_max_temperature_pixel = max_T_pixel;
			}

			if (min_temperature > min_T)
			{
				_min_temperature_point = min_T_point;
				min_temperature = min_T;
				_min_temperature_pixel = min_T_pixel;
			}

		});


		avr_temperature = (float)(avg_T / ((lastX - firstX + 1)*(lastY - firstY + 1)));

		_T_measured = true;

		if (!_is_spec_set)
		{
			_is_spec_set = true;

			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
		return true;
	}


	bool IRBFrame::Extremum(float * temp_vals)
	{
		LOG_STACK();

		_T_measured = false;

		max_temperature = -500.0f;
		min_temperature = 500.0f;
		avr_temperature = 0.0f;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;
		irb_pixel_t *cur_pixel = nullptr;
		float *cur_temp = nullptr;
		double avg_temp = 0.0f;
		float point_temp = 0.0f;

		for (int y = firstY; y <= lastY; ++y/*, cur_raster_line = cur_raster_line + header.geometry.imgWidth*/)
		{
			cur_pixel = &pixels[header.geometry.imgWidth*y + firstX];
			if (temp_vals != nullptr)
				cur_temp = &temp_vals[header.geometry.imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; ++x, ++cur_pixel, ++cur_temp)
			{
				irb_pixel_t pixel = *cur_pixel;

				RETRIEVE_PIXEL_TEMPERATURE(point_temp, pixel);
				avg_temp += point_temp;
				if (max_temperature < point_temp)
				{
					_max_temperature_point.first = (std::uint16_t)x;
					_max_temperature_point.second = (std::uint16_t)y;
					max_temperature = point_temp;
					_max_temperature_pixel = pixel;
				}

				if (min_temperature > point_temp)
				{
					_min_temperature_point.first = (std::uint16_t)x;
					_min_temperature_point.second = (std::uint16_t)y;

					min_temperature = point_temp;
					_min_temperature_pixel = pixel;
				}

				if (temp_vals != nullptr)
				{
					*cur_temp = point_temp;
				}
			}
		}

		avr_temperature = (float)(avg_temp / ((lastX - firstX + 1)*(lastY - firstY + 1)));

		_T_measured = true;

		if (!_is_spec_set)
		{
			_is_spec_set = true;
			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
		return true;
	}

	bool IRBFrame::ExtremumExcludePixels(float * temp_vals, const bad_pixels_mask& pixels_mask)
	{
		LOG_STACK();

		if (temp_vals == nullptr)
			return false;

		_T_measured = false;

		max_temperature = -500.0f;
		min_temperature = 500.0f;
		avr_temperature = 0.0f;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;
		irb_pixel_t *cur_pixel = nullptr;
		float *cur_temp = nullptr;
		double avg_temp = 0;
		float point_temp = 0.0f;
		const bad_pixels_mask::value_type *cur_pixel_mask = nullptr;

		for (int y = firstY; y <= lastY; ++y)
		{
			cur_pixel_mask = &pixels_mask.mask[header.geometry.imgWidth*y + firstX];
			cur_pixel = &pixels[header.geometry.imgWidth*y + firstX];
			if (temp_vals != nullptr)
				cur_temp = &temp_vals[header.geometry.imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; ++x, ++cur_pixel, ++cur_pixel_mask, ++cur_temp)
			{
				if (*cur_pixel_mask)
				{
					*cur_pixel = *(cur_pixel + *cur_pixel_mask);
				}
				{
					irb_pixel_t pixel = *cur_pixel;

					RETRIEVE_PIXEL_TEMPERATURE(point_temp, pixel);
					avg_temp += point_temp;

					if (max_temperature < point_temp)
					{
						_max_temperature_point.first = (std::uint16_t)x;
						_max_temperature_point.second = (std::uint16_t)y;
						max_temperature = point_temp;
						_max_temperature_pixel = pixel;
					}

					if (min_temperature > point_temp)
					{
						_min_temperature_point.first = (std::uint16_t)x;
						_min_temperature_point.second = (std::uint16_t)y;

						min_temperature = point_temp;
						_min_temperature_pixel = pixel;
					}

					if (temp_vals != nullptr)
					{
						*cur_temp = point_temp;
					}
				}
			}
		}

		avr_temperature = (float)(avg_temp / ((lastX - firstX + 1)*(lastY - firstY + 1)));

		_T_measured = true;

		if (!_is_spec_set)
		{
			_is_spec_set = true;
			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
		return true;
	}

	irb_pixel_t IRBFrame::GetPixelFromTemp(float temp)
	{
		float t = temp + corrected_Celsium_offset();

		if (_correction_T_enable)
			t = (t - _correction_T_params.offset) / _correction_T_params.factor;

		BYTE hi = 0, lo = 0;
		for (int i = 0; i < 255; ++i)
		{
			if (header.calibration.tempvals[i] <= t && t < header.calibration.tempvals[i + 1])
			{
				hi = static_cast<BYTE>(i);
				break;
			}
		}
		float dt1 = t - header.calibration.tempvals[hi];
		float dt2 = header.calibration.tempvals[hi + 1] - header.calibration.tempvals[hi];
		if (dt2 == 0) return 0;
		lo = (BYTE)(dt1 / dt2 * 255);
		irb_pixel_t res = (irb_pixel_t)hi << 8 | lo;
		return res;
	}
}
#include "irb_frame_helper.h"
#include <iostream>

#ifdef _WINGDI_
#undef _WINGDI_
#endif
#include <atltime.h>

namespace irb_frame_helper
{

	uint32_t get_frame_coordinate_type_offset()
	{
		return sizeof(IRBFrameGeometry)+sizeof(IRBFrameObjectParametrs)+sizeof(IRBFrameCallibration)+FIELD_OFFSET(IRBFramePresentation, frameCoord);
	}

	uint32_t get_frame_time_offset()
	{
		return sizeof(IRBFrameGeometry)+sizeof(IRBFrameObjectParametrs)+sizeof(IRBFrameCallibration)+FIELD_OFFSET(IRBFramePresentation, imgTime);
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
		for (i = 0; i < pixels_number; i++)
		{
			decodedPixels[i] = (irb_pixel_t)pixelsToDecode[i];
		}

		offset = 1;
		for (i = pixels_number; i < dataSize - 1; i += 2)
		{
			pixel_data_ptr pixel_data = reinterpret_cast<pixel_data_ptr>(&pixelsToDecode[i]);
			for (int j = 0; j < pixel_data->number; j++, offset++)
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
			offset++;
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

		std::memcpy(&irb_frame.coords, &irb_frame.header.presentation.frameCoord, sizeof(FrameCoord));
		auto decode_type = pixelFormat + compression;

		if (decode_type == 2)
		{
			irb_frame.pixels = std::make_unique<irb_pixel_t[]>(irb_frame.get_pixels_count());

			//irb_frame.pixels.resize(irb_frame.get_pixels_count());
			in.read(reinterpret_cast<char*>(irb_frame.pixels.get()), irb_frame.get_pixels_count()*decode_type);
			return in;
		}

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


		return in;
	}

	std::ostream & operator<<(std::ostream & out,const IRBFrame &irb_frame)
	{
		(const_cast<IRBFrame&>(irb_frame)).header.presentation.frameCoord.coordinate = irb_frame.coords.coordinate;
		(const_cast<IRBFrame&>(irb_frame)).header.presentation.frameCoord.line = irb_frame.coords.line;
		(const_cast<IRBFrame&>(irb_frame)).header.presentation.frameCoord.path = irb_frame.coords.path;

		out.write(reinterpret_cast<const char*>(&irb_frame.header), sizeof(IRBFrameHeader));
//		visual.dataSize = 1728 + frame->header.geometry.pixelFormat*frame->header.geometry.imgWidth*frame->header.geometry.imgHeight;
		ULONG32 pixels_data_size = irb_frame.header.geometry.pixelFormat*irb_frame.get_pixels_count();// *sizeof(WORD);
		out.write(reinterpret_cast<const char*>(irb_frame.pixels.get()), pixels_data_size);
		return out;
	}

	std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate)
	{
		in.read(reinterpret_cast<char*>(&frame_coordinate), sizeof(FrameCoord));
		return in;
	}

	std::ostream & operator<<(std::ostream & out, const FrameCoord &frame_coordinate)
	{
		out.write(reinterpret_cast<const char*>(&frame_coordinate), sizeof(FrameCoord));
		return out;
	}

	IRBFrame::IRBFrame()
	{
		_temperature_span_calculated = false;
		min_temperature = max_temperature = avr_temperature = 0.0f;
		std::memset(&this->header, 0, sizeof(IRBFrameHeader));
		_is_spec_set = false;
	}


	IRBFrame::IRBFrame(const IRBFrameHeader & header) :IRBFrame()
	{
		_is_spec_set = false;
		std::memcpy(&this->header, &header, sizeof(IRBFrameHeader));
		auto count_pixels = get_pixels_count();
		if (count_pixels > 0){
			pixels = std::make_unique<irb_pixel_t[]>(count_pixels);
			std::memset(pixels.get(), 0, sizeof(irb_pixel_t)* count_pixels);
		}
	}

	IRBFrame::IRBFrame(const IRBFrame & frame)
	{
		_is_spec_set = frame._is_spec_set;
		_temperature_span_calculated = false;
		std::memcpy(&this->header, &frame.header, sizeof(IRBFrameHeader));
		auto count_pixels = get_pixels_count();
		if (count_pixels > 0){
			pixels = std::make_unique<irb_pixel_t[]>(count_pixels);
			std::memcpy(pixels.get(), frame.pixels.get(), sizeof(irb_pixel_t)* count_pixels);
		}
		std::memcpy(&this->coords, &frame.coords, sizeof(FrameCoord));
		this->id = frame.id;
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

		return true;
	}

	float IRBFrame::retrieve_pixel_temperature(irb_pixel_t pixel)
	{
		byte hiByte = pixel >> 8;
		byte loByte = pixel & 0xFF;

		float Temp1 = header.calibration.tempvals[hiByte];
		float Temp2 = header.calibration.tempvals[hiByte + 1];

		return (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256));

	}

#define RETRIEVE_PIXEL_TEMPERATURE(_value,_pixel) \
	{\
		byte hiByte = _pixel >> 8; \
		byte loByte = _pixel & 0xFF; \
		float Temp1 = header.calibration.tempvals[hiByte];\
		float Temp2 = header.calibration.tempvals[hiByte + 1];\
		_value = (Temp1 + ((Temp2 - Temp1)*(float)loByte / 256)); \
	}


	BOOL IRBFrame::ComputeMinMaxAvr(float * min, float * max, float * avr)
	{
		int x, y;

		if (!Extremum())
			return false;

		float from = *min;
		float to = *max;
		float dtt = to - from;

		double avrw = 0;

		irb_pixel_t fromw = GetPixelFromTemp(from);
		irb_pixel_t tow = GetPixelFromTemp(to);
		WORD dw = tow - fromw;

		float dttDASHdw = dtt / dw;

		for (y = 0; y < header.geometry.imgHeight; y++)
		{
			for (x = 0; x < header.geometry.imgWidth; x++)
			{
				WORD pixel = pixels[header.geometry.imgWidth * y + x];
				unsigned int dt = pixel - fromw;
				float curTemp = dt * dttDASHdw + from;
				avrw += curTemp;// - 273.15;
			}
		}

		if (header.geometry.imgHeight * header.geometry.imgWidth > 0)
			*avr = (float)(avrw / header.geometry.imgHeight / header.geometry.imgWidth);

		return true;

	}


	BOOL IRBFrame::Extremum(float * temp_vals)
	{

		WORD maxw = 0;
		WORD minw = 65535;

		int firstY = header.geometry.firstValidY;
		int lastY = header.geometry.lastValidY;
		int firstX = header.geometry.firstValidX;
		int lastX = header.geometry.lastValidX;
		irb_pixel_t *cur_pixel = nullptr;
		float *cur_temp = nullptr;
		float avg_temp = 0;

		for (int y = firstY; y <= lastY; y++/*, cur_raster_line = cur_raster_line + header.geometry.imgWidth*/)
		{
			cur_pixel = &pixels[header.geometry.imgWidth*y + firstX];
			if (temp_vals != nullptr)
				cur_temp = &temp_vals[header.geometry.imgWidth*y + firstX];

			for (int x = firstX; x <= lastX; x++, cur_pixel++, cur_temp++)
			{
				irb_pixel_t pixel = *cur_pixel;
				if (maxw < pixel){
					maxw = pixel;
					_max_temperature_pixel = pixel;
				}
				if (pixel < minw){
					minw = pixel;
					_min_temperature_pixel = pixel;
				}

				if (temp_vals != nullptr)
				{
					RETRIEVE_PIXEL_TEMPERATURE(*cur_temp, pixel);
					avg_temp += *cur_temp;
				}

			}
		}

		if (temp_vals != nullptr && cur_pixel != nullptr){
			avr_temperature = (float)(avg_temp / ((lastX - firstX + 1)*(lastY - firstY + 1))) - 273.15f;
		}

		BYTE hiByte = maxw >> 8;
		BYTE loByte = maxw & 0xFF;

		FLOAT Temp1 = header.calibration.tempvals[hiByte];
		FLOAT Temp2 = header.calibration.tempvals[hiByte + 1];

		FLOAT dTemp = Temp2 - Temp1;
		FLOAT up1 = dTemp*(float)loByte;

		float temp = Temp1 + up1 / 256 - (float)273.15;
		max_temperature = temp;

		hiByte = minw >> 8;
		loByte = minw & 0xFF;

		Temp1 = header.calibration.tempvals[hiByte];
		Temp2 = header.calibration.tempvals[hiByte + 1];

		dTemp = Temp2 - Temp1;
		up1 = dTemp*(float)loByte;

		temp = Temp1 + up1 / 256 - (float)273.15;
		min_temperature = temp;

		_temperature_span_calculated = true;

		if (!_is_spec_set)
		{
			spec.IRBmin = min_temperature;
			spec.IRBavg = avr_temperature;
			spec.IRBmax = max_temperature;
		}
		return true;
	}

	time_t IRBFrame::time_since_epoch() const
	{
		CTime t = CTime(1, 1, 2000);
		auto delta = convert_irb_frame_time_in_sec(header.presentation.imgTime) - t.GetTime();
		auto ms = delta * 1000;//+(DWORD)header.presentation.imgMilliSecTime;
		return ms;
	}

	time_t IRBFrame::Msec()
	{
		CTime t = CTime(1, 1, 2000);
		auto delta = convert_irb_frame_time_in_sec(header.presentation.imgTime) - t.GetTime();
		auto ms = delta * 1000;//+(DWORD)header.presentation.imgMilliSecTime;
		return ms;
	}

	irb_pixel_t IRBFrame::GetPixelFromTemp(float temp)
	{
		float t = temp + (float)273.15;
		BYTE hi = 0, lo = 0;
		for (int i = 0; i < 255; i++)
		{
			if (header.calibration.tempvals[i] <= t && t < header.calibration.tempvals[i + 1])
			{
				hi = i;
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
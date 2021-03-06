#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

namespace irb_frame_helper
{
	using railway_t = std::wstring;
	using line_t = std::wstring;
	using path_t = std::wstring;

	typedef int64_t coordinate_t;
	typedef uint8_t  direction_t;
	typedef int32_t  camera_offset_t;
	typedef uint32_t  counter_t;
	typedef uint16_t  counter_size_t;
	typedef int32_t  picket_t;
	typedef int32_t  offset_t;
	typedef uint16_t  path_type_t;

	typedef struct _FrameCoord_v4 // ���������� � ����
	{
		_FrameCoord_v4() :
		coordinate(0),
		direction(0), 
		counter(0),
		counter_size(0),
		camera_offset(0),
		picket(0),
		offset(0),
		path_type(0)
		{}
		coordinate_t coordinate; // ���������� �� ������ ��������� � �����������
		railway_t railway;
		path_t path;		// ����
		path_t path_name;		// ����
		path_type_t path_type;
		line_t line;		// �����
		direction_t direction; //����������� ��������
		picket_t picket;
		offset_t offset;
		camera_offset_t camera_offset;
		counter_t counter;
		counter_size_t counter_size;

	}FrameCoord_v4;

	typedef FrameCoord_v4 FrameCoord;

	typedef struct _FrameCoord_v1 // ���������� � ����
	{
		FrameCoord_v4 coord;

	}FrameCoord_v1;

	typedef struct _FrameCoord_v2 // ���������� � ����
	{
		FrameCoord_v4 coord;

	}FrameCoord_v2;

	typedef struct _FrameCoord_v3 // ���������� � ����
	{
		FrameCoord_v4 coord;

	}FrameCoord_v3;


#pragma pack(push,1)

	struct IRBFrameGeometry
	{
		WORD pixelFormat;	// ������ �������
		WORD compression;	// ������ ������
		WORD imgWidth;		// ������ � �������� 
		WORD imgHeight;		// ������ � ��������
		WORD upperLeftX;	// ���������� �������� �������� ������ ���� ������
		WORD upperLeftY;	// ���������� �������� �������� ������ ���� ������
		WORD firstValidX;	// ������ �������� ������� ��������
		WORD lastValidX;	// ��������� �������� ������� ��������
		WORD firstValidY;	// ������ �������� ������ ��������
		WORD lastValidY;	// ��������� �������� ������ ��������
		char filler[4];		// �� ������������
	};

	struct IRBFrameObjectParametrs
	{
		FLOAT emissivity;	// ������������� �����������
		FLOAT objDistance;	// ���������� �� �������  
		FLOAT ambTemp;		// ����������� ���������� �����
		FLOAT absoConst;	// ���������� ����������
		FLOAT pathTemp;		//  
		char filler[4];		// �� ������������
	};

	struct IRBFrameCallibration
	{
		FLOAT lambdaSP;			// �������� ������ �����
		FLOAT deltaLambda;		// ������ �������
		WORD algorithm;			// �������� ������� ����������
		WORD nrTemps;			// ���������� ������� �����
		FLOAT tempvals[270];	// ����������� ������� �����
		FLOAT camtemps[6];		// �������� ����������� ������
		FLOAT useroffset;		// ������ ������������� �����������
		FLOAT accu_cam;			// 
		FLOAT tinteg;			// ����� ����������
		FLOAT tmin;				// ������ ������ ��������� �������� ���������� ��� ��������
		FLOAT tmax;				// ������� ������ ��������� �������� ���������� ��� ��������
		char sw_ver_cam[22];	// ������ �� ������
		char sw_ver_irb[20];	// ������ IRBIS-online
		char camera_name[22];	// �������� ������
		char camera_spec[22];	// ��������� �������� ������
		char camera_sn[16];		// �������� ����� ������
		char lens_name[32];		// �������� ���������
		char lens_spec[32];		// ��������� �������� ���������
		char lens_sn[16];		// �������� ����� ���������
		FLOAT hfov;				// �������������� ������ ������, ����
		FLOAT vfov;				// ������������ ������ ������, ����
		char filter_name[32];	// �������� �������
		char filter_spec[32];	// ��������� �������� �������
		char filter_sn[16];		// �������� ����� �������
		char apert_name[32];	// �������� ��������
		char apert_spec[32];	// ��������� �������� ��������
		char apert_sn[16];		// �������� ����� ��������
		char calib_title[16];	// ������������ ������� ����������
		char calib_ver[4];		// ������ ����������
		char calib_date[16];	// ���� ���������� ����������
		char calib_name[32];	// �������� ����������
		BYTE smooth;			// ������� �����������
		BYTE microscan;			// 
		WORD shutperiod;		// ������ ������������ �������
		WORD dnl;				// 
		BYTE trigmode;			// ����� �����
		WORD lastshut;			// �����, ��������� ����� ���������� ������������ �������
		char filler[5];			// �� ������������
	};

	struct IRBFramePresentation
	{
		FLOAT level;			// ������� ������� ����������
		FLOAT span;				// ������� ����������
		double imgTime;			// ����� ������
		FLOAT imgMilliSecTime;	// ����� ������ � �������������
		WORD imgAccu;//
		char imageComment[80];	// �����������
		FLOAT zoom_hor;			// �������������� ����������
		FLOAT zoom_vert;		// ������������ ����������
		char filler[2];			// �� ������������
	};

	struct IRBFrameHeader
	{
		IRBFrameGeometry geometry;
		IRBFrameObjectParametrs objParams;
		IRBFrameCallibration calibration;
		IRBFramePresentation presentation;
	};

	struct IRBSpec
	{
		IRBSpec() :IRBmin(0.0f), IRBmax(0.0f), IRBavg(0.0f){}
		IRBSpec(float min, float max, float average) :IRBmin(min), IRBmax(max), IRBavg(average){}
		float IRBmin;
		float IRBavg;
		float IRBmax;
	};
#pragma pack(pop)

	typedef struct _irb_frame_key
	{
		_irb_frame_key(uint32_t xid, coordinate_t xcoordinate = 0, double xtime = 0.0) :id(xid), coordinate(xcoordinate), time(xtime){}
		_irb_frame_key() :id(0), coordinate(0), time(0.0){}
		uint32_t id;
		coordinate_t coordinate;
		double time;

		bool operator==(const uint32_t search_id) const
		{
			return id == search_id;
		}
		bool operator==(const coordinate_t search_coordinate) const
		{
			return coordinate == search_coordinate;
		}
		bool operator==(const double search_time) const
		{
			return time == search_time;
		}

	}irb_frame_key;


	class IRBFrame;
	using irb_frame_ptr_t = std::unique_ptr<IRBFrame>;

	using frame_id_t = uint32_t;

	using irb_pixel_t = WORD;
	using irb_frame_pixels_t = std::unique_ptr<irb_pixel_t[]>;

	template<typename TItem, TItem default_value = TItem()>
	class pixels_mask
	{
	public:
		using value_type = TItem;

		struct coordinate_t {
			uint16_t x;
			uint16_t y;
		};
		explicit pixels_mask(uint16_t width, uint16_t height) :
			_width(width), _height(height), count_non_default_values(0),
			mask(std::vector<TItem>::size_type(width*height), default_value)
		{}

		void set_value(const std::vector<coordinate_t> &coordinates,const TItem& value)
		{
			uint16_t count_applyed_values = 0;
			for (const auto & coordinate : coordinates)
			{
				if (_height > coordinate.y && _width > coordinate.x){
					mask[_width*coordinate.y + coordinate.x] = value;
					count_applyed_values++;
				}
			}

			if (value != default_value)
				count_non_default_values += count_applyed_values;
		}

	public:
		std::vector<TItem> mask;

		uint16_t _width;
		uint16_t _height;
		uint16_t count_non_default_values;
	};

	using bad_pixels_mask = pixels_mask<int8_t>;

	class IRBFrame final
	{
	public:
		IRBFrame();
		IRBFrame(const IRBFrameHeader & header);
		~IRBFrame();

		IRBFrame(const IRBFrame & frame);
		IRBFrame & operator = (const IRBFrame &) = delete;
		bool operator<(const IRBFrame&) = delete;
		bool operator>(const IRBFrame&) = delete;
		bool operator<=(const IRBFrame&) = delete;
		bool operator>=(const IRBFrame&) = delete;


		void set_spec(const IRBSpec& spec)
		{
			_is_spec_set = true;
			this->spec = spec;
		}

		inline unsigned int get_pixels_data_size() const { return get_pixels_count()*header.geometry.pixelFormat; }
		inline unsigned int get_pixels_count() const { return header.geometry.imgWidth*header.geometry.imgHeight; }
		inline void set_pixels(irb_frame_pixels_t& pixels) { _temperature_span_calculated = false; this->pixels.swap(pixels); }

		time_t time_since_epoch() const;

		inline double get_frame_time_in_sec() const { return header.presentation.imgTime; }
		inline float get_frame_time_in_msec() const { return header.presentation.imgMilliSecTime; }

		time_t Msec();
		BOOL Extremum(float * temp_vals = nullptr);
		BOOL ExtremumExcludePixels(float * temp_vals, const bad_pixels_mask& pixels_mask);
		BOOL ComputeMinMaxAvr();
		BOOL GetPixelTemp(uint16_t x, uint16_t y, float * tempToReturn);

		float retrieve_pixel_temperature(irb_pixel_t pixel);

		const irb_frame_pixels_t& getPixels(); // ������ ���� ��������
		irb_pixel_t getPixel(int x, int y); // ������ ������� � ��������� ������������

		inline frame_id_t getFrameNum() const	{ return id; }


		irb_pixel_t GetPixelFromTemp(float temp);

		inline bool marked() const { return _marked; }
		inline void set_marked(bool state = true) { _marked = state; }

		void set_temperature_measure(float min, float max, float average) { _temperature_span_calculated = true; min_temperature = min; max_temperature = max; avr_temperature = average; }
		bool is_temperature_span_calculated(void * T_vals = nullptr) const 
		{
			if (T_vals == nullptr || T_vals == _last_T_vals)
				return _temperature_span_calculated;
			return false;
		}
		bool is_bad_pixels_processed() const{return _bad_pixels_processed;}

		irb_pixel_t get_min_temperature_pixel() const { return _min_temperature_pixel; }
		irb_pixel_t get_max_temperature_pixel() const { return _max_temperature_pixel; }

		inline float maxT() const { return max_temperature; }
		inline float minT() const { return min_temperature; }
		inline float avgT() const { return avr_temperature; }

	public:
		IRBFrameHeader header;
		irb_frame_pixels_t pixels;

		frame_id_t id;

		float min_temperature, max_temperature, avr_temperature;
		int points_num;

		FrameCoord coords;
		IRBSpec spec;
	private:

		void * _last_T_vals;

		using pixel_point_t = std::pair<uint16_t, uint16_t>;

		pixel_point_t _max_temperature_point;
		pixel_point_t _min_temperature_point;

		irb_pixel_t _max_temperature_pixel;
		irb_pixel_t _min_temperature_pixel;


		mutable bool _temperature_span_calculated;
		bool wasRead;
		bool _marked;
		bool _is_spec_set;
		bool _bad_pixels_processed;

		friend std::istream & operator>>(std::istream & in, IRBFrame &irb_frame);
		friend std::ostream & operator<<(std::ostream & out, const IRBFrame &irb_frame);

		friend std::istream & operator>>(std::istream & in, FrameCoord_v1 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord_v2 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord_v3 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate);

	};
	

	std::istream & operator>>(std::istream & in, FrameCoord_v1 &frame_coordinate);
	std::istream & operator>>(std::istream & in, FrameCoord_v2 &frame_coordinate);
	std::istream & operator>>(std::istream & in, FrameCoord_v3 &frame_coordinate);

	std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate);
	std::ostream & operator<<(std::ostream & out, const FrameCoord &frame_coordinate);

	inline unsigned int serialized_irb_frame_size(const IRBFrame &irb_frame)
	{
		auto res = sizeof(IRBFrameHeader)+irb_frame.get_pixels_count()*irb_frame.header.geometry.pixelFormat;
		return (unsigned int)res;
	}

	uint32_t get_size_frame_coordinates();

	inline unsigned int calculate_irb_frame_serialized_size(const IRBFrame &irb_frame)
	{
		return serialized_irb_frame_size(irb_frame) + get_size_frame_coordinates();
	}


	inline time_t convert_irb_frame_time_in_sec(double tdatetime)
	{
		return (time_t)((tdatetime - 70 * 365 + 19) * 24 * 3600);
	}

	uint32_t get_frame_coordinate_type_offset();
	uint32_t get_frame_time_offset();

	std::vector<char> get_frame_raw_data(const IRBFrame &irb_frame);
	irb_frame_ptr_t create_frame_by_raw_data(const std::vector<char>& frame_raw_data);

	template<class Pred>
	void for_each_frame_T_value(IRBFrame * frame, float * temp_vals, Pred pred)
	{

		int firstY = frame->header.geometry.firstValidY;
		int lastY = frame->header.geometry.lastValidY;
		int firstX = frame->header.geometry.firstValidX;
		int lastX = frame->header.geometry.lastValidX;

		float * pixel_temp;
		for (int y = firstY; y <= lastY; y++)
		{
			pixel_temp = &_temp_vals[frame->header.geometry.imgWidth*y + firstX];
			for (int x = firstX; x <= lastX; x++, pixel_temp++)
			{
				float curTemp = *pixel_temp - 273.15f;
				pred(curTemp);
			}
		}
	}

}
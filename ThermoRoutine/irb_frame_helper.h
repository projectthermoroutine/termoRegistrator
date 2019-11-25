#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <functional>

namespace irb_frame_helper
{
	inline constexpr float g_Kelvin_Celsius_Delta = 273.15f;
	
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

	typedef struct _FrameCoord_v4 // информация о пути
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
		coordinate_t coordinate; // координата от начала координат в миллиметрах
		railway_t railway;
		path_t path;		// путь
		path_t path_name;		// путь
		path_type_t path_type;
		line_t line;		// линия
		direction_t direction; //направление движения
		picket_t picket;
		offset_t offset;
		camera_offset_t camera_offset;
		counter_t counter;
		counter_size_t counter_size;

	}FrameCoord_v4;

	typedef FrameCoord_v4 FrameCoord;

	typedef struct _FrameCoord_v1 // информация о пути
	{
		FrameCoord_v4 coord;

	}FrameCoord_v1;

	typedef struct _FrameCoord_v2 // информация о пути
	{
		FrameCoord_v4 coord;

	}FrameCoord_v2;

	typedef struct _FrameCoord_v3 // информация о пути
	{
		FrameCoord_v4 coord;

	}FrameCoord_v3;


#pragma pack(push,1)

	struct IRBFrameGeometry
	{
		WORD pixelFormat;	// формат пикселя
		WORD compression;	// формат сжатия
		WORD imgWidth;		// ширина в пикселях 
		WORD imgHeight;		// высота в пикселях
		WORD upperLeftX;	// абсолютная абсцисса верхнего левого угла снимка
		WORD upperLeftY;	// абсолютная ордината верхнего левого угла снимка
		WORD firstValidX;	// первая значащая колонка пикселей
		WORD lastValidX;	// последняя значащая колонка пикселей
		WORD firstValidY;	// первая значащая строка пикселей
		WORD lastValidY;	// последняя значащая строка пикселей
		char filler[4];		// не используется
	};

	struct IRBFrameObjectParametrs
	{
		FLOAT emissivity;	// излучательная способность
		FLOAT objDistance;	// расстояние до объекта  
		FLOAT ambTemp;		// температура окружающей среды
		FLOAT absoConst;	// постоянная поглощения
		FLOAT pathTemp;		//  
		char filler[4];		// не используется
	};

	struct IRBFrameCallibration
	{
		FLOAT lambdaSP;			// основная длинна волны
		FLOAT deltaLambda;		// ширина спектра
		WORD algorithm;			// алгоритм расчета температур
		WORD nrTemps;			// количетсво пробных точек
		FLOAT tempvals[270];	// температуры пробных точек
		FLOAT camtemps[6];		// значения температуры камеры
		FLOAT useroffset;		// ручная корректировка температуры
		FLOAT accu_cam;			// 
		FLOAT tinteg;			// время интеграции
		FLOAT tmin;				// нижний предел диапазона значений температур для проверки
		FLOAT tmax;				// верхний предел диапазона значений температур для проверки
		char sw_ver_cam[22];	// версия ПО камеры
		char sw_ver_irb[20];	// версия IRBIS-online
		char camera_name[22];	// название камеры
		char camera_spec[22];	// подробное описание камеры
		char camera_sn[16];		// серийный номер камеры
		char lens_name[32];		// название объектива
		char lens_spec[32];		// подробное описание объектива
		char lens_sn[16];		// серийный номер объектива
		FLOAT hfov;				// горизонтальный сектор обзора, град
		FLOAT vfov;				// вертикальный сектор обзора, град
		char filter_name[32];	// название фильтра
		char filter_spec[32];	// подробное описание фильтра
		char filter_sn[16];		// серийный номер фильтра
		char apert_name[32];	// название апертуры
		char apert_spec[32];	// подробное описание апертуры
		char apert_sn[16];		// серийный номер апертуры
		char calib_title[16];	// наименование текущей калибровки
		char calib_ver[4];		// версия калибровки
		char calib_date[16];	// дата проведения калибровки
		char calib_name[32];	// название калибровки
		BYTE smooth;			// степень сглаживания
		BYTE microscan;			// 
		WORD shutperiod;		// период срабатывания затвора
		WORD dnl;				// 
		BYTE trigmode;			// режим пуска
		WORD lastshut;			// время, прошедшее после последнего срабатывания затвора
		char filler[5];			// не используется
	};

	struct IRBFramePresentation
	{
		FLOAT level;			// средний уровень температур
		FLOAT span;				// разброс температур
		double imgTime;			// время снимка
		FLOAT imgMilliSecTime;	// время снимка в миллисекундах
		WORD imgAccu;//
		char imageComment[80];	// комментарий
		FLOAT zoom_hor;			// горизонтальное увеличение
		FLOAT zoom_vert;		// вертикальное увеличение
		char filler[2];			// не используеься
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

	struct correction_T_params_t
	{
		float factor;
		float offset;
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

	using pixel_point_t = std::pair<uint16_t, uint16_t>;


	class frame_visiter;
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

	public:

		void set_spec(const IRBSpec& irb_spec) noexcept
		{
			_is_spec_set = true;
			this->spec = irb_spec;
		}

		inline unsigned int get_pixels_data_size() const noexcept { return get_pixels_count()*header.geometry.pixelFormat; }
		inline unsigned int get_pixels_count() const noexcept { return header.geometry.imgWidth*header.geometry.imgHeight; }
		inline void set_pixels(irb_frame_pixels_t& frame_pixels) noexcept { _T_measured = false; this->pixels.swap(frame_pixels); }

		inline double get_frame_time_in_sec() const noexcept { return header.presentation.imgTime; }
		inline float get_frame_time_in_msec() const noexcept { return header.presentation.imgMilliSecTime; }


	public:

		using process_temperature_point_func_t = std::function<void(int, int, float)>;

		void foreach_T_value(const process_temperature_point_func_t&);
		void foreach_T_value_with_bad_pixels(const process_temperature_point_func_t&, const bad_pixels_mask& pixels_mask);


		/* parallel*/
		void foreach_T_value_parallel(const process_temperature_point_func_t&);
		bool Extremum_parallel(float * temp_vals = nullptr);
		/**/

		bool Extremum(float * temp_vals = nullptr) noexcept;
		bool ExtremumExcludePixels(float * temp_vals, const bad_pixels_mask& pixels_mask) noexcept;
		bool ComputeMinMaxAvr();

	public:

		BOOL GetPixelTemp(uint16_t x, uint16_t y, float * tempToReturn) noexcept;

		float retrieve_pixel_temperature(irb_pixel_t pixel) noexcept;

		const irb_frame_pixels_t& getPixels() noexcept; // чтение всех пикселей
		irb_pixel_t getPixel(int x, int y) noexcept; // чтение пикселя с заданными координатами

		inline frame_id_t getFrameNum() const  noexcept { return id; }

		irb_pixel_t GetPixelFromTemp(float temp) noexcept;

		inline bool marked() const noexcept { return _marked; }
		inline void set_marked(bool state = true) noexcept { _marked = state; }


	public:

		irb_pixel_t get_min_temperature_pixel() const noexcept { return _min_temperature_pixel; }
		irb_pixel_t get_max_temperature_pixel() const noexcept { return _max_temperature_pixel; }

		static inline constexpr float corrected_Celsium_offset() noexcept { return g_Kelvin_Celsius_Delta /** (_correction_T_enable ? _correction_T_params.factor : 1)*/;}

		inline float maxT() const noexcept { return spec.IRBmax - corrected_Celsium_offset(); }
		inline float minT() const noexcept { return spec.IRBmin - corrected_Celsium_offset(); }
		inline float avgT() const noexcept { return spec.IRBavg - corrected_Celsium_offset(); }

		bool T_measured() const noexcept { return _is_spec_set; }

		inline pixel_point_t max_T_point() const noexcept { return _max_temperature_point; }
		inline pixel_point_t min_T_point() const noexcept { return _min_temperature_point; }

	public:
		IRBFrameHeader header;
		irb_frame_pixels_t pixels;

		frame_id_t id;

		int points_num;

		FrameCoord coords;
		IRBSpec spec;
	private:

		float min_temperature, max_temperature, avr_temperature;
		pixel_point_t _max_temperature_point;
		pixel_point_t _min_temperature_point;

		irb_pixel_t _max_temperature_pixel;
		irb_pixel_t _min_temperature_pixel;

		bool _marked;
		bool _is_spec_set;
		bool _T_measured;


	private:

		bool _correction_T_enable;
		correction_T_params_t _correction_T_params;

	public:

		inline bool correction_T_enabled() const noexcept { return _correction_T_enable; }
		correction_T_params_t correction_T_params() noexcept { return _correction_T_params; }
		const correction_T_params_t & correction_T_params() const noexcept { return _correction_T_params; }

		void set_correction_temperature_settings(bool enable, float factor, float offset) noexcept
		{
			if (enable)
			{
				_correction_T_params.factor = factor;
				_correction_T_params.offset = offset;
				_correction_T_enable = enable;
			}
			else
			{
				_correction_T_enable = enable;
				_correction_T_params.factor = factor;
				_correction_T_params.offset = offset;
			}
		}
	private:

		friend std::istream & operator>>(std::istream & in, IRBFrame &irb_frame);
		friend std::ostream & operator<<(std::ostream & out, const IRBFrame &irb_frame);

		friend std::istream & operator>>(std::istream & in, FrameCoord_v1 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord_v2 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord_v3 &frame_coordinate);
		friend std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate);

		friend class frame_visiter;

	};//class IRBFrame final
	

	std::istream & operator>>(std::istream & in, FrameCoord_v1 &frame_coordinate);
	std::istream & operator>>(std::istream & in, FrameCoord_v2 &frame_coordinate);
	std::istream & operator>>(std::istream & in, FrameCoord_v3 &frame_coordinate);

	std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate);
	std::ostream & operator<<(std::ostream & out, const FrameCoord &frame_coordinate);

	std::istream & operator>>(std::istream & in, IRBSpec & irb_spec);
	std::ostream & operator<<(std::ostream & out, const IRBSpec & irb_spec);

	std::istream & operator>>(std::istream & in, correction_T_params_t & );
	std::ostream & operator<<(std::ostream & out, const correction_T_params_t & );

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

	uint32_t get_frame_coordinate_type_offset();
	uint32_t get_frame_time_offset();

	std::vector<char> get_frame_raw_data(const IRBFrame &irb_frame);
	irb_frame_ptr_t create_frame_by_raw_data(const std::vector<char>& frame_raw_data);

}
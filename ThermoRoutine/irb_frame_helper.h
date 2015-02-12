#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

namespace irb_frame_helper
{
	typedef uint64_t coordinate_t;
	typedef uint32_t path_t;
	typedef uint32_t line_t;

#pragma pack(push,1)
	typedef struct _FrameCoord // информация о пути
	{
		_FrameCoord() :coordinate(0), path(0), line(0){}
		coordinate_t coordinate; // координата от начала координат в милиметрах
		path_t path;		// путь
		line_t line;		// линия
	}FrameCoord;

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

	typedef struct _FrameCoordPresentation // информация о пути
	{
		coordinate_t coordinate; // пройдено километров 
		path_t path;		// путь
		line_t line;
	}FrameCoordPresentation;

	struct IRBFramePresentation
	{
		FLOAT level;			// средний уровень температур
		FLOAT span;				// разброс температур
		double imgTime;			// время снимка
		FLOAT imgMilliSecTime;	// время снимка в миллисекундах
		WORD imgAccu;//
		union{
			char imageComment[80];	// комментарий
			//FrameCoordPresentation frameCoord;
			struct {
				char filler2[56];
				FrameCoordPresentation frameCoord;
			};
		};
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
#pragma pack(pop)



	typedef struct _irb_frame_key
	{
		_irb_frame_key(uint32_t xid, coordinate_t xcoordinate = 0, time_t xtime = 0) :id(xid), coordinate(xcoordinate), time(xtime){}
		_irb_frame_key() :id(0), coordinate(0), time(0){}
		uint32_t id;
		coordinate_t coordinate;
		time_t time;

		bool operator==(const uint32_t search_id) const
		{
			return id == search_id;
		}
		bool operator==(const coordinate_t search_coordinate) const
		{
			return coordinate == search_coordinate;
		}
		bool operator==(const time_t search_time) const
		{
			return time == search_time;
		}

	}irb_frame_key;


	class IRBFrame;
	using irb_frame_ptr_t = std::unique_ptr<IRBFrame>;

	using frame_id_t = uint32_t;

	using irb_pixel_t = WORD;
	using irb_frame_pixels_t = std::unique_ptr<irb_pixel_t[]>;
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
		BOOL ComputeMinMaxAvr(float * min, float * max, float * avr);
		BOOL GetPixelTemp(uint16_t x, uint16_t y, float * tempToReturn);

		float IRBFrame::retrieve_pixel_temperature(irb_pixel_t pixel);

		const irb_frame_pixels_t& getPixels(); // чтение всех пикселей
		irb_pixel_t getPixel(int x, int y); // чтение пикселя с заданными координатами
		//void getAllPixels();

		inline frame_id_t getFrameNum() const	{ return id; }


		irb_pixel_t GetPixelFromTemp(float temp);

		inline bool is_checked() const { return _is_checked; }
		inline void set_check_state(bool state = true) { _is_checked = state; }

		void set_temperature_measure(float min, float max, float average) { _temperature_span_calculated = true; min_temperature = min; max_temperature = max; avr_temperature = average; }
		bool is_temperature_span_calculated() const { return _temperature_span_calculated; }
		irb_pixel_t get_min_temperature_pixel() const { return _min_temperature_pixel; }
		irb_pixel_t get_max_temperature_pixel() const { return _max_temperature_pixel; }

	public:
		IRBFrameHeader header;
		irb_frame_pixels_t pixels;

		frame_id_t id;

		float min_temperature, max_temperature, avr_temperature;
		int points_num;

		FrameCoord coords;
		IRBSpec spec;
	private:
		using pixel_point_t = std::pair<uint16_t, uint16_t>;

		pixel_point_t _max_temperature_point;
		pixel_point_t _min_temperature_point;

		irb_pixel_t _max_temperature_pixel;
		irb_pixel_t _min_temperature_pixel;


		mutable bool _temperature_span_calculated;
		bool wasRead;
		bool _is_checked;
		bool _is_spec_set;

		friend std::istream & operator>>(std::istream & in, IRBFrame &irb_frame);
		friend std::ostream & operator<<(std::ostream & out, const IRBFrame &irb_frame);
	};
	

	std::istream & operator>>(std::istream & in, FrameCoord &frame_coordinate);
	std::ostream & operator<<(std::ostream & out, const FrameCoord &frame_coordinate);

	inline unsigned int serialized_irb_frame_size(const IRBFrame &irb_frame)
	{
		auto res = sizeof(IRBFrameHeader)+irb_frame.get_pixels_count()*irb_frame.header.geometry.pixelFormat;
		return (unsigned int)res;
	}


	inline time_t convert_irb_frame_time_in_sec(double tdatetime)
	{
		return (time_t)((tdatetime - 70 * 365 + 19) * 24 * 3600);
	}


	//bool LHDecode(const IRBFrameGeometry &geometry, BYTE *pixelsToDecode, ULONG dataSize, IRBFrame *frame);
	//bool Decode8Bit(BYTE *pixelsToDecode, LONG dataSize, IRBFrame *frame);
	//bool Decode16Bit(BYTE *pixelsToDecode, LONG dataSize, IRBFrame *frame);

	uint32_t get_frame_coordinate_type_offset();
	uint32_t get_frame_time_offset();

}
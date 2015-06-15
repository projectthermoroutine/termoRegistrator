#pragma once
#include <vector>
#include <memory>


// условия поиска с учетом фильтрации
enum class FILTER_SEARCH_TYPE{
	FILTER_NO,      // без учета фильтрации
	FILTER_PLUS,    // поиск вперед
	FILTER_MINUS,   // поиск назад
	FILTER_NEAREST // поиск ближайшей
};

// условия фильтрации

#ifndef FlagOn
#define FlagOn(_F,_SF)        ((_F) & (_SF))
#endif


namespace irb_frame_filter
{


	// условия фильтрации (битовые флаги)
#define  FILTER_TIME0      0x01   // начальное время
#define  FILTER_TIME1      0x02   // конечное время
#define  FILTER_METKA      0x04   // метка
#define  FILTER_TEMPFULL   0x08   // max отклонение температуры от среднего по кадру
#define  FILTER_TEMPREGION 0x10   // max отклонение температуры от среднего по области
#define  FILTER_PICKMIN    0x20   // минимальный пикет
#define  FILTER_PICKMAX    0x40   // максимальный пикет

	using irb_frame_shared_ptr_t = std::shared_ptr < irb_frame_helper::IRBFrame > ;

	using temperature_interval_t = std::pair < float, float > ;
	using time_interval_t = std::pair < double, double > ;
	struct FILTER
	{
	public:
		time_interval_t time;
		temperature_interval_t T;
		BYTE metka;
		DWORD flags;

	public:
		FILTER() :metka(0), flags(0)
		{
			time = { 0.0, 0.0 };
			T = { 0.0f, 0.0f };
		}

		FILTER(const FILTER &f)
		{
			copy(f);
		}

		FILTER(FILTER &&f)
		{
			copy(f);
		}
		FILTER & operator = (const FILTER & f)
		{
			copy(f);
			return *this;
		}
		FILTER & operator = (FILTER && f)
		{
			copy(f);
			return *this;
		}

	private:
		void copy(const FILTER & f)
		{
			T = f.T;
			time = f.time;
			metka = f.metka;
			flags = f.flags;
		}

		template<class TFrame>
		friend 	bool frame_filtered(const FILTER& filter, const TFrame& frame);

	};


	template<class TFrame>
	bool frame_filtered(const FILTER& filter, const TFrame& frame)
	{
		if (FlagOn(filter.flags, FILTER_METKA) && (frame->marked()))
			return false;

		bool result = true;

		if (FlagOn(filter.flags, FILTER_TEMPFULL) || FlagOn(filter.flags, FILTER_TEMPREGION))
		{
			frame->ComputeMinMaxAvr();

			auto high_deltaT = frame->maxT() - frame->avgT();

			if (FlagOn(filter.flags, FILTER_TEMPFULL) && high_deltaT <= filter.T.first)
				result = false;

			if (FlagOn(filter.flags, FILTER_TEMPREGION) && high_deltaT <= filter.T.second)
				result = false;
		}

		if (FlagOn(filter.flags, FILTER_TIME0) || FlagOn(filter.flags, FILTER_TIME1))
		{
			auto t = frame->get_frame_time_in_sec();
			if (FlagOn(filter.flags, FILTER_TIME0) && t < filter.time.first)
				result = false;
			if (FlagOn(filter.flags, FILTER_TIME1) && t > filter.time.second)
				result = false;
		}
		return result;
	}


}
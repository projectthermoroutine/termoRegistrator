#pragma once
#include <vector>
#include <memory>
#include "AreaBase.h"
#include "irb_frame_image_dispatcher.h"

//#pragma pack(show)

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
	using picket_interval_t = std::pair < uint16_t, uint16_t >;

	using areas_function_t = std::function<const areas_dispatcher&(void)>;

	struct FILTER
	{
	public:
		time_interval_t time;
		temperature_interval_t T;
		BYTE metka;
		DWORD flags;
		picket_interval_t picket;

		areas_function_t areas;
	public:
		FILTER(const areas_function_t & _areas = areas_function_t())
			:areas(_areas), metka(0), flags(0)
		{}

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
			if (this == &f)
				return;
			T = f.T;
			time = f.time;
			metka = f.metka;
			flags = f.flags;
			picket = f.picket;
			areas = f.areas;
		}

		template<class TFrame>
		friend 	bool frame_filtered(const FILTER& filter, const TFrame& frame);

	};


	template<class TFrame>
	bool frame_filtered(const FILTER& filter, const TFrame& frame)
	{
		if (FlagOn(filter.flags, FILTER_METKA) && (frame->marked()))
			return false;

		bool has_min_picket = true;
		if (FlagOn(filter.flags, FILTER_PICKMIN) && (frame->coords.picket < filter.picket.first))
			has_min_picket = false;

		if (has_min_picket && FlagOn(filter.flags, FILTER_PICKMAX) && (frame->coords.picket <= filter.picket.second))
			return false;
		
		if (!FlagOn(filter.flags, FILTER_PICKMAX) && FlagOn(filter.flags, FILTER_PICKMIN) && has_min_picket)
			return false;

		bool result = true;

		if (FlagOn(filter.flags, FILTER_TEMPFULL))
		{
			frame->ComputeMinMaxAvr();

			auto high_deltaT = frame->maxT() - frame->avgT();

			if (FlagOn(filter.flags, FILTER_TEMPFULL) && high_deltaT <= filter.T.first)
				result = false;
		}

		if (FlagOn(filter.flags, FILTER_TEMPREGION) && filter.areas)
		{
			auto & areas = filter.areas();

			irb_frame_image_dispatcher::retrieve_areas_T(frame, nullptr, const_cast<areas_dispatcher&>(areas));
			auto max_area_T = filter.T.second;

			areas.for_each_area_temperature_measure([&result, &max_area_T](const area_temperature_measure &measure)
			{
				auto high_deltaT = measure.max - measure.avr;

				if (high_deltaT <= max_area_T)
					result = false;
			});


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
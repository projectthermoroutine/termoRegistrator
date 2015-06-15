#pragma once
#include <vector>
#include <memory>


// ������� ������ � ������ ����������
enum class FILTER_SEARCH_TYPE{
	FILTER_NO,      // ��� ����� ����������
	FILTER_PLUS,    // ����� ������
	FILTER_MINUS,   // ����� �����
	FILTER_NEAREST // ����� ���������
};

// ������� ����������

#ifndef FlagOn
#define FlagOn(_F,_SF)        ((_F) & (_SF))
#endif


namespace irb_frame_filter
{


	// ������� ���������� (������� �����)
#define  FILTER_TIME0      0x01   // ��������� �����
#define  FILTER_TIME1      0x02   // �������� �����
#define  FILTER_METKA      0x04   // �����
#define  FILTER_TEMPFULL   0x08   // max ���������� ����������� �� �������� �� �����
#define  FILTER_TEMPREGION 0x10   // max ���������� ����������� �� �������� �� �������
#define  FILTER_PICKMIN    0x20   // ����������� �����
#define  FILTER_PICKMAX    0x40   // ������������ �����

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
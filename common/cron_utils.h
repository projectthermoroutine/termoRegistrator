/////////////////////////////////////
// (C) 2014 ��� "��� ������������"
// ������: SNES-AV eset
// �����: ������ �����
// ������: 02.10.2014
// ������� ��������: ��������� �������� ������ �� ��������� ������� � ������������ ��������� ��������
/////////////////////////////////////

#pragma once

#include <ctime>
#include <string>

namespace cron_utils
{

	struct time_interval_t
	{
		std::time_t begin;
		std::time_t end;
	};

	bool check_time_interval(const time_interval_t& interval);
	bool check_event_has_occurred(const std::wstring& cron_expression, const time_interval_t& time_interval);

}
#pragma once
#include <Windows.h>

typedef struct _area_temperature_measure
{
	float min;
	float max;
	float avr;
}area_temperature_measure;

typedef struct _area_temperature_measure_result
{
	int area_id;
	bool result_is_valid;
	area_temperature_measure measure;
}area_temperature_measure_result;

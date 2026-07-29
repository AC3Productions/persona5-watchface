// resources.h
#ifndef RESOURCE_IDS_H
#define RESOURCE_IDS_H

#include <stdint.h>
#include <stdbool.h>

#include "weather.h"
#include "time.h"

// These functions are all unchecked array access
uint32_t GetDayResourceID(int layer, int day);
uint32_t GetWeekdayResourceID(int layer, int weekday);
uint32_t GetMonthResourceID(int layer, int month, bool two_digit);
uint32_t GetWeatherResourceID(enum WeatherIcon weather, bool two_digit);
uint32_t GetTimeOfDayResourceID(enum TimeOfDay tod);
#endif // #ifndef RESORUCE_IDS_H

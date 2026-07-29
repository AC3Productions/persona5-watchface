// calendar.h
// Calendar Widget (displays, day, month, weekday, and weather condition)
#ifndef CALENDAR_H
#define CALENDAR_H

#include "weather.h"

void CalendarInitResources();
void CalendarUnloadResources();
void CalendarAddLayers(Layer* parent_layer);

void CalendarUpdateDay(int day);
void CalendarUpdateMonth(int month);
void CalendarUpdateWeekday(int weekday);
void CalendarUpdateWeatherIcon(enum WeatherIcon icon);

void CalendarSetAnimationDisabled(bool disabled);

#endif // #ifndef CALENDAR_H

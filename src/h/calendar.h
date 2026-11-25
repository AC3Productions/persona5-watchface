// calendar.h
// Calendar Widget (displays, day, month, weekday, and weather condition)
#ifndef CALENDAR_H
#define CALENDAR_H

#include "weather.h"

enum AnimationMode
{
    AnimDisabled = 0,
    AnimAlways,
    AnimBacklight,
};

enum AnimationMode CalendarGetAnimationMode(const char* anim_mode);

void CalendarInitResources();
void CalendarUnloadResources();
void CalendarAddLayers(Layer* parent_layer);

void CalendarUpdateDay(int day);
void CalendarUpdateMonth(int month);
void CalendarUpdateWeekday(int weekday);
void CalendarUpdateWeatherIcon(enum WeatherIcon icon);

void CalendarSetAnimationMode(enum AnimationMode anim_mode);

#endif // #ifndef CALENDAR_H

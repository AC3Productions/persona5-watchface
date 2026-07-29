// time.h
// Time-dependent assets (current time, time of day text, BG day/night)
#ifndef TIME_H
#define TIME_H
#include <pebble.h>

enum BackgroundMode 
{
    BGNone = -1,
    BGDayOnly = 0,
    BGNightOnly,
    BGConstantTimes,
    BGSunriseSunset
};

enum TimeOfDay
{
    TimeNone = -1,
    TimeEarlyMorning = 0,
    TimeMorning,
    TimeDaytime,
    TimeAfternoon,
    TimeEvening,

    TimeCount,
};

enum BackgroundMode GetBackgroundMode(const char* mode);

void TimeInitResources(GRect bounds);
void TimeUnloadResources();
void TimeAddLayers(Layer* parent_layer);

void TimeSetBackgroundMode(enum BackgroundMode);
void TimeUpdateSunTimes(int rise_h, int rise_m, int set_h, int set_m);

void SetTime(int hour, int min);
void SetTimeOfDayVisible(bool visible);

#endif // #ifndef TIME_H

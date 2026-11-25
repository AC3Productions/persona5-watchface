// positions.h
#ifndef POSITIONS_H
#define POSITIONS_H

#include <pebble.h>

// Applies to all parts of the Calendar Widget (incl. weather)
#define CALENDAR_POS_X 15
#define CALENDAR_POS_Y 5

// Offsets are relative to CALENDAR_POS defines above
#define WEATHER_OFFSET_X 105
#define WEATHER_2DIGIT_OFFSET_X 112
#define WEATHER_OFFSET_Y 1

#define TIME_POS_X 0
#define TIME_POS_Y 106
#define TIME_DROP_SHADOW_OFFSET 3

// Time of Day can be disabled, so two possible x positions for temperature
#define TEMP_POS_X -35
#define TEMP_POS_Y 175
#define TEMP_NO_TOD_POS_X 5
#define TEMP_DROP_SHADOW_OFFSET 3

#define TOD_POS_X 118
#define TOD_POS_Y 172

// Gets day position relative to calendar position
// (so must use CALENDAR_POS defines in addition to this when calculating positions)
GPoint GetDayPosition(int day);

GPoint GetMonthPosition(int month, bool is_2digit);

GPoint GetWeekPosition(int weekday);

#endif // #ifndef POSITIONS_H

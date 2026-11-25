// positions.c

#include "h/positions.h"

// Hand-measured offsets for each day of the month
// relative to calendar origin
static const GPoint s_day_positions[] = 
{
    {0, 0}, // dummy entry
    {63, 7},
    {55, 12},
    {59, 10},
    {54, 9},
    {56, 9},
    {58, 11},
    {58, 7},
    {57, 9},
    {56, 9},
    {44, 8},
    {44, 8},
    {44, 11},
    {44, 10},
    {44, 10},
    {44, 10},
    {44, 11},
    {44, 10},
    {44, 9},
    {44, 9},
    {46, 8},
    {46, 8},
    {46, 12},
    {46, 10},
    {46, 9},
    {46, 10},
    {46, 12},
    {46, 9},
    {46, 9},
    {46, 10},
    {45, 8},
    {45, 8},
};

typedef struct MonthPos
{
    GPoint pos;
    int16_t x_2digit; 
} MonthPos;

static const MonthPos MonthPosition[] = 
{
    {{23, 18}, 10}, // -13
    {{23, 17}, 9},  // -14
    {{17, 18}, 8},  // -9
    {{21, 17}, 9},  // -12
    {{22, 19}, 10}, // -12
    {{27, 19}, 16}, // -11
    {{28, 19}, 16}, // -12
    {{23, 18}, 12}, // -11
    {{22, 18}, 12}, // -10
    {{13, 15}, 1},  // -12
    {{17, 17}, 5}, // -12
    {{13, 13}, 1}, // -12
};

static const GPoint WeekdayOffset[] = 
{
    {58, 37}, // Sunday 
    {52, 32}, // Monday
    {50, 32}, // Tuesday
    {55, 32}, // Wednesday
    {50, 33}, // Thursday
    {51, 36}, // Friday
    {59, 32}, // Saturday
};

GPoint GetDayPosition(int day)
{
    return s_day_positions[day];
}

GPoint GetMonthPosition(int month, bool is_2digit)
{
    if (is_2digit)
    {
        return GPoint(MonthPosition[month].x_2digit, MonthPosition[month].pos.y);
    }

    return MonthPosition[month].pos;

}

GPoint GetWeekPosition(int weekday)
{
    return WeekdayOffset[weekday];
}

// calendar.c
#include <pebble.h>

#include "h/calendar.h"
#include "h/positions.h"
#include "h/resource_ids.h"
#include "h/weather.h"

/* Internal state */

// 1-indexed (1-31)
static int curr_day = -1;
// 0-indexed ("months since january")
static int curr_mon = -1;
static int curr_weekday = -1;
static enum WeatherIcon curr_weather_icon = WeatherIconUnknown;

static bool curr_2digit = false;

static AppTimer* s_weather_anim_timer = NULL;
static bool animation_enabled = false;

/* Assets & resources */

static GBitmap* s_day_bitmap[3] = {NULL};
static BitmapLayer* s_day_layer[3] = {NULL};

static GBitmap* s_month_bitmap[3] = {NULL};
static BitmapLayer* s_month_layer[3] = {NULL};

static GBitmap* s_weekday_bitmap[3] = {NULL};
static BitmapLayer* s_weekday_layer[3] = {NULL};

static GBitmapSequence* s_weather_bitmap_sequence = NULL;
static GBitmap* s_weather_bitmap = NULL;
static BitmapLayer* s_weather_layer = NULL;

/* Private function declarations */ 
static void WeatherFrameAdvanceEvent(void *context) ;


void CalendarInitResources()
{
    // Day of the month
    for (int i = 0; i < 3; ++i)
    {
        s_day_bitmap[i] = gbitmap_create_with_resource(GetDayResourceID(i, 0));
        GRect bounds = gbitmap_get_bounds(s_day_bitmap[i]);
        bounds.origin.x += CALENDAR_POS_X;
        bounds.origin.y += CALENDAR_POS_Y;
        s_day_layer[i] = bitmap_layer_create(bounds);
        bitmap_layer_set_bitmap(s_day_layer[i], s_day_bitmap[i]);
        bitmap_layer_set_compositing_mode(s_day_layer[i], GCompOpSet);
    }

    // Month
    for (int i = 0; i < 3; ++i)
    {    
        s_month_bitmap[i] = gbitmap_create_with_resource(GetMonthResourceID(i, 0, false));
        GRect bounds = gbitmap_get_bounds(s_month_bitmap[i]);
        bounds.origin.x += CALENDAR_POS_X;
        bounds.origin.y += CALENDAR_POS_Y;
        s_month_layer[i] = bitmap_layer_create(bounds);
        bitmap_layer_set_bitmap(s_month_layer[i], s_month_bitmap[i]);
        bitmap_layer_set_compositing_mode(s_month_layer[i], GCompOpSet);
    }

    // Weekday
    for (int i = 0; i < 3; ++i)
    {    
        s_weekday_bitmap[i] = gbitmap_create_with_resource(GetWeekdayResourceID(i, 0));
        GRect bounds = gbitmap_get_bounds(s_weekday_bitmap[i]);
        bounds.origin.x += CALENDAR_POS_X;
        bounds.origin.y += CALENDAR_POS_Y;
        s_weekday_layer[i] = bitmap_layer_create(bounds);
        bitmap_layer_set_bitmap(s_weekday_layer[i], s_weekday_bitmap[i]);
        bitmap_layer_set_compositing_mode(s_weekday_layer[i], GCompOpSet);
    }

    // Current weather
    curr_weather_icon = WeatherIconUnknown;

    uint32_t curr_weather_id = GetWeatherResourceID(curr_weather_icon, curr_2digit);

    s_weather_bitmap_sequence = gbitmap_sequence_create_with_resource(curr_weather_id);
    GSize frame_size = gbitmap_sequence_get_bitmap_size(s_weather_bitmap_sequence);
    gbitmap_sequence_set_play_count(s_weather_bitmap_sequence, PLAY_COUNT_INFINITE);
    s_weather_bitmap = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);
    GRect bounds = gbitmap_get_bounds(s_weather_bitmap);
    bounds.origin.x += CALENDAR_POS_X;
    bounds.origin.y += CALENDAR_POS_Y;
    s_weather_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(s_weather_layer, s_weather_bitmap);
    bitmap_layer_set_compositing_mode(s_weather_layer, GCompOpSet);

    gbitmap_sequence_restart(s_weather_bitmap_sequence);
    gbitmap_sequence_update_bitmap_next_frame(s_weather_bitmap_sequence, s_weather_bitmap, NULL);
}

void CalendarUnloadResources()
{
    for (int i = 0; i < 3; ++i)
    {
        gbitmap_destroy(s_day_bitmap[i]);
        gbitmap_destroy(s_month_bitmap[i]);
        gbitmap_destroy(s_weekday_bitmap[i]);
    }
    gbitmap_destroy(s_weather_bitmap);
    gbitmap_sequence_destroy(s_weather_bitmap_sequence);

    for (int i = 0; i < 3; ++i)
    {
        bitmap_layer_destroy(s_day_layer[i]);
        bitmap_layer_destroy(s_month_layer[i]);
        bitmap_layer_destroy(s_weekday_layer[i]);
    }
    bitmap_layer_destroy(s_weather_layer);

    app_timer_cancel(s_weather_anim_timer);
}

void CalendarAddLayers(Layer* parent_layer)
{
    for (int i = 0; i < 3; ++i)
    {
        layer_add_child(parent_layer, bitmap_layer_get_layer(s_day_layer[i]));
        layer_add_child(parent_layer, bitmap_layer_get_layer(s_month_layer[i]));
        layer_add_child(parent_layer, bitmap_layer_get_layer(s_weekday_layer[i]));

        if (i == 0)
        {
            layer_add_child(parent_layer, bitmap_layer_get_layer(s_weather_layer));
        }
    }
}

// Day of the month (1-31)
void CalendarUpdateDay(int day)
{   
    if (curr_day == day)
    {
        return;
    } 

    curr_day = day;
    for (int i = 0; i < 3; ++i)
    {
        uint32_t curr_day_id = GetDayResourceID(i, day - 1);

        GBitmap* old_bitmap = s_day_bitmap[i];
        s_day_bitmap[i] = gbitmap_create_with_resource(curr_day_id);
        bitmap_layer_set_bitmap(s_day_layer[i], s_day_bitmap[i]);
        gbitmap_destroy(old_bitmap);
    }

    // Month and weather need to update if switching between 1-digit and 2-digit day
    bool day_digits_changed = false;
    if (day > 9)
    {
        // 9 -> 10
        if (!curr_2digit)
        {
            curr_2digit = true;
            day_digits_changed = true;
        }
    }
    else 
    {
        // End of month -> 1
        if (curr_2digit)
        {
            curr_2digit = false;
            day_digits_changed = true;
        }
    }

    if (day_digits_changed)
    {
        CalendarUpdateMonth(curr_mon);
        CalendarUpdateWeatherIcon(curr_weather_icon);
    }
}

void CalendarUpdateMonth(int month)
{
    if (curr_mon == month)
    {
        return;
    }

    curr_mon = month;
    for (int i = 0; i < 3; ++i)
    {
        uint32_t curr_mon_id = GetMonthResourceID(i, curr_mon, curr_2digit);

        GBitmap* old_bitmap = s_month_bitmap[i];
        s_month_bitmap[i] = gbitmap_create_with_resource(curr_mon_id);
        bitmap_layer_set_bitmap(s_month_layer[i], s_month_bitmap[i]);
        gbitmap_destroy(old_bitmap);
    }
}

void CalendarUpdateWeekday(int weekday)
{
    if (curr_weekday == weekday)
    {
        return;
    } 

    curr_weekday = weekday;
    for (int i = 0; i < 3; ++i)
    {
        // day is 1-indexed
        uint32_t curr_weekday_id = GetWeekdayResourceID(i, curr_weekday);

        GBitmap* old_bitmap = s_weekday_bitmap[i];
        s_weekday_bitmap[i] = gbitmap_create_with_resource(curr_weekday_id);
        bitmap_layer_set_bitmap(s_weekday_layer[i], s_weekday_bitmap[i]);
        gbitmap_destroy(old_bitmap);
    }
}

void CalendarUpdateWeatherIcon(enum WeatherIcon icon)
{
    if (curr_weather_icon == icon)
    {
        return;
    }

    curr_weather_icon = icon;
    if (animation_enabled)
    {
        // Stop the animation, swap the bitmap, start the animation over
        app_timer_cancel(s_weather_anim_timer);
    }

    uint32_t curr_weather_id = GetWeatherResourceID(curr_weather_icon, curr_2digit);
    GBitmapSequence* old_seq = s_weather_bitmap_sequence;
    s_weather_bitmap_sequence = gbitmap_sequence_create_with_resource(curr_weather_id);
    gbitmap_sequence_set_play_count(s_weather_bitmap_sequence, PLAY_COUNT_INFINITE);
    gbitmap_sequence_destroy(old_seq);
    
    gbitmap_sequence_restart(s_weather_bitmap_sequence);
    gbitmap_sequence_update_bitmap_next_frame(s_weather_bitmap_sequence, s_weather_bitmap, NULL);
    
    if (animation_enabled)
    {
        s_weather_anim_timer = app_timer_register(500, WeatherFrameAdvanceEvent, NULL);
    }
}

void CalendarSetAnimationDisabled(bool disabled)
{
    animation_enabled = !disabled;

    if (animation_enabled)
    {
        // Animation just started
        if (!s_weather_anim_timer)
        {
            s_weather_anim_timer = app_timer_register(0, WeatherFrameAdvanceEvent, NULL);
        }
    }
    else
    {
        if (s_weather_anim_timer)
        {
            app_timer_cancel(s_weather_anim_timer);
            s_weather_anim_timer = NULL;
        }
        // Set to first frame of the animation
        gbitmap_sequence_restart(s_weather_bitmap_sequence);
        gbitmap_sequence_update_bitmap_next_frame(s_weather_bitmap_sequence, s_weather_bitmap, NULL);
        bitmap_layer_set_bitmap(s_weather_layer, s_weather_bitmap);
    }
}

static void WeatherFrameAdvanceEvent(void *context) 
{
    // Advance to the next APNG frame, and get the delay for this frame
    if (gbitmap_sequence_update_bitmap_next_frame(s_weather_bitmap_sequence, s_weather_bitmap, NULL))
    {
        // Set the new frame into the BitmapLayer
        bitmap_layer_set_bitmap(s_weather_layer, s_weather_bitmap);
        layer_mark_dirty(bitmap_layer_get_layer(s_weather_layer));

        // Timer for that frame's delay
        s_weather_anim_timer = app_timer_register(500, WeatherFrameAdvanceEvent, NULL);
    }
}

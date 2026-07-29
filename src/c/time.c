// time.c
#include <pebble.h>
#include "h/positions.h"
#include "h/resource_ids.h"
#include "h/time.h"

// background (day/night)
static BitmapLayer* s_bg_layer = NULL;
static GBitmap* s_bg_bitmap = NULL;
static enum BackgroundMode bg_mode = BGSunriseSunset;
static uint32_t bg_resource_id = 0;

// sunrise/sunset info
static int sunrise_hour = -1;
static int sunrise_min = -1;
static int sunset_hour = -1;
static int sunset_min = -1;

// Current time
static GFont s_time_font;
static TextLayer* s_time_layer = NULL;
static TextLayer* s_time_drop_shadow_layer = NULL;
static int curr_hour = -1;
static int curr_min = -1;

// time of day
enum TimeOfDay curr_tod = TimeNone;
enum TimeOfDay next_tod = TimeNone;
static GBitmap* s_tod_bitmap = NULL;
static BitmapLayer* s_tod_layer= NULL;
static bool tod_visible = true;

// entries are { hour, minute }
static const int TimeOfDayTimes[TimeCount][2] = 
{
    // Early Morning
    { 5, 0 },
    // Morning
    { 8, 0 },
    // Daytime
    { 11, 0 },
    // Afternoon
    { 14, 0 },
    // Evening
    { 20, 0 }
};

static void UpdateCurrentTime();
static void UpdateCurrentTimeOfDay();
static void UpdateCurrentBG();

enum BackgroundMode GetBackgroundMode(const char* mode)
{
    if (strcmp(mode, "day_only") == 0)
    {
        return BGDayOnly;
    }

    if (strcmp(mode, "night_only") == 0)
    {
        return BGNightOnly;
    }

    if (strcmp(mode, "constant_times") == 0)
    {
        return BGConstantTimes;
    }

    if (strcmp(mode, "sunrise_sunset") == 0)
    {
        return BGSunriseSunset;
    }

    return BGNone;
}

void TimeInitResources(GRect bounds)
{
    // Create the bg layer
    //s_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NIGHT_BG);
    s_bg_layer = bitmap_layer_create(bounds);
    //bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);


    // Current time (including a dropshadow)
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_POLAND_CANNED_INTO_KAITO_40));

    s_time_layer = text_layer_create(
        GRect(TIME_POS_X, TIME_POS_Y, bounds.size.w, 120));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    s_time_drop_shadow_layer = text_layer_create(
        GRect(TIME_POS_X + TIME_DROP_SHADOW_OFFSET, TIME_POS_Y + TIME_DROP_SHADOW_OFFSET, bounds.size.w, 120));
    text_layer_set_background_color(s_time_drop_shadow_layer, GColorClear);
    text_layer_set_text_color(s_time_drop_shadow_layer, GColorBlack);
    text_layer_set_font(s_time_drop_shadow_layer, s_time_font);
    text_layer_set_text_alignment(s_time_drop_shadow_layer, GTextAlignmentCenter);

    // Time of day
    s_tod_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TIME_DAYTIME);
    GRect tod_bounds = gbitmap_get_bounds(s_tod_bitmap);
    tod_bounds.origin.x += TOD_POS_X;
    tod_bounds.origin.y += TOD_POS_Y;
    s_tod_layer = bitmap_layer_create(tod_bounds);
    bitmap_layer_set_compositing_mode(s_tod_layer, GCompOpSet);
    bitmap_layer_set_bitmap(s_tod_layer, s_tod_bitmap);
}

void TimeUnloadResources()
{
    gbitmap_destroy(s_bg_bitmap);
    gbitmap_destroy(s_tod_bitmap);

    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_time_drop_shadow_layer);
    bitmap_layer_destroy(s_bg_layer);
}


void TimeAddLayers(Layer* parent_layer)
{
    layer_add_child(parent_layer, bitmap_layer_get_layer(s_bg_layer));
    layer_add_child(parent_layer, bitmap_layer_get_layer(s_tod_layer));

    layer_add_child(parent_layer, text_layer_get_layer(s_time_drop_shadow_layer));
    layer_add_child(parent_layer, text_layer_get_layer(s_time_layer));
}

void TimeSetBackgroundMode(enum BackgroundMode new_mode)
{
    if (new_mode == bg_mode)
    {
        return;
    }

    bg_mode = new_mode;

    UpdateCurrentBG();
}

void TimeUpdateSunTimes(int rise_h, int rise_m, int set_h, int set_m)
{
    sunrise_hour = rise_h;
    sunrise_min = rise_m;
    sunset_hour = set_h;
    sunset_min = set_m;

    UpdateCurrentBG();
}

void SetTime(int hour, int min)
{
    curr_hour = hour;
    curr_min = min;

    UpdateCurrentTime();
    UpdateCurrentBG();
    UpdateCurrentTimeOfDay();
}

void SetTimeOfDayVisible(bool visible)
{
    if (tod_visible == visible)
    {
        return;
    }

    tod_visible = visible;

    // Only load the time of day asset if it's actually visible
    if (tod_visible)
    {
        if (!s_tod_bitmap)
        {
            s_tod_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TIME_DAYTIME);
        }
        GRect tod_bounds = gbitmap_get_bounds(s_tod_bitmap);
        tod_bounds.origin.x += TOD_POS_X;
        tod_bounds.origin.y += TOD_POS_Y;
        if (!s_tod_layer)
        {
            s_tod_layer = bitmap_layer_create(tod_bounds);
        }
        bitmap_layer_set_compositing_mode(s_tod_layer, GCompOpSet);
        bitmap_layer_set_bitmap(s_tod_layer, s_tod_bitmap);
    }
    else
    {
        if (s_tod_bitmap)
        {
            gbitmap_destroy(s_tod_bitmap);
            s_tod_bitmap = NULL;
        }    
    }
    
    layer_set_hidden(bitmap_layer_get_layer(s_tod_layer), !tod_visible);
}

static void UpdateCurrentTime()
{
    static char s_time_buffer[8];

    if (clock_is_24h_style())
    {
        snprintf(s_time_buffer, sizeof(s_time_buffer), "%02d:%02d", curr_hour, curr_min);
        text_layer_set_text(s_time_layer, s_time_buffer);
        text_layer_set_text(s_time_drop_shadow_layer, s_time_buffer);
    }
    else
    {
        int display_hour = curr_hour % 12;
        if (display_hour == 0)
        {
            display_hour = 12;
        }
    
        snprintf(s_time_buffer, sizeof(s_time_buffer), "%i:%02d", display_hour, curr_min);

        const char* no_leading_zero = s_time_buffer;
        if (s_time_buffer[0] == '0')
        {
            no_leading_zero = no_leading_zero + 1;
        }

        text_layer_set_text(s_time_layer, no_leading_zero);
        text_layer_set_text(s_time_drop_shadow_layer, no_leading_zero);
    }
}

static void UpdateCurrentTimeOfDay()
{
    // normal case, just check for the next time
    bool tod_changed = false;
    if (curr_tod != TimeNone)
    {
        if (curr_hour == TimeOfDayTimes[next_tod][0] && curr_min == TimeOfDayTimes[next_tod][1])
        {
            tod_changed = true;
        }
    }
    else
    {
        for (int i = 0; i < TimeCount; ++i)
        {
            if (curr_hour >= TimeOfDayTimes[i][0])
            {
                next_tod = (enum TimeOfDay) i;
                continue;
            }
            break;
        }
        if (next_tod == TimeNone)
        {
            next_tod = TimeEvening;
        }
        tod_changed = true;
    }

    if (tod_changed)
    {
        curr_tod = next_tod;
        next_tod = (next_tod + 1) % TimeCount;

        uint32_t curr_tod_id = GetTimeOfDayResourceID(curr_tod);

        GBitmap* old_bitmap = s_tod_bitmap;
        s_tod_bitmap = gbitmap_create_with_resource(curr_tod_id);
        bitmap_layer_set_bitmap(s_tod_layer, s_tod_bitmap);
        gbitmap_destroy(old_bitmap);
    }
}

// Based on time and currently set sunrise/sunset times
static bool IsDaytime()
{
    if (sunrise_hour == -1)
    {
        return true;
    }

    // 3:30PM = 15.5 
    float hours_passed = (float)curr_hour + (float)curr_min / 60.0f;
    float sunrise_time = (float)sunrise_hour + (float)sunrise_min / 60.0f;
    float sunset_time = (float)sunset_hour + (float)sunset_min / 60.0f;

    if (hours_passed < sunrise_time)
    {
        return false;
    }
    if (hours_passed < sunset_time)
    {
        return true;
    }
    return false;
}

static void UpdateCurrentBG()
{
    // false means night
    bool bg_day = true;

    if (bg_mode == BGNightOnly)
    {
        bg_day = false;
    }
    else if (bg_mode == BGConstantTimes || bg_mode == BGSunriseSunset)
    {
        bg_day = IsDaytime();
    }

    if (bg_day)
    {
        if (bg_resource_id == RESOURCE_ID_DAY_BG)
        {
            return;
        }

        bg_resource_id = RESOURCE_ID_DAY_BG;
    }
    else
    {
        if (bg_resource_id == RESOURCE_ID_NIGHT_BG)
        {
            return;
        }
        bg_resource_id = RESOURCE_ID_NIGHT_BG;
    }

    GBitmap* old_bitmap = s_bg_bitmap;
    gbitmap_destroy(old_bitmap);
    s_bg_bitmap = gbitmap_create_with_resource(bg_resource_id);
    bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);
}

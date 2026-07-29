#include <math.h>
#include <pebble.h>

#include "h/calendar.h"
#include "h/clay_settings.h"
#include "h/positions.h"
#include "h/resource_ids.h"
#include "h/time.h"
#include "h/weather.h"

static Window* s_main_window = NULL;

// To be called when the weather icon needs to be updated

static void update_time()
{
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    SetTime(tick_time->tm_hour, tick_time->tm_min);
    CalendarUpdateDay(tick_time->tm_mday);
    CalendarUpdateMonth(tick_time->tm_mon);
    CalendarUpdateWeekday(tick_time->tm_wday);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    update_time();

    // Get weather update every 30 minutes
    if (tick_time->tm_min % 30 == 0) 
    {
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_WEATHER, 1);
        app_message_outbox_send();
    }

    // At midnight, get sunrise and sunset for the next day
    if (tick_time->tm_hour == 0 && tick_time->tm_min == 0)
    {
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUNTIMES, 1);
        app_message_outbox_send();
    }
}

// Adjust where stuff is & load/unload stuff depending on settings
static void UpdateFromSettings()
{
    bool show_tod = SettingsShowToD();
    SetTimeOfDayVisible(show_tod);
    TemperatureSetPosition(show_tod);

    enum BackgroundMode bg_mode = SettingsGetBackgroundMode();
    TimeSetBackgroundMode(bg_mode);
    if (bg_mode == BGConstantTimes)
    {
        TimeUpdateSunTimes(SettingsGetConstantSunriseHour(),
                           SettingsGetConstantSunriseMinute(),
                           SettingsGetConstantSunsetHour(),
                           SettingsGetConstantSunsetMinute());
    }
    // We don't know if the current sun times are good, so send the request for them
    else if (bg_mode == BGSunriseSunset)
    {
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUNTIMES, 1);
        app_message_outbox_send();
    }

    SetFahrenheit(SettingsUsingFahrenheit());
    
    CalendarSetAnimationDisabled(SettingsGetAnimationDisabled());
}

static void main_window_load(Window *window)
{
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    // Create the time TextLayer
    TimeInitResources(bounds);
    TemperatureInitResources(bounds);
    
    CalendarInitResources();

    // Add layers to the Window
    TimeAddLayers(window_layer);
    CalendarAddLayers(window_layer);
    TemperatureAddLayers(window_layer);

    UpdateFromSettings();
}

static void main_window_unload(Window *window)
{
    printf("unloading...");

    CalendarUnloadResources();
    TimeUnloadResources();
    TemperatureUnloadResources();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
    Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
    Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

    if (temp_tuple && conditions_tuple) 
    {
        SetWeatherConnected(true);

        SetTemperature((int)temp_tuple->value->int32);

        enum WeatherIcon new_icon = GetWeatherIcon((int)conditions_tuple->value->int32);
        CalendarUpdateWeatherIcon(new_icon);
    }

    // These are for real sunrise/sunset
    if (SettingsGetBackgroundMode() == BGSunriseSunset)
    {
        Tuple *sunrise_h_tuple = dict_find(iterator, MESSAGE_KEY_SUNRISE_HOUR);
        Tuple *sunrise_m_tuple = dict_find(iterator, MESSAGE_KEY_SUNRISE_MIN);
        Tuple *sunset_h_tuple = dict_find(iterator, MESSAGE_KEY_SUNSET_HOUR);
        Tuple *sunset_m_tuple = dict_find(iterator, MESSAGE_KEY_SUNSET_MIN);
        if (sunrise_h_tuple && sunrise_m_tuple && sunset_h_tuple && sunset_m_tuple)
        {
            TimeUpdateSunTimes(sunrise_h_tuple->value->int32, sunrise_m_tuple->value->int32, 
                               sunset_h_tuple->value->int32, sunset_m_tuple->value->int32);
        }
    }

    // Check for Clay settings
    bool bg_settings_changed = false;
    Tuple *background_t = dict_find(iterator, MESSAGE_KEY_BackgroundMode);
    if (background_t) 
    {
        SettingsSetBackgroundMode(GetBackgroundMode(background_t->value->cstring));
        bg_settings_changed = true;
    }

    // User-set Sunrise and Sunset times
    if (SettingsGetBackgroundMode() == BGConstantTimes)
    {
        Tuple *constant_sunrise_t = dict_find(iterator, MESSAGE_KEY_ConstantSunrise);
        if (constant_sunrise_t)
        {
            int sunrise_h = constant_sunrise_t->value->int32;
            int sunrise_m = 0;

            SettingsSetSunriseTimes(sunrise_h, sunrise_m);
            bg_settings_changed = true;
        }
        
        Tuple *constant_sunset_t = dict_find(iterator, MESSAGE_KEY_ConstantSunset);
        if (constant_sunset_t)
        {
            int sunset_h = constant_sunset_t->value->int32;
            int sunset_m = 0;

            SettingsSetSunsetTimes(sunset_h, sunset_m);
            bg_settings_changed = true;
        }
    }
    

    Tuple *temp_unit_t = dict_find(iterator, MESSAGE_KEY_TemperatureUnit);
    if (temp_unit_t) 
    {
        SettingsSetFahrenheit(temp_unit_t->value->int32 == 1);
    }

    Tuple *show_tod_t = dict_find(iterator, MESSAGE_KEY_ShowTimeOfDay);
    if (show_tod_t) 
    {
        SettingsSetShowTimeOfDay(show_tod_t->value->int32 == 1);
    }

    Tuple *disable_anim_t = dict_find(iterator, MESSAGE_KEY_DisableAnimation);
    if (disable_anim_t) 
    {
        SettingsSetAnimationDisabled(disable_anim_t->value->int32 == 1);
    }

    // Save and apply if any settings were changed
    if (background_t || temp_unit_t || show_tod_t || disable_anim_t || bg_settings_changed) 
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "settings updated");

        prv_save_settings();
        UpdateFromSettings();
        
        // Refetch weather if the temperature unit changed so the display updates
        if (temp_unit_t) 
        {
            DictionaryIterator *iter;
            app_message_outbox_begin(&iter);
            dict_write_uint8(iter, MESSAGE_KEY_REQUEST_WEATHER, 1);
            app_message_outbox_send();
        }

        if (background_t && SettingsGetBackgroundMode() == BGSunriseSunset)
        {
            DictionaryIterator *iter;
            app_message_outbox_begin(&iter);
            dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUNTIMES, 1);
            app_message_outbox_send();
        }
    }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init()
{
    prv_load_settings();

    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set the background color
    window_set_background_color(s_main_window, GColorBlack);

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) 
    {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    // Make sure the time is displayed from the start
    update_time();

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    // Register AppMessage callbacks
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    // Open AppMessage
    const int inbox_size = 128;
    const int outbox_size = 128;
    app_message_open(inbox_size, outbox_size);
}

static void deinit()
{
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void)
{
    init();
    app_event_loop();
    deinit();
}

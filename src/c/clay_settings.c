#include <pebble.h>
#include "h/clay_settings.h"
#include "h/calendar.h"

#define SETTINGS_KEY 1

typedef struct ClaySettings 
{
    enum BackgroundMode bg_setting;
    // Only used with Constant Time bg setting
    int sunrise_h;
    int sunrise_m;
    int sunset_h;
    int sunset_m;

    bool use_fahrenheit; // deprecated

    bool show_tod;

    enum AnimationMode anim_mode; // todo: replace with enum for anim mode
} ClaySettings;

static ClaySettings settings;

void prv_default_settings() 
{
    settings.bg_setting = BGSunriseSunset;
    
    settings.sunrise_h = 5;
    settings.sunrise_m = 0;
    settings.sunset_h = 19;
    settings.sunset_m = 0;

    settings.use_fahrenheit = true;
    settings.show_tod = true;
    settings.anim_mode = AnimAlways;
}

void prv_save_settings() 
{
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void prv_load_settings() 
{
    prv_default_settings();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

enum BackgroundMode SettingsGetBackgroundMode()
{
    return settings.bg_setting;
}

int SettingsGetConstantSunriseHour()
{
    return settings.sunrise_h;
}

int SettingsGetConstantSunriseMinute()
{
    return settings.sunrise_m;
}

int SettingsGetConstantSunsetHour()
{
    return settings.sunset_h;
}

int SettingsGetConstantSunsetMinute()
{
    return settings.sunset_m;
}

bool SettingsShowToD()
{
    return settings.show_tod;
}

enum AnimationMode SettingsGetAnimationMode()
{
    return settings.anim_mode;
}

void SettingsSetBackgroundMode(enum BackgroundMode bg)
{
    settings.bg_setting = bg;
}

void SettingsSetSunriseTimes(int hour, int min)
{
    settings.sunrise_h = hour;
    settings.sunrise_m = min;
}

void SettingsSetSunsetTimes(int hour, int min)
{
    settings.sunset_h = hour;
    settings.sunset_m = min;
}


void SettingsSetFahrenheit(bool fahrenheit)
{
    settings.use_fahrenheit = fahrenheit;
}

void SettingsSetShowTimeOfDay(bool show)
{
    settings.show_tod = show;
}

void SettingsSetAnimationMode(enum AnimationMode anim_mode)
{
    settings.anim_mode = anim_mode;
}

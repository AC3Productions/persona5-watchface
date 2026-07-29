#include <pebble.h>
#include "h/clay_settings.h"

#define SETTINGS_KEY 1

typedef struct ClaySettings 
{
    enum BackgroundMode bg_setting;
    // Only used with Constant Time bg setting
    int sunrise_h;
    int sunrise_m;
    int sunset_h;
    int sunset_m;

    bool use_fahrenheit; // false = Celsius, true = Fahrenheit
    bool show_tod;
    bool disable_anim;
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
    settings.disable_anim = false;
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

bool SettingsUsingFahrenheit()
{
    return settings.use_fahrenheit;
}

bool SettingsShowToD()
{
    return settings.show_tod;
}

bool SettingsGetAnimationDisabled()
{
    return settings.disable_anim;
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

void SettingsSetAnimationDisabled(bool disable_anim)
{
    settings.disable_anim = disable_anim;
}

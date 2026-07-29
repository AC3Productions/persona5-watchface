// clay_settings.h
#ifndef CLAY_SETTINGS_H
#define CLAY_SETTINGS_H

#include "h/time.h"

void prv_default_settings();
void prv_save_settings();
void prv_load_settings();

enum BackgroundMode SettingsGetBackgroundMode();
int SettingsGetConstantSunriseHour();
int SettingsGetConstantSunriseMinute();
int SettingsGetConstantSunsetHour();
int SettingsGetConstantSunsetMinute();

bool SettingsUsingFahrenheit();
bool SettingsShowToD();
bool SettingsGetAnimationDisabled();

void SettingsSetBackgroundMode(enum BackgroundMode bg);
void SettingsSetSunriseTimes(int hour, int min);
void SettingsSetSunsetTimes(int hour, int min);

void SettingsSetFahrenheit(bool fahrenheit);

void SettingsSetShowTimeOfDay(bool show);

void SettingsSetAnimationDisabled(bool disable_anim);

#endif // #ifndef CLAY_SETTINGS_H

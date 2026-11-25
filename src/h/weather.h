// weather.h
// This module contains enums and functions for interpreting
// weather data, but only manages the temperature resources
// (NOT weather conditions. Those are in the Calendar module).
#ifndef WEATHER_H
#define WEATHER_H
#include <pebble.h>

enum WeatherIcon 
{
    WeatherIconSun = 0,
    WeatherIconCloud,
    WeatherIconRain,
    WeatherIconSnow,

    WeatherIconUnknown,
};

int GetWeatherIcon(const char* weather_code);

void SetWeatherConnected(bool connected);

void TemperatureInitResources(GRect bounds);
void TemperatureUnloadResources();
void TemperatureAddLayers(Layer* parent_layer);

// Set position based on whether time of day is enabled
void TemperatureSetPosition(bool show_tod);
void SetTemperature(const char* temp);
void SetFahrenheit(bool use_f);

#endif // #ifndef WEATHER_H

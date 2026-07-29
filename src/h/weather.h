// weather.h
// This module contains enums and functions for interpreting
// weather data, but only manages the temperature resources
// (NOT weather conditions. Those are in the Calendar module).
#ifndef WEATHER_H
#define WEATHER_H
#include <pebble.h>

/*
WMO CODES - https://open-meteo.com/en/docs
------------+-------------
0	       | Clear sky
1, 2, 3	   | Mainly clear, partly cloudy, and overcast
45, 48	   | Fog and depositing rime fog
51, 53, 55 | Drizzle: Light, moderate, and dense intensity
56, 57	   | Freezing Drizzle: Light and dense intensity
61, 63, 65 | Rain: Slight, moderate and heavy intensity
66, 67	F  | reezing Rain: Light and heavy intensity
71, 73, 75 | Snow fall: Slight, moderate, and heavy intensity
77	       | Snow grains
80, 81, 82 | Rain showers: Slight, moderate, and violent
85, 86	   | Snow showers slight and heavy
95  	   | Thunderstorm: Slight or moderate
96, 99     | Thunderstorm with slight and heavy hail
*/

enum WeatherCode 
{
    WeatherCodeClear           = 1,
    WeatherCodeCloudy          = 3,
    WeatherCodeFog             = 48,
    WeatherCodeDrizzle         = 55,
    WeatherCodeFreezingDrizzle = 57,
    WeatherCodeRain            = 65,
    WeatherCodeFreezingRain    = 67,
    WeatherCodeSnow            = 75,
    WeatherCodeSnowGrains      = 77,
    WeatherCodeShowers         = 82,
    WeatherCodeSnowShowers     = 86,
    WeatherCodeThunderstorm    = 95,
};

enum WeatherIcon 
{
    WeatherIconSun = 0,
    WeatherIconCloud,
    WeatherIconRain,
    WeatherIconSnow,

    WeatherIconUnknown,
};

int GetWeatherIcon(enum WeatherCode weather_code);

void SetWeatherConnected(bool connected);

void TemperatureInitResources(GRect bounds);
void TemperatureUnloadResources();
void TemperatureAddLayers(Layer* parent_layer);

// Set position based on whether time of day is enabled
void TemperatureSetPosition(bool show_tod);
void SetTemperature(int fahrenheit);
void SetFahrenheit(bool use_f);

#endif // #ifndef WEATHER_H

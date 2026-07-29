// weather.c
#include <math.h>

#include "h/positions.h"
#include "h/weather.h"

static GFont s_temp_font;
static TextLayer* s_temp_layer = NULL;
static TextLayer* s_temp_drop_shadow_layer = NULL;

// Current temperature (fahrenheit)
static int curr_temp = 0;
static bool display_fahrenheit = true;
static bool unit_changed = false;
static bool weather_connected = false;

void SetWeatherConnected(bool connected)
{
    weather_connected = connected;
}

void TemperatureInitResources(GRect bounds)
{
    s_temp_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PUBLIC_SANS_BLACK_35));
    s_temp_layer = text_layer_create(
        GRect(TEMP_POS_X, TEMP_POS_Y, bounds.size.w, 120));
    text_layer_set_background_color(s_temp_layer, GColorClear);
    text_layer_set_text_color(s_temp_layer, GColorWhite);
    text_layer_set_font(s_temp_layer, s_temp_font);
    text_layer_set_text_alignment(s_temp_layer, GTextAlignmentCenter);

    s_temp_drop_shadow_layer = text_layer_create(
        GRect(TEMP_POS_X + TEMP_DROP_SHADOW_OFFSET, TEMP_POS_Y + TEMP_DROP_SHADOW_OFFSET, bounds.size.w, 120));
    text_layer_set_background_color(s_temp_drop_shadow_layer, GColorClear);
    text_layer_set_text_color(s_temp_drop_shadow_layer, GColorBlack);
    text_layer_set_font(s_temp_drop_shadow_layer, s_temp_font);
    text_layer_set_text_alignment(s_temp_drop_shadow_layer, GTextAlignmentCenter);

    text_layer_set_text(s_temp_layer, "--");
    text_layer_set_text(s_temp_drop_shadow_layer, "--");
}

void TemperatureUnloadResources()
{
    text_layer_destroy(s_temp_layer);
    text_layer_destroy(s_temp_drop_shadow_layer);
}

void TemperatureAddLayers(Layer* parent_layer)
{
    layer_add_child(parent_layer, text_layer_get_layer(s_temp_drop_shadow_layer));
    layer_add_child(parent_layer, text_layer_get_layer(s_temp_layer));
}

void TemperatureSetPosition(bool show_tod)
{
    int temp_x_pos = -1;
    if (show_tod)
    {
        temp_x_pos = TEMP_POS_X;
    }
    else
    {
        temp_x_pos = TEMP_NO_TOD_POS_X;    
    }

    // Move Temperature + drop shadow
    Layer* layer_temp = text_layer_get_layer(s_temp_layer);
    GRect temp_bounds = layer_get_frame(layer_temp);
    temp_bounds.origin.x = temp_x_pos;
    temp_bounds.origin.y = TEMP_POS_Y;
    layer_set_frame(layer_temp, temp_bounds);

    Layer* layer_ds_temp = text_layer_get_layer(s_temp_drop_shadow_layer);
    GRect temp_ds_bounds = layer_get_frame(layer_ds_temp);
    temp_ds_bounds.origin.x = temp_x_pos + TEMP_DROP_SHADOW_OFFSET;
    temp_ds_bounds.origin.y = TEMP_POS_Y + TEMP_DROP_SHADOW_OFFSET;
    layer_set_frame(layer_ds_temp, temp_ds_bounds);
}

void SetTemperature(int fahrenheit)
{
    if (!weather_connected)
    {
        return;
    }

    if (!unit_changed && fahrenheit == curr_temp)
    {
        return;
    }

    curr_temp = fahrenheit;
    static char temperature_buffer[8];
    if (display_fahrenheit)
    {
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°F", curr_temp);
    }
    else
    {
        int celcius = round(((float)curr_temp - 32.0f) * 5.0f / 9.0f);
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", celcius);
    }

    text_layer_set_text(s_temp_layer, temperature_buffer);
    text_layer_set_text(s_temp_drop_shadow_layer, temperature_buffer);
}

void SetFahrenheit(bool use_f)
{
    display_fahrenheit = use_f;
    unit_changed = true;
    SetTemperature(curr_temp);
    unit_changed = false;

}

int GetWeatherIcon(enum WeatherCode weather_code)
{
    if (weather_code <= WeatherCodeClear) return WeatherIconSun;

    if (weather_code <= WeatherCodeCloudy) return WeatherIconCloud;
    if (weather_code <= WeatherCodeFog) return WeatherIconCloud;

    if (weather_code <= WeatherCodeDrizzle) return WeatherIconRain;
    if (weather_code <= WeatherCodeFreezingDrizzle) return WeatherIconRain;
    if (weather_code <= WeatherCodeRain) return WeatherIconRain;
    if (weather_code <= WeatherCodeFreezingRain) return WeatherIconRain;
    
    if (weather_code <= WeatherCodeSnow) return WeatherIconSnow;
    if (weather_code <= WeatherCodeSnowGrains) return WeatherIconSnow;

    if (weather_code <= WeatherCodeShowers) return WeatherIconRain;
    if (weather_code <= WeatherCodeSnowShowers) return WeatherIconSnow;
    if (weather_code == WeatherCodeThunderstorm) return WeatherIconRain;
    if (weather_code <= WeatherCodeThunderstorm + 4) return WeatherIconRain;

    // we shouldn't ever get here
    return WeatherIconUnknown;
}
#include <pebble.h>
#include "base.h"
#include "weather.h"

static TextLayer *weather_layer;
static weather_t weather;

enum {
  WEATHER_MESSAGE_ICON_ID = 0,
  WEATHER_MESSAGE_TEMP_KELVIN = 1,
  WEATHER_MESSAGE_TEMP_CELSIUS = 2,
  WEATHER_MESSAGE_TEMP_FAHRENHEIT = 3
};

enum {
  WEATHER_ICON_CLEAR_DAY,
  WEATHER_ICON_CLEAR_NIGHT,
  WEATHER_ICON_CLOUDY,
  WEATHER_ICON_SHOWER_RAIN,
  WEATHER_ICON_RAIN,
  WEATHER_ICON_THUNDERSTORM,
  WEATHER_ICON_SNOW,
  WEATHER_ICON_MIST,
  WEATHER_ICON_ERROR
};

static char* weather_condition(int icon) {
  switch (icon) {
    case 1: return "%d° clear";
    case 2: return "%d° cloudy";
    case 4: return "%d° rainy";
    case 5: return "%d° stormy";
    case 6: return "%d° snowy";
    case 7: return "%d° misty";
    default: return "%d° ---";
  }
}

/** message from PebbleKit JS received */
static void on_received_handler(DictionaryIterator *received, void *context) {
  Tuple *icon_id = dict_find(received, WEATHER_MESSAGE_ICON_ID);
  Tuple *temp_kelvin = dict_find(received, WEATHER_MESSAGE_TEMP_KELVIN);
  Tuple *temp_celsius = dict_find(received, WEATHER_MESSAGE_TEMP_CELSIUS);
  Tuple *temp_fahrenheit = dict_find(received, WEATHER_MESSAGE_TEMP_FAHRENHEIT);

  // update weather
  if(icon_id && temp_kelvin && temp_celsius && temp_fahrenheit) {
    weather.icon_id = icon_id->value->int8;
    weather.temp_kelvin = temp_kelvin->value->int8;
    weather.temp_celsius = temp_celsius->value->int8;
    weather.temp_fahrenheit = temp_fahrenheit->value->int8;
  } else {
    weather.icon_id = WEATHER_ICON_ERROR;
    weather.temp_kelvin = 0;
    weather.temp_celsius = 0;
    weather.temp_fahrenheit = 0;
  }

  char *temp_string = "XXXXX";
  char *condition = weather_condition(weather.icon_id);

  snprintf(temp_string, sizeof("-137° stormy"), condition, weather.temp_fahrenheit);

  text_layer_set_text(weather_layer, temp_string);
}

void weather_init() {
  weather = (weather_t) {
    .icon_id = WEATHER_ICON_ERROR,
    .temp_kelvin = 0,
    .temp_celsius = 0,
    .temp_fahrenheit = 0
  };

  weather_layer = text_layer_create(GRect(2,-3,107,23));
  setup_text_layer(weather_layer, RESOURCE_ID_FONT_DROIDSANS_18);
  text_layer_set_text(weather_layer, "no weather");

  app_message_open(64, 16);
  app_message_register_inbox_received(on_received_handler);
}

void weather_deinit() {
  text_layer_destroy(weather_layer);
  app_message_deregister_callbacks();
}

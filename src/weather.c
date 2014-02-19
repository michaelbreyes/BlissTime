#include <pebble.h>
#include "base.h"
#include "weather.h"

static TextLayer *weather_layer;
static weather_t weather;

enum {
  WEATHER_MESSAGE_ICON = 0,
  WEATHER_MESSAGE_TEMP = 1
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received weather info");
  Tuple *icon = dict_find(received, WEATHER_MESSAGE_ICON);
  Tuple *temp = dict_find(received, WEATHER_MESSAGE_TEMP);

  // update weather
  if (icon && temp) {
    weather.icon = icon->value->int8;
    weather.temp = temp->value->int8;
  } else {
    weather.icon = 0;
    weather.temp = 0;
  }

  char *temp_string = "XXXXX";
  char *condition = weather_condition(weather.icon);

  snprintf(temp_string, sizeof("-137x stormy"), condition, weather.temp);
  APP_LOG(APP_LOG_LEVEL_DEBUG, temp_string);
  text_layer_set_text(weather_layer, temp_string);
}

void weather_init() {
  weather = (weather_t) {
    .icon = 0,
    .temp = 0
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

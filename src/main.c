#include <pebble.h>
#include "base.h"
#include "calendar.h"
#include "clock.h"
#include "weather.h"
#include "battery.h"

static Window *window;

enum {
  MESSAGE_TARGET = 0,
  MESSAGE_WEATHER_ICON = 1,
  MESSAGE_WEATHER_TEMP = 2,
  MESSAGE_TEMPERATURE = 3,
  MESSAGE_TIMEFORMAT = 4,
  MESSAGE_BUZZ_FREQ = 5,
  MESSAGE_BUZZ_OFFSET = 6,
  MESSAGE_BUZZ_FROM = 7,
  MESSAGE_BUZZ_TO = 8,
  MESSAGE_BUZZ_SUNDAY = 9,
  MESSAGE_BUZZ_MONDAY = 10,
  MESSAGE_BUZZ_TUESDAY = 11,
  MESSAGE_BUZZ_WEDNESDAY = 12,
  MESSAGE_BUZZ_THURSDAY = 13,
  MESSAGE_BUZZ_FRIDAY = 14,
  MESSAGE_BUZZ_SATURDAY = 15
};

static int dict_int(DictionaryIterator *dict, int key) {
  Tuple *val = dict_find(dict, key);
  return val->value->int8;
}

/** message from PebbleKit JS received */
static void on_received_handler(DictionaryIterator *received, void *context) {
  int target = dict_int(received, MESSAGE_TARGET);
  int icon = dict_int(received, MESSAGE_WEATHER_ICON);
  int degr = dict_int(received, MESSAGE_WEATHER_TEMP);
  receive_weather_data(icon, degr);

  if (target == 1) {
    // configuration has changed
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "configuration changed in main.c");
    int time_format = dict_int(received, MESSAGE_TIMEFORMAT);
    configure_clock(time_format);
    int buzz_freq = dict_int(received, MESSAGE_BUZZ_FREQ);
    int buzz_offset = dict_int(received, MESSAGE_BUZZ_OFFSET);
    int buzz_from = dict_int(received, MESSAGE_BUZZ_FROM);
    int buzz_to = dict_int(received, MESSAGE_BUZZ_TO);
    int buzz_sunday = dict_int(received, MESSAGE_BUZZ_SUNDAY);
    int buzz_monday = dict_int(received, MESSAGE_BUZZ_MONDAY);
    int buzz_tuesday = dict_int(received, MESSAGE_BUZZ_TUESDAY);
    int buzz_wednesday = dict_int(received, MESSAGE_BUZZ_WEDNESDAY);
    int buzz_thursday = dict_int(received, MESSAGE_BUZZ_THURSDAY);
    int buzz_friday = dict_int(received, MESSAGE_BUZZ_FRIDAY);
    int buzz_saturday = dict_int(received, MESSAGE_BUZZ_SATURDAY);
    configure_buzz(buzz_freq, buzz_offset, buzz_from, buzz_to, buzz_sunday, buzz_monday, buzz_tuesday, buzz_wednesday, buzz_thursday, buzz_friday, buzz_saturday);
  }
}

static void window_load(Window *window) {
  base_init();
  calendar_init();
  clock_init();
  weather_init();
  battery_init();
}

static void window_unload(Window *window) {
  battery_deinit();
  weather_deinit();
  clock_deinit();
  calendar_deinit();
  base_deinit();
}

static void init(void) {
  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true);

  app_message_open(256, 64);
  app_message_register_inbox_received(on_received_handler);
}

static void destroy(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  destroy();
}

#include <pebble.h>
#include "base.h"
#include "calendar.h"
#include "clock.h"
#include "weather.h"
#include "battery.h"

static Window *window;

enum {
  MESSAGE_TARGET = 0x0
};

/** message from PebbleKit JS received */
static void on_received_handler(DictionaryIterator *received, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received message from Js");
  Tuple *target = dict_find(received, MESSAGE_TARGET);
  if (target->value->int8 == 0) {
    receive_weather_data(received);
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

  app_message_open(64, 16);
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
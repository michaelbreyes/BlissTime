#include <pebble.h>
#include "base.h"
#include "calendar.h"
#include "clock.h"
#include "weather.h"
#include "battery.h"

static Window *window;

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
}

static void destroy(void) {
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	destroy();
}

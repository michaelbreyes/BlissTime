#include <pebble.h>
#include "base.h"

static TextLayer *batt_layer;

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "xxxx";

  if (charge_state.is_charging) {
    text_layer_set_text_color(batt_layer, GColorWhite);
    text_layer_set_background_color(batt_layer, GColorBlack);
  } else {
    text_layer_set_text_color(batt_layer, GColorBlack);
    text_layer_set_background_color(batt_layer, GColorWhite);
  }
  snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  text_layer_set_text(batt_layer, battery_text);
}

void battery_init() {
  batt_layer = text_layer_create(GRect(110,1,34,16));
  setup_text_layer(batt_layer, RESOURCE_ID_FONT_DROIDSANS_12);
  text_layer_set_text_alignment(batt_layer, GTextAlignmentRight);

  handle_battery(battery_state_service_peek());
  battery_state_service_subscribe(handle_battery);
}

void battery_deinit() {
  battery_state_service_unsubscribe();
  text_layer_destroy(batt_layer);
}
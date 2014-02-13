#include <pebble.h>

static Window *window;
static Layer *window_layer;
static BitmapLayer *line_layer;
static BitmapLayer *dark_lower_half_layer;

void setup_bitmap_layer(BitmapLayer* lyr) {
  bitmap_layer_set_background_color(lyr, GColorBlack);
  layer_add_child(window_layer, bitmap_layer_get_layer(lyr));
}

void setup_text_layer(TextLayer* lyr, uint32_t font_resource_id) {
  text_layer_set_font(lyr, fonts_load_custom_font(resource_get_handle(font_resource_id)));
  text_layer_set_background_color(lyr, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(lyr));
}

void setup_cal_layer(TextLayer* lyr) {
  text_layer_set_text_alignment(lyr, GTextAlignmentCenter);
  setup_text_layer(lyr, RESOURCE_ID_FONT_DROIDSANS_13);
}

void base_init() {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_layer = window_get_root_layer(window);

  line_layer = bitmap_layer_create(GRect(5,17,133,1));
  dark_lower_half_layer = bitmap_layer_create(GRect(0,53,144,115));
  bitmap_layer_set_background_color(line_layer, GColorBlack);
  bitmap_layer_set_background_color(dark_lower_half_layer, GColorBlack);
  layer_add_child(window_layer, bitmap_layer_get_layer(line_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(dark_lower_half_layer));
}

void base_deinit() {
  bitmap_layer_destroy(line_layer);
  bitmap_layer_destroy(dark_lower_half_layer);
  window_destroy(window);
}

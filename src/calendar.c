#include <pebble.h>
#include "base.h"

static BitmapLayer *active_day_layers[7];
static TextLayer *layers[21];
static int current_day_saved = -1;
static char days[32][3] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31" };
static int days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static void clear_active_bitmap_layers() {
  for (int i = 0; i < 7; i++) {
    bitmap_layer_set_background_color(active_day_layers[i], GColorClear);
  }
}

void show_weeks(int current_month, int current_day, int day_of_week) {
  if (current_day == current_day_saved) return;
  current_day_saved = current_day;

  clear_active_bitmap_layers();
  bitmap_layer_set_background_color(active_day_layers[day_of_week], GColorBlack);

  int start = current_day - day_of_week;
  if (start <= 0) {
    int prev_month = current_month-1;
    if (prev_month < 0) prev_month = 11;
    for (int i = start; i < (14+start); i++) {
      int day = i;
      if (i < 1) day = days_in_month[prev_month]+i;
      text_layer_set_text(layers[7+i-start], days[day]);
    }
  } else {
    for (int i = 0; i < 14; i++) {
      text_layer_set_text(layers[7+i], days[start+i]);
    }
  }

  text_layer_set_text_color(layers[7+day_of_week], GColorWhite);
}

void calendar_init() {
  // Active day bitmaps
  for (int i = 0; i < 7; i++) {
    active_day_layers[i] = bitmap_layer_create(GRect(2+(20*i),30,20,11));
    setup_bitmap_layer(active_day_layers[i]);
  }

  // Days
  int row = 0;
  for (int i = 0; i < 7; i++) {
    layers[i] = text_layer_create(GRect(2 + (20*i),16 + (11*row),20,16));
    setup_cal_layer(layers[i]);
  }

  // Week 1
  row = 1;
  for (int i = 0; i < 7; i++) {
    layers[i+7] = text_layer_create(GRect(2 + (20*i),16 + (11*row),20,16));
    setup_cal_layer(layers[i+7]);
  }

  // Week 2
  row = 2;
  for (int i = 0; i < 7; i++) {
    layers[i+14] = text_layer_create(GRect(2 + (20*i),16 + (11*row),20,16));
    setup_cal_layer(layers[i+14]);
  }

  text_layer_set_text(layers[0],"S");
  text_layer_set_text(layers[1],"M");
  text_layer_set_text(layers[2],"T");
  text_layer_set_text(layers[3],"W");
  text_layer_set_text(layers[4],"T");
  text_layer_set_text(layers[5],"F");
  text_layer_set_text(layers[6],"S");
}

void calendar_deinit() {
  for (int i = 0; i < 7; i++) {
    bitmap_layer_destroy(active_day_layers[i]);
  }
  for (int i = 0; i < 21; i++) {
    text_layer_destroy(layers[i]);
  }
}

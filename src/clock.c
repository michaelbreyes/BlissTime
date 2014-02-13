#include <pebble.h>
#include "base.h"
#include "calendar.h"

#define CLOCK_POS 52

static TextLayer *clock_layer1;
static TextLayer *clock_layer2;
static TextLayer *clock_layer3;
static char singles[17][8] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen" };

static char* get_double_unit_name(int num) {
  if (num < 10) return "o'";
  if (num < 30) return "twenty";
  if (num < 40) return "thirty";
  if (num < 50) return "forty";
  return "fifty";
}

static void show_time(struct tm *tick_time) {
  int hour = tick_time->tm_hour;
  int min = tick_time->tm_min;
  if (hour == 0) hour = 12;
  if (hour > 12) hour = hour - 12;
  char *min_str1 = "";
  char *min_str2 = "";
  text_layer_set_text(clock_layer1, singles[hour]);
  if (min == 0) {
    min_str1 = "o'";
    min_str2 = "clock";
  } else if (min < 10 || min > 19) {
    min_str1 = get_double_unit_name(min);
    min_str2 = singles[min%10];
  } else if (min <= 16 && min != 14) {
    min_str1 = singles[min];
  } else {
    min_str1 = singles[min%10];
    min_str2 = "teen";
  }
  text_layer_set_text(clock_layer2, min_str1);
  text_layer_set_text(clock_layer3, min_str2); 

  show_weeks(tick_time->tm_mon,tick_time->tm_mday,tick_time->tm_wday);
}

static void handle_clock_tick(struct tm *tick_time, TimeUnits units_changed) {
  show_time(tick_time);
}

void clock_init() {
  clock_layer1 = text_layer_create(GRect(5,CLOCK_POS,150,50));
  setup_text_layer(clock_layer1, RESOURCE_ID_FONT_DROIDSANS_38);
  clock_layer2 = text_layer_create(GRect(5,CLOCK_POS+33,150,50));
  setup_text_layer(clock_layer2, RESOURCE_ID_FONT_DROIDSANS_38);
  clock_layer3 = text_layer_create(GRect(5,CLOCK_POS+66,150,50));
  setup_text_layer(clock_layer3, RESOURCE_ID_FONT_DROIDSANS_38);
  text_layer_set_text_color(clock_layer1, GColorWhite);
  text_layer_set_text_color(clock_layer2, GColorWhite);
  text_layer_set_text_color(clock_layer3, GColorWhite);
  
  // subscription
  time_t now = time(NULL);
  struct tm *tick_time;
  tick_time = localtime(&now);
  handle_clock_tick(tick_time, MINUTE_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_clock_tick);
}

void clock_deinit() {
  tick_timer_service_unsubscribe();
  text_layer_destroy(clock_layer1);
  text_layer_destroy(clock_layer2);
  text_layer_destroy(clock_layer3);
}

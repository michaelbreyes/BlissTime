#pragma once

typedef struct {
  int icon_id;
  int temp_kelvin;
  int temp_celsius;
  int temp_fahrenheit;
} weather_t;

void weather_init(void);
void weather_deinit(void);

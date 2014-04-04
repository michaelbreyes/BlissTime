#pragma once

typedef struct {
  int icon;
  int temp;
} weather_t;

void receive_weather_data(int, int);
void weather_init(void);
void weather_deinit(void);
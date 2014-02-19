#pragma once

typedef struct {
  int icon;
  int temp;
} weather_t;

void weather_init(void);
void weather_deinit(void);
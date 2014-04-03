#pragma once

typedef struct {
  int icon;
  int temp;
} weather_t;

void receive_weather_data(DictionaryIterator*);
void weather_init(void);
void weather_deinit(void);
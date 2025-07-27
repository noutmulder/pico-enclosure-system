#ifndef FAN_H
#define FAN_H

#include "pico/stdlib.h"

#define FAN_PIN 15  // GPIO15 gebruiken
#define FAN_PIN2 14  // GPIO15 gebruiken

void fan_init();
void fan_on();
void fan_off();

#endif // FAN_H

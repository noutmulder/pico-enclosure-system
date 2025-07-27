#ifndef DS18B20_H
#define DS18B20_H

#include "pico/stdlib.h"

#define DS18B20_PIN 28


void ds18b20_init();
float ds18b20_read_temp();

#endif

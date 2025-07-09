#ifndef OLED_LIB_H
#define OLED_LIB_H

#include "pico/stdlib.h"

void oled_init();
void oled_clear(uint8_t oled);
void oled_draw_char(uint8_t x, uint8_t y, char c, uint8_t oled);
void oled_draw_text(uint8_t x, uint8_t y, const char* text, uint8_t oled);

#endif // OLED_LIB_H

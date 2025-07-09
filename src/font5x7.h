#ifndef FONT5X7_H
#define FONT5X7_H

#include <stdint.h>

#define FONT_WIDTH 5
#define FONT_HEIGHT 7  

// Font data voor ASCII-tekens 32 t/m 127
extern const uint8_t font5x7[96][FONT_WIDTH];

#endif // FONT5X7_H

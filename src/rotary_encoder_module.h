#ifndef ROTARY_ENCODER_MODULE_H
#define ROTARY_ENCODER_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pt.h"
#include "pico/stdlib.h"

// Pinnen voor de encoder
#define ENCODER_A_PIN 10
#define ENCODER_B_PIN 11 // Definieer pin B

#define DEBOUNCE_TIME_US 2000

// Functiedeclaraties
void rotary_encoder_init(void);
int32_t get_encoder_pos(void);
void reset_encoder_position(void); // Naam aangepast voor consistentie

#endif // ROTARY_ENCODER_MODULE_H

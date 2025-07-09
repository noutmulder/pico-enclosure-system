#ifndef ADC_LIB_H
#define ADC_LIB_H

#include "pico/stdlib.h"

// Definities van de gebruikte registers
#define ADC_BASE1 0x4004C000
#define ADC_CS   (ADC_BASE1 + 0x00)  // ADC CS reg
#define ADC_RESULT (ADC_BASE1 + 0x04) // ADC Result reg
#define ADC_FCS  (ADC_BASE1 + 0x08)  // ADC FIFO Control and Status reg
#define ADC_DIV (ADC_BASE1 + 0x10) // ADC Divider reg
#define ADC_FIFO (ADC_BASE1 + 0x0C) // ADC FIFO reg
#define ADC_INTE (ADC_BASE1 + 0x18) //ADC Interrupt Enable reg
// #define IO_BANK0_BASE 0x40014000 // Om GPIO27 op ADC te selecteren
#define GPIO27_CTRL (IO_BANK0_BASE + 0x0dc) // GPIO27_CTRL register

extern volatile uint8_t nieuw_data;    // Vlag voor nieuwe ADC-data
extern volatile uint16_t sensor_waarde; // Onbewerkte sensorwaarde
extern volatile uint16_t volume;       // Berekend volumepercentage

// Functies
void adc_fifo_init_via_reg();          // Init van interrupts en FIFO
void __not_in_flash_func(adc_interrupt_lezen)(); // Leest de waarde van interrupt
void adc_enable_interrupt();          // Interrupt initialiseren

#endif

#include "adc_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "hardware/irq.h"

// definitie
volatile uint8_t nieuw_data = 0;
volatile uint16_t sensor_waarde = 0;
volatile uint16_t volume = 0;
volatile int oldVolume = 0;

// ADC en FIFO initialisatie via registers
void adc_fifo_init_via_reg() {
    *(volatile uint32_t *)ADC_DIV = 0x00; // Max snelheid, voor de leesbaarheid ingesteld
    *(volatile uint32_t *)ADC_CS &= ~((0x7) << 12); // Wis de AINSEL bits
    *(volatile uint32_t *)ADC_CS |= (1 << 12); // AINSEL voor ADC1 (GPIO27)
    *(volatile uint32_t *)ADC_CS |= (1 << 0); // Zet EN bit aan in CS reg

    // FIFO en interrupts
    *(volatile uint32_t *)ADC_FCS = (1 << 0); // Zet FIFO EN (Bit 0)
    *(volatile uint32_t *)ADC_FCS |= (1 << 3); // DREQ Enable
    *(volatile uint32_t *)ADC_FCS |= (1 << 24); // TRESH op 1 
    *(volatile uint32_t *)ADC_INTE |= (1 << 0); // Enable interrupts
    *(volatile uint32_t *)ADC_CS |= (1 << 2); // START_ONCE enable
}

uint16_t adc_fifo_get() {
    return *(volatile uint32_t *)ADC_FIFO & 0xFFF; // Return de 12-bit result
}

// Interrupt handler
void __not_in_flash_func(adc_interrupt_lezen)() {
    if (!(*(volatile uint32_t *)ADC_FCS & (1 << 8))) { // Als FIFO niet leeg is
        sensor_waarde = adc_fifo_get() & 0xFFF; // Keep full 12-bit value
        volume = 100 - ((sensor_waarde * 100) / 4095); // Fix scaling to ensure 0-100%
       // Zorg dat volume altijd tussen 0 en 100 blijft
        if (volume > 100) {
            volume = 100;
        } else if (volume < 0) {
            volume = 0;
        }
        if (volume == 1){
            volume = 0;
        }

        if (abs(volume - oldVolume) > 2) {
            nieuw_data = 1;
            oldVolume = volume;
        }
        *(volatile uint32_t *)ADC_CS |= (1 << 2); // Herstart ADC
    }
}


void adc_enable_interrupt() {
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_interrupt_lezen);
    irq_set_priority(ADC_IRQ_FIFO, 4); // Lagere prioriteit
    irq_set_enabled(ADC_IRQ_FIFO, true);
}

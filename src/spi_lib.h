#ifndef SPI_LIB_H
#define SPI_LIB_H

#include "pico/stdlib.h"

void mijn_spi_init();
void mijn_spi_send(uint8_t data, uint8_t oled);
void spi_send_command(uint8_t cmd, uint8_t oled);
void spi_send_data(uint8_t data, uint8_t oled);

#endif // SPI_LIB_H

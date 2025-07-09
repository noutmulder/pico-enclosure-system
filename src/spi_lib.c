#include "pico/stdlib.h"

// Dit zijn de GPIO's zoals de voorversterker gebruikt
#define SPI_SCLK  18
#define SPI_MOSI  19
#define SPI_DC    16  
#define SPI_CS1   17  
#define SPI_CS2   6
#define SPI_RST   22

void mijn_spi_init() {
    // GPIO-pinnen instellen als uitgang
    gpio_init(SPI_SCLK);
    gpio_set_dir(SPI_SCLK, GPIO_OUT);
    
    gpio_init(SPI_MOSI);
    gpio_set_dir(SPI_MOSI, GPIO_OUT);

    gpio_init(SPI_CS1);
    gpio_set_dir(SPI_CS1, GPIO_OUT);
    gpio_put(SPI_CS1, 1);  // CS hoog in rust, zoals in datasheet vermeld

    gpio_init(SPI_CS2);
    gpio_set_dir(SPI_CS2, GPIO_OUT);
    gpio_put(SPI_CS2, 1);  // CS hoog in rust, zoals in datasheet vermeld

    gpio_init(SPI_DC);
    gpio_set_dir(SPI_DC, GPIO_OUT);

    // Init reset-pin 
    gpio_init(SPI_RST);
    gpio_set_dir(SPI_RST, GPIO_OUT);
    gpio_put(SPI_RST, 0);  
    sleep_ms(10);  
    gpio_put(SPI_RST, 1);  
    sleep_ms(10);  
}

// Functie om 1 byte via SPI te sturen (totaal 8 bits)
void mijn_spi_send(uint8_t data, uint8_t oled) {
    // CS laag om transmissie te starten
    if (oled == 1) {
        gpio_put(SPI_CS1, 0); // CS laag voor OLED 1
    } else {
        gpio_put(SPI_CS2, 0); // CS laag voor OLED 2
    } 

    for (int i = 7; i >= 0; i--) {
        gpio_put(SPI_MOSI, (data >> i) & 1); // Stuur bit voor bit de data
        gpio_put(SPI_SCLK, 1);  // Klok hoog --> leest bij rising edge
        sleep_us(1); 
        gpio_put(SPI_SCLK, 0);  // Klok weer laag
    }

    if (oled == 1) {
        gpio_put(SPI_CS1, 1); // CS hoog na transmissie voor OLED 1
    } else {
        gpio_put(SPI_CS2, 1); // CS hoog na transmissie voor OLED 2
    }
}

// Functie om commando te sturen (DC = 0)
void spi_send_command(uint8_t command, uint8_t oled) {
    gpio_put(SPI_DC, 0);  // Command mode
    mijn_spi_send(command, oled);
}

// Functie om data te versturen (DC = 1)
void spi_send_data(uint8_t data, uint8_t oled) {
    gpio_put(SPI_DC, 1);  // Data mode
    mijn_spi_send(data, oled);
}

#include "oled_lib.h"
#include "spi_lib.h"
#include "font5x7.h" // Font-array

//Init commands van https://github.com/jmwilson/Adafruit_SSD1306_MicroBlaze/blob/master/Adafruit_SSD1306.cpp
void oled_init() {
    uint8_t init_cmds[] = {
        0xAE,   // SSD1306_DISPLAYOFF
        0xD5,   // SSD1306_SETDISPLAYCLOCKDIV
        0x80,   // The suggested ratio 0x80
        0xA8,   // SSD1306_SETMULTIPLEX
        0x1F,   // LCD Height - 1 (32 pixels for 128x32 screen)
        0xD3,   // SSD1306_SETDISPLAYOFFSET
        0x00,   // No offset
        0x00,   // SSD1306_SETSTARTLINE
        0x8D,   // SSD1306_CHARGEPUMP
        0x14,   // 0x14 for external VCC or 0x10 for internal VCC
        0x20,   // SSD1306_MEMORYMODE
        0x00,   // Horizontal addressing mode
        0xA1,   // SSD1306_SEGREMAP (Reverse the segment order)
        0xC8,   // SSD1306_COMSCANDEC (Reverse the COM scan direction)
        0xDA,   // SSD1306_SETCOMPINS
        0x02,   // Common pins configuration for 128x32
        0x81,   // SSD1306_SETCONTRAST
        0x8F,   // Contrast value (0x8F for a good default)
        0xD9,   // SSD1306_SETPRECHARGE
        0xF1,   // Pre-charge period (for internal VCC)
        0xDB,   // SSD1306_SETVCOMDETECT
        0x40,   // VCOMH deselect level
        0xA4,   // SSD1306_DISPLAYALLON_RESUME
        0xA6,   // SSD1306_NORMALDISPLAY
        0x2E,   // SSD1306_DEACTIVATE_SCROLL
        0xAF    // SSD1306_DISPLAYON
    };

    // Alle commandos sturen via SPI
    for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
        spi_send_command(init_cmds[i], 1); // OLED 1 initialiseren
        spi_send_command(init_cmds[i], 2); // OLED 2 initialiseren
    }
}

// Oled clear functie met oled selectie
void oled_clear(uint8_t oled) {
    for (uint8_t rij = 0; rij < 4; rij++) {
        spi_send_command(0xB0 | rij, oled); // Juiste rij selecteren
        spi_send_command(0x00, oled); // Laagste 4 bits op 0 zetten
        spi_send_command(0x10, oled); // Hoogste 4 bits op 0 zetten
        // Kolommen wissen
        for (uint8_t kol = 0; kol < 128; kol++) {
            spi_send_data(0x00, oled);
        }
    }
}

// Oled character functie met oled selectie
void oled_draw_char(uint8_t x, uint8_t y, char c, uint8_t oled) {
    if (c < 32 || c > 127) return; // Als buiten ASCII-bereik, verlaat functie

    spi_send_command(0xB0 | y, oled); // Juiste rij geselecteerd
    // Kolompositie instellingen
    spi_send_command((x & 0x0F), oled);  // Laagste 4 bits
    spi_send_command(0x10 | (x >> 4), oled); // Hoogste 4 bits

    for (uint8_t i = 0; i < 5; i++) {
        spi_send_data(font5x7[c - 32][i], oled);
    }
}

// Oled text functie met oled selectie
void oled_draw_text(uint8_t x, uint8_t y, const char* text, uint8_t oled) {
    while (*text) {
        oled_draw_char(x, y, *text++, oled);
        x += 6; // 6 pixels per teken voor font = 5
        if (x >= 128) { // Als het groter is dan rand
            x = 0;
            if (y < 3) y++; // Blijf binnen de 4 beschikbare pagina's/rijen
        }
    }
}
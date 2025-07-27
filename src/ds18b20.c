#include "ds18b20.h"

#define DS18B20_PIN 12

static void ds18b20_write_bit(bool bit) {
    gpio_put(DS18B20_PIN, 0);
    sleep_us(bit ? 2 : 60);
    gpio_put(DS18B20_PIN, 1);
    sleep_us(bit ? 60 : 2);
}

static bool ds18b20_read_bit() {
    bool bit;
    gpio_put(DS18B20_PIN, 0);
    sleep_us(2);
    gpio_put(DS18B20_PIN, 1);
    sleep_us(10);
    bit = gpio_get(DS18B20_PIN);
    sleep_us(50);
    return bit;
}

static void ds18b20_write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

static uint8_t ds18b20_read_byte() {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        if (ds18b20_read_bit()) {
            value |= (1 << i);
        }
    }
    return value;
}

static bool ds18b20_reset() {
    gpio_put(DS18B20_PIN, 0);
    sleep_us(480);
    gpio_put(DS18B20_PIN, 1);
    sleep_us(70);
    bool presence = !gpio_get(DS18B20_PIN);
    sleep_us(410);
    return presence;
}

void ds18b20_init() {
    gpio_init(DS18B20_PIN);
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 1);
}

float ds18b20_read_temp() {
    if (!ds18b20_reset()) {
        return -127.0f; // Geen sensor gedetecteerd
    }

    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Start temperature conversion
    sleep_ms(750);            // Wacht tot conversie klaar is

    if (!ds18b20_reset()) {
        return -127.0f;
    }

    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0xBE); // Lees scratchpad

    uint8_t lsb = ds18b20_read_byte();
    uint8_t msb = ds18b20_read_byte();

    int16_t raw = (msb << 8) | lsb;
    return raw / 16.0f;
}

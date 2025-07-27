#include "fan.h"

void fan_init() {
    gpio_init(FAN_PIN);
    gpio_init(FAN_PIN2);
    gpio_set_dir(FAN_PIN, GPIO_OUT);
    gpio_set_dir(FAN_PIN2, GPIO_OUT);
    gpio_put(FAN_PIN, 0); // standaard uit
    gpio_put(FAN_PIN2, 0); // standaard uit
}

void fan_on() {
    gpio_put(FAN_PIN, 1);
    gpio_put(FAN_PIN2, 1);
}

void fan_off() {
    gpio_put(FAN_PIN, 0);
    gpio_put(FAN_PIN2, 0);
}

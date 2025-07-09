#include "pico/stdlib.h"
#include "rotary_encoder_module.h"
#include "pt.h"
#include "hardware/uart.h"
#include "spi_lib.h"
#include "oled_lib.h"
#include "font5x7.h"


#define NUM_LEDS 5 // Number of LEDs

// Global arrays for rotary encoder and combined LED colors
uint32_t led_colors_rotary[NUM_LEDS];
static uint32_t combined_led_colors[NUM_LEDS];

// Globals for protothreads
static struct pt pt1RotEnc, pt2Printf;
volatile int32_t waardeRotEnc = 0; // Current encoder value
volatile uint8_t nWaardeBeschikbaar_print = 0;
volatile uint8_t nWaardeBeschikbaar_led = 0;

// Function declarations
static int protothread1(struct pt *pt);
static int protothread2(struct pt *pt);
static int protothread4(struct pt *pt);
void init_pmod();



int main() {
    stdio_init_all();
//    set_sys_clock_khz(125000, true);



    // Initialize rotary encoder, LED module, and ADC
    rotary_encoder_init();    
    init_pmod();

    oled_clear(1);
    oled_draw_text(0, 2, "OLED 1 Active", 1);

    // Initialize protothreads
    PT_INIT(&pt1RotEnc);
    PT_INIT(&pt2Printf);
    // PT_INIT(&pt4ADC);

    printf("Program started\n");


    while (1) {
        protothread1(&pt1RotEnc);
        sleep_ms(100);
    }

    return 0;
}

// Protothread 1: Update encoder value and set flags
static int protothread1(struct pt *pt) {
    PT_BEGIN(pt);
    while (1) {
        PT_WAIT_UNTIL(pt, get_encoder_pos() != waardeRotEnc);
        waardeRotEnc = get_encoder_pos();
        nWaardeBeschikbaar_print = 1;
        nWaardeBeschikbaar_led = 1;
        
        printf("PT1: Encoder value updated: %d\n", waardeRotEnc);

        char oled_text[20];
        snprintf(oled_text, sizeof(oled_text), "Pos: %ld", waardeRotEnc);
        oled_clear(1);
        oled_draw_text(0, 2, oled_text, 1);

        PT_YIELD(pt);
    }
    PT_END(pt);
}

void init_pmod() {
    stdio_init_all();
    mijn_spi_init();
    oled_init();
    oled_clear(1);
}

// Protothread 2: Print encoder value
static int protothread2(struct pt *pt) {
    PT_BEGIN(pt);
    while (1) {
        PT_WAIT_UNTIL(pt, nWaardeBeschikbaar_print);
        printf("PT2: Encoder Position: %d\n", waardeRotEnc);
        nWaardeBeschikbaar_print = 0;
        PT_YIELD(pt);
    }
    PT_END(pt);
}

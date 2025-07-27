#include "pico/stdlib.h"
#include "rotary_encoder_module.h"
#include "pt.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "spi_lib.h"
#include "oled_lib.h"
#include "font5x7.h"
#include "fan.h"
#include "ds18b20.h"
#include "options.h"



// Globals for protothreads
static struct pt pt1RotEnc, pt2Printf, pt3TempCheck;
volatile int32_t waardeRotEnc = 0; // Current encoder value
volatile uint8_t nWaardeBeschikbaar_print = 0;
volatile uint8_t nWaardeBeschikbaar_led = 0;

// Function declarations
static int protothread1(struct pt *pt);
static int protothread2(struct pt *pt);
static int protothread3(struct pt *pt);
static int protothread4(struct pt *pt);

void init_pmod();

int main()
{
    stdio_init_all();
    //    set_sys_clock_khz(125000, true);

    fan_init();

    // Initialize rotary encoder, LED module, and ADC
    rotary_encoder_init();
    init_pmod();

    oled_clear(1);
    oled_draw_text(0, 2, "OLED 1 Active", 1);

    ds18b20_init();
    printf("DS18B20 Test Start\n");

    // while (1)
    // {
    //     float temp = ds18b20_read_temp();

    //     if (temp > -100.0f)
    //     {
    //         printf("Gemeten temperatuur: %.2f C\n", temp);
    //     }
    //     else
    //     {
    //         printf("Fout bij lezen van sensor\n");
    //     }

    //     sleep_ms(1000); // wacht 1 seconde
    // }

    // Initialize protothreads
    PT_INIT(&pt1RotEnc);
    PT_INIT(&pt2Printf);
    PT_INIT(&pt3TempCheck);



    printf("Program started\n");

    while (1)
{
    protothread1(&pt1RotEnc);
    protothread3(&pt3TempCheck);
    sleep_ms(100);
}


    return 0;
}

// Protothread 1: Update encoder value and set flags
static int protothread1(struct pt *pt)
{
    PT_BEGIN(pt);
    while (1)
    {
        PT_WAIT_UNTIL(pt, get_encoder_pos() != waardeRotEnc);
        waardeRotEnc = get_encoder_pos();
        nWaardeBeschikbaar_print = 1;
        nWaardeBeschikbaar_led = 1;

        printf("PT1: Encoder value updated: %d\n", waardeRotEnc);
        if (waardeRotEnc % 2)
        {
            fan_on();
            printf("fan turning on...");
        }
        else
        {
            printf("fan turning off...");
            fan_off();
        }

        char oled_text[20];
        snprintf(oled_text, sizeof(oled_text), "Pos: %ld", waardeRotEnc);
        oled_clear(1);
        oled_draw_text(0, 2, oled_text, 1);

        PT_YIELD(pt);
    }
    PT_END(pt);
}

// Protothread 2: Print encoder value
static int protothread2(struct pt *pt)
{
    PT_BEGIN(pt);
    while (1)
    {
        PT_WAIT_UNTIL(pt, nWaardeBeschikbaar_print);
        printf("PT2: Encoder Position: %d\n", waardeRotEnc);
        nWaardeBeschikbaar_print = 0;
        PT_YIELD(pt);
    }
    PT_END(pt);
}


static int protothread3(struct pt *pt)
{
    static uint64_t last_check_time = 0;
    static uint64_t fan_off_deadline = 0;
    static bool fan_is_on = false;

    PT_BEGIN(pt);

    while (1)
    {
        uint64_t now = to_ms_since_boot(get_absolute_time());

        // Check elke 2 seconden
        if (now - last_check_time >= 2000) {
            last_check_time = now;

            float temp = ds18b20_read_temp();

            if (temp > -100.0f) {
                printf("Temperatuur: %.2f °C\n", temp);

                if (temp >= FAN_TEMP_THRESHOLD) {
                    if (!fan_is_on) {
                        fan_on();
                        fan_is_on = true;
                        printf("Fan AAN\n");
                    }
                    // Reset de countdown zolang temp boven threshold is
                    fan_off_deadline = now + FAN_OFF_DELAY_MS;
                }
                else if (fan_is_on && now >= fan_off_deadline) {
                    fan_off();
                    fan_is_on = false;
                    printf("Fan UIT (na delay)\n");
                }
            } else {
                printf("Temperatuur fout of sensor niet gevonden.\n");
            }
        }

        PT_YIELD(pt);
    }

    PT_END(pt);
}


void init_pmod()
{
    stdio_init_all();
    mijn_spi_init();
    oled_init();
    oled_clear(1);
}


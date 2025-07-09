#include "rotary_encoder_module.h"

// Interne variabelen voor het bijhouden van de encoder status
volatile int32_t encoder_position = 0; // Huidige positie van de encoder
volatile uint8_t last_state_A = 0; // Vorige staat van het A-kanaal
volatile uint32_t last_interrupt_time = 0; // Tijd van de laatste interrupt

/* ISR (Interrupt Service Routine) voor het A-kanaal.
Dit is een functie die wordt uitgevoerd wanneer een interrupt optreedt. 
In dit geval gebeurt dit wanneer de toestand van het A-kanaal verandert (op- of neergaande flank).
*/
void encoder_isr(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    if (current_time - last_interrupt_time < DEBOUNCE_TIME_US) {
        // Negeert de input als de debounce tijd nog niet voorbij is, om ongewensde input te voorkomen
        return;
    }

    last_interrupt_time = current_time;

    // Lees de huidige staat van de A- en B-kanalen. gpio_get is een functie van de Pico SDK.
    uint8_t state_A = gpio_get(ENCODER_A_PIN);
    uint8_t state_B = gpio_get(ENCODER_B_PIN);

    // Bepaal de richting van de rotatie, en voeg een waarde toe of trek een waarde af.
    if (state_A != last_state_A) {
        if (state_A == state_B) {
            encoder_position++;
        } else {
            encoder_position--;
        }
    }

    // Update de laatste staat van het A-kanaal.
    last_state_A = state_A;
}

/* Stelt interrupts in op de A-pin. 
Deze interrupt wordt geactiveerd bij zowel een opgaande als neergaande flank (GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE).
De functie encoder_isr wordt aangeroepen wanneer de interrupt optreedt. */
void rotary_encoder_init(void) {

    // Initialiseer de GPIO-pinnen voor de A- en B-kanalen
    gpio_init(ENCODER_A_PIN);
    gpio_init(ENCODER_B_PIN);

    // Stel de GPIO-pinnen in als input
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_dir(ENCODER_B_PIN, GPIO_IN);

    // Activeer pull-up weerstanden voor beide kanalen
    gpio_pull_up(ENCODER_A_PIN);
    gpio_pull_up(ENCODER_B_PIN);

    // Configureer interrupts voor het A-kanaal
    gpio_set_irq_enabled_with_callback(ENCODER_A_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 1, &encoder_isr);
}

int32_t get_encoder_pos(void) {
    return encoder_position;
}

// Reset de positie van de encoder naar nul
void reset_encoder_position(void) {
    encoder_position = 0;
}

cmake_minimum_required(VERSION 3.13)

# Stel de locatie van de Pico SDK in
set(PICO_SDK_PATH "/home/nout/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Stel het project in
project(pico_enclosure_system C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initializeer de Pico SDK
pico_sdk_init()

# Voeg bronbestanden toe aan de build
add_executable(pico_enclosure_system
    src/main.c
    src/rotary_encoder_module.c
    src/adc_lib.c
    src/font5x7.c
    src/spi_lib.c
    src/oled_lib.c
    # src/fan_control.c       # Als je deze maakt
)

# Stel UART1 in als standaard voor printf
target_compile_definitions(pico_enclosure_system PUBLIC
    PICO_DEFAULT_UART=1
    PICO_DEFAULT_UART_TX_PIN=4
    PICO_DEFAULT_UART_RX_PIN=5
)

# Link met de noodzakelijke bibliotheken
target_link_libraries(pico_enclosure_system
    pico_stdlib
    hardware_clocks
    hardware_irq
    hardware_adc
    hardware_uart
    hardware_pio
    hardware_gpio 
    hardware_spi 
    hardware_pwm              # als je PWM voor fan gebruikt
)

# Schakel USB-uitvoer in en UART-uitvoer uit
pico_enable_stdio_usb(pico_enclosure_system 1)
pico_enable_stdio_uart(pico_enclosure_system 0)

# Genereer .uf2, .bin, .elf bestanden
pico_add_extra_outputs(pico_enclosure_system)

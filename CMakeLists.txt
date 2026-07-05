#
set(PICO_BOARD waveshare_core2350b)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(blink_pio C CXX ASM)

pico_sdk_init()

add_executable(blink_pio blink.c)

pico_set_program_name(blink_pio "blink_pio")
pico_set_program_version(blink_pio "0.1")

pico_generate_pio_header(blink_pio ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

pico_enable_stdio_uart(blink_pio 0)
pico_enable_stdio_usb(blink_pio  1)

target_link_libraries(blink_pio pico_stdlib hardware_pio)

pico_add_extra_outputs(blink_pio)

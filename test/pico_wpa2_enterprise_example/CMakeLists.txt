cmake_minimum_required(VERSION 3.13)

# Pull in SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_wpa2_enterprise_example)

# Initialize the SDK
pico_sdk_init()

add_executable(wpa2_enterprise_example
    wpa2_enterprise_example.c
)

target_link_libraries(wpa2_enterprise_example 
    pico_stdlib
    pico_cyw43_arch_lwip_poll  # or pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
)

# Enable USB output and disable UART output
pico_enable_stdio_usb(wpa2_enterprise_example 1)
pico_enable_stdio_uart(wpa2_enterprise_example 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(wpa2_enterprise_example)
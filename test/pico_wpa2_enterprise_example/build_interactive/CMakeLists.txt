cmake_minimum_required(VERSION 3.13)

# Pull in SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_wpa2_enterprise_interactive)

# Initialize the SDK
pico_sdk_init()

add_executable(wpa2_enterprise_interactive
    wpa2_enterprise_interactive.c
)

target_link_libraries(wpa2_enterprise_interactive 
    pico_stdlib
    pico_cyw43_arch_lwip_poll
)

# Add include directory for lwIP config
target_include_directories(wpa2_enterprise_interactive PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable USB output and disable UART output
pico_enable_stdio_usb(wpa2_enterprise_interactive 1)
pico_enable_stdio_uart(wpa2_enterprise_interactive 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(wpa2_enterprise_interactive)
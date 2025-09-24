cmake_minimum_required(VERSION 3.13)

# Pull in SDK (set PICO_SDK_PATH env variable)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(simple_wifi_example)

# Initialize the SDK  
pico_sdk_init()

add_executable(simple_wifi_example
    simple_wifi.c
)

# Link libraries - use basic CYW43 arch polling
target_link_libraries(simple_wifi_example 
    pico_stdlib
    pico_cyw43_arch_lwip_poll
)

# Add include directory for lwIP config
target_include_directories(simple_wifi_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable USB output and disable UART output
pico_enable_stdio_usb(simple_wifi_example 1)
pico_enable_stdio_uart(simple_wifi_example 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(simple_wifi_example)
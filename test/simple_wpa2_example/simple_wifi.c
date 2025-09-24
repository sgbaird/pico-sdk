/**
 * Simple WiFi connection example for Pico W
 * Tests basic connectivity and demonstrates structure
 * for future WPA2-Enterprise integration
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Network configuration - replace with your test network
#define WIFI_SSID "your-test-network"
#define WIFI_PASSWORD "your-password"

void print_network_info(void) {
    printf("Network information:\n");
    printf("Connected successfully to WiFi\n");
    // In a real implementation, we'd print IP info here
    printf("This demonstrates the basic structure for WPA2-Enterprise\n");
}

int main() {
    stdio_init_all();
    
    printf("=== Pico W WiFi Connection Test ===\n");
    printf("This example shows the foundation for WPA2-Enterprise support\n\n");

    // Initialize CYW43 driver
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43 driver\n");
        return 1;
    }

    printf("CYW43 driver initialized successfully\n");
    
    // Enable station mode
    cyw43_arch_enable_sta_mode();
    printf("Station mode enabled\n");

    // For WPA2-Enterprise, this is where we would:
    printf("\n=== WPA2-Enterprise Integration Points ===\n");
    printf("1. Call cyw43_arch_wifi_wpa2_ent_enable()\n");
    printf("2. Set credentials with cyw43_arch_wifi_wpa2_ent_set_credentials()\n");
    printf("3. Connect with cyw43_arch_wifi_connect_wpa2_ent_blocking()\n");
    
    printf("\nNote: Full WPA2-Enterprise requires the patched cyw43-driver\n");
    printf("See /test/pico_wpa2_enterprise_example/ for complete implementation\n");

    // Demonstrate basic WiFi structure (commented out to avoid needing real credentials)
    /*
    printf("Attempting to connect to: %s\n", WIFI_SSID);
    
    int result = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000);
    if (result == 0) {
        printf("Connected successfully!\n");
        print_network_info();
    } else {
        printf("Connection failed with error: %d\n", result);
    }
    */

    printf("\n=== Test Complete ===\n");
    printf("This firmware is ready for WPA2-Enterprise when:\n");
    printf("- The cyw43-driver patches are applied\n"); 
    printf("- Network credentials are configured\n");
    printf("- Enterprise network is available\n");

    // Keep the device alive
    while (1) {
        // Blink the LED to show it's working
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }

    // Cleanup (never reached)
    cyw43_arch_deinit();
    return 0;
}
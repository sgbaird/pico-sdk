/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "lwip/netif.h"

// WPA2-Enterprise network configuration
#define ENTERPRISE_SSID         "eduroam"          // Your enterprise network SSID
#define EAP_IDENTITY           "user@example.com"  // Your EAP identity
#define EAP_USERNAME           "user@example.com"  // Your username 
#define EAP_PASSWORD           "yourpassword"      // Your password

// Example CA certificate (you would replace this with your actual CA cert in PEM format)
static const char* ca_cert_pem = 
    "-----BEGIN CERTIFICATE-----\n"
    "MIICertificateDataGoesHere...\n"
    "-----END CERTIFICATE-----\n";

void print_network_info(void) {
    struct netif *netif = netif_default;
    if (netif) {
        printf("IP address: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
        printf("Subnet mask: %s\n", ip4addr_ntoa(netif_ip4_netmask(netif)));
        printf("Gateway: %s\n", ip4addr_ntoa(netif_ip4_gw(netif)));
    }
}

int main() {
    stdio_init_all();

    // Initialize CYW43 driver
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43 driver\n");
        return 1;
    }

    printf("Pico W WPA2-Enterprise Example\n");
    printf("==============================\n");

    // Enable station mode
    cyw43_arch_enable_sta_mode();

    // Enable WPA2-Enterprise authentication
    printf("Enabling WPA2-Enterprise...\n");
    if (cyw43_arch_wifi_wpa2_ent_enable() != PICO_OK) {
        printf("Failed to enable WPA2-Enterprise\n");
        return 1;
    }

    // Configure WPA2-Enterprise credentials
    printf("Setting WPA2-Enterprise credentials...\n");
    cyw43_wpa2_ent_credentials_t credentials = {0};
    
    // Set identity, username, and password
    credentials.identity = (uint8_t*)EAP_IDENTITY;
    credentials.identity_len = strlen(EAP_IDENTITY);
    credentials.username = (uint8_t*)EAP_USERNAME;
    credentials.username_len = strlen(EAP_USERNAME);
    credentials.password = (uint8_t*)EAP_PASSWORD;
    credentials.password_len = strlen(EAP_PASSWORD);
    
    // Set CA certificate (required for server validation)
    credentials.ca_cert = (uint8_t*)ca_cert_pem;
    credentials.ca_cert_len = strlen(ca_cert_pem);
    
    // Use PEAP-MSCHAPv2 method (most common for eduroam)
    credentials.eap_method = CYW43_EAP_METHOD_PEAP;
    
    // Optional: disable time checking if you don't have RTC
    credentials.disable_time_check = true;

    if (cyw43_arch_wifi_wpa2_ent_set_credentials(&credentials) != PICO_OK) {
        printf("Failed to set WPA2-Enterprise credentials\n");
        return 1;
    }

    // Connect to the WPA2-Enterprise network
    printf("Connecting to WPA2-Enterprise network: %s\n", ENTERPRISE_SSID);
    printf("This may take 30-60 seconds for enterprise authentication...\n");
    
    int result = cyw43_arch_wifi_connect_wpa2_ent_timeout_ms(ENTERPRISE_SSID, 60000);
    if (result == PICO_OK) {
        printf("Connected successfully!\n");
        print_network_info();
    } else {
        printf("Connection failed with error: %d\n", result);
        switch (result) {
            case PICO_ERROR_BADAUTH:
                printf("Authentication failed - check credentials\n");
                break;
            case PICO_ERROR_NO_DATA:
                printf("Network not found - check SSID\n");
                break;
            case PICO_ERROR_TIMEOUT:
                printf("Connection timed out\n");
                break;
            case PICO_ERROR_CONNECT_FAILED:
                printf("Connection failed\n");
                break;
            default:
                printf("Unknown error\n");
                break;
        }
        return 1;
    }

    // Simple web server to demonstrate connectivity
    printf("Starting web server on port 80...\n");
    httpd_init();

    printf("Device is now connected and serving web pages\n");
    printf("You can access it at the IP address shown above\n");

    // Main loop
    while (1) {
        // Check connection status periodically
        int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        if (status != CYW43_LINK_UP) {
            printf("Connection lost, attempting to reconnect...\n");
            cyw43_arch_wifi_connect_wpa2_ent_async(ENTERPRISE_SSID);
        }
        
        sleep_ms(5000);
    }

    // Cleanup (never reached in this example)
    cyw43_arch_wifi_wpa2_ent_disable();
    cyw43_arch_deinit();
    
    return 0;
}
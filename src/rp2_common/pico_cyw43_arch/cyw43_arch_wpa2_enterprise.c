/*
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "cyw43.h"

#if PICO_CYW43_ARCH_DEBUG_ENABLED
#define CYW43_ARCH_DEBUG(...) printf(__VA_ARGS__)
#else
#define CYW43_ARCH_DEBUG(...) ((void)0)
#endif

int cyw43_arch_wifi_wpa2_ent_enable(void) {
    if (!cyw43_is_initialized(&cyw43_state)) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    int result = cyw43_wifi_wpa2_ent_enable(&cyw43_state);
    if (result != 0) {
        CYW43_ARCH_DEBUG("Failed to enable WPA2-Enterprise: %d\n", result);
        return PICO_ERROR_GENERIC;
    }
    
    CYW43_ARCH_DEBUG("WPA2-Enterprise enabled\n");
    return PICO_OK;
}

int cyw43_arch_wifi_wpa2_ent_disable(void) {
    if (!cyw43_is_initialized(&cyw43_state)) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    int result = cyw43_wifi_wpa2_ent_disable(&cyw43_state);
    if (result != 0) {
        CYW43_ARCH_DEBUG("Failed to disable WPA2-Enterprise: %d\n", result);
        return PICO_ERROR_GENERIC;
    }
    
    CYW43_ARCH_DEBUG("WPA2-Enterprise disabled\n");
    return PICO_OK;
}

int cyw43_arch_wifi_wpa2_ent_set_credentials(const cyw43_wpa2_ent_credentials_t *credentials) {
    if (!cyw43_is_initialized(&cyw43_state) || !credentials) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    int result = cyw43_wifi_wpa2_ent_set_credentials(&cyw43_state, credentials);
    if (result != 0) {
        CYW43_ARCH_DEBUG("Failed to set WPA2-Enterprise credentials: %d\n", result);
        return PICO_ERROR_GENERIC;
    }
    
    CYW43_ARCH_DEBUG("WPA2-Enterprise credentials set\n");
    return PICO_OK;
}

int cyw43_arch_wifi_connect_wpa2_ent_blocking(const char *ssid) {
    if (!ssid) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    // Start the connection
    int result = cyw43_arch_wifi_connect_wpa2_ent_async(ssid);
    if (result != 0) {
        return result;
    }
    
    // Wait for connection to complete
    int status;
    do {
        status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        if (status < 0) {
            CYW43_ARCH_DEBUG("WPA2-Enterprise connection failed with status %d\n", status);
            switch (status) {
                case CYW43_LINK_BADAUTH:
                    return PICO_ERROR_BADAUTH;
                case CYW43_LINK_NONET:
                    return PICO_ERROR_NO_DATA;  // Network not found
                case CYW43_LINK_FAIL:
                default:
                    return PICO_ERROR_CONNECT_FAILED;
            }
        }
#if PICO_CYW43_ARCH_POLL
        // Do a small amount of polling
        cyw43_arch_poll();
        sleep_ms(5);
#else
        // If we're not polling, we need to sleep to allow background work
        sleep_ms(10);
#endif
    } while (status != CYW43_LINK_UP);
    
    CYW43_ARCH_DEBUG("WPA2-Enterprise connection successful\n");
    return PICO_OK;
}

int cyw43_arch_wifi_connect_wpa2_ent_timeout_ms(const char *ssid, uint32_t timeout_ms) {
    if (!ssid) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    // Start the connection
    int result = cyw43_arch_wifi_connect_wpa2_ent_async(ssid);
    if (result != 0) {
        return result;
    }
    
    // Wait for connection to complete with timeout
    absolute_time_t timeout_time = make_timeout_time_ms(timeout_ms);
    int status;
    do {
        status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        if (status < 0) {
            CYW43_ARCH_DEBUG("WPA2-Enterprise connection failed with status %d\n", status);
            switch (status) {
                case CYW43_LINK_BADAUTH:
                    return PICO_ERROR_BADAUTH;
                case CYW43_LINK_NONET:
                    return PICO_ERROR_NO_DATA;  // Network not found
                case CYW43_LINK_FAIL:
                default:
                    return PICO_ERROR_CONNECT_FAILED;
            }
        }
        
        if (time_reached(timeout_time)) {
            CYW43_ARCH_DEBUG("WPA2-Enterprise connection timed out\n");
            return PICO_ERROR_TIMEOUT;
        }
        
#if PICO_CYW43_ARCH_POLL
        // Do a small amount of polling
        cyw43_arch_poll();
        sleep_ms(5);
#else
        // If we're not polling, we need to sleep to allow background work
        sleep_ms(10);
#endif
    } while (status != CYW43_LINK_UP);
    
    CYW43_ARCH_DEBUG("WPA2-Enterprise connection successful\n");
    return PICO_OK;
}

int cyw43_arch_wifi_connect_wpa2_ent_async(const char *ssid) {
    if (!cyw43_is_initialized(&cyw43_state)) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    if (!ssid) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    CYW43_ARCH_DEBUG("Attempting WPA2-Enterprise connection to SSID: %s\n", ssid);
    
    // Convert SSID to required format
    size_t ssid_len = strlen(ssid);
    if (ssid_len == 0 || ssid_len > 32) {
        return PICO_ERROR_INVALID_ARG;
    }
    
    // Call the low-level join function
    int result = cyw43_wifi_join_wpa2_ent(&cyw43_state, ssid_len, (const uint8_t *)ssid, NULL, CYW43_CHANNEL_NONE);
    if (result != 0) {
        CYW43_ARCH_DEBUG("Failed to start WPA2-Enterprise connection: %d\n", result);
        return PICO_ERROR_GENERIC;
    }
    
    return PICO_OK;
}
/**
 * Interactive WPA2-Enterprise Configuration Example for Pico W
 * 
 * This example allows you to configure WPA2-Enterprise credentials
 * via USB serial input at runtime - no recompilation needed!
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Buffer sizes for credential input
#define MAX_SSID_LEN 32
#define MAX_CREDENTIAL_LEN 128
#define MAX_CERT_LEN 4096

// Runtime credential storage
typedef struct {
    char ssid[MAX_SSID_LEN + 1];
    char identity[MAX_CREDENTIAL_LEN + 1];
    char username[MAX_CREDENTIAL_LEN + 1];
    char password[MAX_CREDENTIAL_LEN + 1];
    char ca_cert[MAX_CERT_LEN + 1];
    int eap_method; // 0=PEAP, 1=TLS, 2=TTLS
    bool credentials_set;
} runtime_credentials_t;

static runtime_credentials_t user_creds = {0};

// Function to read a line from USB serial with echo
int read_line(char* buffer, int max_len, bool hide_input) {
    int pos = 0;
    int ch;
    
    while (pos < max_len - 1) {
        ch = getchar_timeout_us(UINT64_MAX); // Wait indefinitely for input
        
        if (ch == '\r' || ch == '\n') {
            buffer[pos] = '\0';
            printf("\r\n");
            return pos;
        }
        else if (ch == 8 || ch == 127) { // Backspace or Delete
            if (pos > 0) {
                pos--;
                printf("\b \b"); // Erase character on screen
            }
        }
        else if (ch >= 32 && ch <= 126) { // Printable characters
            buffer[pos++] = ch;
            if (hide_input) {
                printf("*");
            } else {
                printf("%c", ch);
            }
        }
    }
    
    buffer[pos] = '\0';
    printf("\r\n");
    return pos;
}

void interactive_credential_setup(void) {
    printf("\n");
    printf("=== WPA2-Enterprise Interactive Setup ===\n");
    printf("Enter your network credentials below.\n");
    printf("Press ENTER after each field.\n\n");
    
    // Get SSID
    printf("Network SSID: ");
    read_line(user_creds.ssid, MAX_SSID_LEN, false);
    
    // Get EAP method
    printf("\nSelect EAP Method:\n");
    printf("0 - PEAP-MSCHAPv2 (username/password, most common)\n");
    printf("1 - EAP-TLS (certificate-based)\n");  
    printf("2 - EAP-TTLS (username/password alternative)\n");
    printf("Choice [0-2]: ");
    
    char method_str[8];
    read_line(method_str, 7, false);
    user_creds.eap_method = atoi(method_str);
    if (user_creds.eap_method < 0 || user_creds.eap_method > 2) {
        user_creds.eap_method = 0; // Default to PEAP
        printf("Invalid choice, defaulting to PEAP\n");
    }
    
    // Get identity
    printf("EAP Identity: ");
    read_line(user_creds.identity, MAX_CREDENTIAL_LEN, false);
    
    if (user_creds.eap_method == 0 || user_creds.eap_method == 2) {
        // PEAP or TTLS - need username and password
        printf("Username: ");
        read_line(user_creds.username, MAX_CREDENTIAL_LEN, false);
        
        printf("Password: ");
        read_line(user_creds.password, MAX_CREDENTIAL_LEN, true);
    }
    
    // Get CA certificate
    printf("\nCA Certificate (PEM format):\n");
    printf("Paste your CA certificate below, then type 'END' on a new line:\n");
    
    char cert_buffer[MAX_CERT_LEN] = {0};
    char line_buffer[256];
    int cert_pos = 0;
    
    while (cert_pos < MAX_CERT_LEN - 256) {
        read_line(line_buffer, 255, false);
        
        if (strcmp(line_buffer, "END") == 0) {
            break;
        }
        
        int line_len = strlen(line_buffer);
        if (cert_pos + line_len + 1 < MAX_CERT_LEN) {
            strcat(cert_buffer, line_buffer);
            strcat(cert_buffer, "\n");
            cert_pos += line_len + 1;
        }
    }
    
    strcpy(user_creds.ca_cert, cert_buffer);
    user_creds.credentials_set = true;
    
    printf("\n=== Credentials Configured ===\n");
    printf("SSID: %s\n", user_creds.ssid);
    printf("Identity: %s\n", user_creds.identity);
    printf("Method: %s\n", 
           user_creds.eap_method == 0 ? "PEAP" : 
           user_creds.eap_method == 1 ? "TLS" : "TTLS");
    if (user_creds.eap_method != 1) {
        printf("Username: %s\n", user_creds.username);
        printf("Password: %s\n", "********");
    }
    printf("CA Cert: %d bytes\n", (int)strlen(user_creds.ca_cert));
    printf("\n");
}

void connect_with_runtime_credentials(void) {
    if (!user_creds.credentials_set) {
        printf("ERROR: No credentials configured!\n");
        return;
    }
    
    printf("Connecting to WPA2-Enterprise network: %s\n", user_creds.ssid);
    printf("This may take 30-60 seconds...\n");
    
    // Enable WPA2-Enterprise
    if (cyw43_arch_wifi_wpa2_ent_enable() != PICO_OK) {
        printf("Failed to enable WPA2-Enterprise\n");
        return;
    }
    
    // Configure credentials
    cyw43_wpa2_ent_credentials_t credentials = {0};
    credentials.identity = (uint8_t*)user_creds.identity;
    credentials.identity_len = strlen(user_creds.identity);
    credentials.username = (uint8_t*)user_creds.username;
    credentials.username_len = strlen(user_creds.username);
    credentials.password = (uint8_t*)user_creds.password;
    credentials.password_len = strlen(user_creds.password);
    credentials.ca_cert = (uint8_t*)user_creds.ca_cert;
    credentials.ca_cert_len = strlen(user_creds.ca_cert);
    credentials.disable_time_check = true; // Disable if no RTC
    
    // Set EAP method
    switch (user_creds.eap_method) {
        case 0: credentials.eap_method = CYW43_EAP_METHOD_PEAP; break;
        case 1: credentials.eap_method = CYW43_EAP_METHOD_TLS; break;
        case 2: credentials.eap_method = CYW43_EAP_METHOD_TTLS; break;
    }
    
    if (cyw43_arch_wifi_wpa2_ent_set_credentials(&credentials) != PICO_OK) {
        printf("Failed to set credentials\n");
        return;
    }
    
    // Attempt connection
    int result = cyw43_arch_wifi_connect_wpa2_ent_timeout_ms(user_creds.ssid, 60000);
    
    if (result == PICO_OK) {
        printf("SUCCESS: Connected to %s!\n", user_creds.ssid);
        printf("WPA2-Enterprise authentication successful\n");
    } else {
        printf("CONNECTION FAILED: Error %d\n", result);
        switch (result) {
            case PICO_ERROR_BADAUTH:
                printf("- Authentication failed (check credentials)\n");
                break;
            case PICO_ERROR_NO_DATA:
                printf("- Network not found (check SSID)\n");
                break;
            case PICO_ERROR_TIMEOUT:
                printf("- Connection timed out\n");
                break;
            default:
                printf("- Unknown error\n");
                break;
        }
    }
}

void show_menu(void) {
    printf("\n=== WPA2-Enterprise Interactive Menu ===\n");
    printf("1. Configure credentials\n");
    printf("2. Connect to network\n");
    printf("3. Show current configuration\n");
    printf("4. Test connection status\n");
    printf("5. Disconnect\n");
    printf("h. Show this menu\n");
    printf("Choice: ");
}

void show_current_config(void) {
    if (!user_creds.credentials_set) {
        printf("No credentials configured yet.\n");
        return;
    }
    
    printf("\n=== Current Configuration ===\n");
    printf("SSID: %s\n", user_creds.ssid);
    printf("Identity: %s\n", user_creds.identity);
    printf("Method: %s\n", 
           user_creds.eap_method == 0 ? "PEAP-MSCHAPv2" : 
           user_creds.eap_method == 1 ? "EAP-TLS" : "EAP-TTLS");
    if (user_creds.eap_method != 1) {
        printf("Username: %s\n", user_creds.username);
    }
    printf("CA Certificate: %s\n", strlen(user_creds.ca_cert) > 0 ? "Present" : "Not set");
    printf("\n");
}

void test_connection_status(void) {
    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("Connection status: ");
    
    switch (status) {
        case CYW43_LINK_DOWN:
            printf("Disconnected\n");
            break;
        case CYW43_LINK_JOIN:
            printf("Connecting...\n");
            break;
        case CYW43_LINK_NOIP:
            printf("Connected but no IP address\n");
            break;
        case CYW43_LINK_UP:
            printf("Connected with IP address\n");
            break;
        case CYW43_LINK_FAIL:
            printf("Connection failed\n");
            break;
        case CYW43_LINK_NONET:
            printf("Network not found\n");
            break;
        case CYW43_LINK_BADAUTH:
            printf("Authentication failed\n");
            break;
        default:
            printf("Unknown status (%d)\n", status);
            break;
    }
}

int main() {
    stdio_init_all();

    printf("\n");
    printf("==========================================\n");
    printf("  Pico W WPA2-Enterprise Interactive\n");
    printf("==========================================\n");
    printf("This firmware allows you to configure\n");
    printf("enterprise WiFi credentials at runtime!\n");
    printf("\n");

    // Initialize CYW43 driver
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43 driver\n");
        return 1;
    }

    printf("CYW43 driver initialized successfully\n");
    
    // Enable station mode
    cyw43_arch_enable_sta_mode();
    printf("WiFi station mode enabled\n");
    
    show_menu();
    
    // Main interactive loop
    while (1) {
        char input[10];
        read_line(input, 9, false);
        
        switch (input[0]) {
            case '1':
                interactive_credential_setup();
                show_menu();
                break;
                
            case '2':
                connect_with_runtime_credentials();
                break;
                
            case '3':
                show_current_config();
                break;
                
            case '4':
                test_connection_status();
                break;
                
            case '5':
                printf("Disconnecting...\n");
                cyw43_arch_wifi_wpa2_ent_disable();
                printf("Disconnected\n");
                break;
                
            case 'h':
            case 'H':
                show_menu();
                break;
                
            default:
                printf("Invalid choice. Press 'h' for menu.\n");
                break;
        }
        
        // Blink LED to show it's alive
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(100);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        
        printf("\nChoice: ");
    }
    
    return 0;
}
# WPA2-Enterprise Support for Pico W

This example demonstrates how to use WPA2-Enterprise authentication with the Raspberry Pi Pico W.

## Features

This implementation supports:
- **EAP-TLS** (certificate-based authentication)
- **PEAP-MSCHAPv2** (username/password authentication)  
- **EAP-TTLS** (username/password authentication)
- Certificate validation with CA certificates
- Client certificate authentication for EAP-TLS
- Configurable time validation (can be disabled if no RTC available)

## Usage

1. **Enable WPA2-Enterprise mode:**
   ```c
   cyw43_arch_wifi_wpa2_ent_enable();
   ```

2. **Configure credentials:**
   ```c
   cyw43_wpa2_ent_credentials_t credentials = {0};
   credentials.identity = (uint8_t*)"user@example.com";
   credentials.identity_len = strlen("user@example.com");
   credentials.username = (uint8_t*)"user@example.com";
   credentials.username_len = strlen("user@example.com");
   credentials.password = (uint8_t*)"password";
   credentials.password_len = strlen("password");
   credentials.ca_cert = (uint8_t*)ca_cert_pem;
   credentials.ca_cert_len = strlen(ca_cert_pem);
   credentials.eap_method = CYW43_EAP_METHOD_PEAP;
   
   cyw43_arch_wifi_wpa2_ent_set_credentials(&credentials);
   ```

3. **Connect to network:**
   ```c
   cyw43_arch_wifi_connect_wpa2_ent_blocking("eduroam");
   ```

## Supported EAP Methods

- `CYW43_EAP_METHOD_TLS` - EAP-TLS (requires client certificate and private key)
- `CYW43_EAP_METHOD_PEAP` - PEAP-MSCHAPv2 (requires username/password and CA cert)
- `CYW43_EAP_METHOD_TTLS` - EAP-TTLS (requires username/password and CA cert)

## Common Networks

### eduroam (Educational networks)
- Use PEAP-MSCHAPv2
- Requires university credentials
- Usually requires CA certificate

### Enterprise WPA2 (Corporate networks)
- May use EAP-TLS for certificate-based auth
- Or PEAP-MSCHAPv2 for username/password
- Requires appropriate certificates

## Building

```bash
mkdir build
cd build
cmake ..
make
```

Make sure to set your PICO_SDK_PATH environment variable and use a Pico W board configuration.

## Notes

- Authentication may take 30-60 seconds for enterprise networks
- Ensure you have the correct CA certificate for your network
- For EAP-TLS, you need both client certificate and private key
- Time validation can be disabled if your device doesn't have an RTC
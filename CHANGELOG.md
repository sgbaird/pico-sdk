# Pico SDK Changelog

## [Unreleased] - 2024-12-16

### Added
- **WPA2-Enterprise support for Pico W**: Implemented comprehensive WPA2-Enterprise authentication support
  - Added support for EAP-TLS, PEAP-MSCHAPv2, and EAP-TTLS authentication methods
  - New authentication constants: `CYW43_AUTH_WPA2_ENTERPRISE_TLS`, `CYW43_AUTH_WPA2_ENTERPRISE_PEAP`, `CYW43_AUTH_WPA2_ENTERPRISE_TTLS`
  - Added `cyw43_wpa2_ent_credentials_t` structure for credential management
  - New low-level API functions:
    - `cyw43_ll_wifi_wpa2_ent_enable()` / `cyw43_ll_wifi_wpa2_ent_disable()`
    - `cyw43_ll_wifi_wpa2_ent_set_credentials()`
    - `cyw43_ll_wifi_join_wpa2_ent()`
  - New high-level API functions:
    - `cyw43_wifi_wpa2_ent_enable()` / `cyw43_wifi_wpa2_ent_disable()`
    - `cyw43_wifi_wpa2_ent_set_credentials()`
    - `cyw43_wifi_join_wpa2_ent()`
  - New arch-level convenience functions:
    - `cyw43_arch_wifi_wpa2_ent_enable()` / `cyw43_arch_wifi_wpa2_ent_disable()`
    - `cyw43_arch_wifi_wpa2_ent_set_credentials()`
    - `cyw43_arch_wifi_connect_wpa2_ent_blocking()`
    - `cyw43_arch_wifi_connect_wpa2_ent_timeout_ms()`
    - `cyw43_arch_wifi_connect_wpa2_ent_async()`
  - Support for certificate-based authentication (EAP-TLS)
  - Support for username/password authentication (PEAP, TTLS)
  - CA certificate validation with option to disable time checking
  - Added example implementation in `/test/pico_wpa2_enterprise_example/`
  - Compatible with eduroam and corporate enterprise networks

### Files Added
- `lib/cyw43-driver/src/cyw43_wpa2_enterprise.c` - Core WPA2-Enterprise implementation
- `src/rp2_common/pico_cyw43_arch/cyw43_arch_wpa2_enterprise.c` - High-level API implementation
- `test/pico_wpa2_enterprise_example/` - Complete example application

### Files Modified
- `lib/cyw43-driver/src/cyw43_ll.h` - Added WPA2-Enterprise constants, structures, and function declarations
- `lib/cyw43-driver/src/cyw43.h` - Added WPA2-Enterprise state management and API functions
- `src/rp2_common/pico_cyw43_arch/include/pico/cyw43_arch.h` - Added convenience API functions
- `src/rp2_common/pico_cyw43_driver/CMakeLists.txt` - Added WPA2-Enterprise source files
- `src/rp2_common/pico_cyw43_arch/CMakeLists.txt` - Added WPA2-Enterprise arch source files

This implementation provides a proof-of-concept for WPA2-Enterprise support that can be extended and refined based on testing with real enterprise networks.
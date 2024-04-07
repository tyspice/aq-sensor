#include "WifiHelper.h"
#include <iostream>
#include "pico/cyw43_arch.h"
#include "secrets.h"

WifiHelper::WifiHelper() {
  if (cyw43_arch_init()) {
    std::cout << "failed to initialise WIFI peripheral" << std::endl;
  }
  
  cyw43_arch_enable_sta_mode();

  std::cout << "Connecting to WIFI..." << std::endl;

  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
    std::cout << "Failed to connect" << std::endl;
    m_initialized = 1;
  } else {
    std::cout << "Connected" << std::endl;
    m_initialized = 0;
  }
}

bool WifiHelper::_init() {
  return m_initialized;
}

bool WifiHelper::init() {
  return getInstance()._init();
}

WifiHelper& WifiHelper::getInstance() {
  static WifiHelper instance;
  return instance;
}
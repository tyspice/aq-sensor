#include <iostream>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "secrets.h"
#include "lwip/ip_addr.h"
#include "lwip/altcp.h"
#include "lwip/apps/mqtt.h"

void mqtt_connect_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
  std::cout << "MQTT Connection Status: " << status <<  std::endl;
}

void mqtt_request_cb(void *arg, err_t error) {
  if (error) {
    std::cout << "Error making MQTT Request. Code: " << error << std::endl;
  }
}

int main() {
  stdio_init_all();
  if (cyw43_arch_init()) {
    std::cout << "failed to initialise WIFI peripheral" << std::endl;
    return 1;
  }
  
  cyw43_arch_enable_sta_mode();

  std::cout << "Connecting to WIFI..." << std::endl;

  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
    std::cout << "Failed to connect" << std::endl;
    return 1;
  } else {
    std::cout << "Connected" << std::endl;
  }

  mqtt_client_t *client = mqtt_client_new();
  struct mqtt_connect_client_info_t ci;
  memset(&ci, 0, sizeof(ci));
  ci.client_id = MQTT_CLIENT;
  ci.keep_alive = 10;
  ci.client_user = MQTT_USER;
  ci.client_pass = MQTT_PASSWD;
  ip_addr_t ip;
  IP4_ADDR(&ip, 192, 168, 20, 73);
  err_t err = mqtt_client_connect(client, &ip, 1883, mqtt_connect_cb, 0, &ci);

  char payload[] = "World";
  u8_t qos = 2;
  u8_t retain = 0;

  while(1) {
    err_t err = mqtt_publish(client, "Hello", payload, strlen(payload), qos, retain, mqtt_request_cb, client);
    sleep_ms(2000);
  }
}
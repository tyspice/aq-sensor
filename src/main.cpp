#include "FreeRTOS.h"
#include <iostream>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "secrets.h"
#include "lwip/ip_addr.h"
#include "lwip/altcp.h"
#include "lwip/apps/mqtt.h"
#include "task.h"
#include "WifiHelper.h"

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY )

void mqtt_connect_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
  std::cout << "MQTT Connection Status: " << status <<  std::endl;
}

void mqtt_request_cb(void *arg, err_t error) {
  if (error) {
    std::cout << "Error making MQTT Request. Code: " << error << std::endl;
  }
}

void main_task(void *params) {
  bool wifiReady = WifiHelper::init();

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
    err = mqtt_publish(client, "Hello", payload, strlen(payload), qos, retain, mqtt_request_cb, client);
    vTaskDelay(1000);
  }
}

int main() {
  stdio_init_all();

  TaskHandle_t task;
  xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);

  vTaskStartScheduler();
}
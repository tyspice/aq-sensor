#include "FreeRTOS.h"
#include "task.h"
#include <iostream>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "WifiHelper.h"
#include "MQTTClient.h"
using std::string;

#define BOARD_LED 25

void publisher(void* params) {
  MQTTClient* mqttClient = static_cast<MQTTClient*>(params);
  for ( ;; ) {
    err_t err = mqttClient->publish(string("Hello"), string("World"));
    vTaskDelay(5000);
  }
}

void statusMonitor(void* params) {
  MQTTClient* mqttClient = static_cast<MQTTClient*>(params);
  // Display WIFI and MQTT connection statuses via onboard LED
  for ( ;; ) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    vTaskDelay(500);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    vTaskDelay(500);
  }
}

void mainTask(void *params) {
  bool wifiReady = WifiHelper::init();
  MQTTClient mqttClient;

  TaskHandle_t publisherTaskHandle;
  TaskHandle_t statusMonitorTaskHandle;

  xTaskCreate(publisher, "publisher", configMINIMAL_STACK_SIZE, &mqttClient, tskIDLE_PRIORITY, &publisherTaskHandle);
  xTaskCreate(statusMonitor, "statusMonitor", configMINIMAL_STACK_SIZE, &mqttClient, tskIDLE_PRIORITY, &statusMonitorTaskHandle);

  while(1) { vTaskDelay(1000); }
}

int main() {
  stdio_init_all();
  TaskHandle_t task;
  xTaskCreate(mainTask, "mainTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &task);
  vTaskStartScheduler();
}
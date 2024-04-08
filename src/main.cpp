#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "WifiHelper.h"
#include "MQTTClient.h"
using std::string;

void main_task(void *params) {
  bool wifiReady = WifiHelper::init();
  MQTTClient mqttClient;

  while(1) {
    err_t err = mqttClient.publish(string("Hello"), string("World"));
    vTaskDelay(1000);
  }
}

int main() {
  stdio_init_all();
  TaskHandle_t task;
  xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &task);
  vTaskStartScheduler();
}
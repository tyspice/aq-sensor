#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "WifiHelper.h"
#include "TimeHelper.h"
#include "MQTTClient.h"
#include "hardware/i2c.h"
#include "bme68x/bme68x.h"
#include "nlohmann/json.hpp"
#include "secrets.h"

#define BAUD_RATE 100 * 1000
// hardware config
#define BOARD_LED 25
#define SDA_PIN 4
#define SCL_PIN 5
// priorities
#define MAIN_TASK_P tskIDLE_PRIORITY + 1U
#define STATUS_MONITOR_TSK_P tskIDLE_PRIORITY + 2U
#define PUBLISHER_TSK_P tskIDLE_PRIORITY + 3U
#define BME_READER_TSK_P tskIDLE_PRIORITY + 5U

using std::string;
using json = nlohmann::json;

QueueHandle_t bmeDataQueue;

BME68X_INTF_RET_TYPE i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  i2c_write_blocking(i2c0, BME68X_I2C_ADDR_LOW, &reg_addr, 1, true);
  i2c_read_blocking(i2c0, BME68X_I2C_ADDR_LOW, reg_data, len, false);
  return 0;
}

BME68X_INTF_RET_TYPE i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t buf[len+1];
  buf[0] = reg_addr;
  std::copy(reg_data, reg_data + len, buf + 1);
  i2c_write_blocking(i2c0, BME68X_I2C_ADDR_LOW, buf, (uint8_t) len+1, false);
  return 0;
}

void delay_us(uint32_t period, void *intf_ptr) {
  sleep_us(period);
}

string preparePayload(const bme68x_data &data) {
  const char *id = MQTT_CLIENT;
  time_t timestamp = TimeHelper::getUnixTimestamp();
  json arr;
  json d;

  d["field"] = "temperature";
  d["value"] = data.temperature;
  d["unit"] = "C";
  arr.push_back(d);

  d["field"] = "pressure";
  d["value"] = data.pressure;
  d["unit"] = "Pa";
  arr.push_back(d);

  d["field"] = "humidity";
  d["value"] = data.humidity;
  d["unit"] = "%";
  arr.push_back(d);

  d["field"] = "gasResistance";
  d["value"] = data.gas_resistance;
  d["unit"] = "ohm";
  arr.push_back(d);

  json j;
  j["sensorId"] = id;
  j["timestamp"] = timestamp;
  j["data"] = arr;

  return j.dump();
}

void bmeReader(void* params) {
  // Initialize the I2C interface
  i2c_init(i2c0, BAUD_RATE);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  struct bme68x_dev bme;
  int8_t rslt;
  struct bme68x_conf conf;
  struct bme68x_heatr_conf heatr_conf;
  struct bme68x_data data;
  uint32_t del_period;
  uint32_t time_ms = 0;
  uint8_t n_fields;
  uint16_t sample_count = 1;

  // Set the BME688 device's I2C address and interface
  bme.intf = BME68X_I2C_INTF;
  bme.variant_id = BME68X_VARIANT_GAS_LOW;
  bme.read = i2c_read;
  bme.write = i2c_write;
  bme.delay_us = delay_us;
  bme.amb_temp = 20;
  rslt = bme68x_init(&bme);

  // set the config values
  conf.filter = BME68X_FILTER_OFF;
  conf.odr = BME68X_ODR_NONE;
  conf.os_hum = BME68X_OS_16X;
  conf.os_pres = BME68X_OS_1X;
  conf.os_temp = BME68X_OS_2X;
  rslt = bme68x_set_conf(&conf, &bme);

  // set the heater config values
  heatr_conf.enable = BME68X_ENABLE;
  heatr_conf.heatr_temp = 300;
  heatr_conf.heatr_dur = 100;
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);

  for ( ;; ) {
    rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
    bme.delay_us(del_period, bme.intf_ptr);
    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
    xQueueSend(bmeDataQueue, &data, 0);
    vTaskDelay(5000);
  }
}

void publisher(void* params) {
  auto mqttClient = *static_cast<std::shared_ptr<MQTTClient>*>(params);
  struct bme68x_data bmeData;
  for (;;)
  {
    if(xQueueReceive(bmeDataQueue, &bmeData, 0)) {
      err_t err = mqttClient->publish(string("data/home/indoor/airQuality"), preparePayload(bmeData));
    }
    vTaskDelay(1000);
  }
}

void statusMonitor(void* params) {
  auto mqttClient = *static_cast<std::shared_ptr<MQTTClient>*>(params);
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
  TimeHelper::init();
  bmeDataQueue = xQueueCreate(5, sizeof(bme68x_data));
  auto mqttClient = std::make_shared<MQTTClient>();

  TaskHandle_t publisherTaskHandle;
  TaskHandle_t statusMonitorTaskHandle;
  TaskHandle_t bmeReaderTaskHandle;

  xTaskCreate(publisher, "publisher", configMINIMAL_STACK_SIZE * 4, &mqttClient, PUBLISHER_TSK_P, &publisherTaskHandle);
  xTaskCreate(statusMonitor, "statusMonitor", configMINIMAL_STACK_SIZE, &mqttClient, STATUS_MONITOR_TSK_P, &statusMonitorTaskHandle);
  xTaskCreate(bmeReader, "bmeReader", configMINIMAL_STACK_SIZE, NULL, BME_READER_TSK_P, &bmeReaderTaskHandle);

  vTaskDelete(NULL);
}

int main() {
  stdio_init_all();
  TaskHandle_t task;
  xTaskCreate(mainTask, "mainTask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_P, &task);
  vTaskStartScheduler();
}
#include <iostream>
#include "MQTTClient.h"
#include "lwip/apps/mqtt.h"
#include "pico/stdlib.h"
#include <string>
#include <cstring>
#include "secrets.h"

using std::string;

static void defaultConnectCB(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
  std::cout << "MQTT Connection Status: " << status <<  std::endl;
}

static void defaultPublishCB(void *arg, err_t error) {
  if (error) {
    std::cout << "Error making MQTT Request. Code: " << error << std::endl;
  }
}

MQTTClient::MQTTClient() {
  m_client = mqtt_client_new();
  std::memset(&m_ci, 0,sizeof(m_ci));
  ip4addr_aton(MQTT_BROKER_ADDR, &m_ip);
  m_ci.client_id = MQTT_CLIENT;
  m_ci.keep_alive = MQTT_KEEP_ALIVE;
  m_ci.client_user = MQTT_USER;
  m_ci.client_pass = MQTT_PASSWD;
  mqtt_client_connect(m_client, &m_ip, MQTT_PORT, defaultConnectCB, NULL, &m_ci);
};

MQTTClient::MQTTClient(string clientId, string brokerAddr, u16_t port, string username, string password, u16_t keepAlive, mqtt_connection_cb_t cb) {
  m_client = mqtt_client_new();
  std::memset(&m_ci, 0,sizeof(m_ci));
  ip4addr_aton(brokerAddr.c_str(), &m_ip);
  m_ci.client_id = clientId.c_str();
  m_ci.keep_alive = keepAlive;
  m_ci.client_user = username.c_str();
  m_ci.client_pass = password.c_str();
  mqtt_client_connect(m_client, &m_ip, port, cb, NULL, &m_ci);
}

err_t MQTTClient::publish(string topic, string payload, u8_t qos = 0, u8_t retain = 0) {
  return mqtt_publish(m_client, topic.c_str(), payload.c_str(), payload.length(), qos, retain, defaultPublishCB, m_client);
}


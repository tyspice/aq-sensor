#pragma once

#include "lwip/ip_addr.h"
#include "lwip/apps/mqtt.h"
#include <string>

using std::string;

/**
 * @class MQTTClient
 * @brief Represents an MQTT client for publishing messages or subscribing to an MQTT broker.
 */
class MQTTClient {
private:
  mqtt_client_t *m_client;
  struct mqtt_connect_client_info_t m_ci;
  ip_addr_t m_ip;
public:
  /**
   * Default constructor for MQTTClient. Uses the values found
   * in secrets.h to establish an mqtt connection.
   */
  MQTTClient();

  /**
   * @brief Constructor for MQTTClient.
   * @param clientId The client ID for the MQTT connection.
   * @param brokerAddr The ip address of the MQTT broker.
   * @param port The port number of the MQTT broker.
   * @param username The username for authentication with the MQTT broker.
   * @param password The password for authentication with the MQTT broker.
   * @param keepAlive The keep-alive interval for the MQTT connection.
   * @param cb The callback function to be called when the MQTT connection is established.
   */
  MQTTClient(string clientId, string brokerAddr, u16_t port, string username, string password, u16_t keepAlive, mqtt_connection_cb_t cb);

  /**
   * @brief Publishes a message to the specified topic.
   * @param topic The topic to publish the message to.
   * @param payload The payload of the message.
   * @param qos The quality of service level for the message.
   * @param retain Whether the message should be retained by the broker.
   * @return An error code indicating the success or failure of the publish operation.
   */
  err_t publish(string topic, string payload, u8_t qos, u8_t retain);
};
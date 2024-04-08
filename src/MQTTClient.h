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
   * Publishes a message to the MQTT broker.
   *
   * @param topic The topic to publish the message to.
   * @param payload The payload of the message.
   * @param qos The quality of service level for the message (default is 0).
   * @param retain Whether the message should be retained by the broker (default is 0).
   * @return An error code indicating the success or failure of the operation.
   */
  err_t publish(string topic, string payload, u8_t qos = 0, u8_t retain = 0);

  /**
   * Publishes a message to the MQTT broker.
   *
   * @param topic   The MQTT topic to publish the message to.
   * @param payload The message payload to be published.
   * @param qos     The quality of service level for the message (0, 1, or 2).
   * @param retain  Whether the message should be retained by the broker.
   * @param cb      The callback function to be called when the publish operation completes.
   * @param arg     An optional argument to be passed to the callback function.
   * @return        An error code indicating the success or failure of the publish operation.
   */
  err_t publish(string topic, string payload, u8_t qos, u8_t retain, mqtt_request_cb_t cb, void *arg);
};
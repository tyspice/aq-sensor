#pragma once

/**
 * The WifiHelper Singleton class provides a helper for managing WiFi connections.
 */
class WifiHelper {
private:
  WifiHelper(); /**< Private constructor to prevent instantiation. */
  bool _init(); /**< Initialize the WiFi helper. */
  bool m_initialized; /**< Flag indicating whether the WiFi helper is initialized. */

public:
  WifiHelper(const WifiHelper&) = delete; /**< Deleted copy constructor to prevent copying. */
  
  /**
   * Initializes the WiFi helper.
   * @return True if initialization is successful, false otherwise.
   */
  static bool init();
  
  /**
   * Retrieves the singleton instance of the WiFi helper.
   * @return Reference to the singleton instance of the WiFi helper.
   */
  static WifiHelper& getInstance();
};
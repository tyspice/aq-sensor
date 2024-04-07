#pragma once

class WifiHelper {
private:
  WifiHelper();
  bool _init();
  bool m_initialized;

public:
  WifiHelper(const WifiHelper&) = delete;
  static bool init();
  static WifiHelper& getInstance();
};
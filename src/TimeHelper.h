#pragma once

#include "hardware/rtc.h"
#include <time.h>

/**
 * Namespace containing helper functions for managing date and time.
 */
namespace TimeHelper {
  /**
   * Initializes the TimeHelper module.
   */
  void init();

  /**
   * Get the current number of seconds since the Unix epoch (January 1, 1970).
   */
  time_t getUnixTimestamp();

  /**
   * Retrieves the current date and time.
   * 
   * @param t Pointer to a datetime_t struct where the current date and time will be stored.
   */
  void getDateTime(datetime_t* t);
}
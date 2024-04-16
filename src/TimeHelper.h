#pragma once

#include "hardware/rtc.h"

/**
 * Namespace containing helper functions for managing date and time.
 */
namespace TimeHelper {
  /**
   * Initializes the TimeHelper module by calling rtc_init().
   */
  void init();

  /**
   * Sets the current date and time.
   * 
   * @param t Pointer to a datetime_t struct containing the desired date and time.
   */
  void setDateTime(datetime_t* t);

  /**
   * Retrieves the current date and time.
   * 
   * @param t Pointer to a datetime_t struct where the current date and time will be stored.
   */
  void getDateTime(datetime_t* t);
}
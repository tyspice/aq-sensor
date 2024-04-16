#include "hardware/rtc.h"
#include "TimeHelper.h"

void TimeHelper::init() {
  rtc_init();
}

void TimeHelper::setDateTime(datetime_t* t) {
  rtc_set_datetime(t);
}

void TimeHelper::getDateTime(datetime_t* t) {
  rtc_get_datetime(t);
}
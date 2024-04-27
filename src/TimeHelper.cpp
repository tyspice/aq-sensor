#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/rtc.h"
#include "TimeHelper.h"
#include <time.h>
#include "lwip/apps/sntp.h"
#include "secrets.h"
#include "lwipopts.h"

extern "C" void SNTPSetRTC(u32_t t, u32_t us) {
  time_t seconds_since_1970 = t - 2208988800;
  struct tm *datetime = gmtime(&seconds_since_1970);
  datetime_t dt;
  dt.year = datetime->tm_year + 1900;
  dt.month = datetime->tm_mon + 1;
  dt.day = datetime->tm_mday;
  dt.dotw = datetime->tm_wday;
  dt.hour = datetime->tm_hour;
  dt.min = datetime->tm_min;
  dt.sec = datetime->tm_sec;
  rtc_init();
  rtc_set_datetime(&dt);
}

void TimeHelper::init() {
  ip_addr_t ip;
  ipaddr_aton(SNTP_SERVER_ADDR, &ip);
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setserver(0, &ip);
  sntp_init();
}

time_t TimeHelper::getUnixTimestamp() {
  datetime_t dt;
  rtc_get_datetime(&dt);
  struct tm t;
  t.tm_year = dt.year - 1900;  // tm_year is years since 1900
  t.tm_mon = dt.month - 1;     // tm_mon is months since January (0-11)
  t.tm_mday = dt.day;
  t.tm_hour = dt.hour;
  t.tm_min = dt.min;
  t.tm_sec = dt.sec;
  t.tm_isdst = -1;              // -1 means unknown DST information
  return mktime(&t);
}

void TimeHelper::getDateTime(datetime_t* t) {
  rtc_get_datetime(t);
}
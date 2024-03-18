#include <stdint.h>
#include <time.h>

const struct timespec setTime(const int32_t sec, const int32_t nsec) {
  struct timespec tv;

  tv.tv_sec = sec;
  tv.tv_nsec = nsec;
  return tv;
}

#include <stdint.h>
#include <time.h>

#include "ft_ping.h"

double timespecToMs(struct timespec t) {
  return t.tv_sec * 1000.0 + t.tv_nsec / 1000000.0;
}

const struct timespec setTime(const int32_t sec, const int32_t nsec) {
  struct timespec tv;

  tv.tv_sec = sec;
  tv.tv_nsec = nsec;
  return tv;
}

void accumulate(struct timespec *total, struct timespec new) {
  total->tv_sec += new.tv_sec;
  total->tv_nsec += new.tv_nsec;
  if (total->tv_nsec >= 1000000000) {
    total->tv_sec++;
    total->tv_nsec -= 1000000000;
  }
}

void calculateAverage(t_stats *stats) {
  if (stats->received_packages > 0) {
    stats->avg.tv_sec = stats->total_rtt.tv_sec / stats->received_packages;
    stats->avg.tv_nsec = stats->total_rtt.tv_nsec / stats->received_packages;
    uint64_t additional_nsec =
        stats->total_rtt.tv_nsec % stats->received_packages;
    stats->avg.tv_nsec +=
        (additional_nsec * 1000000000) / stats->received_packages / 1000000000;
  }
}

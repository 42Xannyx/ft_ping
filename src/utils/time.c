#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ft_ping.h"

double timespecToMs(t_timespec tv) {
  return tv.tv_sec * 1000.0 + tv.tv_nsec / 1000000.0;
}

const t_timespec setTime(const int32_t sec, const int32_t nsec) {
  struct timespec tv;

  tv.tv_sec = sec;
  tv.tv_nsec = nsec;
  return tv;
}

void getClock(t_timespec *tv) {
  int32_t i = 0;

  while (i < 3) {
    if (clock_gettime(CLOCK_MONOTONIC, tv) == 0) {
      break;
    }
    i++;
  }

  if (i == 3) {
    fprintf(stderr, "Failed to get monotonic clock time after %d attempts.\n",
            3);
    exit(EXIT_FAILURE);
  }
}

const t_timespec setDuration(t_timespec t_start, t_timespec t_end) {
  t_timespec time = {0, 0};

  if ((t_end.tv_nsec - t_start.tv_nsec) < 0) {
    time.tv_sec = t_end.tv_sec - t_start.tv_sec - 1;
    time.tv_nsec = 1000000000 + t_end.tv_nsec - t_start.tv_nsec;
  } else {
    time.tv_sec = t_end.tv_sec - t_start.tv_sec;
    time.tv_nsec = t_end.tv_nsec - t_start.tv_nsec;
  }

  return time;
}

void accumulate(t_timespec *total, t_timespec new) {
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

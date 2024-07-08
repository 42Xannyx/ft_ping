#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ft_ping.h"

/**
 * CLOCK_MONOTONIC meaning EPOCH Time
 */
void get_clock(t_timespec *tv) {
  int32_t i = 0;

  while (i < 3) {
    if (clock_gettime(CLOCK_MONOTONIC, tv) == 0) {
      break;
    }
    i++;
  }

  if (i == 3) {
    (void)fprintf(stderr,
                  "Failed to get monotonic clock time after 3 attempts.\n");
    exit(EXIT_FAILURE);
  }
}

t_timespec set_duration(t_timespec t_start, t_timespec t_end) {
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

void calculate_average(t_stats *stats) {
  if (stats->received_packages > 0) {
    stats->avg.tv_sec = stats->total_rtt.tv_sec / stats->received_packages;
    stats->avg.tv_nsec = stats->total_rtt.tv_nsec / stats->received_packages;
    uint64_t additional_nsec =
        stats->total_rtt.tv_nsec % stats->received_packages;
    stats->avg.tv_nsec +=
        (additional_nsec * 1000000000) / stats->received_packages / 1000000000;
  }
}

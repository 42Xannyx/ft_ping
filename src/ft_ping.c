#include "ft_ping.h"
#include "flags.h"
#include "payload.h"

#include <arpa/inet.h>
#include <bits/time.h>
#include <math.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int compare_timespec(const struct timespec *a, const struct timespec *b) {
  if (a->tv_sec < b->tv_sec)
    return -1;
  if (a->tv_sec > b->tv_sec)
    return 1;
  if (a->tv_nsec < b->tv_nsec)
    return -1;
  if (a->tv_nsec > b->tv_nsec)
    return 1;
  return 0;
}

void add_timespec(struct timespec *result, struct timespec *a,
                  struct timespec *b) {
  result->tv_sec = a->tv_sec + b->tv_sec;
  result->tv_nsec = a->tv_nsec + b->tv_nsec;
  if (result->tv_nsec >= 1000000000) {
    result->tv_sec++;
    result->tv_nsec -= 1000000000;
  }
}

struct timespec subtract_timespec(struct timespec *a, struct timespec *b) {
  struct timespec result = {0, 0};

  result.tv_sec = a->tv_sec - b->tv_sec;
  result.tv_nsec = a->tv_nsec - b->tv_nsec;
  if (result.tv_nsec < 0) {
    result.tv_sec--;
    result.tv_nsec += 1000000000;
  }

  return result;
}

volatile sig_atomic_t g_sig_flag = 0; // Signal use only!

/*
 * From the man:
 * When the specified number of packets have been sent (and received) or if the
 * program is terminated with a SIGINT, a brief summary is displayed. Shorter
 * current statistics can be obtained without termination of process with signal
 * SIGQUIT.
 */
void handle_signal(int32_t sig) { g_sig_flag = sig; }

void update_stats(t_stats *stats, t_timespec new) {
  double newMs = timespec_to_ms(new);

  stats->received_packages++;
  stats->sum += newMs;
  stats->sumSquared += newMs * newMs;

  double avgMs = stats->sum / stats->received_packages;
  double meanSq = avgMs * avgMs;
  double sqMean = stats->sumSquared / stats->received_packages;
  double stddevMs = sqrt(sqMean - meanSq);

  stats->avg.tv_sec = avgMs / 1000;
  stats->avg.tv_nsec = (uint64_t)(avgMs * 1000000) % 1000000000;

  stats->stddev.tv_sec = stddevMs / 1000;
  stats->stddev.tv_nsec = (uint64_t)(stddevMs * 1000000) % 1000000000;

  stats->ewma = weighted_moving_average(newMs, stats->ewma);
}

int32_t main(int32_t argc, char *argv[]) {
  if (__builtin_expect(argc < 2, 2)) {
    (void)fprintf(stderr, "%s: need more arguments\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (__builtin_expect(getuid() != 0, 0)) {
    (void)fprintf(stderr, "%s: This program requires root privileges!\n",
                  argv[0]);
    return EXIT_FAILURE;
  }

  (void)signal(SIGINT, handle_signal);
  (void)signal(SIGQUIT, handle_signal);

  t_stats stats;
  t_flags *flags = init_flags(argc, argv);
  const char *ip = argv[argc - 1];

  (void)memset(&stats, 0, sizeof(t_stats));

  const int32_t socket_fd = create_socket();

  set_socket(socket_fd);

  const char *ip_str = fetch_hostname(ip);

  const t_sockaddr_in *address = set_address(ip_str);
  int32_t result = inet_pton(AF_INET, ip_str, (void *)&address->sin_addr);
  if (result < 1) {
    perror("inet_pton()");
    return (EXIT_FAILURE);
  }

  if (flags->verbose) {
    verbose_message_on_start(socket_fd);
  }
  t_packet *packet = init_packet();

  if (flags->verbose && ip) {
    (void)printf("ai->ai_family: AF_INET, ai->ai_canonname: '%s'\n", ip);
  }
  message_on_start(ip_str, ip, sizeof(packet->payload));

  struct timespec t_start = {0, 0}, t_end = {0, 0};
  struct timespec ping_interval = {1, 0}; // 1 second
  struct timespec next_ping_time;

  clock_gettime(CLOCK_MONOTONIC, &t_start);
  clock_gettime(CLOCK_MONOTONIC, &next_ping_time);

  while (true) {
    char buf[84]; // ICMP Payload (64 Bytes) + IP Header (20 Bytes)
    if (g_sig_flag == SIGQUIT) {
      g_sig_flag = 0;
      message_on_sigquit(stats);
    } else if (g_sig_flag == SIGINT) {
      break;
    }

    struct timespec send_time, recv_time;
    clock_gettime(CLOCK_MONOTONIC, &send_time);
    ssize_t ret = send_ping(socket_fd, *address, packet, sizeof(*packet));
    ret = recv_ping(buf, sizeof(buf), socket_fd, *address);
    clock_gettime(CLOCK_MONOTONIC, &recv_time);

    if (ret < 0) {
      (void)fprintf(stderr, "request timeout for icmp_req=%d\n",
                    packet->header.icmp_hun.ih_idseq.icd_seq);
    } else {
      uint16_t ident = packet->header.icmp_hun.ih_idseq.icd_id;
      struct timespec time = subtract_timespec(&recv_time, &send_time);

      if (packet->header.icmp_hun.ih_idseq.icd_seq == 0) {
        stats.min = time;
      }
      if (compare_timespec(&stats.max, &time) < 0) {
        stats.max = time;
      }
      if (compare_timespec(&stats.min, &time) > 0) {
        stats.min = time;
      }

      accumulate(&stats.total_rtt, time);
      update_stats(&stats, time);
      format_message(buf, ret, time, ident, flags->verbose);
    }

    change_packet(packet);
    stats.total_packages = packet->header.icmp_hun.ih_idseq.icd_seq;

    add_timespec(&next_ping_time, &next_ping_time, &ping_interval);

    struct timespec now, sleep_time;
    clock_gettime(CLOCK_MONOTONIC, &now);
    sleep_time = subtract_timespec(&next_ping_time, &now);

    if (sleep_time.tv_sec >= 0 && sleep_time.tv_nsec >= 0) {
      nanosleep(&sleep_time, NULL);
    } else {
      clock_gettime(CLOCK_MONOTONIC, &next_ping_time);
    }

    if (BONUS && flags->deadline == true) {
      flags->amount_deadline--;
      if (flags->deadline && flags->amount_deadline <= 0) {
        break;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &t_end);

  stats.total_time = subtract_timespec(&t_end, &t_start);

  calculate_average(&stats);

  message_on_quit(ip, stats);
  (void)close(socket_fd);
  free(packet);
  free((char *)ip_str);
  free((t_sockaddr_in *)address);

  free(flags);

  return EXIT_SUCCESS;
}

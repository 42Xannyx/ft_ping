#include <arpa/inet.h>
#include <math.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "flags.h"
#include "ft_ping.h"
#include "payload.h"

#define PING_SLEEP_RATE 1000000 // 1 second

/**
volatile Keyword

The volatile keyword is used in variable declarations to tell the compiler that
the value of the variable can change at any time, without any action being taken
by the code the compiler finds nearby. What this means practically is that the
compiler won't optimize away reads or writes to this variable, ensuring that the
program always checks the current value of the variable at runtime instead of
caching it in a register. This is particularly important for variables that may
be changed by hardware, a different thread, or—in this case—a signal handler.
sig_atomic_t Type

The sig_atomic_t type is an integer type that can be accessed atomically,
meaning that operations on this type of variable are completed in a single step.
This property is crucial for signal handlers, as it guarantees that reading or
writing to a sig_atomic_t variable can't be interrupted by a signal. This makes
sig_atomic_t safe to use for communication between signal handlers and the main
program, ensuring that data isn't corrupted by partial writes or reads.
*/
volatile sig_atomic_t g_ping_loop = true; // Only being used in main.c

void handle_signal(int32_t sig) { g_ping_loop = false; }

void updateStats(t_stats *stats, t_timespec new) {
  double newMs = timespecToMs(new);

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

  const t_flags *flags = initFlags(argv);
  const char *ip = argv[argc - 1];

  t_stats stats;
  t_timespec time = {0, 0}, t_start = {0, 0}, t_end = {0, 0};

  (void)memset(&stats, 0, sizeof(t_stats));

  const int32_t socket_fd = createSocket();
  setSocket(socket_fd, ip);
  const char *ip_str = fetchHostname(ip);

  const t_sockaddr_in *address = setAddress(ip_str);
  int32_t result = inet_pton(AF_INET, ip_str, (void *)&address->sin_addr);

  t_packet *packet = initPacket();
  messageOnStart(ip_str, ip, sizeof(packet->payload));

  while (g_ping_loop) {
    char buf[84]; // ICMP Payload (64 Bytes) + IP Header (20 Bytes)

    ssize_t ret = sendPing(socket_fd, *address, packet, sizeof(*packet));
    getClock(&t_start);

    ret = recvPing(buf, sizeof(buf), socket_fd, *address);
    getClock(&t_end);

    if (ret < 0) {
      (void)fprintf(stderr, "request timeout for icmp_req=%d\n",
                    packet->header.icmp_hun.ih_idseq.icd_seq);
    }

    if (ret >= 0) {
      time = setDuration(t_start, t_end);

      if (packet->header.icmp_hun.ih_idseq.icd_seq == 0) {
        stats.min = time;
      }

      if (stats.max.tv_sec + stats.max.tv_nsec < time.tv_sec + time.tv_nsec) {
        stats.max = time;
      }
      if (stats.min.tv_sec + stats.min.tv_nsec > time.tv_sec + time.tv_nsec) {
        stats.min = time;
      }

      accumulate(&stats.total_rtt, time);
      updateStats(&stats, time);

      formatMessage(buf, ret, time);

      (void)usleep(PING_SLEEP_RATE);
    }

    changePacket(packet);
  }

  stats.total_packages = packet->header.icmp_hun.ih_idseq.icd_seq;
  calculateAverage(&stats);

  messageOnQuit(ip, stats);
  (void)close(socket_fd);
  free(packet);
  free((char *)ip_str);
  free((t_sockaddr_in *)address);

  return EXIT_SUCCESS;
}

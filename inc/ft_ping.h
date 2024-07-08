#ifndef FT_PING_H
#define FT_PING_H

#include "payload.h"

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <time.h>

#define PING_SLEEP_RATE 1000000 // 1 second

#ifndef BONUS
#define BONUS 0
#endif

typedef struct timespec t_timespec;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct addrinfo t_addrinfo;

typedef struct stats {
  n_short total_packages;
  n_short received_packages;

  double sum, sumSquared;

  t_timespec total_rtt;
  t_timespec min, max, avg, stddev;
} t_stats;

// socket.c
int32_t create_socket(void);
void set_socket(const int32_t socket_fd);

ssize_t send_ping(const int32_t socket_fd, const t_sockaddr_in address,
                  const t_packet *packet, size_t);

ssize_t recv_ping(char *, size_t, const int32_t, const t_sockaddr_in);

// imcp.c
t_packet *init_packet();
void change_packet(t_packet *);

// address.c
t_sockaddr_in *set_address(const char *);

// message.c
void message_on_start(const char *, const char *, const ssize_t);
void verbose_message_on_start(const int32_t, const char *);
void format_message(const char *, ssize_t, t_timespec);
void message_on_quit(const char *, const t_stats);

// host.c
__attribute__((warn_unused_result)) const char *fetch_hostname(const char *);

// time.c
t_timespec set_duration(t_timespec t_start, t_timespec t_end);
void get_clock(t_timespec *tv);
void calculate_average(t_stats *stats);

//******* Functions *******//

static inline void accumulate(t_timespec *total, t_timespec new) {
  total->tv_sec += new.tv_sec;
  total->tv_nsec += new.tv_nsec;

  if (total->tv_nsec >= 1000000000) {
    total->tv_sec++;
    total->tv_nsec -= 1000000000;
  }
}

__attribute__((warn_unused_result)) static inline t_timespec
set_time(const int32_t sec, const int32_t nsec) {
  t_timespec tv;

  tv.tv_sec = sec;
  tv.tv_nsec = nsec;
  return tv;
}

__attribute__((warn_unused_result)) static inline double
timespec_to_ms(t_timespec tv) {
  return tv.tv_sec * 1000.0 + tv.tv_nsec / 1000000.0;
}

#endif // FT_PING_H

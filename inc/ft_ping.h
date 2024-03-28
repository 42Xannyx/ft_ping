#ifndef FT_PING_H
#define FT_PING_H

#include "flags.h"
#include "payload.h"
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
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
const int32_t createSocket();
void setSocket(const int32_t socket_fd, const char *inet_address);

const ssize_t sendPing(const int32_t socket_fd, const t_sockaddr_in address,
                       const t_packet *packet, size_t);

const ssize_t recvPing(char *, size_t, const int32_t, const t_sockaddr_in);

// imcp.c
t_packet *initPacket();
void changePacket(t_packet *);

// address.c
t_sockaddr_in *setAddress(const char *);

// message.c
void messageOnStart(const char *, const char *, const ssize_t);
void formatMessage(const char *, ssize_t, t_timespec);
void messageOnQuit(const char *, const t_stats);

// host.c
const char *fetchHostname(const char *);

// time.c
const t_timespec setTime(const int32_t, const int32_t);
const t_timespec setDuration(t_timespec t_start, t_timespec t_end);
void getClock(t_timespec *tv);
void calculateAverage(t_stats *stats);
void accumulate(struct timespec *total, t_timespec new);
double timespecToMs(struct timespec t);

#endif // FT_PING_H

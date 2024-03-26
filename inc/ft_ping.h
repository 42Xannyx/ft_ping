#ifndef FT_PING_H
#define FT_PING_H

#include "payload.h"
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <time.h>

typedef struct stats {
  n_short total_packages;
  n_short received_packages;

  struct timespec min;
  struct timespec avg;
  struct timespec max;
  struct timespec stddev;
} t_stats;

// socket.c
const int32_t createSocket();
void setSocket(const int32_t socket_fd, const char *inet_address);

void sendPing(const int32_t socket_fd, const struct sockaddr_in address,
              const t_packet *packet, size_t);

const ssize_t recvPing(uint8_t *, size_t, const int32_t,
                       const struct sockaddr_in);

// imcp.c
t_packet *initPacket();
void changePacket(t_packet *);

// address.c
const struct sockaddr_in *setAddress(const char *);

// message.c
void messageOnStart(const char *, const char *, const ssize_t);
void formatMessage(const u_int8_t *, ssize_t, struct timespec);
void messageOnQuit(const char *, const t_stats);

// host.c
const char *fetchHostname(const char *);

// time.c
const struct timespec setTime(const int32_t, const int32_t);

#endif // FT_PING_H

#ifndef FT_PING_H
#define FT_PING_H

#include "payload.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_ssize_t.h>

// socket.c
const int32_t createSocket();
const struct sockaddr_in setSocket(const int32_t socket_fd,
                                   const char *inet_address);

void sendPing(const int32_t socket_fd, const struct sockaddr_in address,
              const t_packet *packet);

const ssize_t recvPing(uint8_t *buf, const int32_t socket_fd,
                       const struct sockaddr_in address);

// imcp.c
const struct packet *initPacket();
void changePacket(t_packet *);

// message.c
void messageOnStart(const char *, const char *, const ssize_t);
void formatMessage(const u_int8_t *buf, ssize_t numBytes);
void messageOnQuit();

// host.c
const char *fetchHostname(const char *hostname, struct sockaddr_in);

// time.c
const struct timespec setTime(const int32_t, const int32_t);

#endif // FT_PING_H

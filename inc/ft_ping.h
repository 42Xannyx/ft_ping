#ifndef FT_PING_H
#define FT_PING_H

#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/_types/_ssize_t.h>

// socket.c
const int32_t createSocket();
const struct sockaddr_in setSocket(const int32_t socket_fd,
                                   const char *inet_address);

void sendPing(const int32_t socket_fd, const struct sockaddr_in address,
              const struct icmp *icmp);

ssize_t recvPing(uint8_t *buf, const int32_t socket_fd,
                 const struct sockaddr_in address);

// imcp.c
const struct icmp *getIcmp();

// message.c
void messageOnStart(const char *, const ssize_t);
void formatMessage(const u_int8_t *buf, ssize_t numBytes);
void messageOnQuit();

// host.c
const char *fetchHostname(const char *);

#endif // FT_PING_H

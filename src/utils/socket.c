#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_ssize_t.h>
#include <sys/_types/_u_int8_t.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "ft_ping.h"
#include "payload.h"

#define RECV_TIMEOUT 1

const int32_t createSocket() {
  const int32_t socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  if (socket_fd < 0) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

/*
 * Useful link: https://www.geeksforgeeks.org/ping-in-c/
 */
const struct sockaddr_in setSocket(const int32_t socket_fd,
                                   const char *inet_address) {
  int32_t ttl = 64;
  struct sockaddr_in address;
  struct timeval tv;

  tv.tv_sec = RECV_TIMEOUT;
  tv.tv_usec = 0;

  int32_t ret = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO,
                           (const char *)&tv, sizeof(tv));
  if (ret < 0) {
    perror("setsockopt() error for SO_RCVTIMEO");
    exit(EXIT_FAILURE);
  }

  ret = setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
  if (ret < 0) {
    perror("setsockopt() error for IP_TTL");
    exit(EXIT_FAILURE);
  }

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(inet_address);
  address.sin_port = 0;

  return address;
}

void sendPing(int32_t socket_fd, const struct sockaddr_in address,
              const t_packet *packet) {
  ssize_t ret = sendto(socket_fd, packet, sizeof(t_packet), 0,
                       (struct sockaddr *)&address, sizeof(address));
  if (ret < 0) {
    perror("sendto() error");
    exit(EXIT_FAILURE);
  }
}

const ssize_t recvPing(uint8_t *buf, int32_t socket_fd,
                       const struct sockaddr_in address) {
  u_int32_t addr_len = sizeof(address);
  ssize_t ret = recvfrom(socket_fd, buf, sizeof(buf), MSG_DONTWAIT,
                         (struct sockaddr *)&address, &addr_len);

  if (ret < 0) {
    perror("request timeout");
  }

  return ret;
}

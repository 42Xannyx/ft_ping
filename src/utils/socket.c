#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ft_ping.h"
#include "payload.h"

#define RECV_TIMEOUT 1

int32_t create_socket() {
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
void set_socket(const int32_t socket_fd) {
  int32_t ttl = 64;

  struct timeval tv;

  tv.tv_sec = RECV_TIMEOUT;
  tv.tv_usec = 0;

  int32_t ret = setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
  if (ret < 0) {
    perror("setsockopt() error for IP_TTL");
    exit(EXIT_FAILURE);
  }
  ret = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv,
                   sizeof(tv));
  if (ret < 0) {
    perror("setsockopt() error for SO_RCVTIMEO");
    exit(EXIT_FAILURE);
  }
}

ssize_t send_ping(int32_t socket_fd, const struct sockaddr_in address,
                  const t_packet *packet, size_t n) {

  ssize_t ret = sendto(socket_fd, packet, n - 20, 0,
                       (struct sockaddr *)&address, sizeof(address));
  if (ret < 0) {
    perror("sendto() error");
  }

  return ret;
}

ssize_t recv_ping(char *buf, size_t n, int32_t socket_fd,
                  const t_sockaddr_in address) {
  socklen_t addr_len = sizeof(t_sockaddr_in);
  ssize_t ret =
      recvfrom(socket_fd, buf, n, 0, (struct sockaddr *)&address, &addr_len);

  if (errno != EAGAIN && errno != EWOULDBLOCK && ret < 0) {
    perror("recvfrom() error");
    exit(EXIT_FAILURE);
  }

  return ret;
}

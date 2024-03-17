#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ft_ping.h"

#define PING_SLEEP_RATE 1000000

int32_t main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "%s: I need more arguments\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (getuid() != 0) {
    fprintf(stderr, "%s: This program requires root privileges!\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const int32_t socket_fd = createSocket();
  const struct sockaddr_in address = setSocket(socket_fd, argv[1]);
  const struct icmp *icmp = getIcmp();

  bool ping_loop = true;

  int result = inet_pton(AF_INET, argv[1], (void *)&address.sin_addr);
  messageOnStart("8.8.8.8", 56);
  while (ping_loop) {
    uint8_t *buf = malloc(1024 * sizeof(uint8_t));

    sendPing(socket_fd, address, icmp);
    ssize_t ret = recvPing(buf, socket_fd, address);

    formatMessage(buf, ret);

    usleep(PING_SLEEP_RATE);
    free(buf);
  }

  messageOnQuit();
  close(socket_fd);
  free((struct icmp *)icmp);
  return EXIT_SUCCESS;
}

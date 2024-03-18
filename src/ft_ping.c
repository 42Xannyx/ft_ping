#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ft_ping.h"
#include "payload.h"

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

  bool ping_loop = true;
  const int32_t socket_fd = createSocket();
  const struct sockaddr_in address = setSocket(socket_fd, argv[1]);
  const char *ip_str = fetchHostname(argv[1], address);

  int result = inet_pton(AF_INET, ip_str, (void *)&address.sin_addr);

  const t_packet *packet = initPacket();
  messageOnStart(ip_str, argv[1], sizeof(packet->payload));

  while (ping_loop) {
    changePacket((t_packet *)packet);
    uint8_t *buf = malloc(1024 * sizeof(uint8_t));

    sendPing(socket_fd, address, packet);
    ssize_t ret = recvPing(buf, socket_fd, address);

    formatMessage(buf, ret);

    usleep(PING_SLEEP_RATE);
    free(buf);
  }

  messageOnQuit();
  close(socket_fd);
  free((t_packet *)packet);
  return EXIT_SUCCESS;
}

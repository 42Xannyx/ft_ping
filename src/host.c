#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "ft_ping.h"

const char *fetchHostname(const char *hostname) {
  t_addrinfo hints, *result;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_RAW;

  int32_t ret = getaddrinfo(hostname, NULL, &hints, &result);
  if (ret > 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(1);
  }

  if (result->ai_family != AF_INET) {
    fprintf(stderr, "IPv6 addresses not handled\n");
    freeaddrinfo(result);
    exit(1);
  }

  char *ip_str = malloc(NI_MAXHOST);

  if (!ip_str) {
    freeaddrinfo(result);
    perror("malloc() error");
    exit(1);
  }

  t_sockaddr_in *ipv4 = (t_sockaddr_in *)result->ai_addr;
  inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, NI_MAXHOST);

  freeaddrinfo(result);
  return ip_str;
}

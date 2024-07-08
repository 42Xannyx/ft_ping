#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "ft_ping.h"

const char *fetch_hostname(const char *hostname) {
  t_addrinfo hints, *result;

  (void)memset(&hints, 0, sizeof(hints));

  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_RAW;
  hints.ai_canonname = (char *)hostname;

  int32_t ret = getaddrinfo(hostname, NULL, &hints, &result);
  if (__builtin_expect(ret > 0, 0)) {
    (void)fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  if (__builtin_expect(result->ai_family != AF_INET, AF_INET)) {
    (void)fprintf(stderr, "IPv6 addresses not handled\n");
    exit(EXIT_FAILURE);
  }

  char *ip_str = malloc(NI_MAXHOST);
  if (!ip_str) {
    perror("malloc() error");
    freeaddrinfo(result);
    exit(EXIT_FAILURE);
  }

  t_sockaddr_in *ipv4 = (t_sockaddr_in *)result->ai_addr;
  (void)inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, NI_MAXHOST);

  freeaddrinfo(result);
  return ip_str;
}

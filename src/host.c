#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

const char *fetchHostname(const char *hostname, struct sockaddr_in address) {
  struct addrinfo hints, *result;
  char *ip_str = malloc(NI_MAXHOST);

  if (!ip_str) {
    perror("malloc() error");
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_RAW;

  int32_t ret = getaddrinfo(hostname, NULL, &hints, &result);
  if (ret < 0) {
    perror("getnameinfo() error");
    exit(1);
  }

  if (result->ai_family != AF_INET) {
    fprintf(stderr, "IPv6 addresses not handled in this example.\n");
    free(ip_str);
    freeaddrinfo(result);
    exit(1);
  }

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
  inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, NI_MAXHOST);

  freeaddrinfo(result);
  return ip_str;
}

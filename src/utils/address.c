#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ft_ping.h"

struct sockaddr_in *set_address(const char *inet_address) {
  t_sockaddr_in *address = malloc(sizeof(t_sockaddr_in));

  if (!address) {
    perror("malloc():");
    exit(EXIT_FAILURE);
  }

  (void)memset(address, 0, sizeof(t_sockaddr_in));
  address->sin_family = AF_INET;
  address->sin_addr.s_addr = inet_addr(inet_address);
  address->sin_port = htons(0);

  if (address->sin_addr.s_addr == INADDR_NONE) {
    free(address);

    (void)fprintf(stderr, "%s: Invalid address!\n", "setAddress");
    exit(EXIT_FAILURE);
  }

  return address;
}

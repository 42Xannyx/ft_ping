#include "flags.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void handleVerbose(t_flags *flags, char *data) {
  (void)data;
  flags->verbose = true;
}

void handleVersion(t_flags *flags, char *data) {
  (void)flags;
  (void)data;

  (void)printf("ft_ping from 42Xannyx 20240401\n");
  exit(EXIT_SUCCESS);
}

void handleHelp() {
  (void)printf("Usage: ft_ping [OPTIONS] DESTINATION\n");
  (void)printf("Send ICMP ECHO_REQUEST to network hosts.\n\n");
  (void)printf("Options:\n");
  (void)printf(
      "  -v                 Verbose output. List details for each response.\n");
  (void)printf(
      "  -V                 Display the version of ft_ping and exit.\n");
  (void)printf(
      "  -w deadline        Specify a timeout, in seconds, before ft_ping "
      "exits regardless of how many packets have been sent or received.\n");
  (void)printf("  -?, --help         Display this help and exit.\n\n");
  (void)printf("For more details see ping manual.\n");

  exit(EXIT_SUCCESS);
}

void handleDeadline(t_flags *flags, char *data) {
  flags->deadline = true;

  if (!data) {
    (void)fprintf(stderr, "ft_ping: Missing second argument of flag -w\n");
    exit(EXIT_FAILURE);
  }

  for (int32_t i = 0; data[i]; i++) {
    if (isdigit(data[i]) == false) {
      (void)fprintf(stderr, "ft_ping: Invalid second argument of flag -w\n");
      exit(EXIT_FAILURE);
    }
  }

  long amount = strtol(data, NULL, 10);
  if (errno != 0) {
    perror("strtol()");
    (void)fprintf(stderr, "ft_ping: Invalid second argument of flag -w\n");
    exit(EXIT_FAILURE);
  }

  if (amount == 0) {
    flags->deadline = false;
  }

  flags->flag_data.amount_deadline = amount;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"

void handleVerbose(t_flags *flags) {
  if (flags->verbose == false) {
    (void)printf("Verbose is on\n");
  }
  flags->verbose = true;
}

void handleVersion(t_flags *flags) {
  (void)printf("ft_ping from 42Xannyx 20240328\n");
  exit(EXIT_SUCCESS);
}

void handleFlood(t_flags *flags) {
  if (flags->flood == false) {
    (void)printf("Flood is on\n");
  }
}

void handlePreload(t_flags *flags) {
  if (flags->preload == false) {
    (void)printf("Preload is on\n");
  }
}

static void findFlag(t_flags *flags, char *arg) {
  t_typeFlag flag_mappings[] = {{"-v", handleVerbose},
                                {"-f", handleFlood},
                                {"-V", handleVersion},
                                {"-l", handleVersion},
                                {"-n", handleVersion},
                                {"-w", handleVersion},
                                {"-W", handleVersion},
                                {"-p", handleVersion},
                                {"-r", handleVersion},
                                {"-s", handleVersion},
                                {"-T", handleVersion},
                                {"--ttl", handleVersion},
                                {"--ip-timestamp", handleVersion},
                                {NULL, NULL}};

  for (int32_t i = 0; flag_mappings[i].flag; i++) {
    if (strcmp(arg, flag_mappings[i].flag) == 0) {
      flag_mappings[i].handler(flags);
    }
  }
}

const t_flags *initFlags(char *argv[]) {
  t_flags *flags = malloc(sizeof(t_flags));

  memset(flags, false, sizeof(&flags));

  if (!flags) {
    perror("malloc() error");
    exit(EXIT_FAILURE);
  }

  for (int32_t i = 1; argv[i]; i++) {
    findFlag(flags, argv[i]);
  }

  return flags;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"
#include "ft_ping.h"

static void findFlag(t_flags *flags, char *flag, char *data) {
  t_typeFlag flag_mappings[] = {{"-V", handleVersion},
                                {"-w", handleDeadline},
                                {"-s", handleVersion},
                                {NULL, NULL}};

  for (int32_t i = 0; flag_mappings[i].flag; i++) {
    if (strcmp(flag, "-?") == 0 || strcmp(flag, "--help") == 0) {
      handleHelp();
    }
    if (strcmp(flag, "-v") == 0) {
      handleVerbose(flags, data);
    }

    if (BONUS && strcmp(flag, flag_mappings[i].flag) == 0) {
      flag_mappings[i].handler(flags, data);
    }
  }
}

t_flags *initFlags(int32_t argc, char *argv[]) {
  t_flags *flags = malloc(sizeof(t_flags));

  memset(flags, false, sizeof(&flags));

  if (!flags) {
    perror("malloc() error");
    exit(EXIT_FAILURE);
  }

  for (int32_t i = 1; i < argc; i++) {
    findFlag(flags, argv[i], argv[i + 1]);
  }

  return flags;
}

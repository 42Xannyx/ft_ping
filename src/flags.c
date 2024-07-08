#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"
#include "ft_ping.h"

static void find_flag(t_flags *flags, char *flag, char *data) {
  t_typeFlag flag_mappings[] = {
      {"-V", handle_version}, {"-w", handle_deadline}, {NULL, NULL}};

  for (int32_t i = 0; flag_mappings[i].flag; i++) {
    if (strcmp(flag, "-?") == 0 || strcmp(flag, "--help") == 0) {
      handle_help();
    }
    if (strcmp(flag, "-v") == 0) {
      handle_verbose(flags, data);
    }

    if (BONUS && strcmp(flag, flag_mappings[i].flag) == 0) {
      flag_mappings[i].handler(flags, data);
    }
  }
}

t_flags *init_flags(int32_t argc, char *argv[]) {
  t_flags *flags = malloc(sizeof(t_flags));
  if (!flags) {
    perror("malloc() error");
    exit(EXIT_FAILURE);
  }

  (void)memset(flags, false, sizeof(t_flags));

  for (int32_t i = 1; i < argc; i++) {
    find_flag(flags, argv[i], argv[i + 1]);
  }

  return flags;
}

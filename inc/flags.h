
#ifndef FLAGS_H

#include <netinet/ip_icmp.h>
#include <stdbool.h>
#include <stdint.h>

/* ************** */

/*
 * @summary: All possible flags
 *
 * -v - Verbose
 * -f - Flood
 * -l - preload
 * */
typedef struct flags {
  bool verbose;
  bool flood;
  bool preload;
} t_flags;

#endif // !FLAGS_H

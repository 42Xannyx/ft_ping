
#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>
#include <stdint.h>

/* ************** */

/*
 * @summary: All possible flags
 *
- `-V`: Show version of the ft_ping program.
- `-v`: Verbose output.
- `-f`: Flood ping - `-l`: Preload
- `-n`: Numeric output only. Does not resolve hostnames to addresses.
- `-w`: Deadline
- `-W`: Timeout. Time to wait for a response.
- `-p`: Pattern
- `-r`: Bypass the normal routing tables
- `-s`: Specifies the number of data bytes to be sent.
- `-T`: Timestamp. Specifies the timestamp option for ICMP requests.
- `--ttl`: Time To Live.
- `--ip-timestamp`: Include IP timestamp in the packet.
 * */
typedef struct flags {
  bool verbose;
  bool deadline;

  long amount_deadline;
} t_flags;

typedef void (*flag_handler)(t_flags *, char *);

typedef struct {
  char *flag;
  flag_handler handler;
} t_typeFlag;

t_flags *init_flags(int32_t argc, char *argv[]);

// Functions

void handle_verbose(t_flags *flags, char *data);

void handle_version(t_flags *flags, char *data);

void handle_deadline(t_flags *flags, char *data);

void handle_help();

#endif // !FLAGS_H

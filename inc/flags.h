
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

typedef void (*flagHandler)(t_flags *, char *);

typedef struct {
  char *flag;
  flagHandler handler;
} t_typeFlag;

t_flags *initFlags(int32_t argc, char *argv[]);

// Functions

void handleVerbose(t_flags *flags, char *data);

void handleVersion(t_flags *flags, char *data);

void handleDeadline(t_flags *flags, char *data);

void handleHelp();

#endif // !FLAGS_H

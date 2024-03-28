
#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

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
  bool flood;
  bool preload;
  bool numeric;
  bool deadline;
  bool pattern;
  bool bypass;
  bool package_amount;
  bool ttl;
  bool timestamp;
} t_flags;

typedef void (*flagHandler)(t_flags *);

typedef struct {
  char *flag;
  flagHandler handler;
} t_typeFlag;

void handleFlood(t_flags *flags);
void handlePreload(t_flags *flags);
void handleVerbose(t_flags *flags);
void handleVersion(t_flags *flags);

t_flags *initFlags(char *argv[]);

#endif // !FLAGS_H

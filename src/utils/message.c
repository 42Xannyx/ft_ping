#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "ft_ping.h"

/**
  *Understanding the Byte Lengths

    64 Bytes Expectation: This likely corresponds to the payload size of a
  standard ping message, which does not include the ICMP header, IP header, or
  any Ethernet frame overhead.

    84 Bytes from recvfrom: This includes the ICMP message (which includes both
  the ICMP header and its payload) and the IP header. This does not include the
  Ethernet frame overhead, which is not seen by recvfrom since it operates at a
  higher level in the network stack.

    98 Bytes in Wireshark: Wireshark captures at the network interface level,
  including the Ethernet frame. The Ethernet frame adds at least 14 bytes of
  header (destination MAC, source MAC, and EtherType) plus potentially 4 bytes
  of frame check sequence (FCS) at the end, bringing the total observed in
  Wireshark to at least 98 bytes (84 bytes of IP and ICMP + 14 bytes of Ethernet
  header, and sometimes FCS or other Ethernet-level tags).
  */
void formatMessage(const u_int8_t *buf, ssize_t numBytes, struct timespec tv) {
  struct ip *ip = (struct ip *)buf;
  u_int32_t ip_header_len = ip->ip_hl * 4;

  struct icmp *recv_icmp = (struct icmp *)(buf + ip_header_len);
  ssize_t icmp_payload_len = numBytes - ip_header_len;

  int64_t milliseconds = tv.tv_nsec / 1000000;
  int64_t microseconds = (tv.tv_nsec % 1000000) / 1000;

  if (recv_icmp->icmp_type == ICMP_ECHOREPLY) {
    printf("%zd bytes from %s: icmp_seq=%u ttl=%d time=%lld.%03lld ms\n",
           icmp_payload_len, inet_ntoa(ip->ip_src),
           recv_icmp->icmp_hun.ih_idseq.icd_seq, ip->ip_ttl, milliseconds,
           microseconds);
  }
}

void messageOnStart(const char *ip_str, const char *input,
                    const ssize_t numBytes) {
  printf("FT_PING %s (%s): %zu data bytes\n", input, ip_str, numBytes);
}

void messageOnQuit(const char *input, const t_stats stats) {
  printf("--- %s ping statistics ---\n", input);
  printf("%hu packets transmitted, ", stats.total_packages);
  printf("%hu packets received, ", stats.received_packages);
  printf("%hu %% packet loss\n",
         100 - (stats.received_packages / stats.total_packages) * 100);
  printf("round - trip min / avg / max / stddev = ");
  printf("%ld.%03ld / %ld.%03ld / %ld.%03ld / %ld.%03ld ms", stats.min.tv_nsec,
         stats.min.tv_nsec, stats.avg.tv_nsec, stats.avg.tv_nsec,
         stats.max.tv_nsec, stats.max.tv_nsec, stats.stddev.tv_nsec,
         stats.stddev.tv_nsec);
}

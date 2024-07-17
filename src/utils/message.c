#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

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
void format_message(const char *buf, ssize_t numBytes, t_timespec tv,
                    uint16_t ident, bool verbose) {
  struct ip *ip = (struct ip *)buf;
  u_int32_t ip_header_len = ip->ip_hl * 4;
  struct icmp *recv_icmp = (struct icmp *)(buf + ip_header_len);
  ssize_t icmp_payload_len = numBytes - ip_header_len;
  int64_t milliseconds = tv.tv_nsec / 1000000;
  int64_t microseconds = (tv.tv_nsec % 1000000) / 1000;

  if (recv_icmp->icmp_type != ICMP_ECHOREPLY) {
    return;
  }

  char message[256];
  int len;

  if (verbose) {
    len = snprintf(
        message, sizeof(message),
        "%zd bytes from %s: icmp_seq=%u ident=%d ttl=%d time=%ld.%03ld ms\n",
        icmp_payload_len, inet_ntoa(ip->ip_src),
        recv_icmp->icmp_hun.ih_idseq.icd_seq, ident, ip->ip_ttl, milliseconds,
        microseconds);
  } else {
    len = snprintf(message, sizeof(message),
                   "%zd bytes from %s: icmp_seq=%u ttl=%d time=%ld.%03ld ms\n",
                   icmp_payload_len, inet_ntoa(ip->ip_src),
                   recv_icmp->icmp_hun.ih_idseq.icd_seq, ip->ip_ttl,
                   milliseconds, microseconds);
  }

  if (len > 0 && len < (int)sizeof(message)) {
    write(STDOUT_FILENO, message, len);
  }
}

void verbose_message_on_start(const int32_t socket_fd) {
  (void)printf("ft_ping: sock4.fd: %d (socktype: SOCK_RAW), hints.ai_family: "
               "AF_INET\n\n",
               socket_fd);
}

void message_on_start(const char *ip_str, const char *input,
                      const ssize_t numBytes) {
  (void)printf("FT_PING %s (%s): %zu data bytes\n", input, ip_str, numBytes);
}

void message_on_sigquit(const t_stats stats) {
  double minMs = timespec_to_ms(stats.min);
  double avgMs = timespec_to_ms(stats.avg);
  double maxMs = timespec_to_ms(stats.max);
  double ewmaMs = stats.ewma;
  double packet_loss =
      stats.total_packages > 0
          ? 100.0 * (stats.total_packages - stats.received_packages) /
                stats.total_packages
          : 0.0;

  if (avgMs && ewmaMs) {
    printf("%hu/%hu packets, %.1f loss, min/avg/ewma/max = %.3f/%.3f/"
           "%.3f/%.3f ms\n",
           stats.received_packages, stats.total_packages, packet_loss, minMs,
           avgMs, ewmaMs, maxMs);
  }
}

void message_on_quit(const char *input, const t_stats stats) {
  double minMs = timespec_to_ms(stats.min);
  double avgMs = timespec_to_ms(stats.avg);
  double maxMs = timespec_to_ms(stats.max);
  double stddevMs = timespec_to_ms(stats.stddev);
  double total = timespec_to_ms(stats.total_time);
  double packet_loss =
      stats.total_packages > 0
          ? 100.0 * (stats.total_packages - stats.received_packages) /
                stats.total_packages
          : 0.0;

  (void)printf("--- %s ping statistics ---\n", input);
  (void)printf("%hu packets transmitted, ", stats.total_packages);
  (void)printf("%hu packets received, ", stats.received_packages);
  (void)printf("%.1f%% packet loss, ", packet_loss);
  (void)printf("%ldms time\n", (long)total);

  if (avgMs && stddevMs) {
    (void)printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", minMs,
                 avgMs, maxMs, stddevMs);
  }
}

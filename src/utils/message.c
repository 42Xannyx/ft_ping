#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>

#include "ft_ping.h"

void formatMessage(const u_int8_t *buf, ssize_t numBytes) {
  struct ip *ip = (struct ip *)buf;
  u_int32_t ip_header_len = ip->ip_hl * 4;

  struct icmp *recv_icmp = (struct icmp *)(buf + ip_header_len);

  if (recv_icmp->icmp_type == ICMP_ECHOREPLY) {
    printf("%zd bytes from %s: icmp_seq=%u ttl=%d time=?? ms\n", numBytes * 8,
           inet_ntoa(ip->ip_src), recv_icmp->icmp_hun.ih_idseq.icd_seq,
           ip->ip_ttl);
  }
}

void messageOnStart(const char *ip_str, const char *input,
                    const ssize_t numBytes) {
  printf("FT_PING %s (%s): %zu data bytes\n", ip_str, input, numBytes);
}

void messageOnQuit() {
  printf(
      "--- 8.8.8.8 ping statistics ---\n5 packets transmitted, 5 packets "
      "received, 0.0 %% packet loss\nround - trip min / avg / max / stddev = "
      "16.502 / 18.209 / 20.936 / 1.584 ms");
}

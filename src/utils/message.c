#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <sys/_types/_ssize_t.h>

#include "ft_ping.h"

void formatMessage(const u_int8_t *buf, ssize_t numBytes) {
  struct ip *ip = (struct ip *)buf;
  u_int32_t ip_header_len = ip->ip_hl * 4;

  struct icmp *recv_icmp = (struct icmp *)(buf + ip_header_len);

  if (recv_icmp->icmp_type == ICMP_ECHOREPLY) {
    printf("%zd bytes from %s: icmp_seq=%u ttl=%d time=?? ms\n",
           numBytes * 8,          // ICMP message size, excluding IP header
           inet_ntoa(ip->ip_src), // Source IP address in human-readable form
           recv_icmp->icmp_hun.ih_idseq.icd_seq,
           ip->ip_ttl); // TTL value from the IP header
  }
}

void messageOnStart(const char *ip_address, const ssize_t numBytes) {
  printf("PING %s (%s): %zu data bytes\n", ip_address, ip_address, numBytes);
}

void messageOnQuit() {
  printf("--- 8.8.8.8 ping statistics ---\n5 packets transmitted, 5 packets "
         "received, 0.0 %% packet loss\nround - trip min / avg / max / stddev = "
         "16.502 / 18.209 / 20.936 / 1.584 ms");
}

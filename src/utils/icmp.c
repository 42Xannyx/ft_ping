#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdlib.h>

/**
 *The one's complement operation is used in the checksum calculation to ensure
 *that the receiver can easily verify the integrity of the received data. When
 *the receiver gets the data along with the checksum, it sums everything again,
 *including the received checksum. If the data is intact, this sum will equal a
 *number where all bits are 1s (for an 8-bit sum, it would be 1111 1111). The
 *receiver then takes the one's complement of this sum. Because the one's
 *complement of 1111 1111 is 0000 0000, a result of 0 indicates that the data
 *has likely not been altered during transmission.
 */
unsigned short createChecksum(void *b, int len) {
  unsigned short *buf = b;
  unsigned int sum = 0;
  unsigned short result;

  for (; len > 1; len -= 2) {
    sum += *buf++;
  }
  if (len == 1) {
    sum += *(unsigned char *)buf;
  }
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

const struct icmp *getIcmp() {
  struct icmp *icmp = malloc(sizeof(struct icmp));

  icmp->icmp_type = ICMP_ECHO;
  icmp->icmp_code = 0;
  icmp->icmp_hun.ih_idseq.icd_id = htons(1);
  icmp->icmp_hun.ih_idseq.icd_seq = htons(1);

  icmp->icmp_cksum = createChecksum((uint8_t *)icmp, sizeof(struct icmp));

  return icmp;
}

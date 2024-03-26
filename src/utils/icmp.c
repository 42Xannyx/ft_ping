#include "payload.h"
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
unsigned short createChecksum(void *b, int32_t len) {
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

static void createBody(uint8_t *payload, uint64_t size) {
  if (size >= PAYLOAD_SIZE) {
    fprintf(stderr,
            "Size of payload too big; size. Should be under 56. Current size: "
            "%lu\n",
            (unsigned long)size);
    exit(EXIT_FAILURE);
  }

  for (uint8_t i = 0; i < size; ++i) {
    payload[i] = 0x08 + i;
  }

  payload[size] = '\0';
}

static void createHeader(struct icmp *header) {
  int32_t id = getpid() & 0xffff;

  memset(header, 0, sizeof(struct icmp));

  header->icmp_type = ICMP_ECHO;
  header->icmp_code = 0;
  header->icmp_hun.ih_idseq.icd_id = htons(id);
  header->icmp_hun.ih_idseq.icd_seq = htons(0);
  header->icmp_cksum = 0;
}

void changePacket(t_packet *packet) {
  packet->header.icmp_hun.ih_idseq.icd_seq++;

  packet->header.icmp_cksum = 0;
  packet->header.icmp_cksum = createChecksum((uint16_t *)packet, 64);
}

t_packet *initPacket() {
  t_packet *packet = malloc(sizeof(t_packet));

  memset(packet, 0, sizeof(t_packet));

  createHeader(&packet->header);
  createBody((uint8_t *)&packet->payload, sizeof(packet->payload) - 1);
  packet->header.icmp_cksum = createChecksum((uint16_t *)packet, 64);

  return packet;
}

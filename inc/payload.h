#ifndef PAYLOAD_H
#define PAYLOAD_H

#define ICMP_HEADER_LEN 8
#define PAYLOAD_SIZE 56

#include <netinet/ip_icmp.h>
#include <stdint.h>

/* ************** */

/*
 * @summary: A total of 64 Bytes
 * */
typedef struct packet {
  struct icmp header;
  uint8_t payload[PAYLOAD_SIZE];
} t_packet;

#endif // PAYLOAD_H

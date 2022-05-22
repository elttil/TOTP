//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#include "base32.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t alpha_text(const uint8_t x) {
  if (x >= 'A' && x <= 'Z')
    return (x - 'A');

  return x - '2' + 26;
}

uint32_t alpha_hex(const uint8_t x) {
  if (x >= '0' && x <= '9')
    return (x - '0');

  return (x - 'A' + 10);
}

unsigned char shift_right(unsigned char byte, char offset) {
  if (offset > 0)
    return byte >> offset;

  return byte << -offset;
}

unsigned char shift_left(unsigned char byte, char offset) {
  return shift_right(byte, -offset);
}

int decode_sequence(const unsigned char *coded, unsigned char *plain,
                    int is_hex) {
  plain[0] = '\0';
  for (size_t block = 0; block < 8; block++) {
    int offset = (8 - 5 - (5 * block) % 8);
    int octet = (block * 5) / 8;

    int c = (is_hex) ? (alpha_hex(coded[block])) : (alpha_text(coded[block]));
    if (c < 0)
      return octet;

    plain[octet] |= shift_left(c, offset);
    if (offset < 0)
      plain[octet + 1] = shift_left(c, 8 + offset);
  }
  return 5;
}

size_t base32_decode(const char *encoded, char *plain, int is_hex) {
  size_t n = 0;
  for (size_t i = 0, j = 0;; i += 8, j += 5) {
    size_t a = decode_sequence((unsigned char *)(encoded + i),
                               (unsigned char *)(plain + j), is_hex);

    n += a;
    if (5 > a)
      return n;
  }
}

//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#include "hmac.h"
#include <math.h>

uint64_t reverse_64(uint64_t x) {
  return (
      (((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) |
      (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) |
      (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |
      (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000));
}

uint32_t truncate(unsigned char *bytes, size_t len) {
  size_t offset = bytes[len - 1] & 0xF;
  char *p = (char *)(bytes + offset);
  uint32_t return_value;
  return_value = (p[0] & 0x7F) << 24;
  return_value += (p[1] & 0xFF) << 16;
  return_value += (p[2] & 0xFF) << 8;
  return_value += (p[3] & 0xFF);
  return return_value;
}

uint64_t hotp(unsigned char *key, size_t len, uint64_t counter,
              uint64_t digit) {
  uint8_t digest[SHA1_LEN];
  counter = reverse_64(counter);
  hmac_sha1((void *)&counter, sizeof(uint64_t), key, len, digest);
  uint32_t Sbin = truncate(digest, SHA1_LEN);
  digit = pow(10, digit);
  return (Sbin % digit);
}

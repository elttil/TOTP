//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#include "base32.h"
#include "hotp.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define DEFAULT_DIGIT 6
#define DEFAULT_TIME_DURATION 30

uint64_t get_current_unix_time(void) {
  time_t tloc;
  time(&tloc);
  return tloc;
}

uint64_t totp_raw(unsigned char *key, size_t len, uint64_t current_time,
                  uint64_t digit, uint64_t interval) {
  // Ct = (T - T0)/Tx
  uint64_t Tx = interval;
  uint64_t T = current_time;
  uint64_t T0 = 0;
  uint64_t Ct = (T - T0) / Tx;
  return hotp(key, len, Ct, digit);
}

uint64_t totp(const char *key, uint64_t current_time, uint64_t digit,
              uint64_t interval) {
  size_t len = strlen(key);
  char raw_bytes[len];
  size_t real_length = base32_decode(key, raw_bytes, 0);
  return totp_raw((unsigned char *)raw_bytes, real_length, current_time, digit,
                  interval);
}

void usage(char *path) {
  fprintf(stderr,
          "%s -t [UNIX time] -d [digit] -i [interval] -h(print this message) "
          "[TOTP token]\n",
          path);
}

int main(int argc, char **argv) {
  if (!*argv)
    return 1;

  int offset = 0;

  uint64_t cur_time = get_current_unix_time();
  uint64_t digit = DEFAULT_DIGIT;
  uint64_t interval = DEFAULT_TIME_DURATION;

  int ch;
  for (; - 1 != (ch = getopt(argc, argv, "t:"));) {
    offset++;
    if (optarg)
      offset++;
    switch ((char)ch) {
    case 't':
      cur_time = atoi(optarg);
      break;
    case 'd':
      digit = atoi(optarg);
      break;
    case 'i':
      interval = atoi(optarg);
      break;
    case 'h':
      usage(argv[0]);
      return 0;
    }
  }

  argv += offset;

  char *key = argv[1];
  char buffer[4096] = {0};
  if (!key) {
    read(0, buffer, 4096);
    key = buffer;
  }

  uint64_t token = totp(key, cur_time, digit, interval);
  for(size_t i = digit-1;i > 0;i--)
	if(token < pow(10, i))
		putchar('0');

  printf("%ld\n", token);
  return 0;
}

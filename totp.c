//
// Copyright (C) 2022-2023 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: 0BSD
//
#include "base32.h"
#include "hotp.h"
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
  size_t pad_num = 0;

  // Add padding if needed
  switch (len % 8) {
  case 2:
    pad_num = 6;
    break;
  case 4:
    pad_num = 4;
    break;
  case 5:
    pad_num = 3;
    break;
  case 7:
    pad_num = 2;
    break;
  }

  char padded_key[len + pad_num];
  memcpy(padded_key, key, len);
  memset(padded_key + len, '=', pad_num);
  key = padded_key;

  char raw_bytes[len * 2];
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
  for (; - 1 != (ch = getopt(argc, argv, "t:d:i:h"));) {
    offset++;
    if (optarg)
      offset++;
    switch ((char)ch) {
    case 't':
      cur_time = atoll(optarg);
      break;
    case 'd':
      digit = atoll(optarg);
      break;
    case 'i':
      interval = atoll(optarg);
      break;
    case 'h':
      usage(argv[0]);
      return 0;
    }
  }

  argv += offset;

  char secret[4096] = {0};

  char *path = argv[1];
  int file_fd = 0; // Default is stdin
  if (path) {
    file_fd = open(path, O_RDONLY);
    if (-1 == file_fd) {
      perror("open");
      return 1;
    }
  }

  int rc = read(file_fd, secret, 4096);
  if (-1 == rc) {
    perror("read");
    return 1;
  }
  secret[rc] = '\0';

  uint64_t token = totp(secret, cur_time, digit, interval);
  for (size_t i = digit - 1; i > 0; i--)
    if (token < pow(10, i))
      putchar('0');

  printf("%ld\n", token);
  return 0;
}

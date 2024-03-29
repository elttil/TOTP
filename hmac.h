//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: 0BSD
//
#ifndef HMAC_H
#define HMAC_H
#include "SHA1/sha1.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 64

void hmac_sha1(unsigned char *message, size_t message_len, unsigned char *key,
               size_t key_len, uint8_t output[SHA1_LEN]);
#endif

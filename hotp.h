//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: 0BSD
//
#ifndef HOTP_H
#define HOTP_H
#include <stdint.h>
#include <stddef.h>

uint64_t hotp(unsigned char *key, size_t len, uint64_t counter, uint64_t digit);
#endif

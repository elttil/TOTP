//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: 0BSD
//
#ifndef BASE32_H
#define BASE32_H
#include <stddef.h>
#include <stdint.h>

size_t base32_decode(const char *encoded, char *plain, int is_hex);
#endif

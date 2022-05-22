//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: BSD-2-Clause
//
#ifndef BASE32_H
#define BASE32_H
#include <stdint.h>
#include <stddef.h>

size_t base32_decode(const char *encoded, char *plain, int is_hex);
#endif

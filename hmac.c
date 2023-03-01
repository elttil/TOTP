//
// Copyright (C) 2022 by Anton Kling <anton@kling.gg>
//
// SPDX-License-Identifier: 0BSD
//
#include "hmac.h"
#include "SHA1/sha1.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t reverse_32(uint32_t _value);

void hmac_sha1(unsigned char *message, size_t message_len, unsigned char *key,
               size_t key_len, uint8_t output[SHA1_LEN]) {
  // This function can be presented as follows:
  // H(K XOR opad, H(K XOR ipad, message))
  //
  // H    : SHA1 hashing algorithm.
  // K    : Key that got shrunk/padded to block size.
  // ipad : the byte 0x36 repeated B times
  // opad : the byte 0x5C repeated B times.
  // message : The input.

  char o_key_pad[BLOCK_SIZE];
  char i_key_pad[BLOCK_SIZE];

  // (1) append zeros to the end of K to create a B byte string
  // (e.g., if K is of length 20 bytes and B=64, then K will be
  // appended with 44 zero bytes 0x00)
  unsigned char hashed_key[SHA1_LEN];
  if (key_len > BLOCK_SIZE) {
    SHA1_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, key, key_len);
    SHA1_Final(&ctx, hashed_key);
    key = hashed_key;
    key_len = SHA1_LEN;
  }

  // (2) XOR (bitwise exclusive-OR) the B byte string computed in step
  // (1) with ipad
  // (5) XOR (bitwise exclusive-OR) the B byte string computed in
  // step (1) with opad
  memset(i_key_pad, 0x36, BLOCK_SIZE);
  memset(o_key_pad, 0x5C, BLOCK_SIZE);
  for (size_t i = 0; i < key_len; i++) {
    i_key_pad[i] ^= key[i];
    o_key_pad[i] ^= key[i];
  }

  // (3) append the stream of data 'message' to the B byte string resulting
  //     from step (2)
  // (4) apply H to the stream generated in step (3)
  SHA1_CTX first_ctx;
  SHA1_Init(&first_ctx);
  SHA1_Update(&first_ctx, i_key_pad, BLOCK_SIZE);
  SHA1_Update(&first_ctx, message, message_len);
  SHA1_Final(&first_ctx, output);

  // (6) append the H result from step (4) to the B byte string
  // resulting from step (5)
  // (7) apply H to the stream generated in step (6) and output
  //     the result
  SHA1_CTX second_ctx;
  SHA1_Init(&second_ctx);
  SHA1_Update(&second_ctx, o_key_pad, BLOCK_SIZE);
  SHA1_Update(&second_ctx, output, SHA1_LEN);
  SHA1_Final(&second_ctx, output);
}

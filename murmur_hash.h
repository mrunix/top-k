//
// Created by Michael on 2019/2/23.
//

#ifndef TOP_K_MURMUR_HASH_H
#define TOP_K_MURMUR_HASH_H

#include <cstdint>

// 64-bit hash for 64-bit platforms
uint64_t MurmurHash64A(const void *key, int len, unsigned int seed);

// 64-bit hash for 32-bit platforms
uint64_t MurmurHash64B(const void *key, int len, unsigned int seed);

#endif //TOP_K_MURMUR_HASH_H

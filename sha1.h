#ifndef SHA1_H
#define SHA1_H

#include <cstdint>
#include <cstddef>

struct SHA1_CTX {
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[64];
};

void SHA1Init(SHA1_CTX* ctx);
void SHA1Update(SHA1_CTX* ctx, const uint8_t* data, size_t len);
void SHA1Final(uint8_t hash[20], SHA1_CTX* ctx);

#endif

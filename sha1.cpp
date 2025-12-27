#include "sha1.h"
#include <cstring>

static uint32_t rol(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
}

static void sha1_transform(uint32_t state[5], const uint8_t buffer[64]) {
    uint32_t w[80];

    for (int i = 0; i < 16; i++) {
        w[i] = (buffer[i*4] << 24) |
               (buffer[i*4 + 1] << 16) |
               (buffer[i*4 + 2] << 8) |
               (buffer[i*4 + 3]);
    }
    for (int i = 16; i < 80; i++)
        w[i] = rol(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | (~b & d);
            k = 0x5A827999;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        uint32_t temp = rol(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = rol(b, 30);
        b = a;
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void SHA1Init(SHA1_CTX* ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count = 0;
}

void SHA1Update(SHA1_CTX* ctx, const uint8_t* data, size_t len) {
    size_t i = 0;
    size_t index = (ctx->count >> 3) % 64;
    ctx->count += static_cast<uint64_t>(len) << 3;

    size_t partLen = 64 - index;
    if (len >= partLen) {
        memcpy(&ctx->buffer[index], data, partLen);
        sha1_transform(ctx->state, ctx->buffer);

        for (i = partLen; i + 63 < len; i += 64)
            sha1_transform(ctx->state, &data[i]);

        index = 0;
    }
    memcpy(&ctx->buffer[index], &data[i], len - i);
}

void SHA1Final(uint8_t hash[20], SHA1_CTX* ctx) {
    uint8_t bits[8];
    for (int i = 0; i < 8; i++)
        bits[i] = (ctx->count >> ((7 - i) * 8)) & 0xFF;

    size_t index = (ctx->count >> 3) % 64;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);

    static uint8_t padding[64] = { 0x80 };
    SHA1Update(ctx, padding, padLen);
    SHA1Update(ctx, bits, 8);

    for (int i = 0; i < 20; i++)
        hash[i] = (ctx->state[i / 4] >> ((3 - (i % 4)) * 8)) & 0xFF;
}

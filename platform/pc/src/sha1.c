/* SHA-1, for the checkpoints' hashes (input.c), the same hash
 * tools/boottest.py takes of mGBA's palette RAM, VRAM and OAM. */

#include <string.h>

#include "host_platform.h"

#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static void Block(struct HostSha1 *s, const u8 *p)
{
    u32 w[80];
    u32 a, b, c, d, e;
    s32 i;

    for (i = 0; i < 16; i++) {
        w[i] =
            (u32)p[4 * i] << 24 | (u32)p[4 * i + 1] << 16 | (u32)p[4 * i + 2] << 8 | p[4 * i + 3];
    }
    for (; i < 80; i++) {
        w[i] = ROL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    a = s->h[0];
    b = s->h[1];
    c = s->h[2];
    d = s->h[3];
    e = s->h[4];
    for (i = 0; i < 80; i++) {
        u32 f, k, t;

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
        t = ROL(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = ROL(b, 30);
        b = a;
        a = t;
    }
    s->h[0] += a;
    s->h[1] += b;
    s->h[2] += c;
    s->h[3] += d;
    s->h[4] += e;
}

void HostSha1Init(struct HostSha1 *s)
{
    s->h[0] = 0x67452301;
    s->h[1] = 0xEFCDAB89;
    s->h[2] = 0x98BADCFE;
    s->h[3] = 0x10325476;
    s->h[4] = 0xC3D2E1F0;
    s->length = 0;
    s->used = 0;
}

void HostSha1Update(struct HostSha1 *s, const void *data, u32 n)
{
    const u8 *p = (const u8 *)data;

    s->length += n;
    while (n != 0) {
        u32 k = 64 - s->used;

        if (k > n) {
            k = n;
        }
        memcpy(s->block + s->used, p, k);
        s->used += k;
        p += k;
        n -= k;
        if (s->used == 64) {
            Block(s, s->block);
            s->used = 0;
        }
    }
}

void HostSha1Final(struct HostSha1 *s, u8 digest[20])
{
    u64 bits = s->length * 8;
    u8 pad = 0x80;
    u8 len[8];
    s32 i;

    HostSha1Update(s, &pad, 1);
    pad = 0;
    while (s->used != 56) {
        HostSha1Update(s, &pad, 1);
    }
    for (i = 0; i < 8; i++) {
        len[i] = (u8)(bits >> (56 - 8 * i));
    }
    HostSha1Update(s, len, 8);
    for (i = 0; i < 20; i++) {
        digest[i] = (u8)(s->h[i / 4] >> (24 - 8 * (i % 4)));
    }
}

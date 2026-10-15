/* A PNG writer for the screenshots (--screens, the window's screenshot
 * key) with no library: 8-bit RGB, each row unfiltered, the image data in
 * zlib's stored (uncompressed) blocks. 240x160 is 115 KB; tools/boottest.py
 * reads them with PIL, which can re-save them compressed. */

#include <stdio.h>
#include <string.h>

#include "host_platform.h"

static u32 sCrcTable[256];

static void CrcInit(void)
{
    u32 n;
    u32 k;

    if (sCrcTable[1] != 0) {
        return;
    }
    for (n = 0; n < 256; n++) {
        u32 c = n;

        for (k = 0; k < 8; k++) {
            c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        }
        sCrcTable[n] = c;
    }
}

static u32 Crc(u32 crc, const u8 *p, u32 n)
{
    crc = ~crc;
    while (n--) {
        crc = sCrcTable[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

static void Be32(u8 *p, u32 v)
{
    p[0] = v >> 24;
    p[1] = v >> 16;
    p[2] = v >> 8;
    p[3] = v;
}

/* A chunk: length, type, data, the CRC of type and data. */
struct Chunk {
    FILE *f;
    u32 crc;
};

static void ChunkStart(struct Chunk *c, FILE *f, const char *type, u32 length)
{
    u8 head[8];

    Be32(head, length);
    memcpy(head + 4, type, 4);
    fwrite(head, 1, 8, f);
    c->f = f;
    c->crc = Crc(0, head + 4, 4);
}

static void ChunkData(struct Chunk *c, const void *data, u32 n)
{
    fwrite(data, 1, n, c->f);
    c->crc = Crc(c->crc, data, n);
}

static void ChunkEnd(struct Chunk *c)
{
    u8 crc[4];

    Be32(crc, c->crc);
    fwrite(crc, 1, 4, c->f);
}

int HostWritePng(const char *path, const u32 *xrgb, u32 width, u32 height)
{
    static const u8 signature[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
    u32 rowBytes = 1 + 3 * width;
    u32 raw = rowBytes * height;
    /* One stored block per row (a row is at most 65535 bytes). */
    u32 idat = 2 + raw + 5 * height + 4;
    u8 row[1 + 3 * 1024];
    u8 ihdr[13];
    u32 a = 1;
    u32 b = 0;
    struct Chunk c;
    FILE *f;
    u32 x;
    u32 y;
    u32 i;

    if (width > 1024) {
        return 0;
    }
    f = fopen(path, "wb");
    if (f == NULL) {
        return 0;
    }
    CrcInit();
    fwrite(signature, 1, 8, f);

    Be32(ihdr, width);
    Be32(ihdr + 4, height);
    ihdr[8] = 8;  /* bits per channel */
    ihdr[9] = 2;  /* RGB */
    ihdr[10] = 0; /* deflate */
    ihdr[11] = 0; /* adaptive filtering (each row's filter byte: none) */
    ihdr[12] = 0; /* not interlaced */
    ChunkStart(&c, f, "IHDR", 13);
    ChunkData(&c, ihdr, 13);
    ChunkEnd(&c);

    ChunkStart(&c, f, "IDAT", idat);
    ChunkData(&c, "\x78\x01", 2);
    for (y = 0; y < height; y++) {
        u8 block[5];

        row[0] = 0;
        for (x = 0; x < width; x++) {
            u32 p = xrgb[y * width + x];

            row[1 + 3 * x] = p >> 16;
            row[2 + 3 * x] = p >> 8;
            row[3 + 3 * x] = p;
        }
        block[0] = y == height - 1;
        block[1] = rowBytes & 0xFF;
        block[2] = rowBytes >> 8;
        block[3] = ~rowBytes & 0xFF;
        block[4] = (~rowBytes >> 8) & 0xFF;
        ChunkData(&c, block, 5);
        ChunkData(&c, row, rowBytes);
        for (i = 0; i < rowBytes; i++) {
            a = (a + row[i]) % 65521;
            b = (b + a) % 65521;
        }
    }
    {
        u8 adler[4];

        Be32(adler, b << 16 | a);
        ChunkData(&c, adler, 4);
    }
    ChunkEnd(&c);

    ChunkStart(&c, f, "IEND", 0);
    ChunkEnd(&c);
    return fclose(f) == 0;
}

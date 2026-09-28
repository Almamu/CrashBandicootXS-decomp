#include "core.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

extern void *gUnknown_03001308;

extern void sub_8024DFC(void *self, void *vec2);
extern void sub_8024E24(void *self, void *vec2);
extern void sub_8024C64(void *self, void *source);
extern void sub_8024CF0(void *self, void *source);

/* A viewport/parallax-scroll-layer object (docs/rom_map.md's "Visual
 * scrolling background streamer" family is the sibling system built on
 * the same source-descriptor shape - see `sub_8024AA0` there). Its full
 * field layout isn't given a named struct here: several of its fields
 * are only ever written/read by neighboring functions
 * (`sub_8024DFC`/`sub_8024E24`/`sub_8024C64`/`sub_8024CF0`) that sit
 * just before this chunk in ROM order and are still raw asm (out of
 * scope for this issue), so committing a struct now risks getting a
 * field wrong that only shows up once those are matched. */

/* Applies the layer's scale-then-clamp step to `vec2` and accumulates
 * the (Q8, floor-divided) result into the layer's own position. */
void sub_8024E68(void *self, void *vec2)
{
    s32 scaled[2];
    s32 x = ((s32 *)vec2)[0];
    s32 y = ((s32 *)vec2)[1];

    scaled[0] = x;
    scaled[1] = y;
    sub_8024DFC(self, scaled);
    sub_8024E24(self, scaled);
}

/* Same shape as `sub_8024E68`, but seeds the scale step directly from
 * `vec2` in place (no separate stack copy) and finishes by re-deriving
 * the layer's cached-tile buffers from `self->0x2c` instead of
 * accumulating a position. */
void sub_8024E90(void *self, void *vec2)
{
    s32 x = ((s32 *)vec2)[0];
    s32 y = ((s32 *)vec2)[1];

    ((s32 *)self)[0] = x;
    ((s32 *)self)[1] = y;
    sub_8024DFC(self, self);
    sub_8024C64(*(void **)((u8 *)self + 0x2c), self);
}

/* (Re)initializes the layer from `source` (a level/room descriptor -
 * see the comment above): caches its pixel dimensions/scroll bounds,
 * resets the accumulated position to the origin, and re-populates the
 * `self->0x2c` tile-cache sub-object from the same descriptor. Does
 * nothing (besides clearing the ready flag) when `source` is NULL. */
void sub_8024EB4(void *self, void *source)
{
    u8 *readyFlag;
    s32 zero;

    readyFlag = (u8 *)self + 0x28;
    zero = 0;
    *readyFlag = zero;

    if (source != NULL) {
        s32 w, h;

        w = *(u16 *)((u8 *)source + 0x1a);
        *(s32 *)((u8 *)self + 0x18) = w;
        h = *(u16 *)((u8 *)source + 0x1c);
        *(s32 *)((u8 *)self + 0x1c) = h;

        w <<= 3;
        *(s32 *)((u8 *)self + 0x10) = w;
        h <<= 3;
        *(s32 *)((u8 *)self + 0x14) = h;
        w -= 0xf0;
        *(s32 *)((u8 *)self + 8) = w;
        h -= 0xa0;
        *(s32 *)((u8 *)self + 0xc) = h;
        *(s32 *)((u8 *)self + 0x20) = *(s32 *)((u8 *)source + 0xc);
        *(s32 *)((u8 *)self + 0x24) = *(s32 *)((u8 *)source + 0x10);
        *(s32 *)self = zero;
        *(s32 *)((u8 *)self + 4) = zero;

        sub_8024CF0(*(void **)((u8 *)self + 0x2c), source);
        sub_8024C64(*(void **)((u8 *)self + 0x2c), self);

        *readyFlag = 1;
    }
}

u8 sub_8024F04(void *self)
{
    return *((u8 *)self + 0x28);
}

s32 sub_8024F0C(void *self)
{
    return *(s32 *)((u8 *)self + 4);
}

s32 sub_8024F10(void *self)
{
    return *(s32 *)self;
}

s32 sub_8024F14(void *self)
{
    return *(s32 *)((u8 *)self + 0x1c);
}

s32 sub_8024F18(void *self)
{
    return *(s32 *)((u8 *)self + 0x18);
}

s32 sub_8024F1C(void *self)
{
    return *(s32 *)((u8 *)self + 0x14);
}

s32 sub_8024F20(void *self)
{
    return *(s32 *)((u8 *)self + 0x10);
}

/* The 16-slot decode/LRU tile-record cache used throughout this cluster
 * of files (`game_loop3.c`/`game_loop4.c`/`game_loop5.c`; docs/rom_map.md's
 * "Collision/terrain-map streamer" / "`sub_8024F24` (16-slot LRU
 * cache/decode dispatcher)"). `id[N]` holds the record ID currently
 * decoded into the matching 256-byte `buf[N]` slot; `nextSlot` is the
 * ring-buffer eviction cursor this function advances every time it
 * decodes a new record (evicting slot `(nextSlot - 1) & 0xf`, i.e. the
 * slot filled just before the current cursor position). The descriptor
 * this cache is built from (`source` below, populated by `sub_80254F8`
 * in game_loop5.c) is kept as raw offsets rather than its own struct -
 * it's never allocated by any function in this cluster, so its full
 * shape isn't confirmed enough to commit to one. This definition is
 * duplicated (not shared via a header) in game_loop4.c/game_loop5.c -
 * keep them in sync if this layout ever needs revising. */
struct tile_cache {
    void *source;      /* 0x000 */
    void *decodeBase;  /* 0x004 - gUnknown_03001308's camera offset + source->4; sub_8025334's decode-table base */
    s32 unk008;         /* 0x008 - source->0x1a << 3; not read anywhere in this cluster */
    s32 unk00c;          /* 0x00c - source->0x1c << 3; not read anywhere in this cluster */
    s32 unk010;           /* 0x010 - copy of source->0x1a; not read anywhere in this cluster */
    s32 unk014;            /* 0x014 - copy of source->0x1c; not read anywhere in this cluster */
    s32 width;               /* 0x018 - tiles, copy of source->0x16 */
    s32 height;                /* 0x01c - tiles, copy of source->0x18; not read anywhere in this cluster */
    u8 buf[16][0x100];           /* 0x020 - 0x1020, 16 decoded 256-byte chunks */
    s32 id[16];                    /* 0x1020 - 0x105c, record IDs resident in `buf` */
    s32 nextSlot;                    /* 0x1060 */
};

extern void sub_8025334(struct tile_cache *self, s32 recordId, void *dest);

/* Returns the cache slot holding decoded record recordId. On a miss,
 * decodes it (sub_8025334) into the slot just behind the ring cursor and
 * advances the cursor. */
void *sub_8024F24(struct tile_cache *self, s32 recordId)
{
    if (recordId == self->id[0])
        return self->buf[0];
    if (recordId == self->id[1])
        return self->buf[1];
    if (recordId == self->id[2])
        return self->buf[2];
    if (recordId == self->id[3])
        return self->buf[3];
    if (recordId == self->id[4])
        return self->buf[4];
    if (recordId == self->id[5])
        return self->buf[5];
    if (recordId == self->id[6])
        return self->buf[6];
    if (recordId == self->id[7])
        return self->buf[7];
    if (recordId == self->id[8])
        return self->buf[8];
    if (recordId == self->id[9])
        return self->buf[9];
    if (recordId == self->id[10])
        return self->buf[10];
    if (recordId == self->id[11])
        return self->buf[11];
    if (recordId == self->id[12])
        return self->buf[12];
    if (recordId == self->id[13])
        return self->buf[13];
    if (recordId == self->id[14])
        return self->buf[14];
    if (recordId == self->id[15])
        return self->buf[15];
    {
        s32 slot = (self->nextSlot + 15) & 0xf;
        u8 *dest = self->buf[slot];

        sub_8025334(self, recordId, dest);
        self->id[slot] = recordId;
        self->nextSlot = (self->nextSlot + 1) & 0xf;
        return dest;
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r1` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

extern u8 gStaticData_081725AC[];

/* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte cache
 * slot per tile record. */
static inline u16 GetCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    u16 *buf = sub_8024F24(self, (*(u16 **)self->source)[tileY * self->width + tileX]);
    return buf[(y & 7) * 16 + (x & 0xf)];
}

/* The terrain-property row (36 bytes, gStaticData_081725AC) for the cell at
 * pixel (x, y), or NULL when out of bounds or the type is 0 or above 0x23.
 * The cell's flag nibble goes to a local nothing reads. */
void *sub_80250BC(struct tile_cache *self, s32 x, s32 y)
{
    u8 hi;
    u8 *hiOut = &hi;
    u16 cell;
    u8 nibble;
    s32 type;

    if (x < 0 || y < 0)
        return NULL;
    cell = GetCell(self, x, y);
    nibble = (cell >> 8) & 0xf;
    if (nibble)
        *hiOut = nibble;
    type = cell & 0xff;
    if (type == 0 || type > 0x23)
        return NULL;
    return gStaticData_081725AC + type * 36;
}

extern u8 gStaticData_081725B4[];
extern u8 gStaticData_081725BC[];
extern u8 gStaticData_081725C4[];

/* Like sub_80250BC with a collision mode (0-3): each mode has its own
 * property table and its own "not solid" bit in the cell's top nibble.
 * The flag nibble is written to flagsOut. */
void *sub_8025130(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    void *result = NULL;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else
    {
        u16 cell = GetCell(self, x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return NULL;
    switch (mode)
    {
    case 0:
        if (hi & 4)
            result = NULL;
        else
            result = gStaticData_081725AC + type * 36;
        break;
    case 1:
        if (hi & mode)
            result = NULL;
        else
            result = gStaticData_081725B4 + type * 36;
        break;
    case 2:
        if (hi & 8)
            result = NULL;
        else
            result = gStaticData_081725BC + type * 36;
        break;
    case 3:
        if (hi & 2)
            result = NULL;
        else
            result = gStaticData_081725C4 + type * 36;
        break;
    }
    return result;
}

struct terrain_type
{
    u8 modeValue[4];
    u8 unk_04[0x20];
};

extern struct terrain_type gStaticData_081725A8[];

/* The mode byte (0-3) of the cell's terrain type at pixel (x, y): -1
 * when out of bounds or the type is 0x23 or below, 0 when the mode's
 * "not solid" bit is set in the cell's top nibble. */
s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    s8 result = 0;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else
    {
        u16 cell = GetCell(self, x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return -1;
    switch (mode)
    {
    case 0:
        if (hi & 4)
            result = 0;
        else
            result = gStaticData_081725A8[type].modeValue[0];
        break;
    case 1:
        if (hi & mode)
            result = 0;
        else
            result = gStaticData_081725A8[type].modeValue[1];
        break;
    case 2:
        if (hi & 8)
            result = 0;
        else
            result = gStaticData_081725A8[type].modeValue[2];
        break;
    case 3:
        if (hi & 2)
            result = 0;
        else
            result = gStaticData_081725A8[type].modeValue[3];
        break;
    }
    return result;
}

/* Custom RLE/delta token-stream decoder (docs/rom_map.md's "A new find:
 * a custom RLE/delta token-stream decoder"): looks up a base pointer
 * via `self->decodeBase` indexed by `recordId`, then decodes a token
 * stream, budget-limited to 0x7f halfwords, with three run modes per
 * token byte - a literal-fill run, a signed-delta-accumulate run, and a
 * raw-copy run - writing the decoded halfwords into `dest` (a linear
 * 256-byte cache slot in `sub_8024F24`'s caller).
 *
 * Matched (near-miss sweep 2, old_agbcc). Three pieces closed the old
 * 33-halfword gap:
 * - `src` is first loaded with `decodeBase` itself and then advanced by
 *   the record's word offset, so the base lives in `src`'s register (r6)
 *   rather than a scratch one.
 * - The delta run's sign extensions are spelled as explicit `<< 24` /
 *   `<< 16` shifts into an `s32` local, then `acc` is copied into an `s32`
 *   before the `>> 24`. That makes gcc emit the pair's left shift, then
 *   `acc`'s own `lsl/asr #16`, then the pair's `asr #24`, which is the
 *   ROM's interleaving; `(s8)pair` emits the two shifts back to back.
 * - `asm("" : : "r"(n))` after `n -= 2` (an extra-reference nudge that
 *   emits no code, see #468) gives `n` one more reference, so the pair
 *   loop's run counter wins r3 and `acc` keeps r4. Without it the
 *   allocator swaps the two. */
void sub_8025334(struct tile_cache *self, s32 recordId, void *dest)
{
    u16 *out = dest;
    u16 *src = self->decodeBase;
    s32 budget;
    s32 written;

    src = (u16 *)((u32 *)src + src[recordId]);
    budget = 0x7F;
    written = 0;

    do
    {
        u16 token = *src;
        u16 n = *(u8 *)src;

        src++;
        if (token & 0x8000)
        {
            u16 value = *src++;

            budget -= n;
            do
            {
                out[written] = value;
                written++;
                n--;
            } while (n != 0);
        }
        else if (token & 0x4000)
        {
            s16 acc;

            budget -= n;
            acc = *src++;
            out[written] = acc;
            n--;
            written++;
            do
            {
                u16 pair = *src++;

                {
                    s32 lo = pair << 24;
                    s32 a = acc;

                    acc = a + (lo >> 24);
                }
                out[written++] = acc;
                {
                    s32 hi = pair << 16;
                    s32 a = acc;

                    acc = a + (hi >> 24);
                }
                out[written++] = acc;
                n -= 2;
                asm("" : : "r"(n));
            } while (n > 1);
            if (n != 0)
            {
                u16 last = *src++;
                s32 lo = last << 24;
                s32 a = acc;

                out[written] = a + (lo >> 24);
                written++;
            }
        }
        else
        {
            budget -= n;
            do
            {
                out[written] = *src++;
                written++;
                n--;
            } while (n != 0);
        }
    } while (budget >= 0);
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

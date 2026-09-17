#include "core.h"

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

#if NON_MATCHING
/* Looks up (or decodes-and-inserts) the cache slot for `recordId`.
 *
 * NOT YET BYTE-MATCHING: semantics/control-flow/branch topology and the
 * shared per-case tail (an `offset` computed once, added to `self`
 * right before the shared epilogue - mirroring the ROM's own
 * `_080250AC`/`_080250AE` shared block) are all confirmed correct and
 * produce the right total size, but this compiler's register allocator
 * picks the opposite of the ROM's `self`/`recordId` <-> `r7`/`r3`
 * assignment throughout (every comparison/address-add byte differs as
 * a result, even though the instruction shapes match one for one).
 * Explicit `register ... asm("r7")`/`asm("r3")` pins on either variable
 * were tried and made things worse (the compiler falls back to
 * `sp`-relative addressing instead of using the pinned register as a
 * base pointer at all) - parked with the naturally-allocated version
 * instead. See docs/matching/issue-40-terrain-tile-cache.md. */
void *sub_8024F24(struct tile_cache *self, s32 recordId)
{
    s32 offset;

    if (recordId == self->id[0]) {
        return self->buf[0];
    }
    if (recordId == self->id[1]) {
        offset = 0x120;
    } else if (recordId == self->id[2]) {
        offset = 0x220;
    } else if (recordId == self->id[3]) {
        offset = 0x320;
    } else if (recordId == self->id[4]) {
        offset = 0x420;
    } else if (recordId == self->id[5]) {
        offset = 0x520;
    } else if (recordId == self->id[6]) {
        offset = 0x620;
    } else if (recordId == self->id[7]) {
        offset = 0x720;
    } else if (recordId == self->id[8]) {
        offset = 0x820;
    } else if (recordId == self->id[9]) {
        offset = 0x920;
    } else if (recordId == self->id[10]) {
        offset = 0xa20;
    } else if (recordId == self->id[11]) {
        offset = 0xb20;
    } else if (recordId == self->id[12]) {
        offset = 0xc20;
    } else if (recordId == self->id[13]) {
        offset = 0xd20;
    } else if (recordId == self->id[14]) {
        offset = 0xe20;
    } else if (recordId != self->id[15]) {
        s32 slot = (self->nextSlot + 15) & 0xf;
        void *dest = self->buf[slot];

        sub_8025334(self, recordId, dest);
        self->id[slot] = recordId;
        self->nextSlot = (self->nextSlot + 1) & 0xf;
        return dest;
    } else {
        offset = 0xf20;
    }
    return (u8 *)self + offset;
}
#endif

extern u8 gStaticData_081725AC[];

#if NON_MATCHING
/* `x`/`y` in pixels, 16x8px collision-tile granularity. Looks up the
 * decoded tile record for `(x>>4, y>>3)` and, unless out of bounds,
 * returns a pointer into the 36-byte-stride terrain-property table
 * (`gStaticData_081725AC`) for the tile's low-byte type index. `unused`
 * mirrors the (caller-supplied, out-parameter) high-nibble flag write
 * `sub_8025130`'s near-identical body makes - the ROM keeps the
 * computation and store here too even though nothing reads it back.
 *
 * NOT YET BYTE-MATCHING: same shape/register-allocation gap as
 * `sub_8024F24` above - see docs/matching/issue-40-terrain-tile-cache.md. */
void *sub_80250BC(struct tile_cache *self, s32 x, s32 y)
{
    volatile u8 unused;
    volatile u8 *unusedPtr = &unused;
    s32 tileX, tileY, tileIdx;
    void *src;
    u16 recordId;
    u16 *cache;
    u32 wide;
    u16 cell;
    u8 nibble, type;

    if (x < 0 || y < 0) {
        return NULL;
    }

    tileX = x >> 4;
    tileY = y >> 3;
    src = self->source;
    tileIdx = tileY * self->width + tileX;
    recordId = (*(u16 **)src)[tileIdx];
    cache = sub_8024F24(self, recordId);
    wide = cache[((y & 7) << 4) + (x & 0xf)];
    wide <<= 16;
    cell = wide >> 16;

    nibble = (wide >> 0x18) & 0xf;
    if (nibble != 0) {
        *unusedPtr = nibble;
    }

    type = cell & 0xff;
    if (type == 0) {
        return NULL;
    }
    if (type > 0x23) {
        return NULL;
    }
    return gStaticData_081725AC + type * 36;
}
#endif

extern u8 gStaticData_081725B4[];
extern u8 gStaticData_081725BC[];
extern u8 gStaticData_081725C4[];

#if NON_MATCHING
/* Sibling of `sub_80250BC`, adding an output flag-nibble pointer
 * (`flagsOut`) and a `mode`-selected terrain-property table (mirroring
 * `sub_8025228`'s mode dispatch, but each mode's flag test differs and
 * the return is the full table-row pointer rather than a single
 * byte).
 *
 * NOT YET BYTE-MATCHING: semantics/field offsets/table selection/branch
 * topology all confirmed correct (control flow matches instruction for
 * instruction), but this compiler's register allocation assigns
 * `self`/`x`/`y`/`mode` to different callee-saved registers than the
 * ROM (which packs them as r5/r4/r6/r7; several phrasings tried here
 * land on other permutations instead) - a purely cosmetic difference
 * that still changes every encoded byte. See
 * docs/matching/issue-40-terrain-tile-cache.md. */
void *sub_8025130(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    s32 tileX, tileY, tileIdx;
    void *src;
    u16 recordId;
    u16 *cache;
    u32 wide;
    u16 cell;
    u8 nibble, type;
    s32 hi;

    if (x < 0 || y < 0) {
        return NULL;
    }

    tileX = x >> 4;
    tileY = y >> 3;
    src = self->source;
    tileIdx = tileY * self->width + tileX;
    recordId = (*(u16 **)src)[tileIdx];
    cache = sub_8024F24(self, recordId);
    wide = cache[((y & 7) << 4) + (x & 0xf)];
    wide <<= 16;
    cell = wide >> 16;

    hi = wide >> 0x1c;
    nibble = (wide >> 0x18) & 0xf;
    if (nibble != 0) {
        *flagsOut = nibble;
    }

    type = cell & 0xff;
    if (type <= 0x23) {
        return NULL;
    }

    switch (mode) {
    case 0:
        if (hi & 4) {
            return NULL;
        }
        return gStaticData_081725AC + type * 36;
    case 1:
        if (hi & mode) {
            return NULL;
        }
        return gStaticData_081725B4 + type * 36;
    case 2:
        if (hi & 8) {
            return NULL;
        }
        return gStaticData_081725BC + type * 36;
    case 3:
        if (hi & 2) {
            return NULL;
        }
        return gStaticData_081725C4 + type * 36;
    default:
        return NULL;
    }
}
#endif

extern u8 gStaticData_081725A8[];

#if NON_MATCHING
/* The `CheckTerrainFlag(x, y, mode)`-style API docs/rom_map.md
 * identified: re-derives the same tile lookup as `sub_80250BC`, then
 * returns one of 4 adjacent flag bytes from `gStaticData_081725A8`
 * depending on `mode`, each independently bounds-gated by its own bit
 * of the cell's high nibble.
 *
 * NOT YET BYTE-MATCHING: same register-allocation-permutation gap as
 * `sub_8025130` above - see
 * docs/matching/issue-40-terrain-tile-cache.md. */
s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    s32 tileX, tileY, tileIdx;
    void *src;
    u16 recordId;
    u16 *cache;
    u16 cell;
    u8 nibble, type;
    s32 hi;

    if (x < 0 || y < 0) {
        return -1;
    }

    tileX = x >> 4;
    tileY = y >> 3;
    src = self->source;
    tileIdx = tileY * self->width + tileX;
    recordId = (*(u16 **)src)[tileIdx];
    cache = sub_8024F24(self, recordId);
    cell = cache[((y & 7) << 4) + (x & 0xf)];

    hi = cell >> 12;
    nibble = (cell >> 8) & 0xf;
    if (nibble != 0) {
        *flagsOut = nibble;
    }

    type = cell & 0xff;
    if (type <= 0x23) {
        return -1;
    }

    switch (mode) {
    case 0:
        if (hi & 4) {
            return 0;
        }
        return gStaticData_081725A8[type * 36];
    case 1:
        if (hi & mode) {
            return 0;
        }
        return gStaticData_081725A8[type * 36 + 1];
    case 2:
        if (hi & 8) {
            return 0;
        }
        return gStaticData_081725A8[type * 36 + 2];
    case 3:
        if (hi & 2) {
            return 0;
        }
        return gStaticData_081725A8[type * 36 + 3];
    default:
        return 0;
    }
}
#endif

#if NON_MATCHING
/* Custom RLE/delta token-stream decoder (docs/rom_map.md's "A new find:
 * a custom RLE/delta token-stream decoder"): looks up a base pointer
 * via `self->decodeBase` indexed by `recordId`, then decodes a token
 * stream, budget-limited to 0x7f halfwords, with three run modes per
 * token byte - a literal-fill run, a signed-delta-accumulate run, and a
 * raw-copy run - writing the decoded halfwords into `dest` (a linear
 * 256-byte cache slot in `sub_8024F24`'s caller).
 *
 * NOT YET BYTE-MATCHING: the decode loop mechanics (read byte-for-byte
 * from the ROM) are all reproduced and the control flow matches, but
 * the ROM keeps a "written" byte offset alive across the whole function
 * in `r7` (used directly as a write pointer in two of the three token
 * modes) and does not hoist the repeated `0x8000`/`0x4000` bit-test
 * masks out of the loop, while this compiler's natural allocation for
 * the equivalent index-based reconstruction below picks a different
 * register shape (an extra cached mask register, a plain incrementing
 * index instead of a shared pointer). Not yet found a phrasing that
 * reproduces the ROM's exact register/instruction shape here. See
 * docs/matching/issue-40-terrain-tile-cache.md. */
void sub_8025334(struct tile_cache *self, s32 recordId, void *dest)
{
    u16 *table = self->decodeBase;
    u8 *src = (u8 *)table + table[recordId] * 4;
    s32 budget = 0x7f;
    s32 written = 0;
    u16 *out = dest;
    u16 accum;

    do {
        u16 header = *(u16 *)src;
        u8 count = *src;
        src += 2;

        if (header & 0x8000) {
            u16 literal = *(u16 *)src;
            src += 2;
            budget -= count;
            do {
                out[written] = literal;
                written++;
                count--;
            } while (count != 0);
        } else if (header & 0x4000) {
            budget -= count;
            accum = *(u16 *)src;
            src += 2;
            out[written] = accum;
            written++;
            count--;
            while (count > 1) {
                s8 *deltas = (s8 *)src;
                src += 2;
                accum += deltas[0];
                out[written] = accum;
                written++;
                accum += deltas[1];
                out[written] = accum;
                written++;
                count -= 2;
            }
            if (count != 0) {
                s8 delta = *(s8 *)src;
                src += 2;
                accum += delta;
                out[written] = accum;
                written++;
            }
        } else {
            budget -= count;
            do {
                out[written] = *(u16 *)src;
                src += 2;
                written++;
                count--;
            } while (count != 0);
        }
    } while (budget >= 0);
}
#endif

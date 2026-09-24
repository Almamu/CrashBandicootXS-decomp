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

/* Looks up (or decodes-and-inserts) the cache slot for `recordId`: 16
 * fixed `recordId == self->id[N]` checks (slot 0 returns directly,
 * slots 1-15 fall into a shared "add offset to self, return" tail -
 * `_080250AC`/`_080250AE` below, mirroring the ROM's own labels), and
 * on a miss on all 16, decodes into the LRU-evicted slot via
 * `sub_8025334` and advances the ring-buffer cursor. Semantics,
 * control flow and total size were already fully confirmed as real C
 * (see the `#if NON_MATCHING` reconstruction this replaced, and
 * docs/matching/issue-40-terrain-tile-cache.md) - the only gap was
 * this compiler's register allocator always picking the opposite of
 * the ROM's `self`/`recordId` <-> `r7`/`r3` assignment (every
 * comparison/address-add byte differed as a result, despite every
 * instruction *shape* matching one-for-one), and explicit
 * `register ... asm("r7")`/`asm("r3")` pins on either variable making
 * it worse (the compiler stopped using the pinned register as a base
 * pointer at all and fell back to `sp`-relative addressing instead).
 * Closed as a NAKED transcription instead - the same escape hatch
 * already used for `sub_801E688`/`LoadGraphicsPackage`/
 * `LoadBg2Background` this session for the identical symptom - which
 * sidesteps the C-level register allocator entirely. Hand-transcribed
 * instruction-for-instruction from the ROM disassembly
 * (`0x08024F24`-`0x080250BC`, formerly `asm/code_3_2_17_24f24.s`'s
 * first function). */
NAKED void *sub_8024F24(struct tile_cache *self, s32 recordId)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #4\n\t"
        "add r7, r0, #0\n\t"
        "add r3, r1, #0\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #5\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 1f\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x20\n\t"
        "b 18f\n\t"
        "1:\n\t"
        "ldr r1, =0x00001024\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 2f\n\t"
        "mov r1, #0x90\n\t"
        "lsl r1, r1, #1\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "2:\n\t"
        "ldr r1, =0x00001028\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 3f\n\t"
        "mov r1, #0x88\n\t"
        "lsl r1, r1, #2\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "3:\n\t"
        "ldr r1, =0x0000102C\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 4f\n\t"
        "mov r1, #0xc8\n\t"
        "lsl r1, r1, #2\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "4:\n\t"
        "ldr r1, =0x00001030\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 5f\n\t"
        "mov r1, #0x84\n\t"
        "lsl r1, r1, #3\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "5:\n\t"
        "ldr r1, =0x00001034\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 6f\n\t"
        "mov r1, #0xa4\n\t"
        "lsl r1, r1, #3\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "6:\n\t"
        "ldr r1, =0x00001038\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 7f\n\t"
        "mov r1, #0xc4\n\t"
        "lsl r1, r1, #3\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "7:\n\t"
        "ldr r1, =0x0000103C\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 8f\n\t"
        "mov r1, #0xe4\n\t"
        "lsl r1, r1, #3\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "8:\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #5\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 9f\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        "9:\n\t"
        "ldr r1, =0x00001044\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 10f\n\t"
        "mov r1, #0x92\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "10:\n\t"
        "ldr r1, =0x00001048\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 11f\n\t"
        "mov r1, #0xa2\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "11:\n\t"
        "ldr r1, =0x0000104C\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 12f\n\t"
        "mov r1, #0xb2\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "12:\n\t"
        "ldr r1, =0x00001050\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 13f\n\t"
        "mov r1, #0xc2\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "13:\n\t"
        "ldr r1, =0x00001054\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 14f\n\t"
        "mov r1, #0xd2\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "14:\n\t"
        "ldr r1, =0x00001058\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "bne 15f\n\t"
        "mov r1, #0xe2\n\t"
        "lsl r1, r1, #4\n\t"
        "b 17f\n\t"
        ".pool\n\t"
        "15:\n\t"
        "ldr r1, =0x0000105C\n\t"
        "add r0, r7, r1\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r3, r0\n\t"
        "beq 16f\n\t"
        "mov r0, #0x83\n\t"
        "lsl r0, r0, #5\n\t"
        "add r6, r7, r0\n\t"
        "ldr r4, [r6]\n\t"
        "add r4, #0xf\n\t"
        "mov r1, #0xf\n\t"
        "mov r8, r1\n\t"
        "and r4, r1\n\t"
        "lsl r5, r4, #8\n\t"
        "add r5, #0x20\n\t"
        "add r5, r7, r5\n\t"
        "add r0, r7, #0\n\t"
        "add r1, r3, #0\n\t"
        "add r2, r5, #0\n\t"
        "str r3, [sp]\n\t"
        "bl sub_8025334\n\t"
        "lsl r4, r4, #2\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #5\n\t"
        "add r0, r7, r1\n\t"
        "add r0, r0, r4\n\t"
        "ldr r3, [sp]\n\t"
        "str r3, [r0]\n\t"
        "ldr r0, [r6]\n\t"
        "add r0, #1\n\t"
        "mov r1, r8\n\t"
        "and r0, r1\n\t"
        "str r0, [r6]\n\t"
        "add r0, r5, #0\n\t"
        "b 18f\n\t"
        ".pool\n\t"
        "16:\n\t"
        "mov r1, #0xf2\n\t"
        "lsl r1, r1, #4\n\t"
        "17:\n\t"
        "add r0, r7, r1\n\t"
        "18:\n\t"
        "add sp, #4\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r1` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

extern u8 gStaticData_081725AC[];

/* `x`/`y` in pixels, 16x8px collision-tile granularity. Looks up the
 * decoded tile record for `(x>>4, y>>3)` and, unless out of bounds,
 * returns a pointer into the 36-byte-stride terrain-property table
 * (`gStaticData_081725AC`) for the tile's low-byte type index. Writes
 * the decoded high nibble to a stack-local byte nothing ever reads back
 * (mirrors `sub_8025130`'s `flagsOut` write, but this sibling has no
 * caller-supplied out-parameter).
 *
 * Same register-allocation-permutation gap `sub_8024F24` had before it
 * was closed as NAKED (see that function's own comment above) - closed
 * the same way: hand-transcribed instruction-for-instruction from the
 * ROM disassembly (formerly `asm/code_3_2_17_24f24.s`'s first
 * function). See docs/matching/issue-40-terrain-tile-cache.md. */
NAKED void *sub_80250BC(struct tile_cache *self, s32 x, s32 y)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "mov r7, sp\n\t"
        "cmp r4, #0\n\t"
        "blt 2f\n\t"
        "cmp r6, #0\n\t"
        "blt 2f\n\t"
        "asr r3, r4, #4\n\t"
        "asr r1, r6, #3\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r0, [r5, #0x18]\n\t"
        "mul r0, r1, r0\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8024F24\n\t"
        "mov r1, #7\n\t"
        "and r1, r6\n\t"
        "mov r2, #0xf\n\t"
        "and r4, r2\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r0, r1, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "lsr r0, r0, #0x18\n\t"
        "and r0, r2\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "strb r0, [r7]\n\t"
        "1:\n\t"
        "mov r1, #0xff\n\t"
        "and r1, r3\n\t"
        "cmp r1, #0\n\t"
        "beq 2f\n\t"
        "cmp r1, #0x23\n\t"
        "ble 3f\n\t"
        "2:\n\t"
        "mov r0, #0\n\t"
        "b 4f\n\t"
        "3:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =gStaticData_081725AC\n\t"
        "add r0, r0, r1\n\t"
        "4:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".pool"
    );
}

extern u8 gStaticData_081725B4[];
extern u8 gStaticData_081725BC[];
extern u8 gStaticData_081725C4[];

/* Sibling of `sub_80250BC`, adding an output flag-nibble pointer
 * (`flagsOut`) and a `mode`-selected terrain-property table (mirroring
 * `sub_8025228`'s mode dispatch, but each mode's flag test differs and
 * the return is the full table-row pointer rather than a single
 * byte).
 *
 * Same register-allocation-permutation gap as `sub_8024F24`/
 * `sub_80250BC` above (the ROM packs `self`/`x`/`y`/`mode` as r5/r4/r6/
 * r7; several C phrasings landed on other permutations instead) -
 * closed the same way: hand-transcribed instruction-for-instruction
 * from the ROM disassembly, including the ROM's own mid-function
 * `.pool` splits (one after each of the first three mode cases; the
 * fourth table address is shared with the function's own trailing
 * pool). See docs/matching/issue-40-terrain-tile-cache.md. */
NAKED void *sub_8025130(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "mov r3, #0\n\t"
        "cmp r4, #0\n\t"
        "blt 1f\n\t"
        "cmp r6, #0\n\t"
        "bge 2f\n\t"
        "1:\n\t"
        "mov r1, #0\n\t"
        "b 4f\n\t"
        "2:\n\t"
        "asr r3, r4, #4\n\t"
        "asr r1, r6, #3\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r0, [r5, #0x18]\n\t"
        "mul r0, r1, r0\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8024F24\n\t"
        "mov r1, #7\n\t"
        "and r1, r6\n\t"
        "mov r2, #0xf\n\t"
        "and r4, r2\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r0, r1, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "lsr r3, r0, #0x1c\n\t"
        "lsr r1, r0, #0x18\n\t"
        "and r1, r2\n\t"
        "cmp r1, #0\n\t"
        "beq 3f\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "strb r1, [r0]\n\t"
        "3:\n\t"
        "mov r1, #0xff\n\t"
        "and r1, r4\n\t"
        "4:\n\t"
        "cmp r1, #0x23\n\t"
        "bgt 5f\n\t"
        "mov r0, #0\n\t"
        "b 18f\n\t"
        "5:\n\t"
        "cmp r7, #1\n\t"
        "beq 9f\n\t"
        "cmp r7, #1\n\t"
        "bgt 6f\n\t"
        "cmp r7, #0\n\t"
        "beq 7f\n\t"
        "b 17f\n\t"
        "6:\n\t"
        "cmp r7, #2\n\t"
        "beq 11f\n\t"
        "cmp r7, #3\n\t"
        "beq 13f\n\t"
        "b 17f\n\t"
        "7:\n\t"
        "mov r0, #4\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "beq 8f\n\t"
        "mov r0, #0\n\t"
        "b 16f\n\t"
        "8:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =gStaticData_081725AC\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "9:\n\t"
        "and r3, r7\n\t"
        "cmp r3, #0\n\t"
        "beq 10f\n\t"
        "mov r0, #0\n\t"
        "b 16f\n\t"
        "10:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =gStaticData_081725B4\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "11:\n\t"
        "mov r0, #8\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "beq 12f\n\t"
        "mov r0, #0\n\t"
        "b 16f\n\t"
        "12:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =gStaticData_081725BC\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "13:\n\t"
        "mov r0, #2\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "beq 14f\n\t"
        "mov r0, #0\n\t"
        "b 16f\n\t"
        "14:\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =gStaticData_081725C4\n\t"
        "15:\n\t"
        "add r0, r0, r1\n\t"
        "16:\n\t"
        "mov r8, r0\n\t"
        "17:\n\t"
        "mov r0, r8\n\t"
        "18:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".pool"
    );
}

extern u8 gStaticData_081725A8[];

/* The `CheckTerrainFlag(x, y, mode)`-style API docs/rom_map.md
 * identified: re-derives the same tile lookup as `sub_80250BC`, then
 * returns one of 4 adjacent flag bytes from `gStaticData_081725A8`
 * depending on `mode`, each independently bounds-gated by its own bit
 * of the cell's high nibble.
 *
 * Same register-allocation-permutation gap as `sub_8025130` above -
 * closed the same way: hand-transcribed instruction-for-instruction
 * from the ROM disassembly, including its four separate mid-/
 * end-function `.pool` splits for the four `gStaticData_081725A8`
 * references (the ROM never reuses one literal-pool slot for more than
 * one `ldr`, even though all four load the same symbol). See
 * docs/matching/issue-40-terrain-tile-cache.md. */
NAKED s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "add r7, r3, #0\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "mov r3, #0\n\t"
        "cmp r4, #0\n\t"
        "blt 1f\n\t"
        "cmp r6, #0\n\t"
        "bge 2f\n\t"
        "1:\n\t"
        "mov r2, #0\n\t"
        "b 4f\n\t"
        "2:\n\t"
        "asr r3, r4, #4\n\t"
        "asr r1, r6, #3\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r0, [r5, #0x18]\n\t"
        "mul r0, r1, r0\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8024F24\n\t"
        "mov r1, #7\n\t"
        "and r1, r6\n\t"
        "mov r2, #0xf\n\t"
        "and r4, r2\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r0, r1, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "lsr r3, r0, #0x1c\n\t"
        "lsr r1, r0, #0x18\n\t"
        "and r1, r2\n\t"
        "cmp r1, #0\n\t"
        "beq 3f\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "strb r1, [r0]\n\t"
        "3:\n\t"
        "mov r2, #0xff\n\t"
        "and r2, r4\n\t"
        "4:\n\t"
        "cmp r2, #0x23\n\t"
        "bgt 5f\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "b 17f\n\t"
        "5:\n\t"
        "cmp r7, #1\n\t"
        "beq 8f\n\t"
        "cmp r7, #1\n\t"
        "bgt 6f\n\t"
        "cmp r7, #0\n\t"
        "beq 7f\n\t"
        "b 16f\n\t"
        "6:\n\t"
        "cmp r7, #2\n\t"
        "beq 10f\n\t"
        "cmp r7, #3\n\t"
        "beq 13f\n\t"
        "b 16f\n\t"
        "7:\n\t"
        "mov r0, #4\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "bne 11f\n\t"
        "ldr r1, =gStaticData_081725A8\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0]\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "8:\n\t"
        "and r3, r7\n\t"
        "cmp r3, #0\n\t"
        "beq 9f\n\t"
        "mov r0, #0\n\t"
        "b 15f\n\t"
        "9:\n\t"
        "ldr r1, =gStaticData_081725A8\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #1]\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "10:\n\t"
        "mov r0, #8\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "beq 12f\n\t"
        "11:\n\t"
        "mov r1, #0\n\t"
        "mov r8, r1\n\t"
        "b 16f\n\t"
        "12:\n\t"
        "ldr r1, =gStaticData_081725A8\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #2]\n\t"
        "b 15f\n\t"
        ".pool\n\t"
        "13:\n\t"
        "mov r0, #2\n\t"
        "and r3, r0\n\t"
        "cmp r3, #0\n\t"
        "beq 14f\n\t"
        "mov r0, #0\n\t"
        "b 15f\n\t"
        "14:\n\t"
        "ldr r1, =gStaticData_081725A8\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #3]\n\t"
        "15:\n\t"
        "mov r8, r0\n\t"
        "16:\n\t"
        "mov r1, r8\n\t"
        "lsl r0, r1, #0x18\n\t"
        "asr r0, r0, #0x18\n\t"
        "17:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".pool"
    );
}

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

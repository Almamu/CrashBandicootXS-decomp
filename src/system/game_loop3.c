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
 * NAKED: plain C under old_agbcc is 30 halfwords off. The accumulator
 * and the pair loop's induction pointer swap r4 and r5 (allocation
 * priorities 2.4 vs 2.5); the ROM's r7 "shadow pointer" is gcc's own
 * strength-reduced `&dest[written]`, not a C variable.
 *
 * Later pass (#40 retry): the `#if NON_MATCHING` draft is 33 halfwords
 * off under old_agbcc (same size). `acc`/`pair` as `s16` reproduce the
 * ROM's per-use `lsl/asr` sign extensions and a do-while pair loop its
 * missing entry test; left are the r4/r5 swap above and the ROM
 * interleaving the `(s8)pair` shift pair around `acc`'s extension. */
#if NON_MATCHING
void sub_8025334(struct tile_cache *self, s32 recordId, void *dest)
{
    u16 *out = dest;
    u16 *src = (u16 *)((u32 *)self->decodeBase + ((u16 *)self->decodeBase)[recordId]);
    s32 budget = 0x7F;
    s32 written = 0;

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
                s16 pair = *src++;

                acc += (s8)pair;
                out[written++] = acc;
                acc += pair >> 8;
                out[written++] = acc;
                n -= 2;
            } while (n > 1);
            if (n != 0)
            {
                s16 last = *src++;

                out[written] = acc + (s8)last;
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
#else
NAKED void sub_8025334(struct tile_cache *self, s32 recordId, void *dest)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "mov r8, r2\n\t"
        "ldr r6, [r0, #4]\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r6\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r0, r1, #2\n\t"
        "add r6, r6, r0\n\t"
        "mov r0, #0x7f\n\t"
        "mov sb, r0\n\t"
        "mov r1, #0\n\t"
        "mov ip, r1\n\t"
        "mov r7, r8\n\t"
        "1:\n\t"
        "ldrh r1, [r6]\n\t"
        "ldrb r3, [r6]\n\t"
        "add r6, #2\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #8\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "ldrh r2, [r6]\n\t"
        "add r6, #2\n\t"
        "mov r4, sb\n\t"
        "sub r4, r4, r3\n\t"
        "mov sb, r4\n\t"
        "mov r1, ip\n\t"
        "lsl r0, r1, #1\n\t"
        "mov r4, r8\n\t"
        "add r1, r0, r4\n\t"
        "2:\n\t"
        "strh r2, [r1]\n\t"
        "add r1, #2\n\t"
        "add r7, #2\n\t"
        "mov r0, #1\n\t"
        "add ip, r0\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #0\n\t"
        "bne 2b\n\t"
        "b 7f\n\t"
        "3:\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #7\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "beq 5f\n\t"
        "mov r1, sb\n\t"
        "sub r1, r1, r3\n\t"
        "mov sb, r1\n\t"
        "ldrh r4, [r6]\n\t"
        "add r6, #2\n\t"
        "strh r4, [r7]\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "add r7, #2\n\t"
        "mov r2, #1\n\t"
        "add ip, r2\n\t"
        "mov r1, ip\n\t"
        "lsl r0, r1, #1\n\t"
        "mov r2, r8\n\t"
        "add r5, r0, r2\n\t"
        "4:\n\t"
        "ldrh r2, [r6]\n\t"
        "add r6, #2\n\t"
        "lsl r1, r2, #0x18\n\t"
        "lsl r0, r4, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "asr r1, r1, #0x18\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "strh r4, [r5]\n\t"
        "add r5, #2\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsl r0, r4, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "asr r2, r2, #0x18\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r4, r0, #0x10\n\t"
        "strh r4, [r5]\n\t"
        "add r5, #2\n\t"
        "add r7, #4\n\t"
        "mov r0, #2\n\t"
        "add ip, r0\n\t"
        "sub r0, r3, #2\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #1\n\t"
        "bhi 4b\n\t"
        "cmp r3, #0\n\t"
        "beq 7f\n\t"
        "ldrh r1, [r6]\n\t"
        "add r6, #2\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsl r0, r4, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "asr r1, r1, #0x18\n\t"
        "add r0, r0, r1\n\t"
        "strh r0, [r7]\n\t"
        "add r7, #2\n\t"
        "mov r1, #1\n\t"
        "add ip, r1\n\t"
        "b 7f\n\t"
        "5:\n\t"
        "mov r2, sb\n\t"
        "sub r2, r2, r3\n\t"
        "mov sb, r2\n\t"
        "mov r4, ip\n\t"
        "lsl r0, r4, #1\n\t"
        "mov r2, r8\n\t"
        "add r1, r0, r2\n\t"
        "6:\n\t"
        "ldrh r0, [r6]\n\t"
        "strh r0, [r1]\n\t"
        "add r6, #2\n\t"
        "add r1, #2\n\t"
        "add r7, #2\n\t"
        "mov r4, #1\n\t"
        "add ip, r4\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #0\n\t"
        "bne 6b\n\t"
        "7:\n\t"
        "mov r0, sb\n\t"
        "cmp r0, #0\n\t"
        "bge 1b\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
#endif
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

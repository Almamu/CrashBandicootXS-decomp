#include "core.h"
#include "actor.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct level_layer
{
    u8 unk_00[0x10];
    u32 width;                  // 0x10 - extent in the low 24 bits
    u32 height;                 // 0x14 - extent in the low 24 bits
};

struct level_info
{
    u8 unk_00[0x10];
    struct level_layer *layer;  // 0x10
};

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct manager
{
    u8 unk_00[0xC];
    struct { u8 unk_00[0x18]; struct method attach; } *vtable; // 0x0C
};

struct fx_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    void *anim;                 // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;             // 0x28
    u32 flipX:1;
    u32 unk_28_5:3;
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                     // 0x2D
    u8 unk_2E[0x16];
    struct manager *mgr;        // 0x44
};

struct actor_flag_bits
{
    u8 unk_0:1;
    u8 bit1:1;
    u8 bit2:1;
    u8 unk_3:5;
};

#define ACTOR_FLAG_BITS(a) ((struct actor_flag_bits *)&(a)->flags)

extern struct level_info *gUnknown_03001308;
extern void ***gUnknown_030012D0;
extern void *gUnknown_030012F0;

extern struct fx_part *sub_8009ED0(u16 arg0, u16 x, u16 y, u16 arg3);
extern void sub_80087C0(struct fx_part *part);
extern void sub_80087B4(struct fx_part *part);
extern void sub_800872C(struct fx_part *part, s32 val);
extern s32 sub_800815C(struct fx_part *part);
extern void *sub_8026EDC(s32 size);
extern struct manager *sub_800CCE0(void);
extern s32 sub_803AD80(void *self, void *arg, void *fn);
extern void sub_8008E94(void *manager, void *value);

/* Spawns a `sub_8025BAC` part next to `src` (at `src`'s tile X/Y, facing
 * its way), places it beside `src` by their two `sub_8007B98` AABBs'
 * half-widths plus `margin`, offsets its Y by `z`, and seeds its
 * velocity fields (`+0x60`/`+0x48`/`+0x4c`/`+0x50`) from `speed`,
 * negated when `src` is mirrored.
 *
 * NAKED: plain C under old_agbcc is 61 halfwords off. gcc keeps the
 * address of the `src` box in a callee-saved register across the second
 * `sub_8007B98` call, which pushes `speed` out to the stack; the ROM has
 * the first width in r4, `speed` in r7 and `margin` in r8. */
NAKED struct actor *sub_8025B0C(void *pool, s32 arg1, s32 kind, s32 margin, s32 z, s32 speed, void *src)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x28\n\t"
        "mov r8, r3\n\t"
        "ldr r7, [sp, #0x44]\n\t"
        "ldr r6, [sp, #0x48]\n\t"
        "ldr r3, [r6]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r5, [r6, #4]\n\t"
        "asr r5, r5, #8\n\t"
        "add r4, r6, #0\n\t"
        "add r4, r4, #0x28\n\t"
        "ldrb r4, [r4]\n\t"
        "lsl r4, r4, #0x1b\n\t"
        "lsr r4, r4, #0x1f\n\t"
        "str r5, [sp]\n\t"
        "str r4, [sp, #4]\n\t"
        "bl sub_8025BAC\n\t"
        "add r5, r0, #0\n\t"
        "add r0, sp, #8\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r4, [sp, #0x10]\n\t"
        "add r0, sp, #0x18\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r0, [sp, #0x20]\n\t"
        "lsr r1, r4, #0x1f\n\t"
        "add r4, r4, r1\n\t"
        "asr r4, r4, #1\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r4, r4, r0\n\t"
        "add r4, r8\n\t"
        "ldr r0, [r5]\n\t"
        "asr r1, r0, #8\n\t"
        "add r3, r5, #0\n\t"
        "add r3, r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "add r2, r1, r4\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "sub r2, r1, r4\n\t"
    "1:\n\t"
        "ldr r0, [r5, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r1, [sp, #0x40]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r1, r2, #8\n\t"
        "str r1, [r5]\n\t"
        "lsl r0, r0, #8\n\t"
        "str r0, [r5, #4]\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "neg r0, r7\n\t"
        "mov r1, #0x40\n\t"
        "str r0, [r5, #0x60]\n\t"
        "str r0, [r5, #0x48]\n\t"
        "str r1, [r5, #0x4c]\n\t"
        "str r0, [r5, #0x50]\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "mov r0, #0x40\n\t"
        "str r7, [r5, #0x60]\n\t"
        "str r7, [r5, #0x48]\n\t"
        "str r0, [r5, #0x4c]\n\t"
        "str r7, [r5, #0x50]\n\t"
    "3:\n\t"
        "add r0, r5, #0\n\t"
        "add sp, #0x28\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* Spawns a sub_8009ED0 effect part at (x, y) clamped into the current
 * level's bounds, facing left when `mirror` is set, with animation
 * record `anim` (12-byte stride) and tag `tag`. Attaches it to a fresh
 * sub_800CCE0 manager and registers it with gUnknown_030012F0. */
struct fx_part *sub_8025BAC(void *unused0, s32 anim, s32 tag, s32 x, s32 y, s32 mirror)
{
    struct fx_part *part;
    struct level_layer *layer;
    struct manager *mgr;

    if (x < 0)
        x = 0;
    layer = gUnknown_03001308->layer;
    /* Compared sign-extended from 24 bits, clamped zero-extended. */
    if (x >= (s32)(layer->width << 8) >> 8)
        x = (layer->width << 8 >> 8) - 1;
    if (y < 0)
        y = 0;
    if (y >= (s32)(layer->height << 8) >> 8)
        y = (layer->height << 8 >> 8) - 1;
    part = sub_8009ED0(0xffff, x, y, 0);
    part->flipX = mirror != 0;
    part->anim = (u8 *)**gUnknown_030012D0 + anim * 12;
    part->tag = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x10);
    mgr = sub_800CCE0();
    part->mgr = mgr;
    sub_803AD80((u8 *)mgr + mgr->vtable->attach.thisOffset, part, mgr->vtable->attach.fn);
    ACTOR_FLAG_BITS(&part->base)->bit2 = 0;
    ACTOR_FLAG_BITS(&part->base)->bit1 = 0;
    sub_8008E94(gUnknown_030012F0, part);
    return part;
}

/* Same early-out and `+0x49`/`+0x4a`/`+0x4b` tagging shape as
 * `sub_8025A64` (game_loop29.c), but spawns via `sub_801173C` with a
 * "special" 4th argument (`0xFFFF` when `p5` is set or `p4 == 0xff`,
 * `0` otherwise) and fires `sub_801191C`/`sub_8011870` instead of
 * `sub_80111B8`.
 *
 * NAKED: plain C under old_agbcc is 5 halfwords off - only the
 * `movs r0, #0` for the `+0x4b` store is scheduled differently (the ROM
 * loads it right after the `+0x49` address). */
NAKED struct actor *sub_8025CA4(void *unused0, u16 x, u16 y, u8 p3, u8 p4, u8 p5)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r7, r3, #0\n\t"
        "ldr r5, [sp, #0x14]\n\t"
        "add r0, sp, #0x18\n\t"
        "ldrb r6, [r0]\n\t"
        "mov r4, #0\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "cmp r6, #0\n\t"
        "bne 1f\n\t"
        "cmp r5, #0xff\n\t"
        "bne 2f\n\t"
    "1:\n\t"
        "ldr r3, 6f\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "add r0, r3, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012C0\n"
    "6: .4byte 0x0000ffff\n"
    "2:\n\t"
        "ldr r0, 11f\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r3, #0\n\t"
    "3:\n\t"
        "bl sub_801173C\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, r1, #0x49\n\t"
        "mov r0, #0\n\t"
        "strb r7, [r1]\n\t"
        "add r1, r1, #1\n\t"
        "strb r5, [r1]\n\t"
        "add r1, r1, #1\n\t"
        "strb r0, [r1]\n\t"
        "cmp r5, #0xff\n\t"
        "bne 9f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801191C\n\t"
    "9:\n\t"
        "cmp r6, #0\n\t"
        "beq 4f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8011870\n\t"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "11: .4byte 0x0000ffff\n"
    );
}
asm(".align 2, 0");

/* Loads a `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` record from `rec`,
 * indexes `*table` by `tableIdx` (4-byte stride) to get a function
 * pointer, and tail-calls it as `fn(self, p1, p2, p3)`. On real
 * hardware this indirect call has to go through one of this ROM's
 * fixed per-register interworking trampolines
 * (`src/system/reg_trampolines.c`) rather than a direct `blx` - which
 * specific trampoline (here, `sub_803AD8C`/"bx r5") depends purely on
 * which register this compiler's allocator happens to land the
 * function pointer in, hence the `register ... asm("r5")` pin plus the
 * empty-asm "keep this value live" barrier right before the call. */
extern void sub_803AD8C(void *a0, u16 a1, u16 a2, u16 a3);

void sub_8025D28(void **table, void *self, u16 *rec)
{
    register void *tablePtr asm("r1") = *table;
    register u16 idx asm("r3") = rec[0];
    register s32 shifted asm("r0") = idx << 2;
    void *entry = (u8 *)shifted + (s32)tablePtr;
    u16 p1 = rec[1];
    u16 p2 = rec[2];
    u16 p3 = rec[3];
    register void *fn asm("r5") = *(void **)entry;

    asm("" :: "r"(fn));
    sub_803AD8C(self, p1, p2, p3);
}

/* Stores `{a, b}` into the two Q8 words at `self+0`/`self+4`. */
void sub_8025D4C(void *self, s32 a, s32 b)
{
    *(s32 *)((u8 *)self + 4) = b;
    *(s32 *)self = a;
}

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - identical
 * body to `sub_8025A44` above (a second copy at a different ROM
 * address, same as `sub_8025A5C`/`sub_8025D6C` below). */
void sub_8025D54(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Zeroes the two Q8 position words at `self+0`/`self+4` - identical
 * body to `sub_8025A5C` above. */
void sub_8025D6C(void *self)
{
    *(s32 *)self = 0;
    *(s32 *)((u8 *)self + 4) = 0;
}

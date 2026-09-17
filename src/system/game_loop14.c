#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012D0;
extern void *gUnknown_030012F0;
extern void *gUnknown_03001308;

#if NON_MATCHING
extern struct actor *sub_8011114(u16 arg0, u16 arg1, u16 arg2, s32 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void sub_80111B8(void *part);

/* Spawns a part-object via `sub_8011114` at `(x,y)` when
 * `gUnknown_030012C0+0x8c` is clear (returns NULL otherwise), tags its
 * `+0x49`/`+0x4a`/`+0x4b` bytes from `p3`/`p5`/the (always-0, since
 * only reached on that branch) state flag, points `+0x20` at
 * `gUnknown_030012D0`'s shared resource table (fixed slot `0x8d`,
 * 4-byte stride), tags `+0x2d = 0xa`, builds the OAM/keyframe trio and
 * `+0x29` bitfield the same way `trigger_effect.c`'s
 * `sub_8020E84`-family functions do, and optionally fires
 * `sub_80111B8` when `flag6` is set.
 *
 * PARKED, NOT BYTE-MATCHING: semantics traced end-to-end against the
 * ROM (parameter registers, the `+0x8d*4` table-slot arithmetic, the
 * `& -0x10 | (result & 0xf)` bitfield idiom) but not iterated to an
 * exact register allocation within this issue's chunk - same
 * unresolved register-allocation family as the sibling
 * `sub_8020E84`/`sub_8020F7C`/`sub_802107C`/`sub_802117C` functions
 * already parked in trigger_effect.c. See
 * docs/matching/issue-41-game-loop-25894.md. */
struct actor *sub_8025A64(void *unused0, u16 x, u16 y, u8 p3, u32 p5, u8 flag6)
{
    struct actor *newObj = NULL;
    u8 state8c = *((u8 *)gUnknown_030012C0 + 0x8c);

    if (state8c == 0) {
        newObj = sub_8011114(0xFFFF, x, y, 0);
        newObj->flags |= 0x10;
        *((u8 *)newObj + 0x49) = p3;
        *((u8 *)newObj + 0x4a) = (u8)p5;
        *((u8 *)newObj + 0x4b) = state8c;
        {
            void *p2 = *(void **)gUnknown_030012D0;
            void *field0 = *(void **)p2;

            *(void **)((u8 *)newObj + 0x20) = (u8 *)field0 + 0x8d * 4;
        }
        *((u8 *)newObj + 0x2d) = 0xa;
        sub_80087C0(newObj);
        sub_80087B4(newObj);
        sub_800872C(newObj, 0);
        {
            s32 result = sub_800815C(newObj);
            u8 *bf = (u8 *)newObj + 0x29;

            *bf = (*bf & -0x10) | (result & 0xf);
        }
        if (flag6 != 0) {
            sub_80111B8(newObj);
        }
    }
    return newObj;
}

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern struct actor *sub_8025BAC(void *unused0, s32 x, s32 idx, s32 testX, s32 testY, u8 mirrorFlag);
extern void sub_8007B98(struct aabb *buf, void *obj);

/* Spawns via `sub_8025BAC` (using `src`'s Q8 X shifted to tile units as
 * the 4th arg), computes an AABB for both the new part and `src` via
 * the same `sub_8007B98` primitive `actor_part11.c` documents, averages
 * their half-widths plus `margin`, and uses `src`'s `+0x28` mirror bit
 * to place the new part to either side of `src`'s centre on X - Y is a
 * plain `src.y + z` offset. Finally seeds the part's velocity-ish
 * fields (`+0x60`/`+0x48`/`+0x4c`/`+0x50`, the same family
 * `sub_8009F50` zeroes in actor_part8.c) from `z`, negated when the
 * mirror bit is set.
 *
 * PARKED, NOT BYTE-MATCHING: semantics traced against the ROM
 * (register-by-register for the AABB/half-width/mirror-bit dance) but
 * not iterated to an exact match within this issue's chunk - a
 * genuinely large function with several long-lived stack temporaries.
 * See docs/matching/issue-41-game-loop-25894.md. */
struct actor *sub_8025B0C(void *arg0, void *arg1, void *arg2, s32 margin, s32 z, void *src)
{
    u8 *s = (u8 *)src;
    s32 srcX = *(s32 *)s >> 8;
    struct actor *newObj;
    struct aabb buf1, buf2;
    s32 halfW1, halfW2, combined;
    s32 newX, newY;
    u8 *flagsAddr;
    u8 flags;

    /* NOTE: the ROM's call site here does not visibly set up
     * sub_8025BAC's testY/mirrorFlag stack args before this `bl` -
     * they land wherever this function's own `sub sp, #0x28` scratch
     * buffer happens to hold at this point, an implicit stack-reuse
     * coincidence not fully resolved here. Passed as 0/0 pending
     * further investigation. */
    newObj = sub_8025BAC(arg0, (s32)arg1, (s32)arg2, srcX, 0, 0);

    sub_8007B98(&buf1, newObj);
    sub_8007B98(&buf2, src);

    halfW1 = buf1.field_8;
    halfW1 = (halfW1 + (s32)((u32)halfW1 >> 31)) >> 1;
    halfW2 = buf2.field_8;
    halfW2 = (halfW2 + (s32)((u32)halfW2 >> 31)) >> 1;
    combined = halfW1 + halfW2 + margin;

    flagsAddr = (u8 *)newObj + 0x28;
    flags = *flagsAddr;
    {
        s32 x = *(s32 *)newObj >> 8;

        if ((s32)(flags << 27) < 0) {
            newX = x - combined;
        } else {
            newX = x + combined;
        }
    }
    newY = (*(s32 *)((u8 *)newObj + 4) >> 8) + z;

    *(s32 *)newObj = newX << 8;
    *(s32 *)((u8 *)newObj + 4) = newY << 8;

    flags = *flagsAddr;
    if ((s32)(flags << 27) < 0) {
        s32 negZ = -z;

        *(s32 *)((u8 *)newObj + 0x60) = negZ;
        *(s32 *)((u8 *)newObj + 0x48) = negZ;
        *(s32 *)((u8 *)newObj + 0x4c) = 0x40;
        *(s32 *)((u8 *)newObj + 0x50) = negZ;
    } else {
        *(s32 *)((u8 *)newObj + 0x60) = z;
        *(s32 *)((u8 *)newObj + 0x48) = z;
        *(s32 *)((u8 *)newObj + 0x4c) = 0x40;
        *(s32 *)((u8 *)newObj + 0x50) = z;
    }

    return newObj;
}

extern struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2);
extern void *sub_8026EDC(s32 size);
extern void *sub_800CCE0(void);
extern void sub_803AD80(void *arg0, void *arg1, void *fn);
extern void sub_8008E94(void *manager, void *value);

/* Clamps `testX`/`testY` into `[0, extent)` using
 * `gUnknown_03001308`'s sub-object `+0x10`/`+0x14` extents (the same
 * "current level dimensions" object `game_loop3.c`/`game_loop5.c`
 * read), spawns via `sub_8009ED0`, tags the mirror bit from
 * `mirrorFlag`, points `+0x20` at `gUnknown_030012D0`'s shared table
 * (`idx*0xc` stride - a different slot layout than `sub_8025A64`'s
 * fixed `0x8d*4`), tags `+0x2d = idx`, builds the OAM/keyframe trio,
 * registers into a `sub_800CCE0`-owned manager's own `+0xc` trampoline
 * record via `sub_803AD80` (storing the manager itself at `+0x44`),
 * clears flags bits 1/2 (`& ~6`), and registers into
 * `gUnknown_030012F0`'s list via `sub_8008E94`.
 *
 * PARKED, NOT BYTE-MATCHING: semantics traced against the ROM but not
 * iterated to an exact register allocation within this issue's chunk -
 * see docs/matching/issue-41-game-loop-25894.md. */
struct actor *sub_8025BAC(void *unused0, s32 x, s32 idx, s32 testX, s32 testY, u8 mirrorFlag)
{
    struct actor *newObj;
    void *subObj;
    s32 extentX, extentY;

    if (testX < 0) {
        testX = 0;
    }

    subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
    extentX = *(s32 *)((u8 *)subObj + 0x10);
    if (testX >= extentX) {
        testX = extentX - 1;
    }

    if (testY < 0) {
        testY = 0;
    }
    extentY = *(s32 *)((u8 *)subObj + 0x14);
    if (testY >= extentY) {
        testY = extentY - 1;
    }

    newObj = sub_8009ED0(0xFFFF, (u16)testX, (u16)testY);

    {
        u8 *bf = (u8 *)newObj + 0x28;
        u8 bit4 = (u8)(((mirrorFlag != 0) ? 1 : 0) << 4);

        *bf = (*bf & ~0x11) | bit4;
    }
    {
        void *p2 = *(void **)gUnknown_030012D0;
        void *field0 = *(void **)p2;

        *(void **)((u8 *)newObj + 0x20) = (u8 *)field0 + idx * 0xc;
    }
    *((u8 *)newObj + 0x2d) = (u8)idx;

    sub_80087C0(newObj);
    sub_80087B4(newObj);
    sub_800872C(newObj, 0);
    {
        s32 result = sub_800815C(newObj);
        u8 *bf = (u8 *)newObj + 0x29;

        *bf = (*bf & -0x10) | (result & 0xf);
    }

    sub_8026EDC(0x10);
    {
        void *mgr = sub_800CCE0();
        u8 *rec = *(u8 **)((u8 *)mgr + 0xc);
        void *addr = (u8 *)mgr + *(s16 *)(rec + 0x18);
        void *fn = *(void **)(rec + 0x1c);

        *(void **)((u8 *)newObj + 0x44) = mgr;
        sub_803AD80(addr, newObj, fn);
    }

    newObj->flags &= (u8)(~6);

    sub_8008E94(gUnknown_030012F0, newObj);

    return newObj;
}

extern struct actor *sub_801173C(u16 arg0, u16 arg1, u16 arg2, s32 arg3);
extern void sub_801191C(void *part);
extern void sub_8011870(void *part);

/* Same early-out and `+0x49`/`+0x4a`/`+0x4b` tagging shape as
 * `sub_8025A64`, but spawns via `sub_801173C` with a "special" 4th
 * argument (`0xFFFF` when `p5` is set or `p4 == 0xff`, `0` otherwise)
 * instead of a fixed table slot, and fires `sub_801191C`/
 * `sub_8011870` instead of `sub_80111B8`.
 *
 * PARKED, NOT BYTE-MATCHING: semantics traced against the ROM but not
 * iterated to an exact register allocation within this issue's chunk -
 * see docs/matching/issue-41-game-loop-25894.md. */
struct actor *sub_8025CA4(void *unused0, u16 x, u16 y, u8 p3, u8 p4, u8 p5)
{
    struct actor *newObj = NULL;
    u8 state8c = *((u8 *)gUnknown_030012C0 + 0x8c);

    if (state8c == 0) {
        s32 special = (p5 != 0 || p4 == 0xff) ? 0xFFFF : 0;

        newObj = sub_801173C(0xFFFF, x, y, special);
        newObj->flags |= 0x10;
        *((u8 *)newObj + 0x49) = p3;
        *((u8 *)newObj + 0x4a) = p4;
        *((u8 *)newObj + 0x4b) = 0;
        if (p4 == 0xff) {
            sub_801191C(newObj);
        }
        if (p5 != 0) {
            sub_8011870(newObj);
        }
    }
    return newObj;
}
#endif /* NON_MATCHING */
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

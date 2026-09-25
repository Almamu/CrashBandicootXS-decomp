#include "core.h"

/* GitHub issue #12/#14 Phase 2, "accessor cluster" group: `sub_8011248`
 * through `sub_8011390` (11 functions, `0x08011248`-`0x08011448`),
 * carved out of the middle of the still-unexamined 24-function tail
 * documented in docs/matching/issue-14-0x08010d54-physics-apply.md.
 * `self` here is a further, still-unnamed "part"-shaped object -
 * distinct from `struct actor` (only 0x1c bytes) and from the
 * `struct collision_queue` `sub_8010D54`/`sub_8010E14`/`sub_8010E2C`
 * (game_loop50.c) operate on - the same "big, mostly-uncharacterized
 * object, individual fields named only by offset" situation already
 * documented for this object family in `src/graphics/actor_part15.c`'s
 * own file header. Every function here operates on a handful of fields
 * clustered at `self+0xc`/`+0x18`/`+0x38`/`+0x48`-`+0x50`:
 *
 * - `self+0xc`  (u8)  - flags byte (bit 2/bit 3 tested/set by
 *                       `sub_80112C4`/`sub_8011390`)
 * - `self+0x18` (void*) - the usual per-category data table pointer
 *                       (same convention as `struct actor.table`)
 * - `self+0x38` (u8)  - an externally-driven gate byte read (not
 *                       written) by `sub_80112C4`
 * - `self+0x48` (u8)  - a "spawned/active" gate byte, cleared by
 *                       `sub_8011308`, tested by `sub_8011330`
 * - `self+0x49` (u8)  - unexamined byte setter (`sub_8011388`)
 * - `self+0x4a` (u8)  - orbit "mode" (0 = inactive; 1/2 = which way the
 *                       orbit offset is applied to the anchor x; other
 *                       values leave x at the anchor) - set by
 *                       `sub_8011378`, tested by `sub_8011248`'s output
 *                       branch and `sub_8011390`'s activity gate
 * - `self+0x4b` (u8)  - orbit phase/angle index into the shared sine
 *                       table `gStaticData_0816A820`, reset to 0 by
 *                       `sub_8011378`, advanced elsewhere (not in this
 *                       group), read by `sub_8011248`/`sub_8011390`
 * - `self+0x4c` (s32) - orbit anchor x (Q8)
 * - `self+0x50` (s32) - orbit anchor y (Q8)
 * - `self+0x0`  (s32) - current x (Q8) - `sub_8011364` seeds it from
 *                       the anchor; `sub_8011248` never touches it
 *                       (only rewrites `self+4`'s `y`, despite what its
 *                       own field-order might suggest - see below)
 * - `self+0x4`  (s32) - current y (Q8), recomputed every call by
 *                       `sub_8011248`
 *
 * Confirms this is a small "orbiting hazard" behavior mixed into the
 * same object type `sub_8011114`/`sub_80111B8` (game_loop29.c,
 * Phase 2's neighboring group) spawn/manage - `sub_8011364` seeds an
 * orbit anchor+start position, `sub_8011378` (re)starts the orbit at a
 * given mode/phase 0, `sub_8011388` sets an adjacent still-unexamined
 * byte, `sub_8011248` is the per-frame orbit-position update,
 * `sub_8011330` fires a `self->table`-driven hit trampoline once the
 * object is "spawned" (`self+0x48 == 0`) and the player has a specific
 * flag set, `sub_80112C4` re-derives visibility from a
 * `sub_8007A84`/`self+0x38` gate and clears flags bit 3 when gated off,
 * `sub_80112F4`/`sub_8011310`/`sub_8011308` are a small
 * init/reset/table-repoint trio (same `sub_80084A4`/table-swap shape
 * documented throughout `actor_part8.c`), and `sub_8011390` is the
 * per-frame player-proximity/hit-resolve step: gated by the same
 * orbit-mode/phase fields, it AABB-tests against the player (choosing
 * primary vs. secondary AABB build depending on the player's own
 * current state, `player+0xa == 0x13`), and on overlap sets flags bit
 * 3, tail-calls the despawn picker `sub_8011448` (Phase 2's own
 * neighboring group, not read this pass - only extern'd here) with a
 * mode that differs per AABB path, and (primary-AABB path only) plays
 * a hit SFX. */

extern void *gUnknown_030012D8;
extern void *gUnknown_030012CC;
extern void *gUnknown_030012BC;

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8007A84(void *self, void *part);
extern void *sub_8007B98(void *dest, void *pt);
extern void *sub_8007C30(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *sub_803AD7C(void *arg0, void *fn);
extern struct actor *sub_80084A4(struct actor *self);
extern void sub_8008484(struct actor *self, u32 arg1);
extern s32 sub_80008FC(s32 a, s32 b);
extern s16 gStaticData_0816A820[];
extern s32 gStaticData_0816BF08[3];
extern u8 gStaticData_087E40DC[];

/* Phase 2's neighboring group (not read/matched this pass) - the
 * randomized-position despawn picker `docs/rom_map.md` already
 * documents, called as `sub_8011448(entry, 1)`/`(other, 1)` elsewhere
 * (`game_loop40.c`/`game_loop49.c`). */
extern void sub_8011448(void *self, s32 mode);

/* Per-frame orbit-position update. Reads the current orbit phase
 * (`self+0x4b`) twice, at two different scales into the shared sine
 * table (`*4` for the y-offset, `*2` for the x-offset - two different
 * "speeds" around the same table, not a copy/paste of the same lookup)
 * and combines each with a scale factor via the overflow-avoiding
 * fixed-point multiply `sub_80008FC`: the y-offset always uses the
 * fixed scale `0x800`, while the x-offset's scale comes from a local
 * copy of the 3-entry table `gStaticData_0816BF08`, indexed by
 * `self+0x4a - 1` (so `self+0x4a` must be 1-3 to select a scale; mode 3
 * yields an x-offset that's discarded - see below).
 *
 * `self+4` (`y`) is always `self+0x50` (anchor y) minus the y-offset.
 * `self` (`x`) is `self+0x4c` (anchor x) minus the x-offset when
 * `self+0x4a == 1`, plus the x-offset when `self+0x4a == 2`, or just
 * the anchor x unchanged otherwise (`self+0x4a == 3`, or in practice
 * any other value - the local `gStaticData_0816BF08` lookup still runs
 * for mode 3, its result simply unused). */
struct three_words {
    s32 a, b, c;
};

/* Written as NAKED asm, not plain C: a plain-C reconstruction (the
 * struct-copy local, two `gStaticData_0816A820` lookups at different
 * strides, the mode-1/mode-2/else branch) reproduces the ROM's exact
 * *shape* instruction-for-instruction, but gcc 2.9 -O2 persistently
 * picks the opposite register/operand order for the two
 * `table + phase*stride` pointer adds (`adds r0, r4, r0` instead of
 * the ROM's own `adds r0, r0, r4`) no matter how the C source orders
 * the addition (pointer-arithmetic normalizes the pointer operand
 * first internally) - not something a source-level register pin can
 * reach, so closed via NAKED transcription instead, the same escape
 * hatch already established throughout this subsystem
 * (`sub_8010D54`, `game_loop50.c`). */
NAKED void sub_8011248(void *selfArg)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #0xc\n\t"
        "add r5, r0, #0\n\t"
        "mov r1, sp\n\t"
        "ldr r0, 1f\n\t"
        "ldm r0!, {r2, r3, r4}\n\t"
        "stm r1!, {r2, r3, r4}\n\t"
        "ldr r4, 2f\n\t"
        "add r6, r5, #0\n\t"
        "add r6, #0x4b\n\t"
        "ldrb r1, [r6]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r4\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r0, r3]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #4\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_80008FC\n\t"
        "ldr r1, [r5, #0x50]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r5, #4]\n\t"
        "ldrb r6, [r6]\n\t"
        "lsl r0, r6, #2\n\t"
        "add r0, r0, r4\n\t"
        "mov r4, #0\n\t"
        "ldrsh r2, [r0, r4]\n\t"
        "add r4, r5, #0\n\t"
        "add r4, #0x4a\n\t"
        "ldrb r0, [r4]\n\t"
        "sub r0, #1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, sp\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_80008FC\n\t"
        "add r2, r0, #0\n\t"
        "ldrb r0, [r4]\n\t"
        "cmp r0, #1\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5, #0x4c]\n\t"
        "sub r0, r0, r2\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0816BF08\n"
    "2: .4byte gStaticData_0816A820\n"
    "3:\n\t"
        "cmp r0, #2\n\t"
        "bne 5f\n\t"
        "ldr r0, [r5, #0x4c]\n\t"
        "add r0, r0, r2\n\t"
        "b 4f\n\t"
    "5:\n\t"
        "ldr r0, [r5, #0x4c]\n\t"
    "4:\n\t"
        "str r0, [r5]\n\t"
        "add sp, #0xc\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* Re-derives visibility via `sub_8007A84(gUnknown_030012CC, self)`
 * (already matched, `actor_part.c`), then clears flags bit 3
 * (`self+0xc`) when `self+0x38` is nonzero - the same "consumed/hit"
 * flag bit `sub_8011390` below sets. */
void sub_80112C4(void *selfArg)
{
    u8 *self = selfArg;

    sub_8007A84(gUnknown_030012CC, self);
    if (self[0x38] != 0) {
        register s32 mask asm("r0") = 9;
        mask = -mask;
        mask &= self[0xc];
        self[0xc] = mask;
    }
}

/* Trivial - always returns 2, ignoring any argument. */
s32 sub_80112F0(void)
{
    return 2;
}

/* Repoints `self->table` (`self+0x18`) at `gStaticData_087E40DC`, then
 * tail-calls `sub_8008484` (already matched, `actor_part8.c`) with
 * `self` and this function's own second argument passed straight
 * through. */
void sub_80112F4(void *selfArg, u32 arg1)
{
    u8 *self = selfArg;

    *(void **)(self + 0x18) = gStaticData_087E40DC;
    sub_8008484((struct actor *)self, arg1);
}

/* Clears the "spawned/active" gate byte `self+0x48`. */
void sub_8011308(void *selfArg)
{
    u8 *self = selfArg;

    self[0x48] = 0;
}

/* Re-initializes `self` via `sub_80084A4` (already matched,
 * `actor_part8.c`; its return value is discarded - same "call for
 * side effect only" shape used elsewhere in this object family),
 * repoints `self->table` at `gStaticData_087E40DC`, clears the
 * "spawned/active" gate byte via `sub_8011308`, and returns `self`. */
void *sub_8011310(void *selfArg)
{
    u8 *self = selfArg;

    sub_80084A4((struct actor *)self);
    *(void **)(self + 0x18) = gStaticData_087E40DC;
    sub_8011308(self);
    return self;
}

/* If `self+0x48` (the "spawned/active" gate) is clear and the player's
 * (`gUnknown_030012D8`) own `+0xc` byte has bit 7 set, fires
 * `self->table+0x68/0x6c`'s trampoline (`sub_803AD7C`) - the usual
 * "offset + fn pointer" pair convention already established throughout
 * this codebase (e.g. `graphics.c`'s own `+0x10`/`+0x14` pair). Always
 * returns 0. */
s32 sub_8011330(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x48] == 0) {
        u8 *player = gUnknown_030012D8;

        if (player[0xc] >> 7) {
            u8 *entry = *(u8 **)(self + 0x18) + 0x68;
            void *addr = self + *(s16 *)entry;
            void *fn = *(void **)(entry + 4);

            sub_803AD7C(addr, fn);
        }
    }
    return 0;
}

/* Sets `self`/`self+4` (`x`/`y`, Q8) from the raw `x`/`y` arguments
 * scaled by 8, and mirrors both into `self+0x4c`/`self+0x50` - seeding
 * an orbit anchor at the object's own starting position. */
void sub_8011364(void *selfArg, s32 x, s32 y)
{
    u8 *self = selfArg;
    s32 qx, qy;

    *(s32 *)self = x << 8;
    *(s32 *)(self + 4) = y << 8;
    qx = *(volatile s32 *)self;
    qy = *(volatile s32 *)(self + 4);
    *(s32 *)(self + 0x4c) = qx;
    *(s32 *)(self + 0x50) = qy;
}

/* Sets the orbit mode (`self+0x4a`) and resets the orbit phase
 * (`self+0x4b`) to 0. */
void sub_8011378(void *selfArg, u8 mode)
{
    u8 *self = selfArg;
    u8 *modePtr;
    u8 zero;

    modePtr = self + 0x4a;
    zero = 0;
    *modePtr = mode;
    self[0x4b] = zero;
}

/* Unexamined byte setter, `self+0x49` - address-adjacent to the orbit
 * mode/phase pair above but not otherwise read by any function in this
 * group. */
void sub_8011388(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x49] = val;
}

/* Per-frame player-proximity/hit-resolve step. Gated: does nothing
 * unless the orbit is active (`self+0x4a != 0`) and either its phase
 * hasn't wrapped past `0x16` yet or the player (`gUnknown_030012D8`)
 * is in state `+0x88 == 3`; if the orbit is active, the phase has
 * wrapped, and the player isn't in that state, returns immediately.
 *
 * Once past that gate, proceeds only when flags bit 3
 * (the "already hit" latch `sub_80112C4` clears) is clear and flags
 * bit 2 is set. Builds `self`'s own AABB via `sub_8007B98`, then reads
 * the player's own `+0xa` state: if it's `0x13`, builds the player's
 * *primary* AABB (`sub_8007C30`) and tests it against `self`'s own via
 * `sub_8001688`; on overlap, sets flags bit 3, tail-calls
 * `sub_8011448(self, 1)`, and plays a hit SFX
 * (`PlaySfx(gUnknown_030012BC, 6, 0x80)`). Otherwise builds the
 * player's *secondary* AABB (`sub_8007B98`, the same helper used for
 * `self`'s own box) and tests it the same way; on overlap, sets flags
 * bit 3 and tail-calls `sub_8011448(self, 0)` (no SFX on this path). */
void sub_8011390(void *selfArg)
{
    u8 *self = selfArg;
    u8 selfBox[16];
    u8 playerBox[16];
    u8 *player;

    if (self[0x4a] != 0 && self[0x4b] <= 0x16) {
        if (((u8 *)gUnknown_030012D8)[0x88] != 3) {
            return;
        }
    }

    {
        u8 flags = self[0xc];
        u32 shifted = (u32)flags << 0x18;

        if ((shifted >> 0x1b) & 1) {
            return;
        }
        if (!((shifted >> 0x1a) & 1)) {
            return;
        }
    }

    sub_8007B98(selfBox, self);
    player = gUnknown_030012D8;

    if (player[0xa] == 0x13) {
        sub_8007C30(playerBox, player);
        if (sub_8001688(playerBox, selfBox)) {
            register s32 bit asm("r0") = 8;

            bit |= self[0xc];
            self[0xc] = bit;
            sub_8011448(self, 1);
            PlaySfx(gUnknown_030012BC, 6, 0x80);
        }
    } else {
        sub_8007B98(playerBox, player);
        if (sub_8001688(playerBox, selfBox)) {
            register s32 bit asm("r0") = 8;

            bit |= self[0xc];
            self[0xc] = bit;
            sub_8011448(self, 0);
        }
    }
}
/* Trailing byte-padding mismatch fix: the function body isn't a
 * multiple of 4 bytes, and gcc's own default alignment padding (a
 * `nop`/`mov r8, r8` instruction) differs from the ROM's own
 * zero-byte alignment padding at this address - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");

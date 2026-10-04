#include "core.h"
#include "orbit_part.h"

/* GitHub issue #12/#14 Phase 2, "accessor cluster" group: `sub_8011248`
 * through `CheckWumpaPickup` (11 functions, `0x08011248`-`0x08011448`),
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
 *                       `DrawExtraLife`/`CheckWumpaPickup`)
 * - `self+0x18` (void*) - the usual per-category data table pointer
 *                       (same convention as `struct actor.table`)
 * - `self+0x38` (u8)  - an externally-driven gate byte read (not
 *                       written) by `DrawExtraLife`
 * - `self+0x48` (u8)  - a "spawned/active" gate byte, cleared by
 *                       `sub_8011308`, tested by `sub_8011330`
 * - `self+0x49` (u8)  - unexamined byte setter (`sub_8011388`)
 * - `self+0x4a` (u8)  - orbit "mode" (0 = inactive; 1/2 = which way the
 *                       orbit offset is applied to the anchor x; other
 *                       values leave x at the anchor) - set by
 *                       `sub_8011378`, tested by `sub_8011248`'s output
 *                       branch and `CheckWumpaPickup`'s activity gate
 * - `self+0x4b` (u8)  - orbit phase/angle index into the shared sine
 *                       table `gSineTable`, reset to 0 by
 *                       `sub_8011378`, advanced elsewhere (not in this
 *                       group), read by `sub_8011248`/`CheckWumpaPickup`
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
 * same object type `CreateExtraLife`/`SendExtraLifeToHud` (game_loop29.c,
 * Phase 2's neighboring group) spawn/manage - `sub_8011364` seeds an
 * orbit anchor+start position, `sub_8011378` (re)starts the orbit at a
 * given mode/phase 0, `sub_8011388` sets an adjacent still-unexamined
 * byte, `sub_8011248` is the per-frame orbit-position update,
 * `sub_8011330` fires a `self->table`-driven hit trampoline once the
 * object is "spawned" (`self+0x48 == 0`) and the player has a specific
 * flag set, `DrawExtraLife` re-derives visibility from a
 * `DrawSprite`/`self+0x38` gate and clears flags bit 3 when gated off,
 * `DestroyExtraLife`/`InitExtraLife`/`sub_8011308` are a small
 * init/reset/table-repoint trio (same `sub_80084A4`/table-swap shape
 * documented throughout `actor_part8.c`), and `CheckWumpaPickup` is the
 * per-frame player-proximity/hit-resolve step: gated by the same
 * orbit-mode/phase fields, it AABB-tests against the player (choosing
 * primary vs. secondary AABB build depending on the player's own
 * current state, `player+0xa == 0x13`), and on overlap sets flags bit
 * 3, tail-calls the despawn picker `PickUpWumpa` (Phase 2's own
 * neighboring group, not read this pass - only extern'd here) with a
 * mode that differs per AABB path, and (primary-AABB path only) plays
 * a hit SFX. */

extern void *gPlayer;
extern void *gSpriteRenderer;
extern void *gAudioContext;

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void DrawSprite(void *self, void *part);
extern void *sub_8007B98(void *dest, void *pt);
extern void *sub_8007C30(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *_call_via_r1(void *arg0, void *fn);
extern struct actor *sub_80084A4(struct actor *self);
extern void sub_8008484(struct actor *self, u32 arg1);
extern s32 FixedMul(s32 a, s32 b);
extern s16 gSineTable[];
extern s32 gStaticData_0816BF08[3];
extern u8 gExtraLifeVtable[];

/* Phase 2's neighboring group (not read/matched this pass) - the
 * randomized-position despawn picker `docs/rom_map.md` already
 * documents, called as `PickUpWumpa(entry, 1)`/`(other, 1)` elsewhere
 * (`game_loop40.c`/`game_loop49.c`). */
extern void PickUpWumpa(void *self, s32 mode);

/* Per-frame orbit-position update. Reads the current orbit phase
 * (`self+0x4b`) twice, at two different scales into the shared sine
 * table (`*4` for the y-offset, `*2` for the x-offset - two different
 * "speeds" around the same table, not a copy/paste of the same lookup)
 * and combines each with a scale factor via the overflow-avoiding
 * fixed-point multiply `FixedMul`: the y-offset always uses the
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
    s32 a[3];
};

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the sine sample goes
 * through one reused local (`sn`), which old_agbcc keeps in r2 across
 * both calls exactly like the ROM; current agbcc renumbers the first
 * lookup's registers. Every other function in this file compiles the
 * same under either compiler. */
void sub_8011248(struct orbit_part *self)
{
    struct three_words scales = *(struct three_words *)gStaticData_0816BF08;
    s32 dy;
    s32 sn;

    sn = gSineTable[self->phase * 4];
    dy = FixedMul(sn, 0x800);
    self->base.y = self->anchor.y - dy;
    sn = gSineTable[self->phase * 2];
    sn = FixedMul(sn, scales.a[self->mode - 1]);
    if (self->mode == 1)
        self->base.x = self->anchor.x - sn;
    else if (self->mode == 2)
        self->base.x = self->anchor.x + sn;
    else
        self->base.x = self->anchor.x;
}

/* Re-derives visibility via `DrawSprite(gSpriteRenderer, self)`
 * (already matched, `actor_part.c`), then clears flags bit 3
 * (`self+0xc`) when `self+0x38` is nonzero - the same "consumed/hit"
 * flag bit `CheckWumpaPickup` below sets. */
void DrawExtraLife(void *selfArg)
{
    u8 *self = selfArg;

    DrawSprite(gSpriteRenderer, self);
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

/* Repoints `self->table` (`self+0x18`) at `gExtraLifeVtable`, then
 * tail-calls `sub_8008484` (already matched, `actor_part8.c`) with
 * `self` and this function's own second argument passed straight
 * through. */
void DestroyExtraLife(void *selfArg, u32 arg1)
{
    u8 *self = selfArg;

    *(void **)(self + 0x18) = gExtraLifeVtable;
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
 * repoints `self->table` at `gExtraLifeVtable`, clears the
 * "spawned/active" gate byte via `sub_8011308`, and returns `self`. */
void *InitExtraLife(void *selfArg)
{
    u8 *self = selfArg;

    sub_80084A4((struct actor *)self);
    *(void **)(self + 0x18) = gExtraLifeVtable;
    sub_8011308(self);
    return self;
}

/* If `self+0x48` (the "spawned/active" gate) is clear and the player's
 * (`gPlayer`) own `+0xc` byte has bit 7 set, fires
 * `self->table+0x68/0x6c`'s trampoline (`_call_via_r1`) - the usual
 * "offset + fn pointer" pair convention already established throughout
 * this codebase (e.g. `graphics.c`'s own `+0x10`/`+0x14` pair). Always
 * returns 0. */
s32 sub_8011330(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x48] == 0) {
        u8 *player = gPlayer;

        if (player[0xc] >> 7) {
            u8 *entry = *(u8 **)(self + 0x18) + 0x68;
            void *addr = self + *(s16 *)entry;
            void *fn = *(void **)(entry + 4);

            _call_via_r1(addr, fn);
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
 * hasn't wrapped past `0x16` yet or the player (`gPlayer`)
 * is in state `+0x88 == 3`; if the orbit is active, the phase has
 * wrapped, and the player isn't in that state, returns immediately.
 *
 * Once past that gate, proceeds only when flags bit 3
 * (the "already hit" latch `DrawExtraLife` clears) is clear and flags
 * bit 2 is set. Builds `self`'s own AABB via `sub_8007B98`, then reads
 * the player's own `+0xa` state: if it's `0x13`, builds the player's
 * *primary* AABB (`sub_8007C30`) and tests it against `self`'s own via
 * `sub_8001688`; on overlap, sets flags bit 3, tail-calls
 * `PickUpWumpa(self, 1)`, and plays a hit SFX
 * (`PlaySfx(gAudioContext, 6, 0x80)`). Otherwise builds the
 * player's *secondary* AABB (`sub_8007B98`, the same helper used for
 * `self`'s own box) and tests it the same way; on overlap, sets flags
 * bit 3 and tail-calls `PickUpWumpa(self, 0)` (no SFX on this path). */
void CheckWumpaPickup(void *selfArg)
{
    u8 *self = selfArg;
    u8 selfBox[16];
    u8 playerBox[16];
    u8 *player;

    if (self[0x4a] != 0 && self[0x4b] <= 0x16) {
        if (((u8 *)gPlayer)[0x88] != 3) {
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
    player = gPlayer;

    if (player[0xa] == 0x13) {
        sub_8007C30(playerBox, player);
        if (sub_8001688(playerBox, selfBox)) {
            register s32 bit asm("r0") = 8;

            bit |= self[0xc];
            self[0xc] = bit;
            PickUpWumpa(self, 1);
            PlaySfx(gAudioContext, 6, 0x80);
        }
    } else {
        sub_8007B98(playerBox, player);
        if (sub_8001688(playerBox, selfBox)) {
            register s32 bit asm("r0") = 8;

            bit |= self[0xc];
            self[0xc] = bit;
            PickUpWumpa(self, 0);
        }
    }
}
/* Trailing byte-padding mismatch fix: the function body isn't a
 * multiple of 4 bytes, and gcc's own default alignment padding (a
 * `nop`/`mov r8, r8` instruction) differs from the ROM's own
 * zero-byte alignment padding at this address - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");

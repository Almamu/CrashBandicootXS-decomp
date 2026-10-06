#include "core.h"
#include "action_obj.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "level.h"
#include "globals.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24): three
 * gActionCtrlStateTable action-table helpers for the player/action object
 * (include/action_obj.h). Not ROM-adjacent to kill_player.c/
 * action_ctrl_left_ground.c (the still-NAKED `ActionCtrlHandleEvent` sits before it,
 * `ApplyActionCtrlMotion` after) - see docs/matching/issue-16-actor-12420.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15/#16
 * NAKED retry (docs/matching/issue-15-16-naked-retry.md): `HandleActionCtrlAirInput`
 * matches as plain C under it, and `TryActionCtrlDoubleJump` since the second retry
 * (docs/matching/issue-15-16-17-naked-retry-2.md), and `UpdateActionCtrl` in a
 * later pass. */

typedef void (*act_fn3)(void *self, s32 a, s32 b, s32 c);

/* Trio stores as in action_ctrl_run_jump.c: as inline parameters, old_agbcc
 * materializes the values before the stores. */
static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = flag;
    self->motionX = next;
}

static inline void ActTrio28(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->motionYKeepSpeed = cur;
    self->motionYPending = flag;
    self->motionY = next;
}

static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = 1;
    self->motionX = next;
}

static inline void ActSet27(struct act *self, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    self->motionX = next;
}

static inline void ActSet28(struct act *self, s32 next)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    self->motionY = next;
}

static inline void ActSetNext27P(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    *slot = next;
}

static inline void ActHold27P(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 1;
    self->motionXPending = 1;
    *slot = next;
}

/* Byte fields of the part read/written through an offset parameter, which
 * keeps old_agbcc from reusing an earlier 0x100 constant for the address
 * (the ROM rematerializes it). */
static inline u8 PartByte(struct player *part, s32 offset)
{
    return *((u8 *)part + offset);
}

static inline u8 *PartBytePtr(struct player *part, s32 offset)
{
    return (u8 *)part + offset;
}

/* Clears `slamBlocked` (+0x34) once R (`gKeys` bit `0x100`) is
 * released. If `part->slippery` (+0x100) changed since last frame
 * (`prevSlippery`), re-runs `sub_8012238`. Then, using
 * the height of `gLevelLayers`'s layer 0 (pixels, `<< 8` to Q8) as
 * the anchor, checks `part->field_04` against two thresholds: past the near
 * one, resets `part`'s `+0x48`/`+0x4c`/`+0x50` velocity-target fields
 * (and `+0x60` unless slippery); past the far one, additionally
 * clears `part+0x8c` and fires a state-close call (`SetMaskLevel`) plus
 * `_call_via_r4` through the `self+0xc` manager's `+0x10`/`+0x14`
 * trampoline slot. Decrements `self+0x26` if set. While `bumpTimer`'s (+0x2b)
 * countdown is running and `part+0x94 <= 1`, ticks it down and, on
 * reaching zero, resets the trio's first half (`+0x31`/`+0x2f`/`+0x27`/
 * `+0x2c`) if `+0x27` was already clear, then always clears the
 * player's `bumped` (+0x90). Looks up `self+8`'s type in
 * `gActionCtrlStateTable`'s 8-byte-per-slot table - a `{s16 baseOffset;
 * s16 count; s16 recordOffset; s32 fallback}` record - to build the
 * arguments for one `_call_via_r3` trampoline call. If `part+0x68` bit 3
 * got cleared this call and `self+0x28` is `4`/`5`, resets the trio's
 * second half; either way calls `ApplyActionCtrlMotion`, then (unless `part+0xc`
 * bit 7 is set) resets `part`'s `+0x48`/`+0x4c`/`+0x50`/`+0x60` fields
 * again. Finally writes a small fixed value into `part+0xa` from a
 * second, 22-case jump table on the same type. */
/* 0x100 through an empty asm (no code): the ROM materializes the input
 * mask straight into the `ands` operand register, which old_agbcc only
 * does when it can't share the constant with the later 0x100 offsets. */
static inline s32 K100(void)
{
    s32 k;

    asm("" : "=r"(k) : "0"(0x100));
    return k;
}

/*
 * Formerly NAKED. What it took (docs/matching/issue-15-16-naked-retry.md,
 * later pass):
 * - each `self->part` re-read gets its own local, and both camera tests
 *   read `y` into a local first (the ROM loads it before the camera
 *   chain);
 * - `unk_94` goes through an `s32` local, which keeps the signed `bgt`
 *   (a direct `u8 <= 1` compare is shortened to unsigned);
 * - the queued-state store is `ActQueue27` (the queued byte is read
 *   before the stores);
 * - the switch cases are in the ROM's body order.
 * With those, the PMF call's method record lands in the ROM's 8-byte
 * stack slot on its own.
 */
void UpdateActionCtrl(struct act *self)
{
    u32 in = gKeys.all;

    if (self->slamBlocked != 0) {
        u16 held = in & K100();

        if (held == 0)
            self->slamBlocked = held;
    }
    if (self->prevSlippery != PartByte(self->part, 0x100))
        sub_8012238(self);
    self->prevSlippery = PartByte(self->part, 0x100);
    {
        struct player *part = self->part;
        s32 py = part->y;

        if (py > (gLevelLayers->layer0->heightPx << 8) - 0x1400) {
            part->flags.all &= 0x7F;
            {
                struct player *q = self->part;

                if (PartByte(q, 0x100) == 0)
                    q->speedX = 0;
                q->rampX.start = 0;
                q->rampX.step = 0;
                q->rampX.target = 0;
            }
            {
                struct player *r = self->part;
                s32 py2 = r->y;

                if (py2 > (gLevelLayers->layer0->heightPx << 8) + 0x1400) {
                    r->deadline = 0;
                    SetMaskLevel(gLevelState, 0);
                    {
                        struct act_method *m = &self->vt->m10;

                        ((act_fn3)m->fn)((u8 *)self + m->thisOffset, 0, 1, 0);
                    }
                }
            }
        }
    }
    if (self->spinCooldown)
        self->spinCooldown--;
    {
        u8 t = self->bumpTimer;
        s32 v94;

        if (t != 0 && (v94 = self->part->listCount, v94 <= 1)) {
            u8 left = --self->bumpTimer;

            if (left == 0) {
                if (self->motionX == 0) {
                    ActQueue27(self, left, self->bumpedMotionX);
                    self->bumpedMotionX = left;
                }
                gPlayer->bumped = left;
            }
        }
    }
    {
        struct act_method m;
        void (*fn)(void *);
        s32 index = gActionCtrlStateTable[self->state].index;
        s32 off;

        if (index > 0) {
            m = (*(struct act_method **)((u8 *)self + gActionCtrlStateTable[self->state].u.vtableOffset))[index - 1];
            fn = m.fn;
        } else {
            fn = gActionCtrlStateTable[self->state].u.fn;
        }
        off = gActionCtrlStateTable[self->state].thisOffset;
        {
            s32 d;

            if (index > 0)
                d = m.thisOffset + off;
            else
                d = off;
            fn((u8 *)self + d);
        }
    }
    self->part->hitAxes &= 8;
    if (self->part->hitAxes == 8) {
        u8 *slot = &self->motionY;

        if (*slot == 4 || *slot == 5) {
            self->motionYKeepSpeed = 0;
            self->motionYPending = 1;
            *slot = 0;
        }
    }
    ApplyActionCtrlMotion(self);
    {
        struct player *part = self->part;
        u32 top = part->flags.all >> 7;

        if (top == 0) {
            if (PartByte(part, 0x100) == 0)
                part->speedX = top;
            part->rampX.start = top;
            part->rampX.step = top;
            part->rampX.target = top;
        }
    }
    switch (self->state) {
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x21:
        self->part->kind = 0x13;
        break;
    case 0xC:
        self->part->kind = 0x14;
        break;
    case 0x18:
        self->part->kind = 0x15;
        break;
    case 0x19:
        self->part->kind = 0x16;
        break;
    default:
        self->part->kind = 1;
        break;
    }
}

/* A helper of `HandleActionCtrlAirInput` (below): if the input snapshot's D-pad bit
 * `1` is set, `self+0x18`'s counter is 0, `gLevelState` passes
 * `HasDoubleJump`, and a sub-object type of `6`/`0xb`/`0xc` (each with its
 * own extra `+0x30 >= 0` gate) matches, bumps `self+0x18`, fires the
 * `+0x50`/`+0x54` and `+0x20`/`+0x24` trampoline pairs with type-keyed
 * ids, resets the state/flag/table-index trio to a type-keyed value,
 * plays a fixed sound, and returns 1; otherwise returns 0.
 *
 * Formerly NAKED (docs/matching/issue-15-16-17-naked-retry-2.md): the
 * tag tests read `self->part` each time instead of through a local - GCSE
 * turns the reloads into the ROM's copy of the pointer in r2 - and the
 * `pressed & 1` test is folded into `pressed`'s assignment, which puts
 * the constant after `one` as in the ROM. */
u8 TryActionCtrlDoubleJump(struct act *self)
{
    u32 in = gKeys.all;
    u16 pressed;
    s32 one;

    if (self->state == 0xE)
        return 0;
    pressed = INPUT_PRESSED(in) & 1;
    one = 1;
    if (pressed) {
        s32 frame = self->frame;

        if (frame == 0 && (u8)HasDoubleJump(gLevelState)) {
            if (self->part->tag == 6 && self->part->frame >= 0) {
                self->frame++;
                *PartBytePtr(gPlayer, 0x100) = frame;
                ACT_CALL2(self, m50, self->part, 0x12);
                ACT_CALL1(self, m20, 9);
                ACT_CALL2(self, m50, self->part, 6);
                ActTrio27(self, frame, one, 0xD);
                ActTrio28(self, frame, one, 0xD);
                PlaySfx(gAudioContext, 0xC, 0x100);
                return 1;
            } else if (self->part->tag == 0xB && self->part->frame >= 0) {
                self->frame++;
                ACT_CALL1(self, m20, 0xB);
                ACT_CALL2(self, m50, self->part, 0xA);
                ActSet27(self, 0xE);
                ActSet28(self, 0xE);
                PlaySfx(gAudioContext, 0xC, 0x100);
                return 1;
            } else if (self->part->tag == 0xC) {
                self->frame++;
                ACT_CALL1(self, m20, 0xB);
                ACT_CALL2(self, m50, self->part, 0xA);
                ActSet27(self, 0xC);
                ActSet28(self, 0xC);
                PlaySfx(gAudioContext, 0xC, 0x100);
                return 1;
            }
        }
    }
    return 0;
}

/* A proximity-triggered indicator: if `part->field_0x64` is within
 * `0x27f` (or, failing that, `TryActionCtrlDoubleJump` fires), dispatches on
 * `self+8`'s type (`7`/`9`/`0xb`/`0xe`, each with its own distance
 * threshold against `part->field_0x64`/its negation) to set `part+0xd`
 * bit 0 and fire the `+0x20`/`+0x24` trampoline with a fixed id
 * (`0x1a`), or (type `0xe`) tail-call `sub_80151C8`. Then, unless the
 * type is `7`/`9`/`0xb`/`0xe`/`0x1a`, reads the D-pad and remaps
 * `self+0x27`'s table-index byte through a further small dispatch
 * (types `9`/`0x1c`-`0x1d` fire `_call_via_r2`/`ActionCtrlStateFlipBodySlamStart` variants,
 * type `7` fires a `_call_via_r3` pair) before a shared tail that sets
 * `self+0x31` when the player's `+0x100` flag is set.
 *
 * Matched under old_agbcc. The distance tests are two separate `if`s (one
 * `||` gets folded into a single compare); the trio stores mix literal
 * stores with the parameter-passing inlines exactly where the ROM
 * materializes the constants early. */
void HandleActionCtrlAirInput(struct act *self)
{
    u8 near = 0;
    u32 in;

    if (self->part->speedY <= 0x27F) {
        near = 1;
        if (TryActionCtrlDoubleJump(self))
            return;
    }
    in = gKeys.all;
    {
        struct player *part;

        if (self->state == 7) {
            part = self->part;
            if (-part->speedY > 0x1BF)
                goto done;
            if (-part->speedY > 0x17F)
                goto done;
            goto hit;
        } else if (self->state == 9) {
            part = self->part;
            if (-part->speedY > 0x1BF)
                goto done;
            if (-part->speedY > 0x7F)
                goto done;
            goto hit;
        } else if (self->state == 0xB) {
            part = self->part;
            if (-part->speedY > 0xFF)
                goto done;
            if (-part->speedY > 0x1F)
                goto done;
        hit:
            ActOrFlags0D(part, 1);
            ACT_CALL1(self, m20, 0x1A);
        } else if (self->state == 0xE) {
            if (self->unk_22) {
                s32 x;

                part = self->part;
                x = part->speedY;
                if (-x <= 0x7F)
                    ActOrFlags0D(part, 1);
                if (x > 0)
                    sub_80151C8(self);
            }
        }
    }
done:
    if (self->state != 0xE && self->state != 0xB && near && (in & 0x100)) {
        u8 busy = self->slamBlocked;

        if (busy == 0) {
            u8 tag;

            ACT_PART_FLAGS0D(self->part) |= 1;
            tag = self->prevState;
            if (tag == 9 || self->state == 9) {
                ACT_CALL1(self, m20, 0xA);
                self->motionXKeepSpeed = busy;
                self->motionXPending = 1;
                self->motionX = busy;
                ActTrio28(self, busy, 1, 0x16);
                ActionCtrlStateFlipBodySlamStart(self);
                self->unk_2A = busy;
                return;
            }
            if (tag == 7) {
                ACT_CALL1(self, m20, 8);
                ACT_CALL2(self, m50, self->part, 0x19);
                self->motionXKeepSpeed = busy;
                self->motionXPending = 1;
                self->motionX = busy;
                ActTrio28(self, busy, 1, 0x15);
            }
        }
    }
    if (self->state == 7 || self->state == 9 || self->state == 0xB || self->state == 0xE || self->state == 0x1A) {
        if (GetDpadDirection(gInput) <= 2) {
            ActQueue27(self, 0, 0);
        } else {
            u8 *slot = &self->motionX;

            if (*slot == 0x1B || *slot == 0x1C)
                ActHold27P(self, slot, 0x1C);
            else if (self->state == 0xE)
                ActSetNext27P(self, slot, 7);
            else if (self->frame != 0) {
                if (*slot != 0xD)
                    ActSetNext27P(self, slot, 0xD);
            } else
                ActSetNext27P(self, slot, 7);
        }
        if (gPlayer->slippery)
            self->motionXKeepSpeed = 1;
    }
}
/* Trailing byte count isn't a multiple of 4 - pad with zeros, not a nop. */
asm(".align 2, 0");

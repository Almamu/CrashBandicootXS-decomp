#include "core.h"
#include "match.h"
#include "action_obj.h"
#include "actor.h"
#include "vtable.h"
#include "system.h"
#include "audio.h"
#include "crates.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #17, ROM 0x08014674-0x08014F8C, formerly
 * asm/code_3_2_17_14674.s (details in
 * docs/matching/archive/issue-17-0x08012fbc-actor.md, "Second pass").
 *
 * More gActionCtrlStateTable action-table handlers for the player/action
 * object (include/action_obj.h). Built with old_agbcc. */


/* codegen: UpdatePlayerFacing returns s32 (player.h, and its definition
 * in kill_player.c only matches that way), but ActionCtrlStateHangMove
 * tests the result as a u8 (`lsl #0x18`). docs/headers_plan.md */
extern u8 UpdatePlayerFacing_u8(struct act *self) asm("UpdatePlayerFacing");

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio (as ActSetNext) */
static inline void ActSetNext27(struct act *self, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    self->motionX = next;
}

/* The same with the +0x31 value as a parameter too: both are materialized
 * before the stores. */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = 1;
    self->motionX = next;
}

static inline void SetTag(struct player *part, s32 tag)
{
    part->tag = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
}

static inline void ActTrio28(struct act *self, s32 a, s32 b, s32 c)
{
    self->motionYKeepSpeed = a;
    self->motionYPending = b;
    self->motionY = c;
}

static inline void ActHold27(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = next;
    *slot = next;
}

/* On contact (part+0x68 bit 3), picks the landing action from the part's
 * tag (0xD: the +0x29-gated landing, 0x18: re-arm +0x29) or state 0xE;
 * otherwise handles the fire/alt/shoulder inputs and, with the D-pad
 * idle, clears the +0x31/+0x2F/+0x27 trio.
 *
 * The tag test is a `switch` with a shared 0xD/0x18 case: the ROM's
 * `beq` for 0xD is threaded past the inner re-test of 0xD while the
 * 0x18 path keeps it, which an `||` test does not reproduce (see
 * docs/matching/archive/mix-naked-retry-5.md). */
void ActionCtrlStateLeftGround(struct act *self)
{
    u8 hit = self->part->hitAxes & 8;

    if (hit) {
        u8 tag;
        /* the ROM's r5 zero, reused by the state-0xE else trio; without
         * it that trio stores the `& 0x30` result register */
        u8 z;

        ACT_PART_FLAGS0D(self->part) |= 1;
        {
            u8 *p34 = &self->slamBlocked;

            z = 0;
            *p34 = z;
        }
        switch (tag = self->part->tag) {
        case 0xD:
        case 0x18:
            if (tag == 0xD) {
                if (self->turboRun) {
                    self->frame = 0;
                    ACT_VCALL2(self, m50, self->part, 0x18);
                    ACT_VCALL1(self, m20, 4);
                    ActQueue27(self, 0, 0x1B);
                } else {
                    self->frame = 0;
                    ACT_VCALL1(self, m20, 3);
                    {
                        u8 *slot = &self->motionX;

                        if (*slot != 1)
                            ActHold27(self, slot, 1);
                    }
                }
                ActSetNext(self, 0);
            } else if (tag == 0x18) {
                ACT_VCALL1(self, m20, 4);
                self->motionYKeepSpeed = 0;
                self->motionYPending = 1;
                self->motionY = 0;
                self->turboRun = 1;
                ActQueue27(self, 0, 0x1B);
            }
            break;
        default:
            if (self->state == 0xE) {
                if (gKeys.all & 0x30) {
                    ActQueue27(self, 0, 1);
                } else {
                    self->motionXKeepSpeed = z;
                    self->motionXPending = 1;
                    self->motionX = z;
                }
                ActSetNext(self, 0);
                ACT_VCALL1(self, m20, 0xD);
            } else {
                ACT_VCALL1(self, m20, 0);
                self->motionYKeepSpeed = z;
                self->motionYPending = 1;
                self->motionY = 0;
                self->motionXKeepSpeed = 0;
                self->motionXPending = 1;
                self->motionX = 0;
            }
        }
        return;
    }
    {
        u32 in = gKeys.all;
        s32 fire;
        s32 one;
        u16 p;

        p = INPUT_PRESSED(in);
        one = 1;
        /* a fresh 1 for `fire` (the ROM's `movs r3, #1; ands r3, r1`),
         * not a copy of `one` */
        MATCH_CONST(fire, 1);
        fire &= p;
        /* extra reference: keeps `one` in r6 and the input pointer in r7 */
        MATCH_USE(one);
        if (fire) {
            ACT_VCALL1(self, m20, 5);
            ACT_VCALL2(self, m50, self->part, 0x13);
            self->frame = hit;
            ActTrio28(self, hit, one, 7);
        } else {
            u16 alt;
            s32 t = 2;

            t &= p;
            alt = t;

            if (alt) {
                ActOrFlags0D(self->part, 1);
                self->slamBlocked = fire;
                if (gKeys.all & 0x30) {
                    self->motionXKeepSpeed = fire;
                    self->motionXPending = one;
                    self->motionX = one;
                } else {
                    self->motionXKeepSpeed = 0;
                    self->motionXPending = one;
                    self->motionX = 0;
                }
                if ((u32)(self->state - 0xD) > 1)
                    StartActionCtrlSpin(self);
                else
                    ACT_VCALL1(self, m20, 0xD);
            } else if (INPUT_HELD(in) & 0x100) {
                ActOrFlags0D(self->part, 1);
                self->slamBlocked = alt;
                ACT_VCALL1(self, m20, 0x10);
                ACT_VCALL2(self, m50, self->part, 3);
                self->frames = alt;
                self->motionXKeepSpeed = alt;
                self->motionXPending = one;
                self->motionX = alt;
            }
        }
    }
    {
        u8 dir = GetDpadDirection(gInput);

        if (dir == 0) {
            self->motionXKeepSpeed = dir;
            self->motionXPending = 1;
            self->motionX = dir;
        }
    }
}

void ActionCtrlStateDying(struct act *self)
{
    struct player *part = self->part;

    if (part->tag == 0x2F && part->frame == 3 && part->stepTimer == 0)
        PlaySfx(gAudioContext, 0x2E, 0x100);
    part = self->part;
    if (part->animDone) {
        part->flags.all |= 1;
        {
            /* the "mark part gone" bitmap set of cortex.c's
             * MARK_GONE_BITMAP, with the same load-bearing registers */
            MATCH_HOLD_REG(s32, none, r0) = 0xFFFF;
            MATCH_HOLD_REG(u32, cur, r4) = part->id;

            if (cur != none) {
                MATCH_HOLD_REG(s32, id, r3) = *(vu16 *)&part->id;
                MATCH_HOLD_REG(u8 *, base, r2) = (u8 *)gEntityFlags;
                MATCH_HOLD_REG(s32, word, r0) = id;
                s32 off;
                u32 *slot;

                /* a signed shift: hidden from gcc's "a u16 is never
                 * negative" folding, which would make it lsr */
                MATCH_KEEP(word);
                word >>= 5;
                off = word * 4;
                slot = (u32 *)(base + 0x108);
                slot = (u32 *)((u8 *)slot + off);
                word = id - (word << 5);
                *slot |= 1 << word;
            }
        }
    }
}

void ActionCtrlStateWarpIn(struct act *self)
{
    if (self->part->animDone) {
        *((u8 *)gPlayer + 0xC) |= 0x80;
        SetActionCtrlModeAnim(self, 0, 0x12, 0, 0);
        self->motionXKeepSpeed = 0;
        self->motionXPending = 1;
        self->motionX = 0;
        self->motionYKeepSpeed = 0;
        self->motionYPending = 1;
        self->motionY = 0;
        LoadPaletteSlot(gPaletteCache, self->part->slot,
                        self->part->anim->records[self->part->tag].paletteId);
    }
}

void ActionCtrlStateHang(struct act *self)
{
    u8 dir = GetDpadDirection(gInput);
    u32 in = gKeys.all;

    if (dir != 0)
        switch (dir) {
        case 3 ... 8:
            ActSetNext27(self, 0x20);
            ACT_VCALL1(self, m20, 0x25);
            ACT_VCALL2(self, m50, self->part, 0x20);
            break;
        }
    if (INPUT_PRESSED(in) & 1) {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ActionCtrlReleaseHang(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartActionCtrlHangSpin(self);
        UpdatePlayerFacing(self);
    } else {
        UpdatePlayerFacing(self);
    }
}

void sub_8014AEC(struct act *self)
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;

    if (fire) {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ActionCtrlReleaseHang(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartActionCtrlHangSpin(self);
        UpdatePlayerFacing(self);
        self->motionXKeepSpeed = fire;
        self->motionXPending = 1;
        self->motionX = fire;
    }
}

/* Starts the jump: clears part+0x101, lifts the part by 6 px (0x600 Q8),
 * sets animations 0x1A/0x1B, holds the part on its last frame and queues
 * action 4.
 *
 * The 0x600 is a reload, and the ROM loads it into r3. When reload
 * picks a spill register for that insn, r2 and r3 are both free, and it
 * takes the lower one (r2). Every later reload then rotates through the
 * spill-register set {1,2,6} instead of the ROM's {1,3,6}. `hold` is a
 * register variable in r2, set and used only by empty asms (no code).
 * It keeps r2 live across the add, so reload spills r3 there instead
 * (docs/matching/archive/late-naked-retry-3.md). */
void ActionCtrlReleaseHang(struct act *self)
{
    struct player *part;
    s32 count;
    MATCH_HOLD_REG(s32, hold, r2);

    self->part->hanging = 0;
    MATCH_HOLD(hold); /* r2 live from here: no code */
    self->part->y += 0x600;
    MATCH_USE(hold); /* ...to here, so the 0x600 reload takes r3 */
    ACT_VCALL1(self, m20, 0x1A);
    ACT_VCALL2(self, m50, self->part, 0x1B);
    part = self->part;
    count = part->anim->records[part->tag].frameCount;
    part->frame = count - 1;
    ActSetNext(self, 4);
}

/* Crouch/aim handler: fire jumps (ActionCtrlReleaseHang), alt hands off to
 * StartActionCtrlHangSpin, an idle D-pad plays animations 0x28/0x22, a sideways one
 * queues action 0x20 on the +0x31/+0x2F/+0x27 trio, and at the end of
 * the animation it replays 0x26/0x21 from frame 5.
 *
 * The alt and idle trios are written out twice (gcc cross-jumps them into
 * the ROM's one block), so each gets the fire test's 1 from CSE; the
 * method calls use ACT_CALL (include/action_obj.h). */
void ActionCtrlStateHangMoveStart(struct act *self)
{
    void *pad = gInput;
    u32 in = gKeys.all;
    s32 v = INPUT_PRESSED(in) & 1;

    if (v) {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ActQueue27(self, 0, 0);
        ActionCtrlReleaseHang(self);
        return;
    }
    if (INPUT_PRESSED(in) & 2) {
        StartActionCtrlHangSpin(self);
        UpdatePlayerFacing(self);
        self->motionXKeepSpeed = 0;
        self->motionXPending = 1;
        self->motionX = 0;
        return;
    }
    v = GetDpadDirection(pad);
    if (v == 0) {
        ACT_CALL1(self, m20, 0x28);
        ACT_CALL2(self, m50, self->part, 0x22);
        self->motionXKeepSpeed = 0;
        self->motionXPending = 1;
        self->motionX = 0;
        return;
    }
    {
        u8 cur = self->motionX;

        if (cur == 0) {
            switch (v) {
            case 3 ... 8:
                ActQueue27(self, cur, 0x20);
            }
        }
        if (self->part->animDone) {
            struct player *part;
            s32 zero = 0;
            s32 frame;
            s32 count;

            ACT_CALL1(self, m20, 0x26);
            ACT_CALL2(self, m50, self->part, 0x21);
            self->frame = zero;
            part = self->part;
            frame = 5;
            count = part->anim->records[part->tag].frameCount;
            if (frame >= count)
                frame = count - 1;
            part->frame = frame;
            ActQueue27(self, zero, 0x20);
        }
    }
    UpdatePlayerFacing(self);
}

/* Walk handler: retags a finished part (0x21), handles fire/alt like
 * ActionCtrlStateHangMoveStart, steps a 4-frame idle timer that picks animation 0x22/0x23
 * from the part's frame, and while UpdatePlayerFacing reports a step moves the
 * part by the GetSpriteFrame record's (or gEmptySpritePoint's) X offset,
 * mirrored by part+0x28 bit 4.
 *
 * The idle dispatch is written out per case: the ROM's one shared
 * _call_via_r3 call and trio are gcc's cross-jumping of the identical
 * tails, and the 1 the trios store comes from the fire test's constant in
 * r7, which CSE only carries into single-predecessor blocks. The method
 * calls use ACT_CALL (see include/action_obj.h). The record kind is
 * switched on 0..6 with separate case bodies, which is what makes gcc
 * emit the ROM's jump table. */
void ActionCtrlStateHangMove(struct act *self)
{
    u8 dir = GetDpadDirection(gInput);
    u32 in = gKeys.all;
    struct player *part = self->part;
    s32 fire;
    u16 alt;

    if (part->animDone)
        SetTag(part, 0x21);
    fire = INPUT_PRESSED(in) & 1;
    if (fire) {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ActQueue27(self, 0, 0);
        ActionCtrlReleaseHang(self);
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartActionCtrlHangSpin(self);
        UpdatePlayerFacing(self);
        self->motionXKeepSpeed = fire;
        self->motionXPending = 1;
        self->motionX = fire;
        return;
    }
    if (dir == 0) {
        if (++self->frame > 3) {
            s32 f;

            self->frame = alt;
            f = self->part->frame;
            if (f == 0) {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x22);
                self->motionXKeepSpeed = alt;
                self->motionXPending = 1;
                self->motionX = alt;
            } else if (f <= 4) {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x23);
                self->motionXKeepSpeed = alt;
                self->motionXPending = 1;
                self->motionX = alt;
            } else if (f > 9) {
                ACT_CALL1(self, m20, 0x28);
                ACT_CALL2(self, m50, self->part, 0x22);
                self->motionXKeepSpeed = alt;
                self->motionXPending = 1;
                self->motionX = alt;
            } else {
                self->motionXKeepSpeed = alt;
                self->motionXPending = 1;
                self->motionX = alt;
            }
        }
    } else {
        self->frame = alt;
        ActQueue27(self, alt, 0x20);
    }
    if (UpdatePlayerFacing_u8(self)) {
        u8 *info = GetSpriteFrame((struct gfx_part *)self->part);
        s32 x;
        s32 y;

        switch (**(u8 **)(info + 4) >> 4) {
        case 0:
            info += 0x24;
            break;
        case 1:
            info = (u8 *)&gEmptySpritePoint;
            break;
        case 2:
            info = (u8 *)&gEmptySpritePoint;
            break;
        case 3:
            info = (u8 *)&gEmptySpritePoint;
            break;
        case 4:
            info = (u8 *)&gEmptySpritePoint;
            break;
        case 5:
            info = (u8 *)&gEmptySpritePoint;
            break;
        case 6:
            info += 0x14;
            break;
        default:
            info = (u8 *)&gEmptySpritePoint;
            break;
        }
        x = self->part->x >> 8;
        y = self->part->y;
        if ((s8)(self->part->mirror.all << 3) < 0)
            x += *(s16 *)info;
        else
            x -= *(s16 *)info;
        self->part->x = x << 8;
        self->part->y = y;
    }
}

void ActionCtrlStateHangStop(struct act *self)
{
    u32 in = gKeys.all;
    s32 fire = INPUT_PRESSED(in) & 1;
    u16 alt;

    if (fire) {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ActSetNext27(self, 0);
        ActionCtrlReleaseHang(self);
        return;
    }
    alt = INPUT_PRESSED(in) & 2;
    if (alt) {
        StartActionCtrlHangSpin(self);
        UpdatePlayerFacing(self);
        self->motionXKeepSpeed = fire;
        self->motionXPending = 1;
        self->motionX = fire;
        return;
    }
    if (self->part->animDone) {
        ACT_VCALL1(self, m20, 0x20);
        ACT_VCALL2(self, m50, self->part, 0x1F);
        self->frame = alt;
        self->frames = alt;
    }
}
asm(".align 2, 0");

/* GitHub issue #18's chunk, ROM 0x08014F8C-0x080157C0 - continues the
 * same "self" action-table object family documented at the top of
 * action_ctrl_states.c (`self+0xc` a per-category `{s16 offset; void *fn}`
 * table, `self+0x10` a `struct actor *` sub-object, `self+0x27`-`0x32` a
 * shared state/flag/table-index trio) - `StartActionCtrlHighJump`/`SetActionCtrlModeAnim` are
 * both called directly by `ActionCtrlStateStandUp`/`ActionCtrlStateCrawlStart` there, confirming
 * the same object shapes carry over. `gCollidableList` (only touched
 * by `DoSuperBodySlamShockwave` here) is a small list object - `+4` a count, `+0xc`
 * a `struct actor **` array - not referenced by any already-matched
 * code yet, so it stays raw-offset rather than a guessed struct. This
 * file covers `DoSuperBodySlamShockwave` and `StartActionCtrlTornadoSpin` (both matched); the chunk continues in action_ctrl_moves.c and action_ctrl.c, split
 * at each parked function's raw-asm gap - see docs/matching/
 * issue-18-0x08014f8c-actor.md for the full write-up. */

extern s32 _call_via_r1(void *addr, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* For each `struct actor *` in the `gCollidableList` list: skips
 * entries whose `+0x48` trampoline (`_call_via_r1`) reports a width of 4
 * or less, entries further than 0x40 (Manhattan distance) from `self`'s
 * own part, entries without their `+0xc` bit 6 flag set, and entries
 * more than 0x11 away vertically - then fires the `+0x68` trampoline
 * pair via `_call_via_r4` with action `0x16` on whatever survives all
 * four checks. */
void DoSuperBodySlamShockwave(struct act *self)
{
    struct actor *part;
    MATCH_HOLD_REG(s32, threshold, r8);
    s32 px, py;
    s32 i;

    part = *(struct actor **)((u8 *)self + 0x10);
    BreakCratesInArea(part->x >> 8, part->y >> 8, 0x40, 0x12);
    threshold = 0x40;

    part = *(struct actor **)((u8 *)self + 0x10);
    px = part->x >> 8;
    py = part->y >> 8;

    i = 0;
    goto loop_cond;

loop_body:
    {
        u8 *list;
        struct actor *other;
        u8 *rec;
        s16 offset;
        void *addr;
        void *fn;
        MATCH_HOLD_REG(s32, dx, r1);
        MATCH_HOLD_REG(s32, dy, r2);

        /* Anti-CSE: a plain re-read of `gCollidableList` here would
         * let gcc reuse the register value the loop condition below
         * just loaded, across the branch - the ROM reloads it again
         * from scratch inside the body instead (see matching.md's
         * `CountSapphireRelics`/`CountGoldRelics` entry for the general technique).
         * Both this and the condition's read go through the same
         * hand-placed literal-pool word (`.Lgu12f0_8014f8c`, emitted
         * once right after this function) via a real two-instruction
         * `ldr`/`ldr` rather than gcc's own per-use pool management,
         * since letting gcc manage it here would either still let it
         * CSE the address across the branch, or (if forced fresh some
         * other way) emit a *second*, redundant pool word instead of
         * reusing the condition's - the ROM's own single-word pool
         * layout for this symbol needs exactly one entry shared by
         * both `ldr` sites, matching how the two loads share one
         * literal in the ROM (`_08015034` referenced from both
         * `_08014FB8` and `_0801501E`). */
        asm volatile("ldr %0, .Lgu12f0_8014f8c\n\tldr %0, [%0]" : "=r"(list));
        other = (*(struct actor ***)(list + 0xc))[i];
        rec = (u8 *)other->table + 0x48;
        offset = *(s16 *)rec;
        addr = (u8 *)other + offset;
        fn = *(void **)(rec + 4);

        if (_call_via_r1(addr, fn) <= 4) {
            goto loop_inc;
        }

        {
            MATCH_HOLD_REG(s32, sign, r0);

            dx = (other->x >> 8) - px;
            sign = dx >> 31;
            dx ^= sign;
            dx -= sign;

            sign = (other->y >> 8) - py;
            dy = sign >> 31;
            sign ^= dy;
            dy = sign - dy;

            dx += dy;
        }
        if (dx > threshold) {
            goto loop_inc;
        }
        {
            MATCH_HOLD_REG(u8, flagsVal, r1) = other->flags;
            MATCH_HOLD_REG(s32, bit, r0) = flagsVal >> 6;
            MATCH_HOLD_REG(s32, one, r1) = 1;

            bit &= one;
            if (bit == 0) {
                goto loop_inc;
            }
        }
        if (dy > 0x11) {
            goto loop_inc;
        }

        {
            u8 *rec2 = (u8 *)other->table + 0x68;
            s16 offset2 = *(s16 *)rec2;
            void *addr2 = (u8 *)other + offset2;
            MATCH_HOLD_REG(void *, fn2, r4) = *(void *volatile *)(rec2 + 4);

            _call_via_r4(addr2, 0, 0x16, 0);
            (void)fn2;
        }
    }

loop_inc:
    i++;
loop_cond:
    {
        void *listVal;

        asm volatile("ldr %0, .Lgu12f0_8014f8c\n\tldr %0, [%0]" : "=r"(listVal));
        if (i < *(s32 *)((u8 *)listVal + 4)) {
            goto loop_body;
        }
    }
}
asm(".align 2, 0\n\t.Lgu12f0_8014f8c: .word gCollidableList");

/* Same `mgr`/`{s16 offset; void *fn}` trampoline pair at `self+0xc`
 * (`+0x20`/`+0x24` and `+0x50`/`+0x54`) as `ActionCtrlStateStandUp`/`ActionCtrlStateCrawlStart`.
 * `self+0x24` selects one of two variants: while clear, picks a
 * table-index (`+0x21`) from `self+0x22` (1->0x28, 2->0x27, default
 * 0x17), stores it back, fires both trampolines with `id`/that index,
 * resets `self+0x18`/`0x1c` to `0`/`0x14`, plays a sound keyed off the
 * new `+0x21`, then bumps `self+0x22` and - once it reaches `self+0x20`
 * - latches `self+0x24` and clamps `self+0x22` to `0`/`1`. While set,
 * either repeats the same shape with a fixed `+0x21` from `self+0x22`
 * (unless `self+0x22` is already above `0xf0`, i.e. wrapped) or, once
 * wrapped, fires a third fixed-index variant (using `param2` as the
 * mgr's `+0x20` trampoline argument instead of `id`) and stamps
 * `self+0x26` with `0x63`.
 *
 * Built with old_agbcc (the file is on OLD_AGBCC_OBJS). The
 * `self+0x24 != 0` arm tests `self[0x22]` directly rather than through a
 * `u8` local: the byte load then comes after the point where
 * old_agbcc's GCSE inserts its copy of `self + 0x22` (end of the block,
 * before the compare), so the load goes through the copy in r5
 * (`adds r5, r0, #0; ldrb r2, [r5]`) as in the ROM. */
void StartActionCtrlTornadoSpin(struct act *self, s32 id, s32 param2)
{
    struct vtable_slot *mgr;
    u8 *off;

    if (self->unk_24[0] == 0) {
        s32 idx = 0x17;
        s32 zero;
        s32 wait;

        self->unk_21 = 0;
        if (self->unk_22 == 1) {
            idx = 0x28;
            self->unk_21 = 1;
        } else if (self->unk_22 == 2) {
            idx = 0x27;
            self->unk_21 = 2;
        }
        zero = 0;
        wait = 0x14;
        mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)id, mgr[4].fn);
        off = (u8 *)self->vt;
        off += 0x50;
        _call_via_r3((u8 *)self + *(s16 *)off, self->part, (void *)idx, *(void **)(off + 4));
        self->frame = zero;
        self->frames = wait;
        PlaySfx(gAudioContext, self->unk_21 + 0x57, 0x100);
        if (++self->unk_22 >= self->charge) {
            self->unk_24[0] = 1;
            if (self->unk_22 > 1)
                self->unk_22 = 1;
            else
                self->unk_22 = zero;
        }
    } else {
        MATCH_HOLD_REG(s32, hold1, r1);

        /* No code: keeps r1 live across the `self->unk_22` test so the
         * byte loads into r2 and `id` stays in ip, as in the ROM. */
        MATCH_HOLD(hold1);
        if (self->unk_22 > 0xf0) {
            s32 idx;
            s32 zero;
            s32 wait;

            MATCH_USE(hold1); /* end of the r1 hold (no code) */
            idx = 0x17;
            self->unk_21 = 0;
            if (self->unk_22 == 1) {
                idx = 0x28;
                self->unk_21 = 1;
            } else if (self->unk_22 == 2) {
                idx = 0x27;
                self->unk_21 = 2;
            }
            zero = 0;
            wait = 0x14;
            mgr = (struct vtable_slot *)self->vt;
            _call_via_r2((u8 *)self + mgr[4].delta, (void *)id, mgr[4].fn);
            off = (u8 *)self->vt;
            off += 0x50;
            _call_via_r3((u8 *)self + *(s16 *)off, self->part, (void *)idx, *(void **)(off + 4));
            self->frame = zero;
            self->frames = wait;
            PlaySfx(gAudioContext, self->unk_21 + 0x57, 0x100);
        } else {
            u8 *p21 = (u8 *)self + 0x21;
            s32 zero = 0;
            s32 wait;

            *p21 = zero;
            self->charge = zero;
            wait = 0x18;
            mgr = (struct vtable_slot *)self->vt;
            _call_via_r2((u8 *)self + mgr[4].delta, (void *)param2, mgr[4].fn);
            off = (u8 *)self->vt;
            off += 0x50;
            _call_via_r3((u8 *)self + *(s16 *)off, self->part, (void *)0x10, *(void **)(off + 4));
            self->frame = zero;
            self->frames = wait;
            PlaySfx(gAudioContext, 0xa, 0x100);
            self->spinCooldown = 0x63;
        }
        self->unk_22--;
    }
    self->unk_23 = 0;
}

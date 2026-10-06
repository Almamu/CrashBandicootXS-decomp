#include "core.h"
#include "vtable.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #9/#10, tail of the 0x0800B8DC-0x0800D040 cluster (see
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md): the last raw file
 * in the cluster, `asm/code_3_2_17_cbf4.s` - `UpdateEffectCtrl`,
 * `EffectCtrlHandleEvent`, `nullsub_3`, `DestroyEffectCtrl`, `InitEffectCtrl`, ROM
 * 0x0800CBF4-0x0800CD00 (contiguous, no gap on either side -
 * `enemy_ctrl.o`'s `CreateKnockedEnemyCtrl` ends exactly where this file
 * starts, and `crate_touch.o`'s already-matched `PlayerAnimWouldTouchCrate`
 * begins exactly where this file ends). Closes out the entire
 * 43-function cluster investigation that began with `UpdateEnemyCtrl`/
 * `HitEnemy`. */

extern void *_call_via_r1(void *arg0, void *fn);

/* `other` (the second argument - `self`, the first, is never read)
 * shares `struct actor`'s leading header layout (id @8, flags @0xc,
 * table @0x18, same `other->table`-relative `{s16 offset, void *fn}`
 * pair at +0x28/+0x2c that `src/level/room_frame.c`'s `UpdateRoomFrame`
 * already reads via an identical `_call_via_r1` hit-probe call), but
 * is read at +0x38 too - bigger than the 0x1c-byte `struct actor`, so
 * it gets its own `struct cbf4_other` below.
 *
 * Runs the "flag active + bitmap-set" idiom (`other->0xc |= 1`, then,
 * unless `other`'s id sentinel-checks as `0xFFFF`, sets bit
 * `other->8 & 0x1f` of word `other->8 >> 5` in the
 * `gEntityFlags+0x108` bitmap) up to three times, independently
 * gated: once when the `_call_via_r1` hit-probe against `other->table`'s
 * own +0x28/+0x2c pair reports *no* hit, once when `other->0xc` bit 3
 * is already set, and once when `other->0x38` is nonzero. This is the
 * exact idiom `tiny_hop_pad.c`'s `UpdateOneShotAnimCtrl` already matches as
 * real C (its own doc comment: "needs several `register asm` pins ...
 * without them this compiler ... folds the ROM's shift-setup pair ...
 * and CSEs away the ROM's second, seemingly redundant reload") - here
 * inlined three times over (the ROM has no `bl` to a shared helper).
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the whole file
 * matches under it, so effect_ctrl.o is in OLD_AGBCC_OBJS - the
 * constant-before-`ldrb` flag ORs are old_agbcc's tell). `MarkGone` is
 * an inline with the do/while(0) `SET_ID_BIT` that
 * swim_ctrl.c uses (its loop notes give the id reload). The
 * gone bit is set through a bitfield view of +0x0C and bit 3 is tested
 * through the byte view: that is what makes the second copy reuse the
 * tested byte and its `1` for the OR, in the ROM's registers. */
struct cbf4_other {
    u8 unk_00[8];
    u16 id;             // 0x08
    u8 unk_0A[2];
    union {
        u8 flags;       // 0x0C
        struct {
            u8 gone:1;
            u8 unk_1:7;
        } b;            // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    } f;
    u8 unk_10[8];
    struct vtable_slot *table; // 0x18
    u8 unk_1C[0x1C];
    u8 unk_38;          // 0x38
};

#define SET_ID_BIT(idExpr)                                                     \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = (u8 *)gEntityFlags;                                  \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= 1 << (_id - _word * 32);                                     \
    } while (0)

static inline void MarkGone(struct cbf4_other *t)
{
    t->f.b.gone = 1;
    if (t->id != 0xFFFF)
        SET_ID_BIT(t->id);
}

void UpdateEffectCtrl(void *self, struct cbf4_other *other)
{
    if (!(u8)(s32)_call_via_r1((u8 *)other + other->table[5].delta, other->table[5].fn))
        MarkGone(other);
    if ((other->f.flags >> 3) & 1)
        MarkGone(other);
    if (other->unk_38)
        MarkGone(other);
}

/* Genuine empty stubs (`bx lr`). */
void EffectCtrlHandleEvent(void *self)
{
}

void nullsub_3(void *self)
{
}

/* Sets `self+0xc`'s table pointer to `gEffectCtrlVtable`, then
 * tail-calls `DestroyCtrl` - same double-set pattern as
 * `DestroyStompedHopPadCtrl`/`DestroyBossCtrl`/`DestroyMegaMixCtrl`. */
void DestroyEffectCtrl(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = (void *)gEffectCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl`, re-points `self+0xc`'s table pointer at
 * `gEffectCtrlVtable`, and runs `nullsub_3(self)` - the same
 * "reset via `InitCtrl`, re-point `self+0xc`, return `self`"
 * constructor shape already matched for `CreateStompedHopPadCtrl`/`DestroyStompedHopPadCtrl`/
 * `CreateKnockedEnemyCtrl`. */
void *InitEffectCtrl(void *selfArg)
{
    u8 *self = selfArg;

    InitCtrl(self);
    *(void **)(self + 0xc) = (void *)gEffectCtrlVtable;
    nullsub_3(self);
    return self;
}

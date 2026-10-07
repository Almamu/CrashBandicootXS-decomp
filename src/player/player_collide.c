#include "core.h"
#include "match.h"
#include "actor.h"
#include "actor_self.h"
#include "player.h"
#include "objects.h"
#include "level.h"
#include "sprite_bank.h"
#include "globals.h"
#include "math_util.h"

/* GitHub issue #9/#10: 0x0800A884 - the player object's (`struct
 * player`, player.h) collision method. */

/* A per-frame "reentrancy guard"-shaped wrapper (only runs if
 * `self+0xc` bit 7 is set): fires `self->table+0x70`'s trampoline via
 * `_call_via_r1`, then calls `CollideGroundSprite` (still raw) with the global
 * `gLevelLayers+0x2a` flag held set for the duration. If
 * `self+0xac` (a pointer, cleared here) was non-null, sets `self+0x68`
 * bit 3 and clears the `+0x100`/`+0x102`/`+0x103` flag bytes. Then
 * dispatches on `gLevelLayers+0x29` (a pending-action "kind"
 * byte, cleared back to 0 by every path here): kind 0 additionally
 * resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` is exactly 8; kinds
 * 1/5/7/9 (`gEmptySpritePoint`'s index scheme - see the `case`
 * labels below) are no-ops beyond the shared reset; kind 1 also sets
 * `self+0xc` bit 6, clears `+0x8c`, calls `SetMaskLevel`, and fires the
 * `self->table+0x68` trampoline (arg 1); kind 5 sets the `+0x100`
 * flag; kind 7 sets `+0x102`; kind 10 sets `+0x103`. Finally, looks up
 * the current keyframe record (`GetSpriteFrame`, already parked in
 * `sprite_obj.c`) and picks a `{s16 x, s16 y}` offset table off its
 * `+4` byte's upper nibble - the exact same `GetSpriteFrameAnchor`
 * (`sprite_obj.c`) case-to-block mapping (0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else -> the fixed fallback
 * `gEmptySpritePoint`) - applies it (mirrored by `self+0x28` bit 4)
 * to `self`'s de-Q8'd position, and probes the result via
 * `GetTerrainFlagsAt` (still raw). A hit (code 6) snaps `self`'s Y position
 * down to the next multiple of 8 (unless `+0x101` is already set) and
 * fires the table+0x68 trampoline with code `0x17`; any other code
 * fires the same trampoline with code `0x18` if `+0x101` is set.
 * Returns the (possibly just-updated) `self+0x68` state byte.
 *
 * Moved here from asm/code_3_2_16_a884.s as NAKED (issue #9 raw-asm
 * pass); matched in a later pass. Built with old_agbcc (the `movs
 * #0x40`/`movs #8` before their `ldrb`). The methods are gcc 2.x virtual
 * calls through `self+0x18` (`_call_via_r1`/`_call_via_r4`), and the
 * offset-table switch is `GetSpriteFrameAnchor` (sprite_obj.c) inlined. The
 * ROM's "walking" flag offsets (`adds r1, #3`, `subs r2, #3`) are
 * reload's move2add reusing a reload register; they come from r3 holds
 * (no code) that keep reload rotating through r0-r2 only. */

typedef void (*a884_fn0)(void *self);
typedef void (*a884_fn3)(void *self, s32 a, s32 b, s32 c);

#define CALL_M68(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        const struct actor_method *_m = &(obj)->vtable->handleEvent;           \
        ((a884_fn3)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c));       \
    } else (void)0

/* The same dispatch with r3 held (no code) from before the method
 * lookup to after the `this` adjustment: reload skips a live hard
 * register, so the `ldrsh` index reload takes r2 as in the ROM. */
#define CALL_M70H(obj)                                                         \
    if (1) {                                                                   \
        MATCH_HOLD_REG(s32, _h, r3);                                           \
        const struct actor_method *_m;                                         \
        void *_t;                                                              \
        MATCH_HOLD(_h); /* r3 hold starts: no code */                          \
        _m = &(obj)->vtable->collideWithObjects;                               \
        _t = (u8 *)(obj) + _m->thisOffset;                                     \
        MATCH_USE(_h); /* r3 hold ends: no code */                             \
        ((a884_fn0)_m->fn)(_t);                                                \
    } else (void)0

#define CALL_M68H(obj, a, b, c)                                                \
    if (1) {                                                                   \
        MATCH_HOLD_REG(s32, _h, r3);                                           \
        const struct actor_method *_m;                                         \
        void *_t;                                                              \
        MATCH_HOLD(_h); /* r3 hold starts: no code */                          \
        _m = &(obj)->vtable->handleEvent;                                      \
        _t = (u8 *)(obj) + _m->thisOffset;                                     \
        MATCH_USE(_h); /* r3 hold ends: no code */                             \
        ((a884_fn3)_m->fn)(_t, (a), (b), (c));                                 \
    } else (void)0

static inline s16 *A884Offset(void *part)
{
    const struct sprite_frame *info = GetSpriteFrame(part);
    u8 type = info->pieces[0] >> 4;
    MATCH_HOLD_REG(s16 *, result, r3); /* the ROM builds it in r3 */

    switch (type) {
    case 0:
        result = (s16 *)&((const struct sprite_frame_3box_anchor *)info)->anchor;
        break;
    case 3:
    case 4:
        result = (s16 *)&gEmptySpritePoint;
        break;
    case 1:
    case 2:
        result = (s16 *)&gEmptySpritePoint;
        break;
    case 5:
        result = (s16 *)&gEmptySpritePoint;
        break;
    case 6:
        result = (s16 *)&((const struct sprite_frame_1box_anchor *)info)->anchor;
        break;
    default:
        result = (s16 *)&gEmptySpritePoint;
        break;
    }
    return result;
}

u8 CollidePlayer(struct player *self)
{
    if (self->flags.all >> 7) {
        u8 kind;
        s16 *off;
        s32 x, y;
        s32 zero;
        MATCH_HOLD_REG(s32, hold, r3);

        /* Constant-init without live-range doubling (no code). */
        MATCH_CONST(zero, 0);
        self->hitAxes = zero;
        self->cleared = zero;
        CALL_M70H(self);
        self->cleared = 1;
        gLevelLayers->probeFlag = 1;
        CollideGroundSprite((struct box_part *)self);
        /* r3 hold (no code) over the flag resets and the kind switch:
         * the ROM's reloads rotate through r0-r2 only, so the flag
         * offsets reuse one register (`adds r1, #3`, `subs r2, #3`). */
        MATCH_HOLD(hold);
        gLevelLayers->probeFlag = zero;
        if (self->carried != 0) {
            self->hitAxes |= 8;
            self->carried = (struct gobj *)zero;
            self->slippery = zero;
            self->pushLeft = zero;
            self->pushRight = zero;
        }
        kind = gLevelLayers->kind;
        if (kind != 0) {
            switch (kind) {
            case 1:
                self->flags.all |= 0x40;
                {
                    /* The ROM stores a fresh 0 from r0 (address in r1). */
                    u32 *_p = &self->deadline;
                    MATCH_HOLD_REG(s32, _z, r0) = 0;
                    *_p = _z;
                }
                SetMaskLevel(gLevelState, MASK_LEVEL_NONE);
                CALL_M68H(self, 0, EVENT_HIT, 0);
                break;
            case 2:
            case 3:
            case 4:
                break;
            case 5:
                self->pushLeft = 0;
                self->pushRight = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    MATCH_CONST(_one, 1);
                    self->slippery = _one;
                }
                break;
            case 7:
                self->pushRight = 0;
                self->slippery = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    MATCH_CONST(_one, 1);
                    self->pushLeft = _one;
                }
                break;
            case 6:
            case 8:
            case 9:
                break;
            case 10:
                self->pushLeft = 0;
                self->slippery = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    MATCH_CONST(_one, 1);
                    self->pushRight = _one;
                }
                break;
            }
            gLevelLayers->kind = 0;
        } else if (self->hitAxes == 8) {
            self->pushLeft = 0;
            self->pushRight = 0;
            self->slippery = 0;
        }

        MATCH_USE(hold); /* r3 hold ends: no code */
        off = A884Offset(self);
        x = Q8_TO_INT(self->x);
        y = Q8_TO_INT(self->y);
        if (self->mirror.bits.flipX)
            x -= off[0];
        else
            x += off[0];
        y += off[1];
        if (GetTerrainFlagsAt(gLevelLayers, x, y) == 6) {
            if (self->hanging == 0) {
                s32 snap = (y & 0x00FFFFF8) + 7;

                snap -= y;
                self->y += INT_TO_Q8(snap);
                CALL_M68(self, 0, EVENT_HANG_GRAB, 0);
            }
        } else if (self->hanging != 0) {
            CALL_M68(self, 0, EVENT_HANG_RELEASE, 0);
        }
    }
    return self->hitAxes;
}

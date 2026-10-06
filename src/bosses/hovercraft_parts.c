#include "core.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "globals.h"

/* A second per-instance "self" object family sharing the exact same
 * layout convention already documented for the boss-weapon cluster
 * (airship_fireball.c-airship_graphics.c, docs/matching/issue-58-0x08030334-actor.md):
 * state at `+0x28`, table-index/"kind" at `+0xc`, an anim-frame
 * halfword/byte pair at `+0x10`/`+0x12`, an accumulator at `+8`, a
 * "part table" pointer at `+0`, and an event/trampoline table pointer
 * at `+0x50` - plus a health-like countdown at `+0x54` and a death/
 * "dead" byte flag at `+0x6c`. These functions also drive a *singleton*
 * object reached through the global pointer `gHovercraft` (not a
 * per-instance `self`) - see docs/rom_map.md, "Follow-up reads
 * `HovercraftCannonStateFire`'s helper cluster and `UpdateHovercraft`" and "`CreateHovercraft`
 * closes a long-open question: the missing singleton constructor". Most
 * of `gHovercraft`'s own accessors (`GetHovercraftPartsLeft`-`SetHovercraftState`)
 * are trivial one-line getters for its fields; `SetHovercraftState`/
 * `HovercraftStateApproach` are the same state-transition/animation-frame-reset
 * sequence already documented for the boss cluster's
 * `DamageAirshipFireball`/`AirshipStateFall`/`DamageAirship`. See
 * docs/matching/issue-62-0x08033804-actor.md. */

/* One-shot latch: if neither `gHovercraftHitFlashOn` nor `gHovercraftHitFlashTimer`
 * has been set yet, arms both. */
void StartHovercraftHitFlash(void)
{
    if (gHovercraftHitFlashOn == 0 && gHovercraftHitFlashTimer == 0) {
        gHovercraftHitFlashTimer = 1;
        gHovercraftHitFlashOn = 1;
    }
}

/* Palette flash toggle: `gFlashBgPalette`/`gFlashObjPalette` point
 * into palette RAM (BG palette 1 and OBJ palette 10, iwram_data.c), and
 * this sets color 15 of both. The first call caches the original color
 * into `gHovercraftFlashSavedColor`; from then on, `flag` picks between white
 * (`0x7FFF`) and the cached color. */
void SetHovercraftFlashColor(u8 flag)
{
    register u16 val asm("r1");

    if (gHovercraftFlashColorSaved == 0) {
        gHovercraftFlashSavedColor = ((u16 *)gFlashBgPalette)[15];
        gHovercraftFlashColorSaved = 1;
    }

    if (flag != 0) {
        register u16 *p asm("r0") = gFlashBgPalette;

        val = RGB_WHITE;
        p[15] = val;
    } else {
        register u16 *p asm("r2") = gFlashBgPalette;

        val = gHovercraftFlashSavedColor;
        p[15] = val;
    }

    ((u16 *)gFlashObjPalette)[15] = val;
}

/* Constant getter - returns the singleton's lifetime counter
 * (`gHovercraftPartsLeft`). */
s32 GetHovercraftPartsLeft(void)
{
    return gHovercraftPartsLeft;
}

/* The singleton's death/reset transition: plays the death sound, then
 * decrements the lifetime counter `gHovercraftPartsLeft`, and once it
 * reaches zero clears `gHovercraftGone` and fires the state-5/
 * table-index-0 transition via `SetHovercraftState`. */
void LoseHovercraftPart(void)
{
    PlaySfx(gAudioContext, 4, 0x100);

    gHovercraftPartsLeft -= 1;
    if (gHovercraftPartsLeft == 0) {
        gHovercraftGone = gHovercraftPartsLeft;
        SetHovercraftState(5, 0);
    }
}

/* Constant getter - returns `gHovercraftAttack` (a pointer to a small
 * per-state lookup table used by several functions in this cluster). */
const struct singleton_kind *GetHovercraftAttack(void)
{
    return gHovercraftAttack;
}

/* Constant getter - returns `gHovercraftState` (the singleton's
 * current animation "kind" index). */
s32 GetHovercraftState(void)
{
    return gHovercraftState;
}

/* Constant getter - returns `gHovercraftLevel`, the level index
 * `CreateHovercraft` caches (it picks the gHovercraftAttacks record; the
 * side guns test it against 0). */
s32 GetHovercraftLevel(void)
{
    return gHovercraftLevel;
}

/* Constant getter - returns the singleton's Z position field
 * (`gHovercraftZ`). */
s32 GetHovercraftZ(void)
{
    return gHovercraftZ;
}

/* Constant getter - returns the singleton's Y position field
 * (`gHovercraftY`). */
s32 GetHovercraftY(void)
{
    return gHovercraftY;
}

/* Constant getter - returns the singleton's X position field
 * (`gHovercraftX`). */
s32 GetHovercraftX(void)
{
    return gHovercraftX;
}

/* State-transition setter for the singleton (`gHovercraft`):
 * selects animation "kind" `a0`, sets the table index to `a1`, resets
 * the anim-frame halfword/byte pair from the new table entry's first
 * field, and - once the current animation frame reaches the new
 * entry's duration (its `+4` halfword) - clears the accumulator at
 * `+8`. Same idiom as the boss cluster's `DamageAirshipFireball`/`AirshipStateFall`.
 * The `*(T *)&self->...` stores (here and in HovercraftStateApproach) keep gcc from
 * treating them as struct-member accesses, which would let the scheduler
 * move the `anims[]` load below the zero constant. */
void SetHovercraftState(s32 a0, s32 a1)
{
    struct actor_self *self;

    gHovercraftState = a0;
    self = gHovercraft;
    self->animIndex = a1;

    {
        register u16 anim asm("r0") = self->anims[a1].duration;
        register u8 zero asm("r1") = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero;
    }

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx asm("r2") = self->animIndex;
        register u8 *table asm("r3") = (u8 *)self->anims;
        register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
        register s32 four asm("r2");
        register s32 val asm("r1");

        asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
        four = 4;
        val = *(s16 *)(entryPtr + four); /* anims[idx].loopThreshold */

        if (frame >= val) {
            self->animTime = 0;
        }
    }
}

/* gHovercraftStateFuncs[0]: the state CreateHovercraft sets before
 * SpawnHovercraft starts the fight (state 1), the twin of
 * AirshipStateInactive. Empty. */
void HovercraftStateInactive(void)
{
}

/* State-transition setter for the singleton, gated by a depth
 * accumulator: advances `gHovercraftZ` by `gHovercraftVelZ`,
 * and - only while `gHovercraftDistance` is still under its `0x81FF`
 * threshold - resets `gHovercraftVelX`/`gHovercraftVelY`, selects
 * animation "kind" 2, and runs the same table-index-0 anim-frame-reset
 * sequence as `SetHovercraftState`. */
void HovercraftStateApproach(void)
{
    struct actor_self *self;

    gHovercraftZ += gHovercraftVelZ;

    if (gHovercraftDistance <= 0x81FF) {
        register s32 *pCC asm("r1") = &gHovercraftVelX;
        register s32 *pD0 asm("r0") = &gHovercraftVelY;
        register s32 zeroD0 asm("r5") = 0;

        *pD0 = zeroD0;
        *pCC = zeroD0;
        {
            register s32 two asm("r1") = 2;
            register s32 *pB0 asm("r0") = &gHovercraftState;

            *pB0 = two;
        }

        self = gHovercraft;
        self->animIndex = zeroD0;

        {
            register u16 anim asm("r0") = self->anims[0].duration;
            register u8 zero asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            register s32 idx asm("r2") = self->animIndex;
            register u8 *table asm("r3") = (u8 *)self->anims;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four); /* anims[idx].loopThreshold */

            if (frame >= val) {
                self->animTime = zeroD0;
            }
        }
    }
}

/* No-op stub. */
void nullsub_37(void)
{
}

asm(".align 2, 0");

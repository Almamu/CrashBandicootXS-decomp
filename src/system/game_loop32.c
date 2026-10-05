#include "core.h"
#include "crate.h"

extern u8 gCrateListChanged;
extern u8 gCrateKindExplosive[];
extern struct crate *GetCrateBelow(struct crate *obj);
extern struct crate *GetCrateAbove(struct crate *obj);
extern void ExplodeCrate(struct crate *self, u8 arg1);
extern void LightTntCrate(struct crate *self);

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). */

/* A per-frame position-wrap advance: `self+0x4c` is a signed "speed"
 * (defaulting to 1 when 0), `self+0x44` a signed Q8-ish countdown
 * ("remaining"). When `remaining == 0` the whole function is a no-op.
 * Otherwise it sets the global one-shot flag byte `gCrateListChanged`,
 * then loops, once per unit of `speed`, folding `remaining` toward
 * zero by +-0x100 (or +-0x40, when the viewport's own
 * `gPlayer`-relative `+0x88` byte reads 1 - a "half speed"
 * mode) while accumulating the matching step into `self+0x4`; each
 * time `remaining` crosses to <=0 it re-derives the wrap via the
 * `gCrateKindExplosive[self+0x4e]` per-state table and, depending on
 * that table's value and `self+0x48`/`self+0x4d`'s state, dispatches
 * `ExplodeCrate`/`LightTntCrate` on `self` and its whole `GetCrateBelow`
 * "get next" neighbor-list chain (the same list `ResetCrate`/
 * `ResolvePlayerCollisions`, game_loop22.c/game_loop23.c, already establish). At
 * the end, `self+0x4`'s accumulated step is folded into `self`'s own
 * position (`self+0`/`self+4`), and `self+0x4c`'s "speed" byte is
 * either cleared (when `remaining` ended up exactly 0) or incremented
 * toward a clamped max of 5 (saturating, never decremented back down
 * by this function).
 *
 * Matches under old_agbcc (the NAKED note blamed the "two extra
 * high-register accumulators"; see
 * docs/matching/issue-12-13-25-naked-retry.md). What mattered: the
 * speed byte is re-read through `self->unk_4C` each time (GCSE keeps
 * its address in sb), `speed--` is written in both step arms, the
 * neighbour walk skips the first neighbour, and one temporary `t` both
 * carries `fallTargetY` into `y` and re-reads `x` at the bottom of the loop
 * (the ROM's r1). */
void UpdateCrateFall(struct crate *self)
{
    s32 remaining = self->fallDistance;
    s32 speed;
    s32 acc;
    s32 t;

    if (remaining == 0)
        return;
    gCrateListChanged = 1;
    speed = self->fallSpeed;
    if (speed == 0)
        speed = 1;
    acc = 0;
    t = self->x;
    if (speed < 0)
    {
        do
        {
            acc -= 0x100;
            remaining += 0x100;
            speed++;
        } while (speed != 0);
    }
    else
    {
        do
        {
            if (remaining > 0)
            {
                if (PHYS_PLAYER->ctrlMode == 1)
                {
                    acc += 0x40;
                    remaining -= 0x40;
                    speed--;
                }
                else
                {
                    acc += 0x100;
                    remaining -= 0x100;
                    speed--;
                }
            }
            else
            {
                struct crate *n;
                u8 kind;

                speed = 1;
                t = self->fallTargetY;
                self->y = t;
                acc = 0;
                self->fallDistance = remaining;
                if (gCrateKindExplosive[kind = self->kind])
                {
                    if (self->u48.blastState != 0 || kind == 10)
                    {
                        if ((self->state & 0x7f) == 0)
                            ExplodeCrate(self, 0);
                    }
                    else if (kind == 0xe)
                    {
                        struct crate *next = GetCrateAbove(self);
                        struct crate *prev = GetCrateBelow(self);

                        if (next != NULL || prev == NULL)
                            LightTntCrate(self);
                    }
                }
                n = GetCrateBelow(self);
                speed--;
                if (n != NULL)
                {
                    n = GetCrateBelow(n);
                    while (n != NULL)
                    {
                        if (n->kind == 0xe)
                            LightTntCrate(n);
                        n = GetCrateBelow(n);
                    }
                }
            }
            t = self->x;
        } while (speed != 0);
    }
    {
        s32 y = self->y + acc;

        self->x = t;
        self->y = y;
    }
    self->fallDistance = remaining;
    if (remaining == 0)
        self->fallSpeed = 0;
    else
    {
        if (++self->fallSpeed == 0)
            ++self->fallSpeed;
        if (self->fallSpeed > 5)
            self->fallSpeed = 5;
    }
}
/* Trailing byte-padding mismatch fix: the function body is 342 bytes
 * (not 4-aligned), and the ROM pads the 2-byte gap before the next
 * function (FindLineCrossing) with a zero halfword rather than the
 * assembler's default `nop` (`mov r8, r8`) - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");

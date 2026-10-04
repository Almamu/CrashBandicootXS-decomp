#include "core.h"
#include "phys_obj.h"

extern u8 gUnknown_030012B0;
extern u8 gStaticData_0816BBC4[];
extern struct phys_obj *sub_8010708(struct phys_obj *obj);
extern struct phys_obj *sub_801070C(struct phys_obj *obj);
extern void sub_800EEF0(struct phys_obj *self, u8 arg1);
extern void sub_800E620(struct phys_obj *self);

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). */

/* A per-frame position-wrap advance: `self+0x4c` is a signed "speed"
 * (defaulting to 1 when 0), `self+0x44` a signed Q8-ish countdown
 * ("remaining"). When `remaining == 0` the whole function is a no-op.
 * Otherwise it sets the global one-shot flag byte `gUnknown_030012B0`,
 * then loops, once per unit of `speed`, folding `remaining` toward
 * zero by +-0x100 (or +-0x40, when the viewport's own
 * `gPlayer`-relative `+0x88` byte reads 1 - a "half speed"
 * mode) while accumulating the matching step into `self+0x4`; each
 * time `remaining` crosses to <=0 it re-derives the wrap via the
 * `gStaticData_0816BBC4[self+0x4e]` per-state table and, depending on
 * that table's value and `self+0x48`/`self+0x4d`'s state, dispatches
 * `sub_800EEF0`/`sub_800E620` on `self` and its whole `sub_8010708`
 * "get next" neighbor-list chain (the same list `sub_800FEB0`/
 * `sub_80106DC`, game_loop22.c/game_loop23.c, already establish). At
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
 * carries `unk_40` into `y` and re-reads `x` at the bottom of the loop
 * (the ROM's r1). */
void sub_800FC70(struct phys_obj *self)
{
    s32 remaining = self->unk_44;
    s32 speed;
    s32 acc;
    s32 t;

    if (remaining == 0)
        return;
    gUnknown_030012B0 = 1;
    speed = self->unk_4C;
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
                if (PHYS_PLAYER->ringLocked == 1)
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
                struct phys_obj *n;
                u8 kind;

                speed = 1;
                t = self->unk_40;
                self->y = t;
                acc = 0;
                self->unk_44 = remaining;
                if (gStaticData_0816BBC4[kind = self->kind])
                {
                    if (self->u48.n != 0 || kind == 10)
                    {
                        if ((self->state & 0x7f) == 0)
                            sub_800EEF0(self, 0);
                    }
                    else if (kind == 0xe)
                    {
                        struct phys_obj *next = sub_801070C(self);
                        struct phys_obj *prev = sub_8010708(self);

                        if (next != NULL || prev == NULL)
                            sub_800E620(self);
                    }
                }
                n = sub_8010708(self);
                speed--;
                if (n != NULL)
                {
                    n = sub_8010708(n);
                    while (n != NULL)
                    {
                        if (n->kind == 0xe)
                            sub_800E620(n);
                        n = sub_8010708(n);
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
    self->unk_44 = remaining;
    if (remaining == 0)
        self->unk_4C = 0;
    else
    {
        if (++self->unk_4C == 0)
            ++self->unk_4C;
        if (self->unk_4C > 5)
            self->unk_4C = 5;
    }
}
/* Trailing byte-padding mismatch fix: the function body is 342 bytes
 * (not 4-aligned), and the ROM pads the 2-byte gap before the next
 * function (sub_800FDC8) with a zero halfword rather than the
 * assembler's default `nop` (`mov r8, r8`) - see
 * matching_decomp_alignment_fix memory. */
asm(".align 2, 0");

#include "core.h"

/* GitHub issue #9/#10: `sub_800C40C` and `sub_800C5D4`, the last two of
 * the four `self+0x68`-dispatching siblings flagged in
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md - `sub_800C40C` is
 * the specific function that doc's own Phase 1 pass already flagged as
 * "the exact function docs/rom_map.md ties to sharing sub_800B8DC's own
 * self+0x68 field" (called from sub_800B8DC's states 4, 13, 14, 16);
 * `sub_800C5D4` is called from state 3 (and 13's fallthrough).
 *
 * `sub_800C40C`: a 6-case dispatcher (modes 0, 3, 4, 5; anything else,
 * including 1/2, is a no-op). Modes 0 and 4 share an "impact
 * distance" gate - `__modsi3` division/remainder-style scalar check
 * against a `gUnknown_0300082C`-relative table lookup indexed by
 * `self->0x30`/`self->0x34`/`self->0x38` (the same "close enough"
 * primitive `docs/rom_map.md` already ties to hud_counter.c/
 * hud_stat_widget3.c) - only proceeding when the check passes, then
 * consulting `self->0x84`'s pointed record (`+0xc` for mode 0, `+0x14`
 * for mode 4) against a constant `8` to pick between two
 * `sub_800C8CC` trigger constants. Mode 0 additionally clears bit 3 of
 * `owner->0xd` when `self->0x6c` is `0xf`/`0x12`/`0x1a` and re-triggers
 * `sub_800C8BC(self,0)`, or when `self->0x6c` is `0x12`/`0x1a`. Mode 3
 * is the largest case: if `owner->0x38` is set, triggers
 * `sub_800C8CC(self,4)`, then on `self->0x6c` `0x12`/`0x1a` sets bit 3
 * of `owner->0xd` and plays SFX `0x26`, or on `self->0x6c == 0xf` plays
 * SFX `9`; then unconditionally, if `self->0x6c == 0x17` and
 * `owner->0x30 == 9` and `owner->0x34 == 0`, spawns a part via
 * `sub_8025B0C(gEntitySpawner, 0x17, 4, -0x2d, 2, owner)` (the same
 * `sub_8025B0C(pool, kind, ..., z, src)` shape `game_loop14.c`
 * documents), tags the new part's `+0xc`/`+0xa` fields, and plays SFX
 * `0x1e`. Mode 5 mirrors mode 3's `owner->0x38` gate but triggers
 * `sub_800C8CC(self,0)` and, only for `self->0x6c==0xf`, additionally
 * `sub_800C8BC(self,1)`; a shared tail (also reached directly when
 * `owner->0x38` was clear) then plays SFX `0x23` when `self->0x6c==0xf`
 * and `owner->0x30==8` and `owner->0x34==0`.
 *
 * `sub_800C5D4`: a 3-case dispatcher (modes 0, 2; anything else falls
 * to a shared tail). Unconditional prelude: if `self->0x6c==0xb` and
 * `owner->4 < self->0x64`, latches `owner->4 = self->0x64` and fires
 * `sub_800C8AC(self,0)`. Mode 0 builds an AABB at `owner`'s position
 * offset by `self->0x20`/`self->0x24` sized by `self->0x28-0x20`/
 * `self->0x2c-0x24` (via `sub_803AFE4`/`sub_803AFDC`, the same
 * `struct aabb` shape `actor_part4.c`/`actor_part15.c` already use),
 * mirrors it per `owner->0x28` bit 4, then tests it against the player
 * (`gUnknown_030012D8`) via `sub_800B37C` - on overlap, triggers
 * `sub_800C8CC(self,2)` and, if `self->0x6c==0xb`, seeds `owner`'s
 * `0x48`-`0x64` velocity-target fields with fixed constants (a
 * "knockback impulse" shape, same family as `sub_800B8DC` state 17's
 * own jump-impulse). Mode 2 triggers `sub_800C8CC(self,0)` only when
 * `owner->0x38` is set.
 *
 * `sub_800C40C` is real C under old_agbcc (issue #10 NAKED retry,
 * docs/matching/issue-10-naked-retry.md). Its spawn call is an inline
 * copy of `sub_800C9C8` (actor_part116.c): passing the arguments
 * through inline parameters is what materializes them in the ROM's
 * order, and the `+0xC` flag writes are bitfield stores (QImode `-0x41`/
 * `-9` masks). `__modsi3` is a remainder (`a % b`).
 *
 * `sub_800C5D4` is real C too (issue #9-#11 NAKED retry): holding
 * `self->target` in a block-local pinned to r1 reproduces the prelude's
 * load order and registers. `sub_800C5D4`'s trailing
 * byte count needs the trailing `asm(".align 2, 0")` (the ROM
 * zero-pads its last 2 bytes to the next 4-byte boundary). */
#include "part_ctrl.h"

extern s32 __modsi3(s32 a, s32 b);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern struct ctrl_target *sub_8025B0C(void *pool, s32 kind, s32 b, s32 margin, s32 z, s32 e, struct ctrl_target *src);
extern u8 sub_800B37C(struct ctrl_target *obj, struct part_aabb *box);
extern void sub_803AFE4(struct part_aabb *box, s32 x, s32 y);
extern void sub_803AFDC(struct part_aabb *box, s32 w, s32 h);
extern u32 gUnknown_0300082C;
extern void *gAudioContext;
extern void *gEntitySpawner;
extern struct ctrl_target *gUnknown_030012D8;

/* `sub_800C9C8` (actor_part116.c), inlined. */
static inline struct ctrl_target *SpawnPart(s32 a, s32 b, s32 c, s32 d, s32 e, struct ctrl_target *f)
{
    struct ctrl_target *obj = sub_8025B0C(gEntitySpawner, a, b, c, d, e, f);
    obj->visible = 1;
    obj->flag6 = 0;
    return obj;
}

void sub_800C40C(struct part_ctrl *self)
{
    switch (self->mode) {
    case 0:
        if (self->unk_34 > 0
            && __modsi3(gUnknown_0300082C + (self->unk_30 + self->unk_34) * 2 - self->unk_38 - self->unk_30,
                           self->unk_30 + self->unk_34) == 0) {
            if (self->anims[3] != 8)
                sub_800C8CC(self, 3);
            else
                sub_800C8CC(self, 4);
            if (self->kind == 0xf) {
                sub_800C8BC(self, 0);
            } else if (self->kind == 0x12 || self->kind == 0x1a) {
                self->target->solid = 0;
            }
        }
        break;
    case 4:
        if (self->unk_30 > 0
            && __modsi3(gUnknown_0300082C + self->unk_30 + self->unk_34 - self->unk_38,
                           self->unk_30 + self->unk_34) == 0) {
            if (self->anims[5] != 8)
                sub_800C8CC(self, 5);
            else
                sub_800C8CC(self, 0);
        }
        break;
    case 3:
        if (self->target->animDone) {
            sub_800C8CC(self, 4);
            if (self->kind == 0x12 || self->kind == 0x1a) {
                self->target->solid = 1;
                PlaySfx(gAudioContext, 0x26, 0x100);
            } else if (self->kind == 0xf) {
                PlaySfx(gAudioContext, 9, 0x100);
            }
        }
        if (self->kind == 0x17 && self->target->tick == 9 && self->target->timer == 0) {
            SpawnPart(0x17, 4, -0x2d, 2, 0, self->target)->kind = 2;
            PlaySfx(gAudioContext, 0x1e, 0x100);
        }
        break;
    case 5:
        if (self->target->animDone) {
            sub_800C8CC(self, 0);
            if (self->kind != 0xf)
                break;
            sub_800C8BC(self, 1);
        }
        if (self->kind == 0xf && self->target->tick == 8 && self->target->timer == 0)
            PlaySfx(gAudioContext, 0x23, 0x100);
        break;
    }
}

void sub_800C5D4(struct part_ctrl *self)
{
    s32 mode;
    struct part_aabb box;
    s32 x, y, w, h;

    if (self->kind == 0xb) {
        /* r1 pin: the allocator otherwise swaps target/baseY (r2/r1). */
        register struct ctrl_target *t asm("r1") = self->target;
        if (t->y < self->baseY) {
            t->y = self->baseY;
            sub_800C8AC(self, 0);
        }
    }
    switch (mode = self->mode) {
    case 0:
        x = self->target->x >> 8;
        y = self->target->y >> 8;
        w = self->boxR - self->boxL;
        h = self->boxB - self->boxT;
        sub_803AFE4(&box, x + self->boxL, y + self->boxT);
        sub_803AFDC(&box, w, h);
        if (self->target->mirror.u.x)
            box.x = (self->target->x >> 8) * 2 - (box.x + box.w);
        if (sub_800B37C(gUnknown_030012D8, &box)) {
            sub_800C8CC(self, 2);
            if (self->kind == 0xb) {
                struct ctrl_target *target = self->target;
                s32 a = 0x300, b = 0x20, c;

                target->speedY = a;
                target->velB[0] = a;
                target->velB[1] = b;
                target->velB[2] = mode;
                c = -0x200;
                target->speedX = mode;
                target->velA[0] = mode;
                target->velA[1] = b;
                target->velA[2] = c;
            }
        }
        break;
    case 2:
        if (self->target->animDone)
            sub_800C8CC(self, 0);
        break;
    }
}
asm(".align 2, 0");

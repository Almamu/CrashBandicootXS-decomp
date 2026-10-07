#include "core.h"
#include "match.h"
#include "part_ctrl.h"
#include "enemies.h"
#include "util.h"
#include <libgcc.h>
#include "audio.h"
#include "player.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #9/#10: `UpdateEnemyAttackCycle` and `UpdateEnemyTriggerBox`, the last two of
 * the four `self+0x68`-dispatching siblings flagged in
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md - `UpdateEnemyAttackCycle` is
 * the specific function that doc's own Phase 1 pass already flagged as
 * "the exact function docs/rom_map.md ties to sharing UpdateEnemyCtrl's own
 * self+0x68 field" (called from UpdateEnemyCtrl's states 4, 13, 14, 16);
 * `UpdateEnemyTriggerBox` is called from state 3 (and 13's fallthrough).
 *
 * `UpdateEnemyAttackCycle`: a 6-case dispatcher (modes 0, 3, 4, 5; anything else,
 * including 1/2, is a no-op). Modes 0 and 4 share an "impact
 * distance" gate - `__modsi3` division/remainder-style scalar check
 * against a `gRoomFrameCount`-relative table lookup indexed by
 * `self->0x30`/`self->0x34`/`self->0x38` (the same "close enough"
 * primitive `docs/rom_map.md` already ties to hud_lives.c/
 * hud_counters.c) - only proceeding when the check passes, then
 * consulting `self->0x84`'s pointed record (`+0xc` for mode 0, `+0x14`
 * for mode 4) against a constant `8` to pick between two
 * `SetEnemyAnimMode` trigger constants. Mode 0 additionally clears bit 3 of
 * `owner->0xd` when `self->0x6c` is `0xf`/`0x12`/`0x1a` and re-triggers
 * `SetEnemyMotionX(self,0)`, or when `self->0x6c` is `0x12`/`0x1a`. Mode 3
 * is the largest case: if `owner->0x38` is set, triggers
 * `SetEnemyAnimMode(self,4)`, then on `self->0x6c` `0x12`/`0x1a` sets bit 3
 * of `owner->0xd` and plays SFX `0x26`, or on `self->0x6c == 0xf` plays
 * SFX `9`; then unconditionally, if `self->0x6c == 0x17` and
 * `owner->0x30 == 9` and `owner->0x34 == 0`, spawns a part via
 * `LaunchEffectPart(gEntitySpawner, 0x17, 4, -0x2d, 2, owner)` (the same
 * `LaunchEffectPart(pool, kind, ..., z, src)` shape `entity_spawner.c`
 * documents), tags the new part's `+0xc`/`+0xa` fields, and plays SFX
 * `0x1e`. Mode 5 mirrors mode 3's `owner->0x38` gate but triggers
 * `SetEnemyAnimMode(self,0)` and, only for `self->0x6c==0xf`, additionally
 * `SetEnemyMotionX(self,1)`; a shared tail (also reached directly when
 * `owner->0x38` was clear) then plays SFX `0x23` when `self->0x6c==0xf`
 * and `owner->0x30==8` and `owner->0x34==0`.
 *
 * `UpdateEnemyTriggerBox`: a 3-case dispatcher (modes 0, 2; anything else falls
 * to a shared tail). Unconditional prelude: if `self->0x6c==0xb` and
 * `owner->4 < self->0x64`, latches `owner->4 = self->0x64` and fires
 * `SetEnemyMotionY(self,0)`. Mode 0 builds an AABB at `owner`'s position
 * offset by `self->0x20`/`self->0x24` sized by `self->0x28-0x20`/
 * `self->0x2c-0x24` (via `SetAabbPos`/`SetAabbSize`, the same
 * `struct aabb` shape `sprite_obj.c`/`player_update.c` already use),
 * mirrors it per `owner->0x28` bit 4, then tests it against the player
 * (`gPlayer`) via `PlayerTouchesBox` - on overlap, triggers
 * `SetEnemyAnimMode(self,2)` and, if `self->0x6c==0xb`, seeds `owner`'s
 * `0x48`-`0x64` velocity-target fields with fixed constants (a
 * "knockback impulse" shape, same family as `UpdateEnemyCtrl` state 17's
 * own jump-impulse). Mode 2 triggers `SetEnemyAnimMode(self,0)` only when
 * `owner->0x38` is set.
 *
 * `UpdateEnemyAttackCycle` is real C under old_agbcc (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md). Its spawn call is an inline
 * copy of `LaunchHarmfulEffectPart` (enemy_ctrl.c): passing the arguments
 * through inline parameters is what materializes them in the ROM's
 * order, and the `+0xC` flag writes are bitfield stores (QImode `-0x41`/
 * `-9` masks). `__modsi3` is a remainder (`a % b`).
 *
 * `UpdateEnemyTriggerBox` is real C too (issue #9-#11 NAKED retry): holding
 * `self->target` in a block-local pinned to r1 reproduces the prelude's
 * load order and registers. */

/* `LaunchHarmfulEffectPart` (enemy_ctrl.c), inlined. */
static inline struct ctrl_target *SpawnPart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                            struct ctrl_target *f)
{
    struct ctrl_target *obj = LaunchEffectPart(gEntitySpawner, a, b, c, d, e, (struct fx_part *)f);
    obj->visible = 1;
    obj->flag6 = 0;
    return obj;
}

void UpdateEnemyAttackCycle(struct part_ctrl *self)
{
    switch (self->mode) {
    case 0:
        if (self->attackTime > 0 &&
            __modsi3(gRoomFrameCount + (self->idleTime + self->attackTime) * 2 - self->cycleOffset -
                         self->idleTime,
                     self->idleTime + self->attackTime) == 0) {
            if (self->anims[3] != 8)
                SetEnemyAnimMode(self, 3);
            else
                SetEnemyAnimMode(self, 4);
            if (self->kind == ENEMY_KIND_PENGUIN) {
                SetEnemyMotionX(self, 0);
            } else if (self->kind == ENEMY_KIND_WOODEN_CRUSHER ||
                       self->kind == ENEMY_KIND_PISTON_CRUSHER) {
                self->target->solid = 0;
            }
        }
        break;
    case 4:
        if (self->idleTime > 0 &&
            __modsi3(gRoomFrameCount + self->idleTime + self->attackTime - self->cycleOffset,
                     self->idleTime + self->attackTime) == 0) {
            if (self->anims[5] != 8)
                SetEnemyAnimMode(self, 5);
            else
                SetEnemyAnimMode(self, 0);
        }
        break;
    case 3:
        if (self->target->animDone) {
            SetEnemyAnimMode(self, 4);
            if (self->kind == ENEMY_KIND_WOODEN_CRUSHER ||
                self->kind == ENEMY_KIND_PISTON_CRUSHER) {
                self->target->solid = 1;
                PlaySfx(gAudioContext, SFX_CRUSHER_SLAM, 0x100);
            } else if (self->kind == ENEMY_KIND_PENGUIN) {
                PlaySfx(gAudioContext, SFX_UNKNOWN_09, 0x100);
            }
        }
        if (self->kind == ENEMY_KIND_FLAMETHROWER_LAB_ASSISTANT && self->target->tick == 9 &&
            self->target->timer == 0) {
            SpawnPart(0x17, 4, -0x2d, 2, 0, self->target)->kind = 2;
            PlaySfx(gAudioContext, SFX_FLAMETHROWER, 0x100);
        }
        break;
    case 5:
        if (self->target->animDone) {
            SetEnemyAnimMode(self, 0);
            if (self->kind != ENEMY_KIND_PENGUIN)
                break;
            SetEnemyMotionX(self, 1);
        }
        if (self->kind == ENEMY_KIND_PENGUIN && self->target->tick == 8 && self->target->timer == 0)
            PlaySfx(gAudioContext, SFX_UNKNOWN_23, 0x100);
        break;
    }
}

void UpdateEnemyTriggerBox(struct part_ctrl *self)
{
    s32 mode;
    struct aabb box;
    s32 x, y, w, h;

    if (self->kind == ENEMY_KIND_VULTURE) {
        /* r1 pin: the allocator otherwise swaps target/baseY (r2/r1). */
        MATCH_HOLD_REG(struct ctrl_target *, t, r1) = self->target;
        if (t->y < self->baseY) {
            t->y = self->baseY;
            SetEnemyMotionY(self, 0);
        }
    }
    switch (mode = self->mode) {
    case 0:
        x = self->target->x >> 8;
        y = self->target->y >> 8;
        w = self->boxR - self->boxL;
        h = self->boxB - self->boxT;
        SetAabbPos(&box, x + self->boxL, y + self->boxT);
        SetAabbSize(&box, w, h);
        if (self->target->mirror.u.x)
            box.x = (self->target->x >> 8) * 2 - (box.x + box.w);
        if (PlayerTouchesBox(gPlayer, &box)) {
            SetEnemyAnimMode(self, 2);
            if (self->kind == ENEMY_KIND_VULTURE) {
                struct ctrl_target *target = self->target;
                s32 a = 0x300, b = 0x20, c;

                target->speedY = a;
                target->rampY[0] = a;
                target->rampY[1] = b;
                target->rampY[2] = mode;
                c = -0x200;
                target->speedX = mode;
                target->rampX[0] = mode;
                target->rampX[1] = b;
                target->rampX[2] = c;
            }
        }
        break;
    case 2:
        if (self->target->animDone)
            SetEnemyAnimMode(self, 0);
        break;
    }
}

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md): the last four
 * functions of `asm/code_3_2_17_c6a8.s` - `SetEnemyState`, the
 * `menu_ui` dialog-widget system's own 18-state `self+0x74` update
 * (called from all 31 confirmed `menu_ui` dispatch-table entries,
 * `docs/rom_map.md`'s "A parallel fork then found a genuine surprise"
 * section) - plus the three small `self+0x70`-relative accessor
 * triples the existing tracking comment already named
 * (`SetEnemyRangeXSpeed`/`SetEnemyRangeYSpeed`/`SetEnemyRangeX`).
 *
 * Despite the "menu_ui" framing, `SetEnemyState` operates on the exact
 * same field-offset conventions as the rest of this cluster's
 * `self`/`owner` object shape (`self+0x70` "owner", `self+0xc`
 * "anchor" record, `self+0x84` per-instance table) and calls the
 * exact same helpers `StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet`/`_call_via_r3`
 * already matched for `SetEnemyMotionY`/`SetEnemyMotionX`/`SetEnemyAnimMode`
 * (`enemy_ctrl.c`) - not merely the same *convention* reused on a
 * different struct, but the *identical* struct/helper set, just
 * driven by dialog-widget vtable entries instead of the physics
 * cluster's own entries. Confirms `docs/rom_map.md`'s "general-purpose
 * stateful-widget convention" reading and sharpens it: `menu_ui`'s
 * widgets are literal instances of the same object type the rest of
 * this ROM neighborhood uses, not merely a structurally-similar
 * sibling.
 *
 * `SetEnemyState`'s own case bodies never call `SetEnemyMotionY`/
 * `SetEnemyMotionX`/`SetEnemyAnimMode` as functions - each case *manually
 * repeats* those three helpers' own instruction sequences inline
 * (confirmed by the `bl` targets: `StartCtrlTargetMotionXFromSet`/`StartCtrlTargetMotionYFromSet`
 * directly, never `SetEnemyMotionY`/`SetEnemyMotionX`/`SetEnemyAnimMode`
 * themselves) - so the C below inlines them (SetModeA/SetModeB/SetMode).
 *
 * The whole file is built with old_agbcc (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md). Under it `SetEnemyState` is
 * plain C, and the three bounds setters below match without the
 * register pins and `asm volatile` barriers the current compiler
 * needed. In `SetEnemyState`:
 *  - The groups the ROM keeps apart ({1,3,17} vs {6,9,10,11}) are
 *    separate cases; reload picks a different scratch register for the
 *    trigger's `ldrsh` in each, so cross-jumping can't merge them.
 *  - That scratch register is picked round-robin in insn order, so the
 *    {4,14,16} keyframe tail has to be written out in both branches
 *    (cross-jumping then merges the copies): with a single shared tail
 *    the else branch's `ldrsh` gets r4 instead of the ROM's r1.
 *  - State 5's velocity stores go through inline setters, which puts
 *    the shared 0 in r2 before the first store. */

extern s32 _call_via_r3(void *self, struct ctrl_target *target, s32 arg, void *fn);

static inline void SetModeA(struct part_ctrl *self, s32 mode)
{
    self->modeA = mode;
    StartCtrlTargetMotionYFromSet(self, self->target, mode);
}

static inline void SetModeB(struct part_ctrl *self, s32 mode)
{
    self->modeB = mode;
    StartCtrlTargetMotionXFromSet(self, self->target, mode);
}

static inline void SetMode(struct part_ctrl *self, s32 mode)
{
    struct part_method *m;

    self->mode = mode;
    m = &self->anchor->trigger;
    _call_via_r3((u8 *)self + m->thisOffset, self->target, self->anims[mode], m->fn);
}

static inline void SetVelX(struct ctrl_target *t, s32 v, s32 w)
{
    t->speedX = v;
    t->rampX[0] = v;
    t->rampX[1] = w;
    t->rampX[2] = v;
}

static inline void SetVelY(struct ctrl_target *t, s32 v, s32 w)
{
    t->speedY = v;
    t->rampY[0] = v;
    t->rampY[1] = w;
    t->rampY[2] = v;
}

/* `self->state` update (1-18 valid, same shape as UpdateEnemyCtrl's state
 * machine), then latches the target's position into baseX/baseY. */
void SetEnemyState(struct part_ctrl *self, s32 state)
{
    self->state = state;
    switch (state) {
    case 5:
        {
            struct ctrl_target *target = self->target;

            SetVelX(target, -0x180, 0);
            SetVelY(target, 0x400, 0);
            target->flag7 = 1;
        }
        break;
    case 6:
    case 9:
    case 10:
    case 11:
        SetMode(self, 0);
        break;
    case 2:
    case 15:
        SetModeB(self, 1);
        SetMode(self, 0);
        break;
    case 13:
    case 18:
        SetModeB(self, 1);
    case 4:
    case 14:
    case 16:
        if (self->cycleOffset >= self->idleTime) {
            SetMode(self, 4);
            {
                struct ctrl_target *target = self->target;
                target->tick = (*target->keyframes)[target->frame].steps - 1;
            }
        } else {
            SetMode(self, 0);
            if (self->kind == ENEMY_KIND_STATIONARY_SPACE_ENEMY) {
                struct ctrl_target *target = self->target;
                target->tick = (*target->keyframes)[target->frame].steps - 1;
            }
        }
        break;
    case 1:
    case 3:
    case 17:
        SetMode(self, 0);
        break;
    case 8:
        SetModeB(self, 3);
        SetModeA(self, 3);
        SetMode(self, 0);
        break;
    case 7:
        SetModeB(self, 2);
        SetMode(self, 0);
        self->counter = 0;
        break;
    case 12:
        break;
    }
    self->baseX = self->target->x;
    self->baseY = self->target->y;
}

/* Sets the X homing bounds to the target's x +/- `radius` (Q8) and
 * caches the homing speed pair. */
void SetEnemyRangeXSpeed(struct part_ctrl *self, s32 radius, s32 p2, s32 p3)
{
    s32 x = self->target->x;
    self->rangeX[0] = x - (radius << 8);
    self->rangeX[1] = self->target->x + (radius << 8);
    self->accel = p3;
    self->speed = p2;
}

/* Y-axis version of `SetEnemyRangeXSpeed`. */
void SetEnemyRangeYSpeed(struct part_ctrl *self, s32 radius, s32 p2, s32 p3)
{
    s32 y = self->target->y;
    self->rangeY[1] = y - (radius << 8);
    self->rangeY[0] = self->target->y + (radius << 8);
    self->accel = p3;
    self->speed = p2;
}

/* `SetEnemyRangeXSpeed`'s bounds without the speed pair. */
void SetEnemyRangeX(struct part_ctrl *self, s32 radius)
{
    s32 x = self->target->x;
    self->rangeX[0] = x - (radius << 8);
    self->rangeX[1] = self->target->x + (radius << 8);
}

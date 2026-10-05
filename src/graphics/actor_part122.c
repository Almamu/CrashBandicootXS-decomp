#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the last four
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
 * already matched for `sub_800C8AC`/`sub_800C8BC`/`SetEnemyAnimMode`
 * (`actor_part113.c`) - not merely the same *convention* reused on a
 * different struct, but the *identical* struct/helper set, just
 * driven by dialog-widget vtable entries instead of the physics
 * cluster's own entries. Confirms `docs/rom_map.md`'s "general-purpose
 * stateful-widget convention" reading and sharpens it: `menu_ui`'s
 * widgets are literal instances of the same object type the rest of
 * this ROM neighborhood uses, not merely a structurally-similar
 * sibling.
 *
 * `SetEnemyState`'s own case bodies never call `sub_800C8AC`/
 * `sub_800C8BC`/`SetEnemyAnimMode` as functions - each case *manually
 * repeats* those three helpers' own instruction sequences inline
 * (confirmed by the `bl` targets: `StartCtrlTargetMotionXFromSet`/`StartCtrlTargetMotionYFromSet`
 * directly, never `sub_800C8AC`/`sub_800C8BC`/`SetEnemyAnimMode`
 * themselves) - so the C below inlines them (SetModeA/SetModeB/SetMode).
 *
 * The whole file is built with old_agbcc (issue #10 NAKED retry,
 * docs/matching/issue-10-naked-retry.md). Under it `SetEnemyState` is
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

#include "part_ctrl.h"

extern void StartCtrlTargetMotionYFromSet(struct part_ctrl *self, struct ctrl_target *target, s32 mode);
extern void StartCtrlTargetMotionXFromSet(struct part_ctrl *self, struct ctrl_target *target, s32 mode);
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
    t->velA[0] = v;
    t->velA[1] = w;
    t->velA[2] = v;
}

static inline void SetVelY(struct ctrl_target *t, s32 v, s32 w)
{
    t->speedY = v;
    t->velB[0] = v;
    t->velB[1] = w;
    t->velB[2] = v;
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
        if (self->unk_38 >= self->unk_30) {
            SetMode(self, 4);
            {
                struct ctrl_target *target = self->target;
                target->tick = (*target->keyframes)[target->frame].steps - 1;
            }
        } else {
            SetMode(self, 0);
            if (self->kind == 0x1b) {
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
/* The ROM zero-pads to the next function. */
asm(".align 2, 0");

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
asm(".align 2, 0");

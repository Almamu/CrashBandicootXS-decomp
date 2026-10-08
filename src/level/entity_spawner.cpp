#include "bg_layer.hpp"
#include "spawners.hpp"
#include "ctrl.hpp"

extern "C" {
#include "level_data.h"
#include "globals.h"
}

/* EntitySpawner (#664, include/spawners.hpp): the effect parts, the
 * dropped wumpa and the spawn table. Built with old_agbcp - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

/* Seeds the X speed and its ramp: `v` and `k` are expanded before the
 * stores, as the ROM has them. */
static inline void SetVel(MovingSprite *p, s32 v, s32 k)
{
    p->speedX = v;
    p->rampX.start = v;
    p->rampX.step = k;
    p->rampX.target = v;
}

/* Spawns an effect part next to `src` (at its pixel position, facing its
 * way), places it beside `src` by the half-widths of their two
 * hitboxes plus `margin`, offsets its Y by `z`, and seeds its X speed
 * and ramp from `speed`, negated when the part is mirrored. */
MovingSprite *EntitySpawner::LaunchEffectPart(s32 anim, s32 tag, s32 margin, s32 z, s32 speed,
                                              MovingSprite *src)
{
    MovingSprite *part;
    s32 w1, w2, dist, x;

    {
        s32 x0 = Q8_TO_INT(src->x);
        s32 y0 = Q8_TO_INT(src->y);
        s32 m = src->mirrorFlags.mirrorX;

        part = SpawnEffectPart(anim, tag, x0, y0, m);
    }
    {
        struct aabb a = part->GetAnimHitbox();

        w1 = a.w;
        w2 = src->GetAnimHitbox().w;
    }
    dist = w1 / 2 + w2 / 2 + margin;
    {
        s32 ox = Q8_TO_INT(part->x);
        s32 y;

        x = part->mirrorBits.flipX < 0 ? ox - dist : ox + dist;
        y = Q8_TO_INT(part->y) + z;
        part->x = INT_TO_Q8(x);
        part->y = INT_TO_Q8(y);
    }
    if (part->mirrorBits.flipX < 0)
        SetVel(part, -speed, 0x40);
    else
        SetVel(part, speed, 0x40);
    return part;
}

/* An effect part at (x, y) clamped into the current level's bounds,
 * facing left when `mirror` is set, with animation record `anim` of the
 * sprite banks and animation `tag`, driven by its own EffectCtrl and in
 * the collidable list. */
MovingSprite *EntitySpawner::SpawnEffectPart(s32 anim, s32 tag, s32 x, s32 y, s32 mirror)
{
    MovingSprite *part;
    BgLayer *layer;
    EffectCtrl *mgr;

    LIMIT_MIN(x, 0);
    layer = gLevelLayers->layer0;
    /* Compared sign-extended from 24 bits, clamped zero-extended. */
    if (x >= (s32)((u32)layer->widthPx << 8) >> 8)
        x = ((u32)layer->widthPx << 8 >> 8) - 1;
    LIMIT_MIN(y, 0);
    if (y >= (s32)((u32)layer->heightPx << 8) >> 8)
        y = ((u32)layer->heightPx << 8 >> 8) - 1;
    part = MovingSprite::Create(0xffff, x, y, 0);
    part->mirrorFlags.mirrorX = mirror != 0;
    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + anim * 12);
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    mgr = new EffectCtrl;
    part->mover = mgr;
    mgr->Attach(part);
    part->f.b.visible = 0;
    part->f.b.unk_1 = 0;
    CollidableList()->Add(part);
    return part;
}

/* Unless in a time trial, a wumpa at (x, y) (`special` when `toHud` is
 * set or `p4` is 0xFF), always active, its counter, mode and phase p3,
 * p4 and 0; mode 0xFF starts its payout, and `toHud` flies it to the
 * HUD. */
Wumpa *EntitySpawner::DropWumpa(u32 x, u32 y, u32 p3, u32 p4, bool toHud)
{
    Wumpa *part = 0;

    if (gLevelState->timeTrial == 0) {
        if (toHud || p4 == 0xff)
            part = Wumpa::Create(0xffff, x, y, 0xffff);
        else
            part = Wumpa::Create(0xffff, x, y, 0);
        part->SetAlwaysActive();
        part->counter = p3;
        part->mode = p4;
        part->phase = 0;
        if (p4 == 0xff)
            part->StartPayout();
        if (toHud)
            part->SendToHud();
    }
    return part;
}

/* Calls the spawn function of the record's entity type (`rec[0]`) with
 * the entity's id and the record's x, y and index. */
void EntitySpawner::Spawn(u32 id, const struct level_entity *rec)
{
    const entity_spawn_fn *table = funcs;
    const entity_spawn_fn *e = (const entity_spawn_fn *)((rec->type << 2) + (u32)table);
    u16 px = rec->x;
    u16 py = rec->y;
    u16 index = rec->param;

    (*e)(id, px, py, index);
}

void EntitySpawner::SetTable(const entity_spawn_fn *table, s32 n)
{
    count = n;
    funcs = table;
}

/* An identical body to DestroyEntityFlags (entity_flags.cpp): g++'s
 * delete when bit 0 of the flags is set. */
EntitySpawner::~EntitySpawner()
{
}

/* An identical body to InitEntityFlags. */
EntitySpawner::EntitySpawner()
{
    funcs = 0;
    count = 0;
}

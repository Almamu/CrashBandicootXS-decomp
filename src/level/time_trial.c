#include "core.h"
#include "actor.h"
#include "pickups.h"
#include "crates.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "box_part.h"
#include "sprite_bank.h"
#include "globals.h"

/* GitHub issue #34, UpdateGameFrame-MainLoop cluster (docs/rom_map.md).
 * Built with old_agbcc - see docs/matching/archive/game-loop-old-agbcc.md. */

/* An OAM-backed part (same layout as cortex.c's gfx_part). */
struct slot_part {
    struct actor base; // 0x00
    u8 unk_1C[4];
    const struct sprite_bank *anim; // 0x20
    u8 unk_24[5];
    u8 frameNibble:4; // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag; // 0x2D
};

struct entity_vtable {
    u8 unk_00[0x28];
    struct actor_method m28; // 0x28
    u8 unk_30[0x18];
    struct actor_method m48; // 0x48
};

struct entity {
    struct actor base; // 0x00 (+0x08: id, +0x0C: flags)
    u8 unk_1C[0];
};

extern s32 _call_via_r1(void *self, void *fn);

static inline void SetPartTag(struct slot_part *part, s32 tag)
{
    part->tag = tag;
}

#define ACTOR_METHOD(a, m) \
    _call_via_r1((u8 *)(a) + ((struct entity_vtable *)(a)->table)->m.thisOffset, \
                ((struct entity_vtable *)(a)->table)->m.fn)

/* Level restart: resets the level state's +0x8c/+0x90 fields, then
 * (unless the level object is already in state 3) retags and restarts
 * both slot parts, reloads slot B's tiles, runs ConvertCratesForTimeTrial, and
 * re-registers every entity in gTouchableList whose +0x48 method
 * returns 2: those whose +0x28 method fails are despawned, the rest are
 * flagged seen and marked in the collision map's seen bitmap. */
void StartTimeTrial(struct level_state *self)
{
    struct slot_part *part;
    s32 i;

    SetMaskLevel(self, MASK_LEVEL_NONE);
    self->timeTrial = 1;
    self->minutes = 0;
    self->seconds = 0;
    self->tenths = 0;
    self->frames = 0;
    self->countdown = 0;
    if (self->cat->kind == ROOM_KIND_CATEGORY)
        return;

    part = (struct slot_part *)self->bonusPlatform;
    if (part != NULL) {
        SetPartTag(part, 7);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
    }
    part = (struct slot_part *)self->gemPlatform;
    if (part != NULL) {
        SetPartTag(part, 0xc);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        // clang-format off
        LoadPaletteSlot(gPaletteCache, ((struct slot_part *)self->gemPlatform)->frameNibble,
                        ((struct slot_part *)self->gemPlatform)->anim->anims[
                            ((struct slot_part *)self->gemPlatform)->tag].paletteId);
        // clang-format on
    }
    ConvertCratesForTimeTrial();

    i = 0;
    if (i < gTouchableList->count) {
        do {
            struct actor *e = (struct actor *)gTouchableList->items[i];
            struct actor *a = e;
            struct actor_method *m = &((struct entity_vtable *)e->table)->m48;

            if (_call_via_r1((u8 *)e + m->thisOffset, m->fn) == 2) {
                if ((u8)ACTOR_METHOD(e, m28))
                    PickUpWumpa((struct orbit_part *)e, 1);
                else {
                    a->flags |= 1;
                    if (a->id != 0xffff) {
                        s32 id = a->id;
                        gEntityFlags->bits0Copy[id / 32] |= 1 << (id % 32);
                    }
                }
            }
        } while (++i < gTouchableList->count);
    }
}

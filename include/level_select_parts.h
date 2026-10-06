#ifndef GUARD_LEVEL_SELECT_PARTS_H
#define GUARD_LEVEL_SELECT_PARTS_H

/* Shared by src/menus/level_select_widgets.c
 * (GitHub issues #28/#29, ROM 0x0801DA38-0x0801E578): the helpers for the
 * level-select screen's (`struct level_menu`) sub-objects - the zooming
 * BG2 picture, the per-level page entries and the cursor panel. All of
 * them own animated sprite parts built by InitUiSpriteObj. The types are
 * level_menu.h's. */

#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "level_menu.h"
#include "objects.h"
#include "memory.h"
#include "globals.h"

extern void LoadTaggedAsset(const void *asset, void *dest);

typedef void (*dtor_fn)(void *self, s32 flags);

/* `delete part;` - the part's virtual destructor with the "free" flag. */
#define DELETE_PART(p)                                                         \
    do                                                                         \
    {                                                                          \
        struct sprite *_p = (p);                                               \
        if (_p != NULL)                                                        \
        {                                                                      \
            struct actor_method *_m = &_p->vtable->m50;                       \
            ((dtor_fn)_m->fn)((u8 *)_p + _m->thisOffset, 3);                   \
        }                                                                      \
    } while (0)

/* The part's current animation record. A macro, not an inline: an
 * inline returning the record's address changes the load order. */
#define PART_RECORD(p) ((p)->anim->anims[(p)->animIndex])

static inline const struct sprite_bank *AnimTable(s32 offset)
{
    return (const struct sprite_bank *)(SPRITE_BANK_BASE + offset);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetFrame(struct sprite *p, s32 frame)
{
    s32 n = PART_RECORD(p).frameCount;
    if (frame >= n)
        frame = n - 1;
    p->frame = frame;
}

static inline void SetAnim(struct sprite *p, s32 anim)
{
    p->animIndex = anim;
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
}

#endif // GUARD_LEVEL_SELECT_PARTS_H

#include "sprite_obj.hpp"
#include "hud.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "sprite_bank.h"
#include "hud.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"
}

/* Pins the current category's tile-cache slots that every actor part
 * shares - the type-0 sprite family gets 2 slots (7/0xf), the type-1/2
 * family gets 5 (9/0xc/0xe/0xd/8): the palettes of a few animations of
 * sprite banks 35 and 47 (sprite_bank.h). The OBJ tile free list it
 * resets starts 0x1400 bytes into OBJ VRAM. Finishes by (re)
 * building the category's status-icon OAM row via ConfigureHudParts, gated on
 * whether the category's type is nonzero.
 *
 * `gActorCategories[category]`'s address is deliberately computed
 * without a named `struct category_descriptor *` local - keeping it in
 * an unnamed rolling register (matching this compiler's own choice for
 * an expression it never has to keep alive past one immediate use)
 * rather than letting the register allocator give it a stable "home"
 * register, which changes which register ends up holding the combined
 * pointer. Likewise the final `ConfigureHudParts` call's arguments are broken
 * into their own local variables in the exact order this compiler
 * evaluates them (icon array pointer, then the array base, then the
 * index) to reproduce the ROM's exact register assignment - see
 * docs/matching/archive/issue-48-0x080291a4-actor.md. */
void SetupActorVramPool(void)
{
    const struct sprite_bank *entityBank;
    const struct sprite_bank *otherBank;
    PaletteCache *cache;
    const struct sprite_anim *anims;

    InitObjTileFreeList(OBJ_VRAM0 + 0x1400);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();

    entityBank = (const struct sprite_bank *)SPRITE_BANK_BASE + 35;
    otherBank = entityBank + 12;
    cache = gPaletteCache;
    cache->FreeUnlockedSlots();

    {
        u8 *arr = (u8 *)gActorCategories;
        s32 idx = gActorCategory;

        if (*(s32 *)(arr + idx * sizeof(struct category_descriptor)) == CATEGORY_TYPE_POLAR) {
            anims = entityBank->anims;
            cache->BindSlot(7, anims[0].paletteId);
            anims = otherBank->anims;
            cache->BindSlot(0xf, anims[2].paletteId);
        } else {
            anims = entityBank->anims;
            cache->BindSlot(9, anims[0].paletteId);
            anims = otherBank->anims;
            cache->BindSlot(0xc, anims[2].paletteId);
            anims = otherBank->anims;
            cache->BindSlot(0xe, anims[8].paletteId);
            anims = otherBank->anims;
            cache->BindSlot(0xd, anims[5].paletteId);
            anims = otherBank->anims;
            cache->BindSlot(8, anims[12].paletteId);
        }
    }

    {
        Hud *iconArray = gHud;
        u8 *arr = (u8 *)gActorCategories;
        s32 idx = gActorCategory;

        iconArray->ConfigureParts(*(s32 *)(arr + idx * sizeof(struct category_descriptor)) !=
                                  CATEGORY_TYPE_POLAR);
    }
}

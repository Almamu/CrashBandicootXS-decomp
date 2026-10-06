#include "core.h"
#include "actor_anim.h"
#include "vram_pool.h"
#include "hud.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"

/* Pins the current category's tile-cache slots that every actor part
 * shares - the type-0 sprite family gets 2 slots (7/0xf), the type-1/2
 * family gets 5 (9/0xc/0xe/0xd/8) pulled from two ROM-side sub-tables
 * (entityTable/otherTable below) whose own shapes aren't reversed yet -
 * kept as raw offsets per docs/workflow.md step 7. Finishes by (re)
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
    u8 *entityTable;
    u8 *otherTable;
    struct palette_cache *cache;
    void *p;

    InitObjTileFreeList((void *)0x06011400);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();

    entityTable = SPRITE_BANK_BASE + 0x1a4;
    otherTable = entityTable + 0x90;
    cache = gPaletteCache;
    FreeUnlockedPaletteSlots(cache);

    {
        u8 *arr = (u8 *)gActorCategories;
        s32 idx = gActorCategory;

        if (*(s32 *)(arr + idx * 0x34) == 0) {
            p = *(void **)entityTable;
            BindPaletteSlot(cache, 7, *((u8 *)p + 0x14));
            p = *(void **)otherTable;
            BindPaletteSlot(cache, 0xf, *((u8 *)p + 0x4c));
        } else {
            u8 *sub;

            p = *(void **)entityTable;
            BindPaletteSlot(cache, 9, *((u8 *)p + 0x14));
            p = *(void **)otherTable;
            BindPaletteSlot(cache, 0xc, *((u8 *)p + 0x4c));
            p = *(void **)otherTable;
            sub = (u8 *)p + 0xe0;
            BindPaletteSlot(cache, 0xe, sub[0x14]);
            p = *(void **)otherTable;
            sub = (u8 *)p + 0x8c;
            BindPaletteSlot(cache, 0xd, sub[0x14]);
            p = *(void **)otherTable;
            sub = (u8 *)p + 0x150;
            BindPaletteSlot(cache, 8, sub[0x14]);
        }
    }

    {
        void *iconArray = gHud;
        u8 *arr = (u8 *)gActorCategories;
        s32 idx = gActorCategory;

        ConfigureHudParts(iconArray, *(s32 *)(arr + idx * 0x34) != 0);
    }
}

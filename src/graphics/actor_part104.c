#include "core.h"
#include "actor_anim.h"
#include "vram_pool.h"

extern void InitObjTileFreeList(void *addr);
extern void InitSpriteFrameOamQueue(void);
extern void InitSpriteFrameCache(void);
extern void ***gUnknown_030012D0;
extern struct tile_asset_cache *gUnknown_030012B8;
extern void sub_8006EA8(struct tile_asset_cache *self);
extern void sub_8006D40(struct tile_asset_cache *self, s32 slot, s32 index);
extern s32 gUnknown_03001380;
extern void *gUnknown_03001318;
extern void sub_802732C(void *arr, s32 flag);

/* Pins the current category's tile-cache slots that every actor part
 * shares - the type-0 sprite family gets 2 slots (7/0xf), the type-1/2
 * family gets 5 (9/0xc/0xe/0xd/8) pulled from two ROM-side sub-tables
 * (entityTable/otherTable below) whose own shapes aren't reversed yet -
 * kept as raw offsets per docs/workflow.md step 7. Finishes by (re)
 * building the category's status-icon OAM row via sub_802732C, gated on
 * whether the category's type is nonzero.
 *
 * `gStaticData_08175558[category]`'s address is deliberately computed
 * without a named `struct category_descriptor *` local - keeping it in
 * an unnamed rolling register (matching this compiler's own choice for
 * an expression it never has to keep alive past one immediate use)
 * rather than letting the register allocator give it a stable "home"
 * register, which changes which register ends up holding the combined
 * pointer. Likewise the final `sub_802732C` call's arguments are broken
 * into their own local variables in the exact order this compiler
 * evaluates them (icon array pointer, then the array base, then the
 * index) to reproduce the ROM's exact register assignment - see
 * docs/matching/issue-XX-0x080291a4-actor.md. */
void SetupActorVramPool(void)
{
    u8 *entityTable;
    u8 *otherTable;
    struct tile_asset_cache *cache;
    void *p;

    InitObjTileFreeList((void *)0x06011400);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();

    entityTable = (u8 *)**gUnknown_030012D0 + 0x1a4;
    otherTable = entityTable + 0x90;
    cache = gUnknown_030012B8;
    sub_8006EA8(cache);

    {
        u8 *arr = (u8 *)gStaticData_08175558;
        s32 idx = gUnknown_03001380;

        if (*(s32 *)(arr + idx * 0x34) == 0) {
            p = *(void **)entityTable;
            sub_8006D40(cache, 7, *((u8 *)p + 0x14));
            p = *(void **)otherTable;
            sub_8006D40(cache, 0xf, *((u8 *)p + 0x4c));
        } else {
            u8 *sub;

            p = *(void **)entityTable;
            sub_8006D40(cache, 9, *((u8 *)p + 0x14));
            p = *(void **)otherTable;
            sub_8006D40(cache, 0xc, *((u8 *)p + 0x4c));
            p = *(void **)otherTable;
            sub = (u8 *)p + 0xe0;
            sub_8006D40(cache, 0xe, sub[0x14]);
            p = *(void **)otherTable;
            sub = (u8 *)p + 0x8c;
            sub_8006D40(cache, 0xd, sub[0x14]);
            p = *(void **)otherTable;
            sub = (u8 *)p + 0x150;
            sub_8006D40(cache, 8, sub[0x14]);
        }
    }

    {
        void *iconArray = gUnknown_03001318;
        u8 *arr = (u8 *)gStaticData_08175558;
        s32 idx = gUnknown_03001380;

        sub_802732C(iconArray, *(s32 *)(arr + idx * 0x34) != 0);
    }
}

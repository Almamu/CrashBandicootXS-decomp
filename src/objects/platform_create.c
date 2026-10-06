#include "core.h"
#include "gobj_1a794.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #25, ROM 0x0801A878-0x0801AB34: CreatePlatform, the level
 * object spawner (`new` + inlined constructor InitPlatform, spawn-record
 * lookup through the level header at *gEntityFlags, per-type mover
 * attachment). See include/gobj_1a794.h and
 * docs/matching/archive/issue-25-level-objects.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS, see
 * docs/matching/archive/old-agbcc-retry.md). Under the current agbcc reload's
 * scratch-register rotation never matched and the function was parked as
 * NAKED; under old_agbcc it matches with every register pin removed. Two
 * workarounds remain: the hand-written outgoing-argument block for
 * CreatePlatformMover's stack-passed byte (old_agbcc also widens a stack-passed
 * `u8` argument to a word `str`), and the barrier on the palette
 * nibble's 0xF. */
struct gobj *CreatePlatform(u16 id, u16 x, u16 y, u16 index, s32 kind)
{
    struct gobj *obj;
    struct spawn_rec *rec;
    u32 type;
    struct mover_stack_args args;
    struct mover *m;

    obj = GobjInit(OperatorNew(0x80));
    obj->id = id;
    obj->x = x << 8;
    obj->y = y << 8;
    {
        const struct level_entity_list *lvl = gEntityFlags->list;
        const u16 *offsets = lvl->paramOffsets;

        rec = (struct spawn_rec *)((const u8 *)lvl->params + offsets[index]);
        type = rec->type;
    }
    switch (kind)
    {
    case 4:
        type = 2;
        break;
    case 5:
        type = 3;
        break;
    case 8:
        type = 7;
        break;
    case 3:
    case 9:
    case 10:
    case 11:
    case 12:
        type = 4;
        break;
    case 6:
        type = 6;
        break;
    }
    switch (type)
    {
    case 0:
        obj->type = 0;
        break;
    case 1:
        {
            s32 one = 1;

            obj->type = one;
            {
                void *mem = OperatorNew(0x38);
                s32 dX = rec->distX;
                s32 dY = rec->distY;
                u32 fX = rec->dirX != 0;
                u32 fY = rec->dirY != 0;

                *(volatile u8 *)&args.dirY = fY;
                *(volatile s32 *)&args.kind = one;
                m = MOVER_NEW(mem, dX, dY, fX);
            }
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        if (rec->flag)
            obj->flags |= 0x10;
        break;
    case 2:
        obj->type = 2;
        break;
    case 3:
        obj->type = 3;
        sub_801B2A8(obj, rec->flags & 1);
        break;
    case 4:
        obj->type = 4;
        break;
    case 5:
        obj->type = 5;
        {
            void *mem = OperatorNew(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 5;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    case 6:
        obj->type = 6;
        if (GetBossIndex(gLevelState) != 1)
        {
            void *mem = OperatorNew(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 6;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        else
            m = CreateCortexBossPlatformMover(OperatorNew(0x38));
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    case 7:
        obj->type = 7;
        {
            void *mem = OperatorNew(0x38);

            *(volatile u8 *)&args.dirY = 0;
            *(volatile s32 *)&args.kind = 7;
            m = MOVER_NEW(mem, 0, 0, 0);
        }
        obj->mover = m;
        MOVER_CALL2(m, m18, obj);
        break;
    }
    AddToPartList(gUnknown_030012EC, obj);
    obj->anim = (void *)(SPRITE_BANK_BASE + 0x1D4);
    obj->tag = kind;
    ResetSpriteFrameTimer(obj);
    ResetSpriteFrameIndex(obj);
    SetSpriteAnimDone(obj, 0);
    {
        /* two separate bit clears in the ROM */
        u8 *p = &obj->mirror;
        s32 v = ~0x10;

        v &= *p;
        v &= ~0x20;
        *p = v;
    }
    {
        struct anim_rec *r = obj->anim->records;
        u32 id;
        u8 *p;
        u32 low;
        s32 mask;

        r += obj->tag;
        id = GetPaletteSlot(gPaletteCache, r->paletteId);
        p = &obj->slot;
        low = 0xF;
        /* hide 0xF from cse, which would build ~0xF as 0xF - 0x1F */
        asm("" : "+r"(low));
        id &= low;
        mask = ~0xF;
        *p = (mask & *p) | id;
    }
    return obj;
}

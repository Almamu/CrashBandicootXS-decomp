#include "core.h"
#include "actor.h"

extern s32 sub_8007114(struct actor *self, void *box);

/* `part+0x25 == 1` is the same fast override seen in
 * sub_8007F78/sub_8007FD8; otherwise defers to `sub_8007114` (already
 * matched in graphics.c), forwarding `box` straight through
 * unmodified. */
s32 sub_8008304(struct actor *part, void *box)
{
    s32 result = 0;
    register u8 *addr asm("r2") = (u8 *)part + 0x25;
    register u8 byteVal asm("r2");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if ((u8)sub_8007114(part, box)) {
        result = 1;
    }
    return result;
}

extern u8 sub_8006FE4(struct actor *self);

/* Same `part+0x25` fast-override shape as `sub_8008304` above,
 * deferring to `sub_8006FE4` (already matched in `graphics.c`)
 * instead - a single-argument sibling, so the address scratch
 * naturally lands in `r1` instead of `r2` (no second call argument to
 * keep out of the way). */
s32 sub_8008328(struct actor *part)
{
    s32 result = 0;
    register u8 *addr asm("r1") = (u8 *)part + 0x25;
    register u8 byteVal asm("r1");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if (sub_8006FE4(part)) {
        result = 1;
    }
    return result;
}

/* Always-true stub. */
s32 sub_800834C(void)
{
    return 1;
}

extern void sub_8007A84(void *self, void *part);
extern void *gUnknown_030012CC;

/* Tail-calls `sub_8007A84` (already matched in `actor_part.c`) with
 * the global `gUnknown_030012CC` as `self`. */
void sub_8008350(void *part)
{
    sub_8007A84(gUnknown_030012CC, part);
}

extern void sub_8008044(struct actor *part);
extern void *sub_803AD7C(void *arg0, void *arg1);

/* Advances `part`'s animation timer (`sub_8008044`), then resolves two
 * `table+N`/`table+N+4` offset/pointer slot pairs (the same convention
 * documented for `sub_8006FE4`/`sub_8007F78`) into `sub_803AD7C` calls
 * - table+0x60/+0x64 first, then table+8/+0xc. */
void sub_8008364(struct actor *part)
{
    sub_8008044(part);

    {
        void *table = part->table;
        void *slot = (u8 *)table + 0x60;
        s32 offset = *(s16 *)slot;
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)slot + 4);

        sub_803AD7C(addr, ptr);
    }
    {
        void *table = part->table;
        s32 offset = *(s16 *)((u8 *)table + 8);
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)table + 0xc);

        sub_803AD7C(addr, ptr);
    }
}
asm(".align 2, 0");

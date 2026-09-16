#include "core.h"
#include "actor.h"

extern void *gUnknown_03001308;
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *sub_803AD7C(void *arg0, void *fn);

/* Same "extended screen box" filter shape as `sub_800891C`'s own
 * `boxB` pass above (the plain 240x160 GBA screen region, in Q8, at
 * the `gUnknown_03001308` sub-object's own position) - iterates
 * `manager`'s array (`manager+0xc` base, `manager+4` count),
 * filtering each `part` whose `table+0x30/0x34`-driven trampoline
 * passes into a second output array (`manager+0x10` base,
 * `manager+8` count). */
void sub_8008C80(void *manager)
{
    s32 i;
    s32 box[4];
    register void *P asm("r0") = gUnknown_03001308;
    register void *subObj asm("r0");
    s32 v0, v1;
    s32 v2, v3;

    subObj = *(void **)((u8 *)P + 0x10);
    v0 = *(s32 *)subObj << 8;
    v1 = *(s32 *)((u8 *)subObj + 4) << 8;
    box[0] = v0;
    box[1] = v1;
    v2 = 0xf0 << 8;
    v3 = 0xa0 << 8;
    box[2] = v2;
    box[3] = v3;

    *(s32 *)((u8 *)manager + 8) = 0;

    for (i = 0; i < *(s32 *)((u8 *)manager + 4); i++) {
        register void **arrBase asm("r1") = *(void ***)((u8 *)manager + 0xc);
        register s32 idx asm("r0") = i * 4;
        register void **slot asm("r0");
        void *part;

        slot = (void **)((u8 *)idx + (s32)arrBase);
        part = *slot;

        {
            register u8 *tbl asm("r1") = *(u8 **)((u8 *)part + 0x18);
            register s32 offset asm("r0") = *(s16 *)(tbl + 0x30);
            register void *addr asm("r0");
            register void *fn asm("r2");

            addr = (u8 *)part + offset;
            fn = *(void **)(tbl + 0x34);

            if ((u8)sub_803AD80(addr, box, fn)) {
                s32 outCount = *(s32 *)((u8 *)manager + 8);
                void **outArr = *(void ***)((u8 *)manager + 0x10);

                outArr[outCount] = part;
                *(s32 *)((u8 *)manager + 8) = outCount + 1;
            }
        }
    }
}

/* Fires a `part->table+0x50/0x54`-driven trampoline (constant arg 3)
 * via `sub_803AD80` for every entry in `manager`'s array
 * (`manager+0xc` base, `manager+4` count) that isn't already `NULL`,
 * then clears every slot and resets both the count (`manager+4`) and
 * the second array's count (`manager+8`) to 0 - a full teardown of
 * both this manager's arrays. */
void sub_8008CEC(void *manager)
{
    s32 i;

    for (i = 0; i < *(s32 *)((u8 *)manager + 4); i++) {
        s32 idx = i;
        void **arrBase = *(void ***)((u8 *)manager + 0xc);
        void *part = arrBase[idx];

        if (part != 0) {
            register u8 *rec asm("r1") = *(u8 **)((u8 *)part + 0x18) + 0x50;
            register s32 offset asm("r0") = *(s16 *)rec;
            register void *addr asm("r0");
            register void *fn asm("r2");

            addr = (u8 *)part + offset;
            fn = *(void **)(rec + 4);
            sub_803AD80(addr, (void *)3, fn);
        }
        arrBase = *(void ***)((u8 *)manager + 0xc);
        arrBase[idx] = 0;
    }
    *(s32 *)((u8 *)manager + 4) = 0;
    *(s32 *)((u8 *)manager + 8) = 0;
}

/* Iterates `manager`'s array (`manager+0x10` base, `manager+8`
 * count): for each `part`, fires a `table+0x48/0x4c`-driven
 * trampoline via `sub_803AD7C` and skips unless the result equals
 * `arg1` (a caller-supplied selector); skips unless `part->flags`
 * bit 2 is set; then fires a *second*, unconditional `table+8/0xc`
 * trampoline (return value discarded). */
void sub_8008D30(void *manager, s32 arg1)
{
    s32 i;

    for (i = 0; i < *(s32 *)((u8 *)manager + 8); i++) {
        void **arr = *(void ***)((u8 *)manager + 0x10);
        void *part = arr[i];
        register u8 *rec asm("r1") = *(u8 **)((u8 *)part + 0x18) + 0x48;
        register s32 offset asm("r0") = *(s16 *)rec;
        register void *addr asm("r0");
        register void *fn asm("r1");
        s32 result;

        addr = (u8 *)part + offset;
        fn = *(void **)(rec + 4);
        result = (s32)sub_803AD7C(addr, fn);

        if (result != arg1) {
            continue;
        }
        {
            register u8 byte asm("r1") = *((u8 *)part + 0xc);
            register s32 shifted asm("r0") = byte >> 2;
            register s32 mask asm("r1") = 1;
            register s32 test asm("r0");

            test = shifted & mask;
            if (!test) {
                continue;
            }
        }
        {
            register u8 *tbl2 asm("r1") = *(u8 **)((u8 *)part + 0x18);
            register s32 offset2 asm("r0") = *(s16 *)(tbl2 + 8);
            register void *addr2 asm("r0");
            register void *fn2 asm("r1");

            addr2 = (u8 *)part + offset2;
            fn2 = *(void **)(tbl2 + 0xc);
            sub_803AD7C(addr2, fn2);
        }
    }
}
asm(".align 2, 0");

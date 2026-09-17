#include "core.h"

/* Same "self" object family as actor_part39.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. */

#if NON_MATCHING
extern s32 gUnknown_030013C8;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern u8 *GetAnimFrameData(void *self);
extern s32 sub_803B060(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);

/* NOT YET BYTE-MATCHING - see docs/matching/issue-50-actor-2a69c.md.
 * Computes an OBJ scale factor from `self`'s `+0x34` distance metric and
 * `self`'s part-table's `+0x10` field (via `sub_803ADB4`), then a second
 * scale from `gUnknown_030013C8` (via the same helper) used to project
 * `self`'s `+0x1c`/`+0x20` position through `sub_8029E98`/`sub_8029EB4`'s
 * screen-space offsets into on-screen X/Y. Fetches the current anim
 * frame (`GetAnimFrameData`), centers it (frame's own width/height
 * bytes, doubled if the first scale factor exceeds `0xff`), culls if
 * fully off-screen, and - if visible - builds the OAM attribute word
 * (position, `sub_803B060`'s flag byte, an oversize-scale bit, and a
 * priority/palette nibble from `self+0x18`/`self+0x14`) and calls
 * `SetupSpriteFrameOam` with the first scale factor as its OBJ-affine
 * "priority" argument. Every load/store, branch and call is confirmed
 * correct and every register matches the ROM's own choice except one:
 * the second half's `frame[1]` read (the mirror of the first half's
 * `frame[0]` read, done via `r7`) is read by the ROM through `r0` - a
 * leftover, never-reloaded copy of `GetAnimFrameData`'s own return
 * value that's still sitting untouched in `r0` at that point, saving a
 * register materialization the ROM's real compiler recognized was
 * redundant. This agbcc build does not perform that specific redundant-
 * load/value-reuse optimization: any C phrasing that references `frame`
 * a second time re-derives it from its one canonical register (`r7`)
 * instead, and any attempt to pin a second `register` alias onto `r0`
 * at that point emits an explicit (extra) copy instruction instead of
 * silently reusing `r0`'s still-valid contents, which is the opposite
 * of what's needed. Every other one of this ~120-instruction function's
 * bytes matches. */
void UpdateAnimatedActorPart(void *selfArg)
{
    register u8 *self asm("r6") = selfArg;
    register s32 scale asm("r8");
    s32 dist = *(s32 *)(self + 0x34);
    register s32 scaleY asm("r5");
    s32 posX;
    s32 posY;
    u8 *frame;
    u32 flag;
    register s32 delta0 asm("r1");
    s32 delta1;

    scale = sub_803ADB4(dist << 8, *(s32 *)(*(u8 **)(self + 0x30) + 0x10));
    scaleY = sub_803ADB4(gUnknown_030013C8 << 0xc, dist);

    {
        s32 off = sub_8029E98();
        register s32 tmp asm("r1") = *(s32 *)(self + 0x20);

        posX = tmp * scaleY;
        posX >>= 0xc;
        posX += off;
        posX >>= 8;
    }

    {
        s32 off = sub_8029EB4();
        register s32 tmp asm("r1") = *(s32 *)(self + 0x1c);

        tmp = tmp * scaleY;
        tmp >>= 0xc;
        tmp += off;
        posY = tmp >> 8;
    }

    frame = GetAnimFrameData(self);

    flag = 0;
    {
        register s32 scaleCmp asm("r1");

        asm("mov %0, r8" : "=r" (scaleCmp));
        if (scaleCmp <= 0xff) {
            flag = 0x200;
        }
    }

    if (flag != 0) {
        register u8 b asm("r3") = frame[0];
        delta0 = b << 3;
    } else {
        register u8 b asm("r3") = frame[0];
        delta0 = b << 2;
    }

    if (flag != 0) {
        delta1 = frame[1] << 3;
    } else {
        delta1 = frame[1] << 2;
    }

    posY -= delta0;
    posX -= delta1;

    if (posX > 0x9f) return;
    if (posX + delta1 * 2 < 0) return;
    if (posY > 0xef) return;
    {
        register s32 shifted asm("r0") = delta0 << 1;

        if (posY + shifted < 0) return;
    }

    if (scale != 0x100) {
        flag |= 0x100;
    }

    {
        s32 attr = sub_803B060(self);
        u32 packed = (posX & 0xff) | (((u32)posY & 0x1ff) << 16) | attr | flag;
        u32 v = *(u32 *)(self + 0x18);
        u32 pre = v << 0xc;
        register u32 shifted asm("r0");
        u32 attr2;

        if (*(s32 *)(self + 0x14) & 0x8000) {
            shifted = (pre | 0x800) << 0x10;
        } else {
            shifted = v << 0x1c;
        }
        attr2 = shifted >> 0x10;

        SetupSpriteFrameOam(frame, packed, attr2, scale);
    }
}
#endif /* NON_MATCHING */

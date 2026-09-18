#include "core.h"

/* Same "self" object family as actor_part39.c - see that file's header
 * comment and docs/matching/issue-50-actor-2a69c.md. */

extern s32 gUnknown_030013C8;
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern u8 *GetAnimFrameData(void *self);
extern s32 sub_803B060(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);

/* Computes an OBJ scale factor from `self`'s `+0x34` distance metric and
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
 * "priority" argument. See docs/matching/issue-50-actor-2a69c.md for
 * the two register-pinning gaps this needed to close (the `frame[1]`
 * read reusing GetAnimFrameData's still-live `r0` return instead of the
 * `r7` copy used for `frame[0]`, and the `flag` spill-across-call
 * around `sub_803B060` needing to be written out explicitly since it's
 * pinned to a caller-saved register). */
void UpdateAnimatedActorPart(void *selfArg)
{
    register u8 *self asm("r6") = selfArg;
    register s32 scale asm("r8");
    s32 dist = *(s32 *)(self + 0x34);
    register s32 scaleY asm("r5");
    s32 posX;
    s32 posY;
    /* The ROM keeps GetAnimFrameData's raw return value alive in r0
     * (unclobbered by the flag computation below) and reads frame[1]
     * through it directly, while frame[0] is read twice through r7 - an
     * explicit copy (`adds r7, r0, #0`) made right after the call. A
     * single plain `frame` local always collapses back to one canonical
     * register for every access, so this pins the call result to r0 and
     * makes the r7 copy an explicit second variable, matching the ROM's
     * own register choice for each of the three reads instead of gcc's
     * single-register default. */
    register u8 *frame asm("r0");
    u8 *frameCopy;
    register u32 flag asm("r2");
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
    frameCopy = frame;

    flag = 0;
    {
        register s32 scaleCmp asm("r1");

        asm("mov %0, r8" : "=r" (scaleCmp));
        if (scaleCmp <= 0xff) {
            flag = 0x200;
        }
    }

    if (flag != 0) {
        register u8 b asm("r3") = frameCopy[0];
        delta0 = b << 3;
    } else {
        register u8 b asm("r3") = frameCopy[0];
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
        /* `flag` is pinned to r2 (matching the ROM's own choice, needed
         * to reproduce the r7/r0 split above), but a plain
         * `register ... asm("r2")` variable is the caller's own
         * responsibility across a call - gcc, unlike with an ordinary
         * pseudo-register it owns, won't insert protective spill code
         * for it automatically the way it does for a normal local stuck
         * in a caller-saved register. The ROM's real compiler still
         * needed r2 preserved here (no free callee-saved register left -
         * r4-r8 are already dist/scaleY/self/frame/scale) and spilled it
         * to a dedicated stack word around this one call; reproduce that
         * explicitly (same technique as sub_8000140's r2-across-SWI
         * save/restore, see docs/matching.md) rather than relying on the
         * compiler to notice on its own. */
        u32 flagStack[1];
        s32 attr;
        register void *callArg asm("r0") = self;

        asm volatile("str %1, %0" : "=m" (flagStack[0]) : "r" (flag));
        attr = sub_803B060(callArg);

        {
            u32 packed;
            u32 v;
            u32 pre;
            register u32 shifted asm("r0");
            u32 attr2;

            packed = posX & 0xff;
            packed |= ((u32)posY & 0x1ff) << 16;
            packed |= attr;
            /* Reload right before its one remaining use, matching the
             * ROM's own late `ldr r2, [sp]` placement (immediately
             * before the final `orrs r3, r2`) rather than eagerly right
             * after the call. */
            asm volatile("ldr %0, %1" : "=r" (flag) : "m" (flagStack[0]));
            packed |= flag;

            v = *(u32 *)(self + 0x18);
            pre = v << 0xc;

            if (*(s32 *)(self + 0x14) & 0x8000) {
                shifted = (pre | 0x800) << 0x10;
            } else {
                shifted = v << 0x1c;
            }
            attr2 = shifted >> 0x10;

            SetupSpriteFrameOam(frameCopy, packed, attr2, scale);
        }
    }
}
/* Trailing zero-fill padding to the next 4-byte boundary, matching the
 * ROM's own (the assembler's default NOP pad - "mov r8, r8" - mismatches
 * here; see the matching_decomp_alignment_fix precedent). */
asm(".align 2, 0");

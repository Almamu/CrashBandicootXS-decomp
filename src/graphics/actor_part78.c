#include "core.h"
#include "actor.h"

/* GitHub issue #9/#10: 0x0800A884 - the same big, still-unnamed "part"
 * object family as `actor_part15.c`/`actor_part48.c`; raw offset casts
 * throughout for the same reason those files give. */

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800A0FC(void *self);
extern void sub_80231EC(void *arg0, s32 arg1);
extern void *sub_80083B8(void *part);
extern s32 sub_8026BC0(void *arg0, s32 x, s32 y);
extern void *gUnknown_03001308;
extern void *gUnknown_030012C0;
extern u8 gStaticData_0816B300[];

/* A per-frame "reentrancy guard"-shaped wrapper (only runs if
 * `self+0xc` bit 7 is set): fires `self->table+0x70`'s trampoline via
 * `sub_803AD7C`, then calls `sub_800A0FC` (still raw) with the global
 * `gUnknown_03001308+0x2a` flag held set for the duration. If
 * `self+0xac` (a pointer, cleared here) was non-null, sets `self+0x68`
 * bit 3 and clears the `+0x100`/`+0x102`/`+0x103` flag bytes. Then
 * dispatches on `gUnknown_03001308+0x29` (a pending-action "kind"
 * byte, cleared back to 0 by every path here): kind 0 additionally
 * resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` is exactly 8; kinds
 * 1/5/7/9 (`gStaticData_0816B300`'s index scheme - see the `case`
 * labels below) are no-ops beyond the shared reset; kind 1 also sets
 * `self+0xc` bit 6, clears `+0x8c`, calls `sub_80231EC`, and fires the
 * `self->table+0x68` trampoline (arg 1); kind 5 sets the `+0x100`
 * flag; kind 7 sets `+0x102`; kind 10 sets `+0x103`. Finally, looks up
 * the current keyframe record (`sub_80083B8`, already parked in
 * `actor_part5.c`) and picks a `{s16 x, s16 y}` offset table off its
 * `+4` byte's upper nibble - the exact same `sub_80084C4`
 * (`actor_part6.c`) case-to-block mapping (0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else -> the fixed fallback
 * `gStaticData_0816B300`) - applies it (mirrored by `self+0x28` bit 4)
 * to `self`'s de-Q8'd position, and probes the result via
 * `sub_8026BC0` (still raw). A hit (code 6) snaps `self`'s Y position
 * down to the next multiple of 8 (unless `+0x101` is already set) and
 * fires the table+0x68 trampoline with code `0x17`; any other code
 * fires the same trampoline with code `0x18` if `+0x101` is set.
 * Returns the (possibly just-updated) `self+0x68` state byte.
 *
 * The three `sub_803AD88` calls each also load (but never pass through
 * r0-r3) the table's own `+4` function-pointer field right next to the
 * `+0` offset they do use - a "dead read" the ROM performs anyway,
 * same established idiom as `sub_80096C0`'s own `sub_803AD88` calls in
 * `actor_part11.c` (`register void *deadRead asm("r4") = *(void
 * *volatile *)(...)`).
 *
 * PARKED, NOT BYTE-MATCHING: every load, store, branch and call in this
 * reconstruction is confirmed correct against the ROM - both jump-table
 * dispatches reproduce the ROM's own tables exactly (matching
 * `sub_80084C4`'s established case-to-block mapping for the second
 * one), and the shared `_0800A9F4`-style tail store merges via the
 * `goto storeAndDispatch` pair the same way the ROM merges those two
 * blocks into one physical store. An isolated compile of the leading
 * ~40 instructions (through the `sub_800A0FC` call) was iterated to an
 * exact register-for-register match with the ROM (the bit-7 test
 * pinned so the byte loads into r1 and the shifted result into r0, a
 * pinned r4=0 constant kept live across both calls, and the
 * `0x105`-offset/`&gUnknown_03001308` register choices reproduced via
 * natural (unpinned) recomputation instead of caching either address
 * across the whole function, which had been pulling in unwanted
 * r8/r9 spills). The rest of the function (the 10-way "kind" dispatch
 * and the keyframe-lookup/camera-probe tail) has not yet been through
 * the same register-pin iteration - this is real, understood C, just
 * not yet confirmed byte-exact past that first block. See
 * docs/matching/issue-9-10-0x0800a884-graphics.md. */
#if NON_MATCHING
u8 sub_800A884(void *selfArg)
{
    u8 *self = selfArg;
    u8 *storeAddr;
    u8 storeVal;
    u8 kind;

    {
        register u8 byte asm("r1") = self[0xc];
        register u32 result asm("r0") = byte >> 7;
        if (!result) {
            goto end;
        }
    }

    {
        register u8 zero asm("r4") = 0;
        self[0x68] = zero;
        self[0x105] = zero;
    }
    {
        u8 *tbl = *(u8 **)(self + 0x18) + 0x70;
        s16 offset = *(s16 *)tbl;
        void *addr = self + offset;
        void *fn = *(void **)(tbl + 4);
        sub_803AD7C(addr, fn);
    }
    self[0x105] = 1;

    *((u8 *)gUnknown_03001308 + 0x2a) = 1;
    sub_800A0FC(self);
    *((u8 *)gUnknown_03001308 + 0x2a) = 0;

    if (*(void **)(self + 0xac) != 0) {
        self[0x68] |= 8;
        *(void **)(self + 0xac) = 0;
        self[0x100] = 0;
        self[0x102] = 0;
        self[0x103] = 0;
    }

    kind = *((u8 *)gUnknown_03001308 + 0x29);
    if (kind == 0) {
        if (self[0x68] == 8) {
            self[0x102] = 0;
            self[0x103] = 0;
            storeAddr = self + 0x100;
            storeVal = 0;
            goto storeAndDispatch;
        }
        goto dispatch;
    }

    {
        u8 idx = kind - 1;

        if (idx <= 9) {
            switch (idx) {
            case 0:
                self[0xc] |= 0x40;
                *(s32 *)(self + 0x8c) = 0;
                sub_80231EC(gUnknown_030012C0, 0);
                {
                    u8 *tbl = *(u8 **)(self + 0x18) + 0x68;
                    s16 offset = *(s16 *)tbl;
                    void *addr = self + offset;
                    register void *deadRead asm("r4") = *(void *volatile *)(tbl + 4);
                    (void)deadRead;
                    sub_803AD88(addr, 0, 1, 0);
                }
                break;
            case 1:
            case 2:
            case 3:
            case 5:
            case 7:
            case 8:
                break;
            case 4:
                self[0x102] = 0;
                self[0x103] = 0;
                self[0x100] = 1;
                break;
            case 6:
                self[0x103] = 0;
                self[0x100] = 0;
                self[0x102] = 1;
                break;
            case 9:
                self[0x102] = 0;
                self[0x100] = 0;
                self[0x103] = 1;
                break;
            }
        }
    }
    storeAddr = (u8 *)gUnknown_03001308 + 0x29;
    storeVal = 0;
storeAndDispatch:
    *storeAddr = storeVal;

dispatch:
    {
        void *info = sub_80083B8(self);
        u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
        void *tbl;

        switch (type) {
        case 0:
            tbl = (u8 *)info + 0x24;
            break;
        case 6:
            tbl = (u8 *)info + 0x14;
            break;
        default:
            tbl = gStaticData_0816B300;
            break;
        }

        {
            s32 x = *(s32 *)self >> 8;
            s32 y = *(s32 *)(self + 4) >> 8;
            s32 code;

            if (self[0x28] & 0x10) {
                x -= *(s16 *)tbl;
            } else {
                x += *(s16 *)tbl;
            }
            y += *(s16 *)((u8 *)tbl + 2);

            code = sub_8026BC0(gUnknown_03001308, x, y);
            if (code == 6) {
                if (self[0x101] == 0) {
                    s32 snap = (((u32)y & 0x00FFFFF8) + 7) - y;
                    *(s32 *)(self + 4) += snap << 8;
                    {
                        u8 *tbl2 = *(u8 **)(self + 0x18) + 0x68;
                        s16 offset = *(s16 *)tbl2;
                        void *addr = self + offset;
                        register void *deadRead asm("r4") = *(void *volatile *)(tbl2 + 4);
                        (void)deadRead;
                        sub_803AD88(addr, 0, 0x17, 0);
                    }
                }
            } else {
                if (self[0x101] != 0) {
                    u8 *tbl2 = *(u8 **)(self + 0x18) + 0x68;
                    s16 offset = *(s16 *)tbl2;
                    void *addr = self + offset;
                    register void *deadRead asm("r4") = *(void *volatile *)(tbl2 + 4);
                    (void)deadRead;
                    sub_803AD88(addr, 0, 0x18, 0);
                }
            }
        }
    }

end:
    return self[0x68];
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

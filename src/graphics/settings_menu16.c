#include "core.h"
#include "actor.h"
#include "icon_manager.h"
#include "pause_screen_results.h"

extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

/* Draws `label1`/`label2` (a small "N/M" fraction readout - a row's
 * count over its fixed total, e.g. the icon-row helpers in
 * sub_80057E0/sub_80058C0 pass each row's formatted count/total
 * scratch buffers) on the composite pause/options screen's results
 * icons: draws `label1` at `gUnknown_030012DC`'s current position
 * (slot 2), copies that position (x-2, y unchanged) into
 * `gUnknown_030012E0` and draws a literal `/` there (slot 4), then
 * repositions `gUnknown_030012DC` to (that x-5, that y+8) and draws
 * `label2` there (slot 2). `self` is unused - the ROM never reads it
 * either.
 *
 * Still NAKED (transcribed from the ROM). The C draft under
 * NON_MATCHING is 73 halfwords off (16 bytes long) under either
 * compiler. The ROM keeps 0x130 (record) and 0x110 (posX) in r4/r7
 * across the calls but re-materializes 0x114 (posY) in the first
 * reposition, and later derives it as r7 + 4. The draft CSEs 0x114 as
 * well, which pushes label2 and the two icon-manager addresses into
 * r8-r10.
 * Mix-6 pass: reading the first posY through a fresh `"=r"/"0"` 0x114
 * offset reproduces the ROM's first half up to one register (the
 * &gUnknown_030012E0 pool address goes to r8 instead of r6). The
 * second half (0x110 copied to r6, then r7 += 4 for posY) does not come
 * out of any spelling tried: `p[0]/p[1]`, `*p++`, a `u32 *q` store pair
 * (best, 62 hw, 4 bytes long).
 * Inline-argument-order pass: `r6 = r7; r7 += 4` is postreload move2add
 * on fresh 0x110/0x114 constant loads, so the second half's offsets
 * must not be CSE'd with the first half's 0x110. `do { } while (0)`
 * draws and per-field `"=r"/"0"` offsets didn't do that (56+ hw).
 * Last-six pass: `asm volatile` escapes on the second half's source or
 * destination manager (or both), an escaped 0x110 offset and an
 * escaped `&posX` pair were all 67-75 hw.
 * Last-eight pass (docs/matching/last-eight-naked-retry.md): 45 hw, same
 * size and instruction shape. Each posX offset and the second half's
 * posY load offset are opaque constants (`OFF`), so CSE keeps them apart
 * and reload rebuilds the second half's from r7 as in the ROM; the posY
 * stores use plain constants. Left: the &gUnknown_030012E0 load is
 * hoisted to the top (it's a local there), the first half's 0x110 lands
 * in r2 instead of r7, and the second half rebuilds 0x110 instead of
 * copying it from r7 (`adds r6, r7, #0`).
 * Last-nine pass (docs/matching/last-nine-naked-retry.md): 14 hw, same
 * size. The ROM's r7 is never a pseudo's register (local-alloc can't
 * use the frame pointer): it is reload's register for the plain 0x110
 * constant, which it inherits for the posX store and then copies/bumps
 * in the second half (reload_cse `adds r6, r7, #0`, move2add
 * `adds r7, #4`). So the first half uses plain field accesses, the
 * &gUnknown_030012E0 local is assigned where it's first used, and the
 * second half reads posY through an inline (plain address, rebuilt by
 * reload in r7). The first half and the prologue now match. Left: the
 * second half's `ox` gets r2 (the ROM has r6, so r0-r3 must be busy when
 * it is allocated) and reload uses r6 for 0x114 instead of r7. */
#if NON_MATCHING
/* A constant in a register CSE can't see through (the asm emits no
 * code), so each use site gets its own pseudo. */
#define OFF(K) ({ s32 _o; asm("" : "=r"(_o) : "0"(K)); _o; })
/* A u32 field at a register offset, still marked as a struct access so
 * it doesn't alias the icon-manager pointer globals. */
#define AT(m, o) (((struct { u32 v; } *)((u8 *)(m) + (o)))->v)

static inline void set_icon_mgr_pos(struct icon_manager *m, s32 ox, u32 x, u32 y)
{
    AT(m, ox) = x;
    m->posY = y;
}

/* Read through an inline so the posY offset reaches the load as a plain
 * (reg + 0x114) address: reload then builds it (in r7, which move2add
 * turns into `adds r7, #4`), instead of expand forcing it into a pseudo
 * that CSE shares with the first half's posY load. */
static inline u32 get_icon_mgr_posy(struct icon_manager *m)
{
    return m->posY;
}

/* Calls the icon manager's `record->slots[slot]` method on `label` (a
 * gcc 2.x virtual call; sub_803AD80 is `_call_via_r2`). */
#define DRAW_ICON_SLOT(mgrExpr, slot, label)                                          \
    {                                                                                 \
        struct icon_manager *_m = (mgrExpr);                                          \
        struct icon_record *_r = _m->record;                                          \
        sub_803AD80((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    }

void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    struct icon_manager **pdc = &gUnknown_030012DC;
    struct icon_manager **pe0;

    DRAW_ICON_SLOT(*pdc, 2, label1);
    {
        u32 x, y;
        struct icon_manager *d = *pdc;

        x = d->posX;
        y = d->posY;
        pe0 = &gUnknown_030012E0;
        set_icon_mgr_pos(*pe0, 0x110, x - 2, y);
    }
    DRAW_ICON_SLOT(*pe0, 4, (void *)0x2f);
    {
        struct icon_manager *e = *pe0;
        s32 ox = OFF(0x110);

        set_icon_mgr_pos(*pdc, ox, AT(e, ox) - 5, get_icon_mgr_posy(e) + 8);
    }
    DRAW_ICON_SLOT(*pdc, 2, label2);
}
#else
NAKED void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "mov r8, r2\n\t"
    "ldr r5, 1f\n\t"
    "ldr r0, [r5]\n\t"
    "mov r4, #0x98\n\t"
    "lsl r4, r4, #1\n\t"
    "add r2, r0, r4\n\t"
    "ldr r3, [r2]\n\t"
    "mov r6, #0x20\n\t"
    "ldrsh r2, [r3, r6]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r2, [r3, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r1, [r5]\n\t"
    "mov r7, #0x88\n\t"
    "lsl r7, r7, #1\n\t"
    "add r0, r1, r7\n\t"
    "ldr r2, [r0]\n\t"
    "mov r3, #0x8a\n\t"
    "lsl r3, r3, #1\n\t"
    "add r0, r1, r3\n\t"
    "ldr r3, [r0]\n\t"
    "ldr r6, 2f\n\t"
    "ldr r0, [r6]\n\t"
    "sub r2, #2\n\t"
    "add r1, r0, r7\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r1, r0, r4\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x30\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x34]\n\t"
    "mov r1, #0x2f\n\t"
    "bl sub_803AD80\n\t"
    "ldr r1, [r6]\n\t"
    "add r6, r7, #0\n\t"
    "add r0, r1, r6\n\t"
    "ldr r2, [r0]\n\t"
    "add r7, #4\n\t"
    "add r0, r1, r7\n\t"
    "ldr r3, [r0]\n\t"
    "ldr r0, [r5]\n\t"
    "sub r2, #5\n\t"
    "add r3, #8\n\t"
    "add r1, r0, r6\n\t"
    "str r2, [r1]\n\t"
    "add r2, r7, #0\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r4, r0, r4\n\t"
    "ldr r2, [r4]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "mov r1, r8\n\t"
    "bl sub_803AD80\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "1: .4byte gUnknown_030012DC\n"
    "2: .4byte gUnknown_030012E0\n"
    );
}
#endif

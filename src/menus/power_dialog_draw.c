#include "core.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "actor.h"
#include "text.h"
#include "util.h"
#include "system.h"
#include "pause_menu.h"
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* gLevelTable's time-trial thresholds (`level_info.times`, level.h):
 * CountSapphireRelics/CountGoldRelics/CountPlatinumRelics each count how
 * many of a caller's 20 records fall between two adjacent thresholds
 * (times[0]/[1] for one function, times[1]/[2] for the next, and just
 * times[2] alone for the simplest one).
 * CountSapphireRelics/CountGoldRelics read through inline asm rather than plain
 * struct field access on purpose: gcc's CSE otherwise shares the
 * "table[i]" address between the two threshold reads even though the
 * ROM recomputes it fresh for each one (see docs/matching.md, "Matching
 * decompilation"). */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

/* Sets an icon manager's draw position. Both coordinates are inline
 * arguments, so gcc evaluates them (and re-reads the manager global)
 * before either store - the ROM's order. */
static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Positions two OAM icons flanking a number (drawn via DrawWrappedTextInBox in
 * between) - centers each icon horizontally from its rendered pixel
 * width (the manager's `record->slots[0]` method, a gcc 2.x virtual call
 * through `_call_via_r2`), at fixed Y coordinates, then
 * draws it with `slots[2]`. Parked as NAKED for a long time over a
 * register-letter gap; closed by computing the centered X into its own
 * local before passing it to the inline setter (passing the expression
 * straight in swapped the X/Y and 0x130/240 registers) - see
 * docs/matching/archive/strag3-naked-retry.md. */
void DrawPowerDialog(struct sub_8006700_actor *arg0)
{
    struct aabb box;
    s32 w;
    s32 n;
    u32 x;
    struct icon_record *r;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    DrawSpriteWithOffset((struct actor *)arg0->field_18, 0, 0);
    r = gLargeFont->record;
    w = _call_via_r2((u8 *)gLargeFont + r->slots[0].offset, arg0->field_10, r->slots[0].ptr);
    x = (u32)(240 - w) >> 1;
    set_icon_mgr_pos(gLargeFont, x, 0x2d);
    r = gLargeFont->record;
    _call_via_r2((u8 *)gLargeFont + r->slots[2].offset, arg0->field_10, r->slots[2].ptr);
    SetAabbPos(&box, 0x10, 0x6a);
    SetAabbSize(&box, 0xd0, 0x35);
    DrawWrappedTextInBox(arg0->field_14, gSmallFont, &box, 0);
    n = GetUiText(0x2e);
    r = gSmallFont->record;
    w = _call_via_r2((u8 *)gSmallFont + r->slots[0].offset, n, r->slots[0].ptr);
    x = (u32)(240 - w) >> 1;
    set_icon_mgr_pos(gSmallFont, x, 0x90);
    r = gSmallFont->record;
    _call_via_r2((u8 *)gSmallFont + r->slots[2].offset, n, r->slots[2].ptr);
    HideUnusedOamEntries(gOamBuffer);
}

void AnimatePowerDialog(struct sub_8006700_actor *arg0)
{
    arg0->field_1c++;
    AdvanceSpriteAnim((struct box_part *)arg0->field_18);
}

void CommitPowerDialogFrame(struct sub_8006700_actor *arg0)
{
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
    *(vu16 *)REG_ADDR_BG0HOFS = arg0->field_1c >> 3;
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = arg0->field_20;
    *(vu16 *)REG_ADDR_BLDY = (u32)(arg0->field_24.raw << 27) >> 27;
    *(vu16 *)REG_ADDR_DISPCNT = arg0->field_28.all;
}

void DestroyPowerDialog(struct sub_8006700_actor *arg0, u32 arg1)
{
    struct actor *field18;
    u8 *p;

    field18 = &arg0->field_18->base;
    if (field18 != NULL) {
        p = (u8 *)field18->table + 0x50;
        _call_via_r2((u8 *)field18 + *(s16 *)p, 3, *(void **)(p + 4));
    }
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}

void ShowTurboRunDialog(void)
{
    ShowPowerDialog(0x3F, 0x43, 1);
}

void ShowTornadoSpinDialog(void)
{
    ShowPowerDialog(0x3E, 0x42, 0);
}

void ShowDoubleJumpDialog(void)
{
    ShowPowerDialog(0x3D, 0x41, 2);
}

void ShowSuperBodySlamDialog(void)
{
    ShowPowerDialog(0x3C, 0x40, 3);
}

s32 GetProgressLives(void *arg0)
{
    return (u32)(*(u8 *)arg0 << 25) >> 25;
}

/* Counts records whose derived value is <= times[2] alone (no lower
 * bound) - the simplest of the three CountGoldRelics/CountSapphireRelics/CountPlatinumRelics
 * threshold checks. */
s32 CountPlatinumRelics(void *arg0)
{
    s32 count;
    u8 *p;
    u8 *bound;
    u8 *base;
    s32 i;
    u16 raw;
    s32 val;

    count = 0;
    base = (u8 *)gLevelTable;
    bound = base + 0x10; /* &gLevelTable[0].times[2] */
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            if (val <= *(u32 *)bound) {
                count++;
            }
        }
        bound += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

/* Counts records whose derived value falls in (times[2], times[1]]
 * of the matching gLevelTable entry - see the comment at the top of this
 * file for why this reads through inline asm instead of
 * entry->times[1]/entry->times[2]. */
s32 CountGoldRelics(void *arg0)
{
    register u8 *p asm("r3");
    register s32 i asm("r5");
    register s32 count asm("r6");
    register s32 offset asm("r4");
    register s32 val asm("r1");
    register s32 addr asm("r0");
    register u16 raw asm("r0");

    count = 0;
    offset = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0x10\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
                if (val > *(u32 *)addr) {
                    count++;
                }
            }
        }
        offset += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

/* Same shape as CountGoldRelics above, one threshold pair up:
 * (times[1], times[0]]. */
s32 CountSapphireRelics(void *arg0)
{
    register u8 *p asm("r3");
    register s32 i asm("r5");
    register s32 count asm("r6");
    register s32 offset asm("r4");
    register s32 val asm("r1");
    register s32 addr asm("r0");
    register u16 raw asm("r0");

    count = 0;
    offset = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        raw = *(u16 *)(p + 4);
        val = raw >> 3;
        if (val != 0) {
            asm volatile("add %0, %1, #0\n\tadd %0, %0, #8\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
            if (val <= *(u32 *)addr) {
                asm volatile("add %0, %1, #0\n\tadd %0, %0, #0xc\n\tadd %0, %2, %0" : "=r"(addr) : "r"(gLevelTable), "r"(offset));
                if (val > *(u32 *)addr) {
                    count++;
                }
            }
        }
        offset += 0x24;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

s32 CountRelics(void *arg0)
{
    s32 total;
    s32 b;
    s32 c;

    total = CountSapphireRelics(arg0);
    b = CountGoldRelics(arg0);
    c = CountPlatinumRelics(arg0);
    total += b;
    total += c;
    return total;
}

s32 CountGems(void *arg0)
{
    register u8 *p asm("r2");
    register s32 total asm("r4");
    register s32 i asm("r3");
    s32 result;
    u8 flags;
    u8 byte;

    total = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
        p += 4;
        i--;
    } while (i >= 0);
    total += (((u32)*((u8 *)arg0 + 0x64) << 30) >> 31) + (((u32)*((u8 *)arg0 + 0x64) << 29) >> 31);
    flags = *((u8 *)arg0 + 2);
    result = total + (((u32)flags << 31) >> 31);
    result += ((u32)flags << 29) >> 31;
    result += ((u32)flags << 28) >> 31;
    result += ((u32)flags << 30) >> 31;
    return result;
}

s32 CountClearGems(void *arg0)
{
    u8 *p;
    s32 total;
    s32 i;
    u8 byte;

    total = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
        p += 4;
        i--;
    } while (i >= 0);
    byte = *((u8 *)arg0 + 0x64);
    total += (((u32)byte << 30) >> 31) + (((u32)byte << 29) >> 31);
    return total;
}

s32 CountCrystals(void *arg0)
{
    register u8 *p asm("r1");
    register s32 i asm("r2");
    register s32 count asm("r3");
    register u8 byte asm("r4");
    register u32 bit asm("r0");

    count = 0;
    p = (u8 *)arg0;
    i = 0x13;
    do {
        byte = p[4];
        bit = (u32)byte << 31;
        bit >>= 31;
        count += bit;
        p += 4;
        i--;
    } while (i >= 0);
    return count;
}

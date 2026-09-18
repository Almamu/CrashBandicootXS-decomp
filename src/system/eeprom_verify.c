#include "core.h"

/* The read-verify and write-retry pair of the DMA3 bit-serial EEPROM
 * cluster split across src/system/eeprom_util.c (the still-parked
 * `sub_803AB54`/`sub_803AC04` read/write primitives, asm/
 * code_3_2_20e_ab54.s) and this file - its own translation unit per
 * docs/workflow.md step 4, since it isn't adjacent to any
 * already-matched file's functions. See docs/matching/issue-69-*.md. */

/* Same shape as src/system/timer_util.c/eeprom_util.c's own
 * `struct EepromConfig` - redeclared here rather than shared via a
 * header, see eeprom_util.c's copy of this note. */
struct EepromConfig {
    u32 unk0;
    u16 maxCount;
    u16 waitcntBits;
    u8 addrBitCount;
    u8 pad[3];
};

extern struct EepromConfig *gUnknown_03001634;

extern s32 sub_803AB54(u16 addr, u16 *dest);
extern s32 sub_803AC04(u16 addr, u16 *src);

/* Reads `addr` back into a local buffer via `sub_803AB54` and compares
 * it against `expected[0..3]`; returns 0x8000 if any word doesn't
 * match, 0x80FF if `addr` is out of range (its own check, redundant
 * with `sub_803AB54`'s internal one - the ROM never looks at
 * `sub_803AB54`'s own return value at all, so this reconstruction
 * doesn't either), or 0 if all four match. */
s32 sub_803ACE0(u16 addr, u16 *expected)
{
    u16 buf[4];
    s32 result;
    u16 *p;
    u8 i;

    result = 0;
    if (addr >= gUnknown_03001634->maxCount) {
        return 0x80FF;
    }

    sub_803AB54(addr, buf);
    p = buf;
    for (i = 0; i <= 3; i++) {
        u16 a = *expected;
        u16 b = *p;
        p++;
        expected++;
        if (a != b) {
            result = 0x8000;
            break;
        }
    }
    return result;
}

/* Writes `data` to EEPROM at `addr`, verifies it, and retries the
 * write+verify pair up to 3 times total if either step fails. Returns
 * the last attempt's result code (0 on eventual success). `result` is
 * `u16` (not `s32`, even though the two callees it stores return `s32`)
 * to match the ROM truncating each call's return value to 16 bits
 * before comparing it against 0. */
s32 sub_803AD38(u16 addr, u16 *data)
{
    u8 attempt;
    u16 result;

    for (attempt = 0; attempt <= 2; attempt++) {
        result = sub_803AC04(addr, data);
        if (result != 0) {
            continue;
        }
        result = sub_803ACE0(addr, data);
        if (result != 0) {
            continue;
        }
        break;
    }
    return result;
}
asm(".align 2, 0");

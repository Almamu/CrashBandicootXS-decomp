#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md.
 *
 * Two BG1 picture loaders (issue #56) sharing one repack loop: for each
 * of `rows` rows of `cols` map entries, add the tile base
 * (`sub_8029AC4() - 0x200`) to the entry, OR in a 4-bit palette bank
 * taken alternately from the low and high nibble of the next byte, and
 * store it at `0x0600D000 + row * 0x40` (columns 0x20 and up go to the
 * second screen block, +0x7C0).
 *
 * - `sub_802F7B0(pic)`: DMA3-copies `pic`'s 0x200-byte palette to
 *   PLTT, reads cols/rows (s16 at +0x200/+0x202) and the tile count
 *   (+0x204), runs the loop over the map at +0x208 with the nibbles
 *   after the tiles, then enables BG1 (DISPCNT |= 0x200, BG1CNT =
 *   0x5A07) and DMA3-copies the tiles to VRAM + sub_8029AC4() * 32.
 * - `sub_802F8E8(nibbles, map, cols, rows)`: the same loop on explicit
 *   arguments.
 *
 * `sub_802F8E8` is real C, and is the shared loop: declared `inline`
 * ahead of `sub_802F7B0`, which inlines it (the ROM's 7B0 loop has the
 * separately strength-reduced `dest[c]`/`dest[c + 0x3e0]` pointers an
 * inlined copy gets and the standalone compile doesn't). gcc defers
 * emitting an inlinable function to the end of the file, which is why
 * 8E8 sits after 7B0 in the ROM. It needs old_agbcc (the whole object is
 * built with it; 7B0 is NAKED so it doesn't care) and two spellings: the
 * map entry read as `v = *map; v += base;` (the map pointer then outranks
 * `v` for r5) and the high nibble masked, `(*nib >> 4) & 0xf` (without
 * the mask the next-row pointer and row+1 swap ip/r9).
 *
 * `sub_802F7B0` stays NAKED: the draft below (8E8 inlined) has the ROM's
 * instructions but two register swaps - the nibble pointer and `dest`
 * trade r5/r6 (the nibble pointer, 13 refs over 56 insns, narrowly
 * outranks `dest`, 9 over 40, in global allocation) and `cols` and
 * row+1 trade r8/sl (a near-exact priority tie). Declaration order,
 * argument spellings and pseudo-number shifts don't move either. */
extern s32 sub_8029AC4(void);

inline void sub_802F8E8(u8 *nib, u16 *map, s32 cols, s32 rows)
{
    u16 *dest;
    s32 base;
    s32 odd;
    s32 r;
    s32 c;

    dest = (u16 *)0x0600D000;
    base = sub_8029AC4() - 0x200;
    odd = 0;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            s32 v, n;

            v = *map;
            v += base;
            map++;
            if (odd) {
                n = (*nib >> 4) & 0xf;
                nib++;
            } else {
                n = *nib & 0xf;
            }
            odd ^= 1;
            n = (n << 12) | v;
            if (c < 0x20)
                dest[c] = n;
            else
                dest[c + 0x3e0] = n;
        }
        dest += 0x20;
    }
}

#if NON_MATCHING
void sub_802F7B0(u8 *pic)
{
    s32 cols, rows;
    u32 tiles;
    u8 *tileData;

    DmaCopy32(3, pic, (void *)PLTT, 0x400);
    cols = *(s16 *)(pic + 0x200);
    rows = *(s16 *)(pic + 0x202);
    pic += 0x204;
    tiles = *(u32 *)pic;
    pic += 4;
    tileData = pic + ((cols * rows + 1) / 2) * 4;
    sub_802F8E8(tileData + (tiles << 5), (u16 *)pic, cols, rows);
    REG_DISPCNT |= 0x200;
    REG_BG1CNT = 0x5A07;
    {
        void *vd = (void *)(VRAM + sub_8029AC4() * 32);

        DmaCopy16(3, tileData, vd, tiles * 32);
    }
}
#else
NAKED void sub_802F7B0(void *srcArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x14\n\t"
        "ldr r2, 3f\n\t"
        "str r0, [r2]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "str r1, [r2, #4]\n\t"
        "ldr r1, 4f\n\t"
        "str r1, [r2, #8]\n\t"
        "ldr r1, [r2, #8]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #2\n\t"
        "add r1, r0, r2\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r8, r3\n\t"
        "add r2, #2\n\t"
        "add r1, r0, r2\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "str r3, [sp]\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldm r0!, {r2}\n\t"
        "str r2, [sp, #4]\n\t"
        "mov r1, r8\n\t"
        "mul r1, r3\n\t"
        "add r1, #1\n\t"
        "lsr r2, r1, #0x1f\n\t"
        "add r1, r1, r2\n\t"
        "asr r1, r1, #1\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r0, r1\n\t"
        "str r1, [sp, #8]\n\t"
        "ldr r3, [sp, #4]\n\t"
        "lsl r1, r3, #5\n\t"
        "ldr r5, [sp, #8]\n\t"
        "add r6, r1, r5\n\t"
        "add r7, r0, #0\n\t"
        "ldr r5, 5f\n\t"
        "bl sub_8029AC4\n\t"
        "ldr r1, 6f\n\t"
        "add r1, r0, r1\n\t"
        "str r1, [sp, #0xc]\n\t"
        "mov r2, #0\n\t"
        "mov ip, r2\n\t"
        "mov r0, #0\n\t"
        "ldr r3, [sp]\n\t"
        "cmp r0, r3\n\t"
        "bge 12f\n\t"
    "1:\n\t"
        "mov r4, #0\n\t"
        "mov r1, #0x40\n\t"
        "add r1, r1, r5\n\t"
        "mov sb, r1\n\t"
        "add r0, #1\n\t"
        "mov sl, r0\n\t"
        "cmp r4, r8\n\t"
        "bge 11f\n\t"
        "mov r2, #0xf8\n\t"
        "lsl r2, r2, #3\n\t"
        "add r3, r5, r2\n\t"
        "add r2, r5, #0\n\t"
    "2:\n\t"
        "ldrh r5, [r7]\n\t"
        "ldr r0, [sp, #0xc]\n\t"
        "add r5, r5, r0\n\t"
        "str r5, [sp, #0x10]\n\t"
        "add r7, #2\n\t"
        "mov r1, ip\n\t"
        "cmp r1, #0\n\t"
        "beq 7f\n\t"
        "ldrb r5, [r6]\n\t"
        "lsr r1, r5, #4\n\t"
        "add r6, #1\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "3: .4byte 0x040000D4\n"
    "4: .4byte 0x80000100\n"
    "5: .4byte 0x0600D000\n"
    "6: .4byte 0xFFFFFE00\n"
    "7:\n\t"
        "mov r1, #0xf\n\t"
        "ldrb r0, [r6]\n\t"
        "and r1, r0\n\t"
    "8:\n\t"
        "mov r0, #1\n\t"
        "mov r5, ip\n\t"
        "eor r5, r0\n\t"
        "mov ip, r5\n\t"
        "lsl r1, r1, #0xc\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "orr r1, r0\n\t"
        "cmp r4, #0x1f\n\t"
        "bgt 9f\n\t"
        "strh r1, [r2]\n\t"
        "b 10f\n\t"
    "9:\n\t"
        "strh r1, [r3]\n\t"
    "10:\n\t"
        "add r3, #2\n\t"
        "add r2, #2\n\t"
        "add r4, #1\n\t"
        "cmp r4, r8\n\t"
        "blt 2b\n\t"
    "11:\n\t"
        "mov r5, sb\n\t"
        "mov r0, sl\n\t"
        "ldr r1, [sp]\n\t"
        "cmp r0, r1\n\t"
        "blt 1b\n\t"
    "12:\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #0x13\n\t"
        "ldrh r0, [r2]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #2\n\t"
        "add r1, r3, #0\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r2]\n\t"
        "ldr r1, 13f\n\t"
        "ldr r5, 14f\n\t"
        "add r0, r5, #0\n\t"
        "strh r0, [r1]\n\t"
        "bl sub_8029AC4\n\t"
        "lsl r0, r0, #5\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, 15f\n\t"
        "ldr r3, [sp, #8]\n\t"
        "str r3, [r2]\n\t"
        "str r0, [r2, #4]\n\t"
        "ldr r5, [sp, #4]\n\t"
        "lsl r0, r5, #4\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x18\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2, #8]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "add sp, #0x14\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "13: .4byte 0x0400000A\n"
    "14: .4byte 0x00005A07\n"
    "15: .4byte 0x040000D4\n"
    );
}
#endif

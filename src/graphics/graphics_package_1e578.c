#include "core.h"
#include "graphics_package.h"

/* GitHub issue #30's `LoadGraphicsPackage` itself - the "front door" of the
 * 0x0801E578-0x0801FA3C cluster docs/rom_map.md anchored on this function.
 * Loads a palette, a tileset, and a tilemap for one BG background, using
 * the same 5-field `struct bg_package` descriptor
 * `LoadBg2Background`/`LoadObjSpriteTiles` (src/graphics/level_graphics.c)
 * already established, plus the small 0x10-byte scratch "self" buffer
 * `sub_801E640`/`sub_801E644`/`sub_801E8F8`/`sub_801E964`/`sub_801E96C`
 * (src/graphics/graphics_package_1e640.c, _1e8f8.c, _1e964.c) build up
 * field-by-field before calling here - see those files' docs for what's
 * known about each field:
 *  - `self+0x00`: a tile-bank index (used here as the tileset's VRAM char
 *    base, `<<14` i.e. one 0x4000-byte char-block per unit).
 *  - `self+0x04`: a tilemap screen-block index (used here as the
 *    destination tilemap's VRAM base, `<<11` i.e. one 0x800-byte
 *    screen-block per unit).
 *  - `self+0x08`: a palette-bank index, used both for the palette's own
 *    destination (`<<5`, one 0x20-byte 16-color bank per unit) and as the
 *    per-tile palette-select nibble merged into every copied tilemap entry
 *    (`<<12`, bits 12-15 of a BG tilemap halfword).
 *  - `self+0x0c`: the packed BG control byte `sub_801E644` already builds
 *    (priority/char-base/screen-base/color-mode) - this function only
 *    touches its top bit (bit 7, the 256-color/16-color mode select), set
 *    when the palette asset's own declared color count (its header word's
 *    top 24 bits) is bigger than 0x20.
 *  - `pkg` (`struct bg_package *`): `width`/`height`/`paletteAsset`/
 *    `tileAsset`/`mapAsset`, loaded via `LoadTaggedAsset` into their VRAM
 *    destinations above, except `mapAsset` which loads into a
 *    `sub_8026EC0` scratch buffer that gets remapped one fixed 0x40-byte
 *    (32-tile) row at a time - OR-ing in the palette-bank nibble derived
 *    from `self+8` into every copied tilemap halfword - into the real
 *    destination tilemap, then freed via `sub_8026EB4`.
 *
 * Byte-correct, but as a NAKED transcription, not real decompiled C -
 * tracked as parked, same as `sub_801E644`/`sub_801E688`/`sub_801E990`
 * elsewhere in this cluster (see docs/matching/issue-30-graphics-loading.md).
 * Every operation and register choice here was already confirmed correct
 * against the ROM in a much earlier pass via heavy register pinning
 * (`self`->r5, `pkg`->r6, `mapBuf`->r8, `src`->r7, the packed
 * palette-bank mask->ip, `width`->r4, `height`->sl, the row stride->sb,
 * the per-row dest pointer->r0, the inner-loop src/dest/count triple->
 * r2/r1/r3) plus several instruction-order fixes for this compiler's
 * "skips an apparently-redundant copy" habit (matching_decomp_register_
 * pinning memory) - every gap closed but one: this function needs r6
 * free for one more scratch temp (the loaded tilemap halfword, right
 * before it's ORed with the palette-bank mask) *inside* the same window
 * `pkg`'s own r6 binding is still technically in scope for. Reusing r6
 * there (matching the ROM's own `ldrh r6,...`) made gcc's allocator, for
 * reasons that never yielded to further restructuring (statement
 * reordering, barriers, scoping tricks), stop treating `src`'s r7 as
 * needing a callee-save push/pop at all, even though the function body
 * still writes and later reads it through the whole outer loop - the
 * exact same "compiler drops a genuinely-live register from its own
 * auto-generated push/pop list under register pressure" limitation
 * already resolved this way for `sub_80240E4`
 * (src/system/game_loop8.c), `sub_801E688`/`sub_801E990` in this same
 * cluster, and documented as still-open for `LoadBg2Background`
 * (src/graphics/level_graphics.c, issue #65). Converted to `NAKED` and
 * transcribed instruction-for-instruction from the ROM disassembly
 * instead - trivial once the plain-C version's instruction content was
 * already confirmed correct, since this reuses the exact same sequence,
 * just with a hand-written prologue/epilogue (matching the ROM's own
 * `push`/`mov`-dance/`push`/`sub sp` and `add sp`/`pop`/`mov`-dance/
 * `pop`/`pop`/`bx` shape used elsewhere in this cluster) instead of
 * relying on the compiler to synthesize one. */
NAKED void LoadGraphicsPackage(u8 *self, struct bg_package *pkg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "add r6, r1, #0\n\t"
        "ldr r0, [r6, #8]\n\t"
        "ldr r0, [r0]\n\t"
        "lsr r0, r0, #8\n\t"
        "cmp r0, #0x20\n\t"
        "bhi 1f\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "and r0, r1\n\t"
        "b 2f\n\t"
        "1:\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r2, [r5, #0xc]\n\t"
        "orr r0, r2\n\t"
        "2:\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, [r6, #8]\n\t"
        "ldr r1, [r5, #8]\n\t"
        "lsl r1, r1, #5\n\t"
        "mov r2, #0xa0\n\t"
        "lsl r2, r2, #0x13\n\t"
        "add r1, r1, r2\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r0, [r6, #0xc]\n\t"
        "ldr r1, [r5]\n\t"
        "lsl r1, r1, #0xe\n\t"
        "mov r4, #0xc0\n\t"
        "lsl r4, r4, #0x13\n\t"
        "add r1, r1, r4\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r0, [r6, #0x10]\n\t"
        "ldr r0, [r0]\n\t"
        "lsr r0, r0, #9\n\t"
        "lsl r0, r0, #1\n\t"
        "bl sub_8026EC0\n\t"
        "mov r8, r0\n\t"
        "ldr r0, [r6, #0x10]\n\t"
        "mov r1, r8\n\t"
        "bl LoadTaggedAsset\n\t"
        "ldr r0, [r5, #8]\n\t"
        "lsl r0, r0, #0xc\n\t"
        "mov ip, r0\n\t"
        "mov r7, r8\n\t"
        "ldr r0, [r5, #4]\n\t"
        "lsl r0, r0, #0xb\n\t"
        "add r0, r0, r4\n\t"
        "mov r2, #0\n\t"
        "ldr r3, [r6, #4]\n\t"
        "cmp r2, r3\n\t"
        "bge 3f\n\t"
        "ldr r4, [r6]\n\t"
        "mov sl, r3\n\t"
        "lsl r6, r4, #1\n\t"
        "mov sb, r6\n\t"
        "4:\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x40\n\t"
        "str r1, [sp]\n\t"
        "add r5, r2, #1\n\t"
        "cmp r4, #0\n\t"
        "ble 5f\n\t"
        "add r2, r7, #0\n\t"
        "add r1, r0, #0\n\t"
        "add r3, r4, #0\n\t"
        "6:\n\t"
        "mov r0, ip\n\t"
        "ldrh r6, [r2]\n\t"
        "orr r0, r6\n\t"
        "strh r0, [r1]\n\t"
        "add r2, #2\n\t"
        "add r1, #2\n\t"
        "sub r3, #1\n\t"
        "cmp r3, #0\n\t"
        "bne 6b\n\t"
        "5:\n\t"
        "add r7, sb\n\t"
        "ldr r0, [sp]\n\t"
        "add r2, r5, #0\n\t"
        "cmp r2, sl\n\t"
        "blt 4b\n\t"
        "3:\n\t"
        "mov r0, r8\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "bl sub_8026EB4\n\t"
        "7:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Instruction stream is 188 bytes (94 halfwords), already a multiple of 4
 * - this is a no-op alignment, kept for consistency with the rest of this
 * cluster's NAKED transcriptions (matching_decomp_alignment_fix
 * precedent). */
asm(".align 2, 0");

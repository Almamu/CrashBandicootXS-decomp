# Issues #64/#65: NAKED retry (0x08034AA4-0x08036CF4)

This pass retried the NAKED functions in `src/graphics/actor_part131.c`
(issue #64) and `src/graphics/graphics_loading_35780.c` (issue #65).
Seven of the twelve are now real C. Five are still NAKED; four of those
have a near-miss draft under `#if NON_MATCHING`.

The lead going in was the per-file `-fno-strength-reduce` that
`graphics_loading_35780.o` already uses for `InitVvLogoPieces`. It turned out
not to matter: none of the #65 drafts changed with the flag on or off.
What closed them was source shape. The #64 functions had been parked for
"too many live values across calls", and that was not the real obstacle
either.

## Closed

| Function | File | Compiler | Technique |
|---|---|---|---|
| `DrawContinuePrompt` | actor_part131.c | either | Each label draw is the icon manager's virtual call `record->slots[n]` (`_call_via_r2`). This is settings_menu.c's `ICON_TEXT_CALL`, with `set_icon_mgr_pos`. It matched on the first compile. |
| `InitCredits` | actor_part131.c | old_agbcc | The DISPCNT byte OR loads its constant before the `ldrb`. The two icon-manager steps are `static inline` helpers that take the manager as a parameter: `IconSetBase(m, base)` stores `field_108` and calls slot 6, and `IconReserveVram(cursor, m)` wraps `ReserveObjVram`. This keeps the `0x108`/`0x12c`/`0x130` offsets rematerialized per use, as the ROM does. The E0 base value (`DC->field_12c`) goes into a local before the call, so it is read before E0. An empty `asm("")` after the E0 reset produces no code. It adds one insn to the live ranges that cross it, which gives `&gPaletteCache`/`&gObjVramCursor` r4 and `&gSmallFont` r6. No C-level alternative was found (see below). |
| `DrawCreditsText` | actor_part131.c | old_agbcc | Plain C once the source order matches. The icon position goes through `set_icon_mgr_pos`, so x/y are loaded before the stores. The tile count is `rows * cols`. The OAM request is a stack `struct popup_oam` with size/palette bitfields, cleared by `CpuSet` from a zero word. `node->index * 2 + node->index` reproduces the ROM's double `ldrb`. |
| `UpdateCreditsText` | actor_part131.c | old_agbcc | The glyph height/width reads go through `GlyphHeightAt`/`GlyphWidthAt`. These inline accessors take the field's base address first and then add the `index * 0x18` offset, so loop.c hoists `&glyphs[0].height` into a stack slot as in the ROM. The first height read spells the index `index * 2 + index`, which re-reads the byte just stored. The cursor advance uses its own temporary `q`, so the old cursor gives `q[1]` and `q + 1` becomes the next `p`. The popup-y placement keeps `top = y + 0xa0` as its own local. |
| `TitleScreenCheatInput` | graphics_loading_35780.c | old_agbcc | `struct held_pressed_pair input = gKeys;` and then `input.held & 0x100`. The ROM's "0x100 built in r4, copied to r1" comes from the struct copy. Found by brute-forcing 20 gate spellings. |
| `UpdateVvLogoPieces` | graphics_loading_35780.c | old_agbcc | The drain loop's end pointer is its own local, set before the `do`/`while`. The loop condition's invariant was hoisted after the two constants; as a local it is computed first, as in the ROM, and that also fixes the tail's `0x444`/`0x448` registers. |
| `LoadUniversalLogoBg` | graphics_loading_35780.c | old_agbcc | Both branches of the remap store through `*dest++` themselves. Cross-jumping merges them back into the ROM's single `strh`, but global alloc counted `dest`'s references twice, so it outranks `y`/`bg2cnt` and gets r4. |

`actor_part131.o` joined the Makefile's `OLD_AGBCC_OBJS`. The whole
object matches under old_agbcc, which `InitCredits`, `DrawCreditsText` and
`UpdateCreditsText` need, and its previously matched functions compile the
same under both compilers. `struct map_screen` now spells out the five
0x18-byte `struct popup_glyph` records at `+0x1c`, replacing the old
`asset0`-`asset4` placeholders. `struct popup_node` (the 0x18-byte text
popup node) is named too.

## Not closed

| Function | State | What was observed |
|---|---|---|
| `LoadCreditsLogos` | new draft | Everything lines up except one thing. gcc computes the palette-slot address `slot << 5` before the tile loops, next to the `slot + 1`/`i + 1` biv increments it also moves there (the ROM moves those two as well), and spills it to one extra stack word. The ROM computes it at the palette copy. The copy-loop spelling, `u32 slot`, several `palSlots` forms and `-fno-strength-reduce` all leave it unchanged, and it happens even with the copy loop deleted. The insns come from the first loop pass (uids 402-404 in `-dL`). |
| `ResetTitleLogoPieces` | draft, 18 hw | The goto-loop shape is exact; only register choice is left. The ROM has stride/slot in r4/r5 and zero/seedBase/counter in ip/r8/sb. The draft gets them swapped. All 5040 declaration orders, `(i << 3) + base` in integer or pointer form, an inline `HoldBase()`, no-op self-assignments and a real `do`/`while` were tried. The real loop hoists everything and is worse (73). |
| `RunTitleScreen` | draft, 100 hw | The same seed loop, so the same register swap, which shifts the rest. |
| `DrawTitleLogoPieces` | draft, ~190 lines of diff | The OAM builders match. The remaining differences are scattered register and stack-slot choices in the two slot loops. This was not attempted beyond re-measuring. |
| `DrawVvLogoPieces` | not attempted | 1160 bytes, the same shape as `DrawTitleLogoPieces`. |

## Tools

Scratch helpers (not committed) that made this fast:

- a per-function side-by-side disassembly diff (draft vs ROM, either compiler, extra flags);
- a whole-file checker that compiles an object with either compiler and reports each function's halfword diff against the ROM (it confirmed `actor_part131.c` matches under old_agbcc before the switch);
- a variant runner that applies single text replacements to a draft and counts the remaining diff lines. It found the `TitleScreenCheatInput` and `InitCredits` fixes.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`: `crashbandicootxs.gba: OK`.

## Later pass

The issues #64/#65 second NAKED retry closed `ResetTitleLogoPieces`,
`RunTitleScreen` and `DrawTitleLogoPieces`, and split the file at `TitleScreenCheatInput`
(`graphics_loading_35d1c.c`). `LoadCreditsLogos` is still NAKED: its early
`slot << 5` comes from GCSE's PRE. See
[issue-64-65-naked-retry-2.md](issue-64-65-naked-retry-2.md).

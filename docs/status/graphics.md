# Status: graphics

`src/graphics/` (core rendering only) - OAM/sprite rendering, screen
fades, palette blending, per-actor animation frames, text layout. The
per-instance actor object family, the HUD, pause/options overlay UI,
and asset-loading/trigger-effect code also live under `src/graphics/`
on disk but are tracked in their own category pages - see
[actor.md](./actor.md), [hud.md](./hud.md), [overlay_ui.md](./overlay_ui.md),
and [graphics_loading.md](./graphics_loading.md).

## Matched

- `src/graphics/graphics.c`: `AllocVramDmaQueue`, `QueueVramDmaTransfer`,
  `FreeVramDmaQueue`, `FlushVramDmaQueue`, `sub_8006B0C`, `sub_8006AF4`,
  `sub_8006AC8`, `sub_8006AAC`, `sub_8006A78`, `sub_8006A84`, `sub_8006A90`,
  `sub_8006A48`, `sub_8006A14`, `sub_80069E8`, `sub_800697C`,
  `sub_8006C28`, `sub_8006C30`, `sub_8006C38`, `sub_8006C44`, `sub_8006C4C`,
  `sub_8006C58`, `sub_8006C84`, `sub_8006CD0`, `sub_8006CE8`, `sub_8006D08`,
  `sub_8006D40`, `sub_8006D50`, `sub_8006D68`, `sub_8006D84`, `sub_8006DA0`,
  `sub_8006DC8`, `sub_8006DF8`, `sub_8006E64`, `sub_8006EA8`, `sub_8006EF0`,
  `sub_8006F5C`, `sub_8006F94`, `sub_8006FB4`, `sub_8006FC8`, `nullsub_1`,
  `sub_8006FE4`, `sub_8007048`, `nullsub_11`, `sub_80070D4`, `sub_80070E8`,
  `sub_80070EC`, `sub_800710C`, `sub_8007110`, `sub_8007114`,
  `sub_8007174`, `sub_800719C`, `nullsub_12`, `sub_80071E4`,
  `sub_800722C`, `sub_8007230`, `sub_800725C`, `sub_8007278`,
  `sub_8007284`, `sub_8007290`, `sub_800729C`, `sub_80072A8`,
  `sub_80072B4`, `sub_80072C0`, `sub_80072CC`, `sub_80072D8`,
  `sub_800731C`, `sub_8007328`, `sub_8007334`, `sub_8007340`,
  `sub_800734C`, `sub_8007358`, `sub_8007364`, `sub_800736C`,
  `sub_8007374`, `sub_8007378`, `sub_800737C`, `sub_8007388`,
  `sub_8007398`, `sub_80073A0`, `sub_80073B0`, `sub_80073B4`,
  `sub_80073B8`, `sub_80073BC`
- `src/graphics/oam_count.c`: `sub_8006700`, `sub_8006714`, `sub_8006770`,
  `sub_80067A4`, `sub_80067B4`, `sub_80067C4`, `sub_80067D4`, `sub_80067E4`,
  `sub_80067EC`, `sub_8006820`, `sub_8006864`, `sub_80068A8`, `sub_80068CC`,
  `sub_8006920`, `sub_800695C`
- `src/graphics/fade_util.c`: `sub_80012AC`, `sub_800132C`
- `src/graphics/palette_blend.c`: `sub_80013FC`
- `src/graphics/actor_anim.c`: `GetAnimFrameBaseOffset`
- `src/graphics/fade_screen_mode.c` (new file - `sub_8001510`) and
  `src/graphics/fade_screen_mode2.c` (new file - `sub_800153C`,
  `sub_8001550`, `sub_8001564`, `sub_8001578`, `sub_800158C`,
  `sub_80015A0`, `sub_80015B0`, `sub_80015C0`, `sub_80015D0`,
  `sub_80015E0`, `sub_80015F0`, `sub_8001604`, `sub_8001614`): the
  fade/screen-mode utility cluster documented in `docs/rom_map.md`,
  split around two parked functions - see `docs/matching.md`.

- `src/graphics/aabb_util.c` (new file): `sub_8001640`, `sub_8001688`,
  `sub_80016D0`, `sub_80016DC` - two AABB overlap tests (one already
  referenced by name from `actor.md`'s `actor_part15.c`) plus
  `mem_free`/`mem_alloc` wrappers.

- `src/graphics/intro_screen.c` (new file, replacing `asm/code_3_1.s` -
  boot-adjacent but not part of `src/system/boot_util.c` since
  `main.c`/`memory.c`/`irq.c` sit between them in ROM order):
  `sub_80007EC` - BG2 affine setup for a full-screen intro image; see
  `docs/matching.md` for the statement-ordering gotchas.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_80014A4`** (`asm/code_3_1_7.s`, C in
  `src/graphics/fade_screen_mode.c`) - the fade-to-black palette DMA
  loop. This compiler's loop-invariant hoisting either caches nothing
  extra or, once any local variable represents the blended-buffer
  address, hoists it *and* at least one of the other two DMA fields -
  never the ROM's exact "cache only the buffer" split - see
  `docs/matching.md`, "The `0x080014A4`-`0x08001624` fade/screen-mode
  cluster, finally matched".
- **`sub_8001524`** (`asm/code_3_1_8.s`, C in
  `src/graphics/fade_screen_mode.c`) - sets a packed shadow byte's low
  3 bits. This compiler always folds the ROM's fresh `movs r1,#8;
  rsbs r1,r1,#0` mask computation into a `sub` derived from the
  already-loaded `7` mask - a value-propagation optimization no
  respelling or barrier defeated. Same doc section as above.
- **`sub_8001624`** (`asm/code_3_1_9.s`, C in
  `src/graphics/fade_screen_mode.c`) - commits a blend-register
  shadow. This compiler always fuses the ROM's separate
  store-then-pointer-increment into one `stmia` writeback instruction.
  Same doc section as above.
- **`sub_8006600`** (`src/graphics/oam_count.c`) - HUD-icon-plus-number
  renderer. Prologue/epilogue and most register choices now match the ROM;
  four register-letter mismatches remain in the second half, resistant to
  every technique tried so far - see `docs/matching.md`, "Parked, not
  matched: `sub_8006600`" for the full account. (Several `overlay_ui`/
  `actor` functions elsewhere hit this same class of gap - see
  [overlay_ui.md](./overlay_ui.md) and [actor.md](./actor.md).)
- **`sub_8000EE4`** (`src/graphics/text_layout.c`) - word-wrap text
  renderer. Matches the ROM instruction-for-instruction except ~8 bytes
  from two small codegen details (incoming-argument spill ordering, and a
  couple of loop-bound comparisons compiling one instruction shorter than
  the ROM's) - see `docs/matching.md`, "Parked, not matched: `sub_8000EE4`".
- **`sub_80073DC`** (`src/graphics/graphics.c`) - builds and queues one
  OAM entry per visible sub-piece of an animated part, plus a combined
  VRAM tile upload. Logic/instruction shape confirmed correct (every
  AND/OR/shift constant and branch condition matches the ROM), but the
  ROM spills more locals to its stack frame than gcc does here, causing
  register-letter differences through most of the per-piece loop - see
  `docs/matching.md`, "Parked, not matched: `sub_80073DC`".

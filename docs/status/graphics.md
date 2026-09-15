# Status: graphics

`src/graphics/` - OAM/sprite rendering, screen fades, palette blending,
per-actor animation frames, text layout.

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
  `sub_800722C`, `sub_8007230`
- `src/graphics/oam_count.c`: `sub_8006700`, `sub_8006714`, `sub_8006770`,
  `sub_80067A4`, `sub_80067B4`, `sub_80067C4`, `sub_80067D4`, `sub_80067E4`,
  `sub_80067EC`, `sub_8006820`, `sub_8006864`, `sub_80068A8`, `sub_80068CC`,
  `sub_8006920`, `sub_800695C`
- `src/graphics/fade_util.c`: `sub_80012AC`, `sub_800132C`
- `src/graphics/palette_blend.c`: `sub_80013FC`
- `src/graphics/actor_anim.c`: `GetAnimFrameBaseOffset`

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8006600`** (`src/graphics/oam_count.c`) - HUD-icon-plus-number
  renderer. Prologue/epilogue and most register choices now match the ROM;
  four register-letter mismatches remain in the second half, resistant to
  every technique tried so far - see `docs/matching.md`, "Parked, not
  matched: `sub_8006600`" for the full account.
- **`sub_8000EE4`** (`src/graphics/text_layout.c`) - word-wrap text
  renderer. Matches the ROM instruction-for-instruction except ~8 bytes
  from two small codegen details (incoming-argument spill ordering, and a
  couple of loop-bound comparisons compiling one instruction shorter than
  the ROM's) - see `docs/matching.md`, "Parked, not matched: `sub_8000EE4`".

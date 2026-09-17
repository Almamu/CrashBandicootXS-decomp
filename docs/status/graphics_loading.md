# Status: graphics_loading

Asset/graphics-package loading and the "trigger effect type N" dispatch
family. Filed under `src/graphics/` on disk, tracked as its own
`graphics_loading` category since `docs/rom_map.md` and the
`decomp-chunk` issue generator both treat it as a distinct system from
"core" graphics.

## Matched

None yet.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8020E84`**, **`sub_8020F7C`**, **`sub_802107C`**,
  **`sub_802117C`** (real bytes in `asm/code_3_2_17_14674.s` under a
  `.if NON_MATCHING == 0` guard, C in
  `src/graphics/trigger_effect.c`) - the "trigger effect type N" twin
  family (4 of the 15-slot `gStaticData_0816C7D8` dispatch table's
  slots): sound-only-or-full-spawn effect triggers gated by a
  `gUnknown_030012C0+2` flag bit. The spawn-branch tail is
  instruction-for-instruction identical to the ROM; parked on two
  register-allocation gaps (the four parameters' register rotation,
  and the entry bit-test/`sub_8023278` call's register choice) - see
  `docs/matching.md`, "`graphics_loading` chunk `0x0801FA3C`-
  `0x08021668` (issue #31)", for what was tried.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

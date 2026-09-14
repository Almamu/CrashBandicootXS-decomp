# Status: system

`src/system/` - startup, memory allocator, interrupts, input polling,
tagged-asset loading.

## Matched

- `src/system/main.c`: `AgbMain`
- `src/system/memory.c`: `mem_heap_init`, `mem_collect`, `mem_free_bytes`,
  `mem_alloc`, `mem_free`, `sub_8000518`, `sub_800039C` (unreachable -
  see `docs/decomp_dev.md` for what it is and why it's kept)
- `src/system/irq.c`: `IrqDisable`, `IrqSetup`, `IrqEmptyHandler`,
  `sub_8000620`, `sub_8000654`, `sub_8000670`, `sub_8000680` (2025,
  original `code_1.s`/`code_2.s` lineage), plus `sub_80006A8`,
  `sub_80006EC`, `sub_80006F8`, `sub_8000720`, `sub_8000760`,
  `sub_80007AC`, `sub_80007DC` (Sept 2026, `code_3.s` lineage - `irq.c` is
  a mixed file, see `docs/decomp_dev.md`)
- `src/system/asset_util.c`: `LoadTaggedAsset`, `LoadBackgroundTileAndPalette`

`main.c`/`memory.c`/most of `irq.c` were matched earliest of all, before
`docs/matching.md`'s per-function log convention existed, so they don't have
per-function writeups there the way everything since does. They do have a
frozen decomp.dev baseline now (`expected/legacy.s`) - see
[docs/decomp_dev.md](../decomp_dev.md).

See [docs/workflow.md](../workflow.md) for the per-function loop.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_80010E0`** (`src/system/input_util.c`, an input-polling helper) -
  a single bit-test compiles with the branch senses swapped from the ROM
  (same two instructions, same size) in a way that resists every C-level
  rephrasing tried - see `docs/matching.md`, "Parked, not matched:
  `sub_80010E0`".

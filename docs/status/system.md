# Status: system

`src/system/` - startup, memory allocator, interrupts, input polling,
tagged-asset loading.

## Matched

- `src/system/main.c`: `AgbMain`
- `src/system/memory.c`: `mem_heap_init`, `mem_collect`, `mem_free_bytes`,
  `mem_alloc`, `mem_free`, `sub_8000518`
- `src/system/irq.c`: `sub_80006A8`, `sub_80006EC`, `sub_80006F8`,
  `sub_8000720`, `sub_8000760`, `sub_80007AC`, `sub_80007DC`
- `src/system/asset_util.c`: `LoadTaggedAsset`, `sub_80011C0`

`main.c`/`memory.c` were matched earliest of all, before `docs/matching.md`'s
per-function log convention (or `expected/code_3.s`'s frozen baseline -
see [docs/decomp_dev.md](../decomp_dev.md)) existed, so they don't have
per-function writeups there the way everything since does.

See [docs/workflow.md](../workflow.md) for the per-function loop.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_80010E0`** (`src/system/input_util.c`, an input-polling helper) -
  a single bit-test compiles with the branch senses swapped from the ROM
  (same two instructions, same size) in a way that resists every C-level
  rephrasing tried - see `docs/matching.md`, "Parked, not matched:
  `sub_80010E0`".

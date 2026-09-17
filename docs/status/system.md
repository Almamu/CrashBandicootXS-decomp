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
- `src/system/boot_util.c`: `sub_8000140`, `sub_800014C`, `nullsub_9` -
  boot-adjacent BIOS wrappers right after `asm/crt0.s`'s permanent boot
  stub (`start`, left as hand-written asm, not tracked as a function to
  match); see `docs/matching.md`
- `src/system/game_loop2.c`: `sub_8022FEC`, `sub_802306C`, `sub_8023104`,
  `sub_8023110`, `sub_8023118`, `sub_8023120`, `sub_8023128`,
  `sub_8023130`, `sub_8023138`, `sub_8023140`, `sub_802314C`,
  `sub_8023158`, `sub_8023168`, `sub_8023184`, `sub_8023190`,
  `sub_802319C`, `sub_80231A8`, `sub_80231B4`, `sub_80231BC`,
  `sub_80231C4` (GitHub issue #34, `UpdateGameFrame`-`MainLoop` cluster -
  a `self+0x80`/`0x84`/`0x88`/`0xac`/`0xc0`/`+2`-flags accessor family
  plus the two frame-counter/limit tick functions)
- `src/system/timer_util.c`: `sub_803A944`, `sub_803A948`, `sub_803A94C`,
  `LZ77UnCompWrapper`, `sub_803A954`, `RLUnCompWrapper`, `sub_803A95C`,
  `sub_0803A960` (eight BIOS SWI wrappers), `sub_803A968` (picks a
  12-byte `EepromConfig` table by chip-size code), `sub_803A9D0`
  (claims a hardware timer, hands back an IRQ-handler-stub address) -
  GitHub issue #69
- `src/system/reg_trampolines.c`: `sub_803AD78`, `sub_803AD7C`,
  `sub_803AD80`, `sub_803AD84`, `sub_803AD88`, `sub_803AD8C`,
  `sub_803AD90`, `sub_803AD94` (the `bx r0`..`sp` "call through whatever
  register" trampoline table, already referenced by name from `irq.c`'s
  `sub_8000720` and several `actor_part*` files), `nullsub_43` (bonus,
  just past issue #69's listed range) - GitHub issue #69

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
- **`sub_8022BF0`/`sub_8022CA0`** (`src/system/game_loop.c`, GitHub
  issue #34) - level-start progress-total updater and its cached-state/
  snapshot helper; real bytes in `asm/code_3_2_17_22bf0.s`. See
  `docs/matching.md`'s issue #34 entry for the exact register-allocation
  gaps.
- **`sub_8022EA8`/`sub_8022F2C`** (`src/system/game_loop2.c`, GitHub
  issue #34) - record 47's periodic-trigger setter/decrementer; real
  bytes in `asm/code_3_2_17_22ea8.s`. See `docs/matching.md`'s issue
  #34 entry for the exact register-allocation gaps.
- **`sub_803AA08`/`sub_803AA90`/`sub_803AAD4`** (`src/system/timer_util.c`,
  GitHub issue #69) - timer arm/disarm pair plus a DMA3 block-transfer
  helper; real bytes in `asm/code_3_2_20e_aa08.s`. Every field/register
  access confirmed, parked purely on register-allocation/loop-shape
  gaps - see `docs/matching.md`'s issue #69 entry.

## Still raw, category-mapped (GitHub issue #69)

- **`sub_803AB54`/`sub_803AC04`/`sub_803ACE0`/`sub_803AD38`**
  (`asm/code_3_2_20e_ab54.s`, ROM `0x0803AB54`) - a DMA3 bit-serial
  EEPROM read/write/retry cluster (working theory, not confirmed enough
  to commit even a parked reconstruction) - see `docs/matching.md`.

## Still raw, category-mapped (GitHub issue #34)

- **`UpdateGameFrame`** (`asm/code_3_2_17_225a0.s`, ROM `0x080225A0`) -
  the main per-frame game-loop driver, a ~730-instruction jump-table
  state machine. Not understood branch-by-branch with the precision a
  byte-exact reconstruction needs yet - see `docs/matching.md`.
- **`sub_8022D50`** (`asm/code_3_2_17_22d50.s`, ROM `0x08022D50`) - a
  level-start/reset routine with several still-uncharacterized callees
  - see `docs/matching.md`.

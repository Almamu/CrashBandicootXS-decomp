# Status: system

`src/system/` (core startup/init only) - memory allocator, interrupts,
input polling, tagged-asset loading, BIOS wrappers. The top-level game
loop also lives under `src/system/` on disk but is tracked in its own
category page - see [game_loop.md](./game_loop.md).

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
- `src/system/link_cable.c`/`link_cable2.c` (new files - the GBA
  multiplayer link-cable/SIO transport, `0x08001C80`-`0x08002868`,
  interleaved with `audio`/`overlay_ui` in this same address range -
  see `docs/rom_map.md`'s SIO/link-cable section): `sub_8001D30`
  (link-session "stop"), `sub_80026E4` (link-session "start"),
  `sub_800276C` (RCNT/SIOCNT reset helper), `sub_8002798` (reset
  convenience wrapper), `sub_80027B0` (reset + conditional teardown),
  `sub_80027E8` (session object constructor), `sub_8002830`/
  `sub_8002848` (Serial/Timer3 IRQ handlers) - matched, GitHub
  issue #4, see `docs/matching/issue-4-sio-settings-sync.md`

GitHub issue #70 (`0x0803ADB4`-`0x0803B060`, right after
`reg_trampolines.c` above) was categorized `system` by the chunk
generator, but every function in it turned out to be either a generic
math primitive or an AABB/actor-table helper - both matched and parked
functions from it live in `docs/status/util.md`
(`src/util/math_div_util.c`) and [actor.md](./actor.md)
(`src/graphics/actor_aabb_setup.c`) instead. See `docs/matching.md`'s
issue #70 entry for the full writeup.

`main.c`/`memory.c`/most of `irq.c` were matched earliest of all, before
`docs/matching.md`'s per-function log convention existed, so they don't have
per-function writeups there the way everything since does. They do have a
frozen decomp.dev baseline now (`expected/legacy.s`) - see
[docs/decomp_dev.md](../decomp_dev.md).

See [docs/workflow.md](../workflow.md) for the per-function loop.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8001CB8`** (`asm/code_3_1_10_3_1cb8.s`, C in
  `src/system/link_cable.c`) - the per-player CRC-16-style handshake-id
  hash helper called by `sub_8001DB4`. Semantics fully understood; the
  per-byte table-index computation resists matching the ROM's plain
  8-bit shift - see `docs/matching/issue-4-sio-settings-sync.md`,
  issue #4.
- **`sub_8001DB4`** (`asm/code_3_1_10_3_1db4.s`, same C file) - the
  link-session reset/init, a 400 B 4-player-loop function. Semantics
  fully understood, register/stack-plan gap - see
  `docs/matching/issue-4-sio-settings-sync.md`, issue #4.
- **`sub_8002868`/`sub_8002938`** (`asm/code_3_1_10_3_2868.s`, C in
  `src/graphics/settings_menu8d.c`) - EEPROM load/save block-loop pair
  for the settings record, built on this file's own `EepromConfig`
  primitives below. Semantics fully understood, an IME-save/restore
  register-hop gap - see `docs/matching/issue-4-sio-settings-sync.md`,
  issue #4.
- **`sub_80010E0`** (`src/system/input_util.c`, an input-polling helper) -
  a single bit-test compiles with the branch senses swapped from the ROM
  (same two instructions, same size) in a way that resists every C-level
  rephrasing tried - see `docs/matching.md`, "Parked, not matched:
  `sub_80010E0`".
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

## Still raw, category-mapped (GitHub issue #4)

- **`sub_8001F50`** (452 B, `asm/code_3_1_10_3.s`) - the link-
  connection/handshake driver; `docs/rom_map.md` already characterizes
  its broad shape, not read to full per-branch confidence in the time
  available. Left untouched rather than parked or force-matched - see
  `docs/matching/issue-4-sio-settings-sync.md`.
- **`sub_8002114`** (1488 B, same file) - the per-frame SIO
  data-exchange pump, extremely register-heavy with deep nested
  branching. Left untouched for the same reason - see
  `docs/matching/issue-4-sio-settings-sync.md`.

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
- `src/system/input_util.c`: `sub_80010E0` (input-poll-until-button/
  timeout helper, NAKED - a full C reconstruction matched the ROM
  everywhere except one bit-test whose branch senses compiled swapped
  from the ROM; converted to a byte-verified NAKED asm transcription)
- `src/system/boot_util.c`: `sub_8000140`, `sub_800014C`, `nullsub_9` -
  boot-adjacent BIOS wrappers right after `asm/crt0.s`'s permanent boot
  stub (`start`, left as hand-written asm, not tracked as a function to
  match); see `docs/matching.md`
- `src/system/timer_util.c`: `sub_803A944`, `sub_803A948`, `sub_803A94C`,
  `LZ77UnCompWrapper`, `sub_803A954`, `RLUnCompWrapper`, `sub_803A95C`,
  `sub_0803A960` (eight BIOS SWI wrappers), `sub_803A968` (picks a
  12-byte `EepromConfig` table by chip-size code), `sub_803A9D0`
  (claims a hardware timer, hands back an IRQ-handler-stub address),
  `sub_803AA08` (arms the claimed timer; matched via NAKED
  transcription) - GitHub issue #69, see
  `docs/matching/issue-69-eeprom-timer.md`
- `src/system/timer_util_aa90.c` (own file - its real ROM address,
  `0x0803AA90`, sits between `sub_803AA08` and `sub_803AAD4`, so it
  isn't adjacent to `timer_util.c`'s own matched functions):
  `sub_803AA90` (disarms the timer `sub_803AA08` claims), `sub_803AAD4`
  (the DMA3 block-transfer helper used by the whole EEPROM cluster;
  matched via NAKED transcription) - GitHub issue #69, see
  `docs/matching/issue-69-eeprom-timer.md`
- `src/system/eeprom_util.c`: `sub_803AB54` (reads one 8-byte EEPROM
  block), `sub_803AC04` (writes one block, then arms a watchdog timer
  and busy-waits for completion) - the DMA3 bit-serial EEPROM read/
  write pair, matched via NAKED transcription - GitHub issue #69, see
  `docs/matching/issue-69-eeprom-timer.md`
- `src/system/eeprom_verify.c` (own file, same reason - ROM
  `0x0803ACE0`, between `sub_803AC04` and `reg_trampolines.c`'s
  functions): `sub_803ACE0` (reads an EEPROM block back and compares
  it), `sub_803AD38` (write+verify with a 3-attempt retry) - GitHub
  issue #69, see `docs/matching/issue-69-eeprom-timer.md`
- `src/system/reg_trampolines.c`: `sub_803AD78`, `sub_803AD7C`,
  `sub_803AD80`, `sub_803AD84`, `sub_803AD88`, `sub_803AD8C`,
  `sub_803AD90`, `sub_803AD94` (the `bx r0`..`sp` "call through whatever
  register" trampoline table, already referenced by name from `irq.c`'s
  `sub_8000720` and several `actor_part*` files), `nullsub_43` (bonus,
  just past issue #69's listed range) - GitHub issue #69
- `src/system/link_cable.c`/`link_cable2.c` (new files - the GBA
  multiplayer link-cable/SIO transport, `0x08001C80`-`0x08002868`,
  interleaved with `audio`/`overlay_ui` in this same address range -
  see `docs/rom_map.md`'s SIO/link-cable section): `sub_8001CB8`
  (per-player CRC-16-style handshake-id hash helper, NAKED),
  `sub_8001D30` (link-session "stop"), `sub_8001DB4` (link-session
  reset/init, NAKED), `sub_8001F50` (link-connection/handshake driver,
  NAKED), `sub_8002114` (1488 B per-frame SIO data-exchange pump,
  NAKED, this file's biggest function), `sub_80026E4` (link-session
  "start"), `sub_800276C` (RCNT/SIOCNT reset helper), `sub_8002798`
  (reset convenience wrapper), `sub_80027B0` (reset + conditional
  teardown), `sub_80027E8` (session object constructor), `sub_8002830`/
  `sub_8002848` (Serial/Timer3 IRQ handlers) - all matched, GitHub
  issue #4, see `docs/matching/issue-4-sio-settings-sync.md`
- `src/graphics/settings_menu8d.c`'s `sub_8002868`/`sub_8002938`
  (EEPROM load/save block-loop pair for the settings record, built on
  this file's own `EepromConfig` primitives below, NAKED) - matched,
  GitHub issue #4, see `docs/matching/issue-4-sio-settings-sync.md`

GitHub issue #70 (`0x0803ADB4`-`0x0803B060`, right after
`reg_trampolines.c` above) was categorized `system` by the chunk
generator, but every function in it turned out to be either a generic
math primitive or an AABB/actor-table helper - the matched functions
from it live in `docs/status/util.md` (`src/util/math_div_util.c`) and
[actor.md](./actor.md) (`src/graphics/actor_aabb_setup.c`) instead. See
`docs/matching.md`'s issue #70 entry for the original writeup and
`docs/matching/issue-69-eeprom-timer.md`'s "NAKED transcription pass"
section for how the division/modulo trio finished matching.

`main.c`/`memory.c`/most of `irq.c` were matched earliest of all, before
`docs/matching.md`'s per-function log convention existed, so they don't have
per-function writeups there the way everything since does. They do have a
frozen decomp.dev baseline now (`expected/legacy.s`) - see
[docs/decomp_dev.md](../decomp_dev.md).

See [docs/workflow.md](../workflow.md) for the per-function loop.


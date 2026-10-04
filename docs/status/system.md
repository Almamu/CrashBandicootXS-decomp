# Status: system

`src/system/` (core startup/init only) - memory allocator, interrupts,
input polling, tagged-asset loading, BIOS wrappers. The top-level game
loop also lives under `src/system/` on disk but is tracked in its own
category page - see [game_loop.md](./game_loop.md).

## Matched

- `src/system/main.c`: `AgbMain`
- `src/system/memory.c`: `mem_heap_init`, `mem_collect`, `mem_free_bytes`,
  `mem_alloc`, `mem_free`, `mem_heap_shutdown`, `sub_800039C` (unreachable -
  see `docs/decomp_dev.md` for what it is and why it's kept)
- `src/system/irq.c`: `IrqDisable`, `IrqSetup`, `IrqEmptyHandler`,
  `EnableVBlankHandler`, `DisableVBlankHandler`, `RemoveVBlankCallback`, `AddVBlankCallback` (2025,
  original `code_1.s`/`code_2.s` lineage), plus `WaitForVBlank`,
  `DisableFrameLimit`, `SetFrameLimit`, `VBlankHandler`, `GetDpadDirection`,
  `UpdateKeys`, `ClearKeys` (Sept 2026, `code_3.s` lineage - `irq.c` is
  a mixed file, see `docs/decomp_dev.md`)
- The IWRAM image (`0x03000000`, stored at ROM `0x087E55E4`):
  `asm/intr_main.s` (`IntrMain`, hand-written), `src/iwram/string_arm.c`
  and `src/iwram/sprite_arm.c` (ARM C, agbcc_arm: seven matched -
  `strncpy_arm` in the second pass - and three parked: `itoa_arm`,
  `HeapSortActorsByKey`, `LookupSpriteFrameCache`) and `src/iwram/iwram_data.c` (the
  initialised IWRAM globals) - see
  [iwram-image.md](../matching/iwram-image.md).
- `src/system/asset_util.c`: `LoadTaggedAsset`, `LoadBackgroundTileAndPalette`
- `src/system/input_util.c`: `WaitForKeyPress` (input-poll-until-button/
  timeout helper) - was previously NAKED, now matched as real C by
  writing the count-limited loop's cancel-check block textually before
  the poll/confirm-check code (matching the ROM's own basic-block
  layout) instead of the natural top-to-bottom order - see
  [naked-sub_80010e0-matched.md](../matching/naked-sub_80010e0-matched.md).
- `src/system/boot_util.c`: `DivMod`, `MemCopy32`, `nullsub_9` -
  boot-adjacent BIOS wrappers right after `asm/crt0.s`'s permanent boot
  stub (`start`, left as hand-written asm, not tracked as a function to
  match); see `docs/matching.md`
- `src/system/timer_util.c`: `BgAffineSet`, `CpuFastSet`, `CpuSet`,
  `LZ77UnCompVram`, `ObjAffineSet`, `RLUnCompVram`, `Sqrt`,
  `VBlankIntrWait` (eight BIOS SWI wrappers - NAKED but genuinely
  un-improvable trampolines with no real C logic to express, kept
  matched per `docs/matching.md`'s frozen convention), then the start of
  Nintendo's AgbEeprom SDK library ("EEPROM_V122"), built with -O1 as
  plain C with no pins: `EEPROMConfigure` (picks the
  12-byte `EepromConfig` by chip-size code), `EepromTimerIntr` (the
  timer IRQ handler - formerly a raw `.byte` blob),
  `SetEepromTimerIntr` (claims a hardware timer and
  hands back the handler), `StartEepromTimer` -
  GitHub issue #69, see `docs/matching/eeprom-sdk-o1.md`.
- `src/system/timer_util_aa90.c` (own file - its real ROM address,
  `0x0803AA90`, isn't adjacent to `timer_util.c`'s functions; built
  with -O1): `StopEepromTimer` (disarms the timer
  `StartEepromTimer` claims) and `DMA3Transfer` (the DMA3
  block-transfer helper used by the whole EEPROM cluster) - GitHub issue
  #69, see `docs/matching/eeprom-sdk-o1.md`.
- `src/system/eeprom_util.c` (own file, built with -O1):
  `EEPROMRead`/`EEPROMWrite` (the DMA3
  bit-serial EEPROM read/write pair) - GitHub issue #69, see
  `docs/matching/eeprom-sdk-o1.md`.
- `src/system/eeprom_verify.c` (own file, same reason - ROM
  `0x0803ACE0`, between `EEPROMWrite` and `reg_trampolines.c`'s
  functions; built with -O1): `EEPROMCompare` (reads
  an EEPROM block back and compares it), `EEPROMWrite1_check`
  (write+verify with a 3-attempt retry) - GitHub
  issue #69, see `docs/matching/eeprom-sdk-o1.md`.
- `src/system/reg_trampolines.c`: `_call_via_r0`, `_call_via_r1`,
  `_call_via_r2`, `_call_via_r3`, `_call_via_r4`, `_call_via_r5`,
  `_call_via_r6`, `_call_via_r7` (the `bx r0`..`sp` "call through whatever
  register" trampoline table, already referenced by name from `irq.c`'s
  `VBlankHandler` and several `actor_part*` files), `_call_via_lr` (bonus,
  just past issue #69's listed range) - GitHub issue #69
- `src/system/link_cable.c`/`link_cable2.c` (new files - the GBA
  multiplayer link-cable/SIO transport, `0x08001C80`-`0x08002868`,
  interleaved with `audio`/`overlay_ui` in this same address range -
  see `docs/rom_map.md`'s SIO/link-cable section): `LinkStop`
  (link-session "stop"), `LinkStart` (link-session "start"),
  `sub_800276C` (RCNT/SIOCNT reset helper), `sub_8002798`
  (reset convenience wrapper), `sub_80027B0` (reset + conditional
  teardown), `sub_80027E8` (session object constructor), `LinkSerialIntr`/
  `LinkTimer3Intr` (Serial/Timer3 IRQ handlers), `ReadSaveData`/`WriteSaveData`
  (`src/graphics/settings_menu8d.c`, EEPROM load/save block-loop pair
  for the settings record - the previously-suspected register-pressure
  gap in their shared IME-save/IE-clear/IME-restore snippet didn't
  reproduce with the actual field/loop structure; plain C matches
  byte-for-byte) - all
  matched, GitHub
  issue #4, see `docs/matching/issue-4-sio-settings-sync.md`. (`sub_8001DB4`,
  the link-session reset/init, is real C in its own
  `link_cable_01db4.c` since the last-eleven NAKED retry - see
  [last-eleven-naked-retry.md](../matching/last-eleven-naked-retry.md); `sub_8002114`, the
  per-frame SIO pump, is real C since the last-seven NAKED retry - see
  [last-seven-naked-retry.md](../matching/last-seven-naked-retry.md). `sub_8001F50`, the
  link handshake driver, is real C since the second near-miss sweep -
  see [near-miss-polish-2.md](../matching/near-miss-polish-2.md) - and
  `sub_8001CB8`, the per-player CRC-16-style handshake-id hash helper,
  since the early-ROM NAKED retry 2 - see
  [early-rom-naked-retry-2.md](../matching/early-rom-naked-retry-2.md).)

GitHub issue #70 (`0x0803ADB4`-`0x0803B060`, right after
`reg_trampolines.c` above) was categorized `system` by the chunk
generator, but every function in it turned out to be either a generic
math primitive or an AABB/actor-table helper - the matched functions
from it live in `docs/status/util.md` (`src/util/math_div_util.c`) and
[actor.md](./actor.md) (`src/graphics/actor_aabb_setup.c`) instead. See
`docs/matching.md`'s issue #70 entry for the original writeup and
`docs/matching/issue-69-eeprom-timer.md`'s "NAKED transcription pass"
section for how the division/modulo trio's NAKED transcription pass
went (now tracked as parked, not matched - see below).

`main.c`/`memory.c`/most of `irq.c` were matched earliest of all, before
`docs/matching.md`'s per-function log convention existed, so they don't have
per-function writeups there the way everything since does. They do have a
frozen decomp.dev baseline now (`expected/legacy.s`) - see
[docs/decomp_dev.md](../decomp_dev.md).

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

(none left - the last three, the EEPROM trio, became real C in the
EEPROM SDK -O1 pass, see `docs/matching/eeprom-sdk-o1.md`.)

# Status: system

`src/system/` (core startup/init only) - memory allocator, interrupts,
input polling, tagged-asset loading, BIOS wrappers. The top-level game
loop also lives under `src/system/` on disk but is tracked in its own
category page - see [game_loop.md](./game_loop.md).

## Matched

- `src/system/main.cpp`: `AgbMain`
- `src/system/memory.cpp`: `mem_heap_init`, `mem_collect`, `mem_free_bytes`,
  `mem_alloc`, `mem_free`, `mem_heap_shutdown`, `mem_walk_heaps` (unreachable, plain C
  since #662 round 2; see `docs/decomp_dev.md`)
- `src/system/irq.cpp`: `IrqDisable`, `IrqSetup`, `IrqEmptyHandler`,
  `EnableVBlankHandler`, `DisableVBlankHandler`, `RemoveVBlankCallback`, `AddVBlankCallback` (2025,
  original `code_1.s`/`code_2.s` lineage), plus `WaitForVBlank`,
  `DisableFrameLimit`, `SetFrameLimit`, `VBlankHandler` (Sept 2026,
  `code_3.s` lineage - `irq.c` is a mixed file, see `docs/decomp_dev.md`)
- `src/system/key_input.cpp`: `GetDpadDirection`, `UpdateKeys`, `ClearKeys`
  (Sept 2026, `code_3.s` lineage; split from `irq.cpp`, #767)
- The IWRAM image (`0x03000000`, stored at ROM `0x087E55E4`):
  `asm/intr_main.s` (`IntrMain`, hand-written), `src/iwram/string_arm.cpp`
  and `src/iwram/sprite_arm.cpp` (ARM C, all ten matched - `strncpy_arm`
  in the second pass, `HeapSortActorsByKey` in the fourth, `itoa_arm`
  and `LookupSpriteFrameCache` in the seventh with agbcc_arm_patched) and `src/iwram/iwram_data.cpp` (the
  initialised IWRAM globals) - see
  [iwram-image.md](../matching/iwram-image.md).
- `src/system/asset.cpp`: `LoadTaggedAsset`, `LoadBackgroundTileAndPalette`
- `src/system/input.cpp`: `WaitForKeyPress` (input-poll-until-button/
  timeout helper) - was previously NAKED, now matched as real C by
  writing the count-limited loop's cancel-check block textually before
  the poll/confirm-check code (matching the ROM's own basic-block
  layout) instead of the natural top-to-bottom order - see
  [naked-sub_80010e0-matched.md](../matching/archive/naked-sub_80010e0-matched.md).
- `src/system/bios_util.cpp`: `DivMod`, `MemCopy32`, `UpdateCtrl` -
  boot-adjacent BIOS wrappers right after `asm/crt0.s`'s permanent boot
  stub (`start`, left as hand-written asm, not tracked as a function to
  match); see `docs/matching.md`
- `lib/libagbsyscall/libagbsyscall.s`: `BgAffineSet`, `CpuFastSet`, `CpuSet`,
  `LZ77UnCompVram`, `ObjAffineSet`, `RLUnCompVram`, `Sqrt`,
  `VBlankIntrWait` (eight BIOS SWI wrappers, hand-written asm, excluded
  from progress - see `docs/libraries.md`).
- `lib/agb_eeprom/src/eeprom_timer.c`: the start of
  Nintendo's AgbEeprom SDK library ("EEPROM_V122"), built with -O1 as
  plain C with no pins: `EEPROMConfigure` (picks the
  12-byte `EepromConfig` by chip-size code), `EepromTimerIntr` (the
  timer IRQ handler - formerly a raw `.byte` blob),
  `SetEepromTimerIntr` (claims a hardware timer and
  hands back the handler), `StartEepromTimer` -
  GitHub issue #69, see `docs/matching/eeprom-sdk-o1.md`.
- `lib/agb_eeprom/src/eeprom_timer_stop.c` (own file - its real ROM address,
  `0x0803AA90`, isn't adjacent to `eeprom_timer.c`'s functions; built
  with -O1): `StopEepromTimer` (disarms the timer
  `StartEepromTimer` claims) and `DMA3Transfer` (the DMA3
  block-transfer helper used by the whole EEPROM cluster) - GitHub issue
  #69, see `docs/matching/eeprom-sdk-o1.md`.
- `lib/agb_eeprom/src/eeprom_read_write.c` (own file, built with -O1):
  `EEPROMRead`/`EEPROMWrite` (the DMA3
  bit-serial EEPROM read/write pair) - GitHub issue #69, see
  `docs/matching/eeprom-sdk-o1.md`.
- `lib/agb_eeprom/src/eeprom_verify.c` (own file, same reason - ROM
  `0x0803ACE0`, between `EEPROMWrite` and `lib1funcs.s`'s
  functions; built with -O1): `EEPROMCompare` (reads
  an EEPROM block back and compares it), `EEPROMWrite1_check`
  (write+verify with a 3-attempt retry) - GitHub
  issue #69, see `docs/matching/eeprom-sdk-o1.md`.
- `lib/libgcc/lib1funcs.s` (hand-written asm, excluded from progress): `_call_via_r0`, `_call_via_r1`,
  `_call_via_r2`, `_call_via_r3`, `_call_via_r4`, `_call_via_r5`,
  `_call_via_r6`, `_call_via_r7` (the `bx r0`..`sp` "call through whatever
  register" trampoline table, already referenced by name from `irq.c`'s
  `VBlankHandler` and several actor and object files), `_call_via_lr` (bonus,
  just past issue #69's listed range) - GitHub issue #69
- `src/link/link_handshake.cpp`/`link_sio.cpp` (new files - the GBA
  multiplayer link-cable/SIO transport, `0x08001C80`-`0x08002868`,
  interleaved with `audio`/`overlay_ui` in this same address range -
  see `docs/rom_map.md`'s SIO/link-cable section): `LinkStop`
  (link-session "stop"), `LinkStart` (link-session "start"),
  `LinkSetupSio` (RCNT/SIOCNT reset helper), `ResetLinkSession`
  (reset convenience wrapper), `DestroyLinkSession` (reset + conditional
  teardown), `InitLinkSession` (session object constructor), `LinkSerialIntr`/
  `LinkTimer3Intr` (Serial/Timer3 IRQ handlers), `ReadSaveData`/`WriteSaveData`
  (`src/save/save_data.cpp`, EEPROM load/save block-loop pair
  for the settings record - the previously-suspected register-pressure
  gap in their shared IME-save/IE-clear/IME-restore snippet didn't
  reproduce with the actual field/loop structure; plain C matches
  byte-for-byte) - all
  matched, GitHub
  issue #4, see `docs/matching/archive/issue-4-sio-settings-sync.md`. (`ResetLinkSessionState`,
  the link-session reset/init, is real C in its own
  `link_session_reset.cpp` since the last-eleven NAKED retry - see
  [last-eleven-naked-retry.md](../matching/archive/last-eleven-naked-retry.md); `HandleLinkSerial`, the
  per-frame SIO pump, is real C since the last-seven NAKED retry - see
  [last-seven-naked-retry.md](../matching/archive/last-seven-naked-retry.md). `UpdateLinkSession`, the
  link handshake driver, is real C since the second near-miss sweep -
  see [near-miss-polish-2.md](../matching/archive/near-miss-polish-2.md) - and
  `MakeLinkHandshakeId`, the per-player CRC-16-style handshake-id hash helper,
  since the early-ROM NAKED retry 2 - see
  [early-rom-naked-retry-2.md](../matching/archive/early-rom-naked-retry-2.md).)

GitHub issue #70 (`0x0803ADB4`-`0x0803B060`, right after
`lib1funcs.s` above) was categorized `system` by the chunk
generator, but every function in it turned out to be either a generic
math primitive or an AABB/actor-table helper - the matched functions
from it live in `docs/status/util.md` (`lib/libgcc/lib1funcs.s`) and
[actor.md](./actor.md) (`src/util/aabb_setup.c`) instead. See
`docs/matching.md`'s issue #70 entry for the original writeup and
`docs/matching/archive/issue-69-eeprom-timer.md`'s "NAKED transcription pass"
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

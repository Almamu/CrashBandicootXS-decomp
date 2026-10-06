# Issue #31: the text popup spawners under old_agbcc

Issue #31's passes (docs/matching/archive/issue-31-graphics-loading.md) left 24
functions of the "two-line text popup" family NAKED. Almost all of them
were parked on the same gap: the ROM's prologue pushes r7 (often along
with r8/sb/sl), and no plain-C or register-pin rewrite made the current
`agbcc` put r7 in the callee-saved set. The functions that did match
needed heavy register pins and `asm volatile` islands.

Like issue #24's region (docs/matching/archive/old-agbcc-retry.md), this ROM
region was built with `tools/agbcc/bin/old_agbcc`. Its scheduler has the
usual tell here too: `SpawnSquid` loads the `0x7f` mask before the
`ldrb` of `part->flags`. Under old_agbcc the r7 "gap" goes away. The
allocator uses r7 as a byte-load scratch register on its own, and the
prologue comes out as `push {r4-r7,lr}` plus the high registers, exactly
as in the ROM.

## Result

All five files move to old_agbcc whole: `spawn_enemies.c`,
`spawn_bosses.c` and `spawn_objects.c`. Every function
in them already matched under old_agbcc as written (NAKED bodies and asm
islands are literal), so no file needed a split.

33 functions were rewritten as plain C with no register pins, no asm and
no NAKED:

| file | rewritten | was NAKED |
|---|---|---|
| `spawn_enemies.c` | 11 of 12 | 9 |
| `spawn_enemies.c` | 1 of 1 | 0 |
| `spawn_enemies.c` | 12 of 13 | 11 |
| `spawn_bosses.c` | 3 of 4 | 1 |
| `spawn_objects.c` | 6 (the rest were already plain C) | 1 |

Three functions keep their previous form:

- `SpawnVenusFlytrap` keeps its agbcc-era pinned C, which matches under
  old_agbcc. Plain C is 5 halfwords off: three constant loads (1 into r9,
  0 into sl, 1 into r8) come out in a different order. Writing `field_0A`
  through `SetPartField0A` fixes that order but swaps hdr and the
  bitfield constant between registers.
- `SpawnFlamethrowerLabAssistant` stays NAKED. Plain C is 62 halfwords off. The ROM spills
  `part+0x28` to its one stack slot and keeps the constant 1 in r8. The
  reconstruction spills the constant and `&gEntityFlags` instead.
- `SpawnRoomExit` stays NAKED. Plain C is 9 halfwords off. The ROM computes
  the `{x - 2, y - 0x1e}` point into fresh r2/r3, and the reconstruction
  subtracts in place. This is the same gap as `SpawnCrateGemMarker`
  (spawn_pickups.c).

## Shared header

`include/text_popup.h` holds the shared types:

- `struct popup_part`, the CreateMovingSprite part. Its layout matches
  cortex.c's `struct gfx_part`: `flipX` is bit 4 of +0x28,
  `frameNibble` is +0x29 and `hdr` is +0x44.
- `struct enemy_ctrl`, the CreateEnemyCtrl header.
- `struct level_record`, the gEntityFlags record.

It also has the `POPUP_ATTACH`/`LEVEL_RECORD` macros and the inline
setters.

## What mattered under old_agbcc

- **u32 bitfields at +0x28.** With `u8` bitfields, the `1` constant the
  two "collected" bits share is a QImode pseudo. CSE merges it with
  `field_0A = 1`, which stretches its live range, so it loses r6 to hdr.
  Declared on `u32`, it gets its own SImode pseudo and the ROM's
  registers.
- **Value before address.** A store whose value is loaded before its
  address (`hdr->gfx`, `part->tag`, `field_0A`) only comes out that way
  when the value arrives as an inline helper's parameter. This is the
  same effect the issue #23 `AndFlags`/`OrFlags` helpers rely on.
  `AndPartFlags` takes its mask as an `s32` for the same reason: old_agbcc
  then derives the mask from the still-live `field_0A` constant
  (`subs r0, #0x43`).
- **Load-all-then-store-all copies.** The copies of record fields into
  hdr+0x20..0x4c load every value before storing any. That comes from a
  multi-parameter inline (`SetEnemyHitBox`, `SetEnemyAttackCycle`,
  `SetEnemyWave`). Separate statements interleave the loads and stores.
- **A second `rec2` local.** When a function looks the record up twice,
  the second lookup has to go into its own local, computed before the
  next call. Reusing `rec` changes the whole allocation.
- **The flipX toggle** is `{ s32 f = part->flipX; part->flipX = f == 0; }`.
  Written in place as `!part->flipX`, it schedules differently.

## Other agbcc-era gaps worth retrying

The same "r7 never enters the callee-saved set" gap parked other
functions nearby, among them `LoadGraphicsPackage`, `InitBgSetup` and
`FitScaledSprite` (graphics_package.c and neighbours, issue #30). They
are good candidates for the same old_agbcc retry.

## Later pass: NAKED retry (mid45)

`SpawnRoomExit` is now C, using four register pins plus one empty `asm`
nudge for the fresh-register `{x - 2, y - 0x1e}` point. `SpawnFlamethrowerLabAssistant`
stays NAKED at 62 halfwords, now with its draft under `#if NON_MATCHING`.
See [naked-retry-mid45.md](naked-retry-mid45.md).

## Later pass: last-eleven NAKED retry

`SpawnFlamethrowerLabAssistant` is real C now. On top of the last-nine/last-ten draft, an
r3 hold over the post-call gfx store and `rec2` lookup moves the
&gEntityFlags reload to r1, and a copy of `part` escaped by an
empty asm inside the argument puts the `q2` store after the
`adds r1, r7, #0` argument move. The r9 hold is no longer needed. See
[last-eleven-naked-retry.md](last-eleven-naked-retry.md).

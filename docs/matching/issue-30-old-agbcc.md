# Issue #30: the BG loader under old_agbcc

Issue #30's passes (docs/matching/issue-30-graphics-loading.md) left four
functions in `0x0801E578`-`0x0801E8F8` NAKED: `LoadGraphicsPackage`,
`InitBgSetup`, `FitScaledSprite` and `DrawScaledSprite`. All four were parked on
the same gap: the ROM pushes r7, and the current `agbcc` drops it from
the callee-saved set.

Like the text popups right after it (docs/matching/issue-31-old-agbcc.md),
this region was built with `tools/agbcc/bin/old_agbcc` (as #435 also
found for `sub_801EA5C`-`sub_801EE3C` in between). Under it all four
are plain C: no register pins, no inline asm in
a function body and no NAKED.

## Result

Every object in the region moves to `OLD_AGBCC_OBJS` whole. The files
that were already matched (`graphics_package_1e8f8.c`, `_1e964.c` and
`graphics_loading_1e990.c`) match under old_agbcc unchanged.

| function | file | was |
|---|---|---|
| `LoadGraphicsPackage` | `graphics_package_1e578.c` | NAKED |
| `InitBgSetup` | `graphics_package_1e640.c` | NAKED |
| `FitScaledSprite`, `DrawScaledSprite` | `graphics_package_1e688.c` | NAKED |

## Types

- `struct bg_setup` (`include/graphics_package.h`) is the 0x10-byte
  buffer callers build with `InitBgSetup` before `LoadGraphicsPackage`.
  It holds the char block, screen block and palette bank, and a BGnCNT
  bitfield union at +0x0C that `GetBgSetupControl` reads back. The ROM's
  `& 0x3f` in `InitBgSetup` is its `size = 0`.
- `graphics_package_1e688.c` uses a hardware `struct oam_attrs`. In it,
  `x:9` has to be declared on a `u32`, or `DrawScaledSprite`'s stores schedule
  differently.

## What mattered under old_agbcc

- **`InitBgSetup` returns `self`.** Every caller already declared it as
  returning `void *`, and the return is what gives the ROM's
  `pop {r1}; bx r1` epilogue.
- **`LoadGraphicsPackage`'s row loop** takes `u16 *next = dest + 0x20`
  at the top and assigns it at the bottom. That puts the next-row
  pointer in the ROM's stack slot.
- **`FitScaledSprite` divides with plain `/`.** It must not call
  `__divsi3` directly: as a libcall, `__divsi3` doesn't clobber
  memory, so the `+0x11` byte stays in a register across both divides.

## Worth retrying

`LoadTitleScreenBg` (level_graphics.c, issue #65) was parked on the same
dropped-r7 artifact.

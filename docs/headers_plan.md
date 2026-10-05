# Headers plan (#574)

This is the plan for #574: move the `extern` declarations, prototypes and
duplicated local structs out of `.c` files and into headers, without changing
a byte of the ROM.

Matching agents declared what each function needed in the file they were
working in, so most `.c` files open with a block of `extern`s. Many symbols
are declared in dozens of files, and some of those declarations disagree
(`gPlayer` has 16 different types). Headers let the compiler check every
declaration against the definition, and let a file include what it uses
instead of restating it.

The work goes in small batches, one subsystem at a time. This document has
the survey numbers, the rules every batch follows and the batch order. It
also records the codegen surprises found so far: a prototype can change the
generated code, and those cases need to be known before a batch starts.

**Status:** phase 0 done: the audit tool, the apply tool, this plan, and the
`text` subsystem as the pilot batch (`include/text.h`, see "Pilot" below).
Batch 1 (link + hud) done: `include/link.h` and `include/hud.h`, see
"Batch 1" below. Batch 2 (cutscene + pickups + enemies) done:
`include/cutscene.h`, `include/pickups.h` and `include/enemies.h`, see
"Batch 2" below. Batch 3 (save + frontend) done: `include/save.h`,
`include/frontend.h` and `gLinkSession` in `include/link.h`, see
"Batch 3" below.

Audit totals (`tools/extern_audit.py`) as the batches land:

| | Pilot merged | After batch 1 | After batch 2 | After batch 3 |
|---|---:|---:|---:|---:|
| Declarations in `.c` files (symbols defined elsewhere) | 4,572 | 4,513 | 4,436 | 4,279 |
| Unique symbols declared in a `.c` file | 2,317 | 2,284 | 2,214 | 2,112 |
| - conflicting | 231 | 229 | 226 | 222 |
| Local struct/union definitions in `.c` files | 548 | 538 | 522 | 501 |
| Struct names defined in more than one `.c` file | 75 | 74 | 68 | 63 |

## Tools

- **`tools/extern_audit.py`** parses every `.c` file under `src/` and `lib/`
  (plus the headers, for reference). It collects each file-scope `extern`,
  each prototype and each local struct, and groups them by symbol. It writes
  `build/extern_audit/{symbols,structs}.{tsv,json}` (generated, not
  committed) and prints a summary.
  - `--subsystem NAME` lists the conflicting symbols that subsystem owns.
  - `--conflicts` lists every conflicting symbol.
  - `--symbol NAME` prints one symbol's full record: the definition, every
    variant, the files that declare it and its users by subsystem.

  The parser is regex-based and only looks at file scope. It reads both arms
  of `#if NON_MATCHING` and doesn't run the preprocessor. It ignores
  block-scope `extern`s (there are 6). It finds definitions in C files,
  top-level `asm()` blocks, `.s` files, `ldscript.txt`/`sym_*.txt` and the
  generated level/graphics sources under `build/`, so run it after a build.
- **`tools/apply_headers.py HEADER [FILES...]`** applies a header. In each
  file it deletes the local declarations that match the header's (ignoring
  parameter names, `extern` and whitespace) and adds the `#include`. A file
  with a declaration that differs is left alone and listed: including the
  header there would be a "conflicting types" error. Once the header's type
  has been checked in that file, `--adopt SYMBOL` deletes the differing
  declaration too. Files that define one of the header's symbols are only
  converted when named on the command line. The tool prints the trailing
  comments it drops and the comment blocks a deletion may leave orphaned.
  It doesn't build anything.

A quick per-file check before the full build: delete the file's `.o`, run
`make build/crashbandicootxs/src/<dir>/<file>.o` and diff the generated `.s`
against a copy saved from a clean build. Every file in the pilot was checked
this way first. A batch is only done when both full clean checks pass (see
"Verification").

## Survey (origin/main at fc3bdec1, before the pilot)

| | Count |
|---|---:|
| `.c` files scanned (`src/` + `lib/`) | 356 |
| File-scope declarations in `.c` files | 13,043 |
| - forward declarations of a symbol the same file defines (mostly `src/data/` tables pointing at each other) | 8,395 |
| - declarations of a symbol defined elsewhere | **4,648** |
| Unique symbols declared in a `.c` file | **2,346** |
| - every declaration identical (after whitespace) | 2,074 |
| - equivalent (only parameter names differ) | 39 |
| - **conflicting** | **233** |
| Symbols declared in more than one file | 561 (233 of them conflicting) |
| Symbols also declared in a header already | 57 |
| Symbols with no definition found | 1 (`UpdateVvLogoPieces` in title_screen.c: a parser miss, since the definition follows macro invocations without semicolons) |

Conflicts by kind (`extern_audit.py` tags each one by what differs):

| Kind | Symbols | Meaning |
|---|---:|---|
| `pointer` | 109 | only the pointer target differs (`void *` vs `struct foo *` vs `u8 *`) |
| `scalar` | 85 | anything else: `u8` vs `s32`, `void` vs a value return, parameter count, a different struct type for a global |
| `proto` | 39 | an unprototyped `f()` against a prototype. All 540 unprototyped declarations are in `src/data/` tables that only take the function's address, so they are safe to replace |

The most-declared conflicting symbols are the shared globals that already
blocked file merges in #575 (`docs/file_layout_plan.md`, "Kept separate"):

| Symbol | Files | Variants | Kind |
|---|---:|---:|---|
| `gAudioContext` | 78 | 2 | pointer |
| `PlaySfx` | 66 | 3 | scalar |
| `_call_via_r2` | 66 | 5 | scalar |
| `gLevelState` | 60 | 8 | pointer |
| `gPlayer` | 42 | 16 | pointer |
| `_call_via_r1` | 42 | 4 | scalar |
| `OperatorNew` | 35 | 2 | scalar |
| `gPaletteCache` | 34 | 4 | pointer |
| `gLevelLayers` | 30 | 8 | scalar |
| `gKeys` | 29 | 8 | scalar |
| `gOamBuffer` | 28 | 3 | pointer |
| `SetSpriteAnimDone` | 26 | 13 | scalar |
| `gEntityFlags` | 24 | 4 | pointer |
| `gSpriteBankSet` | 23 | 3 | pointer |

Symbols per owner, before the pilot ("owner" is the subsystem of the
defining file, see "Who owns a symbol"):

| Owner | Symbols | Consistent | Conflicting | Declarations |
|---|---:|---:|---:|---:|
| data (`src/data/`, generated) | 557 | 549 | 8 | 656 |
| level | 266 | 225 | 41 | 474 |
| ldscript (`sym_*.txt`) | 215 | 184 | 31 | 895 |
| vehicle | 182 | 175 | 7 | 203 |
| bosses | 127 | 124 | 3 | 146 |
| actor | 125 | 121 | 4 | 245 |
| player | 114 | 101 | 13 | 154 |
| menus | 107 | 100 | 7 | 128 |
| gfx | 95 | 67 | 28 | 351 |
| objects | 91 | 56 | 35 | 306 |
| asm (`data/data.s`) | 80 | 79 | 1 | 82 |
| crates | 47 | 39 | 8 | 59 |
| iwram | 47 | 42 | 5 | 131 |
| lib/gax | 39 | 36 | 3 | 59 |
| save | 38 | 37 | 1 | 45 |
| frontend | 30 | 29 | 1 | 44 |
| enemies | 24 | 23 | 1 | 26 |
| system | 23 | 17 | 6 | 125 |
| pickups | 23 | 20 | 3 | 27 |
| cutscene | 21 | 20 | 1 | 23 |
| util | 18 | 9 | 9 | 70 |
| audio | 18 | 11 | 7 | 111 |
| hud | 16 | 16 | 0 | 26 |
| text | 16 | 14 | 2 | 31 |
| lib/libgcc | 14 | 8 | 6 | 194 |
| link | 10 | 8 | 2 | 14 |
| lib/libagbsyscall | 2 | 2 | 0 | 2 |

**Local structs:** 553 struct/union definitions in `.c` files, 410 names.
75 names are defined in more than one file (218 copies), and 33 names also
have a definition in a header. The audit computes a layout signature (size
and field offsets) for 551 of them. 35 layouts with three or more fields are
shared by more than one definition (168 definitions under 110 different
names): these are copies of one struct under different names. The
most-copied names are `struct aabb` (9 copies), `held_pressed_pair` (8, the
`gKeys` layout), `pool_manager` (8, two layouts), `level_layers` (6, two
layouts), `bg_scroll_layer` (5, three layouts), `cam_ref`, `entry_set`,
`probe_pos` and `tile_cache` (5 each).

## Target header set

**One header per subsystem, named after its `src/` directory:**
`include/<subsystem>.h`. It declares the functions the subsystem's files
define, with their real prototypes copied from the definitions, plus the
globals and data that belong to the subsystem (see below). A `.c` file
includes the subsystem headers of whatever it calls.

The existing headers keep their names. Most of the 37 are **type headers**:
they define the structs of one object (`actor_self.h`, `box_part.h`,
`bitmap_font.h`, `link_session.h`, `vtable.h`, ...) and have few or no
prototypes. They stay as they are, and a subsystem header includes the type
headers its prototypes need. Four existing headers already carry a
subsystem's name: `actor.h`, `audio.h`, `hud.h` and `cutscene.h`. They become
those subsystems' headers, extended in place. A struct used by several
subsystems gets its own small type header rather than living in one
subsystem's header. The pilot added `include/aabb.h` this way, moving
`struct aabb` out of `gobj_1a794.h`.

The full list:

| Subsystem | Header | Notes |
|---|---|---|
| actor | `actor.h` (extend) | |
| audio | `audio.h` (extend) | already includes `<gax.h>` |
| bosses | `bosses.h` (new) | |
| crates | `crates.h` (new) | includes `crate.h` (types) |
| cutscene | `cutscene.h` (extend) | |
| enemies | `enemies.h` (new) | |
| frontend | `frontend.h` (new) | |
| gfx | `gfx.h` (new) | |
| hud | `hud.h` (extend) | |
| iwram | `iwram.h` (new) | the ARM IWRAM routines |
| level | `level.h` (new) | includes `level_state.h`, `level_data.h` |
| link | `link.h` (new) | includes `link_session.h` |
| menus | `menus.h` (new) | |
| objects | `objects.h` (new) | |
| pickups | `pickups.h` (new) | |
| player | `player.h` (new) | |
| save | `save.h` (new) | |
| system | `system.h` (new) | `memory.h` and `irq.h` stay as they are (already real headers) |
| text | `text.h` (**done**, pilot) | |
| util | `util.h` (new) | includes `aabb.h` |
| vehicle | `vehicle.h` (new) | |
| (shared globals) | `globals.h` (new) | see below |
| libgcc | `lib/libgcc/include/libgcc.h` (new) | `__udivsi3`, `__divsi3`, `__modsi3`, `__umodsi3`; not `_call_via_rN` |
| GAX2, AgbEeprom, SWI | `<gax.h>`, `<agb_eeprom.h>`, `<agb_syscall.h>` | already exist (docs/libraries.md) |

`src/data/` has no header of its own: data is declared where it is used.

### Who owns a symbol

1. A function or global defined in a C file belongs to the header of that
   file's subsystem. When the file layout put a function in a neighbour's
   file only for ROM order, the function goes with its subject. For
   example, `DestroyLargeFont`/`DestroySmallFont` are defined in
   `src/util/aabb_setup.c` but are declared in `text.h`.
2. A symbol defined only in data (`src/data/`, the generated level and
   graphics sources, `data/data.s`) or in the linker script (`sym_*.txt`,
   the IWRAM/EWRAM globals) goes in the header of the subsystem that uses it
   most (`top_user` in `symbols.tsv`). If the subsystem it belongs to is
   clearer, that wins: `gSmallFont`/`gLargeFont` are mostly used by menus,
   but they are the text subsystem's font instances, so they are in
   `text.h`.
3. A data or linker-script symbol that **three or more subsystems** use goes
   in `include/globals.h`. Today that is 27 symbols: `gActorList`,
   `gActorVtable`, `gAudioContext`, `gCamera`, `gCollidableList`,
   `gCrateList`, `gDispcnt`, `gEntityFlags`, `gEntitySpawner`,
   `gEntityVtable`, `gHud`, `gInput`, `gJetpackPlayerInactive`, `gKeys`,
   `gLevelLayers`, `gLevelState`, `gMenuSkyBg`, `gOamBuffer`,
   `gObjVramCursor`, `gPaletteCache`, `gPlayer`, `gRoomFrameCount`,
   `gSineTable`, `gSpriteBankSet`, `gSpriteRenderer`, `gUnknown_030012EC`
   and `gUnknown_030012F4`. 19 of them are conflicting.
4. References from one `src/data/` table to another (329 symbols that only
   data files use) stay in the data files. They are address-only and
   generated by the tools in many cases, so a header adds nothing.
5. `_call_via_r0`..`_call_via_r7` never go in a header. They are libgcc's
   register-call thunks (the gcc 2.x virtual-call path), and each call site
   declares the shape it calls with. The argument and return types of that
   declaration decide how the call is set up. There are 5 variants of
   `_call_via_r2` alone.

Where the remaining declarations would go (after the pilot):

| Header | Symbols | Conflicting | Declarations removed | Files touched |
|---|---:|---:|---:|---:|
| link (**done**, batch 1) | 12 | 2 | 19 | 7 |
| hud (**done**, batch 1) | 19 | 0 | 35 | 17 |
| cutscene (**done**, batch 2) | 24 | 1 | 27 | 9 |
| pickups (**done**, batch 2) | 28 | 3 | 33 | 10 |
| enemies (**done**, batch 2) | 32 | 1 | 34 | 6 |
| system | 33 | 6 | 135 | 73 |
| util | 17 | 9 | 69 | 49 |
| audio | 22 | 8 | 117 | 80 |
| libgcc (without `_call_via_rN`) | 8 | 2 | 53 | 39 |
| lib/gax (internal) | 43 | 3 | 74 | 22 |
| save (**done**, batch 3) | 50 | 2 | 62 | 9 |
| frontend (**done**, batch 3) | 60 | 2 | 109 | 15 |
| crates | 57 | 9 | 74 | 24 |
| objects | 100 | 36 | 318 | 75 |
| gfx | 120 | 29 | 383 | 85 |
| player | 129 | 13 | 173 | 33 |
| menus | 152 | 8 | 185 | 26 |
| actor | 197 | 6 | 365 | 49 |
| bosses | 239 | 10 | 344 | 35 |
| vehicle | 277 | 13 | 374 | 38 |
| level | 311 | 45 | 534 | 116 |
| globals | 27 | 19 | 540 | 162 |
| (data-only, stays) | 329 | 0 | 329 | 19 |

## Rules for every batch

### What goes in a header

- **The definition's prototype**, copied, with `extern` (the existing
  headers' style). Parameter names come from the definition, without
  `Arg`-style suffixes left by matching.
- **Real types.** A global gets the type of its definition or, for linker
  script and asm symbols, the type its users agree on. Read-only ROM data
  is `const` (`extern const struct vtable_slot gFontVtable[9];`). Arrays
  stay arrays and pointers stay pointers (see the risk list).
- **Comments from the local declarations** that explain something move
  with them. The apply tool lists the trailing comments it drops.
- Data files should include the header that declares their tables, so the
  compiler checks the definition against the declaration. In the pilot,
  `entity_vtables_7e3bec.c` and `hud_fonts_174be0.c` include `text.h`.

### Conflicting declarations

Every conflicting symbol is resolved in the batch of its owner:

1. The header gets the **definition's** type. If the definition itself is
   wrong (an unused parameter the callers never pass, a `void *` where the
   type is known), fix the definition first when that is byte-neutral.
   `FontResetPalette` lost an unused second parameter this way.
2. In each file whose declaration differs, delete it (`--adopt`), include
   the header and rebuild that file. If its `.s` is identical, that's it.
3. If the bytes change, the file keeps its own declaration as an
   **asm-label alias**, so it can still include the header:

   ```c
   /* codegen: RandRange is u16, but this file was matched against an s32
    * return; the u16 prototype changes the stack slots in InitTitleScreen.
    * docs/headers_plan.md */
   extern s32 RandRange_s32(s32 max) asm("RandRange");
   ```

   Its calls use the alias. The object code is identical, because the call
   is still `bl RandRange` and the call site sees the same type as before.
   Keeping the old declaration under the real name is not possible:
   together with the header's it is a "conflicting types" error. The only
   other choice is not including the header in that file. Record every
   alias in "Codegen exceptions" below.
4. For pointer conflicts, adopting the header type sometimes needs casts or
   struct changes at the uses (`*addr = (void *)gFontVtable;`). Prefer
   fixing the type of the field or local that holds the pointer over a
   cast; both are normally byte-neutral, but check the `.s`.
5. A global declared with several **struct** types (`gPlayer`,
   `gLevelState`, `gKeys`, ...) can only be unified once those structs are
   merged (next section). Such a global is left out of its header batch and
   handled in the `globals.h` batches.

The "most common type" is not a safe default. `RandRange` is defined as
`u16` and declared `s32` in 14 callers. Neither type is right for every
file. Check each file.

### Duplicate structs (coordinated with #552, #557 and #569)

- A struct that one `.c` file uses, and that no header needs, stays local.
  Example: `struct glyph_oam` in `font_glyph.c`.
- A struct defined in several files, or under several names with the same
  layout (the audit's `structs.json`, `by_name` and `by_layout`), gets one
  definition in the owning subsystem's header, or in a small type header if
  several subsystems use it. The copies are deleted and their users are
  switched to it.
- Keep the **most complete field set**. For names, keep a field name that
  is already established in a header. Where copies disagree, pick the
  better-supported name and note the other one in a comment for #552. Don't
  invent names: `unk_XX`/`field_XX` placeholders stay until #557 renames
  them. A batch merges structs; it doesn't rename fields that aren't
  involved in the merge.
- #557's rule applies in reverse here. A merge may change how a field is
  accessed (`self->box[3]` becomes `self->box.h`, a `u8 buf[16]` becomes a
  `struct aabb`), so check each touched file's `.s`. Don't mix those
  changes with unrelated renames in the same PR.
- #569 lists type names that are misleading (`icon_manager`,
  `pause_options_screen`, ...). Header batches use the current names and
  don't rename types. #569 does that separately.
- A copy that doesn't really match (different size or offsets) is a
  different struct, or one of the copies is wrong. Look at the readers
  before merging, and don't force it.

### Prototype-driven codegen risks (check these first)

- **Unprototyped `f()` vs a prototype:** default argument promotion. All of
  today's unprototyped declarations are in `src/data/`, where they are
  address-only and safe.
- **`u8`/`u16`/`s8`/`s16` vs `s32`**, in returns and parameters. The caller
  truncates or extends again (RandRange). Explicit `(u8)` casts at the call
  site often hide the difference (`HasTurboRun`).
- **One-byte struct arguments** (`struct flag8`, passed in a register).
- **`asm("sym")` labels** already in use (`BreakCrateInStack` in
  `crate_break.c`).
- **`volatile`/`const` on globals** can change load scheduling. `const` on
  read-only ROM arrays was neutral in the pilot.
- **Array vs pointer externs**: `extern u8 x[]` loads the address,
  `extern u8 *x` loads the pointer stored there. They are different
  objects, so a mismatch is a bug, not a style choice.
- **Different struct types for one global**: field offsets and access width.

## Batch order

Each batch is one PR. It is one subsystem, or two very small ones, and both
full clean checks must pass before the PR opens. The order goes from leaves
to hubs. Small, low-conflict headers come first, so the method is tested
before the files that every subsystem touches.

1. **Phase 0 (this PR):** tools, plan, `text` pilot, `aabb.h`.
2. **link + hud (done):** 31 symbols, 2 conflicts (`LinkStop`/`ResetLinkSessionState`
   are declared `void (void)`/`void (u8 *)` in `link_sio.c` but defined
   `s32 (struct link_session *)`). See "Batch 1" below.
3. **cutscene + pickups + enemies (done):** 84 symbols, 5 conflicts. See
   "Batch 2" below.
4. **save, frontend (done):** 110 symbols, 4 conflicts, plus
   `gLinkSession` from batch 1. See "Batch 3" below.
5. **util + `libgcc.h`:** `RandRange` (see above), `__modsi3`/`__umodsi3`
   variants. `aabb.h` takes the remaining 8 `struct aabb` copies (crates,
   gfx, objects, player, `src/util/aabb.c`).
6. **system, audio:** `WaitForVBlank` (27 files), `PlaySfx` (66 files, 3
   variants). Big include fan-out but few distinct symbols.
7. **menus, crates, player:** one PR each.
8. **actor, bosses, vehicle:** one PR each. These have many symbols but few
   conflicts, and few files outside the subsystem use them.
9. **objects, gfx, level:** the hubs, with 110 conflicting symbols between
   them (`SetSpriteAnimDone` has 13 variants). These may need two PRs each:
   consistent symbols first, conflicts after.
10. **`globals.h`:** the 27 shared globals, a few at a time, each after the
    struct merge it needs (`level_state` for `gLevelState`,
    `held_pressed_pair` for `gKeys`, the player/actor structs for
    `gPlayer`). With this done, most of `docs/file_layout_plan.md`'s
    "Kept separate" pairs can become merges.

The lib batches (GAX2 internals in `gax_internal.h`) can go anywhere. They
don't interact with game code.

## Verification

Every batch PR, like any change:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
    -> crashbandicootxs.gba: OK
rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate -o /tmp/r.json
    -> 2056/2059 functions, 100% data (2059 means the build wasn't clean)
```

Also check that the batch adds no compiler warnings in the files it
touches. agbcc warns about incompatible pointer types and discarded
qualifiers, which is how a missed type mismatch shows up.

## Pilot: `text`

The `text` subsystem (`src/text/`, 8 files) has 16 symbols, and 24 other
files declare them. It has conflicts in both directions: `FontSetPalette`
had 5 variants in 10 files, `DrawWrappedText` had 2, and the vtable table
declared 8 of its functions unprototyped.

- **`include/text.h`** declares every function `src/text/` defines, plus
  `DestroyLargeFont`/`DestroySmallFont` (defined in `src/util/aabb_setup.c`),
  the two font instances `gSmallFont`/`gLargeFont`, the font vtables and
  the font data (`const`). It includes `aabb.h`, `bitmap_font.h` and
  `vtable.h`.
- **`include/aabb.h`** is new and holds `struct aabb`, which used to be in
  `gobj_1a794.h` (that header now includes it). The text box is an aabb:
  `DrawPowerDialog` fills it with `SetAabbPos`/`SetAabbSize`.
- The 8 text files include `text.h` in place of `bitmap_font.h`, and their
  local declarations of text symbols are gone.
- 24 other files include `text.h`, and 64 local declarations are gone
  from them (12 more from `src/text/` itself). Among them are 18 copies of
  `extern struct bitmap_font *gSmallFont;` and 10 unprototyped declarations
  in `entity_vtables_7e3bec.c`.
- **Struct merges:** `struct wrapped_text_box` (`wrapped_text.c`,
  `field_0/4/8`), `struct wrapped_text_box_params` (`text_box.c`,
  `field_0`/`field_c`) and the `s32 box[4]` in cutscene_player.c's `struct
  pager` all became `struct aabb` (`x`/`y`/`w`/`h`). cutscene_player.c's
  `struct pager_target` was a partial `struct bitmap_font` (`box0` is
  `marginX` at 0x118, `divisor` is `lineHeight` at 0x11C), so `pager.target`
  is now a `struct bitmap_font *`. `power_dialog_draw.c`'s `u8 buf[16]` is
  now a `struct aabb`. The local copies of `struct icon_glyph_metrics`
  (`hud_fonts_174be0.c`) and `struct aabb` (`aabb_setup.c`) are gone.
  Local structs went from 553 to 548.
- **Definition fix:** `FontResetPalette(struct bitmap_font *self, u32
  unused)` became `FontResetPalette(struct bitmap_font *self)`. All 6
  callers declared it with one parameter, and the second one was never
  read.

No local declaration needed an exception, so the pilot added no `codegen:`
comments.

## Batch 1: link + hud

- **`include/link.h`** (new) declares every function `src/link/`
  defines, `gLinkSessionReset` and `const u16 gCrc16Table[256]`. It
  includes `link_session.h`, and the four link files include `link.h` in
  its place. 22 local declarations are gone (17 in `src/link/`, 5 in
  save_menu_draw/input/ui.c).
- **`gLinkSession` was left out of `link.h`** in this batch: it was
  defined `void *` in iwram_data.c, and save_transfer.c declared it as a
  `struct sio_session *`, its own view of the session. Batch 3 merged
  that view and added it (see "Batch 3").
- **Definition fixes in link_sio.c**, all byte-identical:
  `ResetLinkSession`/`DestroyLinkSession`/`InitLinkSession`/`LinkStart`
  took `u8 *`/`void *` and now take `struct link_session *`.
  `ResetLinkSession` passes `self` to `LinkStop` (the ROM already had it
  in r0). `DestroyLinkSession`'s dead loop is the `players[4]` array's
  empty destructor loop (`self->players` .. `&self->players[4]`, step
  `q--`), and `InitLinkSession` stores `ring.field_84/88/8c` and
  `field_5` by name; its per-player loop keeps the raw `0xc6 << 1`
  offset, which is pinned in r6. `HandleLinkSerial` is called with
  `(u16 *)REG_ADDR_SIODATA32`, and the two `IrqSetHandler` calls got the
  `(irq_handler_t *)` casts link_session.c already used, which removes
  link_sio.c's two incompatible-pointer warnings.
- **`include/hud.h`** (extended) declares every function `src/hud/`
  defines, plus `DrawHudPart`/`InitHudPart` (defined in
  `src/gfx/palette_cycle.c`, which holds their ROM range),
  `gHudSlideOffset` (iwram_data.c) and the const part tables
  `gHudPartAnims[35]`/`gHudPartPositions[35]`. `gHud` stays for
  `globals.h`. 42 local declarations are gone. 20 of them were in 11
  callers in actor/, crates/, level/, pickups/ and player/, which
  declared the HUD functions with `void *` parameters.
  `InitHudPart` now takes and returns `struct hud_digit_part *` (it was
  `struct actor *`; it casts for `InitUiSpriteObj`).
- **Struct merges** (10 local definitions gone, 548 -> 538):
  - `struct hud_counter` takes the fields of hud_counters.c's `struct
    hud_score` and hud_slide.c's `struct hud_blink`/`struct blink_slot`:
    `wumpaSlideTimer`, `crateSlide`/`crateSlideTimer`, `wumpa`,
    `crateCount`, `crateTotal`, `value_d`/`value_e`, `shownWumpa`,
    `shownCrateCount`/`shownCrateTotal`, `shown_d`/`shown_e`. The header
    names won where both had one (`livesSlide`/`livesSlideTimer` over
    hud_score's `mode`/`layout_value`). hud_blink's `slots[3]` array of
    {state, timer} pairs became the named pairs (`slots[0]` lives,
    `slots[1]` wumpa, `slots[2]` crates), and hud_init.c's
    `*(s32 *)&self->unknown_0c[N]` stores became `wumpaSlideTimer`/
    `crateSlide`/`crateSlideTimer`.
  - `struct hud_digit_part` takes hud_init.c's `struct hud_slot`
    (`palette:4` at 0x29), and `struct hud_anim_record` takes `struct
    hud_record` (`tile_record` at 0x14).
  - `struct hud_pos` (4 copies: hud_init.c, hud_counters.c,
    hud_boss_clock.c, hud_fonts_174be0.c) is in `hud.h`.
  - hud_slide.c's local `struct icon_slot` copy is `struct vtable_slot`
    (vtable.h): `DestroyHud` calls slot 10 of the part's vtable
    (`gHudPartVtable`).

  The `slots[N].state` -> named-field change, the `selfArg`/cast pair
  dropped from the three hud_counters.c functions and `parts[34]` in
  place of `(u8 *)parts + 0x880` in hud.c were all identical (hud_init.c,
  hud_counters.c and hud_boss_clock.c are old_agbcc).
- iwram_data.c includes `hud.h` and `link.h`, and the two data files
  include the header that declares their tables, so the compiler checks
  each definition.

No file needed an asm-label alias, so batch 1 adds no codegen exceptions.

## Batch 2: cutscene + pickups + enemies

77 local declarations are gone, and 16 local struct definitions
(538 -> 522).

- **`include/cutscene.h`** (extended) declares every function of the
  slideshow player (slideshow.c, slideshow_display.c and the first four
  functions of cutscene_player.c), `gCutscenes`, `gCutsceneTexts` and
  `gSlideshowDispcnt` (now `u32`, the DISPCNT word it is;
  `SetSlideshowDispcnt` takes a `u32` too). It gets **`struct
  cutscene_player`**, which replaces the player's four local views:
  slideshow.c's and slideshow_display.c's `struct SoundChannelList`,
  cutscene_player.c's `struct pager` and level_cutscene.c's `struct
  text_pager` (`slides`, `count`, `toggle`, `pages`, `font`, `box`). Its
  slides are the header's `struct cutscene_slide` (the two `struct
  SoundChannelItem` copies and `struct pager_item`; `field_04`..`field_18`
  became `wait`/`fade`/`fadeAfter`/`buttons`/`duckMusic`/`rearmSfx`/`cue`/
  `sfx`), its pages `struct cutscene_page` (`struct pager_text`), and
  level_cutscene.c's `struct text_list` is `struct cutscene_slides`. The
  pager's box is a `struct aabb` (level_cutscene.c's `box.pos.x`/`size.x`
  became `box.x`/`box.w`). `ResetSlideshow` and `InitCutscenePlayer`
  store by field name.
- **Not in `cutscene.h`:** the background streamer and layer functions in
  cutscene_player.c (`DecodeLayerChunk`..`StepBgLayerScroll`, and
  `gBgStreamerVtable`/`gBgLayerBaseVtable`). They only share its ROM range;
  their subject is the level's background layers (`bg_layer.c`,
  `bg_layer_base.c`), so they go in `level.h` (ownership rule 1). That
  includes the `DestroyBgLayerBase` conflict. Likewise `ResetActionCtrl`
  (wumpa.c) goes in `player.h`.
- **`include/pickups.h`** (new) declares every wumpa, extra life and
  stopwatch function, their three vtables and the two hop width tables
  (`const s32 [3]`, copied as `struct three_words`, which moved there from
  two local copies). Wumpas and extra lives are `struct orbit_part`s, so
  every function of extra_life.c, wumpa.c and wumpa_update.c now takes
  one (they took `void *`, `struct actor *` or `u8 *`). wumpa.c,
  `StartWumpaPayout` and the small extra_life.c setters use field names;
  the larger extra_life.c functions keep their `u8 *` body behind a
  cast. `struct orbit_part` gets `animDone` at 0x38 (the name every other
  sprite part uses).
  - Conflicts: `PickUpWumpa` is called on items of the wumpa/extra-life
    list in crate_break.c and time_trial.c, which cast to `struct
    orbit_part *`. `StartWumpaPayout` takes the orbit part.
    `CreateWumpa` was declared `void (u16)` in spawn_pickups.c: SpawnWumpa
    now passes its four arguments, which also made its `asm volatile`
    keep-alive of `arg1`..`arg3` unnecessary.
  - **Definition fix:** `CreateStopwatch` gets a fourth, unused `u16`
    parameter. SpawnStopwatch passes it in r3 (with three parameters its
    `.s` changes), and the definition's code is the same.
  - drop_extra_life.c's `struct spawn_part` was a view of `struct
    orbit_part` (`anim`/`frameNibble`/`unk_49..4B` are
    `bank`/`slotNibble`/`counter`/`mode`/`phase`); it is gone.
- **`include/enemies.h`** (new) declares every function of src/enemies/,
  the three vtables, `gEnemyCtrlMotionSet` (incomplete `struct
  entry_set`, defined in the data file), the four `gHomingEnemy*` IWRAM
  words and `struct periodic_spawner`. `SetEnemyMotionY`/`X` and
  `SetEnemyAnimMode` moved there from `part_ctrl.h`, which stays the type
  header.
  - **One controller struct.** `struct part_ctrl` (`part_ctrl.h`) absorbs
    enemy_ctrl.c's and enemy_shooter.c's `struct trigger_ctrl` and
    text_popup.h's `struct enemy_ctrl`; `struct ctrl_anchor` absorbs
    `struct popup_vtable` (`attach` at 0x18). New fields: `manager`
    (0x04), `shotPeriod`/`shotPhase` (0x48/0x4C). trigger_ctrl's names
    lost to the header's: `boxX/Y/W/H` -> `boxL/T/R/B`, `unk_30..38` ->
    `idleTime`/`attackTime`/`cycleOffset`, `oscDivisor/Phase/Amplitude`
    -> `period`/`phase`/`amplitude`, `period/phase` -> `shotPeriod`/
    `shotPhase`, `unk_6c` -> `kind`, `owner` -> `target`, `triggerTable`
    -> `anims`, `vtable` -> `anchor`. `anims` is `const s32 *`: the anim
    maps are `const s32 [8]` (popup_tables_16b98c.c), and
    spawn_enemies.c's local declarations of them (`u8 []`) now say so.
    The spawners in spawn_enemies/bosses/objects.c use `struct part_ctrl`
    for their `hdr`.
  - `CreateEnemyCtrl` was declared `struct enemy_ctrl *(void)` in
    text_popup.h, with each spawner calling `OperatorNew(0x8c);` and then
    `CreateEnemyCtrl()` (the block passed on in r0). 15 of the 26 sites
    now call `CreateEnemyCtrl(OperatorNew(0x8c))`; the other 11 keep the
    two statements through an alias (see "Codegen exceptions").
  - spawn_objects.c's `struct periodic_spawner` showed that +0x1C is the
    spawner's `callback` (`UpdatePeriodicSpawner`'s pinned "dead read"
    into r4 is the `_call_via_r4` target), so the shared struct names it
    and `sub_800CB60` takes a function pointer.
  - The knocked controller is a 0x10-byte base `Ctrl`, not a `struct
    part_ctrl`: its functions keep `void *` (see enemies.h).
  - dingodile.c can't include `enemies.h` yet (its local `struct
    part_list` clashes with box_part.h's), so it declares
    `CreateEnemyCtrl`/`DestroyEnemyCtrl` with the header's types and casts
    its `self`. The bosses batch removes them.
- The vtable and table data files include the new headers; 22
  unprototyped declarations are gone from entity_vtables_7e3bec.c.

Every touched object file is identical to the clean build's. A few `.s`
files differ only in local label numbers (`.L`/`.LCB`): cutscene_player.c,
wumpa.c, crate_break.c, time_trial.c, spawn_pickups.c, drop_extra_life.c.
Including a header with static inline functions or a changed function
body can shift gcc's label counter; the labels don't reach the object.

## Batch 3: save + frontend

157 local declarations are gone (4,436 -> 4,279), and 21 local struct
definitions (522 -> 501). No file needed an asm-label alias.

- **`include/save.h`** (new) declares every function of src/save/, the
  save menu (`gSaveMenu`, now `struct save_menu *` in iwram_data.c), the
  menu tables in menu_tables_16b138.c (`const`), `gEepromNeedsInit` and
  the two link-text pointers (`const u8 *`). It includes `save_menu.h`
  and `settings_sync.h`, which stay as type headers; the save files
  include `save.h` in their place.
  - Definition fixes: `SummarizeProgress` takes `struct save_menu *`
    (was `void *`, unused). `EndLinkSaveTransfer` takes an unused
    `struct save_menu *self`: SaveMenuLinkInput passes it (the ROM sets
    it up in r0), so the caller had declared it with a parameter.
    `LoadSaveMenuBg` takes `struct save_menu *`: its local
    `struct bg_widget` was the save menu's DISPCNT shadow (`field_1c`)
    seen as two bytes, now `((u8 *)&self->field_1c)[0]`/`[1]`.
  - `CheckSaveChecksum` returns `u32`; save_menu_input.c had declared it
    `u8`. SaveMenuLinkInput now writes `!(u8)CheckSaveChecksum(...)`, the
    same cast save_data.c uses. Without it the bytes change.
  - `RunSaveMenu` is `u8 (u32, u32)`; game_frame.c declared `s32 (s32,
    s32)`. Identical with the header's type.
  - Block-scope `extern`s of save symbols in save_menu_input.c are gone.
- **`gLinkSession` is in `link.h`** as `struct link_session *` (the
  definition in iwram_data.c too). save_transfer.c's `struct
  sio_session`/`struct sio_channel` were `struct link_session` and
  `struct link_ring`: `tx` is `ring` (+0x40) and `rx[i]` is
  `players[i].ring` (+0x108, stride 0xc8). `struct link_ring` takes the
  channel's names: `field_84`/`field_88`/`field_8c` became
  `count`/`readPos`/`writePos` (link_sio.c, link_session.c and
  link_session_reset.c use them too). ReceiveSaveTransferChunk keeps its
  raw `0x18c`/`0x108` offsets (pinned registers), with comments naming
  the fields. Begin/EndLinkSaveTransfer store `field_5` by name.
- **`include/byte_arg.h`** (new) holds `struct byte_arg`, the packed
  one-byte argument of `DrawSaveSlotStats`. The copies in
  save_menu_draw.c, enemy_ctrl_update.c and jetpack_spawn.c are gone.
- **`include/frontend.h`** (new) declares every function of the
  language select, company logos, title screen, credits and starfield,
  and their data: `gLanguageSelect` (now typed in iwram_data.c), the
  language names and palettes, the logo actor's anim record (incomplete
  `struct anim_record_view`), vtable and IWRAM tile words, the logo piece
  seeds and motions, the title and logo graphics packages (`const struct
  bg_package`, which the users used to declare `u8 []` and cast) and the
  credits text and logos. It includes `actor_self.h`,
  `graphics_package.h`, `logo_screen.h` and `vtable.h`.
  - Struct merges: `struct language_select` (2 copies), `struct
    delta_record` (4 copies, two in data files), `struct slot_seed` (4
    copies; its `record` is a `const struct delta_record *` and the data
    files declare the motion tables with that type), title_screen_init.c's
    copy of `struct logo_piece` (logo_screen.h's is complete), and
    language_select.c's `struct linked_node`, which was a view of `struct
    actor_self` (`DestroyLogoActor` takes one). `struct credits_screen`
    and its node/glyph structs moved into the header with the credits
    prototypes.
  - Definition fixes: `LanguageSelectBlink` takes `struct
    language_select *` (was `s32 *`; `frame` is now `s32`, which keeps the
    ROM's `asr`), `DestroyLanguageSelect` takes it too, and
    `InitLogoActor`'s anim record is `const void *`.
  - Conflicts: `UpdateStarfield` was declared `void (s32)` in three
    callers, which now pass the pointer (`self[0x82]` with a cast).
    `RunCompanyLogos` is `void (u32 *)`; level_state.c declared `s32
    (void)` and now passes the block it already holds in r0. Identical.
  - credits.c reads `gCreditsLogos` (`const struct bg_package [5]`)
    through its own `struct popup_glyph_src`: the loops compare the size
    fields signed, and `bg_package` has them `u32`. The view stays, with
    a cast at the one use.
- Not done here, by ownership (each goes with its subject's batch):
  - the continue prompt functions at the start of credits.c
    (`DrawContinuePrompt`..`RunContinuePrompt`) and `struct
    continue_prompt` (3 copies with different views of +0x10) go to
    `menus.h`, with the rest of the continue prompt;
  - `LoadTaggedAssetBuffered` (language_select.c) and `gIntrTableTimer2`
    (save_data.c) go to the system batch, `gUnpackRleSpriteFrameFunc` to
    gfx;
  - the display register views copied into company_logos.c,
    title_screen.c and title_screen_init.c (`struct dispcnt_bits`, `union
    bgcnt`, `struct oam_attrs`, `struct oam_entry`, `struct oam_buf`) go
    with the gfx batch: menus and gfx have other versions of each (a
    packed `union bgcnt`, a 0x10-byte `struct oam_attrs`, four views of
    `struct oam_shadow_buffer`), and the unpacked `union bgcnt` here is 4
    bytes on the stack;
  - `struct cam_ref` (the actor's `record` seen as a camera) goes with
    the actor batch, `struct held_pressed_pair` (`gKeys`) with
    `globals.h`, `struct icon_frame_nibble` (save_menu_draw.c,
    pause_menu_pages_init.c) with menus.
- **Surprise:** `COMPILE_TIME_ASSERT` names its typedef after the line
  number, so two headers with an assert on the same line can't be
  included together (`logo_screen.h:49` and `actor_anim.h:49`).
  anim_family_178f80.c (defines `gPolarCategoryPalette`) doesn't include
  `frontend.h` for that reason.
- The data files that define the new headers' tables include them, so
  the compiler checks each definition. level_gfx_17cff4.c's anim record
  points into `gPolarCategoryPalette` by byte offset, now through a
  `(const u8 *)` cast.

Every touched object file is identical to the clean build's, and so is
every `.s` file.

## Codegen findings

The pilot itself had **no codegen surprises**: every file's `.s` was
identical. That is evidence that the following changes are byte-neutral
here (built with agbcc and, in `font_glyph.c`, `font_draw_text.c`,
`font_measure.c`, `wrapped_text.c` and `cutscene_player.c`, old_agbcc):

| Change | Where | Result |
|---|---|---|
| parameter `s32` -> `u8` (arguments are constants or `c ? 1 : 2`) | `FontSetPalette` in save_menu_draw/input/ui.c, pause_menu_draw.c | identical |
| return `s32` -> `void` (result unused) | `FontSetPalette` callers | identical |
| parameter `void *` -> `struct bitmap_font *`/`struct aabb *` | `FontSetPalette`, `DrawWrappedTextInBox` | identical |
| `u8 x[]` -> `const struct vtable_slot x[9]` / `const u8 x[]` / `const struct icon_glyph_metrics x[79]` | font_glyph.c (old_agbcc, reads `gSmallFontChars[j]` in a loop), font.c, aabb_setup.c | identical |
| unused trailing parameter removed from a definition | `FontResetPalette` | identical |
| stack `u8 buf[16]` -> `struct aabb` | `DrawPowerDialog` | identical |
| member `s32 box[4]` -> `struct aabb box`, `box[0]`/`box[3]` -> `.x`/`.h` | `RunCutscenePlayer` (old_agbcc) | identical |
| unprototyped `void f();` -> full prototype, in a data table | entity_vtables_7e3bec.c | identical |
| local struct with `field_N` names -> shared `struct aabb` (`x`/`y`/`w`/`h`) | wrapped_text.c, text_box.c, aabb_setup.c | identical |
| parameter `s32` -> `u8`, argument `x != 0` | `ConfigureHudParts` in actor_vram_pool.c | identical |
| parameter `void *` -> `struct hud_counter *`/`struct link_session *`, return `void` -> `s32` (unused) | 11 HUD callers, save_menu_*.c | identical |
| call with no argument -> passing the caller's own first argument | `LinkStop(self)` in `ResetLinkSession` | identical |
| `struct { s32 state, timer; } slots[3]` with constant indices -> six named `s32` fields | hud_slide.c | identical |
| `u8 *` byte offsets -> struct members / array elements, pointer step `-= 0xc8` -> `--` | link_sio.c, hud.c | identical |
| local struct copy -> `const` table pointer (`const struct hud_pos *`) | hud_init.c (old_agbcc) | identical |
| `void *` global -> `u32` (DISPCNT shadow, byte and halfword views through casts) | `gSlideshowDispcnt` in slideshow.c (old_agbcc) | identical |
| parameter `void *`/`struct actor *`/`u8 *` -> `struct orbit_part *`, body kept on a `u8 *` copy | extra_life.c (old_agbcc) | identical, but passing the parameter itself on to a call (rather than the `u8 *` copy) keeps both live: `push {r4, r5, r6}`. Pass the copy (cast) |
| `OperatorNew(n); p = Create();` (block passed on in r0) -> `p = Create(OperatorNew(n))` | spawn_enemies.c (old_agbcc) | identical in 15 of 26 functions; in 11 the registers change |
| call with one argument -> all four slot arguments, `asm volatile` keep-alive removed | `CreateWumpa` in SpawnWumpa | identical |
| call with the 4th argument dropped (3-parameter prototype) | `CreateStopwatch` in SpawnStopwatch | **changes** (r3 setup and a push); the definition takes an unused 4th parameter instead |
| `u8 []` extern -> `const s32 [8]` (the definition's type), stored through `const void **` | spawn_enemies.c anim maps (old_agbcc) | identical |
| caller's `u8` return -> definition's `u32`, call written `!(u8)f(...)` | `CheckSaveChecksum` in SaveMenuLinkInput | identical; without the cast the `lsl #0x18` is missing and r6/r7 swap |
| return `s32` -> `u8`, parameters `s32` -> `u32` | `RunSaveMenu` in game_frame.c | identical |
| call with no argument -> passing the value already in r0 | `RunCompanyLogos(tmp)` in ShowCompanyLogos | identical |
| unused parameter added to a `(void)` definition the caller passes `self` to | `EndLinkSaveTransfer` | identical |
| local struct view of two bytes -> `((u8 *)&self->field_1c)[n]` of the real struct | `LoadSaveMenuBg` | identical |
| local `struct sio_session`/`sio_channel` -> `struct link_session`/`link_ring` | save_transfer.c | identical |
| `s32 *` parameter read as `>> 2 & 1` -> struct pointer, field made `s32` | `LanguageSelectBlink` | identical (keeps `asr`) |
| `u8 []` extern cast to `struct bg_package *` -> `const struct bg_package` and `&` | company_logos.c, title_screen.c (old_agbcc) | identical |
| local list-node struct -> `struct actor_self` (`prev`/`next`/`vtable`) | `DestroyLogoActor` | identical |
| `(s32)` argument -> pointer, `u32` element -> `(void *)` cast | `UpdateStarfield` in title_screen.c (old_agbcc) | identical |

Experiments for later batches (not applied in this PR):

- **`RandRange` (util): neither type works everywhere.** The definition
  (`src/util/rand.c`) returns `u16`. 14 callers declare `s32`, and
  `airship_explode.c` declares `u16`.
  - Switching `airship_explode.c` to `s32` removes every `lsl #0x10`/
    `lsr #0x10` pair after the calls.
  - Switching each `s32` caller to `u16` is identical in 13 of them.
    `title_screen_init.c` changes: its two stack slots swap (`[sp, #0x20]`
    and `[sp, #0x24]`, 14 lines).
  - With the header at `u16`, `title_screen_init.c` can keep
    `extern s32 RandRange_s32(s32) asm("RandRange");` and still include the
    header. That build's `.s` is identical to today's. This is the alias
    pattern from rule 3.
- **`HasTurboRun`/`HasSuperBodySlam` (level):** `action_ctrl_moves.c`
  declares them returning `s32`, but they return `u8`. Switching to `u8` is
  identical, because the call sites already cast the result to `(u8)`.

## Codegen exceptions

Local declarations kept on purpose, with a `codegen:` comment. Each batch
adds its entries here.

| File | Symbol | Local form | Header form | Why |
|---|---|---|---|---|
| src/level/spawn_enemies.c | `CreateEnemyCtrl` | `CreateEnemyCtrl_r0(void) asm("CreateEnemyCtrl")`, called after a bare `OperatorNew(0x8c);` | `struct part_ctrl *(struct part_ctrl *self)` | in 11 of the 26 spawners (old_agbcc) the registers only match with the block left in r0 by the previous call; the other 15 use the header's prototype |

Known permanent exceptions: `_call_via_rN` (rule 5 above), and the
one-argument `LZ77UnCompVram`/`RLUnCompVram` in `src/system/asset.c`
(docs/libraries.md).

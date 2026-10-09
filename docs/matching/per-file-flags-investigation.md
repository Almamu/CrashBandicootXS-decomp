# Per-file optimization flags: investigation

> **Update (#664 part 10c-2):** the one flag this page found,
> `-fno-strength-reduce` on title_screen.o, is gone. Converted to C++
> (title_screen.cpp, [cplusplus.md](../cplusplus.md); company_logos.cpp
> since #770), `InitVvLogoPieces`
> is the plain indexed loop (`slots[i].countdown =
> gVvLogoPieceSeeds[i].hold + 1`), which matches with strength reduction
> on, as does every other function in the file. The C's hand-written
> pointer walks, which matched only without strength reduction, were the
> C front end's problem, not a per-file flag of the original build.
> title_screen_init.cpp is the first half of title_screen.cpp since
> #771, and pause_menu_gems.cpp is pause_menu_collectibles.cpp. The
> rest of the page is the investigation as it was.

Question: besides the old_agbcc/agbcc split, did the original build
also use different `-O2` sub-flags for some translation units? The lead
came from issue #65's retry pass (`issue-65-naked-retry.md`): three NAKED
functions in `src/frontend/title_screen_init.c` (`InitVvLogoPieces`,
`ResetTitleLogoPieces`, `RunTitleScreen`) were all stuck on the same "seed loop".
The ROM keeps an up-counting loop counter, leaves some address
arithmetic unsimplified and does not hoist 0/-1 out of the loop. Both
compilers at plain `-O2` reverse or strength-reduce that loop, and
`-fno-strength-reduce` was seen to stop the reversal in `InitVvLogoPieces`.

The rule used here: a flag only counts if it explains the whole file.
Every real-C function already matched in the file has to stay
byte-exact with it, the way old_agbcc did for its files.

**Result:** `-fno-strength-reduce` meets that bar for
`title_screen_init.o`, so it is now applied to that object alone
(the Makefile's `NO_STRENGTH_REDUCE_OBJS`), and `InitVvLogoPieces` is real C.
It is **not** a global property of the old_agbcc objects. The other two
seed loops need a source-level explanation instead (see below).

## Flags the compilers accept

Probed with a small loop against both `tools/agbcc/bin/old_agbcc` and
`tools/agbcc/bin/agbcc`. Both accept the same set:

| accepted (`-O2` sub-flags that can matter) | rejected (`Invalid option`) |
|---|---|
| `-fno-strength-reduce`, `-fno-rerun-loop-opt`, `-fno-rerun-cse-after-loop`, `-fno-cse-follow-jumps`, `-fno-cse-skip-blocks`, `-fno-gcse`, `-fno-expensive-optimizations`, `-fno-thread-jumps`, `-fno-regmove`, `-fno-caller-saves`, `-fno-force-mem`, `-fno-force-addr`, `-fno-peephole`, `-fmove-all-movables`, `-freduce-all-givs`, `-funroll-loops`, `-fno-omit-frame-pointer`, `-fno-defer-pop`, `-fno-strict-aliasing`, `-fno-function-cse`, `-O0/-O1/-O3/-Os` | `-fno-loop-optimize`, `-fno-schedule-insns`, `-fno-schedule-insns2`, `-fno-optimize-sibling-calls`, `-fno-delete-null-pointer-checks`, `-fno-crossjumping`, `-fno-if-conversion` |

No flag turns off gcc 2.95's loop optimizer as a whole. `-O3` and `-Os`
produced the same code as `-O2` on the probe.

## Whole-file sweep (title_screen_init.c, old_agbcc, NON_MATCHING drafts)

"Matched" means the file's 11 real-C functions at the start of this
work (`UpdateTitleLogoPieces`, `CommitTitleScreenFrame`, `DrawTitleMenuItem`, `DrawTitleScreen`,
`HashTitleCheatInput`, `DestroyTitleScreen`, `RunCompanyLogos`, `LoadVvLogoGraphics`,
`InitLogoActor`, `UpdateLogoActor`, `DrawLogoActor`). The last three columns are
the three blocked drafts **as they stood before this investigation**,
in halfwords off (ROM size in brackets when the size differs).

| flag(s) added to `-O2` | matched still exact | `InitVvLogoPieces` | `ResetTitleLogoPieces` | `RunTitleScreen` | other drafts |
|---|---|---|---|---|---|
| (baseline) | 11/11 | 36 [96 vs 104] | 38 [84 vs 120] | 136 [364 vs 392] | `UpdateVvLogoPieces` 20 |
| `-fno-strength-reduce` | **11/11 (byte-identical .s)** | 35 | 43 | 139 | `UpdateVvLogoPieces` 46 |
| `-fno-rerun-loop-opt` | 11/11 | 36 | 37 | 138 | - |
| `-fno-caller-saves` | 11/11 | no change | no change | no change | - |
| `-fno-peephole` | 11/11 | no change | no change | no change | - |
| `-fno-gcse` | 11/11 | 36 | 38 | 136 | `UpdateVvLogoPieces` 238, `LoadUniversalLogoBg` 124 |
| `-fno-thread-jumps` | 10/11 (`RunCompanyLogos`) | | | | |
| `-fno-regmove` | 10/11 (`DrawLogoActor`) | | | | |
| `-freduce-all-givs` | 10/11 (`DestroyTitleScreen`) | | | | |
| `-fno-rerun-cse-after-loop` | 9/11 (`RunCompanyLogos`, `DrawLogoActor`) | | | | |
| `-fno-cse-follow-jumps` | 9/11 (`RunCompanyLogos`, `UpdateLogoActor`) | | | | |
| `-fmove-all-movables` | 9/11 (`UpdateTitleLogoPieces`, `RunCompanyLogos`) | | | | |
| `-funroll-loops` | 9/11 | | | | |
| `-fno-cse-skip-blocks` | 8/11 | | | | |
| `-fno-force-mem` | 7/11 | | | | |
| `-fno-expensive-optimizations` | 6/11 | | | | |
| `-O1` (± `-fno-strength-reduce`) | 5/11 | | | | |

So the only candidates that keep the whole file are the flags that
change nothing in it (`-fno-caller-saves`, `-fno-peephole`,
`-fno-gcse`, `-fno-rerun-loop-opt`) and `-fno-strength-reduce`. For
`-fno-strength-reduce` the generated assembly of all 11 matched
functions is byte-identical with and without it (only local label
numbers change). That means none of them can tell the two builds apart:
the flag is *consistent* with them, but they are not evidence *for* it.
Most of their loops contain calls or branches that gcc would not
strength-reduce anyway. `UpdateTitleLogoPieces`'s `i * 0x34` stays a `mul` either
way.

The old drafts did not improve under any flag, because they were
written against the `-O2` optimizer. The real question was whether the
ROM's loop shape becomes *reachable* from source with a flag. That was
tested per function below.

## InitVvLogoPieces - matched with `-fno-strength-reduce`

With `-fno-strength-reduce` the first loop keeps `i` counting up, `cmp
r6, #0x13; ble`, like the ROM. With strength reduction on, gcc's
`check_dbra_loop` reverses any loop whose counter only feeds the exit
test into a `sub; cmp #0; bge` count-down. The pointer walks (`seed`,
`active`, `countdown`, `hold`) must then be the source's own, because
nothing reduces them for you. What was left after that was source
order:

- statement order `i = 0; zero = 0; seed = ...; active = ...; hold =
  ...; countdown = ...;` (all 720 orders searched: this one gets `self`
  into r5, `i` into r6 and `seed` into r3 like the ROM);
- the second loop needs its own counter `j` (the ROM uses a different
  register than the first loop's `i`) and its `1` in a local assigned
  before `j` and the pointer (the ROM materializes `mov r2, #1` first).

The resulting C is exact with `-fno-strength-reduce` and **16 halfwords
off without it** (the loop reverses again). A `goto`-loop version of
the same body (no loop notes, so no loop optimizer at all) is 14 off at
best over all 720 statement orders: the loop shape is right but the
register allocation is not. So the flag is the only explanation found
that closes this function.

## ResetTitleLogoPieces / RunTitleScreen - the flag does not explain them

These two seed loops do more than keep their counter. They **hoist
nothing**: `mov r0, #0` and `mov r0, #1; neg r0, r0` sit inside the
loop, and `self+0x14`, `self+0x40` and `seedBase+4` are recomputed every
iteration. `-fno-strength-reduce` only disables `strength_reduce()`.
Loop-invariant motion still runs, and every for-loop phrasing tried
hoists those invariants under every flag in the sweep.
`RunTitleScreen`'s later `while` loop *does* hoist its `REG_BLDCNT`
address (`ldr r5, =0x04000050` before the loop). So the optimizer was
on for that function, and the seed loop itself was simply never seen as
a loop.

That points to a `goto` loop (`label: ... if (++i <= 8) goto label;`).
It emits no `NOTE_INSN_LOOP_BEG`/`END` notes, so gcc 2.95's
`loop_optimize` never touches it. Rewritten that way (plus the ROM's
index-before-base order for the hold address, `(i << 3) + holdBase`),
both drafts get the ROM's exact loop shape and size, with or without
the flag:

| function | before | goto draft | what is left |
|---|---|---|---|
| `ResetTitleLogoPieces` | 38 [84 vs 120] | **18**, right size | register choice only: ROM `seedBase`/`counter`/`zero` = r8/sb/ip and `stride`/`slot` = r4/r5; the draft gets sb/ip/r8 and r5/r4. Declaration order (5040 orders) and statement order make no difference. |
| `RunTitleScreen` | 136 [364 vs 392] | **100**, right size | `i`/`slot` swapped between r3 and r5 in the seed loop, which shifts the rest of the function. |

Both drafts are updated in-tree under `#if NON_MATCHING`. This is a
source-level lead, not a flag. The next person should work on the
global-allocator priority of those goto-loop locals, not on compiler
flags.

## UpdateVvLogoPieces - no contradiction

The ROM's drain loop looks like classic strength reduction plus biv
elimination: a reduced slot pointer compared **signed** (`ble`) against
a hoisted end pointer, with no entry test. If only an `i` loop could
produce that, the file could not have been built with
`-fno-strength-reduce`. But a pointer `do { ... slot += 0x34; } while
((s32)slot <= (s32)((u8 *)self + 0x3dc));` gives the same loop with or
without the flag. The draft is now written that way: 19 halfwords off
(was 20), with the same remaining preheader-order and tail register
differences as before.

## Global test

Adding `-fno-strength-reduce` to **all** `OLD_AGBCC_OBJS`, then a full
clean `make compare`: **`crashbandicootxs.gba: FAILED`** (the ROM even
grows by 28 bytes). A per-object scan (compile each file with and
without the flag and compare the per-function assembly, labels
normalized) finds changed code in matched real-C functions in 11
old_agbcc files:

- `continue_prompt.c` (`InitContinuePromptGraphics`)
- `tiny_update.c` (`PickTinyHopTarget`; now tiny.cpp)
- `cortex.c` (`CreateTiny`; now tiny.cpp)
- `level_select.c` (`DestroyLevelSelect`)
- `level_select_pages.c` (`LevelSelectTurnPage`, `PlaceLevelSelectEntries`, `InitZoomBg`)
- `graphics_package.c` (`LoadGraphicsPackage`)
- `graphics_package.c` (`FitScaledSprite`)
- `hud_init.c` (`InitHud`)
- `title_screen_init.c` (`LoadTitleScreenBg`)
- `pause_menu_gems.c` (`DrawPauseRelicsPage`)
- `crate_break.c` (`SolidifyOutlineCrates`)

Those are all byte-exact today, so each change is a break. Among the
current-agbcc objects, 8 files change (`audio.c`,
`actor_category_stats.c`, `graphics.c`, `palette_cycle.c`, `collision_queue.c`,
`collision_map.c`, `irq.c`, `tile_slot_pool.c`). The flag is **not** a
property of the old_agbcc build as a whole, nor of the whole ROM. Of the
old_agbcc objects, `title_screen_init.o` is one where it changes
nothing that already matched.

## How confident is this?

The evidence for the per-file flag is one function. Every other matched
function in the file is indifferent to it. That is weaker than the
old_agbcc discovery, where the old compiler was *required* by several
already-matched functions. Treat `NO_STRENGTH_REDUCE_OBJS` as the
simplest build that matches, not as proof of the original makefile. If
someone later finds a source phrasing of `InitVvLogoPieces` that matches at
plain `-O2`, drop the list. The helper scripts for this investigation
(flag probe, whole-file sweep, statement-order searches, global scan)
were kept out of the tree.

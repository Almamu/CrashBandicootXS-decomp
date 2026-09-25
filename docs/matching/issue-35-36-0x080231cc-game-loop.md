# GitHub issues #35/#36: 0x080231CC-0x08023488 (49 functions, game_loop)

Both issues cover one contiguous, un-split raw block (formerly
`asm/code_3_2_17_231cc.s`, now retired): issue #35 lists
`sub_80231CC`-`sub_80232D0` (24 of its own 25 functions - `sub_80231C4`
was already matched in an earlier pass), issue #36 lists
`sub_80232D8`-`sub_8023484` (all 25). Together they're exactly this
file's 49 functions, `0x080231CC`-`0x08023488` (`sub_8023484` itself
ends at `0x080234E8`, where `src/system/game_loop10.o` picks up).

The block sits immediately after `src/system/game_loop2.c`'s own
existing functions (`sub_8022EA8`-`sub_80231C4`) in ROM, with no gap -
`ldscript.txt` already links `game_loop2.o` directly before what was
`asm/code_3_2_17_231cc.o`. All 49 functions turned out to be a direct
continuation of the exact same `self` type `game_loop2.c` already
established: the `gUnknown_030012C0`-pointed "level" object (confirmed
by `sub_8023A1C`/`game_loop56.c`, whose own opening dispatch reads
`*gUnknown_030012C0` and passes it as `self` to `sub_8023118`/
`sub_8023110`/`sub_8023484`). Rather than open a new file (which would
need a new `ldscript.txt` entry and a fresh struct-convention writeup),
all 49 were appended directly to `game_loop2.c`, in ROM address order,
keeping the existing object boundary and the existing `self+2`/
`self+0x80`-`0xc0` field convention that file already documents.

## The struct fields this pass adds

Building on `game_loop2.c`'s existing `self+2` flags byte and
`self+0x80`/`0x84`/`0x88`/`0xac`/`0xc0` int fields:

- **`self+2` bit 7**: one more getter (`sub_80231CC`) extending the
  existing flag-bit family (bits 4-6 already covered by
  `sub_80231B4`/`BC`/`C4` above).
- **`self+0x6c`/`self+0x70`/`self+0x74`/`self+0x78`/`self+0xbc`**: a
  counter/threshold/latch cluster. `self+0x70`/`self+0xbc` are a
  counter/limit pair already read by `sub_8022FEC`/`sub_802306C`
  (matched previously) and now also by `sub_8023484` (see below).
  `self+0x78` is a small "last state" latch (`sub_80231EC`/
  `sub_8023220`/`sub_8023224`). `self+0x6c`/`self+0x74` form a
  centisecond/second odometer pair (`sub_8023430`/`sub_8023464`,
  distinct from the digit-cascade odometer at `self+0x90`-`0x9c` that
  `sub_8022F2C` above already documents) - `self+0x6c` wraps at `0x63`
  and bumps `self+0x74` (itself saturating at `0x62`).
- **`self+0x7c`**: a plain free-running counter (get/increment/reset:
  `sub_80232E0`/`E4`/`EC`, plus a "start" helper `sub_8023304` that
  resets it and sets a flag).
- **`self+0x8c`**: a single flag byte, read by `sub_8023234` (guards a
  `self+0x74` decrement-and-notify) and cleared by `sub_80232D8`.
- **`self+0x90`/`0x94`/`0x98`**: getters for the bottom three tiers of
  `sub_8022F2C`'s existing digit-cascade odometer (`sub_8023270`/
  `8023268`/`8023260`).
- **`self+0xa4`-`self+0xa9`**: a bank of six busy/status-flag bytes
  (`sub_80232B8`/`90`/`98`/`A0`/`C8`/`F4` getters, plus clear-to-0 and,
  for three of them, set-to-1 setters) - the same shape as the `self+2`
  bitfield family, just laid out as whole bytes.
- **`self+0xc4`**: a "current index" field, read by two dispatchers
  (`sub_8023378`/`sub_80233B4`, see below), resolved to a slot address
  by `sub_80233FC`/`sub_8023404` (`self + idx*4 + 4`), and re-used as a
  `gStaticData_0816C86C`-style level index by `sub_8024498`
  (`game_loop18.c`) when `sub_80231EC` forwards `self+0xc4`'s address
  into it.
- **`self+0xc8`/`self+0x1c8`**: two more plain word fields
  (`sub_8023324` getter, `sub_8023318` setter) - `self+0x1c8` sits right
  after the `self+0x1c0`/`0x1c4` pair `sub_8023484` reads.

## The two `self+0xc4` dispatchers

`sub_8023378` and `sub_80233B4` both switch on `self+0xc4`'s value, but
compile to different shapes and both needed to be reproduced exactly as
the ROM's own compiler chose them:

- **`sub_8023378`** (values `0x14`-`0x17`, only 4 live cases) compiles to
  a **binary comparison tree** (`cmp/beq`, `cmp/bgt`, `cmp/beq`...), not
  a jump table - and the ROM's own tree treats `0x16`/`0x17` as two
  *separate* explicit equality tests (`cmp r0,#0x16; beq ...; cmp
  r0,#0x17; beq ...`) rather than a single range check, even though
  `0x14`/`0x16`/`0x17` all share one code block. Writing the three
  shared cases as one C `case 0x14: case 0x16: case 0x17:` fall-through
  group made this compiler fold the `0x16`/`0x17` pair into a single
  `bgt`-based range check instead - reproduced by writing each of the
  three cases as its own separate (identical-bodied) `case` block, which
  keeps the compiler from ever noticing they're a contiguous range.
- **`sub_80233B4`** (values `0x14`-`0x18`, 5 live cases) compiles to a
  genuine **5-slot jump table**, and slot 4 (`idx-0x14==4`, i.e.
  `idx==0x18`) points at the *exact same address* as the range-check's
  own "out of bounds" fallthrough target - the same cross-slot code
  sharing this project has already seen in `sub_8023A1C`'s state-1/6
  vs. state-5 tail (`game_loop56.c`). A plain `switch` with only 4
  explicit cases plus `default` stayed under this compiler's jump-table
  threshold and fell back to another comparison tree instead; adding an
  explicit (empty, fall-through) `case 4:` right before `default:`
  pushed the case count over the threshold and produced the ROM's real
  5-slot table, with slot 4 naturally aliasing `default`'s address since
  both bodies are identical.

  A second gotcha in the same function: the four live cases' return
  values (`{9,8,6,7}`) are each stored to a `result` local and *then*
  have `6` subtracted in one shared tail statement after the `switch`
  (`return result - 6;`) - reproducing the ROM's own separate
  `_080233F6: subs r0,#6` instruction, which case 4/default's own early
  `return -1;` bypasses entirely (jumping straight past it). Folding the
  subtraction into each case's literal (`return 9 - 6;` etc., i.e. just
  `return 3;`) constant-folds the whole thing at compile time and drops
  that shared instruction - **this was caught only by the full-clean
  `make compare`, not by any per-function isolated compile**: the
  isolated check only verifies the touched function's own bytes are
  internally self-consistent, but a droppped/added instruction shifts
  every ROM address after it, and this project's build maps everything
  by fixed absolute address via `ldscript.txt` - the actual failure
  surfaced as a 4-byte drift in `game_loop2.o`'s total linked size,
  caught only once the whole ROM was reassembled and diffed against
  `baserom.gba` (see `docs/workflow.md` step 3's warning about isolated
  compiles never being proof of a match).

## `sub_8023484`: closing GitHub issue #37's last gap

`sub_8023484` is the one function `docs/matching/issue-37-game-loop-2375c.md`
called out by name as still open - `sub_8023A1C`'s (`game_loop56.c`)
"init guard" call, firing once per level to lazily initialize this
"level" object's counter-notification state the first time
`self+0x70 == self+0xbc` and both busy flags (`sub_80232B8`/
`sub_8023290`) are clear. Its body is the exact same
`sub_8022FEC`/`sub_802306C`-shaped tail (if the `self+0xdc`-linked
level-state record's `+8` widget-kind field is `3`, OR a bit into the
`sub_8023404`-resolved slot; otherwise forward `self+0x1c0`/`0x1c4` to
`sub_801EB04`), gated by one extra counter/threshold check up front.

Reproducing the ROM's exact register map for the `sub_801EB04` tail
needed three separate register pins (`magic` for the `0xffff` first
argument, loaded early into `r0` rather than right before the call;
`off` pinned to `r3`, incremented in place rather than recomputed from
scratch for the second field's offset; `addr1`/`addr2` pinned to
`r1`/`r2` so the two address computations don't collide) plus **two**
empty-barrier statements (`asm volatile("" : "+r"(var));`) - one after
loading `b` (to stop the compiler deferring `b`'s load until after
`addr2` is computed), and one after `off += 4` (to force the actual
register write-back into `r3` rather than letting the compiler route
the incremented value through a fresh scratch register and never
touch `r3` at all, since nothing reads `off` again afterward). This is
the same class of gotcha `issue-37-game-loop-2375c.md` already
catalogued for `sub_802375C`'s own case-block shapes - a value that
looks "dead" after one more use still has to be visibly written back
to its pinned register if the ROM's own code did so.

## Also needed: `struct AudioContext` forward declaration

`sub_80231EC` (calls `sub_80017BC(gUnknown_030012BC, 0x12)` on state
`3`) needed `extern void sub_80017BC(struct AudioContext *self, u32
songIndex);`, matching the signature already used in
`src/audio/music_player.c`/`src/audio/audio_context.c`/
`src/graphics/level_graphics.c`. Unlike those files, `game_loop2.c` had
no prior reference to `struct AudioContext` anywhere at file scope, so
the tag's first appearance was inside this `extern` declaration's own
parameter list - triggering `agbcc`'s "declared inside parameter list"
warning (harmless, but this project's `make NON_MATCHING=1 report`
convention expects a clean file). Fixed with a one-line `struct
AudioContext;` forward declaration right above it.

## Result

All 49 functions matched as real C - no `NAKED` fallbacks needed for
this batch. Verified via a full clean `make NON_MATCHING=1 report` (no
warnings for `game_loop2.c`) and a full clean `make compare`
(`crashbandicootxs.gba: La suma coincide`). `asm/code_3_2_17_231cc.s`
is retired; `ldscript.txt`'s entry for it is removed (no replacement
needed, since `game_loop2.o` now covers the whole span directly).

This does not close issue #37 by itself - `sub_8023A1C` was already
matched separately (`game_loop56.c`, `NAKED`) in the same prior
session; this pass only closed the one remaining gap
(`sub_8023484`) that document's own follow-up section called out.

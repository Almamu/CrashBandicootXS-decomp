# Issues #9/#10: `0x0800A884`-`0x0800B8DC` follow-up (graphics)

This is the write-up for a follow-up session against three raw regions
flagged in `tools/report_units.py`'s graphics category (all rooted in
GitHub issues #9/#10's original scope, the `actor` "part" object
family already covered at length by
[issue-9-0x08007634-actor.md](./issue-9-0x08007634-actor.md)):

- `0x0800A884`-`0x0800AFF4` (`sub_800A884` through `sub_800AFF4`)
- `0x0800B3F0` (`sub_800B3F0`)
- `0x0800B8DC` onward (`sub_800B8DC`, 546+ lines, and beyond)

## Matched - 1 function

- **`sub_800B3F0`** (`src/graphics/actor_part77.c`) - a part-object
  constructor: re-initializes `self` via `sub_800A6A4` (matched,
  `actor_part14.c`), overwrites its table with `gStaticData_087E3E04`,
  clears its trailing `+0x108`/`+0x10c` fields via `sub_8010E2C` (still
  raw, trivial - a 2-field clear), allocates a fresh `struct
  actor`-shaped child object via `sub_8008434(0, 0, 0, 0)` (matched,
  `actor_part6.c` - called with the same "extra unused 4th zero
  argument" calling convention `graphics_loading_21d80.c`'s own callers
  already use) and hooks it up at `self+0xb0`: points its own `+0x20`
  table-entry pointer at `gUnknown_030012D0`'s shared table (the same
  `(u8 *)(**gUnknown_030012D0) + offset` idiom used throughout
  `graphics_loading_21d80.c`), clears its `+0x2d` byte, and builds it
  via the standard `sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio.
  Clears `self+0xb4`, then calls `sub_800A734` (matched,
  `actor_part48.c`) to finish the reset - `sub_800A734` itself is what
  hooks the `self+0xb0` child up via its own `sub_800815C` call, per
  its existing doc comment. Finally sets `self+8`/`self+0`/`self+4`
  (`field_08`/`x`/`y`) from its three `u16` arguments, the same
  `sub_800A604`-style tail `actor_part14.c` already established, and
  returns `self`.

  Matched on the first real attempt after one register-pin fix: the
  ROM keeps the constant `0` used for both `child+0x2d`'s clear and
  `self+0xb4`'s clear alive in `sl` across all three
  `sub_80087C0`/`sub_80087B4`/`sub_800872C` calls (a callee-saved
  register survives a `bl`), rather than reloading a fresh `0` for the
  second store - closed with `register s32 zero asm("sl") = 0;`
  spanning both stores and the three calls between them (the same
  "keep a reusable constant pinned across calls" idiom documented
  elsewhere in this project, just with `sl` instead of `r4`/`r0`).
  Every other instruction matched without any pin at all - this
  function's register pressure (3 real arguments plus `self`) already
  lines up with the ROM's own `sb`/`r8`/`r5` choices by default.

## Parked (`NON_MATCHING`, not yet byte-exact) - 1 function

- **`sub_800A884`** (`src/graphics/actor_part78.c`) - a per-frame
  "reentrancy guard"-shaped wrapper (only runs while `self+0xc` bit 7
  is set): fires `self->table+0x70`'s trampoline via `sub_803AD7C`
  (matched), then calls `sub_800A0FC` (still raw, its own return value
  discarded) with the global `gUnknown_03001308+0x2a` flag held set
  for the call's duration. If `self+0xac` (a pointer, cleared here)
  was non-null, sets `self+0x68` bit 3 and clears the
  `+0x100`/`+0x102`/`+0x103` flag bytes `actor_part48.c`'s doc comment
  already introduced. Then dispatches on `gUnknown_03001308+0x29` (a
  pending-action "kind" byte the ROM's own 10-entry jump table reads,
  cleared back to `0` by every path here): kind `0` additionally
  resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` reads exactly `8`;
  kind `1` sets `self+0xc` bit 6, clears `+0x8c`, calls `sub_80231EC`
  (still raw) and fires the `self->table+0x68` trampoline with code
  `1`; kinds `5`/`7`/`10` each set one of the `+0x100`/`+0x102`/`+0x103`
  flags; the rest are no-ops beyond the shared kind-reset. Finally
  looks up the current keyframe record via `sub_80083B8` (already
  parked as `NAKED` in `actor_part5.c`) and picks a `{s16 x, s16 y}`
  offset table off its `+4` byte's upper nibble - **the exact same
  case-to-block mapping `sub_80084C4` (`actor_part6.c`, already
  matched) uses**: `0` -> `info+0x24`, `6` -> `info+0x14`, everything
  else -> the fixed fallback `gStaticData_0816B300`. Applies that
  offset (mirrored by `self+0x28` bit 4) to `self`'s de-Q8'd position
  and probes the result via `sub_8026BC0` (still raw, only its return
  code's meaning as an opaque "hit" test against the constant `6` is
  used here, not its internals). A hit snaps `self`'s Y position down
  to the next multiple of 8 (unless `+0x101` is already set) and fires
  the `table+0x68` trampoline with code `0x17`; any other code fires
  the same trampoline with code `0x18` if `+0x101` is set. Returns the
  (possibly just-updated) `self+0x68` state byte.

  Every load, store, branch and call is confirmed correct against the
  ROM by full manual instruction-by-instruction trace (not a guess -
  see "Real gotchas" below for the specific idioms this needed and
  where they came from). Both of the function's jump-table dispatches
  reproduce the ROM's own tables exactly as a plain dense C `switch`
  (no case-scattering trick needed, unlike `sub_80084C4`'s own - this
  switch's case set is already dense enough on its own). The three
  `sub_803AD88` calls each also perform a "dead read" of the trampoline
  table's `+4` function-pointer field that's never actually passed
  through `r0`-`r3` - the same established idiom as `sub_80096C0`'s own
  `sub_803AD88` calls in `actor_part11.c`
  (`register void *deadRead asm("r4") = *(void *volatile *)(...)`).

  **Not yet byte-exact, but far closer after two follow-up sessions.**
  The earlier claim that the leading ~40 instructions were "confirmed"
  came from an *isolated* compile (just that prefix, ending right
  after `sub_800A0FC`'s call) - that isolated match did not survive
  once the rest of the function was compiled alongside it (see "Real
  gotchas" below, point 1). A follow-up session took the whole
  function - the leading block, the 10-way "kind" dispatch (all ten
  case bodies plus both the `idx <= 9` range check and the jump table
  itself), and the keyframe-lookup/camera-probe tail (including its
  own 7-entry `type` jump table sharing `sub_80084C4`'s case-to-block
  mapping) - through the same `arm-none-eabi-cpp`+`agbcc` diagnostic
  loop `docs/workflow.md` describes, iterating against the *whole*
  ROM disassembly (not a truncated prefix) after every change. That
  session closed everything down to two narrow, purely register-
  *choice* gaps (neither changed program behavior or even instruction
  count):

  1. The `self+0x105` clear's transient offset scratch register: `r0`
     here vs. the ROM's `r2` (the *destination* address still
     correctly lands in `r6` either way).
  2. The `kindZero` (`self+0x68 == 8`) test's loaded-byte register:
     `r0` here vs. the ROM's own self-overwriting `ldrb r7, [r7]`
     (which destroys the address register with the loaded byte,
     rather than using a fresh one).

  A **second follow-up session** (objdiff fuzzy-match 96.8% -> 98.0%)
  re-verified the whole function byte-for-byte from scratch (not
  trusting the first session's "only two gaps" claim at face value -
  see "Real gotchas" point 10 below) and found a **third, previously
  undocumented gap** in the camera-probe tail's Y-snap arithmetic, plus
  closed gap 2 above entirely, using two new techniques not tried in
  the first session:

  - **The Y-snap gap** (`s32 snap = (((u32)y & 0x00FFFFF8) + 7) - y;`):
    this compiler's -O2 was re-associating the expression into
    `masked - (y - 7)`, using a *second* register the ROM's own
    `ands`/`adds #7`/`subs` sequence never touches. Closed by forcing
    the whole computation through one pinned accumulator
    (`register s32 acc asm("r0")`, written as three separate
    statements on that same variable) so the compiler has no freedom
    to re-associate it into a different register pairing. Confirmed
    via full-function diff to have **zero effect on any other register
    choice** in the function - a clean, fully isolated fix.
  - **The `kindZero` `r7` self-overwrite gap**: closed via a
    *matching-constraint* asm operand (`"0"(p68)`) rather than a
    freshly `register T v asm("r7")`-declared variable - `asm
    volatile("ldrb r7, [r7]" : "=r"(r7byte) : "0"(p68));` ties the
    output register to wherever `p68` *already* lives (which is `r7`,
    unforced, simply this compiler's own natural allocation choice for
    it), instead of asking the allocator to conjure a *new*,
    independent `r7` binding. This introduces zero new hard-register
    requirements into the function's register-allocation graph, which
    is why - unlike every technique tried in the first session - it
    does **not** ripple the `+0xac` block's own register choices. Also
    closed, as a consequence, the `kindZero` three-clear tail's offset-
    walk register (now correctly continuing in `r2` across all three
    clears, `+1` then `-3`, exactly matching the ROM), at the cost of
    one small remaining side effect: a single extra `movs r1, #0`
    scheduled right after the `cmp r7, #8` branch, that this session
    could not eliminate or relocate (every placement of the source-
    level `storeVal = kind;` assignment - including via the same
    matching-constraint technique - produced the identical extra
    instruction in the identical position, a compiler-driven constant-
    propagation artifact rather than a moveable register choice).

  The `self+0x105` gap (gap 1) remains open - every technique tried
  this second session (a matching-constraint asm block hardcoding
  `r5`/`r6`, a barriered register pin) reproduced the *exact same*
  ripple as the first session (`self` itself moving from `r5` to `r6`
  for the rest of the function), confirming this is a stable, narrow
  compiler-fragility floor rather than an unexplored lead - the
  distinguishing factor from the successfully-closed `kindZero` gap is
  that gap 1 needs a genuinely *new* hard-register binding (`r2` for a
  literal that has no pre-existing forced home), where the `kindZero`
  fix only *reused* a binding (`r7`) already forced there by `p68`'s
  own unrelated, natural allocation.

  Net result of the second session: one gap fully closed (`kindZero`
  `r7` test, worth ~1 instruction), one previously-undocumented gap
  found and fully closed (Y-snap arithmetic, worth ~2 instructions),
  and the `kindZero` offset-walk register also fixed as a consequence
  (~2 more instructions) - at the cost of one small new single-
  instruction artifact (the extra `movs r1, #0`). Verified via
  `objdiff-cli report generate` (96.8% -> 98.0% fuzzy match for this
  unit) and a full clean `make compare` (ROM checksum still matches -
  this function stays `NON_MATCHING`/parked, so the real, ROM-exact
  bytes in `asm/code_3_2_16_a884.s` are still what actually ships;
  only the *parked C reconstruction*, used for eventual matching and
  for readability/documentation purposes, improved). See the source
  file's own doc comment for the full accounting, including the "Real
  gotchas" both sessions needed (case-scattering for the second jump
  table, the `s32`-not-`u8` switch-index type, matching the ROM's own
  block *order* not just its goto targets for `kindZero`, the `case 4`
  vs. `case 6`/`case 10` register-role split that keeps this
  compiler's own tail-merge pass from over-merging, the shift-not-mask
  bit-4 test, the Y-snap re-association fix, and the matching-
  constraint technique that closed the `r7` self-overwrite gap without
  rippling). Real bytes stay in `asm/code_3_2_16_a884.s`
  (`asm/code_3_2_16.o` trimmed to start at `sub_800AAEC`), following
  the same `.if NON_MATCHING == 0` pattern as `sub_800A528`'s own
  `asm/code_3_2_11_a528.s`. A future session picking this up should
  treat the `self+0x105` gap as a genuine compiler-fragility floor for
  this function shape rather than an unexplored lead, unless a new
  technique (not yet tried across either session) presents itself; the
  remaining single extra `movs r1, #0` instruction is a much smaller,
  lower-priority target that may be worth one more look.

### Real gotchas found closing the leading block

1. **A bit-7 test written as `field & 0x80` compiles differently from
   `field >> 7`**, even though both are semantically "is the top bit
   set": the ROM uses `lsrs r0, r1, #7` (a shift), not an AND-mask.
   Matching this needed the source written as `self[0xc] >> 7`, not
   `self[0xc] & 0x80` - a reminder that this compiler doesn't normalize
   between the two, so the *exact* bit-test phrasing in the ROM's own
   disassembly (shift vs mask) has to be read off literally, not
   assumed equivalent.
2. **Even with the right phrasing, the shift's *source* and
   *destination* registers can differ** (`ldrb r1, [r5, #0xc]; lsrs
   r0, r1, #7` - byte loads into r1, shifted result lands in r0) where
   a plain unpinned `self[0xc] >> 7` shifts in place (same register for
   source and result). Closed with `register u8 byte asm("r1") = ...;
   register u32 result asm("r0") = byte >> 7;` - two separate pinned
   variables, matching the ROM's own register split.
3. **A local variable that's live across multiple calls needs an
   explicit register pin, or its scope silently widens far past where
   it's actually needed and pulls in extra high registers.** An early
   draft computed `gUnknown_03001308+0x29`'s address once, up front,
   into a plain (unpinned) `u8 *kindAddr` local kept alive across the
   `sub_803AD7C`/`sub_800A0FC` calls purely so it could be reused much
   later in the function - this compiler's allocator responded by
   spilling it (and a second similarly-early-computed value) into
   `r8`/`r9`, needing a `push {r8, r9}`-equivalent prologue the ROM's
   own function (a true `r0`-`r7`-only leaf-register user, `push
   {r4,r5,r6,r7,lr}` only) never has. Fixed by *not* introducing that
   named variable at all - `gUnknown_03001308` is instead re-dereferenced
   fresh, inline, at each of its actual use sites (the `+0x2a` flag
   sets around the `sub_800A0FC` call, the `kind` read, and the final
   kind-reset store), letting this compiler's own local CSE reuse a
   register (`r6`, matching the ROM) only across the short spans where
   the ROM itself does, rather than one artificially-widened lifetime.
   This is the general form of a lesson already documented for
   `sub_800B270` in issue-9's own write-up (pinning a value into a
   *specific* register too early/broadly can itself cause a mismatch,
   not just leaving it unpinned) - here the fix was the opposite
   direction: stop pinning (i.e. stop naming a persistent variable) at
   all, and let the natural per-statement re-evaluation match instead.
4. **A constant reused for two different stores separated by two
   function calls stays in a *low, callee-saved* register (`r4`) the
   whole time**, matching this project's usual "reusable constant"
   idiom - `register u8 zero asm("r4") = 0;` kept in scope across both
   the `self[0x68]`/`self[0x105]` clears *and* the two `bl` calls in
   between, reproducing the ROM's own `movs r4, #0` once, reused by
   every subsequent `strb`/`str` that needs a zero, instead of this
   compiler's default of reloading a fresh `movs r?, #0` per site.

## Left raw (deeper, still-unexamined dependencies) - 4 functions + 1 large block

- **`sub_800AAEC`** (`asm/code_3_2_16.s`, ROM `0x0800AAEC`) - iterates
  `gUnknown_0300130C` (a count-prefixed pointer array), testing each
  entry via `sub_803AD7C` (matched) and, on a hit (return code 3),
  calling `sub_800CD00` (still fully unexamined) with the entry and
  this function's own second argument; a `<= 1` return from that call
  short-circuits the whole loop. Mechanically clear, but `sub_800CD00`
  itself is genuinely unexamined - not attempted further this session.
- **`sub_800AB9C`** (`asm/code_3_2_16.s`, ROM `0x0800AB9C`) - mostly
  built from already-matched/understood pieces
  (`sub_8007C30`/`sub_800014C`/`sub_8008A40`/`sub_8009868`
  [NAKED-parked]/`sub_8008D30`/`sub_80106DC`, all matched or
  understood elsewhere in this codebase now), gated on `self+0x105`/
  `self+0xc` flag bits. The most tractable of the four left-raw
  functions here; a reasonable next target for a future session, not
  attempted this pass purely for time.
- **`sub_800AC2C`** (`asm/code_3_2_16.s`, ROM `0x0800AC2C`, ~950 B) - a
  38-case jump-table player action-state dispatcher (the same shape
  `docs/status/actor.md` already flags as "left raw, out of scope" for
  `sub_8018008`, GitHub issue #22); calls a dozen still-unexamined
  state-transition functions (`sub_8023404`, `sub_8022D50`,
  `sub_8025BAC`, `sub_80231EC`, `sub_80232E4`, `sub_8023224`,
  `sub_8022EA8`, `sub_80241A4`, `sub_802352C`, `sub_8023510`,
  `sub_8028504`, and others). Left raw per this project's established
  policy for this exact dispatcher shape.
- **`sub_800AFF4`** (`asm/code_3_2_16.s`, ROM `0x0800AFF4`, ~636 B) -
  high register-pressure (`sb`/`sl`/`r8`/`ip` all live simultaneously)
  hitbox-record lookup/commit logic referencing the `+0x20`/`+0x2d`
  convention from `docs/rom_map.md`'s physics/collision write-up, but
  gated on `gUnknown_030012C0+0x78` state values and
  `gStaticData_0816A820` (a per-state table not independently
  confirmed). Left raw for the same reason the issue-9 write-up
  originally gave.
- **`sub_800B8DC`** (`asm/code_3_2_17.s`, ROM `0x0800B8DC`, 546 lines)
  - an 18-case jump-table state dispatcher over `self+0x74`, calling
  **18 entirely unexamined helper functions** (`sub_800C074`,
  `sub_800C40C`, `sub_800C314`, `sub_800C244`, `sub_800C18C`,
  `sub_800C1E8`, `sub_800C8F8`, `sub_800C97C`, `sub_800C940`,
  `sub_800C5D4`, `sub_800C8AC`, `sub_800C8BC`, `sub_800C8CC`,
  `sub_800C9C8`, `sub_800BFA8`, plus already-matched
  `sub_803AD7C`/`sub_803AD88`/`PlaySfx`) none of which have any
  existing write-up anywhere in this project. This function sits
  immediately before the already-flagged-out-of-scope physics/
  collision subsystem (`sub_800D040` onward, GitHub issues #12/#13 -
  see `docs/matching/issue-12-physics-collision.md`), and its own
  callees (`sub_800BD48`, `sub_800BFA8`, `sub_800C074`, etc.) are the
  *same* neighborhood - reconstructing `sub_800B8DC` with real
  confidence would require first understanding a dozen-plus completely
  fresh functions this session didn't have room for, each individually
  as large a task as this whole session's other two chunks combined.
  Left entirely untouched rather than guess at a low-confidence
  reconstruction of an 18-case state machine - the next session picking
  this up should expect this to be its own dedicated multi-session
  effort, most likely paired with issue #12/#13's physics/collision
  subsystem it leads directly into.

None of the left-raw functions were force-matched or guessed at; each
is blocked on genuinely unexamined callees, exactly the reasoning the
original issue-9 write-up already gave for this same neighborhood.

## Follow-up: `sub_8026BC0` closed (dedicated deep investigation)

The camera-probe tail above flagged `sub_8026BC0` as "still raw, only
its return value matters". A follow-up dedicated investigation session
closed it, independently confirming both this doc's own reference and
[issue-9-0x08007634-actor.md](./issue-9-0x08007634-actor.md) (line
~214, "the unexamined `sub_8026BC0`" from a jump-table dispatch
context - that second reference turned out to be the same call site
this doc already covers, not a separate one).

`sub_8026BC0` sat right after the already-matched `sub_8026628`
(`docs/matching/issue-9-10-41-0x08026628-game-loop.md`) in
`asm/code_3_2_17_266bc.s`, exactly 56 bytes
(`0x08026BC0`-`0x08026BF8`). Reading its ~14 instructions directly
resolved the whole function:

```c
extern u16 sub_8025460(void *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut);

s32 sub_8026BC0(void *arg0, s32 x, s32 y)
{
    u8 flagsOut = 0;
    s32 hiOut = 0;
    s32 tileX = x >> 3;
    s32 tileY = y >> 3;

    if (tileX < 0)
        tileX = 0;
    if (tileY < 0)
        tileY = 0;

    sub_8025460(*(void **)((u8 *)arg0 + 0x20), tileX, tileY, &flagsOut, &hiOut);

    return flagsOut;
}
```

It's a thin wrapper around the already-matched terrain-tile-cache
lookup `sub_8025460` (`src/system/game_loop4.c`, GitHub issue #40):
`arg0+0x20` is `gUnknown_03001308`'s own tile-cache-pointer field (the
same global whose `+0x29`/`+0x2a` fields this doc's own leading block
already established), `x`/`y` get divided by 8 and clamped to a
non-negative minimum independently per axis (not a single combined
clamp - each axis can hit zero on its own), and the two out-parameters
`sub_8025460` writes through (`flagsOut`, `hiOut`) are both zero-
initialized locals. Only `flagsOut` - which `sub_8025460` also returns
directly as its own `u16` return value - is returned here; `hiOut`
(the decoded cell's top nibble) is written but never read back,
exactly the "discarded outValue" idiom `sub_8026628`'s own
`sub_8026AE8`/`sub_8026A18` calls already established right next door
in this same neighborhood.

This confirms the caller-side reading above: the "camera-probe" in
`sub_800A884`'s tail passes an already-pixel-unit `{x, y}` (de-Q8'd via
`>>8`, same convention as `sub_8026628`'s own `pos`), and
`sub_8026BC0` itself does the pixel-to-tile-cache-lookup-unit
conversion, so the `code == 6` test in the caller really is just
"did `sub_8025460` report terrain type 6 at this tile" - an opaque
enum comparison, not a geometric hit-test of its own.

Matched as real C on the **first isolated-compile attempt** - no
register pins, opaque `asm volatile`, or statement-order juggling
needed, following `sub_8026628`'s own "matched on the first attempt"
precedent right next door in the same file. Confirmed byte-identical
to the ROM's own instructions (register for register, operand for
operand) via the isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp`
pipeline against `baserom.gba`'s raw bytes at `0x08026BC0`-`0x08026BF8`
(the only difference being the `bl sub_8025460` relocation site, which
resolves correctly once linked), plus a full clean `rm -rf build &&
make NON_MATCHING=1 report` (no warnings) and `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`crashbandicootxs.gba: La suma coincide`). Already flush to
a 4-byte boundary (56 bytes total) with no trailing `.align 2, 0` gap
in the ROM, unlike `sub_8026628`'s own end-of-function padding quirk.

### Build layout

`asm/code_3_2_17_266bc.s` is trimmed to end right after `sub_8026AE8`'s
own trailing `.align 2, 0` (685 lines, matching the same "everything
before, everything after" split `sub_8026628` itself used to get
carved out of this same file). `src/system/game_loop44.c` (new file)
holds the matched `sub_8026BC0`. The remainder - `sub_8026BF8` onward,
still raw/unexamined this session (including `sub_8026C90`,
`sub_8026D8C`, `sub_8026DFC`, `sub_8026E6C` and others referencing
`gUnknown_03001308` and per-object velocity-style fields) - moved
unchanged to the new `asm/code_3_2_17_26bf8.s`, inserted between the
two in `ldscript.txt`:

```
asm/code_3_2_17_266bc.o(.text);
src/system/game_loop44.o(.text);
asm/code_3_2_17_26bf8.o(.text);
```

`sub_800A884` itself (this doc's own primary subject) is unaffected -
it still calls `sub_8026BC0` exactly as before; only the callee's own
body moved from opaque raw bytes to matched, documented C.

## Cross-references

- `docs/status/actor.md` / `docs/status/graphics.md` - matched/parked
  lists updated for this session's two functions.
- `tools/report_units.py` - `0x0800A884` split into its own matched-
  reconstruction unit (`actor_part78.o`, `NON_MATCHING`) plus a new
  `0x0800AAEC` raw entry for the remainder; `0x0800B3F0` now points at
  `actor_part77.o`.
- `docs/matching/issue-9-0x08007634-actor.md` - the original write-up
  for this whole neighborhood's prior pass, including the
  `sub_80083B8`/`sub_80084C4` keyframe-lookup convention this session
  reused directly.
- `docs/matching/issue-12-physics-collision.md` - the physics/collision
  subsystem `sub_800B8DC` leads into, already flagged out of scope for
  the same reasons.

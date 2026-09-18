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

  **Not yet byte-exact.** An isolated compile of the leading ~40
  instructions (through the `sub_800A0FC` call) was iterated to an
  exact register-for-register match with the ROM - see "Real gotchas"
  below. The rest of the function (the 10-way "kind" dispatch and the
  keyframe-lookup/camera-probe tail) compiles and links correctly but
  hasn't been through the same register-pin iteration pass; the next
  session picking this up should start by isolating that tail with the
  same `arm-none-eabi-cpp`+`agbcc` diagnostic loop `docs/workflow.md`
  describes. Real bytes stay in the new `asm/code_3_2_16_a884.s`
  (`asm/code_3_2_16.o` trimmed to start at `sub_800AAEC`), following
  the same `.if NON_MATCHING == 0` pattern as `sub_800A528`'s own
  `asm/code_3_2_11_a528.s`.

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

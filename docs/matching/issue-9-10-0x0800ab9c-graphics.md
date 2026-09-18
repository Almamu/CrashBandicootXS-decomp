# Issues #9/#10: `sub_800AB9C` follow-up (graphics)

This is the write-up for a follow-up session against the two raw
regions `tools/report_units.py` still flagged in the graphics category
as of
[issue-9-10-0x0800a884-graphics.md](./issue-9-10-0x0800a884-graphics.md):

- `0x0800AAEC`-`0x0800AFF4` (that write-up's own "most tractable next
  target", `sub_800AB9C`, plus its still-raw neighbors)
- `0x0800B8DC` onward (a 546+-line, 18-case jump-table state
  dispatcher and beyond)

## Parked (`NON_MATCHING`, not yet byte-exact) - 1 function

- **`sub_800AB9C`** (`src/graphics/actor_part81.c`) - a two-flag-gated
  teardown/notification step on `self`, the same still-unnamed "big
  object" (at least 0x108 bytes) `actor_part15.c`/`actor_part77.c`
  already work on. Guarded by `self+0x105` (a "torn down already"
  latch, read once into `wasCleared` and confirmed 0 on every path
  that reaches either branch below - both branches reuse that
  already-loaded 0 for their own writes rather than reloading a fresh
  constant, matching the ROM's own `r6` reuse).

  `self+0xc` bit 1: relocates `self`'s primary AABB (`sub_8007C30`,
  `actor_part9.c`'s own copy of the same helper) onto a second stack
  slot (`sub_800014C`, a plain `memcpy`) before unpacking it back out
  into scalars for `sub_8008A40` (already NAKED-parked, `actor_part7.c`)
  - the exact "relocate then unpack" idiom `sub_8008A40`'s own doc
  comment already documents from its callers' side, done here
  explicitly in the caller instead of inline in the callee.

  `self+0xc` bit 7: clears `self+0x108`/`self+0x10c` (the same fields
  `sub_8010E2C` clears elsewhere in this object family, just written
  directly here) and fires three teardown/notification calls:
  `sub_8009868` (NAKED-parked, `actor_part11d.c`) against
  `gUnknown_0300130C`'s manager with selector `3`, `sub_8008D30`
  (`actor_part10.c`) against `gUnknown_030012EC`'s manager with
  selector `4`, and `sub_80106DC` (`game_loop23.c`) with no arguments.
  `sub_8009868` was previously declared with only one parameter
  (`manager`) since its only known call site at the time never
  exercised a second argument - this call site is the first one that
  does (`movs r1, #3` loaded but never read inside `sub_8009868`'s own
  NAKED body), so the extern here is widened to
  `void sub_8009868(void *manager, s32 arg1)` to reproduce that dead
  argument load; `sub_8009868`'s own definition is untouched (a dead
  incoming `r1` doesn't change its behavior or bytes).

  Matched everything except one gap:

  1. **Both bit tests** needed the established "byte loads into r1,
     shifted result lands in r0" idiom (`sub_800A884`'s own gotcha,
     `actor_part78.c`) - `register u8 flagByte asm("r1") = self[0xc];
     register u32 bitN asm("r0") = flagByte >> N;`. For the bit-1 test,
     the subsequent `& 1` also needed forcing into `r0` explicitly
     (`register u32 result asm("r0") = bit1 & mask;`) - left as a plain
     `bit1 & 1` expression, this compiler put the AND's result in `r1`
     (the mask register) instead of `r0` (matching the ROM's `ands r0,
     r1`, not `ands r1, r0`).
  2. **The `self+0x108`/`self+0x10c` clear** needed its shared base
     address pinned to `r0` (`register u8 *clearedFields asm("r0") =
     self + 0x108;`) - left as a plain local, this compiler reused
     `self`'s own `r5` in place for the address (since `self` isn't
     read again afterward) instead of computing a fresh `r0`, matching
     the ROM's `adds r0, r5, r1` (a new register, `self` left intact).
  3. **The leading `sub_8007C30`/`sub_800014C` relocate-then-unpack
     pair** needed the relocated box read back via a raw `u8
     boxCopy[0x10]` buffer and pointer casts (`*(s32 *)(boxCopy + N)`),
     not a `struct aabb boxCopy` with `.field_N` member access. A
     named-struct local let this compiler materialize `&boxCopy` once
     (in `r4`) and reuse that register for every later field read -
     even `volatile`-qualifying the whole struct didn't stop it, since
     `volatile` only forces a fresh *load*, not a fresh *address*
     computation, and the address is a pure `sp`-relative constant the
     compiler is free to cache across the whole function regardless.
     The ROM instead re-issues four independent `ldr rX, [sp, #imm]`
     instructions with no cached base at all. The raw-array/pointer-
     cast form closed this on its own (no address is ever taken, so
     there's nothing to cache), and also fixed a related register-8
     spill: with the struct form, `manager` (which needs `r4`) and the
     cached `&boxCopy` (which also wanted `r4`) collided, pushing
     `manager` into `r8` and forcing an unwanted `push {r8}`/`mov r8,
     r3` prologue/epilogue pair the ROM's true `r0`-`r7`-only leaf
     usage never has.

  **The one gap that resisted every technique tried:** the ROM
  evaluates `sub_8008A40`'s 7 arguments in the order `unused`
  (`self[0x24]`, stack slot 2, `[sp,#4]`), `compareViewport` (`self`,
  stack slot 3, `[sp,#8]`), `boxH` (`boxCopy.field_c`, stack slot 1,
  `[sp,#0]`), `boxX`/`boxY`/`boxW` (`r1`/`r2`/`r3`), `manager` (`r0`,
  deferred last since `r0` is reused as scratch throughout every other
  argument's computation) - each stack-bound value's compute is
  immediately followed by its own `str` to the outgoing-argument slot,
  interleaved rather than batched:

  ```
  adds r0, r5, #0      @ unused = self[0x24] ...
  adds r0, #0x24
  ldrb r0, [r0]
  str  r0, [sp, #4]    @ ... stored immediately
  str  r5, [sp, #8]    @ compareViewport = self, stored immediately
  ldr  r0, [sp, #0x28] @ boxH = boxCopy.field_c ...
  str  r0, [sp]        @ ... stored immediately
  ldr  r1, [sp, #0x1c] @ boxX/boxY/boxW straight into r1-r3
  ldr  r2, [sp, #0x20]
  ldr  r3, [sp, #0x24]
  adds r0, r4, #0      @ manager, last
  bl   sub_8008A40
  ```

  Every plain-C phrasing tried instead has this compiler batch *all*
  outgoing-stack-argument stores together immediately before the `bl`,
  after first computing every value (register-bound or not):

  - Inlining all 7 expressions directly in the call: evaluates in
    strict left-to-right declaration order (2,3,4,5,6,7), deferring
    only the `r0`-bound `manager` to last - register args first, stack
    args (in declaration order) second, `manager` third.
  - Precomputing `unusedArg`/`viewport`/`boxH` as named locals, in ROM
    order, as separate statements before the call: changed the
    *compute* order to roughly match (unused's `ldrb` chain first, then
    the register loads, then `boxH`'s `ldr`), but the *stores* to the
    outgoing stack slots still land as one group right before the
    `bl`, not interleaved with their own computation.
  - A `asm volatile("" ::: "memory")` compiler barrier between each
    precomputed argument's statement: no effect on the store grouping
    at all (this compiler's scheduler doesn't appear to treat a bare
    memory clobber as a real ordering barrier for this class of code
    motion).
  - A fully inline-asm call sequence reproducing the ROM's instructions
    verbatim: works in isolation, but the moment the *real* C call to
    `sub_8008A40` is removed, this compiler no longer sees a
    stack-argument call anywhere in the function and shrinks the
    outgoing-argument reservation accordingly, silently shifting every
    `sp`-relative local (`box`/`boxCopy`) by the same amount the
    hand-written asm's hardcoded offsets don't account for - a correct
    fix would mean manually managing the whole frame's `sp` budget for
    this one call, which is effectively rewriting the surrounding
    locals' addressing by hand too. Not attempted further given the
    fragility for a single call's instruction-scheduling gap.

  This is the same class of gap this exact source file already
  documents as unclosable for `sub_8008AD8`/`sub_8008D80`
  (`actor_part7.c`, right next to `sub_8008A40` itself): "this compiler
  has no way to express 'this scalar parameter is already sitting in
  the right stack position for the callee I'm about to build a struct
  pointer into'" - and the same class `PlaySfx`'s own doc comment
  (issue #3) describes: "gcc 2.9's own instruction scheduler picks a
  fixed policy for *when* to materialize a cached register that doesn't
  appear to be steerable from C source at all." Given a real, working
  precedent for accepting this exact gap (`PlaySfx` itself stays parked
  on it), and every C-level technique in this project's own toolbox
  already tried, `sub_800AB9C` is parked under `NON_MATCHING` rather
  than force-matched with a fragile hand-managed-stack asm splice.

  Real bytes stay in the new `asm/code_3_2_16_ab9c.s`. `asm/
  code_3_2_16.s` now ends right after `sub_800AAEC` (trimmed from its
  previous end at `sub_800AFF4`), and the remainder -
  `sub_800AC2C`/`sub_800AFF4` - moved to the new
  `asm/code_3_2_16_ac2c.s`, keeping the same three-way "before /
  parked function / after" split this ROM region's earlier passes
  (`sub_800A884`, `sub_800B270`) already established.

## Left raw (deeper, still-unexamined dependencies) - 3 functions + 1 large block

- **`sub_800AAEC`** (`asm/code_3_2_16.s`, ROM `0x0800AAEC`) - unchanged
  from the prior session's write-up: iterates `gUnknown_0300130C`
  (a count-prefixed pointer array), testing each entry via
  `sub_803AD7C` (matched) and, on a hit (return code 3), calling
  `sub_800CD00` (still fully unexamined) with the entry and this
  function's own second argument. Mechanically clear, but
  `sub_800CD00` itself wasn't examined this session either.
- **`sub_800AC2C`** (`asm/code_3_2_16_ac2c.s`, ROM `0x0800AC2C`,
  ~950 B) - unchanged: a 38-case jump-table player action-state
  dispatcher, calling a dozen still-unexamined state-transition
  functions. Left raw per this project's established policy for this
  exact dispatcher shape (same as `sub_8018008`, issue #22).
- **`sub_800AFF4`** (`asm/code_3_2_16_ac2c.s`, ROM `0x0800AFF4`,
  ~636 B) - unchanged: high register-pressure (`sb`/`sl`/`r8`/`ip` all
  live simultaneously) hitbox-record lookup/commit logic gated on
  `gUnknown_030012C0+0x78` state values and an unconfirmed per-state
  table. Left raw for the same reason the issue-9 write-up originally
  gave.
- **`sub_800B8DC`** (`asm/code_3_2_17.s`, ROM `0x0800B8DC`, 546 lines)
  - not attempted this session either; still the 18-case jump-table
  state dispatcher over 18 entirely unexamined helper functions the
  prior write-up already described in full. Confirmed unchanged: still
  sits directly before the already-flagged-out-of-scope physics/
  collision subsystem (issues #12/#13). This remains its own
  dedicated multi-session effort - not attempted here, since this
  session's time went entirely into closing `sub_800AB9C`'s gap
  instead (which itself only got as far as `NON_MATCHING`, not a full
  match).

## Cross-references

- `docs/status/actor.md` - `sub_800AB9C`'s `NON_MATCHING` entry added
  next to `sub_800A884`'s own; the stale combined "`sub_800A884`/
  `sub_800AAEC`/`sub_800AB9C`/`sub_800AC2C`/`sub_800AFF4`" left-raw
  bullet split into `sub_800AAEC` (its own bullet) and
  `sub_800AC2C`/`sub_800AFF4` (still combined, both blocked on the
  same class of unexamined state-transition callees).
- `tools/report_units.py` - `0x0800AAEC` narrowed to just
  `sub_800AAEC`; new `0x0800AB9C` entry pointing at
  `actor_part81.o` (`NON_MATCHING`); new `0x0800AC2C` entry for the
  remaining raw tail.
- `docs/matching/issue-9-10-0x0800a884-graphics.md` - this session's
  starting point, including the exact function this write-up closes
  (as far as it could be closed) and the `sub_80083B8`/`sub_80084C4`
  keyframe-lookup convention referenced above.
- `src/graphics/actor_part7.c` - `sub_8008A40`'s own doc comment
  (the "relocate then unpack" idiom this function's first branch
  mirrors) and `sub_8008AD8`/`sub_8008D80`'s doc comment (the
  precedent for this exact "argument already in the right stack
  position" unclosable-gap class).
- `docs/matching/issue-3-overlay-ui-audio-wrapper.md` - `PlaySfx`'s
  own precedent for the same "scheduler policy, not steerable from C
  source" gap class, cited above.

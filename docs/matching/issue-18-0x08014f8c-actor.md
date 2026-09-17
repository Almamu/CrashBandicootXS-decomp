# Issue #18: 0x08014F8C-0x08015840 (25 functions)

Category label was `graphics`, but this chunk turned out to be the same
"self" action-table object family already established in
`actor_part18.c`/`actor_part18b.c` (self+0xc a per-category
`{s16 offset; void *fn}` table fed through the `sub_803AD80`/
`sub_803AD84` trampolines together with `self+0x10`, a `struct actor *`
sub-object; self+0x27-0x32 a shared state/flag/table-index trio) - filed
under `docs/status/actor.md`, not `graphics.md`, matching the note in
`docs/status/README.md` that several graphics-labeled chunks are
actually `actor`. Two of this chunk's own functions
(`sub_8015508`/`sub_8015780`) are called directly by `actor_part18.c`'s
`sub_801426C`/`sub_80142B0`, confirming the same object family carries
straight through.

`gUnknown_030012F0` (only touched by `sub_8014F8C` here) is a small list
object - `+4` a count, `+0xc` a `struct actor **` array - not referenced
by any other already-matched code, so it stays raw-offset rather than a
guessed struct.

## New files

The chunk's matched/parked functions split into four new `.c` files
(`actor_part38.c`/`38b.c`/`38c.c`/`38d.c` - numbered `38` rather than
`28`, which would have matched their creation order more naturally,
since issue #62's parallel PR independently claimed `actor_part28.c`
first before this PR merged), each ending where a parked
function's raw bytes sit between it and the next matched run - the same
"widen past the parked function's real end" convention as
`actor_part18.c`. Four new raw `.s` splits carry the parked functions'
real bytes: `asm/code_3_2_17_15038.s`, `code_3_2_17_15238.s` (covers
both `sub_8015238` and `sub_80152F0`, since both ended up parked
back-to-back), `code_3_2_17_156ec.s`, `code_3_2_17_157c4.s`. The
original `asm/code_3_2_17_14674.s` is truncated to end right before
`sub_8014F8C` (0x08014F8C, unchanged name/start address), and a new
`asm/code_3_2_17_15840.s` picks up the still-raw remainder from
`sub_8015840` onward (unexamined, out of scope for this pass).

## Matched (21/25)

- **`sub_8014F8C`**: scans the `gUnknown_030012F0` list of
  `struct actor *`; skips entries whose `+0x48` trampoline
  (`sub_803AD7C`) reports a width of 4 or less, entries further than
  0x40 (Manhattan distance) from `self`'s own part, entries without
  their `+0xc` bit 6 flag set, and entries more than 0x11 away
  vertically - then fires the `+0x68` trampoline pair via
  `sub_803AD88` with action 0x16 on whatever survives. Needed two real
  compiler-codegen fights:
  - **Anti-CSE across a loop back-edge**: the ROM reloads
    `gUnknown_030012F0` completely fresh (both the literal-pool address
    *and* the deref) at both the loop condition and inside the body,
    sharing a single pool word (`_08015034`) between the two `ldr`
    sites. A plain C re-read in the body let gcc reuse the condition's
    already-loaded register value across the back edge instead of
    reloading - worse than the usual "reused value" CSE cases
    documented elsewhere, since here the ROM's compiler didn't even
    reuse the *address-of-symbol* computation, only the shared pool
    *data*. Fixed by restructuring the loop with explicit `goto`s (so
    condition and body are separate, hand-written statements) and
    routing both re-reads through the same hand-placed local literal
    pool (`.Lgu12f0_8014f8c`, emitted once via a trailing
    `asm(".align 2, 0\n\t.Lgu12f0_8014f8c: .word gUnknown_030012F0")`
    after the function) via a real two-instruction
    `ldr %0, .Lgu12f0_8014f8c` / `ldr %0, [%0]` inline-asm block at
    each site - this is opaque to gcc's CSE and reuses the *same*
    literal-pool word (no extra `.word` entries), matching the ROM's
    exact layout.
  - **Branchless-abs register roles**: the Manhattan-distance sum uses
    the classic `sign = x >> 31; x = (x ^ sign) - sign;` idiom twice.
    Needed explicit `register s32 dx asm("r1")`/`dy asm("r2")` pins
    (matching the ROM's own choice to keep `dx` live in `r1` the whole
    time and only use `r0` as scratch for the sign) plus a
    `register s32 sign asm("r0")` shared across both abs computations,
    and a `register u8 flagsVal asm("r1")`/`register s32 bit
    asm("r0")`/`register s32 one asm("r1")` triple for the `other[0xc]`
    bit-6 test - each pin was needed to stop gcc's own register
    allocator from picking a different (but logically equivalent) home
    for the same value.
- **`sub_80151C8`**: one-shot guard (`self+0x23`); the first time
  through, a 5-entry jump table on `self+0x22` (`[0,1]`→0x18, `2`→0x19,
  `[3,4]`→0x1a) into `self+0x28`; always sets `part+0xd` bit 0 and
  clears `self+0x34`. The jump table itself needed a full hand-written
  raw-asm block (see "Jump-table layout" below) - once that was in
  place, one more small gap remained: the trailing `part[0xd] |= 1`
  store needed explicit `register u8 *part asm("r1")`/
  `register s32 one asm("r0")`/`register u8 old asm("r2")` pins to get
  the ROM's own register roles for the `orrs`/`strb` pair (a plain C
  `|=` picked the opposite operand-to-register assignment).
- **`sub_8015350`**: clears `self+0x33`, saves `self+8`'s old value
  (truncated) into `self+0x2d`, overwrites `self+8`, clears
  `self+0x2c`/`0x2b`; if the new value isn't 0xd/0xe, also clears
  `part+0x90` and the player's `+0x92`/`+0x94` (written twice - the ROM
  really does re-derive the player pointer and store the same byte a
  second time, via `*(u8 * volatile *)&gUnknown_030012D8` at each of
  the three sites to stop the redundant derefs being merged). Needed
  the `self[0x33] = 0;` store moved *before* the `self+8` read (source
  order alone wasn't followed by the compiler's own statement-order
  scheduling here, since it originally emitted the read first
  regardless - moving the store to a separate, earlier statement fixed
  it).
- **`sub_8015398`**/**`sub_80153FC`**: same shape (fixed cue, reset
  `self+0x18`/`0x1c`, mgr trampoline pair, clear `self+0x20`-`0x24`) -
  differ only in action codes and whether the field-clear runs before
  or after the trampoline pair.
- **`sub_8015460`**: two near-identical arms keyed on `self+0x29`, both
  firing the mgr trampoline pair and setting the state/counter trio,
  then latching `self+0x31` again if `part+0x100` is set. Needed the
  shared `part` local *removed* (each of the three `self+0x10` derefs
  reads fresh, matching the ROM reloading it three separate times
  rather than hoisting one load out of the branch), and the `0x1b`/`1`
  table-index constant materialized *before* the `self+0x31`/`0x2f`
  field-pointer computations (`u8 val = 0x1b; p31 = self + 0x31; ...`)
  to match the ROM's own "load constant early, use late" pattern - the
  natural statement order (compute pointers, store, *then* load the
  constant right before its one use) compiled to a different
  instruction order/byte sequence even though the two are
  data-independent.
- **`sub_8015508`**/**`sub_8015558`**: same shape (mgr trampoline pair
  with actions 0xb/0xb, reset `self+0x18`/state trio, clear
  `part+0x68`) - differ only in the final table-index constant (0xb vs
  7). Same "materialize the table-index constant early" pattern as
  `sub_8015460` - it needed to be assigned right after the `self+0x18`
  reset (not lazily at its one use site) via a `register u8 idx
  asm("r2")` pin declared early and only *initialized* at the right
  point, plus an `asm volatile("" : "+r"(p28))` anti-fold barrier on
  the `self+0x28` pointer to stop the final `+6`-style offset folding
  into the `strb`'s own addressing mode.
- **`sub_80155A8`**: single-instruction store, `self+0x10 = val`.
- **`sub_80155AC`**: trivial tail-call to `sub_8014B54`.
- **`sub_80155B8`**/**`sub_8015650`**: byte-identical ROM encoding at
  two different addresses (no shared caller) - `part+0x38`-gated mgr
  trampoline pair, clear `self+0x18`/`0x1c`. Needed a
  `register s32 zero asm("r4")` pin (matching the ROM's persistent
  `r4`=0 register, avoiding the plain-C version materializing 0 lazily
  at each use with a fresh `movs r0,#0`).
- **`sub_80155F8`**: bumps `self+0x18`; once it reaches `self+0x1c` (or
  `part+0x38` is already set), resets via the same mgr trampoline
  pair/state-clear as `sub_80155B8`, then tail-calls `sub_80122CC`.
  Needed the `self+0x26` pointer computed *before* the `zero` register
  materializes (opposite of the natural declaration order) to match
  the ROM's `adds r1,r5,#0x26` / `movs r4,#0` / `strb` sequence.
- **`sub_8015690`**: `part+0x38`-gated; sets the player's `+0xc` bit
  0x80 and tail-calls `sub_80241A4`. Needed
  `register u8 *player asm("r1")`/`register s32 bit asm("r0")`/
  `register u8 old asm("r2")` pins for the `orrs`/`strb` pair, same
  reason as `sub_80151C8`'s trailing `part[0xd] |= 1`.
- **`sub_80156B4`**: `part+0x38`-gated mgr trampoline pair with actions
  0x11/4 - matched with no register pins needed.
- **`nullsub_17`**/**`nullsub_18`**: empty stubs.
- **`sub_8015750`**: while `self+0x29` is clear, tail-calls
  `sub_8015460` first; always tail-calls `sub_8012FBC` after. Needed
  `self` pinned to `register u8 *self asm("r4")` (the ROM keeps it in
  `r4` for the whole function; a plain local picked a two-instruction
  double-copy through an intermediate register instead of the ROM's
  single `adds r4,r0,#0`).
- **`sub_8015774`**: trivial tail-call to `sub_8012D24`.
- **`sub_8015780`**: fires the mgr trampoline pair with `a`/`b` as the
  two action arguments (note: `a` is a real, *used* parameter here,
  passed straight through as `sub_803AD80`'s action index - not the
  "unused" parameter it looked like from `actor_part18.c`'s call
  sites alone), then conditionally latches `self+0x18`/`self+0x1c`
  from `c`/`d` unless either equals the `0x7FFFFFFF` sentinel. Matched
  with no register pins needed.

## Jump-table layout: `sub_80151C8`

The `switch (self[0x22]) { case 0/1: 0x18; case 2: 0x19; case 3/4:
0x1a; }` dispatch compiles to a real jump table either way, but this
compiler's own block-layout order for the three targets (`0x18`,
`0x19`, `0x1a`) never matched the ROM's `0x18`→`0x19`→`0x1a` physical
order, no matter how the case labels were scattered in source (tried
five different orderings - the resulting table's *value* mapping was
always semantically correct, but the physical block order came out
different, and seemingly unrelated to source order, each time). A
C-level computed-goto table (`goto *table[idx]` with a `static const
void *table[5] = {&&case18, ...}`) hit the same table-order problem
*and* pulled the `static const` array into a discarded `.data` section
this ROM build has no room for. Fixed by hand-writing the whole
dispatch (bounds check, table, and the three target blocks) as one
`asm volatile` block, with `self` pinned to `r3` for the whole function
so the block's hardcoded `r3` references match wherever the compiler
already keeps it, and plain (non-`.L`-prefixed but otherwise ordinary)
local labels for the table/targets since GAS's implicit literal-pool
placement for a raw `ldr r1, =symbol` pseudo-op inside a hand-written
block doesn't reuse the compiler's own per-function pool management (it
tried to defer to end-of-file, out of pc-relative range this deep into
a 25-function file, when first tried without explicit labels).

## Parked (5/25)

See `docs/status/actor.md`'s "Parked" section for the one-line summary
of each: `sub_8015038`, `sub_8015238`, `sub_80152F0`, `sub_80156EC`,
`sub_80157C4` - 20 matched, 5 parked, out of 25 total.
`sub_8015238`/`sub_80152F0` share one raw file (`code_3_2_17_15238.s`)
since both ended up parked back-to-back, but each is still counted as
its own parked function.

Every parked function's semantics are fully understood; the residual
gaps documented above (and in `docs/status/actor.md`) are all
compiler-register-allocation or instruction-scheduling gaps, not logic
gaps. None of `Closes #18` applies here since 5 functions remain
parked - the issue stays open for whoever picks up the remaining 5.

## Full-build address-shift lessons

Several of the fixes above (`sub_8015460`'s early constant, both
`sub_8015508`/`sub_8015558`'s table-index pin, `sub_80155F8`'s
statement reorder) were only caught by the full clean `make compare`
cycle, not by isolated per-function compiles - the isolated compiles
for these all looked instruction-for-instruction correct in ROM address
order, but a full rebuild shifted a downstream function's address by
exactly the byte delta of the missed instruction reordering, exposing
it via `arm-none-eabi-nm`'s address table before even diffing bytes.
This is the same "isolated compile is a diagnostic tool, never proof"
warning `docs/workflow.md` calls out - it held true again here.

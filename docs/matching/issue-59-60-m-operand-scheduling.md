# Issue #59/#60 follow-up: `m`-operand scheduling fix on 2 NAKED functions (actor_part129.c)

External-contributor PR #415 introduced a narrower matching technique
this project hadn't used before: a bare `"m"` (memory) operand
constraint in inline asm, used to force instruction-scheduling *order*
around a memory access while still letting the compiler own that
access's own address computation and register allocation (see
`src/system/tile_slot_pool.c`'s `sub_80264F8` for the original example
and comment). This session tried adapting that same idea - "isolate
just the two instructions this compiler's scheduler puts in the wrong
relative order, and pin only their order via a narrow inline-asm
snippet, everything else stays plain C" - against the two remaining
`InitActorPart`-family NAKED functions in
`src/graphics/actor_part129.c` flagged as candidates in
[issue-59-60-gap-31a6c-part1.md](issue-59-60-gap-31a6c-part1.md):
`sub_8032440` (closed) and `sub_80321FC` (still NAKED, but with two of
its three real gaps closed and the third now precisely characterized).

## `sub_8032440` - matched

`InitActorPart`-based constructor forcing a fixed `0xFFFF0600` bias for
its own 4th argument, forwarding `d` untouched and stashing the
caller's real `c` into `self+0x5c`. The documented blocker was a
STORE-then-constant-load ordering issue (not the compare-load ordering
technique 3 was demonstrated on): this compiler's call-argument
evaluation always computes the `0xFFFF0600` constant right next to `d`'s
own stack load (both "free, no-dependency" values get scheduled as
early as possible), while the ROM's own build computes `self`-into-r0
and the constant right before the call, *after* `d`'s own outgoing-stack
store.

Every attempt to fix this by choosing a different *value* for the
constant - a plain literal, an inline-asm-computed value assigned to a
separate statement, a GNU statement-expression embedded directly in the
call, a fake data dependency on `self` or `d` - reproduced the exact
same early placement, confirming this is genuine post-expansion
instruction scheduling (not a front-end "constants get evaluated
first" quirk): an opaque asm block with no real dependency gets
scheduled exactly like a plain constant. The only two real ordering
levers this compiler respects are (1) a hard register **write-after-read
hazard** (a value can't be computed into a register that's still holding
an earlier live value) and (2) manually writing the instructions
yourself, since an asm block's own internal order is never touched.

The fix that worked: replace the *entire* call-setup tail (`d`'s
outgoing-stack store, `self`-into-r0, the constant, and the `bl` itself)
with one narrow, ordered inline-asm block:

```c
void *sub_8032440(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 aReg asm("r1") = a;
    register s32 bReg asm("r2") = b;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 2;
    s32 outSlot;

    asm volatile(
        "str %1, %0\n\t"
        "add r0, %2, #0\n\t"
        "ldr r3, 1f\n\t"
        "bl InitActorPart\n\t"
        : "=m"(outSlot)
        : "r"(dReg), "l"(self), "r"(aReg), "r"(bReg)
        : "r0", "r3", "r12", "lr", "memory", "cc");

    *(s32 *)(self + 0x54) = health;
    asm("ldr r0, 2f\n\tstr r0, [%0, #0x50]" : : "l"(self) : "r0", "memory");
    *(s32 *)(self + 0x5c) = c;
    self[0x58] = 0;

    return self;
}
asm(".align 2, 0\n1: .4byte 0xFFFF0600\n2: .4byte gStaticData_087E53CC\n");
```

(see the real, commented version in `src/graphics/actor_part129.c` for
the exact final shape, including `d`'s own outgoing-argument slot as a
plain `"m"` local - letting the compiler still own its address/frame
allocation - and `a`/`b` passed through r1/r2 via plain `"r"` operands
rather than touched at all). Two sub-problems inside this fix, both
needed independently:

- **Order**: writing `self`-into-r0 and the constant as literal
  instructions inside one asm block, placed exactly where the call used
  to be, pins their position relative to `d`'s own store (which the
  compiler still generates automatically, right where the block sits)
  - matching the ROM's exact instruction sequence.
- **Literal pool placement**: inline asm's own `ldr r3, =0xFFFF0600`
  pseudo-op dumps its literal in the assembler's default pool location
  (found `0x652` bytes away, near an unrelated later function) instead
  of immediately after the function like this compiler's own
  `-fhex-asm` literals. The fix was a hand-written local-label pair
  (`1:`/`2:`) with a **trailing file-scope `asm(...)` right after the
  function's closing brace** holding both this function's own
  `0xFFFF0600` literal *and* a hand-written `gStaticData_087E53CC` load
  (replacing that store's own plain C, so both literals land in the
  right relative order) - a plain C statement placed after `return
  self;` *inside* the function gets pruned by `-O2`'s dead-code
  elimination instead of landing where written, so it has to be a
  genuine separate top-level statement, not dead code within the
  function.

Verified via the full clean pipeline: `rm -rf build && make
NON_MATCHING=1 report` (no new warnings), then `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` → `crashbandicootxs.gba: OK`.

## `sub_80321FC` - still NAKED, but narrowed to one precise gap

Parameterized `sub_802E4B8`-based constructor (the "kind" is a 6th
caller-supplied byte argument, unlike `sub_8031F78`/`sub_8032054`/
`sub_80320C4`'s fixed literals). Originally documented with two gaps;
both are now individually closeable:

- **`c`-register re-materialization**: a dual register pin -
  `register s32 cCall asm("r3") = c;` kept untouched and passed directly
  to `InitActorPart`, alongside a separate `register s32 cSaved
  asm("r6") = c;` for the later `self+0x64` store and second call's
  argument - reproduces the ROM's exact "leave r3 alone for the call,
  use a second copy in r6 for everything after" split. The later bias
  add (`cSaved += (s32)0xFFFFC24A;`, an in-place update rather than a
  fresh expression) also matches the ROM's own `add r6, r6, r0` reusing
  its permanent home register, the same idiom documented for
  `sub_8032274`/others in
  [issue-59-60-gap-31a6c-part1.md](issue-59-60-gap-31a6c-part1.md).
- **"kind" byte truncation timing**: a plain `asm volatile` barrier on a
  `u8`-typed register variable (the originally-tried fix) doesn't force
  a physical mask instruction - this compiler's abstract model treats
  the register as "logically already narrow" and only emits the
  `lsl`/`lsr` at the point the value gets *widened* for use (i.e. right
  before the `sub_802E4B8` call, matching the originally-documented
  "defers to point of use" behavior). Forcing a **real, explicit
  `lsl`/`lsr` by 24 via a narrow inline-asm snippet** on an otherwise
  plain `s32` (not relying on the C `u8` type's implicit conversion at
  all) reproduces the eager truncation right after `kind`'s own stack
  load.

A **third, previously-undocumented gap** blocks a full match even with
both of the above fixed: this compiler's own prologue-adjacent
parameter-register-save sequence for `b`/`c`/`d` (all three need a
callee-saved home across the first `InitActorPart` call) always comes
out ordered by **ascending destination register number** - `c` (r6),
then `d` (r7), then `b` (r9) - the opposite of the ROM's own order (`b`
saved first, immediately after `self`, then `c`, then `d` last). This
was tested extensively:

- Per-variable `asm volatile("" : "+r"(bReg))` barriers, C-level
  statement reordering, and declaring-without-initializing-then-
  assigning-later all failed to change `b`'s relative position - it's
  not a source-order effect, it's this compiler's own internal
  register-save ordering (confirmed independent of source order, same
  as `sub_8032440`'s constant-hoisting gap above).
- Bundling `b`+`c`'s saves into one ordered inline-asm block **does**
  fix their relative order against each other (`mov r9, %4` /
  `add r6, %5, #0` in one atomic block, matching the ROM's `b`-then-`c`
  sequence) - but `d`'s load, left as a plain unpinned local outside
  that block (deliberately - see below), still always floats to
  *before* the whole block, never after, regardless of where the plain
  `dVal = d;` statement is written in the C source. This matches this
  session's broader finding (see `sub_8032440` above) that independent
  stack loads with no blocking dependency get scheduled as early as
  possible.
- Forcing an artificial dependency to delay `d`'s load (an inline-asm
  read of `d` via a `"m"` operand, with a bogus additional `"r"` input
  depending on `cSaved`'s already-fixed value purely to create a real
  RTL dependency edge) **does** fix the ordering - but only by pushing
  this compiler's register allocator to give `d` a *different*
  permanent home (`r10`/`sl`) instead of the ROM's `r7`, regardless of
  whether the dependency-forcing asm's own output constraint was
  restricted to low registers (`"l"`) or left general (`"r"`, which hit
  a hard assembler error trying to use `sl` in a low-register-only
  Thumb encoding).
- `d` must stay a **completely plain, unconstrained local** for this
  compiler's natural allocator to land it on `r7` at all - per
  `docs/matching.md`'s "why not just pin r7" (also
  `matching_decomp_register_pinning` memory technique 10): explicitly
  pinning `r7`, or constraining an inline-asm operand strongly enough
  to indirectly force it, is a confirmed agbcc/gcc-2.9-arm bug that
  silently drops r7 from the function's own push/pop list - corrupting
  the *caller's* r7 across this function's call to `InitActorPart`, a
  real correctness bug, not just a byte mismatch. Only genuinely
  unforced, natural allocation reaches r7 safely (register pressure
  from every *other* callee-saved low/high register already being
  claimed by `bReg`/`cSaved`/`kindWide`/`health`'s pins is what makes
  the natural allocator pick r7 for `d` by elimination) - and that
  natural-allocation path is exactly the one that schedules `d`'s load
  early.

**The two mechanisms are mutually exclusive for this specific function**:
fixing the b/c/d order costs the correct register for `d`, and getting
the correct register for `d` costs the order. This is a genuinely
different class of gap from the pure-scheduling one `sub_8032440`
closed - it's entangled with a *register-allocation*-level compiler bug
(the categorical r7 pin bug) that this project's tooling has no known
workaround for, per `docs/matching.md`. Every combination tried (fake
dependencies on `b`, `c`, and `self` individually; `"l"`- vs `"r"`-
constrained outputs; folding `d`'s load into the same combined block as
`b`/`c` with a fully symbolic, unrestricted output operand) either
reproduced this same trade-off or hit the categorical bug outright.

The function stays **NAKED** (byte-identical to before this session -
confirmed via a direct string comparison of the transcribed asm body
against the pre-session source), with its "kind"/"c" gaps now closed in
spirit (documented here) and the actual remaining blocker narrowed to
this one specific, well-understood register-allocation/scheduling
conflict, so a future session (or a permuter-assisted search) doesn't
need to re-derive any of this from scratch.

Verified via the full clean pipeline after reverting to NAKED: `rm -rf
build && make NON_MATCHING=1 report` (no new warnings), then `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare` → `crashbandicootxs.gba: OK`.

See [issue-59-60-gap-31a6c-part1.md](issue-59-60-gap-31a6c-part1.md) for
the original pass this follows up on, and
[docs/status/actor.md](../status/actor.md) for the running matched/
parked list.

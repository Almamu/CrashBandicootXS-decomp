# Static-inline anti-CSE promotion: 2 of 3 NAKED functions closed

Three external-contributor PRs (#412-#416) demonstrated a technique this
project hadn't used before: wrapping a small, repeated field-address
computation in a `static inline` helper function instead of writing it
inline, e.g. `src/system/tile_slot_pool.c`'s `PushFreeSlot`/`GetTileSlot`/
`SetTileSlot`:

```c
static inline void PushFreeSlot(struct tile_slot_pool *pool, s32 slot)
{
    pool->freeSlots[--pool->freeTop] = slot;
}
```

used instead of writing `pool->freeSlots[--pool->freeTop] = slot;` inline
at each call site. When the same base+offset address expression appears
more than once in a function, agbcc's optimizer computes it once and
reuses the register both times - but the ROM sometimes recomputes that
same address twice (or, per-field, computes a *different* address fresh
every single time rather than hoisting a shared base pointer across
several field accesses on it). Wrapping the computation in a tiny
`static inline` helper, called from separate call sites, stops the fold:
each inlined expansion apparently gets treated as a distinct context by
whatever pass would otherwise commonize the address arithmetic. A bare
`asm volatile("" ::: "memory")` barrier does **not** achieve the same
effect (confirmed elsewhere in this project already, and re-confirmed
here) - it invalidates memory contents, not an already-computed
pure-arithmetic register value.

This pass tried the technique on 3 currently-NAKED functions, all
previously documented as blocked by exactly this class of compiler
behavior. Two closed; one didn't (a related but distinct blocker - see
below).

## Closed: `UpdateTitleLogoPieces` (`src/graphics/graphics_loading_35780.c`)

The highest-confidence candidate - its own file's header comment already
described the blocker in almost these exact words: "any plain-C phrasing
that computes `self + offset + r3` repeatedly gets CSE'd into a shared
base register... tried: raw pointer casts, `asm volatile` barriers -
neither stopped the fold." The function updates a 9-slot (`i` = 0..8,
stride 0x34) record array hanging off `self`; the ROM recomputes
`self + CONST + i*0x34` completely fresh (three instructions: copy
`self` to a low register, add the constant offset, add the stride) for
*every* field access - roughly 30 of them across the function - rather
than computing `self + i*0x34` once per slot and reusing it as a base
with small per-field immediate offsets, which is what any natural
struct-array-indexed C (or even raw pointer-cast C) converges to.

The fix: one `static inline` accessor per distinct field offset (13 of
them - `PosCAt`, `VelAAt`, `DeltaCAt`, ... one per `self+CONST` in the
function), each doing:

```c
static inline s32 *PosCAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x20;
    return (s32 *)(base + stride);
}
```

Three additional subtleties were needed on top of the basic technique
before the isolated-compile byte diff (assembling both the ROM's own
NAKED transcription and the candidate `.s` output, then comparing
Thumb mnemonic-by-mnemonic) reached zero mismatches:

1. **`self+CONST` has to be materialized as its own named local before
   adding the `i*0x34` stride.** Writing the return expression as a
   single `self + offset + stride` gets silently reassociated by this
   compiler into `stride + offset` first, `+ self` last - the opposite
   operand order from what the ROM's build chose. Splitting it into two
   statements (`u8 *base = (u8*)self + offset; return base + stride;`)
   pins the evaluation order to match.
2. **The stride (`i * 0x34`) has to be computed once, into its own named
   local (`stride`), before any field-address call**, mirroring the
   ROM's own `mul r3, r0, r3` done once per loop iteration and reused via
   `r3` for every field. Passing `i` directly into each accessor (letting
   it compute `i * 0x34` internally, once per call) produces the right
   *shape* but schedules the multiply late, out of the ROM's actual
   instruction order.
3. **A handful of individual accesses needed explicit register pins**
   matching the ROM's specific temp-register choices: the "delta
   record" pointer's old/new split (`register T *recordLoaded asm("r0")`,
   with a plain second variable `record = recordLoaded` to get the
   *reused-elsewhere* copy into whatever register that reuse needs), and
   the three `<<16` position-field stores (`register u16 tmp asm("r4")`
   for the loaded halfword, `register s32 shifted asm("r1")` for the
   shifted result) - both cases where this compiler's natural register
   choice for a load-then-shift-then-store sequence didn't land in the
   ROM's own registers without an explicit nudge.

The "accumulate" branch (five `*dst += *src` pairs, each on a different
field/delta-field address) hit a related, narrower issue: writing
`*PosCAt(self, stride) += *DeltaCAt(self, stride);` as one statement let
the compiler share the `self`-to-low-register copy between the two
address computations within that one statement (an extra "add r2,r1,#0"
hop appeared vs. the ROM's two fully independent `mov r/ip` copies).
Splitting the compound assignment into its own five-line block per
field, with both addresses computed via their own named `u8 *xxxBase`
locals directly in that block (no nested helper call for this part)
closed it.

Verified via the mandatory full pipeline: `rm -rf build && make
NON_MATCHING=1 report` (no warnings), `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` ->
`crashbandicootxs.gba: OK`.

## Closed: `sub_8032B6C` (`src/graphics/actor_part130.c`)

Lower initial confidence (flagged as "fully inlines `sub_8033828`'s own
P1/P2 speed-toggle shape twice... the same cross-jump-merging register-
pin hazard that function's own writeup documents, doubled"), but this
one turned out to respond well to the same family of fixes, since its
actual blocker was address-recompute/copy-ordering (like `UpdateTitleLogoPieces`),
not branch-body cross-jump merging (like `TitleScreenCheatInput` below).

The function is a frame-counter-gated dispatcher: every 16th frame it
forces max speed (0x7FFF) on a P1/P2 object pair; every 8th-but-not-16th
frame it restores a cached "normal" speed instead; both paths, plus a
"do nothing" fallthrough, converge on `sub_8032AF8()` + a category-vtable
animation dispatch. The already-matched `sub_8033828`
(`src/graphics/actor_part28.c`) implements the identical toggle as a
real, standalone function - but the ROM's own build of `sub_8032B6C`
never calls it (no `bl sub_8033828` anywhere in the disassembly), so the
original source duplicated the logic inline twice rather than sharing it
via a call.

`sub_8033828`'s own already-matched body was the key reference: it
declares `register u16 val asm("r1");` (no initializer) once, then
*assigns* `val = 0x7FFF;`/`val = gUnknown_03001590;` as a separate
statement inside each branch, rather than declaring-and-initializing a
fresh local per branch. Reproducing that exact shape - a register-pinned
variable declared without an initializer, assigned in its own statement
- was what closed the last two bytes here too: assigning a >255 constant
(`0x7FFF`, which needs a literal-pool load rather than a Thumb `MOV
Rd,#imm8`) directly into an *already-declared* pinned register forces
this compiler to materialize the literal in a fresh register first, then
copy into the pinned target in a second instruction - matching the ROM's
own `ldr r2,=0x7FFF; add r1,r2,#0` pair - whereas declaring `u16 val =
0x7FFF;` fresh lets the compiler just load the literal straight into
whatever register it likes, one instruction, wrong bytes.

The other piece: the pointer reload (`p = gFlashBgPalette;`, needed by
both branches right before the shared two-`strh` tail) kept getting
hoisted into that shared tail as a single physical copy, dropping the ROM's
own per-branch duplicate reload (the ROM reloads it independently in
*each* branch, immediately before jumping into the shared store
sequence). An explicit `asm("" : "+r"(p));` barrier right after `p`'s own
register-pinned declaration, before assigning `val`, forced the reload to
stay in each branch instead of being commoned into the merged tail.

Final shape (both branches use the identical idiom):

```c
static inline void CommitSpeed(u8 *p, u16 val)
{
    *(u16 *)(p + 0x1e) = val;
    *(u16 *)((u8 *)gFlashObjPalette + 0x1e) = val;
}
...
{
    register u8 *p asm("r0") = gFlashBgPalette;
    register u16 val asm("r1");

    asm("" : "+r"(p));
    val = 0x7FFF;               /* or gUnknown_03001590 in the other branch */
    CommitSpeed(p, val);
}
```

Verified via the same full pipeline: `rm -rf build && make
NON_MATCHING=1 report` (no warnings), `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` ->
`crashbandicootxs.gba: OK`.

`tools/report_units.py`'s single combined entry for
`0x08032B6C`-`0x08032C0C` (previously `None`, covering
`sub_8032B6C`/`sub_8032C0C`/`sub_8032EA0` together) is split: `0x08032B6C`
now points at `actor_part130.o` (matched), `0x08032C0C` keeps `None`
(`sub_8032C0C`/`sub_8032EA0` remain parked - unrelated many-high-register
gap, untouched by this pass).

## Did not close: `TitleScreenCheatInput` (`src/graphics/graphics_loading_35780.c`)

Given a real attempt per the task, but this one's blocker is a genuinely
different shape than the other two, exactly as this project's own prior
triage predicted: a 7-way bit-tested dispatch, each arm folding one of 7
fixed "signature" constants into a shared rolling-hash update
(`self+0x210`, the same rotate-then-multiply-by-521 primitive
`HashTitleCheatInput` already implements standalone). The ROM's own build merges
4 of the 7 arms into one shared "compute the hash-slot address, fall
into the hash body" tail (using `r0` as the address scratch register),
while the other 3 arms inline their own copy of the same address
computation (using `r4` instead) and jump *directly* into the hash body,
skipping the shared address-compute block - an asymmetric, seemingly
alternating (arm 1, 3, 5, 7 share; arm 2, 4, 6 duplicate) cross-jump/
tail-duplication decision from whatever compiler actually built this
ROM.

Two structurally different C attempts were tried, each verified via the
same isolated-compile assemble + byte-diff method used for
`UpdateTitleLogoPieces`:

1. **A natural `if / else if` chain**, one arm per bit, each calling a
   `static inline HashUpdate(self, signature)` helper (the same
   accessor-wrapping idea that closed the other two functions). Result:
   248 bytes target vs. 268 bytes generated (20 bytes over) - this
   compiler's own cross-jump pass merged 6 of the 7 arms into one shared
   tail and left only the *last* arm (bit 0x8) as a fully separate,
   unmerged copy - a completely different split than the ROM's own
   4-share/3-duplicate pattern, and wrapping the repeated computation in
   a `static inline` call (which worked for pure address CSE in the
   other two functions) did not change this outcome at all: the merge
   decision is being made by this compiler's own tail-duplication/
   cross-jump elimination pass, which operates on the *inlined,
   flattened* RTL after the call boundary is gone, not on the
   pre-inlining call graph the static-inline trick works by "faking"
   distinctness for.
2. **An explicit `goto`-based CFG** mirroring the ROM's exact ARM
   topology by hand (7 `if` tests, 4 of them `goto shared_addr` and 3 of
   them computing their own address then `goto hash_body` - literally
   transcribing the ROM's own block graph into C control flow). Result:
   224 bytes (24 bytes *short*) - this compiler's own redundancy
   elimination recognized the 3 "duplicate" address computations as
   provably identical to the shared one anyway and discarded them
   regardless of the `goto` structure, additionally hoisting one of the
   duplicated computations across the `cmp`/`beq` test that was supposed
   to guard it (dead across both live paths, so provably safe to hoist -
   but a different, *more* aggressive merge than the ROM's own build
   chose).

Both results are real, different, wrong-by-a-different-amount cross-jump/
redundancy-elimination decisions from what a natural or hand-forced C
control-flow shape produces here - confirming this is the "cross-jump/
tail-merging collapses the ROM's own duplicated address computation"
class of gap already catalogued in
[issue-65-0x08035780-graphics-loading.md](issue-65-0x08035780-graphics-loading.md),
not the "shared-base-pointer CSE" class the static-inline technique
targets. `TitleScreenCheatInput` is left exactly as it was (NAKED, byte-verified,
untouched) - no changes were made to it or reverted, since none were
committed to the real file in the first place.

See [docs/status/graphics_loading.md](../status/graphics_loading.md) and
[docs/status/actor.md](../status/actor.md) for the running matched/parked
lists this entry feeds into.

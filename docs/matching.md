# Matching decompilation

Byte-exact matching progress for functions across the whole codebase (not
just graphics) - gotchas, per-function notes, and cleanup-pass history
for turning hand-disassembled `asm/code_3_*.s` blocks into real,
compiling C. The step-by-step loop to actually follow lives in
[workflow.md](./workflow.md); see [graphics.md](./graphics.md) for the
separate, unrelated topic of graphics/sprite asset extraction, and
[audio.md](./audio.md) for the sound engine.

First two functions turned into real, byte-matching C:
`QueueVramDmaTransfer` and `FreeVramDmaQueue`, both in `src/graphics/graphics.c`
(the DMA-transfer queue used by the animation-frame system - see "Found
the real per-actor animation-frame system" above). Both compile with
`tools/agbcc` to output that's byte-identical to the original ROM at
those addresses, verified via a full clean `make compare`.

**A gotcha worth knowing before doing more of this:** a C function whose
compiled body isn't a multiple of 4 bytes gets padded up to one by
`arm-none-eabi-as` when it's the last thing in its translation unit's
`.text` section - and the assembler's default pad-fill for Thumb code is
the NOP encoding (`0xC046`), not zero. The *original* hand-written
`asm/code_3_*.s` almost always has an explicit `.align 2, 0` at these
exact spots (zero-fill, chosen deliberately by whoever wrote the original
disassembly, presumably because they'd observed the real ROM bytes there
were zero) - so a lone matched function ending on a non-4-aligned byte
count will mismatch by exactly those trailing pad bytes, even though
every real instruction matches perfectly. This isn't a linker-script
`FILL()` issue (that only controls gaps the *linker* inserts, not padding
already baked into an object file by `as`).

**The fix:** add `asm(".align 2, 0");` at file scope right after the
function (confirmed to produce zero-fill instead of the assembler's
default NOP-fill, verified by direct byte comparison against the ROM).
Do this any time a match is otherwise perfect but the tail is off by 1-2
bytes and nowhere else - it means the C is already correct, just missing
this explicit directive; don't go looking for a bug in the function
itself. (`QueueVramDmaTransfer`/`FreeVramDmaQueue` happened to sidestep
this the first time since matching both together landed on a 4-byte
total by coincidence - the explicit `asm(...)` is the general fix and
doesn't depend on that kind of luck.)

`asm/code_3.s` (122256 lines) is now split into `asm/code_3_1.s`,
`asm/code_3_2.s`, and `asm/code_3_3.s` around where `src/graphics/graphics.c`'s
and `src/graphics/actor_anim.c`'s functions used to live - expect more such splits
as more functions get matched out of it over time. **One `.c` file per
contiguous ROM region, not one per "topic":** `GetAnimFrameBaseOffset`
(ROM `0x0803B058`) is nowhere near `QueueVramDmaTransfer`/
`FreeVramDmaQueue` (ROM `0x08006B94`-ish) even though all three are part
of the same animation-frame system - since one object file's `.text` can
only be placed as a single contiguous block by `ldscript.txt`, a function
whose real address isn't adjacent to an existing matched file's functions
needs its own new `.c` file (here, `src/graphics/actor_anim.c`), not just an
addition to the existing one - adding it to the wrong file would silently
move it to the wrong ROM address.

Third matched function: `GetAnimFrameBaseOffset` in `src/graphics/actor_anim.c` -
trivial (a single field read + arithmetic shift), included here mainly to
confirm the "new `.c` file, non-adjacent region" workflow above works.

Fourth matched function: `FlushVramDmaQueue`, at the top of
`src/graphics/graphics.c` (ROM `0x08006B1C`, right before `QueueVramDmaTransfer` in
the same contiguous region, so no new split/ldscript entry was needed).
This one took two rounds to get exactly right - the recipe, since it's
non-obvious:

- The loop condition must re-read `gUnknown_03001290.count` **fresh every
  iteration**, matching the ROM's `ldr r0,[r6,#4]` inside the loop body.
  Plain `s32 count` lets gcc 2.9 -O2 treat the trip count as loop-invariant
  and hoist it into a decrementing counter (no re-read at all). The fix is
  a *local* volatile cast used only inside this function -
  `#define QUEUE_COUNT (((volatile struct dma_queue *)&gUnknown_03001290)->count)`,
  then `for (i = 0; i < QUEUE_COUNT; i++)`. Marking the struct's `count`
  field itself `vs32` also forces the re-read, but it's the wrong fix: the
  same field is read (non-volatile, cached-in-a-register) by
  `QueueVramDmaTransfer` earlier in this same file, and making the field
  volatile broke *that* function's already-matched codegen (it started
  re-loading `count` twice instead of caching it - a real regression,
  caught by re-running `make compare` on the whole ROM, not just this
  function's byte range). Casting through a local volatile pointer instead
  keeps the volatility scoped to the one call site that needs it. Also
  worth noting: casting `&gUnknown_03001290.count` (the field's address)
  to a volatile pointer instead of casting the *struct pointer* changes
  which address ends up as the literal-pool constant (`gUnknown_03001290+4`
  instead of `gUnknown_03001290`), which look equivalent after relocation
  but produce different bytes than the ROM actually has - cast the struct
  pointer, not the field address.
- The entries pointer must be written as `entry = &gUnknown_03001290.entries[i];`
  **inside** the loop (array-indexed off the loop variable, not a
  `pointer++` incremented once outside it). GCC's strength reduction then
  turns this into exactly the ROM's pattern: a single pointer load in the
  loop preheader (right after the initial "count > 0" guard) plus an
  `add r2, r2, #0xc` per iteration - and, combined with the volatile count
  read above, this is what makes the compiler allocate a *second*,
  separate copy of `&gUnknown_03001290` (r6 for the repeated count check,
  r5 for the entries-deref/final "count = 0" reset), matching the ROM's
  r5/r6 split exactly. A plain incremented pointer variable collapses both
  roles onto one register and loses the split.
- Reading `entry->field_08` and shifting it (`>> 2`/`>> 1`) before OR-ing
  in the DMA control flags needed one more trick. The ROM loads the raw
  halfword into r1 and shifts the result into r0 (`ldrh r1,[r2,#8]`;
  `lsrs r0,r1,#2`); every ordinary C phrasing tried instead collapsed this
  to a single register (`ldrh r0,[r2,#8]`; `lsr r0,r0,#2`) - gcc 2.9's
  local-alloc pass just doesn't pick the same two registers a human would
  from that C alone, and no amount of reordering/renaming budged it (worse,
  adding named locals anywhere in the function perturbed *unrelated*
  register choices elsewhere, e.g. moved the entries pointer from r2 to
  r1). The fix: pin the two temporaries to explicit hard registers with
  GCC's old-style register-variable syntax -
  `register u16 raw asm("r1");` and `register u32 shifted asm("r0");` -
  then `raw = entry->field_08; shifted = raw >> 2; shifted |= 0x84000000;`
  reproduces the ROM's register choice exactly. This is a legitimate,
  commonly-used decomp technique for exactly this situation (nudging gcc's
  allocator when no plain-C phrasing does it), not a hack specific to this
  function - reach for it whenever a close-but-not-quite match traces back
  to one specific pair of registers gcc won't pick on its own.

Also confirmed along the way: the magic DMA control constants are exactly
the existing `DmaCopy32`/`DmaCopy16` control words -
`0x84000000 == (DMA_ENABLE | DMA_32BIT) << 16` and
`0x80000000 == (DMA_ENABLE | DMA_16BIT) << 16` (see `dma_macros.h`) -
though writing it that way vs. a raw hex literal makes no codegen
difference (both constant-fold identically).

Fifth matched function: `sub_8006B0C` (ROM `0x08006B0C`, immediately
before `FlushVramDmaQueue` in the same contiguous region - joined
`src/graphics/graphics.c` right above it, no new split). A trivial one-shot first
try: `void *sub_8006B0C(void *arg0) { sub_8006A90(arg0); return arg0; }`
matched byte-for-byte immediately - a plain "call a helper for its side
effect, then return the original argument unchanged" idiom, which gcc 2.9
compiles predictably (save the arg across the call in a callee-saved reg,
restore it into r0 for the return). `sub_8006A90` itself is still
unmatched asm - it's part of a little three-function family
(`sub_8006A78`/`sub_8006A84`/`sub_8006A90`, all still asm, all operating
on a 3-field struct at ROM `0x08006A78`-`0x08006AAC`) that looks like a
double-buffer swap/reset utility given how many places call it, but wasn't
investigated further here since the goal was just to match this one
wrapper - left un-renamed (still `sub_8006A90`/`sub_8006B0C`) rather than
guess at a name from partial evidence.

Sixth matched function: `sub_8006AF4` (ROM `0x08006AF4`, immediately
before `sub_8006B0C`, same contiguous region - joined `src/graphics/graphics.c`
right above it, no new split). Another one-shot match: a conditional-call
wrapper, `if (arg1 & 1) sub_8026ED0(arg0);` - gcc 2.9 compiles the
bitwise-AND-then-compare-to-zero idiom for testing a single bit exactly
as the ROM has it (`ands r0, r0, r1; cmp r0, #0; beq ...`, not a `tst`
instruction, which the ROM also doesn't use here). `sub_8026ED0` stays
unmatched asm.

Seventh matched function: `sub_8006AC8` (ROM `0x08006AC8`, immediately
before `sub_8006AF4`, same region - joined `src/graphics/graphics.c` right above
it). Inserts a 2-pointer record (`arg1[0]`/`arg1[1]`) into a slot
`arg0 + count*8 + 0xC` of a 128-slot table living inline in `*arg0`
(bounds-checked against `0x7F`), while preserving the 2-byte value that
was sitting at `+0x12` of that slot (read before the overwrite, written
back after - the field at `+0x12` overlaps the tail of the second pointer
field, i.e. entries are 8 bytes apart but the record touches 20 bytes
starting at `+0xC`, the classic GBA OAM-entry-affine-padding overlap
trick). Took more register-pinning than any function so far - four
separate `register ... asm("rN")` variables were needed to reproduce the
ROM's *inconsistent* register choices for what look like structurally
identical operations (the first "read count, shift, add base" sequence
stays in one register throughout; the second, later "reload count, shift,
add base" sequence spreads across two different registers) - gcc 2.9 just
doesn't allocate the same way twice for near-identical code, and no
amount of C-level rephrasing reproduced it without pinning. Two other
non-obvious pieces were needed too:
- The count field must be read through a **local volatile cast**
  (`*(vs32 *)arg0`) both times, or gcc CSEs the second "reload" away
  entirely (no aliasing barrier otherwise, since nothing in between
  looks like it could change `*arg0` from the compiler's point of view).
- The two incoming record fields must be read into **named temporaries
  before either is stored** (`v0 = arg1[0]; v1 = arg1[1];` then both
  stores) - writing it as two direct `field = arg1[i];` statements makes
  gcc interleave load/store/load/store (reusing one register for both
  loads), which is a different instruction order than the ROM's
  load/load/store/store.
`arg0`'s struct layout (a count at `+0`, then table entries starting at
`+0xC`, most likely with 8 more bytes of header in between not touched by
this function) isn't otherwise identified, and `arg1`'s two fields are
untyped (`u32`) rather than named - this function was matched byte-exact
without pinning down what data it actually manages. (Resolved by the next
function below: the `+0xC` table is an **OAM shadow buffer**.)

Eighth matched function: `sub_8006AAC` (ROM `0x08006AAC`, immediately
before `sub_8006AC8`, same region - joined `src/graphics/graphics.c` right above
it). One-shot match, no register tricks needed - a tiny leaf function
(no `push`/`pop` at all, matching the ROM exactly, since it makes no
calls and needs no callee-saved registers) that DMAs `arg0 + 0xC` to
`0x07000000` (**OAM**, confirmed - that's the GBA's real object-attribute
memory address) for `0x100` 32-bit units = `0x400` = 1024 bytes, exactly
the size of the whole OAM (128 sprites x 8 bytes). This confirms
`sub_8006AC8`'s 128-entry, 8-byte-stride table at `arg0 + 0xC` (previous
entry above) **is** that same shadow OAM buffer, and the struct at `arg0`
is an OAM-shadow-buffer manager: a slot count at `+0`, then the shadow
OAM table at `+0xC`. `DMA3.cnt = 0x84000100` reused the existing
`DMA3`/`struct dma_regs` from `FlushVramDmaQueue` above - no new types
needed. One nice confirmation of gcc 2.9's constant-synthesis behavior
carrying over: `0x07000000` (not representable as an 8-bit rotated
immediate) is built the same way as `FlushVramDmaQueue`'s DMA flags -
`mov r0, #0xe0; lsl r0, r0, #0x13` - triggered here just by writing the
plain decimal-looking hex literal `0x07000000`, no special phrasing
needed.

Ninth, tenth and eleventh matched functions: `sub_8006A78`/`sub_8006A84`/
`sub_8006A90` (ROM `0x08006A78`-`0x08006AAC`, immediately before
`sub_8006AAC` - joined `src/graphics/graphics.c` right above it, no new split).
All three matched byte-exact on the first try, no register tricks. This
is the little "swap/reset" family mentioned as unidentified back at the
fifth match (`sub_8006B0C`) - now given a real (if provisionally-named)
struct, since matching them required picking concrete field types:

```c
struct sub_8006A78_struct {
    s32 field_00;   // count - same field sub_8006AC8/sub_8006AAC read via arg0+0
    s32 field_04;
    s32 field_08;
};
```

(Later folded into `struct oam_shadow_buffer` - see the cleanup-pass entry
further down - once it became clear this is the same 12-byte header
`sub_8006AC8`/`sub_8006AAC`/etc. address into, with `field_00` renamed to
`count` accordingly.)

`sub_8006A78`: `count = field_04; field_08 = 0;`. `sub_8006A84`: the
mirror image, `field_04 = count; field_08 = 0;`. `sub_8006A90`: zero
`count`/`field_08` directly, then call `sub_8006A84` (copies the just-
zeroed `count` into `field_04`, re-zeros `field_08`) then `sub_8006A78`
(copies that zero back from `field_04` into `count`, re-zeros
`field_08` again) - a convoluted-looking but exact way of zeroing all
three fields by reusing the two swap primitives rather than three direct
stores, which only makes sense if the original source is doing the same
thing for consistency with how the swap functions are used elsewhere
(not otherwise justified from this function alone). Since `count` is
the same field `sub_8006AC8` bounds-checks and increments,
`sub_8006A90` (called from `sub_8006B0C`, called from further out still
unmatched) most likely **resets the OAM shadow buffer manager to empty**
- `field_04`/`field_08`'s purpose (double-buffer index? generation
counter?) isn't identified.

Twelfth matched function: `sub_8006A48` (ROM `0x08006A48`, immediately
before `sub_8006A78`, same region - joined `src/graphics/graphics.c` right above
it). The **hardest match yet** - confirms the semantic picture further:
starting from `arg0`'s current "count", it walks every *unused* slot from
`count` to `127` and forces bits `[9:8]` of that OAM entry's `attr0` to
`0b10` (clears the low 2 bits of the byte at `entry+1`, then sets bit 1) -
the standard GBA idiom for **disabling a sprite** (non-affine + disable
bit set). This is the natural counterpart to `sub_8006AC8`: after writing
`count` live sprites, call this to hide the rest of the previous frame's
leftovers.

Getting the loop's *structure* right (a `do`/`while` with the address as
a plain incrementing pointer, register-pinned the same way as
`sub_8006AC8`) was routine by this point. What wasn't routine: gcc 2.9
kept reordering two independent, data-flow-unrelated operations inside
the loop body - copying the loop-invariant mask into a scratch register,
and loading the current byte - putting the **load first** no matter how
the corresponding C statements were ordered, whether the mask was a named
variable or an inlined constant, or whether a `loaded` temporary was
pinned, unpinned, or removed entirely. Explicit `register` pins alone
couldn't fix it (unlike every function so far) because the mismatch
wasn't about *which* register, it was about *instruction order between
two register-only operations gcc's scheduler treats as freely
interchangeable*. The fix: drop to a single inline-asm instruction for
just the copy, anchoring it in place -
`asm volatile("add %0, %1, #0" : "=r"(result) : "r"(mask));` - which
gcc cannot reorder relative to the following C statements. The same
technique fixed a second, unrelated one-instruction case: computing the
loop's starting address as `self + offset` vs `offset + self` produces
different register operand order in the 3-register Thumb `ADD` (`adds
r1, r0, r1` vs `adds r1, r1, r0` - same value, different bytes), and no
combination of writing the addition (plain `+`, `+=`, swapped operands)
changed gcc's canonicalization - so that one add is also inline asm,
`asm volatile("add %0, %1, %0" : "+r"(self) : "r"(result));`, reusing
`result`'s register for the offset since their lifetimes don't overlap.
Both asm statements are simple, single real Thumb instructions (not a
trick or a workaround bug) - see [[matching_decomp_register_pinning]] for
when to reach for this.

Thirteenth matched function: `sub_8006A14` (ROM `0x08006A14`, immediately
before `sub_8006A48`, same region - joined `src/graphics/graphics.c` right above
it). The bulk-copy counterpart to `sub_8006AC8`: instead of copying one
record's fields with the CPU, this DMAs `arg2` whole 8-byte OAM entries
straight from `arg1` into the shadow buffer at the current count
(`arg0 + count*8 + 0xC`), then advances `count` by `arg2`. Matched
byte-exact on the second try, no register pins needed this time - the
only issue was the same additive-grouping quirk seen in `sub_8006A48`
(`base + (offset + const)` vs `(base + offset) + const` producing
different Thumb `ADD` byte sequences for the same value), fixed just by
adding explicit parentheses to group the shift-and-constant before adding
the base pointer - no inline asm needed here, unlike the pathological
case in `sub_8006A48`.

Fourteenth matched function: `sub_80069E8` (ROM `0x080069E8`, immediately
before `sub_8006A14`, same region - joined `src/graphics/graphics.c` right above
it). Writes OAM **affine parameters**: given a pointer directly to an OAM
entry (no `+0xC` bias this time - the caller must already point at the
right shadow-buffer slot) and `arg2` groups, each group writes one `s16`
from `arg1` into the padding field (`+0x12` again, same field
`sub_8006AC8` preserves) of the first of 4 consecutive 8-byte entries,
zeroes the padding of the next two, and writes a second `s16` from
`arg1+2` into the fourth - i.e. `PA = src[0], PB = 0, PC = 0, PD = src[1]`
for each affine group, matching the real GBA OAM affine-matrix layout
(4 entries' padding bytes hold `PA`/`PB`/`PC`/`PD` in sequence) and
building a pure axis-aligned scale matrix (no rotation/shear terms).
Matched byte-exact on the first try, no register pins needed - the
constant `0` gets materialized into a register once before the loop
(matching the ROM), and the only ordering quirk was which of the two
preheader instructions (materializing `0`, or copying `arg0` into the
loop pointer) came first - fixed by writing the `zero = 0;` assignment as
its own earlier statement, ahead of the entry-pointer setup.

Fifteenth matched function: `sub_800695C` (ROM `0x0800695C`) - and the
**first one requiring a new mid-file split**, in `src/graphics/oam_count.c`.
Counts how many of 20 fixed-stride (4-byte) records have bit 0 of the
byte at `+4` set - almost certainly counting how many of a fixed set of
"slots" (particles? actors?) are currently active, unrelated to the OAM
system by address but reusing the same shift-based bit-test idiom
(`(x << 31) >> 31` for extracting bit 0 with an unsigned result) as
`sub_8006A48`'s mask synth - GCC 2.9 reliably produces this specific
shift pair for isolating a single bit, so it's a idiom worth recognizing
elsewhere. Needed register pins (byte load -> `r4`, forcing the
push/pop that a natural, unpinned compile skips entirely since it fits
in caller-saved registers) - a good example of pinning changing not just
*which* register but *whether the prologue needs one at all*.

**The split, and a mistake caught before committing:** `sub_800695C` is
immediately followed in ROM by `sub_800697C` (still unmatched - calls
five other unidentified functions, out of scope here), which is what
`src/graphics.o` used to sit directly behind. Initially added
`sub_800695C` straight into `graphics.c` above `sub_80069E8` - `make
compare` failed the full-ROM checksum, and the reason was exactly the
"one .o's `.text` is one contiguous block" rule from the very first
matched functions: pulling `sub_800695C` into `graphics.c` while
`sub_800697C` stayed asm would have collapsed the 108-byte gap between
them, shifting everything after downstream. Fixed by giving
`sub_800695C` **its own file** (`src/graphics/oam_count.c`) and splitting
`sub_800697C` out into its own asm file (`asm/code_3_1_697c.o`, needs the
usual `.include "asm/macros.inc"` / `.syntax unified` / `.arm` header a
plain `sed`-extracted fragment doesn't have), landing them in the
ldscript in ROM order: `code_3_1.o`, `oam_count.o`, `code_3_1_697c.o`,
`graphics.o`, ... A reminder to check ROM-address contiguity against
*every* neighboring function - including ones several matches back -
before assuming a new match can just join an existing file.

Sixteenth matched function: `sub_800697C` (ROM `0x0800697C`) - the
function flagged as "bigger scope" last time, calling five other
unidentified functions. Sums several subsystems' per-`arg0` contributions
into one total - `sub_800695C` (the active-slot counter matched above),
`sub_80068CC`, `sub_8006864` (halved, rounded toward zero -
`(x + (unsigned)x>>31) >> 1`, the standard signed-divide-by-2 idiom),
`sub_8006820`, `sub_80067EC`, plus four individual bits (7, 5, 6, 4, in
that order) of a flags byte at `arg0+2` - then multiplies the total by
100 and passes it (with a constant `72`) into `sub_803ADB4`, almost
certainly a "draw number as text" call (`72` reads like a screen Y
position) - most likely a debug/menu stat display (something like an
active-object or particle count). None of the five callees were matched
or even given real signatures beyond `s32 f(void *)` inferred from the
call sites - out of scope here, flagged for later.

Matched byte-exact, but needed the session's most elaborate register
pinning yet: five separate `register` variables (`self`→`r6`, the
running `total`→`r4`, and the three intermediate call results spread
across `r9`, `r5`, `r8`) reproduced the ROM's exact save/restore dance for
the high registers `r8`/`r9` (Thumb can't push them directly, so gcc
copies them to low registers first, matching the ROM's
`mov r6,sb`/`mov r5,r8`/`push {r5,r6}` prologue and its mirror-image
epilogue) - once the registers were pinned, gcc produced the *entire*
function's instruction sequence correctly on the very next attempt, no
further reordering fights needed (contrast with `sub_8006A48`). The one
remaining piece, after pinning: the epilogue's final "pop a register,
branch to it" step used `r0` in every attempt, but the ROM uses `r1` -
turned out to depend on whether the function's return value is
considered live at that point. `sub_803ADB4`'s result was being discarded
(`void`-returning call as the last statement); declaring `sub_800697C`
itself to `return sub_803ADB4(...)` instead (making the call result a
genuine, live return value in `r0`) freed `r0` from being reused as the
epilogue's scratch register, forcing gcc onto `r1` and completing the
match - a good reminder that a function's own return type/value can
shape its *own* epilogue register choice, not just its body.

Matching this function also **retired** the `asm/code_3_1_697c.o` split
from the previous entry: since `sub_800697C` occupied that entire split
file and is immediately followed by `sub_80069E8` (already in
`graphics.c`), moving it to `graphics.c` too closed the gap completely -
deleted the split file and removed its `ldscript.txt` line. `oam_count.c`
(holding `sub_800695C`) is still needed as its own file, since it's
followed by `sub_800697C`'s ROM address, not `graphics.c`'s.

Seventeenth matched function: `sub_8006920` (ROM `0x08006920`,
immediately before `sub_800695C` - joined `src/graphics/oam_count.c` above it, no
new split). A near-twin of `sub_800695C`: same 20-record, 4-byte-stride
loop shape, but sums **bits 1 and 2** (not bit 0) of the byte at each
record's `+4`, plus the same two bits from one more byte at `arg0+0x64`
(a fixed field past the array, not part of the loop) - almost certainly
a sibling "count how many records have flag X set" query using a
different bit of the same per-record flags byte. Matched byte-exact on
the second try - no register pins needed at all this time, just
reordering two preheader statements (`total = 0;` before `p = arg0;`,
matching the ROM's `movs r4,#0` before `adds r2,r5,#0`).

Eighteenth matched function: `sub_80068CC` (ROM `0x080068CC`,
immediately before `sub_8006920` - joined `src/graphics/oam_count.c` above it, no
new split, and it's one of the five callees `sub_800697C` left
unidentified). Combines everything the previous two entries found: the
same 20-record-plus-one-extra bit-summing shape as `sub_8006920`
(**bits 1 and 2** again, not bit 0 or the 7/5/6/4 set `sub_800697C`
reads), *plus* four more bits (0, 2, 3, 1, in that order) from the same
flags byte at `arg0+2` that `sub_800697C` reads directly. Confirms
`arg0+2` is a genuinely multi-purpose bitfield byte queried by at least
three of this cluster's functions, each reading a different subset of
its bits.

Needed register pins (`p`→`r2`, `total`→`r4`, `i`→`r3`) to match the
ROM's unusually tight register budget (only `r4`/`r5` pushed, no `r6` -
one fewer register than a first plain-C attempt used, because the ROM
reuses `r2` for two different roles at different times: the loop's
induction pointer, then - once that pointer is dead - the flags byte
read). The final four-bit accumulation also switches from the pinned
`total` (`r4`) to a **second, unpinned local** initialized from it,
matching how the ROM's last `total = total + bit` computation lands in a
fresh register (`r0`) instead of continuing to accumulate in `r4` - by
this point in the session, splitting a single logical accumulator into
"the pinned one" and "the one that picks up after it" is a recognizable
move whenever the ROM's own accumulator changes registers partway
through a chain of additions with no persisting need for the old one.

Nineteenth matched function: `sub_80068A8` (ROM `0x080068A8`, immediately
before `sub_80068CC` - joined `src/graphics/oam_count.c` above it, no new split).
The simplest of this whole cluster: sums three of `sub_800697C`'s five
callees directly - `sub_8006864 + sub_8006820 + sub_80067EC` - no bit
tests, no halving. Matched byte-exact on the first try, no register pins
needed. Two of the five callees (`sub_8006864`, `sub_8006820`) are now
each called by *two* different matched functions (`sub_800697C` and
`sub_80068A8`), and `sub_80067EC` by both of those too - worth matching
next, since it would immediately pay off three call sites at once.

Twentieth matched function: `sub_8006864` (ROM `0x08006864`, immediately
before `sub_80068A8` - joined `src/graphics/oam_count.c` above it, no new split).
Another 20-record loop, this time a genuine **range check** rather than
a bit test: for each record, take the halfword at `+4`, shift right by
3, and - if nonzero - count it only if it falls in `(gStaticData_0816C86C[i].min, gStaticData_0816C86C[i].max]`,
where the per-record bounds live in a *parallel* table (`gStaticData_0816C86C`,
stride `0x24`, `max` at `+8`, `min` at `+0xC`) indexed by the *loop
position*, not by anything read from the record itself.

The hardest register-allocation fight of the whole session. gcc 2.9's
CSE reliably recognized that both bounds live at `base + offset + {8,0xC}`
and always shared the `base + offset` computation between them once
formed as one live register value across the branch - and every
plain-C rewrite tried (a local copy of the base pointer, restructuring
associativity, `volatile`-qualifying the pointer, splitting into nested
`if`s) still let gcc reuse it, which doesn't match the ROM: the ROM
recomputes the full `base + bias + offset` chain from scratch for *each*
bound, and even reloads the `gStaticData_0816C86C` address from its
literal pool **inside the loop** (after the first bit-test branch, not
hoisted to the preheader) rather than once up front. Fixed with two
inline-asm blocks, one per bound, each spelling out the exact three-`ADD`
chain (`add %0,%1,#0` / `add %0,%0,#N` / `add %0,%2,%0`) with the
*symbol itself* (`gStaticData_0816C86C`, not a locally-cached pointer
variable) as an input operand - since nothing hoists the symbol's address
out of the conditional block it's used in, the literal-pool load lands
exactly where the ROM has it, and since each inline-asm block is opaque
to CSE, gcc can't merge the two chains no matter how similar they are.
The loaded halfword's own register also needed an explicit pin distinct
from the shifted result's register (`raw`→`r0`, `val`→`r1`), matching a
now-familiar pattern from earlier entries where the ROM keeps a
freshly-loaded value and its transformed result in different registers.

Twenty-first and twenty-second matched functions: `sub_8006820` and
`sub_80067EC` (ROM `0x08006820` and `0x080067EC`, immediately before
`sub_8006864` - joined `src/graphics/oam_count.c` above it, no new split). Two
more members of the same `gStaticData_0816C86C` range-check family:
`sub_8006820` is `sub_8006864` with different field offsets (`+0xC`/
`+0x10` instead of `+8`/`+0xC` - same anti-CSE inline-asm technique
reused verbatim, just changing the two constants), while `sub_80067EC`
is a **simpler single-bound variant**: only checks `val <= table[i].field_at_0x10`
(no lower bound), and - unlike the other two - the ROM computes the
bound as a **persistent, pre-biased pointer** (`gStaticData_0816C86C + 0x10`,
computed once before the loop and incremented by the `0x24` stride each
iteration) rather than recomputing `base + bias + offset` fresh every
time. Matched byte-exact with no register pins at all: the only fix
needed was introducing a plain local pointer variable for the base
address before adding the `+0x10` bias (`base = gStaticData_0816C86C;
bound = base + 0x10;` instead of `bound = gStaticData_0816C86C + 0x10;`
directly) - writing it as one combined expression let gcc fold the `+0x10`
straight into the literal-pool constant (`.word gStaticData_0816C86C+0x10`,
a single load), whereas the ROM does it as three separate runtime `ADD`s
against the plain unbiased symbol. A cheaper alternative to the inline-asm
anchor from the two entries above, worth trying first when the ROM
"wastes" instructions re-deriving a value gcc would rather fold at
compile/link time.

Twenty-third through twenty-seventh matched functions: `sub_80067A4`,
`sub_80067B4`, `sub_80067C4`, `sub_80067D4`, `sub_80067E4` (ROM
`0x080067A4`-`0x080067EC`, immediately before `sub_80067EC` - joined
`src/graphics/oam_count.c` above it, no new split). A family of four trivial
wrappers, each just `sub_80062A8(constA, constB, constC)` with different
constants (likely per-difficulty or per-mode config calls into whatever
`sub_80062A8` sets up), plus one unrelated one-liner extracting the low 7
bits of a byte. All five matched byte-exact - the four wrappers on the
first try, no tricks; the bit-mask one needed the shift-trick phrasing
(`(u32)(byte << 25) >> 25`) instead of `byte & 0x7F`, since gcc's `&`
with an immediate materializes the mask into a register and ANDs
(2 instructions, valid but different bytes) rather than reproducing the
ROM's shift-based extraction (also 2 instructions) - the same idiom
identified back at `sub_800695C`.

Twenty-eighth matched function: `sub_8006770` (ROM `0x08006770`,
immediately before `sub_80067A4` - joined `src/graphics/oam_count.c` above it, no
new split). Two unrelated pieces in one function: if `arg0->+0x18` is
non-NULL, reads a signed 16-bit offset and a pointer out of a nested
struct (`arg0->+0x18->+0x18`, offset `+0x50`/`+0x54`) and calls
`sub_803AD80(base + offset, 3, ptr)` - looks like resolving a relative
link/index into an absolute address before invoking some renderer or
allocator; then, completely independently, the same `if (arg1 & 1)
sub_8026ED0(arg0);` conditional-call idiom seen before in `sub_8006AF4`.
Matched byte-exact on the second try - the only fix was inlining the
offset and pointer reads directly as call arguments rather than through
named locals, which changed the evaluation order to match the ROM's
(read the signed halfword, compute the base+offset sum, *then* read the
trailing pointer field - not both reads up front).

Twenty-ninth matched function: `sub_8006714` (ROM `0x08006714`,
immediately before `sub_8006770` - joined `src/graphics/oam_count.c` above it, no
new split). A scene/frame setup routine: calls `sub_80006A8(arg0)`, then
`sub_8006DC8`/`sub_8006AAC` on two globals (`gUnknown_030012B8`,
`gUnknown_03001300` - the second call is our own already-matched
`sub_8006AAC`, DMA-flushing an OAM shadow buffer to real OAM), flushes
the VRAM DMA queue (`FlushVramDmaQueue`, also already matched), then pokes
five real GBA I/O registers directly from `arg0`'s fields: `REG_BG0HOFS`
(`0x04000010`, shifted right 3), the first palette color
(`0x05000000`, zeroed), `REG_BLDCNT`+`REG_BLDALPHA` together as one
32-bit write (`0x04000050`), `REG_BLDY` (`0x04000054`, masked to 5 bits
with the by-now-familiar shift-trick idiom), and `REG_DISPCNT`
(`0x04000000`). Matched byte-exact on the **first try**, no pins or
tricks needed at all - the ROM's hardware-address register (which jumps
between `0x04000010`, a freshly-materialized `0x05000000`, then
`0x04000050`, `+4`, and `-0x54` to land on `0x04000000`) falls out
naturally from gcc just evaluating a sequence of plain absolute-address
volatile pointer dereferences in program order.

Thirtieth matched function: `sub_8006700` (ROM `0x08006700`, immediately
before `sub_8006714` - joined `src/graphics/oam_count.c` above it, no new split).
Trivial: increments `arg0->field_1c`, then calls `sub_8008044(arg0->field_18)`
- the same `field_18`/`field_1c` field names as `sub_8006770`, reinforcing
that these functions likely all operate on the same "actor" or "entity"
struct (not unified into one shared type here, consistent with this
file's existing per-function-struct style). Matched byte-exact on the
first try.

**Parked, not matched: `sub_8006600`** (ROM `0x08006600`, immediately before `sub_8006714`). A
HUD-icon-plus-number renderer: resets one OAM manager (`sub_8006A90`),
calls `sub_8006C28` on another global, calls `sub_8008890`, positions a
left icon by computing its centered X (`(240 - width) >> 1`, width from
`sub_803AD80`) and a fixed Y, formats/draws a number via
`sub_803AFE4`/`sub_803AFDC`/`sub_8001214` into a stack buffer, gets its
pixel width via `sub_8026F38`, then positions a second icon the same way
- finally calls the already-matched `sub_8006A48` to hide unused OAM
slots. None of `sub_8006C28`, `sub_8008890`, `sub_803AD80`,
`sub_803AFE4`, `sub_803AFDC`, `sub_8001214`, `sub_8026F38` are matched or
even confidently typed beyond the argument shapes this call site
implies. Uses `struct icon_record`/`struct icon_manager` for the two
OAM-slot-record pairs it reads, and extends the existing `struct
sub_8006700_actor` (shared with `sub_8006700`/`sub_8006714`/
`sub_8006770`) with `field_10`/`field_14` for `self`'s shape rather than
defining a second struct for the same object.

**Build toggle**: this function's C definition in `src/graphics/oam_count.c` is
wrapped in `#if NON_MATCHING`, and the corresponding raw bytes in
`asm/code_3_1.s` are wrapped in `.if NON_MATCHING == 0` / `.endif`, so
exactly one definition of `sub_8006600` is ever assembled. Default builds
(`make`/`make compare`) get `NON_MATCHING=0` from the Makefile and use
the checked-in matching assembly, so `make compare` still passes.
`make NON_MATCHING=1 crashbandicootxs.gba` (or any other target) instead
compiles this C version in - confirmed this session to compile and link
cleanly - useful for testing the reconstructed logic actually behaves
right even while the register-letter mismatch below is unresolved. This
is the first per-function use of this pattern in the project; `#ifndef
NON_MATCHING #define NON_MATCHING 0 #endif` was added to `include/core.h`
so the macro is always defined (default builds don't pass `-D
NON_MATCHING`, and `#if NON_MATCHING` on a genuinely undefined macro
would still evaluate as 0, but defining it explicitly is clearer and
matches how the assembly side already needs a defined symbol from
`--defsym`).

Confirmed via `asmdiff.sh` that the reconstruction matches the ROM's
**total byte count exactly** (every address past this function lines up
again), and this session found one genuinely new, safe fix: caching
`&gUnknown_03001300` in a second high register (`r9`, alongside
`mgrAddrCache`'s existing `r8` for `&gUnknown_030012E0`) and reloading
through it for the final `sub_8006A48` call reproduces the ROM's exact
`mov r4, r9` / `ldr r0, [r4]` instructions byte-for-byte - a whole region
that didn't match in earlier sessions' attempts. High registers keep
working reliably here per technique 8 in `matching_decomp_register_pinning`
memory: both `r8` and `r9` get a proper `mov`-to-lowreg-then-`push`/
`pop`-then-`mov`-back dance in the prologue/epilogue automatically, just
from being assigned to, independent of cross-call liveness.

A follow-up pass narrowed this further by reusing registers that are
already alive rather than introducing new pins - each verified safe and
applied to the checked-in `#if NON_MATCHING` version:

- **`addr` (the temp holding `&gUnknown_030012E0`) pinned to `r1`**: a
  plain scratch local with no cross-call lifetime of its own, so pinning
  it to whatever register the ROM happens to use has no save/restore
  implications at all (`r1` is caller-saved, never part of this
  question). Reproduces the ROM's `ldr r1, =gUnknown_030012E0; mov r8,
  r1; ldr r0, [r1]` exactly, where the unconstrained version had gcc
  pick `r0` instead.
- **`SUB_8006600_STORE_TWO_FIELDS_REUSE_SELF`**: at the *second* of the
  two `STORE_TWO_FIELDS` call sites, `self` (pinned `r4`) is genuinely
  dead - its last read is the `mgr1Base` reload just before this call,
  its next write is the final reassignment to `g1300Addr` near the end
  of the function - so temporarily clobbering it as the scratch register
  for the `0x88 << 1` address computation is completely safe (unlike an
  `r7` pin, this doesn't touch a register that must survive to the
  caller). Matches the ROM's `mov r4, #0x88` / `lsl r4, r4, #1` / `adds
  r1, r0, r4` at this site exactly.
- **`SUB_8006600_GET_RECORD_REUSE_RECOFF`**: the function's *last* read
  of `recOff` (pinned `r5`) computes the final `record` address, so
  computing that address in-place into `recOff` itself (rather than a
  fresh scratch register) is safe and matches the ROM's `adds r5, r0,
  r5` / `ldr r2, [r5]` at that call site exactly.

**Why not just pin `r7`**: `register T x asm("r7")` (explicit
register-variable pinning) is a genuine, silent ABI-violation bug in
this specific agbcc/gcc-2.9-arm toolchain, confirmed by a minimal
standalone repro (`register u32 b asm("r7"); b = a + 1; callee(b);`
compiles to `add r7, r4, #1` / `bl callee` with **no push/pop of r7 at
all**, silently clobbering the caller's r7) and corroborated externally
(pret/agbcc; the overjt/knidl decompilation project documents the same
"r7 (FRAME_POINTER_REGNUM) is never allocated to call-crossing pseudos
by global_alloc even with -fomit-frame-pointer forced" limitation). This
is not fixed in any known agbcc variant and can't be fixed here without
diverging from the exact historical compiler the rest of the ROM was
built with - see technique 10 in `matching_decomp_register_pinning`
memory. It must never be used, even in parked/`NON_MATCHING` code.

**Refined this session**: the bug is narrower than it first looked. It
is specific to *explicit* `register T x asm("r7")` pinning (and likely
inline-asm `"=&r"`-constrained outputs), not to r7 in general. Proof:
`sub_800132C` (`src/graphics/fade_util.c`, already matched byte-exact) has a
do-while loop where a plain, completely unpinned local (`dirBit8`,
originally just a normal C value) survives repeated calls to
`sub_80006A8()` inside the loop, and gcc's own *unforced* allocator
chooses `r7` for it on its own - correctly emitting `push {r4,r5,r6,r7,
lr}` / `pop {r4,r5,r6,r7}`. So values *can* safely live in r7 across
calls in this compiler, as long as they get there via natural,
unforced allocation rather than an explicit pin.

Applying that insight here closed several of the previously-unmatched
spots:

- Replacing the first `STORE_TWO_FIELDS` call's collapsed/inline-asm
  address computation with genuinely separate plain (unpinned) locals
  (`_xOff`/`_addr1`, then a nested block for `_yOff`/`_addr2`), and
  replacing the direct `mgr1Base = *(void **)mgrAddrCache;` reload with
  a block introducing a plain unpinned intermediate, raises the register
  pressure enough that gcc's own allocator reaches for `r7` - the
  prologue/epilogue now matches the ROM exactly: `push {r4,r5,r6,r7,lr}`
  / `mov r7,r9` / `mov r6,r8` / `push {r6,r7}` ... `pop {r3,r4}` / `mov
  r8,r3` / `mov r9,r4` / `pop {r4,r5,r6,r7}`.
- The `halved = (0xF0 - width) >> 1;` computation's shift instruction
  (`lsrs` vs `asrs`) turned out to be sensitive to the *signedness of the
  intermediate*: introducing an explicit two-step temp as `s32 _tmp =
  0xF0 - width;` forces a signed (arithmetic) shift and mismatches the
  ROM's `lsrs`, but `u32 _tmp = 0xF0 - width;` keeps it as an unsigned
  (logical) shift, matching the ROM - the original single-line form
  happened to get this right implicitly (the `0xF0 - width` subexpression
  is unsigned since `width` is `u32`), so this only bit when refactoring
  it into a temp.
- Pinning that same intermediate to `r0` explicitly (`register u32 _tmp
  asm("r0") = 0xF0 - width;`) - safe because it dies immediately with no
  cross-call lifetime - fixes the instruction *ordering/register choice*
  to match the ROM's `subs r0, r6, r0` / `lsrs r3, r0, #1` exactly (the
  unconstrained version computed both steps directly into `r3`).

**What's still unmatched** (four spots, all in the second half of the
function, all resistant to the same techniques so far):

- The first `STORE_TWO_FIELDS`' `mgrAddrCache` → `mgr1Base` reload still
  uses `r0` as its scratch (`mov r0, r8` / `ldr r0, [r0, #0]`) where the
  ROM uses `r7` (`mov r7, r8` / `ldr r0, [r7, #0]`).
- The *second* `halved` computation's reload of `mgr1Base` from `self`
  happens one instruction too early relative to the ROM (ROM completes
  both `subs`/`lsrs` before the reload; this build's reload lands between
  them).
- `SUB_8006600_STORE_TWO_FIELDS_REUSE_SELF`'s *second* field (the
  `0x8a << 1` / y-offset store) still picks `r3`/`r3` (`movs r3, #138` /
  `lsls r3, r3, #1` / `adds r3, r0, r3` / `str r2, [r3, #0]`) where the
  ROM picks `r7`/`r1` (`movs r7, #138` / `lsls r7, r7, #1` / `adds r1,
  r0, r7` / `str r2, [r1, #0]`).
- The second `record->slots[2].offset` (`0x20`) `ldrsh` picks `r7` where
  the ROM picks `r3` (register swap, mirror image of the previous item).

Every attempt this session to touch the second half with the same
"replace collapsed/inline-asm computation with separate plain locals"
technique that worked on the first half **regressed** the hard-won
prologue fix instead of improving these four spots (four separate
variations tried and reverted: two shapes of a new-locals version of the
y-offset store, a version reusing the already-declared `_hv`/`_addr`
instead of new locals, and applying the same `r0`-pinned-intermediate
trick to the second `halved` computation) - each one brought back
`sl`/`r10` usage and lost `r7` from the push/pop list entirely. The
interaction between which specific piece of code is touched and the
resulting whole-function register allocation is fragile and not fully
understood; the second half appears to sit right at a decision boundary
in gcc's allocator that the first half's fix doesn't touch. Given `git
diff` was checked at every step and each regression reverted immediately
(never left in the tree), the version now checked in is a genuine,
confirmed improvement over prior sessions (prologue/epilogue register
list now matches exactly, plus the first `halved` computation's
instruction choice and ordering) even though it stops short of a full
byte-exact match. If revisited, the most promising untried angle is a
permuter search scoped to *only* the second half's four remaining
mismatches (much smaller search space than a fresh whole-function
attempt) rather than more manual rephrasing, since manual attempts on
this specific region have now failed identically four times in a row.
The decomp-permuter run from a prior session was stopped after
plateauing around score 745; its best candidate contained a genuine
uninitialized-variable read (a real correctness bug, not just a
register-letter mismatch) and was discarded rather than adapted.

Thirty-first matched function: `sub_80006A8` (ROM `0x080006A8`, at the
very front of `asm/code_3_1.s`'s remaining content, immediately after
`irq.c`'s `sub_8000680` - moved there, not into `oam_count.c`, once
`make compare` failed after an initial placement: this function's
address is far earlier than `oam_count.o`'s region, and it turned out to
belong right where `irq.c` already left off, its globals
`gUnknown_03000A58`/`gUnknown_03000A5C` sitting immediately before
`irq.c`'s already-established `gUnknown_03000A60`). Ignores its `arg0`
parameter entirely (dead - the ROM never reads `r0` past the prologue,
kept only because the call site passes one). Waits for
`gUnknown_030007D8 >= gUnknown_03000A58` (calling `sub_0803A960()` each
time it isn't, unconditionally at least once if `gUnknown_030007DC` is
0) then adds `gUnknown_03000A5C` onto `gUnknown_03000A58` - looks like a
"wait for some counter to catch up, then advance a threshold" pattern,
maybe a frame-timing/animation-delay wait. Needed one register-pinning
technique: explicit pointer locals (`p1`/`p2`/`p3`) for the three
globals' addresses, loaded unconditionally right after the outer `if`
(matching the ROM's own eager `ldr r5/r4/r6` before branching to the
loop condition) - a direct `while (gUnknown_030007D8 < gUnknown_03000A58)`
with the globals referenced by name let gcc compute the addresses lazily
inside the loop instead. Also needed named locals for both compared
values (loaded fresh each iteration, matching the ROM re-reading through
r5/r4 every pass) so the closing `+=` could reuse the already-loaded
`gUnknown_03000A58` value instead of re-dereferencing it - the ROM reuses
the comparison's last-loaded register for the store afterward. Matched
byte-exact on the second try (first attempt used `s32`/plain globals and
got the branch condition, register letters, and the `+=`'s redundant
reload all slightly wrong).

Next four matched functions, all in `src/system/irq.c` immediately after
`sub_80006A8`, matched first-try each:

- `sub_80006EC` (ROM `0x080006EC`): trivial, `gUnknown_030007DC = 0;`.
- `sub_80006F8` (ROM `0x080006F8`): sets up the wait state
  `sub_80006A8` polls - `gUnknown_03000A5C = arg0; gUnknown_03000A58 =
  gUnknown_030007D8 + arg0; gUnknown_030007DC = 1;` (arm the "wait until
  the counter reaches `gUnknown_030007D8 + arg0`" check for a given
  delay).
- `sub_8000720` (ROM `0x08000720`) - the vblank handler
  (`irq.c` already had `extern irq_handler_t sub_8000720;` as a
  placeholder for its address before this session; changed to a plain
  forward declaration `void sub_8000720(void);` now that it's a real
  function, matching how `IrqEmptyHandler` is declared/referenced
  elsewhere in this file). Conditionally calls `sub_8038B68()`, then
  calls `sub_803AD78()` for every nonzero slot in
  `gUnknown_03000A60.unknown[8]` (iterated via a pointer walking forward
  while a separate counter counts `7` down to `0`, matching this file's
  existing loop idiom), then increments `gUnknown_030007D8` - the
  counter `sub_80006A8`/`sub_80006F8` above poll/arm.
- `sub_8000760` (ROM `0x08000760`): remaps 4 bits of the
  `gUnknown_030007E0` input-flags halfword (`0x10`/`0x20`/`0x80`/`0x40`,
  read fresh via the global each time - a cached local variable made gcc
  insert a redundant register copy for the last comparison that the ROM
  doesn't have) into a 4-bit index (`8`/`4`/`2`/`1` respectively) used to
  look up `gStaticData_0816A810[idx]` - likely a D-pad-bits-to-angle or
  similar remap table. The first bit-check compiles branchless (a
  `rsbs`/`asrs` sign-extend trick turning "is this bit set" straight
  into "0 or 8" without a conditional branch) purely from gcc's own
  optimization of a plain `if (flags & 0x10) idx |= 8;` - no special
  phrasing needed, it just happens to pick a different strategy for the
  first check than the following three (which do branch).

Fifth matched function in this run: `sub_80007AC` (ROM `0x080007AC`,
immediately after `sub_8000760`). Reads `REG_KEYINPUT` (active-low),
inverts it active-high, records newly-pressed bits into
`gUnknown_030007E2` (`gUnknown_030007E0 & ~previousValue`, written
through pointer arithmetic off `gUnknown_030007E0` - a second `extern`
for the adjacent global made agbcc treat the two addresses as
unrelated and emit a non-matching second literal-pool load/store pair),
updates `gUnknown_030007E0`, then returns `1` if the low 4 bits (A/B/
Select/Start) are all held - a "soft reset" combo check. Needed two
techniques, both entirely on plain caller-saved scratch registers
(`r0`-`r3`, none of which carry the r4-r7 hazard from the `sub_8006600`
saga): pinning `keysR1`/`mask` to `r1`/`r0` for the closing mask-and-
compare, since gcc's own unpinned allocator puts the AND's result in a
fresh register instead of reusing `r1` in place and compares against a
fresh immediate instead of reusing `r0`'s already-loaded `0xF`; and a
one-instruction `asm volatile("add %0, %1, #0")` anchor (technique 6 in
`matching_decomp_register_pinning` memory) to force a scratch copy of
`keys` to happen *before* the `gUnknown_030007E0` reload rather than
after - gcc's scheduler freely reordered the two since neither depends
on the other, regardless of the C statement order they were written in.
`addr`/`prevKeys` also needed pinning to `r2`/`r3` specifically (not
just *some* free registers) to match the ROM's exact choice once the
ordering was fixed.

Sixth: `sub_80007DC` (ROM `0x080007DC`, immediately after
`sub_80007AC`) - trivial, zeroes both `gUnknown_030007E0` and the
adjacent `gUnknown_030007E2` (accessed the same way as `sub_80007AC`
above, through pointer arithmetic off `gUnknown_030007E0`). Needed only
`r1`/`r2` pins (both plain scratch) to match the ROM's register choice
for the address/zero-constant pair - matched on the second try.

`sub_80007EC` (ROM `0x080007EC`, right after) is a much larger function
- affine BG transform math (BG2CNT/DISPCNT setup, two calls to
`sub_800090C` feeding `BG2PA`/`PB`/`PC`/`PD` and computed `BG2X`/`BG2Y`
offsets around `0x04000020`) plus a palette DMA transfer and
`LoadTaggedAsset` at the end. Got byte-count-exact and instruction-exact
in every operation, but couldn't pin down one instruction-scheduling
detail: the ROM computes the second `sub_800090C` result's sign-extended
(`s16`) and zero-extended (`u16`) forms interleaved with *other*,
unrelated statements in a way no amount of C-level statement reordering
reproduced (tried ~6 orderings - each shifted which of two independent
instructions landed first, never both). Left unmatched (still raw
assembly) rather than chase the exact ordering further, and - since this
function must stay in `asm/code_3_1.s` either way - the once-contiguous
`asm/code_3_1.s` was split at this exact point so functions *after* it
could still be extracted to C: `asm/code_3_1.s` now holds only
`sub_80007EC`, followed in `ldscript.txt` by the newly-matched
function's own object file, then a new `asm/code_3_1_2.s` picking up
where `sub_80007EC` ends (`sub_80008CC` onward) - the address layout
this relies on was verified with a full `rm -rf build && make compare`.
This is the first split of its kind in the project; reuse the same
pattern (trim the tail of the `.s` file at the boundary, start a new
`_2.s` with the same three-line header, add one `ldscript.txt` line)
whenever a hard-to-match function needs to be skipped without blocking
everything after it.

Seventh matched function: `sub_80008B4` (ROM `0x080008B4`, immediately
after `sub_80007EC`; lives in new file `src/util/math_util.c`, since it's not
yet called by anything matched and didn't fit thematically in
`irq.c`) - a fixed-point squared-distance-style helper:
`((dx>>8)^2 + (dy>>8)^2) << 8`. Matched first-try structurally; needed
statement order matching the ROM's own dx-then-dx² compute-fully-before-
dy pattern (an initial attempt that computed both `dx`/`dy` before either
squaring reordered the two multiplies together instead of interleaving).

Six more matched in the same file right after, all first-try:

- `sub_80008CC` - same `dx`/`dy` squared-distance math as `sub_80008B4`,
  fed through `sub_803A95C` (presumably an integer square root, unmatched
  so far) and the low 16 bits of the result rescaled back up by 8 - an
  actual (non-squared) distance.
- `sub_80008F0` - trivial wrapper, `sub_803ADB4(arg0 << 8, arg1)`.
- `sub_80008FC` - halves whichever of its two arguments is larger before
  multiplying them (an overflow-avoidance trick for a scale/interpolation
  calculation), `a > b ? (a>>8)*b : a*(b>>8)`.
- `sub_800090C`/`sub_8000924`/`sub_800093C` - three small wrappers/helpers
  around `sub_803ADB4`, sign-extending 16-bit operands in and the result
  back out. `sub_803ADB4` itself looks like an atan2-style angle lookup
  (see its use in `sub_800697C` as `sub_803ADB4(total * 100, 0x48)` in
  `src/graphics/graphics.c`), which fits `sub_80007EC` calling `sub_800090C` twice
  to get two BG2 affine scale/rotation parameters from an angle.

Eighth matched function (after a second pass): `itoa`, a custom
itoa (int-to-string, with a fast path for base 16 using bit-AND +
arithmetic-shift instead of a division call, falling back to a
`sub_8000140` divmod helper for other bases, then reversing the digits
in place). Lives in new file `src/util/string_util.c`. This one needed
**every** local pinned to a specific register to match - a good worked
example of the technique 5 warning in `matching_decomp_register_pinning`
memory (the same *kind* of value needing different pins at different
points):

- `v`/`buf`/`baseR` (r3/r6/r4) hold the value/buffer/base parameters -
  unpinned, gcc put `value` in r2 and only homed `buffer`/`base` into
  r6/r4 *before* `v`'s own assignment, instead of after it like the ROM.
- `len`/`negative` needed only `negative` pinned to r5, leaving `len`
  unpinned - gcc's own (unpinned) allocator then puts `len` in r7 on its
  own, which is what actually keeps r7 safe here: a *pinned* r7 never
  gets included in the prologue's push/pop (confirmed again this
  session, see `sub_8006600`'s notes above), but gcc's own unforced
  choice always does. Every other local in this function landing on
  r4-r7 is either never pinned there directly (`len`) or has a lifetime
  that genuinely survives a call within the function (`v`/`buf`/
  `baseR`/`negative`/`j`), so none of them hit that hazard.
- `rem` (r1) needed to stay a plain `s32` (not `u8`) and be stored back
  into itself (`rem = rem + 0x37`) rather than into a separate `u8 c`
  variable - the type change alone eliminated a stack round-trip gcc
  otherwise inserted for the narrowing store, and removed a
  byte-truncation pair (`lsl`/`lsr` by 24) before the final `strb`.
- The `rem <= 9 ? +0x30 : +0x37` branches needed swapping to `rem > 9 ?
  +0x37 : +0x30` to match which branch the ROM falls through to
  unconditionally vs. jumps to.
- The final reversal loop needed its own pins: `i`/`j` (r1/r4, reusing
  the exact registers `rem`/`baseR` used earlier once those are dead)
  for the strlen-style length scan, then a *separate* variable `k`
  pinned to r5 (reusing `negative`'s dead register) for the actual swap
  loop - and the swap body needed named pointer locals (`u8 *p1 =
  &buf[k]; u8 tmp = *p1; u8 *p2 = &buf[j]; u8 val = *p2; *p1 = val; *p2
  = tmp;`) in that exact interleaved order (address, load, address,
  load, store, store) to get gcc to compute each address once and reuse
  it, matching the ROM - a direct `buf[k] = buf[j]; buf[j] = tmp;`
  recomputed one address twice.

All told: ~10 compile-and-diff iterations, but every single mismatch
traced back to a technique already in memory (per-site low-register
pins reused across dead-variable boundaries, the r7-must-stay-unpinned
rule, and address-reuse-via-named-pointers) rather than anything new.

Ninth matched function: `sub_80009F4` (ROM `0x080009F4`, right after
`itoa`, same file). A printf-style single-conversion formatter:
parses an optional width from `*fmt` (digits immediately before the
specifier), reads a `'d'`/`'x'`/`'X'` specifier, calls `itoa` to
render the referenced value into a stack buffer at the parsed base,
left-pads with a caller-supplied fill character to reach the width,
appends the number, and writes back how many format characters it
consumed through a 5th (stack-passed) out-parameter. New findings this
time, all now folded into technique 6 and the general register-pinning
approach in `matching_decomp_register_pinning` memory:

- A `switch` on the specifier had to use `goto` to a single shared call
  site (each case setting up its own value/buffer/base locals, then
  jumping to one shared `itoa(...)` call) rather than `break` -
  agbcc's cross-jump merging only unifies a *literal-identical tail*
  working backward from the branch target until the first difference;
  with `break`, each case's call is a separate, unmerged copy, while the
  `goto`-to-shared-label form produces exactly the ROM's shape (one
  physical `bl`, reached by an explicit jump from one case and a
  fall-through from the other). Whatever's assigned to a per-branch
  local *before* reaching the merge point stays duplicated per branch
  even when the value is identical between branches (matching the ROM,
  which repeats `mov r1, sp` in each case) - only referencing the
  variable *at* the merge point lets gcc share its computation.
- The `default` case needed an explicit `goto` past `width -=
  digitCount` (skipping straight to the unconditional `width -= 1`
  after it), not `digitCount = 0` followed by a normal (no-op)
  subtraction - the ROM has no subtraction instruction at all for the
  no-specifier-matched case.
- The out-parameter write (`*charsConsumedPtr = ...`) had to move to
  right before the `return`, keeping the computed value alive in a
  register (r1) across the entire padding loop and copy loop, rather
  than storing it immediately after computing it - the ROM keeps it
  live in a register the whole time instead of writing it out early.
- Two spots needed a literal constant re-materialized in a register via
  a pin/inline-asm even though an already-live register held the exact
  same value from an unrelated preceding comparison (a `-1` sentinel,
  and a `0` byte for the final NUL write) - plain C recognized the
  value was already available and reused it instead, saving an
  instruction the ROM doesn't save.

Tenth matched function: `sub_8000AA8` (ROM `0x08000AA8`, right after
`sub_80009F4`) - the actual printf-style driver these last several
functions were building toward. Lives in new file `src/util/printf_util.c`.
Walks `fmt`, echoing literal characters, and dispatching `%`-conversions
through a jump table: `%s` (string copy), `%c` (single byte from the
arg array - every slot is 4 bytes regardless of the value's real size),
`%d`/`%x`/`%X` (via `itoa`), `%<digit>...` (space-padded width,
via `sub_80009F4`), and `%0...` (zero-padded width, same). Args are a
raw `u32 *` array, not real varargs.

Three genuinely new findings, on top of everything from
`sub_80009F4`'s writeup:

- The manual bounds pre-check that seemed necessary at first (checking
  `(u8)(c - '%') > 0x53` before the `switch`) turned out to *be* the
  ROM's own switch-statement bounds check in disguise - once an
  explicit `case '%':` was added (mapped to the same body as `default`,
  widening the switch's case range down to `'%'` itself rather than
  starting at `'0'`), agbcc generated that exact single check on its
  own and the manual pre-check became redundant *and wrong* (it produced
  a second, narrower check in addition to the switch's own one).
- Case-block **physical layout order in the source** matters, not just
  which code runs for which case: the `'%'`/`default` case had to be
  written *last* in the switch (matching where the ROM physically places
  its equivalent block, immediately before the loop's read-next-char
  step) rather than first - agbcc lays out non-jump-table code in
  source order, so the case textually closest to a shared successor
  becomes the one that "falls through" to it for free, while every
  other case gets an explicit trailing branch. Writing the same case
  first instead made a *different* case fall through, changing which
  branches were explicit and by how many bytes.
- The two `charsConsumed` variables needed separate block-scoped
  declarations (one per case), matching the ROM's two distinct stack
  slots, rather than one shared variable reused across both cases (see
  the file for detail already captured there).
- The function is declared `void`, not `u8 *`, even though it looks
  exactly like it should return the advanced `dest` pointer (and every
  other function in this call chain does) - the ROM's epilogue never
  sets up r0 before its `pop {r0}; bx r0` return dance, leaving whatever
  was last written there (0, from the terminating NUL) to be silently
  discarded by the caller. Caught by comparing the tail of the compiled
  output against a fresh objdump of the exact ROM bytes at that address
  (not the write-up notes from reading the disassembly a while
  earlier), since a `u8 *` return produces an extra, very plausible-
  looking `adds r0, r4, #0` instruction that's easy to assume belongs
  there.

Eleventh: `sub_8000CA8` (ROM `0x08000CA8`, right after `sub_8000AA8`) -
a genuine variadic wrapper (`void sub_8000CA8(u8 *dest, u8 *fmt, ...)`)
forwarding straight to `sub_8000AA8` with a pointer to the first
vararg. Matched first-try using this toolchain's real `<stdarg.h>`
(`va_list`/`va_start`/`va_end`, backed by `__builtin_next_arg`) - no
project code had used variadics before this. Needed the same trailing
`asm(".align 2, 0")` fix as `itoa` for the padding byte after it.

**Parked, not matched: `sub_8000CBC`** (ROM `0x08000CBC`, right after `sub_8000CA8`, same file).
A case-insensitive `strstr`: `u8 *sub_8000CBC(u8 *haystack0, u8 *needle,
s32 caseInsensitive)` scans `haystack0` for the first occurrence of
`needle`, lowercasing both sides byte-by-byte before comparing whenever
`caseInsensitive` is nonzero (`(u8)(c - 'A') <= 0x19` is the ROM's own
range check for `'A'`-`'Z'`), returning a pointer into `haystack0` at
the match or `0` if not found or if `needle` is empty. Kept in
`printf_util.c` (not printf-related) purely to preserve ROM address
order right after `sub_8000CA8` without another `ldscript.txt` split.

Register findings: `haystack` (the outer scan cursor) is pinned to
`r5` and the outer loop's post-case-fold char to `r3`, matching the ROM;
both are plain scratch here since neither is live across a call.
`needleRest` (a copy of `needle + 1`, the fixed start-of-match cursor
used by the inner comparison loop) is pinned to `ip` (r12) - the ROM
computes it as a plain copy of `needle` *before* reading the first
needle char, then increments it *afterward* via a separate `movs r0,
#1; add ip, r0` (the only way to add an immediate to a high register in
Thumb); `needleRest = needle + 1;` as one C expression instead computes
the sum in a low register first and copies the result into `ip`, which
is shorter but not what the ROM does, so the increment is spelled out
as inline asm (`asm volatile("mov r0, #1\n\tadd %0, %0, r0" :
"+r"(needleRest) : : "r0")`) right after the plain copy. Statement
*order* matters too: `needleRest = needle;` must be assigned before
`haystack = haystack0;` - confirmed directly against `baserom.gba` via
`objdump` (`mov ip, r1` precedes `adds r5, r0, #0`), correcting an
earlier misreading in this same session.

**Unresolved conflict blocking a full match**: the four "normalize a
char to lowercase" blocks (first-char, outer-loop char, and the inner
loop's two chars) each have a subtlety in exactly how the *unchanged*
path is shaped. The ROM computes the fold as `x = *p; p++;` then, on
*both* branches of the range check, routes the value through `r0` and a
shared truncate-back-to-`u8` at the end - even the "value already in
range" branch re-copies it through `r0` before falling into that shared
truncate, rather than skipping straight past. Reproducing this exactly
needs an `s32 t = x; if (cond) t = x + 0x20; x = (u8)t;` shape (a signed
temp so the truncate isn't optimized away as a no-op) - verified in
isolation to produce the right shape - but applying it to all four
blocks in the real function pushes register pressure just far enough
that `caseInsensitive` (correctly in `r7` otherwise) spills into `r8`,
forcing an extra `mov r7, r8; push {r7}; mov r8, r2` prologue/epilogue
dance the ROM doesn't have - a worse mismatch than the branch-shape one
it fixes. The simpler `if (cond) x += 0x20;` form (checked in) keeps
every register correct except this branch shape in the 4 blocks.
Whichever register ends up hosting the `s32 t` temp needs to be pinned
without disturbing anything else already correct here; not yet found.

**Build toggle**: this function's C definition in `src/util/printf_util.c` is
wrapped in `#if NON_MATCHING`, and the corresponding raw bytes in
`asm/code_3_1_2.s` are wrapped in `.if NON_MATCHING == 0` / `.endif`
(same pattern as `sub_8006600`, see above), so exactly one definition is
ever assembled. Default builds (`make`/`make compare`) get
`NON_MATCHING=0` and use the checked-in matching assembly (verified via
a clean `make compare`); `make NON_MATCHING=1 crashbandicootxs.gba`
compiles this C version in instead (verified this session to compile
and link cleanly with no duplicate-symbol errors).

Twelfth matched function: `CountNonSpaceChars` (ROM `0x08000D68`, right after
the still-parked `sub_8000CBC`) - counts the non-space characters in a
NUL-terminated string (`s32 CountNonSpaceChars(u8 *s)`; spaces are skipped,
not counted, everything else including walking off the terminator is).
Matched first-try structurally. Needed the same trailing
`asm(".align 2, 0")` fix as `itoa`/`sub_8000CA8` - the raw
function is 22 bytes (11 instructions) plus a 2-byte pad NOP to reach
the next function's 4-byte-aligned start, and without the explicit
alignment directive agbcc doesn't emit that trailing NOP.

Extracting this one function required a second split of the same kind
as `sub_80007EC`'s: it sits between `sub_8000CBC` (parked, still raw in
`asm/code_3_1_2.s`) and `strcat` onward, so `asm/code_3_1_2.s` was
trimmed to end right after `sub_8000CBC`'s `.endif`, everything from
`strcat` on moved to a new `asm/code_3_1_3.s` (same three-line
header), and `CountNonSpaceChars` itself lives in a new `src/util/string_util2.c`
(not `string_util.c` - that object already links *before*
`printf_util.o`/`code_3_1_2.o` in `ldscript.txt`, which would put this
function's code at the wrong address; a fresh translation unit was the
only way to get its object linked exactly between `code_3_1_2.o` and
`code_3_1_3.o`). `ldscript.txt` now lists, in order:
`code_3_1_2.o`, `string_util2.o`, `code_3_1_3.o`. Verified with a full
`rm -rf build && make compare` (and a `NON_MATCHING=1` build to confirm
`sub_8000CBC`'s parked toggle still links cleanly around the new
split).

Thirteenth matched function: `strcat` (ROM `0x08000D80`, right
after `CountNonSpaceChars`, same file) - `strcat`: appends `src` to the end of
`dst` in place and NUL-terminates the result (`void strcat(u8
*dst, u8 *src)`). The ROM finds the end of `dst` via an *index* rather
than a walked pointer (`p[i]`, incrementing `i`, with `p` fixed), then
converts to a pointer once (`p + i`) for the copy loop - and critically
uses a *different* register for that resulting pointer (`r2`, the same
register `i` was just using, now dead) rather than writing the sum back
into `p`'s own register (`r3`). Matching this needed `p`/`i` explicitly
pinned to `r3`/`r2` (plain scratch, leaf function) *and* the
post-search pointer split into its own block-scoped variable `q`,
itself pinned to `r2` - writing `p = p + i;` into the same `p` variable
compiles fine but keeps everything in `r3,` one register off from the
ROM. Needed the same trailing `asm(".align 2, 0")` fix as the others in
this file.

Fourteenth matched function: `sub_8000DAC` (ROM `0x08000DAC`, right
after `strcat`, same file) - `strncpy`-like: copies at most `n`
bytes from `src` into `dst` (`void sub_8000DAC(u8 *dst, u8 *src, s32
n)`), stopping early at `src`'s NUL terminator, and NUL-terminates
`dst` only if the copy stopped early (fewer than `n` bytes actually
copied) - unlike real `strncpy`, it never pads `dst` out to `n` bytes.
Matched first-try (no alignment fix needed - ends exactly 4-byte
aligned already); the ROM's `-1` sentinel (computed via `movs r4, #1;
rsbs r4, r4, #0` since Thumb has no negative-immediate `mov`) shows up
naturally from writing the loop bound as a plain `n != -1` compare.

Fifteenth and sixteenth matched functions, both first-try: `strcpy`
(ROM `0x08000DE0`, right after `sub_8000DAC`) - a plain `strcpy` (`void
strcpy(u8 *dst, u8 *src)`) - and `strlen` (ROM `0x08000DF8`,
right after it) - a plain `strlen` (`s32 strlen(u8 *s)`, the same
"walk an index, not a pointer" shape as `CountNonSpaceChars`'s search loop).
`strlen` had no `thumb_func_start` label of its own in the original
raw `asm/code_3_1_3.s` - it just ran on as unlabeled bytes right after
`strcpy`'s trailing pad NOP - confirmed via a direct
`baserom.gba` objdump that it's real code (not data), likely just never
called via `bl` from anything disassembled yet so whatever tool
originally produced this file didn't detect the boundary. Gave it the
`strlen` name (its ROM address) like every other function here.

Seventeenth through nineteenth matched functions, all first-try, in new
file `src/util/rand_util.c` (RNG, doesn't fit any existing file): `srand`
(ROM `0x08000E10`, right after `strlen`) seeds a global LCG state
(`gUnknown_030007E4` in IWRAM) with its argument; `rand` (ROM
`0x08000E4C`) advances that LCG (`seed = seed * 0x41C64E6D + 0x3039` -
the standard C library constants) and returns 16 bits from the middle
of the new seed (`(u16)(seed >> 4)`, i.e. the ROM's `lsls #0xc; lsrs
#0x10` pair - avoids the LCG's low bits, which are the least random);
`sub_8000E1C` (ROM `0x08000E1C`, sitting *between* the two, despite
being logically "based on" `rand`) does the same seed advance
inline (not by calling `rand` - the ROM has two physical copies
of these 8 instructions) and forwards the result plus its own `s32
max` argument to `sub_803AF1C` (not yet matched or confidently typed
beyond this call site's `u16, s32 -> u16` shape) - likely a
"random number in `[0, max)`" helper. `sub_8000E1C` needed its return
type declared `u16` (not `s32`) to reproduce the ROM's post-call `lsls
r0, #0x10; lsrs r0, #0x10` truncation of `sub_803AF1C`'s result; without
it, gcc has no reason to truncate a call result it's about to return
as-is.

Removed these three functions' raw bytes directly from the existing
`asm/code_3_1_3.s` (no further file-splitting needed - they sit
entirely inside one already-open file) and added one `ldscript.txt`
line for `rand_util.o` between `string_util2.o` and `code_3_1_3.o`.

Twentieth matched function: `InitBresenhamLine` (ROM `0x08000E6C`, right after
`rand`) - Bresenham-line setup, in new file `src/util/line_util.c`
(doesn't fit any existing file). Given a `struct bresenham_line *` with
`x0`/`y0`/`x1`/`y1` already filled in, computes `dx`/`dy`, records each
axis's step direction (`sx`/`sy`, `+1`/`-1`/`0`) and the absolute
deltas, then - depending on which delta is larger - fills in three more
`s32` fields (an initial error term, a doubled-delta reload value, and
a doubled-difference decrement, all standard Bresenham quantities) plus
a `u8` "which axis is driving the walk" flag. Matched first-try;
non-input fields are left as `field_N` (not e.g. `err`/`step`) since no
caller has been matched yet to confirm their actual roles. Removed the
function's raw bytes directly from `asm/code_3_1_3.s` (no further
splitting needed) and added one more `ldscript.txt` line
(`line_util.o`, between `rand_util.o` and `code_3_1_3.o`).

**Parked, not matched: `sub_8000EE4`** (ROM `0x08000EE4`, right after `InitBresenhamLine`), in new
file `src/graphics/text_layout.c`. A text-layout/word-wrap renderer: walks a
NUL-terminated string one "token" at a time (`GetWordLength` returns each
token's byte length - looks like it splits on word boundaries), drawing
each token through the OAM-icon system (`sub_803AD84`, returning the
token's pixel width; a second call at a different record slot appears
to draw a cursor/highlight) while accumulating a running pixel width
against a per-line budget (`box->field_8`). When the running width would
overflow, it advances to a new "line" (drawing a newline marker via
`sub_803AD80` at a third record slot, and re-drawing the just-measured
token at the line's start) and optionally "flushes" (`sub_80006A8` then
`sub_8006AAC(gUnknown_03001300)` - the same OAM-shadow-buffer flush
pattern used elsewhere) depending on a `mode` parameter (0 = never
flush per-token, 1 = flush after every token, 2 = only flush after a
line wrap) - and unconditionally flushes once more after the whole
string is consumed if `mode != 0`. Recognizes two escape sequences,
`/b` (nudge the render Y position down by 4, a half-line break) and
`/n` (full newline - same drawing as an overflow-driven line advance),
both introduced by a literal `/` byte in the text.

Struct `sub_8000EE4_box` (the 3rd parameter) has only `field_0`/
`field_4` (the starting X/Y, copied into the render-target object's
`0x110`/`0x114` fields before the loop starts) and `field_8` (the
per-line pixel-width budget) named by use; the render-target object
itself (2nd parameter) isn't given a named struct at all - just raw
`u8 *self + offset` arithmetic throughout, matching the style already
established in `sub_8006770` (`src/graphics/oam_count.c`) for the exact same
"record is an array of 8-byte `{s16, pad, void *}` entries, 0x10 bytes
apart" shape - `self->0x130` holds a pointer to that array, and the
function reads 3 different entries from it (`+0x18`/`+0x28`/`+0x38`)
depending on what it's drawing.

Register findings: `self` pinned to `r8`, `cursor` (the running text
pointer) to `r10`, `token` (the current token's start, snapshotted each
iteration) to `r6`, and `charWidth` (the first `sub_803AD84` call's
returned pixel width) to `r9` - all matching the ROM, and all necessary
since each survives multiple calls. The two record-address caches
(`&self->0x110`/`&self->0x114`, used only in the `/b` handler) land in
plain stack slots in both the ROM and this reconstruction, computed
once and reloaded via pointer variables (`xAddr`/`yAddr`) rather than
recomputed from `self` each time. The record-array base address
(`&self->0x130`) is similarly cached once per branch into a plain local
(`fieldAddr`), reloaded through it on each subsequent access - matching
`sub_8006600`'s established address-caching pattern (see its notes
above) rather than letting `self->0x130` be recomputed fresh, which
would use more instructions than the ROM's single cached-address reuse.

Two important non-obvious behavioral details, both verified directly
against the ROM's bytes: the escape-sequence check reads the byte
*after* the `/` exactly once (`token++; c = *token;`, matching the
ROM's `adds r6, #1; ldrb r0, [r6]`) and reuses that one read for both
the `'b'` and `'n'` comparisons - an earlier attempt that read
`token[1]` twice (once per comparison) compiled to an extra load plus a
redundant sign-extension shift-pair, a worse mismatch. Second: in the
overflow ("wrap to a new line") branch, the freshly-reset `widthAccum`
is set from the *first* `sub_803AD84` call's cached return value
(`charWidth`, still held in `r9`), **not** the second (redrawing) call's
return value in that same branch - the ROM explicitly discards the
second call's `r0` and reuses `r9` instead (`mov r0, r9` right after
the call, ignoring what the call just returned).

**Unresolved (~8 bytes)**: everything above matches the ROM
instruction-for-instruction except two small, non-semantic codegen
details. First, the ROM moves `self`/`cursor` into their pinned
registers *before* `box`/`limit` get spilled to their stack homes, but
this reconstruction always produces the opposite order regardless of
where the assignment statements are written in the source - `box`/
`limit`'s stack spill appears to be an unconditional first step agbcc
takes for incoming stack-homed arguments, not something reachable from
C source ordering. Second, two of the loop-bound comparisons
(`posAccum >= limit` near the top of the function, `lineCount < limit`
at the bottom) compile here to a single inverted conditional branch,
but the ROM has a redundant two-instruction "compare in the natural
sense, branch on true to the very next instruction, then an
unconditional branch to the real (far) destination" pair at both spots
- most likely a Thumb conditional-branch encoding range limit (+-256
bytes) forcing the split in the ROM's original build once the function
reached its true size, not reproduced here since standalone tests of
the same shape never got large enough to trigger it either way. An
`asm volatile("" : "+r"(posAccum))` barrier right after zeroing
`posAccum` was needed regardless - without it, gcc notices `posAccum`
and `lineCount` are both provably `0` at the point of the *first*
check and cross-jump-merges it directly into the bottom check's code
(a bigger, wrong-shape mismatch, not just a register/branch-count
detail) - the same barrier applied to `lineCount` at the bottom check
trims the byte count further but introduces a new spurious stack store
there instead, so it's left out pending a real fix.

**Build toggle**: this function's C definition in `src/graphics/text_layout.c`
is wrapped in `#if NON_MATCHING`, and the corresponding raw bytes in
`asm/code_3_1_3.s` are wrapped in `.if NON_MATCHING == 0` / `.endif`
(same pattern as `sub_8006600`/`sub_8000CBC`, see above), so exactly
one definition is ever assembled. Default builds (`make`/`make compare`)
get `NON_MATCHING=0` and use the checked-in matching assembly (verified
via a clean `make compare`); `make NON_MATCHING=1 crashbandicootxs.gba`
compiles this C version in instead (verified this session to compile
and link cleanly with no duplicate-symbol errors).

Twenty-first matched function: `FormatCentiseconds` (ROM `0x0800106C`, right
after the still-parked `sub_8000EE4`), in new file `src/util/time_util.c` -
formats a centisecond count as `"MM:SS.X0"` into a 9-byte buffer (`void
FormatCentiseconds(s32 value, u8 *buf)`); only one fractional digit is
actually computed (`value % 10`) - the other is always `'0'`, so the
displayed precision is really just tenths of a second despite the
two-digit-looking field. Built on two not-yet-matched helpers,
`sub_803AF1C` (mod) and `sub_8037E54` (div) - both declared here with
plain `s32`/`s32` signatures despite `sub_803AF1C` already having a
`u16`/`s32`-typed extern declaration in `src/util/rand_util.c` for a
different call site; harmless; C linkage doesn't check parameter types
across translation units, and both signatures compile to the same
calling convention here anyway. Matched first-try structurally, needed
the same trailing `asm(".align 2, 0")` fix as the others in this
session.

Extracting this one function required the same kind of split as
`sub_8000CBC`'s and `CountNonSpaceChars`'s: `asm/code_3_1_3.s` was trimmed to
end right after `sub_8000EE4`'s `.endif`, and everything from
`sub_80010E0` on moved to a new `asm/code_3_1_4.s`, with
`src/util/time_util.c`'s object linked between them in `ldscript.txt`.
(`asm/code_3_1_4.s` was later removed again - see `sub_80010E0`'s own
notes just below - once it turned out to hold only one function that
also needed parking.)

**Parked, not matched: `sub_80010E0`** (ROM `0x080010E0`, right after `FormatCentiseconds`), in new
file `src/system/input_util.c`. Polls input (the same `sub_80006A8`-then-
`sub_80007AC` VBlank-wait-and-update-keys pair used elsewhere) until a
button matching `mask`'s bit 0 (confirm) or bit 3 (cancel) is newly
pressed, or - if `count != 0` - until `count` polls elapse; returns 0
only on a cancel press, 1 on everything else (confirm press, or hitting
the poll limit). A `checkButtons` byte parameter gates whether the
per-poll button checks happen at all (with it clear, this is just a
plain `count`-poll delay, or an immediate no-op return if `count` is
also 0). Needed `keys` pinned to `r1` and set via an inline-asm copy of
`mask` (`asm volatile("add %0, %1, #0" ...)`) rather than a plain `keys
= mask & load` - gcc's own codegen for the combined expression loads
straight into `r1` and ANDs with the mask register in place, one
instruction shorter than the ROM's redundant `adds r1, mask, #0`
followed by a separate `ands`; the `keys &= 8;` bit-8 checks are
likewise written as in-place ANDs (not `if ((keys & 8) != 0)`) so the
result lands back in `keys`'s own register, matching the ROM's `ands
r1, r0` instead of a fresh scratch register.

**Unresolved (4 bytes)**: with all of the above, only one instruction
differs from the ROM: the count-limited loop's `if (keys & 1)` bit-test
compiles here as `bne done; b continue` where the ROM has the opposite
sense, `beq continue; b done` (same two instructions, same size, just
inverted). Every rephrasing tried - swapping `==`/`!=`, splitting into
explicit `goto`-only chains with no `if`/`else` at all, reordering this
whole loop variant before or after the unlimited-loop version in the
source - produced identical output; the *same* bit-test in the
unlimited-loop variant a few lines below already matches the ROM's
sense with no special handling at all, which points at a fixed gcc-2.9
canonicalization for this exact shape rather than something reachable
from this file's C.

**Build toggle**: this function's C definition in `src/system/input_util.c` is
wrapped in `#if NON_MATCHING`, and the corresponding raw bytes - now
living in `asm/code_3_1_5.s`, right before `LoadTaggedAsset` (since
`asm/code_3_1_4.s`, which held only this one function, was removed
entirely and its ldscript slot given to `input_util.o` instead) - are
wrapped in `.if NON_MATCHING == 0` / `.endif`. Default builds get
`NON_MATCHING=0` and use the checked-in matching assembly (verified via
a clean `make compare`); `make NON_MATCHING=1 crashbandicootxs.gba`
compiles this C version in instead (verified this session to compile
and link cleanly with no duplicate-symbol errors).

Twenty-second matched function: `LoadTaggedAsset` (ROM `0x08001174`,
right after the still-parked `sub_80010E0`), in new file
`src/system/asset_util.c`. Loads (or raw-copies) an asset based on a tag in
its first word's high nibble: `0` = uncompressed (a manual DMA3 setup -
`SAD`/`DAD`/`CNT` written directly through a `vu32 *` at `0x040000D4`,
word-sized transfer, byte count taken from the header's remaining 24
bits), `1` = LZ77 (via not-yet-matched `LZ77UnCompWrapper`), `3` =
run-length (via not-yet-matched `RLUnCompWrapper`); any other tag value
is a silent no-op. Written as a `switch` rather than an if/else chain
specifically to reproduce the ROM's literal compare sequence (`cmp #1;
beq; cmp #1; blo; cmp #3; beq; b` - checking `case 1` twice against the
same immediate, once for equality and once for "below") - an
if/else-based `type == 1` / `type < 1` / `type == 3` chain compiles to
a shorter, differently-ordered set of comparisons instead (gcc
canonicalizes `< 1` on an unsigned value into a single `== 0` check).
`case 0`'s body is listed *first* in the switch source (despite `case
1` being checked first via the initial `beq`) to match the ROM's
physical layout, which puts case 0's body right after the compare
chain - the by-now-familiar "agbcc lays out switch bodies in source
order, not check order" rule from `sub_8000AA8`'s notes. Needed the
usual trailing `asm(".align 2, 0")` fix.

Extracting `LoadTaggedAsset` uncovered a genuine ordering bug from
parking `sub_80010E0` earlier this session: that function's raw bytes
(guarded `.if NON_MATCHING == 0`, so *included* in the default build)
had ended up sharing one `asm/code_3_1_5.s` file with everything after
it, including `LoadTaggedAsset` and `LoadBackgroundTileAndPalette` onward - fine as long
as `LoadTaggedAsset` stayed raw too, but once it moved to C in
`asset_util.o`, the linker had no way to slot that object *between*
`sub_80010E0`'s raw bytes and `LoadBackgroundTileAndPalette`'s (both still in the same
`code_3_1_5.o`), producing a build that linked and passed size checks
but put `LoadTaggedAsset` at the wrong address (silently breaking every
call to it - caught via a direct `cmp -l`/objdump diff showing a `bl`
target pointing at `sub_80010E0`'s address instead). Fixed by splitting
`asm/code_3_1_5.s` again, right after `sub_80010E0`'s `.endif`, into
itself plus a new `asm/code_3_1_6.s` (`LoadBackgroundTileAndPalette` onward), with
`asset_util.o` linked between them. **Lesson**: when a parked function's
raw-bytes file also holds *later, still-to-be-matched* functions,
extracting one of those later functions to C always needs its own
split at that exact boundary - the parked function's raw bytes can
never end up sharing an object with something that no longer sits
immediately next to it in the final link.

Twenty-third matched function: `LoadBackgroundTileAndPalette` (ROM `0x080011C0`, right
after `LoadTaggedAsset`), also in `src/system/asset_util.c` - loads one
background's tile/tileset data (the tagged asset at `asset + 0x200`,
via `LoadTaggedAsset`) into VRAM at `0x06000000`, then DMAs the first
`0x200` bytes of `asset` itself (a raw 256-halfword palette) straight
into palette RAM at `0x05000000`. Matched first-try.

Twenty-fourth matched function: `GetWordLength` (ROM `0x080011F4`, right
after `LoadBackgroundTileAndPalette`), in new file `src/util/word_util.c` - returns the
length of the next "word" starting at `s`: the count of characters up
to and including the first space, or up to (but not including) the NUL
terminator if no space comes first. This is exactly what the still-
parked `sub_8000EE4` (`src/graphics/text_layout.c`) uses to walk text one token
at a time. Needed a single shared `goto done;` return point (matching
the ROM's one `bx lr`) rather than three separate `return` statements,
which otherwise compile to three separate epilogues.

Extracting this one function needed the same kind of split as before:
`asm/code_3_1_6.s` (which held only `GetWordLength`) was trimmed to
nothing and removed, its ldscript slot going to `word_util.o`, with
everything from `sub_8001214` on moved to a new `asm/code_3_1_7.s`.

Twenty-fifth matched function: `sub_8001214` (ROM `0x08001214`, right
after `GetWordLength`), also in `src/util/word_util.c` - a thin wrapper around
the still-parked `sub_8000EE4`: stashes one field from its `params`
struct into the render-target object's own `field_118`, computes a
line-count limit (`params->field_c / self->field_11c`), then forwards
to `sub_8000EE4` with that limit and **returns its result** - genuinely
`s32`, not `void`, even though the one call site matched so far
(`sub_8006600` in `src/graphics/oam_count.c`, still parked) ignores it. Caught
via the epilogue: an initial `void`-returning version compiled the
final "restore LR and branch" step through `r0` (`pop {r0}; bx r0`),
one register off from the ROM's `pop {r1}; bx r1` - changing the return
type to `s32` and actually `return`ing `sub_8000EE4`'s result fixed it,
since `r0` then holds a live value (the forwarded return) that the
epilogue must preserve, forcing `r1` for the restore instead. Also
needed the store to `self->field_118` split into two statements (`s32
v = params->field_0; *addr = v;`) rather than one combined expression,
to get the ROM's exact register letters for the address/value pair
(and, as a side effect, made gcc reuse the already-shifted `0x118`
constant plus 4 for the `field_11c` offset instead of recomputing it
from scratch, incidentally also matching the ROM there).

Twenty-sixth matched function: `StepBresenhamLine` (ROM `0x08001254`, right
after `sub_8001214`), in new file `src/util/line_util2.c` - advances a
Bresenham line (set up by `InitBresenhamLine`, `src/util/line_util.c`) by one
step: the "driving" axis (`x0` if `flag` is set, `y0` otherwise) always
advances by its sign; the other axis advances only when the
accumulated error term (`field_10`) is positive, in which case the
error term is corrected by `field_18` instead of `field_14`. Kept in
its own file (redefining the same `struct bresenham_line` locally
rather than sharing `InitBresenhamLine`'s) purely because `line_util.o`
already links much earlier in `ldscript.txt` and this function's
address requires it to come after `word_util.o` instead. Two things
needed fixing versus a first attempt that seemed to match on casual
inspection but didn't: the error term must be re-read fresh inside
*each* of the two `flag` branches (not hoisted above the `if (flag)`
check, matching the ROM's two separate reloads), and - the actual bug -
each branch's `if (err > 0) {...} else {...}` needs to be written as
`if (err <= 0) {...} else {...}` instead: agbcc always lays out an
`if`'s true branch inline (no jump) and its `else` via a jump, so
writing the condition as `err > 0` put the wrong body first and
produced an inverted branch (`ble`/fallthrough-swapped) that still
*looked* plausible next to the ROM's `bgt` until directly diffed
byte-for-byte. Needed the usual trailing `asm(".align 2, 0")` fix.

Twenty-seventh matched function: `sub_80012AC` (ROM `0x080012AC`, right
after `StepBresenhamLine`), in new file `src/graphics/fade_util.c` - a per-frame
screen-brightness fade tick. Every `gUnknown_030007E8.field_0` frames,
writes the next step to `BLDY` (`0x04000054`), counting up or down
depending on `field_8`'s top bit (fade in vs. out); after 17 steps (a
full fade), resets both counters, briefly disables interrupts
(`0x04000208`, `REG_IE`) while resetting `field_0` to a literal `-1`
(not a decrement of whatever was there - confirmed by the ROM reusing
the register that's *already* holding `0` from a few instructions
earlier, computing `0 - 1` rather than reloading `field_0` first) and
calling `sub_8000670` with `field_4`, then re-enables interrupts.
`mask`/`flag8` are pinned to r0/r1 to match the ROM's exact register
choice for the `& 0x80` check - the natural (unpinned) allocation puts
the loaded byte in r0 and the constant in r1 instead, one register off,
regardless of which order the two operands are written in the C.

Twenty-eighth matched function: `sub_800132C` (ROM `0x0800132C`, right
after `sub_80012AC`), also in `src/graphics/fade_util.c` - starts a screen fade.
`flags` bit 0 selects the blend target (`BLDCNT`, `0xBF` vs `0xFF`),
bit 7 selects direction (fade in from `0x10` vs fade out from `0`);
`frameDelay` (clamped to at least 1) is how many frames each of the 17
steps takes. Refuses to start if a fade is already running, checked via
the same `(~x + 1) | ~x < 0` "`x != -1`" bit-trick as before (needed
here too, spelled out with explicit temporaries - a direct `!= -1`
compiles to a shorter load-and-compare). If `sync` is nonzero, it
registers `sub_80012AC` as a periodic callback (via the already-matched
`sub_8000680`) and returns immediately; otherwise it blocks here,
looping through all 17 steps itself and busy-waiting `frameDelay`
VBlanks (`sub_80006A8`) between each. That inline loop needed the same
"separate next-iteration variable" idiom seen in `sub_8000EE4`'s
notes - `i++` in place reuses one register for the whole loop, but the
ROM computes `next = i + 1` into a *different* register partway
through, then feeds it back as `i` only at the very end (freeing the
original register for reuse in between); this looked functionally
identical either way and only showed up as a real mismatch once
directly diffed.

Twenty-ninth matched function: `sub_80013FC` (ROM `0x080013FC`, right
after `sub_800132C`), in new file `src/graphics/palette_blend.c` - blends the
whole 512-entry palette at `gUnknown_03000A80` toward black by
`factor`/16 per channel (5 bits each, GBA BGR555), writing the result
to `gUnknown_03000E80`. Each channel is extracted via an explicit
shift-left-then-shift-right pair and re-inserted via "clear those
bits, then OR the new value in" - even for pulling the initial raw
16-bit pixel into the `color` accumulator, which is never explicitly
zeroed (its stale value from the previous iteration gets masked away
by the same pattern) - rather than plain `&`/`|` on named bitfields,
matching the ROM's exact instruction shapes. This one needed unusually
heavy register/instruction pinning to get byte-exact: the channel
extraction's two shifts collapse into a single register when written
as one C expression (`(x << a) >> b`), but the ROM computes the first
shift into a *different*, temporary register before the second reads
from it; the post-subtract `(u16)` truncate before the final 5-bit mask
gets optimized away entirely by gcc (same result, but the ROM has a
redundant 16-bit truncate first); and channel 2/3's insert needs the
new value shifted into position *before* the mask constant is loaded
and ANDed, not after (same instructions, wrong order otherwise). All
three were only reachable via small register-pinned scratch blocks
(`tmp`/`diff`, both r0) and, for the truncate, literal inline
`lsl/lsr #0x10` asm - plain C phrasing kept getting "correctly"
optimized past the ROM's own redundant steps. Confirmed the
"mov r0, r1" vs. "adds r0, r1, #0" difference in agbcc's own hex-asm
listing (used throughout this comparison process) really is cosmetic,
not a byte difference - a real integration + `make compare` matched
byte-exact despite that textual mismatch persisting in the isolated
scratch-test comparisons up to the very last iteration.

### Cleanup pass over everything matched so far

After the run of matches above, a pass over `src/graphics/graphics.c`,
`src/graphics/oam_count.c` and `src/graphics/actor_anim.c` to tighten up readability
without touching generated code (`make compare` re-checked after every
edit below):

- **Hardware registers**: every raw address (`0x040000D4`, `0x07000000`,
  `0x05000000`, `0x04000010`/`0x04000050`/`0x04000054`/`0x04000000`) now
  goes through the existing `REG_ADDR_*`/`OAM`/`PLTT` constants from
  `include/gba/io_reg.h`/`defines.h`, and the DMA control words are built
  from `DMA_ENABLE`/`DMA_32BIT`/`DMA_16BIT` instead of the raw hex
  (`0x84000000` → `(DMA_ENABLE | DMA_32BIT) << 16`, etc.) - these headers
  were already in the project, just unused by the newly-matched code.
- **Named bounds instead of magic numbers**: `OAM_ENTRY_COUNT - 1`
  (already-defined, `128`) replaces the `0x7F` bound in `sub_8006A48`/
  `sub_8006AC8`; a new `DMA_QUEUE_MAX_ENTRIES` (`0x300`) replaces the
  `0x2FF` check in `QueueVramDmaTransfer` for the DMA queue's allocated
  capacity.
- **Struct consolidation** (checked for exactly this before adding
  anything new, per the point above about `struct oam_shadow_buffer`):
  - `struct sub_8006700_struct` and `struct sub_8006714_struct`
    (`oam_count.c`) turned out to be the same object at compatible
    offsets - `sub_8006770` independently confirmed `field_18` at the
    same address via raw pointer arithmetic. Folded into one
    `struct sub_8006700_actor` used by all three functions.
  - The standalone `struct sub_8006A78_struct` (`graphics.c`) is exactly
    the 12-byte header of the OAM-shadow-buffer object every other
    function in that cluster (`sub_8006A14`/`AAC`/`AC8`/`A48`/`B0C`) was
    still taking as bare `void *` - folded into one
    `struct oam_shadow_buffer` (with `field_00` renamed `count` to match
    how the other functions already treat it) and used as the parameter
    type everywhere in the cluster, including the ones that still need
    raw pointer casts internally for volatile/register-pinning reasons.
  - A new `struct threshold_table_entry` documents `gStaticData_0816C86C`'s
    layout for `sub_8006820`/`sub_8006864`/`sub_80067EC`. `sub_80067EC`
    now indexes through it with a typed pointer instead of a bare `u8 *`
    (confirmed this doesn't change codegen - the pre-biased-pointer
    pattern it needs survives the cast). `sub_8006820`/`sub_8006864`
    keep their inline-asm address computation as-is (switching those to
    plain `entry->threshold_0C`-style field access reintroduces the
    exact CSE problem documented above for them), but now have a comment
    naming which two threshold fields each one is bounds-checking.
- **Comments added, no code removed**: every register-pinned/inline-asm
  function got a one-line comment pointing at this doc instead of being
  silently unexplained - reducing the asm itself further isn't possible
  without breaking the match (each block here was arrived at only after
  exhausting plain-C rephrasing, per the entries above), so the fix for
  "this looks like unexplained magic" is documentation, not removal.

### Second cleanup pass (after CountNonSpaceChars through sub_80013FC)

Another readability pass over everything matched or parked since the
previous cleanup, again re-checking both `make compare` and
`make NON_MATCHING=1` after every edit:

- **Hardware registers**: `src/graphics/fade_util.c`'s raw `0x04000054`/
  `0x04000050`/`0x04000208` became `REG_BLDY`/`REG_BLDCNT`/`REG_IME` -
  the last one had been mislabeled as `REG_IE` in an earlier writeup
  (`0x04000208` is actually `IME`, the interrupt *master* enable, not
  the per-source `IE` at `0x04000200` - an easy mix-up since both are
  "the interrupt enable register" in casual terms, but the ROM's own
  "write 0, do a critical section, write 1" idiom here specifically
  needs the master switch). `src/system/asset_util.c`'s two raw
  `0x040000D4`-based `vu32 *dma` pointers became `struct dma_regs *`
  (see next point) through `REG_ADDR_DMA3SAD`.
- **Struct consolidation**:
  - `struct dma_regs` (`src/graphics/graphics.c`'s local `{ vu32 src, dst, cnt;
    }`) moved to `include/gba/dma_macros.h` (the header that already
    holds every other DMA-related macro) and is now shared by
    `src/system/asset_util.c`'s `LoadTaggedAsset`/`LoadBackgroundTileAndPalette` instead of each
    doing raw `vu32 *` + manual `[0]`/`[1]`/`[2]` indexing.
  - `struct bresenham_line` (independently declared, identically, in
    both `src/util/line_util.c` and `src/util/line_util2.c` purely because the
    two functions that share it link far apart) moved to a new
    `include/line_util.h`, included by both.
  - `struct icon_manager`/`struct icon_record` (`src/graphics/oam_count.c`,
    previously with `field_10`/`field_14`/`field_20`/`field_24` named
    directly on `icon_record`) moved to a new `include/icon_manager.h`
    and `icon_record` was reshaped into `struct icon_slot { s16 offset;
    u8 unused[2]; void *ptr; } slots[6]` - an 8-byte-stride array,
    confirmed by `sub_8000EE4` (`src/graphics/text_layout.c`, still parked)
    independently needing three *more* slots (`slots[1]`/`[3]`/`[5]`,
    at the offsets right in between the two `sub_8006600` already used)
    for its own per-glyph and newline-marker OAM draws. `text_layout.c`
    now takes a real `struct icon_manager *`/`struct icon_record *`
    instead of raw `u8 *self + <offset>` arithmetic throughout.
  - Checked for (but didn't find) a similar merge opportunity between
    `sub_8000EE4`'s `struct sub_8000EE4_box` and `sub_8001214`'s
    `struct sub_8001214_params` (`src/util/word_util.c`) - different field
    layouts (offsets 0/4/8 vs. 0/0xc), not the same object.
  - Left `gUnknown_030007E0` (`src/system/irq.c`)/`gUnknown_030007E4`
    (`src/util/rand_util.c`)/`gUnknown_030007E8`+`gUnknown_030007F4`+
    `gUnknown_030007F8` (`src/graphics/fade_util.c`) as separate globals despite
    being adjacent in IWRAM (`0x7E0`-`0x7F8`) - `irq.c`'s own notes
    already established that combining even just the first pair into
    one struct changes agbcc's literal-pool codegen for already-matched
    functions, so merging these needs much stronger evidence than
    "they're next to each other" before touching functions that
    currently match.

## `AllocVramDmaQueue`: a third piece of the DMA-queue system, mislabeled by ROM proximity

Matched `sub_8006C00` (right at the boundary between `graphics.c`'s
matched code and the raw `asm/code_3_2.s` chunk `docs/rom_map.md`'s
whole-ROM pass calls `game_loop`) as `AllocVramDmaQueue` in
`src/graphics/graphics.c`, right after `FreeVramDmaQueue` - the missing
constructor counterpart: allocates `DMA_QUEUE_MAX_ENTRIES *
sizeof(struct dma_queue_entry)` bytes via `mem_alloc`, stores the
pointer into `gUnknown_03001290.entries`, zeroes `.count`, and returns
`0`/`-1` on success/failure - the exact mirror image of
`FreeVramDmaQueue`'s `mem_free`+`NULL`-out. A concrete example of the
whole-ROM investigation's own "proximity is a weak signal" lesson: this
function's ROM address happens to sit at `game_loop`'s doorstep, but it
has nothing to do with that system - it belongs with the two other DMA-
queue functions already matched here, just never picked up before since
nobody had looked at this exact address yet.

One real gotcha, caught by the compile-and-compare step rather than by
reading alone: the disassembly's `movs r1, #0x80; lsls r1, r1, #0x17`
computes `0x80 << 23 = 0x40000000` (`MEM_HEAP_EWRAM`), not
`0x80000000` (`MEM_HEAP_IWRAM`) as an eyeballed first guess assumed -
`0x80`'s single set bit is at position 7, so a 23-bit shift lands it at
bit 30, not 31. Building with `MEM_HEAP_IWRAM` compiled to a shift-by-24
instead of shift-by-23, an instant, obvious diff against the real
disassembly. Since `graphics.o` compiles to the exact byte count needed
to land the next raw function (`sub_8006C28`) at the same address it
already had, no new `.c` file was needed and no other file's boundary
moved - only `tools/report_units.py`'s `game_loop` category start moved
forward by this function's size (`0x08006C00` -> `0x08006C28`).

## `sub_8006C28`-`sub_8006FB4`: a VRAM upload-cursor and a tile/palette asset cache

Matched the next 22 functions in ROM order too (`0x08006C28`-`0x08006FC8`),
resolving into two distinct 12-byte and 0x230-byte structs - both new
to `include/vram_pool.h`. This range sits at the very start of what
`docs/rom_map.md` calls the `game_loop` zone, but (same lesson as
`AllocVramDmaQueue` above) turned out to have nothing to do with it -
`tools/report_units.py`'s `game_loop` boundary moves again, to
`0x08006FC8`.

**`struct vram_upload_cursor`** (`sub_8006C28`-`sub_8006CE8`, 9
functions) is a bump/rollback cursor pair over a fixed
`OBJ_VRAM0_SIZE`-byte staging window - see the struct's own doc
comment in `vram_pool.h`. `sub_8006C38` has zero callers anywhere in
the codebase (confirmed by grepping every `asm/*.s` and `src/*.c`) -
matched anyway per the `mem_collect` precedent (see "Resolved:
`main.c`/`memory.c`/`irq.c`" above), not a mistake.

**`struct tile_asset_cache`** (`sub_8006D08`-`sub_8006FB4`, 13
functions) is a 16-slot raw-asset cache backing both an OBJ-tile-VRAM
upload path and an OBJ-palette-bank upload path through the same
32-byte-per-slot storage, depending on which function the caller
invokes - see the struct's doc comment. `sub_8006E64` is also
genuinely unreachable (zero callers, same as `sub_8006C38`).
`oam_count.c`'s existing `void *`-typed `extern`s for
`gUnknown_030012FC`/`gUnknown_030012B8` and
`sub_8006C28`/`sub_8006DC8` were retyped to the real structs as part
of this match (the cleanup-pass "check whether a struct for the same
object already exists elsewhere" rule) - `oam_count.c` was already
calling both, just through untyped pointers.

**Several register-allocation/instruction-order mismatches, all
resolved by the same three techniques already established above** (see
"A gotcha worth knowing"/further down): guard-clause polarity (writing
the *common* case as the branch-away target vs. the *fallthrough*
flips which comparison the compiler emits - `sub_8006C58`/`sub_8006C84`/
`sub_8006D50`/`sub_8006DF8`/`sub_8006E64` all needed the opposite
guard shape from an initial straightforward reading before matching),
explicit intermediate locals to force a specific evaluation order
(`sub_8006D08`'s `src`/`dst` split, `sub_8006DF8`'s `reservedBase`),
and - where plain C genuinely couldn't reproduce the ROM's exact
register/operand choice even after several rephrasings - targeted
`register asm("rN")` pins plus a one-line `asm volatile("add %0, %1,
%2" ...)` for the specific address computation
(`sub_8006D68`/`sub_8006D84`'s pending-slot address, `sub_8006DF8`'s
whole-function register layout since it makes no calls and gcc was
otherwise free to allocate however it liked, `sub_8006FB4`'s dirty-flag
address). One genuinely new fact caught only by compiling and diffing:
`sub_8006EF0`'s `count` parameter is `u16`, not `s32` as first assumed -
the ROM's `lsls r1,r1,#0x10; lsrs r7,r1,#0x10` is the truncation a
16-bit parameter forces on its incoming (possibly dirty-upper-bits)
register, absent entirely once the parameter was declared `u16`.

**`sub_8006FC8`/`nullsub_1`**: `sub_8006FC8` is a small mirror of the
already-matched `sub_8006AF4` (conditionally frees `arg0` based on an
odd/even flag in `arg1`), matched on the first attempt.  `nullsub_1` is
an empty stub whose 2-byte body isn't 4-aligned - the same
NOP-vs-zero-fill padding gotcha documented at the top of this file -
fixed with `asm(".align 2, 0");` right after the function.

**`sub_8006FE4`**: an actor-zone dispatch function (still
struct-less, raw `self` offsets - the `field+0x18 -> {s16 offset; ...;
void *text}` shape docs/rom_map.md has been seeing repeatedly in this
zone) that early-returns a flag byte or otherwise builds a 4-word
buffer and tail-calls `sub_803AD80(self + offset, buf, text)`. Two
real bugs surfaced only by compiling and diffing against the ROM
bytes: an extra dereference on `gUnknown_03001308` (the bare global
name already yields the stored pointer - no further `*` needed) and a
*missing* dereference on `self + 0x18` (that field itself holds a
pointer to another struct - `ldr r1, [r2, #0x18]` is a real load, not
pointer arithmetic). After those two fixes the function still ran two
instructions long, for two independent reasons:
- `self` landed in r3 under plain C phrasing where the ROM keeps it in
  r2 throughout (the function makes one call, so gcc was otherwise
  free to choose) - fixed with a `register void *pSelf asm("r2")` pin.
- The 3rd call argument (`text`) computed into a scratch register
  before argument-shuffling, adding an extra `add r2, r3, #0` the ROM
  doesn't have. Passing the two dereferences directly as call-argument
  expressions instead of through named locals let gcc load `text`
  straight into r2, matching the ROM exactly - this is the opposite of
  the "split into ordered locals" technique above: here inlining the
  expressions, not splitting them out, is what fixed the operand
  order.
- Separately, restructuring from two early-return statements into a
  single shared `u8 result` variable/`if`-`else`/one final `return`
  let both control-flow paths converge on the ROM's own trick: the
  early-return path's flag byte is already sitting in the same
  register (r1) that the truncated call result lands in on the main
  path, so the shared tail (`adds r0, r1, #0`) serves both paths with
  no branch instruction needed to skip it - two separate `return`
  statements had cost an extra unconditional `b`.

**`sub_8007048`**: another actor-zone function, same raw `self`
layout as `sub_8006FE4`. Builds an AABB into a stack buffer from a
record looked up via `sub_803AD7C`, then (conditionally, gated by a
flag bit and a `sub_800B37C` collision-style check) sets another flag
bit and fires off a `sub_803AD88` call using the same
`field+0x18 -> {s16 offset; ...; void *text}` table convention seen in
`sub_8006FE4` - except here the `text` field is read but genuinely
never used (no stack store, no argument register holds it after the
call) - a dead load the ROM itself performs, kept via a `register
void *asm("r4")` pin so the byte count matches. `sub_800B37C`'s return
type had to be `u8` (not `s32`) to reproduce the ROM's
`lsls r0,r0,#0x18` truncation before the boolean test - the same
pattern as `sub_800B37C`'s sibling checks and `sub_803AD80` and
`sub_8006FE4`'s own return value above.

Two flag-byte tests (`(byte >> 2) & 1`, and `byte | 8` stored back)
each needed inline asm rather than plain C: gcc's register choice for
which value is "scratch" and which "survives" into the next
instruction kept coming out backwards from the ROM's own pick, no
matter how the expression was split into ordered locals, register-
pinned individually, or reordered - the *last* two instructions of
each 4-instruction sequence already matched without any changes, only
the *first two* (which register the loaded byte lands in, and whether
a subsequent shift/mask keeps or moves it) needed correcting, which
turned out to only be controllable by writing the whole sequence as
inline asm with a single pinned output register (avoiding the extra
`mov` a normal `"=r"` output constraint into a `u8`-typed C variable
would otherwise insert for the implicit truncation check - fixed by
routing the asm's output through an `s32` temporary instead of a `u8`
one).

**`nullsub_11`/`sub_80070D4`/`sub_80070E8`/`sub_80070EC`/`sub_800710C`/
`sub_8007110`**: six small functions, all matched on the first or
second attempt. `nullsub_11` needed the usual empty-stub alignment
fix. `sub_80070D4` tail-calls `sub_803AD7C` through the same
field+0x18 table convention but discards its return value - the ROM's
epilogue pops the saved LR into r0 (clobbering the call's return
value on purpose), which only happens for a genuinely `void`-returning
function; declaring it to return `void *` instead kept a pointless
value alive in r0 across the pop, forcing a different register
(`pop {r1}`) and breaking the match. `sub_80070EC` stores a
width/height pair both as raw bytes and as `-w/2`-style halved,
negated `s16`s - plain C division by a negative constant reproduced
the ROM's rsb/lsr/add/asr rounding idiom exactly, no tricks needed.
`sub_800710C`/`sub_8007110` are two identical `return 0;` stubs with
no callers found anywhere in the codebase (raw asm or already-matched
C) - genuinely unreferenced, same as `sub_8006C38`/`sub_8006E64`
above.

**`sub_8007114`**: an AABB-vs-rectangle containment test, same raw
`self` layout as the actor-zone functions above. Needed the heaviest
register wrangling so far in this cluster:
- `self` and `box` both had to be pinned (`asm("r5")`/`asm("r6")`) -
  once anything else in the function was pinned, gcc stopped putting
  the plain parameters in the ROM's own registers on its own.
- The width/height-to-min/max computation is one inline asm block
  (mirroring `sub_8007048`'s halfW/halfH-in-place-vs-moved issue,
  same root cause) with its four outputs pinned to r4/r1/r5/r3.
- **`r7` cannot be pinned in this toolchain, ever** - confirmed again
  here: pinning `result` to r7 for a merged single big asm block
  dropped r7 from the prologue's push list entirely and produced
  wrong code (see `matching_decomp_register_pinning` memory, point
  10). Leaving `result` as a completely ordinary, unpinned local let
  gcc's own allocator land it in r7 on its own, matching the ROM -
  the fix for an "r7 mismatch" that looks like it needs a pin is
  usually to pin *less*, not more.
- The two box-corner-vs-AABB comparisons needed `boxY0`/`boxY1`
  pinned individually (r2, then r0) - same in-place-vs-moved pattern
  as the width/height computation, but for the *second* box corner
  read specifically; the *first* one (`boxX0`/`boxX1`) matched with
  plain, unpinned locals.
- The function's body comes out to 94 bytes (not 4-aligned), needing
  the usual `asm(".align 2, 0")` fix (see the very first entry in this
  file) - without it, `as` pads with NOP instead of the ROM's
  zero-fill.

**`sub_8007174`**: matched on the first attempt. Ignores its own first
parameter entirely (overwritten as scratch before ever being read),
reads the same `gUnknown_03001308` sub-object `sub_8006FE4` uses but
as two raw sign-extended-24-bit `s32` fields (dx/dy) rather than
through the record table - a different part of the same object.

**`sub_800719C`/`nullsub_12`**: `sub_800719C` is a close sibling of
`sub_8007174` above, reading the same sub-object fields but through
plain `<< 8` (no sign-extension this time) after a small per-axis
rounding step. Needed the "compute both loads before either use"
technique (see `matching_decomp_register_pinning` memory, point 4):
plain `(x - (*(s32*)subObj << 8)) >> 8` per axis, evaluated inline,
compiled to a compute-use-compute-use order instead of the ROM's
compute-both-then-use-both order - fixed by pulling both shifted
values out into named locals (`subX`/`subY`) ahead of the two
subtractions. `nullsub_12` is the usual empty-stub alignment fix.

**`sub_80071E4`**: an object constructor - allocates 0x1c bytes via
`sub_8026EDC`, wires up a vtable-like pointer
(`gStaticData_087E3BEC`) and calls an init function
(`sub_8007230`), then stores its three `u16` parameters into the new
object (one as a raw halfword, two left-shifted into fixed-point
`s32` fields). Matched on the first attempt, including the `r8`
push/save/restore dance for keeping `arg0` alive across both calls -
gcc reached for `r8` on its own here (three live parameters plus the
allocated object exceeds what r4-r7 alone can hold), no pinning
needed - see `matching_decomp_register_pinning` memory, point 8.

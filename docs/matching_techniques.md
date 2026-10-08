# Matching techniques

This is the reference for the techniques that make the C in `src/` and
`lib/` compile to the ROM's exact bytes with agbcc (gcc 2.9). The
matching phase worked them out pass by pass; the per-pass logs are in
[docs/matching/archive/](./matching/archive/) (frozen), and this page
collects what they found, with links to a few representative cases.

Read it before touching a workaround, and before adding one. Every
technique here is a way of steering gcc 2.9's passes, not of describing
what the game does: the right fix is always the plainest C that gives
the same bytes, and a workaround stays only if a clean rebuild shows the
plainer version doesn't.

- The workaround idioms that fit in a macro have a named form in
  [include/match.h](../include/match.h) (see [The match.h macros](#the-matchh-macros)).
- [tools/match_idioms.py](../tools/match_idioms.py) counts every idiom
  by kind and file (`--kind K --show` lists the sites), checks that the
  macros expand to the spellings they replace (`--check-macros`),
  converts a kind to its macro (`--convert K`) and fails if an idiom
  that has a macro is written out by hand (`--check`, run by CI).
- [tools/match_prune.py](../tools/match_prune.py) finds the workarounds
  that are no longer needed (#662): it removes each site, alone and
  then in pairs and pin bundles, rebuilds the object through the
  Makefile and keeps every removal that leaves the object
  byte-identical (see [Pruning workarounds](#pruning-workarounds)).
- The process (isolated compiles, clean rebuilds, `make compare`, the
  report) is in [workflow.md](./workflow.md) and
  [CONTRIBUTING.md](../CONTRIBUTING.md#verification). An isolated compile
  is never proof of a match: register allocation depends on the whole
  object.

## Writing new matching code

- **Use the macros.** Spell a pin, an empty asm or a register/memory
  clobber with its [match.h](../include/match.h) macro
  ([table](#the-matchh-macros)), not by hand: write
  `MATCH_HOLD_REG(s32, k, r6)`, not `register s32 k asm("r6")`, and
  `MATCH_USE(x)`, not `asm("" : : "r"(x))`. Give each use a comment
  saying what it fixes.
- **Check with the tool.** `tools/match_idioms.py --check` fails on a
  hand-spelled idiom outside the few documented exceptions (its
  `ALLOWED_SPELLED` list, also [below](#other-empty-asm-forms)), and on
  any file-scope `asm(".align 2, 0")`, which the build makes redundant
  ([below](#align-2-0)); CI runs it with `--check-macros`. If a new shape really has no macro, add one
  to match.h with a `--check-macros` case, rather than a new exception.
- **Per-object flags** (old_agbcc, `-O1`, `-fno-strength-reduce`, ...)
  are set in the Makefile, each list with a comment giving its evidence,
  and listed in the [table below](#per-object-flags). A new one needs the
  same evidence: every matched function in the object stays exact.
- Prefer a source-shape fix to any workaround, and remove a workaround
  when a clean rebuild shows it's no longer needed.

## Contents

1. [Compilers and flags](#compilers-and-flags): old_agbcc vs agbcc,
   -O1 SDK code, per-object flags, agbcc_arm and agbcc_arm_patched
2. [Source shape](#source-shape): loops, early returns, switches,
   signedness, statement blocks
3. [Calls](#calls): PMF dispatch, struct-by-value and one-byte-struct
   arguments, inline-argument order, asm-label aliases
4. [Register allocation](#register-allocation): pins, holds and the
   reload round-robin, spill-slot order
5. [The empty-asm idioms](#the-empty-asm-idioms) and
   [the match.h macros](#the-matchh-macros)
6. [Memory accesses](#memory-accesses): retyped field stores, scoped
   volatile, `"m"` operands, static-inline accessors
7. [Assembler-level fixes](#assembler-level-fixes): `.align 2, 0`,
   `.pool`, instruction asm
8. [Warnings](#warnings): `x = x` self-init
9. [NON_MATCHING](#non_matching)
10. [Pruning workarounds](#pruning-workarounds): `tools/match_prune.py` (#662)
11. [Survey and conversion record (#576)](#survey-and-conversion-record-576)

## Compilers and flags

### old_agbcc vs agbcc

The original build used two versions of the Thumb compiler, per
translation unit. `tools/agbcc/bin/old_agbcc` builds the 94 objects in
the Makefile's `OLD_AGBCC_OBJS`; the rest use the current `agbcc`.

How to tell, from the ROM's code:

- **Constant before byte.** In a byte read-modify-write, old_agbcc's
  scheduler loads the constant *before* the `ldrb`
  (`movs r0, #5; negs r0, r0; ldrb r1, [r2, #0xc]; ands r0, r1`), where
  agbcc loads the byte first. The same goes for `|= K`, bitfield inserts
  and constant byte stores.
- **r7 without pins.** old_agbcc's allocator uses r7 on its own, so the
  ROM's `push {r4-r7, lr}` appears with no pins. If a draft needs a pile
  of pins to reach r7, try old_agbcc first: pin-heavy drafts often match
  with fewer pins, or only once pins are removed.
- old_agbcc has no `-fprologue-bugfix` (the Makefile filters it out).
  For agbcc objects the flag is needed (GAX2 breaks without it). An
  unused callee-saved register in a `push` is not the prologue bugfix;
  see `MATCH_CLOBBER(r5)` [below](#other-empty-asm-forms).

A file joins `OLD_AGBCC_OBJS` as a whole: every matched function in the
object must stay byte-exact under old_agbcc. If one doesn't, split the
file at a function boundary into a new object (and `ldscript.txt`
entry), as the link_handshake and title_screen splits did.

Cases: [issue-24-boss-actor.md](./matching/archive/issue-24-boss-actor.md)
(the discovery, Dingodile's file),
[old-agbcc-retry.md](./matching/archive/old-agbcc-retry.md)
(`UpdateCortexBoss`, `CreatePlatform`; whole-file moves),
[issue-31-old-agbcc.md](./matching/archive/issue-31-old-agbcc.md)
(`SpawnSquid` and the r7 push). #662 step 3 moved level_state.o and
level_query.o over: under old_agbcp their pins and asm went with plain C
(`LevelHasEntityType`, `CheckAllCratesBroken`, `UnpackSaveData`), which
agbcp only matched pinned.

### -O1 SDK code

Nintendo's AgbEeprom library (`EEPROM_V122`, `lib/agb_eeprom/`) was
compiled at `-O1`, with the current agbcc (`O1_OBJS`). Its plain SDK C
matches there with no pins or volatile tricks, and is 2-107 halfwords
off at `-O2`. Telltales: a loop exit test evaluated twice
(`duplicate_loop_exit_test`, which `-O2`'s cross-jumping merges back)
and a constant address loaded twice in one function (`-O1` doesn't CSE
it). No `-O2` sub-flag combination reproduces it. See
[matching/eeprom-sdk-o1.md](./matching/eeprom-sdk-o1.md).

### Per-object flags

A flag is accepted for an object only if every matched function in it
stays exact; when one function needs a flag its neighbours can't take,
it's split into its own object. Each override in the Makefile carries a
comment with its evidence.

| Flag | Objects | Why |
|---|---|---|
| ~~`-fno-strength-reduce`~~ | none (`title_screen.o` until #664 part 10c-2) | Its C needed it for `InitVvLogoPieces`'s up-counting loop. As C++ the plain indexed loop matches with strength reduction on, and so does the rest of the file ([per-file-flags-investigation.md](./matching/per-file-flags-investigation.md)). |
| `-fno-rerun-loop-opt` | `link_session_reset.o` | The second loop pass reverses `ResetLinkSessionState`'s copy loop; the flag breaks `HandleLinkSerial`, hence the split. |
| `-O1` | `lib/agb_eeprom` (4 objects) | SDK code, above. |
| no `-mthumb-interwork` | libgcc2 (`__divdi3`, ...) | The only ROM functions that return with `pop {r4-r7, pc}`. |
| agbcc_arm, `-fomit-frame-pointer` | `string_arm.o`, `sprite_arm.o` | ARM code of the IWRAM image ([matching/iwram-image.md](./matching/iwram-image.md)). |
| **agbcc_arm_patched** (instead of agbcc_arm), `-mleaf-no-lr-save`, `-fno-schedule-insns -fno-schedule-insns2` | `string_arm.o` | `itoa_arm` pushes r4-r6 without lr, which stock agbcc_arm can't; and the ROM keeps its loop increments, terminator store and swap in source order, which either scheduling pass reorders. The four other functions come out the same either way. |
| **agbcc_arm_patched** (instead of agbcc_arm), `-minterwork-return-lr` | `sprite_arm.o` | `LookupSpriteFrameCache`'s three returns pop into lr (`ldmfd sp!, {lr}; bx lr`); stock agbcc_arm pops into ip. The four other functions come out the same either way (and need scheduling). |

**agbcc_arm_patched is a locally patched compiler, not a real
toolchain.** The ROM's ARM code was built by a later build of
agbcc_arm's own Cygnus/Red Hat line that has never been released; its
code generation is agbcc_arm's except for two fixed strings in the
prologue and return code, which no C reaches (fifth pass of
[iwram-image.md](./matching/iwram-image.md)). So `itoa_arm` and
`LookupSpriteFrameCache` are built with SAT-R/agbcc's agbcc_arm plus
[tools/agbcc_patches/agbcc_arm_prologue_return.patch](../tools/agbcc_patches/agbcc_arm_prologue_return.patch),
which adds one opt-in option for each behaviour. Without the options
its output is byte-identical to agbcc_arm's (checked on every C file in
the repo). `tools/build_patched_agbcc_arm.sh` builds it (INSTALL.md); the
Makefile uses it only for `PATCHED_ARM_OBJS`. Don't use it, or add
options to the patch, for anything else without the same kind of
evidence: a back-end path in agbcc_arm's source that no C can reach.

No flag turns gcc 2.9's loop optimizer off wholesale
(`-fno-loop-optimize` and `-fno-crossjumping` don't exist in it, and the
Thumb agbcc and old_agbcc have no `-fno-schedule-insns`; agbcc_arm
has). See
[matching/per-file-flags-investigation.md](./matching/per-file-flags-investigation.md),
[last-ten-naked-retry.md](./matching/archive/last-ten-naked-retry.md)
and [gax-toolchain-retry.md](./matching/archive/gax-toolchain-retry.md).

## Source shape

Most mismatches are fixed by writing the C in the shape the original
programmer did, not by workarounds. The recurring ones:

### Loops

- **Rotation.** `for` and `while` loops are rotated: the exit test moves
  to the bottom and the loop is entered with a `b` to it. A `do`/`while`
  isn't. Match the ROM's entry branch with the loop kind.
- **Peeled iterations.** A `break` in a search loop can make gcc
  (old_agbcc especially) copy the body up to the break ahead of the loop
  as a peeled first iteration. Exit with a `goto` past the loop instead.
  An `if (done) break;` near the top of a `do`/`while` becomes the exit
  test; `if (!done) { ... }` keeps the ROM's order.
- **Reversal.** Strength reduction turns an up-counting `i` whose only
  other use is the exit test into a count-down (`subs; cmp #0; bge`).
  If the ROM counts up, index through a pointer, use a `goto` loop, put
  a `MATCH_KEEP(j)` in the body, or (with evidence) the per-object flag.
- **`goto` loops.** A `label: ... if (++i < n) goto label;` loop has no
  loop notes, so loop.c hoists and reduces nothing in it. That's the fix
  when the ROM hoists nothing ([per-file-flags-investigation.md](./matching/per-file-flags-investigation.md),
  `ResetLogoPieces`; `src/frontend/title_screen.cpp`).
- **Hoisting.** If gcc hoists an invariant the ROM recomputes in the
  loop, put a `MATCH_KEEP_VOLATILE(base)` at the use
  ([sub_8009150-loop-invariant-hoist-matched.md](./matching/archive/sub_8009150-loop-invariant-hoist-matched.md),
  `LinkCrateToActiveBucket`; `src/crates/crate_grid_link.c`).
- **Insn counts.** loop.c's decision to move an invariant depends on the
  loop's insn count; `MATCH_BARRIER()`s in the body change it
  (`src/frontend/company_logos.cpp`).
- **`do { } while (0)` is a loop** to agbcc: it gets loop notes and
  loop-weighted register priorities. Statement macros use
  `if (1) { ... } else (void)0` instead (`include/actor_self.h`).

Cases: [big-naked-retry-3.md](./matching/archive/big-naked-retry-3.md)
(rotation, `SpawnRoomEntities`),
[sr65-naked-retry.md](./matching/archive/sr65-naked-retry.md)
(which givs get reduced, `DrawVvLogoPieces`).

### Branches and tails

- **Cross-jumping** (jump2, after reload) merges identical tails, but
  only if they were allocated identically. To keep two tails apart, write
  them differently or put a `MATCH_BARRIER()` in one; to get the ROM's
  separate reload registers, sometimes the shared tail has to be written
  in both branches.
- **Rotated-loop tests.** jump2 also cross-jumps a rotated `while`
  loop's duplicated entry test with its bottom test when a label sits
  before each `cmp` (the entry becomes a `b` to the bottom test, or the
  bottom one a `b` to the entry). If the ROM keeps both, a
  `MATCH_BARRIER()` before the loop and one at the end of its body block
  the two directions; nothing else is emitted
  (`HeapSortActorsByKey`, [iwram-image.md](./matching/iwram-image.md),
  fourth pass).
- **Early return vs one epilogue.** An early `return` gets its own copy
  of the epilogue. If the ROM branches to one, use `goto end;` and a
  single `return`. An early return can also flip which branch falls
  through; put the body inside the `if` instead
  ([issue-52-gap-b364.md](./matching/archive/issue-52-gap-b364.md)).
- **Switch tables.** gcc builds a compare tree unless the cases are dense
  enough. Explicit empty cases (`case 0: case 3: break;`) give the ROM's
  jump table and its base ([huge-naked-retry.md](./matching/archive/huge-naked-retry.md)).

### Types and expressions

- `v >> 31` on an `s32` is `asr`; `(u32)v >> 31` is `lsr`
  ([issue-48-0x080291a4-actor.md](./matching/archive/issue-48-0x080291a4-actor.md)).
- A `u8`/`u16` return or parameter adds `lsl; lsr` truncation at the
  caller; an `s32` one doesn't. Same for locals.
- `&&` on an `s32` range can be folded into an unsigned
  subtract-and-compare, and adjacent byte compares into a word compare;
  nested `if`s keep them apart
  ([issue-12-13-25-naked-retry.md](./matching/archive/issue-12-13-25-naked-retry.md)).
- Operand spelling matters: `a + b*16` and `b*16 + a` and `b << 4` can
  give different code (see [inline-argument order](#inline-argument-order)).
- **A 1/0 flag tested from registers** (`mov r3, rOne; movls r3, rZero;
  cmp r3, #0`, the constants loaded before the loop): an inline `u8`
  comparator that returns two `u8` parameters, `return zero;` /
  `return one;`, fed from `u8 one = 1, zero = 0;` declared inside the
  loop body. Literal returns give immediates (`mov r3, #1; movls r3,
  #0`), an `int` result is folded into the branches, wider parameter
  types let cse fold the constants back, and the loop scope picks which
  preheader loop.c hoists them to (`HeapSortActorsByKey`,
  [iwram-image.md](./matching/iwram-image.md), fourth pass).
- **agbcc_arm: a two-instruction constant added in place** (`add r0, r0,
  #0xF9000000; add r0, r0, #0xFF0000` inside a loop, not a register
  loaded before it): subtract it in two statements, each with a valid ARM
  immediate (`a -= 0x07000000; a += 0xFF0000;`). As one expression the
  addsi3 expander builds the constant in a register, which loop.c
  hoists. As two, combine merges them into one `plus` that is split
  back in place after reload (`LookupSpriteFrameCache`,
  [iwram-image.md](./matching/iwram-image.md), fifth pass).
- **`cmp #10` with `ge`/`lt`, not `cmp #9` with `gt`/`le`.**
  fold-const.c rewrites `x >= 10` to `x > 9` and `x < 10` to `x <= 9`.
  Assigning the constant inside the compare, `x >= (ten = 10)` with a
  local `ten`, isn't folded and still compares with the immediate
  (`itoa_arm`, found by decomp-permuter;
  [iwram-image.md](./matching/iwram-image.md), sixth pass).
- **agbcc_arm: `movge rN, #0; movlt rN, #1` then a `lt` block** (the
  sign of a value): two `if`s on the same test, `if (x >= 0) f = 0; if
  (x < 0) { f = 1; x = -x; }`. As one if/else, jump.c hoists `f = 0`
  above the branch (`mov rN, #0; addlt rN, rN, #1`); as two ifs the
  first becomes a conditional move and cse drops the second compare
  (`itoa_arm`, [iwram-image.md](./matching/iwram-image.md), seventh
  pass).
- **agbcc_arm: `mov rN, #0` after `mov rN, #45` in the same block, not
  `sub rN, rN, #45`.** reload_cse_move2add rewrites a constant load
  into an add from the register's last known constant, but only from a
  wider or equal mode. Loading the first constant through a `u8`
  variable pinned to the same register (`MATCH_HOLD_REG(u8, minus, r4)
  = '-';`) keeps the later `= 0` a `mov` (`itoa_arm`, seventh pass).
- **A constant offset rebuilt after a call** (`movs r2, #0x8d; lsls
  r2, #2` before the call and again, in another register, after it):
  the index passed to an inline function as a parameter
  (`gSpriteBankSet->Anims(47)`, sprite_obj.hpp). Written as
  `banks[47]` twice, gcc keeps the offset in a callee-saved register
  across the call (`FreezeLevelClock`, #662 step 3).
- **Packed fields read with `ldrb`/`ldrh` and shifts** (a byte for a
  field inside one byte, the halfword for one that straddles two):
  bitfields of the struct itself (`u16 lives:7; u16 maskLevel:2; u16
  wumpa:7;` in `game_progress`); gcc accesses each through the narrowest
  mode that holds it. A bitfield sub-struct is word-sized and gives
  `ldr` (`UnpackSaveData`/`PackSaveData`, #662 step 3).
- **Two values computed into a fresh register pair, then stored to a
  stack array** (`subs r2, r1, #2; ...; adds r3, r0, #0; subs r3, #30;
  str r2, [sp]; str r3, [sp, #4]`, or a callee-saved r4 pushed for
  `r3:r4` in a leaf): a `struct vec2` *value* held in a DImode register
  pair. In C, `struct vec2 goal = target->pos;` (camera.c) or an inline
  returning a `struct vec2` gives it; g++ keeps a struct value in
  memory, so the C++ spawners `SpawnRoomExit` and `SpawnCrateGemMarker`
  still need pins (#662 step 3).

## Calls

### PMF dispatch

What was long parked as a "stride-8 trampoline dispatcher" is gcc 2.x's
C++ pointer-to-member-function call, `(this->*table[this->state])()`.
A table entry is
`struct actor_pmf { s16 thisOffset; s16 index; union { s16 vtableOffset; void *fn; }; }`:
an `index > 0` calls virtual slot `index - 1` of the vtable at
`this + vtableOffset`, anything else calls `fn` directly. The call goes
through `_call_via_rN` with the pointer in the first free argument
register. `ACTOR_PMF_CALL(self, table)` and `ACTOR_VCALL(obj, m, arg)`
in [include/actor_self.h](../include/actor_self.h) reproduce it with no
pins. See [pmf-dispatch-retry.md](./matching/archive/pmf-dispatch-retry.md)
(21 functions, e.g. `RunPolarPlayerState`) and
[issue-57-0x0802fbf0-actor.md](./matching/archive/issue-57-0x0802fbf0-actor.md).
In C++ source (built by agbcp, [cplusplus.md](./cplusplus.md)) it is just
`(this->*table[state])()`, and a virtual call is `obj->method(arg)`:
both compile to these exact sequences.

### Struct-by-value and one-byte-struct arguments

- **Struct returns.** A real `struct aabb f(part)` passes the hidden
  result pointer in r0, so the argument is loaded into r1 before the
  result address goes into r0. An explicit `dest` pointer parameter
  gives the other order. Pick whichever the ROM shows
  (`GetSpriteAttackBox`, [issue-24-boss-actor.md](./matching/archive/issue-24-boss-actor.md)).
- **Struct arguments.** A by-value `struct vec2` gives the ROM's
  load-both, store-both copy.
- **One-byte stack arguments.** A `u8` parameter past r3 is stored with
  a word `str`. The ROM's `add rN, sp, #K; strb` comes from a packed
  one-byte struct, `struct byte_arg` ([include/byte_arg.h](../include/byte_arg.h)).
  A packed struct literal is built in a register before the slot address;
  adding `u8 pad[0]` makes it BLKmode and stores it address-first
  (`RunRoom`, `src/level/run_room.c`;
  [hard-register-hold-retry.md](./matching/archive/hard-register-hold-retry.md)).
  See [issue-55-naked-retry.md](./matching/archive/issue-55-naked-retry.md).

### Inline-argument order

gcc 2.x expands *all* arguments of an inlined call before copying any
into the parameters, and a sum argument comes back partly unexpanded:
its address arithmetic is emitted early, the final add or load late, at
parameter-copy time. So when the ROM computes addresses early and loads
late, pass the whole expression to a small `static inline` function
instead of computing it in locals. The spelling matters: in `DrawPlayer`
`hist.x + tbl*16` matched while `tbl*16 + hist.x` and `tbl << 4` were
85+ halfwords off. Ordinary call arguments are evaluated left to right;
to put an `add r0, sp, #K` after the other arguments, compute those in a
block before the call. See
[inline-arg-order-retry.md](./matching/archive/inline-arg-order-retry.md)
and `src/player/player_event.c`.

### Asm-label aliases

When a file only matches with a different prototype for a function or
global than its header's (a `u8` return where the definition has `s32`,
a `struct byte_arg` where it has `u8`, a struct return where it has a
`dest` pointer), the file keeps its own declaration under another name,
bound to the real symbol with an asm label:

```c
/* codegen: RandRange is u16, but this file was matched against an s32
 * return; the u16 prototype changes the stack slots in InitTitleScreen.
 * docs/headers_plan.md */
extern s32 RandRange_s32(s32 max) asm("RandRange");
```

The call is still `bl RandRange`. Redeclaring the real name with another
type would be a "conflicting types" error. Each alias has a `codegen:`
comment and a row in [headers_plan.md's "Codegen exceptions"](./headers_plan.md#codegen-exceptions);
the rules are in CONTRIBUTING.md's "Declarations and headers".

## Register allocation

### Register pins

`MATCH_HOLD_REG(T, x, rN)` (`register T x asm("rN")`) gives `x` the hard
register rN for its whole life: gcc replaces its pseudo with rN before
allocation. Use one when the ROM keeps a value in another register than
gcc picks. An initialiser follows the macro as it would the declarator:
`MATCH_HOLD_REG(u8 *, p, r2) = &self->tag;`. Limits:

- A pin decides where a *variable* lives. It can't steer the registers
  reload picks for temporaries; use a hold for that.
- Don't pin r7 (the Thumb frame pointer): r7 pins have dropped r7 from
  the push, been ignored or crashed the compiler. Don't hold r8.
- Pins inside macros or inlines shift other allocation.
- Many pins date from before old_agbcc was found or before a source-shape
  fix. Removing them is fine when the object stays identical.
- Pinning the old and new value of `x = prev + d` separately keeps the
  result out of `prev`'s register.

A macro whose pins differ per call site takes the registers as
parameters and passes them on to `MATCH_HOLD_REG` as bare names, as
`src/bosses/cortex.c`'s `SET_FRAME_R(part, 1, r3, r4)` and `MARK_GONE`
and `include/entity_bits.h`'s `ENTITY_SET_GONE_BIT_PINNED(t, r2, r4)` did
(#667; they went when cortex.c became C++, #664 part 7i). A macro argument is expanded before it is substituted, so
`MATCH_HOLD_REG`'s `#reg` sees `r4`, not the parameter's name. No pin is
written out as `register T x asm(R)` any more; `tools/match_idioms.py
--kind pin` would list one.

### Holds and the reload round-robin

Reload gives each insn that needs a reload register the first free
call-clobbered register in number order, skipping hard-live ones, then
hands out spill registers round-robin from the last one used. One
different choice shifts every later reload: the ROM's r3 where the draft
has r2 means r2 was busy there in the original. A **hold** makes a
register busy over just that span, with no code:

```c
MATCH_HOLD_REG(s32, hold, r2);
MATCH_HOLD(hold);            /* r2 live from here: no code */
self->part->y += 0x600;      /* its reload now gets r3, as in the ROM */
MATCH_USE(hold);             /* end of the hold */
```

The start can sometimes go: a pinned local that is never assigned is
live from the top of the function to its `MATCH_USE`, which is enough
when nothing earlier needs the register (`ReleaseHang`, where this
example came from, and `PauseMenu::Draw`'s two holds have no
`MATCH_HOLD` since #662). The insn to cover is the one listed under "Spilling for insn
N" in a `-da` `.greg` dump. Holds of callee-saved registers across calls
model registers the ROM leaves unused. See
[late-naked-retry-3.md](./matching/archive/late-naked-retry-3.md)
(`HitEnemy`),
[hard-register-hold-retry.md](./matching/archive/hard-register-hold-retry.md)
(`InitSaveMenuIcons`, `DrawPauseMenu`),
[huge-naked-retry-3.md](./matching/archive/huge-naked-retry-3.md)
(the round-robin wrapping), and `src/crates/crate_list_update.c`,
`src/level/run_room.c`.

### Spill-slot order

Pseudos that don't get a hard register get stack slots in pseudo-number
order. User variables come first, numbered by declaration and first
appearance; GCSE temporaries come after, in hash-bucket order, and the
hash table's size is half the function's insn count. So:

- moving a declaration can reorder the user variables' slots;
- changing the insn count anywhere reshuffles the GCSE temporaries'
  slots. `MATCH_BARRIER()`s at the top of the function are the padding
  knob (three in `InitSaveMenuIcons`, `src/save/save_menu_draw.c`; four
  in GAX's `gax_work_size.c`);
- a different return type of a called function can swap slots.

If slot order is the only difference left, fix the rest first: it often
sorts itself out. See
[huge-naked-retry-3.md](./matching/archive/huge-naked-retry-3.md)
(`QueueCratePlayerCollision`) and
[big-naked-retry.md](./matching/archive/big-naked-retry.md).

**Stack-box addresses** are a separate problem: gcc CSEs `&box` into one
pseudo held in a callee-saved register across calls, where the ROM
recomputes `add r0, sp, #K` before each call. `BOX_ADDR(&box)` on every
use makes each its own opaque value. The constant-init form doesn't work
there, because its input is still CSE'd
([sp-box-retry.md](./matching/archive/sp-box-retry.md),
`PlayerAnimWouldTouchCrate`).

## The empty-asm idioms

An `asm` statement with an empty template emits nothing, but gcc doesn't
know that. Its operands tell gcc that values are read, changed or
defined at that point, which is what these idioms use. Two rules from
gcc 2.9:

- An asm **without outputs** is volatile anyway.
- An asm **with outputs** is an ordinary insn: loop.c can hoist it, CSE
  can merge two identical ones and flow deletes it when the output is
  dead. `asm volatile` stops all three and makes it a full scheduling
  barrier. The `_VOLATILE` macros are for the sites where the plain form
  was moved or merged (the recheck in
  `src/bosses/hovercraft_cannon_flash.cpp`).

| Spelling | Macro | What gcc 2.9 does with it | Typical use |
|---|---|---|---|
| `asm("")` | `MATCH_BARRIER()` | Scheduling barrier; opaque to cross-jumping; counts toward insn-count heuristics. Doesn't make gcc forget register values. | Keep a load/store on one side; keep tails apart; insn-count padding (loop.c, GCSE slot order). |
| `asm("" : : "r"(x))` | `MATCH_USE(x)` | `x` must be live in a register here: extends its range and adds a reference (raising its allocation priority; loop-weighted inside loops). | Keep a value in its register past a call; reach the ROM's register priority; end a hold. |
| `asm("" : "+r"(x))` | `MATCH_KEEP(x)` | `x` has an unknown value afterwards: no constant propagation, CSE, rematerialization or immediate folding through it. | Keep a per-branch reload; stop `w & 0xff` folding; `BOX_ADDR`. |
| `asm("" : "=r"(x))` | `MATCH_HOLD(x)` | Defines `x` here with an unknown value. With a pin, occupies that register until the last use. | Start a hold; a deliberately undefined value. |
| `asm("" : "=r"(v) : "0"(K))` | `MATCH_CONST(v, K)` | Loads K into v's register here, but v isn't a known constant: not CSE'd with another K, hoisted, sunk to its store or folded into an immediate, and no doubled live range. | The ROM loads a constant at a given point, once per use (e.g. before the address); an opaque copy of a value. |
| `({ T _p = (e); asm("" : "+r"(_p)); _p; })` | `MATCH_KEEP_EXPR(T, e)`, `BOX_ADDR(a)` | An expression whose value is opaque at each use. | Stack-box addresses, above. |

Cases: [near-miss-polish-3.md](./matching/archive/near-miss-polish-3.md)
(constant-init), [sp-box-retry.md](./matching/archive/sp-box-retry.md)
(`"+r"` vs `"=r"/"0"`), `src/player/action_ctrl_moves.cpp` (`MATCH_CONST`
as an opaque copy, `m2`), `src/actor/actor_category_select.cpp` (use).

### Other empty-asm forms

The rarer forms, a few sites each:

| Spelling | Macro | Sites | What it does |
|---|---|---|---|
| `asm("" : : "r"(a), "r"(b))` | `MATCH_USE2(a, b)`, `MATCH_USE2_VOLATILE(a, b)` | 3 | `MATCH_USE` of two values in one insn. Not the same as two `MATCH_USE`s, which are two insns. |
| `asm("" : : : "r5")` | `MATCH_CLOBBER(r5)`, `MATCH_CLOBBER_VOLATILE(r4)` | 3 | Tells gcc the register is clobbered, so the prologue saves it even though nothing uses it, as the ROM does ([issue-9-raw-asm-pass.md](./matching/archive/issue-9-raw-asm-pass.md), `UpdateEnemyBob`; `src/enemies/enemy_ctrl.c`); also forces a reload of whatever it held (`src/level/play_room.c`). |
| `asm volatile("" ::: "memory")` | `MATCH_MEMORY_BARRIER()` | 2 | Makes gcc forget memory and acts as a barrier. It does not stop address CSE, which is what it was usually tried for (`src/frontend/title_screen.cpp`). |
| `asm("" : "+m"(x))` | `MATCH_KEEP_MEM(x)` | 2 | `x` is in memory here with an unknown value, so a later read is a real load (the `ldm r1!` re-read in `ConvertAirshipTiles`). |
| `asm("" : : "m"(x))` | `MATCH_USE_MEM(x)` | 1 | `x` must be in memory here: keeps it in its stack slot across a call (`src/level/spawn_enemies.cpp`). |

An asm that reads a field through `"m"` can also fix the order of a load
against a constant
([issue-59-60-m-operand-scheduling.md](./matching/archive/issue-59-60-m-operand-scheduling.md)).

Three empty asms stay written out, each a one-off shape that no macro
covers (a macro for one site would only add a name to look up). They
are `ALLOWED_SPELLED` in `tools/match_idioms.py`:

- `asm volatile("" : : "m"(src), "m"(dst))` in `lib/gax/src/gax_swi.c`:
  two `"m"` inputs in one insn (`mem_ref`).
- `asm volatile("" : "+r"(flags) : "r"(m))` in
  `src/player/action_ctrl_moves.c`: a keep and a use in one insn
  (`keep_volatile`).
- `asm volatile("" : "=r"(ch) : "r"(c + 0x108))` in
  `src/save/save_transfer.c`: an opaque copy whose input isn't tied to
  the output (`"r"`, not `MATCH_CONST`'s `"0"`), so `ch` gets no copy
  preference for the input's register (`empty_other`).

## Memory accesses

- **Retyped field stores** `*(T *)&s->field = v` (191 sites) and reads
  (71): access a field with another mode or width than its declared type
  (`str` vs `strb`, sharing a constant between a byte and a word store),
  or stop gcc treating it as a struct member when it decides where to
  build a zero (`src/actor/actor.c`, the `animTimer`/`animDone` pairs).
  Prefer fixing the field's declared type when that's byte-neutral.
- **Scoped volatile** `*(volatile T *)&x`, or a block-local
  `volatile u8 *p`: forces a real load or store at that point, including
  a dead load the ROM has (`(void)*(volatile s32 *)&p->field;`), or
  keeps an address computed into the ROM's register
  ([big-naked-retry-2.md](./matching/archive/big-naked-retry-2.md),
  [near-miss-polish-3.md](./matching/archive/near-miss-polish-3.md),
  `ActionCtrlStateCrouch`).
- **Static-inline accessors**: wrapping a field-address computation in a
  tiny `static inline` function makes gcc recompute it at each call
  site, the ROM's fresh `self + K + i*stride` per access, where a local
  would be CSE'd into one base register
  ([issue-59-60-static-inline-cse-promotion.md](./matching/archive/issue-59-60-static-inline-cse-promotion.md),
  `UpdateTitleLogoPieces` in its C). In C++ the plain `pieces[i].field`
  gives the same code, and the accessors went (`TitleScreen::UpdateLogoPieces`,
  `CompanyLogos::UpdateVvLogoPieces`; #664 part 10c-2). The opposite
  case, re-reading through `self` instead of caching a local so old_agbcc
  makes the ROM's copies, is in near-miss-polish-3.md.

## Assembler-level fixes

### `.align 2, 0`

The build handles this; C files need no padding statement.

When an object's `.text` ends on a halfword boundary, `as` pads the
section to its 4-byte alignment inside the object. For code that fill is
a Thumb NOP (`mov r8, r8`, `0xC046`), and the ROM has zeros there. The
linker isn't involved: the pad is already part of the `.o`'s `.text`
size, and the ROM output section's own gap fill (`} = 0` in
`ldscript.txt`) is zero anyway. agbcc starts every function with
`.align 2, 0`, so a gap *between* two functions of one object is already
zero; only the end of the last function was affected.

The Makefile's C rules append `ZERO_PAD_TEXT` (`.text` then
`.align 2, 0`) to every compiled `.s` before assembling it, so the end of
`.text` is zero-filled too (#663). This adds nothing to a section that
already ends aligned. Its only side effect is that a data-only object's
empty `.text` becomes 4-byte aligned, which places nothing.

Until #663 this was done by hand: 276 file-scope `asm(".align 2, 0");`
statements in 131 files, one after each function that ended an original
object. `tools/match_idioms.py --check` now rejects a new one. Those sat mid-file after the file-layout merges (#575) and were
redundant there. The archive notes and the old log,
[matching.md](./matching.md), still describe that statement. A match
that differs only in its last 1-2 bytes, `c046` against `0000`, now means
the object wasn't built by these rules. Hand-written `.s` (lib1funcs,
libagbsyscall, `asm/`) still spells its own `.align 2, 0`.

A forced 4-byte alignment for every input section (`SUBALIGN(4)` on the
ROM output section) doesn't work instead: it can't change bytes already
inside an object, and some input sections are only 2-byte aligned in
the ROM, so the layout shifts.

### `.pool`

`asm(".pool")` places the literal pool only for literals that asm
statements themselves load (`ldr rX, =sym`); gcc puts its own literals at
the end of the function and ignores the marker. Splitting a pool
mid-function therefore means writing those loads in asm
(`src/crates/crate_stack.c`, `src/actor/actor.c`). Conversely, an
`ldr =K` written in asm can land in the wrong pool; let gcc generate the
address when it can
([sub_8009150-loop-invariant-hoist-matched.md](./matching/archive/sub_8009150-loop-invariant-hoist-matched.md)).

### Instruction asm

253 asm statements emit real instructions (`add %0, %0, %1`,
`mov %0, #0x10; neg %0, %0`, whole call-setup tails). They are the last
resort, for orders gcc can't be talked into, and every one should say
why. An asm tail is also never cross-jumped with C tails
([naked-sub_800fdc8-matched.md](./matching/archive/naked-sub_800fdc8-matched.md)).
They have no macro: each is a specific instruction sequence.

## Warnings

The build is `-Wall ... -Werror`. Where the ROM really uses an
uninitialized register (a `bestIdx` when the count is 0), an initializer
would add code, so the variable stays uninitialized and is
self-initialized: `s32 sel = sel;` emits nothing and silences
`-Wuninitialized` (4 sites). A `& 0xFFFF0000` on garbage before ORing in
BGxCNT bits is a `union bgcnt` local (`graphics_package.h`) instead:
agbcc pads the union to a word, `cnt.raw = 0` clears only its low half,
and `-Wuninitialized` doesn't check aggregates (`src/frontend/starfield.cpp`,
`TitleScreen::LoadBg`; #662 step 3, formerly self-inits,
[issue-65-naked-retry.md](./matching/archive/issue-65-naked-retry.md)).
The order of escape hatches (self-init, `UNUSED`, a per-object
`-Wno-...`) is in CONTRIBUTING.md's "Compiler warnings".

## NON_MATCHING

`include/core.h` defaults `NON_MATCHING` to 0; `make NON_MATCHING=1`
defines it for C (`-D`) and assembly (`--defsym`). A function that
can't be matched yet keeps its C draft under `#if NON_MATCHING` and the
checked-in `NAKED` transcription under `#else`; the progress report
scores the C draft. Both builds have to work. No function uses this any
more: the last two, `itoa_arm` and `LookupSpriteFrameCache` (#553), are
real C built with agbcc_arm_patched ([per-object flags](#per-object-flags),
[matching/iwram-image.md](./matching/iwram-image.md), seventh pass). See
[naked-transcription-parked-functions.md](./matching/archive/naked-transcription-parked-functions.md)
for the history.

To search a draft with
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
use [tools/permuter/](../tools/permuter/README.md): a compile script with
the agbcc_arm objects' flags, `setup.sh` to build a permuter directory
(the target `.o` comes from a function's `NAKED` asm), and the
`base.c`/`settings.toml` used for the two IWRAM functions before they
matched. For a Thumb function, copy
`compile.sh` and swap in `agbcc` or `old_agbcc` and the object's flags.
Treat its output as hints: it often reaches a lower score with C that
changes what the function does (an uninitialized local, a load hoisted
out of a loop), so port only the changes that keep the behaviour.

## The match.h macros

[include/match.h](../include/match.h) gives each one-statement idiom a
name. Each macro is exactly the spelling it replaces, token for token
(`tools/match_idioms.py --check-macros` checks this, comparing the
preprocessed tokens; `::` counts as two `:`, as it does to agbcc's C
lexer), so converting a site never changes the object. Every use keeps a
comment saying what it fixes.

| Macro | Expands to |
|---|---|
| `MATCH_HOLD_REG(T, name, rN)` | `register T name asm("rN")` |
| `MATCH_BARRIER()` | `asm("")` |
| `MATCH_USE(x)` | `asm("" : : "r"(x))` |
| `MATCH_KEEP(x)`, `MATCH_KEEP_VOLATILE(x)` | `asm("" : "+r"(x))`, `asm volatile(...)` |
| `MATCH_HOLD(x)`, `MATCH_HOLD_VOLATILE(x)` | `asm("" : "=r"(x))`, `asm volatile(...)` |
| `MATCH_CONST(v, K)` | `asm("" : "=r"(v) : "0"(K))` |
| `MATCH_USE2(a, b)`, `MATCH_USE2_VOLATILE(a, b)` | `asm("" : : "r"(a), "r"(b))`, `asm volatile(...)` |
| `MATCH_CLOBBER(rN)`, `MATCH_CLOBBER_VOLATILE(rN)` | `asm("" : : : "rN")`, `asm volatile(...)` |
| `MATCH_MEMORY_BARRIER()` | `asm volatile("" : : : "memory")` |
| `MATCH_KEEP_MEM(x)`, `MATCH_USE_MEM(x)` | `asm("" : "+m"(x))`, `asm("" : : "m"(x))` |
| `MATCH_KEEP_EXPR(T, e)` | `({ T _p = (e); asm("" : "+r"(_p)); _p; })` |
| `BOX_ADDR(a)` | `MATCH_KEEP_EXPR(struct aabb *, a)` |

## Pruning workarounds

`tools/match_prune.py` (#662) re-tests the workarounds mechanically. For
each site in the `.c`/`.cpp` files it's given, it builds the object
without it and keeps the removal if the object is unchanged:

| Site | Removal tried |
|---|---|
| `MATCH_HOLD_REG(T, x, rN)` | a plain local `T x` (same initializer); in a bundle, the declaration deleted |
| `MATCH_BARRIER`, `MATCH_MEMORY_BARRIER`, `MATCH_USE`, `MATCH_USE2`(`_VOLATILE`), `MATCH_KEEP`(`_VOLATILE`), `MATCH_HOLD`(`_VOLATILE`), `MATCH_CLOBBER`(`_VOLATILE`), `MATCH_KEEP_MEM`, `MATCH_USE_MEM` | the statement deleted |
| `MATCH_CONST(v, K)` | `v = K;` |
| `MATCH_KEEP_EXPR(T, e)`, `BOX_ADDR(a)` | `((T)e)`, `a` |
| spelled-out empty asm | deleted, or `v = K;` for an `"=r"` output from one input |
| instruction asm | the same operations in C, when every instruction is a `mov`/`add`/`sub`/`neg`/shift/logic/`mul`/load/store on `%N` operands and immediates (`mov %0, #8; neg %0, %0` becomes `v = -8;`, an `lsl`/`lsr` pair by 16 a `(u16)` cast); labels, `swi`, `.byte` and hard registers have no translation |
| `T x = x;` | `T x;` |

How it decides, in short (the tool's docstring has the details):

- **Build:** make runs on the Makefile itself, with `OBJ_DIR`, the
  source and build directories and the generated-header directory moved
  into a temporary directory per parallel worker. The worker's source
  directory mirrors the file's own one with symlinks, holding the trial
  version of the file in place of its link. So each object gets the
  ROM build's compiler and flags (old_agbcc, agbcp, agbcc_arm_patched,
  `-fno-implement-inlines`, ...), and build/ and src/ are never written.
  A site in a preprocessor arm the ROM build doesn't compile is found
  by preprocessing through the same rule and is not tried.
- **Compare:** the two `.o` files are equal if their sections (other
  than the symbol, string and relocation tables) have the same names,
  flags, sizes and bytes in the same order, their relocations have the
  same offsets and types and point at the same symbols (by name,
  binding, section and value, not by symbol index), and their symbol
  tables hold the same symbols. Only the symbol table's order may
  differ.
- **Search:** every site alone; the ones that work are checked together
  (or added one at a time if they don't all work together); repeat
  until none works alone; then pairs of sites in one function and
  bundles (a pin with every `MATCH_HOLD`/`USE`/`KEEP`/`CONST` of its
  variable), and back to single sites, until a fixed point.

`--write` applies the result, takes the emptied lines and the comment
right above them (when it names the workaround or a register) along,
drops an unused `#include "match.h"`, formats the file, builds that text
once more and only then replaces the file. Comments elsewhere that still
name a removed macro are listed: check them by hand, along with
function-header comments that describe a removed pin. Commit
the result like any other change, after the full clean checks.

On main at the time of #662 step 1, the dry run over src/ (510 sites in
the .c/.cpp files; headers aren't tried) found 206 removable, all of
them alone (no pair or bundle was needed), among them 184 of 357
`MATCH_HOLD_REG` pins. Applying all 206 at once kept
`crashbandicootxs.gba: OK`.

Step 2 applied them, and lib/'s 31, with the diffs reviewed by hand:
de-pinned locals folded into their uses where the object stayed the
same, and the comments that described a removed pin rewritten. That
C cleanup made 7 more sites removable (six of DarkenPalette's pins,
strcat's `i`), so 237 sites went in all. Seven sites the tool finds
removable were kept on purpose; a dry run still lists them:

- **A pin whose register the asm template names.** `itoa_arm`'s `num`
  (the `swi` reads r0), `DivMod`'s `quotient`/`remainder` (`svc #6`),
  `PollSaveTransfer`'s `result` (the template computes in r0) and
  `GaxInfoPlay`'s `cnt`/`cnt2` (`ldrsh r1, ...` writes r1 while the
  output operand is `%0`). Without the pin the code is only right
  because the allocator happens to pick that register, so the object
  stays the same while the C is wrong. Check this before applying a
  removal next to an asm with hard registers in its template.
- **Half of a symmetric hold.** `run_room.cpp`'s r0/r1 hold keeps
  `MATCH_HOLD(hold1)` next to `MATCH_HOLD(hold)`. A never-assigned pin
  is live from the top of the function anyway, so where a hold has only
  the one start, step 2 dropped it (`ReleaseHang`, `PauseMenu::Draw`,
  `GAX2_init`) and said so in the comment.

Step 3 (frontend, menus, save, link): trying the other compiler on each
object is cheap and worth doing first. `save_data.o` and
`save_transfer.o` come out byte-identical under old_agbcc as they stood,
and under it `SetSaveFlags`/`ClearSaveFlags` are plain `|=`/`&= ~`
(four pins and an instruction asm gone); `pause_menu_draw.o` also
matches under old_agbcp, but that frees none of its sites.

## Survey and conversion record (#576)

The conversion is complete. `tools/match_idioms.py` after part 4
(sites / files; "spelled" is what's still written out by hand):

| Kind | Macro sites | Spelled | Macro | Converted in |
|---|---|---|---|---|
| register pins | 2153 / 169 | 5 (cortex.c macros) | `MATCH_HOLD_REG` | part 3 |
| `asm("" : : "r"(x))` | 83 / 34 | 0 | `MATCH_USE`, `MATCH_USE2` | parts 2 and 4 |
| `asm("" : "+r"(x))` | 60 / 32 | 1 (`+r` with an input) | `MATCH_KEEP` | part 2; 1 statement expression became `MATCH_KEEP_EXPR` |
| `asm("" : "=r"(x))` | 22 / 14 | 0 | `MATCH_HOLD` | part 2 |
| `asm("")` | 15 / 8 | 0 | `MATCH_BARRIER` | part 2 |
| `asm("" : "=r"(v) : "0"(K))` | 29 / 19 | 0 | `MATCH_CONST` | part 1 |
| `asm("" : : : "rN")` | 3 / 2 | 0 | `MATCH_CLOBBER` | part 4 |
| `asm volatile("" ::: "memory")` | 2 / 2 | 0 | `MATCH_MEMORY_BARRIER` | part 4 |
| `"+m"` / `"m"` operands | 3 / 3 | 1 (two `"m"` inputs) | `MATCH_KEEP_MEM`, `MATCH_USE_MEM` | part 4 |
| other empty asms | - | 1 (untied `"=r"`/`"r"`) | none | - |
| `BOX_ADDR` | 12 / 3 | 0 | in match.h | part 1 (definition moved) |

The 8 spelled-out sites are `tools/match_idioms.py`'s `ALLOWED_SPELLED`
list, which `--check` (and CI) enforces; they're described under
[Register pins](#register-pins) and
[Other empty-asm forms](#other-empty-asm-forms).

These have no macro, by design:

| Kind | Sites | Why it stays |
|---|---|---|
| file-scope `.align 2, 0` | 276 / 131 | an assembler directive; all removed in #663, the build pads instead ([above](#align-2-0)) |
| instruction asm | 253 / 90 | each is a specific instruction sequence; each needs a comment |
| retyped field stores/reads | 191 / 31, 71 / 34 | part of the struct cleanup |
| asm-label aliases | 20 / 16 | headers_plan's codegen exceptions |
| scoped volatile casts and locals | 52 / 22 | a type, not a statement |
| self-init | 6 / 6 | a declaration |
| per-object flags | 6 groups | documented in the Makefile and [above](#per-object-flags) |

How it was done: one PR per idiom group, each off main, converted with
`tools/match_idioms.py --convert` (or by hand for the part 4 kinds) and
checked object by object (`.o` and `.s`) against a build of main, plus
the two clean checks (`make compare`; the objdiff report at 2056/2059
functions and 100% data).

- **Part 1 (#623):** match.h, this page, the tool, `MATCH_CONST` (29
  sites), `BOX_ADDR` moved to match.h.
- **Part 2 (#624):** `MATCH_BARRIER`, `MATCH_USE`, `MATCH_KEEP`,
  `MATCH_HOLD` (with their `_VOLATILE` forms), 178 sites, plus the source
  comments that quoted the old spellings.
- **Part 3 (#625):** register pins to `MATCH_HOLD_REG`, 2153 sites in
  169 files, by `--convert pin` (which splits each declarator into type
  and name and leaves any initialiser in place).
- **Part 4:** `MATCH_USE2`, `MATCH_CLOBBER`, `MATCH_MEMORY_BARRIER`,
  `MATCH_KEEP_MEM` and `MATCH_USE_MEM` (11 sites), `--check` and its CI
  step, and the [Writing new matching code](#writing-new-matching-code)
  section.

Not done: re-testing whether individual pins and nudges are still needed
(the optional item in #576). Removing one is fine whenever a clean
rebuild shows the object stays identical; `tools/match_prune.py` does
that for every site ([above](#pruning-workarounds), #662).

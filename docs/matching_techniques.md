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
  `--functions` (after a build) counts the functions with no workaround
  at all, the number in README.md, and `--functions --files` lists the
  others with their kinds.
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
   -O1 SDK code, per-object flags, agbcc_arm and agbcp_arm_patched
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
translation unit. `tools/agbcc/bin/old_agbcc` (or `old_agbcp` for C++)
builds the 146 objects in the Makefile's `OLD_AGBCC_OBJS`; the rest use
the current `agbcc` (`agbcp`). `tools/match_idioms.py` prints the
current size of every per-object list.

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
| `-fno-cse-skip-blocks` | `wumpa_update.o` | `Wumpa::Create`'s `phase` 0 has to be forgotten at the frame clamp's join, as in the ROM (`counter` gets a fresh 0). With skip-blocks cse carries it around the clamp. `SendToHud` needs skip-blocks, so it moved with `StartPayout` and `UpdateHop` to the start of `wumpa.cpp`, the next object (#662 round 3). |
| `-O1` | `lib/agb_eeprom` (4 objects) | SDK code, above. |
| no `-mthumb-interwork` | libgcc2 (`__divdi3`, ...) | The only ROM functions that return with `pop {r4-r7, pc}`. |
| agbcp_arm_patched (the ARM C++ compiler), `-fomit-frame-pointer` | `string_arm.o`, `sprite_arm.o` | ARM code of the IWRAM image ([matching/iwram-image.md](./matching/iwram-image.md)); C++ like the rest of the game, the output is agbcc_arm's without the four options below. |
| **agbcp_arm_patched**'s `-mleaf-no-lr-save`, `-mno-cond-return`, `-fno-schedule-insns -fno-schedule-insns2` | `string_arm.o` | `itoa_arm` pushes r4-r6 without lr, which stock agbcc_arm can't; and the ROM keeps its loop increments, terminator store and swap in source order, which either scheduling pass reorders. `strncpy_arm` branches to its final `bx lr` where stock agbcc_arm makes `bxeq lr` for any C. The three other functions come out the same either way. |
| **agbcp_arm_patched**'s `-minterwork-return-lr`, `-mstrict-cross-jump` | `sprite_arm.o` | `LookupSpriteFrameCache`'s three returns pop into lr (`ldmfd sp!, {lr}; bx lr`); stock agbcc_arm pops into ip. `HeapSortActorsByKey` keeps two identical loop tests that stock jump2 cross-jumps for any C, because each follows a label. The three other functions come out the same either way (and need scheduling). |

**agbcc_arm_patched and agbcp_arm_patched are locally patched
compilers, not a real toolchain.** The ROM's ARM code was built by a later build of
agbcc_arm's own Cygnus/Red Hat line that has never been released; its
code generation is agbcc_arm's except for two fixed strings in the
prologue and return code, which no C reaches (fifth pass of
[iwram-image.md](./matching/iwram-image.md)), and two jump.c rules it
doesn't have (ninth step). So `itoa_arm`, `LookupSpriteFrameCache`,
`strncpy_arm` and `HeapSortActorsByKey` are built with SAT-R/agbcc's
agbcc_arm plus
[tools/agbcc_patches/agbcc_arm_prologue_return.patch](../tools/agbcc_patches/agbcc_arm_prologue_return.patch),
which adds one opt-in option for each behaviour. Without the options
its output is byte-identical to agbcc_arm's (checked on every C file in
the repo). Since the C++ conversion the two files are C++, built with
agbcp_arm_patched: the same patch on notyourav/agbcc's ARM C++ compiler
(`g++_arm`), built by `tools/build_agbccpp.sh` (INSTALL.md), whose
output is the C compiler's with and without the options
(iwram-image.md, "Eighth step"). `tools/build_patched_agbcc_arm.sh`
still builds the C one. The Makefile uses agbcp_arm_patched only for
`ARM_OBJS`. Don't use either, or add
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
  `LinkCrateToActiveBucket` in its C; the C++ in
  `src/crates/crate_list_update.cpp` needs none).
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
  the two directions; nothing else is emitted. In the IWRAM ARM code
  the ROM's compiler doesn't have this rule, and `-mstrict-cross-jump`
  turns it off instead (`HeapSortActorsByKey`,
  [iwram-image.md](./matching/iwram-image.md), fourth pass and ninth
  step).
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
- **A saved register nothing uses, or a pointer one reference short in
  global-alloc**, when the code stores one coordinate of a position:
  write it through `Entity::SetPos` with the other coordinate passed
  back unchanged (`part->SetPos(part->x, y)`). The unchanged word's load
  and store are allocated and then deleted after reload (#662 round 5,
  [Pruning workarounds](#pruning-workarounds)).
- **`a / b` rather than `__udivsi3(a, b)`**: the operator is a const
  libcall, the declared function a call that clobbers memory as far as
  the post-reload passes know.
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
  pair. In C, `struct vec2 goal = target->pos;` (camera.cpp) or an inline
  returning a `struct vec2` gives it. In C++ it takes a compound
  literal, `point = (struct vec2){x, y};`: a named `struct vec2` copied
  whole goes through the implicit copy constructor, whose reference
  argument makes it addressable, so it lives on the stack
  (`SpawnRoomExit`, `SpawnCrateGemMarker`, `RunRoom`; #662 round 3).

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
  (`RunRoom`, `src/level/play_room.cpp`;
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
and `src/player/player_update.cpp`.

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
(the round-robin wrapping), and `src/level/play_room.cpp`.

### Spill-slot order

Pseudos that don't get a hard register get stack slots in pseudo-number
order. User variables come first, numbered by declaration and first
appearance; GCSE temporaries come after, in hash-bucket order, and the
hash table's size is half the function's insn count. So:

- moving a declaration can reorder the user variables' slots;
- changing the insn count anywhere reshuffles the GCSE temporaries'
  slots. `MATCH_BARRIER()`s at the top of the function are the padding
  knob (four in GAX's `gax_work_size.c`; `InitSaveMenuIcons`' C had
  three, which its C++ doesn't need);
- a different return type of a called function can swap slots.

If slot order is the only difference left, fix the rest first: it often
sorts itself out. See
[huge-naked-retry-3.md](./matching/archive/huge-naked-retry-3.md)
(`QueueCratePlayerCollision`) and
[big-naked-retry.md](./matching/archive/big-naked-retry.md).

**Stack-box addresses** are a separate problem: gcc CSEs `&box` into one
pseudo held in a callee-saved register across calls, where the ROM
recomputes `add r0, sp, #K` before each call (and reads `box.y` at its
own sp offset while a pointer to the box is live). Pass a standalone
box local (not a member of a frame struct) to an inline helper instead:
util.h's SetAabb, aabb.h's FlipAabbX/FlipAabbY and field accessors.
The inline's argument is the constant `frame + K`, so nothing holds the
address for cse to reuse (#662 round 4, [Pruning
workarounds](#pruning-workarounds)). `BOX_ADDR(&box)` on every use,
which made each its own opaque value, did the same with an asm
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
  was moved or merged. Only `MATCH_USE2_VOLATILE` (`lib/gax/src/gax_swi.c`)
  and one spelled-out `asm volatile` keep (below) are left.

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
(`"+r"` vs `"=r"/"0"`), `src/actor/actor_category_select.cpp` (use).
`EndSpin`'s `MATCH_CONST` opaque copy went in #662 round 4 (a `u16`
mask: thumb's movhi takes only 0-255, so reload builds the constant and
copies it, the ROM's copy).

### Other empty-asm forms

The rarer forms, a few sites each:

| Spelling | Macro | Sites | What it does |
|---|---|---|---|
| `asm("" : : "r"(a), "r"(b))` | `MATCH_USE2(a, b)`, `MATCH_USE2_VOLATILE(a, b)` | 2 | `MATCH_USE` of two values in one insn. Not the same as two `MATCH_USE`s, which are two insns. |
| `asm("" : : : "r5")` | `MATCH_CLOBBER(r5)`, `MATCH_CLOBBER_VOLATILE(r4)` | 2 | Tells gcc the register is clobbered, so the prologue saves it even though nothing uses it, as the ROM does ([issue-9-raw-asm-pass.md](./matching/archive/issue-9-raw-asm-pass.md), `UpdateEnemyBob`; `src/enemies/enemy_ctrl.cpp`); it also forces a reload of whatever the register held. |
| `asm volatile("" ::: "memory")` | `MATCH_MEMORY_BARRIER()` | 0 | Makes gcc forget memory and acts as a barrier. It does not stop address CSE, which is what it was usually tried for. No site needs it any more. |
| `asm("" : "+m"(x))` | `MATCH_KEEP_MEM(x)` | 2 | `x` is in memory here with an unknown value, so a later read is a real load (the `ldm r1!` re-read in `ConvertAirshipTiles`). |
| `asm("" : : "m"(x))` | `MATCH_USE_MEM(x)` | 0 | `x` must be in memory here: kept a value in its stack slot across a call (`SpawnFlamethrowerLabAssistant` until #662 round 4). |

An asm that reads a field through `"m"` can also fix the order of a load
against a constant
([issue-59-60-m-operand-scheduling.md](./matching/archive/issue-59-60-m-operand-scheduling.md)).

One empty asm stays written out, a one-off shape that no macro covers
(a macro for one site would only add a name to look up). It is
`ALLOWED_SPELLED` in `tools/match_idioms.py` (`EndSpin`'s keep-and-use
went in #662 round 4, and `SaveTransfer::ReceiveChunk`'s untied `"=r"`/`"r"`
copy in round 5):

- `asm volatile("" : : "m"(src), "m"(dst))` in `lib/gax/src/gax_swi.c`:
  two `"m"` inputs in one insn (`mem_ref`).

## Memory accesses

- **Retyped field stores** `*(T *)&s->field = v` (6 sites left, from
  191 when they were first counted) and reads (17, from 71): access a
  field with another mode or width than its declared type (`str` vs
  `strb`, sharing a constant between a byte and a word store), or stop
  gcc treating it as a struct member when it decides where to build a
  zero (the `animTimer`/`animDone` pairs of `actor.c`, gone in its C++).
  Prefer fixing the field's declared type when that's byte-neutral.
- **Scoped volatile** (9 sites) `*(volatile T *)&x`, or a block-local
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
mid-function therefore means writing those loads in asm (none is left:
the last, PollSaveTransfer's, became C++ in the C++ conversion, which
needed a `volatile` field instead). Conversely, an
`ldr =K` written in asm can land in the wrong pool; let gcc generate the
address when it can
([sub_8009150-loop-invariant-hoist-matched.md](./matching/archive/sub_8009150-loop-invariant-hoist-matched.md)).

### Instruction asm

17 asm statements emit real instructions (from 253 when the #576 survey
counted them): `swi`/`svc` calls with their registers, GAX2's
hardware settle delays (`.byte`-encoded) and call-into-ARM sequence, two
`ldrsh` forms, a pair of `lsl`/`lsr` halfword truncations and
`FindSubstring`'s case folding. They are the last
resort, for orders gcc can't be talked into, and every one says why. An asm tail is also never cross-jumped with C tails
([naked-sub_800fdc8-matched.md](./matching/archive/naked-sub_800fdc8-matched.md)).
They have no macro: each is a specific instruction sequence.

## Warnings

The build is `-Wall ... -Werror`. Where the ROM really uses an
uninitialized register (a `bestIdx` when the count is 0), an initializer
would add code, so the variable stays uninitialized and is
self-initialized: `s32 sel = sel;` emits nothing and silences
`-Wuninitialized` (2 sites, both in lib/: `GAX_fx` and
`EEPROMWrite1_check`). A `& 0xFFFF0000` on garbage before ORing in
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
real C built with agbcc_arm_patched (C++ built with agbcp_arm_patched
since the C++ conversion; [per-object flags](#per-object-flags),
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

For a function in a C++ object (#662 step 3), port that one function
to C: structs with its fields at their offsets, a virtual call as a
call through the vtable entry (`e = &obj->vtbl[n];
e->fn((u8 *)obj + e->delta, ...)`), inline methods as `static inline`
functions. The compile script feeds the file straight to `old_agbcp`
or `agbcp` inside `extern "C" { }`, so the port has no comments, macros
or `#include`s. Check first that the port with the old workaround
compiles to the ROM's code, and use that object as `target.o`.

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
  ROM build's compiler and flags (old_agbcc, agbcp, agbcp_arm_patched,
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
strcat's `i`), so 237 sites went in all. Six sites the tool finds
removable were kept on purpose; a dry run still lists them:

- **A pin whose register the asm template names.** `itoa_arm`'s `num`
  (the `swi` reads r0), `DivMod`'s `quotient`/`remainder` (`svc #6`),
  and `GaxInfoPlay`'s `cnt`/`cnt2` (`ldrsh r1, ...` writes r1 while the
  output operand is `%0`; since #662 round 2 GaxInfoPlay is plain C
  and has neither). Without the pin the code is only right
  because the allocator happens to pick that register, so the object
  stays the same while the C is wrong. Check this before applying a
  removal next to an asm with hard registers in its template.
- **Half of a symmetric hold.** `play_room.cpp`'s r0/r1 hold keeps
  `MATCH_HOLD(hold1)` next to `MATCH_HOLD(hold)`. A never-assigned pin
  is live from the top of the function anyway, so where a hold has only
  the one start, step 2 dropped it (`ReleaseHang`, `PauseMenu::Draw`,
  `GAX2_init`) and said so in the comment.

Step 3 (#733-#737) rewrote the sites the tool couldn't remove, one
subsystem at a time. What it found, for the next workaround to look at:

**What proved removable.** Most of the remaining workarounds were
fixing the draft, not the compiler:

- **The compiler first.** Many pins and instruction asms were making
  agbcc imitate old_agbcc: its constant loaded before the `ldrb`, its
  copy before an AND (`adds r1, r7, #0` ahead of the load), its BLDY
  address derived from BLDCNT's. Trying the other compiler on an object
  is cheap; save_data.o, save_transfer.o, input.o, irq.o, aabb.o,
  audio.o, display.o, fade.o, sprite_frame.o, level_state.o and
  level_query.o moved to `OLD_AGBCC_OBJS` (146 objects now), and their
  plain C matches: `SetSaveFlags` is a plain `|=`, `AllocVramTileBlock`,
  an asm island with hand-placed labels and a `.pool`, is a loop. Not
  every move pays: pause_menu_draw.o matches under old_agbcp too, but
  that frees none of its sites.
- **Rewrite from the ROM's code, not the draft.** GaxChannelSetInstrument
  (ex-NAKED, 5 pins and 2 asms), GaxDrawText (7 pins, 2 asms) and three
  more GAX2 functions match as straight field stores and loops; their
  workarounds were fixing artefacts of the first draft's shape.
- **What the function really is.** ClearKeys is the KeyInput
  constructor and returns `self`, which is why the ROM keeps r0 out of
  its stores; FormatPaddedNumber's tail is an inlined strcpy and strlen,
  whose own `*dst = 0` is the ROM's re-materialized `movs r0, #0`;
  `CreateYeti`'s `movs r0, #0x1c` before the heap flag is the actors'
  operator new inlined, with the size as the helper's argument
  ([inline-argument order](#inline-argument-order)).
- **Locals, types and reads.** A field read again instead of a copy
  (`CountCategoryCrates`: cse turns the second load into the ROM's
  `adds r0, r1, #0`); a `u8` copy of a byte, which keeps its own
  register where an `s32` one is folded (`UpdateSkidAnim`); constant
  field stores instead of a zero local (`YetiStateChase`); a `bool`
  result where an `s32` one is folded on both paths (`StepCannonFlash`);
  one local per axis (`UpdateActorBgScroll`); globals read by name
  instead of through address locals (`UpdateYetiBg2`); `s16` matrix
  scales (`DrawAffinePieces`). The ones in
  [Types and expressions](#types-and-expressions) marked #662 step 3
  (`FreezeLevelClock`, `UnpackSaveData`) are from this pass too.
- **A value built from pieces in one register** (`and #0xFFFF0000;
  orr`, never zeroed, which the C wrote with an `x = x` self-init) is a
  word-sized union of bitfields held in a local (`DarkenPalette`,
  `QueueSpriteFrameOam`, `TitleScreen::LoadBg`).
- **decomp-permuter on a C port of the function** ([NON_MATCHING](#non_matching))
  found several of these: the `u8` copy, the per-axis locals, a
  `volatile` DMA source halfword (`SetScaledSpriteColor`), an `s32` copy
  of a `u8` argument before a bitfield store (`Platform::SetExitMirror`)
  and two copies of a pointer for the two registers the ROM keeps it in
  (`ProbeEdgeTerrain`).

**What proved unavoidable.** Each kept site has a comment saying what
the ROM does that the plain C doesn't. Where the permuter got below
them, it was only with C that changes the function (an uninitialized
value) or a no-op (`len++; len--;`). The patterns, with one example
each:

- **Register choice the allocator won't make** (`MATCH_HOLD_REG`, 89
  pins): `ActionCtrl::StartTornadoFall` pins `entry` to r2, where agbcp
  swaps `this` and `entry` between r2 and r3.
- **A pin whose register an asm template names** (kept even where the
  tool can remove it, see step 2): `itoa_arm`'s `num` for its `swi`.
- **A register busy over a span** (`MATCH_HOLD` with a pin, 8): the
  r0/r1 hold in `RunRoom`, so the global's address and the player
  pointer both land in r2, as in the ROM.
- **Allocation priority and live ranges** (`MATCH_USE`, 47): the extra
  reference that makes `&gActorSpawnIndex` outrank `base` in
  `SelectActorCategory` (r7, not r8).
- **A value gcc mustn't see through** (`MATCH_KEEP`, 14): `pb = &b` in
  `Platform::ResolveCollision`, which stays in r4 for the overlap tests
  instead of being formed again from sp at each one.
- **A constant loaded where the ROM loads it** (`MATCH_CONST`, 13):
  `ActionCtrl::HandleEvent`'s separate 1 in r5, which cse otherwise
  shares with the test's own 1.
- **Insn-count padding and tail separation** (`MATCH_BARRIER`, 13):
  the four in `GAX2_estimate` for the spill-slot order, the two in
  `HeapSortActorsByKey` against cross-jumping (since fixed by the
  compiler options, below).
- **A stack-box address recomputed before each call** (`BOX_ADDR`, 12):
  `Crate::PlayerAnimWouldTouch` ([spill-slot order](#spill-slot-order)).
- **A struct value in a register pair** (pins, until round 3's
  compound literal, [above](#types-and-expressions)): `SpawnRoomExit`
  and `SpawnCrateGemMarker` pinned what C would hold in DImode.
- **A callee-saved register pushed but unused** (`MATCH_CLOBBER`, 2):
  `EnemyCtrl::UpdateBob`'s r5.
- **A re-read from memory** (`MATCH_KEEP_MEM`/`MATCH_USE_MEM`, 3): the
  `ldm r1!` re-read in `ConvertAirshipTiles`.
- **Instructions no C produces** (17 instruction asms): `swi` calls,
  GAX2's hardware settle delays and call into ARM code.
- **Accesses of another width or a forced load** (23 retyped field
  accesses, 9 scoped volatiles): `Player::HandleEvent`'s dead load of
  `maskLevel`.
- **An uninitialized register the ROM really uses** (2 self-inits, both
  in lib/): `GAX_fx`'s `sel`.

After step 3, `tools/match_idioms.py --functions` counts 1955 of the
2059 functions with no workaround at all (README.md has the
per-directory table).

Round 2 in level/, objects/, vehicle/, cutscene/ and pickups/ (C++):

- **A narrowed load is a narrow local.** A `u8`/`u16` copy of an `s32`
  field (`u8 toggleByte = toggle;`) gives the `ldrb`/`ldrh` the C wrote
  as `*(u8 *)&field`: the expander loads the low part of the memory
  operand directly. Inside an expression the cast can be folded away
  (`1 & (u8)toggle` is `ldr; and`), so it needs its own statement
  (`Slideshow::ShowPicture`, `LevelState::AddBrokenCrate`); a plain cast
  is enough where nothing folds it (`SetupRoomBlend`, `UpdateYeti`).
- **An inline flag test** (`TokenHas`) replaced `TileCache::DecodeChunk`'s
  extra `MATCH_USE` reference; the decomp-permuter on the C++ found it.
- **A cross-jumped tail from the source's own nesting**:
  `YetiStateCharge`'s two stomp cues are one `if` with the two sounds
  inside, sharing their ShakeActorBg call, instead of two branches kept
  apart by a `MATCH_BARRIER`.
- **A byte offset as its own statement** (`LevelEntityFlags::SetActivated`):
  the ROM shifts the index before it forms `bits1`'s address, which
  `bits1 + word` and `&bits1[word]` don't; the offset computed first and
  added to `bits1` as bytes replaced an integer address pinned to r1.
- **A dead load through a pointer-to-volatile parameter**
  (`sub_8026C80`) instead of a volatile cast in the body.

**Round 2, lib/iwram/system/util/audio/text.** 23 functions -> 17
(`tools/match_idioms.py --functions`). What removed them:

- **A re-read instead of a local** (`GaxInfoPlay`, pins, two `ldrsh`
  asms and retyped accesses): the final `(p->speed & 0xff) == 0` reads
  the field again, and GCSE keeps the halfword in a register from the
  top, as the ROM does; `TickAmbientSfx`'s scoped volatile read was the
  shared tail `GAX_set_fx_volume(2, ambientSfxVolume)` written once,
  after an `if (... <= 0) { ... }`.
- **The parameter's type.** A `void *self` copied to a typed local
  makes gcc copy the later parameters first; `GaxInfoPlay` takes
  `struct GaxInfoHandler *` (as `GaxChannelPlay` already did).
- **A field that really is volatile.** GaxPlayerState's `state` is
  advanced by `GAX_irq` (interrupt side) and tested by the main loop;
  marked volatile, `GAX_irq`'s scoped cast goes and every other GAX2
  object stays the same.
- **Dead initializers as insn count.** GAX2_estimate's four empty asms
  only lengthened the RTL for GCSE's hash-table sizing; initializing
  every local to 0 where it's declared does the same.
- **Real loops.** `WaitForKeyPress`'s goto transcription is two loops:
  the front end's loop rotation (`expand_end_loop` moves everything up
  to the *last* exit to the bottom) gives the ROM's block order when
  the cancel test leaves by `goto fail` rather than `break`; the
  permuter then found `(u16)mask &` for the r4/r5 order.
- **Plain C for a hand-written island**: `mem_walk_heaps` (file-scope
  asm) is a walk to each heap's last block, result discarded.
- **Plain stores**: `GAX2_new`'s `*(u16 *)&` store and its `val`
  juggling were a draft artefact; `params->x = 0xFFFF` etc. match.

Kept, with what was tried in each comment: GAX2's hardware settle
delays and ARM calls, `GaxHuffUnComp`'s SWI, the two `x = x`
self-inits (the ROM uses the uninitialized register), `itoa_arm`,
`strncpy_arm`'s conditional-return barrier and `HeapSortActorsByKey`'s
barriers (both since fixed by the compiler options: iwram-image.md,
"Ninth step"), `FindSubstring`'s case folds (63 more spellings: only `char`
locals keep both arms' copies, with the truncation inside one arm),
`GAX2_init`'s and `DrawWrappedText`'s holds and `GaxChannelMix`'s keep.

Round 2, player/crates/frontend (after the C++ conversion):

- **The project's DMA macros instead of a shared register struct.**
  `CompanyLogos::DrawVvLogoPieces` copied its emblem rows through one
  `struct dma_regs *` held for the whole block, which needed an r2 pin
  and three `MATCH_BARRIER`s of loop insn-count padding. With
  `DmaCopy16` (its own `dmaRegs` pointer, set inside the loop) both go.
  `TitleScreen::LoadObjTiles`'s r8 pin went with a plain `pkg++` at the
  end of the pass instead of a `next` copy.
- **A store the optimizer folds, through an inline parameter.**
  `SwimCtrl::StartStroke`'s `ldrb`/`strb` of `tag` is the
  "switch to animation `tag`" idiom (Crate::SetTag's) called with the
  current animation; `t->tag = t->tag` is folded away, the parameter
  store isn't, so the volatile cast went.
- **A byte flag for a separate constant.** In `ActionCtrl::HandleEvent`
  the launch pad's A flag as a `u8` (it is stored to `u8` fields) makes
  the AND a byte operation, whose 1 cse doesn't share with `one`'s
  word-sized 1, so its `MATCH_CONST` went. In the two bounce cases (and
  `StateJump`) the byte AND then lands in another register.
- **Kept:** the stack-box `BOX_ADDR`s are two passes' doing: cse1
  merges the builder calls' `&f.b` inside one basic block (no flag
  changes that), and gcse's PRE moves the overlap test's. -fno-gcse
  frees the latter in both of crate_hit.o's builder functions but puts
  `Crate::QueuePlayerCollision` 2000 lines off, so no flag was added.

Round 2 in menus/, save/ and link/ (C++):

- **A key-state copy** (`struct held_pressed_pair k = KEYS`): the word
  load plus `lsr #16` that `PauseMenu::Loop` pinned two registers for.
- **The same call in both arms of an `if`**: `UpdatePageArrows`'s
  `ShowFrame(sprites[8], 0)` / `(..., 1)`, which the ROM loads
  `sprites[8]` for twice; one call after the `if` needed two pins.
- **A literal where the draft had a variable**: `DrawYesNoPrompt`'s Y
  0x87 at each SetPos; gcc shares the constant the ROM's way by itself.
- **An inline method around a statement group**: `SaveData::StampHeader`
  (the four header stores) makes gcc compute their addresses before the
  EraseSlot loop, as `Validate` does in the ROM; written out, they come
  after it. `DmaClear16` instead of `DmaFill16` then gives the ROM's
  registers.
- **The class's own method for shared code**: `LinkRing::Push`, the ring
  push `SaveTransfer::SendChunk` and `LinkSession::HandleSerial` both
  inline, frees HandleSerial's escaped `p` (`MATCH_KEEP_EXPR`).
- **Hardware registers as volatile**: HandleSerial reads each SIOMULTI
  register twice (the word copy and the 0xffff test) because they are
  volatile, not because of a pointer biv and a `MATCH_CONST`.
- **A field for a retyped access**: LinkSession's packet CRC is a `u16
  hash` field after `id[6]`, as LinkPlayer's.

Round 2 in bosses/, enemies/ and actor/ (C++):

- **A `bool`.** `CanPauseActorCategory` returns `!` of a `bool`
  function's result: the ROM's `movs r1, #1; eors r0, r1` on the call's
  r0 (through an `s32` the result is copied away; the C had the two
  instructions in asm, the first C++ a pin). `MapFill`'s nibble toggle
  as a `bool` (`odd = !odd`) replaces `LoadBgPicture`'s two `MATCH_USE`
  priority nudges.
- **A local's scope.** `CortexTargetCtrl::Update`'s step count declared
  in the step's `if` block instead of for the whole function gives the
  ROM's r5/r6/r7 without a pin.
- **Constants stored where they are used.** The hovercraft's flash
  colour stored to both palettes in each branch, white as a constant,
  instead of a colour local stored after the branches: the ROM's white
  loaded into r2 and copied to r1, which took an r1 pin
  (`SetHovercraftFlashColor`, and `RunHovercraftState`, which inlines it
  as `ApplyHovercraftFlashColor`). `UpdateHovercraftHitFlash`'s palette
  loop as an indexed `for` with a constant white drops two pins and a
  volatile re-read of the timer.
- **The class's own method.** `EnemyCtrl::HandleEvent`'s squash calls
  `Entity::MarkGone`, whose `1` is the ROM's fresh `movs #1` (a
  constant-init asm before); `EnemyCtrl::Update` reads the target
  through a local for the first test and again for the second, the
  ROM's reload (a volatile cast before).
- **A flag the ROM keeps.** `JetpackCheckpointText::Draw` is
  `ActorSelf::Draw` at a fixed scale with its size tests on `scale`: the
  flag folds in the tests but not in the OR, where reload rematerializes
  it as the ROM's `movs r0, #0` (a constant-init before).
- **What didn't move** (evidence in each site's comment): swaps where
  the ROM's load order and its allocation come from different spellings
  (`UpdateOscillateX`, `UpdateTriggerBox`, `UpdateHop`), where the
  permuter on the C++ reached the ROM only through `do { } while (0)`
  wrappers (loop-depth weighting of the references), `x++; x--;` pairs
  or dummy stores; `MegaMixCtrl::Update`'s spill-register rotation for
  the 0x104 reload; the unused saved registers of `UpdateBob` and
  `UpdateOscillateY`.

**Round 3, lib/gax, src/iwram, src/util.** 15 functions -> 14:

- **One result variable that the test also uses.** `FindSubstring`'s
  four opaque lowercase blocks become an inline `ToLower` that computes
  `r = (u8)(c - 'A')`, then `r = c + 0x20` or `r = c`, and returns
  `(u8)r`. jump.c's "`x = b; if (...) x = a;`" rewrite gives up when X
  is referenced in the test, so both arms keep their copy of `c`. Its
  two pins went as well: one variable for the scanned haystack byte and
  the pattern byte (both r3 in the ROM), and the parameters copied to
  locals, `needle` first, for the prologue order.
- **Kept, diagnosed.**
  - `GAX_CALL_ARM` (and `_R`): thumb.md's only indirect call is
    `bl _call_via_rN`, and this gcc has no `long_call`.
  - `HeapSortActorsByKey`: jump2 always cross-jumps the two loop tests,
    because the label before the top `cmp` lowers find_cross_jump's
    minimum to one insn. Since fixed by the compiler options
    (`-mstrict-cross-jump`).
  - `strncpy_arm`: jump.c makes a conditional RETURN of any jump to a
    label that a return follows, whenever use_return_insn holds. Since
    fixed by the compiler options (`-mno-cond-return`).
  - `GAX2_init`: global.c's priority, log2(refs) * refs / live length,
    puts fmt (0.136) above maxRate (0.127) unless maxRate gets the extra
    reference.
  - `GaxChannelMix`: the ROM reloads `item.done` twice in a row, which
    takes volatile. A one-armed clamp lets cse1 carry `row * 28` across
    the join.

**Round 3, src/level and src/player.** 12 functions -> 8:

- **A compound literal for a `struct vec2` in a register pair.**
  `point = (struct vec2){x, y};` is one DImode pseudo set word by word.
  It freed `SpawnRoomExit`'s and `SpawnCrateGemMarker`'s pins (and the
  latter's instruction asm) and `RunRoom`'s r0/r1 hold: the pair is born
  before the player pointer is loaded, so it conflicts with the pointer,
  which takes r2 (copied whole, the pointer died in the DImode load and
  took r0). See [Types and expressions](#types-and-expressions).
- **One variable used in two blocks is global-alloc's.** A local used
  in one basic block only is allocated by local-alloc, before any global
  pseudo, and takes the first free register. `SpawnRoomEntities` had its
  crate id pinned to r1. Now one `aid` serves the first and second
  searches: global-alloc then ranks the strength-reduced slot pointer
  above it, and the pointer gets r0 as in the ROM.
- **A flags2 `|=` through ActOrFlags0D.** The member `|=` expansion's
  dead `& 0` gave `StartTornadoFall`'s `slamBlocked = 0` its early 0
  (there was an r0 pin before).
- **Kept, diagnosed** (each comment has the details):
  - `ActionCtrl::HandleEvent`'s bounce 1s and `StateJump`'s: cse1
    follows the jump and gives the arms' QImode 1 a subreg of the AND's
    SImode 1, even from a `u8` `one`, and regmove copies it.
  - The dead loads in `Player::HandleEvent` and
    `ActionCtrl::HandleEvent`: jump2 merges a test's identical arms
    after reload and leaves the test's load, because no flow pass runs
    after it to delete it. Reproduced, but only with identical arms or
    a dead store.
  - `ReleaseHang`: reload's spill round-robin.
  - `StartTornadoFall`'s entry: global priority, `this` with 8 refs over
    38 insns ahead of entry's 4 over 26.
  - `EndSpin`, and `StateCrouch`, where a block-local address takes r0
    in local-alloc.
  - `SpawnFlamethrowerLabAssistant`: the ROM's `str r3, [sp]` is
    caller-save.c saving the lowest-ranked pseudo.

  None of the per-object flags on the brief's list helped any of them.

Round 3 in crates/, objects/ and pickups/ (C++). Each function was
compiled with `-da` and its RTL dumps read pass by pass:

- **local-alloc's 3-quantity sort.** A block with exactly three local
  quantities is sorted by a hand-written exchange
  (`qty_compare (0, 1)`, `(1, 2)`, `(0, 1)`) that compares quantity
  *numbers*, not sorted positions. When q1 beats q0 and q2 doesn't beat
  q1, the two swaps cancel and q0 (the first born) is allocated first. With two
  quantities, or four or more (qsort), the order is right.
  `CrateList::Unlink`'s free-list push (head, entry, reloaded entry) hit
  it: the head took r0. One function-scope `head` for both searches
  makes it a global-alloc register (r1), and FreeNode's r1 pin went.
- **A plain search loop.** `CrateList::Detach` (Remove and Update) as
  `while (i < capacity && slots[i] != sprite) i++;` drops the asm
  barrier its `n = capacity` local with early returns needed in Update.
- **-fno-cse-skip-blocks, with a move.** `Wumpa::Create`'s `phase` 0
  hidden from cse (MATCH_KEEP) is what cse does without skip-blocks: it
  forgets the 0 at the frame clamp's join, so `counter` gets a fresh 0.
  `SendToHud` in the same object needs skip-blocks, so it and the two
  functions after it moved to the start of wumpa.cpp, the next object.
- **Kept, with the deciding pass in each comment:** the crate box
  builders' `BOX_ADDR`s (calls.c copies each `&f.b` argument into a
  pseudo, since a PLUS costs more than 2 under SMALL_REGISTER_CLASSES,
  and cse1 ties the copies in one block); `MovingSprite::TouchPlayer`'s
  volatile read and `Platform::ResolveCollision`'s `pb` keep (the C++
  front end reads a local struct's fields through a fresh copy of its
  address, which cse1 ties to the pointer the ROM holds);
  `PlatformMover::Update`'s `now` pin (global-alloc priority, 0.83
  against `part`'s 0.74) and its 0x300 (reload's round-robin spill
  register); `Sprite::CheckPlayerContact`'s 1 (the u8 flag's OR is
  expanded with a QImode constant, which cse can't replace with the
  tests' SImode register).

**Round 3, src/bosses, src/enemies, src/actor (C++).** 13 functions ->
13: no function loses its last workaround, two lose one each.

- **A global's real type gives a re-read.** `ConvertAirshipTiles` and
  `ConvertHovercraftTiles` re-read each height after storing the row
  pointer (the ROM's `ldm r1!`), which a `MATCH_KEEP_MEM` forced.
  gAirshipMapFrames/gHovercraftMapFrames are the AnimParts' frame
  offsets, `u32` tables (both creators already cast them to `u32 *`):
  declared so, the row store has the int alias set, may alias
  `heights[]`, and cse no longer reuses the stored value. As `u8 *`/
  `void *` arrays the store's pointer alias set told cse it couldn't.
- **Kept, with the pass that decides it** (in each site's comment):
  the mask asm of the same two functions (cse1's fold_rtx puts an
  operand with a known constant value second, and regmove copies the
  first one); `UpdateTriggerBox`, `StompedHopPadCtrl::Update`,
  `TinyCtrl::SetState` and `SelectActorCategory` (global-alloc's
  ranking of two pseudos, by references over live length);
  `UpdateHop`, `UpdateOscillateX`, `UpdateOscillateY` and
  `ActorSelf::Draw` (local-alloc: a block-local value is placed before
  the globals are ranked, or two quantities tie and the older one wins);
  `MegaMixCtrl::Update`'s 0x104 (reload's round-robin over its spill
  registers); `UpdateBob`'s and `DingodileShieldCtrl::Update`'s saved
  but unused registers (a pseudo the ROM had in that register and whose
  code is gone by the end; the plain functions have none, and no spill
  of those registers happens). Every single -f flag and every pair of
  them (`-fno-gcse`, the cse, loop, strength-reduce, jump-threading,
  defer-pop, function-cse, regmove and peephole switches, `-O1`, plus
  `-flive-range`, the aliasing, inlining and scheduling switches) was
  tried on each plain function, under both compilers, and none matches.

**Round 4, src/enemies and src/bosses (C++), from the compiler source.**
9 functions -> 9; each site's comment now names the code path. A
private old_agbcp printed local-alloc's quantity order, global-alloc's
ranking, reload's spill choices and every insn flow2 deletes after
reload, and could force a pseudo out of its register:

- **What decides it.** Global-alloc's `find_reg` passes: the first
  only takes registers already used and not preferred by a
  lower-ranked conflicting value (`regs_someone_prefers`), and a
  register is "preferred" when `set_preference` sees the value as the
  source, or the source's first operand, of an insn setting a hard
  register or an allocated local (`TinyCtrl::SetState`'s pad gets r0
  from `ldr pad, [addr]`, `UpdateTriggerBox` would need one). Ranking
  uses flow's REG_N_REFS (plus loop depth, hence every permuter
  `do { } while (0)`) and REG_LIVE_LENGTH, counted before combine
  (`StompedHopPadCtrl::Update` needs 2 more insns where only `this`
  lives). Reload's free registers come in number order (thumb.h has no
  REG_ALLOC_ORDER); its spill registers rotate from `last_spill_reg`,
  which inherited reloads and output-register reloads don't move
  (`MegaMixCtrl::Update`).
- **Saved but unused registers.** Only four functions in the
  decompiled code have one: `UpdateBob`, `UpdateOscillateY`,
  `DingodileShieldCtrl::Update` and the matched `ActionCtrl::Update`,
  whose r7 is the high half of a DImode pointer to member. flow2's
  deletions across all 149 old_agbcp objects are of three kinds: copies
  into r8-r10 whose use reload took from the low register they came
  from, chains whose last use reload_cse removed, and halves of DImode
  pairs. 64-bit locals do produce the unused push in UpdateBob (`u64
  t`: the ROM's push and r4/r6, with the pair moving `ph` and the
  target), but no spelling gives the ROM; forced spills show the ROM's
  extra value kept its register through reload.

Round 3 in frontend/, menus/, save/, link/ and text/ (C++), diagnosed
from the `-da` dumps first:

- **An inline helper changes loop.c's choices.** `TitleScreen::Run`'s
  seed loop is `SeedLogoPieces()`, an inline method: integrated into
  Run, loop.c leaves `landTimer[i]`'s address unreduced, which is Run's
  ROM code, where the same loop written out in Run (or compiled on its
  own, as the unused `ResetLogoPieces`, which is that plain loop)
  strength-reduces it. Both functions had hand-written `goto` loops over
  byte offsets with a `MATCH_CONST` and five `MATCH_USE`s.
- **A block-local value for a decrement.** In `CompanyLogos::Run`'s
  fade-out, `v--; fade = v;` keeps `v` live through the branch, so
  local-alloc gives that block's BLDY address r1 first and global-alloc
  puts `v` in r2 (an r1 pin before); `s32 n = v - 1;` as in the other
  branch leaves r1 to `v`.
- **A pointer local instead of a padding barrier.**
  `LinkSession::ResetState`'s nibble decrement through its own
  `struct nibble_pair *nb` breaks the global-alloc priority tie the
  `MATCH_BARRIER()` broke with an extra insn.
- **A reload the ROM got for free.** `SaveMenu::InitIcons`' frame-0
  icon: with `tag` a plain field store, old_agbcp's read-modify-write
  leaves a dead zero mask that loop.c hoists out of the loop; with no
  free register it is rematerialized by reload at frame 0's store, after
  the address, in r1, which also moves the reload-register rotation on.
  That was the frame-0 address pin and the r1 hold.
- **Where the kept sites are decided** (each comment has the details):
  gcse's PRE hoists `slot << 5` into the tile loops' preheader in
  `Credits::LoadLogos` (it ignores hard registers, hence the pin); cse1
  propagates the key-word copy in `LevelSelect::Loop` and the ring copy
  in `LinkSession::HandleSerial`; reload_cse_regs deletes
  `ContinuePrompt::Loop`'s second `lsrs r1, r2, #16` (r1 still holds
  it); combine folds `SaveData::TestFlags`'s parameter extension into
  the `and`; regmove/reload build `CameraLead::Reset`'s `and` in the
  dying 1's register (the ROM's is a copy of `v`, as if the 1 were
  still live); cse2 merges `LinkSession::Update`'s two 1s; global-alloc
  priority (`floor_log2(refs) * refs / length`) for
  `DrawWrappedText`'s `len`, `ResetState`'s `id` and
  `ContinuePrompt::Loop`'s `audio`; local-alloc's tie of a shift to the
  dying operand in `PauseMenu::Draw`; reload's spill-register rotation
  for `DrawWrappedText`'s r1 hold (under `-fno-rerun-loop-opt` the
  hold-free code is two reload registers off, nothing else).

**Round 4, src/player, src/level and src/text (C++).** 9 functions -> 5,
from the passes' source (an instrumented private old_agbcp printed
local-alloc's quantity order):

- **Code jump2 merges again.** Two tails the ROM shares by cross-jumping
  can be written twice, as plain code: `StartTornadoFall` queues its Y
  entry in each `case` (each call's entry is a block-local pseudo, which
  local-alloc places in r2 before global-alloc ranks `this`; one `entry`
  queued after the switch is global and loses r2 to `this`), and
  `DrawWrappedText` ends both branches of a measured token with their own
  flush and `posAccum += len`, which gives `len` the extra loop-weighted
  reference its `MATCH_USE` supplied (floor_log2(16) * 16 / 97 against
  `self`'s 4 * 17 / 141) and drops the draft's gotos and r1 hold.
- **A narrower local changes a live length.** `SpawnFlamethrowerLabAssistant`
  reads the mirror bit into a `u8`: the loaded byte then lives 14 insns
  instead of 12 and ranks just below the mirror address (0.21 against
  0.22), which is the global-alloc order the ROM's caller-saved r3 needs;
  the hold, keep, stack-slot use and four uses went. `EndSpin`'s L bit as
  a `u16`: an HImode 0x200 can't be a movhi immediate, so reload builds it
  in r1 and copies it into the constant's register, the ROM's
  `adds r0, r1, #0`.
- **Kept, with the condition in each comment:** the dead `state` and
  `maskLevel` loads (jump2's delete_computation keeps the feeding load
  after reload, flow2 has already run, so the test's body must be dead
  code flow1 removed or arms jump2 merges); the bounce and `StateJump`
  1s (cse puts two equal constants in one extended block into one
  quantity in either order; only an init moved later by update_equiv_regs
  escapes it); `StateCrouch` (local-alloc's three-quantity exchange
  allocates the address first whatever the priorities, the volatile byte
  load is a fourth quantity); `ReleaseHang` (find_reload_regs spills the
  first free call-clobbered register in number order, so r2 must be live
  at the add, and nothing is).

**Round 4, objects/, crates/ and actor/ (C++), from the compiler
source.** 10 functions -> 5. One mechanism was behind five of them:

- **A stack box's address through an inline argument.** A `struct
  aabb` is 16 bytes, so BLKmode, and Thumb's GO_IF_LEGITIMATE_ADDRESS
  rejects any frame address in a mode under 4 bytes (BLKmode's size is
  0). expr.c then copies a box local's address into a pseudo at every
  use (`&f.b` for a call argument, `b.y` for a field read), and cse1
  ties the copies to the first one, or to a pointer local already
  holding it. The ROM recomputes `add r0, sp, #16` per call and reads
  the fields at their sp offsets. integrate.c expands an inline
  function's arguments with EXPAND_SUM, where a standalone local's
  address is `(plus virtual-stack-vars 16)` itself; process_reg_param
  records it in const_equiv_map and substitutes it for the parameter,
  so the inlined body has no pseudo to tie. aabb.h's field accessors
  (AabbX/AabbY/AabbW/AabbH) and FlipAabbX/FlipAabbY and util.h's
  SetAabb (SetAabbPos then SetAabbSize) replace the crate box builders'
  six `BOX_ADDR`s (with the frame structs split into locals in the
  same stack order: a member's address is still copied),
  `MovingSprite::TouchPlayer`'s volatile read and
  `Platform::ResolveCollision`'s `MATCH_KEEP` and GetSpriteHitbox
  alias. A private old_agbcp built without that BLKmode test compiles
  the two objects' plain code to the ROM's bytes as well, which is how
  the mechanism was confirmed.
- **Kept, with the exact condition in each comment:**
  `CameraLead::Reset` (the `and`'s dying 1 is tied to its result by
  both regmove's fixup_match_1 and local-alloc's combine_regs; only a
  1 that lives on or is a remote constant escapes, and the toggle shows
  either), `Sprite::CheckPlayerContact` (cse's insert_regs puts every
  SImode 1 in one quantity, so the gone bit's shift takes r6 unless r6
  is set again first, and then the OR's result moves into r6),
  `ActorSelf::Draw` (local-alloc: no quantity holds r4 over the
  projection's life, and SMALL_REGISTER_CLASSES turns off block_alloc's
  widened lives; one variable for the projection and the screen x gets
  4 lines off, regmove's replacement_quality then picks the mask's
  register), `PlatformMover::Update` and `SelectActorCategory`
  (global-alloc priorities 0.83/0.74 and 0.129/0.133).

**Round 4, lib/gax, src/iwram and the two ConvertTiles (from the
compiler source).** No function loses its last workaround; two lose
sites:

- **One variable for a list of the same shape.** `GAX2_init`'s r3 hold
  went: `layout` walks on to types[2], the alternative-layout list (a
  count and pointers, like a layout), instead of a second `subs` local.
  As one pseudo it conflicts with the inner scan's `next` (r3), and
  global-alloc gives it r4 as in the ROM.
- **A volatile field instead of a volatile local.** Only
  `GaxMixItem.done`, which the ARM mixer advances, needs to be re-read;
  as a `volatile` field `GaxChannelMix`'s item is a plain local.
- **Kept, with the exact condition in each comment:** the ConvertTiles
  masks (regmove copies the first non-dying AND operand; cse1 swaps a
  known-constant first operand second; so the mask must be set where
  cse1 can't see it but loop.c won't move it out of the row loop, which
  needs a conditional jump before it: only a guard duplicating the
  pixel loop's entry test does that, and it costs the ROM's `n << 4`
  recompute); `GAX2_init`'s two uses (global priorities, the numbers in
  each comment); `GaxChannelMix`'s instrument re-read (gcse PRE finds it
  redundant: nothing kills it, `__muldi3` being a const libcall) and its
  clamp keep.
- **The IWRAM ARM compiler: fixed by the compiler options.**
  `HeapSortActorsByKey`'s barriers and `strncpy_arm`'s are stock
  2.9-arm-000512 behaviour no C avoids (find_cross_jump's lowered
  minimum after a label; jump.c's conditional RETURN). Private builds of
  agbcp_arm_patched that skip the label rule, or refuse conditional
  returns, compile sprite_arm.o and string_arm.o byte-identical to the
  ROM's without them: more evidence for
  [the later ARM gcc](matching/iwram-image.md). The owner adopted them
  afterwards as two more opt-in options of
  agbcc_arm_prologue_return.patch (`-mstrict-cross-jump` for
  sprite_arm.o, `-mno-cond-return` for string_arm.o), and both
  functions are now plain code (iwram-image.md, "Ninth step").
- **Other configurations.** Every round-4 function's plain C (all its
  sites removed) was compiled under agbcc/old_agbcc, agbcp/old_agbcp
  or agbcp_arm_patched with -O1/-O2/-O3/-Os x prologue-bugfix on/off x
  -mthumb-interwork on/off x caller-saves on/off, and for C++
  -mtpcs-frame/-mtpcs-leaf-frame and the scheduling switches (ARM: the
  frame pointer, scheduling and the patch's options). None matches; the
  notyourav `cp` tree's gcc/ is semantically identical to pret's agbcc.

**Round 4 in menus/, link/, save/ and frontend/**, from the compiler
source (8 functions -> 5):

- **A variable set and dead twice is global-alloc's.** local_alloc only
  takes a pseudo with REG_BASIC_BLOCK >= 0 *and* REG_N_DEATHS == 1;
  there it ties an output to an input that dies in the insn.
  `PauseMenu::Draw`'s centring and right-alignment had their own `x`
  and `t` each, so `x` was tied to the dying `t` and SetPos's `y` got
  r3 (two holds and two uses fixed that). One function-scope `x, t`
  pair for both makes them global: no tie, `y` takes r2 first and `x`
  r3, as in the ROM.
- **Two `if`s and a `goto`, not `A || START` and a `break`.**
  `ContinuePrompt::Loop`'s START test keeps its own `lsrs r1, r2, #16`
  because reload_cse_regs forgets every register value at a code label,
  and the A test's false branch jumps to one in front of it until jump2
  cross-jumps the two identical sound-and-leave tails (the ROM's
  layout). The tails leave by `goto done`: a `break` in the A test is a
  jump to the loop's end label within LOOP_TEST_THRESHOLD (30) insns of
  the top, so expand_end_loop would rotate the key reads and the A test
  to the bottom; a user label doesn't count, and only the `dir` test
  moves (combine then folds it away, `dir` being only 0 or 1). That
  freed its `MATCH_KEEP` and `MATCH_USE`.
- **A struct passed by value to an inline hides copies from cse1.**
  `LevelSelect::Loop` reads the direction keys through
  `KeyHalf(union key_state)`, which returns `.half`. The inline's
  parameter and return value are unions in ADDRESSOF pseudos, so cse1
  sees the copies through them as memory moves; they become register
  copies only in the addressof pass, after cse1, and one survives: the
  ROM's `adds r1, r2, #0` between the 0x80 test's `ands` and `cmp`. The
  decomp-permuter on a C port found it (in 25 minutes; a `MATCH_KEEP`
  before).
- **Kept, with the deciding code in each comment:**
  `SaveData::TestFlags` (regmove's optimize_reg_copy_1 moves the test
  onto the copy `result = v`; only a label, jump or loop note between
  the two, or a hard register, stops it; the permuter's 472 zero-score
  variants all used junk for that), `LinkSession::ResetState`
  (priorities 1.21 for `self` against 0.10 for `id`, which would need
  12 references where the code has 3), `Credits::LoadLogos` (gcse's
  PRE hoists `slot << 5`; the shift of a hard register is never a PRE
  candidate), `SaveTransfer::ReceiveChunk` (cse1 shares the two 0xc8
  constants and with them the product, where the ROM shares only the
  constant) and `LinkSession::Update` (45-minute decomp-permuter runs
  on C ports of these four found only junk), and
  `LinkSession::HandleSerial` (`rf = &this->ring` does give a second
  register, through gcse's PRE, but in the wrong place).

**Round 5, src/enemies and src/bosses (C++), looking outside the
function body.** 11 functions -> 3. Round 4 had proved that no edit of
these bodies reaches the ROM; what does is a helper the bodies didn't
use, and one libcall:

- **Whole-position setters with one coordinate unchanged.** Seven of
  the eleven write a position. Written through
  `Entity::SetPos(x, y)` with the other coordinate passed back
  (`part->SetPos(part->x, baseY)`), the unchanged coordinate is a pseudo
  through register allocation, and after reload `reload_cse_regs`
  deletes its store as a no-op (the register still holds that memory
  word, no call or store in between) and flow2 deletes the load. The
  output has no trace of it, but the allocation does: `UpdateBob`'s and
  `UpdateOscillateY`'s saved registers no instruction uses (the x held
  r5 or r8 over the body; this is the "pseudo whose code is gone" of
  round 4), `UpdateOscillateX`'s product in r2, `UpdateTriggerBox`'s
  and `StompedHopPadCtrl::Update`'s extra reference that reorders
  global-alloc, `UpdateHop`'s value in r0 under the baseY copy, and
  `TinyCtrl::SetState`'s y in r0 that keeps `pad` off its r0
  preference. The permuter had found most of them in rounds 2-4 as
  "dummy stores" (`target->x = target->x`) and they were rejected as
  junk; the setter is the natural spelling of the same RTL. Look for it
  wherever a ROM function saves a register it never uses, or global
  priorities need one more reference to a pointer whose fields are
  stored.
- **`/` is a const libcall; a call to `__udivsi3` is not.**
  `UpdateOscillateY` needs the x word known across the division: g++
  expands `INT_TO_Q8(gRoomFrameCount) / period` as a libcall block
  marked const, which `reload_cse_regs` doesn't treat as clobbering
  memory. The explicit `__udivsi3(...)` call the code had (a plain
  `extern` declaration) invalidates every memory value, so the x store
  stays (`mov r1, r8; str r1, [r5]`). The ROM is the same bytes either
  way unless something after the call depends on memory being known.
- **A tail written once more for jump2 to merge.**
  `MegaMixCtrl::Update`'s state 2 runs `Run(this, part)` itself when the
  player is out of range, instead of a `goto` into state 0's copy. The
  three copies are one in the ROM (cross-jumped after reload), but this
  one's reloads (each virtual call's `ldrsh` scratch) still advance
  reload's spill-register rotation, which puts the `dead` read's 0x104
  in r3; that was the `PlayerDeadByte` r3 hold and keep.
- **The `+ 0x100` in the oscillators' index** is `(t + 0x100 - phase) &
  0xff`, which fold-const turns into the ROM's `t - (phase +
  0xFFFFFF00)`; the `Wave` helper that took `phase - 0x100` as a
  parameter to avoid the fold went.
- **Kept:** `DingodileShieldCtrl::Update` (tried: struct-returning
  `GetAttackBox()`/`GetBodyBox()` methods, inline player-box helpers
  shaped like the matched `DingodileProjectileCtrl::Update`'s, the
  BLDCNT chain's start at function scope (gcse's cprop folds it into
  the chain and reload rematerializes it in r0/r1), through a reference
  or a member function on a local, or as a `u64`; none frees r5/r6),
  and the two ConvertTiles masks (the matched sibling idioms in the ROM,
  `bg_picture.cpp`'s `MapFill` and `sprite_arm.cpp`'s ARM
  `UnpackNibbleTiles` with its `ExpandNibble` ternary, were tried in
  every combination of helper and body: 100-118 lines off, as against
  the asm-free near miss's 8).

**Round 5 in link/, save/, frontend/ and lib/gax (outside the function
body).** 8 functions -> 7:

- **A byte offset built in two statements hides a product from cse1.**
  `SaveTransfer::ReceiveChunk` reads the count through `offset =
  playerIndex * sizeof(LinkPlayer); offset = offset + (s32)s;`, the
  shape of `SaveData::ReadSlot`/`WriteSlot`. The second statement
  overwrites the register holding the product, so when
  `&s->players[playerIndex].ring` multiplies again cse1 shares the 0xc8
  constant but has no register left with the product: the ROM's second
  `muls`. In one statement the product is reused. The pop is
  `LinkRing::Pop`, the inline counterpart of `Push`. That removed a
  `MATCH_HOLD_REG` pair and an empty asm.
- **An inline's non-register argument is a copy cse1 may not see
  through.** integrate.c copies an argument that isn't a register into a
  fresh pseudo at the call; after a loop (a new extended basic block)
  cse1 doesn't know it equals an earlier address, and gcse's PRE turns
  it into a copy of the reaching register. That is
  `LinkSession::HandleSerial`'s ring copy (`this->ring.Pop`), but the
  copy lands at the call, after the clamp; the comment there has the
  numbers.
- **Compiler hypothesis tested and refuted:** a private old_agbcp
  without regmove's optimize_reg_copy_1 compiles `SaveData::TestFlags`'s
  natural form as the ROM, but changes 16 other old_agbcp objects that
  match now.
- **Kept** (the round-5 attempts are in each comment):
  `LinkSession::Update`, `LinkSession::ResetState`,
  `LinkSession::HandleSerial`, `SaveData::TestFlags`,
  `Credits::LoadLogos`, `GAX2_init` and `GaxChannelMix`.

**Round 5, player/, objects/ and actor/ (outside the function).** 10
functions -> 6 (ActorSelf::Draw, SelectActorCategory and ReleaseHang
free; PlatformMover::Update loses its 0x300 pin and keep):

- **A sibling's inline.** `ActorSelf::Draw`'s second half is the inline
  `DrawScaledFrame` that polar_pickups.cpp already had for
  `PolarCollectedWumpa::Draw`; as ActorSelf's inline `DrawFrameAt`
  (actor_self.hpp, used by both) the screen X and Y are the inline's
  parameters, copied in after the projection, and the projection shares
  r5 with the screen X without the pin. The `/` operator works as well
  as the explicit `__divsi3` calls there.
- **The sibling's loop test, a tie and a declaration's place.**
  `SelectActorCategory`'s skip loop with RunActorCategoryFrame's
  SUB_EFFECT_DUE test (the record address through `off` and `tb` in a
  comma expression) and as a `for` brings `&gActorSpawnIndex` and
  `base` to the same global priority (0.1333); allocno_compare then
  takes the lower pseudo, and `s32 base = GetCellAnimDistance();`
  declared at its set (C++) is created after the address's pseudo. With
  `base` declared at the top, or a `while`, they still swap.
- **A setter with the other coordinate passed back** (above):
  `part->SetPos(part->x, part->y + 0x600)` in `ReleaseHang` keeps the X
  in r2 across the add, which is what made reload spill r3 for the
  0x600 (an instrumented reload1.c shows the spill set r1, r2, r3, r6
  against r1, r2, r6). PlatformMover::Update's wobble the same way (the
  -0x300 in r5, the 0x300 in r2).
- **Kept, with what was tried in each comment:** the two HandleEvent
  dead loads (the dead arm's register also needs a use elsewhere in the
  function, or cse1's delete_trivially_dead_insns removes it before
  flow1: every inline form, unused parameters, discarded returns,
  `const bool &`, empty inlines and do/while(0) asserts, dies there);
  the bounce and StateJump 1s and Sprite::CheckPlayerContact's 1 (a
  private old_agbcp whose cse doesn't link a constant's registers, in
  four forms, fixes none and changes up to 13 other functions per
  object); StateCrouch (FaceRight-style inlines keep the three
  quantities); CameraLead::Reset; PlatformMover::Update's `now` (`part`
  61 references over 388 insns with the SetPos calls, 64 needed).

**Round 6, player/ and objects/ (the class's own accessors, and one
branch per test).** 7 functions -> 5 (Sprite::CheckPlayerContact and
PlatformMover::Update free):

- **The base class's inline accessors.** `Sprite::CheckPlayerContact`
  written with Entity's IsTouched, IsContactEnabled, SetTouched and
  MarkGone (entity.hpp) instead of open-coded flag tests and
  ENTITY_SET_GONE_BIT is the ROM, register for register, with no pin:
  the u8 accessors give the `lsl #24` the C spelled as `f.flags << 24`
  and the shared 1, and MarkGone's bitmap write loads its own 1. When a
  function tests or sets an object's flags, try the class's accessors
  before tuning the expression.
- **A branch per test, each with its own copy of the body.**
  `PlatformMover::Update`'s two hold-first-frame tests (type 7 not yet
  active, type 6 before its time) as two `else if` branches, each calling
  HoldFirstFrame, instead of one `||` branch: jump2's cross-jumping
  merges the copies after reload, so the ROM has one, but each copy
  counted in global-alloc's priority (`part` 61 -> 64 references), which
  is what the `now` pin stood in for. Duplicated tails merged in the ROM
  are worth trying whenever a pointer is a few references short.
- **Kept:** the two HandleEvent dead loads, the bounce and StateJump 1s,
  StateCrouch and CameraLead::Reset, with what was tried in each
  comment.

**Round 6 in link/, save/, frontend/ and lib/gax.** 7 functions -> 7;
the new evidence is in each comment:

- **A flag that is two switches.** `-fno-expensive-optimizations` on
  save_data.o turns off regmove's optimize_reg_copy_1 (the copy that
  moves `SaveData::TestFlags`'s test) and every other function still
  matches, but it also turns off stmt.c's preserve_subexpressions_p, and
  the parameter's zero-extension then splits. Test both effects of a
  flag before blaming one pass.
- **A const libcall leaves a trace.** In `GaxChannelMix` the ROM keeps
  `self->row` in a register across `__muldi3`, so that call didn't
  clobber memory. The reload of `self->instrument` after it has another
  cause. Through the libgcc.h prototype the reload is right and the rest
  differs only in allocation (84 lines).
- **Unused siblings as inlines.** `LinkSession::Update`'s started block
  is the unused `LinkSession::Start`, and its SIOCNT setup is
  `LinkSetupSio`. Inline copies of both give the same code apart from
  the 1s that the keeps are for.
- **Flag sweep, whole objects, workarounds removed.** For all seven
  sites: -fno-expensive-optimizations, -fno-gcse, the cse/loop/regmove/
  force-mem/caller-saves switches, -fargument-alias/-noalias(-global),
  -f(no-)strict-aliasing, -fno-inline, -fkeep-inline-functions, -O1 and
  -O3. None removes a workaround. -fno-strict-aliasing changes the GAX
  objects and credits.o, so strict aliasing is the compilers' default.

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

The 8 spelled-out sites were `tools/match_idioms.py`'s `ALLOWED_SPELLED`
list, which `--check` (and CI) enforces. The cortex.c pins went with its
C++ conversion; the 3 left are described under
[Other empty-asm forms](#other-empty-asm-forms). The counts in these
tables are as of part 4; `tools/match_idioms.py` gives the current ones.

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

Re-testing whether individual pins and nudges are still needed (the
optional item in #576) was left to #662: `tools/match_prune.py` does it
for every site. With it, the rewrites after it and the C++ conversion
(#664), the pins went from 2153 to 89 ([above](#pruning-workarounds)).

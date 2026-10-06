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
  `ALLOWED_SPELLED` list, also [below](#other-empty-asm-forms)); CI runs
  it with `--check-macros`. If a new shape really has no macro, add one
  to match.h with a `--check-macros` case, rather than a new exception.
- **Per-object flags** (old_agbcc, `-O1`, `-fno-strength-reduce`, ...)
  are set in the Makefile, each list with a comment giving its evidence,
  and listed in the [table below](#per-object-flags). A new one needs the
  same evidence: every matched function in the object stays exact.
- Prefer a source-shape fix to any workaround, and remove a workaround
  when a clean rebuild shows it's no longer needed.

## Contents

1. [Compilers and flags](#compilers-and-flags): old_agbcc vs agbcc,
   -O1 SDK code, per-object flags, agbcc_arm
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
10. [Survey and conversion record (#576)](#survey-and-conversion-record-576)

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
(`SpawnSquid` and the r7 push).

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
| `-fno-strength-reduce` | `title_screen.o` | `InitVvLogoPieces` keeps an up-counting loop that `check_dbra_loop` would reverse. Not global: it breaks 11 other old_agbcc files, and `DrawVvLogoPieces` (split into `company_logos.c`) needs the reversal. |
| `-fno-rerun-loop-opt` | `link_session_reset.o` | The second loop pass reverses `ResetLinkSessionState`'s copy loop; the flag breaks `HandleLinkSerial`, hence the split. |
| `-O1` | `lib/agb_eeprom` (4 objects) | SDK code, above. |
| no `-mthumb-interwork` | libgcc2 (`__divdi3`, ...) | The only ROM functions that return with `pop {r4-r7, pc}`. |
| agbcc_arm, `-fomit-frame-pointer` | `string_arm.o`, `sprite_arm.o` | ARM code of the IWRAM image ([matching/iwram-image.md](./matching/iwram-image.md)). |

No flag turns gcc 2.9's loop optimizer off wholesale
(`-fno-loop-optimize`, `-fno-schedule-insns` and `-fno-crossjumping`
don't exist in it). See
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
  `ResetTitleLogoPieces`; `src/frontend/title_screen.c`).
- **Hoisting.** If gcc hoists an invariant the ROM recomputes in the
  loop, put a `MATCH_KEEP_VOLATILE(base)` at the use
  ([sub_8009150-loop-invariant-hoist-matched.md](./matching/archive/sub_8009150-loop-invariant-hoist-matched.md),
  `LinkCrateToActiveBucket`; `src/crates/crate_grid_link.c`).
- **Insn counts.** loop.c's decision to move an invariant depends on the
  loop's insn count; `MATCH_BARRIER()`s in the body change it
  (`src/frontend/company_logos.c`).
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

Five pins stay written out as `register T x asm(R)`: the ones in
`src/bosses/cortex.c`'s `SET_FRAME_R`, `MARK_GONE` and
`MARK_GONE_BITMAP_OFF` macros, where the register is a macro parameter
holding a string (`R_FRAME`, `R_TAG`, `R_FLAGS`, `R_CUR`, `R_BASE`,
passed as `"r5"` etc.). `MATCH_HOLD_REG` stringizes a bare register name,
so it can't take those. `tools/match_idioms.py --kind pin` lists them.

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

The insn to cover is the one listed under "Spilling for insn
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
  was moved or merged (the `2` sunk to its store in
  `src/bosses/hovercraft_launcher.c`, the recheck in
  `src/bosses/hovercraft_cannon_flash.c`).

| Spelling | Macro | What gcc 2.9 does with it | Typical use |
|---|---|---|---|
| `asm("")` | `MATCH_BARRIER()` | Scheduling barrier; opaque to cross-jumping; counts toward insn-count heuristics. Doesn't make gcc forget register values. | Keep a load/store on one side; keep tails apart; insn-count padding (loop.c, GCSE slot order). |
| `asm("" : : "r"(x))` | `MATCH_USE(x)` | `x` must be live in a register here: extends its range and adds a reference (raising its allocation priority; loop-weighted inside loops). | Keep a value in its register past a call; reach the ROM's register priority; end a hold. |
| `asm("" : "+r"(x))` | `MATCH_KEEP(x)` | `x` has an unknown value afterwards: no constant propagation, CSE, rematerialization or immediate folding through it. | Keep a per-branch reload; stop `w & 0xff` folding; `BOX_ADDR`. |
| `asm("" : "=r"(x))` | `MATCH_HOLD(x)` | Defines `x` here with an unknown value. With a pin, occupies that register until the last use. | Start a hold; a deliberately undefined value. |
| `asm("" : "=r"(v) : "0"(K))` | `MATCH_CONST(v, K)` | Loads K into v's register here, but v isn't a known constant: not CSE'd with another K, hoisted, sunk to its store or folded into an immediate, and no doubled live range. | The ROM loads a constant at a given point, once per use (e.g. before the address); an opaque copy of a pointer. |
| `({ T _p = (e); asm("" : "+r"(_p)); _p; })` | `MATCH_KEEP_EXPR(T, e)`, `BOX_ADDR(a)` | An expression whose value is opaque at each use. | Stack-box addresses, above. |

Cases: [near-miss-polish-3.md](./matching/archive/near-miss-polish-3.md)
(constant-init), [sp-box-retry.md](./matching/archive/sp-box-retry.md)
(`"+r"` vs `"=r"/"0"`), `src/actor/cell_anim.c` (`MATCH_CONST` as an
opaque pointer copy), `src/actor/actor_category_select.c` (use).

### Other empty-asm forms

The rarer forms, a few sites each:

| Spelling | Macro | Sites | What it does |
|---|---|---|---|
| `asm("" : : "r"(a), "r"(b))` | `MATCH_USE2(a, b)`, `MATCH_USE2_VOLATILE(a, b)` | 3 | `MATCH_USE` of two values in one insn. Not the same as two `MATCH_USE`s, which are two insns. |
| `asm("" : : : "r5")` | `MATCH_CLOBBER(r5)`, `MATCH_CLOBBER_VOLATILE(r4)` | 3 | Tells gcc the register is clobbered, so the prologue saves it even though nothing uses it, as the ROM does ([issue-9-raw-asm-pass.md](./matching/archive/issue-9-raw-asm-pass.md), `UpdateEnemyBob`; `src/enemies/enemy_ctrl.c`); also forces a reload of whatever it held (`src/level/play_room.c`). |
| `asm volatile("" ::: "memory")` | `MATCH_MEMORY_BARRIER()` | 2 | Makes gcc forget memory and acts as a barrier. It does not stop address CSE, which is what it was usually tried for (`src/frontend/title_screen.c`). |
| `asm("" : "+m"(x))` | `MATCH_KEEP_MEM(x)` | 2 | `x` is in memory here with an unknown value, so a later read is a real load (the `ldm r1!` re-read in `ConvertAirshipTiles`). |
| `asm("" : : "m"(x))` | `MATCH_USE_MEM(x)` | 1 | `x` must be in memory here: keeps it in its stack slot across a call (`src/level/spawn_enemies.c`). |

An asm that reads a field through `"m"` can also fix the order of a load
against a constant
([issue-59-60-m-operand-scheduling.md](./matching/archive/issue-59-60-m-operand-scheduling.md)).

Three empty asms stay written out, each a one-off shape that no macro
covers (a macro for one site would only add a name to look up). They
are `ALLOWED_SPELLED` in `tools/match_idioms.py`, with the five cortex.c
pins ([Register pins](#register-pins)):

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
  `UpdateTitleLogoPieces`; `src/frontend/title_screen.c`). The opposite
  case, re-reading through `self` instead of caching a local so old_agbcc
  makes the ROM's copies, is in near-miss-polish-3.md.

## Assembler-level fixes

### `.align 2, 0`

When a function is the last in its object and its size isn't a multiple
of 4, `as` pads it with Thumb NOPs (`0xC046`); the ROM has zeros. A
file-scope `asm(".align 2, 0");` after the function fixes it. A match
that differs only in its last 1-2 bytes is this, not the C. It recurs at
every file split (276 sites). See the gotcha at the top of the old log,
[matching.md](./matching.md).

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
uninitialized register (a `& 0xFFFF0000` on garbage before ORing in
BGxCNT bits, a `bestIdx` when the count is 0), an initializer would add
code, so the variable stays uninitialized and is self-initialized:
`u32 bg0cnt = bg0cnt;` emits nothing and silences `-Wuninitialized`
(6 sites; `src/frontend/starfield.c`,
[issue-65-naked-retry.md](./matching/archive/issue-65-naked-retry.md)).
The order of escape hatches (self-init, `UNUSED`, a per-object
`-Wno-...`) is in CONTRIBUTING.md's "Compiler warnings".

## NON_MATCHING

`include/core.h` defaults `NON_MATCHING` to 0; `make NON_MATCHING=1`
defines it for C (`-D`) and assembly (`--defsym`). A function that
can't be matched yet keeps its C draft under `#if NON_MATCHING` and the
checked-in `NAKED` transcription under `#else`; the progress report
scores the C draft. Both builds have to work. Two functions remain,
both ARM code of the IWRAM image (`itoa_arm`, `LookupSpriteFrameCache`,
#553), whose returns and prologues point to another ARM compiler build
than agbcc_arm. See
[matching/iwram-image.md](./matching/iwram-image.md) and
[naked-transcription-parked-functions.md](./matching/archive/naked-transcription-parked-functions.md).

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
| `MATCH_USE(x)`, `MATCH_USE_VOLATILE(x)` | `asm("" : : "r"(x))`, `asm volatile(...)` |
| `MATCH_KEEP(x)`, `MATCH_KEEP_VOLATILE(x)` | `asm("" : "+r"(x))`, `asm volatile(...)` |
| `MATCH_HOLD(x)`, `MATCH_HOLD_VOLATILE(x)` | `asm("" : "=r"(x))`, `asm volatile(...)` |
| `MATCH_CONST(v, K)`, `MATCH_CONST_VOLATILE(v, K)` | `asm("" : "=r"(v) : "0"(K))`, `asm volatile(...)` |
| `MATCH_USE2(a, b)`, `MATCH_USE2_VOLATILE(a, b)` | `asm("" : : "r"(a), "r"(b))`, `asm volatile(...)` |
| `MATCH_CLOBBER(rN)`, `MATCH_CLOBBER_VOLATILE(rN)` | `asm("" : : : "rN")`, `asm volatile(...)` |
| `MATCH_MEMORY_BARRIER()` | `asm volatile("" : : : "memory")` |
| `MATCH_KEEP_MEM(x)`, `MATCH_USE_MEM(x)` | `asm("" : "+m"(x))`, `asm("" : : "m"(x))` |
| `MATCH_KEEP_EXPR(T, e)` | `({ T _p = (e); asm("" : "+r"(_p)); _p; })` |
| `BOX_ADDR(a)` | `MATCH_KEEP_EXPR(struct aabb *, a)` |

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
| file-scope `.align 2, 0` | 276 / 131 | an assembler directive |
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
rebuild shows the object stays identical.

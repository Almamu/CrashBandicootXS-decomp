# C++ (#664)

The game was written in C++ and built with the g++ 2.9 that agbcc comes
from. The decompilation reproduces it as C, which means writing out by
hand what the C++ compiler generated: vtables, vtable pointer stores,
`this` adjustments, pointer-to-member calls, `new` and `delete`. This
page records what the ROM shows, the C++ compiler that matches it, the
objects built from C++ source so far ([Progress](#progress)), and how to
carry on.

**Short answer.** The C++ front end of the same compiler reproduces the
game's code byte for byte, and the C++ source is shorter and closer to
what was written. Every C++ pattern tried (constructors, destructors,
virtual calls, `new`, pointer-to-member calls) compiles to exactly what
the ROM has, with none of the C emulation. The recommendation is to
convert gradually, class family by class family, through the Makefile's
`CXX_OBJS`, starting with the controllers. The libraries stay C.

Contents:

1. [What the ROM shows](#what-the-rom-shows)
2. [Survey: which objects are C++](#survey-which-objects-are-c)
3. [The compiler: agbcp and old_agbcp](#the-compiler-agbcp-and-old_agbcp)
4. [g++ 2.9's C++ ABI, as the ROM has it](#g-29s-c-abi-as-the-rom-has-it)
5. [Experiments](#experiments)
6. [How a C++ object is built](#how-a-c-object-is-built)
7. [Progress](#progress)
8. [Recommendation and plan](#recommendation-and-plan)
9. [Dead ends and gotchas](#dead-ends-and-gotchas)

## What the ROM shows

| C++ artefact | In the ROM | What it means |
|---|---|---|
| g++ 2.x vtables (`{s16 delta, s16 index, void *pfn}` slots) | 93 tables, 831 slots, `0x087E3BEC`-`0x087E55E4` | polymorphic classes, built without `-fvtable-thunks` (the gcc 2.x default) |
| vtable slot 0 (the RTTI slot, `__tf<class>`) | empty in all 93 | built with `-fno-rtti` |
| a nonzero `this` delta in a vtable slot | none of 831 | no multiple inheritance that overrides through a second base |
| a nonzero `thisOffset` in a pointer-to-member table | none of 100 entries (`ACTOR_PMF`) | the same |
| type_info records, type-name strings (`10EffectCtrl`) | none | no RTTI |
| `__pure_virtual` ("pure virtual method called") | none | no pure virtual slot is ever emitted |
| exception tables, `__throw`, `__eh_*`, `terminate` | none | built with `-fno-exceptions` (or no `throw`) |
| static constructor lists (`__CTOR_LIST__`, `.ctors`, a `__main` call) | none: crt0 calls `AgbMain` directly, and `AgbMain` calls no `__main` | no global object has a constructor |
| `operator new`/`delete`/`new[]`/`delete[]` | `OperatorNew` & co. in `src/level/camera.c` | the game's own replacements: see below |

**The operators.** In g++ 2.x the global `operator new(size_t)` has the
assembler name `__builtin_new`, and `new[]`, `delete` and `delete[]` are
`__builtin_vec_new`, `__builtin_delete` and `__builtin_vec_delete`. A
`new X` expression calls `__builtin_new` and then the constructor; a
`delete p` calls the destructor through the vtable with `__in_chrg` = 3.
libgcc's own versions (new1.cc, new2.cc) call `malloc` and the new
handler. The game's four, in camera.c, call `mem_alloc(size,
MEM_HEAP_EWRAM)` and `mem_free`: they are the game's replacement global
operators, and `OperatorNew` is `__builtin_new`. None of libgcc's C++
support (new handler, `__pure_virtual`, `__terminate`) is linked.

**Where the vtables are.** g++ 2.9 puts a vtable in a writable data
section (`.gnu.linkonce.d._vt.<class>`), not in `.rodata`. The 93 tables
are the last thing in the ROM before the IWRAM image, after all the
constant data: exactly where `.data` goes (the GBA build left `.data` in
ROM). The region is nothing but vtables (831 * 8 bytes = 0x19F8). Their
order follows the order of the classes' code: taking each table's
destructor as its class's marker, 85 of the 93 are in ascending code
order, which is what you get when each vtable is emitted with its class's
first virtual method.

**What is C.** The libraries under `lib/`: Shin'en's GAX2, Nintendo's
AgbEeprom SDK, libgcc and the BIOS wrappers ([libraries.md](libraries.md))
have none of the traits. The IWRAM ARM routines were built by agbcc_arm,
the ARM C compiler. For the rest of the game, being "C-like" doesn't mean
it was C: the C++ front end compiles plain C-style code to the same bytes
as the C front end (80% of the game's files, see
[experiment 4](#4-every-game-file-compiled-unchanged-as-c)), so a file
with no C++ constructs can't be told apart.

## Survey: which objects are C++

`tools/cpp_survey.py` classifies each object by the C++ runtime
structures its C writes out by hand. Its output on this branch:

| Trait | Game objects | Count | What it is |
|---|---:|---:|---|
| method | 82 | 415 functions | a vtable slot or a pointer-to-member table points at it |
| vptr | 56 | 200 stores | `x->vtable = gFooVtable`: constructors and destructors |
| ctor | 46 | 90 functions | stores a vptr and returns the object (a constructor returns `this`) |
| dtor | 22 | 38 functions | stores a vptr and takes the `flags` (`__in_chrg`) argument |
| new | 46 | 157 calls | `OperatorNew(...)` / `OperatorNewArray(...)`; 86 are the direct `CreateX(OperatorNew(size))` = `new X` shape |
| delete | 40 | 79 calls | `OperatorDelete(...)` / `OperatorDeleteArray(...)` |
| vcall | 79 | 356 sites | a virtual call: the slot's `delta` added to the object, then `_call_via_rN` |
| pmf | 15 | 29 sites | a pointer-to-member call (`ACTOR_PMF_CALL`, `PMF_CALL`, `PMF_DISPATCH`) |
| fnptr | 64 | 155 calls | other `_call_via_rN` calls, plain function pointers |

- **Game (`src/`, data tables excluded):** 242 objects, about 1,980
  functions. **136 objects (1,541 functions) have C++ traits**; 106 have
  none; 1 is already C++ source (`effect_ctrl.cpp`).
- **Libraries (`lib/`):** 37 objects, none with a C++ trait (the `fnptr`
  hits are GAX's mixer calling its channels through a plain function
  pointer).
- **Vtables:** 93 tables, 831 slots, 320 distinct functions. Every class
  has its own destructor: each table has exactly one `Destroy*` slot, all
  93 different.
- **Pointer-to-member tables:** 100 entries in 13 tables, all
  non-virtual `{0, -1, fn}` constants.

The C++ objects are spread over every game directory: player 20,
objects 18, level 17, vehicle 14, bosses 13, crates 11, menus 8,
frontend 7, actor 6, text 5, and a few in the others. The 106 C-like
objects are mostly non-virtual member functions and helpers (level 16,
bosses 10, menus 9, util 9, crates 8, ...). Run
`tools/cpp_survey.py --objects` for the per-object table, `--plain` for
the C-like list.

## The compiler: agbcp and old_agbcp

**Source.** decomp.me's "agbccpp" is the `cp` release of
[notyourav/agbcc](https://github.com/notyourav/agbcc) (decomp.me's
`platforms/gba/agbccpp` Dockerfile downloads
`releases/download/cp/agbcc.tar.gz`). Its `agbcp` binary is `cc1plus`,
built from the branch's `g++/` tree: gcc 2.9-arm-000512, the same
snapshot as agbcc, with the C++ front end. SAT-R/pret agbcc (the one in
`tools/agbcc`) has no C++ front end: its `gcc/` builds only `cc1`.

The release publishes no checksum (the asset has no digest and there is
no checksum file). For the record, the tarball downloaded on 2026-10-07
was sha256 `05a30f03ee916563bba29efdc9da64c47909892aa878002cf07e8e7ae5bd32f5`.
The build doesn't use it: `tools/build_agbccpp.sh` builds both compilers
from the branch's source, which CI checks out at a pinned commit
(`1caa6becde5e4676b59c31c74d68f45ced79557c`). The source-built `agbcp`
and the release's generate identical assembly for effect_ctrl.cpp.

**What the branch lacks.** Its `g++/` tree is agbcc's compiler *without*
agbcc's two switches, so `tools/agbcc_patches/agbcp_agbcc_options.patch`
adds them:

- **`-fprologue-bugfix`**: without it, `far_jump_used_p` trusts a cached
  answer, the "lr saved for no reason" bug agbcc's option fixes. The
  Makefile passes it to agbcp as it does to agbcc.
- **The `OLD_COMPILER` switches**, so that the tree built with
  `-DOLD_COMPILER` is old_agbcc's C++ twin, `old_agbcp`. The one that
  matters for code generation is in `thumb.c`: old_agbcc's
  `s_register_operand` is plain `register_operand`, and that is what gives
  old_agbcc its "constant before `ldrb`" scheduling
  ([matching_techniques.md](matching_techniques.md#old_agbcc-vs-agbcc)).
  The others (`function.c`'s `use_return_register`, `loop.c`,
  `unroll.c`, the far-jump cache) are ported for completeness.
- **A `-fhex-asm` bug** in the branch: the epilogue of a function with
  stdarg arguments printed `add sp, sp, 0x<decimal>`, which doesn't
  assemble. SAT-R prints `#0x<hex>`; the patch does the same.

With the patch, agbcp generates agbcc's code and old_agbcp generates
old_agbcc's (experiments [1](#1-the-effect-controller-in-this-pr)-[4](#4-every-game-file-compiled-unchanged-as-c)).

**Building it** (INSTALL.md, CI's "Build agbcp and old_agbcp" step):

```
git clone -b cp https://github.com/notyourav/agbcc path/to/agbcc-cp
tools/build_agbccpp.sh path/to/agbcc-cp
```

The script copies the checkout, applies the patch and builds `cc1plus`
twice, installing `tools/agbcc/bin/agbcp` and `tools/agbcc/bin/old_agbcp`.
On a current host gcc the 1999 sources need `-std=gnu99`, warnings off,
`-fpermissive` (gcc 14+) and **`-fstack-reuse=none`**: `grokdeclarator`
(cp/decl.c) keeps a pointer to a block-scoped variable past its block
when it parses a destructor declaration, and with stack slot reuse every
class with a destructor fails with a bare "Internal compiler error". It
takes about a minute per compiler.

## g++ 2.9's C++ ABI, as the ROM has it

Everything below was checked by compiling C++ with agbcp and comparing
with the ROM's code.

**Class layout.** Fields in declaration order, base class first. The
vtable pointer is added after the fields of the first class in the
hierarchy that has virtual methods; derived classes' fields follow it.
That is why the pointer sits at different offsets in different families:

| Base class | Fields before the vtable pointer | Vtable pointer |
|---|---|---|
| controllers (`struct ctrl`) | `owner`, `animSet`, `state` | +0x0C |
| entities (`struct actor`, `gEntityVtable`) | x, y, id, kind, flags, half sizes, raw sizes | +0x18 (size 0x1C) |
| 3D actors (`struct actor_self`) | animation, position, state, links | +0x50 |

**Vtables.** Slot 0 is the RTTI slot (`__tf<class>`, 0 with
`-fno-rtti`), then one slot per virtual method in declaration order,
with an overriding method reusing its base slot and new ones appended.
Each slot is `{s16 delta, s16 index (0), void *pfn}`: `struct
vtable_slot` (include/vtable.h).

**Virtual calls.** `p->f(a)` loads the vtable pointer, adds the slot's
`delta` to `p` and calls `pfn` through `_call_via_rN` (Thumb
interworking), with `this` in r0. The delta is always added, even though
it is always 0 here. The object pointer and the function are evaluated
*before* the arguments; to get the ROM's order where it computes an
argument first, put the argument in a local before the call
([experiment 2](#2-the-controller-base-class-scratch)).

**Constructors.** `X::X()` calls the base class's constructor, stores
X's vtable pointer, runs the body and returns `this` (r0). This is the
`CreateX(self)`/`InitX(self)` shape: `InitCtrl(self); self->vtable =
gEffectCtrlVtable; ...; return self;`.

**`new`.** `new X` is `__builtin_new(sizeof(X))` and then `X::X` on the
result, with no null check: `InitEffectCtrl(OperatorNew(0x10))`.
`new X[n]` calls `__builtin_vec_new(n * size + 4)`, stores `n` in the
first word and constructs each element after it: the
`NEW_ARRAY_COUNT`/`NEW_ARRAY_BLOCK` cookie (include/memory.h).

**Destructors.** `X::~X(int __in_chrg)` runs the body, stores X's vtable
pointer again, and calls the base destructor with the same flags; the
root class's destructor calls `__builtin_delete(this)` when bit 0 is set.
This is the `DestroyX(self, flags)` shape: `self->vtable =
gEffectCtrlVtable; DestroyCtrl(self, flags);` and `DestroyCtrl`'s `if
(flags & 1) OperatorDelete(self)`. `delete p` tests `p` for null and
calls the destructor slot with 3: `actor_vtable.destroy` "called with 3
to delete".

**Pointers to members.** A `void (X::*)()` is `{s16 delta, s16 index,
union {s16 vtable offset; void *pfn}}`: `{0, -1, fn}` for a non-virtual
method, `{0, slot + 1, vptr offset}` for a virtual one. That is `struct
actor_pmf` and `ACTOR_PMF` (include/actor_self.h). `(this->*t[state])()`
compiles to exactly the code `ACTOR_PMF_CALL` spells out
([experiment 3](#3-a-pointer-to-member-dispatch-scratch)).

**Names.** The ROM has no symbols, so mangling doesn't matter, but the
build links C++ objects with C ones. g++ 2.9 mangles a method as
`Method__<len>Class<args>` (`Update__10EffectCtrlP9SpriteObj`), a
constructor as `__<len>Class`, a destructor as `_._<len>Class` and a
vtable as `_vt.<len>Class`. See [How a C++ object is built](#how-a-c-object-is-built).

## Experiments

### 1. The effect controller (#685)

`src/objects/effect_ctrl.c` was the C for the effect controller
(`gEffectCtrlVtable`): `UpdateEffectCtrl`, `EffectCtrlHandleEvent`,
`ResetEffectCtrl`, `DestroyEffectCtrl`, `InitEffectCtrl`, ROM
`0x0800CBF4`-`0x0800CD00`, an old_agbcc object. It is now
`src/objects/effect_ctrl.cpp`, with the classes in `include/ctrl.hpp`:

```cpp
class EffectCtrl : public Ctrl
{
public:
    EffectCtrl(); // InitEffectCtrl
    virtual void Update(SpriteObj *part);
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);
    virtual ~EffectCtrl(); // DestroyEffectCtrl
    void Reset();          // ResetEffectCtrl
};

void EffectCtrl::Update(SpriteObj *part)
{
    if (!part->IsOnScreen())
        part->MarkGone();
    ...
}

EffectCtrl::~EffectCtrl()
{
}

EffectCtrl::EffectCtrl()
{
    Reset();
}
```

**Result: byte-identical** under old_agbcp (`make compare` OK, the
report still 2059/2059). The destructor and constructor are empty
bodies; g++ generates the vtable pointer stores, the `InitCtrl` and
`DestroyCtrl` calls and the `return this`. The virtual call
`part->IsOnScreen()` replaces `_call_via_r1((u8 *)other +
other->table[5].delta, other->table[5].fn)`. `Update`'s body is the C's,
including its one matching trick (the gone bit set through a bitfield
view, the flags tested through the byte view), which is about the
compiler, not the language.

The comparison was also run with agbcp (the agbcc twin): everything but
`Update` matches, and `Update` differs where agbcc and old_agbcc differ
(the constant-before-`ldrb` ORs and the `id` reloads). The old C compiled
with agbcc gives exactly the same code as the C++ with agbcp, so the C++
front end adds no difference of its own here.

**Readability.** 102 lines against the C's 117, and those lines are the
class declarations rather than casts and slot arithmetic. (The file's
own `SpriteObj`/`Entity` view has since moved to
`include/sprite_obj.hpp`.)

### 2. The controller base class (now `src/objects/ctrl.cpp`)

`src/objects/ctrl.c` (`gCtrlVtable`'s own methods) rewritten as
`Ctrl`'s methods, first in a scratch area, then in part 1 of the
conversion:

```cpp
void Ctrl::StartTargetMotionYFromSet(struct gobj *part, s32 index)
{
    const speed_ramp *rec = &gCtrlMotionRecords[animSet->entries[index][1]];

    StartTargetMotionY(part, rec);
}
```

**Result: all ten functions byte-identical, under old_agbcp, with no
pins and no inline asm.** The C needs 7 `MATCH_*` uses and 2 `asm`
statements in this file: each `...FromSet` function pins two registers
and forces an `add` with asm to reproduce the virtual call's evaluation
order, and `SetCtrlTargetAnim` pins three registers to get `movs r0, #9;
negs r0, r0` before the `ldrb`. In C++ the first is the natural virtual
call (with the argument computed into a local first), and the second is
a bitfield clear of `flags` bit 3 under old_agbcp. That last point is a
finding of its own: `ctrl.o` is built with agbcc today, and its pins
reproduce old_agbcc's constant-before-`ldrb` order, so it is most likely
an old_agbcc object (#662). Part 1 confirmed it: `ctrl.cpp` is in
`OLD_AGBCC_OBJS`, unpinned.

### 3. A pointer-to-member dispatch (scratch)

`UpdateHovercraftCannon` (src/bosses/hovercraft_cannon.c) with a minimal
`ActorSelf` class (vtable pointer at +0x50):

```cpp
typedef void (HovercraftCannon::*StateFunc)();
extern const StateFunc gHovercraftCannonStateFuncs[];

void HovercraftCannon::Update()
{
    (this->*gHovercraftCannonStateFuncs[this->state])();
    ...
}
```

**Result: byte-identical** to the ROM, under agbcp. The 30-line
`ACTOR_PMF_CALL` macro, which took a long time to find, is one line of
C++. A table defined in C++ (`const StateFunc t[] = { &X::f, ... }`)
comes out as `ACTOR_PMF`'s `{0, -1, fn}` records.

### 4. Every game file compiled unchanged as C++

Each `src/*/*.c` (not the data tables, the ARM objects or the two
per-file-flag objects), preprocessed as C++ and compiled with agbcp or
old_agbcp (per `OLD_AGBCC_OBJS`) and `-fpermissive`, its `.text`
compared with the C build's:

| Result | Files |
|---|---:|
| identical `.text` | 185 |
| different `.text` | 45 |
| doesn't compile | 7 |

The 7 failures are trivial to fix: `asm("..." :: "r"(x))` (6 files),
where `::` is one token in C++ (write `: :`), and one `asm` whose
operand constraints g++ rejects. The 45 differences are code generation:
the C++ front end lays out some temporaries and stack slots differently
(e.g. `player_contact.c`'s frame is 16 bytes, not 32) or ranks registers
differently (`palette_cycle.c` uses r9). They cluster in the files with
the most matching workarounds, i.e. the ones tuned hardest against the C
front end. So **80% of the game's C is already valid C++ that compiles to
the same bytes**; a conversion only has to re-match the functions it
actually rewrites, plus those 45 files' quirks if they are converted.

## How a C++ object is built

The Makefile's `CXX_OBJS` are the `src/*/*.cpp` files:

```
$(CPP) -x c++ $(CPPFLAGS) $< | $(CXX1) -quiet $(CC1FLAGS) -o foo.s   # agbcp or old_agbcp
printf '$(ZERO_PAD_TEXT)' >> foo.s
$(AS) $(ASFLAGS) -o foo.o foo.s
$(OBJCOPY) --redefine-syms=cxx_symbols.txt foo.o
```

- **Compiler.** `agbcp`, or `old_agbcp` for an object in
  `OLD_AGBCC_OBJS` (the list holds both C and C++ objects). The flags are
  the C ones (warnings, `-O2`, `-fhex-asm`, `-fprologue-bugfix` unless
  old) plus `-fno-rtti -fno-exceptions`.
- **Headers.** All of `include/` parses as C++ when wrapped in `extern
  "C" { }`, with the same warning flags and `-Werror`. C++-only
  declarations go in `.hpp` headers (`include/ctrl.hpp`,
  `include/sprite_obj.hpp`), next to the C structs they mirror (see
  [Progress](#progress)).
- **Vtables stay C data.** The class header has `#pragma interface`, so
  g++ doesn't emit the classes' vtables; the code refers to
  `_vt.<len>Class`, and the ROM's tables stay the C arrays in
  `src/data/entity_vtables_7e3bec.c`.
- **Names.** `cxx_symbols.txt` maps each mangled name the C++ objects
  define or use to its C name (`Update__10EffectCtrlP9SpriteObj
  UpdateEffectCtrl`, `_vt.10EffectCtrl gEffectCtrlVtable`, `__4Ctrl
  InitCtrl`). objcopy renames them after assembling, so the vtable data,
  the C callers, the linker script and the objdiff report see the C names.
  A missing or stale entry fails the link (the C side's reference stays
  undefined).
- **Style.** clang-format formats `.cpp`/`.hpp` too (`tools/format.py`);
  `.clang-format` puts `public:` at the class's indent.

asm labels (`void Update(SpriteObj *) asm("UpdateEffectCtrl");`) work for
methods and constructors, but not for destructors: g++ 2.9 then fails to
find the destructor (`no matching function for call to B::__dt`). That,
and the vtable names, is why the build renames with objcopy instead.

## Progress

The conversion goes one class family per PR (#664). Each row is an
object that is now built from C++ source; "workarounds" counts the
`MATCH_*` pins and `asm` statements of its old C (`tools/match_idioms.py`
counts them by kind) and what the C++ still needs.

| Object | Classes (include/ctrl.hpp unless noted) | Functions | Compiler | Workarounds: C -> C++ | Part |
|---|---|---:|---|---|---|
| `src/objects/effect_ctrl.cpp` | `EffectCtrl` | 5 | old_agbcp | 0 -> 0 | #685 |
| `src/objects/ctrl.cpp` | `Ctrl` | 10 | old_agbcp (was agbcc) | 7 pins, 2 asm -> 0 | 1 |
| `src/bosses/tiny_hop_pad.cpp` | `StompedHopPadCtrl`, `OneShotAnimCtrl::Update` | 4 | old_agbcp (was agbcc) | 5 pins and an `ENTITY_SET_GONE_BIT_ASR` (6 pins, 1 asm), gotos -> 2 pins | 1 |
| `src/player/input_ctrl_queue.cpp` | `InputCtrl`'s queue setters (input_ctrl.hpp since part 3), `BossCtrl` | 9 | agbcp | 0 -> 0 | 1 |
| `src/bosses/mega_mix.cpp` | `MegaMixCtrl` | 7 | agbcp | 23 pins, 8 asm -> 0 | 1 |
| `src/player/player_flags.cpp` | `Ctrl`'s `SetMode`, `SetAnimSet`, `SetTargetMotionY`, `StartTargetMotionY` (ctrl.hpp), with 40 C-linkage player accessors | 4 + 40 | agbcp | 16 pins -> 0 (plus `StorePlayerListEntry`'s 6 -> 0; `GetPlayerListEntry` keeps 3) | 3 |
| `src/player/input_ctrl.cpp` | `InputCtrl` (input_ctrl.hpp), with 6 C-linkage swim controller accessors | 19 + 6 | old_agbcp | 0 -> 0; the vcall macros and the 16-line PMF dispatch go | 3 |
| `src/enemies/enemy_ctrl.cpp` | `EnemyCtrl`'s small methods, `KnockedEnemyCtrl`, `PeriodicSpawner` (include/enemy_ctrl.hpp) | 27 | old_agbcp (was agbcc) | 14 pins, 2 clobbers, 1 const, a `volatile` read and an `ENTITY_SET_GONE_BIT_ASR` -> 4 pins, 2 clobbers, 1 const | 2 |
| `src/enemies/enemy_ctrl_update.cpp` | `EnemyCtrl::Update`, `EnemyCtrl::HandleEvent` | 2 | old_agbcp | 1 pin, 1 hold, 1 use, 1 const -> 1 const | 2 |
| `src/enemies/enemy_attack.cpp` | `EnemyCtrl`'s attack cycle, trigger box, `SetState`, range setters | 6 | old_agbcp | 1 pin -> 1 pin | 2 |
| `src/enemies/enemy_motion.cpp` | `EnemyCtrl`'s homing, hop, flip cycle | 4 | agbcp | 1 pin -> 1 pin | 2 |
| `src/enemies/enemy_patrol.cpp` | `EnemyCtrl::UpdatePatrol` | 1 | old_agbcp | 0 -> 0 | 2 |
| `src/enemies/enemy_shooter.cpp` | `EnemyCtrl::UpdateShooter` | 1 | agbcp | 1 pin -> 0 | 2 |
| `src/player/swim_ctrl.cpp` | `PlayerCtrl` (player_ctrl.hpp) | 26 | old_agbcp | 4 pins -> 0; the vcall macros and the 16-line PMF dispatch go | 4 |
| `src/player/swim_ctrl_drift.cpp` | `PlayerCtrl::SetDriftY` | 1 | agbcp | 13 pins -> 0 | 4 |
| `src/player/swim_ctrl_stroke.cpp` | `PlayerCtrl`'s `StartStroke`, `StartSpin`, `ApplySwimDrift` | 3 | old_agbcp | 0 -> 0 | 4 |
| `src/player/input_ctrl.cpp` (again) | the 6 swim controller accessors are `PlayerCtrl` methods now | 6 | old_agbcp | 0 -> 0 | 4 |
| `src/player/action_ctrl.cpp` | `ActionCtrl` (include/action_ctrl.hpp): constructor, destructor, `SetModeAnim`, `SetTargetAnim`, `Restart`, the motion queue accessors, 4 short state methods; `PlayerCtrl::Reset`/`Restart` | 23 + 2 | agbcp | 6 pins -> 0 (and the `SetTargetAnim` return cast and gotos) | 5a |
| `src/player/action_ctrl_moves.cpp` | `ActionCtrl::SetMode`, `Attach`, the spin/run/jump starts, `StartTornadoFall`, `EndSpin`, `SteerSpin`, 7 state methods | 17 | old_agbcp | 18 pins, 2 keeps, 1 const, 1 `"+r"` asm, 1 asm jump table, 3 volatile casts, 4 retyped stores -> 2 pins, 1 const, 1 `"+r"` asm | 5a |
| `src/player/action_ctrl_update.cpp` | `ActionCtrl::Update`, `TryDoubleJump`, `HandleAirInput` | 3 | old_agbcp | 1 const -> 0; the 20-line PMF dispatch goes | 5a |
| `src/player/action_ctrl_idle.cpp` | `ActionCtrl::ApplyMotion`, `StateIdle` | 2 | old_agbcp | 3 uses -> 0 | 5a |
| `src/player/action_ctrl_land.cpp` | `ActionCtrl::StateCrawlStandUp`, `StateBodySlamLand`, `StateLand` | 3 | agbcp | 0 -> 0 | 5a |
| `src/player/action_ctrl_left_ground.cpp` | `ActionCtrl::CheckLeftGround` | 1 | **old_agbcp** (was agbcc) | 3 pins -> 0 | 5a |
| `src/player/kill_player.cpp` | `ActionCtrl::KillPlayer`, `UpdateSkidAnim`, `UpdateFacing` | 3 | **old_agbcp** (was agbcc) | 44 pins, 1 asm -> 1 pin | 5a |
| `src/bosses/mega_mix_update.cpp` | `MegaMixCtrl::Update` (include/boss_ctrl.hpp) | 1 | old_agbcp | 3 pins, 2 keeps, gotos -> 1 pin, 1 keep, one goto | 6 |
| `src/bosses/tiny_update.cpp` | `TinyCtrl` (include/boss_ctrl.hpp): `Update`, `SetState`, `PickHopTarget`, `SpawnFallingLeaves` | 4 | old_agbcp | 8 pins, 3 keeps, 1 asm -> 2 pins | 6 |

Part 1 in numbers: 30 functions in 4 objects; `MATCH_HOLD_REG` 2151 ->
2118 and instruction-emitting `asm` 249 -> 239 project-wide, plus one
`ENTITY_SET_GONE_BIT_ASR` site (its 6 pins and `asm` are in the macro).
The two pins left are `StompedHopPadCtrl::Update`'s: unpinned, g++ gives
`this` r2 and `part` r4 where the ROM has r3 and r2, with every natural
rewrite tried (switch orders, locals, `this` copies). The C needed the
same two pins, plus gotos for the block order, which a `switch` gives.

Compilers: `ctrl.o` and `tiny_hop_pad.o` match as clean C++ only under
old_agbcp (`SetTargetAnim`'s and `OneShotAnimCtrl::Update`'s
constant-before-`ldrb`), so both moved to `OLD_AGBCC_OBJS`; their C was
pinned to old_agbcc's code under agbcc. `mega_mix.o` and
`input_ctrl_queue.o` compile the same under both and stay agbcc.

Part 3 in numbers: 23 methods in 2 objects, plus the 46 plain functions
that share their files; `MATCH_HOLD_REG` 2118 -> 2096, instruction-emitting
`asm` unchanged (239). `player_flags.cpp` is mostly the player's accessors:
a file is either C or C++, so they are compiled as C++ too, with C linkage
(their prototypes in player.h are C declarations, so the definitions keep
the C names). Every one compiles to the same code; `StorePlayerListEntry`'s
6 pins also go, and `GetPlayerListEntry` keeps its 3 (unpinned, agbcp
swaps r0 and r1 in the address computation). `player_flags.o` compiles
the same under agbcp and old_agbcp and stays on agbcc; `input_ctrl.o`
(old_agbcc C already) matches only under old_agbcp (agbcp's code differs
from `KillPlayer`'s bitfield clears on, old_agbcp's constant before the
`ldrb`) and stays in `OLD_AGBCC_OBJS`.

`InputCtrl` moved from ctrl.hpp to its own header, `include/input_ctrl.hpp`.
Its state table is a static member, `static const StateFunc
stateFuncs[4]`, which cxx_symbols.txt maps to the C table
`gInputCtrlStateFuncs` (`_9InputCtrl.stateFuncs`). Two C idioms stay: the
three in-class inline helpers (`QueueNowX`/`QueueNowY`/`SetLeadSpeed`,
which compute the value before the stores like the C's static inlines),
and `HandleEvent`'s `s32 lo = 1` lower bound (a literal 1 folds into one
unsigned range check).

Part 2 in numbers: the whole enemy family (src/enemies/, ROM
0x0800B8DC-0x0800CBF4), 41 functions in 6 objects; `MATCH_HOLD_REG`
2096 -> 2078 and instruction-emitting `asm` 239 -> 238 project-wide (the
last `ENTITY_SET_GONE_BIT_ASR` site went, and the macro with it), plus
one `MATCH_HOLD`, one `MATCH_USE` and a `volatile` read. The virtual
calls (`SetTargetAnim`, the knocked controller's `Attach`, `delete this`,
the sea mine's `HandleEvent(0, EVENT_HIT, 0)`) and `new
KnockedEnemyCtrl` are plain C++. Kept, each with a comment, because
they're register allocation rather than C emulation (the same under
agbcp and old_agbcp): the three oscillators' 4 pins, 2 clobbers and
constant-init (`UpdateOscillateX`, `UpdateBob`, `UpdateOscillateY`),
`UpdateHop`'s and `UpdateTriggerBox`'s pin, `HandleEvent`'s
`MarkGoneFreshBit` constant-init and `Update`'s `vu8` re-read.
`GetSfxVolumeAt`'s 6 pins, `UpdateShooter`'s and `PeriodicSpawner::Update`'s
pins, and the knocked controller's whole `ENTITY_SET_GONE_BIT_ASR`
were old_agbcc's code reproduced under agbcc, or C emulation; the C++
needs none of them.

Compilers: `enemy_ctrl.o` matches as clean C++ only under old_agbcp
(`KnockedEnemyCtrl::Update` is `OneShotAnimCtrl::Update`'s
constant-before-`ldrb`, and `GetSfxVolumeAt` comes out unpinned), so it
moved to `OLD_AGBCC_OBJS`. `enemy_ctrl_update.o`, `enemy_attack.o` and
`enemy_patrol.o` were already old_agbcc objects. `enemy_motion.o` and
`enemy_shooter.o` compile the same under both and stay agbcc.

Part 4 in numbers: the swim controller, `PlayerCtrl` (new
`include/player_ctrl.hpp`), 36 methods in 4 objects (`swim_ctrl.cpp`,
`swim_ctrl_drift.cpp`, `swim_ctrl_stroke.cpp`, and the six motion-queue
accessors at the top of `input_ctrl.cpp`, which are methods now);
`MATCH_HOLD_REG` 2096 -> 2079, instruction-emitting `asm` unchanged (239).
`ApplyMotion`'s 4 pins go (the record computed into a local before the
direct `Ctrl::StartTargetMotionX/Y` call gives the ROM's order), and so do
all 13 of `SetDriftY`'s: as C++ it is the same plain code as its X twin,
`SetDriftX`, and matches under both agbcp and old_agbcp, so
`swim_ctrl_drift.o` stays on agbcc. `swim_ctrl.o` and `swim_ctrl_stroke.o`
match only under old_agbcp, as their C did under old_agbcc.
`SET_MODE`/`CTRL_SET_ANIM` (vtable-slot macros) and `PMF_DISPATCH` are
`SetMode(...)`, `SetTargetAnim(...)` and `(this->*stateFuncs[state])()`;
the constructor and destructor are empty bodies. `SetDriftX`/`SetDriftY`/
`GetDriftStep` use only `gPlayer`, so they are static members.
`PlayerCtrl`'s `Reset` and `Restart` are still C (`ResetPlayerCtrl`,
`RestartPlayerCtrl`, in `action_ctrl.c` with the action controller, which
the ROM puts them next to); the constructor calls `Reset` through its
cxx_symbols.txt mapping. `struct player_ctrl` (player_ctrl.h) stays the C
view for them and `play_room.c`; its vtable and anim-pair views
(`pctrl_vtable`, `pctrl_method`, `pctrl_anim_pair`) are gone.

One C++ difference turned up: a test of a 1-bit unsigned bitfield
(`if (t->mirror.bits.flipX)`) compiles to `movs #16; ands` as C++, where
the C front end (and the ROM) has `lsls #27` and a sign test. Testing the
signed view, `t->mirror.sbits.flipX < 0`, gives the ROM's code. The one
negated test that extracts the bit (`lsls #27; lsrs #31`) reads it through
an inline `u8 FlipX()` helper. Two C idioms stay: the
`*(volatile u8 *)&t->tag = t->tag` re-store in `StartStroke` (a plain
self-assignment is deleted, in C++ as in C), and the BLKmode `struct keys`
stack copy of `gKeys`.

Part 5a in numbers: the first 7 of the action controller's 11 files, 52
`ActionCtrl` methods plus `PlayerCtrl::Reset`/`Restart`;
`MATCH_HOLD_REG` 2061 -> 1993, instruction-emitting `asm` 238 -> 236,
`MATCH_CONST` 30 -> 29, `MATCH_USE` 82 -> 79, `MATCH_KEEP` 61 -> 59,
retyped field stores 228 -> 224 and scoped volatiles 48 -> 45
project-wide. `ActionCtrl` (include/action_ctrl.hpp) declares all of its
methods, also those still in C files (cxx_symbols.txt maps them to their
C names), and its state table is `static const StateFunc
stateFuncs[ACTION_STATE_COUNT]` (`gActionCtrlStateTable`), dispatched by
`(this->*stateFuncs[state])()`. `kill_player.o` and
`action_ctrl_left_ground.o` move to `OLD_AGBCC_OBJS`: their clean C++
matches only under old_agbcp (`KillPlayer`'s constant-before-`ldrb`); their
47 pins reproduced old_agbcc's code under agbcc. `action_ctrl.o` and
`action_ctrl_land.o` match under both and stay on agbcc.

What replaced the pins, mostly one idea: **an inline function's
parameter is computed before its body.** Where the ROM materializes a
constant early (before a call, before the stores that use it), an inline
helper taking it as a parameter gives that order: `QueueX`/`QueueY`/
`QueueNowX`/`QueueNowY` for the motion queue's three-byte stores,
`SetModeAnimNow(mode, anim, frame[, frames])` for a mode change whose 0
stays in a callee-saved register across the two virtual calls
(`StartHighJump`, `StateHangGrab`), and `UpdateFacing`'s two turns
(`FaceLeft`/`FaceRight`, whose 0 and 1 come first). Likewise an inline
accessor (`IsSlippery`, `SetSlippery`, `KeysHeld`) keeps the player's
`slippery` offset, 0x100, from being shared with another 0x100 (an
R_BUTTON test, a `PlaySfx` volume) through a register, which is what the
C's `PartByte(part, 0x100)` and `K100()` were for.

Three workarounds stay, each with a comment: `StartTornadoFall`'s two
pins (the ROM's `this`/`entry` registers and the late 0), `EndSpin`'s
`MATCH_CONST` plus `"+r"` asm (the ROM ANDs L_BUTTON through a copy into
the input's own register), and `UpdateSkidAnim`'s one pin (the ROM tests
0x18 on a copy of the animation; a `switch` tests all four on one
register). `HandleAirInput` keeps action_obj.h's byte view of `flags2`
(`ACT_PART_FLAGS0D`) for its register order.

`Ctrl::SetTargetAnim` (slot 10) now returns `s32`, not `u8`: the override
returns the base's result, and with a `u8` g++ zero-extends it after the
call, which the ROM doesn't. `ctrl.o`'s code is the same either way. The C
had cast `SetCtrlTargetAnim` to an `s32` function for the same reason.

Part 6 in numbers: the first two boss controller objects (`UpdateMegaMix`,
and Tiny's four functions), in the new `include/boss_ctrl.hpp`, which also
takes `BossCtrl` and `MegaMixCtrl` from ctrl.hpp; `MATCH_HOLD_REG` 2061 ->
2053, `MATCH_KEEP` 61 -> 57 and instruction-emitting `asm` 238 -> 237
project-wide. `TinyCtrl`'s constructor, destructor and `StartHop` are
still C (`CreateTiny`, `DestroyTiny`, `StartTinyHop`, in cortex.c), as is
`OneShotAnimCtrl`'s constructor, which `new OneShotAnimCtrl` calls through
its cxx_symbols.txt mapping. `delete pad->mover`, `new StompedHopPadCtrl`,
`ctrl->Attach(pad)`, `SetTargetAnim`, `SetMode`, the part's `IsOnScreen`
and the crates' `GetClassId` are plain C++. The word at 0x10 is
`BossCtrl`'s `target` or, through an anonymous union, `counter` (Tiny's
round). The palette nibble of `SpriteObj` is a bitfield now (`palette:4`,
as in box_part.h): stored as one, it gives the ROM's `v & 15` before
the `-16` mask, which `SpawnFallingLeaves`'s C pinned and spelled in
`asm`. Kept, each with a comment: `SetState`'s two pins (unpinned, `pad`
is r0 where the ROM has r2, as in the C) and `UpdateMegaMix`'s one pin and
keep on the player's `dead` read in state 2 (below). C idioms kept: the
s32-mask and s32-tag helpers (`ClearFlags`, `SetTag`), named constants
(`one`, `k`, `m`) and the `ExplodeCrate` copy `c`, each placing a constant
or a register as the ROM has it.

`UpdateMegaMix`'s C shared its "run towards the player" tail between
states 0 and 2 with gotos, and pinned two of its three `dead` reads.
Written with the tail as an inline (`Run`) in both places, the
duplicated code puts the reloads of the constant 0x104 into the same
rotation of r1-r3 as the ROM's, and the cross-jumping pass after reload
merges the copies into the ROM's layout; only state 2's out-of-range
path still jumps to state 0's copy (`goto run`), and only state 2's
last `dead` read is one step off (pinned).

**How the C and C++ views share a layout.** One header for both
languages would need `#ifdef __cplusplus` around every class, and a C
struct can't have a class's base or methods, so each family has a C++
class in a `.hpp` header and keeps its C struct in the C header for the
C files: `Ctrl`/`struct ctrl` (objects.h), `InputCtrl`/`struct
input_ctrl` (input_ctrl.hpp) and `BossCtrl`/`struct boss_ctrl` (player.h)
and `MegaMixCtrl`/`struct mega_mix_ctrl` (bosses.h) (boss_ctrl.hpp), `SpriteObj`/`struct
gobj` (gobj_1a794.h), `EnemyCtrl`/`struct part_ctrl` (part_ctrl.h),
`PeriodicSpawner`/`struct periodic_spawner` (enemies.h),
`ActionCtrl`/`struct act` (action_obj.h). Each class has
a `COMPILE_TIME_ASSERT` that its size is the C struct's (the `.hpp` includes the C header), and its
fields keep the C names and offset comments. The C prototypes of the
converted methods stay in the C headers, under their C names, for the
vtable data and the C callers. When the last C user of a struct is
converted, the struct can go.

**Sprite objects.** `include/sprite_obj.hpp` has `Entity` (struct
actor's 0x1C-byte header, all of gEntityVtable's slots, `MarkGone()`)
and `SpriteObj` (struct gobj's fields, `mover` the part's `Ctrl`),
shared by the controllers instead of each file's own partial view.
`Entity`'s constructor is InitEntity (still C) and its destructor is
inline, as DestroyPeriodicSpawner shows; the family itself is converted
later. `EnemyCtrl` reaches its part through an anonymous union, as
part_ctrl.h's `struct ctrl_target` (its fields) or as a `SpriteObj`
(what the `Ctrl` methods and the part's virtual methods take).

### Next batches

Bigger controllers, roughly in order (function counts from
`tools/cpp_survey.py --objects`):

1. ~~**The enemy controllers**~~: done in part 2 (all of src/enemies/). Their
   C callers are still C: the level spawners (`spawn_enemies.c`,
   `spawn_objects.c`'s SpawnSealSpawner) and `dingodile.c`, whose
   Dingodile builds on `CreateEnemyCtrl`.
2. ~~**The rest of `Ctrl` and `InputCtrl`**~~: done in part 3. Only
   `Ctrl::Update` (`UpdateCtrl`, an empty function in `system/boot.c`)
   is still C.
3. ~~**The swim controller**~~ (`PlayerCtrl`): done in part 4; its
   `Reset`/`Restart` (in `action_ctrl.cpp`) in part 5a. **The action
   controller** (`ActionCtrl`): part 5a converted `action_ctrl.c`,
   `_moves.c`, `_update.c`, `_idle.c`, `_land.c`, `_left_ground.c` and
   `kill_player.c`. Left for part 5b: `action_ctrl_event.c`
   (`HandleEvent`), `_hang.c` (11 methods), `_run_jump.c` (2) and
   `_states.c` (11), 25 methods with about 35 pins and asm statements, and
   then action_obj.h's vcall macros (`ACT_CALL*`/`ACT_VCALL*`) and trio
   inlines can go.
4. **The boss controllers:** `tiny_update.c`, `mega_mix_update.c`
   (`UpdateMegaMix`), `dingodile*.c` and `cortex.c` (25 functions, 64 lines

3. ~~**The swim controller**~~ (`PlayerCtrl`): done in part 4, except
   `Reset`/`Restart`, which live in `action_ctrl.c`. Next, **the action
   controller** (`ActionCtrl`, `action_ctrl*.c`, 10 files, about 76
   functions and most of the controllers' virtual calls), which takes
   `PlayerCtrl`'s last two methods with it.
4. **The boss controllers:** ~~`tiny_update.c`, `mega_mix_update.c`~~
   (done in part 6), then `dingodile*.c` (Dingodile, his shield, shark
   and rocket/stalactite, and the Cortex target/cannon/boss controllers'
   methods that sit between them) and `cortex.c` (25 functions, 64 lines
   with pins or asm; it also has `OneShotAnimCtrl`'s and
   `UnusedOneShotAnimCtrl`'s constructors and destructors, Tiny's
   constructor, destructor and `StartHop`, and the Cortex
   cannon/target/shot classes). cortex.c mixes controllers and entities,
   so it may wait for the entity family.
5. **The platform mover** (`src/objects/platform.c`, `gPlatformMoverVtable`:
   a `Ctrl`-shaped 0x38-byte class) with the platforms, in the entity
   family.

## Recommendation and plan

**Convert gradually, with `CXX_OBJS`.** The evidence is unambiguous that
the game is C++, the compiler matches, and every C++ construct comes out
as the ROM has it. The C++ source is shorter, has no casts of `self`, no
hand-written vtable stores, slot arithmetic or `_call_via_rN` calls, and
in experiment 2 no matching workarounds at all. Plan:

1. **One class family per PR,** base class first, so each header defines
   a complete hierarchy: the controllers (`ctrl.c`, then `enemy_ctrl.c`,
   `action_ctrl*.c`, `input_ctrl*.c`, `swim_ctrl*.c`, the boss
   controllers), then the entities/sprite objects (`gEntityVtable` and
   its subclasses: sprites, crates, pickups, platforms), then the 3D
   actors (`actor_self`, the vehicle and boss actors, the PMF tables).
2. **Each converted object** moves from `foo.c` to `foo.cpp`, its class
   goes into a `.hpp` header, its mangled names into `cxx_symbols.txt`.
   Keep it byte-identical: the usual full clean `make compare` and the
   report. Expect most pins in the virtual-call and PMF code to go
   (experiment 2); drop each one that isn't needed.
3. **Recheck the compiler** of each object as it's converted: ctrl.o
   and tiny_hop_pad.o match as clean C++ only under old_agbcp, and were
   held on agbcc by pins; expect more.
4. **While the code is mixed,** C callers keep using the C structs and the
   C names. A class and its C struct must keep the same layout: add
   `sizeof` checks on both sides (as `ctrl.hpp` does), and convert the C
   callers of a family together with it where practical (e.g.
   `SpawnEffectPart`'s `InitEffectCtrl(OperatorNew(0x10))` and its
   `attach` slot call become `new EffectCtrl` and `mgr->Attach(part)`
   once entity_spawner.c is C++).
5. **Vtables** stay C data until a whole family is C++. Then they could
   be emitted by g++ (drop `#pragma interface`), which would also check
   the hierarchy: the slot order and overrides would have to come out the
   same. That needs the `.gnu.linkonce.d` sections placed in the ROM's
   vtable order by the linker script; not tried yet.
6. **Leave C as C:** `lib/` (GAX2, AgbEeprom, libgcc, BIOS wrappers), the
   IWRAM ARM code (agbcc_arm; the `cp` branch also has an ARM `agbcp_arm`,
   untried), and the data tables.

**#656 (base structs).** For a family that becomes C++, the base class
*is* the embedding: `class EffectCtrl : public Ctrl` has the base at
offset 0 by construction, with flat field access, which is #656's goal
with none of its options' downsides. So #656 should not convert families
that are about to become C++; for those, the C structs only need to stay
in step with the classes until their C users are gone. #656's
reconciliation work (one set of field names and types per base, unions
where files need different views) is still needed either way: it is
exactly what the class definitions need.

**#662 (MATCH_ macros).** Many workarounds exist because the C emulates
code the C++ compiler generated: the virtual-call and PMF macros, the
evaluation-order pins around `_call_via_rN` calls, the `if (1) {} else
(void)0` wrapping that keeps `ACTOR_PMF_CALL` from looking like a loop.
Converting an object to C++ removes those outright: part 1 removed 33
pins and 10 `asm` statements and kept 2, part 2 removed 18 pins and an
`asm` and kept 6, plus 2 clobbers and 2 constant-inits
([Progress](#progress)). #662's pruning tool should treat "convert
to C++" and "try old_agbcc/old_agbcp" as two more rewrites to test.

## Dead ends and gotchas

- **SAT-R/pret agbcc has no C++ compiler.** Only notyourav's `cp` branch
  does, and its `g++/` tree lacks agbcc's options: built as is, it can't
  produce old_agbcc's code, and the first `old_agbcp` attempt (porting
  only the far-jump and `function.c` switches) still loaded the byte
  before the constant. The difference turned out to be
  `s_register_operand`.
- **Building `cc1plus` on a current gcc** crashes on every destructor
  until it is built with `-fstack-reuse=none` (above). The release
  binary doesn't have the problem: it was built in 2022 with an older gcc.
- **`::` in inline asm** (`asm("" :: "r"(x))`) is a syntax error in C++.
- **asm labels on destructors** break g++ 2.9's destructor lookup, hence
  the objcopy rename.
- **Without `#pragma interface`,** g++ emits each class's vtable in the
  file that defines its first virtual method, as a weak
  `.gnu.linkonce.d` section the linker script doesn't place.
- **Argument order.** A virtual call evaluates `this` and the slot before
  the arguments. Where the ROM computes an argument first, put it in a
  local (experiment 2), not in a pin.
- **Pins can still be needed.** Register allocation isn't always
  about the C emulation: `StompedHopPadCtrl::Update` keeps the C's two
  `MATCH_HOLD_REG`s (`register T x asm("rN") = this;` works in C++).
- **A direct call to a base method** that is virtual is
  `Ctrl::StartTargetMotionY(part, rec)`: a plain `bl` to its C name, as
  `MegaMixCtrl::StartTargetMotionYFromSet` does. cxx_symbols.txt also
  needs the mangled name of every C method a C++ object calls.
- **`case 1: { ... }`** is formatted with the braces on their own
  indented line; declare the case's locals at the top of the function
  instead (`StompedHopPadCtrl::Update`).
- **The 45 files of experiment 4** differ when their C is compiled as
  C++: converting one of them means re-matching it, not just renaming it.
- **A 1-bit bitfield test** (`if (t->mirror.x)`) is `movs #mask; ands`
  under g++, where the C front end gave the ROM's `lsl` sign test. Spell
  the sign test: `(s32)(part->mirror << 27) < 0` (`UpdateTriggerBox`,
  `UpdatePatrol`). Loads and stores of the bitfield come out the same.
- **An anonymous union member is reloaded after every store.** Reading
  EnemyCtrl's `target`/`sprite` union again after a store through the
  part reloads it, where the C kept it in a register; take it into a
  local once (`HandleEvent`'s `part` and `t2`).
- **Free functions keep C linkage** when their prototype is in a C
  header included inside `extern "C"` (`GetSfxVolumeAt`,
  `LaunchHarmfulEffectPart`): no cxx_symbols.txt entry.
- **A struct passed by value** is copied with `memcpy` from a named
  local in C++: `CollidePartList(list, box, ...)` after `box =
  GetSpriteHitbox(part)` copies twice and grows the frame. Pass the call
  itself, `CollidePartList(list, GetSpriteHitbox(part), ...)`
  (`UpdateMegaMix`).
- **A byte RMW with a folded mask:** `flags &= ~4` on a `u8` member
  compiles to the byte constant `0xFB` where the C front end kept `-5`
  (`movs #5; negs`). Pass the mask as an `s32` (`ClearFlags(&f, ~4)`).
- **A file-local helper with the name of a C function** declared in an
  `extern "C"` header gets C linkage and is emitted out of line as a
  global, even when `static inline` (`IsPlayerDead` in player.h). Pick
  another name.
- **Reload register rotation.** The spill register a constant offset is
  reloaded into (`movs rN, #0x82; lsls` for the player's +0x104) rotates
  through r1-r3, so it depends on every reload before it, including those
  of code cross-jumping deletes later. Code the C shared with gotos may
  need to be written out twice for the rotation to come out as the ROM's
  (`UpdateMegaMix`'s `Run`).
- **`delete this`** is the ROM's `if (self) self->vtable[9](self, 3)`
  (`HandleEvent`), and an inline root destructor folds into the derived
  one, its dead vtable pointer store dropped (`~PeriodicSpawner`).
- **A constant the ROM loads early** (before the calls or stores that use
  it) is usually an inline function's parameter: the inliner computes the
  arguments before the body. Try an inline helper before a pin
  (`ActionCtrl::QueueNowX`, `SetModeAnimNow`; part 5a).
- **A `u8`-returning virtual whose override returns the base's result**
  gets a zero-extension after the call. The ROM's `ActionCtrl::SetTargetAnim`
  has none, so slot 10 returns `s32`.
- **Two tests of the same bit** are threaded into one when spelled the
  same; the ROM's re-test needs two spellings (`sbits.flipX < 0`, then
  `(s32)(mirror.all << 27) >= 0`: `UpdateFacing`).

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
  `src/data/entity_vtables_7e3bec.c`. One header has no `#pragma
  interface`, include/entity.hpp: graphics.cpp needs the out-of-line
  copies of `Entity`'s inline methods (part 7a). It also gets Entity's
  vtable, a weak symbol in a `.gnu.linkonce.d` section, which the linker
  script discards (`/DISCARD/`); the C table wins.
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
| `src/bosses/dingodile.cpp` | `DingodileCtrl`, `DingodileShieldCtrl`, `DingodileProjectileCtrl`, `DingodileSharkCtrl`, and `CortexTargetCtrl`'s, `CortexCannonCtrl`'s and `CortexBossCtrl`'s methods (include/boss_ctrl.hpp) | 25 | old_agbcp | 7 pins, 2 holds, 2 uses, 1 const, the `PREP_VOBJ_CALL2` shared call and gotos, 2 asm labels -> 3 pins, 2 holds, 2 uses, 1 const, 1 asm label | 6b |
| `src/bosses/dingodile_create.cpp` | `DingodileShieldCtrl`'s constructor, `DingodileCtrl`'s `StartMotion`, constructor, destructor and setters | 6 | agbcp | 2 pins, 1 use -> 0 | 6b |
| `src/player/action_ctrl_event.cpp` | `ActionCtrl::HandleEvent` | 1 | old_agbcp | 3 consts, a `volatile` read -> the same | 5b |
| `src/player/action_ctrl_hang.cpp` | `ActionCtrl::StateLeftGround`, `StateDying`, `StateWarpIn`, the 6 hang states, `ReleaseHang`, `DoSuperBodySlamShockwave`, `StartTornadoSpin` | 11 | old_agbcp | 15 pins, 1 const, 3 uses, 3 holds, 1 keep, 2 asm and a file-scope pool word, 2 volatile casts, a retyped store, an asm label -> 1 pin, 1 hold, 1 use | 5b |
| `src/player/action_ctrl_run_jump.cpp` | `ActionCtrl::StateRun`, `StateJump` | 2 | old_agbcp | 1 keep -> 1 keep | 5b |
| `src/player/action_ctrl_states.cpp` | `ActionCtrl::StateAirborne`, `StateFlipBodySlamStart`, `StateSlide`, the spin, crouch and crawl states | 11 | old_agbcp | 6 pins, 1 keep, 1 volatile pointer -> 1 volatile pointer | 5b |
| `src/gfx/graphics.cpp` | `Entity` (include/entity.hpp), and the sprite graphics managers `OamBuffer`, `ObjVramCursor`, `PaletteCache`, `SpriteBankSet` (sprite_obj.hpp), with 7 C-linkage functions (the VRAM DMA queue, `WorldToScreen`, ...) | 76 + 7 | **old_agbcp** (was agbcc) | 64 pins, 11 asm, 5 retyped stores, 1 volatile read -> 0 | 7a |
| `src/objects/sprite.cpp` | `SpriteRenderer`, `Sprite`'s `Reset`, boxes, `CheckPlayerContact` (the pickups'), `IsOnScreen`, `OverlapsRect`, `AdvanceAnim` | 13 | old_agbcp | 22 pins, 2 asm, 2 retyped stores -> 1 pin, 1 retyped store | 7a |
| `src/objects/sprite_obj.cpp` | `Sprite`: constructor, destructor, the other virtual methods, the frame and animation accessors, with the 3 C-linkage hitbox edge helpers | 46 + 3 | **old_agbcp** (was agbcc) | 108 pins, 1 keep, 11 asm, 2 retyped reads -> 0 | 7a |
| `src/objects/sprite_anim.cpp` | the rest of `Sprite`'s accessors, `UiSprite`, `PartList`'s `Update`, `Collide`, `CollideWithPlayer` | 32 | old_agbcp | 22 pins, 3 asm -> 0 | 7a |
| `src/objects/part_list.cpp` | `PartList`'s constructor, destructor, `Draw`, `Remove`, `RemoveAt`, `Add` (include/part_list.hpp; the class is in sprite_obj.hpp), with `InitCrateList` (C linkage) | 6 + 1 | agbcp | 0 -> 0 | 7c |
| `src/objects/part_list_cull.cpp` | `PartList::Cull`, `Clear`, `CollideClass` | 3 | **old_agbcp** (was agbcc) | 25 pins -> 0 | 7c |
| `src/objects/part_collide.cpp` | `PartList::CollideWithObject` | 1 | old_agbcp | 0 -> 0 | 7c |
| `src/objects/collision_queue.cpp` | `CollisionQueue` (include/part_list.hpp) | 4 | agbcp | 1 pin, 1 const (`STACK_ARG_U8_ADDR`) -> 0 | 7c |
| `src/gfx/palette_cycle.cpp` | `PaletteCycles`, `HudPart` (include/part_list.hpp) | 8 | **old_agbcp** (was agbcc) | 11 pins, 9 asm -> 0 | 7c |
| `src/crates/crate.cpp` | `Crate` (include/crate.hpp): `IsInsideRect`, the stack links, `GetClassId`, constructor, destructor; with `ResolvePlayerCollisions` and the line steppers' out-of-line copies | 8 + 3 | **old_agbcp** (was agbcc) | 13 pins, 3 asm -> 0 | 7e |
| `src/crates/crate_create.cpp` | `Crate::Create` | 1 | old_agbcp | 3 uses, 1 const -> 1 const | 7e |
| `src/crates/crate_draw.cpp` | `Crate::Draw` | 1 | **old_agbcp** (was agbcc) | 7 pins, 1 asm -> 0 | 7e |
| `src/crates/crate_update.cpp` | `Crate::Update` | 1 | old_agbcp | 0 -> 0; the vcall macro goes | 7e |
| `src/crates/crate_hit.cpp` | `Crate::PlayerHitboxOverlapsAt`, `ResolveStackHit`, `BreakIfTouchedByPlayer` | 3 | old_agbcp | 3 `BOX_ADDR` -> the same | 7e |
| `src/crates/crate_touch.cpp` | `Crate::PlayerAnimWouldTouch` | 1 | old_agbcp | 3 `BOX_ADDR` -> the same | 7e |
| `src/crates/crate_player_collide.cpp` | `CollidePlayerWithCrates` (C linkage) | 0 + 1 | old_agbcp | 0 -> 0 | 7e |
| `src/crates/crate_reset.cpp` | `Crate::Reset`, with `FindLineCrossing` | 1 + 1 | **old_agbcp** (was agbcc) | 13 pins, 7 asm, 1 retyped store -> 0 | 7e |
| `src/crates/crate_stack.cpp` | `Crate::OpenLife`, `IsKindBreakable`, `GetTop`, `GetBottom`, `CollideWithPlayer` | 5 | **old_agbcp** (was agbcc) | 4 pins, 7 asm, 1 `.pool` -> 1 asm label | 7e |
| `src/crates/crate_time_trial.cpp` | `Crate::OpenAkuAku`, with `ConvertCratesForTimeTrial` | 1 + 1 | **old_agbcp** (was agbcc) | 3 pins, 1 asm, 1 volatile read -> 0 | 7e |
| `src/crates/slot_crate.cpp` | `Crate`'s accessors (the slot crate's word, `kind`, `state`, ...) | 26 | **old_agbcp** (was agbcc) | 6 pins -> 0 | 7e |
| `src/objects/moving_sprite.cpp` | `MovingSprite`: constructor, destructor, `Reset`, `ApplyVelocity`, `Update`, the previous position | 11 | **old_agbcp** (was agbcc) | 30 pins, 1 asm, 2 retyped stores, 2 retyped `vs32` reads -> 0 | 7b |
| `src/objects/moving_sprite_collide.cpp` | `MovingSprite`'s `HandleEvent`, `CheckPlayerContact`, `ClassifyContact`, `AttachCtrl` and accessors | 21 | **old_agbcp** (was agbcc) | 13 pins, 1 volatile read -> 0 | 7b |
| `src/objects/player_contact.cpp` | `MovingSprite::TouchPlayer`, `ResolvePlayerContact` | 2 | **old_agbcp** (was agbcc) | 24 pins, gotos, 5 volatile reads -> 1 volatile read | 7b |
| `src/objects/step_probe.cpp` | `MovingSprite::ProbeEdgeTerrain` | 1 | agbcp | 1 pin -> 1 pin | 7b |
| `src/objects/ground_sprite.cpp` | `GroundSprite`: constructor, destructor, `Reset`, `Draw`, the flag accessors | 16 | **old_agbcp** (was agbcc) | 34 pins -> 0 | 7b |
| `src/objects/ground_sprite_collide.cpp` | `GroundSprite::CheckPlayerContact`, `ProbeTerrainAxes`, `ProbeFloor` | 3 | old_agbcp | 2 pins, gotos -> 0 (one goto) | 7b |
| `src/objects/ground_sprite_update.cpp` | `GroundSprite::Update`, `AnchorHitbox` | 2 | **old_agbcp** (was agbcc) | 14 pins, 4 asm, gotos -> 0 | 7b |
| `src/pickups/extra_life.cpp` | `ExtraLife` (include/pickups.hpp), `Wumpa::CheckPickup` | 16 | old_agbcp | 8 pins, 5 uses, 2 keeps, 1 retyped store, 2 volatile reads -> 0 | 7h |
| `src/pickups/wumpa_update.cpp` | `Wumpa`'s `PickUp`, `Update`, `Create`, `SendToHud`, `StartPayout`, `UpdateHop` | 6 | old_agbcp | 1 pin, 2 uses, 1 keep, 1 const, a volatile store and pointer -> 1 keep | 7h |
| `src/pickups/wumpa.cpp` | `Wumpa`'s small methods, `Stopwatch`, `ActionCtrl::Reset` | 15 | **old_agbcp** (was agbcc) | 15 pins, 3 asm, 5 volatile accesses -> 0 | 7h |
| `src/objects/part_list.cpp` (again) | `CrateList`'s constructor (`InitCrateList`, include/crate_list.hpp; C linkage before) | 1 | agbcp | 0 -> 0 | 7f |
| `src/crates/crate_grid_unlink.cpp` | `CrateList::Unlink` | 1 | old_agbcp | 1 pin -> 1 pin | 7f |
| `src/crates/crate_grid_link.cpp` | `CrateList::LinkActive` | 1 | **old_agbcp** (was agbcc) | 8 pins, 1 keep -> 0 | 7f |
| `src/crates/crate_list_update.cpp` | `CrateList::Update` | 1 | old_agbcp | 1 pin, 1 hold, 1 use, 1 barrier, statement-expression copies -> the barrier (in `Detach`), inline copies | 7f |
| `src/crates/crate_list_draw.cpp` | `CrateList::Draw` | 1 | agbcp | 7 pins, 1 asm -> 0 | 7f |
| `src/crates/crate_grid_collide.cpp` | `CrateList::Collide` (UNUSED), `CollideWithPlayer` | 2 | old_agbcp | 0 -> 0 | 7f |
| `src/crates/crate_player_collide.cpp` (again) | `CrateList::CollidePlayer` (C linkage before) | 1 | old_agbcp | 0 -> 0 | 7f |
| `src/crates/crate_list_reset.cpp` | `CrateList::Reset` | 1 | agbcp | 0 -> 0 | 7f |
| `src/crates/crate_list.cpp` | `CrateList::CollideWithObject`, `Remove`, `RemoveAt`, `AddNode`, `Link`, `Add`, destructor | 7 | old_agbcp | 4 pins -> 0 | 7f |

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

Part 6b in numbers: Dingodile's two files, 31 functions and seven
classes. The Neo Cortex fight's target, cannon and boss controllers
sit between Dingodile's methods in the ROM, so their methods here are
converted too; the rest of them (`UpdateCortexTarget`,
`UpdateCortexBoss`, ...) are still C in cortex.c. `MATCH_HOLD_REG` 1985
-> 1979, `MATCH_USE` 79 -> 78 and asm labels 20 -> 19 project-wide. The
virtual calls (`SetMode`, `SetTargetAnim`, `Attach`, the part's
`IsOnScreen`, `StartTargetMotionY` with the rocket and stalactite
ramps, the shield's and the target's `mover->SetMode`) are plain C++,
and `new DingodileShieldCtrl`/`DingodileProjectileCtrl`/`DingodileSharkCtrl`
replace `Create...(OperatorNew(n))`. `SpawnStalactite` has the
projectile's constructor inlined: that is an inline constructor
(`DingodileProjectileCtrl(DingodileProjectileCtrl *rocket)`) next to
the out-of-line default one. `SetDingodileState`'s four pins and the
macro that loaded a shared indirect call's arguments go: the ROM's
shared `bl _call_via_r3` is cross-jumping's merge of three
`SetTargetAnim(part, n); StartMotion(part, 0); break;` cases, once each
case has its own tail (a fall-through into `case 2:` blocks it).
`StartMotion`'s two pins and use go too. `UpdateDingodileShield` (once NAKED,
then C with holds) is C++ now, but keeps the C's
r5/r6 holds and the BLDCNT accumulator's pin and constant-init: they
are register allocation, needed under agbcp too, and no natural
spelling of the unfolded `orrs` chain was found.

`SpriteObj`'s fields gained names here (sprite_obj.hpp): the flag bits
(`visible`, `active`, `vulnerable`, and `blink` in the byte at 0x0D)
and the mirror byte's bitfield view, `mirrorBits` (`gfxMode`, a signed
`flipX`, `flipY`, `priority`). The view is an anonymous union with the
byte `mirror`, packed: an ARM struct or union is 4-byte sized and
aligned otherwise, which would move the fields after it.

Part 5b in numbers: the rest of the action controller, 25 `ActionCtrl`
methods in 4 objects (`action_ctrl_event.cpp`, `_hang.cpp`,
`_run_jump.cpp`, `_states.cpp`), so the whole class is C++ now except
`Reset` (`ResetActionCtrl`, in wumpa.c). Project-wide: `MATCH_HOLD_REG`
1979 -> 1959, instruction-emitting `asm` 235 -> 233 (and one file-scope
pool word fewer), `MATCH_USE` 78 -> 76, `MATCH_KEEP` 55 -> 53, `MATCH_HOLD`
22 -> 21, `MATCH_CONST` 29 -> 28, asm labels 19 -> 18, retyped field reads
95 -> 92; the four files' scoped volatiles go from 4 to 2. The C's 99
`ACT_CALL*`/`ACT_VCALL*` vcalls are `SetMode(...)`/`SetTargetAnim(Sprite(),
...)`, and with them gone, action_obj.h's vcall macros, `struct
act_vtable`, `struct act_method`, `struct act_anim_pair` and `ActSetNext`
go too (`struct act` keeps its layout, for wumpa.c). All four objects
match only under old_agbcp, like their old_agbcc C.

- **`DoSuperBodySlamShockwave`** had 7 pins, `_call_via_r1`/`_r4` calls
  through raw slot offsets, gotos for the loop and two hand-written
  `ldr` asm statements sharing a file-scope literal pool word. It is now a
  `while` loop calling `other->GetClassId()` and `other->HandleEvent(0,
  EVENT_ATTACK_SUPER_BODY_SLAM, 0)` on a new `MovingSprite` view
  (sprite_obj.hpp, below), with no workaround: the loop's test calls an
  inline function (`CollidableCount()`), which keeps gcc from copying
  the test in front of the loop, so CSE doesn't carry the list's address
  into the body and the ROM's reload comes out; the 0x40 range is a
  variable (`range`), which the ROM keeps in r8.
- **`StartTornadoSpin`** (2 pins, 2 holds/uses, slot arithmetic) and
  **`StateDying`** (the gone-bit sequence spelled out with 5 pins, a
  `MATCH_KEEP` and a `volatile` read) need nothing: the second is
  `SpriteObj::MarkGone()`.
- `StateLeftGround`'s `MATCH_CONST` and `MATCH_USE`, `StateSpin`'s and
  `StateTornadoSpin`'s `ActSetNextB` (2 pins and a keep), `StateCrawlStart`'s
  4 pins and the `UpdatePlayerFacing_u8` asm label go too.

Kept, each with a comment: `ReleaseHang`'s r2 hold (one pin, one empty
`MATCH_HOLD`/`MATCH_USE` pair, no code: the 0x600 reload must take r3),
`StateJump`'s `MATCH_KEEP` and `HandleEvent`'s three `MATCH_CONST`s (a 1
the ROM loads apart from the A test's own 1), `HandleEvent`'s dead
`volatile` read of `state`, and `StateCrouch`'s scoped `volatile` pointer
(the `+0x28` address computed in the part's register). Two C idioms stay:
action_obj.h's byte view of `flags2` (`ACT_PART_FLAGS0D`) in
`StateLeftGround`, `StateAirborne` and `StateAirSpin`, and a `do { }
while (0)` around one call in `StateCrawl` (gotchas).

Part 7a in numbers: the base of the entity family, 4 objects and 177
functions: `graphics.cpp` (the `Entity` base class and the sprite
graphics managers it shares the file with), `sprite.cpp`,
`sprite_obj.cpp` and `sprite_anim.cpp` (`Sprite`, `UiSprite`, the part
list's update and collision passes). Project-wide: `MATCH_HOLD_REG` 1959
-> 1744, instruction-emitting `asm` 233 -> 206, `MATCH_KEEP` 53 -> 52,
retyped field stores 224 -> 217, retyped field reads 92 -> 90 and scoped
volatiles 45 -> 44. All four
match only under old_agbcp, with no workaround but one pin
(`Sprite::CheckPlayerContact`'s shared 1, below) and one retyped store
(`Sprite::Reset` clears the mirror and palette bytes as one halfword).
`graphics.o` and `sprite_obj.o` move to `OLD_AGBCC_OBJS`: their 172 pins
and 22 `asm` statements were old_agbcc's code (constant before `ldrb`,
the accumulator in the constant's register, the operand order of an
`add`) reproduced under agbcc. Even `IsEntityInsideRect`'s 10-instruction
`asm` block and `MarkEntityGone`'s copy-and-shift are plain C++ now.

**The real classes** (include/entity.hpp, include/sprite_obj.hpp), with the
ROM's sizes (`CreateEntity` allocates 0x1C bytes, `CreateSpriteObj` 0x40,
`CreateMovingSprite` 0x78, `CreateGroundSprite` 0x80):

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `Entity` | 0x1C | gEntityVtable (11 slots) | graphics.cpp |
| `Sprite` | 0x40 | gSpriteObjVtable (13) | sprite.cpp, sprite_obj.cpp, sprite_anim.cpp |
| `UiSprite` | 0x40 | gUiSpriteObjVtable | sprite_anim.cpp |
| `MovingSprite` | 0x78 | gMovingSpriteVtable (15) | moving_sprite.cpp, moving_sprite_collide.cpp, player_contact.cpp, step_probe.cpp (part 7b) |
| `GroundSprite` | 0x80 | gGroundSpriteVtable (15) | ground_sprite.cpp, ground_sprite_collide.cpp, ground_sprite_update.cpp (part 7b) |
| `SpriteObj` | 0x80 | none of its own | the controllers' name for what they drive |

The 0x40-byte class is `Sprite`: the C names of its methods say "SpriteObj"
(`InitSpriteObj`, `DestroySpriteObj`, `gSpriteObjVtable`), and cxx_symbols.txt
maps them. The name `SpriteObj` was taken: every controller method takes a
`SpriteObj *` (in its mangled name, `P9SpriteObj`), and the C++ files use
its fields from `mover` to `type`. So `SpriteObj` stays, as an empty
subclass of `GroundSprite`: it has every field it had, and every Entity,
Sprite and MovingSprite method. The controllers drive moving sprites; their
methods can take a `MovingSprite *` once no part of #664 is in flight
(the plan below).

**#656 for this family.** The classes are the definitions now: one set of
names per byte (`tag` the animation, `frame` the step, `stepTimer`,
`bank` the sprite bank, the flag bits of +0x0C/+0x0D in `EntityFlags`,
the mirror byte's two bitfield views). The C views stay for the C files,
each checked against its class in sprite_obj.hpp: actor.h's `struct
actor` (Entity), box_part.h's `struct box_part` (MovingSprite, with a few
subclass fields), gfx_part.h's `struct gfx_part` (a prefix of
MovingSprite) and gobj_1a794.h's `struct gobj` (GroundSprite).

What made the C++ match:

- **`Entity`'s inline methods.** The ROM's graphics.c ends with
  `InitEntity`, 26 one-line accessors and `DestroyEntity`, in that order,
  after `CreateEntity`, `GetEntityClassId` and `ResetEntity`; and the
  subclasses inline `DestroyEntity` (`DestroySpriteObj`) and
  `MarkEntityGone` (the controllers). That is a class whose accessors,
  constructor and destructor are inline, in a header with no `#pragma
  interface`: g++ 2.9 then emits the vtable in the file that defines the
  first non-inline virtual method (`CheckPlayerContact`, graphics.cpp),
  and with it out-of-line copies of all the inline methods, at the end of
  the file, **in the reverse of their declaration order**. So
  include/entity.hpp has no `#pragma interface`, declares the accessors
  inline from `GetId` (`DestroyEntity`'s neighbour) up to
  `ClearAlwaysActive`, and graphics.cpp defines the constructor `inline`
  just before `Create`, which inlines it (`new Entity`); its copy is the
  first one emitted. The vtable g++ emits is a weak symbol in a
  `.gnu.linkonce.d` section: the linker script's `/DISCARD/` drops it, and
  the ROM's table is still the C data. Two accessors call another one out
  of line (`SetPosVec`, `SetPixelPosVec`), as in the ROM: a method declared
  later in the class isn't compiled yet when an earlier one's body is, so
  it can't be inlined there.
- **Struct arguments.** A struct passed by value is copied by g++ with
  `memcpy`, which in this ROM is `MemCopy32` (src/system/boot.c; GAX has
  `.set memcpy, MemCopy32`), so cxx_symbols.txt maps `memcpy` to it. That
  is `CollidePartList`'s "copy the box into a shared temporary" before each
  call: `CollideWithPlayer(box, part)`, with no `MemCopy32` in the source.
- **Bit tests and stores.** The ROM extracts single flag bits as `(flags >>
  n) & 1` (a byte, a shift and a 1), which a 1-bit `u8` field read doesn't
  give (`lsl; lsr`): the accessors and tests spell the shift, with a local
  where the test is negated (`s32 hidden = ...; if (!hidden)`, else g++
  adds an `eor`). Clears and sets are bitfield stores (`f.b.vulnerable =
  0`), which give the ROM's `-0x41` mask; a two-bit store (`attr0.affineMode
  = 2`, gfx.h's new byte view of an OAM entry's attribute 0) gives
  `HideUnusedOamEntries`'s `-4` where `& ~3` gives `0xFC`. The mirror byte
  has an unsigned bitfield view too (`mirrorFlags`), for the values the ROM
  extracts (`lsl #27; lsr #31`); the signed one (`mirrorBits`) is for tests.
- **Small shapes.** `table[i++]` four times gives `SetOamAffineScales`'s
  pointer stepping 8 bytes per store; `if (slotOf[id] != 0xFF) return
  slotOf[id];` gives `GetPaletteSlot`'s registers; reading `gPlayer` again at
  each use (`PlayerSprite()`) gives `CollidePartWithPlayer`'s reloads; a
  local copy of `bank` gives `SetSpriteFrameIndex`'s load order.

Kept: `Sprite::CheckPlayerContact`'s one pin (the 1 shared by the two flag
tests and the gone flag's `orr`; unpinned, old_agbcp gives that register to
the gone bit's shift instead) and `Reset`'s halfword store.

Part 7e in numbers: the crate, `Crate` (new `include/crate.hpp`, a
0x64-byte `Sprite`, gCrateVtable), 11 objects and 54 functions (48
methods, and 6 free functions that share their files:
`ResolvePlayerCollisions`, `CollidePlayerWithCrates`, `FindLineCrossing`,
the two line steppers and `ConvertCratesForTimeTrial`). Project-wide:
`MATCH_HOLD_REG` 1744 -> 1698, instruction-emitting `asm` 206 -> 187,
`MATCH_USE` 76 -> 73, `.pool` 7 -> 6, retyped field stores 217 -> 216 and
scoped volatiles 44 -> 43; asm labels 18 -> 19 (below). All 11 match only
under old_agbcp: the 6 agbcc objects (crate.o, crate_draw.o, crate_reset.o,
crate_stack.o, crate_time_trial.o, slot_crate.o) move to `OLD_AGBCC_OBJS`,
their 46 pins and 15 `asm` statements old_agbcc's code (the constant
before the `ldrb`, the shift into another register) reproduced under
agbcc. `IsCrateInsideRect`'s 10-instruction box `asm`, `ResetCrate`'s
mask `asm`, the `~8` clears of `DrawCrate` and `CollideCrateWithPlayer`
and `GetTopCrate`'s mask checks are plain C++ now (`ClearTouched()`, the
`state & 0x7f` tests).

The class: the fields from 0x40 are crate.h's, the per-kind word at 0x48
an anonymous union (`slotState`, `group`, `bounceTimer`, ...), the stack
links `Crate *`. It overrides `Update`, `Draw`, `IsInsideRect`,
`GetClassId` and the destructor. crate.h's `PHYS_CALL` and
`_call_via_rN` calls are virtual calls (`ApplyVelocity()`,
`GetBounds()`, `e->GetClassId()`, the player's `HandleEvent`), and
`PhysSetTag`/`PhysSetFrame` are inline methods (`SetTag`, `ClampFrame`).
`crate.hpp` also declares crate_break.c's functions that these files call
(`UpdateFall`, `Explode`, `QueuePlayerCollision`, ...) as methods, mapped
to their C names; crate_break.c is part 7g. `CollidePlayerWithCrates`
(the crate list's) keeps C linkage for part 7f's `CrateList`.

What made the C++ match:

- **`new Crate`.** `CreateCrate` has the constructor inlined, then the
  id stored through its result: crate_create.cpp defines `Crate::Crate()`
  `inline` before `Create` (the "inline in one file only" gotcha), and
  `InitCrate` is crate.cpp's out-of-line definition.
- **The line steppers.** `FindLineCrossing`'s four octants are
  `FindLineCrossingXMajor`/`YMajor` inlined (with a step of 1 or -1),
  which the ROM also has out of line, with no caller, at the end of
  crate.o. Written that way, `FindLineCrossing` comes out as the ROM's,
  including the first octant's own copy of the return, which the plain
  four-case C got merged into the others' by the cross-jump: the C's
  `goto`, 10 pins and 5 `asm` statements go. The two are in
  include/crate_line_step.hpp, a header fragment: crate.hpp includes it
  with `CRATE_LINE_STEP` `inline`, crate.cpp at its end with it empty.
- **A `bool` stack argument is stored with `strb`.** `OpenLifeCrate`
  passes `DropExtraLife`'s sixth argument (on the stack) as a byte; a `u8`
  parameter is promoted and stored with `str`, a `bool` isn't. The C spelt
  the whole call in `asm`; the C++ calls it through a `bool`-flag alias
  (an asm label, since drop_extra_life.c is C with a `u32` flag), and
  `OpenLife` takes a `bool`.
- **A u8 result stored into a 4-bit bitfield** loses the zero-extension
  the ROM has (`palette = GetPaletteSlot(...)`, `palette` a `u8:4`): store
  it through a `u32` local (`UpdateCrate`).
- Small shapes: a local copy of `id` (`OpenLifeCrate`'s single load), the
  drop position in locals before the call (the spawner is loaded last),
  `struct player *p; u32 one;` assigned in turn (`SetCrateBusy`'s 1 between
  the player and its address), `ResolveStackHit` through a local copy of
  `this`.

Kept, each with a comment: `CreateCrate`'s `MATCH_CONST` (the mystery
crate's tag 0, which the ROM loads before the tag's address; a plain 0 is
loaded after it, from a register CSE shares) and the 6 `BOX_ADDR`s of
`BreakCrateTouchedByPlayer` and `PlayerAnimWouldTouchCrate` (the player
box's stack address recomputed at each call, as in the C). C idioms kept:
`GetTopCrate`'s and `GetBottomCrate`'s `goto`s (the ROM has both returns
before the loop) and `UpdateCrate`'s `goto done`, nested `if`s and `s32`
copy of `kind`. The `DropExtraLife` alias is the one new asm label.
Part 7b in numbers: `MovingSprite` and `GroundSprite`, 7 objects and 56
functions (moving_sprite.cpp, moving_sprite_collide.cpp, player_contact.cpp,
step_probe.cpp, ground_sprite.cpp, ground_sprite_collide.cpp,
ground_sprite_update.cpp). Project-wide: `MATCH_HOLD_REG` 1707 -> 1590,
instruction-emitting `asm` 197 -> 192, retyped field stores 217 -> 215,
retyped field reads 90 -> 88 and scoped volatiles 44 -> 39. Of the C's 118
pins, 5 `asm` statements and 10 retyped or volatile accesses, one pin and
one volatile read are left (below). Six of the seven objects match only
under old_agbcp: `moving_sprite.o`, `moving_sprite_collide.o`,
`player_contact.o`, `ground_sprite.o` and `ground_sprite_update.o` move to
`OLD_AGBCC_OBJS` (their pins were old_agbcc's constant-before-`ldrb` and
register choices reproduced under agbcc; `ground_sprite_collide.o` was
old already); `step_probe.o` compiles the same under both and stays agbcc.

The classes now have every method of the two vtables, the constructors
(`InitMovingSprite`, `InitGroundSprite`; `new MovingSprite(id, x, y)` and
`new GroundSprite(id, x, y)` inline them into `Create`, as `Sprite` does),
the destructors (`~MovingSprite` is `delete mover`), the motion and hit
mask accessors and the terrain probes. The four `_call_via_rN` trampolines
on the controller (`mover->Update(this)`, `HandleEvent`, `Attach`, `delete
mover`) and the three on the sprite itself (`TouchPlayer()`, `GetBounds()`,
`IsNearCamera()`) are plain virtual calls; the controller's still take a
`SpriteObj *` (step 7b'), so `this` is cast. `EntityFlags` names the two
low bits of +0x0D, `floorProbe` and `grounded` (entity.hpp).

What made the C++ match:

- **Block copies.** `MovingSprite::Pos()`/`PrevPos()` view the position
  and the previous position as a `struct gfx_vec`. `PrevPos() = Pos()` is a
  block copy (both loads, then both stores), after which gcc reloads `x`
  and `y`, as the ROM does; the C read them through `vs32` casts.
  `GetPrevPos` returns `PrevPos()`: a struct return, the hidden pointer in
  r0 before `this`, which is the C's `GetSpritePrevPos(dest, self)`.
- **No locals for the speeds.** `ApplyVelocity`'s tail uses `speedX` and
  `speedY` directly; CSE keeps them in the ROM's registers (the C pinned
  them), and the redundant `gLastSpriteVelY` store the C wrote in `asm` is
  kept as written (`if (gLastSpriteVelY != 0 && speedY == 0)
  gLastSpriteVelY = speedY;`).
- **Early returns instead of a shared store.** `ProbeFloor`'s three exits
  shared one `strb` in the C (a `val` and gotos, with two pins for the
  copy of the flags byte). Written as `f.b.grounded = 1; return hit;` in
  each hit path, and the "was it grounded" re-test inside the retry's
  block, cross-jumping gives the shared store, the copy and the shared 1
  by itself.
- **One result variable.** `ClassifyContact` gets the ROM's shared
  `result = 2` block from `if (...) result = 2; else { ...; if (...)
  result = 2; }`, not from early returns.
- **A pointer argument computed first.** `TouchPlayer` passes `gPlayer` to
  the inlined `IsPlayerInvulnerable` (a file-local copy), so it is loaded
  before the comparison's 0, as in the ROM.
- **Declarations at the first assignment.** `TouchPlayer`'s two boxes are
  declared where they are filled (`struct aabb box = GetAttackBox();`), at
  function scope, so each has its own slot (the frame is 32 bytes) and no
  copy.

Kept: `ProbeEdgeTerrain`'s pin on the tries pointer (unpinned, it and the
saved probe flag swap r6 and r7; rewrites of the retry loop were further
off) and `TouchPlayer`'s volatile read of the body box's `w` (otherwise it
is read through the register holding the box's address, and `this` moves
from r4 to r5). `ProbeTerrainAxes` keeps one goto (the ROM jumps from the
X nudge straight to the Y probe).

**How the C and C++ views share a layout.** One header for both
languages would need `#ifdef __cplusplus` around every class, and a C
struct can't have a class's base or methods, so each family has a C++
class in a `.hpp` header and keeps its C struct in the C header for the
C files: `Ctrl`/`struct ctrl` (objects.h), `InputCtrl`/`struct
input_ctrl` (input_ctrl.hpp) and `BossCtrl`/`struct boss_ctrl` (player.h)
and `MegaMixCtrl`/`struct mega_mix_ctrl` (bosses.h) (boss_ctrl.hpp),
`Entity`/`struct actor` (actor.h; entity.hpp), `MovingSprite`/`struct
box_part` (box_part.h), `GroundSprite`/`struct gobj` (gobj_1a794.h),
`PartList`/`struct part_list`, `OamBuffer`/`struct oam_shadow_buffer`,
`ObjVramCursor`/`struct vram_upload_cursor`, `PaletteCache`/`struct
palette_cache` (sprite_obj.hpp), `EnemyCtrl`/`struct part_ctrl` (part_ctrl.h),
`PeriodicSpawner`/`struct periodic_spawner` (enemies.h),
`ActionCtrl` (whose C view, `struct act`, went in part 7h with its last
C user), `Crate`/`struct crate` (crate.h; crate.hpp), `ExtraLife` and
`Wumpa`/`struct orbit_part` (orbit_part.h; pickups.hpp), `CrateList`/`struct
pool_manager` (crates.h; crate_list.hpp). Each class has
a `COMPILE_TIME_ASSERT` that its size is the C struct's (the `.hpp` includes the C header), and its
fields keep the C names and offset comments. The C prototypes of the
converted methods stay in the C headers, under their C names, for the
vtable data and the C callers. When the last C user of a struct is
converted, the struct can go.

**Sprite objects.** `include/entity.hpp` has `Entity` and
`include/sprite_obj.hpp` the classes built on it (part 7a, above), shared
by the controllers instead of each file's own partial view. `EnemyCtrl`
reaches its part through an anonymous union, as part_ctrl.h's `struct
ctrl_target` (its fields) or as a `SpriteObj` (what the `Ctrl` methods and
the part's virtual methods take). The player is still C: C++ code reaches
it as a `GroundSprite` through `PlayerSprite()`.

Part 7c in numbers: the rest of the part list, the player's collision
queue, the palette cycles and the HUD part, 5 objects and 23 functions
(include/part_list.hpp). Project-wide: `MATCH_HOLD_REG` 1744 -> 1707,
instruction-emitting `asm` 206 -> 197 and `MATCH_CONST` 28 -> 27. Nothing
is kept: all five objects are plain C++. `part_list_cull.o` and
`palette_cycle.o` move to `OLD_AGBCC_OBJS` (`CollideClass`'s `ldrb r1;
lsrs r0, r1, #2` and `Tick`'s index loads are old_agbcp's registers; their
36 pins reproduced them under agbcc); `part_list.o` and `collision_queue.o`
match under both and stay on agbcc.

- **The classes.** `PartList`'s other methods are declared in its class
  in sprite_obj.hpp (a class can't be reopened). `CollisionQueue` (the
  player's +0x108), `PaletteCycles` (`gPaletteCycles`) and `HudPart`
  (`gHudPartVtable`: a `UiSprite` with only its own destructor, so its
  constructor and destructor are empty bodies) are in part_list.hpp, each
  checked against its C view (`struct collision_queue`, `struct
  palette_cycler`, `struct hud_digit_part`). `DrawPartList`'s, `Cull`'s,
  `Clear`'s and `CollideClass`'s slot calls are `Draw()`,
  `OverlapsRect()`, `delete` and `GetClassId()`/`CheckPlayerContact()`;
  the arrays are `new MovingSprite *[n]` and `delete[]`.
- **One-byte stack arguments.** `AddCollisionCandidate`'s two trailing
  bytes and `AddPaletteCycle`'s `direction` are read with `ldrb` from
  their stack words, the address formed first: that is a one-byte struct
  parameter (`struct byte_arg`), read into a local at the top. A `u8`
  parameter loads the word and narrows it. The C needed an `asm` `add
  rX, sp, #N; ldrb` block or `MATCH_CONST` and a pin for it.
- **Indexed loops.** 8 of `TickPaletteCycles`' `asm` statements kept the
  operand order of `add rN, rOff, rList`; as indexed loops over
  `list[j]` (`for (j = counts[i] - 1; j >= 0; j--)`), loop strength
  reduction gives the ROM's pointer walks, operand order included.
- **`RemoveFromPartList`'s search** (`while (items[i] != part) if (++i
  >= n) return;`, `n` a copy of `capacity`) gives the ROM's rotated loop
  and its exit straight to the return; a `for` with a `break` makes gcc
  reuse the bound for the `if (i < capacity)` after it.

Part 7h in numbers: the pickups, 3 objects and 37 functions (the new
include/pickups.hpp): `ExtraLife` and `Wumpa` (0x54-byte `Sprite`s with a
14th virtual, `CheckPickup`), `Stopwatch` (a `Sprite`) and the action
controller's last C method, `ActionCtrl::Reset`, which the ROM puts after
the stopwatch. Project-wide: `MATCH_HOLD_REG` 1544 -> 1520, instruction-emitting
`asm` 173 -> 170, `MATCH_USE` 73 -> 66, `MATCH_KEEP` 52 -> 50, `MATCH_CONST`
27 -> 26, retyped field stores 214 -> 212, retyped field reads 88 -> 82 and
scoped volatiles 38 -> 29. All three match only under old_agbcp: `wumpa.o`
moves to `OLD_AGBCC_OBJS` (`Draw`'s and `Reset`'s constant before the
`ldrb`; its pins reproduced them under agbcc), the other two were old_agbcc
objects already. With `ResetActionCtrl` converted, action_obj.h's `struct
act` has no C user left and is gone: `ActionCtrl` checks its size against
0x38 (PlayRoom's `OperatorNew(0x38)`), and player.h's C prototypes keep
`struct act` as an incomplete type. action_obj.h keeps only the input and
flags2 helpers of the action controller's C++ files. orbit_part.h's `struct
orbit_part` stays the C view of both pickups (drop_extra_life.c,
entity_spawner.c, crate_break.c and time_trial.c still use it).

- **`DropWumpa`'s last parameter is a `bool`,** like `DropExtraLife`'s
  (part 7e). The wumpa's payout calls
  `DropWumpa(spawner, x, y, 0, 0, 1)` with the 1 stored as a byte (`strb`
  into the stack slot), and entity_spawner.c reads it back with `ldrb`. g++
  passes a `bool` exactly that way, register order included; a one-byte
  struct (`struct byte_arg`) gives the same instructions with the 1 loaded
  before the slot's address, which the C needed a `MATCH_CONST` and
  hand-written stack slots for. entity_spawner.c is still C, so the call
  casts `DropWumpa` to the `bool` signature (a function pointer cast; part
  7e's `DropExtraLife` call uses an asm-label alias). The other `struct byte_arg`
  parameters (`AddCollisionCandidate`, `AddPaletteCycle`, ...) may well be
  `bool`s too.
- **One variable for both axes.** The flight step (`Fly`: `n = x + velX;
  x = n; n = y + velY; y = n;`, one `n`) gives the ROM's registers
  (position r1, velocity r0), which the C forced with two `MATCH_USE`s per
  velocity; and `ExtraLife::Update`'s arrival test computes `x + velX`
  again from the old values because the sum's variable then holds the new
  y, which the C did with a `MATCH_KEEP` and a `MATCH_USE`.
- **`UpdateStopwatch`'s distance test** (9 pins and 2 `asr` asm
  statements in the C) is a plain `if`/`else` on one variable, `d`, used
  for both axes (a variable per axis ties the load to the shift), with
  `MarkGone()` written in both `else`s; cross-jumping merges them into the
  ROM's single tail.
- **Kept:** `CreateWumpa`'s `MATCH_KEEP` of `phase` (the C's too). The ROM
  compares an unknown-to-CSE 0 in r6 with the frame count and stores it at
  +0x4B while `counter` gets a fresh 0; written as constants, CSE shares
  one 0. Inline `SetAnim`/`SetFrameIndex`/`SetHop` helpers didn't give it.
  C idioms kept, each with a comment: `u8 one` locals for the `screenSpace`
  stores (the 1 before the address), an `s32` view of `affine` for its
  signed re-read (`Affine`), the `phase + 1` zero-extension spelled out, the
  palette slot through an `s32` for the `u8` return's zero-extension, and
  `SendToHud`'s `Offset` inline (the pool constant reloaded per axis).

Part 7f in numbers: the crate list, `CrateList` (new `include/crate_list.hpp`,
`gCrateList`), 7 objects converted and 16 methods: the 14 functions of the
7 files, plus the constructor (`InitCrateList`, in part_list.cpp for ROM
order) and `CollidePlayer` (`CollidePlayerWithCrates`, crate_player_collide.cpp),
which parts 7c and 7e left with C linkage. Project-wide: `MATCH_HOLD_REG`
1520 -> 1500, instruction-emitting `asm` 170 -> 169, `MATCH_USE` 66 -> 65,
`MATCH_KEEP` 50 -> 49, `MATCH_HOLD` 21 -> 20; `MATCH_BARRIER` stays 17 (the
C's moved into the header). Of the C's 21 pins, 1 `asm`, 1 keep, 1 hold and
1 use, one pin is left (below). `crate_grid_link.o` moves to
`OLD_AGBCC_OBJS` (`LinkActive`'s flag test is old_agbcp's `ldrb r1; lsrs r0,
r1, #4`, which the C pinned); `crate_list_draw.o`, `crate_list_reset.o` and
`part_list.o` match under both and stay on agbcc; the other four were old
already.

The class: crates.h's `struct pool_manager` is its C view (play_room.c,
run_room.c, crate_break.c, ... still use it), and `CrateGridNode`/`CrateGridLink`
are `struct pool_node`/`struct pool_link`'s. The slots and nodes hold
`Sprite *`s (crates in practice; `CollidePlayer` casts to `Crate *`, and the
unused `Collide` and its two helpers take `MovingSprite`s, as PartList's do).
The slot calls are virtual calls: `OverlapsRect`/`Draw` (Draw),
`IsInsideRect`/`Update`/`delete` (Update, Reset), `GetClassId` (Collide). The
grid's column is `ColumnOf(sprite)`, the high halfword of `x` (one `ldrsh`).
`Crates()` is `gCrateList` as the class (crate_create.cpp's `Crates()->Add(this)`).

What made the C++ match:

- **Inline bodies with an out-of-line copy.** The ROM inlines
  `AddCrateGridNode` into `LinkCrateToActiveBucket` (and `UpdateCrateList`)
  and `RemoveCrateFromList` into `UpdateCrateList`, and has both out of line
  too. The bodies are inline methods, `Append` and `Detach`, and the
  out-of-line `AddNode` and `Remove` call them. With `Append` inlined,
  `LinkActive` needs none of the C's 8 pins or its `MATCH_KEEP_VOLATILE`:
  the free-list head's address is recomputed in each column, and the
  registers come out as the ROM's.
- **Column 255's head computed in the loop.** Draw and Update keep `&heads[255]`
  in a register across the loop, computed after the column array's address
  and with the operands of an `add` the other way round from what a local
  computed before the loop gives. Assigned at the top of the loop body, the
  loop optimizer hoists it after the `heads[i]` base, as in the ROM, and
  `heads[i]` keeps its base-first `add`: the C's `asm` `add`, its 7 pins
  (Draw) and its byte-arithmetic indexing (Update) go.
- **`lo = cam->x; lo >>= 8;`** in two statements loads into the shift's
  register, as Collide's C already had.
- `Unlink`'s "always active" test through a local (`s32 active = (f.flags >>
  4) & 1; if (!active)`) gives old_agbcp's `ldrb r1; lsrs r0, r1`.

Kept, each with a comment: `Unlink`'s free-list push keeps its r1 pin (the
ROM loads the head before the entry; unpinned, the registers or the order
swap, with every spelling tried). `Detach` has the C's `MATCH_BARRIER` (no
code): without it, cse swaps `n` and its copy in Update's two inlined
copies (`Remove` is the same either way); declaring `n` first gives the
registers but loads it before the 0. Update keeps the C's register copies
of the sprite for `Destroy`'s two calls, as an inline identity function
(`Copy`) where the C had statement expressions, its `next = i - 1` before
the walk, its double read of `node->data`, and `Append`'s body written out
through `last` (Append recomputes the address, which takes `last` out of its
register).

### Next batches

Bigger controllers, roughly in order (function counts from
`tools/cpp_survey.py --objects`):

1. ~~**The enemy controllers**~~: done in part 2 (all of src/enemies/). Their
   C callers are still C: the level spawners (`spawn_enemies.c`,
   `spawn_objects.c`'s SpawnSealSpawner); Dingodile's shark
   (`DingodileSharkCtrl`, part 6b) derives from it.
2. ~~**The rest of `Ctrl` and `InputCtrl`**~~: done in part 3. Only
   `Ctrl::Update` (`UpdateCtrl`, an empty function in `system/boot.c`)
   is still C.
3. ~~**The swim controller**~~ (`PlayerCtrl`): done in part 4; its
   `Reset`/`Restart` (in `action_ctrl.cpp`) in part 5a. **The action
   controller** (`ActionCtrl`): part 5a converted `action_ctrl.c`,
   `_moves.c`, `_update.c`, `_idle.c`, `_land.c`, `_left_ground.c` and
   `kill_player.c`; part 5b the other four (`action_ctrl_event.c`,
   `_hang.c`, `_run_jump.c`, `_states.c`), and action_obj.h's vcall
   macros went with them. Its last method, `Reset` (`ResetActionCtrl`,
   wumpa.cpp), was converted with the pickups in part 7h.
4. **The boss controllers:** ~~`tiny_update.c`, `mega_mix_update.c`~~
   (done in part 6) and ~~`dingodile*.c`~~ (part 6b). Left: `cortex.c`
   (25 functions, 64 lines with pins or asm; it also has
   `OneShotAnimCtrl`'s and `UnusedOneShotAnimCtrl`'s constructors and
   destructors, Tiny's constructor, destructor and `StartHop`, the
   Cortex target's and boss controller's other methods, and the Cortex
   shot class). cortex.c mixes controllers and entities: it is part 7i
   below.
5. **The entity family** (part 7, below), with the platform mover
   (`src/objects/platform.c`, `gPlatformMoverVtable`: a `Ctrl`-shaped
   0x38-byte class) in 7d.

#### The entity family (part 7)

Whole files only, base classes first. Function and workaround counts are
the C's (`tools/cpp_survey.py --objects`, `tools/match_idioms.py`); "old"
is an object already in `OLD_AGBCC_OBJS`. Expect most objects to match
only under old_agbcp, as all four of 7a's do, and most of their pins to go.

| Part | Files | Classes | Size | Depends on |
|---|---|---|---|---|
| ~~7a~~ | ~~graphics.c, sprite.c, sprite_obj.c, sprite_anim.c~~ | `Entity`, `Sprite`, `UiSprite`, `PartList` (update, collide), the sprite graphics managers | done | |
| ~~7b~~ | ~~objects/moving_sprite.c, moving_sprite_collide.c, player_contact.c, step_probe.c, ground_sprite.c, ground_sprite_collide.c (old), ground_sprite_update.c~~ | `MovingSprite` (its 15 slots, the speeds, the controller), `GroundSprite` (the terrain probe) | done | 7a |
| 7b' | the controller headers and cxx_symbols.txt | the controllers' `SpriteObj *` parameters become `MovingSprite *` (`P9SpriteObj` -> `P12MovingSprite`), and `SpriteObj` goes | a rename, no code change | 7b; no other part of #664 in flight |
| ~~7c~~ | ~~objects/part_list.c, part_list_cull.c, part_collide.c (old), collision_queue.c, gfx/palette_cycle.c~~ | the rest of `PartList`, the player's `CollisionQueue`, `HudPart` (a `UiSprite`) and the palette cycles | done | |
| 7d | objects/platform.c, platform_collide.c (old), platform_contact.c, platform_create.c (old) | `Platform` (a `MovingSprite`, 0x80 bytes), `PlatformMover` (a `Ctrl`) | 18 functions, 1050 lines, 34 pins, 3 asm | 7b |
| ~~7e~~ | ~~crates/crate.c, crate_create.c, crate_draw.c, crate_update.c, crate_hit.c, crate_touch.c, crate_player_collide.c, crate_reset.c, crate_stack.c, crate_time_trial.c, slot_crate.c~~ | `Crate` (include/crate.hpp) and its accessors | done | |
| ~~7f~~ | ~~crates/crate_list.c (old), crate_list_draw.c, crate_list_reset.c, crate_list_update.c (old), crate_grid_collide.c (old), crate_grid_link.c, crate_grid_unlink.c (old)~~ | `CrateList` (include/crate_list.hpp), the crate grid | done | |
| 7g | crates/crate_break.c (old) | the crates' break and bounce paths | 24 functions, 2860 lines, 11 pins, 6 asm | 7e, 7f |
| ~~7h~~ | ~~pickups/extra_life.c, wumpa.c, wumpa_update.c~~ | `ExtraLife`, `Wumpa`, `Stopwatch`, `ActionCtrl::Reset`; `struct act` went | done | |
| 7i | bosses/cortex.c (old) | the Neo Cortex fight's gem, platform mover (a `PlatformMover`) and shot controllers, the rest of the target's, cannon's and boss's methods, Tiny's constructor, destructor and `StartHop`, `OneShotAnimCtrl`'s and `UnusedOneShotAnimCtrl`'s constructors and destructors | 25 functions, 1000 lines, 49 pins, 4 asm | 7d |

Outside this family, the classes built on these: the player (`struct
player`, a `GroundSprite`; player*.c), the level select's camera lead and
launch pad (`MovingSprite`s, menus/level_select.c) and the effect parts'
spawner (level/entity_spawner.c, `new EffectCtrl`). With the family
converted, its vtables could be emitted by g++ (plan item 5 below).

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
  file that defines its first non-inline virtual method, as a weak
  `.gnu.linkonce.d` section the linker script doesn't place (it
  discards it). That file also gets an out-of-line copy of **every**
  inline method of the class, used or not, at its end, in the reverse
  of their declaration order: that is how graphics.cpp ends with
  `InitEntity`, the accessors and `DestroyEntity` (include/entity.hpp).
  An inline method meant only for other files (a second constructor,
  say) would be emitted there too.
- **An inline method calls a later-declared one out of line.** g++
  compiles in-class method bodies in declaration order, so a method
  declared after the caller isn't available for inlining yet
  (`Entity::SetPosVec` calls `SetPos`).
- **A method defined `inline` in one file only** (declared plainly in
  the class) is inlined in that file and called out of line from the
  others: `Entity::Entity()`, inlined into `Entity::Create` (`new
  Entity`), called as `InitEntity` by the subclasses' constructors, as in
  the ROM.
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
  (`UpdateMegaMix`). Where the ROM does copy (a `MemCopy32` call into a
  temporary, then the call), that copy is g++'s: pass the variable
  (`PartList::Collide` passes its `box` parameter on). The game's
  `memcpy` is `MemCopy32`, and cxx_symbols.txt maps the name.
- **A single flag bit read as a value** is `(flags >> n) & 1` in the ROM
  (shift, then AND with a 1); a 1-bit `u8` field gives `lsl; lsr`. And
  `if (!((flags >> n) & 1))` gets an `eor`: test a local (`s32 hidden =
  (flags >> 2) & 1; if (!hidden)`).
- **A byte RMW with a folded mask:** `flags &= ~4` on a `u8` member
  compiles to the byte constant `0xFB` where the C front end kept `-5`
  (`movs #5; negs`). Pass the mask as an `s32` (`ClearFlags(&f, ~4)`).
- **A file-local helper with the name of a C function** declared in an
  `extern "C"` header gets C linkage and is emitted out of line as a
  global, even when `static inline` (`IsPlayerDead` in player.h). Pick
  another name.
- **A struct assigned from a struct return** goes through a temporary
  and an `ldmia`/`stmia` copy, where an output-pointer call fills it in
  place. Where the ROM has the copy (`b = GetSpriteAttackBox_s(gPlayer)`
  over the body box, dingodile.cpp), call the struct-return alias; where
  it doesn't, declare the box at its first assignment, in the block that
  uses it (`struct aabb hit = GetSpriteHitbox(t)`), so it gets a slot of
  its own that later blocks reuse.
- **Cross-jumping needs separate tails.** Two cases that end in the same
  call sequence and `break` are merged after reload (the later one jumps
  into the earlier's copy); a case that falls through into the next one
  instead is not merged with them. `SetDingodileState`'s three
  `SetTargetAnim` cases share one `bl _call_via_r3` only when the third
  has its own `StartMotion(part, 0); break;` rather than falling into
  `case 2:`.
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
- **A loop whose body reloads a global's address** the ROM loaded in the
  test: gcc copies a simple exit test in front of the loop, so the body is
  entered by falling through it and CSE reuses the test's register. A
  test that calls an inline function (a block of its own) isn't copied
  (`DoSuperBodySlamShockwave`'s `CollidableCount()`; part 5b).
- **A constant the ROM keeps in a register across a loop** (`cmp r1, r8`
  where `cmp r1, #0x40` would do) is a variable set before the loop: CSE
  doesn't know its value at the loop's top (`range`, part 5b).
- **CSE following a jump into a block** can give the block's uses of a
  variable a copy in another register, which stops the ROM's
  cross-jump of two identical tails. A `do { ... } while (0)` around a
  call in the block (no code; it is a loop to gcc) stops the path there,
  as the C's `ACT_VCALL` macros did (`StateCrawl`, part 5b).
- **A variable initialised at its declaration and then tested** can
  give the test a copy of it (an extra `adds`); assigning it in the test,
  `if ((hit = x & 8) != 0)`, doesn't (`StateLeftGround`, part 5b).
- **A store the ROM schedules after a constant's load**: `flags.all |=
  0x80` let the scheduler move the next 0 into the `ldrb`'s slot, while
  the bitfield store `flags.bits.flag7 = 1` keeps the ROM's order
  (`StateWarpIn`, part 5b).
- **A `bool` argument passed on the stack is stored with `strb`;** a `u8`
  one is promoted and stored with `str`. Where the ROM stores a byte
  (`add r3, sp, #4; strb`), the parameter is a `bool` (`OpenLifeCrate`'s
  `DropExtraLife` call; part 7e).
- **A `u8` call result stored into a `u8` 4-bit bitfield** isn't
  zero-extended first, where the C's `u32` bitfield (and the ROM) did:
  assign it to a `u32` local first (`UpdateCrate`; part 7e).
- **Inline functions with an out-of-line copy in another file**: the ROM
  has `FindLineCrossing` inline the two line steppers, and the steppers'
  out-of-line copies at the end of crate.o. With `#pragma interface` no
  copy is emitted, and a definition at the top of crate.cpp would come
  first in its `.text`; a header fragment with no include guard
  (crate_line_step.hpp), `inline` through crate.hpp and plain at the end
  of crate.cpp, gives both from one source (part 7e).
- **An inline body the ROM also has out of line:** where one function
  is inlined into another and also exists on its own, make the body an
  inline method and have the out-of-line method call it
  (`CrateList::Append`/`AddNode`, `Detach`/`Remove`; part 7f). The header
  fragment of part 7e is for copies emitted at the end of the file.
- **A loop invariant the ROM computes after the loop's own:** an address
  computed before a loop comes before the ones loop.c hoists out of it, and
  as a different `add`. Assigned inside the loop, it is hoisted with them,
  after them (`CrateList::Draw`'s `last`; part 7f).
- **Reloads after a two-word copy:** where the ROM copies the position
  (`ldr; ldr; str; str`) and then loads `x` and `y` again, the copy is a
  block copy (`PrevPos() = Pos()`, part 7b): gcc reloads after it, where
  two field stores let CSE reuse the loaded values.
- **A C function whose first argument is the result's address**
  (`GetSpritePrevPos(dest, self)`) is a method returning a struct: g++
  passes the hidden result pointer in r0, before `this`.
- **A shared final store with gotos** (three exits jumping to one `strb`)
  can usually be written as a store and a `return` in each path:
  cross-jumping merges the copies into the ROM's shared tail
  (`GroundSprite::ProbeFloor`, part 7b).
- **A value the ROM computes again** instead of reusing a sum it just
  stored: give the sum a variable that is assigned again afterwards (the
  next axis's sum). CSE then has no register holding it and recomputes the
  expression (`ExtraLife::Update`, `Fly`, part 7h). Two `MATCH_USE`s per
  operand were the C's way to get the same registers.

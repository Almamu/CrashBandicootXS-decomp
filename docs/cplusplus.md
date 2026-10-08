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
That conversion is done: every game object with a C++ trait is C++
source since [the final cleanup](#the-final-cleanup).

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
| `operator new`/`delete`/`new[]`/`delete[]` | `OperatorNew` & co. in `src/level/camera.cpp` | the game's own replacements: see below |

**The operators.** In g++ 2.x the global `operator new(size_t)` has the
assembler name `__builtin_new`, and `new[]`, `delete` and `delete[]` are
`__builtin_vec_new`, `__builtin_delete` and `__builtin_vec_delete`. A
`new X` expression calls `__builtin_new` and then the constructor; a
`delete p` calls the destructor through the vtable with `__in_chrg` = 3.
libgcc's own versions (new1.cc, new2.cc) call `malloc` and the new
handler. The game's four, in camera.cpp, call `mem_alloc(size,
MEM_HEAP_EWRAM)` and `mem_free`: they are the game's replacement global
operators, and `OperatorNew` is `__builtin_new`. camera.cpp defines them
as `operator delete[]`, `operator new[]`, `operator delete` and `operator
new` (in ROM order), and cxx_symbols.txt gives them their C names. None of libgcc's C++
support (new handler, `__pure_virtual`, `__terminate`) is linked.

**Where the vtables are.** g++ 2.9 puts a vtable in a writable data
section (`.gnu.linkonce.d._vt.<class>`), not in `.rodata`. The 93 tables
are the last thing in the ROM before the IWRAM image, after all the
constant data: exactly where `.data` goes (the GBA build left `.data` in
ROM). The region is nothing but vtables (831 * 8 bytes = 0x19F8). Their
order follows the order of the classes' code: taking each table's
destructor as its class's marker, 85 of the 93 are in ascending code
order, which is what you get when each vtable is emitted with its class's
first virtual method. Since step 10b, g++ emits all 93 from the classes
([Emitting the vtables](#emitting-the-vtables-step-10), [the last C
vtables](#the-last-c-vtables-step-10b)).

**What is C.** The libraries under `lib/`: Shin'en's GAX2, Nintendo's
AgbEeprom SDK, libgcc and the BIOS wrappers ([libraries.md](libraries.md))
have none of the traits. The IWRAM ARM routines were built by an ARM
build of the same compiler; they are C++ source now too, built by the
ARM C++ compiler ([the IWRAM ARM code](#the-iwram-arm-code)). For the rest of the game, being "C-like" doesn't mean
it was C: the C++ front end compiles plain C-style code to the same bytes
as the C front end (80% of the game's files, see
[experiment 4](#4-every-game-file-compiled-unchanged-as-c)), so a file
with no C++ constructs can't be told apart.

## Survey: which objects are C++

`tools/cpp_survey.py` classifies each object by the C++ runtime
structures its C writes out by hand. Its output when #685 landed (after
the conversion, see [the final cleanup](#the-final-cleanup)):

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
| 3D actors (`struct actor_self`) | animation (a non-polymorphic base, `AnimPart`, 0x1C), position, state, links | +0x50 |

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
`Method__<len>Class<args>` (`Update__10EffectCtrlP12MovingSprite`), a
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
    virtual void Update(MovingSprite *part);
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg);
    virtual ~EffectCtrl(); // DestroyEffectCtrl
    void Reset();          // ResetEffectCtrl
};

void EffectCtrl::Update(MovingSprite *part)
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
own `SpriteObj`/`Entity` view has since become the real classes of
`include/entity.hpp` and `include/sprite_obj.hpp`, and the controllers
take a `MovingSprite *` since part 7b'.)

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
  [Progress](#progress)). A few C declarations have a C++ type: globals.h
  declares `gPlayer` as a `Player *` under `__cplusplus` (`extern class
  Player *gPlayer;`, still C linkage) and as a `struct player *` for C
  (part 8), and `gEntitySpawner` as an `EntitySpawner *` and a `struct
  entity_spawner *` (part 9); text.h `gSmallFont`/`gLargeFont` as `Font
  *`s and DrawWrappedText's and DrawWrappedTextInBox's font parameter as
  a `Font *`, cutscene.h `struct cutscene_player`'s `font` field, and
  level.h `gLevelLayersSingleton` as a `LevelLayers *` (step 10b).
- **Vtables are g++'s.** A class header has no `#pragma interface`, so
  g++ emits each class's vtable in its key-method object, as a weak
  symbol in a `.gnu.linkonce.d._vt.<len><Class>` section, and
  ldscript.txt places that section at the table's ROM address. A key-method
  object that would also get out-of-line copies of the class's inline
  methods the ROM doesn't have is in the Makefile's
  `NO_IMPLEMENT_INLINES_OBJS` (`-fno-implement-inlines`). See [Emitting
  the vtables](#emitting-the-vtables-step-10); until step 10 every header
  but entity.hpp had `#pragma interface` and all 93 tables were C, and
  until step 10b the 8 whose classes' key methods were C stayed C arrays
  (src/data/entity_vtables_7e3bec.c, gone).
  Headers with no polymorphic class (crate_list.hpp, menus.hpp,
  spawners.hpp) keep the pragma.
- **Names.** `cxx_symbols.txt` maps each mangled name the C++ objects
  define or use to its C name (`Update__10EffectCtrlP12MovingSprite
  UpdateEffectCtrl`, `_vt.10EffectCtrl gEffectCtrlVtable`, `__4Ctrl
  InitCtrl`). objcopy renames them after assembling, so the vtable data,
  the C callers, the linker script and the objdiff report see the C names.
  A missing or stale entry fails the link (the C side's reference stays
  undefined).
- **Style.** clang-format formats `.cpp`/`.hpp` too (`tools/format.py`);
  `.clang-format` puts `public:` at the class's indent.

asm labels (`void Update(MovingSprite *) asm("UpdateEffectCtrl");`) work for
methods and constructors, but not for destructors: g++ 2.9 then fails to
find the destructor (`no matching function for call to B::__dt`). That,
and the vtable names, is why the build renames with objcopy instead.

## Progress

The conversion goes one class family per PR (#664). Each row is an
object that is now built from C++ source; "workarounds" counts the
`MATCH_*` pins and `asm` statements of its old C (`tools/match_idioms.py`
counts them by kind) and what the C++ needed when it was converted.
#662 removed many of those since; `tools/match_idioms.py --functions
--files` lists the functions that still have one.

| Object | Classes (include/ctrl.hpp unless noted) | Functions | Compiler | Workarounds: C -> C++ | Part |
| include/objects.h, player.h, crates.h, gfx.h, bosses.h, pickups.h, frontend.h, actor.h, vehicle.h, system.h | the 391 C prototypes of C++ methods with no C caller go, with the C views only a class's size check used (struct ctrl, boss_ctrl, mega_mix_ctrl, part_ctrl and ctrl_anchor, periodic_spawner, spawner, actor_orbit, cannon_flash, actor_hp, jetpack_ring, orbit_part and orbit_part.h); the classes check the ROM sizes | 0 | (unchanged) | 0 -> 0 | cleanup |
| actor_self.h, box_part.h, match.h | `ACTOR_SET_STATE`, `CALL_HIT`, `MATCH_USE_VOLATILE`, `MATCH_CONST_VOLATILE` (no user left) go | 0 | (unchanged) | 0 -> 0 | cleanup |
| actor_self.hpp, `src/actor/actor_category_frame.cpp`, `src/vehicle/polar_nitro.cpp` | `BoxOverlap` and `WorldBox`, the two files' identical inlines, are actor_self.hpp's | 0 | (unchanged) | 0 -> 0 | cleanup |
| `src/actor/actor.cpp`, `actor_spawn.cpp`, `src/bosses/hovercraft.cpp`, `airship_explode.cpp` | the category hooks call `JetpackPlayer`'s and `PolarPlayer`'s `AllocTiles`, `FinishRun`, `IsPauseLocked`, `SetCheckpoint` directly; their C prototypes go (`CatchPolarPlayer` stays, for yeti_update.c) | 0 | (unchanged) | 0 -> 0 | cleanup |
| `src/menus/pause_menu_draw.cpp` | `PauseMenu::Animate`, `Draw`, `DrawRows` (menus.hpp) | 3 | agbcp | 2 pins, 2 holds, 3 uses (`MATCH_USE` x2, `MATCH_USE2`), 14 slot calls -> 2 pins, 2 holds, 3 uses | cleanup |
| `src/menus/pause_menu_gems.cpp` | `PauseMenu::DrawGemsPage`, `DrawRelicsPage` | 2 | old_agbcp | 0 -> 0; 4 slot calls -> 0 | cleanup |
| `src/menus/pause_menu_info.cpp` | `PauseMenu::InitInfo` | 1 | agbcp | 0 -> 0 | cleanup |
| `src/menus/pause_menu_loop.cpp` | `PauseMenu::Loop` | 1 | old_agbcp | 2 pins -> 2 pins | cleanup |
| `src/menus/pause_menu_pages_draw.cpp` | `PauseMenu::DrawTimeTrialPage`, `DrawCrystalsPage`, `DrawPageTitle`, `CommitFrame` | 4 | agbcp | 1 pin, 1 `asm`, 4 slot calls -> 0 | cleanup |
| `src/menus/pause_menu_powers.cpp` | `PauseMenu::DrawPowersPage` | 1 | **old_agbcp** (was agbcc) | 12 pins, 2 slot calls -> 0 | cleanup |
| `src/menus/pause_menu_widgets.cpp` | `PauseMenu::DrawFraction`, `VolumeDown`, `VolumeUp`, `CursorDown`, `CursorUp`, `FormatVolume` (`FormatVolumePercent`, its unused first argument is `this`), with `FormatDecimal` (C linkage) | 6 + 1 | agbcp | 1 pin, 3 slot calls -> 1 pin (`FormatDecimal`'s) | cleanup |
| `src/menus/power_dialog_loop.cpp` | `PowerDialog::Loop` | 1 | old_agbcp | 0 -> 0 | cleanup |
| `src/text/wrapped_text.cpp` | `DrawWrappedText` (C linkage) | 0 + 1 | old_agbcp | 1 pin, 1 hold, 2 uses, 5 slot calls -> the same pin, hold and uses | cleanup |
| `src/save/save_menu.cpp` | `SaveMenu` (include/save_menu.hpp): `MessageInput`, `CommitFrame`; with `CloseSaveMenu` (`delete gSaveMenu`), `OpenSaveMenu` (`new SaveMenu`) (C linkage) | 2 + 2 | agbcp | 0 -> 0; the destructor's direct `(.., 3)` call -> `delete` | cleanup |
| `src/save/save_menu_draw.cpp` | `SaveMenu::LinkExchange` (`new`/`delete` of the save transfer), `DrawMessageLines`, `DrawCancel`, `DrawYesNoPrompt`, `DrawSlotStats`, `DrawSlots`, `InitIcons` (`new UiSprite`) | 7 | old_agbcp (old_agbcc C already) | 3 pins, 1 hold, 1 use, 3 barriers -> 3 pins, 1 hold, 1 use; the font record-slot calls (`ICON_TEXT_CALL`, `_call_via_r1`), `OperatorNew`/`OperatorDelete` -> virtual calls, `new`/`delete` | cleanup |
| `src/save/save_menu_input.cpp` | `SaveMenu`'s constructor (`InitSaveMenu`), destructor (`DestroySaveMenu`), `Input`, the state handlers, `SaveToSlot`, `DrawMain`; with the save transfer's accessors and `RunSaveMenu` (C linkage) | 13 + 4 | agbcp (both match) | 16 pins, 4 asm (2 of them DrawMain's ROM transcription, 1 `.pool`) -> 0; 6 `PART_METHOD` slot calls -> `delete`/`Update()` | cleanup |
| `src/save/save_menu_ui.cpp` | `SaveMenu`'s `LoadBg`, `RefreshSlotSummaries`, `LoadData`, `SummarizeProgress`, `DrawEmptySlotLabel`, `DrawTitle`, `GetBlinkPalette`, the link transfer's begin/end, the per-state draws, `Draw`, `DeleteSlot` | 18 | **old_agbcp** (was agbcc) | 1 pin -> 0; `ICON_TEXT_CALL`s -> virtual calls | cleanup |
| `src/hud/hud_init.cpp` | `Hud` (include/hud.hpp): constructor (`InitHud`: `new HudPart[35]`), `ConfigureParts` | 2 | old_agbcp | 0 -> 0; the hand-written `OperatorNewArray` + count word + `InitHudPart` loop goes (3 bank stores through `SET_PART_BANK`, a retyped store) | cleanup |
| `src/hud/hud.cpp` | `Hud::Update` | 1 | agbcp | 1 pin -> 0 | cleanup |
| `src/hud/hud_boss_clock.cpp` | `Hud::UpdateBoss`, `UpdateClock` | 2 | old_agbcp | 0 -> 0 | cleanup |
| `src/hud/hud_lives.cpp` | `Hud::UpdateLives` | 1 | **old_agbcp** (was agbcc) | 34 pins, 5 `asm`, 1 volatile hold -> 0 | cleanup |
| `src/hud/hud_counters.cpp` | `Hud::UpdateCrates`, `UpdateWumpa`, `UpdatePercentCounters` | 3 | old_agbcp | 0 -> 0; the `getHp` slot call is `HpActor::GetHp` | cleanup |
| `src/hud/hud_slide.cpp` | `Hud`'s slides (`UpdateSlides`, the four `Show*`, `StepSlide`), `SetCrateTotal`, `IncCrateTotal`, destructor (`DestroyHud`: `delete[] parts`) | 9 | agbcp | 0 -> 0; the slot-10 destructor loop and `OperatorDeleteArray` go | cleanup |
| `src/gfx/sprite_pieces.cpp` | `SpriteRenderer::DrawPieces` (`DrawSpritePieces`, sprite_obj.hpp) | 1 | old_agbcp | 0 -> 0; the slot-11 call is `Sprite::GetPriority`, the `struct oam_part` view goes | cleanup |
| `src/gfx/affine_sprite_pieces.cpp` | `SpriteRenderer::DrawAffinePieces` (`DrawAffineSpritePieces`) | 1 | old_agbcp | 1 use -> 1 use; the slot-11 call is `Sprite::GetPriority`, the `struct affine_part` view goes | cleanup |
| `src/gfx/graphics_package.cpp` | `LoadGraphicsPackage`, the BG setup and sprite-box functions (C linkage) | 0 + 8 | old_agbcp | 0 -> 0; `OperatorNewArray`/`OperatorDeleteArray` -> `new u16[]`/`delete[]` | cleanup |
| globals.h, hud.h, part_list.hpp, objects.h, gfx.h, extra_life.cpp, wumpa_update.cpp, crate_break.cpp, player_event.cpp, sprite.cpp, sprite_anim.cpp | `gHud` is a `Hud *` to C++; the C++ callers' `ShowHud*(gHud)` are methods, the pieces' callers `SpriteRenderer` methods; struct hud_digit_part, hud_anim_record/data, HUD_CLAMP_FRAME and 15 dead prototypes go | 0 | (unchanged) | 0 -> 0 | cleanup |
| `src/level/tile_cache.cpp` | `TileCache` (include/bg_layer.hpp; derives from level.h's struct tile_cache): constructor, destructor; with `GetTerrainType` (C linkage) | 2 + 1 | old_agbcp (old_agbcc C already) | 0 -> 0 |  cleanup |
| `src/level/level_layers.cpp` (again) | `LevelLayers`: `new TileCache`, `delete tiles` (`InitTileCache(operator new(0x1064))`, `DestroyTileCache(tiles, 3)` before) | 0 | old_agbcp | 0 -> 0 | cleanup |
| `src/level/entity_flags.cpp` | `LevelEntityFlags` (include/spawners.hpp; derives from struct entity_flags; `EntityFlags` is taken by entity.hpp's union): `CountCrateEntities`, the bitmap accessors, constructor, destructor | 9 | **old_agbcp** (was agbcc) | 8 pins, 2 asm -> 6 pins, 1 asm (`SetEntityIdActivated`'s `ip`/`r2` block: no plain form found under either compiler) | cleanup |
| `src/level/spawn_pickups.cpp` (again) | `LevelState`'s constructor (InitLevelState, include/level_state.hpp; C linkage before); `new LevelEntityFlags` | 0 | old_agbcp | 0 -> 0 | cleanup |
| `src/level/level_query.cpp` | `UnusedLevelObject` (file-local): the unused destructor and empty constructor (`DestroyUnusedLevelObject`, `InitUnusedLevelObject`); the medal tallies and room selection (C linkage) | 2 + 15 | agbcp | 0 -> 0 | cleanup |
| `src/cutscene/slideshow.cpp` | `Slideshow` (new include/cutscene.hpp): `BeginSlide`, `Run` (UNUSED), `Skip`, `ShowPicture` | 4 | old_agbcp (old_agbcc C already) | 7 pins, 1 asm, 1 memory barrier -> 0 | cleanup |
| `src/cutscene/slideshow_display.cpp` | `Slideshow`'s `EndSlide`, destructor, `Reset`; with `SetSlideshowDispcnt` (C linkage) | 3 + 1 | agbcp | 1 pin -> 0 | cleanup |
| `src/cutscene/cutscene_player.cpp` (again) | `Slideshow`'s constructor; `CutscenePlayer` (cutscene.hpp): constructor, `Run`, destructor (all C linkage before) | 4 | old_agbcp | 0 -> 0 | cleanup |
| `src/level/game_frame.cpp` | `UpdateGameFrame` (C linkage): `new TitleScreen`/`delete`, `new LevelEntityFlags`/`delete`, `new Hud`/`delete gHud` and the Hud's methods | 1 | old_agbcp (old_agbcc C already) | 0 -> 0; the `(void *)` conversion of InitHud's result goes | cleanup |
| `src/level/level_cutscene.cpp` | `LevelState`'s destructor (DestroyLevelState, UNUSED); `PlayCutscene` (C linkage) | 1 + 1 | agbcp | 0 -> 0; `DESTROY_FONT`'s slot calls, the slot-6 `SetTileBase` call, 9 spelled-out destructor calls -> `delete`s and virtual calls | cleanup |
| `src/level/level_state.cpp` | the level state's 87 functions (C linkage): `GetLevelState`'s `new LevelState`, `ShowCompanyLogos`' `new CompanyLogos`/`delete` | 0 + 87 | agbcp | 50 pins, 4 asm -> 49 pins, 3 asm (`ShowCompanyLogos`' asm `bl InitCompanyLogos` and its r0 pin go; PackSaveData's and the rest stay) | cleanup |
| `src/level/room_frame.cpp` | `UpdateRoomFrame`, `SetupRoomBlend` (C linkage): the player's `IsOnScreen` and `Draw` virtual calls | 0 + 2 | old_agbcp (old_agbcc C already) | 0 -> 0; 2 hand-written slot calls (struct player_vtable) -> 0 | cleanup |
| `src/level/room_entities.cpp` | `SpawnRoomEntities` (C linkage): the links walk `Crate *`s (`GetBounds()->h`, `SetAbove`, `SetBelow`, `GetAbove`), `gEntitySpawner->Spawn` | 0 + 1 | old_agbcp (old_agbcc C already) | 1 pin -> 1 pin; the `height` slot call and the lk_vtable/lk_actor views -> 0 | cleanup |
| `src/level/run_room.cpp` | `RunRoom` (C linkage): `Player::ResetForRoom`, `IsNearCamera`, `Update`, `GetAnimPaletteSlot`, the controller's `SetMode`, the crates' `GetClassId`, `Platform::GetExitMirror` | 0 + 1 | old_agbcp (old_agbcc C already) | 2 pins, 2 holds, 2 uses, 1 asm label -> the same; `PMF_CALL` and the gl_* vtable structs (4 slot calls) -> 0 | cleanup |
| `src/level/play_room.cpp` | `PlayRoom` (C linkage): `new PartList`, `CrateList`, `Player`, `ActionCtrl`, `PlayerCtrl`, `InputCtrl`, `ctrl->Attach(pl)`, the `delete`s | 0 + 1 | **old_agbcp** (was agbcc) | 14 pins, 1 clobber -> 0; 3 attach slot calls, the player's destroy slot call, the widget vtable structs -> 0 | cleanup |
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
| `src/crates/crate_break.cpp` | `Crate` (include/crate.hpp): the player's hits (`QueuePlayerCollision`, `ApplyCollision`), the breaks, the kinds' contents, the explosions and blasts, the falls, the switches, the TNT countdown and the slot crate's tick; with `UpdateCrates`, `DetonateNitroCrates`, `BreakCratesInArea` (C linkage) | 22 + 3 | old_agbcp | 11 pins, 4 asm, 1 keep, 2 uses, 8 volatile casts, 2 asm labels, 6 `BOX_ADDR` -> 6 `BOX_ADDR` (and the two drop aliases, shared with crate_stack.cpp) | 7g |
| `src/objects/platform.cpp` | `Platform` (include/platform.hpp): `Update`, the exit facing, `GetClassId`, constructor, destructor, `ClearVulnerable`; `PlatformMover`: `Update`, `MovePlayer`, the four motion setters, constructor, destructor, `ClearActive` | 16 | **old_agbcp** (was agbcc) | 27 pins, 2 keeps, 1 asm, 1 asm label, an `ENTITY_SET_GONE_BIT_PINNED`, gotos -> 2 pins, 2 keeps | 7d |
| `src/objects/platform_contact.cpp` | `Platform::CheckPlayerContact` | 1 | **old_agbcp** (was agbcc) | 4 pins -> 0 | 7d |
| `src/objects/platform_collide.cpp` | `Platform::ResolveCollision` | 1 | old_agbcp | 2 pins, 2 holds, 2 uses, 1 keep, 1 asm label, gotos -> 1 keep, 1 asm label | 7d |
| `src/objects/platform_create.cpp` | `Platform::Create` | 1 | old_agbcp | 1 keep, 8 retyped stores, 8 volatiles, the `MOVER_NEW` cast -> 0 | 7d |
| `src/bosses/cortex.cpp` | `OneShotAnimCtrl`'s constructor and destructor (ctrl.hpp); `UnusedOneShotAnimCtrl`; `TinyCtrl`'s `StartHop`, destructor and constructor; `CortexBossCtrl::Update`, `SpawnCannon`, `SpawnTarget`; `CortexTargetCtrl::Update`, `SetState`, `FireShot`; `CortexShotCtrl`, `CortexBossGemCtrl` (boss_ctrl.hpp); `CortexBossPlatformMover` (platform.hpp); with `TinyHitStub` and `SpawnCortexBossGem` (C linkage) | 23 + 2 | old_agbcp | 49 pins, 13 keeps, 4 asm, 3 volatiles, 2 retyped stores, the `MOVER_NEW` cast, the per-site `SET_FRAME_R`/`MARK_GONE`/`GONE_SLOT_R4` macros and entity_bits.h's `ENTITY_SET_GONE_BIT_PINNED` (5 pins), gotos -> 1 pin, one goto | 7i |
| `src/objects/platform_create.cpp` (again) | `Platform::Create`: `new CortexBossPlatformMover` (the C prototype before) | 0 | old_agbcp | 0 -> 0 | 7i |
| the controller headers (ctrl.hpp, enemy_ctrl.hpp, input_ctrl.hpp, player_ctrl.hpp, action_ctrl.hpp, boss_ctrl.hpp, platform.hpp), sprite_obj.hpp, crate_list.hpp, and 38 `.cpp` files | every controller method takes a `MovingSprite *` (`SpriteObj` removed); `Ctrl::owner` a `MovingSprite *`; `PartList`'s items `Sprite *`s, `CrateList`'s `Crate *`s | 0 | (unchanged) | 0 -> 0 | 7b' |
| `src/player/player_update.cpp` | `Player` (include/player.hpp): `ApplyVelocity`, `Update`, `TouchesBox`, destructor, `HasRampYTarget`, `ClearSpeedY`, `StopFalling` | 7 | **old_agbcp** (was agbcc) | 14 pins, 1 asm, 2 retyped reads, gotos, the destructor's slot call -> 0 | 8 |
| `src/player/player_init.cpp` | `Player`'s constructor | 1 | agbcp | 1 pin, 1 retyped store -> 0 | 8 |
| `src/player/player_reset.cpp` | `Player::Reset`, `ResetForRoom` | 2 | **old_agbcp** (was agbcc) | 30 pins, 4 asm, gotos -> 0 | 8 |
| `src/player/player_collide.cpp` | `Player::CheckPlayerContact` | 1 | old_agbcp | 5 pins, 3 holds, 3 uses, 4 consts, three vcall macros -> 0 | 8 |
| `src/player/player_anim_room.cpp` | `Player::HasRoomForAnim` | 1 | agbcp | 0 -> 0; the slot call goes | 8 |
| `src/player/player_event.cpp` | `Player::TouchPlayer`, `HandleEvent`, `Draw` | 3 | old_agbcp | 1 pin, 2 holds, 2 uses, a volatile read, the `NOTIFY` macro and slot calls -> the volatile read | 8 |
| `src/player/player_flags.cpp` (again) | the 40 player accessors are `Player` methods (C linkage before) | 40 | agbcp | 3 pins, gotos -> 0 | 8 |
| `src/objects/collision_queue.cpp` (again) | `CollisionQueue`'s constructor (`ResetCollisionQueue`; `Reset` before) | 0 | agbcp | 0 -> 0 | 8 |
| globals.h, action_obj.h, the controller headers (action_ctrl.hpp, input_ctrl.hpp, player_ctrl.hpp), sprite_obj.hpp, and 43 `.cpp` files | `gPlayer` and the controllers' player pointers are `Player *`s; `PlayerSprite()`, `ActionCtrl::Sprite()` and the `(GroundSprite *)` casts go | 0 | (unchanged) | 4 hand-written vcalls, the flags2 offset macro -> 0 | 8 |
| `src/level/entity_spawner.cpp` | `EntitySpawner` (include/spawners.hpp): `SpawnEffectPart`, `LaunchEffectPart`, `DropWumpa`, `Spawn` (SpawnEntity), `SetTable`, constructor, destructor | 7 | old_agbcp | 4 pins, 1 use, 1 keep, 1 asm label, the `attach` slot call -> 0 | 9 |
| `src/level/drop_extra_life.cpp` | `EntitySpawner::DropExtraLife` | 1 | old_agbcp | 0 -> 0 | 9 |
| `src/level/spawn_gems.cpp` | the crystal and gem spawners (C linkage) | 6 | old_agbcp | 0 -> 0 | 9 |
| `src/level/spawn_gem_platforms.cpp` | the gem platform spawners (C linkage) | 4 | old_agbcp | 0 -> 0 | 9 |
| `src/level/spawn_crates.cpp` | the crate spawners (C linkage) | 7 | **old_agbcp** (was agbcc) | 16 pins, 3 asm -> 0 | 9 |
| `src/level/spawn_start_marker.cpp` | `SpawnStartMarker` (C linkage) | 1 | old_agbcp | 17 pins, 2 asm, 1 retyped read, the slot-13 call, gotos -> 0 | 9 |
| `src/level/spawn_bosses.cpp` | the room exit and the bosses' spawners (C linkage) | 4 | old_agbcp | 4 pins, 1 keep, 3 slot calls -> 4 pins, 1 keep | 9 |
| `src/level/spawn_objects.cpp` | Mega Mix, the decorations, the platform and crate spawners, the seal spawner (C linkage) | 25 | old_agbcp | the slot call -> 0 | 9 |
| `src/level/spawn_pickups.cpp` | the power, stopwatch, blue gem, wumpa and player-start spawners, `CreateEntitySpawner`, `DestroyEntitySpawner`, `InitLevelState` (C linkage), with `KeyInput` (spawners.hpp; its constructor ClearKeys is still C) | 18 | **old_agbcp** (was agbcc) | 38 pins, 10 asm, 5 retyped stores -> 4 pins, 1 asm | 9 |
| `src/level/time_trial.cpp` | `StartTimeTrial` (C linkage) | 1 | old_agbcp | 2 vcall macros, an `ENTITY_SET_GONE_BIT` -> 0 | 9 |
| globals.h, level.h, level_state.h, level_data.h, crate.hpp, enemy_ctrl.hpp, and 17 `.cpp` files | `gEntitySpawner` is an `EntitySpawner *` to C++, and the effect parts' and dropped pickups' callers call its methods; `CreateMovingSprite` calls are `MovingSprite::Create` | 0 | (unchanged) | 2 asm labels (`DropWumpaFlag`, `DropExtraLifeFlag`), a function-pointer cast -> 0 | 9 |
| `src/menus/level_select.cpp` | `CameraLead`, `LaunchPad`, and `LevelSelect`'s constructor, destructor, update, draw, record and loop (include/level_select.hpp), with `RunLevelSelect` (C linkage) | 24 + 1 | old_agbcp | 31 pins, 2 keeps, 2 asm, 2 asm labels, 1 volatile read, the byte views of DISPCNT, BLDY and the save record, the `Opaque` and `ItemAt` helpers, gotos, 7 hand-written vcalls -> 3 pins, 1 keep, 2 asm labels | 10 |
| `src/menus/level_select_pages.cpp` | `LevelSelect`'s page turns, exits and entry refresh; `LevelSelectPageBg`; `ZoomBg`'s constructor; with `SetNewWorldOpened` (C linkage) | 24 + 1 | old_agbcp | 0 -> 0; the entries' 2 function-pointer vcalls go | 10 |
| `src/menus/level_select_widgets.cpp` | `ZoomBg`, `LevelSelectEntry`, `LevelSelectCursor` | 41 | old_agbcp | 0 -> 0; `DELETE_PART` (the parts' slot-10 calls) goes | 10 |
| `src/player/input_ctrl.cpp` (again) | `InputCtrl::StateStart`: `new CameraLead`, `CollidableList()->Add`, `cameraLead->Reset()`; `Update` and `StateDead`: `MarkGone()` | 0 | old_agbcp | 2 `ENTITY_MARK_GONE`s -> 0 | 10 |
| `src/frontend/company_logos.cpp` | `CompanyLogos::DrawVvLogoPieces`, `LoadUniversalLogoBg`; `LogoActor`'s constructor, `Update`, `Draw` (include/frontend.hpp; `ActorSelf`, its base, in include/actor_self.hpp) | 5 | old_agbcp | 2 pins, 2 uses, 3 barriers -> 1 pin, 3 barriers | 10b |
| `src/frontend/language_select.cpp` | `CompanyLogos`'s constructor, destructor, `LoadAssetBuffered`; `LogoActor`'s destructor; `LanguageSelect::Run`, `Input`, `Draw`, `InitGraphics` | 8 | agbcp | 0 -> 0; a goto and the hand-written destructors' vtable stores, unlink and frees go | 10b |
| `src/frontend/language_select_setup.cpp` | `LanguageSelect`'s constructor, destructor, `LoadBg`, `Blink`, `CommitFrame`, `Open`, `Close` | 7 | **old_agbcp** (was agbcc) | 2 pins, a retyped store and read (the DISPCNT bytes) -> 0 | 10b |
| `src/level/spawn_enemies.cpp` | the 26 enemy spawners (C linkage): `new EnemyCtrl`, `hdr->Attach(part)` | 26 | old_agbcp | 22 pins, 6 asm, 6 uses, 1 hold, 1 keep, 1 mem use, 1 asm label, 52 `POPUP_ATTACH` slot calls -> 1 pin, 5 uses, 1 hold, 1 keep, 1 mem use (all `SpawnFlamethrowerLabAssistant`'s) | 9b |
| `src/frontend/starfield.cpp` | `Starfield` (include/frontend.hpp) | 7 | **old_agbcp** (was agbcc) | 16 pins, 5 `asm` -> 0 | 10c |
| `src/frontend/credits.cpp` | `ContinuePrompt`'s `Draw`, `Blink`, `CommitFrame`, destructor, `Run`; `Credits` (include/frontend.hpp) | 13 | old_agbcp | 4 pins, 1 use, 1 volatile keep, 1 barrier, a retyped store -> 1 pin, 1 use, 1 volatile keep, 1 barrier | 10c |
| `src/menus/pause_menu.cpp` | `PauseMenu::Run`, constructor, destructor (include/menus.hpp) | 3 | **old_agbcp** (was agbcc) | 25 pins, 3 `asm`, 1 `.pool` -> 0 | 10d |
| `src/menus/pause_menu_pages_init.cpp` | `PauseMenu`'s five `Init*Page`s | 5 | old_agbcp | `UPDATE_ICON_FRAME_NIBBLE` (4 pins, 1 `asm`) -> 0 | 10d |
| `src/menus/power_dialog.cpp` | `PowerDialog::Show`, constructor | 2 | **old_agbcp** (was agbcc) | 24 pins, 2 `asm`, 3 consts -> 0 | 10d |
| `src/menus/power_dialog_draw.cpp` | `PowerDialog`'s `Draw`, `Animate`, `CommitFrame`, destructor; the four `Show*Dialog`s and the save block's counts (C linkage) | 4 + 12 | **old_agbcp** (was agbcc) | 18 pins, 4 `asm`, 6 retyped reads -> 0 | 10d |
| `src/menus/continue_prompt_init.cpp` | `ContinuePrompt`'s constructor (include/frontend.hpp) | 1 | **old_agbcp** (was agbcc) | 13 pins, 4 `asm`, 1 keep -> 0 | 10d |
| `src/menus/continue_prompt.cpp` | `ContinuePrompt::InitGraphics`, `Loop` | 2 | old_agbcp | 1 use, 1 keep -> the same | 10d |
| `src/frontend/title_screen_init.cpp` | `TitleScreen`'s constructor, `LoadBg`, `LoadObjTiles`, `UpdateLogoPieces`, `DrawLogoPieces` (include/frontend.hpp) | 5 | old_agbcp | 18 pins, 5 `asm`, 12 per-field inline accessors -> 1 pin | 10c-2 |
| `src/frontend/title_screen.cpp` | `TitleScreen`'s `CheatInput`, `Run`, `CommitFrame`, `DrawMenuItem`, `Draw`, `HashCheatInput`, `ResetLogoPieces`, destructor; `CompanyLogos::Run`, `LoadVvLogoGraphics`, `InitVvLogoPieces`, `UpdateVvLogoPieces` | 12 | old_agbcp, **with strength reduction** (was `-fno-strength-reduce`) | 10 pins, 2 keeps, 5 uses, 1 const, 23 per-field inline accessors (12 of them copies of title_screen_init.c's), the hand-written vtable calls -> 1 pin, 5 uses, 1 const | 10c-2 |
| `src/actor/actor.cpp` | `ActorSelf` (include/actor_self.hpp): constructor (`InitActorPart`), destructor (`DestroyActor`), `Update`, `Draw`, `UpdateDepth`, `EnterState` (`SetActorState`), `GetRecordIndex`, `GetX`/`GetY`/`GetZ`, `GetWorldBox`, `IsVisible`; with the category hooks, `IsTouchingPlayer`, the collected spawns and the BG palette cycle (C linkage) | 12 + 13 | **old_agbcp** (was agbcc) | 34 pins, 7 `asm` (one of them all of `UpdateActorPaletteCycle`, with its `.pool`), 2 retyped stores, 1 retyped read, the `destroy` slot call -> 1 pin | 11a |
| `src/actor/actor_anim.cpp` | `AnimPart` (actor_self.hpp): `GetAnimFrameBaseOffset`, `GetAnimFrameAttr`, `GetAnimFrameData`, `SetAnim` (`SetActorAnim`); `HpActor`'s `GetHp`, `Damage`, `IsUnshootable`; 36 subclasses' destructors, and the checkpoint banners' and the jetpack explosion's methods (include/vehicle.hpp, include/boss_actors.hpp); 3 implicit destructors (C linkage) | 49 + 3 | **old_agbcp** (was agbcc) | 27 pins, 2 `asm`, 1 retyped store, 3 `destroy` slot calls -> 1 const | 11a |
| `src/vehicle/polar_player_dispatch.cpp` | `PolarPlayer::RunState` (include/vehicle.hpp): `(this->*stateFuncs[state])()` | 1 | agbcp | `ACTOR_PMF_CALL` -> 0 | 11a |
| `src/data/actor_pmf_17a6b8.cpp` | `PolarPlayer::stateFuncs`, the first pointer-to-member table in C++ (`&PolarPlayer::StateMount`, ...) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11a |
| `src/bosses/airship_fireball.cpp` | `AirshipFireball` (include/boss_actors.hpp): constructor (`CreateAirshipFireball`), `Update`, `Damage`, `IsUnshootable`, `RunState`, `StateExplode` | 6 | agbcp | 9 pins, 2 retyped stores, 2 `ACTOR_PMF_CALL`s, the `destroy` slot call -> 0 | 11i |
| `src/data/actor_pmf_17c2b8.cpp` | `AirshipFireball::stateFuncs` (split from actor_pmf_17c260.c, whose two tables are still C) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11i |
| `src/bosses/airship.cpp` | the airship (an `AnimPart`, gAirship): `SteerAirship`, `CreateAirship` (`new AnimPart`), `SpawnAirship`, `UpdateAirship`, `UpdateAirshipBg2` (C linkage) | 0 + 5 | agbcp | the inline `AllocActor`/`InitAnimPart` pair, the `_call_via_r0` call -> 0 | 11i |
| `src/bosses/airship_damage.cpp` | `DamageAirship` (C linkage) | 0 + 1 | agbcp | 9 pins, 1 `asm`, 2 retyped stores -> 0 | 11i |
| `src/bosses/airship_explode.cpp` | `AirshipStateExplode` (C linkage) | 0 + 1 | agbcp | 0 -> 0 | 11i |
| `src/bosses/airship_fall.cpp` | `AirshipStateFall` (C linkage) | 0 + 1 | agbcp | 8 pins, 1 `asm`, 2 retyped stores -> 0 | 11i |
| `src/bosses/airship_states.cpp` | `AirshipStateApproach`, `AirshipStateFireballs`, `AirshipStateCannon` (C linkage) | 0 + 3 | agbcp | 9 pins, 1 `asm`, 2 retyped stores -> 0 | 11i |
| `src/bosses/airship_graphics.cpp` | `ConvertAirshipTiles`, `UpdateAirshipFlashColor`, `AnimateAirshipPalette` (C linkage) | 0 + 3 | agbcp | 1 const, 1 memory keep -> the same | 11i |
| `src/bosses/airship_load_graphics.cpp` | `LoadAirshipGraphics` (C linkage) | 0 + 1 | agbcp | 0 -> 0 | 11i |
| `src/bosses/airship_map.cpp` | `DrawAirshipMap` (C linkage) | 0 + 1 | old_agbcp (old_agbcc C already) | 0 -> 0 | 11i |
| `src/bosses/airship_touch.cpp` | `IsTouchingAirship` (C linkage) | 0 + 1 | old_agbcp (old_agbcc C already) | 0 -> 0 | 11i |
| `src/actor/actor_factory.cpp` | `PolarPlayer`'s constructor (`ConstructActorPart`, include/vehicle.hpp), with `CreateActor`, `SpawnActor`, `ConstructAnimTableState`, `CreatePolarCheckpointText`, `SpawnPolarCollectedWumpa`, `SpawnPolarAkuAku` (C linkage): the polar actors' `new`s, the crates', wumpa's, riderless polar's and checkpoint banner's constructors inline | 1 + 6 | agbcp | 0 -> 0; 3 macros of hand-written `new`s (allocation, base constructor, vtable store), the `REC_AT` index cast and `AllocActor` go | 11b |
| `src/actor/actor_spawn.cpp` | the category hooks and spawn accessors (C linkage): `DestroyAllActors` (`delete`), `CanPauseActorCategory`, ... | 0 + 16 | agbcp | 3 pins, 3 `asm` -> 1 pin | 11b |
| `src/actor/actor_category_frame.cpp` | `RunActorCategoryFrame` (the `Update`/`Draw` virtual calls), `FindShotTarget` (`IsUnshootable`), `PolarIsTouchingPlayer`, `JetpackIsTouchingPlayer` (C linkage) | 0 + 4 | old_agbcp | 0 -> 0; the slot-offset structs, the explicit `MemCopy32` self-copies and the frame struct go | 11b |
| `src/actor/actor_category_select.cpp` | `SelectActorCategory` (C linkage) | 0 + 1 | agbcp | 1 use -> 1 use | 11b |
| `src/actor/actor_anim.cpp` (again) | `PolarCrate`'s destructor is inline (vehicle.hpp); `DestroyPolarCrate` is its out-of-line copy, with C linkage | 0 + 1 | old_agbcp | 0 -> 0 | 11b |
| `src/vehicle/jetpack_spawn.cpp` | `JetpackPlayer` (include/vehicle.hpp): constructor (`InitJetpackPlayer`), `Update`, `Draw`, `Damage`, `SteerY`, `SteerX`, `StateFly`, `StateRollLeft`, `StateRollRight`; the jetpack spawners, `CreateJetpackActor`, `YetiStateStop` (C linkage): `new JetpackPlayer`, `new JetpackShot`, `new AirshipFireball`, the checkpoint banner's and explosion's inline constructors | 9 + 16 | old_agbcp | 4 pins, 2 retyped stores, 1 retyped read, `ACTOR_PMF_CALL`, 10 `ACTOR_SET_STATE`s -> 0 | 11e |
| `src/vehicle/jetpack_player.cpp` | `JetpackPlayer`'s `DispenseWumpa`, `CountBomber`, `GetHp` (`GetJetpackPlayerHpPercent`), `SetCheckpoint`, `IsPauseLocked`, `AnimatePalette`, `Heal`, `QueueWumpa`, `StateResume`, `StateBoost`, `StateFall`, `StateFinish`, `StateEnter`, destructor, `RunState`; with `IsJetpackPlayerInactive` (C linkage) | 15 + 1 | agbcp | 15 pins, 6 retyped stores, 3 retyped reads, `ACTOR_PMF_CALL`, the hand-written destructor -> 0 | 11e |
| `src/vehicle/jetpack_run.cpp` | `JetpackPlayer::FinishRun`, `PassRing`, `AllocTiles` | 3 | **old_agbcp** (was agbcc) | 32 pins, 4 `asm`, 4 retyped stores -> 0 | 11e |
| `src/vehicle/jetpack_shot.cpp` | `JetpackShot` (include/vehicle.hpp): `Update`, constructor (`CreateJetpackShot`), `IsUnshootable` | 3 | agbcp | 1 pin, 2 `asm`, 3 slot calls and gotos -> 0 | 11e |
| `src/data/actor_pmf_17c1c0.cpp` | `JetpackPlayer::stateFuncs` (gJetpackPlayerStateFuncs) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11e |
| `src/vehicle/polar_player.cpp` | `PolarPlayer` (include/vehicle.hpp): `Update`, `Draw`, `Hurt`, `Shock`, `AllocTiles`, `StateMount`, `StateRun`, `StateJump`, `StateDash`, `StateShocked`, `StateCaught` | 11 | old_agbcp (old_agbcc C already) | 38 pins, 14 retyped stores, 1 retyped read, `ACTOR_PMF_CALL`, 5 `ACTOR_SET_STATE`s, the `destroy` slot call, 7 gotos -> 0 | 11c |
| `src/vehicle/polar_player_states.cpp` | `PolarPlayer`'s `DispenseWumpa`, `IsPauseLocked`, `StateRecover`, `StateFinishLeap`, `StateCarriedOff`, `StateKnockedOff`, `StateBoost` | 7 | agbcp | 11 pins, 4 retyped stores, 2 retyped reads -> 0 | 11c |
| `src/vehicle/polar_player_actions.cpp` | `PolarPlayer`'s `StateLaunched`, `StateFinish`, `StateLand`, `FinishRun`, `Catch`, `QueueWumpa`, `GiveLife`, `Boost`, `GiveMask`, `Launch`, destructor | 11 | agbcp | 40 pins, 1 `asm`, 12 retyped stores, 6 retyped reads, the hand-written destructor -> 0 | 11c |
| `src/vehicle/jetpack_plane.cpp` | `JetpackPlane`, `JetpackBomber`, `JetpackCannonball` (include/vehicle.hpp): constructors (`CreateJetpackPlane`, ...), `Update`, `Damage`, `IsUnshootable`, `Aim`, `Home`, `RunState`s, the 11 states; `AirshipFireball::StateOrbit` and `StateSpiralIn` (include/boss_actors.hpp) | 28 | agbcp (both match) | 14 pins, 4 `ACTOR_PMF_CALL`s, 9 `ACTOR_VCALL`s, 14 `ACTOR_SET_STATE`s, 2 explicit `__divsi3` calls -> 0 | 11f |
| `src/vehicle/jetpack_balloon.cpp` | `JetpackBalloon` (include/vehicle.hpp): constructor (`CreateJetpackBalloon`), `Update`, `Damage`, `IsUnshootable`, `ClearCrate`, `Release`, `Move`, `RunState`, 3 states; `GetAirshipHpPercent`, `DestroyAirship` (`delete gAirship`), `nullsub_30`, `AirshipStateInactive` (C linkage) | 11 + 4 | agbcp (both match) | 7 pins, 2 retyped stores, 3 retyped reads, `ACTOR_PMF_CALL`, `ACTOR_VCALL`, a hand-written slot-7 call, `ACTOR_SET_STATE`, a `goto` -> 0 | 11f |
| `src/data/actor_pmf_17c260.cpp` | `JetpackPlane::stateFuncs`, `JetpackBomber::stateFuncs` (gJetpackPlaneStateFuncs, gJetpackBomberStateFuncs) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11f |
| `src/data/actor_pmf_17c414.cpp` | `JetpackBalloon::stateFuncs` (gJetpackBalloonStateFuncs; split from actor_state_17c3fc.c, with the balloon crate's table after it in the new actor_pmf_17c42c.c, still C) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11f |
| `src/bosses/hovercraft.cpp` | `JetpackRing` (include/vehicle.hpp): constructor (`CreateJetpackRing`), `IsUnshootable`; `JetpackCollectedWumpa` (new): `Update`, `Draw`, destructor, constructor, `IsUnshootable`; `HovercraftFireball` (include/boss_actors.hpp): `Damage`, `Update`, constructor, `StateExplode`, `StateFly`, `RunState`, `IsUnshootable`; the hovercraft (an `AnimPart`, gHovercraft): `UpdateHovercraftHitFlash`, `RunHovercraftState`, its states, `DrawHovercraftMap`, `CreateHovercraft` (`new AnimPart`), `SpawnHovercraft`, `UpdateHovercraft`, `UpdateHovercraftBg2`, `LoadHovercraftGraphics`, `ConvertHovercraftTiles`, `DestroyHovercraft` (`delete`), 3 unused stubs (C linkage) | 14 + 16 | old_agbcp (old_agbcc C already) | 28 pins, 1 const, 2 keeps, 1 memory keep, 4 retyped stores, 1 retyped read, 3 `ACTOR_PMF_CALL`s, 3 slot calls, the inline `AllocActor`/`InitAnimPart` pair -> 4 pins, 1 const, 1 memory keep | 11h |
| `src/bosses/hovercraft_parts.cpp` | the hovercraft's getters, `StartHovercraftHitFlash`, `SetHovercraftFlashColor`, `LoseHovercraftPart`, `SetHovercraftState` (the inline `EnterHovercraftState` out of line), 3 states (C linkage) | 0 + 14 | agbcp | 22 pins, 2 `asm`, 4 retyped stores -> 1 pin | 11h |
| `src/bosses/hovercraft_cannon.cpp` | `HovercraftCannon` (include/boss_actors.hpp): `StateFire`, `Damage`, `Update`, constructor (`CreateHovercraftCannon`), `StateDestroyed`, `StateWait`, `RunState`, `IsUnshootable` | 8 | agbcp | 15 pins, 1 const, 4 retyped stores, 1 retyped read, 2 `ACTOR_PMF_CALL`s, the hand-written destroy-slot call -> 0 | 11h |
| `src/bosses/hovercraft_launcher.cpp` | `HovercraftLauncher` (include/boss_actors.hpp): the same eight methods as the cannon's | 8 | agbcp | 20 pins, 3 consts, 7 retyped stores, 2 `ACTOR_PMF_CALL`s, the hand-written destroy-slot call -> 0 | 11h |
| `src/bosses/hovercraft_side_gun.cpp` | `HovercraftSideGun` (include/boss_actors.hpp): constructor (`CreateHovercraftSideGun`), `Damage`, `Update`, `RunState`, `IsUnshootable`; `HovercraftCannonFlash::Damage` | 6 | agbcp | 20 pins, 2 `asm` (one with a `.pool`), 4 retyped stores -> 0 | 11h |
| `src/bosses/hovercraft_cannon_flash.cpp` | `HovercraftCannonFlash` (include/boss_actors.hpp): `Update`, constructor (`CreateHovercraftCannonFlash`), `RunState`, `IsUnshootable` | 4 | agbcp | 5 pins, 1 keep, 2 retyped stores, 2 hand-written destroy-slot calls -> 1 keep | 11h |
| `src/data/actor_pmf_17c450.cpp` | `HovercraftFireball::stateFuncs` (gHovercraftFireballStateFuncs) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11h |
| `src/data/actor_state_17c4c8.cpp` | gHovercraftStateFuncs (a plain function table, C linkage), `HovercraftCannon::stateFuncs`, `HovercraftLauncher::stateFuncs` | data | agbcp | the `ACTOR_PMF` records -> 0 | 11h |
| `src/vehicle/jetpack_spawn.cpp` (again) | the hovercraft's weapons', the ring's and the collected wumpa's spawners: `new` | 0 + 7 | old_agbcp | the `CreateHovercraftSideGun_b` alias, the `byte_arg` and 8 C-constructor calls on `AllocActor` -> 0 | 11h |
| `src/vehicle/polar_pickups.cpp` | `PolarCollectedWumpa` (include/vehicle.hpp): `Update`, `Draw`, destructor, constructor; `PolarWumpa`'s `Update` and out-of-line constructor; `PolarCrate::Update`; `PolarQuestionCrate`'s, `PolarLifeCrate`'s and `PolarNitroCrate`'s `Update`; `IsPolarPlayerInactive` (C linkage) | 10 + 1 | agbcp (both match) | 59 pins, 2 `asm`, 18 retyped stores, 7 retyped reads, 4 hand-written `destroy` slot calls, 13 gotos -> 0 | 11d |
| `src/vehicle/polar_nitro.cpp` | `PolarNitroCrate::DetonateNearby` | 1 | old_agbcp (old_agbcc C already; agbcp doesn't match) | the `ACTOR_TYPE` cast, the frame struct and its two explicit `MemCopy32` self-copies -> 0 | 11d |
| `src/vehicle/polar_crates.cpp` | `PolarCrate`'s constructor (InitPolarCrate); `PolarAkuAkuCrate`'s, `PolarTimeCrate`'s, `PolarFourWumpaCrate`'s and `PolarBasicCrate`'s `Update`; `PolarNitroCrate::Detonate`; the 7 crate kinds' out-of-line constructors (include/polar_crate_ctors.hpp) | 13 | agbcp (both match) | 21 pins, 19 retyped stores, 5 retyped reads -> 0 | 11d |
| `src/vehicle/polar_objects.cpp` | `PolarElectricFence`, `PolarObstacle`, `PolarLauncher`, `PolarPenguin`, `PolarIcicle`: `Update` and constructor (and `PolarPenguin::Aim`); `PolarAkuAku`'s `Refresh`, `Update`, `Move` | 14 | agbcp (both match) | 56 pins, 1 `asm`, 24 retyped stores, 1 retyped read, `ACTOR_VCALL` and a hand-written slot call, 9 gotos -> 0 (`Move`'s `goto` stays) | 11d |
| `src/vehicle/polar_aku_aku.cpp` | `PolarAkuAku`'s `ClearMask`, `RemoveMask`, `AddMask`, `SetMask`, constructor; `PolarGoal`, `PolarBoostPad`, `PolarCheckpointCrate`: `Update` and constructor; `GetPolarMaskLevel` (C linkage) | 11 + 1 | agbcp (both match) | 9 pins, 4 retyped stores, a hand-written slot call -> 0 | 11d |
| `src/vehicle/jetpack_crates.cpp` | `JetpackBalloonCrate` (include/vehicle.hpp): `Update`, `Damage`, `Break`, `IsUnshootable`, the destructor, the out-of-line constructor (`InitJetpackBalloonCrate`, unused), `ClearBalloon`, `RunState` (unused), 3 states; `JetpackHealthCrate`, `JetpackTimeCrate`, `JetpackQuestionCrate`: constructors (`CreateJetpack*Crate`), `Update`, `Damage`; `JetpackParachuteNitro`, `JetpackRocket`: constructors, `Update`, `Damage`, `IsUnshootable`, `JetpackRocket::Launch`; `JetpackRing::Update` | 30 | agbcp (both match) | 62 pins, 3 `asm`, a file-scope `asm` literal pool, 30 retyped stores, 19 retyped reads, 2 `ACTOR_PMF_CALL`s, 2 `ACTOR_VCALL`s, 5 hand-written slot calls, the `goto` dispatch chains, a `__divsi3` call -> 0 | 11g |
| `src/data/actor_pmf_17c42c.cpp` | `JetpackBalloonCrate::stateFuncs` (gJetpackBalloonCrateStateFuncs) | data | agbcp | the `ACTOR_PMF` records -> 0 | 11g |
| `src/vehicle/jetpack_spawn.cpp` (again) | the crates', the parachute nitro's and the rocket's spawners: `new` | 0 + 1 | old_agbcp | 6 C-constructor calls on `AllocActor` (and `AllocActor`) -> 0 | 11g |
| `src/system/boot.cpp` | `Ctrl::Update` (`UpdateCtrl`), with `DivMod` and `MemCopy32` (C linkage) | 1 + 2 | agbcp | 3 pins, 1 asm -> the same (`DivMod`'s SVC) | step 10b |
| `src/cutscene/cutscene_player.cpp` | `BgStreamer` and `BgLayerBase`'s constructor, destructor and scroll steps (include/bg_layer.hpp), with the cutscene player's 4 functions (C linkage) | 21 + 4 | old_agbcp | 0 -> 0; 4 vtable stores, 4 slot calls -> 0 | step 10b |
| `src/level/bg_layer_base.cpp` | `BgLayerBase`'s `Scroll`, `Reset`, `SetSource` and accessors, with the terrain tile cache's lookups (C linkage) | 10 + 5 | old_agbcp | 1 use -> 1 use | step 10b |
| `src/level/bg_layer_init.cpp` | `BgLayer`'s constructor, `GrowRows`, `GrowColumns`, `ClipColumns`, `ClipRows` | 5 | **old_agbcp** (was agbcc) | 9 pins, 6 asm, a file-scope asm pool -> 0 | step 10b |
| `src/level/bg_layer.cpp` | `BgLayer`'s other methods, and the out-of-line copies of its inline ones (`GetScreenIndex` ... the destructor) | 19 | old_agbcp | 0 -> 0; 6 slot calls -> 0 | step 10b |
| `src/level/pooled_bg_layer.cpp` | `PooledBgLayer`'s overrides, `ReleaseColumn`, `ReleaseRow` (split from bg_layer.c), with `nullsub_26` | 9 + 1 | old_agbcp | 0 -> 0 | step 10b |
| `src/level/tile_slot_pool.cpp` | `PooledBgLayer`'s constructor, destructor, `GetPriority`, with the tile-slot pool (C linkage) | 3 + 5 | **old_agbcp** (was agbcc) | 4 pins, 3 asm -> 2 pins, 1 asm | step 10b |
| `src/level/level_layers.cpp` | `LevelLayers` (bg_layer.hpp), with `sub_80269DC`, `sub_80269F8`, `sub_8026A14` (C linkage) | 8 + 3 | agbcp | 0 -> 0; 6 slot calls -> 0 | step 10b |
| `src/text/font.cpp` | `Font` (include/font.hpp): `MeasureText` (font_measure.c), `UploadTiles`, `SetPalette`, `ResetPalette`, and the out-of-line copies of its inline constructor, accessors and destructor | 15 | old_agbcp (font.c was agbcc) | 5 pins, 2 keeps, 2 asm, 1 asm label -> 1 asm label | step 10b |
| `src/text/font_glyph.cpp` | `Font::DrawGlyph`, `PutChar`; `SmallFont`'s and `LargeFont`'s constructors | 4 | old_agbcp | 8 pins, 3 asm, gotos -> 0 | step 10b |
| `src/text/font_draw_text.cpp` | `Font::DrawText`, `MeasureChars` | 2 | old_agbcp | 3 pins, gotos -> 0 | step 10b |
| `src/text/font_draw_chars.cpp` | `Font::DrawChars` | 1 | agbcp | 0 -> 0 | step 10b |
| `src/text/font_height.cpp` | `Font::TextHeight` | 1 | agbcp | 0 -> 0 | step 10b |
| `src/util/aabb_setup.cpp` | `LargeFont`'s and `SmallFont`'s destructors, with `SetAabbSize`, `SetAabbPos`, `GetLives` (C linkage) | 2 + 3 | agbcp | 4 asm -> 0 | step 10b |
| text.h, cutscene.h, frontend.hpp, level_select.hpp, and 11 `.cpp` files | `gSmallFont`/`gLargeFont` are `Font *`s to C++; the fonts' callers make virtual calls and use the inline accessors | 0 | (unchanged) | 31 spelled-out slot calls -> 0 | step 10b |
| `src/audio/audio.cpp` | `AudioContext` (new include/audio.hpp; derives from audio.h's struct audio_context): its 22 methods, constructor (`InitAudioContext`), destructor (`DestroyAudioContext`), `DisableVCountIrq`; with `EnableMusicVCountIrq`, `MusicVCountIrqHandler` (C linkage) | 25 + 2 | old_agbcp (old_agbcc C already) | 2 pins, 1 use, 1 volatile cast -> 1 volatile cast | [audio](#the-audio-context) |
| globals.h, audio.h, 75 `.cpp` files | `gAudioContext` is an `AudioContext *` to C++; the callers' `PlaySfx(gAudioContext, ...)` & co. are method calls, LevelState's constructor and destructor `new AudioContext` and `delete gAudioContext`; the two `PlayAmbientSfx` callers pass a `bool` (the C's `struct byte_arg`) | 0 | (unchanged) | 0 -> 0 | [audio](#the-audio-context) |
| `src/gfx/bitmap_screen.cpp`, `fade_to_black.cpp` | `ShowBitmapScreen`; `FadePaletteToBlack`, `IsBrightnessFadeActive` (C linkage) | 0 + 3 | agbcp | 0 -> 0 | all-C++: gfx, system, util, text |
| `src/gfx/display.cpp`, `fade.cpp` | the DISPCNT helpers (`SetDispcntMode` ... `CommitDispcnt`); `StepBrightnessFade`, `FadeBrightness`, `DarkenPalette` (C linkage) | 0 + 17 | old_agbcp | 0 -> 0 | all-C++: gfx, system, util, text |
| `src/gfx/sprite_frame.cpp` | the OBJ tile allocator, the overflow OAM queue and the sprite frame cache (C linkage); FlushSpriteFrameOamQueue calls `gOamBuffer->Append`, `HideUnused`, `SetAffineScales` (sprite_obj.hpp), whose C prototypes go | 0 + 18 | old_agbcp | 0 -> 0 | all-C++: gfx, system, util, text |
| `src/system/asset.cpp`, `main.cpp`, `main_loop.cpp`, `memory.cpp` | `LoadTaggedAsset`, `LoadBackgroundTileAndPalette`; `AgbMain`; `MainLoop`, `GetUiText`; the heap (`mem_*`, C linkage) | 0 + 12 | agbcp | 1 file-scope asm -> 1 (`mem_walk_heaps`) | all-C++: gfx, system, util, text |
| `src/system/input.cpp`, `irq.cpp` | `WaitForKeyPress`; the IRQ table, the VBlank handler and callbacks, the key reading (C linkage), and `KeyInput`'s constructor (`ClearKeys`, spawners.hpp), whose C prototype goes | 1 + 17 | old_agbcp | 1 pin -> 1 pin | all-C++: gfx, system, util, text |
| `src/text/text_box.cpp` | `GetWordLength`, `DrawWrappedTextInBox` (C linkage; takes a `Font *`, `self->SetMargin`, `HeightToLines`) | 0 + 2 | agbcp | 0 -> 0 | all-C++: gfx, system, util, text |
| `src/util/aabb.cpp` | `CommitBlendRegs`, `AabbOverlapsInclusiveX`, `AabbOverlaps`, `IwramFree`, `IwramAlloc` (C linkage) | 0 + 5 | old_agbcp | 0 -> 0 | all-C++: gfx, system, util, text |
| `src/util/fixed_math.cpp`, `line.cpp`, `line_step.cpp`, `number_format.cpp`, `printf.cpp`, `rand.cpp`, `string.cpp`, `time_format.cpp` | the fixed-point helpers, the Bresenham line, `itoa`, `sprintf`/`vsprintf`/`FindSubstring`, `rand`/`srand`/`RandRange`, the string functions, `FormatCentiseconds` (C linkage) | 0 + 24 | agbcp | 2 pins, 5 asm, 2 asm labels -> the same | all-C++: gfx, system, util, text |
| `src/actor/actor_bg.cpp` | none (C linkage): the category BG scroll and shake | 0 + 7 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/actor/actor_category_init.cpp` | none (C linkage): `InitActorCategory`, with the pause round trip as an inline function | 0 + 1 | old_agbcp (old_agbcc C already) | 1 asm label -> 1 asm label; the Hud, OamBuffer and ObjVramCursor calls are methods | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/actor/actor_category_stats.cpp` | none (C linkage) | 0 + 4 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/actor/actor_vram_pool.cpp` | none (C linkage): `SetupActorVramPool` calls PaletteCache's and Hud's methods | 0 + 1 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/actor/bg_picture.cpp` | none (C linkage) | 0 + 2 | old_agbcp (old_agbcc C already) | 2 uses -> 2 uses | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/actor/cell_anim.cpp` | none (C linkage) | 0 + 16 | agbcp | 1 asm label -> 1 asm label | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/vehicle/yeti.cpp` | none (C linkage): `CreateYeti` is `new AnimPart`, `DestroyYeti` `delete gYeti` | 0 + 5 | agbcp | 0 -> 0; the hand-written IWRAM allocation and `SetActorAnim` call go | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/vehicle/yeti_graphics.cpp` | none (C linkage) | 0 + 2 | old_agbcp (old_agbcc C already) | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/vehicle/yeti_states.cpp` | none (C linkage): AudioContext's `PlaySfx`/`PlayAmbientSfx`, `AnimPart::RestartAnim` | 0 + 2 | agbcp | 1 barrier, 1 asm label -> 1 barrier | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/vehicle/yeti_update.cpp` | none (C linkage): `PolarPlayer::Catch`, `AnimPart::GetAnimFrameBaseOffset`/`RestartAnim` | 0 + 3 | old_agbcp (old_agbcc C already) | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/bonus_round.cpp` | none (C linkage) | 0 + 2 | agbcp | 1 pin -> 1 pin | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/camera.cpp` | the camera (C linkage) and the global `operator new`, `new[]`, `delete`, `delete[]` | 0 + 8 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/collision_map.cpp` | none (C linkage) | 0 + 6 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/room.cpp` | none (C linkage): the PaletteCache, ObjVramCursor, OamBuffer and LevelLayers calls are methods | 0 + 5 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/terrain.cpp` | none (C linkage) | 0 + 5 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/terrain_probe.cpp` | none (C linkage) | 0 + 1 | agbcp | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/level/terrain_probe_axes.cpp` | none (C linkage) | 0 + 2 | old_agbcp (old_agbcc C already) | 0 -> 0 | [everywhere](#c-everywhere-actor-vehicle-level) |
| `src/link/link_handshake.cpp`, `link_session.cpp`, `link_session_reset.cpp`, `link_sio.cpp` | none: free functions over `struct link_session`, C linkage | 2 + 2 + 1 + 7 | old_agbcp (the first three, old_agbcc C already; link_session_reset.o keeps `-fno-rerun-loop-opt`), agbcp | 0 -> 0 | all-C++: link, save, iwram |
| `src/save/save_data.cpp`, `save_transfer.cpp`, `save_transfer_poll.cpp` | none: free functions, C linkage; LoadSaveData and StoreSaveData call `AudioContext`'s `IsPlaying`, `GetCurrentSong`, `StopSong`, `PlaySong` | 15 + 3 + 1 | old_agbcp, old_agbcp, agbcp | 1 instruction asm (PollSaveTransfer, with its pin) -> 0, with `volatile` on `struct link_session`'s `playerId` | all-C++: link, save, iwram |
| `src/iwram/iwram_data.cpp` | the IWRAM image's initialised globals; `gSaveMenu`, `gLevelSelect`, `gLevelLayersSingleton`, `gActorList`, `gLanguageSelect` and `gHeapSortActorsByKeyFunc` defined with their C++ types | 0 | agbcp | 0 -> 0 | all-C++: link, save, iwram |
| `src/iwram/string_arm.cpp`, `sprite_arm.cpp` | none: the IWRAM image's ARM routines, C linkage; HeapSortActorsByKey takes `ActorSelf **` | 5 + 5 | agbcp_arm_patched (new; agbcc_arm_patched C) | 0 -> 0 | all-C++: link, save, iwram |
| `src/level/tile_slot_pool.cpp` (again) | `TileSlotPool` (include/bg_layer.hpp; was the file-local struct tile_slot_pool): `Reset`, `Acquire`, `Release`, `Upload`, `SetSource` (the C names stay), its five `static inline` helpers private inline methods | 5 | old_agbcp | 0 -> 0 | [#752](#the-tile-slot-pool-the-tile-cache-and-the-bg-setup-752-753) |
| `src/level/bg_layer_base.cpp`, `collision_map.cpp`, `tile_cache.cpp` (again) | `TileCache` (include/bg_layer.hpp; level.h's struct tile_cache's fields move into it): `GetChunk` (GetCollisionChunk), `GetTerrainHeights`, `GetSolidTerrainHeights`, `GetSolidTerrainModeValue`, `DecodeChunk` (DecodeCollisionChunk); `GetCell` (GetCollisionCell), `SetSource` (SetCollisionSource); `GetTerrainType`; the two files' `GetCell` inline is the private `CellAt` | 5 + 2 + 1 | old_agbcp, agbcp, old_agbcp | 1 use -> 1 use (DecodeChunk's `MATCH_USE(n)`) | [#752](#the-tile-slot-pool-the-tile-cache-and-the-bg-setup-752-753) |
| `src/level/pooled_bg_layer.cpp`, `level_layers.cpp`, `terrain.cpp`, `terrain_probe_axes.cpp` (again) | callers: `pool->Acquire(...)`, `tiles->SetSource(...)`, `tiles->GetTerrainType(...)` & co.; level.h's struct level_layers holds a `TileCache *` for C++ | 0 | (unchanged) | 0 -> 0 | [#752](#the-tile-slot-pool-the-tile-cache-and-the-bg-setup-752-753) |
| `src/gfx/graphics_package.cpp` (again) | `BgSetup` (new include/graphics_package.hpp; was graphics_package.h's struct bg_setup): constructor (InitBgSetup), `Load` (LoadGraphicsPackage), `GetControl` (GetBgSetupControl); `ScaledSprite` (was the file-local struct gfx_box_obj; all UNUSED): `Fit`, `Draw`, `SetColor`, `SetPriority`, `SetPos`, `ResetAttrs` | 3 + 6 | old_agbcp | 0 -> 0 | [#753](#the-tile-slot-pool-the-tile-cache-and-the-bg-setup-752-753) |
| `src/menus/pause_menu.cpp`, `power_dialog.cpp`, `level_select_pages.cpp`, `level_select.cpp`, `continue_prompt_init.cpp`, `src/save/save_menu_ui.cpp`, `src/frontend/language_select_setup.cpp` (again) | callers: `BgSetup` members built in the mem-initializer list, stack ones declared at their construction, `new BgSetup(...)`; `bg.Load(...)`, `bg.GetControl()` | 0 | (unchanged) | 0 -> 0 | [#753](#the-tile-slot-pool-the-tile-cache-and-the-bg-setup-752-753) |
| `src/link/link_sio.cpp`, `link_handshake.cpp`, `link_session.cpp`, `link_session_reset.cpp` (again) | `LinkSession` (new include/link_session.hpp, with `LinkRing` and `LinkPlayer`): constructor (`InitLinkSession`: `new LinkSession`), destructor (`DestroyLinkSession`: `delete gLinkSession`), `Start` (UNUSED), `Reset`, `Stop`, `Update`, `HandleSerial`, `ResetState`; LinkSetupSio (UNUSED), MakeLinkHandshakeId and the two IRQ handlers keep C linkage | 8 | (unchanged) | 3 keeps, 1 `MATCH_KEEP_EXPR`, 14 uses, 1 const, 1 barrier -> the same (tools/match_prune.py: none removable); the constructor's ring loop and the destructor's empty players loop are g++'s | [#751](#the-link-session-the-save-data-and-the-save-transfer) |
| `src/save/save_data.cpp`, `save_transfer.cpp`, `save_transfer_poll.cpp`, `save_menu_input.cpp` (again) | `SaveData` and `SaveTransfer` (new include/save_data.hpp): `SaveData`'s `Load`, `Validate` (UNUSED), `CheckChecksum` (the out-of-line copy of the inline `ChecksumOk` that `Validate` expands), `UpdateChecksum`, `GetGameId`, `Store`, the slot and flag accessors, `SetFlags`; `SaveTransfer`'s `SendChunk`, `ReceiveChunk`, `Poll`, `SetRecord`, `GetData`, `Reset`; ReadSaveData and WriteSaveData (a `void *` buffer) keep C linkage | 14 + 6 | (unchanged) | 4 pins, 1 use, 1 empty-template asm -> the same | [#751](#the-link-session-the-save-data-and-the-save-transfer) |
| `src/save/save_menu.cpp`, `save_menu_draw.cpp`, `save_menu_input.cpp`, `save_menu_ui.cpp`, `src/iwram/iwram_data.cpp` | the callers: SaveMenu's `cartSave`/`linkSave` are `SaveData *` (`new SaveData`; `P9save_data` -> `P8SaveData` in cxx_symbols.txt), the link exchange `new`s a `SaveTransfer`, `gLinkSession` is a `LinkSession *` (link.h) | 0 | (unchanged) | 0 -> 0 | [#751](#the-link-session-the-save-data-and-the-save-transfer) |
| `src/level/level_state.cpp` (again) | `LevelState` (include/level_state.hpp, now the whole class: level_state.h's struct level_state and struct level_progress went): its 85 accessors and helpers as methods; `nullsub_24` and `GetLevelState` stay free (no `self`) | 85 + 2 | old_agbcp | 1 pin -> 0 (SetCheckpoint's r2 hold: an inline `CopyBitmapSpan` loads the CpuSet control word at each call) | [#750](#the-level-state-as-a-class-750) |
| `src/level/game_frame.cpp`, `bonus_round.cpp`, `time_trial.cpp`, `level_cutscene.cpp`, `src/util/aabb_setup.cpp` | `LevelState::UpdateGameFrame`, `EndBonusRound`, `SetCheckpointAtPlayer`, `StartTimeTrial`, `PlayCutscene`, `GetLives` | 6 | (unchanged) | 1 pin -> 0 (SetCheckpointAtPlayer's, as SetCheckpoint's) | [#750](#the-level-state-as-a-class-750) |
| `src/level/level_query.cpp`, `play_room.cpp`, `run_room.cpp`, `room.cpp`, `room_frame.cpp` | `LevelProgress` (the room block, `LevelState::room`): `IsInGemPathRoom`, `IsInBonusRoom`, `PlayRoomMusic`, `NextRoom`, `EnterGemPathRoom`, `EnterBonusRoom`, `SelectRoom`, `PlayRoom`, `RunRoom`, `ResumeRoomAfterPause`, `UpdateRoomFrame`, `SetupRoomBlend` | 12 | (unchanged) | 0 -> 0 | [#750](#the-level-state-as-a-class-750) |
| globals.h, level.h, util.h, level_data.h, 62 `.cpp` files (61 callers and iwram_data.cpp) | `gLevelState`, `gLevelStateSingleton` and `gGameFrameLevelState` are `LevelState *`s to C++; every `Foo(gLevelState, ...)` is `gLevelState->Foo(...)`, `Foo(&self->room)` `room.Foo()`; the 103 C prototypes went | 0 | (unchanged) | 0 -> 0 | [#750](#the-level-state-as-a-class-750) |
| include/player.h, actor_self.h, bitmap_font.h, bg_scroll_layer.h, level.h, objects.h, globals.h, hud.h, gfx.h, menus.h, frontend.h, save_menu.h, crates.h | the C views go (#754): `struct player`, `actor_self`, `bitmap_font`, `bg_scroll_layer`, `level_layers`, `collision_queue`, `sprite_bank_set`, `camera_target` and the tags `hud_counter`, `actor`, `crate`, `oam_shadow_buffer`, `palette_cache`, `vram_upload_cursor`, `entity_spawner`, `level_menu`, `language_select`, `credits_screen`, `save_menu` (and the C arms of the globals' declarations), with their ASSERT_VIEW_FIELD checks and their dead C prototypes | 0 | (unchanged) | 0 -> 0 | [views](#the-c-views-go-754) |
| `src/level/level_cutscene.cpp` | `LevelState::PlayCutscene` builds its `CutscenePlayer` with placement new in its stack aggregate and calls `Run` and the destructor; cutscene.h's `struct cutscene_player` and its three C prototypes go | 0 + 1 | agbcp | 0 -> 0 | [views](#the-c-views-go-754) |
| `src/level/camera.cpp`, `run_room.cpp`, `src/menus/level_select.cpp` | the camera follows a `Sprite *` (`Pos()`, `dir`, `mirror`; `Pos()` moved up from MovingSprite) | 0 | (unchanged) | 0 -> 0 | [views](#the-c-views-go-754) |
| `src/level/terrain.cpp`, `terrain_probe.cpp`, `terrain_probe_axes.cpp` | ProbeTerrainX/Y take a `LevelLayers *`, and the terrain lookups cast `self` to one | 0 | (unchanged) | 0 -> 0 | [views](#the-c-views-go-754) |
| include/crate_list.hpp | crates.h's codegen view `struct pool_init_node` (and `struct pool_link`) is `CrateGridNodeInit`, next to `ResetGrid`, its user | 0 | (unchanged) | 0 -> 0 | [views](#the-c-views-go-754) |

Part 1 in numbers: 30 functions in 4 objects; `MATCH_HOLD_REG` 2151 ->
2118 and instruction-emitting `asm` 249 -> 239 project-wide, plus one
`ENTITY_SET_GONE_BIT_ASR` site (its 6 pins and `asm` are in the macro).
The two pins left are `StompedHopPadCtrl::Update`'s: unpinned, g++ gives
`this` r2 and `part` r4 where the ROM has r3 and r2, with every natural
rewrite tried (switch orders, locals, `this` copies). The C needed the
same two pins, plus gotos for the block order, which a `switch` gives.
(#662 step 2 dropped the `this` pin: with `part` pinned, `this` lands in
r3 by itself.)

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
`MATCH_HOLD`/`MATCH_USE` pair, no code: the 0x600 reload must take r3;
#662 step 2 dropped the `MATCH_HOLD`, the never-assigned pin is live
from the top of the function),
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

The 0x40-byte class is `Sprite`: the C names of its methods say "SpriteObj"
(`InitSpriteObj`, `DestroySpriteObj`, `gSpriteObjVtable`), and cxx_symbols.txt
maps them. Part 7a kept a fifth class, `SpriteObj`, an empty subclass of
`GroundSprite` that every controller method took (`P9SpriteObj` in the
mangled names) while other parts were in flight; step 7b' replaced it with
`MovingSprite`, what a controller really drives (below).

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
to their C names; crate_break.c became C++ in part 7g. `CollidePlayerWithCrates`
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
`IsNearCamera()`) are plain virtual calls; the controller's took a
`SpriteObj *` until step 7b', so `this` was cast. `EntityFlags` names the two
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
pool_manager` (crates.h; crate_list.hpp), `Platform`/`struct gobj` and
`PlatformMover`/`struct mover` (gobj_1a794.h; platform.hpp). Each class has
a `COMPILE_TIME_ASSERT` that its size is the C struct's (the `.hpp` includes the C header), and its
fields keep the C names and offset comments. The C prototypes of the
converted methods stay in the C headers, under their C names, for the
vtable data and the C callers. When the last C user of a struct is
converted, the struct can go; since #754 none is left ([The C views
go](#the-c-views-go-754)). `tools/layout_audit.py` (#656) checks the
pairs from the compiler's debug info: `views` lists every C view of a
class (and every other struct that is a prefix or partial view of
another), `diff Class view` puts the two side by side, and `names`
lists the bytes a family names differently.

**Sprite objects.** `include/entity.hpp` has `Entity` and
`include/sprite_obj.hpp` the classes built on it (part 7a, above), shared
by the controllers instead of each file's own partial view. `EnemyCtrl`
reaches its part through an anonymous union, as part_ctrl.h's `struct
ctrl_target` (its fields) or as a `MovingSprite` (what the `Ctrl` methods and
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

Part 7g in numbers: crate_break.c, the rest of `Crate` (include/crate.hpp):
25 functions, 22 of them methods and 3 free functions with C linkage
(`UpdateCrates`, `DetonateNitroCrates`, `BreakCratesInArea`, which take no
crate). Project-wide: `MATCH_HOLD_REG` 1500 -> 1489, instruction-emitting
`asm` 169 -> 165, `MATCH_USE` 65 -> 63, `MATCH_KEEP` 49 -> 48, scoped
volatiles 29 -> 21 and asm labels 19 -> 18. It was an old_agbcc object
already and matches only under old_agbcp. Every function matched as soon
as the C became methods, except five (below); none of the C's pins was
needed.

- **The slot calls.** crate.h's `PHYS_CALL`/`PhysCall3` slot calls are
  `o->GetClassId()`, `o->Update()`, `delete o` (`UpdateCrates`' slot-10
  call with 3, after its own null test) and `PlayerSprite()->HandleEvent(...)`;
  `PhysSetTag`/`PhysSetFrame` are `SetTag`/`ClampFrame`, `PHYS_GONE` with
  its bit is `MarkGone()`, `AddCollisionCandidate` is
  `CollisionQueue::Add`, `LinkCrateToActiveBucket` and `RemoveCrateListAt`
  are `Crates()->LinkActive(this)` and `Crates()->RemoveAt(i)` (part 7f),
  and `PickUpWumpa` is `Wumpa::PickUp` (part 7h). With crate_break.c gone, crate.h's `struct
  crate_vtable`, the `PHYS_*` call macros, `PhysSetTag`/`PhysSetFrame`
  and the bit and slot-state views (`struct phys_flag_bits`, `struct
  phys_b48`) have no users and are removed; `struct crate` stays the C
  view (the crate list and grid, room_entities.c, the C prototypes).
- **Byte flags are `bool`s.** Five shapes the C spelt with one-byte
  structs, `volatile` stores and pins are `bool` parameters: g++ 2.9
  doesn't promote a `bool`, so a `bool` in a register is a QImode value
  (reloaded with `ldrb`) and one on the stack is stored with `strb`.
  `BreakInStack(u8 flag, bool once, u32 dir)` (the C's `u32` arguments
  and a `(u8)` test): `ApplyCollision` passes its `limited` flag straight
  through, which gives the ROM's `mov r5, sp; ldrb r2, [r5]` (the C needed
  a register union of a `u32` and a one-byte struct and an asm-label
  alias); `ApplyCollision`'s three trailing byte arguments are `bool`s;
  `OpenMystery` and `OpenSlot` take a `bool`, as `OpenLife` does; and the
  `DropWumpa`/`DropExtraLife` calls go through `bool`-flag aliases
  (crate.hpp, now shared with crate_stack.cpp), which replace the C's
  four-argument function-pointer casts with `volatile` stores into
  `argP4`/`argP5` and `BounceWumpaCrate`'s two pins. `BreakCrate`'s
  `chained` is a `bool` too, so `OpenLife(chained)` loads `this` first.
- **`LightTntCrate`**'s four `asm` blocks, seven pins and keep (the
  `0x14` before the tag's address, the `0x10` before the `ldrb`, the
  record's address, the palette nibble's mask) are `SetTag(0x14)`,
  `f.b.active = 1` and a `u32` palette slot stored into `palette`, with
  the animation's address in a local before the `GetPaletteSlot` call
  (the 7e `UpdateCrate` shape).
- **A dead compare's reload.** `QueuePlayerCollision`'s first path calls
  `GetPrevX` for nothing, and the ROM reloads `px` after the call: a
  compare deleted after reload. The C kept the reload with `MATCH_USE2(ax,
  px)`; in C++ it is the other path's code, `side = 2; if (px > prevX)
  side = 1;`, with `side` a function-scope variable: the compare then
  lives until after reload, and only then goes (a block-local `side` is
  deleted earlier, with its reload).
- **`UpdateSlotCrate`**'s `MATCH_USE(lw)` is `lw &= CLEAR_PHASE; lw |=
  nx;` in two steps, which keeps `lw` in r1.
- Small shapes: the record lookups through `bank` (`&anims[tag]` from a
  local copy of `bank->anims` where the ROM loads the table first, and a
  local copy of `bank` itself in `DropCratesAbove`), an inline
  `SetPlayerBusy()` for the `p = gPlayer; one = 1; p->busy = one` latch
  set four times, and `MarkStackTouched`'s box as a `struct aabb *`.

Kept, each with a comment: the 6 `BOX_ADDR`s of `QueuePlayerCollision`
(the player box's stack address recomputed at each call, as in 7e's
crate_hit.cpp and crate_touch.cpp; without them it is held in a
callee-saved register and the function's registers shift), and the C's
small inline helpers and local copies (`PosPtr`, `HitResponseAt`, `Span`,
`AddPlayerHit`, `u8 one = 1`, `u16 eid = id`, ...): each was tried
written in place, and each placed a load or a register as the ROM has it.
`BreakCrate` keeps its spelled-out gone bit (entity_bits.h) and
`BlastNearbyCrates`/`BreakCratesInArea` their `kind + table` integer
sums (indexing adds the other way round). Those two and `UpdateCrates`
walk the crate list through `gCrateList`'s C view (`activeCount`,
`slotArray`): through `Crates()`, an inline call, their loop tests aren't
copied in front of the loops (see the gotchas), which changes the
layout. `DetonateNitroCrates` and `ActivateIronSwitchCrate` use
`Crates()->count` and `slots`.

Part 7d in numbers: the platforms, `Platform` (a 0x80-byte `MovingSprite`,
gPlatformVtable) and its controller `PlatformMover` (a 0x38-byte `Ctrl`,
gPlatformMoverVtable), in the new include/platform.hpp: 4 objects and 19
functions. Project-wide: `MATCH_HOLD_REG` 1544 -> 1512, instruction-emitting
`asm` 173 -> 172, `MATCH_KEEP` 52 -> 51, `MATCH_HOLD` 21 -> 19, `MATCH_USE`
73 -> 70, asm labels 19 -> 18, retyped field stores 214 -> 206 and scoped
volatiles 38 -> 30. All four match only under old_agbcp: `platform.o` and
`platform_contact.o` move to `OLD_AGBCC_OBJS` (`ClearVulnerable`'s and
`CheckPlayerContact`'s constant before the `ldrb`; the C pinned
old_agbcc's registers under agbcc), the other two were old already.

- **The classes.** `Platform` overrides `CheckPlayerContact` (slot 1),
  `Update`, `GetClassId` and the destructor; `PlatformMover` overrides
  `Update` (slot 1), the destructor and the two `...FromSet` motion
  setters (slots 11 and 12, which call `Ctrl::StartTargetMotionX/Y`
  directly with gPlatformMoverMotionRecords). The C views stay:
  gobj_1a794.h's `struct gobj` (also the player's base) and `struct mover`
  (cortex.c's Neo Cortex platform mover is built on it), each checked
  against its class; the unused `GobjInit` and the `OBJ_CALL*` macros go.
- **`new PlatformMover(distX, distY, dirX, dirY, kind)`.** The ROM stores
  the fifth argument with `strb` and reads it with `ldrb`: the two
  direction flags are `bool` parameters (part 7e's gotcha), and the C's
  `MOVER_NEW` 4-argument cast, its two volatile stack stores and the
  constructor's `add r0, sp, #0x18` asm go. `CreatePlatform` is `new
  Platform` (the constructor `inline` in platform_create.cpp, as part 7e
  does), the four `new PlatformMover`, `m->Attach(...)`, `SetExitMirror`,
  `SetAlwaysActive` and the sprite accessors; the palette nibble is a
  `u32` local stored into `palette`.
- **`MovePlayer`** (`MovePlayerWithPlatform`) had 11 pins and wrote the
  player's `hitAxes` through `&carried - 0x44`. Plain field stores give the
  ROM's `sub r0, #0x44` once the 8 is a variable set after the `carried`
  store (`u8 m = 8;`, in its own block); the position goes back through
  `SetPrevPos(q->x = ..., q->y = ...)`, which is the call the C made
  through a one-argument alias of `SetSpritePrevPos`.
- **`PlatformMover::Update`** had 20 pins, a keep and the pinned gone-bit
  macro. The direction flips are `dirX ^= 1; if (dirX)`; one
  function-scope `e` for the four motion-record reads keeps it in r3; the
  "travelled far enough" test reads the record directly; the timed types
  are an `else if` chain (the C's three gotos go), with the first-frame
  hold an inline (`HoldFirstFrame`) used twice and `MarkGone()` for the
  crumbling platform; the wobble's `% 30` is the operator.
- **`ResolveCollision`** keeps the C's `pb` keep and its `GetSpriteHitbox`
  alias (below); its 2 holds, 2 uses and 2 pins go once the classify
  block is an `if`/`else if` chain and the two `goto set_hdir`s are `result
  = hdir;` in place.

Kept, each with a comment:
- `PlatformMover::Update`'s pin on the frame count (`now`, r5): it crosses
  the `__umodsi3` call, and global allocation ranks it above `part` (5
  refs over 11 insns against 55 over 369), so unpinned it takes r4 and
  `part` and every temporary after it swap r4 and r5. The C pinned `part`
  instead. Its 0x300 keeps the C's r2 pin and keep (reload picks r1).
- `SetExitMirror`'s keep on the 1: reload would otherwise build the
  `-0x11` mask from it as `subs r2, #18` (as in the C).
- `ResolveCollision`'s `pb` keep and the `GetSpriteHitbox_p` alias
  (headers_plan.md's codegen exceptions): the ROM keeps `&b` in r4 from
  the player's box call to the overlap tests of the no-contact branch, at
  the end of the function, where CSE forms it from sp again. With a
  struct return, `*pb = ...GetAnimHitbox()` goes through a temporary, so
  the box is built through the alias's explicit destination.

Part 7i in numbers: cortex.c, the last file of the entity family and the
last boss controller file, 25 functions: 23 methods of nine classes and
two C-linkage functions (`TinyHitStub`, which tiny_update.cpp calls, and
`SpawnCortexBossGem`, which spawn_gems.c calls). Project-wide:
`MATCH_HOLD_REG` 1457 -> 1404, `MATCH_KEEP` 47 -> 34, instruction-emitting
`asm` 164 -> 160, retyped field stores 204 -> 202 and scoped volatiles 13
-> 10. Of the C's 49 pins, 13 keeps, 4 `asm` statements and its five
per-site macros, one pin is left (below). It was an old_agbcc object and
matches only under old_agbcp (under agbcp 12 of the 25 functions differ).

- **The classes.** The Neo Cortex fight's shot (`CortexShotCtrl`, 0x18
  bytes: the fast flag and the boss) and gem (`CortexBossGemCtrl`, 0x14
  bytes: the gem's colour) and `UnusedOneShotAnimCtrl` are new in
  boss_ctrl.hpp; `CortexBossCtrl` and `CortexTargetCtrl` gain their
  methods here (`SpawnCannon`, `SpawnTarget`, `SetState`, `FireShot`);
  `CortexBossPlatformMover` is a `PlatformMover` in platform.hpp, whose
  constructor is `PlatformMover(0, 0, false, false, 6)`: the `MOVER_NEW`
  cast and its volatile stack stores go, and so does include/mover_new.h.
  `Platform::Create` makes it with `new CortexBossPlatformMover`. The
  file-local views (`gfx_squares`, `gfx_pair_ctrl`, `gfx_offset_ctrl`,
  `gfx_mover`, `gfx_hit_ctrl`, `gfx_kind_ctrl`) and bosses.h's `struct
  gfx_ctrl`/`gfx_vtable` go; the C prototypes take `void *`.
- **Natural C++.** `VTABLE_CALL2/3` and the `_call_via_r2` attach calls
  are `SetMode`, `SetTargetAnim` and `Attach`; `new CortexCannonCtrl`,
  `new CortexTargetCtrl(this)`, `new CortexShotCtrl(boss)`, `new
  CortexBossGemCtrl(kind)` replace `Create...(OperatorNew(n))`; Tiny's
  squares table is `new s16[0x101]` and `delete[] squares`; the palette
  nibble is the `palette` bitfield (the C's `SetFrameNibble` pins and
  `asm`), the frame clamps an inline `ClampFrame` (the C's `SET_FRAME_R`
  pins), the gone bit `MarkGone()` (the C's `MARK_GONE` and
  `ENTITY_SET_GONE_BIT_PINNED`, which has no user left and goes).
  `UpdateCortexBoss` and `CreateCortexBossPlatformMover`, NAKED under
  agbcc once, are plain C++.
- **`Entity::MarkGone`** (entity.hpp) indexes the bitmap
  (`flags->bits0Copy[word] |= ...`) instead of expanding entity_bits.h's
  `ENTITY_MARK_GONE`, which forms the word's address from a byte offset.
  Every object compiles the same either way; the indexed spelling also
  gives the gem's and the shot's updates the ROM's registers (the `1 <<`
  shift reuses the gem's `state` register, which then ranks below `this`
  and `part`).
- **Small shapes**, each with a comment: `e->SetKind(1)` (the inline)
  in the shot's gem loop, where a plain store makes reload pick other
  registers for `part`, held in r8, in the rest of the function;
  `c->SetPos(part->x, part->y)` in `FireShot` (two loads of `part` rank
  it above `this`); `SetCortexTargetState`'s height pattern as an inline
  that switches on a copy of its parameter (the ROM's `== 2` compare on a
  copy, which the C wrote as a hand-made compare tree with a pin and an
  `asm` move); `SetTag` with an `s32` tag; a named 1 in
  `CortexBossPlatformMover::Update`; Tiny's mirror bit set through the
  byte in two steps; the target's step in locals (`dx` before `steps`,
  both stores after the second division).

Kept, with a comment: `CortexTargetCtrl::Update`'s pin of `n` (the step
count) to r5. The ROM has `n` in r5, `part` in r6 and `this` in r7;
unpinned, global allocation ranks `this` (45 refs) first and gives it r6.
No spelling tried (an inline for the step, the blink flag through a
pointer, the step's locals, ...) changed that. The C pinned `part` and
`n` and needed 11 more pins and an `asm` move in case 5; with `n` pinned,
the C++ needs none of them. `UpdateCortexBoss` keeps the C's `goto` from
state 0 into state 2's `SetState(part, 1)`.

Part 7b' in numbers: no object changes and no function converted; the
types the conversion had kept for parts in flight are the real ones now.
Every object is byte-identical to the one before, and the symbol tables
too: only mangled names changed, each mapped to the same C name.

- **The controllers take a `MovingSprite *`.** `SpriteObj`, part 7a's empty
  subclass of `GroundSprite` that every controller method took, is gone.
  Every `Ctrl` virtual (`Update`, `HandleEvent`, `Attach`, the motion
  setters, `SetTargetAnim`) and every controller's own method that takes
  its part (`EnemyCtrl`, `KnockedEnemyCtrl`, `InputCtrl`, `PlayerCtrl`,
  `ActionCtrl`, `BossCtrl`, the Mega Mix, Tiny, Dingodile and Neo Cortex
  controllers, `PlatformMover`, `OneShotAnimCtrl`, `EffectCtrl`, ...) takes
  a `MovingSprite *`, and so do the bosses' part pointers (`cannon`,
  `target`, `shield`, `owner`). A controller is attached to a moving
  sprite's `mover` (+0x44), and the moving sprite calls it with `this`:
  that is the class the ROM shows. None of the controllers' code used a
  `GroundSprite` field. The overrides must keep the base's parameter type,
  so the more specific objects (a platform, the player) stay
  `MovingSprite *` there. cxx_symbols.txt: 64 mangled names
  `P9SpriteObj` -> `P12MovingSprite`, the same C names.
- **`Ctrl::owner`** (`Attach`'s) is a `MovingSprite *`, not a `void *`.
- **The player is a `GroundSprite`.** Where the controllers pass the player
  (`struct player`, still C) to a `Ctrl` method, they cast it to
  `GroundSprite *`, what the player is, not to the controller's type;
  `ActionCtrl::Sprite()` returns a `GroundSprite *` (until part 8, which
  made the player a class).
- **Casts gone:** `mover->Update(this)` and `ctrl->Attach(this)`
  (moving_sprite.cpp, moving_sprite_collide.cpp, platform.cpp), the four
  `m->Attach(obj)` of `Platform::Create`, and the `(MovingSprite *)` of
  the pickups' `Add`.
- **`PartList` holds `Sprite *`s.** gTouchableList holds pickups (class 2),
  platforms (4) and Tiny's hop pads, so `items` and `visible` are `Sprite
  **`, and `Add`/`Remove` take a `Sprite *` (`P6Sprite`). `Update`, `Cull`,
  `Clear`, `Draw` and `CollideClass` use only `Sprite`'s virtuals. `Collide`
  hands on only the parts whose class id is above 4, which are the moving
  sprites (`MovingSprite` 5, `GroundSprite` 6), so it casts there, for
  `CollideWithPlayer` and `CollideWithObject` (slot 13, `HandleEvent`).
- **`CrateList` holds `Crate *`s** (`slots`, the grid nodes' `data`, and
  `Add`, `Remove`, `AddNode`, `Link`, `LinkActive`, `Unlink`: `P5Crate`), so
  `CollidePlayer`'s and crate_break.cpp's `(Crate *)` casts go.
  crate_list.hpp includes crate.hpp for it. The unused `Collide` still casts
  its nodes to `MovingSprite`, as the ROM's copy of `PartList::Collide` does.
- **The C++ callers of the part lists** use the class: `TouchableList()`,
  `CollidableList()` and `ForegroundList()` (sprite_obj.hpp, like
  `Crates()`) replace the `((PartList *)gTouchableList)` casts and the C
  `AddToPartList(gCollidableList, p)` calls of cortex.cpp, dingodile.cpp,
  tiny_update.cpp, platform_create.cpp and the pickups. `InputCtrl::StateStart`
  kept `AddToPartList` for the camera lead, then a C struct, until part 10
  (`CollidableList()->Add(cameraLead)`).

### The player (part 8)

Part 8 in numbers: the player, `Player` (new include/player.hpp, a
`GroundSprite`, 0x350 bytes; gPlayerVtable), all six of its C files
(player_update.c, player_init.c, player_reset.c, player_collide.c,
player_anim_room.c, player_event.c), 15 functions, and player_flags.cpp's
40 accessors as its methods. Project-wide: `MATCH_HOLD_REG` 1404 -> 1351,
`MATCH_USE` 60 -> 55, `MATCH_HOLD` 18 -> 13, `MATCH_CONST` 26 -> 22,
instruction-emitting `asm` 160 -> 155, retyped field stores 202 -> 201 and
reads 82 -> 80. Every pin, hold, use, constant-init and `asm` of the six
files goes; one volatile read stays (below). player_update.o and
player_reset.o match only under old_agbcp (their pins and `asm` were
old_agbcc's constant-before-`ldrb` order under agbcc) and move to
`OLD_AGBCC_OBJS`; player_init.o and player_anim_room.o stay agbcc,
player_collide.o and player_event.o were old_agbcc already.

- **The class.** `Player`'s fields after 0x80 keep `struct player`'s names
  (`busy`, `ctrlMode`, `deadline`, ..., `slippery`, `hanging`, `dead`);
  `list` holds `Crate *`s, `carried` and Aku Aku's `child` are `Sprite *`s,
  `maskTrail` is `struct gfx_vec`s (so the mask's trail reset copies
  `Pos()`), and `collisionQueue` is a `CollisionQueue`. Its slots: 1
  `CheckPlayerContact` (CollidePlayer), 3 `Update`, 4 `Draw`, 10 the
  destructor, 12 `ApplyVelocity`, 13 `HandleEvent`, 14 `TouchPlayer`
  (CollidePlayerWithObjects); the others are the base classes'. player.h's
  `struct player` stays the C view (the rooms, the HUD, the vehicles), and
  player.hpp checks both sizes.
- **The constructor and destructor are g++'s.** `InitPlayer` is
  `Player(id, x, y, unused)`: g++ calls `InitGroundSprite`, stores the
  vtable pointer and then constructs the member `collisionQueue`, which is
  `ResetCollisionQueue`. That makes `ResetCollisionQueue` `CollisionQueue`'s
  constructor (part 7c had it as `Reset`). `DestroyPlayer` is `~Player() {
  delete child; }`: the `delete` is the C's null test and slot-10 call with
  3, and g++ then destroys the member with flags 2 (`DestroyCollisionQueue(q,
  2)`, which part 7c's comment had predicted) and calls
  `DestroyGroundSprite`.
- **`ApplyVelocity` is `MovingSprite::ApplyVelocity`'s code.** The C's 14
  pins, gotos, two retyped reads and the 8-instruction `asm` block for
  the redundant store go. The global is a new name, `gLastPlayerVelY`
  (0x0300129C, the word after `gLastSpriteVelY`; sym_iwram.txt), where the
  C wrote the address: the object's literal becomes a relocation, the
  ROM's word is the same.
- **`gPlayer` is a `Player *` to C++** (globals.h, above). The 42 C++ files
  that use it include player.hpp, and every field access uses the class's
  names (`f.flags`, `f.b.collides`, `mirrorBits.flipX`,
  `mirrorFlags.mirrorX`, `mirror`, `palette`, `mover`, `bank->anims[...]`).
  `PlayerSprite()` and every `(GroundSprite *)` and `(MovingSprite *)`
  cast of the player go, and so does `ActionCtrl::Sprite()`: `ActionCtrl`,
  `PlayerCtrl` and `InputCtrl` hold a `Player *` (`P6Player` in
  `PlayerCtrl::StartMotion?FromSet`'s mangled names). The C-linkage calls
  that took the player are method calls (`gPlayer->TouchesBox(&box)`,
  `p->HasRoomForAnim(0xB)`, `p->ClearSpeedY()`, `gPlayer->HasRampYTarget()`,
  `p->collisionQueue.Resolve()`, `p->mover->state`).
- **Hand-written virtual calls go.** cortex.cpp's, dingodile.cpp's,
  tiny_update.cpp's and mega_mix_update.cpp's slot-13 calls through
  `vtable->handleEvent` are `pl->HandleEvent(0, event, 0)`, byte for byte.
- **The accessors** (player_flags.cpp) are `Player` methods,
  cxx_symbols.txt mapping them to the C names. `GetListEntry`'s 3 pins and
  gotos go: as a method, the plain test matches (`GetPlayerListEntry`
  returns a `struct crate *` now, as `GetPlayerStandingOn` returns a
  `struct gobj *`, in player.h).
- **Inline helpers in the class.** `StoreHitAxes`, `StoreSlippery`,
  `StorePushLeft`, `StorePushRight`: a store through an inline method, whose
  value (a parameter) is materialized before the field's address, as the
  ROM has it in `CheckPlayerContact` and the controllers (they replace
  action_ctrl.hpp's and swim_ctrl.cpp's `SetHitAxes`/`SetSlippery`). And
  `FrameAnchor()`, `GetSpriteFrameAnchor` inlined, which
  `CheckPlayerContact` and `ActionCtrl::HandleEvent` share (below).
  `ActAndFlags0D`/`ActOrFlags0D` moved from action_obj.h to action_ctrl.hpp
  and write flags2 through a pointer to the member, not `(u8 *)p + 0xD`.

What made the C++ match:

- **`CheckPlayerContact`'s registers come from `FrameAnchor`'s.** The
  ROM's `ldrsh` index of every virtual call in the function is r2, where
  g++ gave r3; the C held r3 with register variables through the whole
  function. The cause is reload's spill registers: a hard register that
  global allocation gave to a pseudo is only taken for reloads when no
  free one is left, and in the ROM the frame anchor (`off`) lives in r3.
  With the anchor computed by a `return` per case (action_ctrl_event.cpp's
  spelling), it gets r3, reload's scratch for the earlier `ldrsh`es comes
  out r2, and every register of the function is the ROM's. The C's
  `result = ...; break;` form left the anchor in r0, copied to r2.
- **`f.b.vulnerable = 1` before `deadline = 0`** (hurting ground): with
  `f.flags |= 0x40`, the 0 is scheduled into the `ldrb`'s delay slot.
- **Jump threading.** `Draw` tests the mask level twice; when the first
  test is false, g++ jumps past the second (same RTL, no store between).
  The ROM reloads it, and so does a test through a local (`struct
  level_state *game = gLevelState;`). The C had the effect from its r6
  holds, which C++ doesn't need for the registers.
- **`ResetForRoom`'s switch on a copy** (`s32 mode = ctrlMode; s32 m =
  mode; switch (m)`), as `StepHeight` (part 7i): the C's goto chain and two
  pins.
- **`HasRoomForAnim`'s box through an inline** (`AnimBox`): the `+ 4` of
  the first box is its own `adds` before the copy into the variable.
- **`Reset`'s flags** are bitfield stores, which g++ merges into one
  `ldrb`/`strb` per byte, in the ROM's order of operations under
  old_agbcp: the C's three `asm` blocks and 30 pins go.

Kept: `HandleEvent`'s volatile read of the mask level after the
controller call (the ROM loads it and never uses it).

### The spawners (part 9)

Part 9 in numbers: the effect-part spawner and the level spawners, 10 of
the 11 spawn files in src/level/ (all but spawn_enemies.c, part 9b), 74
functions.
Project-wide (after part 10): `MATCH_HOLD_REG` 1323 -> 1252,
instruction-emitting `asm` 153 -> 139, `MATCH_USE` 55 -> 54, `MATCH_KEEP`
33 -> 32, asm labels 17 -> 14, retyped field stores 201 -> 196 and reads
79 -> 78. spawn_crates.o and
spawn_pickups.o match only under old_agbcp (their pins and `asm` held
agbcc to old_agbcc's constant-before-`ldrb` order) and move to
`OLD_AGBCC_OBJS`; the other eight were old_agbcc already.

- **The entity spawner is a class.** `EntitySpawner` (include/spawners.hpp;
  level.h's `struct entity_spawner` is its C view) holds the spawn table,
  and every function whose first argument is `gEntitySpawner` (the C's
  `pool` or `unused`) is its method: `SpawnEffectPart`, `LaunchEffectPart`,
  `DropWumpa`, `DropExtraLife`, `Spawn` (SpawnEntity) and `SetTable`. Its
  constructor and destructor are InitEntitySpawner and
  DestroyEntitySpawnerObj. It has no vtable, so `delete gEntitySpawner`
  is the null test and a direct call of the destructor with 3
  (DestroyEntitySpawner), and `new EntitySpawner` uses the constructor's
  `return this` (the C's `bl InitEntitySpawner` asm).
- **The effect part is a `MovingSprite`** with an `EffectCtrl`:
  `SpawnEffectPart` is `MovingSprite::Create`, `new EffectCtrl`,
  `part->mover = mgr` and `mgr->Attach(part)` (the C's slot call through
  `_call_via_r2`). `struct fx_part` goes; no effect-part class is needed.
- **`gEntitySpawner` is an `EntitySpawner *` to C++** (globals.h, as
  `gPlayer`), so the C++ callers call the methods: crate_break.cpp,
  crate_stack.cpp, wumpa_update.cpp, sprite.cpp, player_event.cpp, the
  action and swim controllers and the enemy controllers. The casts go:
  `(struct fx_part *)gPlayer` (now `LaunchEffectPart(..., gPlayer)`), the
  `(struct gfx_part *)` and `(MovingSprite *)` casts of the result (the
  sparks' `hidden` and `gfxMode` are `f.b.visible` and
  `mirrorBits.gfxMode`), crate.hpp's `DropWumpaFlag`/`DropExtraLifeFlag`
  asm-label aliases and wumpa_update.cpp's `DropWumpaFunc` cast (the flag
  is the method's `bool` parameter). `LaunchHarmfulEffectPart`
  (enemy_ctrl.cpp) takes and returns a `MovingSprite *`; its prototype
  moved from enemies.h to enemy_ctrl.hpp. level.h keeps one C prototype,
  `SpawnEntity`, for room_entities.c, which passes its `struct
  level_entity` without the `(u16 *)` cast.
- **The spawners build with the classes.** `Sprite::Create`,
  `MovingSprite::Create`, `Entity::Create`, `Platform::Create`,
  `Crate::Create`, `Wumpa::Create`, `ExtraLife::Create` and
  `Stopwatch::Create` replace the C names; `new DingodileCtrl(x, y)`, `new
  TinyCtrl`, `new CortexBossCtrl`, `new MegaMixCtrl`, `new PeriodicSpawner`
  and `new EffectCtrl` replace `CreateX(OperatorNew(n))`, and
  `hdr->Attach(part)` the `POPUP_ATTACH` slot calls. The part lists are the
  class (`TouchableList()->Add`, `DecorationList()`, and `AddUpdateOnly`
  for the update-only list, which holds bare entities). The bosses' own
  `(MovingSprite *)CreateMovingSprite` calls (cortex.cpp, dingodile.cpp,
  tiny_update.cpp) are `MovingSprite::Create` too.
- **`InitLevelState`** (spawn_pickups.cpp) builds the C++ classes with
  `new`: `SpriteRenderer`, `SpriteBankSet`, `PaletteCache`, `OamBuffer`,
  `ObjVramCursor(0)`, `PaletteCycles`, and the key input, `KeyInput`
  (spawners.hpp), whose constructor is ClearKeys (src/system/irq.c, still
  C, `void *ClearKeys(void *self)`, which returns `this` in r0). The C's six `bl`
  asm statements with the pointer pinned to r0 go: a constructor returns
  `this`. The globals keep their C types (the C files use them), so the
  new objects are stored through casts to their C views. The audio
  context, the fonts and the entity flags are still C and are built by
  their C constructors (the fonts are C++ since step 10b: `new
  SmallFont`, `new LargeFont`).
- **The parameter records.** text_popup.h's `struct level_record` is
  level_data.h's `struct entity_params` now (level_menu.h has a `union
  level_record`, and C++ has one tag namespace for both), and
  `EntityParams(index)` (spawners.hpp) looks one up.
- **The level state's platforms and boss** (`bonusPlatform`,
  `gemPlatform`, SetGemPlatform, SetBonusPlatform, SetLevelBoss) take
  `void *`s: the spawners pass the `Platform *` or the boss controller
  without the C's `(s32)` and `(struct level_state_1c8 *)` casts.

What made the C++ match:

- **SpawnEntity's table offset.** The ROM adds the scaled index to the
  table (`lsls r0, r3, #2; adds r0, r0, r1`), with the table loaded first
  and the function after the record's halfwords; `&funcs[rec->type]` adds
  the other way round. The byte offset added to the table, `(rec->type <<
  2) + (u32)table`, gives the ROM's code; the C's 4 pins and `MATCH_USE`
  go.
- **`LaunchEffectPart`'s two boxes.** The first box named (`struct aabb a
  = part->GetAnimHitbox()`) and the second a temporary (`w2 =
  src->GetAnimHitbox().w`) give the ROM's two stack slots and its
  re-added `sp` offsets. Two named boxes hold the second one's address in
  a register; two temporaries share one slot. The flip is the sign test
  (`mirrorBits.flipX < 0`).
- **`DropWumpa`'s always-active bit** is `SetAlwaysActive()` (a bitfield
  store): with `f.flags |= 0x10` the phase's 0 is scheduled before the
  flags' `ldrb`, where the C needed a `MATCH_KEEP`.
- **A bitfield set from an inline's parameter** is a full insert (the field
  cleared with `~mask`, the value ORed in) even when the argument is a
  constant: g++ 2.9 inlines at the RTL level. `SpawnBasicCrate`'s and
  `SpawnStartMarker`'s mirror bits are that (`SetFlipX(obj, 1)`), where a
  literal store is a plain OR; the C spelled the masks out in `asm`.
- **`EntityParams(index)` through an inline** gives the record's address
  a register copy of its own (`adds r3, r0, #0`), which the C made with
  an `asm` copy.
- **`new X` into a global** loads the global's address before the
  allocation, as the ROM does. A C constructor's result is stored through
  a pointer to the global taken first (`struct AudioContext **audio =
  &gAudioContext;`), and an object that needs a later call is kept from
  the `new` expression itself (`gPaletteCache = (struct palette_cache
  *)(cache = new PaletteCache)`). `operator new(n)`, not the C name
  OperatorNew, for the C constructors keeps the object's one undefined
  `OperatorNew` symbol.
- **A constant the ROM keeps in a callee-saved register** across calls is
  a variable: the powers' tags and kinds are `u8` locals (a literal 0 is
  loaded again at the store), and `InitLevelState`'s display-control 0,
  stored again into the unused flags after two calls, is a `u16 zero`,
  with the display control's address taken first.

Kept (both the same gap): `SpawnRoomExit`'s 4 pins and `MATCH_KEEP`
(spawn_bosses.cpp) and `SpawnCrateGemMarker`'s 4 pins and truncation
`asm` (spawn_pickups.cpp). In both, the ROM computes the crate gem's
point into fresh registers (`subs r2, r1, #2`; `lsr r3, r1, #16; lsr r4,
r2, #16`, r4 a callee-saved register it pushes for nothing), as if the
values were global allocation's pseudos; g++ computes them in place with
every spelling tried (locals, `u16` parameters, an inline setter, a point
class with a constructor, a loop around the stores) under both
compilers.

### The enemy spawners (part 9b)

Part 9b in numbers: src/level/spawn_enemies.c, the last level spawner
file, 26 functions. Project-wide (after part 10b): `MATCH_HOLD_REG` 1249
-> 1228, instruction-emitting `asm` 139 -> 133, `MATCH_USE` 52 -> 51,
asm labels 14 -> 13. The object was old_agbcc already and is old_agbcp
now (agbcp differs in all 26).

- **Each spawner is the classes' code.** `MovingSprite::Create` (the
  seal's `GroundSprite::Create`), `new EnemyCtrl`, and `hdr->Attach(part)`
  for the two `POPUP_ATTACH` slot calls; `part->mover`, `part->palette`,
  `mirrorFlags`, `CollidableList()->Add`, `EntityParams(arg3)`, and
  `EnemyCtrl`'s `SetState`, `SetRangeX`, `SetRangeXSpeed` and
  `SetRangeYSpeed`. The field setters the ROM inlines (the animation map,
  the trigger box, the attack cycle, the wave, the shot timing) are
  file-local inlines, as text_popup.h's were. `SetMirror` sets the mirror
  bits from the record and `SetAnim` restarts an animation.
- **`new EnemyCtrl` matches in all 26.** In 11 of them the C needed
  `OperatorNew(0x8c);` and an asm-label alias of CreateEnemyCtrl taking
  no argument (the block left in r0), the other 15 `CreateEnemyCtrl(
  OperatorNew(0x8c))`; g++ gives each the ROM's registers from the one
  spelling.
- **SpawnVenusFlytrap** was still its agbcc-era pinned C (21 pins and 6
  `asm` statements, among them the whole mirror-bit and trigger-box code
  and a hand-written `_call_via_r2` call): the plain C was 5 halfwords off
  under old_agbcc. The C++ is the same code as its siblings', with no
  workaround. Its prototype takes `u16`s like the others', so the spawn
  table's `(entity_spawn_fn)` cast goes, and its enemy kind (0xA) is
  `ENEMY_KIND_VENUS_FLYTRAP` (data/levels/entity_types.json).
- **text_popup.h goes**: `struct popup_part`, `POPUP_ATTACH`,
  `POPUP_ANIM`, `LEVEL_RECORD` and its inline setters had no other user.
  enemies.h keeps only the C names the vtables need (and GetSfxVolumeAt);
  the 31 method prototypes the spawners called are gone.
- **Dead prototypes.** The C prototypes of the C++ constructors no C file
  calls any more went with them: CreateCrate, CreatePlatform,
  CreatePlatformMover, CreateSpriteObj, CreateMovingSprite,
  CreateGroundSprite, CreateEntity, CreateWumpa, CreateExtraLife,
  CreateStopwatch, the bosses' and controllers' `Create*`, and the
  `Init*` constructors of the classes (InitSpriteObj, InitMovingSprite,
  InitPlatform, InitOamBuffer, ...; 43 in objects.h, crates.h, gfx.h,
  pickups.h, bosses.h and player.h). menus.h's were left alone, and about
  440 other prototypes of C++ methods have no C caller either (most of
  objects.h's, crates.h's, player.h's and menus.h's accessors); see "What
  is still C".

Kept: `SpawnFlamethrowerLabAssistant`'s workarounds, one `MATCH_USE` fewer
than the C's (1 pin, 1 `MATCH_HOLD`, 5 `MATCH_USE`, 1 `MATCH_KEEP`, 1
`MATCH_USE_MEM`). The ROM keeps the mirror byte's address (part+0x28) in
r3 across `AddToPartList` through a stack slot, with &gEntityFlags in r9
and -0x11 in sl. g++, like the C front end, ranks the address (6
references) above both of them and gives it a callee-saved register,
the same with every spelling tried (inline setters taking the sprite or
the bits, the pointer taken earlier, the sign test, the second lookup
moved, the pointer set by an inline through a reference). The address is
kept in memory instead (`MATCH_USE_MEM`), stored inside the call's
argument after a `MATCH_KEEP` copy of the part, and the hold and uses
steer reload's registers, as in the C.

### The level select (part 10)

Part 10 in numbers: the level select's three files (src/menus/level_select*.c,
ROM 0x0801B85C-0x0801E578), 91 functions and seven classes in the new
include/level_select.hpp, and the camera lead's C++ caller, input_ctrl.cpp.
Project-wide: `MATCH_HOLD_REG` 1351 -> 1323, `MATCH_KEEP` 34 -> 33,
instruction-emitting `asm` 155 -> 153 and scoped volatiles 10 -> 9. All
three objects were old_agbcc C already and match under old_agbcp.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `CameraLead` | 0x80 | gCameraLeadVtable (a `MovingSprite`) | level_select.cpp |
| `LaunchPad` | 0x78 | gLaunchPadVtable (a `MovingSprite`) | level_select.cpp |
| `LevelSelect` | 0xAC | none | level_select.cpp, level_select_pages.cpp |
| `LevelSelectPageBg` | 0x28 | none | level_select_pages.cpp |
| `ZoomBg` | 0x8C | none | level_select_widgets.cpp (constructor: _pages.cpp) |
| `LevelSelectEntry` | 0x14 | gLevelSelectEntryVtable (a root class: the vtable pointer at +0x10) | level_select_widgets.cpp |
| `LevelSelectCursor` | 0x54 | none | level_select_widgets.cpp |

- **The classes.** The camera lead and the launch pad are `MovingSprite`s:
  the camera lead overrides `Update` and the destructor, the launch pad the
  destructor and `TouchPlayer` (slot 14, `CheckLaunchPadContact`, which
  sends the player `EVENT_LAUNCH_PAD`: `gPlayer->HandleEvent(0,
  EVENT_LAUNCH_PAD, 0)`, where the C pinned `_call_via_r4`'s function).
  The screen's other objects have no vtable but a constructor and a
  destructor, so `DestroyX(p, 3)` after a null test is `delete p` and
  `CreateX(OperatorNew(n), ...)` is `new X(...)`. The screen's sprites are
  `UiSprite`s; `delete` on one is its slot-10 call (`DELETE_PART` went).
- **The C views go.** No C file reads these objects' fields: the camera
  lead's `struct follow_child` (camera_lead.h) and level_menu.h's `struct
  level_menu`, `level_item`, `zoom_bg`, `page_bg`, `twinkle` and their
  vtable views are gone, and so is level_select_parts.h. menus.h keeps the
  C prototypes (the C names) with opaque tags, for the vtables
  (src/data/entity_vtables_7e3bec.c) and the C callers (game_frame.c,
  spawn_objects.c); `gLevelSelect` is a `LevelSelect *` to C++ (menus.h's
  `__cplusplus` declaration, as `gPlayer`'s). level_menu.h keeps `struct
  sprite` and the save block, which the pause menu and the power dialog use.
- **`InputCtrl`** holds a `CameraLead *`: `new CameraLead`,
  `CollidableList()->Add(cameraLead)`, `cameraLead->Reset()`, and
  `MarkGone()` for its two `ENTITY_MARK_GONE`s.
- **The fonts were still C** (C++ since step 10b; src/text/, `struct bitmap_font`): their
  virtual calls stayed spelled out through the record's slots.

What made the C++ match:

- **A store to a union member clobbers everything** (gcc gives every access
  to a union member alias set 0). `Sprite::bank` is in an anonymous union
  with `anim`, so `s->bank = ...` made g++ reload `sprites[i]` (or `frame`,
  or `twinkles[i].part`) after it, where the ROM's class had a plain
  member. Through a pointer to the member (`SetBankNow`, level_select.hpp)
  the store has the pointer type's alias set, and the constructors match as
  written.
- **`SpawnLaunchPad`** is `new LaunchPad(id, x, y)`, an inline constructor
  (as `MovingSprite(id, x, y)`), with the out-of-line default one,
  `InitLaunchPad`, at the end of the class's code. Its two `mov/neg` mask
  `asm` statements are the two mirror bitfield stores, and the palette
  nibble a store through a `u32` local.
- **`UpdateCameraLead`'s `dir`/`hitAxes` stores** come out with the second
  address formed from the first (`adds r0, #0x24; strb; adds r0, #0x44;
  strb`) through an inline taking both values (`SetAxes`).
- **`LevelSelectLoop`'s key copy:** declaring `k` before `union key_state
  keys = gKeys` keeps `keys` in a register (declared first, g++ puts the
  union on the stack); the C's statement expression and `MATCH_KEEP` stay
  for the copy's place. The A-button test is a local (`u32 a = ...; if
  (!a)`), or g++ narrows it to an `ldrb` and an `eor`.
- `UpdateLevelSelect`'s DISPCNT BG2 bit is a bitfield store (the C wrote
  the byte through a `u8 *`), the fade-in a plain `bldy.evy--` (the C's
  byte view goes), `DrawLevelSelect` needs neither the `ItemAt` pin nor the
  `goto` (the sprite array's address taken before the entries' loop gives
  the ROM's order), and `InitLevelSelect` needs none of the C's `Opaque`
  helpers.

Kept, each with a comment: `ResetCameraLead`'s one pin (the ROM tests
`blink` with its own 1 in r1; unpinned, gcc shares the constant with
`ToggleHidden`'s; the C had five pins), `UpdatePageArrows`'s two (the arrow
in r1 and `&tag` in r3; the C had 24 and a keep), the `MATCH_KEEP` of
`LevelSelectLoop` and the two `_rw` asm-label aliases of the const position
tables (`InitLevelSelect` reloads them across calls). C idioms kept:
`LevelSelectLoop`'s and `LoadLevelSelectRecord`'s gotos, the page turns'
goto loops (a `while` gets its test copied in front), and
`GetLevelSelectPageBgOffsets`'s `u32` read of `hofs`/`vofs` (g++ 2.9 has
no anonymous structs for a union view). `SetCameraLeadUnk32` stores the byte at
0x32, inside `Sprite::frame`, through a byte pointer: nothing else
touches it.

### The front end (part 10b)

Part 10b in numbers: the first three objects of src/frontend/
(company_logos.c, language_select.c, language_select_setup.c; ROM
0x0803686C-0x08037648), 20 functions and three classes in the new
include/frontend.hpp, plus a minimal `ActorSelf`, the 3D actors' base, in
the new include/actor_self.hpp. Project-wide: `MATCH_HOLD_REG` 1252 ->
1249, `MATCH_USE` 54 -> 52, retyped field stores 196 -> 195 and reads 78
-> 76. `language_select_setup.o` moves to `OLD_AGBCC_OBJS`;
`company_logos.o` was old_agbcc C already, `language_select.o` stays agbcc.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `ActorSelf` | 0x54 | gActorVtable (a root class: the vtable pointer at +0x50) | still C (src/actor/actor.c; C++ since part 11a) |
| `LogoActor` | 0x54 | gLogoActorVtable (an `ActorSelf`) | company_logos.cpp, language_select.cpp (destructor) |
| `CompanyLogos` | 0x44C | none | company_logos.cpp, language_select.cpp, and title_screen.c (still C; C++ since part 10c-2) |
| `LanguageSelect` | 0x14 | none | language_select.cpp, language_select_setup.cpp |
| `Starfield` | 0x14 | none | still C (src/frontend/starfield.c; C++ since part 10c) |

- **The logo actor is the first 3D actor in C++.** `ActorSelf` declares
  only what `LogoActor` needs: the fields of `struct actor_self`, the
  constructor (`InitActorPart`), slots 1-3 (the destructor, `Update`,
  `Draw`; DestroyActor, UpdateActor and DrawActor), `GetAnimFrameBaseOffset`
  and an inline `SetState` (actor_self.h's `ACTOR_SET_STATE`). Its
  destructor is inline, the unlink from the actor list: g++ puts it into
  `~LogoActor` after the body, with the vtable pointer stores the C wrote
  out. The actors live in mem_alloc's 0x80000000 heap and are freed with a
  direct `mem_free`, not `OperatorDelete`: that is the class's own
  `operator new` and `operator delete` (inline), which the deleting
  destructor calls. `InitLogoActor` is `LogoActor(anim) : ActorSelf(anim,
  0, 0, 0x100)`. Part 11 builds the rest of the 3D actors on this class.
- **The screens are plain classes.** `CompanyLogos` (the C's `struct
  logo_screen`, which title_screen.c still used until part 10c-2) has an empty constructor
  and destructor (`InitCompanyLogos`, `DestroyCompanyLogos`), and
  `LoadTaggedAssetBuffered`, whose unused first argument is the screen
  (LoadVvLogoGraphics's), is its method `LoadAssetBuffered`.
  `LanguageSelect`'s constructor is `new Starfield` and its destructor
  `delete starfield` (a null test and `DestroyStarfield(p, 3)`); `Open` and
  `Close` are `new LanguageSelect` and `delete gLanguageSelect`. `Starfield`
  is declared for those, with starfield.c still C (C++ since part 10c).
- **The C view goes** for the language select: no C file reads `struct
  language_select`'s fields, so it is an opaque tag (frontend.h), and
  `gLanguageSelect` is a `LanguageSelect *` to C++ (a `__cplusplus`
  declaration, as `gPlayer`'s). frontend.h keeps the C prototypes (the C
  names), for the vtable data and the C callers (main_loop.c,
  level_state.c, title_screen.c).
- **The fonts were still C** (C++ since step 10b; src/text/): `DrawLanguageSelect`'s virtual
  calls stayed spelled out through the record's slots.

What made the C++ match:

- **`LoadLanguageSelectBg`'s DISPCNT** is bitfield stores into a `struct
  dispcnt_bits` view (`dispcnt.bits.objMap1D = 1; ... obj = 1;`), which
  g++ merges into one `ldrb`/`strb` per byte. Under old_agbcp each
  constant is loaded before its byte, as in the ROM; the C was built with
  agbcc, wrote the two bytes as `u8`s and pinned two registers for that
  order. `CommitFrame` writes `dispcnt.raw`.
- **`DrawLogoActor`'s attribute 2** is `(palette << 12) | tile` with the
  tile number in a local: the tile is computed first (r0) and the palette
  ORed into r1, where the C pinned the tile to r0.
- `new u16[n]`/`delete[]` for the scratch buffers (the C's
  `OperatorNewArray` and null-tested `OperatorDeleteArray`), `Input`'s two
  confirm paths written out (the cross-jump shares the `PlaySfx`; the C had
  a `goto`), and `Run`'s pressed keys read into a local before the
  `gLanguageSelect` call (the C took `&gKeys.half` into a pointer).

Kept, each with a comment: `DrawVvLogoPieces`'s r2 pin on the DMA base
(unpinned, it and `&frames` swap r2 and r3) and its three
`MATCH_BARRIER`s (instruction-count padding for the loop optimizer's
hoisting order, #481). Its two `MATCH_USE`s went. C idioms kept: the
unsigned `switch ((u32)state)` of `LogoActor::Update` (one bounds check),
the `u8 z = 0` store of the sound-cue flag, `InitGraphics`'s inline icon
helpers and shared `zero`, and the `(struct dispcnt_bits *)gDispcnt`
views.

### The front end, continued (part 10c)

Part 10c in numbers: starfield.c and credits.c (ROM 0x08034480-0x080354E0),
20 functions, and three more classes in include/frontend.hpp.
Project-wide: `MATCH_HOLD_REG` 1249 -> 1230, instruction-emitting `asm`
139 -> 134 and retyped field stores 195 -> 194. `starfield.o` moves to
`OLD_AGBCC_OBJS`; `credits.o` was old_agbcc C already.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `Starfield` | 0x14 | none | starfield.cpp |
| `Credits` | 0x98 | none | credits.cpp |
| `ContinuePrompt` | 0x24 | none | credits.cpp (`Draw`, `Blink`, `CommitFrame`, the destructor, `Run`); its constructor, `InitGraphics` and `Loop` are C++ since part 10d (src/menus/continue_prompt*.cpp) |

- **The classes.** All three are plain classes with a constructor and a
  destructor. `Starfield`'s `particles` and `tileBuffer` are `new
  StarParticle[0x80]` and `new u8[0x4B00]`, freed with `delete[]` (the C's
  `OperatorNewArray` and null-tested `OperatorDeleteArray`). `Credits::Run`
  and `ContinuePrompt::Run` are `new X`, the loop and `delete self`; the
  credits' text lines are `new CreditsPopup` and `delete node` (a POD, so
  the C's direct `OperatorNew(0x18)` and `OperatorDelete`).
- **The C views go** for the credits: no C file reads `struct
  credits_screen`, `struct popup_node` or `struct popup_glyph`, so the
  first is an opaque tag (frontend.h) and the others went. The continue
  prompt keeps menus.h's `struct continue_prompt`, which its C files use.
  frontend.h and menus.h keep the C prototypes (the C names) for the C
  callers (game_frame.c, continue_prompt.c, title_screen*.c). Part 10d
  converted the continue prompt's C files, and `struct continue_prompt`
  went.
- **The fonts were still C** (C++ since step 10b; src/text/): the font calls stayed spelled out
  through the record's slots.

What made the C++ match:

- **The starfield matches as written under old_agbcp,** where the agbcc
  C needed 16 pins and 5 `asm` blocks. `Draw`'s two trail plots are the
  out-of-line `PlotPixel`'s body as an inline method (`Plot`, called with
  `Q8_TO_INT(slot->x)`), and the nibble write is `*entry = (*entry &
  ~(0xf << shift)) | (val << shift)`, where the C wrote the ROM's
  `bic`/`orr` tail as `asm`. The constructor's tile mask is the literal
  `tileIdx | 0xF000` (loop.c hoists it twice: the ROM's `ldr r1, =...;
  adds r5, r1, #0`) and the row address `(row << 6) + mapBase`.
- **`ContinuePrompt::Blink`** is `if (option == selection) return
  (blinkCounter++ >> 1) & 2; return 1;`; the C pinned the result to r0.
- `Credits::Loop`'s key test is a plain `if (gKeys.half.pressed & 9)`
  (the C pinned both operands), and the logos' palette slot is a plain
  `s32` (the C's retyped store).

Kept, each with a comment: `Credits::LoadLogos`'s palette index (an r1
pin, a `MATCH_KEEP_VOLATILE` and a `MATCH_USE`: GCSE hoists any `slot <<
5` to the y loop's pre-test and spills it, and the ROM computes it at the
copy) and the constructor's `MATCH_BARRIER` (no code: the longer live
ranges give `&gPaletteCache` and `&gObjVramCursor` r4, `&gSmallFont` r6).
#662 step 2 found the barrier, the keep and the use no longer needed;
the r1 pin stays.
C idioms kept: `DrawText`'s and `UpdateText`'s gotos, the `bg0cnt =
bg0cnt` self-initialisation and the `UpdateText` font slot calls written
out.

### The pause menu, the power dialog and the continue prompt (part 10d)

Part 10d in numbers: six objects of src/menus/ (pause_menu.c,
pause_menu_pages_init.c, power_dialog.c, power_dialog_draw.c,
continue_prompt_init.c and continue_prompt.c), 29 functions, two classes in
the new include/menus.hpp and the rest of `ContinuePrompt`
(include/frontend.hpp). Project-wide: `MATCH_HOLD_REG` 1209 -> 1125,
instruction-emitting `asm` 128 -> 114, `MATCH_CONST` 22 -> 19, `MATCH_KEEP`
32 -> 31, `.pool` in asm 6 -> 5 and retyped field reads 76 -> 70. Four
objects move to `OLD_AGBCC_OBJS` (pause_menu.o, power_dialog.o,
power_dialog_draw.o, continue_prompt_init.o); the other two were old_agbcc
C already. Every function but `ContinuePrompt::Loop` matches with no
workaround.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `PauseMenu` | 0xD4 | none | pause_menu.cpp, pause_menu_pages_init.cpp; its drawing, input and `InitInfo` are still C (pause_menu_draw.c, _gems.c, _info.c, _loop.c, _pages_draw.c, _powers.c, _widgets.c) |
| `PowerDialog` | 0x2C | none | power_dialog.cpp, power_dialog_draw.cpp; its `Loop` is still C (power_dialog_loop.c) |
| `ContinuePrompt` | 0x24 | none | continue_prompt_init.cpp (the constructor), continue_prompt.cpp (`InitGraphics`, `Loop`), credits.cpp |

- **The classes.** All three are plain classes with a constructor and a
  destructor; their icons are `UiSprite`s. `RunPauseMenu` and
  `ShowPowerDialog` are the static `PauseMenu::Run` and
  `PowerDialog::Show`: `new X`, its loop, `delete`. The destructors are
  `delete icon` for each icon (the C spelled out the slot-10 call with 3),
  and the pause menu's palette cache is `new PaletteCache` and `delete`
  (through casts: gPaletteCache is still a `struct palette_cache *`).
- **The C views stay** for the C files left: pause_menu.h's `struct
  pause_menu` and menus.h's `struct power_dialog`, with size checks in
  menus.hpp. The plain-C files of the two classes (no C++ trait: the pause
  menu's drawing and input, `PowerDialogLoop`) call the methods by their C
  names; the C++ calls them as methods (`InitInfo`, `Loop`), mapped in
  cxx_symbols.txt. menus.h's `struct continue_prompt` went: no C file uses
  it any more.
- **The free functions** in power_dialog_draw.cpp (the four `Show*Dialog`
  wrappers, `GetProgressLives` and the `Count*` tallies of the save block)
  keep C linkage through menus.h; game_frame.c, save_menu_ui.c and
  graphics.cpp call them.
- **menus.h's dead prototypes went:** 91 C names of C++ methods that no C
  file, vtable or C++ file uses (the level select's, the camera lead's,
  the continue prompt's, and the new classes'). 54 are left: the vtables'
  slots, the C callers' and the free functions.
- **The fonts were still C** (C++ since step 10b; src/text/): the font calls stayed spelled out.

What made the C++ match:

- **The blend and display registers** (BLDCNT, BLDY, DISPCNT) are bitfield
  stores into `union blend`, `struct bldy` and a `dispcnt_bits` view, as in
  the level select. Under old_agbcp each constant is loaded before its
  byte (`movs r0, #0xc0; ldrb r1`), and CSE shares the 1 and the 16 with
  DISPCNT's BG0 and OBJ bits, as the ROM does. The C (built with agbcc)
  pinned every one of those registers and spelled the `str; adds #4` of the
  register writes and the constants' order in `asm`; that is most of the
  pins and `asm` this part removes.
- **The icon set-up** (`palette = GetAnimPaletteSlot()`) is a plain
  bitfield store; `UPDATE_ICON_FRAME_NIBBLE` (pause_menu.h, 4 pins and a
  `mov #0x10; neg` asm) and the unused `SET_ICON_FRAME_NIBBLE` went.
- **A sprite's bank** goes through `SetIconBank` (menus.hpp), a store
  through a pointer to the member, as level_select.hpp's `SetBankNow`
  (`bank` is a union member).
- **`slot = new UiSprite` and the slot's address:** `blinkEyes = new
  UiSprite; s = blinkEyes;` computes the field's address before the call
  and keeps the result in r0 for the next stores; `powerIcons[i] = icon =
  new UiSprite` does the same in a loop (the C needed a comma expression).
  The ROM's `subs r4, #8` for `&blinkEyes`, after `r4 = &blend`, is
  reload's move2add (the two pseudos got the same register), not something
  to write: the C wrote `(u8 *)bldcntShadow - 8`.
- **The destructor's three icon loops** each have their own counter: with
  one shared `i`, the counter and the strength-reduced pointer swap r4 and
  r5.
- **`SetEntityPixelPos` is called out of line** in the pause pages: it is
  `Entity::SetPixelPos`, an inline method, so the code calls the C name
  (as level_select.cpp does); the pause menu's eyes use the inline method,
  `SetPixelPos(119, 94)`, which the ROM inlines.
- **The counts** (`CountGoldRelics`, `CountSapphireRelics`, `CountGems`,
  `CountClearGems`, `CountCrystals`) are plain loops over the save block's
  records (`levels[i].h.time`, a `level_save_b` byte with the two gem bits
  named), where the C pinned 18 registers and computed the threshold
  addresses in `asm`.
- **The rows:** `if (gLevelState->timeTrial) rowCount = 5; else rowCount =
  4;` places the literal pool after the `b` as the ROM does; the C needed
  a `.pool` in `asm` (the ternary loads 4 first).

Kept, each with a comment: `ContinuePrompt::Loop`'s `MATCH_USE(audio)`
(one more reference, so `audio` outranks the pair counter for r7) and its
`MATCH_KEEP(k)` (a fresh shift for the START test, whose `u16` result is
the 0 the ROM then stores into `selection`); without either, the loop
differs, also in C++. C idioms kept: `RunPauseMenu`'s `mgr_12c` accessor
(the ROM builds the font's 0x12c offset again for each read), the time
trial page's byte offset into the save block, and `CountGems`' shifts of
the colored-gem flags.

### The title screen (part 10c-2)

Part 10c-2 in numbers: the last two objects of src/frontend/
(title_screen_init.c and title_screen.c, ROM 0x080354E0-0x0803686C), 17
functions: the title screen (`TitleScreen`, include/frontend.hpp) and four
of `CompanyLogos`'s methods. With it every object of src/frontend/ is C++.
Project-wide: `MATCH_HOLD_REG` 1125 -> 1099, instruction-emitting `asm`
114 -> 109 and `MATCH_KEEP` 31 -> 29. Both objects were old_agbcc C
already; title_screen.o loses its `-fno-strength-reduce`, and the
Makefile's `NO_STRENGTH_REDUCE_OBJS` with it.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `TitleScreen` | 0x220 | none | title_screen_init.cpp, title_screen.cpp |
| `CompanyLogos` | 0x44C | none | company_logos.cpp, language_select.cpp, title_screen.cpp (`Run`, `LoadVvLogoGraphics`, `InitVvLogoPieces`, `UpdateVvLogoPieces`) |
| `LogoPiece` | 0x34 | (a struct) | the logo pieces of both screens |

- **The classes.** `TitleScreen` replaces the `u32 *` the C reached the
  title screen through (`self[0x82]`, `self[5]`, `TITLE_SCREEN(self)`):
  its constructor (`InitTitleScreen`) is `new Starfield` and the loads, its
  destructor `delete starfield` and the screen blank, and game_frame.c
  keeps calling `InitTitleScreen(OperatorNew(0x220))`, `RunTitleScreen`
  and `DestroyTitleScreen(p, 3)` by their C names. `CompanyLogos::Run` is
  `new LogoActor(&gLogoActorAnim)` (the class's own `operator new`, the
  C's inline `New`), `part->Update()` and `part->Draw()` (the C spelled
  out the vtable's slots 2 and 3), `delete part`, `new`/`delete
  Starfield` and `delete[]` of the frame buffers.
- **The C views go.** No C file reads the title screen or the company
  logos: `struct title_screen` is an opaque tag (frontend.h),
  `TITLE_SCREEN()` went, and so did logo_screen.h (`struct logo_screen`
  and `struct logo_piece`; the piece is `LogoPiece` in frontend.hpp, with
  its five deltas named). frontend.h's dead prototypes went too: the
  starfield's seven, the title screen's but the three game_frame.c calls,
  `CompanyLogos`'s but `RunCompanyLogos` (level_state.c), and
  `InitLogoActor`, `DrawVvLogoPieces` and `LoadUniversalLogoBg`.
- **The fonts were still C** (C++ since step 10b; src/text/): the font calls stayed spelled out
  (`ICON_TEXT_CALL`, `SetFontTileBase`).

What made the C++ match:

- **Strength reduction is back on for title_screen.o.** Its C needed
  `-fno-strength-reduce` (#64/#65, docs/matching/per-file-flags-investigation.md)
  for `InitVvLogoPieces`, whose hand-written pointer walks only matched
  without it. As C++, the plain indexed loop (`slots[i].countdown =
  gVvLogoPieceSeeds[i].hold + 1; slots[i].record = ...`) matches with it
  on, and the other eleven functions of the file match either way.
- **The per-field inline accessors went.** `UpdateTitleLogoPieces` and
  `UpdateVvLogoPieces` used a one-line `static inline` accessor per field
  (`PosCAt(self, stride)`, `SLOT20_ACCESSOR`; 35 in the two files) so that each field address
  was computed afresh as `self + K + i * 0x34`, plus 9 pins. In C++ the
  plain `pieces[i].posC += pieces[i].deltaC` gives exactly that code, and
  `record = pieces[i].record++` the ROM's r0/r2 split of the record
  pointer, where the C pinned it.
- **`LoadObjTiles`'s five `asm` statements and nine of its ten pins
  went.** The tile remap (`tiles + ((map[i] & 0xff) << 5)`, a plain
  indexed loop), the `ldm r7!` of the next package
  (`(*pkg++)->mapAsset`) and the DMA readback come out as written, with
  the inner loop's DMA base in its own variable (one variable for both
  loads is kept in a callee-saved register). One pin is left, below.
- **The cheat hash** is an inline method, `HashInput(val)`, the body of the
  unused `HashCheatInput` too: the inline's argument is loaded before
  `&cheatHash`, so cross-jumping shares the rest of the seven branches, as
  in the ROM. Its rotate's left shift is `v * 2`: `(v << 1) | (v >> 31)`
  is combined into a `ror`, which the ROM doesn't have (the C pinned four
  registers and kept one in each copy).
- **Smaller ones:** the constructor's palette DMAs are written out (an
  inline with the destination as a parameter shares the constants), the
  fade and timer pointers of `CompanyLogos::Run` and `UpdateVvLogoPieces`
  (`fade = &SLOT_SYSTEM(self)->fade`) are the plain members, and
  `LoadVvLogoGraphics`'s buffers are `frames = buf = new u8[size]` and
  `scratch = new u8[0x1000]`.

Kept, each with a comment: `LoadObjTiles`'s `next` package, pinned to r8.
The ROM keeps `pkg` in r7 only until the map load and the next package in
r8 across the inner loop (`mov r8, r7` ... `mov r7, r8`), where the inner
loop's 0x80000010 reuses r7: a copy in a second variable gives that, but
unpinned it and the pass counter swap r8 and r9. `CompanyLogos::Run`'s r1
pin on the fade-out value, as in the C. C idioms kept: `Run`'s and
`ResetLogoPieces`' seed loops (`goto` loops with their address sums
written out, the `MATCH_USE`s and the `MATCH_CONST` of the C; the natural
loop is strength-reduced), `DrawLogoPieces`' `px + dx` locals and counter
addresses, `LoadBg`'s `bg2cnt = bg2cnt`, and the `u8 *` walk of the
pieces' active flags (`slot[offsetof(TitleScreen, pieces[0].active)]`).

### The 3D actors' base (part 11a)

Part 11a in numbers: actor.c and actor_anim.c (ROM 0x0802A69C-0x0802AC28
and 0x0803B058-0x0803B8B0), polar_player_dispatch.c and the polar player's
pointer-to-member table (src/data/actor_pmf_17a6b8.c), 78 functions and a
table, with the 3D actors' base classes in include/actor_self.hpp and the
first declarations of their subclasses in the new include/vehicle.hpp and
include/boss_actors.hpp. Project-wide: `MATCH_HOLD_REG` 1099 -> 1039,
instruction-emitting `asm` 109 -> 101, `.pool` in asm 5 -> 4, retyped
field stores 194 -> 191 and reads 70 -> 69, `MATCH_CONST` 19 -> 20.
`actor.o` and `actor_anim.o` move to `OLD_AGBCC_OBJS` (128 -> 130);
`polar_player_dispatch.o` stays agbcc.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `AnimPart` | 0x1C | none | actor_anim.cpp (its constructor is inline) |
| `ActorSelf` | 0x54 | gActorVtable (slots 1-3) | actor.cpp |
| `HpActor` | 0x58 | none in the ROM (slots 4-6 added) | actor_anim.cpp (`GetHp`, `Damage`, `IsUnshootable`); the constructor is inline |
| 21 polar actors (`PolarPlayer`, `RiderlessPolar`, `PolarWumpa`, the crates, ...) | | their own (4 slots) | the destructors in actor_anim.cpp, `PolarPlayer::RunState` and its table; the rest still C |
| 14 jetpack actors (`JetpackCheckpointText`, `JetpackShot`, `JetpackBalloonCrate` and its 3 kinds, ...) | | their own (7 slots, 8 for the balloon crates) | the destructors and the checkpoint banner's and explosion's methods in actor_anim.cpp; the rest still C |
| `AirshipFireball` and 5 hovercraft actors (boss_actors.hpp) | | their own (7 slots) | the destructors in actor_anim.cpp; the rest still C |

- **`AnimPart` is the base the ROM implies.** InitActorPart stores the
  keyframes, frames and palette and calls SetActorAnim *before* storing
  gActorVtable, which a g++ constructor only does in a base class's
  constructor; and CreateAirship builds a 0x1C-byte object with the same
  three stores and SetActorAnim (`AllocActor(0x1c)` and `InitAnimPart`,
  airship.c): `new AnimPart(...)`. So `ActorSelf : AnimPart`, with the
  animation methods (`GetAnimFrameBaseOffset`, `GetAnimFrameAttr`,
  `GetAnimFrameData`, `SetAnim`) and the IWRAM `operator new`/`delete` on
  `AnimPart`. The vtable pointer is still at +0x50: `ActorSelf` is the
  first class with virtual methods.
- **`HpActor`** is the jetpack levels' and the 3D bosses' base: the hit
  points at +0x54 (vehicle.h's `struct actor_hp`), and slots 4-6
  (`Damage`, `IsUnshootable`, `GetHp`, whose defaults are DamageActor,
  IsJetpackPlayerUnshootable and GetActorHp). No ROM vtable is its own:
  its constructor is inline (InitHpActor), and its table store is dead
  in every subclass's.
- **The destructor is inline and out of line.** Every subclass's
  destructor expands `~ActorSelf` (the unlink), and the ROM also has it
  out of line, `DestroyActor`, in the middle of actor.c. The class
  declares `virtual ~ActorSelf();`, actor_self.hpp defines it `inline`
  after the class, and actor.cpp, which defines
  `ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE`, has the plain definition at
  DestroyActor's place (the header-fragment idea of part 7e, for one
  function). The 36 subclass destructors in actor_anim.cpp are empty
  bodies: the class's own vtable store is dead before `~ActorSelf`'s and
  goes, as in the ROM.
- **Three destructors are g++'s implicit ones.** The balloon crates'
  kinds (`JetpackHealthCrate`, `JetpackTimeCrate`,
  `JetpackQuestionCrate`) call `~JetpackBalloonCrate`, which is out of
  line (jetpack_crates.c), with no store of their own vtable before the
  call. g++ 2.9 skips that store only when the destructor's body emitted
  no insns at all (`empty_dtor` in cp/decl.c), which an explicit `{}`
  never is (its block note) and a synthesized destructor always is. The
  three classes declare no destructor, and actor_anim.cpp has the
  functions g++ synthesized, with C linkage: `DestroyJetpackBalloonCrate(self,
  0)`, then `AnimPart::operator delete` when bit 0 is set. The other 36
  were probably implicit too, emitted with their vtables; with an inline
  base destructor an explicit empty one compiles the same.
- **The first pointer-to-member table in C++.** `RunPolarPlayerState` is
  `(this->*stateFuncs[state])()`, and gPolarPlayerStateFuncs is
  `const PolarPlayer::StateFunc PolarPlayer::stateFuncs[14] = {
  &PolarPlayer::StateMount, ... }` in src/data/actor_pmf_17a6b8.cpp: g++
  emits the `{0, -1, fn}` records in `.rodata`, byte for byte the C's
  `ACTOR_PMF` table, with no static constructor. The state methods are
  still C (polar_player*.c), mapped to their C names, and
  UpdatePolarPlayer (polar_player.c) still dispatches through the table's
  C view with `ACTOR_PMF_CALL`.
- **The C views stay** for the C files: `struct actor_self`, `struct
  actor_hp` and the prototypes of every converted function (the vtable
  data and C callers use all of them). `gActorList` is an `ActorSelf *`
  to C++ (a `__cplusplus` declaration in globals.h, as `gPlayer`'s).

What made the C++ match:

- **The depth and draw-order key** (InitActorPart, UpdateActor and
  UpdateActorDepth; an inline helper here, with UpdateDepth its
  out-of-line copy) is `depth = ABS_BRANCHLESS(d)` and the key computed
  from `depth`, where the C pinned the value to r2 three times.
- **`UpdateActorPaletteCycle` is plain C++.** The C was one `asm` block,
  for the literal pool the ROM splits after the cursor step's `b`. With
  the step written `if (target - v >= 0) { r = v; if (target != r) r++;
  } else r = v - 1;` the increment comes first, its `b` is the barrier the
  pool goes after, and the decrement after the pool, as in the ROM.
- **`GetWorldBox`** returns the box (g++'s hidden result pointer is the
  C's `out`) and adds the position through a `struct anim_box *` to the
  local copy: the three loads come first and the `sp` copy is the
  `ldrh`/`strh` base, where the C was three `asm` blocks and three pins.
- **`DrawActor`**: the depth in a local (one load for both divisions),
  `screenY = t * proj` from a copy of `y`, and two pointers to the frame,
  the call's result for the height (read through r0) and its copy for the
  width and the OAM call (r7), where the C needed 15 pins and three
  `asm`s (two of them spilling the flags, which g++ does on its own).
- The loops of `IsSpawnCollected` and `MarkSpawnCollected` are plain `for`
  loops over `gCollectedSpawns[i]`, `UpdateActor`'s and
  `UpdateJetpackCheckpointText`'s loop test reads `anims[animIndex]`
  directly (as `LogoActor::Update`), and the `destroy` slot calls are
  `delete this`.

Kept, each with a comment: `DrawActor`'s r5 pin on the projection
(unpinned, it and the screen y swap r4 and r5: global allocation ranks the
projection first, with every spelling tried, and the pin is the C's), and
`DrawJetpackCheckpointText`'s `MATCH_CONST`: the ROM ORs a register holding
0 into the attribute word (`movs r0, #0; orrs r3, r0`), as if an inline
helper's flag argument were 0; g++ folds any spelling of the 0, the C
wrote the `orr` in asm (and pinned 23 registers in that function).

### The airship (part 11i)

Part 11i in numbers: the ten src/bosses/airship*.c files (ROM
0x08030530-0x08031784), 23 functions, and the fireball's pointer-to-member table, split
into src/data/actor_pmf_17c2b8.cpp. Project-wide: `MATCH_HOLD_REG` 1039
-> 1004, instruction-emitting `asm` 101 -> 98, retyped field stores 191
-> 183. Every object matches under the compiler its C had: agbcp, and
old_agbcp for airship_map.o and airship_touch.o (already in
`OLD_AGBCC_OBJS`; neither matches under agbcp). No pins or `asm` are
left in the family but `ConvertAirshipTiles`'s `MATCH_CONST` and
`MATCH_KEEP_MEM`, which are not about C++ (the mask as the AND's first
operand, the height re-read; both tried without, and both still needed).

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `AirshipFireball : HpActor` | 0x6C | gAirshipFireballVtable (7 slots) | airship_fireball.cpp (all but `StateOrbit` and `StateSpiralIn`, still C in jetpack_plane.c), its table in actor_pmf_17c2b8.cpp |
| the airship | 0x1C | none | a bare `AnimPart` (gAirship) and globals; its functions keep their C names |

- **`AirshipFireball`** gets its fields (bosses.h's `struct actor_orbit`
  is its C view, with a `sizeof` check): the orbit's centre, its Z step,
  the radius and the `exploding` flag. Its constructor is `HpActor(rec,
  x, y, z, 2)` and the body's stores, with no pins: the C pinned `b`,
  `c` and `d` to get the stack argument fetched in the ROM's place, which
  g++'s constructor does on its own. `Damage` is `hp -= amount` and the
  inline `SetState(2, 1)`, where the C pinned the state, the index and
  the zeros (5 pins) and stored `animTimer` and `animDone` through casts.
  `Update` is `(this->*stateFuncs[state])()`, then `delete this` once the
  explosion is done or `ActorSelf::Update()`.
- **The table is C++**: no C file reads gAirshipFireballStateFuncs any
  more (its two dispatches were airship_fireball.c's). It was the last of
  actor_pmf_17c260.c's three tables, and that file's other two
  (`JetpackPlane`'s and `JetpackBomber`'s) are 11f's, so the table moves
  to a file of its own, actor_pmf_17c2b8.cpp, linked right after
  actor_pmf_17c260.o (ldscript.txt, data/data.s, docs/data.md). Its two
  flight states are still C (jetpack_plane.c, 11f) and are mapped to
  their C names, as part 11a's polar player states.
- **The airship is an `AnimPart`**: `gAirship` is an `AnimPart *` to
  C++ (a `__cplusplus` declaration in bosses.h, as `gActorList`'s), and
  `CreateAirship`'s inline `AllocActor` and `InitAnimPart` are `gAirship
  = new AnimPart(gAirshipKeyframes, gAirshipMapFrames, 1)`. Everything
  else is globals stepped through gAirshipStateFuncs, a plain function
  table, which C++ calls directly (`gAirshipStateFuncs[gAirshipState]()`,
  the ROM's `bl _call_via_r0`). The functions keep C linkage: their C
  prototypes in bosses.h are what the table, the jetpack files and the
  category vtables use.
- **One state change for the airship**, `SetAirshipState(st, idx)` in
  boss_actors.hpp: the state and its timer, then animation `idx` with the
  frame kept unless it is past the new animation's end. Five copies of
  it were C: three `static inline BossSetState`s and three hand-expanded
  ones (`DamageAirship`, `AirshipStateFall`, `AirshipStateApproach`), each
  with 8 or 9 pins, an `add` in `asm` for the record address and two
  retyped stores. As g++ inlines it, all of them match without.
- `AirshipStateApproach` takes both speed globals' addresses first
  (`s32 *velX = &gAirshipVelX; ...`), where the ROM loads them before the
  zero.
### The jetpack player (part 11e)

Part 11e in numbers: jetpack_spawn.c, jetpack_run.c, jetpack_player.c
(ROM 0x0802E0A4-0x0802F7B0) and jetpack_shot.c (0x0802F97C-0x0802FA38), and
the jetpack player's pointer-to-member table (src/data/actor_pmf_17c1c0.c),
47 functions and a table, all C++ now with no pins and no `asm`. Project-wide (against part 11b): `MATCH_HOLD_REG`
1002 -> 950, instruction-emitting `asm` 95 -> 89, retyped field stores 183
-> 171 and reads 69 -> 65. `jetpack_run.o` moves to `OLD_AGBCC_OBJS` (130 -> 131);
`jetpack_spawn.o` stays there, jetpack_player.o and jetpack_shot.o stay
agbcc (they match under both).

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `JetpackPlayer : HpActor` | 0x58 | gJetpackPlayerVtable (slots 1-4, 6) | jetpack_spawn.cpp, jetpack_player.cpp, jetpack_run.cpp; its state is the gJetpack* globals |
| `JetpackShot : HpActor` | 0x60 (`velX`, `velY`) | gJetpackShotVtable (1, 2, 5) | jetpack_shot.cpp |
| `JetpackCheckpointText`, `JetpackExplosion` | 0x58 | their own | inline constructors (`HpActor(rec, x, y, z, 1)`), expanded by the spawners |

- **`HpActor`'s constructor** is the inline one of part 11a: the subclasses'
  constructors expand it, where the C wrote it out (`InitHpActor`). The
  player's `y` argument is `z == 0 ? -0x9600 : 0`.
- **`new`**: `CreateJetpackPlayer` is `gActorList = p = new
  JetpackPlayer(table, z)`, `SpawnJetpackShot` `new JetpackShot(...)`,
  `CreateJetpackCheckpointText` and `CreateJetpackExplosion` `new
  JetpackCheckpointText(...)`/`new JetpackExplosion(...)`, with their vtable
  stores from the inline constructors, and `SpawnAirshipFireball` `new
  AirshipFireball(...)` (part 11i's class; CreateAirshipFireball's C
  prototype goes). The other kinds `CreateJetpackActor` and the spawners
  build (planes, crates, the hovercraft's weapons, ...) still call their C
  constructors on `AnimPart::operator new` until their classes have their
  fields (parts 11f-11h).
- **The PMF table is C++**: `JetpackPlayer::Update` and `RunState` were its
  only users. `(this->*stateFuncs[state])()` replaces both `ACTOR_PMF_CALL`s.
- **`JetpackShot::Update`** is plain C++: `hit->Damage(2)` and three `delete
  this`. The C needed two `asm` blocks for the `mov rN, #8` of the two
  destroy-slot calls (r3, then r2) and a `goto` into a shared tail: g++ emits
  the null test and the slot call for each `delete` on its own, with the
  ROM's registers.
- **The destructor** is `~JetpackPlayer() { drain; free the tiles; }`: g++
  stores the class's vtable first (the body calls functions), then the
  inline `~ActorSelf`'s gActorVtable and the unlink, as the C wrote out.
- **gJetpackAnimTable** is a `struct anim_table_record *` (vehicle.h; the C's
  `struct kind_entry` was a view of it, `dx`/`dy` are `spawnX`/`spawnY`),
  and `gYeti` an `ActorSelf *` to C++ (a `__cplusplus` declaration in
  vehicle.h, as gActorList's), for `YetiStateStop`, which is in this file
  for ROM order.

What made the C++ match:

- **The animation resets** (`SetState`, and the `animIndex`/`animTimer`/
  `animDone`/`animTime` stores of `YetiStateStop`, `StateBoost`, `PassRing`)
  are plain member stores. The C pinned the duration and the two zeros and
  stored through retyped pointers (45 pins in all).
- **`AllocTiles`** is two `f = CurFrame(this); AllocVramTileBlock(f[1] *
  f[0] * 32)`, as AllocPolarPlayerTiles: the product's copies (`adds r2, r3,
  #0; muls r2, r1; adds r0, r2, #0`) and the second block's `movs r3, #2`
  are old_agbcp's. The C wrote them in two `asm` blocks and 20 pins.
- **`DrawJetpackPlayer`'s tile number** goes in a local, ORed as
  `(palette << 12) | tile`: the operand order decides which of r0 and r1
  holds which (the C pinned the tile to r0).
- **`StateRollLeft`/`StateRollRight`** declare the keys at their
  assignment, after `SteerX()`: assigned to an earlier declaration, the
  struct is copied through two more registers.
- **`PassRing`**: the two speed globals are zeroed through pointers taken
  first (the ROM loads both addresses before the stores), and the ring's
  fifth of the hit points multiplies by a variable (`pct = 0x14`): a literal
  is strength-reduced to shifts, where the ROM has `muls`.

### The hovercraft (part 11h)

Part 11h in numbers: the six src/bosses/hovercraft*.c files (ROM
0x080326E4-0x08034374), 70 functions, and the hovercraft's three
pointer-to-member tables (src/data/actor_pmf_17c450.cpp and
actor_state_17c4c8.cpp). Project-wide (against part 11f): `MATCH_HOLD_REG`
840 -> 735, instruction-emitting `asm` 88 -> 84, `.pool` in asm 4 -> 3,
asm-label aliases 13 -> 12, `MATCH_CONST` 20 -> 16, `MATCH_KEEP` 29 -> 27,
retyped field stores 139 -> 114 and reads 53 -> 51. Every object matches
under the compiler its C had: old_agbcp for hovercraft.o (already in
`OLD_AGBCC_OBJS`, for DrawHovercraftMap), agbcp for the other five, which
match under agbcp as their C did.

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `HovercraftFireball : HpActor` | 0x6C | gHovercraftFireballVtable (7 slots) | hovercraft.cpp; its table in actor_pmf_17c450.cpp |
| `HovercraftCannon : HpActor` | 0x70 | gHovercraftCannonVtable | hovercraft_cannon.cpp; its table in actor_state_17c4c8.cpp |
| `HovercraftLauncher : HpActor` | 0x70 | gHovercraftLauncherVtable | hovercraft_launcher.cpp; its table in actor_state_17c4c8.cpp |
| `HovercraftSideGun : HpActor` | 0x70 | gHovercraftSideGunVtable | hovercraft_side_gun.cpp |
| `HovercraftCannonFlash : HpActor` | 0x5C | gHovercraftCannonFlashVtable | hovercraft_cannon_flash.cpp (its `Damage` in hovercraft_side_gun.cpp) |
| `JetpackRing : HpActor` (vehicle.hpp) | 0x5C | gJetpackRingVtable | hovercraft.cpp (its `Update` is still C, jetpack_crates.c, 11g) |
| `JetpackCollectedWumpa : HpActor` (vehicle.hpp, new) | 0x64 | gJetpackCollectedWumpaVtable | hovercraft.cpp |
| the hovercraft | 0x1C | none | a bare `AnimPart` (gHovercraft) and globals, the airship's twin; its functions keep C linkage |

- **The weapons get their fields** (bosses.h's `struct spawner`, `struct
  actor_orbit` and `struct cannon_flash` are the C views, with `sizeof`
  checks; the side gun's was a file-local struct). Their constructors are
  `HpActor(rec, x, y, z, hp)` and the body's stores, and their states and
  `RunState`s are methods. `delete this` replaces the hand-written
  destroy-slot calls (`_call_via_r2` on `table->destroy`), and the fireball
  hurts the player with `((HpActor *)gActorList)->Damage(6)` (slot 4).
- **The PMF tables are C++**: their only users were these files. The
  fireball's table is all of actor_pmf_17c450.cpp, and actor_state_17c4c8
  holds the cannon's and the launcher's after gHovercraftStateFuncs, a plain
  function table that keeps its C name (bosses.h declares it in `extern
  "C"`), so the whole file is C++.
- **The hovercraft is an `AnimPart`**, as the airship: `gHovercraft` is an
  `AnimPart *` to C++ (a `__cplusplus` declaration in bosses.h),
  `CreateHovercraft` is `gHovercraft = new AnimPart(...)` and
  `DestroyHovercraft` `delete gHovercraft` (AnimPart has no destructor: a
  plain `mem_free`). `RunHovercraftState` calls
  `gHovercraftStateFuncs[gHovercraftState]()` directly.
- **One state change for the hovercraft**, `EnterHovercraftState(st, idx)`
  in boss_actors.hpp (SetAirshipState without the timer):
  `SetHovercraftState` is its out-of-line copy, `HovercraftStateApproach`,
  `HovercraftStateCloseIn`, `HovercraftStateFallBack`, `CreateHovercraft`
  and `SpawnHovercraft` expand it. The C had an inline `SingletonSetKind`
  and two hand-expanded copies with 8 pins and an `add` in `asm` each.
- **The spawners use `new`** (jetpack_spawn.cpp, part 11e's file):
  `new HovercraftCannon(...)`, `new JetpackRing(...)`, ... The side gun's
  `left` is a `bool`, which g++ passes as a byte on the stack (`add r2, sp,
  #4; strb`) and reads back with `ldrb`, so the `CreateHovercraftSideGun_b`
  asm-label alias over a one-byte struct goes. `SpawnHovercraftSideGun`
  takes a `bool` too, in a `__cplusplus` declaration (only C++ calls it): a
  `u8` would be converted (`negs; lsrs`) before the store.
- **`JetpackCollectedWumpa`** is `PolarCollectedWumpa`'s twin with hit
  points. Its destructor is explicit (it adds the fruit it carries), so g++
  stores its own vtable first, then the inline `~ActorSelf`'s, as the C
  wrote out.
- **C views**: the prototypes of the methods no C file uses any more go
  (the states, the `RunState`s, the constructors, gHovercraft*StateFuncs'
  C declarations); the rest stay for the vtable data.

What made the C++ match:

- **The animation resets** are `SetState`, where the C pinned the duration
  and the two zeros and stored through retyped pointers (most of the 105
  pins and all 25 retyped stores).
- **The side gun's constructor** was two `asm` blocks (the vtable store,
  and the X offset's two-way diamond with a `.pool` after its `b`) and 5
  pins. In C++ the hit points are HpActor's argument (`GetHovercraftLevel()
  == 0 ? 0x18 : 0x10`, computed before the base constructor, as in the
  ROM), the X offset an `if`/`else` of two stores (cross-jumping makes the
  diamond, and the pool lands after its `b`), and the animation index
  `idx = 1; if (l != 0) idx = 0;` from a `u8` local.
- **The cannon's and the launcher's `Update`** read `state` into a local
  before the `step = 1`, as the C did; everything else in the two files
  matched as written.

Kept, each with a comment:
- `UpdateHovercraftCannonFlash`'s `MATCH_KEEP_VOLATILE`: the ROM tests the
  0 or 1 its inlined body returns again, and CSE folds the test with every
  spelling tried (a `bool`, a local, empty loops around the call or the
  test), as in the C.
- The flash colour's r1 pin in `SetHovercraftFlashColor` and
  `RunHovercraftState` (the C had 3 and 4 pins, and two `MATCH_KEEP`s): the
  ROM loads the white into r2 and copies it to r1.
- `UpdateHovercraftHitFlash`'s two loop pins (it had eight) and the volatile
  re-read of the timer.
- `ConvertHovercraftTiles`' `MATCH_CONST` and `MATCH_KEEP_MEM`, as
  `ConvertAirshipTiles`' (part 11i).

### The balloon crates, the parachute nitro and the rocket (part 11g)

Part 11g in numbers: jetpack_crates.c (ROM 0x08031A6C-0x08032650), 30
functions, and the balloon crate's pointer-to-member table
(actor_pmf_17c42c.cpp), all C++ now with no pins and no `asm`.
Project-wide (against part 11d): `MATCH_HOLD_REG` 590 -> 528,
instruction-emitting `asm` 81 -> 78, other file-scope `asm` 4 -> 3, retyped
field stores 49 -> 19 and reads 38 -> 19. The object matches under agbcp
and old_agbcp (identical assembly) and stays agbcp.

| Class (include/vehicle.hpp) | Size | Vtable | Code |
|---|---:|---|---|
| `JetpackBalloonCrate : HpActor` | 0x70 | gJetpackBalloonCrateVtable (1, 2, 4, 5, 7) | jetpack_crates.cpp; its balloon, `done`, the sway's centre and phase, `fallSpeed`; its table in actor_pmf_17c42c.cpp |
| `JetpackHealthCrate`, `JetpackTimeCrate : JetpackBalloonCrate` | 0x70 | their own (2, 4) | jetpack_crates.cpp (their destructors are g++'s implicit ones, actor_anim.cpp) |
| `JetpackQuestionCrate : JetpackBalloonCrate` | 0x74 | gJetpackQuestionCrateVtable (2, 4) | jetpack_crates.cpp; the level spawn record |
| `JetpackParachuteNitro : HpActor` | 0x60 | gJetpackParachuteNitroVtable (2, 4, 5) | jetpack_crates.cpp; `dead`, `limitY` |
| `JetpackRocket : HpActor` | 0x68 | gJetpackRocketVtable (2, 4, 5) | jetpack_crates.cpp; the swing's origin, `limitY`, `stepY`, `triggered`, `hit` |
| `JetpackRing` | | | `Update` (the rest is 11h's, hovercraft.cpp) |

- **The kinds expand the crate's constructor; the ROM also has it out of
  line**, uncalled (InitJetpackBalloonCrate, in the middle of the file). One
  file can't have one function both inline and out of line, so the class
  has two: the public `(rec, x, y, z, u8 kind)` one, defined plainly at
  InitJetpackBalloonCrate's place, and a protected inline one with an
  `s32` kind, which the kinds' `: JetpackBalloonCrate(rec, x, y, z, 0x28)`
  picks (an `int` literal is an exact match). Both are `HpActor(rec, x, y,
  z, 2)` and one inline body, `Hang`. The C pinned the hit points to r8 in
  each kind's constructor: the base constructor's argument does it.
- **`new`**: CreateJetpackActor's crates, nitro and rocket are `new X(...)`
  (jetpack_spawn.cpp), byte-identical, and the spawners' `AllocActor`
  goes. `JetpackQuestionCrate` takes the spawn record as a `void *`.
- **The PMF table is C++**: `Update` and `RunState` were its only users.
  `RunState` and the out-of-line constructor have no caller and no pointer
  in the ROM: tagged UNUSED.
- **The calls**: the destroy-slot calls are `delete this`, the player's slot
  4 `Player()->Damage(n)`, `HealJetpackPlayer` & co. `Player()->Heal(0x14)`,
  `QueueWumpa`, `PassRing` (their C prototypes go: no C caller left), the
  balloon's `Release` and `Move` methods (theirs go too), and the kinds'
  tail call `JetpackBalloonCrate::Update()`. jetpack_balloon.cpp's
  `ClearJetpackCrateBalloon(crate)` is `crate->ClearBalloon()`.
- **`~JetpackBalloonCrate`** is an empty body: its own vtable store is dead
  before the inline `~ActorSelf`'s, as in the ROM (the C pinned the unlink's
  two loads).

What made the C++ match:

- **The animation resets** are `SetState` and `RestartAnim` (most of the 62
  pins and all 30 retyped stores). The kinds' `stateTime = kind` and
  `animTime = kind` (the tested 0, reused) are `SetState` after the test.
- **The payout dispatch** is a plain `switch`: the C's `goto` chains were
  the compare tree gcc builds for four cases.
- **The nitro's and the rocket's constructors** pass their fixed `y`
  (`-0xFA00`, `0xFA00`) to `HpActor`. The C needed an `asm` block for the
  nitro's whole `InitActorPart` call, a file-scope `asm` literal pool and an
  inline wrapper for the rocket's: the constant goes through the inline
  base constructor's argument, which puts its load after the stack
  argument's store.
- **The rocket's box** is `box = gJetpackRocketBox` (a struct assignment, the
  ROM's `ldm`/`stm`), where the C copied a `struct vec3_words` view (vehicle.h; its last user, it goes).
- `StateFall` reads `y` into a local before `fallSpeed`, as the C did.
- `DamageJetpackRocket`'s two flag stores are two plain stores, where the C
  stepped the pointer with an `add` in `asm`.

### The polar actors' constructors and the category frame (part 11b)

Part 11b in numbers: actor_factory.c, actor_spawn.c, actor_category_frame.c
and actor_category_select.c, 28 functions, all with C linkage but
`PolarPlayer`'s constructor. Project-wide: `MATCH_HOLD_REG` 1039 -> 1037,
instruction-emitting `asm` 101 -> 98. The objects keep their compilers
(`actor_category_frame.o` old_agbcp, the others agbcp; all three of the
latter match under both).

- **The polar actors get their fields and constructors** (include/vehicle.hpp):
  `PolarLifeCrate::spawn`, `PolarBoostPad::once`, `PolarPenguin`'s
  velocity and targets, and a new class, `PolarCollectedWumpa` (0x60:
  the velocity and the fruit count). `CreateActor` is the per-kind `new`:
  `new PolarTimeCrate(rec, x, y, z)` is AnimPart's inline operator new
  (mem_alloc into IWRAM), then the constructor, with its arguments
  evaluated after the allocation, which is the ROM's order with no
  `AllocActor` wrapper or `REC_AT` index cast.
- **The crate kinds derive from `PolarCrate`.** CreateActor expands their
  constructors (InitPolarCrate, then the kind's vtable store), so they are
  inline over PolarCrate's out-of-line one. Their destructors expand
  PolarCrate's, so `~PolarCrate` is inline too (vehicle.hpp), and
  actor_anim.cpp has its out-of-line copy at DestroyPolarCrate's place as
  a C-linkage function, the deleting destructor g++ would emit with the
  class's vtable (`crate->PolarCrate::~PolarCrate()`, then AnimPart's
  operator delete). The ROM's ~PolarTimeCrate & co. are unchanged: both
  dead vtable stores go.
- **`PolarPlayer`'s constructor** is ConstructActorPart:
  `ActorSelf(rec, 0, z != 0 ? 0x2800 : -0x5000, z)`, then `AllocTiles()`
  (AllocPolarPlayerTiles, still C) and `SetState(0xD, 0xC)` or
  `RestartAnim(8)`. `AnimPart::RestartAnim` is new, SetAnim's body inline
  (the goal's and obstacle's second parts in CreateActor use it too).
  `gActorList = new PolarPlayer(gActorAnimTable, z)` is
  ConstructAnimTableState.
- **The category frame's virtual calls** are `n->Update()` and
  `gActorDrawList[i]->Draw()`, FindShotTarget's slot-5 query is
  `n->IsUnshootable()` on an `HpActor`, and DestroyAllActors is `delete n`
  for each actor and `delete gActorList` for the root.
- **The category vtable stays C:** `struct category_vtable` is 13 plain
  function pointers (two of them hold values), not a g++ vtable; the
  slots are called as `gActorCategoryVtable->fn[5]()`, with casts where a
  slot takes arguments or returns a value.
- **C views:** CreateActor, SpawnActor and SpawnPolarAkuAku return an
  `ActorSelf *` to C++, and gActorDrawList and gHeapSortActorsByKeyFunc
  use `ActorSelf **` (`__cplusplus` declarations in actor.h and
  vehicle.h, as `gActorList`'s). PolarIsTouchingPlayer and
  JetpackIsTouchingPlayer take a `void *` (their only users are the
  category tables). ConstructActorPart's C prototype goes (no C caller).

What made the C++ match:

- **The box tests' `MemCopy32(box, box, 12)` self-copies are g++'s.**
  The ROM copies each actor's moved box into a slot and then copies the
  slot onto itself with memcpy. That is a returned struct bound to a
  `const &` parameter: `BoxOverlap(WorldBox(pl), WorldBox(self))`, with
  `WorldBox` ActorSelf::GetWorldBox's body inline and `BoxOverlap` taking
  `const struct anim_box &`. The C wrote the two calls and kept the boxes
  in a frame struct. (Initialising a local from the call, or passing by
  value, copies once with no memcpy.)
- **DestroyAllActors** is two plain loops of `delete`: the C needed three
  pins and two `asm` blocks for the root's address copied to r5.
- **FindShotTarget's query** tests the low byte of slot 5's result
  (`lsls r0, r0, #24`): `(u8)n->IsUnshootable() == 0`, as HpActor's slot
  returns `s32` (actor_self.hpp; the jetpack slices define its overrides).

Kept, each with a comment: `CanPauseActorCategory`'s r0 pin (the ROM
EORs the 1 into the call's result in r0; g++ copies the result to r1
and builds the 1 in r0 with every spelling tried; the C wrote the two
instructions in asm), `SelectActorCategory`'s `MATCH_USE` (the C's
extra-reference nudge, still needed), and the four `GetActorSpawn*`
accessors' byte-offset arithmetic (`base + 0x18` before the index:
indexing the struct folds the offset into the load).

### The polar player (part 11c)

Part 11c in numbers: polar_player.c, polar_player_states.c and
polar_player_actions.c (ROM 0x0802B364-0x0802C1BC), 29 functions, all
C++ now with no pins and no `asm`. Project-wide (against part 11e):
`MATCH_HOLD_REG` 950 -> 861, instruction-emitting `asm` 89 -> 88,
retyped field stores 171 -> 141 and reads 65 -> 56. The objects keep
their compilers: polar_player.o old_agbcp (it doesn't match under
agbcp), the other two agbcp (they match under both).

| Class | Size | Vtable | Code |
|---|---:|---|---|
| `PolarPlayer : ActorSelf` | 0x54 | gPolarPlayerVtable (slots 1-3) | polar_player.cpp, polar_player_states.cpp, polar_player_actions.cpp (and its constructor in actor_factory.cpp, `RunState` in polar_player_dispatch.cpp); its state is the gPolar* globals |

- **gPolarPlayerStateFuncs has no C user left**: `PolarPlayer::Update` is
  `(this->*stateFuncs[state])()`, and the table's C view
  (`struct actor_pmf gPolarPlayerStateFuncs[14]`, vehicle.h) goes. The 14
  states are methods (part 11a mapped their names).
- **The methods the other polar actors call on gActorList** (`Hurt`,
  `Shock`, `Launch`, `Boost`, `Catch`, `FinishRun`, `QueueWumpa`,
  `GiveLife`, `GiveMask`, `IsPauseLocked`, `AllocTiles`) keep their C
  prototypes in vehicle.h: the polar crates, pickups, objects, Aku Aku and
  the yeti are still C (11d, 11j). The prototypes of the states,
  `DispensePolarWumpa` and `RunPolarPlayerState` go (no C caller).
  `AllocPolarPlayerTiles` takes a `void *`, as `AllocJetpackPlayerTiles`
  does, so actor.cpp's cast goes.
- **`~PolarPlayer`** is the body only (drain the queued wumpas, free the two
  tile buffers), as `~JetpackPlayer`: the vtable stores and the unlink are
  g++'s.
- **`StateMount`** deletes the riderless bear with `delete gRiderlessPolar`
  (the C's `ACTOR_VCALL(..., destroy, 3)`) and restarts its animation with
  `RestartAnim(1)`. gRiderlessPolar and gPolarAkuAku are `ActorSelf *` to
  C++ (`__cplusplus` declarations in vehicle.h, as gYeti's).

What made the C++ match:

- **The animation resets** are `SetState` and `RestartAnim`: 22 of them,
  where the C pinned the state, the index, the zeros and the duration (most
  of the 89 pins) and stored through `*(u16 *)`/`*(u8 *)` casts. Where the
  ROM stores a register it just tested against 0 as the zeros
  (`LaunchPolarPlayer`'s `str r3`, `PolarPlayerStateBoost`'s), `SetState`
  after the test gives it: CSE knows the register is 0 on that path.
- **`StateRun`** is plain if/else: `RestartAnim(0)`, `RestartAnim(1)` when
  `RandRange(3) == 0`, else `RestartAnim(0)`. The C wrote it with five
  `goto`s into shared tails and 17 pins; cross-jumping merges the tails as
  the ROM has them.
- **`Hurt` and `Shock`** take both globals' addresses first (`s32 *timer =
  &gPolarInvulnTimer; ... ActorSelf **aku = &gPolarAkuAku;`), as part 11e's
  `PassRing`: the ROM loads them before the tests and keeps them for the
  else branch. The C pinned all three and shared the `return` with a goto.
- **`StateJump`'s A test** is `(gKeys.all & 1) == 0`; `!(gKeys.all & 1)`
  in the `&&` chain gets an `eor`.
- **The B test before `state = 2`** (`StateRun`, `StateBoost`) is a mask
  variable the AND overwrites: `u32 bit = B_BUTTON; bit = keys &= bit;`.
  With `gKeys.all & 2`, CSE reuses the mask's register for the 2 stored
  into `state` (`str r1`), where the ROM loads it again; the C pinned the
  state to r0.

### The planes, bombers, cannonballs and balloons (part 11f)

Part 11f in numbers: jetpack_plane.c (ROM 0x0802FA38-0x08030530) and
jetpack_balloon.c (0x08031784-0x08031A6C), 43 functions, and three
pointer-to-member tables (actor_pmf_17c260.cpp, and the balloon's, split
into actor_pmf_17c414.cpp), all C++ now with no pins and no `asm`.
Project-wide (against part 11e): `MATCH_HOLD_REG` 950 -> 929, retyped
field stores 171 -> 169 and reads 65 -> 62. Both objects match under
agbcp and old_agbcp and stay agbcp.

| Class (include/vehicle.hpp) | Size | Vtable | Code |
|---|---:|---|---|
| `JetpackPlane : HpActor` | 0x80 | gJetpackPlaneVtable (2, 4, 5) | jetpack_plane.cpp; the hop's velocity, accelerations, steps and next target, the cannonball cooldown, `dying` |
| `JetpackBomber : HpActor` | 0x64 | gJetpackBomberVtable (2, 4, 5) | jetpack_plane.cpp; the home point, `unshootable` |
| `JetpackCannonball : HpActor` | 0x60 | gJetpackCannonballVtable (2, 5) | jetpack_plane.cpp; the velocity |
| `JetpackBalloon : HpActor` | 0x64 | gJetpackBalloonVtable (2, 4, 5) | jetpack_balloon.cpp; its crate, `dying`, `velY` |

- **Constructors and `new`**: each constructor is `HpActor(rec, x, y, z,
  hp)` and the body's stores. The C pinned the hit points (and the
  bomber's arguments and stack argument, 6 pins) to get them loaded before
  InitActorPart, which the inline base constructor's argument does. The
  bomber's kind is `(u8)rec->index`, an `ldrb` of the record's first
  byte. `CreateJetpackActor`'s planes and bombers, `SpawnJetpackBalloon`
  and `SpawnJetpackCannonball` (jetpack_spawn.cpp) are `new X(...)` now,
  byte-identical, where 11e called the C constructors on `AllocActor`.
- **The PMF tables are C++**: the five `ACTOR_PMF_CALL`s are `(this->*stateFuncs[state])()`.
  gJetpackBalloonStateFuncs was the middle table of actor_state_17c3fc.c,
  between the airship's function table (C, kept there) and the balloon
  crate's (11g's, still read from C): it moves to actor_pmf_17c414.cpp
  and the crate's to a new C file, actor_pmf_17c42c.c, both linked after
  actor_state_17c3fc.o (ldscript.txt, data/data.s, docs/data.md). The
  three objects' `.rodata` and relocations are the old one's.
- **The airship fireball's two flight states** are `AirshipFireball`
  methods: the C's split locals (centre, target, player) stay, its three
  pins go.
- **`DestroyAirship` is `delete gAirship`**: `AnimPart` has no destructor,
  so the `delete` is its inline operator delete (mem_free) with no null
  test, as the ROM has it.
- **The balloon's crate** is a `JetpackBalloonCrate *`, and `Damage` breaks
  it off with `crate->Break()`, slot 7 (BreakJetpackBalloonCrate): the C
  read `vtable->m38` by hand. `JetpackBalloonCrate` (11g's class) gets that
  one declaration.
- **Other rewrites**: the `ACTOR_SET_STATE`s are `SetState`, the slot
  calls `PlayerActor()->Damage(n)` and `delete this`, the anim step of
  `Move`, `StatePop` and `StateFloatAway` one inline method (`Animate`)
  reading `(s16)animTimer` (the C's retyped read), and `Update`'s `goto`
  into the shared `delete` two plain `delete this`.
- **Prototypes**: vehicle.h keeps the vtable data's and jetpack_crates.c's
  (as `void *`); the states', the constructors', `AimJetpackPlane`,
  `HomeJetpackBomber`, the `Run*State`s and the tables' go, with the
  file-local `struct jetpack_plane`/`_bomber`/`_cannonball`/`_balloon`/
  `actor_fa38` views. bosses.h's fireball state prototypes go.
- `ClearJetpackBalloonCrate` (`JetpackBalloon::ClearCrate`) has no caller in
  the ROM: tagged UNUSED.

Kept: `JetpackPlane::Aim`'s `scale2` assigned inside the X term (the C's
spelling): the ROM doubles the scale after the GetActorSpawnX call, and
assigned before it, the shift is scheduled first.

### The polar crates, pickups, hazards and Aku Aku (part 11d)

Part 11d in numbers: polar_pickups.c, polar_nitro.c, polar_crates.c,
polar_objects.c and polar_aku_aku.c (ROM 0x0802C1EC-0x0802D5D4, between
polar_player_dispatch.cpp and yeti_update.c), 51 functions, all C++ now
with no pins and no `asm`. With them every polar class is C++.
Project-wide (against part 11h): `MATCH_HOLD_REG` 735 -> 590,
instruction-emitting `asm` 84 -> 81, retyped field stores 114 -> 49 and
reads 51 -> 38. The objects keep their compilers: polar_nitro.o old_agbcp
(it doesn't match under agbcp), the other four agbcp (they match under
both).

| Class (include/vehicle.hpp) | Size | Vtable | Code |
|---|---:|---|---|
| `PolarCollectedWumpa : ActorSelf` | 0x60 | gPolarCollectedWumpaVtable (1-3) | polar_pickups.cpp: flies to the HUD, its destructor counts in its fruit |
| `PolarWumpa : ActorSelf` | 0x54 | gPolarWumpaVtable (1, 2) | polar_pickups.cpp |
| `PolarCrate : ActorSelf` | 0x54 | gPolarCrateVtable (1, 2) | constructor (polar_crates.cpp), `Update` (polar_pickups.cpp), `Break` (inline) |
| its 7 kinds (`PolarTimeCrate`, ..., `PolarLifeCrate` 0x58) | | their own (1, 2) | `Update` (polar_crates.cpp, polar_pickups.cpp); `PolarNitroCrate::Detonate`, `DetonateNearby` (polar_nitro.cpp), `Explode` (inline); the constructors in include/polar_crate_ctors.hpp |
| `PolarElectricFence`, `PolarObstacle`, `PolarLauncher`, `PolarPenguin` (0x68), `PolarIcicle` | | their own (1, 2) | polar_objects.cpp |
| `PolarAkuAku : ActorSelf` | 0x54 | gPolarAkuAkuVtable (1, 2) | polar_objects.cpp (`Refresh`, `Update`, `Move`), polar_aku_aku.cpp (the masks, constructor) |
| `PolarGoal`, `PolarBoostPad` (0x58), `PolarCheckpointCrate` | | their own (1, 2) | polar_aku_aku.cpp |

- **The crate kinds' constructors are inline and out of line** from one
  source: include/polar_crate_ctors.hpp, a header fragment with no include
  guard (part 7e's crate_line_step.hpp). vehicle.hpp includes it with
  `POLAR_CRATE_CTOR` `inline` (CreateActor expands them, part 11b), and
  polar_crates.cpp, which defines `POLAR_CRATE_CONSTRUCTORS_OUT_OF_LINE`,
  includes it at its end with the macro empty, for the ROM's
  CreatePolarTimeCrate & co. (no caller). `PolarWumpa`'s one constructor
  uses the `~ActorSelf` way (part 11a): inline in vehicle.hpp under
  `#ifndef POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE`, plain in
  polar_pickups.cpp (CreatePolarWumpa).
- **The constructors** are `ActorSelf(rec, x, y, z)` and the body:
  InitPolarCrate's look by place (the C's explicit `__divsi3` is `/
  0x14`), the icicle's by `(u8)rec->index`, the boost pad's by side, Aku
  Aku's offset position and mask level. The C pinned 21 registers for
  them.
- **The animation resets** are `RestartAnim` and `SetState`; a crate's
  break is `Break()` (`RestartAnim(0x12)`) and a nitro's `Explode()`
  (`stateTime = 0` too). The C pinned the duration and the zeros in
  nearly every one and stored through `*(u16 *)`/`*(u8 *)` casts. Where
  the ROM stores a register just tested against 0 (the launcher's, the
  penguin's, the nitro's yeti path, the checkpoint crate's), the inline
  after the test gives it, as part 11c found.
- **The `destroy` slot calls** (4 hand-written `_call_via_r2` calls and an
  `ACTOR_VCALL`) are `delete this`, and the calls on the player are
  `static_cast<PolarPlayer *>(gActorList)->Hurt()` and the like.
- **`DrawPolarCollectedWumpa`** (25 pins and an `asm` for the dead `flag
  = 0` in r8) is ActorSelf::Draw's tail at a constant scale:
  a static inline `DrawScaledFrame(this, x >> 8, y >> 8, 0x140)` with
  DrawActor's double-size and affine tests. The tests fold, and the
  dead zero, the `h << 3`/`halfW << 1` asymmetry of the culling and the
  ORed 0x100 come out as the ROM has them.
- **Switch trees**: `PolarQuestionCrate::Update`'s 9 gotos are a plain
  `switch` on the record (cases 0x1C-0x1F), and `PolarLauncher::Update`'s
  5 a `switch (state)`. The icicle's two steps are two `if`s with the
  anim switch and `state + 1` each, merged by cross-jumping.
- **gPolarAkuAku and SpawnPolarAkuAku are `PolarAkuAku *`** to C++
  (vehicle.h's `__cplusplus` declarations), so polar_player.cpp and
  polar_player_actions.cpp call `gPolarAkuAku->Move(...)`,
  `AddMask()`, `RemoveMask()` and `ClearMask()`, where 11c called the C
  names with a cast. `IsTouchingYeti` (yeti_graphics.c) takes an
  `ActorSelf *` to C++ the same way.
- **Prototypes**: vehicle.h keeps the vtable data's (the `Update`s, the
  collected wumpa's `Draw` and destructor), and the two C-linkage
  getters; the constructors', InitPolarCrate's, the Aku Aku methods',
  `AimPolarPenguin`'s, `DetonateNearbyPolarNitros`' and
  `DetonatePolarNitroCrate`'s go, with `struct polar_life_crate` and
  `struct actor_once`. Of PolarPlayer's, `HurtPolarPlayer`,
  `ShockPolarPlayer`, `LaunchPolarPlayer`, `BoostPolarPlayer`,
  `QueuePolarWumpa`, `GivePolarPlayerLife` and `GivePolarPlayerMask` go
  (these files were their last callers); `CatchPolarPlayer`
  (yeti_update.c), `AllocPolarPlayerTiles`, `FinishPolarRun` and
  `IsPolarPauseLocked` (actor.cpp's and actor_spawn.cpp's C-linkage
  hooks, which call their jetpack twins the same way) stay.

What made the C++ match, beyond the above:

- **`DetonateNearby`'s overlap test** goes through a `u8` inline,
  `ActorsOverlap(this, n)` (BoxOverlap of the two WorldBoxes, as in
  actor_category_frame.cpp): written as `BoxOverlap(...)` in the `&&`
  chain, the result is stored in a register and tested again.
- **`PolarCollectedWumpa`'s constructor** adds the BG centre as `x +
  GetActorBgCenterX()` and reads `y` again after the first `abs`, as the
  ROM does.
- **The boost pad's side** is `if (side < -0x14) idx = 0; else { idx = 1;
  if (side > 0x14) idx = 2; }`: initialised to 0 first, the 0 is loaded
  before the -0x14.

Kept, each with a comment: the electric fence's four hits and the
penguin's two knock-aways are written out (through an inline helper,
PlaySfx's two constant arguments are loaded in the other order), and
`PolarAkuAku::Move`'s `goto` into the shared Y/Z easing (the ROM's one
copy; with the targets set per branch and one easing after them the
registers differ). The BoxOverlap/WorldBox inlines are a copy of
actor_category_frame.cpp's (file-local there).

### The entity family is done

With 7b', part 7 is complete: `Entity` and every class built on it whose
code is in the family's files (`Sprite`, `UiSprite`, `MovingSprite`,
`GroundSprite`, `PartList`, `Crate`, `CrateList`, `Platform`,
`PlatformMover`, the pickups, `HudPart`), and every controller, are C++.
The C++ conversion so far, #685 to part 7b' (`tools/match_idioms.py`
counts, project-wide; `tools/cpp_survey.py`):

| | #685 | now | change |
|---|---:|---:|---:|
| objects built from C++ (`CXX_OBJS`) | 1 | 74 | +73 |
| functions in them | 5 | 793 | +788 |
| classes with a ROM vtable (`_vt.` names in cxx_symbols.txt) | 1 of 93 | 36 of 93 | +35 |
| objects in `OLD_AGBCC_OBJS` | 94 | 118 | +24 (agbcc objects whose clean C++ matches only under old_agbcp) |
| `MATCH_HOLD_REG` pins | 2151 | 1404 | -747 |
| instruction-emitting `asm` | 249 | 160 | -89 |
| `MATCH_USE` | 83 | 60 | -23 |
| `MATCH_KEEP` | 61 | 34 | -27 |
| `MATCH_HOLD` | 23 | 18 | -5 |
| `MATCH_CONST` | 30 | 26 | -4 |
| asm labels | 20 | 17 | -3 |
| `.pool` in asm | 7 | 6 | -1 |
| file-scope asm blocks | 5 | 4 | -1 |
| retyped field stores | 228 | 202 | -26 |
| retyped field reads | 96 | 82 | -14 |
| scoped `volatile`s | 50 | 10 | -40 |

The 74 objects: bosses 7, crates 19, enemies 6, gfx 2, objects 20, pickups 3,
player 17. `MATCH_BARRIER` (17), the clobbers (3), the memory barriers (2)
and the `BOX_ADDR`s (12) are unchanged. Every workaround a C++ object kept
then is listed, with its reason, in its part's notes above; #662 has
removed many of them since (see docs/matching_techniques.md, "Pruning
workarounds").

**What is still C**: since [the final cleanup](#the-final-cleanup),
nothing with a C++ trait. Every item this list had (the level select's
sprites, the enemy spawners, the 3D actors, the background layers and
fonts, the C prototypes of C++ methods, the save menu, the pause menu's
and the power dialog's C files, the cutscene player's C files, the text
box's wrapper, the HUD and the room code) is done, and since the "C++
everywhere" PRs (#746-#748) every game file is C++: only lib/ and
src/data/ stay C, as listed there.

### Emitting the vtables (step 10)

**85 of the 93 vtables are emitted by g++** from the class declarations
since step 10, and the other 8 since [step
10b](#the-last-c-vtables-step-10b). Each emitted table comes out byte for byte
as the ROM has it, so the compiler now checks the hierarchy: a slot out
of order, a missing override or an extra virtual would change the bytes
and fail `make compare`. The report stays 2059/2059 functions and 100%
data.

**The mechanism.**

- **Where g++ puts a vtable.** Without `#pragma interface`, g++ 2.9
  emits a class's vtable in the object that defines the class's *key
  method*: its first virtual method, in declaration order, that is
  declared in that class and isn't inline. The table goes in a section
  of its own, `.gnu.linkonce.d._vt.<len><Class>`, flagged `"aw"`, under a
  weak symbol `_vt.<len><Class>`. The contents are the ROM's: slot 0 is
  `{0, 0, 0}` (with `-fno-rtti`) and every slot is `{0, 0, fn}`, with the
  base's methods where the class doesn't override them. A class whose
  key method is C (until step 10b, `Ctrl`: `Update` was `UpdateCtrl` in
  system/boot.c) or defined in no object gets no vtable anywhere.
- **The names.** objcopy's `--redefine-syms` (cxx_symbols.txt) already
  renamed `_vt.<len><Class>` to the C table's name, and the relocations
  in the table to the C names of the methods, so nothing else changes:
  the constructors and destructors store the same symbol, and the
  remaining C files' references still resolve. Only the section keeps
  its mangled name.
- **The placement.** ldscript.txt lists the 93 tables one by one, in ROM
  order (0x087E3BEC-0x087E55E4), each as `<key-method
  object>(.gnu.linkonce.d._vt.<len><Class>)` (until step 10b, a C one as
  `entity_vtables_7e3bec.o(.rodata.<name>)`, in a section of its own
  from include/vtable.h's `VTABLE_SECTION`; both went). Naming the object
  makes the build check that g++ emitted the table where expected: in
  another object, the link fails. A `.gnu.linkonce.d` section the script
  doesn't list is still dropped by `/DISCARD/`: HpActor's, the one
  emitted table with no ROM counterpart (nothing refers to it).
- **The out-of-line inline methods.** An object that gets a class's
  vtable also gets an out-of-line copy of every inline method of the
  class (the [gotcha](#dead-ends-and-gotchas) graphics.cpp shows). The
  ROM has them only in graphics.o (Entity's, with `DestroyEntity` last).
  In 17 other key-method objects they would be new code: inline
  constructors (`Sprite(u16, u16, u16)`, `LaunchPad`'s, ...), accessors
  (`MovingSprite::Pos`, `Player::StoreSlippery`, ...), `ActionCtrl`'s 12
  queue helpers, and the copy-like `DingodileProjectileCtrl(rocket)`.
  Those 17 are built with g++'s **`-fno-implement-inlines`**, the
  Makefile's `NO_IMPLEMENT_INLINES_OBJS`, which drops exactly these
  copies: each object's code is the same as with `#pragma interface`.
  This snapshot of g++ drops the inline *virtual* ones too, so a table
  that points at an inline destructor gets an undefined reference
  instead: `PolarCrate`'s and the balloon crate kinds' implicit ones.
  Their ROM copies are the C-linkage `DestroyPolarCrate` and
  `DestroyJetpack{Health,Time,Question}Crate` in actor_anim.cpp, and
  cxx_symbols.txt maps the mangled destructors (`_._10PolarCrate`, ...)
  to them.
- **The report.** tools/report_units.py reads an emitted table as a data
  blob of its object, like a src/data table (its symbol is weak, `w`,
  not `g`); each one is a 100% data unit named after its `.cpp`.

**The tables**, by the object that emits them:

| Object | Tables | Classes |
|---|---:|---|
| `src/actor/actor_anim.cpp` | 35 | every polar, jetpack and boss actor whose destructor is here: `RiderlessPolar` ... `PolarCheckpointCrate`, `JetpackCheckpointText` ... `JetpackRing`, `AirshipFireball`, the hovercraft's five weapons |
| `src/bosses/cortex.cpp` | 6 | `UnusedOneShotAnimCtrl`, `CortexBossGemCtrl`, `CortexBossPlatformMover`, `CortexShotCtrl`, `CortexTargetCtrl`, `CortexBossCtrl` |
| `src/bosses/dingodile.cpp` | 5 | `CortexCannonCtrl`, `DingodileSharkCtrl`, `DingodileProjectileCtrl`, `DingodileShieldCtrl`, `DingodileCtrl` |
| `src/vehicle/jetpack_crates.cpp` | 4 | `JetpackBalloonCrate` and its three kinds |
| `src/enemies/enemy_ctrl.cpp`, `src/pickups/wumpa.cpp`, `src/bosses/tiny_hop_pad.cpp`, `src/menus/level_select.cpp`, `src/vehicle/polar_pickups.cpp` | 2 each | `PeriodicSpawner`, `KnockedEnemyCtrl`; `Wumpa`, `Stopwatch`; `StompedHopPadCtrl`, `OneShotAnimCtrl`; `CameraLead`, `LaunchPad`; `PolarCollectedWumpa`, `PolarCrate` |
| `src/cutscene/cutscene_player.cpp`, `src/util/aabb_setup.cpp` (step 10b) | 2 each | `BgStreamer`, `BgLayerBase`; `LargeFont`, `SmallFont` |
| 29 others (4 of them since step 10b) | 1 each | `Entity` (graphics), `Sprite`, `UiSprite`, `MovingSprite`, `GroundSprite`, `Player`, `EnemyCtrl`, `EffectCtrl`, `Crate`, `ExtraLife`, `ActionCtrl`, `PlayerCtrl`, `InputCtrl`, `BossCtrl`, `MegaMixCtrl`, `TinyCtrl`, `Platform`, `PlatformMover`, `LevelSelectEntry`, `HudPart`, `ActorSelf` (gActorVtable), `PolarPlayer`, `JetpackPlayer`, `JetpackCollectedWumpa` (hovercraft.cpp), `LogoActor`; `Ctrl` (system/boot.cpp), `BgLayer` (level/bg_layer.cpp), `PooledBgLayer` (level/tile_slot_pool.cpp), `Font` (text/font.cpp) |

**Still C after step 10** (src/data/entity_vtables_7e3bec.c), 8 tables
whose class's key method was C code: `gCtrlVtable` (`Ctrl::Update` was
`UpdateCtrl`, an empty function in system/boot.c), the background layers'
four (`gBgStreamerVtable`, `gBgLayerBaseVtable`, `gBgLayerVtable`,
`gPooledBgLayerVtable`) and the fonts' three (`gLargeFontVtable`,
`gSmallFontVtable`, `gFontVtable`). Step 10b converted their classes
([below](#the-last-c-vtables-step-10b)); the file is gone.

The headers that still have `#pragma interface` (crate_list.hpp,
menus.hpp, spawners.hpp) have no class with a vtable.

**What the ROM's order says.** Within one object, g++ writes the
tables in the order the ROM has them (cortex.cpp's six, actor_anim.cpp's
35). But the ROM interleaves some objects' tables: `TinyCtrl`'s
(tiny_update.cpp) and `CortexCannonCtrl`'s (dingodile.cpp) sit among
cortex.cpp's, and `PolarPlayer`'s, `PolarCollectedWumpa`'s,
`PolarCrate`'s, `JetpackPlayer`'s, the balloon crates' and
`JetpackCollectedWumpa`'s among actor_anim.cpp's. If the original
linked each table with its key-method object, as g++ does, those
classes' key methods were in the same file as their neighbours'
(cortex.cpp's, actor_anim.cpp's) there, and the reconstruction's file
split or method order differs. The explicit placement keeps the bytes
right either way; moving those methods is a possible follow-up, not
needed for the match.

### The last C vtables (step 10b)

**Every vtable is g++'s now.** Step 10b converted the classes of the 8
tables step 10 left as C data, and src/data/entity_vtables_7e3bec.c
went, with include/vtable.h's `VTABLE_SECTION` and `VTABLE_SLOT` (the
header keeps `struct vtable_slot`, the layout the C files read a vtable
through). The report stays 2059/2059 functions and 100% data.

- **`Ctrl::Update`** is the empty `UpdateCtrl` in system/boot.c, the
  ROM's second object (after crt0). boot.c is boot.cpp now (agbcp, its
  three functions unchanged, `DivMod`'s SVC asm and pins with them), and
  `void Ctrl::Update(MovingSprite *) {}` is Ctrl's key method, so g++
  emits gCtrlVtable in boot.o. No function moved.
- **The BG layers** (include/bg_layer.hpp): the tile-map ring buffer
  `BgStreamer` (gBgStreamerVtable: only its destructor), `BgLayerBase`
  (gBgLayerBaseVtable: the position, the parallax step and the
  streamer), `BgLayer` (gBgLayerVtable: a hardware BG layer's resident
  window) and `PooledBgLayer` (gPooledBgLayerVtable: BG layer 0, its
  tiles through the VRAM tile-slot pool), and `LevelLayers`, the
  singleton that owns them (no vtable). bg_scroll_layer.h's struct
  bg_scroll_layer stays the C view of a layer (the C++ files that read
  `gLevelLayers->layer0` keep it too); `struct bg_streamer`,
  `struct pooled_layer` and the method-table structs went.
- **The fonts** (include/font.hpp): `Font` (gFontVtable) and its
  `SmallFont` and `LargeFont` (their own vtables: they override only the
  destructor). bitmap_font.h's struct bitmap_font stays the C view, for
  the C files (the text box only, since [the final
  cleanup](#the-final-cleanup)). The C++ files see gSmallFont and gLargeFont as `Font *`s (text.h,
  under `__cplusplus`) and make virtual calls where they spelled out the
  record's slots: the front end, the menus, the level select, the
  continue prompt and the cutscene player.

**Out-of-line copies of inline methods.** Twice the ROM showed the
pattern graphics.cpp has (a class's inline methods emitted at the end of
the object that gets its vtable, in the reverse of their declaration
order), and it says which functions were inline and where a file ended:

- bg_layer.c ended `GetBgLayerScreenIndex`, `WrapBgLayerColumn`,
  `WrapBgLayerRow`, the four BGnCNT setters and a getter,
  `WriteBgLayerOffsetRegs`, `WriteBgLayerCntReg` and `DestroyBgLayer`,
  none of them called. They are `BgLayer`'s inline methods (the
  destructor declared first, `GetScreenIndex` last, after the wraps it
  calls), and the code that had them written out in place uses them now
  (`DrawColumn`'s `GetScreenIndex(col, rowLo)`, `Reset`'s
  `WriteCntReg()`, ...). With the destructor inline, `BgLayer`'s key
  method is `Reset`, in bg_layer.cpp, and g++ emits the copies at its end
  in the ROM's order. `PooledBgLayer`'s methods, which followed in the C
  file, are pooled_bg_layer.cpp. The inline setters also gave
  `InitPooledBgLayer` its `& 0x7f` before the `| 0x80`: through an
  inline's parameter a constant bitfield store is a general insert (the
  part 9 gotcha), which the C wrote as an asm block.
- font.c ended `InitFont`, `FontHeightToLines`, `FontGetTileCount`,
  `FontSetPos`, `FontNewLineAt`, `FontGetMargin`, `FontSetMargin`,
  `FontGetY`, `FontGetX`, `FontSetTileBase` and `DestroyFont`, none
  called: `Font`'s inline constructor, accessors and destructor. The
  subclasses' constructors expand the constructor (the C's
  `InitIconManager`), their destructors the destructor (the C's two
  stores to `+0x130` in a row, each behind an asm address anchor), and
  the callers the accessors (`HeightToLines` is RunCutscenePlayer's
  `__udivsi3`, `SetTileBase` the menus' slot-6 helpers). With the
  destructor inline, `Font`'s key method is `MeasureText`, which was
  font_measure.c, the object before font.c: the two are one object,
  font.cpp (old_agbcp, as font_measure.c was; font.c's code matches under
  it too).

**Per object** (`tools/match_idioms.py`'s kinds, C -> C++):

| Object | Classes | Compiler | Workarounds: C -> C++ |
|---|---|---|---|
| `src/system/boot.cpp` | `Ctrl::Update` (ctrl.hpp), with `DivMod` and `MemCopy32` (C linkage) | agbcp | 3 pins, 1 asm (DivMod's SVC) -> the same |
| `src/cutscene/cutscene_player.cpp` | `BgStreamer`, `BgLayerBase`'s constructor, destructor, `ClampScrollStep`, `ClampScrollMax`, `ScaleScroll`, `StepScroll`, with the cutscene player's 4 functions (C linkage) | old_agbcp | 0 -> 0; 4 vtable stores, 4 slot calls -> 0 |
| `src/level/bg_layer_base.cpp` | `BgLayerBase`'s `Scroll`, `Reset`, `SetSource` and accessors, with the terrain tile cache's lookups (C linkage) | old_agbcp | 1 use -> 1 use (`DecodeCollisionChunk`'s: without it r3/r4 swap) |
| `src/level/bg_layer_init.cpp` | `BgLayer`'s constructor, `GrowRows`, `GrowColumns`, `ClipColumns`, `ClipRows` | **old_agbcp** (was agbcc) | 9 pins, 6 asm, a file-scope asm pool -> 0; 5 slot calls -> 0 |
| `src/level/bg_layer.cpp` | `BgLayer`'s other methods, and the copies of its inline ones | old_agbcp | 0 -> 0; 6 slot calls, a vtable store -> 0 |
| `src/level/pooled_bg_layer.cpp` (new) | `PooledBgLayer`'s overrides, `ReleaseColumn`, `ReleaseRow`, with `nullsub_26` | old_agbcp | 0 -> 0 |
| `src/level/tile_slot_pool.cpp` | `PooledBgLayer`'s constructor, destructor, `GetPriority`, with the tile-slot pool (C linkage) | **old_agbcp** (was agbcc) | 4 pins, 3 asm, 3 vtable stores -> 2 pins, 1 asm (`AcquireTileSlot`'s residency test: the ROM loads 0x200 before the entry and loads the entry again; neither a plain test nor an inline with the constant as a parameter does). #662 step 2: the plain `pool->slotForTile[id] != TILE_SLOT_NONE` test matches now; the asm and both pins went |
| `src/level/level_layers.cpp` | `LevelLayers` | agbcp | 0 -> 0; 6 slot calls (the layers' `delete`s, `Scroll`s and `Reset`s) -> 0 |
| `src/text/font.cpp` (font_measure.c and font.c) | `Font`'s `MeasureText`, `UploadTiles`, `SetPalette`, `ResetPalette`, and the copies of its inline methods | old_agbcp (font.c was agbcc) | 5 pins, 2 keeps, 2 asm, 1 asm label, 2 slot calls, 2 vtable stores -> the asm label (`GetPaletteSlot`'s `s32` return) |
| `src/text/font_glyph.cpp` | `Font::DrawGlyph`, `PutChar`; `SmallFont`'s and `LargeFont`'s constructors | old_agbcp | 8 pins, 3 asm, gotos, 2 slot calls, 3 vtable stores -> 0 (`PutChar` is a `switch`, as `MeasureText` is) |
| `src/text/font_draw_text.cpp` | `Font::DrawText`, `MeasureChars` | old_agbcp | 3 pins, gotos, 2 slot calls -> 0 (a `switch`) |
| `src/text/font_draw_chars.cpp` | `Font::DrawChars` | agbcp | 0 -> 0; 2 slot calls -> 0 |
| `src/text/font_height.cpp` | `Font::TextHeight` | agbcp | 0 -> 0 |
| `src/util/aabb_setup.cpp` | `LargeFont`'s and `SmallFont`'s destructors, with `SetAabbSize`, `SetAabbPos`, `GetLives` (C linkage) | agbcp | 4 asm, 4 vtable stores -> 0 |
| the font callers: credits.cpp, language_select.cpp, title_screen.cpp, title_screen_init.cpp, continue_prompt.cpp, level_select.cpp, pause_menu.cpp, power_dialog.cpp, power_dialog_draw.cpp, cutscene_player.cpp, spawn_pickups.cpp | virtual calls and the inline accessors for the fonts' record-slot calls (`ICON_TEXT_CALL`, `_call_via_rN`) and their own copies of the accessors (`SetFontPos`, `SetFontTileBase`, `mgr_12c`, ...); `new SmallFont`/`new LargeFont` | (unchanged) | 0 -> 0; 31 spelled-out slot calls (`ICON_TEXT_CALL`s, `_call_via_rN`s and the helpers') -> 0 |

**In numbers** (project-wide, `tools/match_idioms.py`): `MATCH_HOLD_REG`
528 -> 501, instruction-emitting `asm` 78 -> 61, `MATCH_KEEP` 27 -> 25,
file-scope asm blocks 3 -> 2, retyped field reads 19 -> 17;
`OLD_AGBCC_OBJS` 131 -> 134 objects (bg_layer_init.o, tile_slot_pool.o
and the new pooled_bg_layer.o; font.o replaces font_measure.o); 14 more
C++ objects (153 -> 167 `.cpp` files under src/).

### The final cleanup

**#664 is complete.** No game object outside lib/ is C with a C++ trait
any more: `tools/cpp_survey.py --objects` listed 201 objects under src/
built from C++ source and 49 C-like ones (plus the data tables) then, and
the only traits it still finds in C files are plain function pointers
(the IRQ table, the yeti's state table, the sprite-frame cache hook,
GAX's mixer), which aren't C++. The report stays 2059/2059 functions
and 100% data.

- **The dead C declarations.** 391 C prototypes had no user left in a
  .c, .cpp or .s file, ldscript.txt, sym_*.txt or another header: the C
  names of C++ methods whose last C callers were converted (objects.h
  111, player.h 118, crates.h 65, gfx.h 31, bosses.h 23, pickups.h 16,
  frontend.h 16, actor.h 9). They went, with the C structs whose only use
  was a class's size check (struct ctrl, part_ctrl, orbit_part, ...: the
  classes check the ROM sizes now) and orbit_part.h. The conversions
  below removed about 100 more (each family's notes list them). The C
  names that data tables, the linker script or C files use stay.
- **The dead macros.** `ACTOR_SET_STATE` (ActorSelf::SetState since part
  11), `CALL_HIT` (box_part.h), `MATCH_USE_VOLATILE` and
  `MATCH_CONST_VOLATILE` had no user. tools/match_idioms.py still counts
  and rejects the two spelled-out volatile forms; a site that needs one
  would add its macro back. The save menu's conversion took
  `PART_METHOD`/`struct part_method` and `ICON_TEXT_CALL` with it.
- **The small follow-ups.** `BoxOverlap`/`WorldBox` are actor_self.hpp's
  (actor_category_frame.cpp and polar_nitro.cpp had identical copies),
  and the vehicle category hooks (actor.cpp, actor_spawn.cpp) and
  hovercraft.cpp/airship_explode.cpp call the vehicle players' methods
  directly, so those methods' last C prototypes went.
- **The last C files with C++ traits**, one family per commit: the pause
  menu's and the power dialog's C files and DrawWrappedText (the font
  callers), the save menu (`SaveMenu`, include/save_menu.hpp), the HUD
  (`Hud`, include/hud.hpp) and the sprite pieces, the level and cutscene
  destructors (`TileCache`, `LevelEntityFlags`, `Slideshow`,
  `CutscenePlayer`, `LevelState`'s constructor and destructor), the game
  frame, the level state's file, and the room code. Their rows are at the
  end of the [Progress](#progress) table; the notes follow.

Every object is byte-identical to origin/main's except
save_menu_input.o, whose code and relocations are identical but whose
symbol table differs (the undefined symbols' order, and DrawMain's size:
the C's inline-asm transcription put its literal pool inside the
function, g++ puts it after).

**In numbers** (project-wide, `tools/match_idioms.py`, before -> after
the cleanup): `MATCH_HOLD_REG` 501 -> 411, instruction-emitting `asm` 61
-> 49, `MATCH_BARRIER` 17 -> 14, `MATCH_HOLD` 13 -> 12,
`MATCH_MEMORY_BARRIER` 2 -> 1, `MATCH_CLOBBER` 3 -> 2, `.pool` in asm 3
-> 2; `OLD_AGBCC_OBJS` 134 -> 139 objects (hud_lives.o, entity_flags.o,
pause_menu_powers.o, play_room.o, save_menu_ui.o); 167 -> 201 `.cpp`
files under src/.

**What stays C**, on purpose:

- lib/ (GAX2, AgbEeprom, libgcc, the BIOS wrappers): third-party code
  that was C, linked as-is.
- src/data/: the ROM's data tables (no code; C or C++ gives the same
  bytes).

Every game source file under src/ outside src/data/ is C++ since the
"C++ everywhere" PRs (#746 gfx/system/util/text, #747 actor/vehicle/level,
#748 link/save/iwram, including the IWRAM ARM routines with
`agbcp_arm_patched`); they replaced the list of 49 C-like objects this
section had.
- Possible follow-ups, not needed for the match: ~~level_state.cpp's 87
  functions as `LevelState` methods~~ (done in
  [#750](#the-level-state-as-a-class-750)),
  ~~graphics_package.cpp's BgSetup as a class~~ (done, #753), the link session
  as a `LinkSession` class (`InitLinkSession`/`DestroyLinkSession` are
  its `new`/`delete`), and the C views of classes that no C file reads
  any more (struct player and others): they can go now that every game
  file is C++.

**The pause menu, the power dialog and DrawWrappedText (cleanup A).** The
last C files of PauseMenu and PowerDialog (menus.hpp) are their methods
now, so both classes are all C++, and DrawWrappedText (the font
callers' last C file in src/text/) is C++ with C linkage. The 32 font
slot calls (`_call_via_r2`/`_call_via_r3` through `struct icon_record`)
are Font's virtual `MeasureText`, `DrawText`, `DrawChars`, `MeasureChars`,
`DrawGlyph` and `PutChar`, and the files' calls into each other method
calls; the new method names are one block at the end of cxx_symbols.txt.
menus.h's 27 prototypes of these methods, pause_menu.h's struct
pause_menu and menus.h's struct power_dialog went (no C user left;
menus.hpp checks the classes against the ROM sizes, 0xD4 and 0x2C).
pause_menu.h keeps struct settings_icon_actor for save_menu_draw.c.
`MATCH_HOLD_REG` 501 -> 488, instruction-emitting `asm` 61 -> 60.

What made them match:
- **DrawPowersPage** matches as plain C++ under old_agbcp (the object
  moves to OLD_AGBCC_OBJS): its four flag tests load the mask before the
  byte, the old compiler's order, which the C pinned 12 registers to get
  out of agbcc.
- **CommitFrame** writes `dispcnt.raw` plainly: the C's asm address anchor
  (pinned to r0, so `self`'s register wasn't reused for the address) isn't
  needed.
- **DrawRows** casts MeasureText's `s32` result to `u32` before halving
  it (the C's slot call returned `u32`: `lsr`, not `asr`).
- **DrawWrappedText**'s `/b` handler reads the position as
  `self->SetPos(self->GetX(), self->GetY() + 4)`: through the inline
  getters' `this` the reads share the loop-hoisted `&posX`/`&posY` with
  the stores, as the C's address-returning accessors did. With the C's
  accessors kept, the hoisted `&posY` spill lands inside the loop
  instead of before the first test (38 lines off).
- **FormatVolume** is FormatVolumePercent with its ignored first argument
  as `this` (InitInfo passed `(s32)self`).

Kept, each with its comment: DrawPauseMenu's two r2 holds over the
computed-x SetPos calls (without them y and x swap registers), Loop's
`key`/`pressed` pins (r3/r1; without either the input tests change, 8-18
lines), DrawWrappedText's `MATCH_USE(len)` (r7 priority; 340 lines
without) and r1 hold (48 lines without), FormatDecimal's r5 pin (a
plain copy of `value` since #662 step 2; the two holds' `MATCH_HOLD`s
went there too).

Left C: src/text/text_box.c (GetWordLength, DrawWrappedTextInBox: no C++
trait; the box wrapper only reads two font fields).

**The save menu (cleanup B).** The save menu is class SaveMenu (include/save_menu.hpp, `#pragma
interface`: no vtable; 0xE4 bytes), the object OpenSaveMenu `new`s into
gSaveMenu (save.h declares it as a `SaveMenu *` under `__cplusplus`) and
CloseSaveMenu deletes: the C's `if (gSaveMenu) DestroySaveMenu(gSaveMenu,
3)` is `delete gSaveMenu`, and DestroySaveMenu's `if (flags & 1)
OperatorDelete(self)` is the g++ destructor's own. Its 40 methods map to
their C names in cxx_symbols.txt; save.h keeps only the C-linkage
functions game_frame.cpp calls (OpenSaveMenu, RunSaveMenu, CloseSaveMenu)
and the save transfer's three accessors; struct save_menu (save_menu.h) is
now only a tag. The slot list's 15 icons are `UiSprite *`s (`new
UiSprite`, the C's `InitUiSpriteObj(OperatorNew(0x40))`); their
PART_METHOD slot calls are `rowObjA[i]->Update()` (slot 3) and `delete
rowObjA[i]` (slot 10, with its null test). The save transfer (struct
save_transfer) and the save data stay C structs, `new`/`delete`d as
PODs: save_data.c and save_transfer*.c have no C++ trait.

What made it match: DrawMain, two inline-asm transcriptions of the ROM in
the C, is plain C++ once the option's label is loaded inside the
MeasureText argument (after the vtable lookup, as in the ROM) and the
stack-passed `struct byte_arg` is set before the loop: its 0 then has no
register of its own and is rematerialized after the argument slot's
address (`mov r1, sp; movs r0, #0; strb`), as in the ROM; set at the call,
the 0 comes first. The constructor matches with the two saves' addresses
taken up front (`cartSaveAddr`/`linkSaveAddr`, the C's pinned r9/r8) and
no pin. In LinkInput the C's local `state` would shadow the member: it is
`result`. save_menu_ui.cpp matches without LoadBg's r1 pin under
old_agbcp, so it moved to OLD_AGBCC_OBJS.

Kept: InitIcons' frame-0 address pin and r1 hold (2 pins, 1 hold, 1 use;
without them the frame-0 icon's tag store and palette mask take other
registers), and DrawYesNoPrompt's `y` pin (r9; without it the 0x87 and
0x130 swap registers). The C's three MATCH_BARRIER()s of insn-count
padding in InitIcons go. The `affine` store stays a plain `u16 *` store:
written as a field store, old_agbcp's read-modify-write leaves a dead
zero that loop.c hoists instead of the 0x80.

Gone with the conversion: box_part.h's PART_METHOD and struct
part_method, bitmap_font.h's ICON_TEXT_CALL (both had no other user), and
objects.h's InitUiSpriteObj prototype.

`MATCH_HOLD_REG` 501 -> 484, instruction-emitting `asm` 61 -> 58,
`MATCH_BARRIER` 17 -> 14, `.pool` in asm 3 -> 2; OLD_AGBCC_OBJS +1
(save_menu_ui.o).

**The HUD, the sprite pieces and the BG package loader (cleanup C).** The HUD (gHud, 0x68 bytes, no vtable) is class Hud in include/hud.hpp;
struct hud_counter (hud.h) stays its C view for the C callers left
(bonus_round.c, actor_category_init.c, actor_vram_pool.c), which keep the C names (cxx_symbols.txt block "#664
cleanup C: the HUD"). `new HudPart[35]` and `delete[] parts` reproduce the
ROM's hand-written array construction (count word, constructor loop) and
destruction (each part's virtual destructor, slot 10, back to front, then
OperatorDeleteArray) exactly. The only workaround added is SET_PART_BANK
(hud.hpp), a retyped store of Sprite's `bank`: it is a union member (alias
set 0), so a plain store makes gcc reload `parts`, and the menus'
pointer-to-member helper (SetIconBank) moves the store's base register; the
retyped store in place gives `str rN, [part, #0x20]`. UpdateHudLives was the
most pinned function of the HUD (34 pins, 5 instruction asm): as plain C++
under old_agbcp it matches once the second digit's `__modsi3` is computed
before its part's address (moved to OLD_AGBCC_OBJS). DrawAffinePieces keeps
MATCH_USE(pa) (without it the pa/pd registers swap). graphics_package.cpp
keeps C linkage: making BgSetup a class means changing its C++ callers'
`InitBgSetup(&bg, ...)` to constructors (level select, pause menu, power
dialog, continue prompt, language select, the save menu); a follow-up. Project-wide (match_idioms.py): MATCH_HOLD_REG 501 -> 466,
instruction asm 61 -> 56, MATCH_HOLD 13 -> 12. Nothing in this family was left C.

**The level and cutscene destructors, the game frame and the level state
(cleanup D1).**

- **The destructors.** A g++ destructor with nothing to tear down is
  `X::~X() {}`: g++ passes `__in_chrg` and frees `this` on bit 0, the
  `if (flags & 1) OperatorDelete(self)` the C spelled out. A derived
  class's (CutscenePlayer's, DestroyCutscenePlayer) passes its
  `__in_chrg` unchanged to the base's, which frees: the C's forwarding
  trampoline. An object destroyed at scope end gets `__in_chrg` 2
  (PlayCutscene's `DestroyCutscenePlayer(&f.pager, 2)`).
- **Deriving from the C struct.** TileCache, LevelEntityFlags and
  LevelState (until #750, which made it the whole class) are `class X :
  public <c struct>`: the C struct stays the
  one field list, the lookups that take the struct (bg_layer_base.cpp,
  collision_map.c, the level-state accessors) take the class through the
  implicit conversion, and `new X` / `delete p` match the ROM.
- **`new` and an empty out-of-line constructor** (ShowCompanyLogos):
  g++ keeps `__builtin_new`'s result in r0 across the constructor call
  and copies it to a callee-saved register only after, which the C could
  only write as an asm `bl` with r0 pinned.
- **`delete` of a class with no destructor** (KeyInput, gInput) calls
  `__builtin_delete` with no null test; the ROM tests, so the destructor
  keeps `if (gInput != NULL)`.
- **Struct return.** An inline returning a struct by value goes through a
  stack temporary in g++ (PlayCutscene's frame grew by 0x10); the pair
  is set through an inline taking a pointer instead.
- **ShowSlidePicture** needed no workaround as a C++ method under
  old_agbcp, only the DISPCNT shadow rebuild's mask and byte in `s32`
  locals (word AND, `movs #17; negs`, not a byte `0xef`).
- **Left as C-like code:** level_state.cpp's 87 functions stay C-linkage
  functions taking `struct level_state *` (until #750: making them LevelState methods
  is mechanical but touches every C caller's names; their 49 pins are
  plain register-allocation workarounds, and removing them all changes
  most functions); entity_flags.cpp's SetEntityIdActivated keeps its pins
  and asm (5 plain variants tried under old_agbcp). With the HUD's class
  (cleanup C) merged, game_frame.cpp's `InitHud(operator new(0x68))` is
  `new Hud` and its `DestroyHud(gHud, 3)` `delete gHud` (still
  byte-identical), and level_state.cpp calls the Hud's methods.
- **Gone C prototypes:** InitTileCache, DestroyTileCache, SetEntityIdGone,
  SetEntityFlagsPos, InitEntityFlags, DestroyEntityFlags, DestroyUnusedLevelObject,
  InitUnusedLevelObject, BeginSlide, RunSlideshow, SkipSlides, ShowSlidePicture,
  EndSlide, DestroySlideshow, ResetSlideshow, InitSlideshow,
  InitTitleScreen, RunTitleScreen, DestroyTitleScreen, InitLevelState,
  DestroyLevelState, DestroyOamBuffer, DestroyObjVramCursor,
  DestroyPaletteCache, DestroySpriteBankSet, DestroyPaletteCycles,
  DestroySpriteRenderer, DestroyCompanyLogos, RunCompanyLogos (and struct
  title_screen's tag).

Project-wide (tools/match_idioms.py): MATCH_HOLD_REG 501 -> 490,
instruction-emitting asm 61 -> 58, MATCH_MEMORY_BARRIER 2 -> 1.
OLD_AGBCC_OBJS +1 (entity_flags.o). 8 more `.cpp` files.

**The room code (cleanup D2).** The four files of the room loop are C++,
all under old_agbcp. room_frame.cpp needed nothing beyond the two virtual
calls. room_entities.cpp walks the crate list as `Crate *`s; its one
`MATCH_HOLD_REG` (the first search's id in r1) is still needed: without it
r0/r1 swap at the search's load, under both compilers. run_room.cpp
matched once the wait loop's flag test was written `(gPlayer->f.bytes.flags
& 1) == 0`: `!(... & 1)` in C++ comes out as an `eor`/`and` pair (the C's
`!` folded to a `beq`). Its post-fade hard-register hold (2 pins, 2
`MATCH_HOLD`s, 2 `MATCH_USE`s, keeping r0/r1 live while the player pointer
is loaded) is still needed: plain, or as two field copies, the global's
address and the player land in r0/r1. The `AddPaletteCycle_fx` asm-label
alias (the one-byte BLKmode struct argument) stays too. play_room.cpp is
plain C++ with no workaround: it matches under old_agbcp only (agbcp loads
the flag byte before the `0x10`), so play_room.o joins `OLD_AGBCC_OBJS`;
the ROM's locals are kept (each controller case reads `gPlayer` once into
`pl` after the sprite bank, and the two constant stores, `tag = 0x1f` and
`ctrlMode = 3`, go through `u8` locals so the constant is loaded before
the field's address). No cxx_symbols.txt entry was needed (every mangled
name was already mapped). The C prototypes these files were the last
users of went (InitPlayer, ResetPlayerForRoom, InitActionCtrl,
CreateInputCtrl, InitPlayerCtrl, SetCtrlAnimSet, InitPartList,
DestroyPartList, InitCrateList, DestroyCrateList, GetLevelLayers,
DestroyLevelLayers, SpawnEntity, GetPlatformExitMirror, GetCrateAbove,
SetCrateAbove, SetCrateBelow), with player.h's `struct player_vtable`
(`struct player`'s `vtable` is a `const void *` now). Project-wide:
`MATCH_HOLD_REG` 501 -> 487, `MATCH_CLOBBER` sites 3 -> 2, instruction asm
61 -> 61. No file of the family was left C.

**The class families' leftover C views (#656 batches 2-5).** After the
conversion, the C views of the classes (`struct ctrl_target`, `struct
gobj`, `struct box_part`, ...) had almost no C user left: the .cpp files
still read through them. `tools/layout_audit.py views` lists them.

- **Batch 2, the controllers.** The two state-table files are C++
  (src/data/action_table_16bf20.cpp: `ActionCtrl::stateFuncs`;
  player_pmf_16c250.cpp: `PlayerCtrl::stateFuncs` and
  `InputCtrl::stateFuncs`), so player.h's 53 C prototypes of the
  controllers' and the player's methods (the state methods, KillPlayer,
  DoSuperBodySlamShockwave, CollidePlayer, SetPlayerBusy: the C++ callers
  call the methods), the three `struct actor_pmf` table externs, and `struct actor_pmf`/`ACTOR_PMF` (actor_self.h) went. The
  objects are byte-identical, symbols included. With them went the
  controllers' C views: player_ctrl.h (`struct player_ctrl`), player.h's
  `struct input_ctrl` and `struct act` tag, gobj_1a794.h's `struct mover`
  and `struct mover_vtable`, and part_ctrl.h (`struct ctrl_target`): the
  classes check the ROM sizes (0x30, 0x28, 0x38) instead. EnemyCtrl's
  `target` and `popup` are `MovingSprite *`s (the `target`/`sprite` union
  went); the enemy files read the class's fields (`f.b.gone`,
  `mirrorFlags.mirrorX`, `rampX.start`, `bank->anims[tag].frameCount`,
  ...). The six enemy objects are byte-identical, the `MATCH_HOLD_REG`
  pins (now on `MovingSprite *`s) unchanged.
- **Batch 3, the 3D actors.** The last three .cpp users of `struct
  actor_self` take the classes: FindShotTarget is `HpActor
  *FindShotTarget(ActorSelf *)` (declared in actor_self.hpp, C linkage;
  JetpackShot::Update passes `this`), and DestroyPolarCrate takes a
  `PolarCrate *` (vehicle.hpp). actor.h's GetAnimFrameAttr and
  GetAnimFrameData prototypes had no user (the C++ calls the AnimPart
  methods). `struct actor_self` is now only the yeti's and the IWRAM
  sorter's C view (and ActorSelf's size check). The objects are
  byte-identical.
- **Batch 4, the dead views.** gobj_1a794.h's `struct gobj` and `struct
  gobj_vtable` (GroundSprite's and Platform's C view: only their size
  checks used it, which compare with 0x80 now; player.h's `carried` is a
  `void *`), gfx_part.h's `struct gfx_part`, `struct anim_bank` and
  `struct anim_record` (the header keeps `struct gfx_vec` and
  `PART_FLAG_SET`), and level_menu.h with its `struct sprite`,
  `sprite_f28` and `sprite_vtable` (spawn_objects.cpp's
  SpawnLaunchPadEntity calls `LaunchPad::Spawn`, so menus.h's
  SpawnLaunchPad prototype went too). Six more C prototypes of C++
  methods had no user but the method calls of the same name
  (UpdateTntCountdown, PlayerHitboxOverlapsAt, AddPaletteCycle,
  AttachCtrl, CheckPlayerContact, ResolvePlayerContact). Found by
  deleting each candidate and compiling everything: the 51 others are
  still called by their C names from .cpp files (batch 5 turns the
  ones on a class's object into method calls). The C-linkage
  destructor copies (DestroyJetpackHealthCrate & co.) need their
  prototypes for the linkage.
- **Batch 5, the entity family's casts.** The globals the C++ files cast
  have their classes under `#ifdef __cplusplus`, as gPlayer already did:
  gLevelLayers (`LevelLayers *`), gCrateList (`CrateList *`), the five
  part lists (`PartList *`), gPaletteCycles, gPaletteCache, gOamBuffer,
  gObjVramCursor, gSpriteBankSet and gSpriteRenderer. The part lists,
  the crate list and the palette cycles have no C user, so they have no C
  declaration at all; the others keep their C views for the C files.
  Their C++ users call the methods instead of the C names
  (`DrawPartList(gX)` is `gX->Draw()`, `GetPaletteSlot(gPaletteCache, id)`
  `gPaletteCache->GetSlot(id)`, `CommitOamBuffer(gOamBuffer)`
  `gOamBuffer->Commit()`: about 230 calls), and read the classes' fields
  (`gCrateList->slots[i]`, `gLevelLayers->layer0` as a `BgLayer *`). The
  sprite helpers the controllers and bosses called by their C names on a
  `void *` (ResetSpriteFrameTimer, ResetSpriteFrameIndex,
  SetSpriteAnimDone, ClassifySpriteContact, GetSpriteHitbox,
  GetSpriteAnimPaletteSlot) are method calls too, the crates' (BreakCrate,
  ExplodeCrate, IsCrateKindBreakable) as well, and the 30 `(struct actor
  *)` casts went: GetSpriteAnimPaletteSlot is `p->GetAnimPaletteSlot()`;
  SetEntityPos and SetEntityPixelPos are graphics.cpp's out-of-line copies
  of inline methods, which the ROM calls from the other files (a method
  call would be inlined), so entity.hpp declares them taking an `Entity
  *` for the C++ files. The collision queue holds `Crate *`s
  (`collision_candidate.neighbor` under `__cplusplus`,
  `CollisionQueue::Add(Crate *, ...)`, cxx_symbols.txt updated); its
  resolve keeps calling ApplyCrateCollision, now declared in crate.hpp
  with a `Crate *`, because the method's bool flags are passed as words
  where the ROM stores one-byte structs. With that, these C views had no
  user left and went: box_part.h's `struct box_part`, `struct keyframe`
  and `struct part_list`, crates.h's `struct pool_manager` and `struct
  pool_node`, gfx.h's `struct palette_cycler` (its notes are
  PaletteCycles' now), crate.h's `struct crate` (its field notes are
  Crate's), and pause_menu.h with `struct settings_icon_actor`; 35 more C
  prototypes of methods (DrawPartList, UpdateCrateList, LoadRoom,
  GetPaletteSlot, AddOamEntry, ...) had no caller left. Every object is
  byte-identical. What still casts: the actor list's root
  (`(JetpackPlayer *)gActorList`, a downcast), gPlayerCtrl and gInput
  (`void *`s their C and C++ users pass around as such), gEntityFlags
  (LevelEntityFlags derives from the C struct, and entity.hpp's inline
  methods use it before spawners.hpp can be included) and the DISPCNT
  shadow bytes.

- **Batch 6, one name per field.** The C views that C files still read
  have their class's field names and types, and the class header checks
  every named field with `ASSERT_VIEW_FIELD(tag, Class, view, f)`
  (core.h: `COMPILE_TIME_ASSERT(tag, offsetof(Class, f) == offsetof(struct
  view, f))`; agbcp takes offsetof of a class with a vtable pointer and of
  a base class's field): struct player (player.hpp; the +0x0C flags are
  Entity's `union EntityFlags f`, now in actor.h for both, so `hit`,
  `flag4`, `flag6` and `flag7` are `bit3`, `active`, `vulnerable` and
  `collides`; `slot` is `palette`, `ctrl` `mover`, `hitMask` an `s32`,
  `maskTrail` `struct gfx_vec`s, and the halves, raw sizes, `affine` and
  `unk_40` are named as in the class), struct actor_self (actor_self.hpp;
  `box` is a `struct anim_box`, which moved to actor_self.h, so the yeti
  files lost their casts), struct bg_scroll_layer and struct level_layers
  (bg_layer.hpp), struct bitmap_font (font.hpp), struct collision_queue
  (part_list.hpp), struct sprite_bank_set (sprite_obj.hpp), the camera's
  struct camera_target (sprite_obj.hpp, against Sprite: `dirFlags` and
  `flags` are `dir` and `mirror`), and the two codegen views, struct
  cutscene_player (cutscene.hpp; PlayCutscene's stack aggregate, which
  calls the constructor and destructor by their C names) and crates.h's
  struct pool_init_node (crate_list.hpp; ResetGrid's zeroing view). The
  vtable pointer has no name in a class, so the views' `vtable`/`record`
  is unchecked, as are bitfields. The views whose fields no C file reads
  are tags now, their notes moved into the classes: actor.h's struct actor
  (Entity), hud.h's struct hud_counter (Hud), gfx.h's struct
  oam_shadow_buffer, struct palette_cache and struct vram_upload_cursor
  (OamBuffer, PaletteCache, ObjVramCursor; vram_pool.h had nothing else
  and went) and level.h's struct entity_spawner (EntitySpawner). player.h's
  `union player_flags`, `union player_mirror`, `struct player_pos`,
  `struct act_anim_record` and `struct act_anim_bank` went with the old
  field types. Every object is byte-identical.

- **Batch 9, the plain-C record copies.** One type per layout, merged
  where every object stays byte-identical:
  - the sprite bank: Sprite's `anim`/`bank` union has one member, `const
    struct sprite_bank *bank` (still a union, for the alias set 0 its
    stores were compiled with: hud.hpp's SET_PART_BANK), so
    gobj_1a794.h's `struct anim_table` and `struct anim_rec` went (about
    60 `->anim` stores and `->anim->records[...]` reads are `->bank`
    and `->bank->anims[...]`; `frames` is `frameCount`);
  - actor_self.h's `struct actor_method` and `struct actor_vtable`:
    actor_self's `vtable` points at vtable.h's `struct vtable_slot`s;
  - the DISPCNT copies: gfx.h's `union dispcnt` (`raw`, `bits`) replaces
    level_select.hpp's `LevelSelectDispcnt`, menus.hpp's `MenuDispcnt`
    and the continue prompt's and language select's anonymous unions;
  - the level spawn record: actor.h's `struct actor_spawn` (with its
    offsets: the tail of a struct sub_effect_record), which
    jetpack_spawn.cpp's `struct jetpack_spawn_rec` was a copy of
    (`kind[0..2]` are `kind`, `altKind`, `bonusKind`);
  - the {x, y} pairs: aabb.h's `struct vec2` replaces gfx_part.h's
    `gfx_vec`, objects.h's `e08c_pos`, level.h's `probe_pos`, hud.h's
    `hud_pos`, menus.h's `xy_pair` and `icon_pos`, pickups.h's
    `orbit_vec`, gobj_1a794.h's `pos2` and the file-local `text_vec`,
    `lk_point` and `gl_point` (cxx_symbols.txt's two mangled names with
    `7xy_pair`/`8e08c_pos` say `4vec2`); level_cutscene.cpp's `struct
    text_rect` is a `struct aabb`; gfx.h's `struct piece_offset` is
    sprite_bank.h's `struct sprite_piece_pos`;
  - the OAM entries: font_glyph.cpp's `struct glyph_oam`, credits.cpp's
    `struct popup_oam` (byte and halfword units) and graphics_package.cpp's
    `struct oam_attrs_u16` (halfword units; its comment said gfx.h's word
    units changed a DrawScaledSprite store, which old_agbcp's C++ doesn't
    do) are gfx.h's `struct oam_attrs`;
  - pause_menu_loop.cpp's `struct pause_keys` is `gKeys.half`.

  Kept, each with its comment: swim_ctrl.cpp's `struct keys` (its
  zero-length array makes the copy BLKmode, on the stack, as in the
  ROM), gfx.h's `struct piece_info` (it reads the piece count as the
  `tiles` word's top byte), actor_anim.h's `struct sub_effect_table_end`
  (the 12 bytes after a spawn table's last record), crates.h's `struct
  pool_init_node` and cutscene.h's `struct cutscene_player` (batch 6).
  run_room.cpp's `gl_input` had already gone.

`tools/layout_audit.py views` on origin/main listed 90 view pairs; after
batches 2-5 it lists 49, none of them a C view a C++ file reads through;
after batches 6 and 9, 34 (`--min 1`: 273 -> 75), and `names` 7 rows in 3
families (20 in 4 before): the C views and their classes agree, and what
is left is unrelated layouts of the same shape (a box's `w` and a
star's `dx`, the crate grid's node and the sprite-frame cache's) and
CameraLead's own field at GroundSprite's `type`.
`MATCH_HOLD_REG` stays at 411 and instruction-emitting `asm` at 49:
the retyped code compiled to the same bytes with the pins as they were.

### The audio context

src/audio/audio.c, listed above as C-like, was C++: the music and
sound-effect front end is class AudioContext (include/audio.hpp, no
vtable, `#pragma interface`), and audio.cpp is its methods. The survey
missed it because a class with no vtable has no vptr store, so its
constructor and destructor don't look like ones. The traits:

- **The destructor** takes `__in_chrg` and frees on bit 0
  (DestroyAudioContext's `StopSong(self); gGaxIrqEnabled = 0; if (flags &
  1) IwramFree(self)`), and its one caller tested the pointer and passed
  3 (`if (gAudioContext != NULL) DestroyAudioContext(gAudioContext, 3)`):
  `delete gAudioContext`.
- **The constructor** returns `this` (InitAudioContext), and its one
  caller allocated with IwramAlloc first (`InitAudioContext(IwramAlloc(0x2094))`):
  `new AudioContext`, with the class's own inline operator new and delete
  calling IwramAlloc and IwramFree (the IWRAM heap, as ActorSelf's).
- **The state tests** every method repeats (`flag = 0; if (state == 1)
  flag = 1; if (flag)`) are inline `bool` methods (IsStopped, IsPlaying,
  IsPaused). In StartSong and StopSong the ROM builds the flag in one
  register and copies it to a second, whose known 0 is then stored to
  `state` and `gGaxIrqEnabled`: that copy is the inline's return value.
  The C needed a `MATCH_HOLD_REG` for it in each function (`wasStopped`
  in r1, `isStopped` in r2); `if (!IsStopped())` gives it with no pin.
  PlaySfx's `MATCH_USE(&gSfxVoiceToggle)` (an extra reference that lifted
  the global's address above `self` and `id` in global-alloc's ranking)
  isn't needed either, nor its `pself` copy of `self`: with `this`, the
  ranking comes out as the ROM's.
- **DisableMusicVCountIrq** takes the context and ignores it: a method
  (`DisableVCountIrq`) that doesn't touch `this`. EnableMusicVCountIrq
  and MusicVCountIrqHandler (`gAudioContext->Update()`) keep C linkage:
  they take nothing, and the IRQ table points at the handler.

The whole object matches on the first try under old_agbcp; agbcp gives
the same methods and differs only in the two DISPSTAT updates (it loads
the byte before the mask), which is why audio.o was an old_agbcc object
already. PlayAmbientSfx's fifth argument is a `bool` (the stack-passed
byte the C spelled as a `struct byte_arg`); its two C++ callers pass a
literal: through a `bool` local, the saucer's 0 got a register of its own
that the later stores reused, where the ROM rematerializes it after the
argument slot's address (enemy_ctrl_update.cpp). yeti_states.c keeps its
asm-label alias.

struct audio_context (audio.h; it was `struct AudioContext`, now the
class's name) is the field list and the C view. The C files left,
save_data.c and yeti_states.c, call PlaySfx, GetCurrentSong, PlaySong and
StopSong by their C names; audio.h keeps only those prototypes (the 21
others went). The 75 C++ files that called the C names make method calls
(`gAudioContext->PlaySfx(SFX_JUMP, 0x100)`, about 300 calls), and every
object is byte-identical.

Kept: TickAmbientSfx's volatile read of `ambientSfxVolume` after its store
(the ROM reloads it; plain, CSE passes the stored value's register and
the zero after it moves). Not converted: save_data.c's LoadSaveData and
StoreSaveData test `audio->state == 1` with the same flag shape, so they
are probably C++ that inlined IsPlaying too; the save data has no other
trait, and they stay C for now.

`MATCH_HOLD_REG` 89 -> 87, `MATCH_USE` 47 -> 46 project-wide
(tools/match_idioms.py); 203 -> 204 `.cpp` files under src/.

### The last C files: gfx, system, util, text

The 21 C-like game files of src/gfx/, src/system/, src/text/ and
src/util/ are C++ now (98 functions; the "What stays C" list of [the
final cleanup](#the-final-cleanup) predates this): bitmap_screen,
display, fade, fade_to_black, sprite_frame; asset, input, irq, main,
main_loop, memory; text_box; aabb, fixed_math, line, line_step,
number_format, printf, rand, string, time_format. Each is built by the
compiler its C used (old_agbcp for the six objects in `OLD_AGBCC_OBJS`:
display, fade, sprite_frame, input, irq, aabb; agbcp for the rest), and
every object is byte-identical to its C build, symbol table included.

As experiment 4 predicted, the conversion is mostly mechanical: the C
headers are included inside `extern "C" { }`, so every function keeps
its C name (crt0's `AgbMain`, the IRQ handlers, the ldscript and the
remaining C files) with no cxx_symbols.txt entry. What C++ rejected:

- **`NULL`** is `((void *)0)` (stddef.h), which C++ doesn't convert to
  another pointer type: `0`, as the other `.cpp` files write it
  (sprite_frame, irq, memory).
- **Local declarations of external functions** get C++ linkage:
  asset's one-argument `LZ77UnCompVram`/`RLUnCompVram` and irq's
  `_call_via_r0` are `extern "C"`; sprite_frame's own
  `void *_call_via_r1(...)` went for gobj_1a794.h's `s32` one.
- **`asm(... :: "r0")`**: `::` is one token in C++ (printf, `: :`).
- **Signed/unsigned comparisons** are a warning (an error with
  `-Werror`) in g++'s `-Wall`: mem_alloc's two compares of the block's
  `int` size against the `u32` request spell the C's implicit
  conversion, `(u32)`.

What reads as C++ now: FlushSpriteFrameOamQueue calls the OAM shadow
buffer's methods (`gOamBuffer->Append`, `HideUnused`, `SetAffineScales`;
gfx.h's three C prototypes went), DrawWrappedTextInBox takes a `Font *`
and uses its inline `SetMargin` and `HeightToLines` (text.h's C
prototypes of it and DrawWrappedText went: no C caller), and ClearKeys
is `KeyInput::KeyInput()` (spawners.hpp; cxx_symbols.txt already mapped
`__8KeyInput`; system.h's `void *ClearKeys(void *self)` went). The
other functions stay free functions: UpdateKeys and GetDpadDirection
take `gInput`, a `void *` to their many callers, and the rest have no
class.

Kept, each still needed under C++ (tools/match_prune.py, every site
tried): WaitForKeyPress's r4 pin (`.text` differs at +0x6 without),
FindSubstring's two pins (ip: 8 bytes longer; r3: differs at +0x242)
and its five instruction `asm`s (the four lower-casing folds also
tried as an inline function under agbcp: still a conditional move),
string's `strcpy`/`strlen` asm labels and memory's hand-written
`mem_walk_heaps`. `MATCH_HOLD_REG` 87 -> 87 and instruction-emitting
`asm` 17 -> 17 project-wide (tools/match_idioms.py).
### The link, save and IWRAM data

The link cable (src/link/), the save data and its link transfer
(save_data, save_transfer, save_transfer_poll) and the IWRAM image's
initialised globals (iwram_data) are C++ source now. Each file is the C
with its C headers wrapped in `extern "C" { }`, so the functions keep
their C names (no cxx_symbols.txt lines) and the globals theirs. Every
object is byte-identical to the C build's, under the compiler the C
object had: old_agbcp for the OLD_AGBCC_OBJS ones (link_handshake,
link_session, link_session_reset, save_data, save_transfer), agbcp for
the others. link_session_reset.o keeps `-fno-rerun-loop-opt`; the
per-object flags apply to the `.o`, whatever its source.

- **C-only constructs.** iwram_data.cpp initialises its pointers with 0
  (NULL is `(void *)0`, which C++ doesn't convert implicitly) and
  defines the globals that C++ declares as class pointers with those
  types (`SaveMenu *gSaveMenu`, `LevelSelect *gLevelSelect`,
  `LevelLayers *gLevelLayersSingleton`, `ActorSelf *gActorList`,
  `LanguageSelect *gLanguageSelect`); HeapSortActorsByKey has an
  `ActorSelf **` prototype for C++ in iwram.h, like actor.h's
  `gHeapSortActorsByKeyFunc`. Data and symbols are unchanged.
- **Two g++ code-generation differences**, both in UpdateLinkSession and
  HandleLinkSerial (link_session.cpp). `!(v & 1)` on an int is a bool
  negation in C++, which combine turns into `eor`/`and`; the ready test
  is written `(v & 1) == 0`, as the ROM tests it. And g++ keeps a 4-bit
  `u32` bitfield unsigned in a compare with an `s32` (C promotes it to
  int), which -Werror rejects as a signed/unsigned comparison; a `(s32)`
  cast gives the C's code.
- **The save data's audio calls.** LoadSaveData and StoreSaveData
  tested `audio->state == 1` with the flag shape AudioContext's inline
  IsPlaying returns ([the audio context](#the-audio-context)), so they
  were C++ that inlined it: `bool wasPlaying = audio->IsPlaying()`, and
  method calls for GetCurrentSong, StopSong and PlaySong. Same bytes;
  audio.h's C prototypes of those three go, and PlaySfx's, whose other
  C caller, yeti_states, is C++ too: no C file calls an AudioContext
  method by its C name any more.
- **PollSaveTransfer** was the last instruction-for-instruction asm
  transcription with a `.pool` (and a `MATCH_HOLD_REG` for its result).
  As C++ it matches with no workaround once `struct link_session`'s
  `playerId` is `volatile`: the ROM reads it twice (`== 0`, then `==
  1`), and the serial IRQ (HandleLinkSerial) writes it. The field's
  other users compile the same. `tools/match_idioms.py`: pins 87 -> 86,
  `.pool` 1 -> 0.
- **Not made methods.** The link session has C++ traits the survey
  missed, like the audio context: InitLinkSession returns `this` and its
  caller allocates first (`InitLinkSession(IwramAlloc(0x408))`, save_menu_input.cpp:
  `new LinkSession`, IWRAM's heap as AudioContext's), and
  DestroyLinkSession takes `__in_chrg`, frees on bit 0 and is called with
  3 (`delete gLinkSession`); its dead loop from `&players[4]` down to
  `players` is the empty destructor loop of the `players` array. There
  was no class for it yet (`struct link_session`, link_session.h), so the
  functions stayed free functions; [#751](#the-link-session-the-save-data-and-the-save-transfer)
  made the class. The save data (`struct save_data`) and the transfer
  state (`struct save_transfer`) have no trait, but every function of
  theirs takes one as `self`: #751 made them classes too.

### The link session, the save data and the save transfer

#751 turns the free functions of the previous section into three
classes (and two helpers), with every object byte-identical and every
function keeping its C name through cxx_symbols.txt:

- **LinkSession** (include/link_session.hpp) is the link session, with
  the layouts of `struct link_session`, `link_player` and `link_ring`
  moved into it, `LinkPlayer` and `LinkRing` (link_session.h keeps the
  packed bit views and a `struct link_session` tag). The constructor
  and destructor are g++'s shape of the C's. InitLinkSession's ring
  resets are LinkRing's inline constructor, run for the outgoing `ring`
  and then, in g++'s array loop (`i = 3; ... while (i != -1)`, the
  C's `zero`/`fill`/`sentinel` locals), for the four players' rings;
  the body is `Reset(); enabled = 0;`. DestroyLinkSession's dead loop
  from `&players[4]` down to `players`, with its null test, is the
  players array's destructor loop: LinkRing's empty `~LinkRing() {}`
  makes LinkPlayer's implicit destructor non-trivial, so g++ walks the
  array calling it, and the calls are empty. The class's inline
  operator new and delete call IwramAlloc and IwramFree (as
  AudioContext's), so the save menu's `InitLinkSession(IwramAlloc(0x408))`
  is `new LinkSession` and its `if (gLinkSession != NULL)
  DestroyLinkSession(gLinkSession, 3)` is `delete gLinkSession`.
  ResetLinkSessionState's ring resets call the same `LinkRing::Reset`
  the constructor does. LinkStart and LinkStop are methods that don't
  read `this` (their callers pass the session); LinkSetupSio (no
  argument) and MakeLinkHandshakeId (it takes the id bytes) stay free
  functions, as do the IRQ handlers the IRQ table points at.
- **SaveData** and **SaveTransfer** (include/save_data.hpp) have no
  constructor, destructor or vtable: the save menu's `new save_data` and
  `delete linkSave` are POD allocations already, and stay so. The
  checksum loop ValidateSaveData expanded inline (a `static inline` copy
  in the C) is the inline method `ChecksumOk`, and CheckSaveChecksum is
  `return ChecksumOk();` ([An inline body the ROM also has out of
  line](#dead-ends-and-gotchas)). TestSaveFlags' and ClearSaveFlags'
  parameter was called `flags` like the field; it is `mask` now.
  ReadSaveData and WriteSaveData take a `void *` buffer and a length and
  stay free functions.

The layouts are the classes' alone now: no C file used them (the two
data files that include link.h and save.h only see the tags). save.h's
and link.h's C prototypes of the 28 methods go.

Kept, every site tried with tools/match_prune.py and still needed as
C++: UpdateLinkSession's two `MATCH_KEEP`s, HandleLinkSerial's
`MATCH_KEEP`, `MATCH_KEEP_EXPR`, `MATCH_USE` and `MATCH_CONST`,
ResetLinkSessionState's 13 `MATCH_USE(id)` and `MATCH_BARRIER`,
ValidateSaveData's pin and `MATCH_USE`, TestSaveFlags' pin (without it
g++ drops the ROM's zero-extension of the `u8` parameter; written as a
`mask &= flags` test it is two instructions shorter), and
ReceiveSaveTransferChunk's two pins and its empty-template asm.
`tools/match_idioms.py` counts are unchanged (pins 86, uses 46).

### The IWRAM ARM code

The IWRAM image's ten ARM routines (src/iwram/string_arm.cpp,
sprite_arm.cpp) are C++ too, built with **agbcp_arm_patched**:
notyourav/agbcc's `cp` branch also has an ARM tree, `g++_arm`, which
is agbcc_arm's gcc 2.9-arm-000512 (same version string, same
`config/arm/arm.c` and `arm.h`) with the C++ front end; its `cc1plus` is
agbcp_arm. tools/build_agbccpp.sh builds it with
tools/agbcc_patches/agbcc_arm_prologue_return.patch (the two opt-in
options itoa_arm and LookupSpriteFrameCache need,
[iwram-image.md](matching/iwram-image.md), "Seventh pass"), its paths
rewritten from `gcc_arm/` to `g++_arm/` (it applies cleanly), and
installs it as `tools/agbcc/bin/agbcp_arm_patched`. CI's "Build agbcp,
old_agbcp and agbcp_arm_patched" step runs it; the step that built
agbcc_arm_patched went, since nothing uses that compiler any more
(tools/build_patched_agbcc_arm.sh stays, for comparisons).

Evidence, on both files (assembled code compared, the objects' flags):

- agbcc_arm (C) against stock agbcp_arm and agbcp_arm_patched (C++)
  without the options, with and without `-fno-schedule-insns
  -fno-schedule-insns2`: the same code.
- agbcc_arm_patched (C) against agbcp_arm_patched (C++) with
  `-mleaf-no-lr-save`, `-minterwork-return-lr` and both: the same code.
- The Makefile build (`ARM_OBJS`: agbcp_arm_patched, `-mthumb-interwork
  -O2 -fomit-frame-pointer -fno-rtti -fno-exceptions` plus each object's
  options): string_arm.o and sprite_arm.o byte-identical to the
  agbcc_arm_patched objects.

The only source change besides the `extern "C"` includes is
HeapSortActorsByKey's list, `ActorSelf **` (sprite_arm.cpp includes
actor_self.hpp; sortKey is AnimPart's): with the C's `struct actor_self
**` the definition no longer matched iwram.h's C++ prototype and got a
mangled name. The Makefile's ARM objects use the C++ rule like every
other `.cpp`, with `CXX1` set to agbcp_arm_patched.

### C++ everywhere: actor, vehicle, level

The owner chose one language for the game: every game file under src/
(not the data tables, not lib/) becomes C++. This batch is the actor
zone's C helpers (actor_bg, actor_category_init, actor_category_stats,
actor_vram_pool, bg_picture, cell_anim), the yeti (yeti, yeti_graphics,
yeti_states, yeti_update) and the level's C files (bonus_round, camera,
collision_map, room, terrain, terrain_probe, terrain_probe_axes): 17
objects, all byte-identical to origin/main's (code, data, relocations and
symbol table), each with the compiler its C had.

Their functions keep C linkage: the headers they define are included in
`extern "C" { }`, so the definitions get the C names with no
cxx_symbols.txt entry. Five of the files compiled unchanged; the others
needed the C++ types of the globals (globals.h and vehicle.h declare
gHud, gPaletteCache, gOamBuffer, gObjVramCursor, gLevelLayers, gPlayer,
gAudioContext and gActorList as classes to C++), which turned their C
calls into method calls:

- **The yeti** is a 0x1C-byte AnimPart, like the airship: gYeti is an
  `AnimPart *` to C++ (it was an `ActorSelf *`; vehicle.h has no C
  declaration left), CreateYeti is `new AnimPart(gYetiKeyframes,
  gYetiFrames, 0xF)` (the hand-written IWRAM allocation, field stores and
  SetActorAnim call went) and DestroyYeti `delete gYeti`. The animation
  restarts are `RestartAnim`, UpdateYeti calls
  `static_cast<PolarPlayer *>(gActorList)->Catch()` and
  `GetAnimFrameBaseOffset()`, and IsTouchingYeti takes an `ActorSelf *`.
  vehicle.h's CatchPolarPlayer prototype and actor.h's
  GetAnimFrameBaseOffset/SetActorAnim prototypes (the yeti's C names) went.
- **YetiStateChase's PlayAmbientSfx** is the method with a `bool`
  argument: the asm-label alias that passed four arguments and stored the
  stack byte by hand (`PlayAmbientSfx_4`, so that `mov r4, sp` came
  before `mov r1, #1`) isn't needed; g++ stores the argument in the ROM's
  order.
- **The global operators** are `operator delete[]`, `operator new[]`,
  `operator delete` and `operator new` in camera.cpp (`__builtin_vec_delete`
  & co., which cxx_symbols.txt already renamed to the C names); memory.h
  keeps the C names for the code that calls them by name.
- **The rest** are the class methods the C called by their C names:
  `PaletteCache::FreeUnlockedSlots`/`BindSlot`/`Upload`,
  `Hud::ConfigureParts`/`Update`/`UpdateSlides`/`ShowCounters`/`SetCrateTotal`,
  `OamBuffer::Rewind`/`Reset`/`Commit`, `ObjVramCursor::Reset`,
  `LevelLayers::SetScroll`/`Reset`, `AudioContext::PlaySfx`, and
  `SetEntityPos(gPlayer, ...)` without its cast. hud.h's five Hud
  prototypes and level.h's SetLevelScroll/ResetLevelLayers had no C
  caller left and went. struct player (player.h) has no C reader any more
  (bonus_round.c was the last) but stays for now.

None of these functions became methods: they act on globals (the yeti,
the camera, the category state) or take C views no class replaces
(struct tile_cache's lookups are free functions in tile_cache.cpp too),
and no class was invented for them.

**InitActorCategory** was the one that didn't compile to the same code
unchanged: the stack frame grew by 4 bytes, gActorCategories was spilled
and the two `ret = 1` exits were cross-jumped. Its RTL as the front ends
emit it is the same; what differs is loop.c's invariant motion. The C++
front end opens a binding level around every expression statement and,
with no temporaries to clean up, leaves its begin note as a
NOTE_INSN_DELETED: one note per statement. loop.c numbers every insn,
notes included, so a register's lifetime there grows with the statements
it spans, and `move_movables` hoists an invariant when `threshold *
savings * lifetime >= insn_count`. The pause menu's two palette DMAs share
the OBJ_PLTT and 0x80000100 constants across ten statements: lifetime 17
and 15 in the C, 28 and 25 as C++, so the inner loop hoisted them into r8
and r9 and the outer loop's gActorCategories had no register left. The
pause round trip is an inline function now (`RunCategoryPauseMenu`):
expand_inline_function drops NOTE_INSN_DELETED notes, so the inlined
body's lifetimes are the C's again and the whole object matches. See
the gotcha below.

Kept, with their comments: bonus_round.cpp's r2 hold (SetCheckpointAtPlayer's
CpuSet control word), bg_picture.cpp's two `MATCH_USE`s, yeti_states.cpp's
`MATCH_BARRIER` and the `SetCheckpointAtPlayer_1` asm-label aliases of
cell_anim.cpp and actor_category_init.cpp (the call leaves r1 as it is);
tools/match_prune.py removes none of them. asm-label aliases 12 -> 11
project-wide (tools/match_idioms.py); 17 more `.cpp` files under src/.

### The tile-slot pool, the tile cache and the BG setup (#752, #753)

Three plain structs whose functions all took them as `self` are classes
now, with no vtable; every object is byte-identical, and the C names
stay (cxx_symbols.txt's `#752` and `#753` blocks):

- **TileSlotPool** (include/bg_layer.hpp, PooledBgLayer's `pool`): the
  pool's five functions are methods, and its `static inline` free-stack
  and slot-table helpers private inline methods (they still make g++
  recompute `this + 0x4808`/`this + 0x408`). PooledBgLayer's
  constructor allocates it with `new TileSlotPool` (no constructor: the
  ROM's plain OperatorNew) and its Reset empties it.
- **TileCache** (include/bg_layer.hpp, LevelLayers' `tiles`) had only its
  constructor and destructor; its eight lookups (bg_layer_base.cpp,
  collision_map.cpp, tile_cache.cpp) are methods now, and level.h's
  struct tile_cache, its base, is gone: the fields are the class's, and
  struct level_layers (the probes' C view) holds a `class TileCache *`
  for C++ (C sees an opaque `struct tile_cache *`). The `GetCell` inline
  both files had is one private `CellAt`.
- **BgSetup** (new include/graphics_package.hpp; graphics_package.h's
  struct bg_setup goes, no C file read it): `InitBgSetup` is its
  constructor. A member (the pause menu's, the power dialog's, the level
  select page strip's `bg`, all at offset 0 and set first) is built in the
  mem-initializer list; a stack one (the level select's sky, the save
  menu's and the language select's) is declared where InitBgSetup was
  called; the continue prompt's three are `new BgSetup(...)`, which g++
  compiles to the same OperatorNew and constructor call with no null test.
  `LoadGraphicsPackage` and `GetBgSetupControl` are `Load` and
  `GetControl`.
- **ScaledSprite** (graphics_package.hpp; was graphics_package.cpp's
  struct gfx_box_obj): the six sprite-box functions, all UNUSED, are its
  methods; gfx.h's nine prototypes went.

### The level state as a class (#750)

`LevelState` (include/level_state.hpp) was a class only for its
constructor and destructor (`class LevelState : public level_state`, the
#664 cleanup); its functions were C functions taking `struct level_state
*self`. It is now the whole class: the field list moved from level_state.h
into the class, with the room block as a class of its own, `LevelProgress`
(the `room` member, +0xC4; level_state.h's struct level_progress), and
every function that took either as `self` is a method under its C name
(cxx_symbols.txt): LevelState's 85 accessors in level_state.cpp,
`UpdateGameFrame`, `EndBonusRound`, `SetCheckpointAtPlayer`,
`StartTimeTrial`, `PlayCutscene` and `GetLives` in their files, and
LevelProgress's 12 room functions (level_query.cpp, play_room.cpp,
run_room.cpp, room.cpp, room_frame.cpp). C sees `struct level_state` only
as an incomplete type (the data tables include level.h); globals.h and
level.h declare gLevelState, gLevelStateSingleton and gGameFrameLevelState
as `LevelState *`s to C++, so the 61 calling files' `Foo(gLevelState, ...)`
are `gLevelState->Foo(...)` and UpdateGameFrame's `Foo(&self->room)` are
`room.Foo()`. The methods' bodies are the C's with `self->` dropped, and
every object is byte-identical to origin/main's.

- **Stayed free functions:** `nullsub_24` and `GetLevelState` (no `self`).
  The functions that took `void *self` (SetMaskLevel, SetCheckpoint,
  PackSaveData, the five `LevelHas*Gem` wrappers, which ignore it, and the
  three cutscene starters and ShowCompanyLogos, which only pass it on or
  ignore it) are methods like the others: `this` is r0 either way.
- **`struct game_progress` stays a plain struct** in level_state.h, as
  LevelState's `progress`, `checkpointData` and `saveData`: the save slots
  (save_data.h), the menus' counts (CountGems & co., power_dialog_draw.cpp)
  and GetCompletionPercent (graphics.cpp) share it, and none of them is
  the level state's. A GameProgress class would retype the save slot's
  member and the menu prototypes in the headers the data tables parse as
  C, for no change in the code.
- **Pins gone:** SetCheckpoint's and SetCheckpointAtPlayer's r2 hold
  (`MATCH_HOLD_REG(u32, ctrl, r2)`, which kept gcc from holding the CpuSet
  control word in a callee-saved register across the first of the two
  bitmap copies). Through an inline `CopyBitmapSpan(dst, src)` the
  constant is loaded at each call, as the ROM has it ("A constant the ROM
  loads early" is the opposite case of the same inliner behaviour).
  `tools/match_idioms.py`: `MATCH_HOLD_REG` 86 -> 84.
- **Kept:** the `SetCheckpointAtPlayer_1` asm-label aliases of
  actor_category_init.cpp and cell_anim.cpp (they call the method with
  r1 left as it is, and a method call can't leave out an argument), and
  RunRoom's `x` reuse: `x = (s32)gLevelState; LevelState *state =
  (LevelState *)x;` matches, a `LevelState *state = gLevelState;` doesn't
  (the global goes through another register).

### The C views go (#754)

With every game file C++ (#746-#748), the C structs kept as views of the
classes for the C files had no C reader left: src/data/ and lib/ never
read an object's fields. They go, with their ASSERT_VIEW_FIELD checks,
the C (`#else`) arms of the globals' declarations (gPlayer, gActorList,
gHud, gLevelLayers, gSpriteBankSet, gPaletteCache, gOamBuffer,
gObjVramCursor, gAudioContext, gSpriteRenderer, gEntitySpawner,
gLevelSelect, gLanguageSelect, gSaveMenu, gSmallFont, gLargeFont, gAirship,
gHovercraft, gPolarAkuAku, gRiderlessPolar, gActorDrawList and the actor
sort hook) and the C prototypes that only took a view (gfx.h's
OamBuffer, PaletteCache and ObjVramCursor methods, SetEntityPos and
SetEntityPixelPos; cutscene.h's CutscenePlayer methods). Every object is
byte-identical to origin/main's.

- **The views**: player.h's `struct player` (and objects.h's `struct
  collision_queue`, which only it embedded), actor_self.h's `struct
  actor_self`, bitmap_font.h's `struct bitmap_font`, bg_scroll_layer.h's
  `struct bg_scroll_layer`, level.h's `struct level_layers` and `struct
  camera_target`, globals.h's `struct sprite_bank_set` and the opaque tags
  (`hud_counter`, `actor`, `oam_shadow_buffer`, `palette_cache`,
  `vram_upload_cursor`, `entity_spawner`, `level_menu`, `level_item`,
  `language_select`, `credits_screen`, `save_menu`, `follow_child`,
  `actor_283c`, `collect_part`). The field notes the views had and the
  classes didn't moved to the classes (ActorSelf's, Font's, LevelLayers').
  The classes check their ROM sizes instead of the views'.
- **What the headers keep**: the plain records (actor_self.h's
  `anim_frame_record` and `anim_box`, bitmap_font.h's
  `icon_glyph_metrics`, bg_scroll_layer.h's `union bg_cnt`, player.h's
  PLAYER_DIR_* and PLAYER_HIT_* bits). Where a C header's struct or the
  category vtable data (src/data/actor_category_175558.c, slot 1) needs a
  pointer to a class, it names the class's own tag: `struct Crate *` in
  `struct crate_group` and `struct collision_candidate`, `struct ActorSelf
  *` in SpawnActor's prototype. To C that is an incomplete struct; to C++
  it is the class, so one declaration serves both, with no `#ifdef
  __cplusplus`.
- **The camera** (`struct camera`, still a C struct) follows a `Sprite *`:
  StepCameraDirectional and StepCameraFacing copy `target->Pos()` (the
  8-byte block copy the view's `struct vec2 pos` gave; `Pos()` moved from
  MovingSprite up to Sprite), and SnapCamera reads `x`/`y`. PlayRoom and
  the level select store `gPlayer` and `this` with no cast.
- **The terrain probes** (terrain.cpp, terrain_probe.cpp,
  terrain_probe_axes.cpp) take and cast to `LevelLayers *` and call its
  `tiles`' TileCache methods (#755). They stay free functions with C
  linkage.
- **PlayCutscene** kept a `struct cutscene_player` in its stack aggregate,
  a codegen view: a CutscenePlayer member would be constructed where the
  aggregate is declared, before the display setup the ROM does first. The
  aggregate (`CutsceneLocals`) now holds raw storage for the player, built
  with placement new (`new (&f.pager) CutscenePlayer`, an inline `operator
  new(size_t, void *)`) and destroyed with an explicit destructor call;
  `Run` is the method. Each use casts the storage to `CutscenePlayer *`:
  through a pointer variable, or an inline accessor, gcc keeps the
  player's address in a register and stores through it (`str r0, [r4,
  #20]`) where the ROM stores sp-relative.
- **The crate list's codegen view** stays, as C++: crates.h's `struct
  pool_init_node` (with `struct pool_link`) is crate_list.hpp's
  `CrateGridNodeInit`, next to ResetGrid, its one user, whose `wrap` is a
  `CrateGridLink *` (`struct pool_link` was its only other use). The
  untyped fields are still what keeps gcc from reloading `nodes`.
- **Not views**: audio.h's `struct audio_context` is AudioContext's base
  (its field list); vtable.h's `struct vtable_slot` has no user left.
  level.h's opaque `struct tile_cache` tag (#755), which only struct
  level_layers used, goes with it: LevelLayers holds the `TileCache *`.

CONTRIBUTING.md's "One layout, one type" now says there are no C views of
the classes.

### Next batches

Bigger controllers, roughly in order (function counts from
`tools/cpp_survey.py --objects`):

1. ~~**The enemy controllers**~~: done in part 2 (all of src/enemies/). Their
   C callers, the level spawners (`spawn_enemies.c`, `spawn_objects.c`'s
   SpawnSealSpawner), are C++ since parts 9 and 9b; Dingodile's shark
   (`DingodileSharkCtrl`, part 6b) derives from it.
2. ~~**The rest of `Ctrl` and `InputCtrl`**~~: done in part 3, and
   `Ctrl::Update` (`UpdateCtrl`, an empty function in system/boot.c) in
   step 10b.
3. ~~**The swim controller**~~ (`PlayerCtrl`): done in part 4; its
   `Reset`/`Restart` (in `action_ctrl.cpp`) in part 5a. **The action
   controller** (`ActionCtrl`): part 5a converted `action_ctrl.c`,
   `_moves.c`, `_update.c`, `_idle.c`, `_land.c`, `_left_ground.c` and
   `kill_player.c`; part 5b the other four (`action_ctrl_event.c`,
   `_hang.c`, `_run_jump.c`, `_states.c`), and action_obj.h's vcall
   macros went with them. Its last method, `Reset` (`ResetActionCtrl`,
   wumpa.cpp), was converted with the pickups in part 7h.
4. ~~**The boss controllers**~~: `tiny_update.c` and `mega_mix_update.c`
   in part 6, `dingodile*.c` in part 6b, and `cortex.c` (the rest of the
   Neo Cortex fight, Tiny's constructor, destructor and `StartHop`, the
   one-shot animation controllers' constructors and destructors) in part
   7i.
5. ~~**The entity family**~~ (part 7, below), with the platform mover
   (`PlatformMover`, gPlatformMoverVtable: a 0x38-byte `Ctrl`; include/platform.hpp)
   done in 7d, and the controllers' `MovingSprite *` in 7b'.

Next, the classes built on the family, then the 3D actors (one family per
PR, as before):

6. ~~**The player**~~: done in part 8 (`Player : GroundSprite`,
   include/player.hpp; `PlayerSprite()` and the `(GroundSprite *)` casts
   gone). `struct player` stays the C view for the C files left (the
   rooms, the HUD, the vehicles).
7. ~~**The spawners**~~: entity_spawner.c, drop_extra_life.c, time_trial.c
   and the level's spawn_*.c but spawn_enemies.c in part 9
   (`EntitySpawner`, include/spawners.hpp), spawn_enemies.c (the 26
   enemy spawners) in part 9b, and text_popup.h went with it.
8. ~~**The level select's sprites**~~: done in part 10 (the camera lead and
   launch pad as `MovingSprite` subclasses, and the whole level select,
   include/level_select.hpp; `InputCtrl::StateStart`'s `AddToPartList` is
   `CollidableList()->Add`). The other C++-trait objects of the frontend and
   the menus are next, a few files per part: ~~frontend/company_logos.c,
   language_select.c and language_select_setup.c~~ (part 10b: the logo
   actor, the first `ActorSelf`, and the language select),
   ~~starfield.c and credits.c~~ (part 10c: `Starfield`, the credits and
   the continue prompt's last methods), ~~the pause menu and the power
   dialog~~ (part 10d: pause_menu.c, pause_menu_pages_init.c,
   power_dialog.c, power_dialog_draw.c and continue_prompt*.c, the objects
   with C++ traits; the classes' plain-C files stay C) and
   ~~title_screen_init.c and title_screen.c~~ (part 10c-2: `CompanyLogos`'s
   other methods and the title screen).
9. **The 3D actors** (part 11 onwards, [below](#the-3d-actors-part-11)):
   the base classes (`AnimPart`, `ActorSelf`, `HpActor`) in part 11a,
   the actor factory and the category frame in part 11b, then the rest of
   src/actor/, the vehicles and the 3D bosses; the PMF
   tables become `const StateFunc t[] = { &X::f, ... }` (experiment 3,
   part 11a's actor_pmf_17a6b8.cpp) and `ACTOR_PMF_CALL` goes.
10. ~~**Then the vtables**~~ (plan item 5 below): done in step 10, for
   every class whose key method was C++: 85 of the 93 tables
   ([Emitting the vtables](#emitting-the-vtables-step-10)). The other 8
   (`Ctrl`, the BG layers, the fonts) in step 10b, with their classes
   ([The last C vtables](#the-last-c-vtables-step-10b)).

#### The 3D actors (part 11)

Whole files only, base classes first, as in part 7. Counts are the C's:
the functions in the objects, and `tools/match_idioms.py`'s
`MATCH_HOLD_REG` pins and instruction-emitting `asm`; "old" is an
object already in `OLD_AGBCC_OBJS`. The class hierarchy: `AnimPart` (the
animation, 0x1C; the airship is one) -> `ActorSelf` (gActorVtable, 4
slots: the polar actors, the logo actor) -> `HpActor` (hit points, slots
4-6: the jetpack actors and the 3D bosses' weapons) -> the balloon crate
(`JetpackBalloonCrate`, an 8th slot) -> its three kinds. The yeti, the
airship and the hovercraft themselves are singletons driven by plain
function tables (gYetiStateFuncs, gAirshipStateFuncs, ...), not classes
with vtables. A slice gives its classes their fields (from the C structs:
`struct actor_hp`, `struct jetpack_plane`, `struct orbit_actor`, ...) and
their real constructors, converts its PMF dispatches (`ACTOR_PMF_CALL`)
and, once no C file reads one, its PMF table to C++.

| Part | Files | Classes | Functions, pins, `asm` | PMF tables | Depends on |
|---|---|---|---|---|---|
| ~~11a~~ | ~~actor/actor.c, actor_anim.c; vehicle/polar_player_dispatch.c; data/actor_pmf_17a6b8.c~~ | `AnimPart`, `ActorSelf`, `HpActor`; the subclasses' destructors; `PolarPlayer::RunState` | done | gPolarPlayerStateFuncs | |
| ~~11b~~ | ~~actor/actor_factory.c, actor_spawn.c, actor_category_frame.c (old), actor_category_select.c~~ | the polar actors' constructors (CreateActor's inlined `new`s, `ConstructActorPart` = `PolarPlayer`'s), `FindShotTarget`, the category frame's virtual calls | done | | 11a |
| ~~11c~~ | ~~vehicle/polar_player.c (old), polar_player_actions.c, polar_player_states.c~~ | `PolarPlayer` (`Update`, `Draw`, the 14 states, the destructor, the methods the polar actors call) | done | (gPolarPlayerStateFuncs' last C user) | 11a |
| ~~11d~~ | ~~vehicle/polar_crates.c, polar_pickups.c, polar_objects.c, polar_aku_aku.c, polar_nitro.c (old)~~ | the polar crates (`PolarCrate` and its kinds), wumpas, hazards, Aku Aku, goal, boost pad | done | | 11b |
| ~~11e~~ | ~~vehicle/jetpack_spawn.c (old), jetpack_player.c, jetpack_run.c, jetpack_shot.c~~ | `HpActor`'s constructor, `JetpackPlayer`, `JetpackShot`, the jetpack spawners | done | gJetpackPlayerStateFuncs | 11a |
| ~~11f~~ | ~~vehicle/jetpack_plane.c, jetpack_balloon.c~~ | `JetpackPlane`, `JetpackBomber`, `JetpackCannonball`, `JetpackBalloon`; two of `AirshipFireball`'s states | done | gJetpackPlaneStateFuncs, gJetpackBomberStateFuncs, gJetpackBalloonStateFuncs (split into actor_pmf_17c414.cpp) | 11e |
| ~~11g~~ | ~~vehicle/jetpack_crates.c~~ | `JetpackBalloonCrate` and its kinds, `JetpackParachuteNitro`, `JetpackRocket`, `JetpackRing::Update` | done | gJetpackBalloonCrateStateFuncs | 11e |
| ~~11h~~ | ~~bosses/hovercraft.c (old), hovercraft_cannon.c, hovercraft_cannon_flash.c, hovercraft_launcher.c, hovercraft_side_gun.c, hovercraft_parts.c~~ | the hovercraft's weapons (`HovercraftFireball`, `HovercraftCannon`, ...), `JetpackRing`'s and `JetpackCollectedWumpa`'s constructors and methods (in hovercraft.c), the hovercraft (an `AnimPart` singleton) | done | gHovercraftFireballStateFuncs, gHovercraftCannonStateFuncs, gHovercraftLauncherStateFuncs | 11e |
| ~~11i~~ | ~~bosses/airship*.c (10 files; airship_map.c, airship_touch.c old)~~ | `AirshipFireball` (but its two flight states, 11f's); the airship (an `AnimPart` singleton) | done | gAirshipFireballStateFuncs (split into actor_pmf_17c2b8.cpp) | |
| 11j | actor/actor_bg.c, actor_category_init.c (old), actor_category_stats.c, actor_vram_pool.c, bg_picture.c (old), cell_anim.c; vehicle/yeti*.c (yeti_graphics.c, yeti_update.c old) | none: C-like (no C++ trait), only if the family's files should all be C++; left C ([the final cleanup](#the-final-cleanup)) | 43, 33, 3 | | |

Since 11g every 3D actor class is C++: actor_self.h's `ACTOR_PMF_CALL`,
`ACTOR_VCALL`, `VTABLE_CALL2`/`3`, `VCALL1`/`2` and the `ACTOR_RECORD`/
`ACTOR_LINK_*` casts went with their last C users (11d's polar_objects.c,
11g's jetpack_crates.c), and the family's 47 vtables (with the logo
actor's) can be emitted by g++ (item 10).

#### The entity family (part 7)

Whole files only, base classes first. Function and workaround counts are
the C's (`tools/cpp_survey.py --objects`, `tools/match_idioms.py`); "old"
is an object already in `OLD_AGBCC_OBJS`. Expect most objects to match
only under old_agbcp, as all four of 7a's do, and most of their pins to go.

| Part | Files | Classes | Size | Depends on |
|---|---|---|---|---|
| ~~7a~~ | ~~graphics.c, sprite.c, sprite_obj.c, sprite_anim.c~~ | `Entity`, `Sprite`, `UiSprite`, `PartList` (update, collide), the sprite graphics managers | done | |
| ~~7b~~ | ~~objects/moving_sprite.c, moving_sprite_collide.c, player_contact.c, step_probe.c, ground_sprite.c, ground_sprite_collide.c (old), ground_sprite_update.c~~ | `MovingSprite` (its 15 slots, the speeds, the controller), `GroundSprite` (the terrain probe) | done | 7a |
| ~~7b'~~ | ~~the controller headers and cxx_symbols.txt~~ | the controllers' `SpriteObj *` parameters become `MovingSprite *` (`P9SpriteObj` -> `P12MovingSprite`), and `SpriteObj` goes; `PartList`'s and `CrateList`'s items retyped | done | 7b |
| ~~7c~~ | ~~objects/part_list.c, part_list_cull.c, part_collide.c (old), collision_queue.c, gfx/palette_cycle.c~~ | the rest of `PartList`, the player's `CollisionQueue`, `HudPart` (a `UiSprite`) and the palette cycles | done | |
| ~~7d~~ | ~~objects/platform.c, platform_collide.c (old), platform_contact.c, platform_create.c (old)~~ | `Platform` (a `MovingSprite`, 0x80 bytes), `PlatformMover` (a `Ctrl`) (include/platform.hpp) | done | 7b |
| ~~7e~~ | ~~crates/crate.c, crate_create.c, crate_draw.c, crate_update.c, crate_hit.c, crate_touch.c, crate_player_collide.c, crate_reset.c, crate_stack.c, crate_time_trial.c, slot_crate.c~~ | `Crate` (include/crate.hpp) and its accessors | done | |
| ~~7f~~ | ~~crates/crate_list.c (old), crate_list_draw.c, crate_list_reset.c, crate_list_update.c (old), crate_grid_collide.c (old), crate_grid_link.c, crate_grid_unlink.c (old)~~ | `CrateList` (include/crate_list.hpp), the crate grid | done | |
| ~~7g~~ | ~~crates/crate_break.c (old)~~ | the crates' break and bounce paths | done | |
| ~~7h~~ | ~~pickups/extra_life.c, wumpa.c, wumpa_update.c~~ | `ExtraLife`, `Wumpa`, `Stopwatch`, `ActionCtrl::Reset`; `struct act` went | done | |
| ~~7i~~ | ~~bosses/cortex.c (old)~~ | the Neo Cortex fight's gem, platform mover (a `PlatformMover`) and shot controllers, the rest of the target's, cannon's and boss's methods, Tiny's constructor, destructor and `StartHop`, `OneShotAnimCtrl`'s and `UnusedOneShotAnimCtrl`'s constructors and destructors | done | 7d |

Outside this family, the classes built on these were still C: the
player (part 8 since), the level select's sprites and the effect parts'
spawner (see [The entity family is done](#the-entity-family-is-done) and
items 6-8 above).

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
5. **Vtables** are emitted by g++ once a class's key method is C++
   (drop the header's `#pragma interface`), which also checks the
   hierarchy: the slot order and overrides come out as the ROM has them
   or `make compare` fails. ldscript.txt places each
   `.gnu.linkonce.d._vt.<class>` section at its ROM address, between the
   C tables left. Done in step 10 for 85 of the 93
   ([Emitting the vtables](#emitting-the-vtables-step-10)) and in step
   10b for the last 8 ([The last C vtables](#the-last-c-vtables-step-10b)).
6. **Leave C as C:** `lib/` (GAX2, AgbEeprom, libgcc, BIOS wrappers)
   and the data tables. (The IWRAM ARM code was on this list; the `cp`
   branch's ARM `agbcp_arm` turned out to match it,
   [the IWRAM ARM code](#the-iwram-arm-code).)

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
  `.gnu.linkonce.d` section (ldscript.txt places it, step 10). That file
  also gets an out-of-line copy of **every** inline method of the class,
  used or not, at its end, in the reverse of their declaration order:
  that is how graphics.cpp ends with `InitEntity`, the accessors and
  `DestroyEntity` (include/entity.hpp). An inline method meant only for
  other files (a second constructor, say) would be emitted there too;
  `-fno-implement-inlines` (`NO_IMPLEMENT_INLINES_OBJS`) drops them.
  Defining the inline method `inline` in the `.cpp` instead of in the
  class doesn't help: it is still emitted.
- **Uncalled functions at the end of a file are inline methods' copies.**
  A run of small functions nothing calls, ending with a destructor, at
  the end of a C file (often UNUSED-tagged accessors) is the out-of-line
  copies g++ emitted for a class whose vtable that object had: the
  methods were inline, and their callers have them written out in place.
  Make them inline methods in reverse emission order (the destructor
  declared first) and use them; the file may have to end there
  (bg_layer.cpp, step 10b) or take in the object before it that has the
  key method (font.cpp, step 10b).
- **`-fno-implement-inlines` drops inline virtual methods too** in this
  g++ snapshot (later gccs keep them), so a vtable that points at an
  inline or implicit destructor gets an undefined `_._<len><Class>`.
  Map it in cxx_symbols.txt to the destructor's C-linkage ROM copy
  (`_._10PolarCrate DestroyPolarCrate`, step 10).
- **A g++-emitted vtable's symbol is weak** (`objdump -t` flags ` w O`),
  not global: tools that look for a table's `g` symbol need to accept
  both (tools/report_units.py).
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
  about the C emulation: `StompedHopPadCtrl::Update` keeps one of the
  C's two `MATCH_HOLD_REG`s, `part`'s (a `register T x asm("rN") = this;`
  pin also works in C++).
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
- **A byte the ROM passes in a register and reloads with `ldrb`** (a
  QImode argument) is a `bool` parameter: g++ 2.9 doesn't promote a
  `bool`, where a `u8` is promoted to a word. The C used a one-byte
  struct, which needed an asm-label alias of the callee
  (`Crate::BreakInStack`'s `once`; part 7g).
- **A reload left by a deleted compare** (the ROM reloads a variable
  after a call for nothing) is a dead test of it whose result is a
  function-scope variable (used elsewhere): flow keeps the compare until
  after reload. A block-local one is deleted before reload
  (`QueuePlayerCollision`'s `side`; part 7g).
- **A byte flag flipped and tested** (`ldrb; eors` with the 1 loaded
  first, the result stored and tested) is `dirX ^= 1; if (dirX)`; a local
  `u8 d = 1 ^ dirX` gives other registers (`PlatformMover::Update`, part
  7d).
- **A store addressed off another field's address** (`adds r0, #0xac;
  str; subs r0, #0x44; strb`) comes out when the stored constant is a
  variable set between the two stores (`p->carried = part; { u8 m = 8;
  p->hitAxes = m; }`); with a literal, the second address is formed from
  the base again (`MovePlayer`, part 7d).
- **Two long-lived pseudos swapping r4 and r5** (or r8 and r9) is global
  allocation's order: by `floor_log2(refs) * refs / live_length`, so a
  short-lived value that crosses a call can outrank a pointer used all
  through the function. The `.greg` dump (`-dg`) lists both numbers. When
  no spelling changes them, pin the short-lived one (`now` in
  `PlatformMover::Update`, part 7d).
- **A `goto` into another branch's tail** can cost registers the plain
  duplicate doesn't: `ResolveCollision`'s two `goto set_hdir`s needed two
  holds and a pin, `result = hdir;` in place needs nothing (part 7d).
- **When only the registers differ, try the same code through an inline.**
  An inline call's arguments and an inline's body are different RTL from
  the same statement written in place, even where the final code is the
  same, and that can move global allocation's ranking or reload's
  round-robin of reload registers. In part 7i, `c->SetPos(part->x,
  part->y)` for a block copy of the position, `e->SetKind(1)` for
  `e->kind = 1`, and `Entity::MarkGone` with the bitmap indexed instead of
  addressed by byte offset each fixed a function whose only difference was
  a permutation of registers. A script that builds every combination of a
  few such spellings (variants of one function, compared with the ROM's)
  finds these faster than reasoning from the `.greg` dump.
- **A switch on a copy:** where the ROM runs the last compare of a
  switch's tree on a copy of the index in another register, switch on a
  local copy of an inline function's parameter (`StepHeight`, part 7i).
- **Reload avoids a register global allocation gave a pseudo.** A
  register difference in a reload's scratch (an `ldrsh` index, a constant's
  register) can come from a pseudo elsewhere in the function: reload takes
  a hard register that holds a pseudo only when no free one is left. In
  `Player::CheckPlayerContact` the frame anchor's spelling decided whether
  r3 held it, and with it every virtual call's `ldrsh` register (part 8).
- **Jump threading over a repeated test.** Two identical tests of a global
  with no store between are threaded: the false path of the first jumps
  past the second, where the ROM tests again. Testing through a local copy
  of the global's pointer keeps the second load (`Player::Draw`, part 8).
- **A member object's constructor and destructor** are called by g++: the
  constructor after the base's constructor and the vtable pointer store,
  the destructor after the body with flags 2 (no delete), before the
  base's destructor (`Player`'s `collisionQueue`, part 8).
- **A `#ifdef __cplusplus` declaration** gives a C global its class type in
  C++ with the same symbol (`extern class Player *gPlayer;` inside `extern
  "C"`), so the C++ files need no accessor or cast for it (part 8).
- **A store to a union member clobbers every other memory value** for CSE:
  gcc gives union member accesses alias set 0. A store to `Sprite::bank`
  (in an anonymous union with `anim`) makes g++ reload the pointer it
  stored through; store through a pointer to the member instead
  (`SetBankNow`, part 10).
- **Declaration order can put a union local in memory:** `union key_state
  keys = gKeys;` declared before another union local went to the stack;
  declared after it, it stays in a register (`LevelSelectLoop`, part 10).
- **g++ 2.9 has no anonymous structs:** `union { struct { u16 a, b; }; u32
  ab; }` is "anonymous class type not used to declare any objects".
- **A bitfield stored from an inline's parameter** is a general insert (the
  field cleared, the value ORed in) even with a constant argument, where a
  literal store is a plain OR: g++ 2.9 inlines at the RTL level, after the
  store has been expanded (`SetFlipX(obj, 1)`, `SpawnBasicCrate`, part 9).
- **`gX = new X` loads `&gX` first,** before the allocation, as the ROM's
  constructors-into-globals do; a C function's result is stored after the
  call unless written through a pointer to the global taken first
  (`InitLevelState`, part 9).
- **`delete p` of a class with no vtable** is the null test and a direct
  call of its destructor with 3 (`delete gEntitySpawner`, part 9).
- **`delete p` of a struct with no destructor** (a POD) is a plain
  `OperatorDelete(p)` call, with no null test: the credits' text lines
  and the continue prompt's BG buffers (part 10c). `new T` of one is
  `OperatorNew(sizeof(T))`, and `new T[n]`/`delete[]` are
  `OperatorNewArray` and the null-tested `OperatorDeleteArray`.
- **`operator new(n)` in C++, not OperatorNew(n):** the C name next to a
  `new` gives the object two undefined `OperatorNew` symbols after the
  rename, one from `__builtin_new` (part 9).
- **A constant held in a callee-saved register across calls** (stored
  before and after them) is a variable set before the calls; a literal 0
  is loaded again at each store (the powers' tags, part 9).
- **A destructor that frees with something other than `OperatorDelete`**
  (the 3D actors' direct `mem_free`) is a class with its own inline
  `operator delete`: the deleting destructor calls it, and it is inlined.
  The matching `operator new` (`mem_alloc(size, 0x80000000)`) goes with it
  (`ActorSelf`, part 10b).
- **An inline base destructor** is expanded into the derived one after the
  body, with its own vtable pointer store: the C's two stores (the class's
  table, then the base's) and the base's code after the body are g++'s
  (`~LogoActor` with `~ActorSelf`'s actor list unlink inlined; part 10b).
- **A pointer the ROM moves to another register mid-loop** (`mov r8, r7`
  after its last use, `mov r7, r8` before the loop's test, and r7 reused in
  between) is two variables: the loop's pointer, and a copy of it taken
  after its last use and copied back at the end (`next = pkg; ... pkg =
  next;`). flow folds the copy into the post-increment (`ldm r7!`). The
  two registers may still need a pin (`TitleScreen::LoadObjTiles`, part
  10c-2).
- **`(v << 1) | (v >> 31)` is a rotate** to combine, which emits `ror`.
  Where the ROM has the two shifts and the `orr`, write the left shift as
  `v * 2` (`TitleScreen::HashInput`, part 10c-2).
- **A loop that matched only with `-fno-strength-reduce` in C** may match
  with it on in C++, written as the plain indexed loop: the C's pointer
  walks were the workaround (`CompanyLogos::InitVvLogoPieces`, part 10c-2,
  which removed the project's only `-fno-strength-reduce`).
- **A value the ROM keeps in a call-clobbered register across a call**
  (saved to the stack just before the `bl`, loaded back later) ranked
  below every callee-saved candidate in the ROM's allocation; when g++
  ranks it higher (more references than the values it displaces) no
  spelling changes that, and the C's `MATCH_USE_MEM` stack slot is still
  the way (`SpawnFlamethrowerLabAssistant`'s mirror byte address, part
  9b).
- **A field address the ROM forms from another field's** (`subs r4, #8`
  after `r4 = this + 0xc8`) is reload's move2add: the two pseudos got the
  same hard register, and the second constant offset became an add to the
  first. Get the register allocation right and it follows; there is no
  source arithmetic to write (`PauseMenu::PauseMenu`, part 10d).
- **A strength-reduced loop's counter and pointer** swap registers when
  several loops share one counter variable: give each loop its own
  (`~PauseMenu`, part 10d).
- **`a[i] = p = new X`** computes the slot's address before the
  allocation, like `gX = new X` (part 9), and keeps `p` in r0 for the stores
  after it (the pause menu's icons, part 10d).
- **A union of `u16` and a `u16` bitfield struct takes a word** (a struct
  is 4-aligned on ARM): a padding field after it must go (`union
  MenuDispcnt`, part 10d).
- **Stores before the vtable pointer's are a base class's constructor.**
  g++ stores a class's vtable pointer after its bases' constructors and
  before its own body, so a constructor whose C sets fields and calls a
  function before the vtable store (InitActorPart's keyframes, palette
  and SetActorAnim) has a base class that does that: `AnimPart`, which
  another object (the airship) is built from alone (part 11a).
- **A virtual destructor both inline and out of line:** declare it in the
  class, define it `inline` after the class in the header under an
  `#ifndef`, and have the one `.cpp` that holds the ROM's copy define the
  macro and the destructor plainly at its place (`~ActorSelf`,
  `ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE`; part 11a).
- **An explicit destructor always stores its class's vtable,** even an
  empty `{}` one: g++ 2.9 skips the store only when the body emitted no
  insns at all, which only a synthesized (implicit) destructor's does.
  The store is dead, and goes, before an inline base destructor's own; it
  stays before a call of an out-of-line one. Where the ROM has none there
  (the balloon crates' kinds), the destructor was the implicit one: write
  the function g++ synthesized, with C linkage, and declare none in the
  class (part 11a).
- **A register holding 0 ORed into a value** (`movs r0, #0; orrs r3,
  r0`) can't be spelled: g++ folds the 0 from a variable, a parameter or
  an inline's argument. `MATCH_CONST(flag, 0)` right before the OR gives
  it (`DrawJetpackCheckpointText`, part 11a).
- **A PMF table in a data file with tables still read from C** moves to a
  data file of its own, split at its ROM address and linked right after
  the old one (actor_pmf_17c2b8.cpp, part 11i): a file is either C or
  C++, and the other tables' classes are another part's.
- **A pointer-to-member table in C++** (`const X::StateFunc X::t[] = {
  &X::f, ... }`) is the C's `{0, -1, fn}` records in `.rodata`, with no
  static constructor; the data file becomes a `.cpp` and the methods'
  mangled names map to their C ones (src/data/actor_pmf_17a6b8.cpp, part
  11a).
- **A memcpy of a struct onto itself** (`MemCopy32(p, p, 12)` right after
  a copy into `p`) is a returned struct bound to a `const &` parameter of
  an inline function: `BoxOverlap(WorldBox(a), WorldBox(b))`
  (actor_category_frame.cpp, part 11b).
- **An inline base destructor with an out-of-line copy in a file that
  also expands it** (actor_anim.cpp has the crate kinds' destructors and
  DestroyPolarCrate): define it `inline` in the header, and write the
  out-of-line copy as the C-linkage deleting destructor,
  `p->Base::~Base()` and the class's `operator delete` when bit 0 is set
  (part 11b).
- **A `new` with an inline operator new** evaluates the constructor's
  arguments after the allocation: `new X(&gTable[i], ...)` loads the
  table and scales the index after `mem_alloc` returns (CreateActor,
  part 11b).
- **A struct local assigned after a call** (`keys = gKeys.half;` after
  `SteerX()`) is copied through two more registers than one declared at
  that assignment (`struct held_pressed_pair keys = gKeys.half;`):
  `JetpackPlayer::StateRollLeft`, part 11e.
- **A multiplication the ROM does with `muls`** by a constant is by a
  variable holding it (`s32 pct = 0x14; pct * max`): a literal is
  strength-reduced to shifts and adds at expansion (`PassRing`, part 11e).
- **Two globals' addresses loaded before their stores** (`ldr r2, =a; ldr
  r1, =b; movs r0, #0; str r0, [r1]; str r0, [r2]`) are stores through
  pointers taken first; stored in place, each address is loaded just before
  its store (`PassRing`, part 11e; `AirshipStateApproach`, part 11i).
- **An OR's operand order picks the registers** of a call's argument:
  `(palette << 12) | tile`, with the tile in a local, puts the tile in r0 and
  the palette in r1, as the ROM has it, where `tile | (palette << 12)` swaps
  them (`JetpackPlayer::Draw`, part 11e).
- **A constant equal to an AND's mask** (`gKeys.all & 2`, then `state =
  2` on the taken path) is CSE'd to the mask's register (`str r1`), where
  the ROM loads it again (`movs r0, #2`). Make the mask a variable that
  the AND overwrites (`u32 bit = B_BUTTON; bit = keys &= bit;`): the
  register no longer holds 2 (`PolarPlayer::StateBoost`, `StateRun`, part
  11c).
- **Stores of a register just tested against 0** (`cmp r3, #0; bne`, then
  `str r3` for the zeros of a state reset) are an inline `SetState(st,
  idx)` after the test: CSE knows the register is 0 on that path and uses
  it for the inline's zero stores (`PolarPlayer::Launch`, `StateBoost`,
  part 11c).
- **Two tests that share a `delete this`** need no `goto`: written as two
  `delete this` in an `else if` chain, the copies are cross-jumped into the
  ROM's one tail (`JetpackBalloon::Update`, part 11f).
- **A value the ROM computes in the middle of an expression** (after a
  call in it) is an assignment inside that expression (`(scale2 = scale *
  2)` in `JetpackPlane::Aim`, part 11f); assigned as a statement before,
  it is computed before the call.
- **A `bool` parameter passed on the stack** is the ROM's `strb` by the
  caller and `ldrb` by the callee (`add rN, sp, #k; ldrb`), where a `u8`
  is a promoted word: a constructor's last argument can be a `bool` where
  the C needed a one-byte struct and an asm-label alias
  (`HovercraftSideGun`, part 11h). A `u8` handed on to it is converted
  first (`negs; lsrs`), so the caller's parameter is a `bool` too.
- **A base constructor's argument computed before the call** (the hit
  points of `HovercraftSideGun`, from a function call, ahead of
  InitActorPart's) is an expression in the mem-initializer list:
  `HpActor(rec, x, y, z, f() == 0 ? 0x18 : 0x10)` (part 11h).
- **A two-way diamond with a literal pool after its `b`**, where an `if`
  only sets a value, is an `if`/`else` of two stores of the field:
  cross-jumping merges the stores and keeps the branch, and the pool goes
  after it. A ternary or a conditional overwrite has no `b` (the side gun's
  X offset, part 11h, where the C needed an `asm` block with a `.pool`).
- **A `&&` chain with an inline box test** (`... && BoxOverlap(a, b) &&
  ...`) keeps the test's result in a register and tests it again; through
  a `u8`-returning inline wrapper the chain's branches are the ROM's
  (`PolarNitroCrate::DetonateNearby`, part 11d).
- **A constant argument loaded before another** (`movs r2, #0x80; lsls`
  before `movs r1, #4` for `PlaySfx(ctx, 4, 0x100)`) is a call written in
  place: the same call in an inline helper loads them in argument order
  (`PolarElectricFence::Update`, `PolarPenguin::Update`, part 11d).
- **Dead code of an inline with a constant argument** stays in the ROM's
  shape: ActorSelf::Draw's tail as an inline called with scale 0x140
  gives `DrawPolarCollectedWumpa`'s dead `flag = 0`, its folded size
  tests and its ORed 0x100 (part 11d), which the C wrote in asm.
- **A constructor both inlined and out of line in one file** (the kinds
  expand it, and the ROM has its own copy in the middle of the file) is two
  overloads: the out-of-line one at its place, and a protected inline one
  told apart by a parameter type the callers' arguments match exactly (an
  `s32` kind for an `int` literal, against the out-of-line `u8`), both on
  one inline body (`JetpackBalloonCrate`, part 11g). Define that body before
  the inline constructor: an inline function used before its definition is
  called out of line, and the link fails.
- **A constant argument loaded after the stack argument's store** (`str rN,
  [sp]; adds r0, r4, #0; ldr r3, =K; bl`) is a constant passed through an
  inline function's parameter, such as the inline base constructor's:
  `HpActor(rec, x, -0xFA00, z, 2)` (`JetpackParachuteNitro`, part 11g).
  Written directly as a call argument, it is loaded before the store.
- **A class with no vtable is invisible to tools/cpp_survey.py:** it has
  no vptr store, so its constructor and destructor aren't recognised.
  Look for a `Destroy*(self, flags)` that frees on bit 0, a caller that
  allocates and then calls an `Init*` returning `self`, and a state test
  repeated in the `flag = 0; if (x == k) flag = 1; if (flag)` shape (an
  inline `bool` method): the audio context was C++ ([The audio
  context](#the-audio-context)), where a copy of that flag into a second
  register had needed a pin.
- **Every expression statement adds a note in the C++ front end** (a
  NOTE_INSN_DELETED: the binding level opened for its temporaries), and
  loop.c counts notes in a register's lifetime. A long C function whose
  match depended on an invariant *not* being hoisted can hoist it as C++
  (InitActorCategory: two DMA constants spanning ten statements). Moving
  the statements between the invariant's uses into an inline function
  fixes it: inlining drops those notes ([C++ everywhere: actor, vehicle,
  level](#c-everywhere-actor-vehicle-level)). Compare the `.loop` dumps
  (`-da`) of both builds: the "regno N (life L)" lines show it.

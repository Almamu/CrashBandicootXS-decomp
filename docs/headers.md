# Headers and declarations

Every function and global is declared once, in a header, with the type of
its definition. Source files include the headers of what they use; they
don't declare things themselves. The compiler then checks every
declaration against the definition, and a declaration whose type would
change the generated code is a documented exception, not a silent local
copy.

This was set up by #574, which moved about 4,600 local `extern`s and 550
local structs out of the source files into headers, a subsystem at a time,
without changing a byte of the ROM. Its batch-by-batch plan, survey
numbers and per-change codegen findings were in `docs/headers_plan.md`,
removed in #844; the last commit that has it is `d1e4df8b`
(`git show d1e4df8b:docs/headers_plan.md`).

CONTRIBUTING.md's "Declarations and headers" is the short version of this
page, with the tools that check it (`tools/extern_audit.py`,
`tools/layout_audit.py`).

## Who owns a symbol

1. A function or global defined in a source file belongs to the header of
   that file's subsystem: `include/<area>/<namespace>/<subsystem>.h` or
   `.hpp`, in the directory that mirrors its `src/` one
   (`game/crates/crates.h`, `game/level/level.h`,
   `game/crash/player.hpp`, ...; docs/file_layout.md). A
   class's methods are declared in the class's header. When a function
   sits in a neighbour's file only for ROM order, it goes with its
   subject: `DestroyLargeFont`/`DestroySmallFont` are defined in
   `src/engine/util/aabb_setters.cpp` but declared in `text.h`.
2. A symbol defined only in data (`src/data/`, the generated level and
   graphics sources) or in the linker script (`sym_*.txt`, the IWRAM/EWRAM
   globals) goes in the header of the subsystem that uses it most, unless
   the subsystem it belongs to is clearer: `gSmallFont`/`gLargeFont` are
   mostly used by menus, but they are the text subsystem's fonts, so they
   are in `text.h`.
3. A data or linker-script symbol that **three or more subsystems** use
   goes in `include/globals.h` (`gPlayer`, `gLevelState`, `gKeys`,
   `gAudioContext`, ...).
4. References from one `src/data/` table to another (or to a `data/`
   label) stay in the data files. They are address-only and many are
   generated, so a header adds nothing.
5. `_call_via_r0`..`_call_via_r7` are not declared at all. They are
   libgcc's register-call thunks, which the compiler calls on its own for
   an indirect call; the code writes that call as a call through a typed
   function pointer, a virtual call or a member function call (#1021).
   Nothing in `src/`, `include/` or `lib/gax/` names them.

Other homes:

- GAX engine internals (what one GAX object calls in another):
  `lib/gax/src/gax_internal.h`. The libraries' public headers are in each
  library's `include/` (docs/libraries.md); libgcc's division routines are
  in `lib/libgcc/include/libgcc.h`.
- A struct that several subsystems share gets its own small type header
  (`aabb.h`, `hitbox_quad.h`, `line_util.h`) rather than living in one
  subsystem's header. A subsystem header includes the type headers its
  prototypes need.
- A struct that one file uses, and that no header needs, stays in that
  file. A struct defined twice (in two files, or in a file and a header)
  is a copy: merge it into one definition.
- `src/data/` has no header of its own: data is declared where it is
  used. A data file includes the header that declares its tables, so the
  compiler checks the definition against the declaration.

## What goes in a header

- **The definition's prototype**, copied, with the definition's
  parameter names.
- **Real types.** A global gets the type of its definition or, for linker
  script symbols, the type of the object stored there. Read-only ROM data
  is `const`. Arrays stay arrays and pointers stay pointers: `extern u8
  x[]` and `extern u8 *x` are different objects, so a mismatch is a bug,
  not a style choice.
- If the header's type doesn't fit a caller, fix the header (or the
  definition) to the real type and then fix the callers. A cast at the
  use, or fixing the type of the field or local that holds the value, is
  normally byte-neutral; check the object. Don't add a local copy with
  another type, unless it is a codegen exception (below).
- **A class in its namespace** (#1033, docs/cplusplus.md "Namespaces"):
  a `.hpp` declares its classes inside their namespace (`namespace
  Engine { namespace Sys { class Irq ... } }`), and the C declarations it
  includes stay outside it, global. A header never has `using
  namespace`: its inline code and its declarations name another
  namespace's class by its qualified name
  (`Engine::Sys::VramDmaQueue::Flush()`, and a base class: `class
  EnemyCtrl : public Game::Entities::Ctrl`, even inside `namespace
  Game`). The `.cpp` files name it with a
  `using` declaration after their includes (`using Engine::Sys::Irq;`).
  A C header that gives a global a C++ class type under `__cplusplus`
  forward declares the class in its namespace (globals.h's `gInput`, an
  `Engine::Sys::KeyInput *`). `extern "C"` functions are never in a
  namespace.

These are the prototype changes that most often change the generated
code. Rebuild and compare before assuming a change is neutral:

- **`u8`/`u16`/`s8`/`s16` vs `s32`** in returns and parameters: the caller
  truncates or extends again (`lsl`/`lsr #0x18`). An explicit `(u8)` cast
  at the call often hides the difference.
- **One-byte struct arguments** (`struct byte_arg`, passed on the stack
  with `strb`, where a `u8` argument is stored as a word).
- **Struct returns vs an explicit destination pointer**: the call can go
  through a stack temporary and the frame grows.
- **`const`/`volatile` on globals** can change load scheduling: through a
  `const` object gcc may keep a load across calls.
- **A global assigned a call's result** with a type other than the
  function's return type can move the global's address load before or
  after the call.
- **Unprototyped `f()` vs a prototype**: default argument promotion.

## What stays local

A declaration of something defined elsewhere stays in a source file only
for one of these reasons. `tools/extern_audit.py` sorts the declarations
in the `.c` files (`src/` data tables and `lib/`) by these kinds; anything
else is "remaining" and must move to a header ("remaining" is 0). It
doesn't read the C++ files, so their aliases are only in the table below.

| Kind | Why |
|---|---|
| Codegen aliases (`extern T Foo_x(...) asm("Foo");`) | the file needs another type for byte-identical code; each one is in "Codegen exceptions" below |
| `_call_via_r0`..`_call_via_r7` | none are left (rule 5 above) |
| Data-to-data references | rule 4 above |
| Library-internal | a library's own internal symbol, declared inside that library where its original source declares it (libgcc2.c's `__div0` and `__clz_tab`) |
| Documented | `src/engine/gfx/asset.cpp`'s one-argument `LZ77UnCompVram`/`RLUnCompVram` (docs/libraries.md); the `DOCUMENTED` set in `tools/extern_audit.py` |

## Codegen exceptions

When a file only matches with another type for a symbol than its
header's, it keeps a local declaration under another name, bound to the
real symbol with an asm label, and says why in a `codegen:` comment that
points here:

```c
/* codegen: RandRange returns u16 (util.h), but DrawLogoPieces was matched
 * against an s32 return: with the u16 prototype its two stack slots
 * swap. docs/headers.md */
extern "C" s32 RandRange_s32(s32 max) asm("RandRange");
```

The call is still `bl RandRange`, and the file can still include the
header. Redeclaring the real name with another type would be a
"conflicting types" error. Add every new alias to this table, and remove
its row when a later change makes the header's type match.

| File | Symbol | Local form | Header form | Why |
|---|---|---|---|---|
| src/game/ui/title_screen.cpp | `RandRange` | `s32 RandRange_s32(s32 max)` | `u16 RandRange(s32 max)` (util.h) | with the `u16` return, DrawLogoPieces's two stack slots swap |
| src/game/ui/level_select.cpp | `gLevelSelectGemPos`, `gLevelSelectTrialIconPos` | `struct vec2 gLevelSelectGemPos_rw` (and `_rw` for the other) | `const struct vec2` (menus.hpp) | the constructor reads each twice across calls and the ROM loads it again each time; through the `const` object gcc keeps the first loads in registers |
| src/engine/text/font.cpp | `GetPaletteSlot` | `s32 GetPaletteSlot_s32(u8 *cache, s32 recordId)` | `u8 PaletteCache::GetSlot(s32 id)` (sprite_obj.hpp) | ResetPalette uses the slot as a word; through the `u8` return the shift is `lsl #0x18; lsr #0x14` for the ROM's `lsl #4` |
| src/game/level/play_room.cpp | `AddPaletteCycle` | `void AddPaletteCycle_fx(..., struct fx_direction direction)` | `PaletteCycles::Add(..., struct byte_arg direction)` (part_list.hpp) | RunRoom passes the direction as a one-byte struct stored with `strb`; `fx_direction`'s zero-length pad makes it BLKmode, so the slot address (`add rN, sp, #4`) comes before the constant as in the ROM, where a `byte_arg` or `u8` is built in a register first |
| src/game/bosses/dingodile.cpp | `GetSpriteAttackBox` | `struct aabb GetSpriteAttackBox_s(void *part)` | explicit destination (objects.h) | assigned to a box that already holds the body box, the struct return fills a temporary and copies it, as the ROM's frame and its `ldmia`/`stmia` copy do |
| src/engine/actor/actor_category_init.cpp, src/engine/actor/actor_category_stats.cpp | `SetCheckpointAtPlayer` | `void SetCheckpointAtPlayer_1(void *self)` | `LevelState::SetCheckpointAtPlayer(u8 checkpointFlags)` (level_state.hpp) | these callers pass the state only and leave r1 as it is; the definition stores the flag |

Not aliases, but kept for the same reason, next to the real declaration
in the header:

- `include/game/crates/crate_list.hpp`'s `struct CrateGridNodeInit`, an untyped view
  of the grid nodes that ResetGrid zeroes them through: through the real
  `CrateGridNode *` fields gcc takes the zeroing stores as possible writes
  to the list's `nodes` and reloads it (docs/casts/crate_grid.md).
- `include/game/crates/crate.hpp`'s `ApplyCrateCollision`, `Crate::ApplyCollision`
  under its C name, with the three flags as one-byte structs so they are
  stored with `strb`, as CollisionQueue::Resolve does in the ROM.

`src/engine/util/string.cpp`'s `CopyString`/`StringLength` are asm labels too,
but on the definitions (`strcpy`/`strlen` under other C names, to avoid
gcc's built-in prototypes), not codegen aliases.

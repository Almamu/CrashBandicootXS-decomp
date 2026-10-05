# Issue #24: 0x0801967C-0x0801A794, graphics - actor-part controller classes and a boss state machine

All 25 functions of the former `asm/code_3_2_17_188d0_1967c.s` now live
in the new `src/graphics/actor_part_1967c.c` (named by address, like
`actor_part_17524.c`). **23 are real C; `sub_801A03C` and `sub_801A114`
are NAKED transcriptions** with complete C reconstructions kept under
`#if NON_MATCHING`. Issue #24 stays open for those two.

Verified with a clean `make NON_MATCHING=1 report` (no warnings from this
file) and a clean `make compare` (`crashbandicootxs.gba: OK`).

## Headline findings (useful beyond this chunk)

### 1. This code was compiled with `old_agbcc`

`tools/agbcc/bin/old_agbcc` has always been in the tree (`CC1_OLD` in the
Makefile) but no file used it. Here it reproduces the ROM's
"constant first" order for byte read-modify-writes, which the current
`agbcc` never produces:

```
ROM / old_agbcc                 agbcc
movs r0, #5                     ldrb r1, [r2, #0xc]
rsbs r0, r0, #0                 movs r0, #5
ldrb r1, [r2, #0xc]             rsbs r0, r0, #0
ands r0, r1                     ands r0, r1
```

The same holds for `|= 0x10`, for 4-bit bitfield inserts (`movs r1,#0x10;
rsbs` before the `ldrb`), and for byte stores of a constant through an
inline setter. Other files handled this pattern with per-site register
pins (`actor_part_17524.c`'s `register s32 m asm("r0") = 0x7F`,
`graphics_loading_1fdec.c`'s inline-asm `mov r1, #0x10; neg r1, r1`).
With `old_agbcc` the natural C gives these sequences with no pins.
`sub_801A584`, for example, matches as plain C under `old_agbcc` and
does not match at all under `agbcc`.

The Makefile now has an `OLD_AGBCC_OBJS` list. Objects in it are
compiled with `$(CC1_OLD)`, and `-fprologue-bugfix` is dropped because
`old_agbcc` does not accept it. **This is probably worth trying on
neighbouring issue chunks.** `actor_part_17524.c` (issue #21), right
before this one, has the same pin-shaped idioms.

### 2. Virtual calls are indirect calls through `_call_via_rN`

`_call_via_r1`/`AD80`/`AD84`/`AD88` (`bx r1`..`bx r4`,
`src/system/reg_trampolines.c`) are this ROM's copies of libgcc's
`_call_via_rN`. A C++ virtual call is really
`((fn_t)m->fn)(this + m->thisOffset, args...)`, and gcc puts the function
pointer in the first free argument register, which gives exactly the
ROM's r1/r2/r3/r4 choice. This file writes them as real indirect calls
(`VCALL1`/`VCALL2`) and aliases the helpers with a top-level

```c
asm(".set _call_via_r3, sub_803AD84\n" ...); /* since dropped: the trampoline is now named _call_via_r3 itself */
```

This resolves each `bl _call_via_rN` relocation to the ROM's trampoline.
Writing indirect calls (instead of calling `_call_via_r3(..., fn)` with the
pointer as an explicit argument) matters in two places:

- the 4-argument `_call_via_r4` call needs no `register void *fn
  asm("r4")` "dead read" trick;
- `SetDingodileState`: its cases share one `bl` (see below).

### 3. The AABB builders return their box by value

`GetSpriteAttackBox`/`GetSpriteBodyBox`/`GetSpriteHitbox` are declared elsewhere as
`void *f(void *dest, void *part)`. They are really
`struct box f(struct part *)`: gcc passes the hidden result pointer in r0
and returns it. Declaring them this way reproduces the ROM's argument
order: the player global is loaded into r1 *before* the temporary's
address goes into r0. A plain pointer argument evaluates it the other way
round. The fallback `if (!valid) box = playerBox` is
`struct box *pb = &b; *pb = GetSpriteAttackBox(player);`: assigning through the
pointer makes gcc build the result in a temporary (the ROM's third
16-byte stack slot) and copy it with `ldmia`/`stmia`.

The ROM re-reads the just-filled `box.valid` from its stack slot
(`ldr r0, [sp, #0x18]`), not through the register that holds the box
address. gcc's CSE rewrites that address. A volatile read (`BOX_VALID()`)
is the only thing found that stops it.

## What the code is

Small C++ classes, each with a method table at `self+0x0C`. The tables
are 0x68 bytes apart, and each entry is `{s16 thisOffset; u16; fn}`.
The entry at +0x08 (function at +0x0C) is the per-frame update and the
entry at +0x48 (+0x4C) is the destructor. Found by scanning the ROM for
Thumb pointers:

| table | ctor | dtor | other methods |
|---|---|---|---|
| `gStaticData_087E4704` | `sub_80196F8` | `sub_80196E4` | `sub_80196B8` (position/delta setter, `gStaticData_0816C358` byte lookup) |
| `gStaticData_087E476C` | `sub_8019758` | `sub_8019744` | `sub_8019730` (update), `sub_8019770` (set-state wrapper; state 3 also pokes its part's controller and `SpawnBodySlamPower`) |
| `gCortexBossVtable` | `CreateCortexBoss` | `DestroyCortexBoss` | - |
| `gStaticData_087E483C` | `sub_801A724` | `sub_801A73C` | `sub_801A64C` (update: sets the part's velocity from `gStaticData_0816C3B8`, removes it past either level edge) |
| `gStaticData_087E48A4` | `sub_801A768` | `sub_801A750` | `sub_801A2A8` (update) |
| `gStaticData_087E490C` | `sub_801A794` (issue #25) | `sub_801A780` | `sub_801A114` (update) |
| `gDingodileVtable` | (issue #25 range) | `DestroyDingodile` (issue #25) | `UpdateDingodile` (update) |

`sub_801967C` sets `kind` (+0x0A) on every part in `gUnknown_030012EC`'s list.
`sub_8019718`/`sub_80197F4` are **UNUSED**: no `bl`, no `.4byte` and no
Thumb pointer anywhere in the ROM. They are matched anyway.

The `087E4974` object (`struct dingodile_boss`) is a boss-like state machine.
`UpdateDingodile` keeps a companion part 6 px ahead of the boss, counts hits
(overlap of the player's box with the boss's hurt box while the player's
`kind` is 0x13), walks the boss along the level using the approach
tables `gStaticData_0816C368/78` (facing) and `0816C390/A0`, turns it
round at either level edge, spawns projectiles (`sub_8019EBC` mode 1)
on animation frame 0x14, and finally, once the part falls below the
level, signals `RequestRoomExit` (the "entity ready" barrier in
`docs/rom_map.md`). `SetDingodileState` is its "enter state N" routine: it
calls the object's slot +0x20 method and then runs the state's
animation, spawns and sounds. `sub_8019EBC`, `sub_801A03C` and
`sub_801A584` spawn parts through `CreateMovingSprite`. `sub_8019EBC` reads the
level's "collected" bits (`gEntityFlags->info`) into the new part's
`+0x28` flags.

`struct part` gained a few named fields: `+0x0C` flags (bit 0 gone,
bit 2 shown, bit 4 active, bit 6 hit), `+0x0D` bit 2 blink, `+0x28`
(`struct part_f28`: 2-bit mode, a signed 1-bit `facing`, a flag), `+0x29`
low nibble slot, `+0x2D` tag, `+0x30` frame, `+0x38` "animation done",
`+0x44` controller, `+0x104` busy (player only).

## Matching notes (real C)

- **Signed `facing` bitfield**: the ROM tests `+0x28` bit 4 with
  `lsl #27; bge`. That comes from an `s32 facing:1` field (an unsigned
  1-bit field gives `movs #0x10; ands`). `struct part_f28` is
  `__attribute__((packed))` so that the `s32` bitfield does not widen
  the struct. The same applies to the `fl` union at `+0x0C`: without
  `packed`, ARM's 4-byte struct alignment moves every later field.
- **`SetDingodileState`'s shared call**: three states differ only in the
  animation id they pass to slot +0x50. The ROM loads `this`, the
  function pointer and `other` separately in each case, then branches to
  one shared `bl`. `PREP_VCALL2` plus `goto call` gives that, with the
  four argument locals pinned to r0-r3. Without the pins, gcc hoists the
  `other` move into the shared block.
- **`(u16)(width + n)`**: the ROM computes this in the upper halfword
  (`lsl #16; add #n<<16; lsr #16`) from a *word* load. An `s16` local gives
  that shape but narrows the load to `ldrh`. The `LayerWidthPlus()`
  macro spells out the shift.
- **Statement order drives load order**: `s32 x = other->x;` before
  comparing against the level edge (several places), per-branch
  `x`/`y`/`p` loads in `UpdateDingodile`'s prologue, and `x >>= 8` read then
  adjusted in the turn-round snap.
- **Small inlines shape registers**: `LevelRight()`/`LevelBottom()`
  (Q8 level edges), `AtLevelEdge()` returning a comparison (the ROM's
  `hit` value copied into r0), `Approach()` (the ROM's distance lands in
  r0), and `SetTag()` (a setter parameter makes old_agbcc load the tag
  constant before the address).
- `sub_8019EBC` case 0 holds the constant 1 in r5 across three calls for
  the `kind` store: an `s32 kind = 1` local does this; a `u8` or a
  literal does not. `mode`/`kind` otherwise match via a `switch`, not an
  if-chain (the ROM puts the default block first).
- `sub_8019EBC` mode 0 attaches a new `087E490C` controller
  (`struct obj_490c`, 0x28 bytes, `+0x24` target = the boss's part) to
  the part it spawns. `sub_801A114` then blinks that part.
- `sub_801A2A8`'s hit test is `(fl.raw >> 6) & 1`, a value extract rather
  than a bit test.
- `sub_801A64C`/`sub_801A2A8`'s "mark gone" (`MarkCollected`, the
  `MarkEntityGone` bitmap idiom) matches as a plain inline under
  `old_agbcc`: `w /= 32` gives the ROM's `asr`, and loading the global
  before copying the id gives its register order. When `MarkCollected` is
  inlined twice, gcc cross-jumps the two copies into the ROM's shared
  tail.

## NAKED

### `sub_801A03C` (floor-part spawner)

The only difference is register allocation. gcc gives the new
controller r4 and the part r5. The ROM has the part in r4 and the
controller in r5, and keeps the constant 1 for `facing & 1` in r8 while
still loading a separate `mov r0, #1` for the tag store. Tried: inline
and plain tag stores; a named `one` variable (`s32`/`u32`/`u8`, declared
or chained as `p->tag = one = 1`); an inline facing setter; declaration
order and scoping of the controller; re-reading `p->ctl`. Pinning the
part to r4 blocks the controller's own `add r5, r5, r0` at the second
call. Pinning `one` to r8 drops r8 from push/pop (the known agbcc
callee-saved-register bug), so it is not an option.

### `sub_801A114` (`087E490C` controller's per-frame update)

The ROM keeps `other` in r9, `&gPlayer` in r10, the player-box
pointer in r8 and `self` in r7, which leaves r4-r6 for temporaries. gcc
spreads the same four values over r4-r7/r8. State 0's `BLDCNT|BLDALPHA`
word is also built by eight separate `orr`s in two accumulators (r5,
then r0), and gcc constant-folds those even through register variables.
Pinning `other`/the global address/the box pointer to r9/r10/r8 plus
per-step `asm("" : "+r")` barriers gets the prologue and state 0 close,
but `self` then lands in r6 (r7 cannot be pinned) and an extra box
temporary appears on the stack.

## Later pass (issue #12/#24/#26 NAKED retry)

`sub_801A03C` is real C now: its two virtual calls are plain blocks
(`VCALL1_B`) instead of VCALL1's `do { } while (0)`, whose loop notes
swapped the part/controller registers, and `facing` is stored into the
1-bit field unmasked (the explicit `& 1` made the tag store reuse the
held constant 1). `sub_801A114` stays NAKED; its state-0 `orr` chain is
now reproduced (see
[issue-24-26-12-naked-retry.md](issue-24-26-12-naked-retry.md)).

## Later pass: hard-register hold

`sub_801A114` is now real C (old_agbcc, also identical under agbcc).
r5 and r6 held live across the box builders make global-alloc start
the long-lived values at r7, as the gap4 note predicted (159 -> 16
halfwords). The state-0 BLDCNT accumulator is a block-scoped r5
variable initialised through the constant-init asm (16 -> 0). See
[hard-register-hold-retry.md](hard-register-hold-retry.md).

# Issue #21: 0x08017524-0x08017A44, graphics - a D-pad-driven actor-part controller

All 25 functions are byte-exact matched as plain C in the new
`src/graphics/actor_part_17524.c` (named by address, following
`actor_part_12fbc.c`'s precedent, since what this object controls in-game
isn't established). It replaces the tail of `asm/code_3_2_17_16048.s`,
which now ends at `sub_801751C` (the rest of that file is issue #20).
Built with old_agbcc since a later pass (see "Later pass: old_agbcc" at the
end); the pins and barriers described under "Matching notes" are gone.

## What the code is

- `sub_8017524`-`sub_8017554`: byte accessors for an object not otherwise
  characterized here - clear/read the `+0x2C`/`+0x2D` flags, or set a flag
  and its value byte (`+0x24`/`+0x25`).
- The rest are one actor-part subclass, `struct input_ctrl`:

| offset | field |
|---|---|
| 0x04 | animation set: pointer to an array of `{u32 a, u32 b}` record indices |
| 0x08 | state (0-3; 3 skips the input handling) |
| 0x0C | method table (`gInputCtrlVtable`) |
| 0x10 | target actor |
| 0x14/0x15 | current animation pair per channel (`animA`/`animB`) |
| 0x16 | up/down latch (0 none, 1 up, 2 down) |
| 0x17/0x18 | channel dirty flags; 0x19/0x1A alternate-method flags |
| 0x1C | child object (0x80 bytes, `CreateCameraLead`), its `+0x78` a speed-like value |
| 0x20/0x24 | a left-hold flag and timer |

Found by scanning the ROM for Thumb pointers: the method table holds
`UpdateInputCtrl` (+0x0C, per-frame update), `sub_80179D4` (+0x14),
`AttachInputCtrl` (+0x1C, set target) and `DestroyInputCtrl` (+0x4C, destroy); the
slots this code calls through (+0x24 ... +0x54) are all base-class
`sub_800B6xx`/`sub_800B8xx` functions, and `CreateInputCtrl` (constructor,
called from `game_loop39.c`) chains to the base constructor `InitCtrl`.

`UpdateInputCtrl` reads the held keys (`gKeys`): up/down pick
channel B's pair (3/5, back to 0 when neither is held), left/right channel
A's (7 with speed 0x3200 while the left-hold flag lasts - it expires after
30 frames held, then re-arms once a 10-frame countdown has run out with
left released - 8 with speed 0xA00, else 1 with speed 0x1E00). Once the target's x passes the level's right edge
(`gLevelLayers`'s layer 0 width, less 0xA00) the child is marked gone
and `RequestRoomExit` is signalled. `sub_8017808` then applies any dirty
channel through the method table using `gInputCtrlMotionRecords`'s 12-byte
records.

The state dispatch at the end of `UpdateInputCtrl` goes through
`gInputCtrlStateFuncs`, a table of gcc 2.x pointer-to-member-functions
(`{s16 delta; s16 index; union {fn, s16 vtable offset}}`): state 0
`sub_8017600` (reset the pairs, spawn the child), 1 `sub_801796C`,
2 `sub_801793C`, 3 `sub_80178EC` (mark the target gone). That call
sequence, and the `_call_via_r1`/`AD80`/`AD84` trampolines every virtual
call goes through (`bx r1`/`r2`/`r3` - gcc's `_call_via_rN` interworking
thunks), are what gcc's C++ front end emits, so this object was very
likely written in C++. The C here models the vtable/PMF layouts as
structs; nothing is renamed.

UNUSED - no reference anywhere (the asm/ sources, src/, and a whole-ROM
Thumb pointer scan): the six byte accessors and `sub_8017A20`-
`sub_8017A40`. Matched anyway.

## Matching notes

- **Virtual calls** take the method-table entry's address once
  (`struct ctrl_method *m = &vtable->method_50;`) and read the `this`
  adjustment and function through it - what gcc 2.x's own virtual-call
  lowering looks like. Indexing the table directly for each field gives
  the right instructions for small offsets but not for entries at 0x40+.
- **Stored constants before loads.** Several stores in the ROM build the
  constant before loading anything the store needs (`dirtyA = 1;
  animA = 2;` materializes the 2 first; the child's `+0x78` speed
  constant before the child pointer). Inline helpers `SetAnimA`/`SetAnimB`
  /`SetChildSpeed` reproduce this - the argument is evaluated before the
  inlined body runs. The same idea as `tile_slot_pool.c`'s (#43) helpers.
- **The "mark actor gone" sequence** (`MarkEntityGone`'s, inlined in
  `sub_80178EC` and `UpdateInputCtrl`). Earlier matches of this sequence
  (`actor_part27c.c`, `actor_part39.c`, `actor_part124.c`, ...) needed an
  inline-asm `add`/`asr` pair. Here it's plain C: the ROM's copy +
  `asr #5` + subtract is gcc's **signed** `/ 32` and `% 32` of the
  zero-extended id (the sign fix-up is then dropped as provably dead), so
  the word index is `register s32 word = id; word /= 32;` and the bit is
  `id - word * 32`. The id is re-read after the `0xFFFF` test through a
  `volatile` access (the ROM reloads it), and register pins reproduce the
  ROM's allocation, including callee-saved r4/r5 in what is otherwise a
  leaf path. The `flags |= 1` update uses the same pinned-statement form
  as `MarkEntityGone` (constant first).
- **`sub_80179D4`'s range check** (`arg2` in 1..4) keeps its lower bound
  in a local: with a literal, gcc folds `>= 1` into `> 0` and merges the
  two tests into one unsigned range check; the ROM has `cmp #1 / blt`,
  `cmp #4 / bgt`.
- **`sub_8017808`** needs pins on the animation-set base (r1), the index
  (r2, behind an empty `asm("" : "+r")`), and the entry pointer (r0,
  computed as `(idx << 3) + base` for the ROM's operand order).
- **`InputCtrlKillPlayer`** loads `gPaletteCache` and computes the tag byte's
  address before loading the record table - done with explicit
  statements; `+0x29`'s slot is a 4-bit bitfield (`lsl #28/lsr #28`).
- **`UpdateInputCtrl`'s PMF dispatch** re-indexes `gInputCtrlStateFuncs[state]`
  for each field and copies the virtual entry through a stack struct,
  which is what gives the ROM's `sub sp, #8` frame; the up/down `else if`
  chain is written with a pinned `dirState` read and a `goto` (the only
  form found that loads it into r1 as the ROM does).

## Later pass: old_agbcc

`actor_part_17524.o` is on the Makefile's `OLD_AGBCC_OBJS` now, like
`actor_part_16048.o` before it (issue #20 showed `InputCtrlKillPlayer` stripped
of its pins matches under old_agbcc and is 30 bytes off under agbcc).
All 25 functions still match. What became unnecessary:

- **`InputCtrlKillPlayer`**: all four pinned blocks. The two flag clears are
  bitfield stores (`target->flag7 = 0; target->flag6 = 0;`, new 1-bit
  fields at `+0x0C`), `+0x104` is a plain `unk_104 = 1`, and the
  `LoadPaletteSlot` call reads `t->table->records[t->tag * 28 + 0x14]`
  directly. The two `asm("" : "+r")` barriers are gone. What remains is
  the ordering `cache = gPaletteCache;` before `t = self->target;`.
- **The "mark gone" sequence** (`sub_80178EC` and `UpdateInputCtrl`): no pins
  and no `volatile` id re-read. It is `MARK_GONE(t)` - `t->gone = 1`, then
  `SET_ID_BIT(t->field_08)` unless the id is `0xFFFF` - the same
  sequence as `actor_part_16048.c`'s `MarkGone`. `SET_ID_BIT` stays a
  `do`/`while (0)` on purpose: its loop notes are what reproduce the id
  reload. The target/child pointer is loaded into a local first.
- **`UpdateInputCtrl`'s up/down chain**: the pinned `dirState` read and the
  `goto` are gone. It is an ordinary `if`/`else if` chain.
- **`sub_8017808`**: all pins and both `asm("" : "+r")` barriers. The
  record is `gInputCtrlMotionRecords + self->animSet->entries[idx].a * 12`,
  and the `dirtyB` test is a plain `if`.
- The virtual-call macros use `if (1) { } else (void)0` instead of
  `do`/`while (0)` (both match here; this is the form
  `include/actor_self.h` uses).

Still needed under old_agbcc: `sub_80179D4`'s `s32 lo = 1` lower bound
(with a literal `1`, gcc folds `>= 1` into `> 0`), and the
`SetAnimA`/`SetAnimB`/`SetChildSpeed` inline helpers (written inline,
`UpdateInputCtrl` and `sub_801793C` stop matching).

`tools/patch_expected_target.py`/`expected/corrections.txt` need nothing
here: the frozen disassembly labels all 25 correctly.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: OK`).

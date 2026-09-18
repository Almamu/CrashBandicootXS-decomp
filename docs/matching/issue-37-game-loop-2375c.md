# Issue #37 follow-up: `sub_802375C` matched, `sub_8023A1C` still raw

[docs/matching/issue-37-game-loop-234e8.md](issue-37-game-loop-234e8.md)
left this pair (`asm/code_3_2_17_2375c.s`, ROM `0x0802375C`-`0x08024007`)
completely untouched as "not yet confidently understood branch-by-branch."
This pass picks the smaller of the two back up.

## `sub_802375C` - matched (`src/system/game_loop39.c`)

A level-start dispatcher, called once from `UpdateGameFrame` when the
level object's own `+0xdc->+8` state field is `2`
(`asm/code_3_2_17_225a0.s`). It:

1. Fires two no-argument setup calls (`sub_8022208`, `sub_8024198`).
2. Allocates the whole per-level widget set: five `dual_array_manager`s
   (`gUnknown_030012E8`/`EC`/`F0`/`F8`/`F4`, the same struct
   `actor_part11.c`'s `sub_8008EE4` already returns) and one
   `pool_manager` (`gUnknown_0300130C`, `sub_8008F20`'s own type from
   `actor_part12.c`), a generic 0x18-byte block (`gUnknown_030012D4`),
   the text-box singleton (`gUnknown_03001308`, lazily built by the
   still-raw `sub_80268AC`), and the player actor itself
   (`gUnknown_030012D8`, a 0x350-byte block handed to `sub_800B3F0`
   - the same constructor `actor_part77.c` already matched, called here
   with a genuine 5th stack argument the matched 4-parameter signature
   there simply never touches).
3. Sets the player's position from `self+0x10`/`0x14`
   (`sub_8007398`), sets `player+0xc` bit 4, and mirrors `self+0x1c`
   bit 0 into `player+0x28` bit 4.
4. Reads the level-state record's (`self->0x18`) own `+8` "widget kind"
   field and dispatches on it (0/1/2) to construct one of three HUD
   counter/ring-buffer widgets, each via
   `sub_801588C`/`sub_80174EC`/`sub_8017A00` (three different
   constructors, still raw) plus a `gStaticData_0816B92C`/`0816B934`/
   `0816B93C` action-table pointer stashed at `widget+4`
   (`sub_800B69C`). Widget kind `1` additionally builds an OAM entry via
   the standard `sub_80087C0`/`sub_80087B4`/`sub_800872C` trio. All
   three cases finish by pointing `player+0x20`/`0x44` at the freshly
   built table/widget and firing `sub_803AD80` on it.
5. Unconditionally calls `sub_8023A1C` (see below) and stashes its
   return value.
6. Tears the per-frame update queues back down: `sub_802680C` on the
   text-box singleton if non-NULL, `sub_8026ED0` on the 0x18-byte
   block, a `sub_803AD80` call on the player object if non-NULL, then
   `sub_8008EB4`/`sub_8009B9C` on each of the six widget-manager
   globals if non-NULL, and finally `sub_80221F0`.
7. Returns `sub_8023A1C`'s result.

### Gotchas worth recording

- **The seven allocation stores need their destination address computed
  *before* the two calls that produce the value**, not after (the
  natural C evaluation order for a plain `global = fn(fn2(...))`
  assignment computes the address *after* the RHS). Fixed with the
  established `T **slot = &global; *slot = fn(fn2(...));` idiom - this
  reproduces the ROM's `ldr r4, =global` sitting *before* both `bl`s
  (with `r4` surviving them, since it's callee-saved) rather than a
  fresh `ldr r1, =global` computed afterward. The lone exception is the
  single-call `gUnknown_03001308 = sub_80268AC();` assignment, which
  the ROM computes address-after-value for (only one call in the way,
  so no register needs to survive it) - the plain, un-idiomed form
  reproduces that one directly.
- **`gUnknown_030012D8`'s own address stays live in `r7` across the
  entire construction-and-dispatch section** (from its own allocation
  through all three switch cases), reused via repeated *fresh*
  `ldr r1/r2/r4, [r7]` reloads rather than ever being cached in a
  second local - by the time the function reaches the post-`sub_8023A1C`
  teardown section, the ROM re-derives the address from scratch again
  (a fresh PC-relative literal, no `r7` left alive). Reproduced with one
  `void **d8 = &gUnknown_030012D8;` local declared once near the top and
  referenced as `*d8` throughout construction/dispatch, while the
  teardown section refers to `gUnknown_030012D8` by name again instead.
- **Every "fresh reload, do two field stores, then use the same pointer
  as a trailing call argument" case-block shape needs its *source
  value* computed before its *destination pointer*.** All three switch
  cases (and the `player+0xc`/`+0x28` bit-manipulation pair right before
  them) write to `player+0x20` and `player+0x44` (or similar) back to
  back; the ROM computes whatever's being stored (`**gUnknown_030012D0`,
  a table pointer, etc.) *first*, then reloads `gUnknown_030012D8`'s
  value into a register, then does both stores through it. A plain
  `u8 *pl = *d8; *(void**)(pl+0x20) = value;` computes `pl` first
  instead. Fixed by hoisting the value into its own local declared
  *before* the `pl` local in every such block.
- **`self` is pinned to `r8` for the entire function** (it has to
  survive dozens of `bl`s while r4-r7 are already busy with other live
  locals) via `register u8 *self asm("r8") = selfArg;`. Thumb can't
  address `[r8, #imm]` directly, so every field access needs its own
  `mov rN, r8` first - reproduced by wrapping each access in its own
  small block with a `register u8 *p asm("rN") = self;` local, matching
  the ROM's register choice at each individual site (mostly `r4`, once
  `r1`).
- **The `player+0xc |= 0x10` store** needs its *mask value* (`0x10`)
  loaded before the *current byte* is read, the opposite order a plain
  `*p |= 0x10;` compiles to. Fixed the same way as the case-block
  gotcha above, with explicit register-pinned locals in the ROM's exact
  order (`val` before `cur`).
- **The `player+0x28` bit-clear/set mask (`& ~0x10 | bit`)**: written
  as `~0x10`, this compiler constant-folds the whole mask to a single
  immediate load - one instruction short of the ROM's real `movs r0,
  #0x11 / rsbs r0, r0, #0` runtime negation. The established
  "negative-constant bit-clear idiom" (`docs/matching.md`,
  `sub_800A70C` in `actor_part14.c`) fixes it, but *only* when the
  negative literal (`-0x11`) is bound to a `register ... asm("r0")`
  local first - a bare `*p & -0x11` inline still constant-folds despite
  being the "right" literal, since nothing forces the compiler to treat
  it as a runtime value rather than a compile-time one. This one 2-byte
  gap is also why the function's overall size looked 4 bytes short
  during isolated compiles: the missing instruction plus its knock-on
  4-byte alignment padding before the following literal pool.
- **The `bit = (self->0x1c & 1) << 4` computation's operand order
  matters**: the ROM loads the literal `1` into `r1` *before* reading
  `self->0x1c` into `r4` (reusing the same register the pointer copy
  was just in, i.e. `ldrb r4, [r4, #0x1c]`), not after. A plain
  `(self[0x1c] & 1) << 4` computes the load first. Fixed by writing
  `(one & rawbit) << 4` with `one` a `register u32 asm("r1") = 1;`
  local declared *before* the register-pinned `self[0x1c]` read.
- **A redundant `self`-from-`r8` reload right before the mode fetch**:
  the ROM re-issues `mov r4, r8` immediately after the bit-manipulation
  block above, even though `r4` already holds that exact value and
  nothing has clobbered it - this compiler's own value tracking
  otherwise elides the second `mov`. An `asm volatile("" ::: "r4");`
  barrier between the two register-pinned blocks forces the reload
  back in, matching the ROM's (mildly wasteful) instruction count.
- **The 3-case switch on `mode` (0/1/2) compiles to a binary-search
  comparison tree** (`cmp mode,1; beq case1; cmp mode,1; bgt (>1 ?
  case2 : default); cmp mode,0; beq case0; else default`), not a linear
  if/else-if chain or a jump table - reproduced directly by writing a
  plain `switch (mode) { case 0: ...; case 1: ...; case 2: ...; }` and
  letting this compiler's own switch lowering pick that shape, rather
  than hand-writing the comparison order.
- **`sub_800B3F0`'s 5th argument**: the matched 4-parameter signature in
  `actor_part77.c` never reads a 5th argument, but this call site (and,
  per that file's own doc comment, `sub_8008434` elsewhere in the same
  neighborhood) passes one anyway - a real stack argument (`str r2,
  [sp]` sitting *before* the register arguments are even fully loaded,
  reusing whichever register already held `0`). Declared here with an
  unprototyped `extern void *sub_800B3F0();` so the call `sub_800B3F0(ptr,
  0xffff, 0, 0, 0)` can pass the extra trailing `0` the ROM's own
  (differently-prototyped, in this translation unit) declaration
  allowed.

## `sub_8023A1C` - still raw

The ~650-instruction jump-table-driven continuation this function calls.
Its real bytes now live in `asm/code_3_2_17_23a1c.s` (renamed from
`asm/code_3_2_17_2375c.s`, since that file no longer starts at
`0x0802375C`). Left untouched for this pass - see
`docs/rom_map.md`'s "Traced the fade-to-black's trigger" and "Resolved:
`sub_80241B0`'s gate" sections for what's already understood about its
6-case jump table, the `gStaticData_0816C86C` per-level table it indexes,
and its wait-loop/fade/post-fade structure. `sub_8027018`'s exact
6-argument call shape (self, targets, lists, angle, list_count,
direction - matched in `hud_icon_slot.c`) is now confirmed against all
four of this function's own call sites, closing one of the previously
open questions; the `gStaticData_0816C81E`/`0816C830`/`0816C842`/
`0816C862` tables it points at are still just `u16*`/opaque data,
their record shape not derived. A good next target for a dedicated pass.

See [docs/status/game_loop.md](../status/game_loop.md) for the updated
matched/parked/raw lists.

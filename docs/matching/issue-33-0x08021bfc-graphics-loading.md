# Issue #33: 0x08021BFC-0x08022354 - dispatch trampolines and the game-loop "origin point"

GitHub issue #33 (`decomp-chunk`, category `graphics_loading`) listed 25 raw
functions in `asm/code_3_2_17_1e990.s`, immediately following the
still-raw remainder of the `LoadGraphicsPackage` cluster (issue #30) and
the "trigger effect type N" family (issue #31). This is the write-up for
the work done against that list.

## What this cluster turned out to be

More slots of the unified ~92-slot function-pointer dispatch array
docs/rom_map.md documents (`gStaticData_0816C6A4`), plus one landmark
function at the very end:

- **`sub_8021BFC`/`sub_8021C50`-`sub_8021CE0`/`sub_8021D04`**: more
  instances of the already-documented `sub_800FF0C` entity-constructor
  trampoline family, feeding the 93-entry `gStaticData_087Exxx` family
  with type constants `1`-`7`. `sub_8021D04` additionally indexes a
  small per-record flags byte via `gUnknown_030012B4`'s own table (same
  shape as `sub_80187FC`'s table read in `actor_part27c.c`) and folds
  two of its bits into the constructed object's `+0x28` bitfield.
- **`sub_8021D80`/`sub_8021DFC`/`sub_8021E78`/`sub_8021EF4`/
  `sub_802200C`/`sub_8021F70`/`sub_80220C4`**: the `gStaticData_084A5600`
  record-indexed OAM-trio spawner shape (docs/rom_map.md's "master
  12-byte record array") - allocate via `sub_8008434` (or `sub_8011B0C`
  for `sub_8021F70`), point `+0x20` at `table_base + record*12`, tag
  `+0x2d`, build via the standard `sub_80087C0`/`sub_80087B4`/
  `sub_800872C` OAM trio, update the `+0x29` bitfield via
  `sub_800815C`, set `+0xa`, and register into `gUnknown_030012EC`'s
  manager via `sub_8008E94`. `sub_802200C` and `sub_8021F70` are gated
  (only spawn under a flag-bit/accessor test); `sub_80220C4` takes its
  record index, tag, and `+0xa` value as runtime parameters instead of
  fixed constants (matches `sub_8025BAC`'s already-documented
  `param1*12` runtime-indexed access to the same array).
- **`sub_802209C`**: a plain state-write slot - packs two args into a
  stack `{x, y}` pair and calls `sub_8023500` (already matched in
  `game_loop10.c`), storing them into `gUnknown_030012C0->0x1c0`/
  `->0x1c4`.
- **`sub_8022158`**: conditionally calls `sub_801173C` (the
  achievement/unlock-icon family spawner) when `gUnknown_030012C0+0x8c`
  is clear.
- **`nullsub_22`/`nullsub_23`**: empty stubs, the same "shared no-op
  fallback" convention already documented for the 42-slot action table
  and this same 92-slot array.
- **`sub_802218C`/`sub_80221BC`**: plain tail-call trampolines to
  `sub_801E990` - still raw, at the top of this same `asm/*.s` file,
  out of this chunk's scope.
- **`sub_80221A4`/`sub_80221D4`**: write a Q8.8 `{x, y}` position
  straight into `gUnknown_030012D8` (the hot camera/viewport struct) -
  leaf functions, no `push`/`pop` at all.
- **`sub_80221F0`/`sub_8022208`**: the `{table_base, count}` descriptor
  constructor/consumer pair docs/rom_map.md's "local vtable copy"
  investigation resolved as generic (nothing table-specific) - allocate
  an 8-byte object, zero it via `sub_8025D6C`, and hand it
  `{&gStaticData_0816C6A4, 0x5c}` via `sub_8025D4C`.
- **`sub_8022230`** (292 B, the "origin point" - docs/rom_map.md,
  "Found the origin point"): the function `sub_8023738` calls once at
  the top of the game loop to construct essentially every hot IWRAM
  global this whole ROM region references - `gUnknown_030012BC` (an
  8340-byte `AudioContext` allocation, `sub_80016DC`+`sub_8001C2C`),
  `030012CC`/`D0`/`B8`/`DC`/`E0`/`03001300`/`FC`/`03001304`/`030012B4`/
  `C8`, clears `gUnknown_03001288`'s mode byte, and zeroes `self+0xc0`
  before returning `self` unchanged. `gUnknown_030012D0` gets pointed
  at a freshly-allocated 4-byte pointer cell which itself is set to
  `&gStaticData_084A5600` (the 729 KB master asset index).

## Matched (24 functions, full clean `make compare` passing)

`src/graphics/graphics_loading_21bfc.c` (`sub_8021BFC`-`sub_8021CE0`, 6
fns): the `sub_800FF0C` trampoline family, types `1`-`7`.

`src/graphics/graphics_loading_21d80.c` (`sub_8021D80`-`sub_8022230`,
18 fns): the `gStaticData_084A5600` spawner family, `sub_802209C`,
`sub_8022158`, both `nullsub`s, the `sub_801E990` trampolines, the
`gUnknown_030012D8` position writers, the descriptor pair, and
`sub_8022230` itself.

### Gotchas worth recording

- **Returning a value vs. discarding it changes which register the
  epilogue pops the return address into.** Every function in this
  chunk that genuinely returns a value to its caller ends with
  `pop {..., r1}; bx r1` (the return-address pop targets `r1` since
  `r0` still holds the live return value) - but *most* of this chunk's
  functions are called only for their side effects and the ROM's
  epilogue instead does `pop {..., r0}; bx r0`, clobbering whatever was
  in `r0` (the constructed object's address, in every case) and
  discarding it entirely. Declaring these `void` (not `void *`) is
  what makes gcc choose `r0` for the epilogue pop, matching the ROM -
  the opposite choice (a `void *` return with the value silently
  unused) makes gcc reach for `r1` instead to protect the "live"
  return value, a 2-byte-per-function mismatch that's easy to miss on
  a quick visual diff since both forms *look* like `pop {reg}; bx reg`.
  Only `sub_80220C4` and `sub_8022230` actually return their value (both
  have real callers that use the result) and keep `void *`/`void *`
  return types with an explicit `return`.
- **The `sub_800FF0C` trampolines' cross-jump merge.** A naive
  `if (cond) return f(...,A); return f(...,B);` (or the `void`
  equivalent, `if (cond) { f(...,A); return; } f(...,B);`) gets
  cross-jump-merged by this compiler into one shared call site with the
  constant hoisted before the branch - the ROM instead has two fully
  duplicated call sites (`sub_8021BFC`'s two `bl sub_800FF0C`s, one per
  branch). Assigning each branch's result to a `void *result;` local
  (even though the value is never read afterward) is what defeats the
  merge - the same "assign-then-fall-through" shape used elsewhere in
  this project to force duplication instead of sharing.
- **`sub_800FF0C`'s own first parameter must be declared `u16`, not
  `u32`, in the caller-side prototype**, even though the *callee*
  function truncates it again internally - the ROM's callers defer
  `arg0`'s truncation to inside each branch (`u32 arg0` on the caller's
  own parameter, deferred to point of use), but the call *itself* still
  truncates before passing, which only happens if `sub_800FF0C`'s
  extern prototype types that parameter `u16`.
- **The negative-mask idiom, again**: `sub_8021D04`'s (parked) and
  every OAM-trio spawner's `+0x29` bitfield update need the explicit
  `asm("mov %0, #0x10\n\tneg %0, %0")` register-pin trick (see
  `UPDATE_ICON_FRAME_NIBBLE` in `settings_menu6.c`) - a bare `& -0x10`
  in C gets constant-folded into a single-instruction bitwise-complement
  immediate, one off from the ROM's actual two's-complement value.
- **`sub_8022230`'s five "void helper leaves the pointer in r0" calls**
  (`nullsub_2`, `nullsub_1`, `sub_8006FB4`, `sub_80007DC`,
  `sub_8025A5C`, and `sub_8022208`'s own `sub_8025D6C`): each is called
  immediately after an allocation, and the ROM leaves the fresh
  pointer in `r0` across the call (valid only because each real callee
  never writes r0) instead of reloading/saving it - reproduced with the
  pointer pinned to `r0` across an inline-asm `bl`, the same technique
  `sub_8023674`'s `nullsub_7` call already established (see
  `docs/matching/issue-37-game-loop-234e8.md`).
- **`gUnknown_030012D0`'s triple pointer-to-pointer-to-pointer
  dereference**: declaring it `void ***gUnknown_030012D0;` (matching
  `settings_menu6.c`'s already-confirmed-matching `sub_8005A78`) and
  writing `**gUnknown_030012D0` reproduces the ROM's exact 4-load
  chain (address load, then three register-indirect dereferences) in
  one expression, cleaner than the two-step `void *`-typed alias used
  in the still-parked `trigger_effect.c`.
- **Pre-computing a global's address into a local *before* a call it's
  used after** (`sub_8022230`'s dozen `{addr = &global; ...; *addr =
  result;}` blocks): writing the assignment as `global = f(...);`
  directly lets gcc defer the address computation to right before the
  store (after the call), while the ROM computes it once, early, and
  keeps it live in a callee-saved register across the call - matched
  by declaring the address as its own local *before* the call
  expression that uses it.
- **A single-use two-operand computation's operand-evaluation order is
  a genuine source-order dependency, not just an expression-tree
  question**: `sub_8022230`'s tail (`gUnknown_03001288` halfword store,
  then the `self+0xc0` word store, both writing the same zero constant)
  needed the *address* local declared before the *constant* local (not
  the reverse) to match the ROM's `ldr r0,=addr` / `movs r4,#0` order -
  swapping the declaration order alone changed which one gcc computed
  first, with no other source change needed.

## Parked (`NON_MATCHING`) - 1 function

- **`sub_8021D04`** (`src/graphics/graphics_loading_21bfc.c`, real bytes
  in `asm/code_3_2_17_21d04.s`) - every field/mask/branch is confirmed
  correct and the bit-test/mask-write tail matches the ROM
  byte-for-byte, but the middle "resolve the per-record flags byte"
  section doesn't: the ROM keeps the record's base object
  (`S = *(void **)gUnknown_030012B4`) live in `r1` across both of its
  field reads and only copies the final resolved address into a third
  register (`adds r3, r0, #0`) right at the end, whereas this compiler
  resolves the same value one register (and 4 bytes) short no matter
  how the reads/locals are ordered. A 4-byte gap in an otherwise fully
  understood 120-byte function - parked rather than keep fighting gcc's
  CSE for it. Splitting this function out of the otherwise-contiguous
  `sub_8021D80`+ block required a second `.c` file
  (`graphics_loading_21d80.c`) plus the small raw
  `asm/code_3_2_17_21d04.s`, following the established "matched
  functions on both sides of a parked one need to live in different
  translation units" pattern.

## Second pass: `sub_8021D04` matched via NAKED transcription

Now byte-exact matched, confirmed by a full clean `make compare` ("La
suma coincide"). Every field, mask and branch was already confirmed
correct against the ROM; the residual 4-byte CSE gap documented above
never responded to further plain-C restructuring, so it was converted
to `NAKED` and its ROM disassembly transcribed instruction-for-
instruction - the same escape hatch this project already established
for `sub_8001CB8`/`sub_8001DB4` (`src/system/link_cable.c`, see
`docs/matching/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section).

**Pre-existing bug found and fixed along the way**: `asm/code_3_2_17_21d04.s`
was missing the `.if NON_MATCHING == 0` / `.endif` guard this project's
other parked functions' raw `.s` fragments use - it assembled
`sub_8021D04`'s real bytes unconditionally, regardless of the
`NON_MATCHING` flag. This didn't affect the normal `make compare` build
(which never compiles the `#if NON_MATCHING`-guarded C version and so
never conflicted with it), but would have produced a duplicate-symbol
link error the moment someone ran `make NON_MATCHING=1 report` with
this function still parked - never actually hit only because nobody had
run that combination against this exact function before it got matched
here. Moot now that the whole file is deleted and the function is real,
always-compiled `NAKED` C.

See [docs/status/graphics_loading.md](../status/graphics_loading.md)
for the running matched/parked list this updates.

## Third pass: `sub_8021D04` closed as real C

The 4-byte gap above (the ROM's `adds r3,r0,#0` copy at the very end
of the table-resolution chain, which a plain C statement always got
optimized away) closes with the same technique used elsewhere in this
project: an opaque `asm volatile("add %0, %1, #0" : "=r"(flagsAddr) :
"r"(tmp))` forces the copy as a real, un-eliminable instruction instead
of a redundant SSA value. Register-pinning the rest of the chain to the
ROM's own choices (`rec` in r1, the array-base/loaded-value pair
reusing r0/r4 in the ROM's own load order, splitting the `+0xc` base
load into its own block so it lands between the address computation and
the `ldrh` exactly where the ROM has it) and reordering both bitfield
checks' constant-vs-reload evaluation order (pinning the mask constant
to r0 and the reloaded flags byte to r1/r3 respectively, matching which
one the ROM computes first) closed the remaining two 2-byte ordering
diffs. Now genuine matched C, not `NAKED` - `tools/report_units.py`'s
entry for `0x08021D04` now points at
`src/graphics/graphics_loading_21bfc.o` instead of `base_object=None`.
Full clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide`.

This doesn't yet extend to `sub_801E990`'s own copy of this same
`gUnknown_030012B4 -> *rec -> {+8, +0xc}` resolution shape
(`src/graphics/graphics_loading_1e990.c`, issue #30) - that function's
residual is a different register-choice/mask-derivation gap (`byte` in
r0, a `movs r0,#1`/`subs r0,#0x12` mask derivation rather than a
negative-immediate one), not the copy-elimination gap closed here -
left for a future pass.

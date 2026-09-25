# Issue #9/#10: `sub_800AFF4` (graphics) - the "dizzy stars" companion update

Dedicated deep-investigation session against `sub_800AFF4` (ROM
`0x0800AFF4`, 636 bytes), the second function in the small two-function
region `0x0800AC2C`-`0x0800B270` `tools/report_units.py` tracked as
parked (`base_object=None`, still raw). Its neighbor, `sub_800AC2C` (a
38-case player action-state jump-table dispatcher directly above it),
is out of scope and untouched - same standing exclusion this project
already applies to `sub_8018008` (issue #22): a large jump-table
dispatcher calling a dozen still-unexamined state-transition helpers,
left raw rather than guessed at.

## Starting point

`docs/rom_map.md`'s "eight more core reads" passage had already sampled
this function once, from one angle only: "`sub_800AFF4` (636 B) reaches
[the 28-byte-record table] through a *child* object's `+0x20` field and
reads a third offset, byte `+0x16` this time, clamping the result into
`self+0x30`" (that prose actually meant the *child's own* `+0x30`, not
`self`'s - `self` here and the "child" are two distinct objects, as the
raw bytes below confirm). `docs/matching/issue-9-10-0x0800ab9c-graphics.md`
had also already flagged the function's risk shape from the outside:
"high register-pressure (`sb`/`sl`/`r8`/`ip` all live simultaneously)
hitbox-record lookup/commit logic gated on `gUnknown_030012C0+0x78`
state values and an unconfirmed per-state table." Neither prior pass
read the function start-to-finish; this session does.

## Reading the raw bytes

The full disassembly (formerly the tail of `asm/code_3_2_16_ac2c.s`,
`0x0800AFF4`-`0x0800B270`) has **no loops at all** - every branch is a
forward branch, so the function is a purely linear sequence of gated
blocks. `self` (`r0`/`r7`) is the same wide, still-unnamed "big object"
struct (0x108+ bytes) referenced by raw offset throughout this ROM
neighborhood - the same fields `actor_part16.c`/`actor_part79.c`/
`actor_part108.c` already establish:

- `self+0xc` - flags byte (bit 3 cleared at the very end).
- `self+0x18` - a per-category `{s16 offset; void *fn}` trampoline
  table pointer, the `sub_803AD7C` convention `actor_part108.c`
  documents (`self + *(s16*)(table+N), *(void**)(table+N+4)`).
- `self+0x20` - a per-tag 28-byte-record table pointer:
  `*(self+0x20) + tag*0x1c`, the exact convention `actor_part79.c`
  documents from a sibling call site (there written
  `sub_8012160`'s `xptr`/`base`/`record` chain).
- `self+0x28` bit 4 - the mirror-flag bit `actor_part16.c`/
  `actor_part17.c`/`actor_part108.c` already read (tested via
  `lsls rX, rY, #0x1b` / `bge`, the same "shift bit into the sign
  position" idiom used everywhere else this bit is read).
- `self+0x2d` - a per-tag selector byte (indexes the `+0x20` table).
- `self+0x8c` - a `gUnknown_0300082C`-relative deadline, the exact
  `IsTimerArmed`/`SetTimer` convention `actor_part16.c` names:
  `*(u32 *)(self+0x8c) > gUnknown_0300082C` means "still armed",
  confirmed here by the identical comparison shape appearing twice.
- `self+0xb0` - a pointer to a single **"child" companion object**
  (the same object dereferenced by every block in this function).
- `self+0xb4` - a write index into an **8-slot circular buffer** of
  `self`'s own recent `{s32 x, s32 y}` Q8 positions at `self+0xb8`
  (8-byte stride, confirmed by the `str [base + index*8]` /
  `str [base + index*8 + 4]` pair and the `(index+1) & 7` wraparound
  computed via the ROM's signed-division-by-8 idiom
  `asr #3; lsl #3; sub` immediately after).
- `self+0x38` - a one-shot flag, tested only at the very end.

Two file-scope globals not previously declared anywhere in `src/`:
`gUnknown_03000818`/`gUnknown_0300081C` (both plain `u32`, alongside
the already-`extern`'d `gUnknown_0300082C` frame counter and
`gUnknown_030012CC`, a plain `void *` OAM-manager-style global several
other files already pass to `sub_8007A84`).

### Block by block

The whole function is gated by `gUnknown_030012C0+0x78` (the central
game-state "mode" field several other functions in this ROM region
gate on - `actor_part13.c`, `actor_part16.c`, and others already
established this exact `*(void**)gUnknown_030012C0 + 0x78` dereference
chain):

1. **`mode == 3`** ("just got hit" / stun-entry) - runs a whole block
   skipped entirely for any other mode:
   - Every ~8 frames (`gUnknown_0300082C & 7 == 0`), re-rolls
     `gUnknown_03000818 = (u16)sub_8000E1C(2) + 2 - (mirrored ? 2 : 0)`
     - `0`/`1` if `self` is mirrored, `2`/`3` otherwise: which side the
       effect "starts" from, tied to facing.
   - Clamps `gUnknown_03000818` against the **child's own** hitbox/
     variant record's `+0x16` byte (`child[0x20] -> *table + child[0x2d]*0x1c`,
     read `[+0x16]` - the same table-dereference chain
     `sub_800AAEC`/`sub_800CD00` and `actor_part79.c`'s `sub_8012160`
     already established, just with a different single-byte field read
     out of the 28-byte record than either of those): `val = min(roll,
     limit) if roll < limit else limit - 1`, stored into the child's
     own `+0x30`.
   - Repositions the child directly next to `self`'s own *current*
     position: `child.x = self.x +/- 0x600` (Q8 `6.0`, sign chosen by
     the mirror-flag bit), `child.y = self.y - 0x1300` (Q8 `19.0`,
     unconditional) - a fixed offset near the head, mirrored
     horizontally with facing.
   - Toggles the child's own `+0x2d` tag between `1`/`2` on a 4-frame
     parity of `gUnknown_0300082C` (`&4`) - a flicker - then fires the
     child's own `+0x18`-table `+0x20`/`+0x24` trampoline via
     `sub_803AD7C` (a "refresh/notify" call, same convention
     `sub_800AAEC` uses at its own `+0x18`-table `+0x48` slot - a
     *different* vtable slot here, `+0x20`/`+0x24`).
2. **Unconditionally** (any mode, using `self`'s own `self+0x8c`
   deadline): draws `self` itself via
   `sub_8007A84(gUnknown_030012CC, self)` (matched, `actor_part.c` -
   queues `self`'s own OAM using its own Q8 position, truncated to
   int) - unconditionally if `mode == 3` **or** the deadline has
   expired (`self+0x8c <= gUnknown_0300082C`); while the deadline is
   still armed and `mode != 3`, only on the same 4-frame parity
   (`gUnknown_0300082C & 4`). This is a standard hit-invincibility
   blink, reusing the identical "armed deadline + 4-frame parity"
   shape as the child's own flicker in block 1 - both driven off the
   same frame counter, so the child and `self` flicker in lockstep.
3. **`mode == 3` again**, but only once the `self+0x8c` deadline has
   *expired* (`self+0x8c <= gUnknown_0300082C`, the "not armed"
   case): calls `sub_80231EC(gUnknown_030012C0, 2)` - a mode-transition
   call, the same "state close" convention `actor_part84.c`/
   `actor_part58.c` already establish for this function acting on
   `gUnknown_030012C0`. Reads as: once the blink/stun period is over,
   transition the central game mode from `3` back to `2`.
4. **Unconditionally** (any mode): pushes `self`'s own current
   `{x, y}` into the 8-slot `self+0xb8` position-history ring buffer
   at the `self+0xb4` write index, then advances the index mod 8.
5. **`mode == 1` or `mode == 2`** only (skipped for every other value,
   including `3` - blocks 1 and 5 are mutually exclusive in practice,
   since only one mode value is active at a time, but structurally
   independent gates):
   - Every ~8 frames (`gUnknown_0300082C & 7 == 0`), random-walks
     `gUnknown_0300081C += sub_8000E1C(3) - 1` (so `-1`/`0`/`+1`),
     clamped to `[0, 3]`.
   - Clamps `gUnknown_0300081C` against the **same child record's**
     `+0x16` byte (identical clamp-and-store idiom as block 1, just a
     different source counter) and stores the result into the child's
     own `+0x30`.
   - Reads the *oldest surviving* entry of the position-history ring
     buffer - specifically `self+0xb8[nextWriteIndex]`, the slot about
     to be overwritten *next* frame, i.e. `self`'s own position from
     (up to) 8 frames ago - and adds a rotating offset built from the
     shared 256-entry sine-ish table `gStaticData_0816A820` (already
     `extern s16 gStaticData_0816A820[];`-declared and confirmed real
     in `actor_part72.c`, itself tied by `docs/rom_map.md` to the
     minimap and an "orbiting-companion actor"):
     `child.x = oldX + (table[frame & 0xff] << 4)`,
     `child.y = oldY + (table[(frame >> 1) & 0xff] << 3) - 0x1800`
     (Q8 `-24.0`) - two different angular speeds (full-speed X,
     half-speed Y) around a point 24px above the trailed position: a
     classic elliptical "orbiting" motion.
   - Sets the child's own `+0x2d` tag to `mode - 1` (`0` for mode `1`,
     `1` for mode `2`) and fires the same `+0x18`-table `+0x20`/`+0x24`
     trampoline refresh.
6. **Finally** (any mode): if `self+0x38` is nonzero, clears bit 3
   (`0x08`) of `self+0xc` - a one-shot "(re)armed" flag consumed once.

### Reading it all together

This is the per-frame update for a **"stars orbiting a dizzy head"
companion effect** attached to `self` (almost certainly the player,
given `gUnknown_030012C0+0x78`'s "mode" values `1`/`2`/`3` read as an
idle/orbit-active/just-stunned state progression, and given
`actor_part.c`'s `sub_8007A84` - already established as an OAM-queue
call, not a hitbox commit): while `mode == 3` (just took a hit), the
single child effect object snaps to a fixed spot near `self`'s head and
both `self` and the child flicker together on the same 4-frame parity
while a blink-deadline (`self+0x8c`) counts down; once that deadline
expires, the game transitions back to `mode == 2`. While `mode` is `1`
or `2` (the ordinary "stars still circling" idle state), the same child
object instead orbits smoothly around `self`'s own recent (8-frame-
delayed) trail position using two different-speed sine-table lookups -
the textbook Lissajous-style "stars circling a stunned character's
head" visual effect used throughout this genre of platformer.

This reconciles and corrects `docs/rom_map.md`'s own earlier partial
read: the `+0x16` byte clamp it flagged is real and confirmed, but it's
not "the" function's purpose - it's a small shared sub-step (used
*twice*, once per mode-branch, with two different source counters) of
a much larger per-frame companion-object animator.

## Matching

**NAKED transcription (not real C).** This is exactly the register-
pressure shape this ROM neighborhood's risk flag named up front:
`sb`/`sl`/`r8`/`ip` are all simultaneously live across the ring-buffer
index/base-pointer bookkeeping and the two trig-table lookups (`sb`/
`sl` hold the history-array X/Y base pointers, `r8` holds the saved
mode value, `ip` holds the saved `&self[0xb0]` child-pointer address) -
the same class of shape this project has already proven resistant to
gcc 2.9 C reconstruction on multiple sibling functions this session
(`sub_800CD00`, `sub_800A178`/`sub_800A420`,
`sub_8026AE8`/`sub_8026A18`). Per this session's own scoping ("don't
over-invest fighting this if an early isolated-compile attempt shows a
structurally different register allocation"), no C reconstruction
attempt was made - the function was transcribed directly, instruction-
for-instruction, as byte-exact NAKED asm instead, translating the
ROM's disassembly to this project's established NAKED-transcription
conventions:

- GNU-as local numeric labels (`1:`/`1f`/`1b` etc.) in place of the
  ROM's own `_0800Bxxxx`-style absolute-address labels - straightforward
  here since the function has no loops at all, so *every* label
  reference in the whole function is a forward reference (`Nf`); there
  is no backward branch anywhere in this function.
- Suffix-less Thumb mnemonics (`add`/`sub`/`and`/`lsl`/`lsr`/`asr`/`mov`
  in place of the ROM disassembly's `adds`/`subs`/`ands`/`lsls`/`lsrs`/
  `asrs`/`movs`) - required by `arm-none-eabi-as` in this project's
  default (non-`.syntax unified`) assembly mode for agbcc-compiled
  output; the ROM disassembly's own suffixed forms are purely an
  objdump-display convention for the flag-setting encoding, not a
  distinct mnemonic. One additional gotcha beyond the generic
  suffix-strip: the ROM's `rsbs r0, r0, #0` (reverse-subtract-with-
  immediate-zero) needs the dedicated `neg r0, r0` pseudo-mnemonic in
  this mode, not a bare `rsb r0, r0, #0` (Thumb1 has no general
  register-minus-immediate `rsb` encoding beyond the `#0` case, which
  assembles as `NEG`).
- Literal pools manually placed at the exact same relative positions
  the ROM's own compiler chose (mid-function, right after the nearest
  preceding unconditional branch - three separate pools total, one
  small one after the mirror-flag positioning block, one smaller one
  after the child-tag flicker block, and the large final pool at the
  very end), each `.4byte` entry given its own local numeric label so
  every `ldr rX, N` reproduces the ROM's own choice of *which* literal
  pool slot to reload from at each of its (sometimes repeated) use
  sites - `gUnknown_030012C0`, `gUnknown_0300082C`, and
  `gUnknown_03000818`/`gUnknown_0300081C` are each backed by two (or
  three, for `gUnknown_030012C0`) *separate* pool entries at different
  addresses rather than one shared literal, exactly matching the ROM's
  own conservative-reload pattern (the same "reload from the literal
  pool fresh every use" convention `sub_800AAEC`'s own doc comment
  already names for `gUnknown_0300130C`).

Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
`0x0800AFF4`-`0x0800B270` (636 bytes): the standalone-compiled output
is exactly 636 bytes, and every differing byte (49 of 636) falls
inside one of the function's `bl` call-site half-word pairs (6 calls:
`sub_8000E1C` x2, `sub_803AD7C` x2, `sub_8007A84` x1, `sub_80231EC`
x1) or a symbol-relocated `.4byte` literal-pool entry (8 of the 11
pool entries reference RAM-address symbols; the remaining 3 are plain
numeric constants - `0xFFFFFA00`/`0xFFFFED00`/`0xFFFFE800` - and those
already matched byte-for-byte with no linking at all, confirming the
constants and every non-relocated instruction are correct as written).
These differences resolve to zero once linked, per this project's
established verification convention.

## Full-ROM verification

`rm -rf build && make NON_MATCHING=1 report` - clean, no warnings.
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

## Files changed

- **New**: `src/graphics/actor_part111.c` - `sub_800AFF4`, NAKED.
- `asm/code_3_2_16_ac2c.s` - trimmed to end right after `sub_800AC2C`
  (`sub_800AFF4`'s real bytes, and its own trailing literal pool,
  removed).
- `ldscript.txt` - new `actor_part111.o(.text)` entry inserted between
  `code_3_2_16_ac2c.o` and `actor_part49.o`, preserving link order.
- `tools/report_units.py` - the combined `0x0800AC2C` (`None`) entry
  split: `0x0800AC2C` narrowed to just `sub_800AC2C` (still raw), new
  `0x0800AFF4` entry pointing at `actor_part111.o`.
- `docs/status/actor.md` - the old combined `sub_800AC2C`/`sub_800AFF4`
  "Left raw" bullet narrowed to just `sub_800AC2C`; new `sub_800AFF4`
  bullet added to the "Parked - NAKED transcription" section.

## Cross-references

- `docs/rom_map.md` - "eight more core reads" (the original partial
  `+0x16`-byte-clamp flag) and the `gStaticData_0816A820`/
  `gUnknown_030012C0+0x78` mentions this session reconciled against.
- `docs/matching/issue-9-10-0x0800aaec-graphics.md` - the
  `self+0x20`/`+0x2d`/28-byte-record convention worked out in detail
  there, reused here for the child object's own record lookup.
- `src/graphics/actor_part79.c` - `sub_8012160`'s own
  `part+0x20 -> *ptr + tag*0x1c` chain, the closest existing sibling of
  this function's own child-record clamp.
- `src/graphics/actor_part16.c` - the `self+0x8c`
  `IsTimerArmed`/`SetTimer` convention and the mirror-flag-bit
  convention, both reused here.
- `src/graphics/actor_part72.c` - `gStaticData_0816A820`'s own
  `extern s16 [];` declaration and confirmed 256-entry sine-table
  shape.
- `src/graphics/actor_part.c` - `sub_8007A84`'s own matched definition
  (confirms it's an OAM-queue/draw call, not a hitbox operation).

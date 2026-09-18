# Issue #45, second pass: the HUD stat-widget dispatcher

GitHub issue #45's first pass (see `docs/matching.md`'s frozen
`0x08026EEC`-`0x08028568` entry, since this issue predates the
`docs/matching/` restructure) matched 14 of the chunk's 24 functions and
left 10 untouched: the fx ring-buffer pair (`sub_8026F54`/`sub_8027018`),
the 34/35-slot OAM array setup pair (`sub_8027138`/`sub_802732C`), the
HUD stat-widget dispatcher and its callees (`sub_80274EC`/`sub_802757C`/
`sub_802763C`), and the rest of the digit-counter family
(`sub_8027940`/`sub_8027D5C`/`sub_8027E88`).

This pass picks up that remainder. Net result: **1 more function matched
(`sub_80274EC`), 9 still untouched** - the issue stays open.

## Matched: `sub_80274EC` (new `src/graphics/hud_stat_widget.c`)

The dispatcher itself. Takes the same `struct hud_counter *self` that
`sub_8027838` (already matched, `hud_counter.c`) receives - confirmed by
the dispatcher calling `sub_8027838(self)` directly with its own
argument, and every field it touches (`mode`, `field_08` (new),
`icon_flag` (new), `parts`) already fitting inside `struct hud_counter`'s
existing 0x68-byte layout (`include/hud.h`) rather than needing a bigger
struct.

Control flow, reading the disassembly directly rather than trusting
`docs/rom_map.md`'s earlier summary (which described `sub_8027940`/
`sub_8027D5C` as "unconditionally" called - true for the *other*
branch, but the `sub_802757C` branch actually returns immediately and
skips them entirely):

```c
void sub_80274EC(struct hud_counter *self)
{
    gUnknown_0300086C = 0;
    if (self->icon_flag) sub_8027E88(self);
    sub_8027838(self);

    if (sub_80233B4(gUnknown_030012C0) != -1) {
        sub_802757C(self);
        return;                       /* early return - the rest is skipped */
    }
    if (sub_80232B8(gUnknown_030012C0)) {
        gUnknown_0300086C = 0;
        sub_8008044(&self->parts[34]);        /* recomputed fresh both times, not cached */
        sub_80270E0(&self->parts[34], 0, 0);
    }
    if (*((u8 *)gUnknown_030012C0 + 0x8c) && self->mode == 0 && self->field_08 == 0)
        sub_802763C(self);
    sub_8027940(self);
    sub_8027D5C(self);
}
```

Compiled byte-identical on the first structurally-correct attempt once
one register-pin quirk was worked out (see below) - no `NON_MATCHING`
needed.

### Codegen gotcha: pinning `r7` never emits its push/pop in this compiler

Plain C (no pins beyond `self`) put `self` in `r5` and
`&gUnknown_030012C0` in `r6` correctly on its own, but spilled
`&gUnknown_0300086C` into `r8` (an extra push/pop pair) instead of
keeping it in `r7` the way the ROM does - this compiler's allocator
prefers a fresh caller-saved-adjacent `r8` slot over reusing `r7` once
`r4`-`r6` are otherwise occupied by live-across-call locals.

The natural first fix - `register s32 *layout_addr asm("r7") = ...` -
compiles *without* emitting `r7` in the function's `push`/`pop` list at
all (a known bug in this `gcc 2.9` build when a `register ... asm("r7")`
local is live across a `bl`: see `src/graphics/actor_part35.c`'s comment
on the identical bug, and `src/system/game_loop3.c`'s note that even
un-pinning both sides of a swapped `r7`/`r3` pair doesn't help there
either). Shipping that version would silently corrupt the caller's `r7`.

Fix: leave `&gUnknown_0300086C` and the `0` it's compared/stored against
as **plain, unpinned locals** (`s32 *layout_addr = &gUnknown_0300086C;`,
no `register`), and only pin `self` to `r5`. With `self` and
`&gUnknown_030012C0` (unpinned, but naturally chosen as `r6`) already
occupying two of the four low callee-saved slots, the allocator's next
choice for the third and fourth live-across-call values is genuinely
`r7`/`r4` (with `r4` reused for the offset constant `0x880` later in the
function, exactly like the ROM) rather than overflowing to `r8`. No
pin at all - beyond `self` - was the correct move here, the opposite of
the instinct to pin harder when a register mismatches.

### `include/hud.h` field additions

Extended `struct hud_counter` (previously only characterized up to what
`sub_8027838` touches) with four fields inside what were opaque padding
ranges, all confirmed by this function alone:

- `field_08` (`+0x08`) - read by the dispatcher, compared against 0
  alongside `mode` to gate `sub_802763C`; meaning not established yet.
- `icon_flag` (`+0x18`) - gates the `sub_8027E88` (percentage counter)
  call; also the field `sub_802732C` (still raw) writes from its own
  second argument while building this same object.
- `sync_value_a`/`sync_value_b`/`sync_value_c` (`+0x2c`/`+0x30`/`+0x34`)
  - **not** touched by `sub_80274EC` itself, but read while confirming
  the struct's shape doesn't collide with `sub_802763C`'s (still raw)
  three change-detection caches at the same offsets - named now so a
  future pass matching that function doesn't have to re-derive them.

`sizeof(struct hud_counter) == 0x68` still holds; nothing here changes
the struct's size, only which of its already-allocated padding bytes
have real names now.

## Still untouched - all 9 remaining functions

Read enough of each to have working confidence in general shape (mostly
already documented in `docs/rom_map.md`'s "hud"/"fx" investigation
sections), but none were matched or parked this pass:

- **`sub_8026F54`/`sub_8027018`** (fx ring-buffer pair) - unchanged from
  the first pass; still genuinely not at byte-precision confidence.
- **`sub_8027138`/`sub_802732C`** (34/35-slot OAM array setup pair) -
  unchanged; heavy `sl`/`sb`/`r8` register usage throughout, on top of
  needing the byte-index-cache table below.
- **`sub_802757C`/`sub_802763C`/`sub_8027940`/`sub_8027D5C`/
  `sub_8027E88`** - the rest of the digit-counter/icon-indicator family.
  Reading `sub_802757C` while confirming `sync_value_a`/`b`/`c` above
  surfaced why these are a much bigger job than `sub_80274EC`: they read
  and write a **large per-instance byte-indexed cache table** living
  well past `struct hud_counter`'s currently-documented 0x68 bytes (byte
  fields at offsets like `+0x36D`, `+0x369`, `+0x3AD`, `+0x3ED`,
  `+0x42D`, `+0x4ED`, `+0x56D`, `+0x5AD`, `+0x5ED`, `+0x62D`, `+0x66D`,
  `+0x6AD`, `+0x6ED`, `+0x72D`, `+0x76D`, `+0x7AD`, `+0x7ED`, `+0x82D`,
  `+0x86D` all show up across this family and `sub_8027138`/
  `sub_802732C`) - one "last selected glyph index" byte per drawn
  digit/icon slot, clamped against a per-glyph-set max read through
  `self->parts[i].anim_data`. Naming that whole table (and confirming
  `struct hud_counter`'s *real* total size, which is clearly much larger
  than the 0x68 bytes `sub_8027838` alone needed) is prerequisite work
  for matching any of these five to the register-pin precision
  `sub_8027838` needed - out of scope for what fits in this pass. Left
  exactly as the first pass found them: raw `.s`, real bytes in
  `asm/code_3_2_17_2757c.s` (`sub_802757C`/`sub_802763C`) and
  `asm/code_3_2_20.s` (`sub_8027940`/`sub_8027D5C`/`sub_8027E88`).

## File structure

`asm/code_3_2_17_27138.s` is now truncated to just `sub_8027138`/
`sub_802732C` (previously also held `sub_80274EC`-`sub_802763C`).
Followed by `hud_stat_widget.o` (`sub_80274EC`), then the new raw
`asm/code_3_2_17_2757c.s` (`sub_802757C`/`sub_802763C` - the removed
tail of the original file), then the existing `hud_counter.o`. See
`ldscript.txt` and `tools/report_units.py`'s `hud` category entries,
both updated to match. Verified via a full clean `make compare`
(`La suma coincide`).

## Cross-references

- `docs/status/hud.md` - matched list updated.
- `docs/rom_map.md`'s "hud"/"fx" investigation sections - background on
  what each still-raw function does, referenced above.
- `docs/matching.md`'s frozen `0x08026EEC`-`0x08028568` entry - the
  first pass on this same issue.

## Third pass

Picking up the remaining 9 functions issue #45's second pass left
untouched (see "Still untouched - all 9 remaining functions" above).

### Matched: `sub_8026F54`/`sub_8027018` (the fx ring-buffer pair)

Both now live in `src/graphics/hud_icon_slot.c`, placed at the top of
the file (ROM order puts them immediately before `sub_8027088`, which
this file already held). `struct hud_fx_queue` (previously a stub with
two named `s32[3]` arrays and unlabelled padding, in this same file)
is now fully characterized: `targets[3]`/`lists[3]` (pointer pairs,
`+0x10`/`+0x1c`), `periods[3]`/`counts[3]` (`+0x28`/`+0x34`), `count`
(`+0x40`), `direction` (`+0x44`), and a still-unexplained
write-only `fields_e[3]` at `+0x04` (`sub_8027018` zeroes it; nothing
in either function reads it back).

- **`sub_8026F54`** (consumer, called with just `self`): for each of
  `self->count` active slots, checks
  `sub_803AF1C(gUnknown_0300082C, periods[i]) == 0` (a "how many frames
  since this slot's period elapsed" test) and, when it fires, rotates
  `targets[i]` by one position along the permutation order
  `lists[i]` gives - forwards (`direction` set) or backwards
  (`direction` clear). This is a palette/sprite-index **cycling**
  effect, not the "trajectory queue" the second pass's doc comment
  speculated - no angle/trig value is ever read back as an angle, only
  used as a modulus.
- **`sub_8027018`** (producer): appends one new slot at `self->count`
  (no wraparound in either function - the caller resets the queue via
  `sub_8027088`/`sub_80270C0` between bursts), computing `periods[idx]`
  from `sub_803ADB4(0x3C, angle)`.

Both compiled byte-identical only after a long series of register-pin
and instruction-ordering fixes, all following the established
techniques from `docs/workflow.md` step 3/7 and the gotchas already
documented for this same function family:

- **Caching `i*4` across a `bl`**: `sub_8026F54`'s per-iteration offset
  needs to survive the `sub_803AF1C` call (used again afterward for
  `targets[i]`/`lists[i]`/`counts[i]`). Plain `self->arr[i]` struct
  access recomputes `i*4` fresh after the call every time (its
  register is caller-saved, clobbered by the callee) - the fix is the
  same `register s32 offset asm("r5") = i << 2;` pattern already used
  elsewhere in this project, then reusing that value via raw
  `(u8 *)self + FIELD + offset` pointer arithmetic instead of array
  indexing for every access this iteration.
- **The ROM never shares a computed base pointer between adjacent field
  accesses.** A single combined C expression like
  `*(TYPE *)((u8 *)self + FIELD + offset)` written twice back-to-back
  gets CSE'd by this compiler into one shared `self+offset` base with
  per-field immediate-offset loads - the ROM instead redoes the full
  three-instruction address build (`self`, `+FIELD`, `+offset`) fresh
  for each field. Fix: build the pointer through **separate sequential
  statements** (`p = (u8 *)self; p += FIELD; p += offset;`) inside its
  own small block per field, which this compiler does not fold back
  together.
- **`i`/`next_i` via `ip`, mirroring the ROM's odd early-increment**:
  the ROM computes `i+1` and stashes it in `ip` immediately after the
  `sub_803AF1C` call (before even checking the call's result), freeing
  r4 (still holding the *old* `i`, safe since r4 is callee-saved across
  the call) for `target` to occupy for the rest of the iteration. A
  plain `for (i = 0; i < count; i++)` loop's natural increment placement
  doesn't reproduce this. Fix: `register s32 next_i asm("ip");`
  computed explicitly right after the call, used as the `for` loop's
  own increment expression (`for (; i < count; i = next_i)`) so both
  the normal and `continue`d paths funnel through the same single
  bottom-of-loop test - a genuine `do { } while` written directly (or
  `goto`-based equivalents) instead produced a *duplicated* top-of-loop
  test that doesn't match; only routing through the `for` clause itself
  got the ROM's single rotated test.
- **`i = 0;` before `count = self->count;`, not after**: independent
  statements with no data dependency between them still get emitted in
  their C source order by this compiler - swapping the two changed
  which one came first in the generated code, matching the ROM's
  `movs r4, #0` preceding its `ldr r0, [r7, #0x40]`.
- **Deferred global dereference**: `sub_803AF1C(gUnknown_0300082C, *p)`
  written as a plain `u32 global_val = gUnknown_0300082C;` local
  dereferences the global immediately after taking its address. The
  ROM takes the address first, does unrelated work (the offset/pointer
  build for the second argument), and only dereferences it right before
  the call. Fix: `u32 *global_addr = &gUnknown_0300082C;` (address
  only) followed by `*global_addr` used directly in the call expression,
  which this compiler schedules where the value is actually needed.
- **Index-read vs. shifted-address in different registers**: the ROM's
  `ldrh r1, [r3]` / `lsls r0, r1, #1` idiom (used throughout this
  family for `array[index]`-style accesses) keeps the raw index and its
  shifted byte offset in two different registers. A plain
  `target[*list]`-shaped C expression collapses both into one register
  once the index is dead - this compiler's own CSE, not something a
  register pin alone fixes (pinning just the pointer variable itself
  made this *worse*, forcing the same address to be recomputed twice
  for a read-then-write). What actually worked: split the shift out
  into its own `asm volatile("lsl %0, %1, #1" : "=r"(shifted) : "r"(idx))`
  anchor, with an explicit intermediate `idx_val` local for the raw
  index - reproduces the exact two-register shape everywhere it
  appears, including in `sub_8027018`'s `list += n` (needing the
  operand order forced too, via the established `add %0, %1, %0` idiom
  from `hud_counter.c`, this time keeping the pointer being updated as
  the *output* operand rather than the shift result).
- **`n`'s register differs by branch - don't share one pin across
  both.** `sub_8026F54`'s `if (self->direction)` branch keeps
  `counts[i]` in r1 the whole time (compared against 0, then copied to
  a separate r2 loop counter); the `else` branch never touches r1 for
  it at all - it's loaded straight into a scratch register and
  decremented directly into r2 in one `subs`. A single
  `register s32 n asm("r1")` declared once above the `if` and shared by
  both branches forced the `else` branch into a register it never
  actually asks for, and lost the ROM's fused
  `n = *(s32 *)p - 1;` single-`subs` shape besides. Fix: two separate
  local declarations, one per branch, each following its own branch's
  actual register need instead of a single "the value called `n`" pin.
- **A `u8` 6th-argument (stack-passed) parameter**: same gcc-2.9 gap
  documented in `docs/matching/issue-3-overlay-ui-audio-wrapper.md`'s
  `sub_80019F8` entry - plain `u8` parameter access reads the full
  stack word and narrows it with two shifts instead of a genuine
  `ldrb`. Here the ROM additionally keeps the address computation and
  the final byte value in *two different* registers (`add r0, sp,
  #0x18` / `ldrb r7, [r0]`), unlike that precedent's single-register
  case - fixed with a two-output inline-asm anchor
  (`"add %1, sp, #0x18\n\tldrb %0, [%1]" : "=r"(direction),
  "=r"(addr_scratch)`), the second output pinned to `r0` so it doesn't
  drift onto `r8` (which would need an extra push/pop this function
  doesn't have) picked freely by the allocator.
- **Never pin `self` (or anything else) to `r7` when it's live across a
  `bl`.** Confirmed yet again (`docs/matching/
  issue-45-hud-stat-widget-dispatcher.md`'s own second-pass note, and
  `src/graphics/actor_part35.c`): an explicit `register ... asm("r7")`
  parameter live across a call compiles *without* emitting r7's
  push/pop at all, silently corrupting the caller's r7. `self` reaches
  r7 here purely through natural allocation, arrived at only once every
  other competing pin was in place and ordered correctly - never
  through a direct pin.

Verified via a full clean `make compare` (`La suma coincide`) with both
functions cut into `src/graphics/hud_icon_slot.c` and their
`asm/code_3_2_17_26f54.s` fragment (now empty) removed from
`ldscript.txt`.

### Parked: `sub_802757C`/`sub_802763C` (real gap, not a budget cut)

Both fully understood and reconstructed as C (`src/graphics/
hud_stat_widget2.c`, guarded by `#if NON_MATCHING`; real bytes stay in
`asm/code_3_2_17_2757c.s`, now itself split into two
`.if NON_MATCHING == 0` blocks, one per function) but not byte-matched:

- **`sub_802757C`** (icon-indicator widget - lives display): positions
  and clamps `parts[22]` unconditionally, then `parts[23]` only when
  `sub_8023378`'s count exceeds 1. Every register in this function
  matches ROM by construction *except one*: the ROM keeps each icon's
  `anim_index` byte (`self + 0x5AD`/`self + 0x5ED`) in **r7** right up
  until using it as the `records[]` array subscript. Pinning it there
  (`register u8 index asm("r7")`) reliably miscompiles - not just "the
  wrong register", a genuine wrong-*value* bug: the next statement that
  touches the pinned r7 value gets `mov r0, sp` / `lsl`/`lsr #0x18`
  (reading the *stack pointer itself*, shifted, as if it were the
  spilled byte) instead of the real value, silently corrupting the
  result. Reproduced identically across every phrasing tried: plain
  `records[index]`, an explicit `record_offset = index * sizeof(*records)`
  local, and the full manual shift-and-subtract-plus-asm-forced-add
  idiom `sub_8027838` needed for its own analogous clamp. Confirmed this
  isn't about crossing a `bl` (the documented r7 quirk everywhere else
  in this codebase) - there's no call between r7's definition and this
  use. This looks like a distinct, second r7 miscompilation class in
  this same `gcc 2.9` build, on top of the already-documented
  live-across-a-`bl` one. Every other register does match: `self`=r6,
  the position-table base=r5, the first icon's slot pointer=r3 and
  clamp value=r4, the second icon's clamp value=r3 and slot pointer=r4
  (each via a narrowly-scoped `register ... asm("rN")` local, not a
  shared one - sharing one pinned variable between the two icons'
  blocks, or between the outer clamp-value and the inner slot-pointer,
  reliably knocked `self` out of r6 instead, the same "pin one thing,
  something unrelated moves" churn `sub_8026F54` hit before its offset
  ended up correctly cached).
- **`sub_802763C`**: three more change-detection-gated widgets, keyed
  off `sync_value_a`/`b`/`c` (`include/hud.h`, already named from the
  second pass) against `sub_8023270`/`sub_8023268`/`sub_8023260`. The
  first two split their value into tens/ones digits
  (`sub_8037E54`/`sub_803AF1C`, div/mod by 10) across a slot pair each
  (14/15, 17/18) using the same clamp idiom as `sub_802757C`; the third
  does **not** split - slot 20 gets the raw value as its desired frame,
  slot 21 always gets a fixed desired frame of 0 (a single-frame icon,
  not a digit, matching `sub_8027138`'s own slot-21 setup). All six
  slots, plus `sub_8027138`'s own slots 16/19, get redrawn
  unconditionally afterward via nine `sub_80270E0` calls - slot 21
  appears twice in that list, which the C reconstruction reproduces
  exactly rather than treating as a typo. Not attempted for
  byte-matching this pass, given `sub_802757C`'s r7 blocker sitting
  right next to it in the same file and very likely recurring here too
  (the raw disassembly shows the same `ldrb r7, [...]` shape at every
  one of its six clamp sites).

### Still fully untouched: `sub_8027138`/`sub_802732C`,
    `sub_8027940`/`sub_8027D5C`/`sub_8027E88`

Read enough to characterize but not reconstructed this pass, given the
time already spent on the six functions above:

- **`sub_8027138`** (constructor) / **`sub_802732C`** (its own tail,
  called by `sub_8027138` itself with a second argument of 0): allocates
  the 35-slot `parts` array (`sub_8026EC0(0x8C4)` - a leading 4-byte
  header word holding the count `0x23`, *then* the 35
  `struct hud_digit_part` slots, not the trailing-padding read the
  second pass guessed), constructs every slot via the already-matched
  `sub_8027120`, zeroes three more `struct hud_counter` fields
  (`+0x0c`/`+0x10`/`+0x14`, still nameless - meaning not established),
  then loops over all 35 slots wiring each one's `anim_data` from a
  triple-indirected shared table (`**gUnknown_030012D0`, the same
  global `src/graphics/settings_menu6.c` already names and uses via its
  own `(**gUnknown_030012D0) + (const << N)` idiom) and either a
  per-slot glyph table (`gStaticData_08174BE0[i]`) or, for slot 22
  specifically, `sub_80233B4(...)+ 6`. `sub_802732C` is the exact same
  per-slot setup loop's *tail end*: it (re)positions and re-clamps a
  further handful of specific slots (13, 16, 19, 21) using the same
  clamp idiom as `sub_802757C`/`sub_802763C` above. Both make heavy use
  of the high registers (`sl`/`sb`/`r8`) throughout the loop body - the
  same class of "loop/self pointer never lands in r8/sb no matter how
  the source is phrased" difficulty `src/graphics/settings_menu6.c`'s
  own `UPDATE_ICON_FRAME_NIBBLE` comment already documents giving up on
  for four near-identical functions in that file; near-certain to hit
  the same wall here, on top of needing `sub_802757C`'s r7 blocker
  solved for every one of the ~10 clamp sites across both functions.
- **`sub_8027940`** (score-style counter): a *much* bigger sibling of
  `sub_8027838` - two separate 3-digit displays (fields `+0x24`/`+0x48`
  and `+0x28`/`+0x4c`, each with its own change-detection cache),
  each independently branching 3-digit vs. 2-digit vs. 1-digit (hiding
  the unused leading slots via `frame_index = -1`, exactly like
  `sub_8027838`'s single-digit case), plus one more icon
  (`gStaticData_08174C6C`-positioned, slot 10) whose x/y table index is
  itself picked from a 3-way digit-count check. Over 1000 bytes with
  roughly a dozen clamp sites - the same r7/high-register concerns as
  above apply throughout.
- **`sub_8027D5C`**/**`sub_8027E88`**: not read in this pass beyond
  their entry (mode-dispatch header identical in shape to
  `sub_8027940`'s own `self+8`/`self+0xc` check) - `sub_8027D5C` calls
  `sub_802325C` where `sub_8027940` called `sub_8023414`, suggesting the
  same digit-counter shape against a different value source;
  `sub_8027E88`, per the rom_map.md "fx" investigation, is the
  percentage-counter widget with a `cmp r1, #0x64` special case.

These five stay exactly as the earlier passes found them - real bytes,
`asm/code_3_2_17_27138.s` (`sub_8027138`/`sub_802732C`) and
`asm/code_3_2_20.s` (`sub_8027940`/`sub_8027D5C`/`sub_8027E88`) - not
parked, not matched. **Issue #45 stays open**: 2 of the original 9
untouched functions (`sub_8026F54`/`sub_8027018`) are now byte-exact
matched, 2 more (`sub_802757C`/`sub_802763C`) are parked with a
specific, reproducible compiler blocker identified, and 5 remain fully
raw.

## Cross-references (third pass)

- `src/graphics/hud_stat_widget2.c` - new file, `sub_802757C`/
  `sub_802763C`'s `NON_MATCHING` reconstructions.
- `docs/status/hud.md` - matched and parked lists both updated.

## NAKED-transcription pass

Closes out `sub_802757C`/`sub_802763C`, the two functions the third pass
parked above. Both are now byte-exact matched, converted from their
`NON_MATCHING`-guarded plain-C reconstructions to `NAKED` functions
whose bodies are a direct instruction-for-instruction transcription of
the ROM's own disassembly, following this project's established escape
hatch for this exact class of problem (`src/system/link_cable.c`'s
several `NAKED` functions, `src/audio/gax_swi.c`'s `sub_80392C4`,
`docs/matching/issue-4-sio-settings-sync.md`'s "NAKED transcription,
byte-verified" section for the worked-out general method).

The third pass's blocker was real, not a budget cut, and re-attempting
register-pin tricks here would have been pointless: every phrasing of
`records[index]` for the per-slot `anim_index` byte kept in r7
(`self+0x5AD`/`self+0x5ED` for `sub_802757C`, and the equivalent
per-slot byte at each of `sub_802763C`'s six clamp sites) reliably
miscompiled the very next use of that byte into a bogus `mov r0, sp` /
shift-mask read of the stack pointer itself, with no call in between to
blame it on. Since both functions were already fully understood
semantically (the third pass's doc comments above walk every field,
branch and call), nothing here is a fresh reverse-engineering problem -
just a mechanical transcription, checked instruction-by-instruction
against the ROM disassembly rather than inferred.

### Mechanics

Both functions' real bytes (previously `asm/code_3_2_17_2757c.s`, split
into two `.if NON_MATCHING == 0` blocks) were translated from the
disassembler's unified syntax to the plain (divided) syntax this
project's other `NAKED` functions and `arm-none-eabi-as`'s default mode
use: `adds`/`subs`/`lsls`/`movs`->`add`/`sub`/`lsl`/`mov` throughout (no
`s`-suffixed Thumb1 mnemonic survived - `arm-none-eabi-as` in this
project's default mode rejects several of them as hand-written text even
though they're valid encodings). The original's real `_08XXXXXX:`
address labels (both branch targets and `ldr rX, =symbol`/`=literal`
literal-pool entries) became GNU-as local numeric labels, referenced
`Nf`/`Nb`, since a `NAKED` function's asm block can't use the real ROM
address as a label. `sub_802763C` reuses one such label (`2:`, the
`gUnknown_030012C0` pool entry) across three separate `ldr r4, 2f`
sites spread through the function - safe here since only one `2:` is
ever defined in that asm block, so every forward reference resolves to
the same single literal-pool word regardless of how many places load
it.

### Verification

Isolated compile (`arm-none-eabi-cpp`/`agbcc`/`arm-none-eabi-as`, the
same flags `Makefile`'s `C_BUILDDIR` rule uses) produced both functions
at exactly their expected ROM sizes - `sub_802757C` at 0xC0 (192) bytes,
`sub_802763C` at 0x1FC (508) bytes, matching `sub_802763C - sub_802757C`
and `sub_8027838 - sub_802763C` respectively - and `objdump -d` showed
every single instruction, operand and relative branch offset identical
to the original ROM disassembly before this was ever wired into
`ldscript.txt`. Confirmed for real via a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide`), with `hud_stat_widget2.o` now replacing
`asm/code_3_2_17_2757c.o` in `ldscript.txt`'s link order and the
now-fully-empty `asm/code_3_2_17_2757c.s` deleted (same treatment the
third pass gave `asm/code_3_2_17_26f54.s` once `sub_8026F54`/
`sub_8027018` were both matched out of it).

Issue #45's remaining scope: `sub_8027138`/`sub_802732C`,
`sub_8027940`/`sub_8027D5C`/`sub_8027E88` are still fully untouched (see
"Still fully untouched" above) - this pass did not attempt them, and
issue #45 stays open.

### Cross-references

- `src/graphics/hud_stat_widget2.c` - `sub_802757C`/`sub_802763C`, now
  `NAKED`, no `NON_MATCHING` guard.
- `tools/report_units.py` - `0x0802757C` entry now points at
  `src/graphics/hud_stat_widget2.o` instead of `None`.
- `docs/status/hud.md` - moved from "Parked" to "Matched".

## Fourth pass

Closes out the last 5 functions the third pass left "fully untouched":
`sub_8027138`/`sub_802732C` (the 35-slot icon-array setup pair) and
`sub_8027940`/`sub_8027D5C`/`sub_8027E88` (the rest of the digit-counter
family). **All 5 are now byte-exact matched** - GitHub issue #45's
`0x08026EEC`-`0x08028568` chunk is complete.

### Approach: NAKED transcription from the start, not a fresh C attempt

The third pass's own writeup already flagged both groups as near-certain
to hit compiler blockers this project had already fully diagnosed and
worked around exactly once before, on this exact function family's
immediate siblings:

- Every clamp site in all 5 functions (roughly two dozen of them) uses
  the identical `records[index]`-via-a-byte-kept-in-r7 idiom that
  `sub_802757C`/`sub_802763C` (this same doc's "NAKED-transcription
  pass", directly above) already proved miscompiles in this `gcc 2.9`
  build - a wrong-*value* read (a bogus `mov r0, sp` / shift-mask
  sequence) on the very next use of the pinned byte, not merely a
  register-choice mismatch, and confirmed unrelated to the
  live-across-a-`bl` r7 bug documented elsewhere in this project.
- `sub_802732C`'s own loop additionally keeps a running byte offset in
  `r8` and two more loop-invariant values in `sb`/`sl` live across the
  entire ~10-`bl`-per-iteration loop body - the same "loop/self-pointer
  register allocation difficulty `src/graphics/settings_menu6.c`'s own
  comment documents giving up on for four near-identical functions"
  the third pass's read-through already named for this exact function.

Given both blockers were already reproduced and root-caused (not just
suspected) on this family's immediate neighbors, re-attempting
register-pin tricks here first would have just re-derived the same
negative result at several times the cost. Both groups went straight to
full `NAKED` instruction-for-instruction transcription instead -
this project's established escape hatch for this exact class of problem
(`src/system/link_cable.c`'s several `NAKED` functions,
`docs/matching/issue-4-sio-settings-sync.md`'s "NAKED transcription,
byte-verified" section for the general method, and this same doc's own
"NAKED-transcription pass" above for the worked example on this
family). Semantics for all 5 were read and understood in full first (see
the field-by-field comments in the two new files below) - nothing here
is unreviewed opaque asm, just asm written by hand rather than by gcc.

### Matched: `sub_8027138`/`sub_802732C` (new `src/graphics/hud_digit_array.c`)

`sub_8027138` allocates the 35-slot `struct hud_digit_part` array
(`sub_8026EC0(0x8C4)` - a leading 4-byte header word holding the count
`0x23` = 35, then the 35 slots themselves), constructs every slot via
the already-matched `sub_8027120`, zeroes `mode`/`layout_value`/
`field_08`/the rest of `unknown_0c`, then loops over all 35 slots wiring
each one's `anim_data` from the same triple-indirected shared table
`settings_menu6.c` already names (`(**gUnknown_030012D0) + (const <<
N)`) and a frame index from `gStaticData_08174BE0[i]` (or, for slot 22,
`sub_80233B4(...) + 6` - the same "life count" special case the
dispatcher itself, `sub_80274EC`, also singles out). One more slot past
the main 35 gets the same treatment from a different shared-table
offset, three trailing digit/icon slots get their initial clamped frame
index set up front, and finally `sub_802732C(self, 0)` runs to finish
the rest. `sub_802732C` is that same per-slot loop's tail: stores its
second argument into `self->icon_flag`, finishes the two slots
`sub_8027138` only partially set up (including the same `field_29`-low-
nibble update `settings_menu6.c`'s `UPDATE_ICON_FRAME_NIBBLE` macro
documents for the unrelated `struct settings_icon_actor` family), then
loops over all 35 slots again repositioning/re-clamping a handful of
specific ones (13, 22, 29) depending on the current level/game-mode and
`self->icon_flag`, before DMA-filling nine words at `self+0x40` with
`-1` via a raw `REG_DMA3SAD`/`DAD`/`CNT` poke (the same low-level idiom
`settings_menu8e.c`'s `sub_8002AA4` already uses for an unrelated
struct, address kept raw in the transcribed asm the same way that file
keeps it).

### Matched: `sub_8027940`/`sub_8027D5C`/`sub_8027E88` (new `src/graphics/hud_stat_widget3.c`)

`sub_8027940` is a much larger sibling of the already-matched
`sub_8027838`: two independent 3-digit displays (change-detection cache
pairs at `self+0x24`/`self+0x48` and `self+0x28`/`self+0x4c`), each
independently branching 3-digit vs. 2-digit vs. 1-digit (hiding unused
leading slots via a desired frame of `-1`, exactly like `sub_8027838`'s
own single-digit case), plus one more icon whose x/y table index is
itself picked from a 3-way digit-count check on the first counter's
value. `sub_8027D5C` is a smaller sibling - one 2-digit display sourced
from `sub_802325C` - that also always refreshes one more fixed slot
regardless of whether its value changed. `sub_8027E88` is the
percentage-counter widget (`docs/rom_map.md`'s "fx" investigation named
it this from its own `cmp r1, #0x64` special case): a value of exactly
100 shows a single dedicated icon instead of splitting into digits,
otherwise the usual 2-digit-vs-1-digit split runs; the function then
repeats the same shape a second, independent time, gated by its own
`sub_8031784`/`self->field_3c`/`self->field_60` change-detection triple
- two percent-style readouts sharing one function body.

### Verification

Isolated compile (`arm-none-eabi-cpp`/`agbcc`/`arm-none-eabi-as`, the
same flags `Makefile`'s `C_BUILDDIR` rule uses) caught two mechanical
issues before the real build: this project's established `neg rX, rX`
spelling (not `rsb rX, rX, #0`) for the two-operand negate idiom in
plain (divided) syntax, and one literal-pool block in `sub_8027E88`
that needed 4 more entries merged into it (the ROM batches literal
pools at the next safe point rather than one per use site, and this
transcription's first draft under-counted how many distinct pool slots
that particular batched pool covers). Confirmed via a full clean `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare` (`La suma coincide`), with both new files replacing
`asm/code_3_2_17_27138.s` and `asm/code_3_2_20.s` (both now deleted, no
raw asm left anywhere in this issue's `0x08026EEC`-`0x08028568` chunk)
in `ldscript.txt`'s link order.

**GitHub issue #45's scope is now fully matched.**

### Cross-references

- `src/graphics/hud_digit_array.c` - new file, `sub_8027138`/
  `sub_802732C`, both `NAKED`.
- `src/graphics/hud_stat_widget3.c` - new file, `sub_8027940`/
  `sub_8027D5C`/`sub_8027E88`, all three `NAKED`.
- `tools/report_units.py` - the `0x08027138` and `0x08027940` entries
  now point at these two new object files instead of `None`.
- `docs/status/hud.md` - matched list updated with both new files.

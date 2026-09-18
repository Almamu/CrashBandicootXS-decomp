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

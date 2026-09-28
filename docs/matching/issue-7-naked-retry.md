# Retrying issue #7's NAKED functions (`0x08004CB4`-`0x080060AC`)

Issue #7's second pass
([issue-7-0x08004d74-overlay-ui.md](issue-7-0x08004d74-overlay-ui.md))
converted 13 functions of the composite pause/options screen to NAKED
transcriptions and deleted their C drafts. This pass wrote each one again
as C and tested it under both compilers. It closed 9 of the 13.

**Compilers:**

- `settings_menu6.c` (4 closed) and `settings_menu22.c` (2 closed) are
  now on `OLD_AGBCC_OBJS`. Their functions only match under old_agbcc.
  `sub_8005A78`, the one function in `settings_menu6.c` that was already
  matched, compiles the same under both.
- `settings_menu7.c` (2) and `sub_800556C` in `settings_menu21.c` match
  under both compilers, so they stay on current agbcc.

No file needed splitting.

## Closed (9 functions)

| Function | What it took |
|---|---|
| `sub_8005AE8`, `sub_8005B80`, `sub_8005C58` (icon-array constructors) | The ROM's loop recomputes `self + 0x8c + i*4` and the table addresses on every iteration, so there is no strength reduction. The old note blamed "self never lands in r8". The actual fix is the store `self->iconsXX[i] = (icon = ctor(...), icon)`. Because its right-hand side is not a bare call, gcc computes the slot address *before* the two calls, and that also stops the loop pass from reducing it. Three smaller fixes were needed. The position is passed through `static inline set_icon_pos(actor, struct icon_pos *)`. The `field_29` low nibble is a `u8 lo:4` bitfield assignment, which replaces the pinned `UPDATE_ICON_FRAME_NIBBLE`. The `for` loop has to stay a real loop: the `-16` mask is hoisted and then re-materialized as `mov r3,#0x10; neg; add r1,r3`. old_agbcc only. |
| `sub_8005D44` (medal widget) | Three changes. The record word is read through a byte-offset local, `off = levelIdx*4 + 4`, which keeps `lsl; add #4` ahead of the base load. `(u16)word >> 3` gives the word load plus `lsl 16; lsr 19`; a `time:13` bitfield would give `ldrh`. The medal tagging goes through `static inline set_icon_frame(icon, u32 frame)`: a *word* parameter keeps the ROM's `ldr =tbl; ldr icon; ldr frame` order, and a `u8` one narrows to `ldrb`. old_agbcc only. |
| `sub_8005EF4`, `sub_8005FBC` (percentage dec/inc) | The first argument of `sub_80060AC` is `count * 5`, which is the percentage. The old draft passed `count` and had also dropped `lsl #2; add`. The fix is a plain `switch` on the row type plus `static inline format_pct(buf, value)` that writes `" <" digits "%>"` using `buf[len + 2]` indexing. The level is `((count << 8) + 1) / 20` through `__divsi3`. Matches under both compilers. |
| `sub_80057E0`, `sub_80058C0` (fraction readouts) | `sub_803AD80` is `_call_via_r2`, so the draws are icon-manager virtual calls. The call is a block macro with `_m = mgr; _r = _m->record` locals, so `this` is computed before the label argument; an inline function computes the label first. The position is set through `static inline set_icon_mgr_pos`, and the loop over `iconsB0` is a plain `for`. old_agbcc only: it produces the mask-before-`ldrb` order in the flag tests. |
| `sub_800556C` (row list renderer) | Matched on the first try with a plain `for` loop, a `switch` on the row type tag, and the same macro-plus-setter shape. The stack-resident row Y that the old note worried about comes out naturally. `sub_8028A40` has to be declared with one argument. Matches under both compilers. |

## Didn't close (4 functions, near-miss drafts left under `NON_MATCHING`)

- **`sub_8005100`** (input driver): 14 halfwords off under old_agbcc
  (72 under current agbcc). Everything else matches. The input loop
  has to be a hand-written goto loop (`goto body; top: B check; body:
  ...`), because a real loop hoists `&keys`, `0x1e` and the audio
  context. The two fade ramps have to stay real `while` loops over a
  packed `level:5` bitfield. The remaining gap is the L/R key-repeat
  test. The ROM re-materializes the key mask (`mov r3,#K; mov r0,#K; and
  r0,r1`) and reads the pressed half with `lsr #16`. The draft either
  folds `in >> 16` into `ldrh [keys+2]` or allocates the mask
  differently.
- **`sub_80053F4`** (per-frame draw): 20 halfwords off under both
  compilers. The jump table, the `field_74` block and every draw match.
  The two positions whose x comes from a measured label width are left:
  the ROM has x in r3 and y in r2, and the draft has x in r1/r2 and y in
  r3. Inline setters with different parameter orders, macros,
  temporaries and pins did not change this.
- **`sub_8004D74`** (screen constructor): 54 halfwords off under both
  compilers. `init_icon_mgr(mgr, base)` and `reserve_icon_vram(n)`
  inline helpers now reproduce the ROM's `0` held in r8. The slot-6
  calls are `_call_via_r1` virtual calls. Register allocation is still
  wrong: the ROM keeps `&gUnknown_030012B8` in r6 and the two icon
  managers in r4/r5, so it has no register left for `0x12c` and
  re-materializes it. Pinning those registers causes a stack spill.
- **`sub_8005E5C`** (fraction draw): 73 halfwords off (the draft is 16
  bytes too long). The ROM keeps `0x130` and `0x110` in callee-saved
  registers but not `0x114`, which it later derives as `r7 + 4`. The
  draft CSEs all three and runs out of low registers.

## Verification

`rm -rf build && make NON_MATCHING=1 report` shows no warnings from the
touched files. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` prints `crashbandicootxs.gba: OK`.

## Later pass

The early-ROM NAKED retry
([early-rom-naked-retry.md](early-rom-naked-retry.md)) brought the
`sub_8005100` draft to 5 halfwords. The key-repeat tests now match. The
other three are unchanged.

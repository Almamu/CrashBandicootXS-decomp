# GAX2 audio engine retry: toolchain, then plain C (issues #66-#68)

Retry of the NAKED/raw functions in `0x08037110`-`0x0803A944` (the GAX2
sound engine plus the counter widget and GAX2's bundled libgcc
helpers). 27 targets: 26 NAKED + `sub_8037FC0` (raw asm). **21 closed
as real C**, 1 is hand-written asm in the original and stays NAKED on
purpose, 5 are left (4 NAKED with drafts, 1 raw).

## Step 1: which toolchain?

GAX2 is a third-party library, so the first question was whether it was
built with a different compiler, flags or instruction set.

- **Compiler.** A compiler map over every already-matched file in the
  range (compile each under `agbcc` and `old_agbcc`, count exact
  functions) came back "both" for everything except
  `gax_channel_effect_table.c`, which only matches under current agbcc.
  Every function closed below matches under current agbcc; old_agbcc is
  never better. **GAX2 is current-agbcc code with the project's normal
  flags.**
- **ARM?** None of the NAKED bodies are ARM - all Thumb. The ARM code in
  this range is the raw DSP/mixer block after `sub_803A5A8`
  (`gStaticData_0803A630` onward, tagged "FILT"/"BART"), which is
  hand-written ARM asm copied into IWRAM at play start; it stays a byte
  transcription (it isn't a function in the report).
- **Flags - the libgcc exception.** `sub_8037648`/`sub_8037A7C`/
  `sub_8037ECC` return via a combined `pop {r4-r7, pc}` that no other
  code in the ROM uses. That's just agbcc's epilogue *without*
  `-mthumb-interwork`: with the flag dropped, gcc 2.x's `libgcc2.c`
  source compiles to them byte-for-byte (`__divdi3`, `__udivdi3`,
  `__muldi3`; generic `longlong.h` macros, `UDIV_NEEDS_NORMALIZATION`,
  a static `__clz_tab` per object - the two tables are
  `gStaticData_085A4C70`/`085A4D70`). `math_div64_util.o` is now built
  with `-mthumb-interwork` filtered out (Makefile `NO_INTERWORK_OBJS`);
  every function in that object matches with it (`sub_8037E54` is
  NAKED, so unaffected), and `sub_8037F3C` - the one function in the
  old file with a normal interworking return - moved to
  `src/audio/gax_zero_fill.c` (it's GAX2 code, called from
  `sub_8038538`/`sub_8039B44`). `__udivmoddi4` lives in
  `include/libgcc2_udivmoddi4.h`, included once per division object
  with that object's `__clz_tab`; passing the table as a parameter
  instead hoists its address into a register.
- **Hand-written asm.** `sub_8037E54` is lib1funcs.asm's `__udivsi3`
  (shift-by-4-then-1 normalization, per-path `push {r4}` ... `mov pc,
  lr` vs `push {lr}; bl __div0` = `nullsub_8`). Not compiler output -
  **leave NAKED**. Don't retry it.

## Step 2: what actually blocked the rest

Not the compiler. Two things:

1. **The "manual return-address trampoline" is GAX2's own inline asm.**
   `mov r2, pc; adds r2, #5; mov lr, r2; bx r1` (calling an ARM routine
   from Thumb on ARMv4T) was the documented reason `sub_803A278`/
   `sub_803A2C8`/`sub_803A324`/`sub_803A5A8`/`sub_8039B44` "couldn't be
   C". The shape around it - the argument stored to a stack slot and
   reloaded *after* `mov r1, rX` - pins down the operands: it's
   ```c
   asm volatile("mov r1, %1\n\tldr r0, %0\n\tmov r2, pc\n\tadd r2, #5\n\t"
                "mov lr, r2\n\tbx r1\n\tnop" : : "m"(arg), "r"(fn)
                : "r0", "r1", "r2", "lr")
   ```
   (`GAX_CALL_ARM` in `include/audio.h`; `sub_8039B44` uses a
   register-operand variant, `GAX_CALL_ARM_R`). The "fused" labels
   `sub_803A318`/`sub_803A608`/`sub_8039E50` are just the `nop` the call
   returns to - not functions. `sub_803A278`'s work item is a struct with
   a non-constant initializer (the `bl sub_800014C` in the ROM is gcc's
   own memcpy of the initializer temp: `.set memcpy, sub_800014C`); the
   first field that's read before the stores has to go through a local.
2. **Over-pinned drafts.** Almost every "many-register r8/sb/sl
   allocation ceiling" function matched outright once rewritten plainly
   against named structs with no register pins at all. The GAX2 object
   model (all in `include/audio.h` now): a player is an array of
   handlers (`[0]` mixer, `[1]` Info, then channels, then SFX voices),
   each a `struct GaxHandler` header (`type`, `format`, `children`)
   followed by its own state; `struct GaxHandlerType` holds
   `init`/`unknown`/`play`, `childCount`, `childTypes`, `instanceSize`
   and a data union (song data / order list / DSP taps). The channel
   state is `struct GaxChannelState`.

Recurring details that mattered:

- **Type-based aliasing is on.** Stores through one pointer type don't
  invalidate loads of another: `sub_80398DC` needs every access written
  as `self->instrument->...` (caching it in a local loses a `mov`), and
  `sub_8038C88` needs the player's handler array typed
  `struct GaxHandler **` (a `void **` view makes the loop reload
  `songPtr`). `GAX_PLAYER()`/`GAX_SONG()`/`GAX_MIXER()`/`GAX_INFO()`
  give these typed views.
- **Re-derive, don't cache**, like the ROM: `GAX_SFX_VOICE(i)` re-walks
  player -> mixer -> children at every use (`sub_8038DC0`/`E74`).
- Write known-zero stores as `0`, not as the variable that happens to
  be zero (`sub_8038A1C`) - the ROM's reuse of the zero register is CSE.
- `sub_8038E74` compares its "found" index against `0x0FFFFFFF`, not
  -1 - a bug in the original, reproduced.
- Pointer-walking loops: when the ROM loads `[p, #off]` and bumps `p`,
  walk a struct pointer (`sub_803A324`'s voice scan, the tap-table scans
  in `sub_8038538`); indexing lets gcc fold the offset into the pointer.
- `sub_8039518`: store the division result through an inline helper
  taking the destination pointer first
  (`SetMixRateReciprocal(&gUnknown_03001618, self)`) - that's what
  loads the address into a callee-saved register before the call.
- `sub_80372BC`: call the icon method trampoline `sub_803AD80` directly
  with the glyph assigned inside the first call's argument list.
- `sub_80392C4` (HuffUnComp wrapper): the ROM saves r8 but not r7 even
  though it clobbers r7 - agbcc's r7-pin bug. Here the bug *is* the
  evidence: the source pinned the saved argument to r7, and reproducing
  the ROM needs that same pin. (Flagged because the project normally
  never pins r7.)
- Trailing `asm(".align 2, 0")` where a file's last function ends on a
  halfword boundary.

## Results

| Function | File | Result |
|---|---|---|
| `sub_80372BC` | counter_selector_icons.c | **C** (direct trampoline call) |
| `sub_8037388` | counter_selector_icons.c | NAKED, draft (later: **C**, [late-rom-naked-retry.md](./late-rom-naked-retry.md)) |
| `sub_8037648` | math_div64_util.c | **C** (libgcc2 `__divdi3`, no-interwork) |
| `sub_8037A7C` | math_div64_util.c | **C** (libgcc2 `__udivdi3`, no-interwork) |
| `sub_8037E54` | math_div64_util.c | NAKED - hand-written asm (final) |
| `sub_8037ECC` | math_div64_util.c | **C** (libgcc2 `__muldi3`, no-interwork) |
| `sub_8037FC0` | asm/code_3_2_20c.s | raw, see below (later: NAKED + draft in gax_work_size.c) |
| `sub_8038240` | gax_channel_table_alloc.c | NAKED, draft |
| `sub_8038538` | gax_playstart.c | NAKED, close draft |
| `sub_8038A1C` | gax_channel_pool_alloc.c | **C** |
| `sub_8038C88` | gax_voice_steal.c | **C** (typed handler array) |
| `sub_8038DC0` | gax_voice_steal.c | **C** |
| `sub_8038E74` | gax_voice_steal.c | **C** |
| `sub_80392C4` | gax_swi.c | **C** (r7 pin reproduces the ROM's r7 bug) |
| `sub_8039518` | gax_sound_handler_channel_init.c | **C** (inline dest-pointer helper) |
| `sub_80395A4` | gax_sound_handler_channel_play.c | **C** |
| `sub_8039658` | gax_sound_handler_channel_play.c | **C** |
| `sub_80398DC` | gax_channel_note_scheduler.c | **C** (no cached instrument) |
| `sub_8039AA4` | gax_channel_envelope_tick.c | **C** |
| `sub_8039B44` | gax_note_trigger.c | NAKED, full draft |
| `sub_8039F30` | gax_note_lookup.c | **C** |
| `sub_803A03C` | gax_channel_pos_sweep.c | **C** |
| `sub_803A158` | gax_channel_note_cut_driver.c | **C** |
| `sub_803A278` | gax_unknownc_play.c | **C** (`GAX_CALL_ARM`) |
| `sub_803A2C8` | gax_unknownc_play.c | **C** (`GAX_CALL_ARM`) |
| `sub_803A324` | gax_unknownc_play.c | **C** |
| `sub_803A5A8` | gax_unknownc_play.c | **C** (`GAX_CALL_ARM`) |

All current agbcc, normal flags, except the three libgcc2 functions
(current agbcc without `-mthumb-interwork`).

## What's left, and what was observed

- **`sub_8039B44`** (per-channel mixer, 1004 bytes incl. `sub_8039E50`).
  The draft under `#if NON_MATCHING` has the ROM's control flow, the
  10-word work item, the four ARM-code patches and tail merges, and is
  within 12 bytes of the ROM's size, but allocation differs from the
  first instruction: the ROM homes `self`/`info` in r6/r4, keeps `flag`
  in r5, `vol` in r7, `row` in sb, the step in sl and spills the wave
  pointer; agbcc picks r5/ip for `self`/`info` and keeps the wave in sl.
- **`sub_8038538`** (play start). Close draft: same control flow,
  literal pool and buffer carving; agbcc gives the format pointer r8 and
  `maxRate` r9, the ROM the reverse, and that cascades. Declaration
  order and writing the zero stores as constants didn't flip it.
- **`sub_8038240`** (handler instantiation/linking). Draft has the
  ROM's shape; the first loop's spill-slot/register assignment differs
  (the ROM spills the handler pointer and keeps the layout count in an
  extra stack slot).
- **`sub_8037388`** (counter widget init). Draft matches through the
  tile-copy loop; the tail's `0x108`/`0x12c`/`0x130` field offsets get
  CSE'd into callee-saved registers across the calls where the ROM
  rematerializes them after each call. Pinning the manager addresses to
  r4-r6 just moves the constants to r8-sl; `do {} while (0)` around the
  method calls doesn't split the CSE blocks.
- **`sub_8037FC0`** (still raw asm, `asm/code_3_2_20c.s`): computes the
  work-RAM size a song header needs - handler instances for the layout
  (and the largest SFX sub-layout), 0x58 per SFX voice, the echo
  buffer (`maxRate * rate / 1000 * 2 + 0x18`), 0x130/0xdc for the ARM
  mixer copy, and two mix buffers (`rate * 1000 / 0xE94F * 2` each).
  A first C draft (in the scratch area, not checked in) has the right
  shape but the ROM spills more (samples, `samples << 5`, a u16 copy of
  the flags) and reloads the layout count every iteration of the first
  loop instead of strength-reducing it.

## Reporting note

`expected/code_3.s` still labels `sub_803A318`/`sub_803A608` (and
`sub_8039E50`) as functions. They're gone from the matched objects (they
were only return points), so objdiff will pair `sub_803A2C8`/
`sub_803A5A8` against a shorter target symbol. The ROM is unaffected;
`expected/corrections.txt` has no "merge" directive for this yet.

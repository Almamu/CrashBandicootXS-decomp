# Second near-miss sweep

This pass went back to eleven NAKED functions whose `#if NON_MATCHING`
drafts were 20-62 halfwords off. It tried the extra-reference nudge from
the first near-miss pass ([near-miss-polish.md](near-miss-polish.md)),
`asm("" : : "r"(x))`, on each of them. Five now match as real C:

| Function | File | Compiler | Was | Technique |
|---|---|---|---|---|
| `UpdateLinkSession` | `src/system/link_cable.c` | old_agbcc (both match) | 37 | `asm("" : "+r"(one1))` keeps a second 1; `asm("" : "+r"(arm3))` between `^` and `&` blocks the `bic` fold |
| `RunPauseMenu` | `src/graphics/settings_menu15.c` | agbcc | 54 (4 bytes short) | `static inline` accessor for `field_12c` so CSE doesn't share the 0x12c offset; two locals fix load order |
| `DecodeCollisionChunk` | `src/system/game_loop3.c` | old_agbcc | 33 | explicit `<< 24 >> 24` sign extensions through `s32` locals; `asm("" : : "r"(n))` fixes the r4/r5 swap |
| `InitCellAnim` | `src/graphics/actor_part95.c` | agbcc (both match) | 37 | `asm("" : "=r"(reload) : "0"(a4))` copies the address used for the reload; evaluation-order tweaks |
| `SelectActorCategory` | `src/graphics/actor_part102.c` | agbcc (both match) | 125 (4 bytes long) | local pointer to `gActorSpawnIndex` plus one `asm("" : : "r"(idx))` so it outranks `base` |

All of the `asm` statements emit no code. Each one has a comment at its
use.

## What worked

**Extra references, as in the first pass.** `SelectActorCategory` and
`DecodeCollisionChunk` were plain priority swaps. A global's address can't take
an `asm` operand directly, so `SelectActorCategory` stores through a
local `s32 *idx = &gActorSpawnIndex`, and the reference goes on `idx`.
Its scan loops still have to use the global by name. With `idx` in the
loops too, the ROM's loop-local copies of the address (`adds r3, r7, #0`)
disappear.

**`"+r"` to keep a value gcc would merge or fold.** Uses in
`UpdateLinkSession`:

- Two `1` constants would be merged by CSE. `asm("" : "+r"(one1))`
  hides the fact that `one1` is 1. The two statements must not be
  identical: two non-volatile `asm("" : "+r"(v))` with the same input
  are merged by CSE too.
- The ROM computes the flag as `eor` then `and` with the same register.
  Combine turns `(x ^ m) & m` into `m & ~x` (`bic`) for any `m`. An
  `asm("" : "+r"(arm3))` between the two operations stops that.

`InitCellAnim` needed a second pointer to the same global, one for the
store and one for the reload. Here the `asm` has an output operand tied
to its input: `asm("" : "=r"(reload) : "0"(a4))`.

**`static inline` accessors to break CSE (`RunPauseMenu`).** The ROM
rebuilds the 0x12c offset (`movs r1, #150; lsls r1, #1`) for every
`field_12c` read, even twice within one expression. Reading through
`static inline u32 mgr_12c(struct icon_manager *m)` stops CSE from
sharing it, which frees the register it held. The five callee-saved
pointers then fall into the ROM's r4-r7.

**Sign extensions as shifts (`DecodeCollisionChunk`).** The ROM interleaves
`(s8)pair`'s `lsl #24`/`asr #24` around `acc`'s own `lsl #16`/`asr #16`.
`acc += (s8)pair` emits the pair's shifts back to back. Writing

```c
s32 lo = pair << 24;
s32 a = acc;

acc = a + (lo >> 24);
```

emits the left shift first, then `acc`'s extension, then the right
shift.

## Improved but not closed

| Function | Now | Left |
|---|---|---|
| `UpdateSlotCrate` | 7 (was 58, old_agbcc) | Separate locals for the phase test, the loop and the `0x38` switch, `asm("" : : "r"(lw))`, and `asm("" : "+r"(ph0))` fixed everything except the count update. There the ROM's `& 0xc7` writes the reloaded word's register (r1, with `t` in r2); the draft writes the constant's (r0, with `t` in r1). Operand order, a fresh local, and 0-2 extra references on `w`/`t` in every position didn't change it. The draft in the file is updated. |

## Not closed

| Function | Was | Best | What was observed |
|---|---|---|---|
| `DrawPauseMenu` | 20 | 15 | x/y locals with `"+r"`/`"r"` on `y` fix the first `set_icon_mgr_pos`. Extra references inside the inline helper have no effect at all. Putting the stores in the caller lets CSE share the 0x110/0x114 offsets between the two calls, which costs more than it saves. |
| `HitEnemy` | 21 | 21 | The remaining differences are reload registers. reload rotates through its spill registers (ROM r3, r3, r3, r4, r6, r2; draft r6, r4, r4, r6, r2), so they depend on how many reloads come earlier. Nudges and `do`/`if (1)`/block forms of `MarkGone` changed nothing. |
| `sub_800CD00` | 42 | 39 | `asm("" : "+r"(q))` on the player record's box pointer fixes 3 halfwords around it. The rest is the known sub_800D040 gap: `&f.b` gets a callee-saved register (r6) across the two builder calls, and the ROM recomputes `add r0, sp, #16` for each. Separate locals, an array, `(u8 *)&f + 16`, an opaque `pb` and moving `pb` earlier all left it the same. |
| `sub_800E08C` | 49 | 49 | The ROM's case 3 reads the byte flag from its spill slot with `mov r5, sp; ldrb`. Reading `*(u8 *)&f20` gives that `ldrb`, but the prologue then changes because `f20` lives in memory. `(u8)f20` after an `asm("" : "+r"(f20))` reloads a word and masks it (`lsl`/`lsr`). Prototype widths, struct/u8/s32 parameters and local types didn't produce the ROM's reload. |
| `SpawnFlamethrowerLabAssistant` | 62 | 62 | The ROM keeps `arg3` (later `arg3 * 2`) in r4; the draft gets r9. Extra references on `arg3` before its first use change nothing. References that keep it alive past the second `LEVEL_RECORD` make things worse, because the ROM only keeps `arg3 * 2`. |

## Notes for next time

- A non-volatile `asm` with identical operands is CSE'd like any
  expression. When two values need to stay distinct, give their `asm`
  statements different shapes or make one `volatile`.
- An `asm` barrier stops combine only between the two operations it
  separates. `UpdateLinkSession`'s `bic` fold is gone, but the same barrier
  placed before `arm3` is computed did nothing.
- Brute-force runners and specs are in the scratchpad `polish2/`:
  `brute2.py` (sorted by halfword count, accepts a list of (old, new)
  replacement pairs), `applyvar.py` (writes one variant into the
  source), and `s*.py` specs.

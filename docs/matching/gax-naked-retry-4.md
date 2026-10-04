# GAX NAKED retry 4: `GaxChannelMix` and `GAX2_init` (issues #68/#67)

Fourth pass, after [gax-naked-retry-3.md](./gax-naked-retry-3.md).
**Neither function closed.** Both stay NAKED. The `GaxChannelMix` draft
moved closer; `GAX2_init` is unchanged.

| Function | File | Before | Now |
|---|---|---|---|
| `GaxChannelMix` | gax_note_trigger.c | 46 | **43** |
| `GAX2_init` | gax_playstart.c | 100 | 100 |

Scores are `brute2.py`'s sequence score, as in retry 3, with the
`GaxChannelMix` ROM side cut at `0x3EC`. That score includes the literal
pools, whose words are relocations (zero) in the object and addresses
in the ROM. Checking the relocations shows every pool of the
`GaxChannelMix` draft already sits at the ROM's offset with the ROM's
entries. Most of the remaining 43 is that noise. The real differences
are about a dozen instructions, listed below.

Everything is plain agbcc with the normal flags.

## `GaxChannelMix`: what moved

1. **The row copy at the tune.** `row = self->row;` moved from after the
   empty-wave test to the tune, followed by `asm volatile("" :
   "+r"(row));`. GCSE still gives the row read its sb copy, and the tune
   now copies sb right where it uses it (ROM `mov r3, sb`). The escape
   keeps CSE from sharing the tune's `row * 28` with the ping-pong test.
   Without it that product gets merged, as retry 3 reported. With the
   copy no longer live across the pitch block, the `#38`/`#20` offset
   constants go to r3 like the ROM instead of r7.
2. **The sweep-length initializer reloads the instrument.** Reading
   `self->instrument` through a `volatile` pointer cast in the
   initializer gives the ROM's `ldr r1, [r6, #60]; ldrb r2, [r6, #16]`.
   Without it, GCSE reuses the `inst` copy in r8.

## `GaxChannelMix`: what's left

- **The tune's instrument pointer.** The ROM loads `self->instrument`
  into r0, uses r0 for the tune, and copies it to r8 (`inst`, which the
  ping-pong test uses) only between the `0xef3` load and the clamp's
  `cmp`. In the draft the load goes straight into r8, and the tune adds
  r8. The RTL shows the clamp is a `MIN_EXPR`: target copy, then the
  constant forced by `cmpsi`, then `cmp`. Nothing in `compare_from_rtx`,
  `emit_cmp_insn` or the thumb `cmpsi` expander emits code between the
  constant and the compare. So the ROM's copy placement isn't explained
  yet. These didn't change it:
  - `inst = self->instrument` before, between or after the tune and
    period statements, with the tune reading `inst` or `self->instrument`
    (all orders).
  - a second, volatile or barrier-separated `self->instrument` read.
  - the clamp as a named constant or `if`, or in an inline helper
    (value, value + limit, or the whole lookup).
  - an inline accessor for `self->instrument`, `self->row` or the row
    pointer.

  CSE always merges the copy into the load, or makes the tune's load
  the global pseudo.
- **The backward end's `sweepMin`** (`item.end = ... sweepMin << 11`).
  The ROM loads it into r3 and shifts into r0. The draft uses r0 for
  both. `"+r"`/input-only escapes, a volatile read and `* 2048` all
  leave it in r0, or make things worse. A local pseudo that dies in the
  shift gets r0 from local-alloc, so the ROM's value is probably a
  global pseudo left without a hard register and reloaded (the reload
  rotation lands on r3). That needs its set and use in different basic
  blocks, which the ROM's straight-line code doesn't show.

## `GAX2_init`: tried, no gain

- A `struct GaxPlayerState **pg = &gGaxPlayerState` local (before or
  after `k = 0`) for one or both copy loops. The global's address does
  move into sl first, but `*pg` becomes loop-invariant, so the copy
  loops get strength-reduced (`ldmia/stmia` countdown). Worse (103-113).
- The zero stored to `field_180`/`field_39`/`field_3a` as one variable,
  plain or through the constant-init asm (volatile or not). The asm form
  puts the zero in a high register (sl, not r8) but reshuffles the
  `GaxCreateHandlers` argument registers (128). The plain form is CSE'd back
  to constants (101).

The copy-loop setup order and the `GaxCreateHandlers` argument registers
(`layout`/`sfx`/`&size`/`&g` copy in r4/sb/r6/r5 in the ROM, sb/r8/r5/r4
in the draft) look like one global-alloc priority problem. It was not
pursued further.

## Tools

Scratch area `gax4/` (not checked in): `showall.py` prints only the
differing rows per variant and drops pool-relocation noise.
`pools.py`/`objpools.sh` list the ROM's and the object's literal-pool
words and offsets. `tos.sh` prints one function's agbcc assembly. The
variant specs are `b1`-`b10` (GaxChannelMix) and `p1`/`p2` (GAX2_init),
run with retry 3's `gax3/brute2.py` and `sbs.py`.

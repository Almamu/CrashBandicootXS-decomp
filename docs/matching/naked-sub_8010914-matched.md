# `sub_8010914`/`sub_801095C` converted from NAKED transcription to real matched C

`sub_8010914` and `sub_801095C` (`src/system/game_loop30.c`, the
"get prev"/"get next" neighbor-list-walk-and-filter helpers) had been
parked as byte-correct NAKED asm transcriptions - see
[issue-13-fc70-continuation.md](./issue-13-fc70-continuation.md) for
the original parking rationale. They're now genuinely matched as real
decompiled C.

## The original gaps

Each function walks its own list direction (`sub_801070C`/
`sub_8010708`), returning the furthest node reachable while every node
visited (other than `self`) has a `+0x4d & 0x7f` state != 1:

```c
cur = sub_801070C(self);
if (cur == NULL) return self;
if ((cur[0x4d] & 0x7f) == 1) return self;
for (;;) {
    next = sub_801070C(cur);
    if (next == NULL) return cur;
    if ((next[0x4d] & 0x7f) == 1) return cur;
    cur = next;
}
```

Two gaps, both shared with other functions already documented
elsewhere in this project:

1. **Cross-jump-merge shared-tail placement** (same class of gap as
   `sub_800FDC8`, game_loop33.c). The loop's two `return cur;` sites
   compile to byte-identical code (`add r0, r4, #0; b <exit>`); the
   ROM keeps them as a single physical copy reached by two *backward*
   branches, positioned *before* the loop body in the ROM's own
   layout - not because the ROM's compiler failed to merge them, but
   because that's genuinely where the shared block sits, physically
   ahead of the loop it's jumped into from. A natural top-to-bottom C
   reconstruction (guard checks, then `for(;;)` loop, then the shared
   return at the very end after the loop) places that block *after*
   the loop instead, letting it fall straight into the epilogue
   without needing its own trailing branch - correct bytes for the
   return itself, but positioned wrong relative to the loop, and the
   two backward `beq`s the ROM has become one that jumps 4 bytes
   forward instead of back.
2. **Mask-check instruction order** (same class of gap as
   `sub_800FEB0`/`sub_801085C`, game_loop22.c/game_loop24.c). A plain
   `(node[0x4d] & 0x7f) == 1` compiles as load-byte-then-mask; the ROM
   computes the field address first, loads the `0x7f` mask constant
   *before* the byte load, then loads the byte into the same register
   the address was just computed in:
   ```
   add r1, node, #0
   add r1, #0x4d
   mov r0, #0x7f
   ldrb r1, [r1]
   and r0, r0, r1
   ```

## The fix

### Gap 1: block placement

Write the shared return with explicit `goto`s, and place the
`returnCur:` label - and its own `return cur;` - *before* `loop:` in
program order, exactly where the ROM physically has it. The loop's own
exit checks (`next == NULL`, `masked == 1`) then become forward-looking
C `goto`s that compile to *backward* branches (since their target now
precedes them in the emitted code), matching the ROM's `beq 2b`
instructions one-for-one - and since `returnCur:`'s single instruction
now falls straight into the shared epilogue (nothing else sits between
it and `pop {r4,r5}; pop {r1}; bx r1`), no extra branch is needed
there either, matching the ROM's own "no trailing branch, straight
into the pop" shape:

```c
void *sub_8010914(void *selfArg)
{
    void *self = selfArg;
    void *cur;
    void *next;
    register u32 masked asm("r0");

    cur = sub_801070C(self);
    if (cur == NULL) {
        goto returnSelf;
    }
    /* ... mask check ... */
    if (masked != 1) {
        goto loop;
    }

returnSelf:
    return self;

returnCur:
    return cur;

loop:
    next = sub_801070C(cur);
    if (next == NULL) {
        goto returnCur;
    }
    /* ... mask check ... */
    if (masked == 1) {
        goto returnCur;
    }
    cur = next;
    goto loop;
}
```

This is the same "match the ROM's basic-block source order, not just
condition polarity" technique documented for `sub_80010E0`
(`naked-sub_80010e0-matched.md`), applied to block *placement* (which
label comes textually first) rather than which branch sense a single
`if` compiles to.

### Gap 2: the mask check

Materialized as one opaque `asm volatile` block per call site,
matching the ROM's own address-then-mask-then-load order and reusing
its exact register roles (r1 for the address/byte, r0 for the mask and
final result) - the same "opaque to the optimizer" technique used on
`sub_8001524`/`sub_8001624`/`sub_8000CBC`/`sub_80014A4`:

```c
asm volatile(
    "add r1, %1, #0\n\t"
    "add r1, r1, #0x4d\n\t"
    "mov r0, #0x7f\n\t"
    "ldrb r1, [r1]\n\t"
    "and r0, r0, r1\n\t"
    : "=r"(masked)
    : "r"(cur)
    : "r1", "cc"
);
```

`masked` is declared as `register u32 masked asm("r0");` (not a plain
`"=r"` local) - an unconstrained `"=r"` output left the compiler free
to pick a register other than the one the asm template's `and r0, r0,
r1` literally writes to, silently reading garbage at the `if (masked
== ...)` check that follows. Hard-pinning the C variable to the exact
register the template writes closes that gap.

This same mask-check block is already the established pattern in this
same file - `sub_80109A4` (already matched, further down
`game_loop30.c`) uses the equivalent materialization for its own
`self+0x4d & 0x7f` check.

`sub_801095C` is the "get next" twin, identical structure, using
`sub_8010708` in place of `sub_801070C`.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, `objdiff-cli
diff` against `build/expected/units/game_loop30_target.o`: 100% match
for both `sub_8010914` and `sub_801095C`. `objdiff-cli report
generate` succeeds (no symbol-pairing errors). Full clean `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map
&& make compare` - `crashbandicootxs.gba: La suma coincide`. Both
functions are folded into the same `src/system/game_loop30.o` unit as
the already-matched `sub_80109A4` in `tools/report_units.py`, since
it's the same object file.

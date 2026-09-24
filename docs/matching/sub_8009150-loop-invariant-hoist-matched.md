# `sub_8009150` converted from `NON_MATCHING` to real matched C

`sub_8009150` (now `src/graphics/actor_part11g.c`, previously an
`#if NON_MATCHING` block inside `src/graphics/actor_part11.c`) had been
parked as "extremely close" since an earlier pass - see `docs/matching.md`,
"Parked, not matched: `sub_8009150`". It's now genuinely matched as real
decompiled C.

## What it does

Lazily creates a "large object" bucket-255 spatial-hash-grid
registration for an object that didn't get one at insert time. Searches
every bucket (254 down to 0, i.e. every bucket except the special
"large object" bucket 255) of `manager`'s grid for a node whose data
pointer equals `obj`. On the first match: if the object's `+0xc` flags
byte bit 4 isn't set, returns immediately (nothing to do). If it IS set
but the node already has a bucket-255 secondary link (`node->field_0xc
!= 0`, the same field `sub_8009AF0`/`sub_8009B3C` set up), also returns
immediately - the link already exists. Otherwise, pops a fresh node off
the free list (the same `sub_8009AF0` pop idiom), wraps `obj` in it, and
inserts that new node into bucket 255's head/tail list, finally linking
the two nodes together via the original node's `field_0xc`. `sub_8009B3C`
only creates this link when `obj->flags` bit 4 is already set at insert
time; `sub_8009150` looks like the retroactive counterpart, called when
an object transitions to "large" status after insertion.

## The two gaps

### Gap 1: loop-invariant hoisting of the free-list-head address

The free-list-head field's address (`&manager->freeListHead`, i.e.
`manager+0x814`) doesn't change across the whole 255-bucket outer loop,
so a plain `&manager->freeListHead` (even written lexically inside the
loop body, only reached when a bucket turns out non-empty) is loop-
invariant from the optimizer's point of view - this compiler correctly
proves that and hoists the computation above the loop, caching it once
(into `ip`, an ARM/Thumb high register) and just doing `mov r6, ip` per
non-empty bucket:

```
ldr   r0, .L19        @ hoisted above the loop
add   r0, r0, r7
mov   ip, r0
.L6:                  @ loop top
    ...
    mov   r6, ip        @ per-iteration, just a register copy
```

The ROM instead recomputes the address fresh every time a non-empty
bucket is found - the same two-instruction sequence (a literal-pool load
plus an add) sitting *inside* the loop body, not hoisted:

```
.L6:                  @ loop top
    ...
    ldr   r0, .L19       @ per-iteration, full recompute
    add   r6, r7, r0
```

This is the opposite direction from most gaps in this codebase (where
gcc does too *little* work or picks the wrong register) - here gcc's
codegen is genuinely *better* than the ROM's, and has to be forced to be
"dumber" to match.

No portable C rephrasing defeats this - the expression really is
loop-invariant, so any equivalent C keeps getting proven invariant and
hoisted the same way. The fix is an opaque `asm volatile("" :
"+r"(manager))` barrier placed at the exact point in the loop body where
the address computation happens (once per non-empty bucket, same as the
ROM):

```c
register void **headField asm("r6");

asm volatile("" : "+r"(manager));
headField = &manager->freeListHead;
```

The barrier's `"+r"` constraint tells gcc `manager`'s value may have
been changed by the (empty) asm statement - which by itself only forces
`manager` to be re-read from its own register at that point, not
recomputed from anything else. But since the barrier sits *inside* the
loop, on the loop's back-edge gcc can no longer prove
`&manager->freeListHead`'s value on iteration N+1 is the same value
computed on iteration N (the "proof" would have to cross the opaque
barrier), so the whole hoist is defeated - without needing to hand-write
the address arithmetic, and without disturbing any of the *other*,
already-correct per-use address computations elsewhere in the same
function (`&manager->gridHead[255]`, `&manager->gridTail[255]`), which
aren't inside any loop and were never hoisting candidates to begin with.

Letting gcc regenerate the address computation itself (rather than
writing raw inline asm for it) also has a second benefit: it lands back
in the *same* shared literal-constant pool the ROM's own build uses -
`0x814`/`0x40c`/`0x80c`, grouped together, in that exact order, right
after the function's fall-through return path. An earlier attempt that
hand-wrote the `ldr`+`add` pair via inline asm (using the assembler's
own `ldr rd, =0x814` pseudo-op) produced the right *instructions* but
the *wrong pool placement* - GNU `as`'s automatic literal-pool placement
put the `0x814` word in its own pool at the very end of the object
instead of joining the other two, which is functionally fine but not
byte-identical (different `ldr [pc, #imm]` offsets, different overall
function layout). Going back to a plain C address expression (just
barriered against hoisting) let gcc's own literal-pool grouping logic -
which already handled the other two constants correctly - handle this
one the same way.

### Gap 2: a small, unrelated register-choice gap

Once gap 1 was closed, a second, much smaller gap showed up in the
`data->flags` bit-4 test (`if (!((*(data+0xc) >> 4) & 1)) return;`).
The ROM's sequence:

```
ldrb  r1, [r5, #0xc]
lsrs  r0, r1, #4
movs  r1, #1
ands  r0, r1
cmp   r0, #0
```

keeps the shift result in `r0` (matching the AND's destination), with
the mask constant loaded into `r1`. This compiler's default codegen for
the same C expression instead threaded everything through a single
register (`r0` for the load, reused in place for the shift, `r1` for the
constant, then `ands r1, r0` - result left in `r1`, the constant's
register, instead of `r0`). Fixed the usual way, pinning each
intermediate value to the register the ROM actually used:

```c
register u8 flags asm("r1") = *((u8 *)data + 0xc);
register s32 shifted asm("r0") = flags >> 4;
register s32 mask asm("r1") = 1;
register s32 result asm("r0") = shifted & mask;

if (!result) {
    return;
}
```

## Verification

Isolated-compile iteration first: `cpp` + `tools/agbcc/bin/agbcc` on a
scratch copy of the function, assembled with `arm-none-eabi-as -mcpu=arm7tdmi
-mthumb-interwork -I asminclude`, and compared byte-for-byte (via
`objcopy -O binary --only-section=.text` + `cmp`) directly against the
ROM's own raw bytes (assembled from the pre-integration
`asm/code_3_2_13_9150.s`) - identical. Then, after integrating into
`src/graphics/actor_part11g.c` and updating `ldscript.txt`/
`tools/report_units.py`, a full clean `rm -rf build && make
NON_MATCHING=1 report` followed by `objdiff-cli report generate`
(100.0%), then a full clean `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` -
`crashbandicootxs.gba: La suma coincide`.

## File/link-order change

`sub_8009150` moves from an `#if NON_MATCHING` block inside
`src/graphics/actor_part11.c` (whose real bytes, while parked, lived in
the standalone `asm/code_3_2_13_9150.s`, guarded by `.if NON_MATCHING ==
0`) into its own new `src/graphics/actor_part11g.c` - its real ROM
address isn't adjacent to `actor_part11.c`'s own matched functions
(`sub_8008DC0`-`sub_8008EE4`), sitting instead between the NAKED
`sub_8009008` (`actor_part11b.c`) and `sub_80091D4` (`actor_part11c.c`),
per `docs/workflow.md` step 4's "needs its own new `.c` file" case.
`asm/code_3_2_13_9150.s` held only this one function and is retired
entirely; `ldscript.txt`'s `code_3_2_13_9150.o` line is replaced with
`actor_part11g.o` at the same link-order position. The `struct
pool_manager` definition that block relied on is still needed by the
three other `#if NON_MATCHING` blocks remaining in `actor_part11.c`
(`sub_8009914`/`sub_800944C`/`sub_8009528`), so it stays there too,
duplicated (matching this codebase's existing convention of a
per-translation-unit local copy of shared structs like this one - see
also `actor_part12.c`'s own copy).

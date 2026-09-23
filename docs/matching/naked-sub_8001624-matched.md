# `sub_8001624` converted from NAKED transcription to real matched C

`sub_8001624` (`src/graphics/aabb_util.c`) had been parked as a
byte-correct NAKED asm transcription since an early pass - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

The function commits `gUnknown_03001280`'s shadow copy to the real
blend hardware registers: a word write to `REG_BLDCNT`/`REG_BLDALPHA`
(`0x04000050`), then a halfword write to `REG_BLDY` (`0x04000054`,
i.e. the same pointer plus 4). The ROM does the pointer increment as
its own separate instruction:

```
str  r0, [r2]
add  r2, #4
...
strh r0, [r2]
```

but this compiler's peephole optimizer always recognizes a plain C
`*(vu32 *)addr = word; addr += 4;` (or any equivalent phrasing - a
separate statement, a memory-clobber barrier between the two, a fresh
pointer variable) as a store-then-increment-same-register pair and
fuses it into a single `stmia r2!, {r0}` (store-multiple with
writeback) instead - a different (functionally equivalent, but
byte-different) Thumb instruction.

## The fix

Same idea as [`sub_8001524`'s fix](./naked-sub_8001524-matched.md):
the peephole pass only fires on instruction pairs the compiler itself
generated from C source. Emitting the store and the increment as one
opaque inline-asm block removes them from its view entirely:

```c
asm volatile("str %1, [%0]\n\tadd %0, %0, #4" : "+r"(bldReg) : "r"(word));
```

The `"+r"` constraint on `bldReg` both reads its incoming value and
writes the incremented result back into the same C variable, so the
subsequent `REG_BLDY` write uses the advanced pointer.

A second, unrelated gap surfaced once the fusion was defeated: the
`bldy` byte's low-5-bits mask (`& 0x1f`) compiled as a plain
AND-immediate here, where the ROM computes it via a left-shift/
right-shift pair (`lsl r0, r1, #0x1b; lsr r0, r0, #0x1b`) - the
established "shift and shift back" idiom already used elsewhere in
this project to reproduce the ROM's own bit-width-truncation style.
Writing the mask as `(bldy << 27) >> 27` instead of `bldy & 0x1f`
reproduces it.

Final C:

```c
void sub_8001624(void)
{
    register vu32 *bldReg asm("r2") = (vu32 *)0x04000050;
    register struct unk_03001280 *src asm("r1") = &gUnknown_03001280;
    register u32 word asm("r0") = src->bldcntAlpha;
    register u32 bldy asm("r1");
    register u32 masked asm("r0");

    asm volatile("str %1, [%0]\n\tadd %0, %0, #4" : "+r"(bldReg) : "r"(word));

    bldy = src->bldy;
    masked = (bldy << 27) >> 27;
    *(vu16 *)bldReg = masked;
}
```

This reproduces the ROM's instruction sequence and register choices
one-for-one: `ldr, ldr, ldr, str, add, ldrb, lsl, lsr, strh, bx`.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, then full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide`. `sub_8001624` is folded into the same
`src/graphics/aabb_util.o` unit as the already-matched
`sub_8001640`-`sub_80016DC` siblings in `tools/report_units.py`, since
it's the same object file.

# `sub_80010E0` converted from NAKED transcription to real matched C

`sub_80010E0` (`src/system/input_util.c`) had been parked as a
byte-correct NAKED asm transcription - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

The count-limited loop has (logically) three checks per iteration:
poll input, then (if `checkButtons`) test the confirm bit and the
cancel bit. The confirm-bit test - `if ((keys & 1) != 0) goto done;`
- always compiled here as:

```
cmp r0, #0
bne done
b   continueTowardCancelCheck
```

while the ROM has the opposite branch sense:

```
cmp r0, #0
beq continueTowardCancelCheck
b   done
```

Same two instructions, same size, just inverted - and every rephrasing
tried (positive/negative sense, if/else, nested vs flat, goto-only
chains) produced the *same* `bne`-first output, regardless of source
phrasing. The original writeup concluded this was a fixed gcc-2.9
canonicalization for the shape and parked it as NAKED.

## The real cause: block order, not condition polarity

The compiler's canonicalization isn't actually about which condition
is tested - it's about **source block order**. Every prior attempt
kept the cancel-bit check written *after* the confirm-bit check in the
C source (the natural top-to-bottom reading order: poll, check
confirm, check cancel, increment). For that source order, this
compiler always lays out the "continue" path as a forward fallthrough
and the "exit" path as the explicit branch, regardless of how the
condition itself is spelled - hence identical output no matter how the
`if` was rephrased.

The ROM's own basic-block layout is different: the cancel-check code
is placed *before* the increment/poll code, with the confirm-check (at
the very end of the loop body) branching *backward* into it on a miss,
and falling through to an unconditional exit branch on a hit. Once the
C source is restructured to match that same block order - the
cancel-check written textually first, with the poll/confirm-check code
below it branching back via `goto cancelCheck` - this compiler
reproduces the ROM's exact `beq cancelCheck; b done` pair without any
further coaxing. The *unlimited*-loop variant right below never had
this problem because it never has a separate cancel-check block to
place - the confirm and cancel checks run back-to-back with no
loop-back edge between them.

## The fix

```c
cancelCheck:
    keys &= 8;
    if (keys != 0) {
        goto fail;
    }
increment:
    i++;
checkCount:
    if (i >= count) {
        goto done;
    }
    sub_80006A8();
    sub_80007AC(gUnknown_03001304);
    addr = &gUnknown_030007E0;
    asm volatile("add %0, %1, #0" : "=r"(keys) : "r"(mask));
    keys &= *(u16 *)((u8 *)addr + 2);
    if (flagR == 0) {
        goto increment;
    }
    confirm = keys & 1;
    if (confirm == 0) {
        goto cancelCheck;
    }
    goto done;
```

`keys` is deliberately left unmasked by the confirm check (the test
result goes into a separate `confirm` local) since `cancelCheck` needs
`keys`'s original `mask & newlyPressed` value, matching the ROM's own
register reuse (`and r0, r1` for the confirm test writes to r0, never
touching r1's `keys` value).

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, then full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide`. `tools/report_units.py`'s entry now points at the real
`src/system/input_util.o`.

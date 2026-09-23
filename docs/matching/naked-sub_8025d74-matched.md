# `sub_8025D74` converted from NAKED transcription to real matched C

`sub_8025D74` (`src/system/game_loop15.c`, a BG-scroll-layer hardware-
register/bitfield initializer) had been parked as a byte-correct NAKED
asm transcription - see
[issue-41-game-loop-25894.md](./issue-41-game-loop-25894.md) for the
original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gaps

The function initializes a BG-scroll-layer object: caches a static
data pointer, a packed affine-ish constant, two hardware register
addresses (`BGnCNT`/`BGnHOFS`, computed from `bgIndex`), and two
bitfields via `(byte & mask) | value`-style read-modify-write on two
adjacent bytes. Two independent gaps stood between a plain C
reconstruction and a byte-exact match:

1. **Negative-mask constant folding** (same class as `sub_8001524`/
   `sub_80109A4`): `*addr35 & -0x20` and `*addr34 & -0xd` always
   compile to their positive byte-immediate equivalent (`& 0xe0`,
   `& 0xf3`) here, since this compiler recognizes `-0x20`/`-0xd` are
   representable as an 8-bit AND-immediate and folds them - where the
   ROM computes each mask at runtime (`movs r0,#0x20; rsbs r0,r0,r0`
   / `movs r0,#0xd; rsbs r0,r0,r0`, i.e. `mov`+`neg` in Thumb's plain
   syntax) and does a genuine register-register `AND`.
2. **Shared literal pool identity**: the function loads three
   pointer-sized constants (`gStaticData_087E4C14`, `0x04000008`,
   `0x04000010`) that the ROM keeps in one pool, in that exact order,
   right after the function. Letting even one of the three stay an
   ordinary C-level reference (relying on this compiler's own
   automatic literal pooling) makes that one constant's register
   choice and the *other* two's relative pool offsets sensitive to
   exactly how many other constants the compiler happens to discover
   nearby - any small change elsewhere in the function shifts them out
   of step with each other and with the ROM's fixed layout.

## The fix

### Gap 1: opaque inline-asm mask materialization

Same technique as `sub_8001524`/`sub_80109A4`: each mask fold becomes
one `asm volatile` block computing the mask via `mov`+`neg` and doing
the AND as a real instruction, opaque to the constant-folding pass
that would otherwise recognize and fold it:

```c
asm volatile(
    "mov r0, #0x20\n\t"
    "neg r0, r0\n\t"
    "ldrb r4, [%0]\n\t"
    "and r0, r0, r4\n\t"
    "orr r0, r0, %1\n\t"
    "strb r0, [%0]\n\t"
    : : "r"(addr35), "r"(t) : "r0", "r4", "memory"
);
```

The byte load (`ldrb`) is *inside* the asm block, not a separate C
statement feeding the asm an input operand - letting the compiler
schedule an ordinary C-level `byte = *addr;` load independently
produced the right bytes in isolation but a different (still correct,
just differently-scheduled) instruction order once compiled alongside
the rest of the real file, since the scheduler had more surrounding
code to reorder against. Folding the load into the same opaque block
as the mask computation removes that degree of freedom entirely.

### Gap 2: one function-owned literal pool

All three constant loads are materialized as their own tiny asm
blocks, referencing one explicit trailing pool this function owns
outright (declared via a top-level `asm(...)` right after the
function, in the exact order the ROM has them) instead of any of them
going through the compiler's own automatic pooling:

```c
{ register void *gsPtr asm("r0");
  asm volatile("ldr %0, 90f" : "=r"(gsPtr));
  *(void **)(s + 0x30) = gsPtr; }
...
{ register s32 bgnCntAddr asm("r3");
  asm volatile("ldr %0, 90f+4" : "=r"(bgnCntAddr));
  *(s32 *)(s + 0x38) = shifted1 + bgnCntAddr; }
...
{ register s32 bgnHofsAddr asm("r0");
  asm volatile("ldr %0, 90f+8" : "=r"(bgnHofsAddr));
  idx = idx + bgnHofsAddr; }
```

```c
asm(".align 2, 0\n90: .word gStaticData_087E4C14\n.word 0x04000008\n.word 0x04000010");
```

(An earlier attempt materializing only the `0x04000008` load - the one
constant referenced just once in the whole function, versus the other
two which each have a second plain-C use elsewhere - removed it from
the compiler's own pool entirely without removing the *other* plain-C
references, shrinking the auto-generated pool by one slot and making a
hardcoded pool offset for a *different* load silently point at the
wrong constant. Materializing all three together, against one pool
this function fully owns, avoids that class of mistake structurally.)

Since the pool is emitted via a *separate* top-level `asm()` statement
after the function's closing brace (not inside the function body,
where an unreachable trailing block would be dead-code-eliminated -
see the `sub_8000CBC` derivation doc for that failure mode), it falls
outside the compiler's own `.size sub_8025D74, ...` calculation for
the function symbol - purely a symbol-table/metadata detail with no
effect on the actual emitted bytes (confirmed by the full clean
`make compare` below), but visible as a handful of "extra" bytes
`objdiff`'s per-symbol diff view attributes to no symbol at all rather
than to `sub_8025D74`.

**A genuine assembler-syntax bug found along the way**: an early draft
wrote `orr r0, r0, #8` directly, which isn't valid Thumb (`ORR` has no
immediate-operand form at all) - `arm-none-eabi-cpp | agbcc` happily
emitted the (invalid) text into the `.s` file with no complaint, since
that pipeline never runs the assembler; the mistake was only caught by
actually invoking `arm-none-eabi-as` and hitting `Error: unshifted
register required`, not by the isolated-compile step this project
otherwise leans on for fast iteration. Fixed by loading `8` into a
register first (`mov r1, #8; orr r0, r0, r1`), matching the ROM's own
instruction shape.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report` succeeds.
`objdiff-cli report generate` succeeds (no symbol-pairing errors).
`objdiff-cli diff` against `build/expected/units/game_loop15_target.o`
shows every real instruction matching one-for-one (the only reported
diff is the trailing-pool/symbol-size artifact described above, not a
byte content difference - confirmed by direct `arm-none-eabi-objdump`
comparison of the assembled instruction bytes). Full clean `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map
&& make compare` - `crashbandicootxs.gba: La suma coincide`.
`sub_8025D74` is folded into the same `src/system/game_loop15.o` unit
as the already-matched
`sub_8025DE8`/`sub_8025E2C`/`sub_8025E70`/`sub_8025E84` in
`tools/report_units.py`, since it's the same object file.

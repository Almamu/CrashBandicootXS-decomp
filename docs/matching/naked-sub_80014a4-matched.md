# `sub_80014A4` converted from NAKED transcription to real matched C

`sub_80014A4` (`src/graphics/fade_screen_mode.c`, the fade-to-black
palette DMA loop) had been parked as a byte-correct NAKED asm
transcription since an early pass - see
[naked-transcription-parked-functions.md](./naked-transcription-parked-functions.md)
for the original parking rationale. It's now genuinely matched as real
decompiled C.

## The original gap

The function backs up the real palette into `gUnknown_03000A80`, then
for each blend factor 0/2/.../16 blends it toward black into
`gUnknown_03000E80` and DMAs that into the real palette. The ROM's
per-iteration DMA setup:

```
str r6, [r4]           ; src = gUnknown_03000E80 (cached across the loop)
mov r3, #0xa0
lsl r3, r3, #0x13
str r3, [r4, #4]          ; dst = 0x05000000 (recomputed fresh every iteration)
ldr r2, 4f
str r2, [r4, #8]             ; cnt = 0x80000200 (reloaded fresh from the pool every iteration)
ldr r0, [r4, #8]
```

caches the blended-buffer address (`gUnknown_03000E80`) in a register
across the whole loop, while recomputing the other two DMA fields
(`0x05000000`, `0x80000200`) fresh every iteration, plus does an extra
"rename" copy of the DMA register pointer (`add r4, r1, #0`) right
before the loop starts. A plain C reconstruction (`struct dma_regs
*dma = ...; for (...) { dma->src = ...; dma->dst = 0x05000000; dma->cnt
= 0x80000200; }`) always got this compiler's loop-invariant-hoisting
pass to cache *all three* fields once introducing a local for the
buffer address got it cached at all - "every combination tried moves
the line but never lands on the ROM's exact split" (the original NAKED
write-up's own words).

A second, related gap: after the loop, the ROM's post-loop DMA setup
reuses the loop's *own last-iteration register values* for the dest/
count fields instead of recomputing them:

```
ldr r0, 2f
ldr r1, 3f
str r1, [r0]
str r3, [r0, #4]        ; reuses r3 from the loop's last iteration
str r2, [r0, #8]           ; reuses r2 from the loop's last iteration
ldr r0, [r0, #8]
```

## The fix

Combines register-pinned locals matching the ROM's own register roles
(including the "rename" copy) with inline-asm-materialized DMA-field
writes for the two fields the ROM keeps fresh - the same "opaque to
the optimizer" technique used on `sub_8001524`/`sub_8001624`/
`sub_8000CBC` - but threading the asm's own computed values back out as
real C operands (`dstVal`/`cntVal`, pinned to r3/r2 matching the ROM)
so the post-loop block can reuse them as genuine inputs instead of
recomputing:

```c
void sub_80014A4(void)
{
    register struct dma_regs *dma asm("r1");
    register struct dma_regs *dma2 asm("r4");
    register s32 factor asm("r5");
    u32 val;
    register u32 *bufAddr asm("r6");
    register u32 dstVal asm("r3");
    register u32 cntVal asm("r2");

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = 0x05000000;
    dma->dst = (u32)gUnknown_03000A80;
    dma->cnt = 0x80000200;
    val = dma->cnt;

    factor = 0;
    dma2 = dma;
    bufAddr = (u32 *)gUnknown_03000E80;

    do {
        sub_80013FC(factor);
        sub_80006A8();
        dma2->src = (u32)bufAddr;
        asm volatile(
            "mov %0, #0xa0\n\t"
            "lsl %0, %0, #0x13\n\t"
            "str %0, [%2, #4]\n\t"
            "ldr %1, .L8+0x8\n\t"
            "str %1, [%2, #8]\n\t"
            "ldr r0, [%2, #8]\n\t"
            : "=r"(dstVal), "=r"(cntVal) : "r"(dma2) : "r0", "memory");
        factor += 2;
    } while (factor <= 0x10);

    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;

    {
        register struct dma_regs *dma3 asm("r0");
        asm volatile(
            "ldr %0, .L8\n\t"
            "ldr r1, .L8+0x4\n\t"
            "str r1, [%0, #0]\n\t"
            "str %1, [%0, #4]\n\t"
            "str %2, [%0, #8]\n\t"
            "ldr %0, [%0, #8]\n\t"
            : "=r"(dma3), "+r"(dstVal), "+r"(cntVal) :: "r1", "memory");
    }
}
```

Several details, each load-bearing:

- **`do { } while`, not `for`**: a `for (factor = 0; factor <= 0x10;
  ...)` loop got gcc to emit a redundant zero-trip guard test before
  the loop (`cmp r5,#0x10; bgt <end>`) that the ROM doesn't have, since
  gcc couldn't prove the loop always executes at least once from that
  shape. A `do/while` (matching the fact the loop body always runs at
  least once here) skips that guard entirely.
- **The register-pinned "rename" copy (`dma2`, pinned to r4)**: the
  ROM's own `add r4, r1, #0` right before the loop is reproduced by
  simply *having* a second, separately-pinned register-copy variable
  the loop body uses instead of the original `dma`.
- **`.L8+0x8` (and `.L8`, `.L8+0x4`) hardcode this function's own
  compiler-generated literal-pool label.** This is intentionally
  fragile and documented in the source: since `0x80000200` is also
  referenced via plain C at the top of the function, gcc already pools
  it once: referencing that *same* pool slot from inside the asm block
  (rather than a fresh `"i"`/`"=CONST"` operand, which would create a
  *second*, duplicate literal and cost 4 extra bytes) keeps the byte
  count and pool layout identical to the ROM's single shared entry.
  This only works because the plain-C reads of these constants
  (`REG_ADDR_DMA3SAD`, `gUnknown_03000A80`, `REG_BLDCNT`'s address at
  `.L8+0x10`) are still present in the source - removing any of them
  would remove that literal from the pool and break the offset.
- **`REG_BLDCNT`/`REG_BLDY` stay plain C, deliberately not folded into
  asm too**: this is what keeps `REG_ADDR_DMA3SAD+0x10` (i.e.
  `REG_ADDR_BLDCNT`, `0x04000050`) genuinely referenced by the
  compiler, so it lands in the pool at `.L8+0x10` the way the ROM's
  own compiler put it - the *sole* purpose of the final asm block is
  to stop gcc from noticing it can derive `REG_ADDR_DMA3SAD` cheaply
  from the just-computed `REG_ADDR_BLDY` address (`REG_ADDR_DMA3SAD ==
  REG_ADDR_BLDY + 0x80`) via `add r1, r1, #0x80` - a real, shorter
  instruction sequence this compiler prefers over a fresh pool reload,
  but not what the ROM does.

## Verification

Full clean `rm -rf build && make NON_MATCHING=1 report`, `objdiff-cli
diff` against `build/expected/units/fade_screen_mode_target.o` for
`sub_80014A4`: 100% match. Full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. `sub_80014A4` is
folded into the same `src/graphics/fade_screen_mode.o` unit as the
already-matched `sub_8001510` in `tools/report_units.py`, since it's
the same object file.

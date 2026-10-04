# Inline-argument-order retry

This pass retried four drafts left by the hard-register hold passes
([hard-register-hold-retry.md](hard-register-hold-retry.md),
[issue-9-raw-asm-pass.md](issue-9-raw-asm-pass.md) "Hold pass"):
`DrawPlayer` (#9), `sub_802062C` (#31), `DrawPauseFraction` (#7) and
`sub_8002E20` (#5). One closed.

## Closed (1)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `DrawPlayer` | `src/graphics/actor_part111.c` (already old_agbcc) | old_agbcc | Was 40 halfwords off. The orbit tail now passes its two position sums straight in as arguments to a small inline setter (see below). That gives the ROM's order in the tail, and the `&82C`/`&81C` r4/r5 swap went away with it. The two r6 holds from the hold pass stay. |

## New technique: sums passed to an inline are expanded late

gcc 2.x expands every argument of an inline call before it copies any
of them into the parameter registers. An argument that is a sum
(`EXPAND_SUM`) comes back unforced, as `(plus (mult reg 16) (mem addr))`,
after only its address arithmetic and its non-deferrable parts have
been emitted. Here those parts are the `ldrsh` table reads. The multiply,
the MEM load and the add are only emitted when the value is copied into
its parameter, one argument after another, after the earlier arguments
(here the `child` MEM) have been loaded.

So when the ROM computes addresses early but loads from them late, or a
`lsl` sits far from the `ldrsh` it applies to, try passing the whole
expression to an inline instead of computing it in locals. In
`DrawPlayer`:

```c
static inline void SetChildPos(struct box_part *child, s32 x, s32 y)
{
    child->x = x;
    child->y = y;
}
...
SetChildPos(self->child,
            self->hist[idx].x + gStaticData_0816A820[gRoomFrameCount & 0xff] * 16,
            self->hist[idx].y + gStaticData_0816A820[(gRoomFrameCount >> 1) & 0xff] * 8 - 0x1800);
```

The spelling matters:

- `hist.x + tbl * 16` matched. `tbl * 16 + hist.x` and
  `hist.x + (tbl << 4)` were 85 halfwords or more off. `EXPAND_SUM`
  leaves a `MULT` unforced, but a shift is always emitted at once.
- `(x, y, child)` parameter order missed.
- The same sums in locals or written straight into `child->x`/`->y`
  without the inline were 225+ halfwords off.
- `const` on the parameters makes no difference.

## Not closed (3)

| Function | Before | Now | What was tried / what's left |
|---|---|---|---|
| `sub_802062C` (#31, `graphics_loading_1feec.c`) | 62 | 62 (draft unchanged) | In the ROM, `part+0x28` gets r3 through **caller-save**: `str r3,[sp]` right before `bl sub_8008E94`, restored lazily before the flip. old_agbcc does this, as checked on a test file. That needs r4 (arg3) and r5 (`hdr+0x84`) to be taken first. In the draft, local-alloc gives `&gEntityFlags` r5 and arg3 r4. Reload later moves arg3 to r9 and spills `arg3 << 16` to the stack. Zero-length r4/r5 holds, brute-forced over every statement boundary (about 150 placements), got arg3 into r4 and `hdr+0x84` into r5. Each time, though, `part+0x28` went to r8 instead of r3, and part/hdr swapped r6/r7 (80+ hw). Pinning the table pointer to r9 as well didn't help (87+). |
| `DrawPauseFraction` (#7, `settings_menu16.c`) | 73 | 73 (draft unchanged) | The second half's `r6 = r7; r7 += 4` is postreload `reload_cse`/move2add. Before reload, the second half loads fresh 0x110/0x114 constants into r6/r7 while r7 still holds the first half's 0x110. So the second half's constants must not be CSE'd with the first half's 0x110 pseudo. None of these gave that: `do { } while (0)` on each draw (8 combinations), fresh or shared `"=r"/"0"` offsets per field and half (81 combinations; best 56 hw, 8 bytes long), or passing the sums to the position setter as above. |
| `sub_8002E20` (#5, `settings_menu8a2.c`) | 104 | 104 (draft unchanged) | The ROM builds both channel addresses as `(idx * 0xc8 + s) + K` with its own `muls`. Inline accessors (`&s->rx[i]` with and without the global), byte-offset forms and a re-read global all still CSE the product (100-121 hw). Not pursued further, because of the time budget. |

Helper scripts (not committed) are in the session scratchpad's `hold2/`:
`var.py` (parallel variant runner), `d.py`, `rtl.sh`/`fnrtl.py`, and
the variant specs `s1.py`-`s10.py`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

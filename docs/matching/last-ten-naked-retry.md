# Last-ten NAKED retry

This pass retried the three drafts left by
[last-nine-naked-retry.md](last-nine-naked-retry.md). One function
closed and one draft got much closer.

| Function | File | Before | Now | Status |
|---|---|---|---|---|
| `DrawPauseFraction` (#7) | `src/graphics/settings_menu16.c` | 14 | match (both compilers) | **Closed** |
| `sub_8001DB4` (#4) | `src/system/link_cable_01db4.c` (split) | 136 | 51 (same size, old_agbcc + `-fno-rerun-loop-opt`) | Draft updated |
| `sub_802062C` (#31) | `src/graphics/graphics_loading_1feec.c` | 4 | 4 | Note added |

## `DrawPauseFraction`: closed

Last-nine kept the second half's posX offset in an opaque `OFF(0x110)`
pseudo. That pseudo was the problem. The ROM's second half has no 0x110
pseudo at all:

- Reload loads the constant for each use.
- The first reload register is r6 (reload_cse turns it into
  `adds r6, r7, #0`), and the second use inherits it.
- The 0x114 reload then goes to r7 (move2add's `adds r7, #4`).

With plain field accesses the second half comes out as in the ROM:

```c
{
    struct icon_manager *e = *pe0;

    set_icon_mgr_pos(*pdc, get_icon_mgr_posx(e) - 5, get_icon_mgr_posy(e) + 8);
}
```

`get_icon_mgr_posx`/`get_icon_mgr_posy` are inline getters and
`set_icon_mgr_pos` is an inline `posX`/`posY` setter. The getters put
both loads ahead of the `*pdc` load. A direct `e->posX - 5` argument is
69 halfwords (8 bytes long). The first half is unchanged from last-nine.
The `OFF`/`AT` macros are gone, and so are all the asm statements.

Things that didn't help (before the fix): hard-register holds on
r0-r3 around `ox`'s birth (14-74 hw), a `u32 x` local for the second
posX, and making `ox` global with a volatile `"+r"` tail.

## `sub_8001DB4`: 136 to 51

**Split.** `-fno-rerun-loop-opt` changes the matching `sub_8002114`, so
`link_cable.c` is now three objects, all still on `OLD_AGBCC_OBJS`:

- `link_cable.c`: `sub_8001CB8`, `LinkStop`
- `link_cable_01db4.c`: `sub_8001DB4` only
- `link_cable_01f50.c`: `sub_8001F50`, `sub_8002114`

The structs moved to `include/link_session.h`. The Makefile's new
`NO_RERUN_LOOP_OPT_OBJS` list gives `link_cable_01db4.o` the flag. The
NAKED body doesn't depend on it; the flag is there for the draft.

**Draft.** Under old_agbcc with the flag, what brought it from 136 to 51:

- **First loop.** It is written over one pointer (`p[9]`/`p[8]` read,
  `p[0]`/`p[1]` written) with its own counter `k`. That gives the
  ROM's reversed loop. Sharing `i` with the outer loop moved every
  register.
- **Inner copy loop.** It counts up (a single loop pass doesn't reverse
  it), with the destination as
  `((struct link_player *)((u8 *)self + 8))[i + 1].id[j * 2]`. That form
  gives the ROM's `i + 1` precompute and its per-pass `self + 0xd0`.
- **Source pointer.** `id = self->id` is passed to `sub_8001CB8` and
  copied into a `src` local inside the outer loop. Loop motion moves the
  copy out. That is the ROM's `str r4, [sp, #4]` after the hoisted
  invariants.
- **field_30.** It goes through `s32 *f30 = &self->players[0].field_30`
  plus an `s32 t = i * 0xc8` offset. The base is hoisted (`[sp]`) and
  the add is `f30 + t`.
- **Tail.** `field_400` is stored through a `u16 *` and read back
  through `vu16`, which gives the ROM's re-read in its order.
- **Extra references.** 13 `asm("" : : "r"(id))` lift `id`'s
  global-alloc priority (15 refs over 41 insns, 1.10 → 1.56) above
  `self`'s (34 over 151, 1.13). `id` then gets r4 and `self` r5.
- **Padding.** One bare `asm("")` after the nibble decrement breaks an
  exact priority tie. `self + i * 0xc8` has 8 refs over 24 insns and the
  nibble pointer 6 over 12. The extra insn makes the pointer win, so it
  gets r4 and the base ip, as in the ROM.

Left:

- **Base order.** `self + i * 0xc8` for the nibble/magic block comes
  out as `adds r0, r2, r5`; the ROM has `self` first. The expander
  builds it as `(plus (mult i 200) self)`. Nibble-address spellings
  (pointer arithmetic, `players + i`, the `[i + 1]` form) don't change
  it. Writing the magic stores through pointer arithmetic moves the
  loop (+4 bytes).
- **0x1234.** The ROM reloads 0x1234 for the second store
  (`adds r2, r1, #0; strh r2`), so its value had no register. Here one
  pseudo keeps it in r1. Twelve spellings didn't change that:
  chained, separate, casts, u16/u32 locals, `"+r"` escapes, inline
  pair setters. The pointer-based ones also break the loop.
- **Tail registers.** The tail's reload registers (r1/r2/r3 where the
  ROM has r3/r7/r2) probably follow from the two above.

Without the flag the same draft is 142 halfwords (8 bytes long), even
with an `asm` use of `j` to block the inner loop's reversal.

## `sub_802062C`: notes only

Old_agbcc does do caller-saves. A test function with every callee-saved
register held puts a pointer in r1 with `str r1, [sp]` after the
argument setup, which is the ROM's shape. But in a single part+0x28
pointer draft here, the caller-save never survives:

- global.c's order puts the pointer after `hdr`/`part`. `arg3` and
  `&gEntityFlags` are block-local in r4/r5 and conflict with it,
  and an `"l"` reference restricts it to LO_REGS. So it goes to the
  caller-save path and gets r3.
- Reload then spills r3 for the first flip's `ldrb` reload (insn 252).
  retry_global_alloc moves the pointer to r5 and `&gEntityFlags`
  to r9.
- The output is identical with `-fno-caller-saves`.

In the ROM the first flip's reloads use r5 instead (`mov r5, r8`,
`ldrb r5, [r3]`), so r5 must have ranked below r3 in
`order_regs_for_reload` there. The 4-halfword draft is unchanged.
Variants tried (`c3.py` in the helpers): 0-2 `"l"` references,
0/2/4 references on the `hdr + 0x84` pointer, rec2 references, the r9
hold, and a pointer reference. All were 62-140 hw.

## Helpers

These are in the scratchpad's `last10/`:

- `d.py`/`var.py`/`rtl.sh`, as before. `d.py` passes `-f...` flags
  through to the compiler.
- `regs.sh`: local and global register dispositions per variant.
- `l3.py`: the composable sub_8001DB4 variant generator. Filter it with
  `FLT='key=a|b,...'`.
- `w.c` and `w1.py`: the 51-hw sub_8001DB4 base and later experiments.
- `h5.py`/`h6.py`: the DrawPauseFraction spellings that matched.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

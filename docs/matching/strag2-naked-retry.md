# strag2 retry: issue #62's raw pair and issue #18's NAKED four

This pass covered six unfinished functions that no open issue tracked:

- Two still-raw functions from issue #62 (closed too early), parked back
  in PR #216.
- Four NAKED transcriptions from issue #18's `actor_part38` family, in
  the confirmed old_agbcc range.

**5 of 6 closed.** All five match under both agbcc and old_agbcc.

## Closed

| Function | File | Was | Technique |
|---|---|---|---|
| `sub_80339DC` | `actor_part29.c` | raw asm (99 hw off) | real `/` via `__divsi3` alias (libcall keeps `self+0x24` CSE'd across it), divisor fixed to `-0x1AA`, one shared `store:` label for the new cooldown, a stored `count` local, `dx` then `dy` statement order, `"=r"`/`"0"` `u8` zero |
| `sub_8033CF8` | `actor_part35.c` | raw asm (102 hw off) | same as `sub_80339DC`, plus `"=r"`/`"0"` escapes for the reset block's `0`/`2`, the `2` via `asm volatile` so it isn't sunk |
| `sub_80156EC` | `actor_part38c.c` | NAKED | `u8 *self` parameter instead of a `selfArg` copy that GCSE left in the `else` arm |
| `sub_80152F0` | `actor_part38b.c` | NAKED | `u8` locals for the table indices so they are loaded before the stores (moves `mode` to `r4`) |
| `sub_8015238` | `actor_part38b.c` | NAKED | real `u8 *`/`u8` params (fixes the entry-copy order); `0x200` built in `m`, copied by an `"=r"`/`"0"` escape, and a volatile `"+r"(flags)`/`"r"(m)` use right after the `and` (stops combine and regmove from retargeting it); pointer local for `self+0x29` |

Bookkeeping:

- Deleted `asm/code_3_2_20_28568_c99c_31784_339dc.s` and `..._33cf8.s`
  and their `ldscript.txt` lines. The C objects were already linked in
  ROM order.
- `actor_part29.o`/`actor_part35.o` stay on agbcc: both compilers give
  the same bytes, and they sit outside the known old_agbcc ranges.
- `actor_part38.o`/`38b.o`/`38c.o` joined `OLD_AGBCC_OBJS`. Each file's
  whole `.text` is identical under both compilers, and
  0x08013C60-0x0801EA5C is confirmed old_agbcc territory.

None of the parked notes' diagnoses held up:

- "An extra high-register relay pair" and "`r7` can't be pinned" were
  both caused by the wrong division form.
- The "parameter home-copy order" came from a `void *` parameter copied
  into a `u8 *` local.
- The "addressing-mode fold" was really constant-materialization order.

## Not closed

- **`sub_8015038`** (`actor_part38.c`, 400 bytes) is still NAKED
  (matched later in the strag4 retry, see `strag4-naked-retry.md`). It
  now has a C draft under `#if NON_MATCHING`, down from about 163
  halfwords off to 2 under both compilers.
- **What is left:** at the top of the `self+0x24 != 0` arm, the ROM does
  `adds r5, r0, #0; ldrb r2, [r5]`: it copies `self+0x22` into `r5`
  first, then loads through the copy. The draft loads through `r0` and
  copies afterwards. GCSE inserts the copy after the load.
- **What was tried:** explicit `p22` locals (function-scope or
  arm-scope), BOX-style `"+r"` escapes, and an `asm volatile` barrier.
  None moved it; the escapes break the rest of the arm.
- **What got the draft there:** `zero`/`wait` locals (the ROM's
  `sb`/`r4` constant pair across the calls), `off = *(u8 **)(self + 0xc);
  off += 0x50;` (drops an extra copy), and three no-code holds on `r0`,
  `r1` and `r2`. Those steer one reload register and two allocation
  choices. The holds are fine to keep in a final match.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`
  prints `crashbandicootxs.gba: OK`.

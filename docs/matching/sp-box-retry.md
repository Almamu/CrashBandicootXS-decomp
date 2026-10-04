# Stack-box NAKED retry

This pass looked at one blocker that three drafts share. gcc kept a
stack box's address (`&f.b`, `sp+N`) in a callee-saved register across
two or more box-builder calls. The ROM instead recomputes
`add r0, sp, #N` right before each call. Two of the three functions
now match as real C:

| Function | File | Issue | Compiler | Was | Now |
|---|---|---|---|---|---|
| `sub_800CD00` | `src/graphics/actor_part109.c` | #11 | old_agbcc | 42 | match |
| `sub_800D040` | `src/system/game_loop6.c` | #12 | old_agbcc | 151 | match |
| `sub_0800D18C` | `src/system/game_loop47.c` | #12 | old_agbcc | 968 | 938, still NAKED |
| `sub_800E08C` | `src/system/game_loop47.c` | #12 | old_agbcc | 49 | 49, still NAKED |

Each of `actor_part109.o` and `game_loop6.o` holds only its one
function, and both joined `OLD_AGBCC_OBJS`. Under current agbcc the
closed C is 25 and 129 halfwords off. Both objects end with
`asm(".align 2, 0")` because the ROM pads with zeros, not a `nop`.

## Where the shared pseudo comes from

The RTL dumps (`-da`) for `sub_800CD00` show what happens. At expand
time every `&f.b` is a separate pseudo holding
`(plus frame-pointer 16)`. cse1 then replaces each later one with the
first, since the value is still in that register. gcse copy-propagates
the `pb = &f.b` user variable into the same pseudo. One pseudo is then
live from the first builder call to the end, and it gets r6.

The ROM has a single-use pseudo for each builder argument. combine
folds each one into the `add r0, sp, #16` right before its `bl`, after
the x/y arguments. The only pseudo that lives across calls is born at
the first overlap test (`add r6, sp, #16`).

Flags don't give that: `-fno-cse-follow-jumps`, `-fno-cse-skip-blocks`,
`-fno-gcse` and `-fno-rerun-cse-after-loop`, alone or together, either
leave it as it was or change the function's size.

## The fix: an empty `"+r"` copy

```c
/* `a` through a copy that an empty asm claims to modify (emits nothing):
 * it hides the copy's value from cse, so each use of a stack box address
 * is its own pseudo instead of one held across calls. */
#define BOX_ADDR(a) ({ struct part_aabb *_p = (a); asm("" : "+r"(_p)); _p; })
```

Because the asm "modifies" `_p`, cse removes `_p` from the equivalence
class of `sp+16`. The class is left with no register, so the next
`&f.b` computes a new pseudo instead of reusing an old one. The asm
emits nothing, and combine still folds each copy into its call's
`add r0, sp, #N`.

The `asm("" : "=r"(p) : "0"(&f.b))` form doesn't work. Its input is
still `&f.b`, which cse replaces with the first pseudo, and that pseudo
then lives across the call again. The cast spelling
`(struct part_aabb *)((u8 *)&f + 16)` folds to the same RTL, so it
doesn't help either.

Three more things matter:

- **Every use needs the copy.** In `sub_800CD00` the macro goes on
  both player-box builder calls and on `pb = BOX_ADDR(&f.b)` at the
  first overlap test. Dropping any one of the three leaves 2 to 32
  halfwords.
- **Compute the first call's x/y before the call.**
  `{ s32 x = offX + px, y = offY + py; SetAabbPos(BOX_ADDR(&f.b), x, y); }`.
  Arguments are evaluated left to right, and the asm pins where the
  copy is computed. Written inline, the `add r0, sp, #16` comes before
  the x/y adds, while the ROM has it after them.
- **`sub_800CD00`'s record pointer.** In the second block the ROM puts
  `rec` in r1, the register of the `frame * 28` offset, not in r0, the
  register of the table base. local-alloc ties a block-local `rec` to
  the base operand. With one function-scope `rec` shared by the first
  two blocks, `rec` isn't block-local any more, so it goes to global
  alloc and lands in r1. Sharing it with the third block, or sharing
  `q` instead, breaks other blocks.

`sub_800D040` also needed `px`/`py` at function scope, shared by both
blocks as the ROM's r7/r8. With per-block `px`/`py` and the macro, it
was 45 halfwords off. With them shared, it matched.

## `sub_0800D18C`: 968 to 938, still NAKED

The first player box now matches the ROM. The builder calls use the
macro, and a `bb` local, set with `bb = BOX_ADDR(&f.b)`, holds the
box from `sub_8001688` to `sub_800CF70` in r4. The draft is still
exactly 3840 bytes.

The same fix on the rebuilt box (builder calls plus
`bb = BOX_ADDR(&f.b)` for its `sub_8001640`) also makes that block
match. It puts the `&gPlayer` temp in r6, as in the ROM, but
other low registers shift elsewhere and the draft comes out 8 bytes
short: a copy gets coalesced and the dead `ldr r1, [sp, #0x70]` is
dropped. The draft therefore uses it only on the first box. What's left
is low-register choice for constants and temps, and the `kind * 4`
spill slot order (0x94/0x98).

## `sub_800E08C`: 49, still NAKED

The only real difference is the case-3 read of the first flag. The ROM
has `mov r5, sp; ldrb r2, [r5]` from the flag's word-sized spill slot.
The draft has `ldr r2, [sp]`. Thumb's `zero_extendqisi2` only emits
`ldrb` if its operand is a MEM at expand time. It is also emitted if
reload turns a QImode subreg of a spilled pseudo into a MEM, which it
does because `sp` isn't a valid QImode base. Tried:

- **`s32`/`u32`/`u16`/`s8` flag with a `(u8)` cast, or `& 0xff`:** the
  flag's nonzero bits are known from the prologue `ldrb`, so the
  extension is dropped and the load is `ldr`.
- **The same with an `asm("" : "+r")` on the flag:** `ldr; lsl; lsr`,
  4 bytes long.
- **An addressable `s32` flag read as `*(u8 *)&f20`:** gives the
  `ldrb`, but the address goes into r0 before the argument moves, and
  the other two flags' prologue changes (+8 bytes).
- **Passing the packed struct, or calling through an unprototyped or
  `u8`-parameter function-pointer cast:** either no change or 4 to 12 bytes too long.

`game_loop47.o` stays on current agbcc. Both of its functions are still
NAKED, so the compiler choice doesn't matter yet. When one of them
closes, the file can simply move to `OLD_AGBCC_OBJS`.

## Tools

The scratchpad `spbox/` folder has these runners:

- `d.py SRC FUNC [old|agbcc] [-q] [-S] [-fflags]`: compiles with
  `NON_MATCHING=1` and prints the triage halfword count plus an
  alignment-aware instruction diff count (`insdiff`). It also prints the
  first differing offsets and a unified diff (`-S` masks registers).
- `var.py SPEC.py [names] [-v]`: a parallel variant runner. The spec
  gives `BASE`, `FUNC` and `VARIANTS = {name: [(old, new), ...]}`. Each
  pattern must occur exactly once, which avoids the `brute2.py`
  first-match caveat.
- `rtl.sh SRC.c`: dumps all RTL passes. `fnrtl.py` cuts one function
  out of a dump.
- `romdis.py ADDR SIZE`: disassembles the ROM with real addresses.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

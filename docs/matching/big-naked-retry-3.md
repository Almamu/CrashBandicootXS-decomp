# Third big NAKED retry: `SpawnRoomEntities`, `UpdateWumpa`, `HandleLinkSerial`

Three large NAKED functions. One closed. The other two now have C
drafts that are the same size as the ROM.

| Function | File | Issue | Size | Result |
|---|---|---|---|---|
| `SpawnRoomEntities` | `src/system/game_loop41.c` | #40 | 704 bytes | matched, old_agbcc (object added to `OLD_AGBCC_OBJS`) |
| `UpdateWumpa` | `src/pickups/wumpa_update.c` | #15 | 500 bytes | still NAKED; draft size-exact, 21 halfwords off (was 84 bytes too long) |
| `HandleLinkSerial` | `src/link/link_handshake.c` | #4 | 1488 bytes | still NAKED; first draft, size-exact, 514 halfwords off |

All three are measured under old_agbcc. `wumpa_update.o` and
`link_handshake.o` were already on `OLD_AGBCC_OBJS`.

## Loop rotation in old_agbcc

old_agbcc's `expand_end_loop` (stmt.c) moves a loop's exit test to the
bottom. It looks for the last jump to the loop's end label within 30
insns of the top. Unlike the gcc 2.95 in most references, this copy
does **not** stop at a nested loop's `NOTE_INSN_LOOP_BEG`. Two effects
decided most of the work here:

- A `break` in a search loop (`if (x->id == id) { found = 1; break; }`)
  is a jump to the end label, so the rotation moves the loop test *and
  the body up to the break* to the bottom. `duplicate_loop_exit_test`
  then copies all of that ahead of the loop, which gives a peeled first
  iteration. Where the ROM has a plain rotated loop, the search leaves
  with `goto` to a label just after the loop.
- An outer `do`/`while` is rotated too (its exit is a conditional jump
  around a `goto end`, so the "already ends in a condjump" check does
  not apply). `if (done) break;` near the top of the body is taken as
  the exit test and the body is laid out starting after it. Writing the
  rest of the body as `if (!done) { ... }` keeps the ROM's order.
- A `for (;;)` whose only jump to the end label is the last test
  (`if (missing) break; to = next;`) is rotated whole: the ROM's
  `b top; next: to = next; top: ...` shape.

## `SpawnRoomEntities` (219 -> 0 halfwords)

The draft was already the right size. The layout fixes are the three
loop points above. The rest:

- **The dead `mov r0, #0; cmp r0, #0` tests.** The ROM tests a "got an
  actor" flag after each search even though it is always 0 there. It
  is a flag set to 1 only on paths that jump straight to the move code.
  gcse proves it is 0 everywhere else and substitutes the constant, and
  reload then loads that into a register for the compare. For that the
  flag's `got = 0` must sit before the `for (;;)`, not at the top of
  each pass (then cse folds the test away).
- **`if (got && actor != NULL)`** at the move label lets the
  no-link-left exit (`if (missing) break;`) jump straight past the move
  code, as the ROM does.
- **No `continue` before `move:`.** loop.c's `find_and_verify_loops`
  moves a block that jumps out of a loop to just before the jump's
  target, if it finds a barrier at the target's loop depth. A
  `continue;` right before `move:` provided that barrier, and the
  `actor = a; goto move` block moved there.
- The link scan in the third pass is `m = 0; if (n > 0) do { ... }
  while (++m < n);`. The ROM tests `n <= 0` against a constant, which
  the plain `for` does not give.
- Each search has its own counter (`k`, `k2`, `k3`). With one shared
  `k` the counters and cached counts swap registers.
- `i` (the first link pass's counter) is declared at function scope.
  That puts it in the second spill slot, as in the ROM.
- The move call is `p.x = ...; pp->y = ...; SetEntityPos(actor, p.x,
  pp->y);` with `pp = &p`, the same shape as `platform_collide.c`. It
  gives the ROM's hoisted `add r4, sp, #4`.
- The DMA fills use a `u32 zero` local, so the 0 is loaded before the
  `list` compare.
- **One register pin.** In the first search of the third pass the ROM
  keeps the item pointer in `r0` and the id in `r1`. The id is a
  single-block temporary, so local-alloc gives it `r0` before
  global-alloc places the item pointer. `register u16 aid asm("r1")`
  gives the ROM's allocation. Declaration order, nudges, loop forms and
  `(*pl)` did not.

## `UpdateWumpa` (84 bytes long -> size-exact, 21 halfwords)

The three "flags |= 1, set the id bit" tails now merge the way the ROM
does (cross-jumping in jump2, which runs after reload, so the copies
must allocate identically):

- **Mode 3's 1.** The ROM reuses the spawn call's byte argument (`r5`,
  1) for `flags |= 1`, but loads a fresh 1 for the bit shift. The byte
  argument is now a plain `*(volatile u8 *)&argP5 = 1`, so its 1 is a
  QImode constant. cse matches it with the QImode 1 of `flags |= 1`
  but not with the SImode 1 of `1 << bit`. With the old inline
  (`OrbitArgByte`) the parameter was promoted to SImode and the shift
  used `r5` instead. An `asm` constant (brief item 10) also stopped the
  shift sharing, but the `orr` then tied its result to `r5`.
- **The phase test's zero-extension is written as shifts**
  (`(ph << 24) >> 24 > 9`). As `++self->phase > 9`, cse reused a
  QImode 0xff register from the counter test across the spawn call.
- **The timer tests** read the timer through an `s32` inline
  (`OrbitTimer`). A direct `self->timer > 0x100` is shortened to an
  unsigned compare; the ROM re-reads with `ldrh` and uses a signed
  `ble`, with the constant loaded first (old_agbcc scheduling). A
  `vu16` read gives the re-read but keeps it unsigned and pins the load
  before the constant.
- The state is re-read for each test (a `u8 state` local was copied
  into `r1`).

Left (21 halfwords): modes 1 and 2 load x/y into `r0` and the velocity
into `r1` (the ROM has them the other way round; no spelling moved
them), the spawn's `movs r5, #1` comes one instruction before
`add r3, sp, #4`, and the state-3 tail stores x before computing y.
`switch`, `ORBIT_POS` copies and setter inlines were worse.

## `HandleLinkSerial` (no draft -> size-exact, 514 halfwords)

The per-frame SIO pump. The first draft was written straight from the
ROM, and its control flow and block order match. `data` is a pointer
(`link_sio.c` passes `0x04000120`, i.e. SIOMULTI0-3). It fills in
two layout details: each player record has a 16-entry `u16 rx[]` ring
at +0x0a (`field_2c` is its index), and a `link_ring`'s bytes start at
+4 (`field_8c` is the write index, `field_88` the read index,
`field_84` the count). The rest of the semantics are in the draft's
comments.

What moved it to the right size:

- The ring pushes/pops are written out in place (an inline
  `LinkRingPop` gives the ROM's ring pointer in the session pop but is
  12 bytes longer).
- No local for `&self->id[1]`; the ROM keeps one spilled copy and a
  local made two.
- `p->id[1]` is read inline in the compare, not cached in a `u8`.
  Taking the address of a byte local for the nibble cast put it on
  the stack.
- The hash-match exit is `if (want == hash) goto copy; continue;`.

Left is register allocation that shifts everything after it. The ROM
keeps the `field_4 = 1` constant in `r2` and reuses it for both SIOCNT
bit tests. The first receive loop walks `data` with two pointers where
this compile combines the givs. The ring loops recompute the field
addresses in each loop's preheader and keep `n` in `r7`.

## Tools

The helpers in the scratch area were `brute2.py`/`triage.py` from the
previous pass plus a side-by-side dumper (`sbs.py`). The `-da` RTL dumps
of old_agbcc (`.rtl`, `.cse`, `.gcse`, `.loop`, `.lreg`, `.greg`) showed
where each difference came from: loop rotation in the `.rtl` dump, the
block move in `.loop`, and the ties in `.regmove`/`.lreg`.

## Verification

- `rm -rf build && make NON_MATCHING=1 report`: no warnings from the
  touched files.
- `rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare`:
  `crashbandicootxs.gba: OK`.

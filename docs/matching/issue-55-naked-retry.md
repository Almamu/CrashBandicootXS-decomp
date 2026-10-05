# Retrying issue #55's NAKED functions (`0x0802E0A4`-`0x0802F0DC`)

The first pass over this gap
([issue-54-issue-56-gap-e0a4.md](issue-54-issue-56-gap-e0a4.md))
matched 4 of its 25 functions and parked the other 21 as NAKED
transcriptions, mostly without trying them as C. This pass tried every
one of them and closed all 21. `src/graphics/actor_part128.c` now has
no NAKED functions left.

**Compiler:** old_agbcc. `DrawJetpackPlayer` only matches under it, and every
other function in the file compiles the same under both compilers, so
the whole object went onto the Makefile's `OLD_AGBCC_OBJS`. No split
was needed.

## What the range is

- `CreateJetpackActor` is the level's spawn dispatcher. It is a plain
  `switch (kind)` over the stride-40 per-kind record table
  `gJetpackAnimTable`. `SpawnJetpackActor` picks a spawn record's kind byte
  and forwards to it. `CreateJetpackCheckpointText`-`SpawnJetpackShot` are small
  `new Foo(...)` constructors, each tied to a fixed record of the table.
- `CreateJetpackPlayer`-`JetpackPlayerStateRollRight` are the player vehicle object (method
  table gJetpackPlayerVtable): constructor, per-frame update, sprite
  draw, damage handler, d-pad steering and the per-state input steps.
  Its state lives in the `gJetpackBomberSfxTimer`-`gJetpackPlayerTiles`
  singletons.

## Closed (21 functions)

| Function | What it took |
|---|---|
| `CreateJetpackActor` | A plain `switch` matched on the first try. It uses case 23's `if` plus a fallthrough into cases 20-22 (the ROM's shared tail) and returns each constructor's result. The first pass's worry that the jump table would be hard to reproduce was unfounded. |
| `SpawnJetpackActor` | Straight C. The only fix needed was declaring `IsCrystalSaved` as returning `u8`. |
| `SpawnJetpackBalloon`, `SpawnHovercraftFireball`, `SpawnAirshipFireball`, `SpawnJetpackCannonball`, `SpawnJetpackShot` | The `static inline AllocActor(size)` wrapper from `actor_part_2ac28.c`, which gets the ROM's size-before-flags order for `mem_alloc`. The "r8/sb pressure" noted in the first pass was not a real problem. |
| `CreateJetpackCheckpointText`, `CreateJetpackExplosion`, `InitJetpackPlayer` | The ROM loads the hit-point constant (1 / 100) into a callee-saved register *before* the `InitActorPart` call. An inline base constructor `InitHpActor(obj, rec, x, y, z, hp)` that takes the value as an argument reproduces this. |
| `SpawnHovercraftSideGun` | The ROM stores the last argument into its stack slot with `add rN, sp, #4; strb`, which is a one-byte struct passed by value. `struct byte_arg { u8 v; } __attribute__((packed))` reproduces it. A promoted `u8` stores with `str`, and an unpacked struct is 4 bytes. |
| `CreateJetpackPlayer` | Straight C. The chained assignment `g = &(p = ctor(...))->base` keeps the ROM's store order. |
| `DamageJetpackPlayer` | Straight C, using the branchless `Abs()` and `CLAMP_SPEED` helpers. |
| `SteerJetpackPlayerY`, `SteerJetpackPlayerX` | These are C++ methods that ignore `this`. Three changes were needed. The key word is read as a whole struct (the ROM uses `ldr`, not `ldrh`). The decay toward zero goes through an inline that takes `s32 *`, which makes the ROM reload the global for the `abs` check. `abs` is written branchless (`asrs/eors/subs`), because `__builtin_abs` and `?:` both produce branches. |
| `JetpackPlayerStateFly`, `JetpackPlayerStateRollLeft`, `JetpackPlayerStateRollRight` | The key word is copied into a local `struct keys_pair` and its `.held` field is tested. The field extraction gives the ROM's `lsl/lsr #16`. The steering helpers are called with `self`. |
| `UpdateJetpackPlayer` | `ACTOR_PMF_CALL` on gJetpackPlayerStateFuncs. The depth-bits term goes into a scoped local so it is computed before the two `abs` terms, and the byte-padding after the function needs `asm(".align 2, 0")`. |
| `DrawJetpackPlayer` | Inline copies of actor_anim.c's accessors (`GetAnimFrameBaseOffset` → `CurFrame`, `GetAnimFrameAttr` → `CurAttr`). `scale` is declared first, which puts its spill slot at `[sp]`. The tile number is written as the first operand of the OR and pinned to `r0`. `__divsi3` is aliased to `__divsi3`. **old_agbcc only**: current agbcc loads `attr` straight into `r4` (`ldrh r4; lsls r4`) where the ROM goes through `r0`, and nothing tried at the C level changed that. |

The first pass's four matched constructors (`SpawnJetpackCollectedWumpa`,
`SpawnHovercraftCannonFlash`, `SpawnHovercraftLauncher`, `SpawnHovercraftCannon`) were also rewritten to use
`AllocActor` and `&gJetpackAnimTable[n]` in place of their `r0`/`r1`
register pins and raw byte offsets. Their bytes are unchanged.

## Didn't close

Nothing. All 25 functions in the range are now real C.

## Verification

`rm -rf build && make NON_MATCHING=1 report` gives no warnings from
`actor_part128.c`. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` prints `crashbandicootxs.gba: OK`.

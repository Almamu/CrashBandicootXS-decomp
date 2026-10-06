# Issue #57: 0x0802FBF0-0x08030334 (actor), plus two #58 stragglers

`asm/code_3_2_20_28568_c99c_2fbf0.s` held 27 raw functions,
`AimJetpackPlane`..`AirshipFireballStateSpiralIn`: all 25 of issue #57's range and the first
two of issue #58's range (`AirshipFireballStateOrbit`/`AirshipFireballStateSpiralIn`, the only raw
functions #58 had left). **All 27 are now real C** in
`src/vehicle/jetpack_plane.c`, with no NAKED or `NON_MATCHING`
functions. The raw `.s` file is deleted and a clean `make compare`
passes.

**Compiler:** the current `agbcc`. `old_agbcc` produces byte-identical
output for every function in this file, because none of them has the
constant-before-byte scheduling that tells the two apart. The object
therefore stays off `OLD_AGBCC_OBJS`, like its neighbors.

## What the code is

The functions are the methods of three small gcc 2.x C++ classes built
on the shared per-instance "self" object (issues #54-#62). This pass
gives that object a real struct, `struct actor_self` in the new
`include/actor_self.h`: `anims` +0x00, `animTime` +0x08, `animIndex`
+0x0C, `animTimer`/`animDone` +0x10/+0x12, `x`/`y`/`z` +0x1C-0x24,
`state` +0x28, `depth` +0x34, `stateTime` +0x44 and `vtable` +0x50.
Each class embeds it as `base` and declares its own fields from +0x54
on. `struct anim_frame_record` moved from `actor_anim.c` into the same
header.

- **`gJetpackPlaneVtable`** (`struct jetpack_plane`): a hopping object.
  `AimJetpackPlane` aims the next ballistic hop at a level sub-effect
  target (`sub_802A5xx` accessors). It solves for per-step
  accelerations from the height difference and hop speed. `JetpackPlaneStateFly`
  integrates those accelerations and re-aims when the steps run out.
  `DamageJetpackPlane` is the damage handler, `CreateJetpackPlane` the constructor,
  `RunJetpackPlaneState` the per-state member-pointer dispatch, and
  `JetpackPlaneStateFall`/`JetpackPlaneStateKnockedOut`/`JetpackPlaneStateFollow`/`IsJetpackPlaneUnshootable` small
  helpers.
- **`gJetpackBomberVtable`** (`struct jetpack_bomber`): the constructor
  `CreateJetpackBomber` switches on the spawn record's kind byte (4-9) to pick
  the starting state. `UpdateJetpackBomber` is the per-frame update: it handles
  player contact, then the state dispatch, then death. The per-state
  movers `JetpackBomberStateCircle`/`JetpackBomberStateSwingHorizontal`/`JetpackBomberStateBobVertical` circle a home point
  on the shared sine table `gSineTable`, or fall back to
  `HomeJetpackBomber` (ease toward the player). `DamageJetpackBomber` is the damage
  handler.
- **`gJetpackCannonballVtable`** (`struct jetpack_cannonball`): a straight-line
  projectile. The constructor is `CreateJetpackCannonball`, the step is
  `UpdateJetpackCannonball`, and `IsJetpackCannonballUnshootable` is a constant-true predicate.
- **`AirshipFireballStateOrbit`/`AirshipFireballStateSpiralIn`** (#58): the orbiting-companion
  updaters. The first grows its radius and the second spirals inward,
  as `docs/rom_map.md` describes.

## Techniques that mattered

- **The "r7 hazard" member-pointer dispatch is plain C.**
  `RunJetpackPlaneState`/`RunJetpackBomberState` and the middle of `UpdateJetpackBomber` have the
  shape that has been parked NAKED across the project as a toolchain
  hazard: `RunPolarPlayerState`, `RunJetpackPlayerState`, `UpdateJetpackPlane`, `UpdateAirshipFireball`,
  `RunAirshipFireballState`, `RunJetpackBalloonState`, `UpdateJetpackBalloonCrate`, `RunJetpackBalloonCrateState`, and more
  in `polar_player_dispatch.c`, `hovercraft_cannon.c`, `hovercraft_launcher.c`,
  `jetpack_balloon.c`, `jetpack_crates.c` and `hovercraft.c`. The shape is gcc 2.x's
  expansion of `(this->*table[this->state])()`, where each table entry
  is a `{s16 delta; s16 index; union {s16 vtableOffset; fn}}`
  pointer-to-member (`struct actor_pmf`). The macro `ACTOR_PMF_CALL`
  reproduces it byte for byte, with the table base in r7 and no
  register pins. Four things were needed:
  1. A real indirect call `fn(self + d)` rather than an explicit
     `_call_via_r3(...)` call. Thumb gcc emits it as
     `bl _call_via_r3`, which resolves to the ROM's trampoline (then
     through a `.set` alias, the `dingodile.c` technique). The explicit-call form forces
     values into r1/r2 that the ROM leaves as garbage.
  2. Load the `index` field once into a local, but re-index
     `table[self->state]` for every other field.
  3. Hold the 8-byte method record in a struct local, so it is copied
     as a whole into r5:r6.
  4. Compute the final `this` adjustment into a separate local
     through `if/else`.

  The macro must **not** be wrapped in `do { } while (0)`. agbcc treats
  that as a loop when weighing register priorities, and the whole
  allocation shifts. With the wrapper removed, all the header's
  statement macros use `if (1) { ... } else (void)0`. **The NAKED
  functions listed above are worth retrying with this macro.**
- **Virtual calls** go through `ACTOR_VCALL(obj, m20/m08, arg)`, a real
  indirect call through the method record, not an explicit
  `_call_via_r2`.
- **State resets** (`ACTOR_SET_STATE`) put state and animation index
  in locals first. The ROM then materializes a constant pair
  (`movs r0,#K1; movs r1,#K2`) before the stores. This removed the
  need for the per-site `register ... asm("rN")` pins that neighboring
  files use for the same pattern.
- **Constructors** use the neighbors' pins. In `CreateJetpackPlane`, the
  constant 4 is pinned to r4. In `CreateJetpackBomber`, `part`/`b`/`c`/2 are
  pinned to r8/r5/r6/r4, and the stack argument is pinned to r0
  *after* them (the `CreateAirshipFireball` fix). The kind byte is read through
  an `r1`-pinned pointer copy. `CreateJetpackCannonball` copies `CreateJetpackShot`'s
  arrangement.
- **`AirshipFireballStateOrbit`/`AirshipFireballStateSpiralIn`** had been abandoned by the first #58
  pass. They needed two changes. First, split the orbit-centre update
  into `px = player->x; cx = self->centerX; tx = cx - 0x600;
  cx += (px - tx) >> 5;`. Otherwise gcc folds
  `px - (cx - 0x600)` into `(px + 0x600) - cx` and loads in a
  different order. Second, pin the two centre values and the sine-table
  pointer to r3/r4/r5. `self` and the r7 angle counter are left to the
  allocator, and it picks r6/r7 as the ROM does.
- Smaller fixes: an explicit `if/else` for a two-way constant choice.
  The ROM loads the tested field before the constant, while
  `idx = K; if (..) idx = K2;` loads the constant first. Also, a
  hoisted `s16 *sine = gSineTable;` in `JetpackBomberStateCircle`, and a
  scoped `steps` local plus an in-expression `scale2 = scale * 2` in
  `AimJetpackPlane` to get the ROM's evaluation order.

None of the 27 functions is called by name from other code: they are
reached through method tables and dispatch tables in data. The
constructors are called from the raw-bytes NAKED `jetpack_spawn.c`.

## Verification

`rm -rf build && make NON_MATCHING=1 report` produced no warnings from
the touched files. `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` printed
`crashbandicootxs.gba: OK`.

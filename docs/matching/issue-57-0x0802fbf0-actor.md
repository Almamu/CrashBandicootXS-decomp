# Issue #57: 0x0802FBF0-0x08030334 (actor), plus two #58 stragglers

`asm/code_3_2_20_28568_c99c_2fbf0.s` held 27 raw functions,
`sub_802FBF0`..`sub_803044C`: all 25 of issue #57's range and the first
two of issue #58's range (`sub_8030334`/`sub_803044C`, the only raw
functions #58 had left). **All 27 are now real C** in
`src/graphics/actor_part_2fbf0.c`, with no NAKED or `NON_MATCHING`
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

- **`gStaticData_087E51B4`** (`struct actor_51b4`): a hopping object.
  `sub_802FBF0` aims the next ballistic hop at a level sub-effect
  target (`sub_802A5xx` accessors). It solves for per-step
  accelerations from the height difference and hop speed. `sub_802FE78`
  integrates those accelerations and re-aims when the steps run out.
  `sub_802FD1C` is the damage handler, `sub_802FD8C` the constructor,
  `sub_802FEA4` the per-state member-pointer dispatch, and
  `sub_802FE04`/`sub_802FE1C`/`sub_802FE58`/`sub_802FF00` small
  helpers.
- **`gStaticData_087E51EC`** (`struct actor_51ec`): the constructor
  `sub_802FF08` switches on the spawn record's kind byte (4-9) to pick
  the starting state. `sub_802FFB8` is the per-frame update: it handles
  player contact, then the state dispatch, then death. The per-state
  movers `sub_80300E0`/`sub_803013C`/`sub_8030188` circle a home point
  on the shared sine table `gStaticData_0816A820`, or fall back to
  `sub_80300B0` (ease toward the player). `sub_80301EC` is the damage
  handler.
- **`gStaticData_087E5224`** (`struct actor_5224`): a straight-line
  projectile. The constructor is `sub_8030300`, the step is
  `sub_8030298`, and `sub_8030330` is a constant-true predicate.
- **`sub_8030334`/`sub_803044C`** (#58): the orbiting-companion
  updaters. The first grows its radius and the second spirals inward,
  as `docs/rom_map.md` describes.

## Techniques that mattered

- **The "r7 hazard" member-pointer dispatch is plain C.**
  `sub_802FEA4`/`sub_8030234` and the middle of `sub_802FFB8` have the
  shape that has been parked NAKED across the project as a toolchain
  hazard: `sub_802C208`, `sub_802F748`, `sub_802FA38`, `sub_8030574`,
  `sub_8030648`, `sub_8031A08`, `sub_8031A6C`, `sub_80322F4`, and more
  in `actor_part19e/31/33/37/64/125/129/130.c`. The shape is gcc 2.x's
  expansion of `(this->*table[this->state])()`, where each table entry
  is a `{s16 delta; s16 index; union {s16 vtableOffset; fn}}`
  pointer-to-member (`struct actor_pmf`). The macro `ACTOR_PMF_CALL`
  reproduces it byte for byte, with the table base in r7 and no
  register pins. Four things were needed:
  1. A real indirect call `fn(self + d)` rather than an explicit
     `sub_803AD84(...)` call. Thumb gcc emits it as
     `bl _call_via_r3`, aliased to `sub_803AD84` with `.set` (the
     `actor_part_1967c.c` technique). The explicit-call form forces
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
  `sub_803AD80`.
- **State resets** (`ACTOR_SET_STATE`) put state and animation index
  in locals first. The ROM then materializes a constant pair
  (`movs r0,#K1; movs r1,#K2`) before the stores. This removed the
  need for the per-site `register ... asm("rN")` pins that neighboring
  files use for the same pattern.
- **Constructors** use the neighbors' pins. In `sub_802FD8C`, the
  constant 4 is pinned to r4. In `sub_802FF08`, `part`/`b`/`c`/2 are
  pinned to r8/r5/r6/r4, and the stack argument is pinned to r0
  *after* them (the `sub_80305F8` fix). The kind byte is read through
  an `r1`-pinned pointer copy. `sub_8030300` copies `sub_802FA04`'s
  arrangement.
- **`sub_8030334`/`sub_803044C`** had been abandoned by the first #58
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
  hoisted `s16 *sine = gStaticData_0816A820;` in `sub_80300E0`, and a
  scoped `steps` local plus an in-expression `scale2 = scale * 2` in
  `sub_802FBF0` to get the ROM's evaluation order.

None of the 27 functions is called by name from other code: they are
reached through method tables and dispatch tables in data. The
constructors are called from the raw-bytes NAKED `actor_part128.c`.

## Verification

`rm -rf build && make NON_MATCHING=1 report` produced no warnings from
the touched files. `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` printed
`crashbandicootxs.gba: OK`.

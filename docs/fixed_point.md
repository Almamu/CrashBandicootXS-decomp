# Fixed point

The game keeps positions and speeds as integers with fraction bits:
fixed point. Most of it is Q8 (24.8): 8 fraction bits, so `0x100` is
one pixel (or 1 px a frame, or 1.0), `0x80` half of one, and `x >> 8`
the whole pixels of `x`. This page lists the formats, the macros that
spell them and the conventions for using them (#916).

## The formats

| Format | One unit | Where |
|---|---|---|
| Q8 | `0x100` | positions (`Entity::x/y`, `ActorSelf::x/y/z`, the camera's `Camera::x/y`), speeds and speed ramps (`MovingSprite::speedX/Y`, `rampX/Y`, the `struct speed_ramp` tables in src/data), accelerations and gravity, scroll steps, affine scales (`0x100` = 1.0, `Sprite::affine`, `ScaledSprite::scaleX/Y`), `animTime` (Q8 frames), the sine table's values (`SIN_Q8`) |
| Q4 | `0x10` | the jetpack course's planes' velocities: sixteenths of their (Q8) position's unit, `x += Q4_TO_INT(velX)` |
| Q12 | `0x1000` | the scale factors of the 3D (vehicle) levels' projection and aim vectors (`0x1000 / n`, `Q12_MUL`) |
| Q16 | `0x10000` | the company logos' pieces (`posA.q`, a Q16 word whose high half is the pixel) and the logos' zooms (`LogoPiece::velA/velB`, the Universal logo's `scale`: `INT_TO_Q16(1)` = 1:1, whose OBJ/BG step is the Q8 `Q8_DIV(INT_TO_Q16(1), scale)`), FixedMul16/FixedDiv16 |

Angles aren't fixed point: they are 256 steps a turn (`& 0xFF`), the
index into gSineTable, whose values are Q8 (`SIN_Q8(angle)`).

`tools/magic_numbers.py --fixed` finds the format of a value from the
code: a field that is shifted right by 8 where it is read
(`Q8_TO_INT(p->x)`) or assigned an `INT_TO_Q8(...)` is Q8; a local
assigned a conversion's result (`px = Q8_TO_INT(x)`) holds whole pixels.

## The macros (`include/math_util.h`)

| Macro | Expands to | Use |
|---|---|---|
| `INT_TO_Q8(n)` | `(n) << 8` | a whole number of pixels (px a frame, ...): `INT_TO_Q8(2)` for `0x200` |
| `Q8(d)` | `(s32)((d) * 256)` | a short decimal: `Q8(1.5)` for `0x180`, `Q8(0.125)` for `0x20` |
| `Q8_FRAC(n, f)` | `((n) << 8) + (f)` | `n` pixels and `f` 256ths, for the fractions that aren't short decimals: `Q8_FRAC(0, 51)` |
| `Q8_TO_INT(x)` | `(x) >> 8` | the whole pixels of `x` (an arithmetic shift for a signed `x`: it rounds down) |
| `Q8_TO_INT_INPLACE(x)` | `(x) >>= 8` | the same, converting a variable in place |
| `Q8_FLOOR(x)`, `Q8_FLOOR_INPLACE(x)` | `(x) & ~0xFF`, `(x) &= ~0xFF` | `x` rounded down to a whole pixel, still in Q8 (also spelled `x & 0xFFFFFF00`): a position snapped onto the floor it was probed against |
| `Q8_MUL(a, b)`, `Q8_DIV(a, b)` | `((a) * (b)) >> 8`, `((a) << 8) / (b)` | a Q8 product and quotient |
| `INT_TO_Q4(n)`, `Q4_TO_INT(x)` | `(n) << 4`, `(x) >> 4` | Q4 |
| `INT_TO_Q12(n)`, `Q12_TO_INT(x)`, `Q12_MUL(a, b)` | `<< 12`, `>> 12` | Q12 |
| `INT_TO_Q16(n)`, `Q16_TO_INT(x)` | `<< 16`, `>> 16` | Q16 |

`Q8(d)` and `Q8_FRAC(n, f)` are integer constant expressions: agbcc
folds the cast of the floating constant, so they work in `case` labels
and in the initializers of src/data's tables, and a converted site
compiles to the same code as the literal it replaces. `Q8(d)` needs a
`d` that is a multiple of 1/256 (the product is exact); it is for the
short decimals (multiples of 1/8, and of 1/16 under 1). Both take
non-negative parts: a negative value is `-Q8(1.5)`, `-Q8_FRAC(0, 51)`.

## Named constants

A tuning value that recurs, or whose meaning the code shows, gets a
name in its subsystem's header, spelled with the macros:

| Name | Value | Header |
|---|---|---|
| `PLAYER_GRAVITY` | `Q8_FRAC(0, 51)` | `player.hpp`: the jump records' step, 51/256 px a frame per frame |
| `LEVEL_SELECT_ZOOM_MIN`, `ZOOM_BG_ZOOM_STEP` | `Q8_FRAC(0, 8)` | `level_select.hpp`: the level-select zooms' smallest scale (1/32) and the picture's zoom a frame |
| `PLAYER_MAX_FALL_SPEED` | `INT_TO_Q8(6)` | `player.hpp`: the jump and fall records' target |
| `SWIM_STROKE_SPEED`, `SWIM_SPIN_SPEED` | `Q8(2.75)`, `Q8(3.75)` | `swim_ctrl.hpp`: the diver's stroke and spin, split between X and Y by the direction (3/4, 1/2, 1/4 of it) |
| `PLAYER_JUMP_SPEED`, `PLAYER_JUMP_HELD_SPEED`, `PLAYER_JUMP_RELEASED_SPEED`, `PLAYER_DOUBLE_JUMP_SPEED`, `PLAYER_BOUNCE_SPEED`, `PLAYER_BOUNCE_HIGH_HELD_SPEED`, `PLAYER_LAUNCH_PAD_SPEED` | `Q8_FRAC(4, 98)` ... `Q8_FRAC(7, 208)` | `player.hpp`: the jumps' start speeds in gCtrlMotionRecords (negated: up), by the motion entry that queues each |
| `PLAYER_FALL_START_SPEED`, `PLAYER_FALL_ACCEL`, `PLAYER_TORNADO_FALL_ACCEL_*`, `PLAYER_SLIDE_DECEL`, `PLAYER_ICE_STOP_DECEL`, `PLAYER_DOUBLE_JUMP_ACCEL_X` | `Q8_FRAC(0, n)` | `player.hpp`: gCtrlMotionRecords' other tuned starts and steps |
| `SWIM_RAMP_STEP`, `SWIM_SLOW_RAMP_STEP`, `SWIM_IDLE_BOB_SPEED`, `SWIM_STOP_START_SPEED` | `Q8_FRAC(0, n)` | `swim_ctrl.hpp`: gSwimCtrlMotionRecords' steps and small speeds |
| `ACTOR_FOCAL_LENGTH`, `ACTOR_FOCAL_LENGTH_POLAR` | `INT_TO_Q8(28)`, `INT_TO_Q8(47)` | `actor.h`: gActorFocalLength, the 3D levels' projection distance (InitActorBgScroll) |
| `ACTOR_PROJECTION` | `INT_TO_Q12(ACTOR_FOCAL_LENGTH)` | `actor.h`: the Q12 projection's numerator; `ACTOR_PROJECTION / depth` is the Q12 scale of a Q8 position (the jetpack player, the airship, the hovercraft) |
| `MEGA_MIX_*_SPEED`, `DINGODILE_WALK_*`, `DINGODILE_HIT_RECOIL_*`, `DINGODILE_ROCKET_RISE_ACCEL`, `DINGODILE_STALACTITE_*` | `Q8_FRAC(n, f)` | `bosses.h`: the bosses' speed-ramp records (gMegaMixMotionRecords, gDingodileMotionRecords, the rocket's and the stalactite's) |
| `HOVERCRAFT_*_SPEED_Z`, `HOVERCRAFT_SPEED_Z_STEP`; `AIRSHIP_*_SPEED_Z`, `AIRSHIP_SPEED_Z_STEP` | `Q8_FRAC(0, f)` | `hovercraft.hpp`, `airship.hpp`: each state's Z speed, which the state ramps toward a 256th a frame |
| `AIRSHIP_STEER_ACCEL_X/Y`, `AIRSHIP_EDGE_PUSH_X/Y`, `AIRSHIP_MAX_SPEED_X/Y`, `AIRSHIP_FALL_GRAVITY`, `AIRSHIP_GUN_OFFSET_X/Y/Z` | `Q8_FRAC(n, f)` | `airship.hpp`: Steer's pull, edge push and limits, the fall, and where its fireballs and cannonballs start |
| `AIRSHIP_FIREBALL_*`, `HOVERCRAFT_FIREBALL_*` | `Q8_FRAC(0, f)` | `boss_actors.hpp`: the fireballs' launch speed, drag and minimum speed, the orbit's radius |

`--fixed` counts a `#define` in include/ whose value is spelled with a
fixed-point macro as spelled. A value with no evidence for a name keeps
its spelling (`INT_TO_Q8(5)`) or its number.

## Conventions

- **Keep the value and the arithmetic.** A converted site has the same
  value and the same operations: only the literal's spelling changes.
  `x >> 8` and `x / 256` are different code for a signed `x` (the shift
  rounds down, the division toward zero), so `Q8_TO_INT` replaces a
  shift only; a `/ 256` stays as it is. A bound just under a value is
  spelled from the value it is under, keeping the comparison:
  `speedY <= 0x27F` is `speedY <= Q8(2.5) - 1`.
- **Keep the literal's type.** A hex literal above `0x7FFFFFFF`
  (`0xFFFFF880`) is unsigned; `-Q8(7.5)` is signed. Converting it is
  safe in an assignment to an `s32`, not in a comparison or a division,
  where the signedness changes the code.
- **Whole, short or odd.** `INT_TO_Q8(n)` for a whole number of units,
  `Q8(d)` for a short decimal, `Q8_FRAC(n, f)` where the split between
  pixels and fraction says something. A designer's odd tuning value (a
  jump speed in 256ths, `-1122`) gets a name from what the code does
  with it, in its owner's header, spelled with `Q8_FRAC`:
  `{ -PLAYER_JUMP_SPEED, PLAYER_GRAVITY, PLAYER_MAX_FALL_SPEED }`. A
  value used in several places shares a name only where it means the
  same thing.
- **Not every `0x100` is a pixel.** A byte extraction (`>> 8 & 0xFF`), a
  volume (`0x100`), a palette bank (`<< 12`), an OAM field (`<< 16`), a
  tile index or a count isn't fixed point; `--fixed` classifies those as
  `integer` or `bitfield`.

## The inventory (`--fixed`)

```
tools/magic_numbers.py --fixed --category fixed [--show]  # the raw fixed-point sites, with a spelling
tools/magic_numbers.py --fixed --report OUT               # per kind, format, role, directory and field
tools/magic_numbers.py --fixed --values                   # per field, each raw value and its count
tools/magic_numbers.py --fixed --json --path src/bosses   # for agents
```

It scans src/ and include/ (and the `struct speed_ramp` tables of
src/data) for literals assigned to, compared with, added to or clamped
against a value whose name says position, velocity, acceleration, ramp,
camera, affine, angle or sub-frame time, the shifts by 4, 8, 12 and 16
and the `& 0xFF` / `& ~0xFF` masks on them, and the sites already
spelled with a macro. A literal passed to a function counts as the
parameter it is passed as (`SetLeadSpeed(0x3200)` is `speed`), from the
parameter names of the function's definition or declarations; a member
of a shared struct (`struct vec2`, `struct aabb`) counts per variable
(`vec2::y [pos in ProbeFloor]`), with that variable's evidence. Each site is `fixed` (with its format and
spelling), `integer` (a pixel, a count, a byte field, a value already
converted), `bitfield` (OAM, palette, nibble packing) or `spelled`. It
is a heuristic: check a site before converting it, and rebuild and
compare after.

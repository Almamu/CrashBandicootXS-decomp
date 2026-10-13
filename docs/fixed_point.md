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
| Q16 | `0x10000` | the company logos' pieces (`posA.q`, a Q16 word whose high half is the pixel), FixedMul16/FixedDiv16 |

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
| `PLAYER_MAX_FALL_SPEED` | `INT_TO_Q8(6)` | `player.hpp`: the jump and fall records' target |
| `SWIM_STROKE_SPEED`, `SWIM_SPIN_SPEED` | `Q8(2.75)`, `Q8(3.75)` | `swim_ctrl.hpp`: the diver's stroke and spin, split between X and Y by the direction (3/4, 1/2, 1/4 of it) |
| `ACTOR_FOCAL_LENGTH`, `ACTOR_FOCAL_LENGTH_POLAR` | `INT_TO_Q8(28)`, `INT_TO_Q8(47)` | `actor.h`: gActorFocalLength, the 3D levels' projection distance (InitActorBgScroll) |
| `ACTOR_PROJECTION` | `INT_TO_Q12(ACTOR_FOCAL_LENGTH)` | `actor.h`: the Q12 projection's numerator; `ACTOR_PROJECTION / depth` is the Q12 scale of a Q8 position (the jetpack player, the airship, the hovercraft) |

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
  pixels and fraction says something; a designer's decimal tuning value
  in a table (`{ -1122, PLAYER_GRAVITY, PLAYER_MAX_FALL_SPEED }`, a jump
  speed in 256ths) keeps its number.
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
spelled with a macro. Each site is `fixed` (with its format and
spelling), `integer` (a pixel, a count, a byte field, a value already
converted), `bitfield` (OAM, palette, nibble packing) or `spelled`. It
is a heuristic: check a site before converting it, and rebuild and
compare after.

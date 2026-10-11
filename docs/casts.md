# Casts and the types they stand for (#855)

Much of the matched code converts values at their uses: `(u8)f()`,
`(MovingSprite *)from`, `(u16 *)(base + off)`. Many of these exist only
because a local, a field, a parameter or a return type was guessed wrong
during matching, or because the code was written before the struct or the
class existed. The aim is fewer casts and types that say what the value
is. A cast stays only when it is a real conversion or the match needs it,
and then it has a short comment.

## The inventory: `tools/magic_numbers.py --casts`

[tools/magic_casts.py](../tools/magic_casts.py) lists the C-style casts in
the game code (src/ without src/data/, and include/), grouped by the thing
being cast:

- a **field**: `Sprite::x`, the class that declares the member and the
  member path from there (magic_bits.py's model, as `--bits` uses it);
- a **local** or a **parameter**: `local from (HandleEvent,
  src/player/player_update.cpp)`;
- a **global**: `global gActorList`;
- a function's **result**: `GetUiText()`, `LevelState::HasTurboRun()`;
- an **address**: `&Entity::x`, the `(T *)&x` views;
- an **address macro**: `macro REG_ADDR_BLDCNT`, `macro BG_PLTT`;
- `<expr>`: anything else (`(u8)(a - b)`, `(u32)v >> 31`).

For each thing it gives the declared type (when the model finds it), the
cast types, how many of its reads are cast (the uses that aren't
assignments to it) and the kind of each cast:

| kind | what |
|---|---|
| `scalar` | to an integer from an integer: `narrow`, `widen` or `sign` (and `+sign`) |
| `ptrint` | a pointer to an integer, `(u32)p` |
| `pointer` | to a pointer type from another pointer type (`view`: between two known classes or structs), an integer (`from int`) or `void *` |
| `derived` | to a class pointer from a base class pointer (`down`) or the other way (`up`) |
| `byteoff` | `(T *)((u8 *)p + N)`, `(T *)((u32)p + N)`, `(T *)(base + N)` with a byte pointer or an address |
| `same` | to the type the thing already has |

The listing is ranked by cast uses x consistency, the share of the reads
cast to the most common type, so a thing every read casts the same way
comes first: those are the retype candidates.

```
tools/magic_numbers.py --casts                 per thing, ranked, with its sites
tools/magic_numbers.py --casts --report [OUT]  the same as Markdown
tools/magic_numbers.py --casts --json          every cast as JSON
tools/magic_numbers.py --casts --path src/menus --field 'GetUiText|Text'
tools/magic_numbers.py --casts --kind derived --kind byteoff
```

It doesn't run the preprocessor, so a cast inside a macro's body isn't
seen. The model is the regex one of magic_numbers.py: a thing it can't
resolve shows as `?::member`, and a declared type it can't find as `?`.

## The inventory on origin/main (when #855 started)

1156 casts on 480 things:

| thing | things | casts | scalar | ptrint | pointer | derived | byteoff | same |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| field | 96 | 209 | 66 | 23 | 76 | 15 | 15 | 14 |
| local | 215 | 285 | 65 | 29 | 140 | 7 | 32 | 12 |
| global | 44 | 96 | 2 | 9 | 60 | 15 | 3 | 7 |
| result | 64 | 205 | 125 | 4 | 49 | 0 | 3 | 24 |
| address | 37 | 63 | 0 | 0 | 63 | 0 | 0 | 0 |
| macro | 23 | 171 | 3 | 0 | 103 | 0 | 65 | 0 |
| expr | 1 | 127 | 103 | 0 | 18 | 0 | 6 | 0 |

The top retype candidates:

- `IsTouchingPlayer()` (declared `s32`): cast at 31/31 reads, to `u8` 31; scalar/narrow+sign 31
- `GetUiText()` (declared `s32`): cast at 25/39 reads, to `u8 *` 23, `void *` 2; pointer/from int 25
- `RandRange()` (declared `u16`): cast at 18/19 reads, to `u16` 16, `s32` 2; same 16, scalar/widen+sign 2
- `AnimPart::animTimer` (declared `u16`): cast at 12/12 reads, to `s16` 12; scalar/sign 12
- `anim_table_record::index` (declared `u32`): cast at 12/13 reads, to `u8` 12; scalar/narrow 12
- `LevelEntityFlags::IsActivated()` (declared `s32`): cast at 8/8 reads, to `u8` 8; scalar/narrow+sign 8
- `local from (HandleEvent, src/player/player_update.cpp)` (declared `s32`): cast at 8/8 reads, to `MovingSprite *` 8; pointer/from int 8
- `PartList::items[]` (declared `Sprite *`): cast at 9/9 reads, to `MovingSprite *` 8, `Wumpa *` 1; derived/down 9
- `LevelState::HasTurboRun()` (declared `s32`): cast at 7/7 reads, to `u8` 7; scalar/narrow+sign 7
- `LevelLayers::Probe()` (declared `s32`): cast at 6/6 reads, to `u8` 6; scalar/narrow+sign 6
- `LevelState::HasTornadoSpin()` (declared `s32`): cast at 6/6 reads, to `u8` 6; scalar/narrow+sign 6
- `bg_package::width` (declared `u32`): cast at 6/7 reads, to `s32` 6; scalar/sign 6
- `LevelState::HasSuperBodySlam()` (declared `s32`): cast at 5/5 reads, to `u8` 5; scalar/narrow+sign 5
- `bg_package::height` (declared `u32`): cast at 5/5 reads, to `s32` 5; scalar/sign 5
- `global PLTT` (declared `uint16_t *`): cast at 11/15 reads, to `vu16 *` 8, `void *` 2, `u16 *` 1; pointer 11
- `global gActorSpawnTable` (declared `sub_effect_record *`): cast at 8/9 reads, to `u8 *` 6, `actor_spawn *` 2; pointer 6, byteoff 2
- `LevelState::gemPlatform` (declared `void *`): cast at 4/4 reads, to `Platform *` 4; pointer/from void * 4
- `CrateList::nodes` (declared `CrateGridNode *`): cast at 5/7 reads, to `CrateGridNodeInit *` 5; pointer/view 5
- `PaletteCache::slots[]` (declared `u8`): cast at 5/5 reads, to `u16 *` 4, `u32` 1; pointer/from int 4, scalar/widen 1
- `IsSpawnCollected()` (declared `s32`): cast at 3/3 reads, to `u8` 3; scalar/narrow+sign 3

## How to fix a cast

1. **Wrong declared type.** A local, a parameter, a return type or a field
   that every read casts the same way gets that type, and the casts go.
   Change one field or one signature at a time: a field's or a function's
   type reaches every reader, so the whole ROM has to stay the same (a full
   `make compare`, not one object). A retype that changes any object is
   reverted, or kept local with a comment.
2. **A struct or class view.** `(T *)((u8 *)p + N)` and `(u32)p + n`
   become a field or a member access ("prefer structs").
3. **Derived-class casts.** Where the same downcast recurs, a trivial
   inline helper on the class says what the cast means: `AsMovingSprite()`,
   `AsPlayer()`, or a view of an embedded member (`LinkSession::Player(i)`).
   They are `inline`, `return (T *)this;` or a member's address, in the
   class's header. An inline can still change register allocation, so each
   helper is checked with the full build like any retype.
4. **Casts the compiler doesn't need** (to the type the value already has,
   or that an assignment does anyway) go when every object stays the same.
5. **Signedness.** A field's readers in the ROM say what it is: `ldrsb` and
   `ldrsh` (or `lsl`/`asr`) read a signed one, `ldrb` and `ldrh` (or
   `lsl`/`lsr`) an unsigned one.
6. **Casts the match needs** stay, with a short comment, at the
   declaration when every use has it (`/* stays s32: ... */`) or at the
   site.

No `MATCH_*` macros, volatiles or other workarounds to keep a new type:
`tools/match_idioms.py --functions` doesn't go up.

Each change adds its area's notes, what was retyped and which casts had
to stay and why, as a page in docs/casts/.

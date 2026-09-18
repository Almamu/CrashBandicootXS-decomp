# Status: actor

The per-instance actor "self" object family - `struct actor` and its
many satellite files (`src/graphics/actor_part*.c`,
`src/graphics/actor_aabb_setup.c`). Filed under `src/graphics/` on disk
(the ROM's actor code lives interleaved with rendering code, and
several actor functions are themselves OAM/sprite-draw routines), but
tracked as its own `actor` category here since `docs/rom_map.md` and
the `decomp-chunk` issue generator both treat it as a distinct system
from "core" graphics.

## Matched

- `src/graphics/actor_part.c` (new file - `sub_8007A48`'s real ROM
  address isn't adjacent to `graphics.c`'s matched functions, since
  `sub_8007634` sits unclaimed between them; see
  `docs/matching.md`): `sub_8007A48`, `sub_8007A84`, `sub_8007A98`,
  `nullsub_2`, `sub_8007AB4`
- `src/graphics/actor_part2.c` (new file, now directly adjacent to
  `actor_part.c`'s matched functions): `sub_8007C30`, `sub_8007CF8`,
  `sub_8007DBC` (the last one is a pre-existing NAKED transcription -
  see `docs/matching/naked-sub_8007dbc.md`)
- `src/graphics/actor_part3.c` (new file - directly adjacent to
  `actor_part2.c`'s matched functions now that `sub_8007DBC` is
  matched too, closing the old raw gap between them):
  `sub_8007F78`, `sub_8007FD8`
- `src/graphics/actor_part4.c` (new file, now directly adjacent to
  `actor_part3.c`'s matched functions): `sub_80080C0`, `sub_800815C`
- `src/graphics/actor_part5.c` (new file, now directly adjacent to
  `actor_part4.c`'s matched functions): `sub_8008304`,
  `sub_8008328`, `sub_800834C`, `sub_8008350`, `sub_8008364`,
  `sub_8008394`, `sub_80083A8`
- `src/graphics/actor_part6.c` (new file, now directly adjacent to
  `actor_part5.c`'s matched functions): `sub_8008408`, `sub_8008434`, `sub_8008480`,
  `sub_8008484`, `sub_80084A4`, `sub_80084C4`, `sub_8008518`,
  `sub_8008564`, `sub_80085B8`, `sub_8008604`, `sub_8008618`,
  `sub_8008640`, `sub_8008648`, `sub_8008650`, `sub_800865C`,
  `sub_8008674`, `sub_8008680`, `sub_800868C`, `sub_8008698`,
  `sub_80086A4`, `sub_80086B0`, `sub_80086BC`, `sub_80086C4`,
  `sub_80086CC`, `sub_80086D8`, `sub_80086E4`, `sub_80086EC`,
  `sub_80086F4`, `sub_8008710`, `sub_800872C`, `sub_8008734`,
  `sub_8008748`, `sub_8008754`, `sub_8008768`, `sub_800876C`
- `src/graphics/actor_part7.c` (new file, now directly adjacent to
  `actor_part6.c`'s matched functions): `sub_800878C`, `sub_80087A0`, `sub_80087B4`,
  `sub_80087BC`, `sub_80087C0`, `sub_80087C8`, `sub_80087D0`,
  `sub_80087F4`, `sub_80087FC`, `sub_8008804`, `sub_800880C`,
  `sub_8008814`, `sub_8008818`, `sub_800881C`, `sub_8008824`,
  `sub_8008830`, `sub_8008844`, `sub_8008850`, `sub_800885C`,
  `sub_8008864`, `sub_8008870`, `sub_800887C`, `sub_8008888`,
  `sub_800888C`, `sub_8008890`, `sub_80088D8`, `sub_80088E8`,
  `sub_80088F0`, `sub_8008904`

- `src/graphics/actor_part10.c` (new file, now directly adjacent to
  `actor_part7.c`'s matched functions): `sub_8008C80`, `sub_8008CEC`, `sub_8008D30`

- `src/graphics/actor_part11.c` (new file, now directly adjacent to
  `actor_part7b.c`'s `sub_8008D80` range): `sub_8008DC0`, `sub_8008DEC`, `sub_8008E50`,
  `sub_8008E94`, `sub_8008EB4`, `sub_8008EE4`

- `src/graphics/actor_part12.c` (new file - `sub_8009A30`'s real ROM
  address isn't adjacent to `actor_part11.c`'s matched functions
  either, since the parked `sub_8008F20` and the raw `sub_8009008`-
  `sub_8009914` span sit between them; see `docs/matching.md`):
  `sub_8009A30`, `sub_8009AA0`, `sub_8009AF0`, `sub_8009B3C`,
  `sub_8009B70`, `sub_8009B9C`

- `src/graphics/actor_part13.c` (new file - `sub_8009CA0`'s real ROM
  address isn't adjacent to `actor_part12.c`'s matched functions
  either, since the raw `sub_8009BE0` sits between them; see
  `docs/matching.md`): `sub_8009CA0`

- `src/graphics/actor_part8.c` (new file - `sub_8009EA8`'s real ROM
  address isn't adjacent to `code_3_2_15.o`'s raw content either, since
  the parked `sub_8009DF4` sits raw between them, and `sub_8009BE0`
  before that was left raw rather than guessed at; see
  `docs/matching.md`):
  `sub_8009EA8`, `sub_8009EB0`, `sub_8009EBC`, `sub_8009EC4`,
  `sub_8009ECC`, `sub_8009ED0`, `sub_8009F1C`, `sub_8009F50`,
  `sub_8009F90`, `sub_8009FB0`
- `src/graphics/actor_part9.c` (new file - `sub_8009FD4`'s real ROM
  address isn't adjacent to `actor_part8.c`'s matched functions
  either, since the parked `sub_8009DF4` sits raw between them; see
  `docs/matching.md`): `sub_8009FD4`, `sub_8009FF4`, `sub_800A050`,
  `sub_800A068`, `sub_800A06C`, `sub_800A078`, `sub_800A080`,
  `sub_800A088`, `sub_800A090`, `sub_800A098`, `sub_800A09C`,
  `sub_800A0A0`, `sub_800A0A4`, `sub_800A0A8`, `sub_800A0AC`,
  `sub_800A0CC`, `sub_800A0D8`, `sub_800A0E0`, `sub_800A0EC`,
  `sub_800A0F4`

- `src/graphics/actor_part14.c` (new file - `sub_800A5F4`'s real ROM
  address isn't adjacent to `actor_part9.c`'s matched functions
  either, since a large raw span (`sub_800A0FC`-`sub_800A590`) sits
  between them; see `docs/matching.md`): `sub_800A5F4`, `sub_800A600`,
  `sub_800A604`, `sub_800A650`, `sub_800A664`, `sub_800A6A4`,
  `sub_800A6C4`, `sub_800A6D0`, `sub_800A6DC`, `sub_800A6E8`,
  `sub_800A6F4`, `sub_800A700`, `sub_800A70C`, `sub_800A718`,
  `sub_800A724`, `sub_800A730`

- `src/graphics/actor_part49.c` (new file, GitHub issue #9): `sub_800B270`
  - a per-frame velocity integrator moving `self+0x60`/`self+0x64`
  toward `self+0x50`/`self+0x5c` by `self+0x4c`/`self+0x58` each call,
  deriving a `self+0x24` direction-flag byte and applying the result to
  the object's position; see
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).

- `src/graphics/actor_part15.c`/`src/graphics/actor_part16.c` (new
  files, split around the raw untouched `sub_800B3F0` - see
  `docs/matching.md`): a new not-yet-named big object's accessors -
  `sub_800B324`, `sub_800B334`, `sub_800B33C`, `sub_800B360`,
  `sub_800B37C`, `sub_800B3AC`, `sub_800B4A4`, `sub_800B4AC`,
  `sub_800B4B8`, `sub_800B4C4`, `sub_800B4D0`, `sub_800B4F0`,
  `sub_800B4F8`, `sub_800B508`, `sub_800B510`, `sub_800B51C`,
  `sub_800B524`, `sub_800B53C`, `sub_800B544`, `sub_800B554`,
  `sub_800B55C`, `sub_800B564`, `sub_800B56C`, `sub_800B574`,
  `sub_800B57C`, `sub_800B584`, `sub_800B58C`, `sub_800B5A0`,
  `sub_800B5A8`, `sub_800B5B0`, `sub_800B5BC`, `sub_800B5C4`,
  `sub_800B5CC`, `sub_800B5D8`, `sub_800B5E0`, `sub_800B5E8`,
  `sub_800B5F0`, `sub_800B5FC`, `sub_800B608`, `sub_800B614`,
  `sub_800B620`, `sub_800B62C`, `sub_800B638`, `sub_800B644`,
  `sub_800B650`, `sub_800B678`, `sub_800B698`, `sub_800B69C`

- `src/graphics/actor_part17.c` (new file - see `docs/matching.md`):
  `sub_800B704`, `sub_800B734`, `sub_800B7B0`, `sub_800B838`,
  `nullsub_13`, `sub_800B86C`, `sub_800B8A4`, `sub_800B8A8`,
  `sub_800B8C8`, `sub_800B8D8`

- `src/graphics/actor_part18.c`/`actor_part18b.c` (new files, non-
  adjacent since the parked `sub_801434C` sits raw between them - see
  `docs/matching.md`, issue #17): `sub_801426C`, `sub_80142B0`,
  `sub_80144E0`, `sub_8014524` - four entries of the `gStaticData_0816BF20`
  42-slot action dispatch table

- `src/graphics/actor_part19.c`/`actor_part19c.c`/`actor_part19d.c`/
  `actor_part19f.c`/`actor_part19g.c` (new files, non-adjacent since
  three parked functions and one left-raw function sit between them -
  see `docs/matching.md`, issue #52): `sub_802BED8`, `sub_802BF30`,
  `sub_802BFA0`, `sub_802BFD4`, `sub_802C018`, `sub_802C078`,
  `sub_802C0A8`, `sub_802C0BC`, `sub_802C128`, `sub_802C14C`,
  `sub_802C19C`, `sub_802C264`, `sub_802C270`, `sub_802C394`,
  `sub_802C464`, `sub_802C4A4`, `sub_802C4C8`, `sub_802C540`,
  `sub_802C614`, `sub_802C6C0`, `sub_802C904` - the same large
  per-instance "self" object's action-table/trampoline/circular-list
  conventions as `actor_part17.c`/`actor_part18.c`

- `src/graphics/actor_aabb_setup.c` (new file, GitHub issue #70, ROM
  `0x0803AFDC`-`0x0803B060` - right after the parked division/modulo
  trio in `src/util/math_div_util.c`, see that file's `docs/matching.md`
  entry): `sub_803AFDC`/`sub_803AFE4` (the shared AABB set-size/
  set-position primitive already referenced by name from
  `actor_part.c`/`actor_part2.c`/`oam_count.c`), `sub_803AFEC` (a
  trivial raw-offset getter), `sub_803AFF0`/`sub_803B024` (two more
  `gStaticData_087E3BEC`-family per-type descriptor table constructors)

- `src/graphics/actor_part20.c`/`actor_part21.c`/`actor_part22.c`/
  `actor_part23.c`/`actor_part24.c`/`actor_part25.c`/`actor_part26.c`
  (new files, issue #58, ROM `0x08030334`-`0x08031784` - the boss-
  weapon effect state machine, non-adjacent since 18 raw functions sit
  between/around them; see
  [docs/matching/issue-58-0x08030334-actor.md](../matching/issue-58-0x08030334-actor.md)):
  `sub_8030530`, `sub_8030640`, `sub_80306A4`, `sub_8030C98`,
  `sub_80312C4`, `sub_803146C`, `sub_803171C`, `sub_8031744` - a
  countdown-timer state transition, a trivial byte setter/getter pair,
  a screen-accumulator/tracker-reset step, a BG2 zoom-effect updater,
  a "charge" countdown, and a palette flash/animation-refresh pair.
- `src/graphics/actor_part27.c` (new file, GitHub issue #22, ROM
  0x08017A44-0x08017AAC - numbered `27` rather than `20` since issue
  #58's parallel PR above independently claimed `actor_part20.c`-
  `actor_part26.c` first): `sub_8017A44`-`sub_8017AAC` (9 functions) -
  the same player/action-object family as `actor_part18.c`/
  `actor_part19.c` (`self+0xc` table pointer, `self+0x10` part
  pointer); see `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part27b.c` (new file, GitHub issue #22, ROM
  0x08017ECC-0x08017FE8, non-adjacent to `actor_part27.c` since the
  raw `sub_8017AB0` sits between them): `sub_8017ECC`, `sub_8017F14`,
  `sub_8017F5C`, `sub_8017F80`, `sub_8017FA4`, `sub_8017FD4`,
  `sub_8017FE8` - a `self+4` double-pointer-chain record lookup (same
  shape as `sub_800B704`/`sub_800B838`) feeding the
  `gStaticData_0816C2D8` per-vector-component trampoline table; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part27c.c` (new file, GitHub issue #22, ROM
  0x080187FC-0x08018884, non-adjacent to `actor_part27b.c` since the
  raw `sub_8018008`-`sub_80186F0` block sits between them):
  `sub_80187FC`, `sub_8018858`, `sub_801886C`, `sub_8018884`; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/graphics/actor_part28.c`/`actor_part30.c`/`actor_part32.c`/
  `actor_part34.c`/`actor_part36.c` (new files, GitHub issue #62, ROM
  0x08033804-0x08033EF4 - the `gUnknown_030015AC` singleton system's
  accessor/state-machine cluster, non-adjacent since 5 parked functions
  sit interleaved between them; see
  [docs/matching/issue-62-0x08033804-actor.md](../matching/issue-62-0x08033804-actor.md)):
  `sub_8033804`, `sub_8033828`, `sub_8033880`, `sub_803388C`,
  `sub_80338C4`, `sub_80338D0`, `sub_80338DC`, `sub_80338E8`,
  `sub_80338F4`, `sub_8033900`, `sub_803390C`, `nullsub_36`,
  `sub_803395C`, `nullsub_37`, `sub_8033AE0`, `sub_8033BB8`,
  `sub_8033BFC`, `sub_8033C28`, `sub_8033CF0`, `sub_8033E18` - the
  singleton's one-shot latches, field getters, state-transition/
  anim-frame-reset setters, an `InitActorPart`-based constructor, and
  several "self" object accessors/setters sharing the boss cluster's
  layout convention.
- `src/graphics/actor_part38.c` (new file, GitHub issue #18, ROM
  0x08014F8C - numbered `38` rather than `28` since issue #62's
  parallel PR above independently claimed `actor_part28.c` first):
  `sub_8014F8C` - a `gUnknown_030012F0`-list proximity-
  trigger scan for the same "self" action-table object family as
  `actor_part18.c`; see `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38b.c` (new file, GitHub issue #18, ROM
  0x080151C8, non-adjacent to `actor_part38.c` since the parked
  `sub_8015038` sits raw between them): `sub_80151C8`; see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38c.c` (new file, GitHub issue #18, ROM
  0x08015350-0x080156B4, non-adjacent to `actor_part38b.c` since the
  parked `sub_8015238`/`sub_80152F0` sit raw between them):
  `sub_8015350`, `sub_8015398`, `sub_80153FC`, `sub_8015460`,
  `sub_8015508`, `sub_8015558`, `sub_80155A8`, `sub_80155AC`,
  `sub_80155B8`, `sub_80155F8`, `sub_8015650`, `sub_8015690`,
  `sub_80156B4` - more of the same self+0xc/self+0x10 trampoline-pair
  family, including two near-identical self+0x29-keyed mgr-trampoline
  arms (`sub_8015460`) and several part+0x38-gated trampoline firers;
  see `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38d.c` (new file, GitHub issue #18, ROM
  0x0801574C-0x08015780, non-adjacent to `actor_part38c.c` since the
  parked `sub_80156EC` sits raw between them): `nullsub_17`,
  `sub_8015750`, `nullsub_18`, `sub_8015774`, `sub_8015780` - two
  nullsubs, two tail-call wrappers, and the shared trampoline-pair-
  plus-sentinel-store helper called by `actor_part18.c`'s
  `sub_801426C`/`sub_80142B0`; see
  `docs/matching/issue-18-0x08014f8c-actor.md`.

- `src/graphics/actor_anim.c` (extended, GitHub issue #71, ROM
  `0x0803B060`-`0x0803B46C` - immediately adjacent to the file's existing
  `GetAnimFrameBaseOffset`, which itself ends exactly at `0x0803B060`):
  `sub_803B060` (reads the current keyframe's `attr` halfword pre-shifted
  into the high 16 bits), `GetAnimFrameData` (resolves the current
  keyframe's tile-graphics pointer via `frameOffsets`/`gUnknown_0300137C`),
  `sub_803B0A8` (selects a new keyframe, resetting the playback
  accumulator), `sub_803B0F0` (advances a Q8 fall/scroll accumulator,
  then either fires the `self+0x50` trampoline or tail-calls
  `sub_802A7B8`), and 20 byte-identical `gStaticData_087E4DF4` "kind"
  teardown handlers (`sub_803B0C4`, `sub_803B128`, `sub_803B154`,
  `sub_803B180`, `sub_803B1AC`, `sub_803B1D8`, `sub_803B204`,
  `sub_803B230`, `sub_803B25C`, `sub_803B288`, `sub_803B2B4`,
  `sub_803B2E0`, `sub_803B30C`, `sub_803B338`, `sub_803B364`,
  `sub_803B390`, `sub_803B3BC`, `sub_803B3E8`, `sub_803B414`,
  `sub_803B440` - unlink `self` from its `+0x48`/`+0x4c` circular list,
  set `+0x50` to the shared "dead" vtable, and conditionally free) - see
  [docs/matching/issue-71-0x0803b060-actor.md](../matching/issue-71-0x0803b060-actor.md).
- `src/graphics/actor_part39.c` (new file, GitHub issue #16, ROM
  0x080119A8-0x08011BD4): `sub_80119A8`, `sub_80119D4`, `sub_80119D8`,
  `sub_80119EC`, `sub_80119FC`, `sub_8011A1C`, `sub_8011A50`,
  `sub_8011A64`, `sub_8011A84`, `sub_8011A8C`, `sub_8011B0C`,
  `nullsub_16`, `sub_8011B5C`, `sub_8011B70`, `sub_8011B90` - a run of
  `struct actor` vtable-swap constructor helpers (same
  `sub_80084A4`/`sub_8008484`/`nullsub` shape as `actor_part6.c`), a
  handful of small setters/getters on offsets beyond `struct actor`'s
  own 0x1c bytes, and a distance-gate (`sub_8011A8C`) reusing
  `actor_part2.c`'s `gUnknown_030012B4+0x108` bitmap idiom verbatim.
  Recategorized `graphics`->`actor` from the issue's label: every
  matched function here operates on `struct actor` via the same
  `table@0x18`/`flags@0xc`/`field_08@8` layout `actor_part*.c` already
  established, not the `game_loop`-core "child object" family
  `docs/rom_map.md` traces through the chunk's remaining (unmatched)
  functions. See
  [docs/matching/issue-16-actor-11b0c.md](../matching/issue-16-actor-11b0c.md).
- `src/graphics/actor_part43.c`/`actor_part44.c`/`actor_part45.c`/
  `actor_part46.c` (new files, GitHub issue #56, ROM
  0x0802F0DC-0x0802FBF0 - a second boss-weapon "spawn/pre-attack"
  singleton and its `self` object, non-adjacent since the left-raw
  `sub_802F164`/`sub_802F7B0`/`sub_802F8E8`/`sub_802FA38` and the
  parked `sub_802F338`/`sub_802F748`/`sub_802F97C`/`sub_802FA04` sit
  interleaved between them; see
  [docs/matching/issue-56-0x0802f0dc-actor.md](../matching/issue-56-0x0802f0dc-actor.md)):
  `sub_802F0DC`, `sub_802F3BC`, `sub_802F46C`, `sub_802F47C`,
  `sub_802F4AC`, `sub_802F4C0`, `sub_802F4CC`, `sub_802F50C`,
  `sub_802F540`, `sub_802F570`, `sub_802F5AC`, `sub_802F5E4`,
  `sub_802F640`, `sub_802F69C`, `sub_802F6DC`, `sub_802F7A4`,
  `sub_802FA34` - a constructor/reset, an accumulator-drain/reward-
  dispenser, accessors, accumulator drivers, idle-state-reset idioms,
  and the singleton's teardown/destructor, all sharing
  `actor_part17.c`/`actor_part18.c`/`actor_part20.c`'s established
  "self" object conventions.
- `src/graphics/actor_part50.c`/`actor_part52.c`/`actor_part54.c`/
  `actor_part56.c` (new files, GitHub issue #50, ROM
  0x0802A69C-0x0802AC28 - numbered `50`/`52`/`54`/`56` rather than
  `39`/`41`/`43`/`45` since issues #16 and #56's parallel PRs above
  independently claimed those numbers first, non-adjacent since the
  parked `UpdateAnimatedActorPart`/`sub_802AA0C`/`sub_802AB58` sit
  interleaved between them; see
  [docs/matching/issue-50-actor-2a69c.md](../matching/issue-50-actor-2a69c.md)):
  `sub_802A69C`, `sub_802A6B0`, `sub_802A6C4`, `sub_802A6D8`,
  `sub_802A6EC`, `InitActorPart`, `sub_802A7B8`, `sub_802A980`,
  `sub_802A9D4`, `sub_802A9DC`, `sub_802AA00`, `sub_802AA04`,
  `sub_802AA08`, `sub_802AA4C`, `sub_802AA54`, `sub_802AA80`,
  `sub_802AAB4`, `sub_802AAFC`, `sub_802AB08`, `sub_802AB34`,
  `sub_802ABC8`, `sub_802ABFC` - the `InitActorPart` constructor itself
  (previously only forward-declared by every other `actor_part*.c`
  file), its movement-threshold recompute pair, the fixed 15-slot
  object registry (`gUnknown_03001428`/`gUnknown_03000888`), and the
  `gUnknown_03001464`-gated palette-cycle DMA cluster's non-parked
  members.

- `src/graphics/actor_part57.c` (new file, GitHub issue #19, ROM
  0x08015840-0x080159A4 - recategorized `graphics`->`actor` from the
  issue's label, same self+0xc/self+0x10 trampoline-pair and state/
  counter/table-index-trio family as actor_part38c.c/actor_part38d.c;
  non-adjacent to actor_part38d.c's matched span since the parked
  `sub_80157C4` sits between them): `sub_8015840`, `sub_8015878`,
  `sub_801588C`, `sub_80158AC`, `sub_80158B4`, `sub_80158BC`,
  `sub_80158C4`, `sub_80158CC`, `sub_80158D4`, `sub_80158DC`,
  `sub_80158E4`, `sub_80158EC`, `sub_80158F4`, `sub_8015908`,
  `sub_8015920`, `sub_8015938`, `sub_8015950`, `sub_8015958`,
  `sub_80159A4` - a run of small accessors/resetters on the state-trio
  bytes, the `gStaticData_087E4224` double-table-set idiom already seen
  in `actor_part27.c`, and a larger field-reset pair; see
  [docs/matching/issue-19-0x08015840-actor.md](../matching/issue-19-0x08015840-actor.md).
- `src/graphics/actor_part57b.c` (new file, GitHub issue #19, ROM
  0x08015FDC, non-adjacent to actor_part57.c since the left-raw
  `sub_80159F8`/`sub_8015C6C`/`sub_8015DF8` sit between them):
  `sub_8015FDC` - a player-velocity-relative record writer; see
  [docs/matching/issue-19-0x08015840-actor.md](../matching/issue-19-0x08015840-actor.md).
- `src/graphics/actor_part58.c` (new file, GitHub issue #54, non-
  adjacent to `actor_part56.c` since the whole 0x0802D3A8-0x0802E0A4
  range sits between them; numbered `58` rather than `57` since issue
  #19's PR independently claimed `actor_part57.c`/`57b.c` first - see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802D490`, `sub_802D4B0`, `sub_802D4EC`, `sub_802D528`,
  `sub_802D57C`, `sub_802D590`, `sub_802D59C`, `sub_802D5D4`,
  `sub_802D600`, `sub_802D648`, `sub_802D6A0`, `sub_802D764` -
  `InitActorPart`-based constructor variants plus the
  `gUnknown_030012C0+0x78` Aku-Aku-mask-style add/remove pair.
- `src/graphics/actor_part74.c` (new file, GitHub issue #54, ROM
  0x0802D7B0-0x0802DA84, sits between `actor_part58.c` and
  `actor_part59.c`; see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802D7B0`, `sub_802D9A8`, `sub_802DA68` - a confirmed slot (index
  3) of the type-0 `category_vtable` (also runs a full 3-axis AABB
  overlap test against the player before dispatching a `sub_803AD80`
  trampoline) and a palette-gradient/hardware-sound cursor pair for the
  `gUnknown_030014BC` object; all three NAKED-transcribed.
- `src/graphics/actor_part59.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part74.c` sits between it and `actor_part58.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802DB2C`, `sub_802DCC0` - the `gUnknown_030014BC` position-
  tracking object's two `gStaticData_0817A840` vtable-slot update
  functions (accumulate/clamp, tier-keyed `PlaySfx`/`sub_80019F8`
  cues, and a shared kind/anim-reset transition tail).
- `src/graphics/actor_part75.c` (new file, GitHub issue #54, ROM
  0x0802DD9C-0x0802E058, sits between `actor_part59.c` and
  `actor_part60.c`; see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802DD9C`, `sub_802DE70` - the self-vs-player 3-axis AABB overlap
  test factored out of `sub_802D7B0` (used by `sub_802D6A0`,
  actor_part58.c) and the `gUnknown_030014BC` object's ~160-instruction
  VRAM gauge-tile bitmap generator/DMA setup; both NAKED-transcribed.
- `src/graphics/actor_part60.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part75.c` sits between it and `actor_part59.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802DFBC`, `sub_802DFC8`, `sub_802DFDC` - the
  `gUnknown_030014BC` object's state-flag setter, destructor, and
  constructor.
- `src/graphics/actor_part76.c` (new file, GitHub issue #54, ROM
  0x0802E058, sits between `actor_part60.c` and `actor_part61.c`; see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802E058` - a parameterized twin of `sub_802DE70`'s VRAM gauge-tile
  triangular-fill loop; NAKED-transcribed.
- `src/graphics/actor_part61.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part76.c` sits between it and `actor_part60.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `nullsub_27` - a genuine no-op stub.
- `src/graphics/actor_part62.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part76.c` sits between it and `actor_part61.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802D3A8` (eases `self`'s cached position toward a per-state
  target/table-scatter offset, NAKED - hit this project's confirmed
  categorical gcc-2.9 r7-pin bug, transcribed instruction-for-instruction
  from the ROM disassembly instead).
- `src/graphics/actor_part63.c`/`actor_part65.c`/`actor_part67.c`/
  `actor_part69.c`/`actor_part71.c`/`actor_part73.c` (new files, GitHub
  issue #63, ROM 0x08033EF4-0x08034AA4 - three `InitActorPart`-rooted
  "self" object kinds immediately following issue #62's cluster, non-
  adjacent since 6 parked and 4 left-raw functions sit interleaved
  between them; numbered `63`-`73` rather than `57`-`67` since issues
  #19 and #54's PRs independently claimed `actor_part57.c`-`62.c`
  first - see
  [docs/matching/issue-63-0x08033ef4-actor.md](../matching/issue-63-0x08033ef4-actor.md)):
  `sub_8033EF4`, `sub_8033F48`, `sub_8033F74`, `sub_8034050`,
  `sub_8034110`, `sub_8034188`, `sub_80341F8`, `sub_8034264`,
  `nullsub_38`, `sub_80342D4`, `sub_803436C`, `sub_8034688`,
  `sub_80346C8`, `sub_80346FC` - two constructors, a trampoline-fire
  helper, a position-sync/state-transition helper, a damage/death
  handler, a position-sync/orbit-effect updater and its non-identical
  near-twin, two trivial getters, a no-op stub, a particle-spawn-budget
  driver, an input-poll busy-wait, and a buffer-release/teardown helper.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_800A528`/`sub_800A590`** (`src/graphics/actor_part47.c`,
  GitHub issue #9; real bytes in `asm/code_3_2_11_a528.s`) - a moving-
  platform "ride along" hookup, nudging `self->y` by the delta between
  a cached and current position-record lookup. Matches the ROM's
  register roles for `self`/the record pointer exactly; parked on a
  genuine fifth-scratch-register need (`r5`, just to hold an offset
  immediate) this compiler never introduces. See
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).
- **`sub_800A734`/`sub_800A810`** (`src/graphics/actor_part48.c`,
  GitHub issue #9; real bytes in `asm/code_3_2_16_a734.s`) - a part-
  object velocity/state reset pair, one of which also hooks up a child
  object and one of which dispatches a sub-state byte to one of three
  teardown helpers. Field writes and dispatch semantics fully
  confirmed; parked on the ROM's running-pointer address-increment
  style (`sub_800A734`) and an exact compare-chain-vs-register-letter
  tradeoff (`sub_800A810`) neither reproduced together by any C
  phrasing tried. See
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).
- **`sub_800B6A0`/`sub_800B6D0`** (`src/graphics/actor_part16.c`) -
  mirror-flag-gated 3-vector copies. This compiler unconditionally
  spills the `vec` pointer to a callee-saved register (`push
  {r4,lr}`/`pop {r4}`) whenever it's referenced in both branches of an
  if/else, even with nothing to clobber it - the ROM is a true leaf
  function using only r0-r3. Three independent fixes (pinning `self`
  alone; also pinning/reassigning `vec`; restructuring into a
  `goto`-based flow) all produced an identical 8-byte-larger result -
  see `docs/matching.md`, "A new unnamed object:
  `actor_part15.c`/`actor_part16.c`".
- **`sub_8007B00`/`sub_8007B98`** (`src/graphics/actor_part.c`),
  **`sub_8008044`** (`src/graphics/actor_part3.c`),
  **`sub_8008188`/`sub_8008200`/`sub_8008278`**
  (`src/graphics/actor_part4.c`), **`sub_80083B8`**
  (`src/graphics/actor_part5.c`), **`sub_8008770`**
  (`src/graphics/actor_part6.c`), and
  **`sub_800891C`/`sub_8008A40`/`sub_8008AD8`**
  (`src/graphics/actor_part7.c`) / **`sub_8008D80`**
  (`src/graphics/actor_part7b.c`) - stayed parked here for a long time
  on exactly the register-canonicalization/stack-layout classes of gap
  documented at length throughout this page (`r7`-pin hazards, a
  stubborn `add`-operand-order canonicalization, a compiler-elided
  redundant byte truncation, and a C-inexpressible stack-layout
  coincidence). All ten are now `NAKED` functions whose bodies are a
  literal instruction-for-instruction transcription of the ROM's own
  assembly (byte-exact, confirmed via a full clean `make compare`),
  rather than a derived C reconstruction - see
  `docs/matching/naked-oam-actor-part-batch.md`. Per project policy, a
  NAKED transcription standing in for a substantial function's
  register-allocation gap doesn't count as "matched" the way real
  decompiled C does, so all ten stay filed here rather than in
  "Matched" above, and `tools/report_units.py` tracks their address
  ranges as unmatched (`base_object: None`).
- **`sub_8008F20`** (`src/graphics/actor_part11.c`) - initializes a
  fixed-slot object-pool manager struct: two big 256-word zeroed
  tables (likely a pair of spatial-partition/collision grids), plus a
  singly-linked free list built over an allocated node array. Every
  load, store, and field offset confirmed correct; parked purely on a
  many-register (item count, two persistent field addresses, a reused
  loop index, a running byte offset) allocation gap across
  `r3`/`sb`/`sl`/`r4`/`r8` - see `docs/matching.md`, "Parked, not
  matched: `sub_8008F20`".
- **`sub_8009150`** (`src/graphics/actor_part11.c`) - lazily creates a
  "large object" bucket-255 grid registration for an object that
  didn't get one at insert time. Every load, store, and field offset
  confirmed correct; parked purely on a loop-invariant-hoisting gap (a
  free-list-head address computation this compiler correctly hoists
  out of a 255-iteration loop, where the ROM recomputes it fresh every
  non-empty bucket) - see `docs/matching.md`, "Parked, not matched:
  `sub_8009150`".
- **`sub_800944C`** (`src/graphics/actor_part11.c`) - the same
  "extended screen box" filter shape as `sub_8008C80`, but iterating
  the spatial hash grid directly and firing per-object trampolines
  instead of building a second array. Every load, store, and field
  offset confirmed correct; parked purely on a single register-reuse
  choice (`bucket = baseIdx + 2` computed in-place instead of into a
  fresh register) - see `docs/matching.md`, "Parked, not matched:
  `sub_800944C`".
- **`sub_8009528`** (`src/graphics/actor_part11.c`) - the spatial-
  hash-grid-cluster analog of `sub_8008A40`: the same grid-iteration
  shape as `sub_800944C`, dispatching each hit to `sub_80096C0`/
  `sub_80099F0` exactly like `sub_8008A40` dispatches to
  `sub_8008AD8`/`sub_8008D80`. Every branch, field offset, and call
  argument is semantically confirmed; parked on a stack-frame/register
  gap larger than the established `boxH` issue alone, not chased
  further given the size of the remaining cluster - see
  `docs/matching.md`, "Parked, not matched: `sub_8009528`".
- **`sub_80096C0`** (`src/graphics/actor_part11.c`) - `sub_8008AD8`'s
  twin: byte-identical collision-hit resolution logic, operating in
  this spatial-hash-grid cluster instead of the plain array manager.
  Parked on the same `boxH` stack-layout gap as `sub_8008AD8`/
  `sub_8008D80`/`sub_80099F0` - see `docs/matching.md`, "Parked, not
  matched: `sub_80096C0`".
- **`sub_8009914`** (`src/graphics/actor_part11.c`) - resets a pool
  manager to empty: tears down every active object, then rebuilds the
  grid and free list from scratch. The teardown loop is confirmed
  correct; the rebuild loop is a byte-for-byte copy of `sub_8008F20`'s
  own tail and hits the identical many-register allocation gap - see
  `docs/matching.md`, "Parked, not matched: `sub_8009914`".
- **`sub_80099F0`** (`src/graphics/actor_part12.c`) - `sub_8008D80`'s
  twin: byte-identical in shape (same collision-hit-resolve logic,
  same "dead read" trampoline call), called from elsewhere in this
  cluster. Parked on the same `boxH` stack-layout gap - see
  `docs/matching.md`, "Parked, not matched: `sub_80099F0`".
- **`sub_8009D5C`** (`src/graphics/actor_part13.c`) - fires a
  `part->table+0x68`-driven trampoline based on `gUnknown_030012C0`'s
  mode, on the player and/or `part` depending on the mode value.
  Every branch, call, and argument confirmed correct (a `switch`
  reproduces the ROM's exact 3-way mode dispatch, and explicit `goto`s
  into a shared, ABI-register-pinned tail reproduce the mode-0/mode-
  1-2 call sharing); parked on a single remaining conditional-branch
  encoding gap in the mode-3 case - see `docs/matching.md`, "Parked,
  not matched: `sub_8009D5C`".
- **`sub_8009DF4`** (`src/graphics/actor_part8.c`) - a velocity/
  position integrator: steps each axis's velocity toward its max by
  its accel amount (clamped so it never overshoots), builds a
  direction-flags byte from the clamped velocities' signs, caches the
  pre-move position, applies the velocity, and updates a global with
  the Y velocity. Every branch and memory access confirmed correct;
  parked purely on a leaf-vs-non-leaf register-budget gap (the ROM
  needs no stack frame at all, fitting entirely in r0-r3 with `self`
  in r2 reused once dead; every arrangement tried here needs one extra
  spilled register) - see `docs/matching.md`, "Parked, not matched:
  `sub_8009DF4`".
- **`sub_801434C`** (`asm/code_3_2_17_1434c.s`, C in
  `src/graphics/actor_part18.c`) - the shared handler
  `sub_80142B0` tail-calls; one of the `gStaticData_0816BF20` action-
  table entries. Every load/store, branch and call confirmed correct,
  including the ROM's case-`0`/`2`-before-case-`1` switch layout and
  its shared `sub_803AD84` tail call; parked purely on instruction-
  *scheduling* for a handful of mutually-independent instructions in
  the closing `masked = *(u16 *)&snap & 0x180` block (right
  address/constant/load ordering, wrong relative order) - see
  `docs/matching.md`, issue #17, for everything tried.
- **`sub_80145E4`** (`asm/code_3_2_17_145e4.s`, C in
  `src/graphics/actor_part18b.c`) - same shape as the matched
  `sub_8014524` (boolean/raw-value bit test, `sub_8015780` reset
  block) but keeps the raw masked bit value rather than a `!= 0`-
  normalized boolean. Every load/store and branch confirmed correct;
  parked on a single materialize-then-copy gap in the opening bit-test
  triggered by a required nested `if` sharing the value's live range
  across both branches - see `docs/matching.md`, issue #17.
- **`sub_802C208`** (`asm/code_3_2_20_28568_c208.s`, C in
  `src/graphics/actor_part19e.c`) - a `gStaticData_0817A6B8` stride-8
  trampoline-record dispatcher. Every load/store, branch and call
  confirmed correct; parked on register-allocation/instruction-
  scheduling around two `record = base + state*8` re-derivations - see
  `docs/matching.md`, issue #52.
- **`sub_802C2FC`** (`asm/code_3_2_20_28568_c2fc.s`, C in
  `src/graphics/actor_part19b.c`) - OAM setup for one sprite frame.
  Matches instruction-for-instruction except a single dead `flag = 0`
  initializer this compiler's dead-store elimination always removes -
  see `docs/matching.md`, issue #52.
- **`sub_802C3E8`** (`asm/code_3_2_20_28568_c3e8.s`, C in
  `src/graphics/actor_part19c2.c`) - a homing/seek-toward-point spawn-
  effect constructor. Every field access and call confirmed correct;
  parked on this compiler's register choice for a couple of
  intermediate abs-value-computation values - see `docs/matching.md`,
  issue #52.
- **`sub_80339DC`** (`asm/code_3_2_20_28568_c99c_31784_339dc.s`, C in
  `src/graphics/actor_part29.c`) - a proximity-triggered effect/hazard
  detector measuring `self`'s distance to the player after syncing to
  the singleton's position. Every load/store, branch and call
  confirmed correct; parked on a residual register-allocation gap for
  one 16-bit constant materialization this compiler won't place in the
  ROM's chosen scratch register without breaking the surrounding
  `ip`/`r8`/`r9` pins - see
  `docs/matching/issue-62-0x08033804-actor.md`, issue #62.
- **`sub_8033B44`** (`asm/code_3_2_20_28568_c99c_31784_33b44.s`, C in
  `src/graphics/actor_part31.c`) - position-update-then-draw helper via
  a `gStaticData_0817C4E0` stride-8 trampoline table, the same shape
  and register-allocation gap as the already-parked `sub_802C208` - see
  `docs/matching/issue-62-0x08033804-actor.md`, issue #62.
- **`sub_8033C84`** (`asm/code_3_2_20_28568_c99c_31784_33c84.s`, C in
  `src/graphics/actor_part33.c`) - `sub_8033B44`'s predicate twin,
  parked on the identical gap - see
  `docs/matching/issue-62-0x08033804-actor.md`, issue #62.
- **`sub_8033CF8`** (`asm/code_3_2_20_28568_c99c_31784_33cf8.s`, C in
  `src/graphics/actor_part35.c`) - `sub_80339DC`'s sibling proximity/
  spawn detector. Every branch and call confirmed correct; parked
  because this agbcc build never emits a callee-save push/pop for a
  plain low-register (`r0`-`r7`) `register` variable used across a
  call unless another high register is *also* live in the same
  function (confirmed with an isolated test) - pinning `self+0x64`'s
  cache to `r7` here (matching the ROM) would silently corrupt the
  caller's `r7` - see `docs/matching/issue-62-0x08033804-actor.md`,
  issue #62.
- **`sub_8033E80`** (`asm/code_3_2_20_28568_c99c_31784_33e80.s`, C in
  `src/graphics/actor_part37.c`) - `sub_8033B44`'s twin using the
  second stride-8 table (`gStaticData_0817C4F8`), parked on the same
  gap - see `docs/matching/issue-62-0x08033804-actor.md`, issue #62.
- **`sub_8015038`** (`asm/code_3_2_17_15038.s`, C in
  `src/graphics/actor_part38.c`) - a three-arm mgr-trampoline handler
  keyed on `self+0x24`/`self+0x22`, picking one of three table-index
  fallbacks. Every load/store, branch and call is understood and
  semantically correct; parked on this compiler's register allocation
  across the three near-identical arms (it won't keep the computed
  `self+0x21`/`self+0x22` field addresses in the ROM's own `r7`/`r5`
  once real trampoline calls intervene) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_8015238`** (`asm/code_3_2_17_15238.s`, C in
  `src/graphics/actor_part38b.c`) - `self+0x26`/`mode`/`flags`-gated
  mgr-trampoline dispatcher. Every load/store, branch and call is
  correct, in the right order, and in the right registers - parked
  purely on the two parameter home-copies at function entry (this
  compiler always truncates `mode` before copying `self`, the ROM does
  the opposite, and neither order nor register pins nor hand-written
  `asm volatile` copies could override the compiler's own fixed
  parameter-home-copy prologue pass) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80152F0`** (`asm/code_3_2_17_15238.s`, C in
  `src/graphics/actor_part38b.c`) - `self+0x27`/`self+0x2b`/`mode`-
  gated state/counter/table-index trio reset, tail-calling
  `sub_80122CC`. Every load/store, branch and call confirmed correct
  and in the right order; parked on a single instruction (a `+6` byte
  offset folds into a `strb`'s own addressing mode where the ROM keeps
  it as a separate `adds`) - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80156EC`** (`asm/code_3_2_17_156ec.s`, C in
  `src/graphics/actor_part38c.c`) - `part+0x38`/`sub_80231BC`-gated
  mgr-trampoline dispatcher. Every load/store, branch and call
  confirmed correct; parked on the `else` arm recomputing `self` into
  a fresh register (an extra push/pop this compiler insists on once
  its own `mgr` local is redeclared in that arm) where the ROM reuses
  the same `self` register the whole function already lives in - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80157C4`** (`asm/code_3_2_17_157c4.s`, C in
  `src/graphics/actor_part38d.c`) - player's `+0x100`-flag-gated
  `mode` remapper (a 3-way dispatch playing a fixed cue via
  `sub_80019A8`/`PlaySfx`), tail-calling `sub_800B86C`. Every load/
  store, branch and call is understood and semantically correct;
  parked on register allocation across the 3-way dispatch - see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_802F338`** (`src/graphics/actor_part43b.c`) - computes two
  keyframe-driven tile-cache sizes via `sub_8028CD4`. Every load/store,
  branch and call confirmed correct; parked on a "materialize the
  multiply result, then copy it again before shifting" gap this
  compiler's dead-store elimination always collapses - see
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802F748`** (`src/graphics/actor_part44b.c`) - a
  `gStaticData_0817C1C0` stride-8 trampoline-record dispatcher, same
  shape as the parked `sub_802C208`; parked on the same
  `record = base + state*8` re-derivation register-allocation gap -
  see `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802F97C`** (`src/graphics/actor_part45b.c`) - a physics-step-
  and-collision-react updater. Every load/store, branch and call
  confirmed correct; parked on a residual `r2`-vs-`r3` register choice
  for a repeated `8` immediate - see
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802FA04`** (`src/graphics/actor_part45c.c`) - an
  `InitActorPart`-based 7-argument constructor, the same shape as the
  left-raw `sub_80305F8`; parked on this compiler's own high-register
  push/pop allocation never matching the ROM's `r4=self,r5=1,r6=e,r7=f`
  assignment - see `docs/matching/issue-56-0x0802f0dc-actor.md`.

- **`sub_803B46C`** (`src/graphics/actor_anim.c`, GitHub issue #71) -
  fixed-position (120, 106) OAM setup for one sprite frame: screen-space
  visibility cull, then builds the OAM attribute words (masked position,
  `sub_803B060`'s attr flag, and a priority/palette nibble from
  `self+0x18`/`self+0x14`) and calls `SetupSpriteFrameOam`. Near-
  identical twin of the already-parked `sub_802C2FC`
  (`actor_part19b.c`) - hits the same two gaps: a `| 0`-with-a-zero-
  valued-term this compiler's dead-store elimination always removes
  (the ROM keeps a real materialize-and-OR pair) and a register-budget
  difference needing an extra spilled/high register to keep `frame`
  alive across both calls where the ROM fits entirely in r4-r7 - see
  [docs/matching/issue-71-0x0803b060-actor.md](../matching/issue-71-0x0803b060-actor.md).
- **`UpdateAnimatedActorPart`** (`asm/code_3_2_20_8b7c_a88c.s`, C in
  `src/graphics/actor_part55.c`) - the OAM draw/scale routine for the
  `InitActorPart`-constructed "self" object. Every byte of this
  ~120-instruction function matches except one `frame[1]` read the ROM
  serves from a leftover, never-reloaded copy of `GetAnimFrameData`'s
  return value still sitting in `r0` - a redundant-load/value-reuse
  optimization this agbcc build doesn't perform - see
  `docs/matching/issue-50-actor-2a69c.md`.
- **`sub_802AA0C`** (`asm/code_3_2_20_8b7c_aa0c.s`, C in
  `src/graphics/actor_part51.c`) - a 12-byte little-vector velocity
  integrator. Every load/store confirmed correct (including the ROM's
  own `ldm`/`stm` 3-word block-copy idiom at both ends); parked purely
  on instruction scheduling around the three per-axis `>>8` shifts
  between the two block copies - see
  `docs/matching/issue-50-actor-2a69c.md`.
- **`sub_802AB58`** (`asm/code_3_2_20_8b7c_ab58.s`, C in
  `src/graphics/actor_part53.c`) - the palette-cycle cursor-advance DMA
  step. Every load/store, branch and call confirmed correct; parked
  because this compiler speculatively computes the cursor's decrement
  ahead of the branch that decides whether it's needed, folding away a
  redundant unconditional jump the ROM's own build still has - see
  `docs/matching/issue-50-actor-2a69c.md`.
- **`sub_8033FE4`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_33fe4.s`, C
  in `src/graphics/actor_part64.c`, GitHub issue #63) - a
  `gStaticData_0817C4F8` stride-8 trampoline-record dispatcher, same
  shape as the parked `sub_8033B44`/`sub_8033C84`; parked on the same
  `record = base + state*8` re-derivation gap - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034058`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s`, C
  in `src/graphics/actor_part66.c`, GitHub issue #63) - an
  `InitActorPart`-based constructor with a trailing byte stack argument;
  parked because this compiler reads that argument as a shifted/masked
  full word where the ROM's build addresses it directly with `ldrb` -
  see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034270`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34270.s`, C
  in `src/graphics/actor_part68.c`, GitHub issue #63) - a position-sync/
  flag/trampoline updater; parked on this compiler's dead-branch
  elimination collapsing a redundant compute-then-recheck step the
  ROM's own build still has - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034314`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34314.s`, C
  in `src/graphics/actor_part70.c`, GitHub issue #63) - `sub_8034270`'s
  boolean-returning twin, parked on the identical gap - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_80345B0`/`sub_8034634`**
  (`asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s`, C in
  `src/graphics/actor_part72.c`, GitHub issue #63) - a particle-slot
  spawner and a 4-bit tilemap nibble writer; parked on register-
  allocation differences for the 16-bit table lookups and an
  unconditional leaf-function parameter spill - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.

## Left raw (not attempted, or attempted and set aside)

- **`sub_8017AB0`** (`asm/code_3_2_17_17ab0.s`, ROM 0x08017AB0-
  0x08017ECC, GitHub issue #22) - a ~500-instruction player-vs-camera-
  viewport state dispatcher; left raw, out of scope for this pass - see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- **`sub_8018008`/`sub_8018400`/`sub_801865C`/`sub_80186F0`**
  (`asm/code_3_2_17_18008.s`, ROM 0x08018008-0x080186F0, GitHub issue
  #22) - a ~480-instruction jump-table player action-state machine
  plus two high-register-pressure helpers it calls; left raw, out of
  scope for this pass - see `docs/matching/issue-22-0x08017a44-actor.md`.
- **`sub_802F164`** (`asm/code_3_2_20_28568_c99c_2f164.s`, GitHub issue
  #56) - a ~160-instruction state-machine update for the "spawn/pre-
  attack" singleton, including a `mov pc, r0` computed-goto 5-case
  jump table; left raw, out of scope for this pass - see
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802F7B0`**/**`sub_802F8E8`**
  (`asm/code_3_2_20_28568_c99c_2f7b0.s`, GitHub issue #56) - a pair of
  ~130-170-instruction VRAM tile-remap loops with heavy `sb`/`sl`/`r8`
  register pressure; left raw, out of scope for this pass - see
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802FA38`** (`asm/code_3_2_20_28568_c99c_2fa38.s`, GitHub issue
  #56) - a ~150-instruction position-update/collision-damage function
  with heavy `sb`/`r8` register pressure; left raw, out of scope for
  this pass - see `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_8011BD4`/`sub_8012160`/`sub_8012238`/`sub_80122CC`/
  `sub_8012420`/`sub_8012694`/`sub_801283C`/`sub_8012A7C`/
  `sub_8012AF4`/`sub_8012D24`** (`asm/code_3_2_17_11bd4.s`, ROM
  0x08011BD4-0x08012FBC, GitHub issue #16) - `sub_8011BD4` itself is
  `docs/rom_map.md`'s documented ~1420B, 25-case/7-case nested
  jump-table companion state machine to `sub_8016288` (still raw,
  type-`0x1d` player-control family); the rest are further members of
  the 42-slot action-dispatch-table family (`gStaticData_0816BF20`)
  reading/writing a still-unnamed "child object" struct (`self+0xc`/
  `+0x10`/`+0x18` sub-record pointers, distinct from `struct actor`)
  that `docs/rom_map.md` itself says isn't understood with byte-exact
  precision yet. Left raw, out of scope for this pass - see
  [docs/matching/issue-16-actor-11b0c.md](../matching/issue-16-actor-11b0c.md).
- **`sub_8007634`** (`asm/code_3_2.s`, ROM 0x08007634, GitHub issue #9)
  - real GBA hardware-affine sprite-matrix setup; already flagged in
  `docs/matching.md` as needing "a dedicated session" of its own, not
  attempted again here - see
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_8009008`/`sub_80091D4`/`sub_8009868`** (`asm/code_3_2_13.s`,
  ROM 0x08009008-0x08009914, GitHub issue #9) - spatial-hash-grid
  removal/list-management logic and a function calling into the
  still-mostly-raw physics/collision subsystem; each individually
  understood mechanically but not to a byte-exact-reconstruction
  precision - see `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_8009BE0`** (`asm/code_3_2_14.s`, ROM 0x08009BE0, GitHub issue
  #9) - a physics/collision step-probe calling still-unexamined
  helpers - see `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_800A0FC`/`sub_800A178`/`sub_800A420`** (`asm/code_3_2_11.s`,
  ROM 0x0800A0FC-0x0800A5F4, GitHub issue #9) - part-object
  update/collision dispatchers built on unmatched
  `sub_8008200`/`sub_8026628`/`sub_8026C3C`/`sub_8026BF8` - see
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_800A884`/`sub_800AAEC`/`sub_800AB9C`/`sub_800AC2C`/
  `sub_800AFF4`** (`asm/code_3_2_16.s`, ROM 0x0800A884-0x0800B270,
  GitHub issue #9) - a reentrancy-guard wrapper, a global-list iterator,
  a hitbox-lookup dispatcher, a 38-case player action-state machine,
  and a high-register-pressure hitbox commit function; each calls one
  or more still-unexamined helpers - see
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_80159F8`/`sub_8015C6C`/`sub_8015DF8`** (`asm/code_3_2_17_159f8.s`,
  ROM 0x080159F8-0x08015FDC, GitHub issue #19) - three large jump-table
  state-machine dispatchers on the part object's velocity fields; left
  raw, out of scope for this pass - see
  `docs/matching/issue-19-0x08015840-actor.md`.
- **`sub_8016048`** (`asm/code_3_2_17_16048.s`, ROM 0x08016048, GitHub
  issue #19) - a smaller joystick-input-gated dispatcher; left raw, out
  of scope for this pass - see
  `docs/matching/issue-19-0x08015840-actor.md`.
- **`sub_8034374`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`,
  ROM 0x08034374, GitHub issue #63) - a ~150-instruction graphics/
  palette/DMA setup routine with an apparent uninitialized-local read
  partway through; left raw, out of scope for this pass - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034480`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`,
  ROM 0x08034480, GitHub issue #63) - a 128-entry OAM/screen-box scan
  driving `sub_8034634`'s tilemap writer; left raw alongside
  `sub_8034374`, out of scope for this pass - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_803472C`/`sub_803487C`/`sub_8034994`**
  (`asm/code_3_2_20_28568_c99c_31784_33ef4_3472c.s`, ROM
  0x0803472C-0x08034AA4, GitHub issue #63) - a graphics-package loading
  setup, a larger multi-subsystem orchestration routine, and a
  ~140-instruction state-machine/input-poll loop with heavy `sb`/`sl`/
  `r8` register pressure; left raw, out of scope for this pass - see
  `docs/matching/issue-63-0x08033ef4-actor.md`.

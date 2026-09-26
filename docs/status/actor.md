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
- `src/graphics/actor_part2.c` (new file - `sub_8007C30`'s real ROM
  address isn't adjacent to `actor_part.c`'s matched functions either,
  since the parked `sub_8007B00`/`sub_8007B98` sit raw between them;
  see `docs/matching.md`): `sub_8007C30`, `sub_8007CF8`. (This file's
  `sub_8007DBC` is a NAKED transcription tracked as parked, not matched
  - see below and `docs/matching/naked-sub_8007dbc.md`.)
- `src/graphics/actor_part3.c` (new file - directly adjacent to
  `actor_part2.c`'s matched functions now that `sub_8007DBC` is
  byte-exact too, closing the old raw gap between them):
  `sub_8007F78`, `sub_8007FD8`
- `src/graphics/actor_part4.c` (new file, now directly adjacent to
  `actor_part3.c`'s matched functions): `sub_80080C0`, `sub_800815C`,
  `sub_8008188`, `sub_8008200`, `sub_8008278` (the latter three
  originally a NAKED transcription, matched to real C in a later
  session - see `docs/matching.md`'s "Parked, not matched: sub_8008188"
  entry and its "Update" note)
- `src/graphics/actor_part5.c` (new file, now directly adjacent to
  `actor_part4.c`'s matched functions): `sub_8008304`,
  `sub_8008328`, `sub_800834C`, `sub_8008350`, `sub_8008364`,
  `sub_8008394`, `sub_80083A8`, `sub_80083B8` (the latter originally a
  NAKED transcription, matched to real C in a later session - see
  `docs/matching.md`'s "Parked, not matched: sub_80083B8" entry and its
  "Update" note)
- `src/graphics/actor_part6.c` (new file, now directly adjacent to
  `actor_part5.c`'s matched functions): `sub_8008408`, `sub_8008434`, `sub_8008480`,
  `sub_8008484`, `sub_80084A4`, `sub_80084C4`, `sub_8008518`,
  `sub_8008564`, `sub_80085B8`, `sub_8008604`, `sub_8008618`,
  `sub_8008640`, `sub_8008648`, `sub_8008650`, `sub_800865C`,
  `sub_8008674`, `sub_8008680`, `sub_800868C`, `sub_8008698`,
  `sub_80086A4`, `sub_80086B0`, `sub_80086BC`, `sub_80086C4`,
  `sub_80086CC`, `sub_80086D8`, `sub_80086E4`, `sub_80086EC`,
  `sub_80086F4`, `sub_8008710`, `sub_800872C`, `sub_8008734`,
  `sub_8008748`, `sub_8008754`, `sub_8008768`, `sub_800876C`,
  `sub_8008770`
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
  `sub_8008E94`, `sub_8008EB4`, `sub_8008EE4`. (This file's `sub_8008F20`,
  compiled right after these, is a NAKED transcription tracked as
  parked, not matched - see below and
  `docs/matching/naked-sub_8008f20-sub_8009914-freelist.md`.)

- `src/graphics/actor_part11b.c`/`actor_part11c.c`/`actor_part11f.c`/
  `actor_part11e.c`/`actor_part11d.c` (new files, NAKED-transcription-
  only - see "Parked - NAKED transcription" below for what each one
  holds): `sub_8009008`, `sub_80091D4`, `sub_8009528`, `sub_80096C0`,
  `sub_8009868` respectively, each dropped into `ldscript.txt` between
  the remaining `asm/code_3_2_13*.s` guard splits at its own real ROM
  address

- `src/graphics/actor_part11g.c` (new file - `sub_8009150`'s real ROM
  address sits between the NAKED `sub_8009008` (`actor_part11b.c`) and
  `sub_80091D4` (`actor_part11c.c`), replacing the retired
  `asm/code_3_2_13_9150.s` guard; named `actor_part11g.c` since
  `sub_8009528`'s own NAKED conversion claimed `actor_part11f.c` first):
  `sub_8009150` - see
  `docs/matching/sub_8009150-loop-invariant-hoist-matched.md`

- `src/graphics/actor_part11h.c` (new file - `sub_800944C`'s real ROM
  address isn't adjacent to `actor_part11.c`'s matched functions,
  since the NAKED `sub_8008F20` and `sub_8009008`/`sub_80091D4` plus
  the now-matched `sub_8009150` (`actor_part11g.c`) sit between them;
  named `actor_part11h.c` since `actor_part11f.c`/`actor_part11g.c`
  were both already claimed by the time this landed): `sub_800944C`

- `src/graphics/actor_part11i.c` (new file, NAKED-transcription-only,
  `sub_8009914`'s real ROM address isn't adjacent to `actor_part11.c`'s
  own functions - see `docs/matching/naked-sub_8008f20-sub_8009914-freelist.md`.
  Named "i" - `sub_8009528` claimed `actor_part11f.c`, the now-matched
  `sub_8009150` claimed `actor_part11g.c`, and the now-matched
  `sub_800944C` claimed `actor_part11h.c`, all in parallel PRs merged
  first), dropped into `ldscript.txt` in place of the retired
  `asm/code_3_2_13_9914.s`

- `src/graphics/actor_part12.c` (new file - `sub_8009A30`'s real ROM
  address isn't adjacent to `actor_part11.c`'s matched functions
  either, since the NAKED `sub_8008F20`, the now-matched `sub_8009150`
  (`actor_part11g.c`) and `sub_800944C` (`actor_part11h.c`), the NAKED
  `sub_8009008`/`sub_80091D4`/`sub_8009528`/`sub_80096C0`/`sub_8009868`/`sub_8009914`,
  all sit between them; see `docs/matching.md`):
  `sub_8009A30`, `sub_8009AA0`, `sub_8009AF0`, `sub_8009B3C`,
  `sub_8009B70`, `sub_8009B9C`

- `src/graphics/actor_part12b.c` (new file, NAKED-transcription-only -
  `sub_8009BE0`, see "Parked - NAKED transcription" below)

- `src/graphics/actor_part13.c` (new file - `sub_8009CA0`'s real ROM
  address isn't adjacent to `actor_part12.c`'s matched functions
  either, since NAKED `sub_8009BE0` (`actor_part12b.c`) sits between
  them; see `docs/matching.md`): `sub_8009CA0`

- `src/graphics/actor_part8.c` (new file - `sub_8009DF4`'s real ROM
  address isn't adjacent to `code_3_2_15.o`'s raw content either, since
  NAKED `sub_8009BE0` (before that) was already handled separately; see
  `docs/matching.md`):
  `sub_8009DF4`, `sub_8009EA8`, `sub_8009EB0`, `sub_8009EBC`,
  `sub_8009EC4`, `sub_8009ECC`, `sub_8009ED0`, `sub_8009F1C`,
  `sub_8009F50`, `sub_8009F90`, `sub_8009FB0`
- `src/graphics/actor_part9.c` (new file - `sub_8009FD4`'s real ROM
  address is adjacent to `actor_part8.c`'s matched functions; see
  `docs/matching.md`): `sub_8009FD4`, `sub_8009FF4`, `sub_800A050`,
  `sub_800A068`, `sub_800A06C`, `sub_800A078`, `sub_800A080`,
  `sub_800A088`, `sub_800A090`, `sub_800A098`, `sub_800A09C`,
  `sub_800A0A0`, `sub_800A0A4`, `sub_800A0A8`, `sub_800A0AC`,
  `sub_800A0CC`, `sub_800A0D8`, `sub_800A0E0`, `sub_800A0EC`,
  `sub_800A0F4`

- `src/graphics/actor_part110.c` (issue #9/#10 follow-up - see
  [docs/matching/issue-9-0x0800a178-graphics.md](../matching/issue-9-0x0800a178-graphics.md)):
  `sub_800A0FC` - the part-object physics dispatcher, the sole caller
  of `sub_800A178` (both now share this file). Was left raw the first
  pass since its own gate logic calls `sub_8009BE0`, then still-
  unresolved; `sub_8009BE0` is now fully understood (see "Parked -
  NAKED transcription" below), which was enough to close this one as
  real, byte-exact matched C. Returns `self+0x68` (a persistent,
  cumulative per-object collision-axis mask, distinct from
  `sub_800A178`'s own per-call `self+0x74` scratch mask) unchanged
  unless `self+0xc` bit 7 is set; if set, calls `sub_800A178` and OR's
  its result into `self+0x68`, fires the already-matched `sub_800A050`
  trampoline, then - if `self+0x68` bit 3 (the Y-axis bit) is now set -
  clears `self+0xc` bits 0/5 and, unless `self+0xd` bit 1 is already
  set, cross-checks the Y-axis hit via a second, independent
  `sub_8009BE0(self, 8, quad)` step-probe, rolling `self+0xc` bit 5 in
  and `self+0x68` bit 3 back out if that probe doesn't also confirm it.
  Needed direct register pinning at several points to reproduce the
  ROM's exact register choices (a `flags`/`bit7` pair for the leading
  gate test, the established negative-constant-mask idiom for the
  `&= ~0x21` clear, a 4-register chain for the `(self+0xd>>1)&1` gate,
  `sub_800A050`'s own addr-before-fn trampoline ordering reused for
  `self->table+0x10/0x14`, and two more mask/byte/result pairs for the
  trailing `|= 0x20`/`&= 7` writes). Retires `asm/code_3_2_11.s`
  entirely (it held only this function).

- `src/graphics/actor_part14.c` (new file - `sub_800A5F4`'s real ROM
  address isn't adjacent to `actor_part9.c`'s matched functions
  either, since a raw/parked span (`sub_800A178`-`sub_800A590`,
  `sub_800A178`/`sub_800A420` NAKED-parked but `sub_800A528`/
  `sub_800A590` matched in `actor_part47.c`) sits between them; see
  `docs/matching.md`): `sub_800A5F4`, `sub_800A600`,
  `sub_800A604`, `sub_800A650`, `sub_800A664`, `sub_800A6A4`,
  `sub_800A6C4`, `sub_800A6D0`, `sub_800A6DC`, `sub_800A6E8`,
  `sub_800A6F4`, `sub_800A700`, `sub_800A70C`, `sub_800A718`,
  `sub_800A724`, `sub_800A730`

- `src/graphics/actor_part47.c` (new file, GitHub issue #9): `sub_800A528`/
  `sub_800A590` - a moving-platform "ride along" hookup, nudging `self->y`
  by the delta between a cached and current position-record lookup.
  Matches the ROM's register roles for `self`/the record pointer
  directly (`r4`/`r3`); the remaining gap (a genuine fifth scratch
  register, `r5`, just to hold an offset immediate for the record's
  `+2`/`+5` field reads, which no C-level phrasing alone ever made this
  compiler introduce) closed by materializing the ROM's own load
  sequence directly via `asm volatile`. Retires the raw
  `asm/code_3_2_11_a528.s`. See
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).

- `src/graphics/actor_part48.c` (GitHub issue #9): `sub_800A734` - a
  part-object velocity/state reset+constructor that hooks up a child
  object at `self+0xb0` (closed a gap an earlier session parked on -
  needed interleaved running-pointer cursors, several register-pinned
  idioms, and an inline-asm-anchored instruction order in a few spots)
  - and `sub_800A810` - a part-object velocity/state reset that
  dispatches a sub-state byte to one of three teardown helpers; see
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
  `sub_800B650`, `sub_800B678`, `sub_800B698`, `sub_800B69C`,
  `sub_800B6A0`, `sub_800B6D0` (issues #84/#85 - see
  [docs/matching/issue-84-85-sub_800B6A0.md](../matching/issue-84-85-sub_800B6A0.md);
  matched with `self`/`vec` pinned to `r3`/`r2` and each branch's X/Y/Z
  locals pinned to their own ABI registers in the ROM's actual load
  order, avoiding the callee-saved spill three earlier attempts hit)

- `src/graphics/actor_part77.c` (new file, GitHub issue #9/#10, ROM
  `0x0800B3F0`, non-adjacent to `actor_part48.c` since the matched
  `actor_part15.c`/`sub_800B270`/`actor_part16.c` sit between
  them): `sub_800B3F0` - a part-object constructor re-initializing
  `self` via `sub_800A6A4`, allocating a child `struct actor` via
  `sub_8008434`, hooking it up at `self+0xb0` via the standard
  `sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio, then calling
  `sub_800A734` to finish the reset and setting `self`'s `field_08`/
  `x`/`y` from its three `u16` arguments; see
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).

- `src/graphics/actor_part17.c` (new file - see `docs/matching.md`):
  `sub_800B704`, `sub_800B734`, `sub_800B7B0`, `sub_800B838`,
  `nullsub_13`, `sub_800B86C`, `sub_800B8A4`, `sub_800B8A8`,
  `sub_800B8C8`, `sub_800B8D8`

- `src/graphics/actor_part18.c`/`actor_part18b.c` (new files, non-
  adjacent since `sub_801434C` sits between them - see
  `docs/matching.md`, issue #17): `sub_801426C`, `sub_80142B0`,
  `sub_80144E0`, `sub_8014524` - four entries of the `gStaticData_0816BF20`
  42-slot action dispatch table (`sub_801434C`/`sub_80145E4`, also in
  these files, are NAKED transcriptions - see "Parked - NAKED
  transcription" below)

- `src/graphics/actor_part19.c`/`actor_part19c.c`/`actor_part19d.c`/
  `actor_part19f.c`/`actor_part19g.c` (new files, non-adjacent since
  the now-matched `sub_802C3E8` (see below), the NAKED-transcribed `sub_802C208`
  (`actor_part19e.c` - see below), and one left-raw function sit
  between them - `sub_802C2FC` (`actor_part19b.c`), previously also
  parked here, is now matched as real C (see below) - see
  `docs/matching.md`, issue #52): `sub_802BED8`, `sub_802BF30`,
  `sub_802BFA0`, `sub_802BFD4`, `sub_802C018`, `sub_802C078`,
  `sub_802C0A8`, `sub_802C0BC`, `sub_802C128`, `sub_802C14C`,
  `sub_802C19C`, `sub_802C264`, `sub_802C270`, `sub_802C394`,
  `sub_802C464`, `sub_802C4A4`, `sub_802C4C8`, `sub_802C540`,
  `sub_802C614`, `sub_802C6C0`, `sub_802C904` - the same large
  per-instance "self" object's action-table/trampoline/circular-list
  conventions as `actor_part17.c`/`actor_part18.c`

- `src/graphics/actor_part19i.c` (new file, directly adjacent to
  `actor_part19d.c`'s matched functions - GitHub issue #53):
  `sub_802C99C`, `sub_802CA28`, `sub_802CA6C`, `sub_802CAD0` - the
  type-byte-dispatch/proximity "used"-state transition family (same
  shape as `sub_802C540`/`sub_802C614`, `actor_part19g.c`); `sub_802CB34`
  and its seven thin forwarding wrappers (`sub_802CB9C`, `sub_802CBC0`,
  `sub_802CBE4`, `sub_802CC08`, `sub_802CC2C`, `sub_802CC54`,
  `sub_802CC78`) - an `InitActorPart`-based constructor family
  classifying a "kind" from a `sub_803ADB4`-scaled/clamped value plus a
  range-keyed offset. See
  [docs/matching/issue-53-actor-c7a8.md](../matching/issue-53-actor-c7a8.md).

- `src/graphics/actor_part126.c` (new file, `0x0802CDE4`-`0x0802D2DC`,
  the remainder of the `0x0802CC9C`-`0x0802D3A8` gap between issues #53
  and #54): `sub_802CDE4`/`sub_802CE38`/`sub_802CF0C`/`sub_802D0C8`/
  `sub_802D1B8` - `InitActorPart`-based constructors on the same `self`
  object family; `sub_802CE5C` - a 3-way `self+0x28` state dispatch;
  `sub_802CF30` plus its `sub_802D044` homing-velocity helper - a
  velocity/proximity state machine; `sub_802CE10`/`sub_802D0F4` - small
  proximity-gated state advances; `sub_802D204`/`sub_802D2DC` - a
  VRAM-gauge/state-transition pair for a `gUnknown_030014B8`-counted
  effect. 12 functions, all matched. See
  [docs/matching/issue-53-issue-54-gap-cc9c.md](../matching/issue-53-issue-54-gap-cc9c.md).

- `src/graphics/actor_aabb_setup.c` (new file, GitHub issue #70, ROM
  `0x0803AFDC`-`0x0803B060` - right after the parked division/modulo
  trio in `src/util/math_div_util.c`, see that file's `docs/matching.md`
  entry): `sub_803AFDC`/`sub_803AFE4` (the shared AABB set-size/
  set-position primitive already referenced by name from
  `actor_part.c`/`actor_part2.c`/`oam_count.c`), `sub_803AFEC` (a
  trivial raw-offset getter), `sub_803AFF0`/`sub_803B024` (two more
  `gStaticData_087E3BEC`-family per-type descriptor table constructors)

- `src/graphics/actor_part20.c`/`actor_part20d.c`/`actor_part21.c`/
  `actor_part21c.c`/`actor_part22.c`/`actor_part23.c`/
  `actor_part24.c`/`actor_part25.c`/`actor_part26.c` (new files, issue
  #58, ROM `0x08030334`-`0x08031784` - the boss-weapon effect state
  machine, non-adjacent since 18 raw functions sit between/around them;
  see
  [docs/matching/issue-58-0x08030334-actor.md](../matching/issue-58-0x08030334-actor.md)):
  `sub_8030530`, `sub_80305F8`, `sub_8030640`, `sub_80306A4`,
  `sub_80306AC`, `sub_8030C98`, `sub_80312C4`, `sub_803146C`,
  `sub_803171C`, `sub_8031744` - a countdown-timer state transition, an
  `InitActorPart`-based constructor, a trivial byte setter/getter pair,
  a camera-relative position accumulator with its own state-2/table-
  index-0 transition, a screen-accumulator/tracker-reset step, a BG2
  zoom-effect updater, a "charge" countdown, and a palette flash/
  animation-refresh pair.
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
  accessor/state-machine cluster, non-adjacent since 2 parked functions
  (`sub_80339DC`/`sub_8033CF8`) and the 3 NAKED-transcribed
  `sub_8033B44`/`sub_8033C84`/`sub_8033E80` (see below) sit interleaved
  between them; see
  [docs/matching/issue-62-0x08033804-actor.md](../matching/issue-62-0x08033804-actor.md)):
  `sub_8033804`, `sub_8033828`, `sub_8033880`, `sub_803388C`,
  `sub_80338C4`, `sub_80338D0`, `sub_80338DC`, `sub_80338E8`,
  `sub_80338F4`, `sub_8033900`, `sub_803390C`, `nullsub_36`,
  `sub_803395C`, `nullsub_37`, `sub_8033AE0`,
  `sub_8033BB8`, `sub_8033BFC`, `sub_8033C28`,
  `sub_8033CF0`, `sub_8033E18` - the
  singleton's one-shot latches, field getters, state-transition/
  anim-frame-reset setters, an `InitActorPart`-based constructor, and
  several "self" object accessors/setters sharing the boss cluster's
  layout convention.
- `src/graphics/actor_part38.c` (new file, GitHub issue #18, ROM
  0x08014F8C - numbered `38` rather than `28` since issue #62's
  parallel PR above independently claimed `actor_part28.c` first):
  `sub_8014F8C` - a `gUnknown_030012F0`-list proximity-
  trigger scan for the same "self" action-table object family as
  `actor_part18.c` (`sub_8015038`, also in this file, is a NAKED
  transcription - see "Parked - NAKED transcription" below); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38b.c` (new file, GitHub issue #18, ROM
  0x080151C8, non-adjacent to `actor_part38.c` since `sub_8015038`
  sits between them): `sub_80151C8` (`sub_8015238`/`sub_80152F0`, also
  in this file, are NAKED transcriptions); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38c.c` (new file, GitHub issue #18, ROM
  0x08015350-0x080156B4, non-adjacent to `actor_part38b.c` since
  `sub_8015238`/`sub_80152F0` sit between them):
  `sub_8015350`, `sub_8015398`, `sub_80153FC`, `sub_8015460`,
  `sub_8015508`, `sub_8015558`, `sub_80155A8`, `sub_80155AC`,
  `sub_80155B8`, `sub_80155F8`, `sub_8015650`, `sub_8015690`,
  `sub_80156B4` - more of the same self+0xc/self+0x10 trampoline-pair
  family, including two near-identical self+0x29-keyed mgr-trampoline
  arms (`sub_8015460`) and several part+0x38-gated trampoline firers
  (`sub_80156EC`, also in this file, is a NAKED transcription); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/graphics/actor_part38d.c` (new file, GitHub issue #18, ROM
  0x0801574C-0x08015840, non-adjacent to `actor_part38c.c` since
  `sub_80156EC` sits between them): `nullsub_17`,
  `sub_8015750`, `nullsub_18`, `sub_8015774`, `sub_8015780`,
  `sub_80157C4` - two nullsubs, two tail-call wrappers, the shared
  trampoline-pair-plus-sentinel-store helper called by
  `actor_part18.c`'s `sub_801426C`/`sub_80142B0`, and `sub_80157C4`
  (player's `+0x100`-flag-gated `mode` remapper tail-calling
  `sub_800B86C`, reinterpreted through a `s32`-returning function-
  pointer cast to steer the epilogue's `pop`/`bx` scratch register
  choice - see
  [docs/matching/naked-sub_80157c4-matched.md](../matching/naked-sub_80157c4-matched.md));
  see `docs/matching/issue-18-0x08014f8c-actor.md`.

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
  set `+0x50` to the shared "dead" vtable, and conditionally free), and
  `sub_803B46C` (fixed-position OAM setup for one sprite frame) - see
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
- `src/graphics/actor_part79.c` (new file, GitHub issue #16, ROM
  0x08012160-0x08012420): `sub_8012160`, `sub_8012238`, `sub_80122CC` -
  three more members of the 42-slot action-dispatch-table family
  (`gStaticData_0816BF20`), operating on the same still-unnamed "child
  object" struct (`self+0xc`/`self+0x10` sub-record pointers, the
  `+0x27`-`+0x32` state/flag/table-index trio) `actor_part18.c`/
  `actor_part18b.c` already established conventions for. Not
  ROM-adjacent to those files (the raw `sub_8012420`/`sub_8012694`/
  `sub_801283C` and the still-raw `sub_8011BD4` sit between them), so
  a new file. See
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- `src/graphics/actor_part80.c` (new file, GitHub issue #16, ROM
  0x08012A7C-0x08012AF4): `sub_8012A7C` - another member of the same
  action-dispatch-table family, not ROM-adjacent to `actor_part79.c`'s
  functions either (the raw `sub_8012420`/`sub_8012694`/`sub_801283C`
  sit in between). See
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- `src/graphics/actor_part43.c`/`actor_part44.c`/`actor_part45.c`/
  `actor_part46.c` (new files, GitHub issue #56, ROM
  0x0802F0DC-0x0802FBF0 - a second boss-weapon "spawn/pre-attack"
  singleton and its `self` object, non-adjacent since the parked
  `sub_802F338`/`sub_802FA04`, the NAKED-transcribed `sub_802F748`
  (`actor_part44b.c` - see below), and the NAKED-transcribed
  `sub_802F7B0`/`sub_802F8E8`/`sub_802FA38` (`actor_part45d.c`/
  `actor_part46b.c` - see below) sit interleaved between them; see
  [docs/matching/issue-56-0x0802f0dc-actor.md](../matching/issue-56-0x0802f0dc-actor.md)):
  `sub_802F0DC`, `sub_802F164`, `sub_802F3BC`, `sub_802F46C`, `sub_802F47C`,
  `sub_802F4AC`, `sub_802F4C0`, `sub_802F4CC`, `sub_802F50C`,
  `sub_802F540`, `sub_802F570`, `sub_802F5AC`, `sub_802F5E4`,
  `sub_802F640`, `sub_802F69C`, `sub_802F6DC`,
  `sub_802F7A4`, `sub_802F97C`, `sub_802FA34` - a constructor/reset, a
  state-machine update, an accumulator-drain/reward-
  dispenser, accessors, accumulator drivers, idle-state-reset idioms,
  and the singleton's teardown/destructor, all sharing
  `actor_part17.c`/`actor_part18.c`/`actor_part20.c`'s established
  "self" object conventions.
- `src/graphics/actor_part50.c`/`actor_part51.c`/`actor_part52.c`/
  `actor_part53.c`/`actor_part54.c`/`actor_part55.c`/`actor_part56.c`
  (new files, GitHub issue #50, ROM 0x0802A69C-0x0802AC28 - numbered
  `50`-`56` rather than `39`-`45` since issues #16 and #56's parallel
  PRs above independently claimed those numbers first; see
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
  `gUnknown_03001464`-gated palette-cycle DMA cluster's members - plus
  `UpdateAnimatedActorPart`, `sub_802AA0C`, and `sub_802AB58`, all three
  matched in a later pass that closed the register-pinning/pool-split
  gaps documented in that same writeup (all 25 of this chunk's functions
  are now real C, none NAKED).
- `src/graphics/actor_part127.c` (new file, ROM `0x0802B364`-`0x0802BC68`
  - the start of the actor zone before issue #52, right before
  `actor_part107.c`'s own range): `sub_802BB4C` - a frame-counter
  threshold DMA driver sharing the same reset idiom as `sub_802B730`.
  Just 1 of this gap's 11 functions matched as real C; the other 10
  (`sub_802B364`, `sub_802B5B4`, `sub_802B730`, `sub_802B7E0`,
  `sub_802B864`, `sub_802B8E8`, `sub_802B990`, `sub_802BA5C`,
  `sub_802BAD0`, `sub_802BBE4`) are NAKED-parked in the same file, see
  the "Parked - NAKED transcription" section below. See
  [docs/matching/issue-52-gap-b364.md](../matching/issue-52-gap-b364.md).
- `src/graphics/actor_part107.c` (new file, ROM 0x0802BC68-0x0802BED8 -
  the literal tail of `asm/code_3_2_20_8b7c_ac28.s`, one raw file's
  leftover portion out of GitHub issue #50's original chunk scope;
  everything before it in that raw file - `sub_802AC28`'s giant
  kind-dispatch actor-part-factory constructor and the run of actor-
  part-factory/animation-table-state functions between it and here -
  stays raw, still out of scope; see
  [docs/matching/issue-50-actor-bc68.md](../matching/issue-50-actor-bc68.md)):
  `sub_802BC68`, `sub_802BD18`, `sub_802BD24`, `sub_802BD64`,
  `sub_802BDD0`, `sub_802BE34`, `sub_802BE80` - an accumulator-drain/
  reward-dispenser (docs/rom_map.md already reads it as a structural
  twin of `actor_part44.c`'s `sub_802F3BC`), a trivial byte getter, two
  frame-counter-threshold state-reset functions sharing the state/
  table-index/anim-frame reset idiom, and a three-axis hazard-threshold
  driver family (screen-flash trigger via `sub_800132C`, hazard-
  direction arming via `sub_802A668`) on the same `gUnknown_0300148x`/
  `gUnknown_030014Ax` global cluster `actor_part19.c`/`actor_part44.c`
  already established; matched.

- `src/graphics/actor_part57.c` (new file, GitHub issue #19, ROM
  0x08015840-0x080159A4 - recategorized `graphics`->`actor` from the
  issue's label, same self+0xc/self+0x10 trampoline-pair and state/
  counter/table-index-trio family as actor_part38c.c/actor_part38d.c;
  immediately adjacent to actor_part38d.c's matched span): `sub_8015840`, `sub_8015878`,
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
- `src/graphics/actor_part59.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part74.c` sits between it and `actor_part58.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802DB2C`, `sub_802DCC0` - the `gUnknown_030014BC` position-
  tracking object's two `gStaticData_0817A840` vtable-slot update
  functions (accumulate/clamp, tier-keyed `PlaySfx`/`sub_80019F8`
  cues, and a shared kind/anim-reset transition tail).
- `src/graphics/actor_part60.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part75.c` sits between it and `actor_part59.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `sub_802DFBC`, `sub_802DFC8`, `sub_802DFDC` - the
  `gUnknown_030014BC` object's state-flag setter, destructor, and
  constructor.
- `src/graphics/actor_part61.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part76.c` sits between it and `actor_part60.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `nullsub_27` - a genuine no-op stub.
- `src/graphics/actor_part128.c` (new file, ROM `0x0802E0A4`-
  `0x0802F0DC`, the gap between issue #54's chunk and issue #56's
  chunk): `sub_802E0A4` (a state-3 anim-frame edge reset) and
  `sub_802E484`/`sub_802E504`/`sub_802E57C`/`sub_802E5B0` (four
  `mem_alloc`-plus-forwarding-call constructors, part of the
  `sub_802E170` 31-case dispatcher's own per-"kind" case family). Just
  4 of this gap's 25 functions matched as real C; the other 21 are
  NAKED-parked in the same file, see the "Parked - NAKED transcription"
  section below. See
  [docs/matching/issue-54-issue-56-gap-e0a4.md](../matching/issue-54-issue-56-gap-e0a4.md).
- `src/graphics/actor_part63.c`/`actor_part65.c`/`actor_part67.c`/
  `actor_part69.c`/`actor_part71.c`/`actor_part72.c`/`actor_part73.c`
  (new files, GitHub issue #63, ROM 0x08033EF4-0x08034AA4 - three
  `InitActorPart`-rooted "self" object kinds immediately following
  issue #62's cluster, non-adjacent since 2 remain parked
  (`sub_8034270`/`sub_8034314`, in `actor_part68.c`/`actor_part70.c` -
  `sub_8034058`/`sub_8034374`/`sub_8034480`/`sub_80345B0`/`sub_8034634`
  are now matched too, closing `actor_part85.c` entirely, see below),
  the NAKED-transcribed `sub_8033FE4`
  (`actor_part64.c` - see below), and 3 left-raw functions sit
  interleaved between them; numbered `63`-`73` rather than `57`-`67`
  since issues #19 and #54's PRs independently claimed
  `actor_part57.c`-`62.c` first - see
  [docs/matching/issue-63-0x08033ef4-actor.md](../matching/issue-63-0x08033ef4-actor.md)):
  `sub_8033EF4`, `sub_8033F48`, `sub_8033F74`, `sub_8034050`,
  `sub_8034110`, `sub_8034188`, `sub_80341F8`, `sub_8034264`,
  `nullsub_38`, `sub_80342D4`, `sub_803436C`, `sub_80345B0`,
  `sub_8034634`, `sub_8034688`, `sub_80346C8`, `sub_80346FC` - two
  constructors, a trampoline-fire helper, a position-sync/state-transition
  helper, a damage/death handler, a position-sync/orbit-effect updater and
  its non-identical near-twin, two trivial getters, a no-op stub, a
  128-slot particle-spawner (needed a swapped multiply operand order to
  match this compiler's own left-operand-materializes-into-dest choice; no
  register pins or opaque asm required), a 4-bit tilemap nibble writer
  (needed a `u32`-typed intermediate mask to avoid a spurious 16-bit
  truncation sequence, split address-half statements to pin evaluation
  order, and one opaque `asm volatile` for the ROM's own redundant
  compute-then-copy tail - see that file's doc comments for the full
  account), a particle-spawn-budget driver, an input-poll busy-wait, and a
  buffer-release/teardown helper.
- **`sub_803472C`** (`src/graphics/actor_part87.c`, GitHub issue #63) - a
  standalone `struct fade_overlay` object's constructor half: allocates
  and loads its three BG scratch buffers, builds DISPCNT, hands off to
  `sub_803487C`, then builds the BLDCNT/BLDALPHA alpha-blend value.
  Previously parked (`NON_MATCHING`) over a final handful of accumulator/
  temp-register choices in the BLDCNT/BLDALPHA byte-packing tail; now
  matched as real C - both gaps were ordinary register-pin/barrier fixes,
  not a genuine compiler limitation (a `register u8 asm("r1")` pin for one
  stray reload, and an empty `asm("":"+r"(tmp))` compiler barrier to stop
  this compiler from eliding a mask-to-accumulator copy the ROM's own
  build keeps) - see
  [docs/matching/issue-63-final-raw-actor.md](../matching/issue-63-final-raw-actor.md).

- `src/graphics/actor_anim.c` (extended, GitHub issue #72, ROM
  0x0803B4EC-0x0803B8B0 - directly contiguous with this file's existing
  coverage, which already ended right at 0x0803B4EC): `sub_803B4EC` (an
  animation-frame-advance/loop-back function, plus a `+0x50` trampoline
  dispatch when the "held" flag is set) and 15 more "kind" teardown/
  dispatch handlers through `sub_803B884` (the same `struct linked_node`
  unlink-and-free shape, and the same `sub_80321D0`-based teardown shape,
  already established earlier in this file). Also recovered 7 functions
  the original disassembly never gave their own `thumb_func_start` label
  for, sandwiched inside what looked like padding/literal-pool gaps
  between the labelled ones (`sub_803B54C`, `sub_803B550`, `sub_803B57C`,
  `sub_803B5AC`, `sub_803B5DC`, `nullsub_44`, `sub_803B5E4` - see
  `expected/corrections.txt`'s matching `split` entries, and
  `docs/matching/issue-72-0x0803b4ec-actor.md` for how each was found
  and confirmed via a from-scratch `arm-none-eabi-as`+`objdump`
  reassembly of the original raw block, not by eyeballing the
  disassembly's padding). `struct anim_frame_record`'s `unknown_04[4]`
  became two named `s16` fields (`loopThreshold`/`loopBase`), both read
  by `sub_803B4EC`.

- `src/graphics/actor_part87.c`/`actor_part100.c`/`actor_part88.c`/
  `actor_part95.c`/`actor_part89.c`/`actor_part96.c`/`actor_part90.c`/
  `actor_part97.c`/`actor_part91.c`/`actor_part98.c`/`actor_part92.c`
  (new files, GitHub issue #48, ROM 0x080291A4-0x08029E4C):
  `SetupActorVramPool` (pins the category's tile-cache slots and
  rebuilds its status-icon OAM row), `sub_802968C` (counts
  `sub_effect_table` entries matching a type-dependent "kind" byte
  set), and the rest of a BG-tilemap double-buffer scroll-effect
  subsystem interleaved in this same ROM region (`sub_8029720`-
  `sub_8029E40`, minus the NAKED functions below) - see
  [docs/matching/issue-48-0x080291a4-actor.md](../matching/issue-48-0x080291a4-actor.md).
- `src/graphics/actor_part92.c`/`actor_part99.c`/`actor_part93.c`/
  `actor_part94.c` (GitHub issue #49, ROM 0x08029E4C-0x0802A69C):
  `nullsub_6`, `sub_8029E50`, `sub_8029E98`, `sub_8029EB4` (the
  BG2-affine scroll subsystem's tail), the `gUnknown_03001400`
  `sub_effect_table` record accessor family (`sub_802A4D4`-
  `sub_802A650`/`sub_802A668`), and a circular-list marker-drawing pass
  (`sub_802A5E4`) - see
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).

- **`sub_8009D5C`** (`src/graphics/actor_part13.c`) - fires a
  `part->table+0x68`-driven trampoline based on `gUnknown_030012C0`'s
  mode, on the player and/or `part` depending on the mode value. A
  `switch` reproduces the ROM's exact 3-way mode dispatch, and explicit
  `goto`s into a shared, ABI-register-pinned tail reproduce the
  mode-0/mode-1-2 call sharing. The remaining conditional-branch
  encoding gap in the mode-3 case (ROM's 3-instruction `cmp;beq;b`
  versus this compiler's usual 2-instruction `cmp;bne` collapse) closed
  once the mode-3 `checkMode3:`/`if (mode == 3)` block was moved to be
  the *last* thing in the function (after `mode0`/`mode1or2`/`tail`
  instead of right after the `switch`) - with the call body no longer
  textually adjacent to its own dispatch test, this compiler's
  block-layout pass can't collapse the two paths and emits the real
  3-instruction form on its own, no inline asm needed. Also needed
  `mode0`'s source order swapped ahead of `mode1or2` (matching ROM's
  real address order - the compiler places case bodies in source order,
  not case-value order) and an explicit `r0` register pin on `mode0`'s
  `player` local (this compiler otherwise picks `r2` there, since
  `mode1or2`'s own `sub_803AD88` call already forces the same value
  into `r0` via the ABI, but `mode0` has no such call to hint it).
  Retires the old raw `asm/code_3_2_15.o` guard entirely.

- `src/graphics/actor_part125.c` (new file, ROM 0x08031784-0x08031A6C,
  Phase 1 of the boss-weapon/singleton cluster's gap between issue #58
  and issue #62): `sub_8031784` (tracker "ready" check scaling the
  countdown via `sub_803ADB4`), `sub_80317C4` (tracker destructor,
  `sub_8030F88`'s counterpart), `nullsub_30`/`nullsub_31`/`nullsub_32`
  (no-op stubs), `sub_8031850` (trivial `self+0x58` setter),
  `sub_80318B4` (full state/accumulator/anim-frame reset idiom),
  `sub_8031920` (`InitActorPart`-based constructor), and `sub_8031A64`
  (trivial `self+0x5c` getter, needed a trailing `asm(".align 2, 0")`
  for the same lone-function-at-end-of-TU padding gap as
  `sub_80306A4`/`sub_8033CF0`) - see
  [docs/matching/issue-59-0x08031784-actor.md](../matching/issue-59-0x08031784-actor.md).

- `src/graphics/actor_part129.c` (new file, ROM 0x08031B0C-0x08032688,
  first 30 of issue #59 Phase 2's 60-function remainder): the
  "type-byte event dispatch" family (`sub_8031B0C`/`sub_8031C0C`/
  `sub_8031D04`/`sub_8031D7C`/`sub_8031E80`), three `sub_802E4B8`-based
  "spawn effect type N" constructors (`sub_8031F78`/`sub_8032054`/
  `sub_80320C4`), trivial setters/getters (`sub_8032138`/`sub_8032478`/
  `sub_8032350`/`sub_8032680`), a full reset idiom (`sub_8032140`), a
  countdown-gated trampoline-flush transition (`sub_8032170`), a
  doubly-linked-list unlink/`mem_free` destructor (`sub_80321D0`), a
  no-op stub (`nullsub_33`), a trivial accumulator (`sub_8032274`), a
  second independent orbital-motion consumer of the shared trig table
  `gStaticData_0816A820` (`sub_8032290`, alongside the already-flagged
  `sub_8032480`), a state-1 trampoline-flush/proximity transition
  (`sub_8032358`), more countdown transitions (`sub_80323F4`/
  `sub_80325A4`), `sub_8032480` itself (the orbital-motion consumer,
  plus its state-transition helper `sub_803256C`), and a type-byte-gated
  proximity check (`sub_8032688`) - see
  [docs/matching/issue-59-60-gap-31a6c-part1.md](../matching/issue-59-60-gap-31a6c-part1.md).
  (Two InitActorPart-based constructors originally attempted as real C
  in this same file, `sub_8032440`/`sub_80325EC`, ended up NAKED-parked
  instead - see below.)
- `src/graphics/actor_part131.c` (new file, ROM 0x08034AA4-0x080354E0,
  GitHub issue #64): `sub_8034C40` (the fade overlay's Yes/No-dialog
  blink/toggle helper), `sub_8034C5C` (fade overlay per-frame "yield"
  helper), `sub_8034C84` (fade overlay teardown), `sub_8034CB0` (the
  "Are you sure?" confirmation-dialog trigger), `sub_8034E2C` (the
  between-level map/progress screen's per-frame driver - needed a
  register-pinned `mask &= p->pressed` accumulate-into-existing-register
  rewrite of a plain `&` boolean test, plus a `void *unused` parameter
  kept on `sub_803544C` since the ROM's own call site passes `self` into
  it even though the callee never reads it), `sub_803544C` (map screen
  end-of-frame commit), `sub_803547C` (map screen teardown), and
  `sub_80354BC` (the map screen's top-level entry point) all matched as
  real C; `sub_8034AA4`, `sub_8034CEC`, `sub_8034EF0`, `sub_80350A4`,
  and `sub_80352AC` transcribed as NAKED (see "Parked - NAKED
  transcription" below) - see
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED transcription (byte-correct, not decompiled)

These are byte-exact (confirmed by a full clean `make compare`), but
as `NAKED` functions whose body is the ROM's own disassembly
transcribed instruction-for-instruction rather than real decompiled C,
they don't count as "matched" for this project's tracking - the goal
is readable C, and an asm blob wrapped in a C function signature
doesn't advance that even when byte-correct. See
[docs/workflow.md](../workflow.md)'s NAKED-transcription escape hatch
(`sub_8001CB8`/`sub_8001DB4` in `src/system/link_cable.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

- **`sub_802E0CC`**, **`sub_802E170`**, **`sub_802E3CC`**,
  **`sub_802E420`**, **`sub_802E4B8`**, **`sub_802E538`**,
  **`sub_802E5E4`**, **`sub_802E62C`**, **`sub_802E674`**,
  **`sub_802E6CC`**, **`sub_802E710`**, **`sub_802E740`**,
  **`sub_802E84C`**, **`sub_802E9FC`**, **`sub_802EB78`**,
  **`sub_802EC64`**, **`sub_802ED10`**, **`sub_802EDBC`**,
  **`sub_802EED0`**, **`sub_802EFD8`** (`src/graphics/actor_part128.c`,
  ROM `0x0802E0A4`-`0x0802F0DC`, the gap between issues #54 and #56) -
  a 31-case jump-table dispatcher (`sub_802E170`, indexing a stride-40
  per-"kind" RAM table `docs/rom_map.md` already flagged), the rest of
  its own per-"kind" constructor-family case bodies (each with `r8`/
  `sb`, and for `sub_802E9FC` also `sl`, simultaneously live), and a
  run of `self`-object position-offset/physics/hazard-timer helpers.
  Parked outright rather than chased at the C level given this gap's
  size and how heavily this whole neighborhood already needs the NAKED
  escape hatch. See
  [docs/matching/issue-54-issue-56-gap-e0a4.md](../matching/issue-54-issue-56-gap-e0a4.md).
- **`sub_802B364`**, **`sub_802B5B4`**, **`sub_802B730`**,
  **`sub_802B7E0`**, **`sub_802B864`**, **`sub_802B8E8`**,
  **`sub_802B990`**, **`sub_802BA5C`**, **`sub_802BAD0`**,
  **`sub_802BBE4`** (`src/graphics/actor_part127.c`, ROM
  `0x0802B364`-`0x0802BC68`, the start of the actor zone before issue
  #52) - a countdown-timer/respawn state machine, a sprite-frame OAM
  queuer (`r8`/`sb`/`sl` all simultaneously live), a VRAM-tile-block
  allocator pair, and a run of spawn/reset-trigger and camera-catch-up
  functions on the `gUnknown_0300148x`-`gUnknown_030014Bx` object
  cluster. Each hit its own gcc-2.9 gap (heavy multi-register reuse, the
  "materialize into one register then copy to a second" idiom, or a
  ROM branch layout - an early-return sharing its epilogue with the
  main fallthrough path - this compiler's own scheduling wouldn't
  reproduce from an equivalent C guard clause). See
  [docs/matching/issue-52-gap-b364.md](../matching/issue-52-gap-b364.md).
- **`sub_80159F8`/`sub_8015C6C`** (`src/graphics/actor_part86.c`, ROM
  0x080159F8-0x08015DF8, GitHub issue #19) and **`sub_8015DF8`**
  (`src/graphics/actor_part86b.c`, ROM 0x08015DF8-0x08015FD0, split
  into its own unit) - three large (13-case and 9-case) jump-table
  state-machine dispatchers on the part object's `+0x60`/`+0x64`
  velocity fields, `self+0x21` state, and (for `sub_8015DF8`) a
  `sub_8025BAC` object-spawn call. Fully understood and near-matched
  as real C - a recurring gcc-2.9 gap (ROM routes several `(x*3)>>2`
  computations through an extra scratch-register copy that this
  compiler's allocator always collapses away) blocked the last step
  in all three. See `docs/matching/issue-19-0x08015840-actor.md`.
- **`sub_8031A6C`**, **`sub_80322F4`** (`src/graphics/actor_part129.c`,
  ROM `0x08031A6C`-`0x08032688`, issue #59 Phase 2 part 1) - near-
  duplicate keyframe-table-relative dispatch helpers (indexing
  `gStaticData_0817C42C`), structurally identical to the already-parked
  `sub_8031A08` (issue #59 Phase 1) - same `r7`-as-table-base-pin gap.
- **`sub_80321FC`** (`src/graphics/actor_part129.c`, same range) - a
  parameterized `sub_802E4B8`-based constructor (the "kind" is a 6th
  caller-supplied argument here, rather than one of the fixed literals
  `sub_8031F78`/`sub_8032054`/`sub_80320C4` use). This compiler always
  re-materializes the incoming `c` argument register from its own
  cached copy for the `InitActorPart` call, instead of leaving the
  ROM's original parameter register untouched until the call, and
  separately defers the "kind" byte truncation to its point of use
  rather than the ROM's eager truncation right after loading it from
  the stack. See
  [docs/matching/issue-59-60-gap-31a6c-part1.md](../matching/issue-59-60-gap-31a6c-part1.md).
- **`sub_8032440`** (`src/graphics/actor_part129.c`, same range) - an
  `InitActorPart`-based constructor forcing a fixed `0xFFFF0600` bias
  for its own 4th argument. This compiler's independent-instruction
  scheduler always groups the pure register loads (the `d` argument off
  the stack, the `0xFFFF0600` constant) together regardless of source
  order, while the ROM's own build interleaves them with the
  intervening `str`/`adds` steps.
- **`sub_80325EC`** (`src/graphics/actor_part129.c`, same range) - a
  clamping `InitActorPart`-based constructor. This compiler couldn't be
  steered into the ROM's exact register choreography for the `d`
  argument (transiently held in `r0`, pushed to the outgoing stack
  slot, then `r0` reused for `self`) simultaneous with the health
  literal (`1`) needing to survive in `r4` across the same call. See
  [docs/matching/issue-59-60-gap-31a6c-part1.md](../matching/issue-59-60-gap-31a6c-part1.md)
  for both.
- **`sub_8009008`** (`src/graphics/actor_part11b.c`) - the
  spatial-hash-grid removal primitive `sub_8009A30`/`sub_8009AA0`
  call: a two-phase search (the object's own primary bucket, then
  every bucket 255 down to 0) that unlinks its pool node(s) from the
  `struct pool_manager` grid (`actor_part12.c`) and returns them to
  the free list. Fully understood; parked on a single-instruction
  register-discard quirk in phase 2's early-exit path. See
  `docs/matching/naked-spatial-grid-tail.md`.
- **`sub_80091D4`** (`src/graphics/actor_part11c.c`) - a per-frame
  grid-maintenance pass over the 3-bucket window around the tracked
  sub-object's own column (plus bucket 255): lazily links newly-large
  objects into bucket 255 (`sub_8009150`'s own body, inlined), removes
  and destroys objects flagged for removal, and box-tests/marks the
  rest. Fully understood; parked on a many-high-register allocation
  gap across its three inner-loop branches. See
  `docs/matching/naked-spatial-grid-tail.md`.
- **`sub_8017AB0`** (`src/graphics/actor_part27a.c`, ROM 0x08017AB0-
  0x08017ECC, GitHub issue #22) - a ~500-instruction, fully-understood
  3-state dispatcher (player-vs-camera-viewport gating on
  `gUnknown_030012D8+0x104` and `other+0x28` bit 4; see
  `docs/matching/issue-22-0x08017ab0-actor.md` for the full state map).
  Two independent, already-diagnosed-elsewhere gcc-2.9 gaps recur here:
  the `gUnknown_0300130C` object-list walk's per-iteration literal-pool
  reload (`actor_part108.c`'s `sub_800AAEC`'s own unclosable gap) and
  `sub_8008A40`'s 7-argument/3-stack-slot marshalling order
  (`actor_part81.c`'s `sub_800AB9C`'s own unclosable gap, confirmed by
  this ROM call using the identical stack-argument order) - recognized
  from precedent and NAKED-transcribed directly rather than
  re-litigating either wall at 5x the previous scale.
- **`sub_8009868`** (`src/graphics/actor_part11d.c`) - another
  3-bucket-window grid pass, this one reading the player's state to
  dispatch `sub_800D040`/`sub_80109A4` per object. Fully understood;
  parked on a cross-branch register-role gap (`r8` reused for two
  different base addresses). See
  `docs/matching/naked-spatial-grid-tail.md`.
- **`sub_8009BE0`** (`src/graphics/actor_part12b.c`) - a physics/
  collision step-probe: runs `self`'s position through `sub_8008278`,
  then probes it via `sub_8026628` up to 4 times (nudging Y each
  retry) before giving up. Fully understood; parked on a register-
  reload quirk in the retry loop. See
  `docs/matching/naked-spatial-grid-tail.md`.
- **`sub_800CD00`** (`src/graphics/actor_part109.c`, GitHub issue
  #9/#10) - `sub_800AAEC`'s only callee: builds three AABBs (the
  entry's own current hitbox, the player's current hitbox, and the
  player's hitbox for the caller's target action) via the same
  `self+0x20`-table-at-28-byte-stride "keyframe/hitbox record"
  convention `sub_800D040` (`game_loop6.c`) documents, then returns
  whether the entry's hitbox overlaps the target-action hitbox without
  already overlapping the current one. A strict superset of
  `sub_800D040`'s own two-AABB shape, already documented there as
  resistant to gcc 2.9 C reconstruction (the `r7`/`r8`/`sb`
  register-reuse pattern across AABB blocks) - not re-attempted as C
  here; transcribed directly as byte-exact NAKED asm instead. See
  [docs/matching/issue-9-10-0x0800aaec-graphics.md](../matching/issue-9-10-0x0800aaec-graphics.md).
- **`sub_800A178`/`sub_800A420`** (`src/graphics/actor_part110.c`,
  GitHub issue #9/#10) - the part-object movement-resolution pair
  `docs/matching/issue-9-0x08007634-actor.md` flagged as built on
  `sub_8008200`/`sub_8026628`/`sub_8026C3C`/`sub_8026BF8` (the first
  two now matched, see `actor_part4.c`/`game_loop43.c`). `sub_800A420`
  is a single Y-axis "floor" probe via `sub_8026BF8`; `sub_800A178` is
  the larger orchestrator - two gate checks, an unconditional
  `self+0x74` zero (confirming and completing `docs/rom_map.md`'s own
  partial note: `self+0x74` accumulates which movement axes/directions
  collided this call), a fast quad-based floor/wall probe via
  `sub_800A420`/`sub_8026C3C`, then a per-axis fallback through the
  already-matched `sub_8026628` tile-scan API. Both keep `sb`/`sl`/`r8`
  (and, for `sub_800A178`, `r7` too) live simultaneously across many
  `bl` calls, reused for different values block to block - the same
  `r7`/`r8`/`sb` cross-block register-reuse shape this exact ROM
  neighborhood already established as gcc-2.9-resistant (`sub_8009BE0`,
  `sub_800CD00` above, `sub_800CEAC`/`sub_800CF70` below); confirmed
  directly via one isolated-compile attempt against `sub_800A420`
  (this compiler's natural register allocation used no high registers
  at all, a structurally different solution rather than a near-miss).
  Transcribed directly as byte-exact NAKED asm instead. See
  [docs/matching/issue-9-0x0800a178-graphics.md](../matching/issue-9-0x0800a178-graphics.md).
- **`sub_800AFF4`** (`src/graphics/actor_part111.c`, GitHub issue
  #9/#10) - the per-frame update for a "stars orbiting a dizzy head"
  companion effect, gated on `gUnknown_030012C0+0x78`'s mode field:
  mode 3 (stun-entry) repositions and flickers a `self+0xb0` child
  object next to `self`'s head and blinks `self` itself via
  `sub_8007A84` on a `self+0x8c` deadline, ending the stun
  (`sub_80231EC(gUnknown_030012C0, 2)`) once that deadline clears;
  modes 1/2 (idle-orbit) drive the same child object in an elliptical
  path from `self`'s own 8-frame-delayed position-history ring buffer
  (`self+0xb4`/`self+0xb8`) plus the shared `gStaticData_0816A820`
  sine-ish table. Both the mode-3 and mode-1/2 branches reuse the same
  `self+0x20`-table/`+0x2d`-tag 28-byte-record clamp-and-store idiom
  (`actor_part79.c`'s sibling shape) to pick the child's own variant.
  Keeps `sb`/`sl`/`r8`/`ip` all simultaneously live across the
  ring-buffer and trig-table math - the same register-pressure shape
  already proven gcc-2.9-resistant on `sub_800CD00`/`sub_800A178`/
  `sub_800A420` above and `sub_8026AE8`/`sub_8026A18` elsewhere this
  session; not attempted as C, transcribed directly as byte-exact
  NAKED asm instead. See
  [docs/matching/issue-9-10-0x0800aff4-graphics.md](../matching/issue-9-10-0x0800aff4-graphics.md).
- **`sub_801434C`** (`src/graphics/actor_part18.c`) - the shared
  handler `sub_80142B0` tail-calls; one of the `gStaticData_0816BF20`
  action-table entries. See `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80145E4`** (`src/graphics/actor_part18b.c`) - same shape as
  the matched `sub_8014524` (boolean/raw-value bit test, `sub_8015780`
  reset block) but keeps the raw masked bit value rather than a
  `!= 0`-normalized boolean. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_8015038`** (`src/graphics/actor_part38.c`) - a three-arm
  mgr-trampoline handler keyed on `self+0x24`/`self+0x22`, picking one
  of three table-index fallbacks. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_8015238`** (`src/graphics/actor_part38b.c`) -
  `self+0x26`/`mode`/`flags`-gated mgr-trampoline dispatcher. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80152F0`** (`src/graphics/actor_part38b.c`) -
  `self+0x27`/`self+0x2b`/`mode`-gated state/counter/table-index trio
  reset, tail-calling `sub_80122CC`. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_80156EC`** (`src/graphics/actor_part38c.c`) -
  `part+0x38`/`sub_80231BC`-gated mgr-trampoline dispatcher. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **`sub_8030574`** (`src/graphics/actor_part20b.c`) - boss-weapon
  keyframe-table AABB lookup/dispatch; same `gStaticData_*` stride-8
  table shape and r7-hazard as the already-parked `sub_802C208`
  (actor_part19.c) - this compiler's unforced allocator never reaches
  r7 for this shape, and a direct C translation compiles noticeably
  shorter/differently-structured code. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8030648`** (`src/graphics/actor_part21b.c`) - same
  keyframe-table AABB lookup shape as `sub_8030574`, minus its
  state-transition tail. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_802FA38`** (`src/graphics/actor_part46b.c`, issue #56) -
  position-update/collision-damage function for the issue #56
  singleton's `self` object; its keyframe-table lookup is the same
  `gStaticData_0817C260` stride-8 r7-hazard shape as `sub_802C208`/
  `sub_802F748`/`sub_8030574` above, compounded with `r8`/`sb`
  register pressure held live across two `sub_803ADB4` calls and a
  `sub_802E674` call in the trailing damage-calculation block. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802F7B0`**/**`sub_802F8E8`** (`src/graphics/actor_part45d.c`,
  issue #56) - a pair of VRAM tile-remap/nibble-repack loops (4-bit
  palette-index packing into a `0x0600D000`-based tile buffer via raw
  `REG_DMA3SAD`/`DAD`/`CNT` pokes at `0x040000D4`); each nested loop
  keeps three high registers (`r8`, `sb`, `sl`) simultaneously live
  across the whole loop body. Every other DMA3-setup function in this
  codebase with the same `0x040000D4`/`0x0600D000` literal-pool shape
  (`actor_part26b.c`, `actor_part74.c`, `actor_part75.c`,
  `fade_screen_mode.c`, `hud_digit_array.c`, `settings_menu8e.c`,
  `timer_util_aa90.c`) is NAKED too, not plain C with
  `REG_DMA3SAD`/`DAD`/`CNT` macros. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_8030734`** (`src/graphics/actor_part21d.c`) - `sub_80306AC`'s
  companion: ramps `gUnknown_03001560` toward a fixed target and, on
  its phase counter's armed tick, spawns via `sub_802E62C`. A plain-C
  reconstruction got everything but one statement's evaluation order
  byte-identical - this compiler always computes a `*dest = *(source
  expr)` assignment's RHS address before its LHS's, opposite of the
  ROM's own build. See `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8030834`** (`src/graphics/actor_part21e.c`) - distance/speed-
  gated variant of `sub_8030734`'s spawn (via `sub_802E674`); keeps the
  weapon table's phase pointer/value alive in `sb`/`r8` across a real
  `sub_803ADB4` call, the same many-high-register difficulty as
  `sub_8006600` et al. See `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_80309B4`** (`src/graphics/actor_part21f.c`) - large 5-way
  weapon-kind projectile spawner dispatching on `gUnknown_0300153C`;
  shares base coordinates across the dispatch in `r7`/`sb`/`sl`/`r8`.
  See `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8030D48`** (`src/graphics/actor_part23b.c`) - a rectangular
  BG-tilemap blit routine (docs/rom_map.md); nested loop keeps its
  counters/cursor in `sl`/`sb`/`r8`/`ip`. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8030E08`** (`src/graphics/actor_part23c.c`) - position-easing
  helper called from `sub_8030734`/`sub_8030834`; keeps two accumulator
  addresses and the player-position table in `sb`/`r8`/`sl`/`ip`. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8030F88`** (`src/graphics/actor_part23d.c`) - the tracker
  object's constructor (`mem_alloc` + the same state-0/table-index-0
  transition idiom already matched elsewhere in this cluster); a first
  plain-C attempt compiled shorter/differently-structured code and
  wasn't pursued further given the ~50-instruction prologue's many
  independent register-order choices. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8031040`** (`src/graphics/actor_part23e.c`) - large "arm this
  weapon-kind instance" setup; keeps its three position arguments and a
  per-kind table pointer in `r8`/`sb`/`sl` across several real calls.
  See `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_80311C4`** (`src/graphics/actor_part23f.c`) - large per-frame
  "advance this weapon-kind instance" driver; shares
  `sub_8031040`'s accumulator-recompute tail and hits the same
  operand-address-ordering gap as `sub_8030734`. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8031378`** (`src/graphics/actor_part24b.c`) - AABB overlap
  test against a keyframe-table box; same 12-byte
  `{s16 x,y,z,sizeX,sizeY,sizeZ}` shape and register-pressure reasons
  as the already-parked `sub_802DD9C`/`sub_802D7B0`
  (actor_part75.c/actor_part74.c). See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8031504`** (`src/graphics/actor_part26b.c`) - DMA/tile-cache
  setup + palette fade; pinning `&gUnknown_03001538` to `r7` (matching
  the ROM's own choice) hits a *correctness* hazard, not just a byte
  mismatch - this compiler reuses the "spare" r7 as scratch for an
  unrelated assignment in between the pin's two dereferences, so the
  second read would silently come from the wrong place. Abandoned
  immediately per docs/workflow.md step 3's warning. See
  `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8031604`** (`src/graphics/actor_part26c.c`) - VRAM fill-level
  meter nibble-repack loop (docs/rom_map.md's "procedurally-generated
  VRAM fill-level meter" finding); inner loop holds `r8`/`sb`/`sl`/`ip`
  live simultaneously. See `docs/matching/issue-58-0x08030574-actor.md`.
- **`sub_8011BD4`** (`src/graphics/actor_part82.c`, GitHub issue #16) -
  docs/rom_map.md's documented companion state machine to `sub_8016288`
  (still raw), sharing its type-`0x1d` gate: a 25-case jump table on a
  second parameter, with two of those cases sharing a further 7-case
  sub-dispatch on a nibble of a child object's `+4` byte. The single
  largest still-unmatched member of the `gStaticData_0816BF20` action-
  dispatch-table family (1420 B) - a first plain-C attempt at the
  smaller sibling `sub_8012420` (below) diverged immediately at the
  prologue, and a jump table this wide (25 outer cases plus two
  independent 7-case inner ones) was judged not worth the same
  register-pinning gauntlet already exhausted throughout this section
  for smaller members of the same table. See
  `docs/matching/issue-16-actor-remainder.md`.
- **`sub_8012420`**, **`sub_8012694`**, **`sub_801283C`**
  (`src/graphics/actor_part84.c`, GitHub issue #16) - three more
  members of the same 42-slot action-dispatch table
  (`gStaticData_0816BF20`): a `part`-visibility/OAM-priority
  housekeeping pass with a trailing 22-case jump table, a proximity-
  gated child-object-type dispatch that fires a `+0x50`/`+0x54`
  trampoline pair, and a proximity-triggered indicator dispatching on
  `self+8`'s type against per-type distance thresholds. A first plain-C
  attempt at `sub_8012420` compiled logically-equivalent code that
  diverged in overall stack-frame shape (the ROM reserves an unused
  8-byte stack slot and a 5th callee-saved register, `r7`, that the
  straightforward translation never needed) - the same unexplained-
  frame-shape gap this table's sibling members hit throughout this
  section. See `docs/matching/issue-16-actor-remainder.md`.
- **`sub_8012AF4`**, **`sub_8012D24`** (`src/graphics/actor_part83.c`,
  GitHub issue #16) - the chunk's final two functions: an OAM-
  visibility/priority pass keyed on a `gStaticData_0816B304` per-tag
  12-byte stack-local record (copied via `ldm`/`stm`, with `r8` holding
  a value across several calls - a real step up in register-allocation
  complexity from every other member of this table matched so far), and
  a further sibling/callee handling frame-counter thresholds, D-pad
  input, and `sub_8012A7C`'s busy-check. See
  `docs/matching/issue-16-actor-remainder.md`.
- **`sub_8012FBC`**, **`sub_8013228`** (`src/graphics/actor_part_12fbc.c`),
  **`sub_80134B8`** (`src/graphics/actor_part_134b8.c`), **`sub_80138E8`**,
  **`sub_8013994`** (`src/graphics/actor_part_138e8.c`), GitHub issue #17
  (leading portion) - five more members of the same 42-slot action-
  dispatch table: `sub_8012FBC`/`sub_80134B8` show the same unexplained
  extended-register-budget shape as this table's other NAKED members
  (`r8` alone, and `r8`/`sb`/`sl` together respectively, on top of the
  usual `r4-r7`); `sub_8013228`'s ordinary-looking `r4-r6`/`lr` prologue
  still diverged in a first plain-C attempt (an extra `r7` push plus the
  usual runtime-negated-mask idiom needed for `part[0xd] &= ~2`-style
  bit clears); `sub_80138E8`/`sub_8013994` each reproduce the exact
  `part[0x38]`-gated single-vs-double trampoline shape already confirmed
  unmatchable for `sub_80156EC` (above). New files are address-suffixed
  (`actor_part_<addr>.c`) rather than the next sequential `actor_partNN`,
  since `actor_part85.c` is independently claimed by unrelated issue #63
  work. Issue #17's own chunk continues past this pass's range
  (0x0801426C) through 0x08014F8C; the remainder stays raw in the
  trimmed `asm/code_3_2_17_12af4.s`. See
  `docs/matching/issue-17-0x08012fbc-actor.md`.
- **`InitActorCategory`** (`src/graphics/actor_part101.c`, GitHub issue
  #48) - the category (re)initialization + per-VBlank loading-screen
  driver. Fully understood; sustains four simultaneous high-register
  pins (`sb`/`sl`/`r8`/`ip`, each reused for 2-3 different roles) plus a
  stack-spilled loop-state variable threaded through many non-adjacent
  gotos, on a ~230-instruction, 20+-call, four-state loop body - a much
  larger instance of this project's "many-high-register-difficulty" gap
  (`sub_80091D4`/`sub_8009868` above). Verified byte-for-byte identical
  to the original raw disassembly (both assembled independently and
  compared directly, not just against `baserom.gba`). See
  [docs/matching/issue-48-0x080291a4-actor.md](../matching/issue-48-0x080291a4-actor.md).
- **`sub_80297C8`**, **`sub_8029890`**, **`sub_802996C`**
  (`src/graphics/actor_part95.c`, GitHub issue #48) - a "console"/text-
  plane cursor-cell DMA trigger, its geometry (re)configuration entry
  point, and the VRAM tilemap double-buffer fill pair it calls. Same
  register-role-permutation gap as `InitActorCategory` above, at
  smaller scale. See
  [docs/matching/issue-48-0x080291a4-actor.md](../matching/issue-48-0x080291a4-actor.md).
- **`sub_8029BC4`** (`src/graphics/actor_part98.c`, GitHub issue #48) -
  a VRAM tilemap-fill nested loop sharing `sub_802996C`'s shape and
  register-pressure wall. See
  [docs/matching/issue-48-0x080291a4-actor.md](../matching/issue-48-0x080291a4-actor.md).
- **`SelectActorCategory`** (`src/graphics/actor_part102.c`, GitHub
  issue #49) - sets up the selected category's runtime state and runs a
  two-pass `sub_effect_table` threshold scan. A real-C attempt
  reproduced every instruction but needed one extra live register
  (`r9`) beyond the ROM's own `sb`/`r8` pair. See
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).
- **`sub_802A018`**, **`sub_802A110`**, **`sub_802A3AC`**
  (`src/graphics/actor_part103.c`, GitHub issue #49) - `self`-vs-player
  3-axis AABB overlap tests (`sub_802A018`/`sub_802A110` near-identical,
  differing only in guard byte; `sub_802A3AC` wraps the same test in an
  actor-list walk) hitting this project's confirmed categorical
  gcc-2.9 `r7` register-allocation bug - the same wall as
  `sub_802D7B0`/`sub_802DD9C`/`sub_802C7A8` above. See
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).
- **`sub_802A208`** (`src/graphics/actor_part103.c`, GitHub issue #49) -
  a scroll enter/exit trampoline + `sub_effect_table` draw loop +
  double actor-list walk. Not the AABB shape above - a related but
  distinct gap, needing one extra high register (`r9`) beyond the ROM's
  single `r8` to keep three values simultaneously live. See
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).
- **`sub_802A674`**, **`sub_802A688`** (`src/graphics/actor_part94.c`,
  GitHub issue #49) - plain one-call trampolines, identical in shape to
  already-matched siblings elsewhere (`actor_part50.c`'s `sub_802A69C`);
  this compiler's epilogue register allocator picks `r1` instead of the
  usual `r0` for these two specific functions, a quirk that looks tied
  to this translation unit's cumulative pseudo-register count rather
  than anything controllable per-function. See
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).
- **`sub_8034994`** (`src/graphics/actor_part89.c`, GitHub issue #63) -
  the `struct fade_overlay` object's (`sub_803472C`/`sub_803487C`,
  actor_part87.c/actor_part88.c) per-frame input-poll/blend-alpha
  driver: busy-loops polling input twice per outer iteration (confirm
  exits with a cue; L/R step a one-shot flag with a cue), ping-ponging a
  0-15 blend-alpha counter into `REG_BLDCNT`/`BLDALPHA` every two
  iterations. Six live values (`sb`, `sl`, `r8`, three of `r4`-`r7`)
  across four different `bl` sites with no spare register - the same
  "many high registers held live across calls inside a loop" shape
  already NAKED throughout this codebase
  (`sub_80309B4`/`sub_8031040`/`sub_80311C4`,
  `actor_part21f.c`/`23e.c`/`23f.c`). See
  `docs/matching/issue-63-final-raw-actor.md`.
- **`sub_80317E0`** (`src/graphics/actor_part125.c`) - a proximity-
  gated event trigger: syncs via `sub_802A980`, checks a camera-
  relative bound against `self+0x34`, flushes a pending trampoline
  call at `self+0x58`, and either draws a text popup through the event
  table at `self+0x50` or falls back to `sub_8031A08`. Fully
  understood; this compiler doesn't reach for `r4` as the whole-
  function `self` pin the ROM uses without also perturbing the branch
  layout. See `docs/matching/issue-59-0x08031784-actor.md`.
- **`sub_8031858`** (`src/graphics/actor_part125.c`) - a health/
  damage-countdown death transition at `self+0x54`, same overall shape
  as the boss cluster's `sub_8030530`. Fully understood; the death-
  byte store's `1`/`0` constants need to stay live and shared across
  both the early-flush branch and the later state-transition block in
  a way that resisted a register-pin reproduction without changing the
  branch shape. See `docs/matching/issue-59-0x08031784-actor.md`.
- **`sub_80318D0`/`sub_8031954`/`sub_80319A0`** (`src/graphics/actor_part125.c`)
  - three instances of the same shared anim-frame-advance-and-clamp
  idiom (the first stashes 3 incoming args, the third adds an
  oscillation drive). Fully understood; this compiler always schedules
  the `#4`/`#6` `ldrsh` constant loads one instruction earlier than the
  ROM's own build. See `docs/matching/issue-59-0x08031784-actor.md`.
- **`sub_8031A08`** (`src/graphics/actor_part125.c`) - draws a
  keyframe-table-relative text popup, indexing `gStaticData_0817C414`
  by `self+0x28`. Fully understood; resists a byte-exact reproduction
  of the ROM's specific `r7`-as-table-base-pin choice. See
  `docs/matching/issue-59-0x08031784-actor.md`.
- **`sub_8034AA4`** (`src/graphics/actor_part131.c`, GitHub issue #64) -
  the fade overlay's Yes/No dialog draw, another instance of the
  "refresh OAM + center text" pattern: `self` (r6), the OAM-shadow-
  buffer address (sl), the constant `0x87` (r7), and two `0x130`/`0x98<<1`
  index constants (r8/sb) all stay resident across many `bl` sites with
  no register left over. See
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).
- **`sub_8034CEC`** (`src/graphics/actor_part131.c`, GitHub issue #64) -
  the map screen's constructor: `self` (r5), a zero constant (r8), and
  `&gUnknown_030012E0` (sb) stay resident across a long run of `bl`
  sites, while r4/r6 each get rebound to a different global's address
  multiple times over that same span with several unrelated calls in
  between each rebinding - the same shape `sub_803487C`
  (actor_part88.c) hit. See
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).
- **`sub_8034EF0`** (`src/graphics/actor_part131.c`, GitHub issue #64) -
  the map screen's per-frame OAM-icon draw dispatcher for the minimap
  object: the inner tile loop keeps six independent running values live
  simultaneously (sl/sb/r8 plus r4-r7) across a `bl` inside a nested
  loop. See
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).
- **`sub_80350A4`** (`src/graphics/actor_part131.c`, GitHub issue #64) -
  the map screen's floating-text popup driver: `self` (r5), the
  list-tail pointer (r8), the running max-width accumulator (sb), and
  the horizontal pen-position accumulator (sl) all stay resident across
  many `bl` sites spanning several nested loops. See
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).
- **`sub_80352AC`** (`src/graphics/actor_part131.c`, GitHub issue #64) -
  the map screen's popup-text asset loader: the innermost tile-index
  loop holds seven live values simultaneously (r8/sl/sb/ip plus r0-r6),
  a fully packed register budget with no `bl` inside the innermost loop
  to even attempt a spill. See
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).

## Parked (`NON_MATCHING`, not yet byte-exact)

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

- **`sub_8007DBC`** (`src/graphics/actor_part2.c`) - the player-collision
  "kind" spawner. Hits this project's confirmed categorical r7-pin
  compiler bug. GitHub issue not tracked separately, see
  `docs/matching/naked-sub_8007dbc.md`.
- **`sub_802C7A8`** (`src/graphics/actor_part19h.c`) - a circular-list
  AABB-overlap "chain pickup" scan: walks the whole `self+0x4c`-rooted
  actor list looking for type-4 nodes overlapping `self`'s own
  translated `self+0x38` box, firing the shared used-state transition
  on each match. Same heavy two-scratch-AABB-record-plus-loop-lifetime-
  `r7` shape as `sub_802D7B0`/`sub_802DD9C` below - hits the same
  confirmed categorical gcc-2.9 r7-pin bug. GitHub issue #53, see
  [docs/matching/issue-53-actor-c7a8.md](../matching/issue-53-actor-c7a8.md).
- **`sub_802CC9C`** (`src/graphics/actor_part126.c`) - a heavy AABB-
  overlap/proximity state machine: reuses `self+0x38` as a scratch box
  across three `gStaticData_0817A7xx` record snapshots and two
  `sub_802A6EC`/`sub_802DD9C`/`sub_802B7E0` proximity checks, with
  `r5`/`r6`/`r7` each switching roles mid-function. Same categorical
  gcc-2.9 register-reuse family as `sub_802D7B0`/`sub_802DD9C` below.
  Between issues #53 and #54, see
  [docs/matching/issue-53-issue-54-gap-cc9c.md](../matching/issue-53-issue-54-gap-cc9c.md).
- **`sub_802D3A8`** (`src/graphics/actor_part62.c`) - eases `self`'s
  cached position toward a per-state target/table-scatter offset. Hits
  this project's confirmed categorical gcc-2.9 r7-pin bug. GitHub issue
  #54, see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md).
- **`sub_802D7B0`**, **`sub_802D9A8`**, **`sub_802DA68`**
  (`src/graphics/actor_part74.c`) - a confirmed slot (index 3) of the
  type-0 `category_vtable` (also runs a full 3-axis AABB overlap test
  against the player before dispatching a `sub_803AD80` trampoline) and
  a palette-gradient/hardware-sound cursor pair for the
  `gUnknown_030014BC` object. GitHub issue #54, see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md).
- **`sub_802DD9C`**, **`sub_802DE70`** (`src/graphics/actor_part75.c`) -
  the self-vs-player 3-axis AABB overlap test factored out of
  `sub_802D7B0` (used by `sub_802D6A0`, `actor_part58.c`) and the
  `gUnknown_030014BC` object's ~160-instruction VRAM gauge-tile bitmap
  generator/DMA setup. GitHub issue #54, see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md).
- **`sub_802E058`** (`src/graphics/actor_part76.c`) - a parameterized
  twin of `sub_802DE70`'s VRAM gauge-tile triangular-fill loop. GitHub
  issue #54, see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md).
- **`sub_803B46C`** (`src/graphics/actor_anim.c`) - fixed-position
  (120, 106) OAM setup for one sprite frame - screen-space visibility
  cull, then builds the OAM attribute words and calls
  `SetupSpriteFrameOam`; near-identical twin of `sub_802C2FC`
  (`actor_part19b.c`, see below). Now fully matched as real C: the
  `| 0`-dead-store idiom closes via the established opaque-asm idiom,
  and the register-budget gap the original parking cited turned out to
  be an r7-pin-hazard artifact (leaving `frame` unpinned lets the
  natural allocator land it in r7 correctly) rather than a genuine
  register shortage. GitHub issue #71, see
  [docs/matching/issue-71-0x0803b060-actor.md](../matching/issue-71-0x0803b060-actor.md).
- **`sub_802C2FC`** (`src/graphics/actor_part19b.c`) - fixed-position
  OAM setup for one sprite frame, `sub_803B46C`'s twin above; now fully
  matched as real C. The dead `flag = 0` initializer closes via an
  opaque two-instruction `asm volatile` materialization (a single
  `mov r8, #0` isn't valid Thumb - only lo registers take an
  immediate `mov`), and the remaining register-choice gaps close with
  the same pin-matching techniques worked out for `sub_803B46C`. The
  old raw `asm/code_3_2_20_28568_c2fc.s` is retired. See
  `docs/matching.md`, issue #52.
- **`sub_802C3E8`** (`src/graphics/actor_part19c2.c`) - a homing/
  seek-toward-point spawn-effect constructor; now fully matched as
  real C. The Manhattan-distance abs-value computation uses the ROM's
  own branchless idiom (`(x ^ (x >> 31)) - (x >> 31)`, compiling to
  `asr`/`eor`/`sub`) rather than a `(x < 0) ? -x : x` ternary, which
  this compiler turns into a `cmp`/`bge`/`neg` branch instead. Also
  fixes a genuine correctness bug found while tightening the register
  match: pinning the `self+0x1c` reload to `r1` *before* the
  `sub_8029EB4()` call it's actually meant to follow let this
  compiler's optimizer silently skip the reload and reuse a stale
  register value from an unrelated earlier computation - caught by a
  direct byte compare against the ROM, not just a register-choice
  cosmetic mismatch. The old raw `asm/code_3_2_20_28568_c3e8.s` is
  retired. See `docs/matching.md`, issue #52.
- **`sub_8034270`** (`src/graphics/actor_part68.c`) - a position-sync/
  flag/trampoline updater; now fully matched as real C. The ROM
  computes a "should animate" 0/1 value and re-checks it against zero
  even though the value is a compile-time constant on each path -
  this compiler's dead-branch elimination always collapsed that
  redundant compute-then-recheck step for a plain local, closed via
  an empty `asm volatile("" : "+r"(doAnim))` making the value opaque
  right before the check. The old raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34270.s` is retired. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034314`** (`src/graphics/actor_part70.c`) - `sub_8034270`'s
  boolean-returning twin; matched as real C immediately, no opaque-asm
  fix needed - returning the value directly (rather than branching on
  it to decide whether to call `sub_802A7B8`) means there's no
  recheck for dead-branch elimination to collapse. The old raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34314.s` is retired. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_802F338`** (`src/graphics/actor_part43b.c`) - computes two
  keyframe-driven tile-cache sizes via `AllocVramTileBlock`; now fully
  matched as real C. The ROM's "materialize the multiply result, then
  copy it again before shifting" idiom (`adds r2,r3,#0; muls r2,r1,r2;
  adds r0,r2,#0; lsls r0,r0,#5`) closes via an opaque `asm volatile`
  forcing the exact register-to-register copy this compiler's
  dead-store elimination always collapsed. Closing that gap surfaced a
  further chain of register-role mismatches in the index/address
  computation (`table` needing an early load into `r3`, the `+2` index
  constant needing to be materialized via an opaque `mov #2` rather
  than a plain `register`-pinned local, and the two blocks' final
  byte-load pairs needing per-block register pins matching the ROM's
  own `ldrb` register choices), each closed with the same
  register-pin/opaque-asm technique. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_802FA04`** (`src/graphics/actor_part45c.c`) - an
  `InitActorPart`-based constructor for this cluster's `self` object:
  forwards its first three real arguments plus one stack argument
  straight to `InitActorPart`, then marks `self+0x54` = 1, sets
  `self+0x50`'s event/trampoline table to `gStaticData_087E517C`, and
  stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`;
  now fully matched as real C, closing the gap the same 7-argument
  `InitActorPart`-wrapper shape is still parked on for `sub_80305F8`.
  Explicitly pinning `e`/`f` to their ROM registers (`r6`/`r7`) either
  adds a spurious extra `r8` push/pop (a relay attempt) or - for `r7`
  specifically - drops that register from the compiler's own prologue
  push/pop list outright (a genuine agbcc/gcc 2.9 Thumb-prologue bug,
  the same quirk `sub_802F6DC` above hit for a plain low-register pin).
  The fix: pin only the constant `1` to `r5`; leave `self`, the stack
  argument `d`, and both `e`/`f` completely unpinned (`self` as a plain
  `u8 *` local, `d` used directly as `InitActorPart`'s stack argument,
  `e`/`f` as plain `register` locals with no explicit hardware
  register). With that much natural register pressure, this compiler's
  own allocator picks `r4`/`r6`/`r7` for `self`/`e`/`f` on its own -
  correctly including all of `r4`-`r7` in the push/pop list - and, in
  the declaration order `self`, then the pinned constant, then `e`,
  then `f`, schedules the loads in the ROM's own self/d/e/f/constant
  order. Retires the old raw `asm/code_3_2_20_28568_c99c_2fa04.s`. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`sub_800B270`** (`src/graphics/actor_part49.c`, GitHub issue #9) -
  a per-frame velocity integrator moving `self+0x60`/`self+0x64`
  toward `self+0x50`/`self+0x5c` by `self+0x4c`/`self+0x58` each call,
  deriving a `self+0x24` direction-flag byte and applying the result to
  the object's position, then recording the resulting Y velocity into
  an unlabeled RAM address (`0x0300129C`); now fully matched as real C.
  The trailing `0x0300129C` block's "genuinely redundant" conditional
  store (see the file's header comment) gets proven dead by this
  compiler regardless of C-level phrasing, collapsing its guard down to
  just one half of the `&&` - closed by emitting the whole load/
  compare/branch/store sequence verbatim via one opaque `asm volatile`
  block instead of fighting the optimizer's proof, which also
  reproduces the ROM's own address-in-`r0`/value-in-`r2` register
  choice directly. Also found and fixed a genuine gcc-2.9 register-pin
  miscompile while closing this: pinning both `vx` and `vy` (the X/Y
  velocities, needed in `r3`/`r1` to match ROM) at once made the
  function's own `return (vx != 0 || vy != 0)` fold to an unconditional
  `mov r0, #1` - fixed by pinning only `vx`, leaving `vy` an unpinned
  local (it lands in `r1` naturally anyway). Retires the raw
  `asm/code_3_2_16_b270.s`. See
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_8009DF4`** (`src/graphics/actor_part8.c`, issue #97) - a
  velocity/position integrator: steps each axis's velocity toward its
  max by its accel amount (clamped so it never overshoots), builds a
  direction-flags byte from the clamped velocities' signs, caches the
  pre-move position, applies the velocity, and updates a global
  (`gUnknown_03001298`) with the Y velocity; now fully matched as real
  C. Closed using the exact fix worked out for its near-identical twin
  `sub_800B270` above (same per-axis clamp shape, same field offsets):
  `vx` pinned to `r3` while `vy` stays unpinned, and the trailing
  global-update block's "genuinely redundant" conditional store emitted
  verbatim via one opaque `asm volatile` block instead of fighting this
  compiler's dead-store-elimination proof, reproducing the ROM's own
  address-in-`r0`/value-in-`r2` register choice directly. Retires the
  old raw `asm/code_3_2_9.s`. See
  [docs/matching/issue-97-sub_8009DF4.md](../matching/issue-97-sub_8009DF4.md).
- **`sub_8034374`** (`src/graphics/actor_part85.c`) - constructs the
  particle-trail BG0 object; now fully matched as real C. The ROM
  builds a 4-bit-palette-bank tile-index mask (0xFFFFF000) by loading
  the literal into `r1` first and copying it into `r5` (`ldr
  r1,=0xFFFFF000; adds r5,r1,#0`), rather than the single direct `ldr`
  a plain `mask = -0x1000;` compiles to - closed by pinning an
  intermediate local to `r1` (its initializer must stay a plain C
  constant, not an inline-asm-embedded immediate, so the value stays in
  the compiler's own literal pool at the ROM's actual pool position
  rather than becoming a second, separately-pooled literal appended
  after it) and forcing the `r1`->`r5` copy via `asm volatile`. Also
  needed the `mapBase + (row << 6)` addition's operand order pinned the
  same way, and the `col = 0x1d` initializer moved after that
  computation in the C source (gcc otherwise schedules a trivial
  immediate move ahead of a nearby pinned-register asm block by its
  literal source position). Retired the multi-function raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`, split at the time
  into `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s` (real bytes for
  the twin `sub_8034480`) - that fragment is now retired too (see
  below), closing `actor_part85.c` entirely. GitHub issue #63, see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034480`** (`src/graphics/actor_part85.c`) - `sub_8034374`'s
  companion per-frame updater; now fully matched as real C, closing the
  file. Commits last frame's `tileBuffer` to tile VRAM, clears it back
  to zero, then for each active particle inlines the same nibble-
  address formula as `sub_8034634` (`actor_part72.c`) *twice* (nibble
  `1` pre-move, nibble `2` post-move), applying `dx`/`dy` and
  respawning via `sub_80345B0` in between. Needed a mix of
  `sub_8034634`'s own three fixes (unsigned casts for the `x`/`newX`
  bounds checks alongside plain signed `>> 11` block-index shifts,
  `addr`'s two halves split into separate statements, and one opaque
  `asm volatile` for the `bic`/`orr`/`strh` tail - simpler here than
  `sub_8034634`'s own tail since this function's ROM build never needs
  an extra materialize-then-copy-back step) plus two more scheduling-
  order fixes this larger, twice-inlined function surfaces on its own:
  `x`'s raw value and its `>>8` pixel value must be computed
  immediately, before `y` is even loaded (same for `blockX`'s `<<6`
  term before `blockY` loads), the post-move `slot->x = newX` store
  must happen immediately after computing `newX` rather than batched
  with the `y` store, and the second inlined copy's `oldVal` (pinned to
  `ip`) must be assigned by a plain statement after the position
  reload rather than as its `register` declaration's own initializer.
  Retires `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s` entirely.
  GitHub issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_80345B0`/`sub_8034634`** (`src/graphics/actor_part72.c`) - a
  128-slot particle-slot spawner (rolls two `sub_8000E1C` random values
  against the 256-entry `gStaticData_0816A820` direction table to seed a
  position/velocity record) and a 4-bit-per-cell tilemap nibble writer;
  now fully matched as real C. `sub_80345B0`'s previously-parked
  register-allocation gap for the final multiply/shift turned out not to
  need any register pins or opaque asm at all: this compiler's codegen
  for `dest = a * b` always materializes/copies the *left* operand into
  the destination register before the `muls`, and the ROM's own build
  happens to write the table lookup as the left operand
  (`table[...] * speed`) rather than the speed value - simply writing the
  C multiplication in that same order matched immediately.
  `sub_8034634`'s residual `addr`/`blockY` register-role gap turned out
  to be three separate, independently-found issues: `x`'s bounds check
  wants an unsigned compare but `x >> 3` wants a *signed* arithmetic
  shift (modeled with an explicit `(s32)x >> 3` cast), `addr`'s two
  halves needed splitting into separate statements to pin gcc's
  operand-evaluation order (a combined `a + b` expression let it
  evaluate the blockY half first, opposite the ROM's x-half-first
  order), and the temporary `mask` needed to stay a plain 32-bit type
  (`u32`, not `u16`) to avoid a spurious 4-instruction 16-bit-truncation
  sequence this compiler otherwise inserts around `0xf << shift` (the
  low 16 bits are all `bics`/`orrs` ever reads, so the truncation was
  never actually needed). The final residual gap - the ROM's own
  "materialize `cell`, `bics` it, then copy the result back before
  `orrs`/`strh`" idiom, the same class of redundant-copy-after-a-binary-op
  quirk already seen for `sub_802F338`'s multiply - closed with one
  opaque `asm volatile` block reproducing that exact instruction
  sequence, taking `shift` and a `register ... asm("r2")`-pinned
  `tileMapEntry` as inputs and the already-`r3`-pinned `val` as an
  in/out operand. Retires
  `asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s` entirely. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034058`** (`src/graphics/actor_part66.c`, GitHub issue #63) -
  an `InitActorPart`-based constructor with a trailing (stack-passed,
  byte-sized) 6th argument and a spawn-record ternary; now fully
  matched as real C. Two gaps, both closed with `asm volatile`
  anchors: the 6th argument needs the same stack-slot-address-then-
  `ldrb` anchor already established for other trailing byte arguments
  (see `sub_8003A60` in `issue-5-overlay-ui-sync.md`), materialized
  into a `register u32 asm("r9")` pin mirroring the ROM's own `sb`
  cache; and the `self+0x5c` spawn-record ternary needs the ROM's
  genuine two-way branch diamond (a forward `beq`/`ldr`/`b` skipping a
  computed false branch, with two literals pooled together right after
  the skip) rather than the eager "compute one value, conditionally
  overwrite" shape this compiler always produces from a plain ternary
  or if/else - and, separately, forcing that diamond through any
  register-pinned C-level if/else made this compiler's own parameter-
  homing pass reorder `self`'s prologue copy relative to
  `part`/`b`/`cParam` (still the correct register, just the wrong
  instruction position), a discrepancy that didn't respond to any
  combination of pinning/goto/precomputed-address tried. Closed by
  moving the diamond into one opaque `asm volatile` block referencing
  `self`'s known `r5` home directly by name (not as an operand, which
  is what avoids perturbing the surrounding allocation) with a real
  `ldr r0, =0xFFFFBF00` assembler pseudo-op and a manual `.pool`
  directive right after the skip branch - and moving the preceding
  `gStaticData_087E5554` store into its own tiny `asm volatile` island
  too, since a real, respected `.pool` split only works for symbols
  whose literal load is itself opaque assembler text, the same gap
  already documented for `sub_802AB58` in `actor_part53.c`. Retires
  the raw `asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s`. See
  `docs/matching/issue-63-0x08033ef4-actor.md`.

- **`sub_800A884`** (`src/graphics/actor_part78.c`, GitHub issue
  #9/#10; real bytes in `asm/code_3_2_16_a884.s`) - a per-frame
  reentrancy-guard-shaped wrapper dispatching a pending-action "kind"
  byte (`gUnknown_03001308+0x29`) through a 10-case jump table, then a
  keyframe-lookup/camera-position probe via `sub_80083B8`/
  `sub_8026BC0` sharing `sub_80084C4`'s case-to-block mapping. Every
  load/store/branch/call confirmed correct against the ROM, and now
  (a second follow-up session, 98.0% fuzzy-matched, up from 96.8%)
  register-for-register byte-exact almost everywhere: both jump
  tables, all 10 case bodies (two needing `asm volatile` islands to
  reproduce a ROM cross-case tail-merge), the camera-probe tail's
  Y-snap arithmetic (closed this session - a `(masked + 7) - y`
  expression was getting re-associated into a different register
  pairing than the ROM's own), and `kindZero`'s `self+0x68 == 8` test
  (also closed this session, via a matching-constraint `asm volatile`
  reusing `p68`'s own already-`r7` allocation rather than forcing a
  brand-new register binding) all match. One narrow, purely
  register-*choice* gap remains (the `self+0x105` clear's scratch-
  register pick, `r0` here vs. the ROM's `r2` - reconfirmed resistant
  to every register-pin/`asm` variation tried across two sessions now,
  without perturbing other, already-matching code, the "ripple"
  effect), plus one small single-instruction side effect of closing
  the `kindZero` gap (a redundant `movs r1, #0` the compiler schedules
  from provable-constant-propagation that no placement of the source
  assignment moved or eliminated). See
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).
- **`sub_800AB9C`** (`src/graphics/actor_part81.c`, GitHub issue #9/#10;
  real bytes in `asm/code_3_2_16_ab9c.s`) - a two-flag-gated teardown/
  notification step on the same still-unnamed "big object" (0x108+
  bytes) `actor_part15.c`/`actor_part77.c` work on: relocates `self`'s
  AABB onto a second stack slot before unpacking it for `sub_8008A40`
  (bit 1), and clears `self+0x108`/`self+0x10c` plus fires three
  teardown/notification calls (bit 7). Every instruction matches except
  one gap: the ROM evaluates `sub_8008A40`'s 7 arguments in an order
  (stack-bound values interleaved with their own store, register
  values and the `r0`-bound `manager` last) this compiler never
  reproduces from any C-level phrasing tried - the same already-
  accepted-as-unclosable class as `sub_8008AD8`/`sub_8008D80` right
  next door (`actor_part7.c`) and `PlaySfx` (issue #3). See
  [docs/matching/issue-9-10-0x0800ab9c-graphics.md](../matching/issue-9-10-0x0800ab9c-graphics.md).
- **`sub_800AAEC`** (`src/graphics/actor_part108.c`, GitHub issue
  #9/#10) - the input-action-check function the 42-slot
  `gStaticData_0816BF20` action-dispatch table's entries call: gates a
  `sub_8026628` proximity probe against the player, then walks
  `gUnknown_0300130C`'s object list, testing each entry via
  `sub_803AD7C` and calling `sub_800CD00` (now examined and closed,
  `actor_part109.c`) on a hit. Every instruction matches except one
  5-instruction pair (the `gUnknown_0300130C` list-walk's loop-
  condition-check/loop-entry register roles - the ROM re-loads
  `&gUnknown_0300130C` from the literal pool fresh every iteration,
  landing it in `r0` and reusing that register as the dereferenced
  value in place; no C phrasing tried reproduced that exact
  per-iteration reload shape without either losing the reload
  entirely via gcc's own loop-invariant hoisting, or costing an extra
  callee-saved register). Default build uses a byte-exact NAKED
  transcription instead - see
  [docs/matching/issue-9-10-0x0800aaec-graphics.md](../matching/issue-9-10-0x0800aaec-graphics.md).
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
  **`sub_8008044`** (`src/graphics/actor_part3.c`), and
  **`sub_800891C`/`sub_8008A40`/`sub_8008AD8`**
  (`src/graphics/actor_part7.c`) / **`sub_8008D80`**
  (`src/graphics/actor_part7b.c`) - stayed parked here for a long time
  on exactly the register-canonicalization/stack-layout classes of gap
  documented at length throughout this page (`r7`-pin hazards and a
  C-inexpressible stack-layout coincidence). All seven are `NAKED`
  functions whose bodies are a literal instruction-for-instruction
  transcription of the ROM's own assembly (byte-exact, confirmed via a
  full clean `make compare`), rather than a derived C reconstruction -
  see `docs/matching/naked-oam-actor-part-batch.md`. Per project
  policy, a NAKED transcription standing in for a substantial
  function's register-allocation gap doesn't count as "matched" the way
  real decompiled C does, so all seven stay filed here rather than in
  "Matched" above, and `tools/report_units.py` tracks their address
  ranges as unmatched (`base_object: None`). `sub_8008770`
  (`src/graphics/actor_part6.c`) and `sub_8008188`/`sub_8008200`/
  `sub_8008278`/`sub_80083B8` (`src/graphics/actor_part4.c`/
  `actor_part5.c`), which shared this list in earlier versions of this
  page, were converted back to real C and matched in later sessions -
  see "Matched" above and `docs/matching.md`'s "Parked, not matched:
  sub_8008770"/"Parked, not matched: sub_8008188" entries (and the
  latter's siblings) for those accounts.
- **`sub_8008F20`** (`src/graphics/actor_part11.c`) - initializes a
  fixed-slot object-pool manager struct: two big 256-word zeroed
  tables (likely a pair of spatial-partition/collision grids), plus a
  singly-linked free list built over an allocated node array. Every
  load, store, and field offset confirmed correct; hits a many-register
  (item count, two persistent field addresses, a reused loop index, a
  running byte offset) allocation gap across `r3`/`sb`/`sl`/`r4`/`r8`
  that no C-level reconstruction reproduced, so converted to `NAKED`
  the same way as `sub_80096C0`/`sub_80099F0` below: a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction, compiled directly into `actor_part11.o`
  alongside the matched functions above it (no separate object needed,
  same as `sub_8008044`/`actor_part3.c`'s precedent for a NAKED
  function sharing a file with matched ones). Per project policy this
  doesn't count as "matched" the way real decompiled C does, so it
  stays filed here rather than in "Matched" above - see
  `docs/matching/naked-sub_8008f20-sub_8009914-freelist.md`.
- **`sub_8009528`** (`src/graphics/actor_part11f.c`) - the spatial-
  hash-grid-cluster analog of `sub_8008A40`: the same grid-iteration
  shape as `sub_800944C`, dispatching each hit to `sub_80096C0`/
  `sub_80099F0` exactly like `sub_8008A40` dispatches to
  `sub_8008AD8`/`sub_8008D80`. Every branch, field offset, and call
  argument was confirmed correct, but the `#if NON_MATCHING` C
  reconstruction's compiled size stayed larger than the real ROM
  function (a bigger gap than the established `boxH` issue alone), so
  converted to `NAKED`: a literal instruction-for-instruction
  transcription of the ROM's own assembly (byte-exact, confirmed via a
  full clean `make compare`), both grid passes byte-identical to each
  other. Per project policy this doesn't count as "matched" the way
  real decompiled C does, so it stays filed here rather than in
  "Matched" above - see `docs/matching.md`, "Parked, not matched:
  `sub_8009528`".
- **`sub_80096C0`** (`src/graphics/actor_part11e.c`) - `sub_8008AD8`'s
  twin: byte-identical collision-hit resolution logic, operating in
  this spatial-hash-grid cluster instead of the plain array manager -
  in fact its instruction stream is byte-identical to `sub_8008AD8`'s,
  down to the label offsets. Same `boxH` stack-layout gap as
  `sub_8008AD8`/`sub_8008D80`/`sub_80099F0`, so converted to `NAKED`
  the same way: a literal instruction-for-instruction transcription of
  the ROM's own assembly (byte-exact, confirmed via a full clean `make
  compare`), rather than a derived C reconstruction. Per project
  policy this doesn't count as "matched" the way real decompiled C
  does, so it stays filed here rather than in "Matched" above - see
  `docs/matching.md`, "Parked, not matched: `sub_80096C0`".
  `sub_8008AD8` itself (`src/graphics/actor_part7.c`) is unaffected by
  this change and remains its own separate `NAKED` function.
- **`sub_8009914`** (`src/graphics/actor_part11i.c`, new file - its
  real ROM address, `0x08009914`, doesn't sit adjacent to
  `actor_part11.c`'s own functions, the same reason `sub_80096C0`
  above got its own `actor_part11e.c` (named "i" - `sub_8009528`
  claimed `actor_part11f.c`, the now-matched `sub_8009150` claimed
  `actor_part11g.c`, and the now-matched `sub_800944C` claimed
  `actor_part11h.c`, all in parallel PRs merged first)) - resets a
  pool manager to empty: tears down every active object, then rebuilds the grid and
  free list from scratch. The teardown loop was confirmed correct on
  its own; the rebuild loop is a byte-for-byte copy of `sub_8008F20`'s
  own tail and hit the identical many-register allocation gap, closed
  via the same `NAKED` transcription technique - a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction. Per project policy this doesn't count as
  "matched" the way real decompiled C does, so it stays filed here
  rather than in "Matched" above - see
  `docs/matching/naked-sub_8008f20-sub_8009914-freelist.md`.
- **`sub_80099F0`** (`src/graphics/actor_part12.c`) - `sub_8008D80`'s
  twin: byte-identical in shape (same collision-hit-resolve logic,
  same "dead read" trampoline call), called from elsewhere in this
  cluster - in fact its instruction stream is byte-identical to
  `sub_8008D80`'s, down to the label offsets. Same `boxH` stack-layout
  gap, so converted to `NAKED` the same way: a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction. Per project policy this doesn't count as
  "matched" the way real decompiled C does, so it stays filed here
  rather than in "Matched" above - see `docs/matching.md`, "Parked,
  not matched: `sub_80099F0`". `sub_8008D80` itself
  (`src/graphics/actor_part7b.c`) is unaffected by this change and
  remains its own separate `NAKED` function.
- **NAKED transcription (byte-correct, not decompiled)**: `sub_802C208`
  (`src/graphics/actor_part19e.c`, issue #52), `sub_802F748`
  (`src/graphics/actor_part44b.c`, issue #56), `sub_8033B44`/
  `sub_8033C84`/`sub_8033E80` (`src/graphics/actor_part31.c`/
  `actor_part33.c`/`actor_part37.c`, issue #62), and `sub_8033FE4`
  (`src/graphics/actor_part64.c`, issue #63) - the same
  `gStaticData_*` stride-8 trampoline-record dispatcher shape across
  four different tables, each hitting this project's confirmed
  categorical gcc-2.9 r7-pin bug (the ROM keeps the table's base
  address alive in `r7` for the whole function; an explicit
  `register T x asm("r7")` compiles correct instructions but never
  makes it into the prologue/epilogue push/pop list, and this
  compiler's own unforced allocator never reaches r7 here either).
  Every instruction is a byte-verified transcription of the ROM
  disassembly, so these produce byte-identical output, but since the
  function bodies are hand-written asm rather than real decompiled C
  they are **not** counted as matched for this project's tracking
  (`tools/report_units.py`'s `UNITS` list keeps their `base_object` as
  `None`) - see docs/matching/issue-52-0x0802bed8-actor.md and the
  per-issue docs for #56/#62/#63.
- **`sub_80339DC`** (`asm/code_3_2_20_28568_c99c_31784_339dc.s`, C in
  `src/graphics/actor_part29.c`) - a proximity-triggered effect/hazard
  detector measuring `self`'s distance to the player after syncing to
  the singleton's position. Every load/store, branch and call
  confirmed correct; parked on a residual register-allocation gap for
  one 16-bit constant materialization this compiler won't place in the
  ROM's chosen scratch register without breaking the surrounding
  `ip`/`r8`/`r9` pins - see
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

- **`sub_803487C`** (`asm/code_3_2_20_28568_c99c_31784_33ef4_3487c.s`, C
  in `src/graphics/actor_part88.c`, GitHub issue #63) - the fade
  overlay's other setup half: VRAM upload cursor flush, icon-manager
  hookup, and a 16-iteration tile-cache seeding loop; matched except the
  loop's trip counter, which the ROM keeps live in r7 for the whole
  loop - this project's confirmed categorical gcc-2.9 r7-pin bug (see
  `graphics_package_1e688.c`/`oam_count.c`/`actor_part7.c`) - see
  `docs/matching/issue-63-final-raw-actor.md`.

## Left raw (not attempted, or attempted and set aside)

- **`sub_8018008`/`sub_8018400`/`sub_801865C`/`sub_80186F0`**
  (`asm/code_3_2_17_18008.s`, ROM 0x08018008-0x080186F0, GitHub issue
  #22) - a ~480-instruction jump-table player action-state machine
  plus two high-register-pressure helpers it calls; left raw, out of
  scope for this pass - see `docs/matching/issue-22-0x08017a44-actor.md`.
- **`sub_8011BD4`** (`asm/code_3_2_17_11bd4.s`, ROM 0x08011BD4, GitHub
  issue #16) - `docs/rom_map.md`'s documented ~1420B, 25-case/7-case
  nested jump-table companion state machine to `sub_8016288` (still
  raw, type-`0x1d` player-control family); left raw, out of scope for
  this pass - see
  [docs/matching/issue-16-actor-11b0c.md](../matching/issue-16-actor-11b0c.md)
  and
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- **`sub_8012420`/`sub_8012694`/`sub_801283C`** (`asm/code_3_2_17_12420.s`,
  ROM 0x08012420-0x08012A7C, GitHub issue #16) - further members of the
  42-slot action-dispatch-table family (`gStaticData_0816BF20`) reading/
  writing the same still-unnamed "child object" struct `sub_8012160`/
  `sub_8012238`/`sub_80122CC` (matched below) now operate on; not
  attempted to byte-exact precision this pass. Left raw - see
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- **`sub_8012AF4`/`sub_8012D24`** (`asm/code_3_2_17_12af4.s`, ROM
  0x08012AF4-0x08012FBC, GitHub issue #16) - `sub_8012AF4` uses a
  stack-local 12-byte record copy and `r8`; `sub_8012D24` is a further
  sibling; neither attempted to byte-exact precision this pass. Left
  raw - see
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- **`sub_8007634`** (`asm/code_3_2.s`, ROM 0x08007634, GitHub issue #9)
  - real GBA hardware-affine sprite-matrix setup; already flagged in
  `docs/matching.md` as needing "a dedicated session" of its own, not
  attempted again here - see
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`sub_800AC2C`** (`asm/code_3_2_16_ac2c.s`, ROM 0x0800AC2C, GitHub
  issue #9/#10) - a 38-case player action-state jump-table dispatcher
  calling a dozen still-unexamined state-transition functions; left
  raw per this project's established policy for this exact dispatcher
  shape (same as `sub_8018008`, issue #22) - see
  `docs/matching/issue-9-10-0x0800ab9c-graphics.md`.
- **`sub_8016048`** (`asm/code_3_2_17_16048.s`, ROM 0x08016048, GitHub
  issue #19) - a smaller joystick-input-gated dispatcher; left raw, out
  of scope for this pass - see
  `docs/matching/issue-19-0x08015840-actor.md`.

# Issue #52: 0x0802BED8-0x0802C99C (actor)

Follow-up to `docs/matching.md`'s frozen "0x0802BED8-0x0802C99C" entry
(issue #52) - that file is a frozen historical record and never gains
new entries, so this file picks up where it left off for functions in
this same chunk that got closed out later.

## Parked - NAKED transcription (byte-correct, not decompiled)

- **`sub_802C208`** (`src/graphics/actor_part19e.c`) - a
  `gStaticData_0817A6B8` stride-8 trampoline-record dispatcher (`{s16
  baseOff; s16 count; s16 subOffset}`, count-gated between an inline
  fallback pair and a per-instance list's last entry). Previously
  parked (see docs/matching.md's original entry) on "register-
  allocation/instruction-scheduling around two `record = base +
  state*8` re-derivations". Revisiting this: the ROM keeps
  `gStaticData_0817A6B8`'s base address alive in `r7` for the whole
  function (a plain `adds r7, r1, #0`, not a high-register relay) -
  this is this project's confirmed categorical gcc-2.9 r7-pin bug (see
  docs/matching.md's `sub_8007DBC`/`sub_8006600` entries): an explicit
  `register T x asm("r7")` compiles the right instructions but never
  makes it into the prologue/epilogue `push`/`pop` list, and this
  compiler's own *unforced* allocator never reaches r7 here either
  (confirmed by testing - r7 only enters this compiler's natural
  push/pop set via the separate `mov r7, sb` high-register relay
  idiom, which doesn't apply to this function). Every instruction is a
  direct, byte-verified transcription of the ROM disassembly (same
  technique as `src/system/link_cable.c`'s `sub_8001CB8`), so the
  built ROM is byte-identical here - but per this project's current
  tracking policy, a NAKED transcription of a substantial function
  (branches, struct-shaped field access - not a trivial wrapper/stub)
  does **not** count as "matched": the logic was never actually
  re-expressed as C, just copied. `tools/report_units.py`'s `UNITS`
  entry for this address keeps `base_object` as `None` accordingly.
  The same r7 hazard and NAKED-transcription-but-not-matched status
  apply identically to this dispatcher's other four instances sharing
  this exact stride-8 shape: `sub_802F748` (issue #56,
  `src/graphics/actor_part44b.c`, `gStaticData_0817C1C0`),
  `sub_8033B44`/`sub_8033C84`/`sub_8033E80` (issue #62,
  `src/graphics/actor_part31.c`/`actor_part33.c`/`actor_part37.c`,
  `gStaticData_0817C4E0`/`gStaticData_0817C4F8`), and `sub_8033FE4`
  (issue #63, `src/graphics/actor_part64.c`, `gStaticData_0817C4F8`) -
  see those files' own doc comments and the matching-issue docs for
  issues #56/#62/#63.

Verified byte-identical with a full clean `make compare` (`La suma
coincide`) and `make NON_MATCHING=1 report`, but none of the six are
recorded as matched - see the note above.

# `DrawPowerDialog` and thirteen more: NAKED asm transcription (byte-exact, tracked as parked)

**Tracking note**: a NAKED transcription of a substantial function
(one with real loops/branches/struct access, standing in for a
codegen gap the C reconstruction couldn't close) is byte-exact but
does **not** count as "matched" in this project's tracking - only a
genuine decompiled-C match does. All fourteen functions below stay
filed under "Parked" in `docs/status/graphics.md`/`docs/status/actor.md`
(not "Matched"), and `tools/report_units.py`'s `UNITS` list tracks
each one's own address range with `base_object: None` rather than
folding it into the surrounding matched file's tracked range. This
does not apply to trivial pre-existing wrapper-style NAKED functions
(BIOS SWI call wrappers, bare register trampolines, no-op stubs) -
those remain an accepted, separate convention since there's no real C
logic to express for them in the first place; `CheckSpritePickup`
(`naked-sub_8007dbc.md`), a substantial function matched via this same
technique in an earlier session before this tracking distinction was
drawn, is left as-is rather than retroactively re-filed.

This batch closes - as byte-exact NAKED transcriptions, not real
matches - every function that was previously parked under
`#if NON_MATCHING` across `src/graphics/oam_count.c`, `graphics.c`,
`actor_part.c`, `actor_part3.c`, `actor_part4.c`, `actor_part5.c`,
`actor_part6.c`, and `actor_part7.c`/the new `actor_part7b.c`:
`DrawPowerDialog`, `DrawSpritePieces`, `GetSpriteBounds`, `GetSpriteHitbox`,
`AdvanceSpriteAnim`, `sub_8008188`, `sub_8008200`, `sub_8008278`,
`GetSpriteFrame`, `IsSpriteAnimLooping`, `UpdatePartList`, `CollidePartList`,
`CollidePartWithPlayer`, and `CollidePartWithObject`. Every one of these had already been
fully semantically understood (see their doc comments, now preserved
in git history alongside the plain-C reconstructions they replace, and
`docs/matching.md`'s corresponding "Parked, not matched" entries) -
the only remaining gap was register-letter/codegen-shape mismatches
that resisted every C-level technique tried across one or more prior
sessions: the categorical `r7`-pin hazard (`DrawPowerDialog`,
`GetSpriteBounds`, `UpdatePartList`, `CollidePartList` - see
`matching_decomp_register_pinning` memory point 10 and
`naked-sub_8007dbc.md`'s account of the same wall), a register-register
`add`'s stubborn destination-operand canonicalization that this
compiler never reorders no matter the C source shape (`sub_8008188`,
`sub_8008200`, `sub_8008278`, `GetSpriteFrame`), a redundant
byte-truncation the compiler always optimizes away once it can prove
an `AND`'s range (`IsSpriteAnimLooping`), a genuine stack-frame/local-variable
shape this reconstruction couldn't reverse-engineer (`DrawSpritePieces`,
`AdvanceSpriteAnim`), and (`CollidePartWithPlayer`/`CollidePartWithObject`) a C-level
inexpressibility - leaving one incoming scalar argument untouched in
its own stack slot while still building a struct pointer that includes
it, which C has no syntax for.

Rather than keep chasing these - several of which prior sessions had
already exhausted every cataloged technique against, including
`decomp-permuter` runs - every one of these fourteen functions was
converted to a `NAKED` function whose body is a single `asm()` block
transcribing the real ROM disassembly instruction-for-instruction, the
same technique already used for `CheckSpritePickup`
(`naked-sub_8007dbc.md`) and several functions in
`src/system/link_cable.c`/`src/audio/gax_swi.c`. Since this is a
literal byte-for-byte transcription rather than a derived
reconstruction, it reproduces the ROM's own register choices, operand
order, and padding directly and needs no register-allocation coaxing
at all. Each transcription:

- Uses GNU-as local numeric labels (`1:`/`1f`/`1b`, ...) in the exact
  order the ROM's own branch targets, jump tables, and literal-pool
  entries appear, rather than named labels, since the whole function
  is one `asm()` string (reused across sibling functions in the same
  file - GNU as scopes numeric labels locally, so this is safe).
- Translates the disassembler's unified-syntax mnemonics to the
  divided syntax this project's other `asm()` blocks use (`adds`→`add`,
  `ands`→`and`, `orrs`→`orr`, `lsls`/`lsrs`/`asrs`→`lsl`/`lsr`/`asr`,
  `subs`→`sub`, `movs`→`mov`).
- Reproduces every `.align 2, 0` the ROM's raw assembly had, at the
  exact same points - both the ones sitting between code and a
  mid-function literal pool (needed so the assembler doesn't reject a
  misaligned PC-relative `ldr`) and the ones after a function's last
  real instruction (needed to reproduce the ROM's own padding byte
  value, `0x0000`, since a `NAKED` function has no automatic trailing
  alignment the way a normal compiled function does - the linker's own
  inter-object padding defaults to a NOP fill, `0x46c0`, not zero, so
  omitting a genuinely-needed trailing `.align 2, 0` produces a
  byte-exact-looking function that still fails the final checksum).
  This is the standard `matching_decomp_alignment_fix` technique,
  applied via an explicit assembler directive inside the `asm()`
  string rather than a separate `asm(".align 2, 0");` statement,
  since the latter only works when it's genuinely the last thing in a
  translation unit.

Two functions needed splitting into their own translation unit rather
than being appended to their semantically-nearest file, since their
real ROM address isn't contiguous with that file's other functions
(the established "needs its own new .c file" case from
`docs/workflow.md` step 4):

- `CollidePartWithObject` moved out of `actor_part7.c` into a new
  `src/graphics/actor_part7b.c` - its real ROM address, `0x08008D80`,
  sits after `actor_part10.c`'s `CullPartList`/`ClearPartList`/
  `sub_8008D30`, not right after `CollidePartWithPlayer` the way the old
  `#if NON_MATCHING` C draft's position in the file implied (that
  position never mattered before, since the guarded C never actually
  linked into the matching build - only now that it's unconditionally
  compiled does its link-order position matter). `ldscript.txt` now
  places `actor_part7b.o` between `actor_part10.o` and `actor_part11.o`,
  exactly where the old `asm/code_3_2_12.o` used to sit.

This batch retires six now-empty raw-assembly splits entirely -
`asm/code_3_1_10_11.s` (`DrawPowerDialog`), `asm/code_3_2_2.s`
(`GetSpriteBounds`/`GetSpriteHitbox`), `asm/code_3_2_4.s` (`AdvanceSpriteAnim`),
`asm/code_3_2_5.s` (`sub_8008188`/`sub_8008200`/`sub_8008278`),
`asm/code_3_2_6.s` (`GetSpriteFrame`), `asm/code_3_2_7.s`
(`IsSpriteAnimLooping`), `asm/code_3_2_8.s` (`UpdatePartList`/`CollidePartList`/
`CollidePartWithPlayer`), and `asm/code_3_2_12.s` (`CollidePartWithObject`) - each deleted
and its `ldscript.txt` line dropped, the same "retire an emptied
split" convention as `asm/code_3_1.s`/`ShowBitmapScreen` and
`asm/code_3_2_3.s`/`CheckSpritePickup` before it (see `docs/matching.md` and
`naked-sub_8007dbc.md`). `graphics.c`'s `asm/code_3_2.s` split stays
(other raw functions remain in it after `DrawSpritePieces`'s removal).

Full clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` confirms `La suma coincide`
after this batch, alongside `make NON_MATCHING=1 report` (kept
working, though now a no-op for these fourteen functions specifically
since they're unconditionally compiled either way).

**Update: `IsSpriteAnimLooping` matched in a later session.** Converted back
from this NAKED transcription to real C - an empty
`asm volatile("" : "+r"(test))` barrier right after the `and` that
computes the result made its value opaque to the optimizer, forcing
the automatic `s32`-to-`u8` return-value truncation the ROM has (and
this compiler otherwise proves redundant) to actually materialize.
See `docs/matching.md`'s "Parked, not matched: IsSpriteAnimLooping" entry for
the full account.

**Update: `sub_8008188`/`sub_8008200`/`sub_8008278`/`GetSpriteFrame`
matched in a later session.** Converted back from this NAKED
transcription to real C. `sub_8008188`/`sub_8008200`/`sub_8008278`'s
shared holdout - the kind-8/12 switch block's register-register `add`
canonicalizing the "wrong" way around - was closed by routing both
`case 8` and `case 12` to one `goto`-shared label holding the entire
load+load+add sequence as a single atomic `asm volatile` block, rather
than an asm anchor on just the `add` inside two ordinary switch-case
bodies (which an earlier attempt found broke the compiler's own
case-body identical-code merging). `sub_8008278` additionally needed
every other switch case rewritten the same `goto`-to-an-outside-label
way (not just the kind-8/12 one) to get the compiler's block *layout*
matching, and an explicit `register void *rec asm("r2")` pin on the
incoming parameter to stop the compiler from spilling it to a
callee-saved register once its live range was forced to span every
`goto`-connected block. `GetSpriteFrame`'s single-instruction
`add`-operand-order gap sits in genuinely straight-line code with no
case merging to protect, so a plain inline-asm anchor on just that one
instruction - the same technique that had backfired for the other
three - worked here directly. See `docs/matching.md`'s "Parked, not
matched: sub_8008188" entry (and its `sub_8008200`/`sub_8008278`/
`GetSpriteFrame` siblings just below it) for the original account of what
had been tried and failed before this session. The remaining nine
functions in this batch are unaffected and remain NAKED, tracked as
parked.

## Later pass (issue #9 NAKED retry)

`GetSpriteBounds`/`GetSpriteHitbox`, `AdvanceSpriteAnim`, `CollidePartList`/`CollidePartWithPlayer`
and `CollidePartWithObject` are now real C under old_agbcc. The box builders
return the box by value. `CollidePartList`/`CollidePartWithPlayer`/`CollidePartWithObject` take
it by value, which fixes the "stack-layout coincidence". `UpdatePartList`
is still NAKED, with a C draft 18 halfwords off. See [issue-9-naked-retry.md](./issue-9-naked-retry.md) for details.

## Later pass (strag1)

`DrawSpritePieces` is now real C. It moved out of `graphics.c` into
`src/graphics/graphics_73dc.c` (old_agbcc, on `OLD_AGBCC_OBJS`), written
in the same shape as its matched affine sibling `DrawAffineSpritePieces`. See
[strag1-naked-retry.md](strag1-naked-retry.md).

## Later pass (strag3): DrawPowerDialog matched

`DrawPowerDialog` is now real C in `src/graphics/oam_count.c` (agbcc). It uses an inline position setter, with the centered X computed into a local first. See [strag3-naked-retry.md](strag3-naked-retry.md).

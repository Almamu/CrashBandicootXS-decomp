# Issue #64: 0x08034AA4-0x080354E0 (13 functions, actor)

All 13 functions from this chunk now have a byte-exact answer - either
real C or a NAKED transcription - with the whole raw span
(`asm/code_3_2_20_28568_c99c_31784_33ef4_34aa4.s`) removed, replaced by
`src/graphics/actor_part131.c`.

This picks up straight from
[issue-63-final-raw-actor.md](issue-63-final-raw-actor.md): the first
five functions here are more methods on that same `struct fade_overlay`
"self" object (`sub_803472C`/`sub_803487C`/`sub_8034994`,
actor_part87.c/88.c/89.c), which had already flagged `sub_8034AA4`/
`sub_8034C5C` as its per-frame yield helpers before this pass started.
The remaining eight operate on a completely different, previously
undocumented object - a between-level map/progress screen driven
directly from `game_loop`'s level-load state machine (see
`docs/rom_map.md`'s "A fourth thing in this file" section).

## `struct fade_overlay` gets two of its vague fields clarified

`sub_8034C40`/`sub_8034C5C`/`sub_8034C84`/`sub_8034CB0`/`sub_8034AA4` all
operate on the fade overlay object actor_part87.c already named. Working
through them pins down real meanings for two fields that file's own
comment left vague:

- `unused_1c` is actually a per-item blink/flash toggle counter,
  advanced by `sub_8034C40` and read back as `(counter >> 1) & 2` - a
  0/2 flicker mask consumed by `sub_8034AA4` to hide a Yes/No option's
  label every other frame-pair while it's the current selection.
- `flag_20` - guessed in actor_part87.c as "which of two alternating
  cue sfx last fired" - turns out, in this sibling function set, to hold
  the Yes/No dialog's currently-selected option index (0/1; any other
  value means neither option is highlighted) instead. Same field, a
  related but distinct use by this file's functions - both files keep
  their own field name/comment rather than one relitigating the other's
  (docs/workflow.md step 7's "check whether a struct already exists"
  rule extends the existing struct, it doesn't get to overwrite another
  file's documented understanding of a shared field with a narrower
  one).

### Matched, real C

- **`sub_8034C40`** - the blink/toggle helper described above. This one
  needed a genuinely interesting fix caught only by the full-ROM
  `make compare` (not the isolated compile, which "looked fine" every
  step of the way - exactly the class of trap `docs/workflow.md` warns
  about): a plain `if (mode != self->selection) return 1; ...compute...;
  return result;` (or its `if`/`else` mirror) always compiled with the
  *compute* block as the immediate fallthrough after the `cmp` and the
  one-line `return 1` sunk to a tail block - regardless of which way
  the source was phrased, and even with explicit `goto`s forcing the
  block order in the C source. The ROM's own build has the opposite
  physical layout (`movs r0, #1` immediately after the `cmp`, the
  compute block reached via a forward `beq`). What actually closed the
  gap was two independent fixes together: `register s32 result
  asm("r0")` pinning the return value's home register for the whole
  function (this compiler otherwise picks a scratch register for it
  and only moves it into r0 right before `bx lr`), *and* testing the
  negated condition (`mode != self->selection`) as the `if`'s true-arm
  (`result = 1`) with the compute path in the `else` arm - the mirror
  of the "natural" phrasing, but the one that happens to make this
  compiler's block-layout choice match the ROM's. Neither fix alone was
  enough; both together reproduce the ROM's exact byte sequence. Also
  needed: chaining `sub_803472C(sub_8026EDC(0x24))` directly as a
  nested call in `sub_8034CB0` rather than through an intermediate
  named variable (the ROM never copies the allocator's return value
  into a register before immediately passing it on).
- **`sub_8034C5C`** - one frame's "yield" helper: OAM shadow buffer
  sync, VRAM queue flush, DISPCNT re-apply. Matched immediately, no
  register-pin needed.
- **`sub_8034C84`** - the fade overlay's teardown (frees its three BG
  scratch buffers, then `self` too if `mode & 1`). Matched immediately.
- **`sub_8034CB0`** - the "Are you sure?" confirmation-dialog trigger
  (`docs/rom_map.md`'s already-resolved read of this function): builds
  the fade overlay, runs the Yes/No dialog to completion, tears it
  down if it was actually built, returns which option was selected.
  Matched immediately once `sub_8034C84`/`sub_803472C`/`sub_8034994`
  were in scope.

### Parked, NAKED: `sub_8034AA4`

Draws the Yes/No dialog's three labels (icon-manager mode ids
`0x28`/`0x29`/`0x2a`) via `self->icons->record->slots[6]`'s position,
applying `sub_8034C40`'s blink mask to each option's own OAM-hide byte
via `sub_8028A30` in between, and re-commits the OAM shadow buffer.
`self` (r6), the OAM-shadow-buffer address (sl), the constant `0x87`
(r7), and two `0x130`/`0x98<<1` index constants (r8/sb) all stay
resident across many `bl` sites with no register left over - the same
"many live values across calls, no spare register" shape already NAKED
throughout this codebase (`sub_8034994`, actor_part89.c) - and this
function was already flagged as exactly this class of difficulty in
`docs/matching/issue-63-0x08033ef4-actor.md` before this pass even
started. Transcribed instruction-for-instruction from the ROM's own
disassembly rather than attempted as plain C, given the precedent.

## A new object: the between-level map/progress screen

`sub_8034CEC`/`sub_8034E2C`/`sub_8034EF0`/`sub_80350A4`/`sub_80352AC`/
`sub_803544C`/`sub_803547C`/`sub_80354BC` all operate on a brand-new
0x98-byte heap object (`sub_8026EDC(0x98)`, `struct map_screen` in
actor_part131.c) - a combined minimap-reveal + floating-text-popup
screen shown between levels, already partially characterized by
`docs/rom_map.md`'s "A fourth thing in this file"/"Correction: `sub_8034CB0`
turns out to be a separate screen trigger" sections from earlier
reconnaissance passes. Only the fields this chunk's real-C functions
actually touch are named on the struct (`popupListHead`, `streamBase`/
`streamCursor`, `mapObj`, `drawMode`, `suppressCounter`, `asset0`-
`asset4`, `frameParity`) - the NAKED functions' doc comments describe
the rest of what's understood without committing it to the struct.

### Matched, real C

- **`sub_8034E2C`** - the map screen's per-frame driver: an input-gated
  busy loop toggling `frameParity` every iteration (driving the
  popup-text system, `sub_80350A4`, every other frame) alongside the
  minimap reveal (`sub_8034688`) every frame, until confirm or
  D-pad-down+L is pressed; then a fixed 17-frame wipe/transition effect
  poking the window-blend hardware registers directly; then frees every
  remaining popup-text list node. Two gaps closed:
  - The confirm/D-pad-down+L check (`gUnknown_030007E0.pressed & 9`)
    needed an explicit `register ... asm("r0")`/`asm("r1")` pin pair
    (the mask constant and the struct pointer) plus writing it as a
    `mask &= p->pressed;` compound assignment rather than a plain `if
    (mask & p->pressed)` boolean test - the plain form left the AND's
    result in whichever register this compiler's own scratch-allocator
    picked (reusing the pointer's register), where the ROM keeps
    accumulating into the mask constant's own register instead. The
    same "accumulate into the constant's own register" idiom
    `sub_803472C`/actor_part87.c's `four &= 0x3f;` already established.
  - `sub_803544C` needed a `void *unused` parameter it never reads:
    both of `sub_8034E2C`'s call sites materialize `self` into r0 right
    before calling it, even though `sub_803544C`'s own body never
    touches r0 - the same "caller passes a value the callee's ABI slot
    never consumes" shape already noted for `sub_8028A40`'s second
    parameter (hud_icon_widget4.c) elsewhere in this codebase. Keeping
    the parameter (rather than dropping it and hoping the caller's
    `self` argument silently disappears) is what reproduces those two
    `adds r0, r5, #0` instructions exactly.

  The overall loop *shape* - jump straight to the condition check on
  entry, body above the check, check below the body branching back up
  - matched a plain `while (1) { poll; if (cond) break; body; }` with no
  extra effort; this compiler's own loop-rotation optimization produces
  exactly the ROM's "do-while with an initial guard jump" layout for
  that shape.
- **`sub_803544C`** - map screen end-of-frame commit (BG0 scroll
  registers reset, tile cache flush, OAM shadow buffer flush, VRAM
  upload queue flush). Matched immediately (see the `unused` parameter
  note above).
- **`sub_803547C`** - map screen teardown: kicks the minimap object's
  own teardown (`sub_80346FC(mapObj, 3)`) if one was ever built, frees
  each of the five popup-asset buffers still allocated (a real `do
  {...} while` loop over `self+0x30`/`+0x48`/`+0x60`/`+0x78`/`+0x90`,
  kept as raw pointer arithmetic with a one-line comment rather than
  named struct fields since the loop's *stride* is what's being
  matched, not individual field access), then frees `self` too if
  `mode & 1`. Matched immediately.
- **`sub_80354BC`** - the map screen's top-level entry point: allocates
  and constructs the screen, runs it to completion, tears it down.
  Matched immediately once its three callees were in scope (the same
  "chain the allocator call directly, no intermediate variable" fix
  `sub_8034CB0` needed).

### Parked, NAKED

- **`sub_8034CEC`** - the map screen's constructor: builds the minimap
  sub-object, syncs the OAM shadow buffer, hooks both text-icon
  managers (`gUnknown_030012DC`/`gUnknown_030012E0`) up for this screen
  (firing each one's slot-6 OAM trampoline, and copying
  `gUnknown_030012DC->field_12c` into `gUnknown_030012E0->field_108` -
  a new, previously-unexplained cross-wiring between the two icon
  managers), loads the popup-text glyph assets, resets the shared tile
  cache and VRAM upload cursor, initializes the popup-text opcode-stream
  fields, sets a DISPCNT-adjacent bit, and ducks the audio context.
  `self` (r5), a zero constant (r8), and `&gUnknown_030012E0` (sb) all
  stay resident across a long run of `bl` sites, while r4/r6 each get
  rebound to a *different* global's address multiple times over that
  same span with several unrelated calls in between each rebinding - the
  same shape `sub_803487C` (actor_part88.c, this cluster's own sibling
  constructor) hit, there closed only partially (parked NON_MATCHING
  over a single r7-pin bug). Given the similarity and this function's
  additional mid-function register-rebinding on top of that, transcribed
  as NAKED rather than attempted as plain C.
- **`sub_8034EF0`** - the map screen's per-frame OAM-icon draw
  dispatcher for the minimap object: for `drawMode` 0/1/2 draws a single
  centered label; for any other value DMA3-transfers a procedurally-built
  tile buffer and iterates a per-tile record array, building each dot's
  OAM attribute halfwords in place and applying them via `sub_8006AC8`,
  before advancing to the next linked object and repeating. The inner
  tile loop keeps six independent running values live simultaneously
  across a `bl` inside a nested loop (sl/sb/r8 plus r4-r7) - NAKED.
- **`sub_80350A4`** - the map screen's floating-text popup driver
  (`docs/rom_map.md`'s "A floating-text/glyph popup system" note): walks
  the popup-node list decrementing/expiring timed nodes, parses a
  byte-opcode stream (allocate/link a node, set draw mode, terminate a
  line by measuring both icon managers' text width and drawing it
  centered), then re-derives a suppress counter from the measured width
  and walks the list once more shifting each node into position. `self`
  (r5), the list-tail pointer (r8), the running max-width accumulator
  (sb), and the horizontal pen-position accumulator (sl) all stay
  resident across many `bl` sites spanning several nested loops - NAKED.
- **`sub_80352AC`** - the map screen's popup-text asset loader: iterates
  `gStaticData_0817CF40`'s 5 records into `self->asset0`-`asset4`,
  converting each record's raw width/height into rounded runtime units,
  DMA3-transferring custom glyph tile data and building each glyph
  cell's OAM tile index via a nested nibble/row loop, then loading the
  shared palette tail the same way and pinning the freshly-built asset
  into the shared tile cache. The innermost tile-index loop holds seven
  live values simultaneously (r8/sl/sb/ip plus r0-r6) - a fully packed
  register budget with no spare, NAKED.

## Tally

- **Real C, matched:** 8 (`sub_8034C40`, `sub_8034C5C`, `sub_8034C84`,
  `sub_8034CB0`, `sub_8034E2C`, `sub_803544C`, `sub_803547C`,
  `sub_80354BC`)
- **NAKED (byte-exact, not real C):** 5 (`sub_8034AA4`, `sub_8034CEC`,
  `sub_8034EF0`, `sub_80350A4`, `sub_80352AC`)
- **Parked (`NON_MATCHING`):** 0
- **Left raw:** 0

Every function in this chunk now has a byte-exact answer, but 5 of the
13 are NAKED rather than genuinely real C - per this project's
convention (see `CONTRIBUTING.md`'s "Opening the PR" section), NAKED
transcription doesn't count toward closing the tracked issue. Issue #64
stays open with these 5 functions as the remaining gap for anyone who
wants to take another run at reproducing this compiler's exact
register-allocation choices for them.

Confirmed byte-identical against the real ROM via a full clean `rm -rf
build && make NON_MATCHING=1 report` (no compile warnings) and `rm -rf
build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
make compare` (`La suma coincide`). The raw bytes that used to live in
`asm/code_3_2_20_28568_c99c_31784_33ef4_34aa4.s` are gone - that file
held nothing but these 13 functions, so it was deleted outright, with
its `ldscript.txt` line replaced by the new `actor_part131.o`.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.

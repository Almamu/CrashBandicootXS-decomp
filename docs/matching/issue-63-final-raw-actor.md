# Issue #63's final raw span: 0x0803472C-0x08034AA4 (actor)

This closes out the last three raw functions from issue #63's original
25-function chunk (`sub_803472C`/`sub_803487C`/`sub_8034994`, previously
left raw as "out of scope for this pass" - see
[issue-63-0x08033ef4-actor.md](issue-63-0x08033ef4-actor.md)'s "Left
raw" section). All three are now understood and parked/matched, leaving
issue #63 with zero fully-raw functions - though the issue itself still
has 8 parked (7 NON_MATCHING + 1 NAKED) functions from the earlier pass
plus these two newly-parked ones, so it stays open (see "Tally" below).

## A new, unrelated "self" object: `struct fade_overlay`

Unlike every other function in this chunk (all members of the
`InitActorPart`-rooted per-instance object family), these three
functions operate on a completely different, standalone object - no
`InitActorPart` call, and a field layout that doesn't match that
family's `self+0xc` table-index/`self+0x28` state/`self+0x44` frame
counter conventions at all:

```c
struct fade_overlay {
    u8 *bg1Buf;   // 0x00 - LoadGraphicsPackage "self" scratch for BG1
    u8 *bg0Buf;   // 0x04 - ditto for BG0
    u8 *bg2Buf;   // 0x08 - ditto for BG2
    u16 dispcnt;  // 0x0c - written as one halfword to REG_DISPCNT
    u8 unused_0e[2];
    union { u32 word; struct { u8 bldcntLo, bldcntHi, bldalphaLo, bldalphaHi; } b; } blend; // 0x10
    u8 unused_14[4];
    struct icon_manager *icons; // 0x18
    u32 unused_1c;
    u32 flag_20;  // 0x20 - which of two alternating cue sfx last fired
};
```

It's a full-screen alpha-blend overlay controller: three small BG
scratch buffers feeding `LoadGraphicsPackage`, a combined
BLDCNT/BLDALPHA mirror re-applied every frame to drive a flicker/pulse
effect, and a hookup to the shared text icon manager
(`gUnknown_030012DC`) and tile cache (`gUnknown_030012B8`).

## `sub_803472C` (`src/graphics/actor_part87.c`) - parked, NON_MATCHING

The constructor half: allocates and initializes the three BG scratch
buffers (`sub_8026EDC`/`sub_801E644`), loads their graphics packages,
clears palette entry 0, builds the DISPCNT value (mode 0, 1D OBJ
mapping, BG0/BG1/BG2 enabled), calls `sub_803487C` for the other setup
half, then builds the BLDCNT/BLDALPHA alpha-blend value (BG2 -> BG0,
mode 1, EVA=8/16 EVB=16/16), applies every register, zeroes two more
fields, and ducks the audio context out via `sub_8001AC4`.

This one came *extremely* close to a real match - every field, struct
offset, and the overwhelming majority of individual register choices
were reproduced exactly through heavy register pinning
(`self`->r5, a `buf` pin->r0 relying on `sub_801E644` not clobbering
r0 across the call, `zero`->r8, `c0x40`->sb, `one`->r6, `four`->r4) plus
several inline-asm anchors for spots where this compiler's own optimizer
takes a shortcut the ROM's build didn't (reusing a still-valid
low-register copy of a pinned high-register constant instead of
re-deriving it fresh, and CSE-folding a store-then-reload of a
known-zero value across a width-mismatched halfword/byte access
boundary). The `& -8`/`& -0x20` negative-constant bit-clear idiom, the
"materialize a constant right before first use, not up front" ordering,
and the "keep a constant's own register as the running accumulator
instead of the freshly-loaded byte" idiom (needed differently at
*different* points within the same function - one spot accumulates into
the freshly-loaded value, a different structurally-identical-looking
spot accumulates into the pre-existing constant) all had to be
reproduced by hand.

Despite all of that, a final handful of individual accumulator/temp-
register choices in the tail of the BLDCNT/BLDALPHA byte-packing
sequence never converged, confirmed only by the real linked ROM diff
(not the isolated compile, which "looked right" at each step - the
exact class of trap `docs/workflow.md` warns about). Parked
(`NON_MATCHING`) rather than pushed further given how much of the
function is already real, byte-exact-verified C-shaped logic; the raw
bytes live in
`asm/code_3_2_20_28568_c99c_31784_33ef4_3472c.s` under a
`.if NON_MATCHING == 0` guard (LoadGraphicsPackage's established
pattern, `docs/matching/issue-30-graphics-loading.md`).

## `sub_803487C` (`src/graphics/actor_part88.c`) - parked, NON_MATCHING

The other setup half, called from `sub_803472C`: flushes/double-flushes
the shared VRAM upload cursor (`gUnknown_030012FC`, `struct
vram_upload_cursor`), hooks `self->icons` up to the global text icon
manager (`gUnknown_030012DC`, `struct icon_manager` - already fully
described in `include/icon_manager.h`), fires its 7th (index 6) OAM
trampoline slot via `sub_803AD7C` (the same `icon_slot` shape
`sub_8011A1C`/`actor_part39.c` already established), clears
`icons->field_118` and re-derives the cursor's limit from
`icons->field_12c << 5` (`sub_8006C58`), resets the shared tile cache
(`gUnknown_030012B8`, `struct tile_asset_cache`) and pins its first four
slots (`sub_8006D50`), hand-seeds those same four slots with four fixed
32-byte tile patterns straight from ROM data
(`gStaticData_0817C512`/`532`/`552`/`572`) via a 16-iteration loop with
six independent running pointers, flushes the cache, sets the fade
overlay's `dispcnt`'s OBJ-enable bit, and finally flushes/double-syncs
the OAM shadow buffer (`gUnknown_03001300`).

Two structural findings worth recording since they recur throughout
this codebase but were freshly re-confirmed here:

- `gUnknown_030012FC`/`gUnknown_030012B8` are re-read fresh from their
  global pointer at every single use, never cached in a local across a
  call - the ROM's own build only ever caches the *address of the
  global* in a register (one `ldr rX, =gUnknown_...`), re-dereferencing
  through it after every `bl`. A plain `struct vram_upload_cursor
  *cursor = gUnknown_030012FC;` local compiles noticeably shorter/wrong
  here.
- `self->icons` is likewise re-read from `self` (not kept in a
  register, and not re-fetched from `gUnknown_030012DC` again) after the
  `sub_803AD7C` call, even though the exact same pointer was already
  live in a register right before that call.

Matched real C for everything except the tile-cache seeding loop's trip
counter, which the ROM keeps live in r7 for the whole loop - this
project's **confirmed categorical gcc-2.9 r7-pin bug** (see
`graphics_package_1e688.c`/`oam_count.c`/`actor_part7.c` and the several
`docs/matching/naked-*.md` entries): an explicit
`register s32 counter asm("r7")` pin compiles the exact right
instructions but this compiler's own push/pop-list computation never
includes r7 for it, regardless of how the source is phrased. Tried and
failed here: a barrier extending the pin's live range across the
`sub_8006DC8` call immediately after the loop, hoisting the declaration
to function scope (matching how `self`'s own r8 pin *does* get
protected), and narrowing every other pinned local's scope so r7 isn't
"crowded". All six of the loop's *other* running-pointer registers
(`destA`/`destB`->r1/r2, `srcA`-`srcD`->r5/r6/r3/r4) needed explicit
pins too, matching this compiler's unforced allocator reaching a
different (merely equivalent) permutation of the same seven registers
otherwise. Parked (`NON_MATCHING`); raw bytes live in
`asm/code_3_2_20_28568_c99c_31784_33ef4_3487c.s` under the same
`.if NON_MATCHING == 0` guard pattern.

## `sub_8034994` (`src/graphics/actor_part89.c`) - NAKED, parked

The fade overlay's per-frame driver (caller not yet identified in this
pass - a per-frame "run this overlay" hook somewhere in `game_loop`,
out of scope here). Busy-loops, yielding via `sub_8034AA4`/
`sub_8034C5C` each iteration (both still-raw functions just past this
file's own raw-asm boundary, `asm/..._34aa4.s`), polling input twice per
outer iteration: a confirm press (bit 0) or D-pad-down-with-L (bit 3) of
`gUnknown_030007E0.pressed` immediately exits with a "confirm" SFX cue
(`PlaySfx(gUnknown_030012BC, 0x49, 0x100)`); otherwise L alone (bit 6,
gated on `self+0x20`'s one-shot flag already being set) or R alone (bit
7, gated on it being clear) plays a "step" cue
(`PlaySfx(..., 0x46, 0x100)`) and flips that flag - the same
`gUnknown_030007E0`/`PlaySfx` input-dispatch shape already established
in `src/audio/counter_selector.c`'s `sub_8037224`. Every two inner
iterations, a 0-15 ping-pong counter (the low 5 bits of `self+0x12`,
which is the same byte as `self->blend.b.bldalphaLo`'s partner within
the `dispcnt`/`blend` layout above) gets folded back into `self+0x10`'s
combined BLDCNT/BLDALPHA word and re-applied to `REG_BLDCNT`, driving
the overlay's flicker/pulse animation frame by frame. Returns 1 if
`self+0x20`'s flag is still clear when the loop exits via the confirm
branch, else 0.

Six live values across four different `bl` sites with no register left
over (`sb`, `sl`, `r8`, and three of `r4`-`r7`, holding the "already
confirmed" one-shot flag, the packed pressed/held bitmask pointer, the
audio context pointer, `self`, a scratch "player pressed either
qualifying button" flag, and the 0-15 ping-pong counter, respectively)
is the same "many high registers held live across calls inside a loop"
shape already NAKED throughout this codebase for this exact reason
(`sub_80309B4`/`sub_8031040`/`sub_80311C4`,
`actor_part21f.c`/`23e.c`/`23f.c`) - and was already flagged as this
class of difficulty for this specific function in the original issue-63
writeup before this pass even started. Transcribed instruction-for-
instruction from the ROM's own disassembly rather than attempted as
plain C.

## Tally

Issue #63's original 25-function chunk, plus these 3:

- **Real C, matched:** 14 (unchanged from before this pass)
- **Parked, NON_MATCHING (real C, not byte-exact):** 9 (7 from before +
  `sub_803472C` + `sub_803487C`)
- **NAKED (byte-exact, not real C):** 2 (`sub_8033FE4` from before +
  `sub_8034994`)
- **Left raw:** 0

Issue #63 stays open - only 14/28 functions in its now-expanded scope
are genuinely real-C-matched, and NAKED/NON_MATCHING parking doesn't
count toward closing per this project's convention (see
`CONTRIBUTING.md`'s "Opening the PR" section).

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.

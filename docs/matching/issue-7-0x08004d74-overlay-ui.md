# Issue #7: 0x08004D74-0x08005A78/0x08005E5C - the composite pause/options
screen's own constructor and icon-manager-heavy sub-widgets

**Naming note:** these files are numbered `settings_menu15`-`19` rather
than `settings_menu13`-`17` (which would have matched their creation
order more naturally) because issue #8's parallel PR independently
claimed `settings_menu13.c`/`14.c` first, before this PR merged -
resolved as a rename on merge to avoid an add/add filename collision.
The whole five-file family was renumbered together (not just the two
that literally collided) to keep it visually contiguous.

GitHub issue #7 covers 12 functions: the composite pause/options
screen's top-level constructor/driver pair, its per-row list renderer
and draw step, its five icon-row-group draw handlers, and the results
sub-region constructor. All 12 live in what was `asm/code_3_1_10_7.s`
(0x08004D74-0x08005A78, contiguous up to the already-matched
`sub_8005A78` in `src/graphics/settings_menu6.c`) plus the single
isolated `sub_8005E5C` (0x08005E5C-0x08005EF4, between
`settings_menu6.o` and `settings_menu7.o`).

5 of the 12 are now matched; 7 stay parked under `NON_MATCHING`. Issue
#7 stays open - not every function closed.

## New shared header: `include/pause_screen_results.h`

Before this pass, the composite screen's 0xd4-byte top-level object had
three independent, non-overlapping partial views: `struct
pause_screen_results` (settings_menu6.c), `struct
pause_screen_row_counts` (settings_menu7.c), and `struct
pause_screen_apply_state` (settings_menu12.c). Tracing the real call
chain this pass (`sub_8004D74` allocates the object and hands it to
`sub_8004EC0`, which hands the *same pointer* to `sub_800599C`, which
hands it to `sub_8005A78`/`AE8`/`B80`/`C58`/`D44`; `sub_8004EC0`
separately hands it to `sub_8006250`/`sub_8005100`) confirms these
three views are genuinely the same allocation - merging them into one
struct in `include/pause_screen_results.h`, with all three previously-
separate field sets agreeing at zero-overlap boundaries once combined
(strong independent confirmation this is one real object, not a
coincidence). `settings_menu6.c`, `settings_menu7.c`, and
`settings_menu12.c` now `#include` the shared header instead of
defining their own local copies; this is a pure type-rename (no field
offsets changed) verified not to affect their own already-matched
bytes by the same full `make compare` this issue's own work required.
Also moved: `struct settings_icon_actor`, `struct icon_pos`, and the
`UPDATE_ICON_FRAME_NIBBLE` macro (previously local to
`settings_menu6.c`), since the new file needs them too.

Distinct from (and still not reconciled with) `struct
pause_options_screen` (`include/pause_options_screen.h`) - a smaller,
separately-allocated settings-sync/spinner object that happens to share
some byte offsets by coincidence, per that header's own comment.

## Matched (5 of 12)

- **`sub_8004EC0`** - the composite screen's per-instance constructor:
  `sub_801E644` init, a local BLDCNT/BLDY/DISPCNT setup
  (`field_c8`/`field_cc`/`field_d0`, the same fields `sub_8006250`
  applies), `LoadGraphicsPackage`, a row-stats handle from
  `gUnknown_030012C0`, hands off to `sub_800599C` to build the results
  sub-widgets, builds one more icon directly (the row-cursor/highlight
  icon at `field_c0`), seeds the settings-row bookkeeping fields
  (`field_14`/`field_18`/`field_1c`/`field_20`/`field_24`/`field_28`),
  and applies BG0CNT/BG0HOFS before returning `self` unchanged.
- **`sub_8005004`** - re-probes every icon field/array the results
  screen owns (`field_c0`, `field_bc`, `iconsB0[3]`, `icons9c[5]`,
  `icons8c[4]`, `field_88`, in that order) via the same "re-probe an
  actor's own category-table slot 0x50/0x54" shape `sub_8006770`
  (`src/graphics/oam_count.c`) already established, then frees `self`
  if bit 0 of `flags` is set.
- **`sub_8005304`** - the icon-group reveal/cycle animation
  (`field_24`/`field_28` cycle through hiding one icon group per call,
  advancing every 180 calls) plus the row-cursor icon's independent
  blink countdown (`field_c4`/`field_c0`/`field_38`).
- **`sub_800570C`** - shows whichever of `icons8c[0..3]` has a matching
  bit set in the row-stats handle's flag byte (bits
  0x20/0x80/0x40/0x10), or draws a fallback centered label (text id
  0x3a) if none did.
- **`sub_800599C`** - the results sub-region constructor: resolves the
  current level's name/index label, formats the completion percentage,
  formats two BG-scroll-speed settings via a `(v+0xc)*20/256` scale,
  then builds the five icon-widget sub-groups in order (delegating to
  `sub_8005A78`/`AE8`/`B80`/`C58`/`D44`).

New files: `src/graphics/settings_menu15.c` (`sub_8004EC0`,
`sub_8005004`, plus the parked functions below), `settings_menu17.c`
(`sub_8005304`), `settings_menu18.c` (`sub_800570C`), `settings_menu19.c`
(`sub_800599C`).

### Real codegen gotchas hit and fixed (all five functions)

This chunk needed far more register/scheduling massaging than most -
worth cataloging since every technique here reproduced a genuine ROM
instruction-for-instruction, not a guess:

- **Missing `u16` truncation drops 2 instructions.** `sub_8000E1C(...)
  + 0x78` (a random-range countdown) compiles shorter than the ROM
  unless the return value is explicitly truncated first: `(u16)
  sub_8000E1C(0x78) + 0x78` forces the ROM's `lsls r0,r0,#0x10 / lsrs
  r0,r0,#0x10` pair back in (hit identically in both `sub_8004EC0` and
  `sub_8005304`).
- **Mid-function literal pool split via `asm volatile(".pool")`** (the
  same technique documented in `docs/matching/issue-5-overlay-ui-sync.md`
  for `sub_8003A60`): the ROM splits `sub_8004EC0`'s literal pool into
  an early 5-word group (right after an unconditional branch mid-
  function) and a late 2-word group at the tail; this compiler defaults
  to dumping everything at the tail unless told otherwise. Needed a
  real `if`/`else` (not a ternary) with the store deferred to a shared
  merge point, and the `.pool` directive placed inside the `if` branch
  right before its own `b`.
- **Statement order inside one basic block is significant** even when
  it doesn't change instruction *count* - several spots needed
  reordering to match which value gets computed first (e.g. `v = 0xc0;
  v |= *addr;` instead of `v = 0xc0 | *addr;`, and moving a `zero = 0;`
  assignment to after an address computation it doesn't depend on) to
  get the ROM's exact register/constant scheduling instead of an
  equally-valid but differently-ordered one.
- **A register-pinned local without an initializer can silently break
  the automatic callee-save push/pop list.** `sub_8005004` needs four
  pointers (`icons9c`/`icons8c`/`field_88`/`iconsB0` bases) set up in a
  specific order across `r7`/`r8`/`r9`(`sb`)/`r4` before four back-to-
  back loops. Pinning all four the same way self/flags were pinned
  (`register T x asm("rN") = initializer;`) either dropped `r7` from
  the prologue's push list entirely (a real calling-convention bug -
  the caller's `r7` never gets restored) or, with a dummy `= NULL`
  initializer, fixed the push list but flipped which of `r8`/`r9` won.
  The working combination: pin only the two *high* registers
  (`r8`/`r9`) with a `= NULL` initializer, and leave the two *low*
  registers (`r4`/`r7`) as plain unpinned locals - this compiler's own
  natural allocator picks `r4`/`r7` correctly and includes them in the
  standard `push {r4,r5,r6,r7,lr}` prologue on its own.
- **`goto`-based control flow to force branch polarity**
  (`sub_8005304`'s `field_c4` blink-countdown check): a plain `if
  (*countAddr != 0) { decrement } else { <big icon block> }` compiles
  to the *opposite* branch sense from the ROM (a short `beq` skipping
  the decrement, vs. the ROM's `bne` skipping the icon block) - same
  final behavior, different bytes. Rewritten with explicit
  `goto decrement; ...icon block...; goto store; decrement: ...; store:
  ...;` to pin the ROM's exact branch target layout.
- **Trailing byte-padding** (`matching_decomp_alignment_fix`): both
  `sub_8005004` and `sub_8005304` needed a trailing `asm(".align 2,
  0");` right after their closing brace - the ROM pads the gap to the
  next function with zero bytes, this compiler's default inter-
  function padding is a `mov r8, r8` NOP-equivalent.
- **A dereference's destination register vs. the address holding
  it**: several `X & *(u8 *)addr`-shaped bit tests (e.g. `sub_800570C`'s
  four `field_10`-byte checks) need the mask constant and the
  dereferenced byte pinned to specific opposite registers
  (`register s32 mask asm("r0"); register u8 *p asm("r1"); ... byte =
  *p; mask &= byte;`) to get the ROM's exact `ldr r1,[..]; movs r0,#K;
  ldrb r1,[r1,#N]; ands r0,r1` shape instead of the compiler's own
  (also valid) register choice.

## Parked (`NON_MATCHING`, not yet byte-exact) - 7 of 12

All seven are semantically understood and cross-checked against the
matched functions above and `docs/rom_map.md`'s `overlay_ui`
investigation; none were guessed. Real bytes stay in the asm fragments
below (each wrapped `.if NON_MATCHING == 0`); the NON_MATCHING C
reconstructions live alongside the matched functions in
`src/graphics/settings_menu15.c` (`sub_8004D74`, `sub_8005100`,
`sub_80053F4`, `sub_800556C`, `sub_80057E0`, `sub_80058C0`) and
`src/graphics/settings_menu16.c` (`sub_8005E5C`).

- **`sub_8004D74`** (`asm/code_3_1_10_7.s`) - the composite screen's
  top-level orchestrator: frees pending heap bytes, resets the audio
  channel, swaps `gUnknown_030012B8` for a fresh tile cache sized for
  this screen, re-inits both icon managers, builds the screen object
  (`sub_8004EC0`) and hands it to the blocking driver (`sub_8005100`),
  tears it down, and restores the original cache. By far the longest
  function in this chunk (~150 instructions). Got the instruction
  *order* and *count* extremely close via heavy register pinning
  (matching the ROM's own long-lived choices for the heap flag, the
  cache-pointer cursor, and the two icon-manager addresses), but the
  ROM additionally keeps a literal `0` live in `r8` across ~100
  intervening instructions to avoid one reload, and shares a couple of
  shifted-constant computations between otherwise-separate statements
  - scheduling decisions this compiler doesn't reach for from plain C
  at this call depth.
- **`sub_8005100`** (`asm/code_3_1_10_7_5100.s`) - the blocking cursor/
  confirm/cancel driver: ramps a blend/fade level down then up
  (`field_cc`'s low 5 bits), an input loop with L/R slider-adjust,
  D-pad value bump with initial-press-vs-held-repeat distinction, and
  A/B confirm/cancel with a row-type-tag gate. Fully understood but by
  far the largest and most control-flow-heavy function in the whole
  chunk - parked without attempting the same register-pressure fight
  documented at length for the others; the effort-to-payoff ratio for
  hand-tuning a function this size wasn't worth it this pass.
- **`sub_80053F4`** (`asm/code_3_1_10_7_53f4.s`) - the per-frame "draw
  the current settings row" step: draws the current level's name label,
  conditionally draws a "LEVEL N"-shaped composite label, right-aligns
  the completion percentage, calls the per-row list renderer and an
  unread sibling, dispatches on `field_24` to one of five per-icon-
  group draw handlers, and conditionally hides the row-cursor icon.
  Same register-pressure class of difficulty as the others - several
  sequential `sub_803AD80` draws with hand-scheduled constant/offset
  register reuse (including an `ip`-register spill in one branch) this
  compiler doesn't reach for from plain C.
- **`sub_800556C`** (`asm/code_3_1_10_7_53f4.s`, same fragment as
  `sub_80053F4`) - the per-row list renderer: draws each row (from
  `field_14`'s 8-byte-stride record array), highlighting the selected
  index, in three layout variants keyed by the record's type tag (a
  plain centered label, or - for tags 4/5 - offset left by half of a
  second "<NN%>"-shaped string's width, with that string drawn
  immediately after at the same position). Fully understood; the ROM
  additionally keeps the running row-Y coordinate in a stack slot
  (not a register) across the whole loop, a scheduling choice this
  compiler doesn't reach for from a plain loop-local.
- **`sub_80057E0`**, **`sub_80058C0`** (`asm/code_3_1_10_7_57e0.s`) -
  the `icons9c`/`iconsB0` per-row-group draw handlers: per-bit icon
  show/hide (only `sub_80057E0`) plus one or three fixed "N/M"-shaped
  fraction readouts via `sub_8005E5C`. Same register-pressure class as
  above - the ROM keeps `self` in a specific register and evolves a
  single other register through three different offset meanings via
  incremental arithmetic on its own prior value; every restructuring
  tried (direct field stores, hoisted x/y locals, an explicit register
  pin on `self`) always needs one register more than the ROM does.
- **`sub_8005E5C`** (`asm/code_3_1_10_9.s`, its own file - not
  contiguous with the rest of this chunk) - draws a numerator/`/`/
  denominator fraction stack across `gUnknown_030012DC`/
  `gUnknown_030012E0`. Same register-pressure class: the two icon-
  manager addresses and the second label argument (all three genuinely
  live across three separate draw calls) always land in `r8`/`r9`/`sl`
  here regardless of how the source is restructured, never the ROM's
  own `r5`/`r6`/`r8` choice.

See `docs/status/overlay_ui.md` for the updated matched/parked lists.

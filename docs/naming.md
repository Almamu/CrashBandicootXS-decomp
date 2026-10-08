# Function naming

## Default: PascalCase

Most functions get a PascalCase name once they're understood well enough to
name, matching the wider pret/GBA-decomp convention this project otherwise
follows: `AgbMain`, `LoadTaggedAsset`, `QueueVramDmaTransfer`,
`FreeVramDmaQueue`, `FlushVramDmaQueue`, `GetAnimFrameBaseOffset`,
`PlaySfx`, `InitTitleScreen`, `LoadTitleScreenBg`, `LoadTitleScreenObjTiles`,
`IrqDisable`, `IrqSetup`, `IrqEmptyHandler`.

`LZ77UnCompVram`/`RLUnCompVram`/`VBlankIntrWait` look like they break
this (`UnComp`, not `Uncomp`) but don't - the BIOS `swi` wrappers in
`lib/agb_eeprom/src/eeprom_timer.c` are named exactly after the BIOS calls they make
(the libagbsyscall names other GBA decomps use). Library code we can
identify keeps its library's names the same way: the AgbEeprom SDK's
`EEPROMRead`/`EEPROMWrite1_check` (TMC's and pokeemerald's spelling) and
libgcc's `__divsi3`/`_call_via_r0`. When a function is a thin wrapper
around a real, documented BIOS/hardware call, or is a known library
function, match *that* name's casing over generic PascalCase rules.

## Exception: snake_case for the memory allocator

`mem_alloc`/`mem_free`/`mem_heap_init`/`mem_collect`/`mem_free_bytes` (and
their `static` helpers) in `src/system/memory.c` intentionally use
snake_case, matching the `malloc`/`free`-style C standard library
convention they mirror. This is a **deliberate exception for this specific
subsystem**, not a general "system-level code can be snake_case" rule -
compare `IrqDisable`/`IrqSetup` right next door in `irq.c`, which don't get
this exception since there's no equivalent libc naming convention for
interrupt handling to match.

If a to-be-matched function turns out to be a genuine C standard library
equivalent - `strcpy`, `strlen`, `strcat`, `memcpy`, `memset`, and so on -
give it that exact lowercase libc name rather than a PascalCase invention,
the same way the memory allocator does. This is the *only* other case
snake_case is appropriate; a function merely reminiscent of a libc one
(e.g. a case-insensitive `strstr` with a different signature) doesn't
automatically qualify - see "When to actually rename" below.

## Exception: GAX2's own API names

The Shin'en GAX2 sound engine's public entry points keep Shin'en's own
`GAX2_xxx`/`GAX_xxx` names (`GAX2_new`, `GAX2_init`, `GAX2_jingle`,
`GAX_irq`, `GAX_play`, `GAX_fx_ex`, `GAX_stop`, ...) instead of PascalCase.
The ROM itself names several of them - its error reports pass the
function name ("GAX2_NEW", "GAX2_INIT", "GAX2_JINGLE", "GAX_IRQ", "GAX_PLAY
HAS NOT FINISHED BEFORE GAX_IRQ") to the fatal-error screen's "FUNCTION
NAME:" line - and the rest follow the engine's published API. Like the
memory allocator, this is a **deliberate exception for this one
library**: the engine's internal helpers (`GaxCreateHandlers`,
`GaxChannelMix`, ...) and its data (`gGaxPeriodTable`, ...) use the
normal conventions. See [`docs/audio.md`](./audio.md#engine-api-names).

## Reserved fallback names - never invent new ones that look like these

- **`sub_XXXXXXXX`** (`X` = the function's ROM address, uppercase hex, no
  `0x` prefix, however many digits the address needs - `sub_80006A8`, not
  `sub_080006A8` or `sub_80006a8`) - the default for every function until
  its behavior is understood confidently enough to name. This is the
  *starting point* for everything, not a failure state - most of the ROM
  is still, and will remain for a long time, `sub_XXXXXXXX`.
- **`nullsub_N`** - reserved for empty/do-nothing stub functions, an
  established pret/GBA-decomp convention (`N` assigned sequentially as
  they're found; don't renumber existing ones to "fix" gaps).

## Functions with no caller anywhere in the ROM

A function can turn out to be genuinely dead code - nothing in the whole
ROM (`bl`/`.4byte` reference) ever calls it, confirmed by grepping every
`asm/*.s`/`expected/*.s`/`src/**/*.c` for its address/symbol before
concluding this, not just the files nearby. This does **not** mean skip
matching it - it still goes through the full workflow.md loop like any
other function, since it's still real code that shipped in the ROM and its
behavior is worth understanding. What changes is the documentation:

- Say so plainly in the function's doc comment - `/* UNUSED - no caller
  anywhere in the ROM (checked <what was grepped>). ... */` - not a vague
  question or a TODO. If there's a plausible reason it's dead (leftover
  debug/dev code, an earlier design that got replaced, a duplicate of
  another function that superseded it), say that too, but don't invent a
  confident reason that isn't actually supported by what's visible in the
  ROM.
- Still record it as matched (or parked) in `docs/matching.md`/
  `docs/status/<system>.md` the normal way - being unused isn't a reason to
  leave it out of the log.
- Don't confuse this with a function that merely computes something and
  discards the result (e.g. calling a function purely for a side effect,
  ignoring its return value) - that function still *has* a caller and is
  reachable; only tag `UNUSED` when nothing calls the function itself.

## When to actually rename

Only once the function's behavior is understood confidently enough that the
name won't need revising later - a vague behavioral paragraph isn't enough.
"A HUD-icon-plus-number renderer" or "a word-wrap text renderer" don't
condense into one confident name yet; "the game's entry point" (`AgbMain`)
or "queues a VRAM DMA transfer" (`QueueVramDmaTransfer`) do. When in doubt,
leave it `sub_XXXXXXXX` - a wrong or overly-specific name is worse than an
honest placeholder, since other code and docs will start depending on it.

## What renaming touches (checklist)

1. The function's own definition and prototype.
2. Its declaration in the owning header (see
   [`docs/headers_plan.md`](./headers_plan.md)), any codegen alias's
   `asm("<name>")` label, and every reference: `src/`, `lib/`, `asm/*.s`
   (`bl <name>`/`.4byte <name>`), `ldscript.txt` and `sym_*.txt`.
3. `docs/matching.md`'s per-function entry.
4. `docs/status/<system>.md`'s matched-function list.
5. If the function is covered by `expected/code_3.s` or `expected/legacy.s`
   (i.e. it was matched from one of those historical eras), add or update
   its `rename` line in `expected/corrections.txt` to the new name - see
   [`docs/decomp_dev.md`](./decomp_dev.md). A function renamed *after*
   already having a real name (not straight from `sub_XXXXXXXX`) needs its
   correction's target name updated too, not a second entry.
6. Rebuild: `make compare` must still pass (a rename shouldn't change any
   bytes, but a missed call site breaks the link) and
   `make NON_MATCHING=1 report` should still produce a sane result (see
   `docs/decomp_dev.md` - this is how a missed corrections.txt update
   would surface, as the function silently vanishing from the report).

# Constants

Named values (sound IDs, flag bits, states, kinds, ...) are `#define`s in
`include/constants/<topic>.h`, one header per value set (#655). The
subsystem header that owns the type or the function includes it
(`audio.h` includes `constants/sfx.h` and `constants/songs.h`,
`level_state.h` includes `constants/level_flags.h`), so a `.c` file gets
the names with the header it already includes. A data table in
`src/data/` may include a constants header directly.

| Header | Prefix | What |
|---|---|---|
| `sfx.h` (generated) | `SFX_` | sound-effect IDs (`PlaySfx`, `PlayAmbientSfx`, `StopSfx`), from `sound/sfx_table.json` |
| `songs.h` (generated) | `SONG_` | song IDs, the `gSongTable` index (`PlaySong`, `StartSong`), from `sound/gax_manifest.json` |
| `level_flags.h` | `LEVEL_FLAG_` | the bits of a `game_progress.levels[]` word |
| `mask_level.h` | `MASK_LEVEL_` | `level_state.maskLevel` values |
| `packed_stats.h` | `PACKED_STATS_` | the packed lives/mask/wumpa halfword |
| `action_states.h` | `ACTION_STATE_` | the player's action-controller states, the `gActionCtrlStateTable` index |
| `attack_kinds.h` | `ATTACK_KIND_` | how the player hits a crate, the `gCrateHitResponse` column |
| `entities.h` (generated) | `ENTITY_`, `ENEMY_KIND_` | entity types (the `gEntitySpawnFuncs` index), enemy-controller kinds |
| `crates.h` (generated) | `CRATE_KIND_` | `crate.kind` (`CreateCrate`, the `gCrateKind*` tables) |
| `events.h` | `EVENT_` | event IDs (the event method's `case` labels, `NOTIFY`, the `handleEvent` slot calls); also the kinds that touched objects send |
| `levels.h` (generated) | `LEVEL_` | level ids, the `gLevelTable` index, from `data/levels/levels.json` |
| `rooms.h` | `ROOM_KIND_` | `struct level_room.kind` (on foot, underwater, hover, category stage) |
| `bosses.h` | `BOSS_` | `GetBossIndex` results |
| `categories.h` | `CATEGORY_` | actor categories (`gActorCategories` index), `CATEGORY_TYPE_*` and the `CATEGORY_EXIT_*` statuses |
| `chunk_tokens.h` | `CHUNK_TOKEN_` | the run kinds of a layer/collision chunk's token stream (`DecodeCollisionChunk`, `DecodeLayerChunk`) |

**Constants that describe data the repository has as source files are
generated from those files**, not written by hand: the names live in the
data, and the build writes the header into `build/include/constants/`,
which is on the include path (after `include/`), so code includes it as
`"constants/<topic>.h"` all the same. Every C object waits for the
generated headers (an order-only prerequisite), and `-MMD` rebuilds the
objects that include one when it changes. The entity types come from
`data/levels/entity_types.json`, the crate kinds from
`data/levels/crate_kinds.json` and the level ids from
`data/levels/levels.json`'s `"levels"` list (`tools/levels.py constants`,
see docs/levels.md). The sound-effect IDs come from the names in
`sound/sfx_table.json` (`tools/sfx_table.py --constants`) and the song IDs
from `sound/gax_manifest.json`'s `song_table` (`tools/gax_audio.py
--constants`, see docs/audio.md). To rename one, edit the JSON.


`tools/magic_numbers.py` lists the literals that are left, by topic
(`--report` for the counts), and `--topic T --fix` replaces the ones that
have exactly one name.

- **`#define`, not `enum`.** The project has no enums. A define can't
  change the width of a field or a parameter the way an int-sized enum
  type could, it works in `#if`, in `.s` files and in the tools, and it
  doesn't trigger `-Wswitch`. This is also pret's convention.
- **Same literal.** A define expands to exactly the value it replaces,
  with the same type: `#define SFX_CRATE_BREAK 0x3`, not `0x3u` or
  `((u8)3)`. Anything else (a cast, an unsigned suffix, `~MASK` for a
  spelled-out `0xFFFFFE7F`) needs a check that every object stays
  identical.
- **Flag bits are masks:** `#define LEVEL_FLAG_CRATE_GEM (1 << 1)`, used
  as `flags & LEVEL_FLAG_CRATE_GEM` and `flags |= LEVEL_FLAG_CRATE_GEM`.
  A multi-bit field gets a `_MASK` and a `_SHIFT`.
- **Names come from evidence:** the ROM's strings, the GAX manifests,
  the code that uses the value, existing comments. A comment on each
  define says where it comes from. A value whose meaning isn't clear
  keeps its number (as the naming rounds did); `<PREFIX>_<n>`-style
  names are not used, with one exception below.
- **`SFX_UNKNOWN_<ID>` placeholders.** Every sound-effect ID the code
  plays has a name. One whose sound can't be identified from where it
  plays is `SFX_UNKNOWN_<ID>`, the ID in two upper-case hex digits
  (`SFX_UNKNOWN_2E` for 0x2E), and its `comment` in
  `sound/sfx_table.json` says where it plays. The name doesn't depend on
  any other entry, so it stays the same as other sounds get named. When
  you identify one (by ear in an emulator, say), rename it in the JSON
  and update its call sites; nothing else uses the placeholder.

# Helper macros

Operations that the code spells out at many sites have shared `#define`
helpers (#667). Use them in new and converted code instead of writing
the expression out or adding another file-local copy:

| Header | Helpers | What |
|---|---|---|
| `core.h` | `ARRAY_COUNT(a)` | element count of an array |
| `math_util.h` | `Q8_TO_INT`, `INT_TO_Q8`, `Q12_*`, `Q16_*` | fixed-point conversions (`x >> 8`, `x << 8`, ...) |
| `math_util.h` | `Q8_TO_INT_INPLACE(x)` | the in-place conversion `x >>= 8` |
| `math_util.h` | `Q8_MUL(a, b)`, `Q8_DIV(a, b)`, `Q12_MUL(a, b)` | the Q8 product `(a * b) >> 8`, the Q8 quotient `(a << 8) / b` (a ratio or scale), a value times a Q12 scale `(a * b) >> 12` (the screen projections) |
| `math_util.h` | `MIN`, `MAX`, `ABS`, `CLAMP` | the ternaries (`a < b ? a : b`, ...) |
| `math_util.h` | `CLAMP_MIN(x, lo)`, `CLAMP_MAX(x, hi)` | one side of `CLAMP`, `x < lo ? lo : x` / `x > hi ? hi : x` (not `MAX`/`MIN`, which compare the other way round) |
| `math_util.h` | `ABS_BRANCHLESS(x)`, `MAKE_ABS_BRANCHLESS(x, sign)` | the branchless abs `(x ^ (x >> 31)) - (x >> 31)`, and in place: `sign = x >> 31; x ^= sign; x -= sign;` |
| `math_util.h` | `LIMIT_MAX`, `LIMIT_MIN`, `MAKE_ABS`, `CLAMP_INDEX` | the clamp statements (`if (x > hi) x = hi`, `if (i >= n) i = n - 1`, ...) |
| `math_util.h` | `ANIM_REWIND(animTime, rec)` | the animation loop rewind `animTime -= INT_TO_Q8(rec.loopThreshold - rec.loopBase)` |
| `math_util.h` | `SIN_Q8(angle)`, `COS_Q8(angle)` | `gSineTable[angle & 0xFF]` and the quarter-turn `+ 0x40` cosine |
| `entity_bits.h` | `ENTITY_ID_NONE`, `ENTITY_SET_GONE_BIT(_OF)`, `ENTITY_MARK_GONE` | MarkEntityGone's "gone" bitmap set, inlined |

Macros that several files used to define for themselves now live in the
header that owns their type:

| Header | Helpers | What |
|---|---|---|
| `aabb.h` | `AABB_VALID(box)` | a box's `w`, read through a volatile (the "box isn't empty" re-read) |
| `gfx_part.h` | `PART_FLAG_SET(part, shift)` | a +0x28 flag bit tested as a sign test |
| `gba/dma_macros.h` | `DMA3` | channel 3's registers as a `struct dma_regs` |
| `frontend.h` | `CLEAR_OAM(oam)` | the logo screens' one-entry OAM clear |
| `player.h` | `CTRL_KEEP` | SetPlayerCtrlState's "keep the current timer" value |

- **A helper expands to exactly the expression it replaces**: the same
  operands in the same order, the same casts and signedness, the same
  statement or expression form. agbcc (gcc 2.9) compiles some
  "equivalent" spellings differently: `MIN` (a ternary) is not
  `LIMIT_MAX` (an `if`), `(b * a) >> 8` is not `Q8_MUL(a, b)`, and
  wrapping a sequence in `do { } while (0)` adds loop notes that a plain
  `{ }` block doesn't (`ENTITY_SET_GONE_BIT` needs them; crate_break.cpp's
  copy must not have them). Convert a site only when it already has the
  helper's shape, and compare the object.
- **The helpers don't cast.** A shift's signedness comes from its
  operand, as before: `Q8_TO_INT` of an `s32` is `asr`, of a `u32` `lsr`.
- **Only convert what the helper means.** `>> 8` that takes a byte out
  of a word, or `(x << 16) >> 16` that sign-extends to 16 bits, is not a
  fixed-point conversion and keeps its spelling (or gets a helper of its
  own). Literals stay literals: `0x1400` doesn't become
  `INT_TO_Q8(0x14)`.
- **Matching copies stay local, with a comment.** A copy that differs for
  codegen (other register pins, a `match.h` idiom inside the sequence,
  another wrapper) keeps its spelled-out form and says which helper it
  would be and why it isn't. When several files share the same
  variant, it becomes a helper of its own, which takes its registers as
  bare names (`r2`) and passes them to `MATCH_HOLD_REG` (cortex.c's
  `ENTITY_SET_GONE_BIT_PINNED` was one, until cortex.c became C++).
- **Naming:** a helper is named after what it does, upper case:
  `<FORMAT>_TO_<FORMAT>` for conversions, a verb for statements
  (`LIMIT_MAX`, `ENTITY_MARK_GONE`). A subsystem's own helper gets the
  subsystem prefix and lives in its header (`HUD_CLAMP_FRAME` in
  `hud.h`), not a `.c` file, once two files use it.

`tools/common_ops.py` lists the sites that are still spelled out, by
shape (`--report` for the counts per shape, subsystem and file), and the
file-local macros that more than one file defines (`--macros`; the pairs
kept local on purpose are marked). The sites it still lists are either
not what the helper means (byte packing, register and OAM fields, sign
extensions, asset header sizes, CRC steps) or spelled in a form no
helper expands to exactly (a register pin inside the sequence, a sign
mask taken from another value, an explicit `__divsi3` call, a clamp
`if` with more statements or an `else`).

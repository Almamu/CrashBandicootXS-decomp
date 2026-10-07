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
| `sfx.h` | `SFX_` | sound-effect IDs (`PlaySfx`, `PlayAmbientSfx`, `StopSfx`) |
| `songs.h` | `SONG_` | song IDs, the `gSongTable` index (`PlaySong`, `StartSong`) |
| `level_flags.h` | `LEVEL_FLAG_` | the bits of a `levelFlags[]` word |
| `mask_level.h` | `MASK_LEVEL_` | `level_state.maskLevel` values |
| `packed_stats.h` | `PACKED_STATS_` | the packed lives/mask/wumpa halfword |

Planned topics use the same scheme (`entities.h`/`ENTITY_`,
`crates.h`/`CRATE_KIND_`, `events.h`/`EVENT_`, ...).
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
  names are not used.

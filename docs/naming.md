# Function naming

## Default: PascalCase

Most functions get a PascalCase name once they're understood well enough to
name, matching the wider pret/GBA-decomp convention this project otherwise
follows: `AgbMain`, `LoadTaggedAsset`, `QueueVramDmaTransfer`,
`FreeVramDmaQueue`, `FlushVramDmaQueue`, `GetAnimFrameBaseOffset`,
`PlaySfx`, `LoadLevelGraphics`, `LoadBg2Background`, `LoadObjSpriteTiles`,
`IrqDisable`, `IrqSetup`, `IrqEmptyHandler`.

`LZ77UnCompWrapper`/`RLUnCompWrapper` look like they break this (`UnComp`,
not `Uncomp`) but don't - they intentionally mirror the real GBA BIOS `swi`
names (`LZ77UnCompVram`, `RLUnCompVram`). When a function is a thin wrapper
around a real, documented BIOS/hardware call, match *that* name's casing
over generic PascalCase rules.

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
2. Every `extern` declaration and call site, across both `src/*.c` and any
   remaining `asm/*.s` file that still calls it via `bl <name>`/
   `.4byte <name>`.
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

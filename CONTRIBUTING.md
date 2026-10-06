# Contributing

The decompilation itself is done: 2056 of the ROM's 2059 functions are
byte-exact C (the other three are parked, #553), and all of its data is
source. What's left is making the code better to read without changing a
byte of the ROM: real names for the remaining placeholders, compiler
warnings, formatting, and documenting the matching workarounds. That
work is tracked as GitHub issues. Anyone - human or AI agent - can pick
one up. This doc is the short version; the pieces it links to have the
real detail.

## Finding something to work on

- Start from **[the ready-to-work list](https://github.com/Almamu/CrashBandicootXS-decomp/issues?q=is%3Aopen%20is%3Aissue%20-label%3Ablocked%20sort%3Acreated-asc)**,
  which leaves out anything labeled `blocked` and sorts oldest first.
- Most open issues carry the `cleanup` label. The naming ones (#554 to
  #558, #569) list placeholder names by kind; naming one subsystem at a
  time (its functions, globals, data and struct fields together) gives
  the most consistent names, so a PR scoped like "name the level loader"
  is preferred over a long flat list of renames.
- The `parked-function` label marks a function that is understood but
  not byte-exact. Only #553 is left.
- Category labels (`game_loop`, `actor`, `graphics`, `overlay_ui`,
  `graphics_loading`, `audio`, `hud`, `system`, `util`) tell you roughly
  what part of the game an issue is about; [docs/status/](docs/status/)
  has a page per category.
- `python3 tools/chunk_remaining_work.py --cleanup-scan --issues-dir /tmp/issues`
  scans `src/` and `lib/` for raw pointer-arithmetic field access and raw
  hardware addresses. It reports none today; run it to check that new
  code doesn't add any (#550 lists the forms it still misses).

## Claiming work

- Comment on the issue saying what you're taking. For an issue that lists
  many items (names, files), name the subset - say, one subsystem - so
  others can take the rest in parallel.
- If you stop partway through, say so in a comment, with what's done and
  what's left.
- Two people picking the same item is a minor, self-correcting race:
  whoever's PR lands first wins, and the other rebases and drops it.

## Doing the work

**[docs/workflow.md](docs/workflow.md) is the process for changing
matched code - read it before starting.** In short:

- **Every change must keep the ROM byte-identical.** A cleanup that
  silently breaks a match is worse than no cleanup at all. Rebuild after
  each edit, not just at the end, and compare the touched files' `.o`
  against a build of `origin/main` as you go.
- **Don't touch a matching workaround just because it looks unclean.**
  Raw offsets, inline `asm`, `register ... asm("rN")` pins and odd
  statement orders are often load-bearing: they hold the ROM's exact
  register allocation or instruction order in place. Replace one only
  when the rebuild shows the cleaner version produces the same bytes;
  otherwise leave it, with a one-line comment saying why.
- **Renames** follow [docs/naming.md](docs/naming.md): rename only once
  the meaning is understood confidently, and go through its "What
  renaming touches" checklist, including the `rename` line in
  `expected/corrections.txt` so the progress report still pairs the
  symbol. An honest placeholder is better than a wrong name.
- **Files:** a new file goes in the `src/` subsystem directory that fits
  what it holds and is named after its subject (see
  [docs/file_layout_plan.md](docs/file_layout_plan.md)); library code goes
  under `lib/` (see [docs/libraries.md](docs/libraries.md)). Each object
  is placed at its ROM address by its position in `ldscript.txt`, so
  splitting or merging files means updating `ldscript.txt` and
  `tools/report_units.py` too.
- **Data and assets** are rebuilt from source by the Makefile (see
  [README.md](README.md#data-and-assets)): edit the PNG, the level JSON or
  the table in `src/data/`, never anything under `build/`.
- **Frozen files:** `expected/` (the progress report's targets, see
  [expected/README.md](expected/README.md)) and
  [docs/matching.md](docs/matching.md) (the old matching log) are never
  edited. A symbol rename goes into `expected/corrections.txt` instead.
- **The parked functions** (#553) follow the full matching loop in
  [docs/workflow.md](docs/workflow.md), steps 1 to 8.

### Verification

Only a **full clean rebuild** counts as proof. Before every PR:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate -o report.json
```

`make compare` must print `crashbandicootxs.gba: OK`. If it doesn't,
`cmp` each touched object against the same object built from
`origin/main` to find the one that changed, rather than guessing. The report must still show 2056/2059 functions and 100%
data (2059 means the build wasn't clean). The Makefile tracks header
dependencies, so a plain `make` is fine while you work, but the PR check
is always from clean.

If your change touches what a `docs/status/<category>.md` page or a
`docs/` page says, update it in the same PR.

### Declarations and headers

Every function and global is declared once, in a header, with the type
of its definition. `.c` files include headers; they don't declare
things themselves. [docs/headers_plan.md](docs/headers_plan.md) has the
full rules and the history (#574). For new code:

- **Which header:** a function or global goes in the header of the
  subsystem that owns it - `include/<subsystem>.h`, named after its
  `src/` directory (`player.h`, `crates.h`, `level.h`, ...). A global
  that three or more subsystems use (`gPlayer`, `gLevelState`, `gKeys`,
  ...) goes in `include/globals.h`. GAX engine internals (what one GAX
  object calls in another) go in `lib/gax/src/gax_internal.h`. A struct
  that several subsystems share gets its own small type header
  (`aabb.h`, `hitbox_quad.h`, `camera_lead.h`).
- **Don't redeclare externs in a `.c` file.** Include the header. If
  the header's type doesn't fit, fix the header (or the definition) to
  the real type, then fix the callers - don't add a local copy with
  another type.
- **Codegen exceptions:** if a file only matches with another type for
  a symbol (a `u8` return where the definition has `s32`, a one-byte
  struct argument, ...), keep a local declaration under another name
  with an asm label, and say why in a comment:

  ```c
  /* codegen: RandRange is u16, but this file was matched against an s32
   * return; the u16 prototype changes the stack slots in InitTitleScreen.
   * docs/headers_plan.md */
  extern s32 RandRange_s32(s32 max) asm("RandRange");
  ```

  The call still links to `RandRange`, and the file can include the
  header. Add the alias to the "Codegen exceptions" table in
  docs/headers_plan.md.
- **What stays local:** `_call_via_r0`..`_call_via_r7` (each call site
  declares the shape it calls through) and references from one
  `src/data/` table to another.
- **Header edits rebuild their dependents.** The Makefile tracks header
  dependencies (a `.d` file per object, written by the preprocess
  step), so a plain `make` after editing a header rebuilds every object
  that includes it. Still do a clean build before opening a PR (see
  "Verification" above).
- **agbcc's compile errors don't contain the word "error"** (for example
  ``structure has no member named `x'``). Check `make`'s exit status
  rather than grepping the log for "error".
- **Check your work** with `python3 tools/extern_audit.py` after a build:
  "remaining" must stay 0 and no struct name may be defined in more
  than one `.c` file. `--remaining` lists the declarations that no
  exception covers, and `--views` lists `.c`-file structs that have the
  layout of a header struct (often a copy or a view of it; check the
  readers before merging).

### Compiler warnings

The build is warning-free and stays that way (#577). Every C object,
whichever compiler builds it (agbcc, old_agbcc or agbcc_arm, including
the per-object flag overrides), gets the Makefile's `WARNFLAGS`:

```
-Wall -Wmissing-prototypes -Wpointer-arith -Wnested-externs -Wredundant-decls -Werror
```

`-Wall` already includes `-Wimplicit`, `-Wparentheses`, `-Wunused`,
`-Wreturn-type` and (at `-O1`/`-O2`) `-Wuninitialized`. `-Werror` makes
any warning fail the build, locally and in CI. As with any compile error,
agbcc's message doesn't contain the word "error": look for "warnings
being treated as errors" and the `warning:` line under it.

Fix a warning the way that leaves the bytes alone, and check it with the
two clean builds above:

- **Missing prototype** (`no previous prototype for X`): declare the
  function in its owner's header (see "Declarations and headers"), even
  when it's only reached through a table or a `UNUSED` function. A prior
  prototype with the definition's exact types doesn't change the code.
- **Redundant redeclaration:** delete the second declaration. If two
  headers declare the same symbol, keep it in the owner's header and have
  the other one include that header.
- **Pointer arithmetic on `void *`:** use a `u8 *` (or, better, the
  real struct type) for the arithmetic.
- **Unused variable:** delete it, if the object stays identical.
- **Might be used uninitialized:** many of these are deliberate, since
  the ROM reads whatever the register held. A real initializer usually
  adds or changes code, so check it before keeping it. An inline-asm
  operand marked `"+r"` that the asm only writes should be `"=r"`.

When the honest fix changes the bytes, keep the code and silence just
that one site, with a comment saying why. agbcc 2.9 has no
`#pragma GCC diagnostic`, so use one of these, in this order:

1. **Self-initialization** for `-Wuninitialized`: `u32 bg0cnt = bg0cnt;`
   gcc emits no code for it. Used in `starfield.c`,
   `title_screen_init.c`, `fade.c`, `sprite_frame.c`, `eeprom_verify.c`
   and `gax_voice_steal.c`.
2. **`__attribute__((unused))`** on a variable or parameter that has to
   stay for codegen, for `-Wunused`.
3. **A per-object override** in the Makefile, as a last resort:
   `$(C_BUILDDIR)/foo/bar.o: CC1FLAGS += -Wno-<warning>`, with a comment
   saying which site needs it and why nothing narrower works. It
   silences the warning for the whole file, so prefer 1 or 2.

Never remove `-Werror` or a flag from `WARNFLAGS` to get a change in.

## Opening the PR

Say what the PR changes and how you verified it (the two clean checks
above). Reference the issue with `Refs #N`; use `Closes #N` only in the
PR that finishes everything the issue asks for. If you only did part of
it, say what's left in the PR description so the next person can pick
it up.

**Never write the literal substring `close #N` / `closes #N` anywhere in
a PR description when you mean the opposite** - not even inside "this
does **not** close #N" or "leaving #N open, not closing it". GitHub's
issue-linking scanner matches that substring as a real closing keyword
regardless of any surrounding negation, and will auto-close the issue
the moment the PR merges, silently reversing your own stated intent.
This has already happened for real (8 issues auto-closed this way with
functions still fully raw - see the reopening comments on #2/#13/#19/
#30/#40/#58/#63/#68 for the concrete examples). When you need to say a
PR does *not* close an issue, phrase it without the word "close" next
to the issue number at all - e.g. "issue #N stays open" or "the rest of
#N is still to do".

## If you're an AI agent

The process above is tool-agnostic, and [README.md](README.md#ai-assisted-decompilation)
has this project's stance on AI-assisted work in general. Claude Code
sessions can also use the
[`match-chunk` skill](.claude/skills/match-chunk/SKILL.md), written for
the matching campaign: it takes an issue number, works through the
functions it lists, verifies, and prepares the PR.

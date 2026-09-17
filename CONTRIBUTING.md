# Contributing

This project's remaining work is tracked as GitHub issues, one per
scoped chunk of still-raw ROM code (or one per already-parked function
that needs a compiler-workaround idea). Anyone - human or AI agent - can
pick one up. This doc is the short version; the pieces it links to have
the real detail.

## Finding something to work on

- Look through the [issues list](../../issues) for the `decomp-chunk`
  label (a group of ~10-25 still-unmatched functions in one address
  range), the `parked-function` label (one specific function whose
  semantics are already understood, just not yet byte-exact), or the
  `cleanup` label (already-matched code that still needs docs/workflow.md
  step 7's struct/register-macro cleanup applied - see "Cleanup tasks"
  below).
- **A `decomp-chunk` issue is just a scoping convenience, not a
  contract.** The grouping exists to make the ROM's remaining work
  easier to browse and claim - there's no obligation to match every
  function in one before opening a PR, and no obligation to do it alone.
  Multiple people can work different functions from the same issue in
  parallel (say which ones you're taking, in a comment, so others don't
  duplicate it), split it across several smaller PRs, or hand off
  partial progress to someone else via a comment. Open a PR for however
  much you've matched/parked/left-with-a-reason, reference the issue
  without `Closes` unless you finished the whole thing, and leave a
  comment saying what's left for the next person.
- Category labels (`game_loop`, `actor`, `graphics`, `overlay_ui`,
  `graphics_loading`, `audio`, `hud`, `system`, `util`) tell you roughly
  what part of the game it's in - see [docs/rom_map.md](docs/rom_map.md)
  for what's actually known about each.
- No open issue for what you want to work on? Regenerate the current
  list yourself:
  ```
  python3 tools/chunk_remaining_work.py --max-functions 25 --issues-dir /tmp/issues --parked-issues
  ```
  This scans `asm/*.s` fresh, so it reflects whatever's actually still
  raw right now - open issues can go stale as other PRs land.

## Claiming work

Each `decomp-chunk` issue lists its functions as a GitHub task list
(`- [ ] sub_XXXXXXXX`), specifically so claiming can happen at the
function level, not just the whole-issue level:

- **Claiming the whole issue:** comment saying you're starting it.
- **Claiming a subset:** comment naming which functions you're taking
  (e.g. "taking `sub_8001640`-`sub_8001688`"), and check their boxes in
  the issue body as you go (anyone with write access can edit a task
  list; if you don't have that, ask in a comment and a maintainer will
  check them off, or just leave a comment - the checkboxes are a
  convenience, not the source of truth). Leave the rest unchecked for
  someone else.
- **`parked-function` issues** are single-function, so a comment saying
  you're starting it is enough - no task list needed.
- If you stop partway through, say so in a comment and leave whatever
  you checked off checked - don't uncheck progress that's real just
  because you're stepping away from the rest.
- Two people checking the same box around the same time is a minor,
  self-correcting race, not a problem to design around - whoever's PR
  lands first wins, and the second person's `make compare` catches it
  immediately (the function will already be gone from `asm/*.s`) so
  they just drop that one and move to another box.

## Doing the work

**[docs/workflow.md](docs/workflow.md) is the actual process - read it
before starting, it's not optional style guidance.** In short, for each
function in your chunk:

1. Read its disassembly and understand what it does. Check
   [docs/rom_map.md](docs/rom_map.md) and
   [docs/matching.md](docs/matching.md) first - a lot of structs,
   globals, and calling conventions in this ROM are already named and
   documented, and re-deriving them from scratch wastes time.
2. Write the C reconstruction. Prefer named structs/fields and
   `REG_*`/hardware-register macros over raw pointer-arithmetic casts.
3. Compile in isolation and diff the resulting instructions against the
   ROM's disassembly, byte by byte - **this isolated compile is a
   diagnostic tool, never proof of a match.**
4. Iterate until it's byte-identical, using the techniques cataloged in
   [docs/matching.md](docs/matching.md) (register pins, `goto`s to force
   block ordering, the negative-constant bit-clear idiom, and more).
5. **Matched:** cut it out of the raw `asm/*.s`, add it to the right
   `src/**/*.c` file, update `ldscript.txt`/`tools/report_units.py` if
   you introduced or split a file.
6. **Fully understood but genuinely resistant to byte-exact matching:**
   park it under `#if NON_MATCHING` / `.if NON_MATCHING == 0`, with a
   doc comment explaining exactly what you tried and why it didn't work.
7. **Semantics not confidently understood:** leave it completely
   untouched and say so in your PR - never guess or force a low-
   confidence match.
8. **No caller anywhere in the ROM:** still match or park it like any
   other function, then tag it `UNUSED - no caller anywhere in the ROM
   (checked ...)`.

Only a **full clean rebuild** counts as proof:

```
rm -rf build && make NON_MATCHING=1 report
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
```

`make compare` must print `La suma coincide`. If it doesn't, see
`docs/workflow.md`'s map-file address-shift diagnostic method rather
than guessing at what regressed.

Then update `docs/matching.md` and `docs/status/<system>.md` to reflect
what you matched/parked/left, and commit.

## Opening the PR

Reference the issue (`Closes #N`) only if you got through every function
in the chunk (matched, parked, or explicitly left with a stated reason).
If you only got partway through a large chunk, leave the issue open and
say what's left in the PR description instead.

## Cleanup tasks

Not all remaining work is about matching new functions - a lot of it is
going back over **already-matched** code and applying
[docs/workflow.md](docs/workflow.md) step 7's cleanup pass
retroactively, for functions that were matched before that step was
consistently applied (or where it was reasonably deferred at the time).
Look for the `cleanup` label, or generate a fresh list yourself:

```
python3 tools/chunk_remaining_work.py --cleanup-scan --issues-dir /tmp/issues
```

This scans already-matched `src/**/*.c` files for raw pointer-arithmetic
field access (`*(u32 *)((u8 *)base + 0x10)`-style casts) and raw
hardware addresses that should be named structs/`REG_*` macros, and
groups the results per file into issues the same way `decomp-chunk`
ones work (claim, comment, PR - same conventions as above).

**The important carve-out, straight from docs/workflow.md step 7:**
don't touch a raw offset or an inline-asm/`register ... asm("rN")` pin
just because it looks unclean - a lot of them are load-bearing, holding
the exact ROM register allocation in place. Only replace one where you
can actually demonstrate (by rebuilding) that the cleaner version still
compiles to the identical bytes; if it doesn't, revert the change and
leave a one-line comment explaining why it has to stay low-level,
rather than force it through. **Every cleanup PR needs the same full
clean `make compare` verification as a new match** - a cleanup that
silently breaks a match is worse than not doing the cleanup at all.

## If you're an AI agent

Claude Code sessions working in this repo can use the
[`match-chunk` skill](.claude/skills/match-chunk/SKILL.md), which wraps
the same process above into a single dispatcher: point it at an issue
number and it reads the issue, works through the functions, verifies,
and prepares the PR. Other agents are welcome too - the process above is
tool-agnostic, and [README.md](README.md#ai-assisted-decompilation) has
this project's stance on AI-assisted work in general.

## Naming functions

`sub_XXXXXXXX` is a completely valid, permanent name - most of the ROM
will stay that way for a long time, and it's not a placeholder blocking
progress. Only replace it with a real name when you're genuinely
confident, per [docs/naming.md](docs/naming.md). A wrong or
overconfident name is worse than an honest `sub_XXXXXXXX`.

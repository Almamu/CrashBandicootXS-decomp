# Simplifying the matched code (#846)

Matching leaves shapes in the source that only made sense while a
function was being matched: bare `{ }` blocks that scope a temporary,
locals assigned once and read once, copies of a variable into another,
an `if` nested in an `if`, switch cases with the same body. The
[#846](https://github.com/Almamu/CrashBandicootXS-decomp/issues/846) pass
removed them wherever the object stays byte-identical, and put a short
comment on each one the match needs, so nobody tries it again.

## What the pass does

- **Flatten bare blocks.** A block that only scopes locals loses its
  braces and its body moves out one level. Its declarations stay where
  they were (C++ style). Two cases need more than that:
  - a `goto` or a case label would jump over one of its initialized
    locals: the declarations go to the top of the function, and the
    initializers stay as assignments;
  - the function already has a local of the same name (the same
    temporary in two blocks): the block's declaration becomes an
    assignment to that local.
- **Inline single-use locals and copies.** A local assigned once and
  read once is replaced by its value, in parentheses only where needed.
  A *copy* is the same thing where the value is a plain variable or
  field.
- **Fold nested ifs.** `if (a) { if (b) S }`, with nothing between the two
  tests and no `else`, becomes `if (a && b) S`.
- **Merge duplicate cases.** Switch cases with identical bodies share
  one body. This one is done by hand.

Each change is kept only when the object's `.text`, `.rodata`, `.data`
and relocations stay byte-identical. Otherwise it is reverted.

## The tool: `tools/simplify_scan.py`

The tool parses every `src/**/*.cpp` (not `lib/`, which is original
source, and not the C files in `src/data/`) and finds seven kinds of
candidate:

| kind | what |
|---|---|
| `block` | a bare `{ }` block that declares locals (it only scopes them) |
| `braces` | a bare block that declares nothing |
| `caseblk` | a `case X: { ... }` body with no declarations |
| `nestif` | `if (a) { if (b) S }` with nothing between and no `else` |
| `single` | a local assigned once and read once |
| `copy` | the same, where the value is a plain variable or field |
| `dupcase` | switch cases with identical bodies |

A candidate is *documented*, and never tried, when a comment covers it:

- a `register` local (a pin);
- a local named in a comment of its function, or in the comment above it;
- a block or `if` with a comment on its first line or just above it.

The deliberate shapes (#841/#842's dead tests, the round notes, the
accepted redundant copies, the pins and the codegen views) all have such
a comment, so the tool leaves them alone.

```
tools/simplify_scan.py [PATH...]                  # inventory per kind and directory (default: src/)
tools/simplify_scan.py --list [PATH...]           # every candidate, one per line
tools/simplify_scan.py --try [-j N] [--kinds K] [--log TSV] FILE.cpp|DIR...
tools/simplify_scan.py --try --annotate FILE.cpp|DIR...
```

- **No option:** the inventory. For each kind, the open candidates, the
  documented ones and the number of functions; then the open
  candidates per directory.
- **`--list`:** one line per candidate: `file:line`, kind, function, and
  the local's name (or the duplicate cases' lines), plus `documented`
  when a comment covers it.
- **`--try`:** applies the mechanical kinds (`block`, `braces`,
  `caseblk`, `nestif`, `single`, `copy`) one at a time. After each one
  it rebuilds that file's object with the Makefile's own commands
  (`make -n`: the same compiler and per-object flags), into a temporary
  directory, and compares it with the object built from the file as it
  was. The kept changes are written to the files. It works on the
  sources in place, so commit or stash first and read the diff after.
  - `-j N`: the files tried in parallel (default: the number of CPUs).
  - `--kinds K`: a comma-separated subset of the mechanical kinds.
  - `--log TSV`: where the log of every try goes (default
    `simplify_try.tsv`). Each row is `kept`, `needed` (the object
    changes; the row says how), `error` (it doesn't compile) or `skip`
    (not tried: a side effect would move, the result would pass 100
    columns, a macro evaluates its argument twice, a name would shadow
    another).
- **`--annotate`** (with `--try`): once the loop is done, tries what is
  left once more and puts a one-line comment above each local, block and
  nested `if` that is needed. The comment names the local, so the next
  run counts it as documented.

Only `.cpp` files are tried: a header's inline functions are in many
objects. The whole tree takes about 20 seconds. After a `--try`, run
`tools/format.py` and verify as usual (a full clean `make compare`, the
report, the format and idiom checks).

The 100-column check measures every line the change produces, with its
indentation: a folded `&&` test, an inlined value, a hoisted
declaration. A change that would add a line past 100 columns
(`.clang-format`'s `ColumnLimit`) is skipped rather than left for
`format.py` to wrap.

## The comments

`--annotate` writes three kinds of comment. Each says what the simpler
form does to the object, from a disassembly of both builds:

- **"is needed"**, on a local the match needs:
  `` /* `dir` is needed: inlined, the code is 2 instructions shorter */ ``.
  Adjacent locals needed for the same reason share one comment
  (`` /* `dx`, `dy` are needed: ... */ ``).
- **"scoped"**, on a block that keeps its own copy of a temporary that
  another block of the function also declares:
  `` /* scoped: its own `a` (shared, the registers differ) */ ``. One shared
  local changes the register allocation or the code.
- **"nested"**, on an `if` that has to stay nested:
  `` /* nested: as one `&&` test, the code is 4 instructions shorter */ ``.

The reason is one of:

| phrase | what changed |
|---|---|
| the registers differ | the same instructions, other registers |
| the instructions are reordered | the same instructions in another order |
| a call moves | the same instructions, a `bl` in another place |
| the code is N instructions longer/shorter | other code, N instructions more or fewer |
| the instructions differ | other code, the same length |

A few of these were rewritten by hand: where a comment nearby already
said it, the generated one was dropped or the nearby one now names the
local. Some byte-identical changes were also left out on purpose, with
a comment: a flatten that would leave one local shadowing another,
inlining one half of a matching pair (`absDx`/`absDy`, `aMaxX`/`bMaxX`,
`dxSq`/`dySq`, `p0`-`p3`), and an inline that turns named terms into one
long cast-laden expression.

## Before and after

`tools/simplify_scan.py` over `src/`. Before is the inventory on
276c358c, when the pass started. After is main with #938-#941,
#943-#946 and #947-#950 applied.

| kind | open before | open after | documented before | documented after |
|---|---:|---:|---:|---:|
| single | 442 | 65 | 84 | 380 |
| block | 298 | 6 | 9 | 38 |
| copy | 267 | 20 | 68 | 259 |
| nestif | 42 | 11 | 0 | 6 |
| dupcase | 17 | 8 | 0 | 0 |
| braces | 5 | 0 | 0 | 0 |
| caseblk | 1 | 1 | 0 | 0 |
| **total** | **1072** | **111** | **161** | **683** |

What the first dry run over the whole tree showed, per kind:

1. **Blocks:** nearly all of them flatten. Scope never decided the code:
   no flatten that compiled changed the bytes. The ones that stay keep
   their own copy of a temporary that another block also declares.
2. **Nested ifs:** most of them fold.
3. **Single-use locals and copies:** only about a quarter can go. The
   rest set the load order, a call's place, the register allocation or a
   narrowing.

## Why the remaining 111 stay

- **single / copy (85):** the ones the tool doesn't try:
  - the line would pass 100 columns;
  - the value goes into a macro that evaluates it twice
    (`ABS_BRANCHLESS`, `CLAMP_INDEX`);
  - a call or other side effect would move;
  - the local is the target of a chained assignment;
  - a macro uses the local by name (`SPAWN_DUE`'s `scroll`).
- **nestif (11):** the inner `if` has a comment before it, or its test has
  an assignment or `++`; or the folded test would wrap over three lines
  (PolarPlayer::Launch), where the nested form reads better.
- **dupcase (8):** merging the labels changes the object: the ROM keeps
  its own copy of each body. CreateActor's and GetFrameAnchor's cases say
  so in a comment, and EnemyCtrl::SetState's header explains its groups.
  The other five are in `player/` (StateHangMove, CheckPlayerContact,
  HandleEvent) and `crates/` (PlayerThirdBox, two groups).
- **block (6) + caseblk (1):** all in LevelProgress::PlayRoom. The
  comment on its `switch` says why: each step has its own `bank`/`pl`,
  one shared `pl` makes the code 2 instructions longer, and the case
  labels would cross them. The tool only counts a comment on the block
  itself, so `--try` still flattens four of them, byte-identical: revert those.

Also left alone: the original-source asm (DivMod, GaxHuffUnComp), the
codegen views, link_session_reset's `(players - 1)[i + 1]` (#908) and
DrawVvLogoPieces' `buf = scratch` (#920).

## Where things are

- The inventory and the closing summary are on
  [#846](https://github.com/Almamu/CrashBandicootXS-decomp/issues/846).
- The PRs, one per directory or family: #938 (with the tool), #939,
  #940, #941 (player, level, crates, frontend + menus); #943, #944, #945,
  #946 (objects, bosses, vehicle/jetpack, save); #947, #948, #949, #950
  (actor + enemies, vehicle/polar + pickups + hud, gfx + cutscene + text
  + iwram, link + util + system + audio). Each one lists what it
  simplified and what had to stay.

> :warning: **This project is in it's infancy, nothing useful comes out of it yet**

Work in progress matching decompilation of Crash Bandicoot XS (Crash Bandicoot: The Huge Adventure).

It builds the following ROM:
* [**crashbandicootxs.gba**](https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0310) `sha1: bdd061e1b5187c0528928ec0ddb6886b1de16970` (Europe) (En,Fr,De,Es,It,Nl)

> :warning: **Matching a function? Read [docs/workflow.md](./docs/workflow.md) first.**
> It's the required per-function loop (match byte-exact, then clean up
> hardware registers/structs/pointer arithmetic before moving on to the
> next one) - not optional style guidance.

## Current state

Main decomp efforts just started. `src/` is split into `graphics/`, `util/`,
`system/`, and (once anything's matched there) `audio/`, mirroring how the
game's code is actually organized - see [docs/status/](./docs/status/) for
the full per-system breakdown of what's matched, what's parked and why, and
what's still fully raw asm.

## AI-assisted decompilation

A large part of this project's matching work (function reversal, byte-exact
C reconstruction, ROM address-space mapping, and documentation) has been
done with the help of AI coding agents (Claude Code), under human
supervision and review. This isn't a secret or an experiment on the side -
it's the primary way this project has been making progress, and it's
expected to keep being used that way. A few things follow from that:

- **Nothing is trusted on the AI's say-so.** Every function marked
  "matched" has gone through the full clean-rebuild verification described
  in [docs/workflow.md](./docs/workflow.md) - an isolated compile matching
  is not proof, only a byte-identical `make compare` against the real ROM
  is. This applies equally whether a human or an AI made the change.
- **Contributions from AI-assisted work are welcome**, including from
  contributors using their own AI tooling - as long as the same
  verification standard is met (byte-exact match or an honestly-documented
  `NON_MATCHING` park, never a guess presented as a match).
  [docs/rom_map.md](./docs/rom_map.md) and [docs/matching.md](./docs/matching.md)
  exist specifically so an AI agent (or a human) picking up a fresh session
  has enough context to keep going without re-deriving everything from
  scratch.
- **Semantic naming should stay honest.** `sub_XXXXXXXX` is a perfectly
  valid, permanent name for a fully-matched function - AI agents (and
  humans) should only replace it with a real name when genuinely confident,
  per [docs/naming.md](./docs/naming.md). An overconfident or invented name
  is worse than an honest placeholder.
- **Humans still make the calls that matter**: what gets merged, how the
  project is organized, and any judgment call that isn't purely mechanical
  verification.

If this isn't the kind of project you want to contribute to for that
reason, that's a completely reasonable position - just know it going in.

## Setting up the repo

Please see follow [these instructions](./INSTALL.md)

## Notes

- [CONTRIBUTING.md](./CONTRIBUTING.md) - **how to pick up a chunk of work from the issue tracker, human or agent, and get it to a PR**
- [docs/status/](./docs/status/) - **per-system matched/parked function status, the "Current state" section moved here**
- [docs/workflow.md](./docs/workflow.md) - **the required per-function matching loop, must be followed for every function**
- [docs/naming.md](./docs/naming.md) - **the function naming convention, must be followed whenever a function gets a real name**
- [docs/audio.md](./docs/audio.md) - how the Shin'en GAX2 sound engine's data is laid out and rebuilt
- [docs/graphics.md](./docs/graphics.md) - how graphics were extracted, and ongoing notes on the sprite/actor system
- [docs/matching.md](./docs/matching.md) - byte-exact matching decompilation gotchas, and the per-function matching/parked log
- [docs/decomp_dev.md](./docs/decomp_dev.md) - how this project's [decomp.dev](https://decomp.dev) progress report is generated in CI
- [docs/rom_map.md](./docs/rom_map.md) - a rough, whole-ROM map of what still-unmatched code probably belongs to which system
- The [Kirby & The Amazing Mirror](https://github.com/jiangzhengwenjz/katam/) decompilation uses a very similar codebase, as it was written by the same dev team (Dimps)
- https://decomp.me is a great resource for helping to create matching functions
- `ldscript.txt` tells the linker the order which files should be linked
- For more info, see the [FAQs section](https://zelda64.dev/games/tmc) of TMC

## Credits

- [@normmatt](https://github.com/normmatt) for the basic initial repository setup that he did for [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation
- [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation as a starting point to get the project building too
- [Pokemon Reverse Engineering Tools](https://github.com/pret) for some of the GBA tooling

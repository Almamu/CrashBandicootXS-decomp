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

## Setting up the repo

Please see follow [these instructions](./INSTALL.md)

## Notes

- [docs/status/](./docs/status/) - **per-system matched/parked function status, the "Current state" section moved here**
- [docs/workflow.md](./docs/workflow.md) - **the required per-function matching loop, must be followed for every function**
- [docs/naming.md](./docs/naming.md) - **the function naming convention, must be followed whenever a function gets a real name**
- [docs/audio.md](./docs/audio.md) - how the Shin'en GAX2 sound engine's data is laid out and rebuilt
- [docs/graphics.md](./docs/graphics.md) - how graphics were extracted, and ongoing notes on the sprite/actor system
- [docs/matching.md](./docs/matching.md) - byte-exact matching decompilation gotchas, and the per-function matching/parked log
- [docs/decomp_dev.md](./docs/decomp_dev.md) - how this project's [decomp.dev](https://decomp.dev) progress report is generated in CI
- The [Kirby & The Amazing Mirror](https://github.com/jiangzhengwenjz/katam/) decompilation uses a very similar codebase, as it was written by the same dev team (Dimps)
- https://decomp.me is a great resource for helping to create matching functions
- `ldscript.txt` tells the linker the order which files should be linked
- For more info, see the [FAQs section](https://zelda64.dev/games/tmc) of TMC

## Credits

- [@normmatt](https://github.com/normmatt) for the basic initial repository setup that he did for [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation
- [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation as a starting point to get the project building too
- [Pokemon Reverse Engineering Tools](https://github.com/pret) for some of the GBA tooling

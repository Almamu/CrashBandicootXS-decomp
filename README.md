> :warning: **This project is in it's infancy, nothing useful comes out of it yet**

Work in progress matching decompilation of Crash Bandicoot XS (Crash Bandicoot: The Huge Adventure).

It builds the following ROM:
* [**crashbandicootxs.gba**](https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0310) `sha1: bdd061e1b5187c0528928ec0ddb6886b1de16970` (Europe) (En,Fr,De,Es,It,Nl)

## Current state

- Main decomp efforts just started
- First 17 functions matching: `QueueVramDmaTransfer`/`FreeVramDmaQueue`/
  `FlushVramDmaQueue`/`sub_8006B0C`/`sub_8006AF4`/`sub_8006AC8`/
  `sub_8006AAC`/`sub_8006A78`/`sub_8006A84`/`sub_8006A90`/`sub_8006A48`/
  `sub_8006A14`/`sub_80069E8`/`sub_800697C` in `src/graphics.c`,
  `sub_8006920`/`sub_800695C` in `src/oam_count.c`,
  `GetAnimFrameBaseOffset` in `src/actor_anim.c` (see `docs/graphics.md`,
  "Matching decompilation" for the workflow and a few gotchas worth
  knowing before doing more of this)

## Setting up the repo

Please see follow [these instructions](./INSTALL.md)

## Notes

- [docs/audio.md](./docs/audio.md) - how the Shin'en GAX2 sound engine's data is laid out and rebuilt
- [docs/graphics.md](./docs/graphics.md) - how graphics were extracted, and ongoing notes on the sprite/actor system
- The [Kirby & The Amazing Mirror](https://github.com/jiangzhengwenjz/katam/) decompilation uses a very similar codebase, as it was written by the same dev team (Dimps)
- https://decomp.me is a great resource for helping to create matching functions
- `ldscript.txt` tells the linker the order which files should be linked
- For more info, see the [FAQs section](https://zelda64.dev/games/tmc) of TMC

## Credits

- [@normmatt](https://github.com/normmatt) for the basic initial repository setup that he did for [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation
- [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation as a starting point to get the project building too
- [Pokemon Reverse Engineering Tools](https://github.com/pret) for some of the GBA tooling

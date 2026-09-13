> :warning: **This project is in it's infancy, nothing useful comes out of it yet**

Work in progress matching decompilation of Crash Bandicoot XS (Crash Bandicoot: The Huge Adventure).

It builds the following ROM:
* [**crashbandicootxs.gba**](https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0310) `sha1: bdd061e1b5187c0528928ec0ddb6886b1de16970` (Europe) (En,Fr,De,Es,It,Nl)

## Current state

- Main decomp efforts just started
- First 53 functions matching: `QueueVramDmaTransfer`/`FreeVramDmaQueue`/
  `FlushVramDmaQueue`/`sub_8006B0C`/`sub_8006AF4`/`sub_8006AC8`/
  `sub_8006AAC`/`sub_8006A78`/`sub_8006A84`/`sub_8006A90`/`sub_8006A48`/
  `sub_8006A14`/`sub_80069E8`/`sub_800697C` in `src/graphics.c`,
  `sub_8006700`/`sub_8006714`/`sub_8006770`/`sub_80067A4`/`sub_80067B4`/
  `sub_80067C4`/`sub_80067D4`/`sub_80067E4`/`sub_80067EC`/`sub_8006820`/
  `sub_8006864`/`sub_80068A8`/`sub_80068CC`/`sub_8006920`/`sub_800695C`
  in `src/oam_count.c`, `sub_80006A8`/`sub_80006EC`/`sub_80006F8`/
  `sub_8000720`/`sub_8000760`/`sub_80007AC`/`sub_80007DC` in `src/irq.c`,
  `sub_80008B4`/`sub_80008CC`/`sub_80008F0`/`sub_80008FC`/`sub_800090C`/
  `sub_8000924`/`sub_800093C` in `src/math_util.c`,
  `sub_800094C`/`sub_80009F4` in `src/string_util.c`,
  `sub_8000AA8`/`sub_8000CA8` in `src/printf_util.c`,
  `sub_8000D68`/`sub_8000D80`/`sub_8000DAC`/`sub_8000DE0`/`sub_8000DF8`
  in `src/string_util2.c`,
  `GetAnimFrameBaseOffset` in `src/actor_anim.c`
  (see `docs/graphics.md`, "Matching decompilation" for the workflow and
  a few gotchas worth knowing before doing more of this)
- `sub_8006600` (in `src/oam_count.c`) is reconstructed but not yet
  byte-matching - compiled only under `make NON_MATCHING=1` for now (see
  `docs/graphics.md`, "Parked, not matched: sub_8006600"); a local
  decomp-permuter instance is searching for the remaining register
  picks in the sibling `decomp-permuter` checkout
- `sub_80007EC` (ROM `0x080007EC`) is understood but not yet
  byte-matching (an instruction-scheduling detail); `asm/code_3_1.s` was
  split into itself (just this one function) plus `asm/code_3_1_2.s`
  (everything after it) so later functions could still be matched -
  see `docs/graphics.md` for the pattern to reuse if this happens again
- `sub_8000CBC` (in `src/printf_util.c`, a case-insensitive `strstr`) is
  also understood but not yet byte-matching - compiled only under
  `make NON_MATCHING=1` for now (see `docs/graphics.md`, "Parked, not
  matched: sub_8000CBC"): matching a specific redundant-truncate branch
  shape in its lowercase-fold logic conflicts with keeping
  `caseInsensitive` out of `r8`; `asm/code_3_1_2.s` now holds just this
  parked function, with everything after it (from `sub_8000D80` on)
  moved to `asm/code_3_1_3.s` so `sub_8000D68` in between could still be
  matched

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

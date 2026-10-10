# decomp-permuter setup for the IWRAM ARM functions

A reusable [decomp-permuter](https://github.com/simonlindholm/decomp-permuter)
setup for the two `NAKED` functions in the agbcc_arm objects (#553):
`itoa_arm` (`src/iwram/string_arm.c`) and `LookupSpriteFrameCache`
(`src/iwram/sprite_arm.c`). The results are in
`docs/matching/iwram-image.md` ("Sixth pass").

**Both functions match now** (seventh pass, with agbcc_arm_patched), so
their `NAKED` asm is gone and `setup.sh` can't build their targets from
the current source. Use `src/iwram/*.c` from before that change to rerun
them, or this directory as a template for another ARM function.
`itoa_arm` has been assembly since #662 (`asm/itoa_arm.s`: its ROM code
isn't gcc's); its directory here is kept as a record.

## Files

- `compile.sh`: the permuter's compile script (`compile.sh in.c -o
  out.o`). It runs the repo's cpp include paths (so `include/match.h`
  works), then agbcc_arm with the objects' flags from the Makefile's
  `ARM_OBJS` rule (`-mthumb-interwork -O2 -fomit-frame-pointer`), then
  GNU as. It also copies any candidate that pushes registers without lr,
  or that has two `ldmfd sp!, {lr}; bx lr` returns, to `hits/` next to the
  permuter dir (or `$PERMUTER_HITS`), whatever its score. Neither shape
  has ever appeared.
- `mktarget.py`: writes `target.s` from a function's `NAKED` asm.
- `setup.sh <function> <source.c> <dir>`: makes a permuter dir with
  `base.c`, `settings.toml`, a `compile.sh` symlink and `target.s`/`.o`.
- `itoa_arm/`, `LookupSpriteFrameCache/`: each function's `base.c` (the
  `NON_MATCHING` draft, with `PERM_*` macros) and `settings.toml`.
  `itoa_arm/base.c` defines the `match.h` macros with
  `#pragma _permuter latedefine`, so the permuter's C parser never sees
  the asm, and keeps the pins and the SWI inside `PERM_IGNORE`.

## Running

Needs `tools/agbcc` (with `agbcc_arm`), the devshell's
`arm-none-eabi-*` tools, and a decomp-permuter clone (Python with
`pycparser`, `toml`, `Levenshtein`).

```sh
git clone https://github.com/simonlindholm/decomp-permuter /path/to/decomp-permuter
tools/permuter/setup.sh itoa_arm src/iwram/string_arm.c /path/to/perm/itoa
cd /path/to/decomp-permuter
timeout -s INT 2700 ./permuter.py /path/to/perm/itoa -j"$(nproc)" \
    --stop-on-zero --better-only > /path/to/perm/itoa.log 2>&1
```

Improvements are written to `<dir>/output-<score>-<n>/` (`source.c`,
`diff.txt`). The progress line is long and rewritten in place, so
summarise the log rather than printing it, for example
`grep -o 'best score! ([0-9]* vs [0-9]*)' itoa.log`.

## What score 0 meant here

(Before the seventh pass.) Neither function could reach 0 with agbcc_arm. `itoa_arm`'s ROM pushes
r4-r6 without lr, and `LookupSpriteFrameCache`'s three returns pop into
lr. agbcc_arm always adds lr to a register push, and every return insn
pops into ip (docs/matching/iwram-image.md, fifth pass). The floor is
the score of those instructions: 30 for `LookupSpriteFrameCache` (its
draft is already there), and the push/pop pair for `itoa_arm`. The
permuter is still useful to bring a draft closer: it found the
`(ten = 10)` compare that gives `itoa_arm`'s `cmp r1, #10`.

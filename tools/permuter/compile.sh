#!/usr/bin/env bash
# decomp-permuter compile script for the IWRAM ARM objects (#553).
# The permuter calls it as `compile.sh in.c -o out.o`.
#
# It preprocesses with the repo's include paths (so include/match.h is
# available), compiles with agbcc_arm and string_arm.o/sprite_arm.o's
# exact flags (Makefile ARM_OBJS: -mthumb-interwork -O2
# -fomit-frame-pointer) and assembles with GNU as.
#
# It also watches for the two shapes agbcc_arm can't produce (see
# docs/matching/iwram-image.md) and copies any candidate showing one to
# $PERMUTER_HITS (default: hits/ next to the permuter dir), whatever its
# score:
#  - a register push without lr (itoa_arm's `push {r4, r5, r6}`)
#  - two or more `ldmfd sp!, {lr}` directly followed by `bx lr`
#    (LookupSpriteFrameCache's returns). The call+return peephole's
#    `ldmfd sp!, {lr}; b func` tail call doesn't count.
set -e
IN="$1"
OUT="$3"
# The repo root: this script's ../.., also when the permuter runs a copy
# of it, as long as REPO is set or the copy is a symlink.
R="${REPO:-$(cd "$(dirname "$(readlink -f "$0")")/../.." && pwd)}"
HERE=$(cd "$(dirname "$0")" && pwd)
HITS="${PERMUTER_HITS:-$HERE/../hits}"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
arm-none-eabi-cpp -I "$R/tools/agbcc/include" -iquote "$R/include" -iquote "$R/build/include" -nostdinc -undef -D NON_MATCHING=1 "$IN" >"$TMP/a.i"
"$R/tools/agbcc/bin/agbcc_arm" -mthumb-interwork -O2 -fomit-frame-pointer -o "$TMP/a.s" "$TMP/a.i" 2>/dev/null
arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork -o "$OUT" "$TMP/a.s"
if grep -E 'stmfd' "$TMP/a.s" | grep -qv 'lr'; then
    mkdir -p "$HITS"
    cp "$IN" "$HITS/nolr-$(date +%s%N).c"
fi
if [ "$(grep -A1 -E 'ldmfd[[:space:]]+sp!, \{lr\}' "$TMP/a.s" | grep -cE 'bx[[:space:]]+lr')" -ge 2 ]; then
    mkdir -p "$HITS"
    cp "$IN" "$HITS/lrpop-$(date +%s%N).c"
fi

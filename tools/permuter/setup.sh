#!/usr/bin/env bash
# setup.sh <function> <source.c> <dir>
# Creates a decomp-permuter directory for one of the IWRAM ARM functions:
#   <dir>/base.c         copied from tools/permuter/<function>/base.c
#   <dir>/settings.toml  copied from tools/permuter/<function>/settings.toml
#   <dir>/compile.sh     symlink to tools/permuter/compile.sh
#   <dir>/target.s/.o    the function's NAKED asm from <source.c>
# Then: ./permuter.py <dir> -j"$(nproc)" --stop-on-zero --better-only
set -e
FN="$1"
SRC="$2"
DIR="$3"
HERE=$(cd "$(dirname "$(readlink -f "$0")")" && pwd)
mkdir -p "$DIR"
cp "$HERE/$FN/base.c" "$HERE/$FN/settings.toml" "$DIR/"
ln -sf "$HERE/compile.sh" "$DIR/compile.sh"
python3 "$HERE/mktarget.py" "$SRC" "$FN" "$DIR/target.s"
arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork -o "$DIR/target.o" "$DIR/target.s"

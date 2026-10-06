#!/bin/sh
# Build agbcc_arm_patched: SAT-R/agbcc's ARM compiler (gcc_arm) with
# tools/agbcc_patches/agbcc_arm_prologue_return.patch applied.
#
#   tools/build_patched_agbcc_arm.sh <agbcc-source-dir> [<install-dir>]
#
# <agbcc-source-dir> is a SAT-R/agbcc checkout (built or not; it isn't
# modified). <install-dir> is the decomp repository, as for agbcc's own
# install.sh (default: this script's repository). The compiler is
# installed as <install-dir>/tools/agbcc/bin/agbcc_arm_patched, next to
# the stock agbcc, old_agbcc and agbcc_arm, which stay untouched.
#
# The patch adds two opt-in options (-mleaf-no-lr-save,
# -minterwork-return-lr). Without them the output is byte-identical to
# agbcc_arm's. The Makefile uses it for the two IWRAM objects that need
# them (PATCHED_ARM_OBJS); see docs/matching/iwram-image.md.
set -e

if [ -z "$1" ]; then
	echo "Usage: $0 <agbcc-source-dir> [<install-dir>]" >&2
	exit 1
fi

HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$(cd "$1" && pwd)
DEST=$(cd "${2:-$HERE/..}" && pwd)
PATCH="$HERE/agbcc_patches/agbcc_arm_prologue_return.patch"

if [ ! -d "$SRC/gcc_arm" ]; then
	echo "$SRC: not an agbcc checkout (no gcc_arm/)" >&2
	exit 1
fi

WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

# gcc_arm's configure reads ../config.if and its build ../include, so
# copy the whole checkout. A checkout that build.sh already built holds
# stock objects; `make clean` below drops them (the Makefile doesn't
# track arm.h, so they wouldn't be rebuilt).
cp -R "$SRC" "$WORK/agbcc"
cd "$WORK/agbcc"
patch -p1 < "$PATCH"

# The same steps as SAT-R's build.sh for agbcc_arm.
cd gcc_arm
rm -f config.status config.cache
./configure --target=arm-elf --host=i386-linux-gnu
make clean
make cc1

mkdir -p "$DEST/tools/agbcc/bin"
cp cc1 "$DEST/tools/agbcc/bin/agbcc_arm_patched"
echo "agbcc_arm_patched installed in $DEST/tools/agbcc/bin"

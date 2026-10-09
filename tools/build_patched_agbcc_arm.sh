#!/bin/sh
# Build agbcc_arm_patched: SAT-R/agbcc's ARM compiler (gcc_arm) with
# tools/agbcc_patches/agbcc_arm_prologue_return.patch applied.
#
#   tools/build_patched_agbcc_arm.sh <agbcc-source-dir> [<install-dir>]
#
# <agbcc-source-dir> is a SAT-R/agbcc checkout (built or not, with the
# patch already applied or not; it isn't modified). <install-dir> is the
# decomp repository, as for agbcc's own install.sh (default: this
# script's repository). The compiler is
# installed as <install-dir>/tools/agbcc/bin/agbcc_arm_patched, next to
# the stock agbcc, old_agbcc and agbcc_arm, which stay untouched.
#
# The patch adds four opt-in options (-mleaf-no-lr-save,
# -minterwork-return-lr, -mno-cond-return, -mstrict-cross-jump). Without
# them the output is byte-identical to agbcc_arm's. The build doesn't
# need it any more: the two IWRAM objects
# that need the options are C++, built by agbcp_arm_patched, the same
# patch on notyourav/agbcc's ARM C++ compiler (tools/build_agbccpp.sh).
# This C build of the patch stays for comparisons; see
# docs/matching/iwram-image.md ("Seventh pass", "Eighth step: C++").
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

# The 1999 configure probes are old C: on a current host gcc (C23 by
# default, implicit int an error) some of them fail to compile and give
# wrong answers (no ANSI headers, no working vfork, no printf %p). Build
# with an old C dialect and warnings off, as tools/build_agbccpp.sh does;
# -fpermissive turns implicit int and implicit declarations back into
# warnings on gcc 14 and later (older gcc doesn't accept it for C, and
# doesn't need it). gcc_arm's Makefile adds its own -std=gnu11.
HOSTCC="${CC:-gcc} -std=gnu99 -w"
if ${CC:-gcc} -fpermissive -Werror -x c -c -o /dev/null /dev/null 2>/dev/null; then
	HOSTCC="$HOSTCC -fpermissive"
fi

# gcc_arm's configure reads ../config.if and its build ../include, so
# copy the whole checkout. A checkout that build.sh already built holds
# stock objects; `make clean` below drops them (the Makefile doesn't
# track arm.h, so they wouldn't be rebuilt). A checkout that already has
# the patch applied (by hand, or an earlier build in place) is used as
# it is.
cp -R "$SRC" "$WORK/agbcc"
cd "$WORK/agbcc"
if patch -p1 -f -s --dry-run < "$PATCH" >/dev/null 2>&1; then
	patch -p1 -f < "$PATCH"
elif patch -p1 -R -f -s --dry-run < "$PATCH" >/dev/null 2>&1; then
	echo "$SRC: patch already applied"
else
	echo "$PATCH doesn't apply to $SRC" >&2
	exit 1
fi

# The same steps as SAT-R's build.sh for agbcc_arm.
cd gcc_arm
rm -f config.status config.cache
CC="$HOSTCC" ./configure --target=arm-elf --host=i386-linux-gnu
make clean
make cc1

mkdir -p "$DEST/tools/agbcc/bin"
cp cc1 "$DEST/tools/agbcc/bin/agbcc_arm_patched"
echo "agbcc_arm_patched installed in $DEST/tools/agbcc/bin"

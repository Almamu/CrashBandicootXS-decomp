#!/bin/sh
# Build agbcp and old_agbcp, the C++ compilers (cc1plus) that match
# agbcc and old_agbcc, and agbcp_arm_patched, the one that matches
# agbcc_arm_patched, from notyourav/agbcc's `cp` branch.
#
#   tools/build_agbccpp.sh <agbcc-cp-source-dir> [<install-dir>]
#
# <agbcc-cp-source-dir> is a checkout of https://github.com/notyourav/agbcc
# at branch `cp` (decomp.me's "agbccpp" is that branch's release), with
# the patches below already applied or not. It isn't modified.
# <install-dir> is the decomp repository (default: this script's
# repository). The compilers are installed as
# <install-dir>/tools/agbcc/bin/agbcp, .../old_agbcp and
# .../agbcp_arm_patched, next to agbcc.
#
# Both are the branch's g++/ tree with
# tools/agbcc_patches/agbcp_agbcc_options.patch applied, which ports
# agbcc's -fprologue-bugfix and OLD_COMPILER switches; old_agbcp is built
# with -DOLD_COMPILER, the way SAT-R/agbcc builds old_agbcc. The Makefile
# builds the C++ objects (CXX_OBJS) with them; see docs/cplusplus.md.
#
# The script also installs agbcp_arm_patched, the ARM-targeting C++
# compiler for the IWRAM image's ARM code (the Makefile's ARM_OBJS): the
# branch's g++_arm/ tree (agbcc_arm's gcc 2.9-arm-000512 with the C++
# front end) with tools/agbcc_patches/agbcc_arm_prologue_return.patch,
# the patch tools/build_patched_agbcc_arm.sh applies to agbcc's gcc_arm/,
# its paths rewritten to g++_arm/. The four options it adds are off by
# default; see docs/matching/iwram-image.md and docs/cplusplus.md.
set -e

if [ -z "$1" ]; then
	echo "Usage: $0 <agbcc-cp-source-dir> [<install-dir>]" >&2
	exit 1
fi

HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$(cd "$1" && pwd)
DEST=$(cd "${2:-$HERE/..}" && pwd)
PATCH="$HERE/agbcc_patches/agbcp_agbcc_options.patch"
ARM_PATCH="$HERE/agbcc_patches/agbcc_arm_prologue_return.patch"

if [ ! -d "$SRC/g++/cp" ] || [ ! -d "$SRC/g++_arm/cp" ]; then
	echo "$SRC: not an agbcc \`cp' checkout (no g++/cp/)" >&2
	exit 1
fi

WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

# The 1999 sources need an old C dialect and warnings off on a current
# host gcc. -fstack-reuse=none: grokdeclarator (cp/decl.c) keeps a
# pointer to a block-scoped variable past its block when it parses a
# destructor; with stack slot reuse that crashes ("Internal compiler
# error") on any class with a destructor. gcc 14 made implicit int and
# implicit declarations errors; -fpermissive turns them back into
# warnings (older gcc doesn't accept it for C, and doesn't need it).
HOSTCC="${CC:-gcc} -std=gnu99 -w -fcommon -fstack-reuse=none"
if ${CC:-gcc} -fpermissive -Werror -x c -c -o /dev/null /dev/null 2>/dev/null; then
	HOSTCC="$HOSTCC -fpermissive"
fi

# Same configure lines as the branch's build.sh.
build() { # <tree> <extra host CFLAGS> [<compiler dir> <target>]
	(cd "$1/${3:-g++}" &&
	 rm -f config.cache config.status &&
	 CC="$HOSTCC $2" ./configure --target=${4:-thumb-elf} --disable-werror \
		--with-cpu=arm7tdmi --with-no-thumb-interwork --disable-multilib \
		--enable-languages="c++" --host=i686-pc-linux --build=i686-pc-linux >/dev/null &&
	 make clean >/dev/null &&
	 make cc1plus >/dev/null)
}

cp -R "$SRC" "$WORK/src"
# A checkout that already has the patch applied is used as it is.
cd "$WORK/src"
if patch -p1 -f -s --dry-run < "$PATCH" >/dev/null 2>&1; then
	patch -p1 -f < "$PATCH"
elif patch -p1 -R -f -s --dry-run < "$PATCH" >/dev/null 2>&1; then
	echo "$SRC: patch already applied"
else
	echo "$PATCH doesn't apply to $SRC" >&2
	exit 1
fi
cd "$WORK"

cp -R "$WORK/src" "$WORK/new"
build "$WORK/new" ""

cp -R "$WORK/src" "$WORK/old"
build "$WORK/old" "-DOLD_COMPILER"

# The ARM patch is written against agbcc's gcc_arm/; g++_arm/'s
# config/arm/arm.c and arm.h are the same files.
cp -R "$SRC" "$WORK/arm"
sed 's#^\([-+][-+][-+] [ab]/\)gcc_arm/#\1g++_arm/#' "$ARM_PATCH" > "$WORK/arm.patch"
cd "$WORK/arm"
if patch -p1 -f -s --dry-run < "$WORK/arm.patch" >/dev/null 2>&1; then
	patch -p1 -f < "$WORK/arm.patch"
elif patch -p1 -R -f -s --dry-run < "$WORK/arm.patch" >/dev/null 2>&1; then
	echo "$SRC: ARM patch already applied"
else
	echo "$ARM_PATCH doesn't apply to $SRC/g++_arm" >&2
	exit 1
fi
cd "$WORK"
build "$WORK/arm" "" g++_arm arm-elf

mkdir -p "$DEST/tools/agbcc/bin"
cp "$WORK/new/g++/cc1plus" "$DEST/tools/agbcc/bin/agbcp"
cp "$WORK/old/g++/cc1plus" "$DEST/tools/agbcc/bin/old_agbcp"
cp "$WORK/arm/g++_arm/cc1plus" "$DEST/tools/agbcc/bin/agbcp_arm_patched"
echo "agbcp, old_agbcp and agbcp_arm_patched installed in $DEST/tools/agbcc/bin"

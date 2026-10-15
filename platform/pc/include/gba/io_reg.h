#ifndef GUARD_PC_GBA_IO_REG_H
#define GUARD_PC_GBA_IO_REG_H

/* The GBA's registers and their bits, with REG_BASE on the host's I/O
 * array (gHostIo) instead of 0x04000000. Every REG_* macro is relative to
 * REG_BASE, so redefining it moves them all. An integer, as on the GBA
 * (docs/platforms.md, "64-bit hosts"). */
#include "../../../gba/include/gba/io_reg.h"
#include "./host.h"

#undef REG_BASE
#define REG_BASE ((uintptr_t)gHostIo)

#endif /* GUARD_PC_GBA_IO_REG_H */

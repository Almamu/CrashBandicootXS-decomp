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

/* The game reads the keys once a frame (KeyInput::Update): on the host
 * that read is the platform's call, which plays the scripted input and
 * counts the game's frames (platform/pc/src/input.c). */
#undef REG_KEYINPUT
#define REG_KEYINPUT (HostReadKeys())

#endif /* GUARD_PC_GBA_IO_REG_H */

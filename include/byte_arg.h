#ifndef GUARD_BYTE_ARG_H
#define GUARD_BYTE_ARG_H

#include "core.h"

/* A one-byte by-value argument. The caller stores it into its stack slot
 * with `strb` and the callee reads it back with `ldrb`; a promoted `u8`
 * parameter is stored with `str` and read as a whole word. Used where the
 * ROM passes a byte this way (DrawSaveSlotStats, and the copies in
 * src/enemies/enemy_ctrl_update.cpp and src/vehicle/jetpack_spawn.cpp). */
struct byte_arg {
    u8 v;
} __attribute__((packed));

#endif /* GUARD_BYTE_ARG_H */

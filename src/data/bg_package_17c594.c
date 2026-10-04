#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817C594-0x0817C5D0: the fade overlay's three BG packages,
 * loaded by InitContinuePrompt (actor_part87.c) into its BG0/BG2/BG1 buffers.
 * Linked in ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

extern const u8 gStaticData_0861BF30[];
extern const u8 gStaticData_0861BF58[];
extern const u8 gStaticData_0861BF80[];
extern const u8 gStaticData_08628C50[];
extern const u8 gStaticData_0862A958[];
extern const u8 gStaticData_0862B34C[];
extern const u8 gStaticData_086308F0[];
extern const u8 gStaticData_08630E1C[];
extern const u8 gStaticData_08631158[];

/* BG0. */
const struct bg_package gStaticData_0817C594 = {
    0x1e,
    0x14,
    (void *)gStaticData_0861BF30,
    (void *)gStaticData_08628C50,
    (void *)gStaticData_086308F0,
};

/* BG2. */
const struct bg_package gStaticData_0817C5A8 = {
    0x1e,
    0x14,
    (void *)gStaticData_0861BF58,
    (void *)gStaticData_0862A958,
    (void *)gStaticData_08630E1C,
};

/* BG1. */
const struct bg_package gStaticData_0817C5BC = {
    0x1e,
    0x14,
    (void *)gStaticData_0861BF80,
    (void *)gStaticData_0862B34C,
    (void *)gStaticData_08631158,
};

#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0816BF20-0x0816C090: the player's per-action dispatch table and
 * the per-mode animation-row table that follows it. Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_17();
extern void nullsub_18();
extern void sub_8012D24();
extern void sub_8012FBC();
extern void sub_8013228();
extern void sub_80134B8();
extern void sub_80138E8();
extern void sub_8013994();
extern void sub_8013C60();
extern void sub_8013D94();
extern void sub_8013EAC();
extern void sub_8013FD4();
extern void sub_8014084();
extern void sub_801426C();
extern void sub_80142B0();
extern void sub_801434C();
extern void sub_80144E0();
extern void sub_8014524();
extern void sub_80145E4();
extern void sub_8014674();
extern void sub_8014940();
extern void sub_80149BC();
extern void sub_8014A3C();
extern void sub_8014AEC();
extern void sub_8014B54();
extern void sub_8014BCC();
extern void sub_8014D18();
extern void sub_8014EE0();
extern void sub_80155AC();
extern void sub_80155B8();
extern void sub_80155F8();
extern void sub_8015650();
extern void sub_8015690();
extern void sub_80156B4();
extern void sub_80156EC();
extern void sub_8015750();
extern void sub_8015774();

/* 42-slot action dispatch table: one member-function pointer per player
 * action, indexed by the action id (actor_part84.c's `struct act_pmf`
 * view; docs/rom_map.md "gStaticData_0816BF20 is a 42-slot,
 * fully-populated action dispatch table"). sub_80134B8 is the shared
 * default handler (6 slots). */
const struct actor_pmf gStaticData_0816BF20[42] = {
    ACTOR_PMF(sub_8012D24),
    ACTOR_PMF(sub_8015774),
    ACTOR_PMF(nullsub_18),
    ACTOR_PMF(sub_8012FBC),
    ACTOR_PMF(sub_8015750),
    ACTOR_PMF(sub_8013228),
    ACTOR_PMF(nullsub_17),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_80156EC),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_80138E8),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_8013994),
    ACTOR_PMF(sub_8013C60),
    ACTOR_PMF(sub_8013D94),
    ACTOR_PMF(sub_8013EAC),
    ACTOR_PMF(sub_8013FD4),
    ACTOR_PMF(sub_8014084),
    ACTOR_PMF(sub_801426C),
    ACTOR_PMF(sub_80142B0),
    ACTOR_PMF(sub_801434C),
    ACTOR_PMF(sub_80144E0),
    ACTOR_PMF(sub_8014524),
    ACTOR_PMF(sub_80145E4),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_80134B8),
    ACTOR_PMF(sub_80156B4),
    ACTOR_PMF(sub_8014674),
    ACTOR_PMF(sub_8014940),
    ACTOR_PMF(sub_8015690),
    ACTOR_PMF(sub_8015650),
    ACTOR_PMF(sub_8014A3C),
    ACTOR_PMF(sub_80155F8),
    ACTOR_PMF(sub_8014AEC),
    ACTOR_PMF(sub_80155B8),
    ACTOR_PMF(sub_8014B54),
    ACTOR_PMF(sub_8014BCC),
    ACTOR_PMF(sub_8014D18),
    ACTOR_PMF(sub_80155AC),
    ACTOR_PMF(sub_8014EE0),
    ACTOR_PMF(sub_80149BC),
};

/* The 13-level animation rows (4-byte `struct level_anim` records,
 * gStaticData_0816C0B0 in speed_table_16c090.c), one pointer per mode:
 * actor_part_16048.c reads `gStaticData_0816C070[mode][level]`. */
extern const u8 gStaticData_0816C0B0[8][13][4];

const u8 *const gStaticData_0816C070[8] = {
    gStaticData_0816C0B0[0][0],
    gStaticData_0816C0B0[1][0],
    gStaticData_0816C0B0[2][0],
    gStaticData_0816C0B0[3][0],
    gStaticData_0816C0B0[4][0],
    gStaticData_0816C0B0[5][0],
    gStaticData_0816C0B0[6][0],
    gStaticData_0816C0B0[7][0],
};

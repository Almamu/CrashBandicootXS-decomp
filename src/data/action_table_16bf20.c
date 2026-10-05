#include "core.h"
#include "actor_self.h"
#include "player.h"

/*
 * ROM 0x0816BF20-0x0816C090: the player's per-action dispatch table and
 * the per-mode animation-row table that follows it. Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* 42-slot action dispatch table: one member-function pointer per player
 * action, indexed by the action id (action_ctrl_update.c's `struct act_pmf`
 * view; docs/rom_map.md "gActionCtrlStateTable is a 42-slot,
 * fully-populated action dispatch table"). ActionCtrlStateAirborne is the shared
 * handler of the six airborne states (7 jump, 9 flip jump, 0xB high jump,
 * 0x18 body slam, 0x19 super body slam, 0x1A fall).
 *
 * The state ids, from the modes each handler sets and the player
 * animation (sprite bank 0) it plays: 0 idle, 3 run, 4 turbo run, 5 jump
 * takeoff, 8/0xA body slam start (from a jump / a flip), 0xC slide,
 * 0xD/0xE/0xF spin (ground / air / tornado), 0x10 crouch down, 0x11
 * crouch, 0x12 stand up, 0x13/0x14 crawl, 0x15 stand up from a crawl,
 * 0x16 body slam landing, 0x17 landing, 0x1B crawl stop, 0x1C just left
 * the ground, 0x1D dying, 0x1E warp out, 0x1F-0x28 hanging (grab, hang,
 * hang spin, move, stop), 0x29 warp in. Nothing sets states 1, 2, 6,
 * 0x22, 0x23, 0x24 or 0x27. */
const struct actor_pmf gActionCtrlStateTable[42] = {
    ACTOR_PMF(ActionCtrlStateIdle),
    ACTOR_PMF(sub_8015774),
    ACTOR_PMF(nullsub_18),
    ACTOR_PMF(ActionCtrlStateRun),
    ACTOR_PMF(ActionCtrlStateTurboRun),
    ACTOR_PMF(ActionCtrlStateJump),
    ACTOR_PMF(nullsub_17),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateBodySlamStart),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateFlipBodySlamStart),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateSlide),
    ACTOR_PMF(ActionCtrlStateSpin),
    ACTOR_PMF(ActionCtrlStateAirSpin),
    ACTOR_PMF(ActionCtrlStateTornadoSpin),
    ACTOR_PMF(ActionCtrlStateCrouchDown),
    ACTOR_PMF(ActionCtrlStateCrouch),
    ACTOR_PMF(ActionCtrlStateStandUp),
    ACTOR_PMF(ActionCtrlStateCrawlStart),
    ACTOR_PMF(ActionCtrlStateCrawl),
    ACTOR_PMF(ActionCtrlStateCrawlStandUp),
    ACTOR_PMF(ActionCtrlStateBodySlamLand),
    ACTOR_PMF(ActionCtrlStateLand),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateAirborne),
    ACTOR_PMF(ActionCtrlStateCrawlStop),
    ACTOR_PMF(ActionCtrlStateLeftGround),
    ACTOR_PMF(ActionCtrlStateDying),
    ACTOR_PMF(ActionCtrlStateWarpOut),
    ACTOR_PMF(ActionCtrlStateHangGrab),
    ACTOR_PMF(ActionCtrlStateHang),
    ACTOR_PMF(ActionCtrlStateHangSpin),
    ACTOR_PMF(sub_8014AEC),
    ACTOR_PMF(sub_80155B8),
    ACTOR_PMF(ActionCtrlReleaseHang),
    ACTOR_PMF(ActionCtrlStateHangMoveStart),
    ACTOR_PMF(ActionCtrlStateHangMove),
    ACTOR_PMF(sub_80155AC),
    ACTOR_PMF(ActionCtrlStateHangStop),
    ACTOR_PMF(ActionCtrlStateWarpIn),
};

/* The 13-level animation rows (4-byte `struct level_anim` records,
 * gPlayerCtrlModeLevelAnims in speed_table_16c090.c), one pointer per mode:
 * swim_ctrl.c reads `gPlayerCtrlModeAnimRows[mode][level]`. */
const struct level_anim *const gPlayerCtrlModeAnimRows[8] = {
    gPlayerCtrlModeLevelAnims[0],
    gPlayerCtrlModeLevelAnims[1],
    gPlayerCtrlModeLevelAnims[2],
    gPlayerCtrlModeLevelAnims[3],
    gPlayerCtrlModeLevelAnims[4],
    gPlayerCtrlModeLevelAnims[5],
    gPlayerCtrlModeLevelAnims[6],
    gPlayerCtrlModeLevelAnims[7],
};

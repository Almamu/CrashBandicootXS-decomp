#include "action_ctrl.hpp"

/*
 * ROM 0x0816BF20-0x0816C090: the player's per-action dispatch table and
 * the per-mode animation-row table that follows it. Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* The action controller's state methods, one per player action, indexed
 * by the action id (gActionCtrlStateTable; ActionCtrl::Update,
 * action_ctrl_update.cpp, dispatches through it; docs/rom_map.md
 * "gActionCtrlStateTable is a 42-slot, fully-populated action dispatch
 * table"). Each non-virtual `&ActionCtrl::f` is g++'s {0, -1, f} record.
 * StateAirborne is the shared handler of the six airborne states (7 jump,
 * 9 flip jump, 0xB high jump, 0x18 body slam, 0x19 super body slam, 0x1A
 * fall).
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
const ActionCtrl::StateFunc ActionCtrl::stateFuncs[ACTION_STATE_COUNT] = {
    &ActionCtrl::StateIdle,
    &ActionCtrl::StateUnusedIdle,
    &ActionCtrl::StateNop2,
    &ActionCtrl::StateRun,
    &ActionCtrl::StateTurboRun,
    &ActionCtrl::StateJump,
    &ActionCtrl::StateNop6,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateBodySlamStart,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateFlipBodySlamStart,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateSlide,
    &ActionCtrl::StateSpin,
    &ActionCtrl::StateAirSpin,
    &ActionCtrl::StateTornadoSpin,
    &ActionCtrl::StateCrouchDown,
    &ActionCtrl::StateCrouch,
    &ActionCtrl::StateStandUp,
    &ActionCtrl::StateCrawlStart,
    &ActionCtrl::StateCrawl,
    &ActionCtrl::StateCrawlStandUp,
    &ActionCtrl::StateBodySlamLand,
    &ActionCtrl::StateLand,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateAirborne,
    &ActionCtrl::StateCrawlStop,
    &ActionCtrl::StateLeftGround,
    &ActionCtrl::StateDying,
    &ActionCtrl::StateWarpOut,
    &ActionCtrl::StateHangGrab,
    &ActionCtrl::StateHang,
    &ActionCtrl::StateHangSpin,
    &ActionCtrl::StateUnusedHang,
    &ActionCtrl::StateUnusedHangGrab,
    &ActionCtrl::ReleaseHang,
    &ActionCtrl::StateHangMoveStart,
    &ActionCtrl::StateHangMove,
    &ActionCtrl::StateUnusedHangRelease,
    &ActionCtrl::StateHangStop,
    &ActionCtrl::StateWarpIn,
};

/* The 13-level animation rows (4-byte `struct level_anim` records,
 * gSwimCtrlModeLevelAnims in speed_table_16c090.c), one pointer per mode:
 * swim_ctrl.cpp reads `gSwimCtrlModeAnimRows[mode][level]`. C linkage:
 * player.h declares it. */
const struct level_anim *const gSwimCtrlModeAnimRows[8] = {
    gSwimCtrlModeLevelAnims[0], gSwimCtrlModeLevelAnims[1], gSwimCtrlModeLevelAnims[2],
    gSwimCtrlModeLevelAnims[3], gSwimCtrlModeLevelAnims[4], gSwimCtrlModeLevelAnims[5],
    gSwimCtrlModeLevelAnims[6], gSwimCtrlModeLevelAnims[7],
};

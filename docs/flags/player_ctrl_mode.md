# `Player::ctrlMode`: the player's controller (#843)

`Player::ctrlMode` (+0x88) says which controller drives the player, so
which kind of room it is in. The names, `PLAYER_CTRL_MODE_*`, sit in
include/player.hpp next to the class.

## Evidence

LevelProgress::PlayRoom (play_room.cpp) switches on the room kind
(`ROOM_KIND_*`, constants/rooms.h), builds that kind's controller, and
sets the mode:

| Room kind | Controller | `ctrlMode` |
|---|---|---|
| `ROOM_KIND_ON_FOOT` (0) | `new ActionCtrl` | the kind, 0 |
| `ROOM_KIND_UNDERWATER` (1) | `new SwimCtrl` (gSwimCtrl) | the kind, 1 |
| `ROOM_KIND_HOVER` (2) | `new InputCtrl` | 3 |

Player::ResetForRoom switches on the mode and restarts the controller:
0 `ActionCtrl::Restart`, 1 `SwimCtrl::Restart`, 3 `InputCtrl::Restart`,
and 2 is an empty case. Player::Reset sets 0.

| Value | Name | Meaning |
|---|---|---|
| 0 | `PLAYER_CTRL_MODE_ACTION` | on foot, the action controller |
| 1 | `PLAYER_CTRL_MODE_SWIM` | underwater, the swim controller |
| 2 | `PLAYER_CTRL_MODE_UNK2` | an empty case in ResetForRoom; nothing sets it |
| 3 | `PLAYER_CTRL_MODE_INPUT` | the hover levels, the input controller |

Mode 2 is `_UNK2`: no room kind maps to it (the hover kind, 2, gives
mode 3), and no code sets it.

## What the code does with it

- On foot only (`== PLAYER_CTRL_MODE_ACTION`): the player's ring of
  touched crates (`list`, `listCount`) records (Player::IncrementListCount,
  GetListEntry, StoreListEntry; crate_break.cpp's RingPush and the stack
  break; crate_states.cpp's RingAt); SpawnRoomExit puts down the exit
  pad.
- Underwater (`== PLAYER_CTRL_MODE_SWIM`): crates fall at a quarter of
  the speed (Crate::UpdateFall); a touched enemy just vanishes in a puff
  (EnemyCtrl::HandleEvent); Crate::QueuePlayerCollision and
  ApplyCollision resolve the crate sides another way (the vertical push,
  `r += 2`, lighting a TNT on a side touch); the crate grid's collision
  takes the player as idle, or spinning when its `kind` is
  EVENT_ATTACK_SPIN, instead of the controller's state
  (crate_list_collide.cpp); an Aku Aku mask (EVENT_MASK_GAIN) raises the
  mask level to 2 at most, never to `MASK_LEVEL_INVINCIBLE`
  (Player::HandleEvent).
- Hovering (`== PLAYER_CTRL_MODE_INPUT`): the crate grid breaks every
  crate the player touches (crate_list_collide.cpp); the wumpa fruit and
  extra lives can be picked up at once, early in their hop
  (Wumpa::CheckPickup, ExtraLife::CheckPickup).

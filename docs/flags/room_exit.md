# Room exit statuses (#843)

How a room ended: what LevelProgress::PlayRoom (through RunRoom) and
InitActorCategory return. LevelState::UpdateGameFrame keeps it in its
local `status`. The names sit in engine/include/constants/rooms.h, next to
`ROOM_KIND_*`.

They are not the `CATEGORY_EXIT_*` values (categories.h). Those are what
RunActorCategoryFrame returns to InitActorCategory's loop each frame
(none, cleared, boss death, death). InitActorCategory turns them into one
of the statuses below.

| Value | Name | Returned by | UpdateGameFrame |
|---|---|---|---|
| 0 | `ROOM_EXIT_CLEARED` | RunRoom when the room's exit was requested (RequestRoomExit); InitActorCategory on `CATEGORY_EXIT_CLEARED` (and on a pause result of 3, which no pause row has) | ends a bonus round or gem path as won (`EndBonusRound(status == ROOM_EXIT_CLEARED)`); otherwise NextRoom. After a boss level, the power and its cutscene |
| 1 | `ROOM_EXIT_RETRY` | RunRoom when the player is gone, or on the pause menu's restart (row type 1, the fifth row, shown only in a time trial: "restart trial"); InitActorCategory after a death, and on that restart | RestoreCheckpoint; with no lives left, the continue prompt; in a time trial, back to the level's first room |
| 2 | `ROOM_EXIT_QUIT` | RunRoom and InitActorCategory on the pause menu's quit (row type 2) | leaves the room loop for the level select; the mask level goes back to `best`, the level start's, when that is lower |

UpdateGameFrame starts `status` at `ROOM_EXIT_RETRY` before the room loop.

## Left as numbers

- RunRoom's `s32 ret = 1`: the ROM stores the same 1 as the camera's mode
  (`gCamera->mode = ret`), so it stays a number, with a comment.
- The pause menu's results are `PAUSE_ROW_TYPE_*` (menus.hpp,
  [pause_menu.md](pause_menu.md)), not room exit statuses.

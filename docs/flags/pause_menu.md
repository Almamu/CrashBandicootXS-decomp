# The pause menu's row types (#843)

`struct pause_row.type` (menus.hpp): what a row of the pause menu does.
`gPauseMenuRows` (src/game/ui/data/pause_menu_rows.cpp) has five rows;
PauseMenu's constructor shows the fifth only in a time trial
(`rowCount` 5, else 4). PauseMenu::Loop returns the type of the row A
confirms, or `PAUSE_ROW_TYPE_RESUME` on START, and RunPauseMenu passes it
on. The names, `PAUSE_ROW_TYPE_*`, sit next to the struct.

| Value | Name | Row (label id) | What happens |
|---|---|---|---|
| 0 | `PAUSE_ROW_TYPE_RESUME` | 0 (0x32) | RunRoom resumes the room (ResumeRoomAfterPause); InitActorCategory carries on |
| 1 | `PAUSE_ROW_TYPE_RESTART` | 4 (0x34), time trial only | RunRoom and InitActorCategory end with `ROOM_EXIT_RETRY` |
| 2 | `PAUSE_ROW_TYPE_QUIT` | 3 (0x33) | `ROOM_EXIT_QUIT`, back to the level select |
| 3 | `PAUSE_ROW_TYPE_UNK3` | none | InitActorCategory would end with `ROOM_EXIT_CLEARED`; no row has it, so it is `_UNK3` |
| 4 | `PAUSE_ROW_TYPE_MUSIC` | 1 (0x30) | left/right: VolumeDown/VolumeUp on the music volume; DrawRows draws its " <NN%>" text. A only plays SFX_MENU_ERROR |
| 5 | `PAUSE_ROW_TYPE_SOUND` | 2 (0x31) | the same for the sound effects (with a cue) |

Loop's volume-row test is one unsigned range check, `(u32)(result -
PAUSE_ROW_TYPE_MUSIC) <= PAUSE_ROW_TYPE_SOUND - PAUSE_ROW_TYPE_MUSIC`, as
the ROM has it.

The room exit statuses those results become are in
[room_exit.md](room_exit.md).

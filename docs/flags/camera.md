# `Camera::mode` (#843)

`mode` (+0x14 of gCamera, engine/include/game/level/camera.hpp) picks how Camera::Update
moves the look-ahead (`vx`, `vy`) before it eases the position a quarter
of the way to the target plus the look-ahead and publishes the scroll.
The names sit next to the class.

| Value | Name | Set by | Meaning |
|---|---|---|---|
| 1 | `CAMERA_MODE_FACING` | LevelProgress::RunRoom, for every room | StepFacing: the look-ahead grows to ±0x1276 on the side the target faces (its mirror bit), 0x10 px up. Snap seeds it at that limit. The hover ride keeps it, with the camera lead (CameraLead) as the target |
| 2 | `CAMERA_MODE_DIRECTIONAL` | RunRoom, in an underwater room (`ROOM_KIND_UNDERWATER`) | StepDirectional: the look-ahead follows the player's `dir` bits (`PART_DIR_*`) in 0x100 steps, and decays on an axis with neither bit set. Snap starts it at 0 |
| 3 | `CAMERA_MODE_STILL` | ActionCtrl::HandleEvent, on a plain hit (`EVENT_HIT`, `EVENT_HIT_EXPLOSION`, `EVENT_HIT_BITE`) | no step: the camera stops where it is while the player dies. Update's empty `case` keeps the ROM's switch shape |

`new Camera` (PlayRoom) leaves `mode` unset; 0 has no meaning.

RunRoom stores the 1 through its `ret` local, the function's
return value, which starts at 1 (`s32 ret = 1; gCamera->mode = ret;`). That
store stays as it is: the one constant serves both.

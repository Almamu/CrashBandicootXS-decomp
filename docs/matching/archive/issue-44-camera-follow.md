# Issue #44: 0x08026C80-0x08026EEC, game_loop - the camera-follow block

`sub_8026C80`/`sub_8026C8C` were already matched (`terrain.c`) before
this pass. The remaining eight functions - the whole of the former
`asm/code_3_2_17_26bf8.s` - are now all byte-exact matched in the new
`src/level/camera.c`. Nothing parked, nothing left raw.

## What the block is

`gCamera` (the "generic 0x18-byte block"
`docs/matching/archive/issue-37-game-loop-2375c.md` saw flushed via
`SnapCamera`) is a camera follower:

| offset | field | meaning |
|---|---|---|
| 0x00/0x04 | `x`/`y` | Q8 camera position |
| 0x08/0x0C | `vx`/`vy` | Q8 look-ahead offset added to the target position |
| 0x10 | `target` | followed object (its own `+0x0`/`+0x4` are a Q8 position) |
| 0x14 | `mode` | 1 = `StepCameraFacing`, 2 = `StepCameraDirectional`, anything else = hold |

Both publishers pass `(x - (120 << 8), y - (80 << 8))` - half the
240x160 screen - to `SetLevelScroll(gLevelLayers, ...)`. That function
is still raw (`asm/code_3_2_17_266bc.s`), but it's short enough to read
directly: it clamps both coordinates to `>= 0`, shifts Q8 down to pixels,
caps each at the object's own `+0x0`/`+0x4`, and stores them at
`+0x8`/`+0xC`. So the target is kept centered on screen, clamped to the
level bounds.

- **`StepCameraDirectional`** (mode 2) - `target+0x24` direction bits steer the
  look-ahead in 0x100 steps: bit 0/1 push `vx` toward +0x27FF/-0x2800,
  bit 2/3 push `vy` toward -0x1AAA/+0x1AA9. An axis with neither of its
  bits set decays toward 0 by the same step. The position then eases a
  quarter of the way to `target + look-ahead`. What writes
  `target+0x24` wasn't traced here, so the field is named only by what
  this function does with it (`dirFlags`).
- **`StepCameraFacing`** (mode 1) - `vx` grows toward -0x1276 or +0x1276
  depending on `target+0x28` bit 4 (the mirror flag several actor-side
  write-ups already document at that offset), `vy` is fixed at -0x1000,
  same quarter-step easing.
- **`SnapCamera`** - snap: copies the target position, seeds the mode-1
  look-ahead at its limit (zero for any other mode), adds it, publishes.
  Callers: `ResumeRoomAfterPause` (`room.c`) and `PlayRoom`'s tail
  (`run_room.c`).
- **`UpdateCamera`** - the per-frame update: dispatch on `mode`, then
  publish. Caller: `UpdateRoomFrame` (`room_frame.c`).
- **`OperatorDeleteArray`/`OperatorDelete`** - `mem_free(ptr)` wrappers;
  **`OperatorNewArray`/`OperatorNew`** - `mem_alloc(size, MEM_HEAP_EWRAM)`
  wrappers. Two byte-identical pairs, each with its own set of callers.

Function names stay `sub_XXXXXXXX` (see `docs/naming.md`) - the
camera reading is solid, but which of mode 1/mode 2 corresponds to which
gameplay situation isn't established, so the file and struct are named
and the functions aren't.

## Matching notes

- **`target+0x28` bit 4** needs the `(flags << 27) < 0` sign-test form
  (the same idiom `player_anim_room.c` uses for this flag) to get the ROM's
  `lsls #27` / `bge`; `flags & 0x10` and a 1-bit unsigned bitfield both
  compile to `movs #0x10` / `ands` / `beq`.
- **`tx`/`ty` pinned to r2/r3** in `StepCameraDirectional`/`StepCameraFacing`. Unpinned,
  the allocator puts `cam` in the low register and `tx`/`ty` in r3/r4
  (r4/r5 in `StepCameraDirectional`). Tried without success: a separate target
  local, split declaration/assignment, `void *` parameter re-typed into a
  local, `register` without a pin, fused/split easing expressions,
  `old_agbcc`. Pinning `cam` to r4 instead fixes the prologue but loses
  the shared `adds r0, rX, r5` tail for the two `+-0x100` look-ahead
  steps.
- **Easing tail in `StepCameraDirectional`** - with the pins in place, `cam->x +=
  (tx - cam->x) / 4` put the second axis's `cam->y` temp in the freed r2
  instead of the ROM's r4. An r4-pinned `cur` fixes the register; writing
  the sum into a separate `n` (rather than `cam->x = cur + ...` directly,
  which reused r4 as the destination, or `(tx - cur) / 4 + cur`, which
  swapped the operands) gives the ROM's `adds r0, r4, r0`.
  `StepCameraFacing`'s tail matches as plain `+=` without this.
- **`UpdateCamera`'s switch** - the ROM tests `== 2`, then `> 2`, then
  `!= 1`: gcc's balanced decision tree for three or more cases. A plain
  `case 1`/`case 2` switch (in either order), `default:`, a `case 0`, or
  an if-chain all give different shapes; adding an empty `case 3: break;`
  reproduces it exactly. The empty case has no behavior.
- **Trailing `asm(".align 2, 0")`** - without it the file ends with a
  `46C0` nop pad where the ROM has `0000` (caught by the full build,
  `cmp` showed exactly 2 bytes at `0x08026EEA`).

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: OK`).

# The extra life's and the wumpa's `state`, `mode` and `phase` (#843)

ExtraLife and Wumpa (include/pickups.hpp; extra_life.cpp, wumpa.cpp,
wumpa_update.cpp) have the same three bytes after Sprite's fields:
`state` (+0x48), `mode` (+0x4A) and `phase` (+0x4B). The names sit in
include/pickups.hpp: `EXTRA_LIFE_STATE_*` and `WUMPA_STATE_*` next to each
class, and the shared `PICKUP_MODE_*`, `WUMPA_MODE_PAYOUT` and
`PICKUP_PHASE_*` before ExtraLife.

## `state`

Reset (the constructor) sets 0. CheckPlayerContact calls CheckPickup only
in state 0, and Update's chain runs each state's step.

| Value | ExtraLife | Wumpa | Entered by |
|---|---|---|---|
| 0 | `EXTRA_LIFE_STATE_IDLE` | `WUMPA_STATE_IDLE` | Reset: on the spot, it bobs or hops (`mode`) |
| 1 | `EXTRA_LIFE_STATE_TO_HUD` | `WUMPA_STATE_TO_HUD` | PickUp(0) (touched by the player) and SendToHud: flies to the HUD's lives/wumpa counter, which adds a life (AddLife) or collects the wumpa (CollectWumpa) when it gets there |
| 2 | `EXTRA_LIFE_STATE_FLY_OFF` | `WUMPA_STATE_FLY_OFF` | PickUp(1) (a crate blast, or the wumpa hit by the spin attack): flies to a random point off the screen, `affine` shrinking (`counter` 0) or growing, then gone |
| 3 | | `WUMPA_STATE_PAYOUT` | StartPayout (`mode` 0xFF): stays at the player and drops a wumpa flying to the HUD every 11 frames, 10 in all (`phase` counts them), then gone |

## `mode`

How the pickup moves on the spot (state 0). UpdateHop moves a hop's x by
`gExtraLifeHopWidths`/`gWumpaHopWidths[mode - 1]` (0x300, 0, 0x200).

| Value | Name | Meaning |
|---|---|---|
| 0 | `PICKUP_MODE_BOB` | bobs: `counter` steps a sine (Create sets it) |
| 1 | `PICKUP_MODE_HOP_LEFT` | hops to the left (`anchor.x - sn`, width 0x300) |
| 2 | `PICKUP_MODE_HOP_RIGHT` | hops to the right (`anchor.x + sn`); its width is 0, so it hops in place |
| 3 | `PICKUP_MODE_HOP_UP` | hops in place (UpdateHop's `else`) |
| 0xFF | `WUMPA_MODE_PAYOUT` | the wumpa only: SetHop, Create and DropWumpa start the payout (StartPayout) |

The crates drop their pickups with these: the basic crate and the extra
life crate a `HOP_UP`, the "?" crate a fan of `HOP_LEFT`/`HOP_RIGHT`/`HOP_UP`
wumpas, and its single wumpa (contents 10) a `WUMPA_MODE_PAYOUT`.

SendToHud reads `mode` as a pixel offset to the left instead. A pickup
dropped with `toHud` set is already on its way to the HUD, so its `mode`
never hops: the bounce crate's `DropWumpa(..., 0xe, true)` and
`DropWumpa(..., 0, true)` and the payout's `DropWumpa(..., 0, 0, true)` stay
numbers, with a comment.

## `phase`

`phase` is the hop's step (UpdateHop's sine index), not a state. Update
counts it up while hopping and, past `PICKUP_PHASE_LAST` (0x1F), sets
`mode` back to `PICKUP_MODE_BOB`. Up to `PICKUP_PHASE_NO_PICKUP` (0x16) a
hopping pickup can't be picked up, unless the player is on a hover level
(`PLAYER_CTRL_MODE_INPUT`). In the payout, `phase` counts the wumpas
dropped.

## Not named here

- The payout's counts (`counter` 0xA, `++counter > 10`, `phase > 9`).
- `counter`, the bob's sine index and the fly-off's way.
- The `0` stores to `phase` (the hop's start).
- DropExtraLife's local `state` (the time-trial flag) and Hud::ShowWumpa's
  `slotState`.

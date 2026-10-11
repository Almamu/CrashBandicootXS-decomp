# The hit events: what a touched object sends (#843)

A touched object sends its own `kind` (Entity::kind) as the event to the
player's event method (include/constants/events.h), so the `EVENT_HIT_*`
values are also the kinds of the hazards. Hits 1-10 take a mask away or
kill the player; each controller picks the death animation.

Two of them have one sender each and no other evidence of what kind of
hit they are, so they are named after the sender:

| Value | Name | Sender | Handled by |
|---|---|---|---|
| 5 | `EVENT_HIT_PUFFERFISH` | SpawnPufferfish (`part->kind`) | Player::HandleEvent's hit cases; SwimCtrl::HandleEvent's death animation 0x2D (the one ActionCtrl uses for Neo Cortex's shot) |
| 7 | `EVENT_HIT_FROG` | SpawnFrog | Player::HandleEvent's hit cases; ActionCtrl::HandleEvent's death animation 0x2B |

The other hits (`EVENT_HIT`, `_FIRE`, `_ELECTRIC`, `_EXPLOSION`, `_BITE`,
`_DART`, `_CORTEX_SHOT`, `_CRUSH`) were already named; events.h lists
their senders.

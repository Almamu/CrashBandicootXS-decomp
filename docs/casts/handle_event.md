# HandleEvent's `from`: an int (#855)

The event methods take `(from, event, arg)`. MovingSprite's and Player's
HandleEvent (sprite_obj.hpp, player.hpp) declared `from` as `s32`, and
the controllers' (Ctrl and its subclasses) as `MovingSprite *sender`. So
the sprites cast it at each forward to their controller,
`mover->HandleEvent((MovingSprite *)from, event, arg)`: 8 casts in
Player::HandleEvent and 1 in MovingSprite::HandleEvent.

Every caller passes a literal 0 or 1 (`part->HandleEvent(1, kind, 0)`,
`gPlayer->HandleEvent(0, EVENT_BUMP, dir)`), never a sprite, and no
handler reads it. It is an int, so the controllers' parameter is now
`s32 from` too, and the 9 casts go. Typing it `MovingSprite *` instead
would need a `(MovingSprite *)1` at the callers that pass 1.

| method | mangled name was | is |
|---|---|---|
| Ctrl::HandleEvent (CtrlHandleEvent) | `HandleEvent__4CtrlP12MovingSpriteii` | `HandleEvent__4Ctrliii` |
| EffectCtrl::HandleEvent | `HandleEvent__10EffectCtrlP12MovingSpriteii` | `HandleEvent__10EffectCtrliii` |
| BossCtrl::HandleEvent | `HandleEvent__8BossCtrlP12MovingSpriteii` | `HandleEvent__8BossCtrliii` |
| InputCtrl::HandleEvent | `HandleEvent__9InputCtrlP12MovingSpriteii` | `HandleEvent__9InputCtrliii` |
| EnemyCtrl::HandleEvent (HitEnemy) | `HandleEvent__9EnemyCtrlP12MovingSpriteii` | `HandleEvent__9EnemyCtrliii` |
| SwimCtrl::HandleEvent | `HandleEvent__8SwimCtrlP12MovingSpriteii` | `HandleEvent__8SwimCtrliii` |
| ActionCtrl::HandleEvent | `HandleEvent__10ActionCtrlP12MovingSpriteii` | `HandleEvent__10ActionCtrliii` |

cxx_symbols.txt maps the new names onto the same C names. Both types
are a word in r1, so every object is the same.

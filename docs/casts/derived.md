# Derived-class casts: the `AsX()` helpers (#855)

Where a base-class pointer is turned into a subclass pointer the same way
in many places, the base class has a small inline helper that says so.
Each one is `return (T *)this;`. It is declared in the base class and
defined in the subclass's header, right after the subclass, because
`(T *)this` needs the complete class to be a real downcast.

| helper | declared in | defined in | replaces |
|---|---|---|---|
| `Sprite::AsMovingSprite()` | sprite_obj.hpp | sprite_obj.hpp, after MovingSprite | `(MovingSprite *)gTouchableList->items[i]` and `gCollidableList`'s (the bosses, the player's shockwave), PartList::Collide's sprite |
| `MovingSprite::AsPlayer()` | sprite_obj.hpp | player.hpp, after Player | the controllers' `(Player *)owner` (ActionCtrl, SwimCtrl, InputCtrl::Attach) |
| `ActorSelf::AsHpActor()` | actor_self.hpp | actor_self.hpp, after HpActor | `(HpActor *)gActorList` (the jetpack enemies' hits on the player), `(HpActor *)n->next` (FindShotTarget) |
| `ActorSelf::AsJetpackPlayer()` | actor_self.hpp | vehicle.hpp, after JetpackPlayer | `((JetpackPlayer *)gActorList)->...`: the actor list's root on the jetpack stages |
| `ActorSelf::AsPolarPlayer()` | actor_self.hpp | vehicle.hpp, after PolarPlayer | `((PolarPlayer *)gActorList)->...`: the polar stages' |

Six jetpack files had their own copies of the same cast, a file-local
`PlayerActor()` (`HpActor *`) or `Player()` (`JetpackPlayer *`). They are
gone, and their call sites use `gActorList->AsHpActor()` and
`gActorList->AsJetpackPlayer()`.

An inline's `this` is its own pseudo, so a helper can change register
allocation where the cast didn't. One site keeps the cast, with a
comment: TinyCtrl::Update's `(MovingSprite *)gTouchableList->items[stomped]`.
Through the helper, the item is loaded into r0 and copied to r4, and the
ROM loads it into r4 directly. Every other site is the same.

## Left

- Single casts that don't recur: `(Crate *)e` (ConvertCratesForTimeTrial),
  `(Wumpa *)e` and `(Wumpa *)gTouchableList->items[i]` (StartTimeTrial,
  Crate::BlastNearby), `(Sprite *)e` (AddUpdateOnly).
- `MovingSprite::mover` as each controller (Player::ResetForRoom,
  Platform::Mover): one cast per controller.

## LinkSession's player records

ResetState wrote the players' ids through
`((LinkPlayer *)((u8 *)this + 8))[i + 1]`, players[i] indexed from the
record before it, then (#855) `(players - 1)[i + 1]`, the same address
without the cast. Since #908 it is `(players + i)->id`, the same code:
`players[i].id` expands with `self + 0xd0` as a term of its own, which
loop hoists out of the outer loop, and a `Player(i)` accessor or a
record pointer local is its own pseudo. The comment in
link_session_reset.cpp has the shapes tried.

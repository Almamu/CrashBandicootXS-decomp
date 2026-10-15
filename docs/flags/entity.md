# `Entity::f`: the entity flags (#843)

Every entity (`class Entity`, engine/include/game/entities/entity.hpp) has two flag bytes at
+0x0C and +0x0D, `union EntityFlags f` (engine/include/engine/actor/actor.h). The code reaches
them in three ways:

- `f.b.<bit>`: the bitfields, one per bit;
- `f.flags` (or `f.bytes.flags`): the byte at +0x0C;
- `f.bytes.flags2`: the byte at +0x0D.

The names for whole-byte access sit next to the union. `ENTITY_BIT_*` and
`ENTITY_BIT2_*` are the bit numbers in each byte; `ENTITY_FLAG_*` and
`ENTITY_FLAG2_*` are the masks. They replace sprite_flags.h's `PART_FLAG_GONE`
and `PART_FLAG_TOUCHED`.

## +0x0C, `f.flags`

| Bit | Mask | Name | Bitfield | Accessors | Set / cleared by | Read by |
|---|---|---|---|---|---|---|
| 0 | 0x01 | `GONE` | `gone` | MarkGone, ClearGone, IsGone | MarkEntityGone and its inline copies (entity_bits.h) | the part and crate lists drop it; RunRoom's loop ends when the player is gone |
| 1 | 0x02 | `ATTACKS` | `attacks` | Get/Set/ClearFlag1 | only Player::Reset sets it | Player::TouchPlayer |
| 2 | 0x04 | `CONTACT` | `visible` | Is/Enable/DisableContact | 30 `f.b.visible = 0` and 3 `= 1`; EnemyCtrl's effect part and Tiny set or clear it as a mask | CheckPlayerContact, PartList::Collide(Class), CollidePart, MovingSprite::TouchPlayer; the pickups read it from `f.flags << 24` |
| 3 | 0x08 | `TOUCHED` | `bit3` | IsTouched, Set/ClearTouched | CrateList and PartList::CollideWithPlayer (`\|= ENTITY_FLAG_TOUCHED`); SetTargetAnim clears it | TargetHit, EffectCtrl::Update, the pickups |
| 4 | 0x10 | `ALWAYS_ACTIVE` | `active` | Is/Set/ClearAlwaysActive | the spawners of the bosses, seals, the seal spawner, stopwatch and extra life; crates that drop or explode; PlayRoom for the player | IsNearCamera, IsInsideRect (the camera tests are skipped), CrateList::Update/Unlink |
| 5 | 0x20 | `UNK5` | `unk_5` | Get/Set/ClearFlag5 (GroundSprite) | GroundSprite::CheckPlayerContact sets it when its floor hit has no edge terrain under it, and clears it on each floor hit | only GetFlag5; what it is for isn't known |
| 6 | 0x40 | `VULNERABLE` | `vulnerable` | Sprite::IsVulnerable | 19 bitfield writes; Crate::QueuePlayerCollision sets it on the player; masks (`~ENTITY_FLAG_VULNERABLE`) in the enemy spawners, EnemyCtrl and Tiny | MovingSprite::ClassifyContact, Crate::Explode and QueuePlayerCollision (the player), Player::HandleEvent, DoSuperBodySlamShockwave, Dingodile's projectiles |
| 7 | 0x80 | `COLLIDES` | `collides` | Is/Enable/DisableCollision | `&= ~ENTITY_FLAG_COLLIDES` in all the enemy spawners, ActionCtrl::Update (fell out of the level), RunRoom and EVENT_WARP_EXIT (warping out); StateWarpOut sets it | `f.flags >> ENTITY_BIT_COLLIDES` (the top bit) gates the terrain probes: GroundSprite, Player::CheckPlayerContact, the crates, the launch pad, the platform mover, the pickups |

## +0x0D, `f.bytes.flags2`

| Bit | Mask | Name | Bitfield | Accessors | Set / cleared by | Read by |
|---|---|---|---|---|---|---|
| 0 | 0x01 | `FLOOR_PROBE` | `floorProbe` | Enable/DisableFloorProbe, IsFloorProbeEnabled | ActionCtrl's airborne states (ActOrFlags0D); the event and jump states, and the high jump and the air spin out of the spin, slide, crouch and crawl states, clear it (`ActAndFlags0D(part, ~ENTITY_FLAG2_FLOOR_PROBE)`) | GroundSprite::ProbeTerrainAxes |
| 1 | 0x02 | `GROUNDED` | `grounded` | IsGrounded | ProbeFloor; ActionCtrl::HandleEvent and the same jump starts clear it (`ActAndFlags0D(part, ~ENTITY_FLAG2_GROUNDED)`) | GroundSprite::CheckPlayerContact, ProbeFloor |
| 2 | 0x04 | `HIDDEN` | `blink` | Is/ToggleHidden | blinking parts toggle it | Sprite::IsOnScreen, OverlapsRect, CameraLead::Reset |
| 3 | 0x08 | `SOLID` | `solid` | Is/Set/ClearSolid | | CollideWithPlayer's solid branch (CrateList, PartList), MovingSprite::TouchPlayer |
| 4 | 0x10 | `EXIT_MIRROR` | `exitMirror` | Platform::Get/SetExitMirror | | Platform::GetExitMirror |

## How the code tests the bits

Each site keeps the spelling the ROM's code needs:

- **Entity's inline accessors:** IsGone, GetFlag1, IsContactEnabled,
  IsTouched and IsAlwaysActive, which are `(f.flags >> n) & 1` or `f.flags &
  1`. They compile like the spelled-out test, so the sites that read one of
  these bits call them.
- **The spelled-out test with the name:** `(f.flags >> ENTITY_BIT_VULNERABLE)
  & 1`, `(f.bytes.flags2 >> ENTITY_BIT2_SOLID) & 1`. These bits have no
  inline accessor on Entity: their accessors (Sprite::IsVulnerable,
  IsSolid, IsHidden, GroundSprite::IsGrounded) are out-of-line functions in
  the ROM.
- **The top bit:** `f.flags >> ENTITY_BIT_COLLIDES`.
- **Masks:** `|= ENTITY_FLAG_ALWAYS_ACTIVE`, `&= ~ENTITY_FLAG_COLLIDES`. A
  `~` mask is an `int` (0xFFFFFF7F where the code had 0x7f), and every
  object stays the same. The register-built masks (`s32 m =
  ~ENTITY_FLAG_CONTACT; m &= ...`) keep their order.
- **The pickups' `u32 flags = f.flags << 24`:** `(flags >> (24 +
  ENTITY_BIT_TOUCHED)) & 1`.

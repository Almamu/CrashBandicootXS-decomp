# Pointer parameters, fields and tables with their real types (#855)

Parameters declared `void *` that the function cast back at once, fields
and tables whose readers cast them to the record they hold, and copies
written through a cast that an accessor already gives. Each one was
checked with the full build, object by object.

| thing | was | is | casts gone | notes |
|---|---|---|---:|---|
| OffsetToHitboxEdge, OffsetToHitboxEdgeStart, OffsetFromHitboxEdge (objects.h) | `void *dest`, `void *rec` | `struct vec2 *dest`, `const struct hitbox_quad *rec` | 12 | their own 6 casts, and the callers' `(void *)quad` (ProbeTerrainAxes 4, ProbeFloor, ProbeEdgeTerrain). C linkage, so no mangled name changes. |
| `Sprite::lastHitbox` (sprite_obj.hpp, 0x1C) | `void *` | `const struct hitbox_quad *` | 2 | KeepHitboxAnchored stores GetBounds' result and reads it back. |
| Crate::PlayerHitboxOverlapsAt's `quad`, Platform::ResolveCollision's `box` | `struct hitbox_quad *` | `const struct hitbox_quad *` | 2 | both take a sprite bank's `box[0]`, which is const. Mangled name: `PlayerHitboxOverlapsAt__5CrateP11hitbox_quadP4aabbii` to `...PC11hitbox_quad...`. |
| Sprite::SetBank / GetBank (SetSpriteAnimTable, GetSpriteAnimTable) | `void *` | `const struct sprite_bank *` | 2 | UNUSED. Mangled name: `SetBank__6SpritePv` to `SetBank__6SpritePC11sprite_bank`. |
| LevelState::SetLevelBoss | `void *` | `BossCtrl *` | 1 | spawn_bosses.cpp passes the controllers. Mangled name: `SetLevelBoss__10LevelStatePv` to `SetLevelBoss__10LevelStateP8BossCtrl`. |
| `category_descriptor::bgPicture` (actor_anim.h, 0x0C), LoadBgPicture, FillBgPictureMap | `void *`, `u8 *` | `const u8 *` | 2 | InitActorCategory's and RunCategoryPauseMenu's `(u8 *)CUR_CATEGORY.bgPicture`; the table's `(void *)gCategoryNBgPicture` go too (data isn't counted). The pictures are const, so LoadBgPicture, MapFill and FillBgPictureMap read them through const pointers. |
| `SaveMenu::dispcnt` (save_menu.hpp, 0x1C) | `u16` and 2 bytes of padding | `union dispcnt` | 4 | LoadBg set its two bytes through `((u8 *)&dispcnt)[0]` and `[1]`. It is now LanguageSelect::LoadBg's bitfield spelling, `dispcnt.bits.objMap1D = 1` and so on, which gives the same byte read-modify-writes. |
| `LinkSession::handshakeWord` (link_session.hpp, 0x20) | `struct link_id_word` | `union link_id_word` (`bits`, `raw`) | 2 | HandleSerial and ResetState sent it as `*(u16 *)&handshakeWord`; now `handshakeWord.raw`. |
| gExtraLifeHopWidths, gWumpaHopWidths | `const s32 [3]` | `const struct three_words` | 2 | UpdateHop copies the table onto the stack in one go. |
| gDingodileRocketRiseMotion, gDingodileStalactiteFallMotion | `const s32 [3]`, `const s32 [9]` | `const struct speed_ramp`, `const struct speed_ramp [3]` | 2 | the motion records StartTargetMotionY takes. The second table is three records; only the first is read. |
| Slideshow::ShowPicture's `asset` | `void *` | `const u16 *` | 3 | `(u8 *)asset + 0x200` is `&asset[0x100]`, the bitmap after the 256-colour palette. |
| HovercraftLauncher::StateLaunch | `(struct actor_spawn *)slot` | `0` | 3 | `slot` is the cooldown, zero on that path: the spawn record is NULL. gcc reuses the zero register either way. |
| Player::HasRoomForAnim | `(struct vec2 *)&pos` | `&pos` | 1 | `pos` is a `struct vec2`. |

38 casts go (`--casts`: 682 to 644). 4 casts in LoadBgPicture changed
only by gaining a `const`.

## Left as they are

- Sprite::Pos() and MovingSprite::PrevPos() keep their
  `*(struct vec2 *)&x` and `&prevX`: they are the vector view of the two
  words, which the block copies need.
- CutsceneLocals::pager (level_cutscene.cpp): raw storage the player is
  constructed in, reached through a cast at each use; its comment says
  why a pointer or an accessor doesn't match.
- SwimCtrl's `*(struct keys *)&gKeys` (6): `struct keys` has a
  zero-length array to make it BLKmode, so the copy lives on the stack as
  in the ROM. gKeys' own `struct held_pressed_pair` would be copied in a
  register.
- The `sprite_frame` views (`sprite_frame_1box`, `_3box`, `_anchor`):
  one record read through the layout its frame type selects.
- `gSlideshowDispcnt`: a word that SetSlideshowDispcnt stores whole,
  ShowPicture rebuilds a byte of and REG_DISPCNT takes a halfword of.
- `Hovercraft::palette`, `Airship::palette` and their `mapFrames`: the
  palette is the start of a blob (palette, then the frame heights and
  frames), and `mapFrames` holds the frame addresses as words, as
  AnimPart::frameOffsets does for every animation.
- The LoadGraphics tile clears (`*(u32 *)i = zero`): a signed-address
  loop, as in the ROM.
- SaveTransfer's `cursor = (u8 *)tmpl`: the save data streamed as bytes.

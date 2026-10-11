# Casts to the type the value already has, and the next small retypes (#855)

`tools/magic_numbers.py --casts --kind same` listed 21 casts to the type
the value already has. Most dropped a `const`, because the field or the
parameter that took the value wasn't `const`. Those fields and parameters
are `const` now, as the data they point at is. 20 of the 21 go. A few
more small retypes from the top of the refreshed inventory are here too.
39 casts go (`--casts`: 806 to 767), and every object is the same.

## Casts to the type the value already has (20)

| casts | was | now |
|---:|---|---|
| 2 | `(u32)state` in SaveMenu::Draw and Input | `state` is already a `u32` |
| 2 | `(Crate *)` in crate_states.cpp's RingAt and crate_break.cpp's RingPush | `Player::list[]` and RingPush's `obj` are already `Crate *` |
| 5 | `(u8)` on AabbOverlaps (3), IsNearCamera, PaletteCache::GetSlot | they already return `u8` |
| 1 | `(u32)oam.affineMode` in ScaledSprite::Draw | the switch reads the 2-bit field the same way |
| 1 | `(u8 *)gVramTileBlockIndex` to mem_free | it is a `u8 *` |
| 3 | `(struct anim_frame_record *)keyframes` (Hovercraft, Airship, Yeti::Create) | `AnimPart::anims` and the constructor's `a` are `const struct anim_frame_record *` |
| 1 | `(struct anim_table_record *)rec` in ActorSelf's constructor | `ActorSelf::record` is `const struct anim_table_record *` |
| 1 | `(u8 *)` on a cutscene page's string | CutscenePlayer::Run's `str` is `const u8 *` |
| 1 | `(void *)desc->tileData` in BgLayer::Load | `BgLayer::tileData` is `const void *` |
| 2 | `(void *)asset` to LZ77UnCompVram and RLUnCompVram in LoadTaggedAsset | asset.cpp's one-argument declarations take `const void *` |
| 1 | `(struct level_layer_desc *)` in LevelLayers::LoadRoom | TileCache::SetSource takes a `const struct level_layer_desc *` |

The other readers of `anims` (LogoActor::CurFrame and Draw, AnimPart::CurAttr)
take `const struct anim_frame_record *` locals. Nothing writes through it.

## Small retypes (19)

| thing | was | is | casts gone | the ROM |
|---|---|---|---:|---|
| `TileCache::source` (bg_layer.hpp, 0x000) | `void *` | `const struct level_layer_desc *` | 2 | GetCell and CellAt read the chunk grid through `(*(u16 **)source)[i]`; they read `source->chunkGrid[i]`, the struct's first field. |
| `anim_frame_record::attr << 16` (GetAnimFrameAttr, CurAttr, LogoActor::Draw) | `(s32)attr << 16` | `attr << 16` | 3 | a `u16` is promoted to `int` anyway. |
| `Sprite::Pos()` reads | `*(struct vec2 *)&Pos()` | `Pos()` | 6 | Pos() already returns a `struct vec2 &` (ProbeTerrainAxes, ProbeFloor, ProbeEdgeTerrain). |
| `SaveData::CheckChecksum()` | `u32` | `u8` | 2 | both callers (SaveData::Load, SaveMenu::LinkInput) cast it to `(u8)`. The result is ChecksumOk's 0 or 1, so the function's own code doesn't change. |
| `Airship::stateTimer` (gAirshipStateTimer) | `s32` | `u32` | 4 | LoadGraphics compares it with `(u32)` casts (`bhi`). The other reads are `==` tests and `& 8`, the same for either type. |
| `AnimPart::sortKey` (actor_self.hpp, 0x14) | `s32` | `u32` | 2 | KeyGreater (sprite_arm.cpp, the heap sort's compare) compares it unsigned. The other reads are `& SORT_KEY_FLAG_BEHIND_BG` tests. |

cxx_symbols.txt has one new mangled name:
`SetSource__9TileCachePC16level_layer_desc` (SetCollisionSource). The other
changes are return types, field types and AnimPart's inline constructor,
none of which is in a mangled name.

## Left

- `asset = (void *)desc->asset` in LevelLayers::LoadRoom (a `same` cast).
  `LevelLayers::asset` is either the level's asset in the ROM or a buffer
  it allocates, unpacks into and frees (`assetOwned`), so it stays a
  `void *`, and the ROM case drops the `const`. It has a comment.
- `(u16 *)GetChunk(...)` in CellAt and GetCell: the cache slots are
  `u8 [16][0x100]` buffers (DecodeChunk's output), and the cell lookups
  read them as halfwords.
- `(u16)lk[j].to` in LevelEntityFlags::SpawnRoomEntities: a truncation
  into the actor ids, which are halfwords.

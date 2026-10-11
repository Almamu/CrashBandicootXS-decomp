# `affineMode`: the OAM attr0 affine mode (#843)

`affineMode` is attr0 bits 8-9 of a GBA OAM entry, in the shadow entry
(`union oam_shadow_entry`'s `attr0`) and in `struct oam_attrs`, the entry
the draw code builds (include/gfx.h). It is a hardware field: bit 8 turns
on rotation/scaling, and bit 9 is "double size" with it and "OBJ disable"
without it. The names follow the hardware's meaning and sit next to the
structs in gfx.h.

| Value | Name | Hardware | Set by |
|---|---|---|---|
| 0 | `OAM_AFFINE_MODE_NORMAL` | a regular OBJ; attr1 bits 12-13 are the h/v flip (`matrixBit3`/`matrixBit4`) | SpriteRenderer::DrawPieces; ScaledSprite::Fit at a scale of 1; `CLEAR_OAM` (frontend.hpp) zeroes a whole entry, which leaves it 0 |
| 1 | `OAM_AFFINE_MODE_AFFINE` | rotated/scaled with the affine matrix attr1 bits 9-13 pick, inside the OBJ's own box | SpriteRenderer::DrawAffinePieces up to a scale of 1; ScaledSprite::Fit when it shrinks the OBJ; the level select's cursor; the title logo's piece at full size; the VV logo's scaled piece B |
| 2 | `OAM_AFFINE_MODE_HIDDEN` | not displayed | OamBuffer::HideUnused, on the entries past `count` |
| 3 | `OAM_AFFINE_MODE_DOUBLE` | affine, in a box twice the OBJ's size, so a grown OBJ isn't clipped | SpriteRenderer::DrawAffinePieces above a scale of 1; ScaledSprite::Fit when it grows the OBJ (a scale parameter below 0x100); the title logo's pieces while they zoom; the VV logo's piece C when scaled |

ScaledSprite::Draw switches on it to place the OBJ: as is when `NORMAL`,
centred on the preset box when `AFFINE`, and centred on the double-size
box when `DOUBLE`; any mode but `NORMAL` takes an affine matrix.

# `objMode`: the OBJ mode (#843)

attr0 bits 10-11, `objMode` in the same structs. Sprites keep it as
`mirrorBits.gfxMode` / `mirrorFlags.gfxMode` (sprite_obj.hpp), and
SpriteRenderer::DrawPieces / DrawAffinePieces copy it into the entry.
The names, `OAM_OBJ_MODE_*`, are the hardware's, next to the affine
modes in gfx.h.

| Value | Name | Hardware | Set by |
|---|---|---|---|
| 0 | `OAM_OBJ_MODE_NORMAL` | drawn as is | ScaledSprite and the level select's cursor build their entry with it; a new sprite starts at 0 |
| 1 | `OAM_OBJ_MODE_SEMI_TRANSPARENT` | always alpha-blended, as the first target | the effects LaunchEffectPart / SpawnEffectPart / SpawnAt / SpawnPickupEffect spawn (ActionCtrl::StateRun and StateAirborne, SpawnCrateGem, the super body slam's hit, the pickup effects in sprite.cpp), the level select picture's twinkles, Dingodile's shield |
| 2 | `OAM_OBJ_MODE_WINDOW` | the OBJ window's mask, not drawn | nothing |
| 3 | `OAM_OBJ_MODE_UNK3` | prohibited | nothing |

Left as numbers: Dingodile's SpawnShieldOrRocket stores one `kind = 1` as
both the shield's `gfxMode` and its part `kind`, so it stays a number.

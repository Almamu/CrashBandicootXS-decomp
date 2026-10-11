# Records and words read through the wrong type (#855)

Fields and results that were declared as raw words or bytes, while every
reader cast them to the record they hold. Each one was checked with the
full build, object by object.

| thing | was | is | casts gone |
|---|---|---|---:|
| `Yeti::chargeParams` (yeti.hpp, gYetiChargeParams) | `const s32 [6][3]` | `const struct YetiCharge [6]` | 6 (3 `(u8 *)`, 3 `(s32 *)` byte offsets) |
| `Font::oam_scratch` (font.hpp, 0x000) | `u8 [8]` | `struct oam_attrs` | 2 |
| `LevelState::GetLevelFlags()`, `GetCurrentLevelFlags()` | `u8 *` | `union level_record *` | 5 (UpdateGameFrame's 4, GetLevelFlags' `(u8 *)`) |

13 casts go (`--casts`: 806 to 793).

## Yeti::chargeParams

Each record is three words: how far StateCharge moves the yeti each frame,
and StateChase's two rolls (out of 0x100) for starting a charge, one while
the cell animation runs faster than 0x24 and one otherwise. The readers
built the addresses by hand: `u8 *tableBase = (u8 *)chargeParams;`,
`tablePlus4 = tableBase + 4`, `*(s32 *)(tablePlus4 + offset)`. They are
now `chargeParams[paramsIndex].fastChance`, `.slowChance` and `.step`,
and StateCharge's `s32 *c8 = &position` local is gone. gcc gives the
same address arithmetic for the field reads. The static member's mangled
name has no type in it (`_4Yeti.chargeParams`), so cxx_symbols.txt is
unchanged, and the table in actor_tables_17a728.cpp is the same data.

## Font::oam_scratch

The glyph's OAM entry, which DrawGlyph fills and hands to
`gOamBuffer->Add(this)`. DrawGlyph and LargeFont's constructor cast it to
`struct oam_attrs *`; SetPalette and ResetPalette set the palette nibble
by hand (`oam_scratch[5] = (oam_scratch[5] & 0xF) | palette << 4`). As a
`struct oam_attrs`, those are `oam_scratch.size = 1` and
`oam_scratch.palette = palette`: gcc stores the 4-bit field with the
same `ldrb`/`and #0xF`/`orr`/`strb`. font.hpp includes gfx.h for the
struct.

## GetCurrentLevelFlags

It returns the address of the level's progress word in
`progress.levels[]`, which is a `union level_record`. UpdateGameFrame
cast the result back to the union four times (`w.cleared`, `w.time`,
`low`). The union has a new byte member, `flags`: the crystal and gem
bits, which the pickups (Player::HandleEvent), the gem spawners and
LevelState's crate-gem checks OR in and test a byte at a time
(`GetCurrentLevelFlags()->flags |= LEVEL_FLAG_CRATE_GEM`). With a scalar
member the union stays SImode, so `w.time` is still read as a word.
Return types aren't mangled, so cxx_symbols.txt is unchanged.

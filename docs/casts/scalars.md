# Scalar casts the types don't need (#855)

Casts to `s32` on values that are promoted to `int` anyway, `(u8)`
truncations of parameters that were declared wider than the byte they
carry, signed thresholds declared `u32`, and pointers compared as
integers. Each one was checked with the full build, object by object.

| thing | was | is | casts gone | notes |
|---|---|---|---:|---|
| `INT_TO_Q8((s32)px)` in the Sprite, MovingSprite, GroundSprite, Player, ExtraLife, Wumpa and Stopwatch constructors, Entity::Create, SpawnPlayerPosition, SpawnUnderwaterPlayerPosition | `(s32)` on a `u16` | `INT_TO_Q8(px)` | 20 | a `u16` is promoted to `int` before the shift. |
| Crate::SetSlotStage, SetSlotSpins, SetSlotPhase, SetState, SetSolidKind (crate_fields.cpp) | `u32` parameter, `u8 s = (u8)stage` | `u8` parameter | 5 | g++ narrows a `u8` parameter on entry the same way. UNUSED. Mangled names: `...__5CrateUi` to `...__5CrateUc`. |
| Crate::DecrementSlotStage, DecrementSlotSpins | `u8 stage = (u8)GetSlotStage()`, `u8 next = (u8)(stage - 1)` | no casts | 4 | the assignment to the `u8` local narrows. |
| `category_descriptor::maskAssistDeaths`, `bonusKindDeaths`, `retryBossDeaths` (actor_anim.h) | `u32` | `s32` | 3 | IsActorMaskAssistDue and InitActorCategory compare them signed (`blt`) with the death counters, their only reads; LevelState's and level_info's `maskAssistDeaths` are `s32` too. |
| LevelProgress::IsInGemPathRoom, IsInBonusRoom (level_query.cpp) | `s32 cached = (s32)cat; cached == (s32)list->extra2` | `cat == list->extra2` | 4 | |
| SpawnJetpackBalloon (vehicle.hpp) | `void *` result, `s32 d` | `JetpackBalloon *`, `JetpackBalloonCrate *crate` | 3 | JetpackBalloonCrate::Hang passed `(s32)this` and cast the result back. C linkage. |
| `anchor = *(struct vec2 *)&x` (ExtraLife, Wumpa: constructors and SetHome) | | `anchor = Pos()` | 4 | Sprite::Pos() already is that view. |

43 casts go (`--casts`: 682 to 639).

## Left as they are

These are mixed: some readers need the declared type and others the
cast one, so a retype moves the cast elsewhere.

- `Font::MeasureText()`: the pause menu's three `(u32)w >> 1` shift right
  logically, the language select's and the title screen's
  `(240 - w) >> 1` arithmetically.
- `BgLayerBase::widthPx`/`heightPx`: SpawnEffectPart's
  `(s32)((u32)widthPx << 8) >> 8` sign-extends from 24 bits for the test
  and zero-extends for the clamp; the other readers use it signed.
- `Crate::slotState`: as a `u32` the stage reads lose their casts, but
  OpenSlot's and UpdateSlot's `switch (slotState & 7)` turn their signed
  range tests (`ble`) into unsigned ones (`bcc`).
- `SaveMenu::cursor`: `(u32)cursor <= 4` is MoveCursor's one-compare
  range check; the other 35 reads are signed.
- WaitForKeyPress's `(u16)mask` and DingodileCtrl's `(u16)x`: with a
  `u16` parameter the narrowing moves into the prologue and the callers.
- TitleScreen::DrawLogoPieces' `landTimer` reads (docs/casts/byte_offsets.md).
- `link_rx_word::lo`'s `(s32)`: a signed 4-bit field would sign-extend.

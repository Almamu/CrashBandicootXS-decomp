# Byte-offset tables read as arrays (#855)

Bitmaps, buffers and tables that the code walked with a byte pointer and
a hand-scaled index, `(s32 *)((u8 *)this + 8 + (word << 2))`, while the
thing has an array type. Each one was checked with the full build,
object by object.

| thing | was | is | casts gone |
|---|---|---|---:|
| LevelEntityFlags' bit accessors (entity_flags.cpp): SetGone, IsGone, IsActivated, SetActivated, MarkActivated | `(u8 *)this + 8`, `+ 0x208`, `+ 0x308` and a byte offset | `bits0[n / 32]`, `bits1[...]`, `bits1Copy[...]` | 10 |
| Bitmap::Set, ClearBit (entity_bitmap.cpp) | `(s32 *)((u8 *)bits + (wordIndex << 2))` | `bits[n / 32]` | 4 |
| `BgStreamer::ring` (bg_layer.hpp) | `u8 *` | `u16 *`, 64x32 cells | 6 |
| Crate::Break's gone bit (crate_break.cpp) | `(u32 *)((u8 *)slot + off)` | `base->bits0Copy[word]` | 2 |
| SetupActorVramPool (actor_vram_pool.cpp) | `*(s32 *)((u8 *)gActorCategories + idx * sizeof(...))` | `arr[idx].type` | 4 |
| `Starfield::tileBuffer` (frontend.hpp) | `u8 *` | `u16 *` | 1 |

27 casts go (`--casts`: 682 to 655).

## The entity bitmaps

`n / 32` and `n % 32` are the ROM's `n < 0 ? n + 31 : n`, `asr #5` and
`n - (word << 5)`. The hand-written versions spelled that sign fix-up and
the shifts out. Indexing the member arrays gives the same code, the word
index shifted before the member's offset is added to `this`. SetActivated
(a NAKED function once, docs/matching/archive/naked-sub_80259d4-matched.md)
was the exception noted in its comment: a byte pointer for `bits1` and
`live += word` for `bits1Copy`. `bits1[word] |= 1 << (n % 32)` and the
same for `bits1Copy` give the same code, the shifted index computed once.
Crate::Break's copy of ENTITY_SET_GONE_BIT_OF is the same:
`base->bits0Copy[word] |= one << (eid - word * 32)`. Bitmap::ClearBit
needs the mask as its own local (the ROM computes it before the word's
address).

## BgStreamer::ring

The ring holds halfword tilemap cells, 64 columns by 32 rows. Every
reader cast it to `u16 *`. As a `u16 *`, GetColumn returns
`&base[col]`, GetRow `&base[row * 0x40]` and GetCell reads `*base` after
`base += idx`; StreamRow and StreamColumn step by `* 0x200` and `* 0x10`
cells instead of `<< 10` and `<< 5` bytes, and Fill and DecodeChunk take
the pointer as it is. The ring is still loaded into a `base` local before
the index is shifted, as in the ROM. The constructor allocates
`new u16[0x800]`, the same 0x1000 bytes.

## Starfield::tileBuffer

The 4bpp screen buffer is read and written a halfword at a time by Plot:
`entry = &tileBuffer[addr >> 2]`. The DMA calls take it as it is.

## Left as they are

- TitleScreen::DrawLogoPieces' `landTimer[7 - i]` and `landTimer[k]`:
  the ROM shifts the index before it builds `&landTimer[0]`, which no
  array spelling does. They stay as an integer address plus a byte
  offset, with a comment.
- crate_break.cpp's HitResponseAt: `t[*row][k]` adds the row to the table
  before it adds `k * 4`. The ROM adds the table last.
- BlastNearby's and BreakCratesInArea's `*(u8 *)(k + commit)`: the ROM
  adds the kind to the table, `kind + table`; indexing adds the other
  way round. (Their comments say so.)
- PauseMenu::InitInfo's gLevelTable and InitTimeTrialPage's `progress`
  read: their comments say why the byte offset stays.

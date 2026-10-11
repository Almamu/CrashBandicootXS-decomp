# Casts to `void *` and back that the types make unnecessary (#855)

Arguments cast to `u8 *` for functions that take `void *`, and a tile
base returned as an integer only to be cast back to a pointer. Each one
was checked with the full build, object by object.

| thing | was | is | casts gone | notes |
|---|---|---|---:|---|
| `mem_free` arguments (sprite_frame.cpp: FreeObjTileFreeList, FreeSpriteFrameOamQueue 2, FreeSpriteFrameCache, FreeCategorySpriteSheet; vram_dma_queue.cpp: FreeVramDmaQueue) | `mem_free((u8 *)p)` | `mem_free(p)` | 6 | mem_free takes a `void *`. |
| IwramFree (util.h, iwram_alloc.cpp) | `u8 *address` | `void *address` | 2 | AudioContext's and LinkSession's `operator delete` passed `(u8 *)p`; it only hands the address to mem_free. C linkage. |
| mem_free's heap test (memory.cpp) | `address >= (void *)mem_iwram_heap_pointer` | `address >= mem_iwram_heap_pointer` | 1 | a `void *` compares with any object pointer. |
| Sprite::GetTileBase (GetSpriteTileBase) | `s32`, `(s32)table->tileBase` | `const u8 *` | 3 | DrawPieces and DrawAffinePieces added the frame's offset and cast the sum back to `void *` for ObjVramCursor::Upload, which now takes a `const void *` (it hands it to QueueVramDmaTransfer, which does too). Mangled name: `Upload__13ObjVramCursorPvi` to `Upload__13ObjVramCursorPCvi`; return types aren't mangled. |

12 casts go (`--casts`: 682 to 670).

## Left as they are

- The `mem_alloc` results cast to the pool's type: C++ needs the cast
  from `void *`.
- CompanyLogos' `tilesA`/`tilesB`/`tilesC` (`u32`): the tile numbers are
  computed from them as integers (`(tilesA - OBJ_VRAM0) >> 5`), as other
  VRAM addresses are.
- The VRAM tile buffers (gJetpackPlayerTiles, gPolarPlayerTiles,
  gLogoActorTiles) cast to `u16 *` for gUnpackRleSpriteFrameFunc: they
  hold AllocVramTileBlock's `void *` result, which a `u16 *` would need
  a cast for instead.
- `gCategorySpriteSheet` (`void *`): GetAnimFrameData's
  `(u8 *)gCategorySpriteSheet + offset` would move to the `mem_alloc`
  result in DecompressCategorySpriteSheet.

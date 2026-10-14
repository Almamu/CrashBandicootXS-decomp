#ifndef GUARD_CATEGORY_SPRITE_SHEET_HPP
#define GUARD_CATEGORY_SPRITE_SHEET_HPP

/* The category sprite sheet (src/engine/gfx/sprite_frame.cpp, #915): the 3D
 * stages' sprite sheet, LZ77-compressed in the ROM (the category
 * descriptor's `sprite_sheet`) and decompressed into an EWRAM buffer
 * while the category runs. The animation records' frame offsets are
 * relative to it (AnimPart::GetAnimFrameData).
 *
 * An all-static class, like the sprite frame cache next to it
 * (docs/cplusplus.md, "The category sprite sheet as an all-static
 * class"): its one variable is an IWRAM global, and a static data member
 * compiles like that global, static member functions like plain
 * functions. The data member is only declared: cxx_symbols.txt (block
 * "#915 CategorySpriteSheet") maps it to its C name
 * (`_Q36Engine3Gfx19CategorySpriteSheet.data` is gCategorySpriteSheet), whose address
 * is in sym_iwram.txt, and the functions keep their C names the same way
 * (`Free__Q36Engine3Gfx19CategorySpriteSheet` is FreeCategorySpriteSheet).
 *
 * In the namespace Engine::Gfx (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

namespace Engine
{
namespace Gfx
{

class CategorySpriteSheet
{
public:
    static void Free();                        // FreeCategorySpriteSheet
    static void Decompress(const void *sheet); // DecompressCategorySpriteSheet: into a new buffer

    /* gCategorySpriteSheet - the EWRAM buffer. Public: GetAnimFrameData
     * (AnimPart) adds its frame offsets to it, and an inline getter would
     * load it before the offset instead of after (docs/cplusplus.md). */
    static void *data;
};

} // namespace Gfx
} // namespace Engine

#endif // GUARD_CATEGORY_SPRITE_SHEET_HPP

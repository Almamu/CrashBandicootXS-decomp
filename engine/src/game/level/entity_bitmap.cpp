#include "game/level/entity_flags.hpp"

extern "C" {
#include "core.h"
#include <agb_syscall.h>
}

using Game::Level::Bitmap;

/* A 32-word (1024-bit) bitmap's helpers, ROM 0x08025554-0x080255D4:
 * set, clear, clear all and the constructor-like InitBitmap. Nothing
 * calls them; LevelEntityFlags::SetGone (entity_flags.cpp) uses the same
 * bit idiom. In collision_map.cpp (now tile_cache_cell.cpp) until #770.
 * They are class Bitmap's methods (include/game/level/entity_flags.hpp, #762) under
 * their C names (cxx_symbols.txt); all UNUSED. */

/* Sets bit `n % 32` of word `n / 32` of `bits` (the signed division is
 * the ROM's `n < 0 ? n + 31 : n` then `asr #5`). Returns 1 if the bit
 * was previously clear (newly set), 0 if it was already set. */
s32 Bitmap::Set(s32 n)
{
    s32 result = 0;
    s32 word = n / 32;
    u32 mask = 1 << (n % 32);

    if (!(bits[word] & mask)) {
        bits[word] |= mask;
        result = 1;
    }
    return result;
}

/* Clears bit `n & 0x1f` of the bitmap (SetBitmapBit sets them). */
void Bitmap::ClearBit(s32 n)
{
    /* `word` is needed: inlined, the code is 3 instructions longer */
    s32 word = n / 32;
    /* `mask` is needed: inlined, the instructions are reordered */
    u32 mask = 1 << (n % 32);

    bits[word] &= ~mask;
}

/* ClearBitmap: zero-fills `bits` via the BIOS `CpuSet` wrapper, 32-bit
 * fixed-source mode. */
void Bitmap::Clear()
{
    s32 zero = 0;

    CpuSet(&zero, bits, CPU_SET_32BIT | CPU_SET_SRC_FIXED | sizeof(bits) / sizeof(u32));
}

/* InitBitmap: clears the bitmap (g++'s constructor returns `this`, as
 * the C wrapper returned the pointer it cleared). */
Bitmap::Bitmap()
{
    Clear();
}

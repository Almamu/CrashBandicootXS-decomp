#include "entity_flags.hpp"

extern "C" {
#include "core.h"
#include <agb_syscall.h>
}

/* A 32-word (1024-bit) bitmap's helpers, ROM 0x08025554-0x080255D4:
 * set, clear, clear all and the constructor-like InitBitmap. Nothing
 * calls them; LevelEntityFlags::SetGone (entity_flags.cpp) uses the same
 * bit idiom. In collision_map.cpp (now tile_cache_cell.cpp) until #770.
 * They are class Bitmap's methods (include/entity_flags.hpp, #762) under
 * their C names (cxx_symbols.txt); all UNUSED. */

/* Sets bit `n & 0x1f` of the (32-bit-word-per-block) bitmap array at
 * `this`, floor-dividing `n` by 32 to find the word (so it behaves
 * correctly for negative `n` too). Returns 1 if the bit was previously
 * clear (newly set), 0 if it was already set. */
s32 Bitmap::Set(s32 n)
{
    s32 t = n;
    s32 result = 0;
    s32 wordIndex, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;
    word = (s32 *)((u8 *)bits + (wordIndex << 2));

    if (!(*word & mask)) {
        *word |= mask;
        result = 1;
    }
    return result;
}

/* Clears bit `n & 0x1f` of the bitmap (SetBitmapBit sets them). */
void Bitmap::ClearBit(s32 n)
{
    s32 t = n;
    s32 wordIndex, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;
    word = (s32 *)((u8 *)bits + (wordIndex << 2));

    *word &= ~mask;
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

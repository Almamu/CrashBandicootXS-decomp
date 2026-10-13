#ifndef GUARD_SPRITE_FRAME_CACHE_HPP
#define GUARD_SPRITE_FRAME_CACHE_HPP

/* The sprite frame cache (src/gfx/sprite_frame.cpp and the ARM Lookup in
 * src/iwram/sprite_arm.cpp, #915): the animation frames whose pixels are
 * in OBJ VRAM, as nodes (`struct sprite_frame_cache_node`, gfx.h) on two
 * ring lists: `current`, this frame's, newest first, and `previous`, last
 * frame's, which LoadTiles evicts from the tail when OBJ VRAM is full. Age
 * moves `current` to the front of `previous` once a frame.
 *
 * An all-static class, like the OBJ tile allocator (docs/cplusplus.md,
 * "The OAM queue and the sprite frame cache as all-static classes"): its
 * variables are separate IWRAM globals (0x03001354-0x0300137B) that no
 * other object reads, and static data members compile like those globals,
 * static member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 SpriteFrameCache") maps each one
 * to its C name (`_16SpriteFrameCache.current` is
 * gSpriteFrameCacheCurrent, ...), whose address is in sym_iwram.txt, and
 * the functions keep their C names the same way (`Age__16SpriteFrameCache`
 * is AgeSpriteFrameCache).
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct sprite_frame_cache_node;

class SpriteFrameCache
{
public:
    static s32 LoadTiles(u8 *frame); // LoadSpriteFrameTiles: the frame's OBJ tile number
    static void Destroy();           // FreeSpriteFrameCache
    static void Age();               // AgeSpriteFrameCache
    static void Init();              // InitSpriteFrameCache
    /* LookupSpriteFrameCache (ARM, IWRAM): the frame's OBJ tile number if
     * it is cached, or -1. LoadTiles calls it through
     * gLookupSpriteFrameCacheFunc (iwram_data.cpp). */
    static s32 Lookup(u8 *frame);

private:
    /* IWRAM (sym_iwram.txt). `current` and `previous` are the two lists'
     * sentinels, `spares` the unused nodes' stack, `pool` the 128 nodes. */
    static struct sprite_frame_cache_node current;  // gSpriteFrameCacheCurrent
    static struct sprite_frame_cache_node previous; // gSpriteFrameCachePrevious
    static struct sprite_frame_cache_node *spares;  // gSpriteFrameCacheSpares
    static struct sprite_frame_cache_node *pool;    // gSpriteFrameCachePool
};

#endif // GUARD_SPRITE_FRAME_CACHE_HPP

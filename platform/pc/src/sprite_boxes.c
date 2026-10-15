/* Sprite::GetAttackBox and Sprite::GetBodyBox (sprite.cpp) return a
 * struct aabb. The bosses call them by their C names (cxx_symbols.txt) as
 * functions that fill a box through a pointer, `GetSpriteAttackBox(&box,
 * sprite)` (objects.h), which on the ARM is the same call: g++ 2.9 returns
 * the struct through a pointer in r0 and passes `this` in r1. On x86-64
 * the method takes `this` first and returns the 16-byte box in two
 * registers, so the C names here are the pointer form, calling the
 * methods; dingodile.cpp's view that returns the box (STRUCT_RETURN_VIEW,
 * gba/defines.h) gets HostStructReturn_GetSpriteAttackBox. */

#include "gba/gba.h"
#include "engine/util/aabb.h"

/* The methods, by their host names: `this` first, the box returned. */
struct aabb _ZN4Game8Entities6Sprite12GetAttackBoxEv(void *self);
struct aabb _ZN4Game8Entities6Sprite10GetBodyBoxEv(void *self);

void *GetSpriteAttackBox(void *dest, void *sprite)
{
    *(struct aabb *)dest = _ZN4Game8Entities6Sprite12GetAttackBoxEv(sprite);
    return dest;
}

void *GetSpriteBodyBox(void *dest, void *sprite)
{
    *(struct aabb *)dest = _ZN4Game8Entities6Sprite10GetBodyBoxEv(sprite);
    return dest;
}

struct aabb HostStructReturn_GetSpriteAttackBox(void *sprite)
{
    return _ZN4Game8Entities6Sprite12GetAttackBoxEv(sprite);
}

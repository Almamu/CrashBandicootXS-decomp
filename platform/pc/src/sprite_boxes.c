/* Sprite::GetAttackBox and Sprite::GetBodyBox (sprite.cpp) return a
 * struct aabb. The bosses call them by their C names (cxx_symbols.txt) as
 * functions that fill a box through a pointer, `GetSpriteAttackBox(&box,
 * sprite)` (objects.h), which on the ARM is the same call: g++ 2.9 returns
 * the struct through a pointer in r0 and passes `this` in r1. On x86-64
 * the method takes `this` first and returns the 16-byte box in two
 * registers, so the C names here are the pointer form, calling the
 * methods; dingodile.cpp's view that returns the box (STRUCT_RETURN_VIEW,
 * gba/defines.h) gets HostStructReturn_GetSpriteAttackBox. */

#include <stdio.h>

#include "gba/gba.h"
#include "engine/gfx/sprite_bank.h"
#include "engine/util/aabb.h"
#include "host_platform.h"

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

/* A frame with no pieces (an empty frame: Crash's frame 440, the
 * checkpoint text's 5, the icons' 36) is only its 12-byte header, its
 * `pieces` the next frame's, so its first piece byte is the next frame's
 * and says it has a box after its header (layout type 2). GetBodyBox and
 * GetAttackBox read it: in the ROM, the next frame's header, two ROM
 * addresses, whose halves make a box over 2048 pixels below the sprite
 * and less than 0 wide (each low half is 0x8000 or more), which touches
 * nothing. On the host the header is 24 bytes and the read lands in the
 * 8 bytes of padding before the next frame (aligned to 32): a box 0 wide,
 * which touches nothing either, so every collision is the ROM's (the
 * sprite_banks objects have no ASan redzones for it, platform.mk). That
 * holds only while those bytes are padding, or anything 0 wide or less: a
 * wider box would touch what the ROM's doesn't. So this checks, before
 * the game starts, that every empty frame an animation shows reads boxes
 * 0 wide or less, and no anchor (layout types 0 and 6), which would be
 * drawn. */

extern const struct sprite_bank_table gSpriteBankTable; /* sprite_banks_00_09.cpp */

/* The boxes past the header of each layout type (sprite_bank.h). */
static const u8 sLayoutBoxes[16] = { 3, 0, 1, 2, 3, 1, 1 };

void HostCheckEmptyFrames(void)
{
    u32 b;

    for (b = 0; b < gSpriteBankTable.bankCount; b++) {
        const struct sprite_bank *bank = &gSpriteBankTable.banks[b];
        u32 a;

        for (a = 0; a < bank->animCount; a++) {
            const struct sprite_anim *anim = &bank->anims[a];
            u32 s;

            for (s = 0; s < anim->frameCount; s++) {
                u32 index = anim->seq[s];
                const struct sprite_frame *frame = bank->frames[index];
                const struct sprite_frame_3box *boxed = (const struct sprite_frame_3box *)frame;
                u32 type = frame->pieces[0] >> 4;
                u32 i;

                if ((frame->tiles >> 24) != 0) {
                    continue;
                }
                for (i = 0; i < sLayoutBoxes[type]; i++) {
                    if (type == 0 || type == 6 || boxed->box[i].w > 0) {
                        fprintf(stderr,
                                "sprite bank %u frame %u: an empty frame whose reads past its "
                                "header (layout type %u) aren't padding\n",
                                b, index, type);
                        HostExit(1, "error");
                    }
                }
            }
        }
    }
}

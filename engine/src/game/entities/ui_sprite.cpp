#include "game/entities/sprite_obj.hpp"

using Game::Entities::UiSprite;

/* UiSprite, the sprite with its own OBJ priority (#664,
 * engine/include/game/entities/sprite_obj.hpp): GetPriority, its key method (g++ emits
 * gUiSpriteObjVtable here), and the empty destructor and constructor.
 * An old_agbcp object (OLD_AGBCC_OBJS). */

/* The OBJ priority of its own mirror byte (the sprite's is layer 0's). */
s32 UiSprite::GetPriority()
{
    return mirrorFlags.priority;
}

UiSprite::~UiSprite()
{
}

UiSprite::UiSprite()
{
#ifdef GAME_N2
    /* N-Tranced's (0x080097B0) also turns off the contact, attack and
     * terrain-probe bits */
    DisableContact();
    ClearFlag1();
    f.b.collides = 0;
#endif
}

#include "sprite_obj.hpp"

/* UiSprite, the sprite with its own OBJ priority (#664,
 * include/sprite_obj.hpp): GetPriority, its key method (g++ emits
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
}

#include "core.h"

struct anim_part_instance {
    u8 unknown_00[8];
    s32 field_08;
};

s32 GetAnimFrameBaseOffset(struct anim_part_instance *self)
{
    return self->field_08 >> 8;
}

asm(".align 2, 0");

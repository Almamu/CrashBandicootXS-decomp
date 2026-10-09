#include "platform.hpp"
#include "player.hpp"

/* The rest of Platform's methods (#664, include/platform.hpp), ROM
 * 0x0801B208-0x0801B304. PlatformMover follows in platform_mover.cpp.
 *
 * UNUSED - no caller anywhere in the ROM: the out-of-line constructor
 * InitPlatform (CreatePlatform inlines it). */

/* While near the camera, steps the animation; then the velocity, and the
 * mover. A Neo Cortex platform that has crumbled (type 6 past frame 0x12)
 * drops the player. */
void Platform::Update()
{
    if (IsNearCamera()) {
        AdvanceAnim();
        ApplyVelocity();
        if (type == 6 && frame > 0x12) {
            Sprite **c = &gPlayer->carried;

            if (*c == this)
                *c = 0;
        }
        if (mover)
            mover->Update(this);
    } else {
        ApplyVelocity();
        if (mover)
            mover->Update(this);
    }
}

/* The bonus platform's exit facing (flags2 bit 4), set by CreatePlatform
 * from bit 0 of its entity's parameter flags (SetExitMirror). RunRoom
 * passes it to SetCheckpoint as the checkpoint flags, so the player comes
 * back from the bonus round on the platform X-mirrored when it is set. */
s32 Platform::GetExitMirror()
{
    return (f.bytes.flags2 >> 4) & 1;
}

/* Sets the exit facing GetExitMirror reads (CreatePlatform, type 3: the
 * active bonus platform). */
void Platform::SetExitMirror(u8 value)
{
    s32 mirror = value;

    f.b.exitMirror = mirror;
}

s32 Platform::GetClassId()
{
    return 4;
}

Platform::~Platform()
{
}

/* Clears `vulnerable`, as the constructor does. */
void Platform::ClearVulnerable()
{
    f.b.vulnerable = 0;
}

Platform::Platform()
{
    ClearVulnerable();
}

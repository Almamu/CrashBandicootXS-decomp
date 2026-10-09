#include "actor_self.hpp"
#include "vehicle.hpp"
#include "boss_actors.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "gfx.h"
#include "globals.h"
}

/* AnimPart's methods (#664 part 11a, include/actor_self.hpp), then the
 * 3D actors' destructors (vehicle.hpp, boss_actors.hpp), all but the
 * polar player's, the jetpack player's, the balloon crate's and the
 * polar collected wumpa's, in the ROM's order, with a few small methods
 * among them: the polar and jetpack checkpoint banners', the jetpack
 * explosion's and HpActor's defaults. Every destructor here is an empty
 * body: g++ expands ActorSelf's inline one (the unlink, with
 * gActorVtable stored; the class's own table store before it is dead)
 * and the delete through AnimPart's operator delete (mem_free). */

s32 AnimPart::GetAnimFrameBaseOffset()
{
    return Q8_TO_INT(animTime);
}

/* The current keyframe's `attr`, in the high halfword: an OAM attribute
 * word's flags (DrawActor, DrawJetpackCheckpointText). */
s32 AnimPart::GetAnimFrameAttr()
{
    return (s32)anims[animIndex].attr << 16;
}

/* The current frame's graphics: the keyframe's frameIndex plus the
 * animation's frame, indexing frameOffsets (byte offsets into
 * gCategorySpriteSheet). */
u8 *AnimPart::GetAnimFrameData()
{
    s32 base = GetAnimFrameBaseOffset();

    return (u8 *)gCategorySpriteSheet + frameOffsets[anims[animIndex].frameIndex + base];
}

/* Starts keyframe `idx`: its duration, not done, from the start. */
void AnimPart::SetAnim(s32 idx)
{
    animIndex = idx;
    animTimer = anims[idx].duration;
    animDone = 0;
    animTime = 0;
}

RiderlessPolar::~RiderlessPolar()
{
}

/* Rises 0x180 a frame and deletes itself once its animation is done. */
void PolarCheckpointText::Update()
{
    y += -0x180;

    if (animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

PolarCheckpointText::~PolarCheckpointText()
{
}

PolarWumpa::~PolarWumpa()
{
}

PolarTimeCrate::~PolarTimeCrate()
{
}

PolarQuestionCrate::~PolarQuestionCrate()
{
}

PolarAkuAkuCrate::~PolarAkuAkuCrate()
{
}

PolarNitroCrate::~PolarNitroCrate()
{
}

PolarLifeCrate::~PolarLifeCrate()
{
}

PolarFourWumpaCrate::~PolarFourWumpaCrate()
{
}

PolarBasicCrate::~PolarBasicCrate()
{
}

/* PolarCrate's destructor is inline (vehicle.hpp), as the crate kinds'
 * destructors above expand it; this is its out-of-line copy, the
 * deleting destructor g++ emits with the class's vtable (part 11b). */
void DestroyPolarCrate(PolarCrate *self, u32 flags)
{
    self->PolarCrate::~PolarCrate();
    if (flags & 1)
        AnimPart::operator delete(self);
}

PolarElectricFence::~PolarElectricFence()
{
}

PolarObstacle::~PolarObstacle()
{
}

PolarLauncher::~PolarLauncher()
{
}

PolarPenguin::~PolarPenguin()
{
}

PolarIcicle::~PolarIcicle()
{
}

PolarAkuAku::~PolarAkuAku()
{
}

PolarGoal::~PolarGoal()
{
}

PolarBoostPad::~PolarBoostPad()
{
}

PolarCheckpointCrate::~PolarCheckpointCrate()
{
}

/* The current frame at the fixed screen position (120, 106), culled off
 * screen: ActorSelf::Draw (actor.cpp) at a fixed scale of 0x100, its
 * double-size flag and size tests still written against `scale`. The ROM
 * ORs a zero into the attribute word: the flag folds away in the size
 * tests but not in the OR, where reload rematerializes it as `movs r0,
 * #0`. The C wrote the `orr` in asm and the first C++ hid the constant
 * with a constant-init asm (#662 round 2). */
void JetpackCheckpointText::Draw()
{
    s32 x = 120;
    s32 y = 106;
    s32 scale = 0x100;
    u8 *frame;
    u32 flag;
    s32 halfW;
    s32 halfH;

    frame = GetAnimFrameData();
    flag = 0;
    if (scale <= 0xff)
        flag = 0x200;
    if (scale <= 0xff)
        halfW = frame[0] << 3;
    else
        halfW = frame[0] << 2;
    if (scale <= 0xff)
        halfH = frame[1] << 3;
    else
        halfH = frame[1] << 2;

    x -= halfW;
    y -= halfH;
    if (y > 159)
        return;
    if (y + halfH * 2 < 0)
        return;
    if (x > 239)
        return;
    if (x + halfW * 2 < 0)
        return;

    {
        s32 attr = GetAnimFrameAttr();
        u32 packed = (y & 0xff) | ((x & 0x1ff) << 16) | attr | flag;
        u32 pal = palette;
        u32 pre = pal << 0xc;
        u32 attr2;

        if (sortKey & SORT_KEY_FLAG_BEHIND_BG)
            attr2 = ((pre | 0x800) << 0x10) >> 0x10;
        else
            attr2 = (pal << 0x1c) >> 0x10;

        SetupSpriteFrameOam(frame, packed, attr2, scale);
    }
}

/* Plays the animation once (ActorSelf::Update's loop, without the depth
 * and the clipping) and deletes itself when it is done. */
void JetpackCheckpointText::Update()
{
    sortKey = 1;

    if (animDone != 0) {
        delete this;
    } else {
        animTime += (s16)animTimer;
        animDone = 0;
        if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
            animTime -= INT_TO_Q8(anims[animIndex].loopThreshold - anims[animIndex].loopBase);
            animDone = 1;
        }
    }
}

s32 JetpackCheckpointText::IsUnshootable()
{
    return 1;
}

JetpackCheckpointText::~JetpackCheckpointText()
{
}

/* Moves away 170 a frame and deletes itself once its animation is done. */
void JetpackExplosion::Update()
{
    z += 170;

    if (animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

s32 JetpackExplosion::IsUnshootable()
{
    return 1;
}

JetpackExplosion::~JetpackExplosion()
{
}

s32 HpActor::GetHp()
{
    return hp;
}

/* The classes that take no damage. */
void HpActor::Damage(s32 amount)
{
}

/* Shootable (the jetpack player's: the other classes return their own). */
s32 HpActor::IsUnshootable()
{
    return 0;
}

JetpackShot::~JetpackShot()
{
}

JetpackPlane::~JetpackPlane()
{
}

JetpackBomber::~JetpackBomber()
{
}

JetpackCannonball::~JetpackCannonball()
{
}

AirshipFireball::~AirshipFireball()
{
}

JetpackBalloon::~JetpackBalloon()
{
}

/* The balloon crates' kinds: g++'s implicit destructors (the classes
 * declare none), which call JetpackBalloonCrate's out of line (in
 * jetpack_crates.cpp), with no delete, and then delete. They are written as
 * the functions g++ synthesized: g++ skips the class's own vtable store
 * only in an implicit destructor (an explicit one, even empty, stores it
 * before the call). */
void DestroyJetpackHealthCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1)
        AnimPart::operator delete(self);
}

void DestroyJetpackTimeCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1)
        AnimPart::operator delete(self);
}

void DestroyJetpackQuestionCrate(void *self, u32 flags)
{
    DestroyJetpackBalloonCrate(self, 0);
    if (flags & 1)
        AnimPart::operator delete(self);
}

JetpackParachuteNitro::~JetpackParachuteNitro()
{
}

JetpackRocket::~JetpackRocket()
{
}

JetpackRing::~JetpackRing()
{
}

HovercraftFireball::~HovercraftFireball()
{
}

HovercraftCannon::~HovercraftCannon()
{
}

HovercraftLauncher::~HovercraftLauncher()
{
}

HovercraftSideGun::~HovercraftSideGun()
{
}

HovercraftCannonFlash::~HovercraftCannonFlash()
{
}

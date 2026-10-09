#ifndef GUARD_BOSS_CTRL_HPP
#define GUARD_BOSS_CTRL_HPP

/* The boss controllers as C++ (#664, docs/cplusplus.md): BossCtrl and
 * the bosses' controllers built on it (src/bosses/).
 *
 * No `#pragma interface`: g++ emits their vtables, each in its key-method
 * object (see ctrl.hpp); cxx_symbols.txt maps their mangled names onto
 * the C names. */

#include "ctrl.hpp"
#include "sprite_obj.hpp"
#include "enemy_ctrl.hpp"

/* The boss controller (gBossCtrlVtable, src/bosses/boss_ctrl.cpp):
 * the base class of the bosses'
 * controllers (Mega Mix, Tiny, Neo Cortex's fight, Dingodile and his
 * shield and rocket/stalactite). Its event handler keeps the event's msg
 * and arg; nothing reads them back. The word at 0x10 is the controlled
 * part (GetTarget) or, in the bosses that don't use that, a counter:
 * Tiny's and Neo Cortex's round, Dingodile's hits. */
class BossCtrl : public Ctrl
{
public:
    union {
        void *target; // 0x10 - the controlled part
        s32 counter;  //        or a subclass's counter
    };
    s32 msg; // 0x14
    s32 arg; // 0x18

    BossCtrl(); // CreateBossCtrl
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg);
    virtual ~BossCtrl(); // DestroyBossCtrl
    void *GetTarget();   // GetCtrlTarget
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(BossCtrl) == 0x1C);

/* The Mega Mix boss's controller (gMegaMixCtrlVtable;
 * src/bosses/mega_mix.cpp, and its Update,
 * UpdateMegaMix, in src/bosses/mega_mix_update.cpp). Its motion records
 * are gMegaMixMotionRecords, not gCtrlMotionRecords. */
class MegaMixCtrl : public BossCtrl
{
public:
    s32 stamp; // 0x1C - reset to -1 by Reset
    u8 latch;  // 0x20

    MegaMixCtrl(); // CreateMegaMixCtrl
    virtual void Update(MovingSprite *part);
    virtual ~MegaMixCtrl(); // DestroyMegaMixCtrl
    virtual void StartTargetMotionXFromSet(MovingSprite *part, s32 index);
    virtual void StartTargetMotionYFromSet(MovingSprite *part, s32 index);
    void SetMotionYFromSet(MovingSprite *part, s32 index);
    void SetMotionXFromSet(MovingSprite *part, s32 index);
    void Reset();
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(MegaMixCtrl) == 0x24);

/* The Tiny boss's controller (gTinyVtable; src/bosses/tiny.cpp).
 * Tiny hops his part along parabolic arcs between gTouchableList's
 * anchors (the hop pads), stomping them: Update steps the hop and the
 * state machine, SetState enters a state. `counter` is the round (1-3,
 * the hits taken). spawn_bosses.cpp creates it in a 0x4C-byte block. */
class TinyCtrl : public BossCtrl
{
public:
    s32 nextState; // 0x1C - the state after the anim (5) or the wait (4)
    s32 timer;     // 0x20
    s32 stomped;   // 0x24 - the anchor to stomp, or -1
    s32 anchor;    // 0x28 - the anchor hopped to (a gTouchableList index)
    s32 count;     // 0x2C
    s32 x;         // 0x30 - the hop's start
    s32 y;         // 0x34
    s32 steps;     // 0x38 - the hop's steps left
    s32 total;     // 0x3C - and its length
    s32 dy;        // 0x40
    s32 dx;        // 0x44
    s16 *squares;  // 0x48 - i * i >> 8 for i = 0..0x100 (CreateTiny)

    TinyCtrl(); // CreateTiny
    virtual void Update(MovingSprite *part);
    virtual ~TinyCtrl(); // DestroyTiny
    void SetState(MovingSprite *part, s32 next);
    s32 PickHopTarget();
    void SpawnFallingLeaves(MovingSprite *part, s32 n);
    void HitStub(MovingSprite *part);  // TinyHitStub (empty)
    void StartHop(MovingSprite *part); // StartTinyHop
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(TinyCtrl) == 0x4C);

/* The Neo Cortex fight's controller (gCortexBossVtable;
 * src/bosses/cortex.cpp). `counter` is the round. It spawns the
 * cannon and the target (crosshair) parts. spawn_bosses.cpp creates it in
 * a 0x24-byte block. */
class CortexBossCtrl : public BossCtrl
{
public:
    MovingSprite *cannon; // 0x1C - SpawnCannon's part
    MovingSprite *target; // 0x20 - SpawnTarget's part

    CortexBossCtrl(); // CreateCortexBoss
    virtual void Update(MovingSprite *part);
    virtual ~CortexBossCtrl(); // DestroyCortexBoss
    void SetState(MovingSprite *part, s32 next);
    void SpawnCannon(MovingSprite *part); // SpawnCortexCannon
    void SpawnTarget(MovingSprite *part); // SpawnCortexTarget
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(CortexBossCtrl) == 0x24);

/* The Neo Cortex fight's cannon (gCortexCannonVtable; 0x10 bytes,
 * src/bosses/cortex.cpp): hides its part in state 0. */
class CortexCannonCtrl : public Ctrl
{
public:
    CortexCannonCtrl(); // CreateCortexCannonCtrl
    virtual void Update(MovingSprite *part);
    virtual ~CortexCannonCtrl(); // DestroyCortexCannonCtrl
    void SetState(MovingSprite *part, s32 next);
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(CortexCannonCtrl) == 0x10);

/* The Neo Cortex fight's target, the crosshair that hops between the
 * player and the platforms (gCortexTargetVtable, 0x40 bytes;
 * src/bosses/cortex.cpp). */
class CortexTargetCtrl : public Ctrl
{
public:
    u8 dirLeft;           // 0x10
    u8 high;              // 0x11
    u8 top;               // 0x12
    s32 x;                // 0x14 - the hop's destination
    s32 y;                // 0x18
    s32 dx;               // 0x1C - and its distance from the start
    s32 dy;               // 0x20
    s32 stepsLeft;        // 0x24
    s32 steps;            // 0x28 - gCortexTargetHopSteps[the boss's round]
    s32 nextState;        // 0x2C
    s32 timer;            // 0x30
    s32 blink;            // 0x34
    u8 blinking;          // 0x38
    CortexBossCtrl *boss; // 0x3C

    CortexTargetCtrl(CortexBossCtrl *boss); // CreateCortexTargetCtrl
    virtual void Update(MovingSprite *part);
    virtual ~CortexTargetCtrl(); // DestroyCortexTargetCtrl
    void SetPlatformsKind(u8 flag);
    void SetDest(MovingSprite *part, s32 x, s32 y);
    void SetState(MovingSprite *part, s32 next); // SetCortexTargetState
    void FireShot(MovingSprite *part, s32 kind); // FireCortexShot
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(CortexTargetCtrl) == 0x40);

/* The target's shot (gCortexShotVtable, 0x18 bytes; src/bosses/cortex.cpp):
 * a slow one (kind 0) hurts the player, a fast one (1) also shrinks the
 * Neo Cortex gems it hits (CortexBossGemCtrl) and tells the boss. */
class CortexShotCtrl : public Ctrl
{
public:
    u8 fast;              // 0x10 - kind 1 (FireShot)
    CortexBossCtrl *boss; // 0x14

    CortexShotCtrl(CortexBossCtrl *boss); // CreateCortexShotCtrl
    virtual void Update(MovingSprite *part);
    virtual ~CortexShotCtrl(); // DestroyCortexShotCtrl
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(CortexShotCtrl) == 0x18);

/* A gem of the Neo Cortex fight (gCortexBossGemVtable, 0x14 bytes;
 * src/bosses/cortex.cpp, SpawnCortexBossGem): once a fast shot hits it
 * (the part's `kind` 1), it plays its shrink animation and goes. */
class CortexBossGemCtrl : public Ctrl
{
public:
    s32 kind; // 0x10 - 0 red, 1 green, 2 yellow

    CortexBossGemCtrl(s32 kind); // CreateCortexBossGemCtrl
    virtual void Update(MovingSprite *part);
    virtual ~CortexBossGemCtrl(); // DestroyCortexBossGemCtrl
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(CortexBossGemCtrl) == 0x14);

/* gUnusedOneShotAnimCtrlVtable's class (src/bosses/tiny.cpp): does what
 * OneShotAnimCtrl does. Its constructor has no caller (UNUSED). */
class UnusedOneShotAnimCtrl : public Ctrl
{
public:
    UnusedOneShotAnimCtrl(); // CreateUnusedOneShotAnimCtrl
    virtual void Update(MovingSprite *part);
    virtual ~UnusedOneShotAnimCtrl(); // DestroyUnusedOneShotAnimCtrl
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(UnusedOneShotAnimCtrl) == 0x10);

/* Dingodile (gDingodileVtable, src/bosses/dingodile.cpp and
 * dingodile_create.cpp): he walks the level, stopping at the approach
 * tables' x positions to fire a rocket, turns round at the level's ends
 * and hides behind his shield (`shield`). `counter` is the hits he has
 * taken. spawn_bosses.cpp creates him in a 0x30-byte block. */
class DingodileCtrl : public BossCtrl
{
public:
    s32 step;             // 0x1C - the approach table's index
    s32 timer;            // 0x20
    s32 nextState;        // 0x24
    s32 passes;           // 0x28
    MovingSprite *shield; // 0x2C - SpawnShieldOrRocket's mode-0 part

    DingodileCtrl(u32 x, u32 y); // CreateDingodile
    virtual void Update(MovingSprite *part);
    virtual ~DingodileCtrl(); // DestroyDingodile
    s32 GetHits();
    void SetState(MovingSprite *part, s32 next);
    void SpawnShieldOrRocket(s32 mode, u16 x, u16 y, MovingSprite *owner);
    void SpawnShark(u16 x, u16 y, u8 facing);
    void StartMotion(MovingSprite *part, s32 index);
    void SetStep(s32 value);
    void SetNextState(s32 value);
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(DingodileCtrl) == 0x30);

/* Dingodile's shield (gDingodileShieldVtable, 0x28 bytes): hurts the
 * player on contact, and blinks twice when Dingodile is hit. */
class DingodileShieldCtrl : public BossCtrl
{
public:
    s32 blinkTimer;      // 0x1C
    s32 blinksLeft;      // 0x20
    MovingSprite *owner; // 0x24 - Dingodile's part

    DingodileShieldCtrl(); // CreateDingodileShieldCtrl
    virtual void Update(MovingSprite *part);
    virtual ~DingodileShieldCtrl(); // DestroyDingodileShieldCtrl
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(DingodileShieldCtrl) == 0x28);

/* Dingodile's rocket, and the stalactite it drops
 * (gDingodileProjectileVtable, 0x20 bytes): both hurt the player, and
 * the stalactite hurts Dingodile (`owner`) if it lands on him. */
class DingodileProjectileCtrl : public BossCtrl
{
public:
    MovingSprite *owner; // 0x1C - Dingodile's part

    DingodileProjectileCtrl(); // CreateDingodileProjectileCtrl
    /* The stalactite's, from its rocket's (SpawnStalactite): inline, as
     * the ROM has it there */
    DingodileProjectileCtrl(DingodileProjectileCtrl *rocket)
    {
        owner = rocket->owner;
    }
    virtual void Update(MovingSprite *part);
    virtual ~DingodileProjectileCtrl(); // DestroyDingodileProjectileCtrl
    void SpawnStalactite(u16 x, u16 y);
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(DingodileProjectileCtrl) == 0x20);

/* The shark Dingodile's fight sends across the level
 * (gDingodileSharkVtable): an enemy controller in a 0x8C-byte block. */
class DingodileSharkCtrl : public EnemyCtrl
{
public:
    DingodileSharkCtrl(); // CreateDingodileSharkCtrl
    virtual void Update(MovingSprite *part);
    virtual ~DingodileSharkCtrl(); // DestroyDingodileSharkCtrl
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(DingodileSharkCtrl) == 0x8C);

#endif /* !GUARD_BOSS_CTRL_HPP */

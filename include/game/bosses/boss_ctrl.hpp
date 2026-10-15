#ifndef GUARD_BOSS_CTRL_HPP
#define GUARD_BOSS_CTRL_HPP

/* The boss controllers as C++ (#664, docs/cplusplus.md): BossCtrl and
 * the bosses' controllers built on it (src/game/bosses/).
 *
 * No `#pragma interface`: g++ emits their vtables, each in its key-method
 * object (see ctrl.hpp); cxx_symbols.txt maps their mangled names onto
 * the C names.
 *
 * The classes are in the namespace Game::Bosses (#1033, docs/cplusplus.md
 * "Namespaces"), with ctrl.hpp's two Tiny controllers, platform.hpp's
 * CortexBossPlatformMover and the hovercraft (hovercraft.hpp,
 * boss_actors.hpp):
 * `Update__Q34Game6Bosses8TinyCtrlPQ34Game8Entities12MovingSprite`,
 * `_vt.Q34Game6Bosses8BossCtrl` (in the section
 * `.gnu.linkonce.d._vt.Q34Game6Bosses8BossCtrl`, which games/xs/ldscript.txt
 * names) & co. bosses.h's C structs and declarations stay global. */

#include "game/entities/ctrl.hpp"
#include "game/entities/sprite_obj.hpp"
#include "game/enemies/enemy_ctrl.hpp"

namespace Game
{
namespace Bosses
{

/* The boss controller (gBossCtrlVtable, src/game/bosses/boss_ctrl.cpp):
 * the base class of the bosses'
 * controllers (Mega Mix, Tiny, Neo Cortex's fight, Dingodile and his
 * shield and rocket/stalactite). Its event handler keeps the event's msg
 * and arg; nothing reads them back. The word at 0x10 is the controlled
 * part (GetTarget) or, in the bosses that don't use that, a counter:
 * Tiny's and Neo Cortex's round, Dingodile's hits. */
class BossCtrl : public Game::Entities::Ctrl
{
public:
    union {
        void *target; // 0x10 - the controlled part
        s32 counter;  //        or a subclass's counter
    };
    s32 msg; // 0x14
    s32 arg; // 0x18

    BossCtrl(); // CreateBossCtrl
    virtual void HandleEvent(s32 from, s32 event, s32 arg);
    virtual ~BossCtrl(); // DestroyBossCtrl
    void *GetTarget();   // GetCtrlTarget
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(BossCtrl) == 0x1C);

/* The Mega Mix boss's controller (gMegaMixCtrlVtable;
 * src/game/bosses/mega_mix.cpp, and its Update,
 * UpdateMegaMix, in src/game/bosses/mega_mix_update.cpp). Its motion records
 * are gMegaMixMotionRecords, not gCtrlMotionRecords. */
/* MegaMixCtrl's `state` (#843, docs/flags/boss_ctrl.md): Reset starts it
 * running; it stops while the player is dead. */
#define MEGA_MIX_STATE_STOP 0 // the player is dead or warping out: anim 2, X record 0; runs once he lives
#define MEGA_MIX_STATE_RUN 1  // runs after the player, breaking crates; in range: GRAB
#define MEGA_MIX_STATE_GRAB 2 // anim 1: the player takes a hit on frame 8; then RUN (or STOP)
class MegaMixCtrl : public BossCtrl
{
public:
    s32 stamp; // 0x1C - reset to -1 by Reset
    u8 latch;  // 0x20

    MegaMixCtrl(); // CreateMegaMixCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~MegaMixCtrl(); // DestroyMegaMixCtrl
    virtual void StartTargetMotionXFromSet(Game::Entities::MovingSprite *part, s32 index);
    virtual void StartTargetMotionYFromSet(Game::Entities::MovingSprite *part, s32 index);
    void SetMotionYFromSet(Game::Entities::MovingSprite *part, s32 index);
    void SetMotionXFromSet(Game::Entities::MovingSprite *part, s32 index);
    void Reset();

    /* Its entity type's gEntitySpawnFuncs slot (spawn_bosses.cpp, #915):
     * the boss's MovingSprite, moved by a new MegaMixCtrl. */
    static void Spawn(u32 id, u16 x, u16 y, u16 param); // SpawnMegaMix
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(MegaMixCtrl) == 0x24);

/* The Tiny boss's controller (gTinyVtable; src/game/bosses/tiny.cpp).
 * Tiny hops his part along parabolic arcs between gTouchableList's
 * anchors (the hop pads), stomping them: Update steps the hop and the
 * state machine, SetState enters a state. `counter` is the round (1-3,
 * the hits taken). spawn_bosses.cpp creates it in a 0x4C-byte block. */

/* TinyCtrl's `state` (#843, docs/flags/boss_ctrl.md): what Update runs,
 * entered by SetState. A hop is two states: the rise to its top (`x`,
 * `y`), then the drop onto the pad. */
#define TINY_STATE_START 0          // contact off, then HOP_DOWN onto pad 1
#define TINY_STATE_HOP_DOWN 1       // drops onto the pad, then STOMP
#define TINY_STATE_HOP_DOWN_REST 2  // drops onto the pad, then REST
#define TINY_STATE_HOP_UP 3         // rises towards the next pad (PickHopTarget), then HOP_DOWN
#define TINY_STATE_WAIT 4           // `timer` runs out, then nextState; only ANIM falls into it
#define TINY_STATE_ANIM 5           // the animation plays out (or `timer` runs out), then nextState
#define TINY_STATE_STOMP 6          // the stomp animation; the fourth: HOP_UP_REST
#define TINY_STATE_HOP_UP_REST 7    // rises, then HOP_DOWN_REST
#define TINY_STATE_REST 8           // 0xB4 frames: a spin attack is a HIT; then HOP_UP
#define TINY_STATE_HIT 9            // SFX_BOSS_HIT, the hit counted; the third: DEFEATED
#define TINY_STATE_LEAP_OFF 10      // rises off the top of the screen, then OFFSCREEN
#define TINY_STATE_DROP_IN 11       // drops onto the round's pad, then STOMP_PAD
#define TINY_STATE_STOMP_PAD 12     // the round's pad sinks (`stomped`), then HOP_UP
#define TINY_STATE_OFFSCREEN 13     // three falling leaves, 0x46 frames apart, then DROP_IN
#define TINY_STATE_DEFEATED 14      // rises; the tornado spin power, then FALL_OUT
#define TINY_STATE_FALL_OUT 15      // drops out of the level: LevelProgress::RequestRoomExit, then GONE
#define TINY_STATE_GONE 16          // the end: nothing runs
class TinyCtrl : public BossCtrl
{
public:
    s32 nextState; // 0x1C - the state after ANIM or WAIT; in OFFSCREEN, the leaves' countdown
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
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~TinyCtrl(); // DestroyTiny
    void SetState(Game::Entities::MovingSprite *part, s32 next);
    s32 PickHopTarget();
    void SpawnFallingLeaves(Game::Entities::MovingSprite *part, s32 n);
    void HitStub(Game::Entities::MovingSprite *part);  // TinyHitStub (empty)
    void StartHop(Game::Entities::MovingSprite *part); // StartTinyHop

    /* Its entity type's gEntitySpawnFuncs slot (spawn_bosses.cpp, #915):
     * the boss's MovingSprite, moved by a new TinyCtrl. */
    static void Spawn(u32 id, u16 x, u16 y, u16 param); // SpawnTiny
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(TinyCtrl) == 0x4C);

/* The Neo Cortex fight's controller (gCortexBossVtable;
 * src/game/bosses/cortex.cpp). `counter` is the round. It spawns the
 * cannon and the target (crosshair) parts. spawn_bosses.cpp creates it in
 * a 0x24-byte block. */

/* CortexBossCtrl's `state` (#843, docs/flags/boss_ctrl.md). */
#define CORTEX_BOSS_STATE_START 0    // spawns the cannon and the target, then AIM
#define CORTEX_BOSS_STATE_AIM 1      // turns Neo Cortex and the cannon to the target
#define CORTEX_BOSS_STATE_HIT 2      // a fast shot hit a gem (CortexShotCtrl): the round counted
#define CORTEX_BOSS_STATE_DEFEATED 3 // the third: the target stops, the power; sinks out of the level
#define CORTEX_BOSS_STATE_GONE 4     // LevelProgress::RequestRoomExit done: nothing runs
class CortexBossCtrl : public BossCtrl
{
public:
    Game::Entities::MovingSprite *cannon; // 0x1C - SpawnCannon's part
    Game::Entities::MovingSprite *target; // 0x20 - SpawnTarget's part

    CortexBossCtrl(); // CreateCortexBoss
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~CortexBossCtrl(); // DestroyCortexBoss
    void SetState(Game::Entities::MovingSprite *part, s32 next);
    void SpawnCannon(Game::Entities::MovingSprite *part); // SpawnCortexCannon
    void SpawnTarget(Game::Entities::MovingSprite *part); // SpawnCortexTarget

    /* Its entity type's gEntitySpawnFuncs slot (spawn_bosses.cpp, #915):
     * the boss's MovingSprite, moved by a new CortexBossCtrl. */
    static void Spawn(u32 id, u16 x, u16 y, u16 param); // SpawnCortexBoss
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(CortexBossCtrl) == 0x24);

/* The Neo Cortex fight's cannon (gCortexCannonVtable; 0x10 bytes,
 * src/game/bosses/cortex.cpp): hides its part in state 0. */
/* CortexCannonCtrl's `state`: only 0, nothing sets another. */
#define CORTEX_CANNON_STATE_IDLE 0 // contact with the player off
class CortexCannonCtrl : public Game::Entities::Ctrl
{
public:
    CortexCannonCtrl(); // CreateCortexCannonCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~CortexCannonCtrl(); // DestroyCortexCannonCtrl
    void SetState(Game::Entities::MovingSprite *part, s32 next);
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(CortexCannonCtrl) == 0x10);

/* The Neo Cortex fight's target, the crosshair that hops between the
 * player and the platforms (gCortexTargetVtable, 0x40 bytes;
 * src/game/bosses/cortex.cpp). */

/* CortexTargetCtrl's `state` (#843, docs/flags/boss_ctrl.md). The hops
 * (ENTER, HOP, HOP_OFF) and ANIM go on to `nextState`. */
#define CORTEX_TARGET_STATE_START 0   // contact off, then ENTER
#define CORTEX_TARGET_STATE_ENTER 1   // hops to the level's top right, then HOP
#define CORTEX_TARGET_STATE_HOP 2     // a hop across the level, then AIM, or CHASE at the far edge
#define CORTEX_TARGET_STATE_AIM 3     // the red crosshair, then FIRE after ANIM
#define CORTEX_TARGET_STATE_FIRE 4    // a slow shot, then HOP
#define CORTEX_TARGET_STATE_CHASE 5   // hops after the player, blinking; a fast shot, then ENTER
#define CORTEX_TARGET_STATE_HOP_OFF 6 // LEAVE's hop off the bottom, then GONE
#define CORTEX_TARGET_STATE_ANIM 7    // the animation plays out, then nextState
#define CORTEX_TARGET_STATE_LEAVE 8   // aims below the level, then HOP_OFF
#define CORTEX_TARGET_STATE_STOP 9    // Neo Cortex is beaten (CORTEX_BOSS_STATE_DEFEATED): LEAVE
#define CORTEX_TARGET_STATE_GONE 10   // the end: nothing runs
class CortexTargetCtrl : public Game::Entities::Ctrl
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
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~CortexTargetCtrl(); // DestroyCortexTargetCtrl
    void SetPlatformsKind(u8 flag);
    void SetDest(Game::Entities::MovingSprite *part, s32 x, s32 y);
    void SetState(Game::Entities::MovingSprite *part, s32 next); // SetCortexTargetState
    void FireShot(Game::Entities::MovingSprite *part, s32 kind); // FireCortexShot
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(CortexTargetCtrl) == 0x40);

/* The target's shot (gCortexShotVtable, 0x18 bytes; src/game/bosses/cortex.cpp):
 * a slow one (kind 0) hurts the player, a fast one (1) also shrinks the
 * Neo Cortex gems it hits (CortexBossGemCtrl) and tells the boss. */
/* FireShot's `kind` (#843, docs/flags/boss_ctrl.md). */
#define CORTEX_SHOT_KIND_SLOW 0 // CORTEX_TARGET_STATE_FIRE: animation 0xE, SFX_CORTEX_SHOT
#define CORTEX_SHOT_KIND_FAST 1 // the end of CHASE: animation 0x11, SFX_CORTEX_SHOT_FAST; shrinks gems
class CortexShotCtrl : public Game::Entities::Ctrl
{
public:
    u8 fast;              // 0x10 - CORTEX_SHOT_KIND_FAST (FireShot)
    CortexBossCtrl *boss; // 0x14

    CortexShotCtrl(CortexBossCtrl *boss); // CreateCortexShotCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~CortexShotCtrl(); // DestroyCortexShotCtrl
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(CortexShotCtrl) == 0x18);

/* A gem of the Neo Cortex fight (gCortexBossGemVtable, 0x14 bytes;
 * src/game/bosses/cortex.cpp, SpawnCortexBossGem): once a fast shot hits it
 * (the part's `kind` 1), it plays its shrink animation and goes. */
/* CortexBossGemCtrl's `state`. */
#define CORTEX_BOSS_GEM_STATE_WAIT 0   // until a fast shot hits it (the part's `kind`)
#define CORTEX_BOSS_GEM_STATE_SHRINK 1 // the shrink animation, then gone
class CortexBossGemCtrl : public Game::Entities::Ctrl
{
public:
    s32 kind; // 0x10 - CORTEX_BOSS_GEM_KIND_* (bosses.h)

    CortexBossGemCtrl(s32 kind); // CreateCortexBossGemCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~CortexBossGemCtrl(); // DestroyCortexBossGemCtrl
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(CortexBossGemCtrl) == 0x14);

/* gUnusedOneShotAnimCtrlVtable's class (src/game/bosses/tiny.cpp): does what
 * OneShotAnimCtrl does. Its constructor has no caller (UNUSED). */
class UnusedOneShotAnimCtrl : public Game::Entities::Ctrl
{
public:
    UnusedOneShotAnimCtrl(); // CreateUnusedOneShotAnimCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~UnusedOneShotAnimCtrl(); // DestroyUnusedOneShotAnimCtrl
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(UnusedOneShotAnimCtrl) == 0x10);

/* Dingodile (gDingodileVtable, src/game/bosses/dingodile.cpp and
 * dingodile_create.cpp): he walks the level, stopping at the approach
 * tables' x positions to fire a rocket, turns round at the level's ends
 * and hides behind his shield (`shield`). `counter` is the hits he has
 * taken. spawn_bosses.cpp creates him in a 0x30-byte block. */

/* DingodileCtrl's `state` (#843, docs/flags/boss_ctrl.md): what Update
 * runs, entered by SetState. */
#define DINGODILE_STATE_START 0        // contact and vulnerability off, then WALK
#define DINGODILE_STATE_WALK 1         // to the approach table's next stop (FIRE) or the edge (TURN)
#define DINGODILE_STATE_WAIT 2         // `timer` runs out, then nextState
#define DINGODILE_STATE_TURN 3         // the turn animation, then WAIT before the next pass
#define DINGODILE_STATE_UNK4 4         // animation 1, then WALK; nothing enters it
#define DINGODILE_STATE_FIRE 5         // a rocket at frame 0x14, then WALK on
#define DINGODILE_STATE_FIRE_TURN 6    // the pass's last rocket, then vulnerable and TURN
#define DINGODILE_STATE_STUNNED 7      // the stalactite landed on him (DingodileProjectileCtrl)
#define DINGODILE_STATE_DAZED 8        // 0xD2 frames, the shield blinking: a spin attack is a HIT
#define DINGODILE_STATE_RECOVER 9      // animation 2, then WALK
#define DINGODILE_STATE_RECOVER_HIT 10 // animation 2 after a hit, then RETREAT
#define DINGODILE_STATE_HIT 11         // SFX_BOSS_HIT, the hit counted; the third: DEFEATED
#define DINGODILE_STATE_RETREAT 12     // three sharks; walks to near the level's edge
#define DINGODILE_STATE_TURN_BACK 13   // the turn animation, then WALK_BACK
#define DINGODILE_STATE_WALK_BACK 14   // walks to the edge without stopping, then TURN
#define DINGODILE_STATE_SHARK 15       // a shark from behind him, then WALK
#define DINGODILE_STATE_DEFEATED 16    // the power-up; falls out of the level
#define DINGODILE_STATE_GONE 17        // fell off: LevelProgress::RequestRoomExit done, nothing runs
/* SpawnShieldOrRocket's `mode` (#843, docs/flags/boss_ctrl.md). */
#define DINGODILE_SPAWN_MODE_SHIELD 0 // the constructor: the shield (DingodileShieldCtrl), in gTouchableList
#define DINGODILE_SPAWN_MODE_ROCKET 1 // FIRE/FIRE_TURN: a rocket (DingodileProjectileCtrl), in gCollidableList
class DingodileCtrl : public BossCtrl
{
public:
    s32 step;      // 0x1C - the approach table's index
    s32 timer;     // 0x20
    s32 nextState; // 0x24
    s32 passes;    // 0x28
    Game::Entities::MovingSprite
        *shield; // 0x2C - SpawnShieldOrRocket's DINGODILE_SPAWN_MODE_SHIELD part

    DingodileCtrl(u32 x, u32 y); // CreateDingodile
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~DingodileCtrl(); // DestroyDingodile
    s32 GetHits();
    void SetState(Game::Entities::MovingSprite *part, s32 next);
    void SpawnShieldOrRocket(s32 mode, u16 x, u16 y, Game::Entities::MovingSprite *owner);
    void SpawnShark(u16 x, u16 y, u8 facing);
    void StartMotion(Game::Entities::MovingSprite *part, s32 index);
    void SetStep(s32 value);
    void SetNextState(s32 value);

    /* Its entity type's gEntitySpawnFuncs slot (spawn_bosses.cpp, #915):
     * the boss's MovingSprite, moved by a new DingodileCtrl. */
    static void Spawn(u32 id, u16 x, u16 y, u16 param); // SpawnDingodile
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(DingodileCtrl) == 0x30);

/* Dingodile's shield (gDingodileShieldVtable, 0x28 bytes): hurts the
 * player on contact, and blinks twice when Dingodile is hit. */
/* DingodileShieldCtrl's `state`: Dingodile sets 1 and 2 (SetMode) to
 * make it blink twice; 1/3 and 2/4 run the same code. */
#define DINGODILE_SHIELD_STATE_START 0             // the alpha blending on, then STEADY
#define DINGODILE_SHIELD_STATE_BLINK_START 1       // DINGODILE_STATE_RECOVER(_HIT): two blinks
#define DINGODILE_SHIELD_STATE_BLINK_START_DAZED 2 // DINGODILE_STATE_DAZED: two blinks
#define DINGODILE_SHIELD_STATE_BLINK 3             // toggles the blink bit every 20 frames, then STEADY
#define DINGODILE_SHIELD_STATE_BLINK_DAZED 4       // the same
#define DINGODILE_SHIELD_STATE_STEADY 5            // nothing more
class DingodileShieldCtrl : public BossCtrl
{
public:
    s32 blinkTimer;                      // 0x1C
    s32 blinksLeft;                      // 0x20
    Game::Entities::MovingSprite *owner; // 0x24 - Dingodile's part

    DingodileShieldCtrl(); // CreateDingodileShieldCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~DingodileShieldCtrl(); // DestroyDingodileShieldCtrl
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(DingodileShieldCtrl) == 0x28);

/* Dingodile's rocket, and the stalactite it drops
 * (gDingodileProjectileVtable, 0x20 bytes): both hurt the player, and
 * the stalactite hurts Dingodile (`owner`) if it lands on him. */
/* DingodileProjectileCtrl's `state`: the rocket (0-2) and the
 * stalactite it spawns (5, 3). */
#define DINGODILE_PROJECTILE_STATE_LAUNCH 0 // the rocket: the rise motion, then RISE
#define DINGODILE_PROJECTILE_STATE_RISE 1   // at the top, spawns the stalactite, then SPENT
#define DINGODILE_PROJECTILE_STATE_SPENT 2  // the animation plays out, then gone
#define DINGODILE_PROJECTILE_STATE_FALL 3   // the stalactite falls: BREAK on the player or the floor
#define DINGODILE_PROJECTILE_STATE_UNK4 4   // falls as FALL, but breaks only on the floor; nothing enters it
#define DINGODILE_PROJECTILE_STATE_DROP 5   // the stalactite (SpawnStalactite): the fall, then FALL
#define DINGODILE_PROJECTILE_STATE_BREAK 6  // stops; the animation plays out, then gone
class DingodileProjectileCtrl : public BossCtrl
{
public:
    Game::Entities::MovingSprite *owner; // 0x1C - Dingodile's part

    DingodileProjectileCtrl(); // CreateDingodileProjectileCtrl
    /* The stalactite's, from its rocket's (SpawnStalactite): inline, as
     * the ROM has it there */
    DingodileProjectileCtrl(DingodileProjectileCtrl *rocket)
    {
        owner = rocket->owner;
    }
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~DingodileProjectileCtrl(); // DestroyDingodileProjectileCtrl
    void SpawnStalactite(u16 x, u16 y);
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(DingodileProjectileCtrl) == 0x20);

/* The shark Dingodile's fight sends across the level
 * (gDingodileSharkVtable): an enemy controller in a 0x8C-byte block. */
/* DingodileSharkCtrl's `state` (Ctrl::state, not EnemyCtrl's; #843,
 * docs/flags/boss_ctrl.md): the constructor leaves it 0 and nothing sets
 * another value. */
#define DINGODILE_SHARK_STATE_START 0 // sets the motion across the level (record 3) every frame
#define DINGODILE_SHARK_STATE_UNK1 1  // gone once past the level's edge; nothing enters it
class DingodileSharkCtrl : public Game::Enemies::EnemyCtrl
{
public:
    DingodileSharkCtrl(); // CreateDingodileSharkCtrl
    virtual void Update(Game::Entities::MovingSprite *part);
    virtual ~DingodileSharkCtrl(); // DestroyDingodileSharkCtrl
};

LAYOUT_ASSERT(boss_ctrl_hpp, sizeof(DingodileSharkCtrl) == 0x8C);

} // namespace Bosses
} // namespace Game

#endif /* !GUARD_BOSS_CTRL_HPP */

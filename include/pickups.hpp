#ifndef GUARD_PICKUPS_HPP
#define GUARD_PICKUPS_HPP

/* Part 7h of the C++ conversion (#664, docs/cplusplus.md): the pickups.
 *
 *   ExtraLife  0x54  gExtraLifeVtable (14 slots) src/pickups/extra_life.cpp
 *   Wumpa      0x54  gWumpaVtable     (14 slots) src/pickups/wumpa.cpp,
 *                                                wumpa_update.cpp
 *   Stopwatch  0x40  gStopwatchVtable (13 slots) src/pickups/stopwatch.cpp
 *
 * The sizes are the ROM's: CreateExtraLife and CreateWumpa allocate 0x54
 * bytes, CreateStopwatch 0x40. The extra life and the wumpa have the same
 * fields and a 14th slot of their own, CheckPickup, but no common base:
 * each constructor calls InitSpriteObj (Sprite's) directly, and each
 * destructor DestroySpriteObj.
 *
 * cxx_symbols.txt maps the methods to their C names. Their spawners
 * (src/level/, include/spawners.hpp) are C++ since part 9.
 *
 * No `#pragma interface`: g++ emits the vtables in extra_life.cpp,
 * wumpa.cpp and stopwatch.cpp (see ctrl.hpp). */

#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
}

/* The x offset scale of each hop mode (1-3) of UpdateExtraLifeHop and
 * UpdateWumpaHop (src/data/object_tables_16bb6c.cpp), a struct:
 * ExtraLife::UpdateHop and Wumpa::UpdateHop copy the table onto the
 * stack in one go. */
struct three_words {
    s32 a[3];
};

extern const struct three_words gExtraLifeHopWidths;
extern const struct three_words gWumpaHopWidths;

/* Sets `frame` to the animation's first step: 0, or the last one
 * (frameCount - 1) if that is lower. */
static inline void ClampFrame(Sprite *part)
{
    s32 frame = 0;
    s32 count = part->bank->anims[part->tag].frameCount;

    CLAMP_INDEX(frame, count);
    part->frame = frame;
}

/* `affine` read again, as an `s32`: the ROM reloads it after the store
 * and compares it signed. */
static inline s32 Affine(Sprite *part)
{
    return part->affine;
}

/* The extra life's and the wumpa's `mode` (#843, docs/flags/pickups.md):
 * how it moves on the spot. A hop's x offset scale is
 * gExtraLifeHopWidths/gWumpaHopWidths[mode - 1]. SendToHud reads `mode` as
 * a pixel offset instead (the bounce crate's DropWumpa(..., 0xe, true)). */
#define PICKUP_MODE_BOB 0       // bobs (`counter` steps a sine)
#define PICKUP_MODE_HOP_LEFT 1  // hops to the left (width 0x300)
#define PICKUP_MODE_HOP_RIGHT 2 // hops to the right (width 0: in place)
#define PICKUP_MODE_HOP_UP 3    // hops in place (UpdateHop's else)
#define WUMPA_MODE_PAYOUT 0xff  // the wumpa only: SetHop/Create/DropWumpa start the payout

/* `phase` steps a hop (UpdateHop's sine index); then `mode` goes back to
 * PICKUP_MODE_BOB. Up to PICKUP_PHASE_NO_PICKUP, the hop can't be picked
 * up but on a hover level. */
#define PICKUP_PHASE_NO_PICKUP 0x16
#define PICKUP_PHASE_LAST 0x1f

/* The picked-up pickups' scale (Sprite's `affine`, Q8: 0x100 is 1:1,
 * docs/fixed_point.md). A wumpa flying to the HUD grows by
 * WUMPA_TO_HUD_GROW_STEP a frame up to 1:1. Flying off the screen (the
 * wumpa's and the extra life's state 2), it shrinks by
 * PICKUP_FLY_OFF_SHRINK_STEP a frame until it is under
 * PICKUP_FLY_OFF_MIN_SCALE, or (the other way, `counter` 1) grows by
 * PICKUP_FLY_OFF_GROW_STEP until it is past PICKUP_FLY_OFF_MAX_SCALE, and
 * is gone. */
#define WUMPA_TO_HUD_GROW_STEP     Q8_FRAC(0, 4)
#define PICKUP_FLY_OFF_SHRINK_STEP Q8_FRAC(0, 4)
#define PICKUP_FLY_OFF_MIN_SCALE   Q8(0.25)
#define PICKUP_FLY_OFF_GROW_STEP   Q8_FRAC(0, 0xc)
#define PICKUP_FLY_OFF_MAX_SCALE   Q8(1.6875)

/* ExtraLife's `state`. */
#define EXTRA_LIFE_STATE_IDLE 0    // on the spot: bobs or hops, picked up on contact
#define EXTRA_LIFE_STATE_TO_HUD 1  // flies to the HUD's lives counter, adds a life there
#define EXTRA_LIFE_STATE_FLY_OFF 2 // flies off the screen (`counter`: which way), gone

/* The extra life (gExtraLifeVtable). It bobs on the spot (`counter`
 * steps a sine) or hops (`mode` 1-3, `phase` steps the hop: UpdateHop)
 * until the player touches it (CheckPickup); then it flies off (`state` 1:
 * to the HUD's lives counter, adding a life when it gets there; 2: off
 * the screen), stepping `velX`/`velY` and Sprite's `affine` halfword. */
class ExtraLife : public Sprite
{
public:
    s32 velX;           // 0x40 - the flight's step (Q8 per frame)
    s32 velY;           // 0x44
    u8 state;           // 0x48 - 0 on the spot, 1/2 flying off
    u8 counter;         // 0x49 - the bob's sine index; in state 2, `affine`'s way (0 down)
    u8 mode;            // 0x4A - 0 bob; hop 1: to the left, 2: to the right, 3: in place
    u8 phase;           // 0x4B - the hop's sine index
    struct vec2 anchor; // 0x4C - the home position (Q8)

    ExtraLife();                      // InitExtraLife
    virtual s32 CheckPlayerContact(); // 1 CollideExtraLife
    virtual void Update();            // 3 UpdateExtraLife
    virtual void Draw();              // 4 DrawExtraLife
    virtual s32 GetClassId();         // 9 GetExtraLifeClassId
    virtual ~ExtraLife();             // 10 DestroyExtraLife
    virtual void CheckPickup();       // 13 CheckExtraLifePickup

    /* Create's `new ExtraLife(id, x, y)`, as Wumpa's. */
    ExtraLife(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8(px);
        y = INT_TO_Q8(py);
        anchor = Pos();
    }
    static ExtraLife *Create(u16 id, u16 x, u16 y, s32 unused); // CreateExtraLife
    void PickUp(u8 randomize);                                  // PickUpExtraLife
    void SendToHud();                                           // SendExtraLifeToHud
    void UpdateHop();                                           // UpdateExtraLifeHop
    void Reset();                                               // ResetExtraLifePickup
    void SetHome(s32 x, s32 y);                                 // SetExtraLifePos
    void SetHop(u8 mode);                                       // SetExtraLifeHop
    void SetCounter(u8 value);                                  // SetExtraLifeCounter

    /* The flight's step: the position plus the velocity. `n` holds the
     * new x, then the new y; with a variable per axis, the position
     * takes r0 and the velocity r1, the other way round from the ROM. */
    void Fly()
    {
        s32 n = x + velX;

        x = n;
        n = y + velY;
        y = n;
    }
};

COMPILE_TIME_ASSERT(pickups_hpp, sizeof(ExtraLife) == 0x54);

/* The wumpa fruit (gWumpaVtable): the extra life's fields and behaviour,
 * plus a payout (`state` 3: it stays at the player and drops 10 wumpas
 * that fly to the HUD, one every 11 frames; StartPayout). */
#define WUMPA_STATE_IDLE 0    // on the spot: bobs or hops, picked up on contact
#define WUMPA_STATE_TO_HUD 1  // flies to the HUD's wumpa counter, collected there
#define WUMPA_STATE_FLY_OFF 2 // flies off the screen (`counter`: which way), gone
#define WUMPA_STATE_PAYOUT 3  // stays at the player, dropping 10 wumpas to the HUD
class Wumpa : public Sprite
{
public:
    s32 velX;           // 0x40
    s32 velY;           // 0x44
    u8 state;           // 0x48 - 0 on the spot, 1/2 flying off, 3 paying out
    u8 counter;         // 0x49
    u8 mode;            // 0x4A - 0xFF: start the payout (SetHop)
    u8 phase;           // 0x4B - in state 3, the wumpas dropped
    struct vec2 anchor; // 0x4C

    Wumpa();                          // InitWumpa
    virtual s32 CheckPlayerContact(); // 1 CollideWumpa
    virtual void Update();            // 3 UpdateWumpa
    virtual void Draw();              // 4 DrawWumpa
    virtual s32 GetClassId();         // 9 GetWumpaClassId
    virtual ~Wumpa();                 // 10 DestroyWumpa
    virtual void CheckPickup();       // 13 CheckWumpaPickup (wumpa_update.cpp)

    /* Create's `new Wumpa(id, x, y)`: the constructor inlined, then the
     * spawn's id and position, which is home. */
    Wumpa(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8(px);
        y = INT_TO_Q8(py);
        anchor = Pos();
    }
    static Wumpa *Create(u16 id, u16 x, u16 y, u16 special); // CreateWumpa
    void PickUp(u8 randomize);                               // PickUpWumpa
    void SendToHud();                                        // SendWumpaToHud
    void StartPayout();                                      // StartWumpaPayout
    void UpdateHop();                                        // UpdateWumpaHop
    void Reset();                                            // ResetWumpaPickup
    void SetHome(s32 x, s32 y);                              // SetWumpaPos
    void SetHop(s32 mode);                                   // SetWumpaHop
    void SetCounter(u8 value);                               // SetWumpaCounter

    /* The flight's step, as ExtraLife's. */
    void Fly()
    {
        s32 n = x + velX;

        x = n;
        n = y + velY;
        y = n;
    }
};

COMPILE_TIME_ASSERT(pickups_hpp, sizeof(Wumpa) == 0x54);

/* The time trial's stopwatch (gStopwatchVtable): a plain sprite, updated
 * while the player is within 0x180 pixels and gone once the player is
 * farther. */
class Stopwatch : public Sprite
{
public:
    Stopwatch();           // InitStopwatch
    virtual void Update(); // 3 UpdateStopwatch
    virtual ~Stopwatch();  // 10 DestroyStopwatch

    /* Create's `new Stopwatch(id, x, y)`: the constructor inlined, then
     * the spawn's id and position. */
    Stopwatch(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8(px);
        y = INT_TO_Q8(py);
    }
    static Stopwatch *Create(u16 id, u16 x, u16 y, u16 unused); // CreateStopwatch
    void Reset();                                               // ResetStopwatch
};

COMPILE_TIME_ASSERT(pickups_hpp, sizeof(Stopwatch) == 0x40);

#endif /* !GUARD_PICKUPS_HPP */

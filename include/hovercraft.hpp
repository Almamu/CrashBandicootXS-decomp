#ifndef GUARD_HOVERCRAFT_HPP
#define GUARD_HOVERCRAFT_HPP

/* The hovercraft boss (#772, docs/cplusplus.md "The hovercraft as an
 * all-static class"): src/bosses/hovercraft.cpp and hovercraft_state.cpp. A class
 * whose members are all `static`: the ROM loads a separate address for
 * each of its variables, so they were separate globals, not one object's
 * fields, and static data members compile exactly like them (and static
 * member functions like plain functions). The data members are only
 * declared: cxx_symbols.txt (block "#772 Hovercraft") maps each mangled
 * name to its C name, whose address is in sym_iwram.txt; the functions
 * keep their C names the same way. Its weapons (include/boss_actors.hpp)
 * read it through the public getters.
 *
 * The ROM data it reads stays C globals of src/data/ (bosses.h):
 * gHovercraftPalette, gHovercraftPicture, gHovercraftAttacks,
 * gHovercraftBox and gHovercraftKeyframes. The actor category table
 * (src/data/actor_category_175558.c, C) names Create, Update, UpdateBg2,
 * Destroy and LoadGraphics by their C names (bosses.h). */

#include "actor_self.hpp"

extern "C" {
#include "bosses.h"
}

class Hovercraft
{
public:
    static void Create(s32 lvl);                         // CreateHovercraft
    static void Spawn(s32 kind, s32 sx, s32 sy, s32 sz); // SpawnHovercraft
    static void Update();                                // UpdateHovercraft
    static void UpdateBg2();                             // UpdateHovercraftBg2
    static void LoadGraphics();                          // LoadHovercraftGraphics
    static void Destroy();                               // DestroyHovercraft

    /* What its weapons and the level state read and do. */
    static void StartHitFlash();                        // StartHovercraftHitFlash
    static s32 GetPartsLeft();                          // GetHovercraftPartsLeft
    static void LosePart();                             // LoseHovercraftPart
    static const struct hovercraft_attack *GetAttack(); // GetHovercraftAttack
    static s32 GetState();                              // GetHovercraftState
    static s32 GetLevel();                              // GetHovercraftLevel
    static s32 GetZ();                                  // GetHovercraftZ
    static s32 GetY();                                  // GetHovercraftY
    static s32 GetX();                                  // GetHovercraftX

private:
    static void UpdateHitFlash();         // UpdateHovercraftHitFlash
    static void RunState();               // RunHovercraftState
    static void DrawMap(void *tileRow);   // DrawHovercraftMap
    static void ConvertTiles();           // ConvertHovercraftTiles
    static void SetFlashColor(u8 flag);   // SetHovercraftFlashColor
    static void SetState(s32 a0, s32 a1); // SetHovercraftState

    /* The states, indexed by `state` (stateFuncs). */
    static void StateInactive();    // HovercraftStateInactive
    static void StateApproach();    // HovercraftStateApproach
    static void StateCloseIn();     // HovercraftStateCloseIn
    static void StateFallBack();    // HovercraftStateFallBack
    static void StateExplodeStub(); // HovercraftStateExplodeStub
    static void StateFall();        // HovercraftStateFall

    /* UNUSED, after Destroy (hovercraft.cpp) */
    static void nullsub_34();
    static s32 sub_80337FC();
    static void nullsub_35();

    /* EnterState: state `st` and animation `idx`, keeping the current
     * frame unless it is past the new animation's end (Airship::SetState
     * without the timer). SetState is its out-of-line copy. */
    static void EnterState(s32 st, s32 idx)
    {
        AnimPart *a;

        state = st;
        a = anim;
        a->animIndex = idx;
        a->animTimer = a->anims[idx].duration;
        a->animDone = 0;
        if (a->GetAnimFrameBaseOffset() >= a->anims[a->animIndex].loopThreshold)
            a->animTime = 0;
    }

    /* ApplyFlashColor: colour 15 of the hovercraft's BG and OBJ palettes
     * (gFlashBgPalette, gFlashObjPalette) white when `flag` is set, or
     * else the colour saved the first time. SetFlashColor is its
     * out-of-line copy; RunState inlines it twice. Each branch stores
     * both palettes: that gives the ROM's white loaded into r2 and copied
     * to r1, which a colour local assigned in the branches and stored
     * once after them only got with an r1 pin (#662 round 2). */
    static void ApplyFlashColor(u8 flag)
    {
        if (flashColorSaved == 0) {
            flashSavedColor = gFlashBgPalette[15];
            flashColorSaved = 1;
        }
        if (flag != 0) {
            gFlashBgPalette[15] = RGB_WHITE;
            gFlashObjPalette[15] = RGB_WHITE;
        } else {
            gFlashBgPalette[15] = flashSavedColor;
            gFlashObjPalette[15] = flashSavedColor;
        }
    }

    /* The state functions (gHovercraftStateFuncs,
     * src/data/actor_state_17c4c8.cpp), a plain function table. */
    static void (*const stateFuncs[6])();

    /* IWRAM 0x1590-0x1600, in address order (sym_iwram.txt) */
    static u16 flashSavedColor;                    // gHovercraftFlashSavedColor
    static s32 flashColorSaved;                    // gHovercraftFlashColorSaved
    static s32 bg2Page;                            // gHovercraftBg2Page
    static u8 bg2PageFlip;                         // gHovercraftBg2PageFlip
    static s32 mapCols;                            // gHovercraftMapCols
    static s32 mapRows;                            // gHovercraftMapRows
    static s32 mapTileBase;                        // gHovercraftMapTileBase
    static AnimPart *anim;                         // gHovercraft
    static s32 state;                              // gHovercraftState
    static s32 x;                                  // gHovercraftX
    static s32 y;                                  // gHovercraftY
    static s32 z;                                  // gHovercraftZ
    static s32 screenX;                            // gHovercraftScreenX
    static s32 screenY;                            // gHovercraftScreenY
    static s32 distance;                           // gHovercraftDistance
    static s32 velX;                               // gHovercraftVelX
    static s32 velY;                               // gHovercraftVelY
    static s32 velZ;                               // gHovercraftVelZ
    static s32 level;                              // gHovercraftLevel
    static const struct hovercraft_attack *attack; // gHovercraftAttack
    /* The twins of the airship's gAirshipHp/gAirshipFireTimer/
     * gAirshipVolleyCount (same place in the same IWRAM layout, set the
     * same way by Spawn and the states), but nothing reads them: the
     * hovercraft's parts keep their own hit points and spawn timers. */
    static s32 hp;            // gHovercraftHp
    static s32 fireTimer;     // gHovercraftFireTimer
    static s32 volleyCount;   // gHovercraftVolleyCount
    static s32 phase;         // gHovercraftPhase
    static s32 orbitRadius;   // gHovercraftOrbitRadius
    static s32 frameCount;    // gHovercraftFrameCount
    static s32 partsLeft;     // gHovercraftPartsLeft
    static s16 hitFlashTimer; // gHovercraftHitFlashTimer
    static u8 hitFlashOn;     // gHovercraftHitFlashOn
    static u8 gone;           // gHovercraftGone
    /* The picture's row addresses, as the AnimPart's u32 frame offsets
     * (ConvertTiles fills them; a u32 store may alias its heights[]). */
    static u32 mapFrames[]; // gHovercraftMapFrames
};

#endif // GUARD_HOVERCRAFT_HPP

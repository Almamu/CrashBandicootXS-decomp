#ifndef GUARD_LEVEL_SELECT_HPP
#define GUARD_LEVEL_SELECT_HPP

/* The level select's classes as C++ (#664, docs/cplusplus.md, part 10):
 *
 *   CameraLead          0x80  gCameraLeadVtable       src/objects/camera_lead.cpp
 *   LaunchPad           0x78  gLaunchPadVtable        src/objects/launch_pad.cpp
 *   LevelSelect         0xAC                          src/menus/level_select.cpp,
 *                                                     level_select_pages.cpp
 *   LevelSelectPageBg   0x28                          src/menus/level_select_pages.cpp
 *   ZoomBg              0x8C                          src/menus/level_select_widgets.cpp
 *                                                     (constructor: _pages.cpp)
 *   LevelSelectEntry    0x14  gLevelSelectEntryVtable src/menus/level_select_widgets.cpp
 *   LevelSelectCursor   0x54                          src/menus/level_select_widgets.cpp
 *
 * The sizes are the ROM's (the `new`s in InputCtrl::StateStart,
 * SpawnLaunchPad, RunLevelSelect and InitLevelSelect). Only three have a
 * vtable; the screen's other objects are plain classes with a constructor
 * and a destructor (`delete` calls the destructor with 3, and it frees the
 * object when bit 0 is set).
 *
 * The camera lead and the launch pad are the two moving sprites in this
 * family: the first is spawned by the input controller (InputCtrl, the
 * player's controller in the hover-vehicle rooms), the second by the level
 * spawners (spawn_objects.c's SpawnLaunchPadEntity). The screen's sprites
 * are UiSprites.
 *
 * They have no C views (menus.h's opaque `struct level_menu` tag went in
 * #754); menus.h has the C names of the methods the vtable data uses, and
 * cxx_symbols.txt maps the methods to them.
 *
 * No `#pragma interface`: g++ emits the vtables, CameraLead's in
 * camera_lead.cpp, LaunchPad's in launch_pad.cpp, LevelSelectEntry's in
 * level_select_widgets.cpp (see ctrl.hpp). */

#include "sprite_obj.hpp"
#include "player.hpp"
#include "font.hpp"
#include "graphics_package.hpp"

extern "C" {
#include "core.h"
#include <agb_syscall.h>
#include "gfx.h"
#include "graphics_package.h"
#include "line_util.h"
#include "menus.h"
#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"
#include "level_state.h"
#include "util.h"
#include "system.h"
#include "objects.h"
#include "globals.h"
}

/* The animation set at `offset` in the sprite bank data. */
static inline const struct sprite_bank *AnimTable(s32 offset)
{
    return (const struct sprite_bank *)(SPRITE_BANK_BASE + offset);
}

/* Sets the sprite's bank. `bank` is in Sprite's one-member union (sprite_obj.hpp),
 * and gcc gives every access to a union member alias set 0, so a
 * store to it would make gcc reload everything after it (the ROM's class
 * had a plain member). Through a pointer to the member, the store has the
 * pointer's own alias set. */
static inline void SetBankNow(Sprite *p, const struct sprite_bank *bank)
{
    const struct sprite_bank **field = &p->bank;

    *field = bank;
}

/* Sprite::SetAnim, inlined: animation `anim` from its start. */
static inline void StartAnim(Sprite *p, s32 anim)
{
    p->tag = anim;
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
}

/* Shows step `frame` of the animation, clamped to its last one. */
static inline void ShowFrame(Sprite *p, s32 frame)
{
    s32 n = p->bank->anims[p->tag].frameCount;

    CLAMP_INDEX(frame, n);
    p->frame = frame;
}

/* The camera lead (gCameraLeadVtable): the moving sprite the input
 * controller spawns (InputCtrl::StateStart). It trails the player at a
 * horizontal offset that eases 2 px per frame toward a clamped target, and
 * is gCamera's follow target while it is alive. */
class CameraLead : public MovingSprite
{
public:
    s32 targetOffset; // 0x78 - Q8 x offset from the player, 0xA00-0x3200
    s32 offset;       // 0x7C - eases toward targetOffset

    CameraLead();          // CreateCameraLead
    virtual void Update(); // 3 UpdateCameraLead
    virtual ~CameraLead(); // 10 DestroyCameraLead
    void SetUnk32();       // SetCameraLeadUnk32 (UNUSED)
    void Reset();          // ResetCameraLead
    void SetOffset(s32 v); // SetCameraLeadOffset (UNUSED)
    s32 GetOffset();       // GetCameraLeadOffset (UNUSED)
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(CameraLead) == 0x80);

/* The launch pad (gLaunchPadVtable): the green pad of entity type 0x3D
 * (sprite bank 28). When the player touches it (slot 14), it sends the
 * player event EVENT_LAUNCH_PAD, which makes the action controller launch
 * Crash upward in an air spin with a full tornado charge. */
class LaunchPad : public MovingSprite
{
public:
    LaunchPad();                // InitLaunchPad (UNUSED: Spawn has it inlined)
    virtual ~LaunchPad();       // 10 DestroyLaunchPad
    virtual void TouchPlayer(); // 14 CheckLaunchPadContact

    /* Spawn's `new LaunchPad(id, x, y)`: the constructor inlined, then
     * the spawn's id and position. */
    LaunchPad(u16 id, u16 px, u16 py)
    {
        ClearVulnerable();
        this->id = id;
        x = INT_TO_Q8(px);
        y = INT_TO_Q8(py);
    }
    static LaunchPad *Spawn(u16 id, u16 x, u16 y, u16 unused); // SpawnLaunchPad
    void ClearVulnerable(); // ClearLaunchPadVulnerable: Sprite's, again
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LaunchPad) == 0x78);

/* One level's entry on the level-select page (gLevelSelectEntryVtable):
 * `icon` shows the level's picture (or, past index 4, a per-world
 * animation), `frame` the box around it. The vtable pointer follows the
 * fields, as in every root class. */
class LevelSelectEntry
{
public:
    s32 id;          // 0x00 - the level id (GetLevel)
    u8 selected;     // 0x04
    UiSprite *icon;  // 0x08
    UiSprite *frame; // 0x0C
    // 0x10: the vtable pointer

    LevelSelectEntry();                          // CreateLevelSelectEntry
    virtual void Animate(s32 phase);             // 1 AnimateLevelSelectEntry
    virtual void SetLevel(s32 world, s32 index); // 2 SetLevelSelectEntryLevel
    virtual void SetPos(const struct vec2 *pos); // 3 SetLevelSelectEntryPos
    virtual void Draw();                         // 4 DrawLevelSelectEntry: empty
    virtual ~LevelSelectEntry();                 // 5 DestroyLevelSelectEntry
    u8 IsSelected();                             // IsLevelSelectEntrySelected
    s32 GetLevel();                              // GetLevelSelectEntryLevel
    void SetSelected(u8 value);                  // SetLevelSelectEntrySelected
    void SetBox(s32 kind);                       // SetLevelSelectEntryBox
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelectEntry) == 0x14);

/* BG1, the page strip (CreateLevelSelectPageBg). `bg` is its BG setup
 * (BGxCNT at +0x0C, BgSetup::GetControl). */
class LevelSelectPageBg
{
public:
    BgSetup bg; // 0x00 - BG1
    s32 scroll; // 0x10 - current page scroll, Q8 (0x100 = a page)
    s32 target; // 0x14 - scroll `scroll` eases toward
    u8 unk_18[0x0C];
    u16 hofs; // 0x24 - BG1HOFS (GetOffsets returns hofs|vofs)
    u16 vofs; // 0x26 - BG1VOFS

    LevelSelectPageBg(s32 charBlock, s32 screenBlock); // CreateLevelSelectPageBg
    ~LevelSelectPageBg();                              // DestroyLevelSelectPageBg
    s32 GetScroll();                                   // GetLevelSelectPageBgScroll
    u8 IsSettled();
    void TurnBack();
    void TurnForward();
    void Scroll(); // ScrollLevelSelectPageBg
    u32 GetOffsets();
    void SetOffsets();
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelectPageBg) == 0x28);

/* One twinkle sprite at the picture's corners (ZoomBg::RandomizeTwinkle,
 * TickTwinkle). */
struct Twinkle {
    s32 timer;      // 0x00 - frames until a new frame is picked
    s32 blink;      // 0x04 - blink window, counts down with timer
    UiSprite *part; // 0x08
};

/* REG_BG2CNT's layout in halfword bitfields (ZoomBg::GetControl's side). */
struct ZoomBgCntBits16 {
    u16 priority:2;
    u16 charBase:2;
    u16:2; // bits 4-5, unused by the hardware
    u16 mosaic:1;
    u16 colors256:1;
    u16 screenBase:5;
    u16 wrap:1;
    u16 size:2;
} __attribute__((packed));

/* BG2, the zooming level picture. It zooms out to swap in the selected
 * level's picture (states 1 -> 2 -> 0), wobbles on the sine table while
 * shown (state 3) and zooms away on exit (states 4 -> 5). Four twinkles
 * sit on top of it, each showing a random frame for a random time and
 * blinking along with the wobble. */
class ZoomBg
{
public:
    u8 unk_00[0x0C];
    s32 state;       // 0x0C - see Update
    s32 image;       // 0x10 - gLevelSelectPictures index, 11 = none
    s32 scale;       // 0x14 - zoom, 0x100 = 1:1, 8 = smallest
    s32 charBlock;   // 0x18
    s32 screenBlock; // 0x1C
    s32 x;           // 0x20 - screen centre
    s32 y;           // 0x24
    s32 dx;          // 0x28 - wobble offset
    s32 dy;          // 0x2C
    u32 phase;       // 0x30 - wobble phase, 0-0xFF
    /* The BG2CNT shadow (GetControl). The constructor sets it through
     * byte bitfields (`bits`), StartExit through halfword ones
     * (`bits16`); `packed` keeps the union 2 bytes. */
    union {
        u16 raw;
        struct {
            u8 priority:2; // 0x34
            u8 charBase:2;
            u8:2; // bits 4-5, unused by the hardware
            u8 mosaic:1;
            u8 color256:1;
            u8 screenBase:5; // 0x35
            u8 wrap:1;       // bit 13, the affine wrap-around
            u8 screenSize:2;
        } __attribute__((packed)) bits;
        struct ZoomBgCntBits16 bits16;
    } __attribute__((packed)) bgcnt;
    // 0x38 - BgAffineSet's source: the screen centre is (x, y) plus the
    // wobble, the angle turns on exit
    struct bg_affine_src affine;
    /* BgAffineSet destination, committed by Commit */
    s16 pa;              // 0x4C
    s16 pb;              // 0x4E
    s16 pc;              // 0x50
    s16 pd;              // 0x52
    s32 bgx;             // 0x54
    s32 bgy;             // 0x58
    Twinkle twinkles[4]; // 0x5C

    ZoomBg(s32 charBlock, s32 screenBlock); // InitZoomBg (level_select_pages.cpp)
    ~ZoomBg();                              // DestroyZoomBg
    void Update();                          // UpdateZoomBg
    void Draw();
    void Commit();
    u8 IsExiting();
    u8 IsGone();
    u8 IsShown();
    u8 IsWaiting();
    u8 IsZoomingOut();
    void StartExit();
    void ClearPicture(); // ClearZoomBgPicture
    void SetPicture(s32 image);
    void MoveTwinkle(Twinkle *t); // MoveZoomBgTwinkle
    void RandomizeTwinkle(Twinkle *t);
    void TickTwinkle(Twinkle *t);
    u16 GetControl(); // GetZoomBgControl
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(ZoomBg) == 0x8C);

/* The level-select cursor. It glides between entries along a Bresenham
 * line (two steps per frame), plays an idle animation cycle at random
 * intervals, and grows in (state 4) / shrinks away (state 5) as an affine
 * OBJ drawn straight into the OAM shadow buffer. */
class LevelSelectCursor
{
public:
    struct bresenham_line line;   // 0x00 - (x0, y0) is the position
    UiSprite *part;               // 0x28
    s32 timer;                    // 0x2C - frames to the next idle cycle
    s32 state;                    // 0x30 - see Update
    struct oam_attrs oam;         // 0x34
    s32 speed;                    // 0x3C - zoom step (Move)
    s32 scale;                    // 0x40 - 0x100 = 1:1
    struct obj_affine_src affine; // 0x44 - ObjAffineSet's source
    s16 matrix[4];                // 0x4C - pa, pb, pc, pd

    LevelSelectCursor();  // CreateLevelSelectCursor
    ~LevelSelectCursor(); // DestroyLevelSelectCursor
    void Update();        // UpdateLevelSelectCursor
    void Draw();
    void SetMatrix();
    u8 IsHidden();  // UNUSED
    u8 IsGrowing(); // UNUSED
    void Hide();
    void Park();
    void Glide();
    u8 HasArrived();
    void Move(s32 x, s32 y);       // MoveLevelSelectCursor
    void MoveTo(struct vec2 *pos); // UNUSED (Park has it inlined)
    void SetPos(s32 x, s32 y);     // SetLevelSelectCursorPos
    void ResetIdleTimer();         // ResetLevelSelectCursorIdleTimer

    /* ResetIdleTimer's and MoveTo's bodies, which other methods inline. */
    void ResetIdleTimerNow()
    {
        timer = (u16)RandRange(300) + 600;
    }
    void MoveToNow(struct vec2 *pos)
    {
        Move(pos->x, pos->y);
    }
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelectCursor) == 0x54);

/* The level-select screen (RunLevelSelect): five levels per page (a
 * world), each level's fixed record (name text, three time-trial
 * thresholds) in gLevelTable and its saved record in the save block. The
 * screen keeps shadow copies of BLDCNT/BLDALPHA/BLDY/DISPCNT and commits
 * them with the scroll registers every frame (CommitDisplay). */
class LevelSelect
{
public:
    u8 result;                    // 0x00 - returned by RunLevelSelect
    s32 lastIndex;                // 0x04 - last valid `index` on this page
    s32 index;                    // 0x08 - cursor, 0-5
    s32 world;                    // 0x0C - page
    s32 levelId;                  // 0x10 - gLevelTable index
    s32 nameText;                 // 0x14 - the level name's text
    const struct vec2 *positions; // 0x18 - cursor position per index
    LevelSelectPageBg *bg1;       // 0x1C - BG1, the page strip
    ZoomBg *bg2;                  // 0x20 - BG2, the level picture
    LevelSelectEntry *items[6];   // 0x24
    LevelSelectCursor *panel;     // 0x3C - the cursor
    UiSprite *sprites[10];        // 0x40
    u8 timeText[9];               // 0x68 - best time
    u8 recordText[9];             // 0x71 - next threshold to beat
    u32 scroll;                   // 0x7C - BG0 auto-scroll counter
    s32 panelSlideX;              // 0x80 - x offset of the record panel
    s32 clearedIconY;             // 0x84 - sprite 2's y offset (0 or 0x1C), see LoadRecord
    s32 flag1IconY;               // 0x88 - sprite 3's
    s32 gemIconY;                 // 0x8C - sprite 4's (the `rank` gem icon)
    s32 trialIconY;               // 0x90 - sprite 5's (time-trial icons)
    s32 trialIcon2Y;              // 0x94 - sprite 6's
    s32 rank;                     // 0x98 - LoadRecord's classification, 5 = none
    struct game_progress *save;   // 0x9C - PackSaveData's save block
    union blend blend;            // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;             // 0xA4 - REG_BLDY
    union dispcnt dispcnt;        // 0xA8 - REG_DISPCNT (gfx.h)

    LevelSelect(s32 arg); // InitLevelSelect
    ~LevelSelect();       // DestroyLevelSelect
    void Update();        // UpdateLevelSelect
    void UpdatePageArrows();
    void DrawRecord();
    void DrawTime(u32 time);
    void Draw(); // DrawLevelSelect
    void LoadRecord();
    s32 Loop(); // LevelSelectLoop
    void SettlePage();
    void CursorLeft(); // LevelSelectCursorLeft
    void CursorRight();
    /* level_select_pages.cpp */
    void TurnPage(); // LevelSelectTurnPage
    void WaitCursor();
    void Confirm();
    void Exit();
    u8 HasPrevWorld(); // LevelSelectHasPrevWorld
    u8 IsNextWorldOpen();
    void RefreshPage();
    void PrevWorld();
    void NextWorld();
    void PlaceEntries(); // PlaceLevelSelectEntries
    void LoadEntries();
    void SetEntryBoxes();
    void CommitFrame(); // CommitLevelSelectFrame (UNUSED)
    void ReloadPalette();

    /* The per-frame register commit. */
    void CommitDisplay()
    {
        FlushVramDmaQueue();
        bg2->Commit();
        scroll++;
        *(vu16 *)REG_ADDR_BG0HOFS = scroll >> 3;
        *(vu32 *)REG_ADDR_BG1HOFS = bg1->GetOffsets();
        *(vu16 *)REG_ADDR_BG1CNT = bg1->bg.GetControl();
        *(vu16 *)REG_ADDR_BG2CNT = bg2->GetControl();
        *(vu16 *)PLTT = 0;
        *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
        *(vu16 *)REG_ADDR_BLDY = bldy.evy;
        *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
    }

    /* One frame of the screen: the update, the vblank and the commits. */
    void BeginFrame()
    {
        Update();
        WaitForVBlank();
        gPaletteCache->Upload();
        gOamBuffer->Commit();
        CommitDisplay();
    }

    /* LoadEntries, PlaceEntries and SetEntryBoxes, which TurnPage also has
     * inlined (level_select_pages.cpp). */
    void LoadItems()
    {
        s32 i;

        for (i = 0; i <= 5; i++)
            items[i]->SetLevel(world, i);
    }

    /* Picks the cursor layout (six slots once all five levels of the page
     * are cleared) and places every entry. A counter separate from `i`,
     * and the index taken into a local before the lookup: both are what
     * the ROM's register use and address order need. */
    void PlaceItems()
    {
        s32 n = 0;
        s32 i;

        {
            s32 j;

            for (j = 0; j <= 4; j++) {
                s32 k = world * 5 + j;

                n += save->levels[k].b.cleared;
            }
        }
        if (n == 5) {
            positions = gLevelSelectEntryPositionsAllCleared;
            lastIndex = n;
        } else {
            positions = gLevelSelectEntryPositions;
            lastIndex = 4;
        }
        for (i = 0; i <= 5; i++)
            items[i]->SetPos(&positions[i]);
    }

    void SkinItems()
    {
        s32 i;

        for (i = 0; i <= 5; i++)
            items[i]->SetBox(gLevelSelectWorldEntryBoxAnims[world]);
    }
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelect) == 0xAC);

#endif /* !GUARD_LEVEL_SELECT_HPP */

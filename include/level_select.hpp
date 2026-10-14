#ifndef GUARD_LEVEL_SELECT_HPP
#define GUARD_LEVEL_SELECT_HPP

/* The level select's classes as C++ (#664, docs/cplusplus.md, part 10):
 *
 *   CameraLead          0x80  gCameraLeadVtable       src/objects/camera_lead.cpp
 *   LaunchPad           0x78  gLaunchPadVtable        src/objects/launch_pad.cpp
 *   LevelSelect         0xAC                          src/menus/level_select.cpp,
 *                                                     level_select_pages.cpp
 *   LevelSelectPageBg   0x28                          src/menus/level_select_page_bg.cpp
 *   ZoomBg              0x8C                          src/menus/zoom_bg.cpp
 *   LevelSelectEntry    0x14  gLevelSelectEntryVtable src/menus/level_select_entry.cpp
 *   LevelSelectCursor   0x54                          src/menus/level_select_cursor.cpp
 *
 * The sizes are the ROM's (the `new`s in InputCtrl::StateStart,
 * SpawnLaunchPad, LevelSelect::Run and InitLevelSelect). Only three have a
 * vtable; the screen's other objects are plain classes with a constructor
 * and a destructor (`delete` calls the destructor with 3, and it frees the
 * object when bit 0 is set).
 *
 * The camera lead and the launch pad are the two moving sprites in this
 * family: the first is spawned by the input controller (InputCtrl, the
 * player's controller in the hover-vehicle rooms), the second by the level
 * spawners (spawn_objects.c's LaunchPad::SpawnEntity). The screen's sprites
 * are UiSprites.
 *
 * They have no C views (the opaque `struct level_menu` tag went in
 * #754); cxx_symbols.txt maps the methods to their C names.
 *
 * No `#pragma interface`: g++ emits the vtables, CameraLead's in
 * camera_lead.cpp, LaunchPad's in launch_pad.cpp, LevelSelectEntry's in
 * level_select_entry.cpp (see ctrl.hpp). */

#include "sprite_obj.hpp"
#include "vram_dma_queue.hpp"
#include "player.hpp"
#include "font.hpp"
#include "graphics_package.hpp"
#include "game_progress.hpp"
#include "menus.hpp"
#include "frame_limiter.hpp"

extern "C" {
#include "core.h"
#include <agb_syscall.h>
#include "gfx.h"
#include "line_util.h"
#include "sprite_bank.h"
#include "util.h"
#include "system.h"
#include "objects.h"
#include "globals.h"
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

/* Shows step `frame` of the animation, clamped to its last one. */
static inline void ShowFrame(Sprite *p, s32 frame)
{
    s32 n = p->bank->anims[p->tag].frameCount;

    CLAMP_INDEX(frame, n);
    p->frame = frame;
}

namespace Game
{
namespace Objects
{

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
    /* Entity type ENTITY_LAUNCH_PAD's slot (spawn_objects.cpp, #915): a
     * tail call to Spawn. Not an overload of it: the slot's `u32 id` would
     * make the call inside pick this one. */
    static void SpawnEntity(u32 id, u16 x, u16 y, u16 param); // SpawnLaunchPadEntity
    void ClearVulnerable(); // ClearLaunchPadVulnerable: Sprite's, again
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LaunchPad) == 0x78);

} // namespace Objects
} // namespace Game

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
    Engine::Gfx::BgSetup bg; // 0x00 - BG1
    s32 scroll;              // 0x10 - current page scroll, Q8 (0x100 = a page)
    s32 target;              // 0x14 - scroll `scroll` eases toward
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

/* The level-select zooms (ZoomBg's picture, LevelSelectCursor) are Q8
 * scales, INT_TO_Q8(1) = 1:1 (docs/fixed_point.md). Both start and end
 * at 1/32 of full size; ZoomBg zooms by 1/32 a frame, the cursor by
 * the step its glide sets (LevelSelectCursor::Move). */
#define LEVEL_SELECT_ZOOM_MIN Q8_FRAC(0, 8)  // 1/32: the smallest zoom, not drawn
#define ZOOM_BG_ZOOM_STEP     Q8_FRAC(0, 8)  // ZoomBg's zoom a frame

/* ZoomBg's `state` (#843, docs/flags/level_select.md): what Update does. */
#define ZOOM_BG_STATE_ZOOM_IN 0  // zooms in to 1:1, then SHOWN
#define ZOOM_BG_STATE_ZOOM_OUT 1 // zooms out (ClearPicture), then WAIT
#define ZOOM_BG_STATE_WAIT 2     // waits for a picture (SetPicture), loads it, then ZOOM_IN
#define ZOOM_BG_STATE_SHOWN 3    // shown, wobbling
#define ZOOM_BG_STATE_EXIT 4     // zooms away while turning (StartExit), then GONE
#define ZOOM_BG_STATE_GONE 5     // the end: zoomed away
class ZoomBg
{
public:
    u8 unk_00[0x0C];
    s32 state;       // 0x0C - see Update
    s32 image;       // 0x10 - gLevelSelectPictures index (a theme), ZOOM_BG_NO_PICTURE = none
    s32 scale;       // 0x14 - zoom (Q8): INT_TO_Q8(1) = 1:1, LEVEL_SELECT_ZOOM_MIN = smallest
    s32 charBlock;   // 0x18
    s32 screenBlock; // 0x1C
    screen_x_t x;    // 0x20 - screen centre
    screen_y_t y;    // 0x24
    screen_dx_t dx;  // 0x28 - wobble offset
    screen_dy_t dy;  // 0x2C
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

    ZoomBg(s32 charBlock, s32 screenBlock); // InitZoomBg (zoom_bg.cpp)
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

/* LevelSelectCursor's `state` (#843, docs/flags/level_select.md): 0-3 are
 * the idle animation cycle, 4-5 the affine zoom. */
#define LEVEL_SELECT_CURSOR_STATE_IDLE 0   // gLevelSelectCursorAnims[0]; waits for `timer`
#define LEVEL_SELECT_CURSOR_STATE_CYCLE1 1 // gLevelSelectCursorAnims[1], to its end
#define LEVEL_SELECT_CURSOR_STATE_CYCLE2 2 // gLevelSelectCursorAnims[2], to its end
#define LEVEL_SELECT_CURSOR_STATE_CYCLE3 3 // gLevelSelectCursorAnims[3], to its end, then IDLE
#define LEVEL_SELECT_CURSOR_STATE_GROW 4   // grows in to 1:1 (the constructor), then IDLE
#define LEVEL_SELECT_CURSOR_STATE_SHRINK 5 // shrinks away (Hide)
class LevelSelectCursor
{
public:
    struct bresenham_line line;   // 0x00 - (x0, y0) is the position
    UiSprite *part;               // 0x28
    s32 timer;                    // 0x2C - frames to the next idle cycle
    s32 state;                    // 0x30 - see Update
    struct oam_attrs oam;         // 0x34
    s32 speed;                    // 0x3C - zoom step (Move), Q8
    s32 scale;                    // 0x40 - zoom (Q8): INT_TO_Q8(1) = 1:1
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
    void Move(screen_x_t x, screen_y_t y);   // MoveLevelSelectCursor
    void MoveTo(struct vec2 *pos);           // UNUSED (Park has it inlined)
    void SetPos(screen_x_t x, screen_y_t y); // SetLevelSelectCursorPos
    void ResetIdleTimer();                   // ResetLevelSelectCursorIdleTimer

    /* ResetIdleTimer's and MoveTo's bodies, which other methods inline. */
    void ResetIdleTimerNow()
    {
        timer = RandRange(300) + 600;
    }
    void MoveToNow(struct vec2 *pos)
    {
        Move(pos->x, pos->y);
    }
};

COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelectCursor) == 0x54);

/* The level-select screen (LevelSelect::Run): LEVELS_PER_WORLD levels and
 * the world's boss per page (a world; LEVEL_SELECT_ENTRIES, 5 + 1 with
 * the ROM's levels), each level's fixed record (name text, three
 * time-trial thresholds) in gLevelTable and its saved record in the
 * save block. The screen keeps shadow copies of BLDCNT/BLDALPHA/BLDY/
 * DISPCNT and commits them with the scroll registers every frame
 * (CommitDisplay). */
class LevelSelect
{
public:
    u8 result;                    // 0x00 - returned by Run
    s32 lastIndex;                // 0x04 - last valid `index` on this page
    s32 index;                    // 0x08 - cursor, 0 to LEVEL_SELECT_ENTRIES - 1
    s32 world;                    // 0x0C - page
    s32 levelId;                  // 0x10 - gLevelTable index
    const u8 *nameText;           // 0x14 - the level name's text
    const struct vec2 *positions; // 0x18 - cursor position per index
    LevelSelectPageBg *bg1;       // 0x1C - BG1, the page strip
    ZoomBg *bg2;                  // 0x20 - BG2, the level picture
    // 0x24 - the page's levels, then its boss
    LevelSelectEntry *items[LEVEL_SELECT_ENTRIES];
    LevelSelectCursor *panel;        // 0x3C - the cursor
    UiSprite *sprites[10];           // 0x40
    u8 timeText[9];                  // 0x68 - best time
    u8 recordText[9];                // 0x71 - next threshold to beat
    u32 scroll;                      // 0x7C - BG0 auto-scroll counter
    screen_dx_t panelSlideX;         // 0x80 - x offset of the record panel
    screen_dy_t clearedIconY;        // 0x84 - sprite 2's y offset (0 or 0x1C), see LoadRecord
    screen_dy_t flag1IconY;          // 0x88 - sprite 3's
    screen_dy_t gemIconY;            // 0x8C - sprite 4's (the `rank` gem icon)
    screen_dy_t trialIconY;          // 0x90 - sprite 5's (time-trial icons)
    screen_dy_t trialIcon2Y;         // 0x94 - sprite 6's
    s32 rank;                        // 0x98 - LoadRecord's classification, 5 = none
    Game::Level::GameProgress *save; // 0x9C - PackSaveData's save block
    union blend blend;               // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;                // 0xA4 - REG_BLDY
    union dispcnt dispcnt;           // 0xA8 - REG_DISPCNT (gfx.h)

    LevelSelect(s32 level); // InitLevelSelect
    ~LevelSelect();         // DestroyLevelSelect

    /* The singleton (#915), static: they take no screen. Run
     * (RunLevelSelect, level_select.cpp) is the whole modal screen, for
     * UpdateGameFrame: it builds the screen into `instance`, runs its
     * Loop, returns `result` and the chosen level through `*level`, and
     * deletes it. SetNewWorldOpened (level_select_pages.cpp) sets
     * `newWorldOpened` when a boss's power opens the next world: the next
     * screen then turns to it. */
    static s32 Run(s32 *level); // RunLevelSelect
    static void SetNewWorldOpened();
    void Update(); // UpdateLevelSelect
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
        Engine::Sys::VramDmaQueue::Flush();
        bg2->Commit();
        scroll++;
        *(vu16 *)REG_ADDR_BG0HOFS = scroll >> 3;
        *(vu32 *)REG_ADDR_BG1HOFS = bg1->GetOffsets();
        *(vu16 *)REG_ADDR_BG1CNT = bg1->bg.GetControl();
        *(vu16 *)REG_ADDR_BG2CNT = bg2->GetControl();
        BACKDROP_COLOR = 0;
        *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
        *(vu16 *)REG_ADDR_BLDY = bldy.evy;
        *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
    }

    /* One frame of the screen: the update, the vblank and the commits. */
    void BeginFrame()
    {
        Update();
        Engine::Sys::FrameLimiter::WaitForVBlank();
        gPaletteCache->Upload();
        gOamBuffer->Commit();
        CommitDisplay();
    }

    /* LoadEntries, PlaceEntries and SetEntryBoxes, which TurnPage also has
     * inlined (level_select_pages.cpp). */
    void LoadItems()
    {
        s32 i;

        for (i = 0; i < LEVEL_SELECT_ENTRIES; i++)
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

            for (j = 0; j < LEVELS_PER_WORLD; j++) {
                s32 k = world * LEVELS_PER_WORLD + j;

                n += save->levels[k].b.cleared;
            }
        }
        if (n == LEVELS_PER_WORLD) {
            positions = gLevelSelectEntryPositionsAllCleared;
            lastIndex = n;
        } else {
            positions = gLevelSelectEntryPositions;
            lastIndex = LEVELS_PER_WORLD - 1;
        }
        for (i = 0; i < LEVEL_SELECT_ENTRIES; i++)
            items[i]->SetPos(&positions[i]);
    }

    void SkinItems()
    {
        s32 i;

        for (i = 0; i < LEVEL_SELECT_ENTRIES; i++)
            items[i]->SetBox(gLevelSelectWorldEntryBoxAnims[world]);
    }

private:
    /* Initialised IWRAM globals of their own (iwram_data.cpp;
     * cxx_symbols.txt maps them to their C names), not fields. */
    static LevelSelect *instance; // gLevelSelect: the screen while Run runs
    static u8 newWorldOpened;     // gNewWorldOpened: set by SetNewWorldOpened, cleared by Loop
};

/* 0xAC with the ROM's levels: `items` has LEVEL_SELECT_ENTRIES entries. */
COMPILE_TIME_ASSERT(level_select_hpp, sizeof(LevelSelect) == 0x94 + 4 * LEVEL_SELECT_ENTRIES);

#endif /* !GUARD_LEVEL_SELECT_HPP */

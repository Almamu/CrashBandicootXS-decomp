#ifndef GUARD_FRONTEND_HPP
#define GUARD_FRONTEND_HPP

/* The front end's classes as C++ (#664, docs/cplusplus.md, parts 10b,
 * 10c and 10c-2):
 *
 *   Starfield       0x14                     src/frontend/starfield.cpp
 *   Credits         0x98                     src/frontend/credits.cpp
 *   ContinuePrompt  0x24                     src/menus/continue_prompt*.cpp
 *   TitleScreen     0x220                    src/frontend/title_screen.cpp
 *   CompanyLogos    0x44C                    src/frontend/company_logos.cpp,
 *                                            company_logos_ctor.cpp
 *   LogoActor       0x54   gLogoActorVtable  src/frontend/company_logos.cpp,
 *                                            company_logos_ctor.cpp
 *   LanguageSelect  0x14                     src/frontend/language_select.cpp,
 *                                            language_select_setup.cpp
 *
 * The sizes are the ROM's (the allocations in RunCredits,
 * RunContinuePrompt, UpdateGameFrame, ShowCompanyLogos, RunCompanyLogos,
 * OpenLanguageSelect and InitLanguageSelect). Only the logo actor has a
 * vtable; the others are plain classes with a constructor and a
 * destructor (`delete` calls the destructor with 3, and it frees the
 * object when bit 0 is set).
 *
 * They have no C views (the last, the opaque `struct language_select`
 * and `struct credits_screen` tags, went in #754);
 * cxx_symbols.txt maps the methods to their C names.
 *
 * No `#pragma interface`: g++ emits LogoActor's vtable, the one class here
 * with one, in company_logos_ctor.cpp (see ctrl.hpp). */

#include "actor_self.hpp"
#include "font.hpp"
#include "graphics_package.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "math_util.h"
}

/* One step of a logo piece's motion (the gTitleLogoPieceMotionN and
 * gVvLogoPieceMotionNN tables). When the piece's hold count runs out it
 * loads the next record: a new hold count (0 ends the sequence), three
 * Q16.16 positions, two Q24.8 velocities and the five per-frame deltas
 * added to them while the hold lasts. */
struct delta_record {
    s16 hold;  /* 0x00 - countdown reload value */
    u16 dPosA; /* 0x02 - Q16.16 position (<<16) */
    u16 dPosB; /* 0x04 - Q16.16 position (<<16) */
    /* dPosA/dPosB are a screen x and y, kept narrow (#979): ROM data, read
     * with `ldrh` and shifted into the high halfword of posA/posB, so a
     * negative one is stored as a u16 (65535 for -1). They hold any
     * position of a screen up to 32767 pixels. */
    u16 dPosC;  /* 0x06 - Q16.16 position (<<16) */
    s16 dVelA;  /* 0x08 - x scale, Q8 (<<8 into velA's Q16) */
    s16 dVelB;  /* 0x0a - y scale, Q8 (<<8 into velB's Q16) */
    s32 deltaA; /* 0x0c - raw delta for dPosA's live field */
    s32 deltaB; /* 0x10 - raw delta for dPosB's live field */
    s32 deltaC; /* 0x14 - raw delta for dPosC's live field */
    s32 deltaD; /* 0x18 - raw delta for dVelA's live field */
    s32 deltaE; /* 0x1c - raw delta for dVelB's live field */
};

/* One entry of the {motion, initial hold} seed tables
 * (gTitleLogoPieceSeeds/gVvLogoPieceSeeds) the logo pieces start from. */
struct slot_seed {
    const struct delta_record *record;
    s32 hold;
};

/* The language select, between OpenLanguageSelect and
 * CloseLanguageSelect (src/iwram/iwram_data.cpp): a LanguageSelect
 * (below). */
extern class LanguageSelect *gLanguageSelect;

/* The six language names (src/data/digit_glyphs_17e714.cpp) and the four
 * palettes InitLanguageSelectGraphics copies (palettes_17e72c.cpp,
 * level_tilesets_17e78c.cpp). */
extern const u8 *const gLanguageNames[6];
extern const u16 gLanguageSelectPalette0[16];
extern const u16 gLanguageSelectPalette1[16];
extern const u16 gLanguageSelectPalette2[16];
extern const u16 gLanguageSelectPalette3[16];

/* The company logo actor (InitLogoActor): its animation record
 * (src/data/level_gfx_17cff4.cpp, an anim_table_record) and the two VRAM
 * tile blocks DrawLogoActor
 * double-buffers its frames in (IWRAM, sym_iwram.txt). */
extern const struct anim_table_record gLogoActorAnim; /* actor_anim.h */
extern void *gLogoActorTiles[2];
extern s32 gLogoActorTileBuffer;  /* the block on screen, 0 or 1 */
extern void *gLogoActorLastFrame; /* the frame last unpacked */

/* The animation family RunCompanyLogos loads the logo actor's palette
 * from, and gLogoActorAnim its frames (src/data/anim_family_178f80.cpp). */
extern const u16 gPolarCategoryPalette[0x200];

/* The logo pieces' seed tables (src/data/popup_glyphs_17cf40.cpp,
 * slot_seeds_17d6c0.cpp), NULL-terminated. */
extern const struct slot_seed gTitleLogoPieceSeeds[10];
extern const struct slot_seed gVvLogoPieceSeeds[21];

/* The {x, y} offsets of the eight OAM pieces DrawTitleLogoPieces draws
 * around each of its two slots (src/data/level_gfx_17cff4.cpp). */
extern const s32 gTitleArrowPieceOffsets[8][2];

/* The title screen's graphics (src/data/level_gfx_17cff4.cpp): the menu
 * palettes InitTitleScreen DMAs to OBJ palettes 13-15, the BG2 package
 * LoadTitleScreenBg loads and the four OBJ packages LoadTitleScreenObjTiles
 * loads (gTitleObjPackages, src/iwram/iwram_data.cpp). */
extern const u16 gTitleMenuPalette[16];
extern const u16 gTitleMenuSelectedPalette[16];
extern const u16 gTitleMenuBlinkPalette[16];
extern const struct bg_package gTitleScreenBg;
extern const void *gTitleObjPackages[4];
/* The four packages gTitleObjPackages points at (src/data/level_gfx_17cff4.cpp). */
extern const struct bg_package gTitleCrashObj;
extern const struct bg_package gTitleArrow2Obj;
extern const struct bg_package gTitleArrow1Obj;
extern const struct bg_package gTitleBandicootObj;

/* The company logos' graphics (src/data/slot_seeds_17d6c0.cpp,
 * countdown_17d7a4.cpp). */
extern const struct bg_package gVvLogoEmblemObj;
extern const struct bg_package gVvLogoLettersObj;
extern const struct bg_package gVvLogoUrlObj;
extern const struct bg_package gUniversalLogoBg;

/* The credits (src/data/credits_17c5d0.cpp, popup_glyphs_17cf40.cpp). */
extern const u8 gCreditsText[];
extern const u8 gCreditsEmptyText[4];
extern const struct bg_package gCreditsLogos[5];

/* src/frontend/credits.cpp: the C name of a Credits method
 * (cxx_symbols.txt), which the game loop calls it by, so it keeps C
 * linkage. */
extern "C" void RunCredits(void);

/* src/frontend/language_select.cpp (as above) */
extern "C" s32 RunLanguageSelect(void);

/* src/frontend/language_select_setup.cpp (as above) */
extern "C" void CloseLanguageSelect(void);
extern "C" void OpenLanguageSelect(void);

/* Clears one OAM entry (4 halfwords) with a DMA3 fill from `zero`, a
 * variable the caller declares (company_logos.cpp, title_screen.cpp).
 * A macro, not a function: `zero` is stored before the DMA base is
 * loaded, as in the ROM. */
#define CLEAR_OAM(oam)                                  \
{                                                       \
    zero = 0;                                           \
    DmaFill16From(3, &zero, oam, sizeof(*(oam)));       \
}

/* One star: a 24.8 fixed-point position and its per-frame step. */
struct StarParticle {
    screen_x_q8_t x;
    screen_y_q8_t y;
    screen_dx_q8_t dx;
    screen_dy_q8_t dy;
};

/* The starfield's 4bpp frame: the whole screen, in bytes (0x4B00). */
#define STARFIELD_FRAME_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT / 2)

/* The starfield the language select, the credits and the company logos
 * draw behind their text (starfield.cpp). */
class Starfield
{
public:
    u32 tileVramBase;        // 0x00 - VRAM, where tileBuffer is DMA'd
    u32 mapVramBase;         // 0x04 - BG0's screen base
    StarParticle *particles; // 0x08 - 128 stars
    s32 count;               // 0x0C - stars alive
    u16 *tileBuffer;         // 0x10 - the 4bpp screen buffer

    Starfield();                                          // InitStarfield
    ~Starfield();                                         // DestroyStarfield
    void Draw();                                          // DrawStarfield
    void SpawnStar(s32 idx);                              // SpawnStar
    void PlotPixel(screen_ux_t x, screen_y_t y, s32 val); // PlotStarfieldPixel (unused)
    void Plot(screen_ux_t x, screen_y_t y, s32 val);      // inline: Draw's, PlotPixel's body
    void Update();                                        // UpdateStarfield
    void WaitForButton();                                 // StarfieldWaitForButton
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(Starfield) == 0x14);

/* The credits stream's opcodes (gCreditsText, credits_17c5d0.cpp spells
 * them as CREDITS_PICTURE_n / CREDITS_SMALL / CREDITS_LARGE strings;
 * #843, docs/flags/text.md). */
#define CREDITS_OP_PICTURE 1 // the next byte is a logo's index
#define CREDITS_OP_SMALL 2   // the text that follows in gSmallFont
#define CREDITS_OP_LARGE 3   // ... in gLargeFont

/* CreditsPopup's `mode`: what it draws. */
#define CREDITS_POPUP_MODE_SMALL_CHAR 0 // a character of gSmallFont
#define CREDITS_POPUP_MODE_LARGE_CHAR 1 // a character of gLargeFont
#define CREDITS_POPUP_MODE_LOGO 2       // one of the logos

/* One line of the credits on screen: a run of text, or one of the
 * logos, floating up from the bottom (0x18 bytes). */
struct CreditsPopup {
    CreditsPopup *next; // 0x00
    screen_x_t x;       // 0x04
    screen_y_t y;       // 0x08 - counts down while alive
    screen_h_t height;  // 0x0C - it dies once y + height <= 0
    s32 mode;           // 0x10 - CREDITS_POPUP_MODE_*: a small or large character, a logo
    u8 index;           // 0x14 - the character, or the logo's index
};

/* One of the five logos LoadLogos loads from gCreditsLogos (0x18 bytes). */
struct CreditsLogo {
    s32 cols;    // 0x00 - width in 32-px OAM cells
    s32 rows;    // 0x04 - height in 32-px OAM cells
    s32 height;  // 0x08 - pixel height
    s32 width;   // 0x0C - pixel advance
    s32 palette; // 0x10 - its palette-cache slot
    u8 *tiles;   // 0x14 - its 4bpp tiles, OAM-cell ordered
};

/* The credits screen (RunCredits, run from the title menu and after the
 * ending, game_frame.cpp; 0x98 bytes): a starfield, with the credits text
 * (gCreditsText, a byte stream: characters, 1 <logo>, 2/3 small/large
 * font, '\n' a line) floating up over it, a line at a time. */
class Credits
{
public:
    CreditsPopup *popups; // 0x00 - the lines on screen, oldest first
    const u8 *streamBase; // 0x04 - gCreditsText
    const u8 *stream;     // 0x08 - the next line
    Starfield *starfield; // 0x0C
    s32 largeFont;        // 0x10 - the font of the text to come: 0 small, 1 large
    s32 lineDelay;        // 0x14 - updates to wait before the next line
    u8 unused_18[4];      // 0x18
    CreditsLogo logos[5]; // 0x1C
    u32 frameParity;      // 0x94 - the text moves every other frame

    Credits();          // InitCredits
    ~Credits();         // DestroyCredits
    static void Run();  // RunCredits
    void Loop();        // CreditsLoop
    void DrawText();    // DrawCreditsText
    void UpdateText();  // UpdateCreditsText
    void LoadLogos();   // LoadCreditsLogos
    void CommitFrame(); // CommitCreditsFrame
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(Credits) == 0x98);

/* The continue prompt ("Continue? Yes/No" over the Uka Uka background,
 * 0x24 bytes). Its constructor, InitGraphics and Loop are in
 * src/menus/continue_prompt*.cpp (part 10d); its other methods start
 * credits.cpp. */
class ContinuePrompt
{
public:
    BgSetup *bg1Buf;       // 0x00 - BG1
    BgSetup *bg0Buf;       // 0x04 - BG0
    BgSetup *bg2Buf;       // 0x08 - BG2
    union dispcnt dispcnt; // 0x0C - REG_DISPCNT (gfx.h)
    union blend blend;     // 0x10 - REG_BLDCNT/BLDALPHA; Loop pulses `eva`
    u8 unused_14[4];
    Font *icons;      // 0x18 - gSmallFont
    s32 blinkCounter; // 0x1C - the selected option's blink counter
    s32 selection;    // 0x20 - the Yes/No cursor, 0/1

    ContinuePrompt();      // InitContinuePrompt
    ~ContinuePrompt();     // DestroyContinuePrompt
    static u8 Run();       // RunContinuePrompt
    void InitGraphics();   // InitContinuePromptGraphics
    s32 Loop();            // ContinuePromptLoop
    void Draw();           // DrawContinuePrompt
    s32 Blink(s32 option); // GetContinuePromptBlink
    void CommitFrame();    // CommitContinuePromptFrame
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(ContinuePrompt) == 0x24);

/* One logo piece (0x34 bytes): the title screen has nine, the company
 * logos twenty (the Vicarious Visions logo's). It moves through its
 * motion (struct delta_record, above) a step at a time: while
 * `countdown` runs it adds the five deltas each frame, and when it runs
 * out it loads the next step from `record`. */
struct LogoPiece {
    /* posA/posB are the piece's screen x and y in Q16.16: their integer
     * halves (`h.i`) are s16s, the ROM's halfword reads (#979). */
    u8 active;     // 0x00
    s32 countdown; // 0x04 - frames left in this step
    union {
        s32 q; // 0x08 - Q16.16 x
        struct {
            u16 frac;
            s16 i;
        } h;
    } posA;
    union {
        s32 q; // 0x0C - Q16.16 y
        struct {
            u16 frac;
            s16 i;
        } h;
    } posB;
    s32 posC;                          // 0x10
    s32 velA;                          // 0x14 - x scale (Q16, INT_TO_Q16(1) = 1:1)
    s32 velB;                          // 0x18 - y scale (Q16)
    s32 deltaA;                        // 0x1C - added to posA each frame
    s32 deltaB;                        // 0x20 - to posB
    s32 deltaC;                        // 0x24 - to posC
    s32 deltaD;                        // 0x28 - to velA
    s32 deltaE;                        // 0x2C - to velB
    const struct delta_record *record; // 0x30 - the next motion step
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(LogoPiece) == 0x34);

/* The centre of the title logo's BG2 picture (gTitleScreenBg: 16x16
 * tiles of the affine map, 128x128 pixels), in its own coordinates: the
 * point TitleScreen::Draw puts at the logo's screen position. */
#define TITLE_LOGO_BG_CENTER (16 * 8 / 2)

/* The title screen (InitTitleScreen, RunTitleScreen and
 * DestroyTitleScreen, from game_frame.cpp; 0x220 bytes): the CRASH
 * BANDICOOT logo flying in over the starfield, piece by piece, then the
 * three-item menu. */
class TitleScreen
{
public:
    s32 selection;        // 0x000 - the menu choice Run returns (0-2)
    s32 blinkCounter;     // 0x004 - the selected item blinks with bit 2
    u8 menuShown;         // 0x008 - Draw draws the menu items
    Font *font;           // 0x00C - gSmallFont
    LogoPiece pieces[9];  // 0x010
    s32 landTimer[9];     // 0x1E4 - -1 until the piece lands, then frames to its cue
    Starfield *starfield; // 0x208
    s32 shake;            // 0x20C - frames the BG2 logo keeps shaking
    u32 cheatHash;        // 0x210 - CheatInput's rolling hash
    s32 bgX;              // 0x214 - REG_BG2X
    s32 bgY;              // 0x218 - REG_BG2Y
    s32 bgScale;          // 0x21C - REG_BG2PA/PD

    TitleScreen();                               // InitTitleScreen
    ~TitleScreen();                              // DestroyTitleScreen
    void LoadBg();                               // LoadTitleScreenBg
    void LoadObjTiles();                         // LoadTitleScreenObjTiles
    void UpdateLogoPieces();                     // UpdateTitleLogoPieces
    void DrawLogoPieces();                       // DrawTitleLogoPieces
    u32 CheatInput(u32 pressed);                 // TitleScreenCheatInput
    void HashInput(u32 val);                     // inline: CheatInput's, HashCheatInput's body
    void SeedLogoPieces();                       // inline: Run's seed loop
    s32 Run();                                   // RunTitleScreen
    void CommitFrame();                          // CommitTitleScreenFrame
    void DrawMenuItem(const u8 *text, s32 item); // DrawTitleMenuItem
    void Draw();                                 // DrawTitleScreen
    void HashCheatInput(u32 val);                // HashTitleCheatInput (unused)
    void ResetLogoPieces();                      // ResetTitleLogoPieces (unused)
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(TitleScreen) == 0x220);

/* The 0x44C-byte company-logo screen (ShowCompanyLogos, level_state.cpp):
 * the Vicarious Visions logo's 20 pieces, the frame strip of its first
 * one, then the Universal logo on BG2. Its constructor is empty. */
class CompanyLogos
{
public:
    LogoPiece slots[20]; // 0x000
    u8 sfxPending[0x12]; // 0x410 - per-slot "play the cue once" flags
    u32 tilesA;          // 0x424 - OBJ VRAM tile block (0x1200 bytes)
    u32 tilesB;          // 0x428 - OBJ VRAM tile block (0x400 bytes)
    u32 tilesC;          // 0x42C - OBJ VRAM tile block (0x1000 bytes)
    u8 *frames;          // 0x430 - unpacked frame strip, 0xa00 bytes a frame
    u8 *scratch;         // 0x434 - 0x1000-byte frame build buffer
    s32 frame;           // 0x438 - index into `frames` (0-9)
    s32 loops;           // 0x43C - `frames` passes played, stops at 2
    s32 frameTick;       // 0x440 - ticks on the current frame (0-3)
    s32 fade;            // 0x444 - fade/zoom counter, -1 when idle
    s32 timer;           // 0x448 - -1 while the slots move, then the outro countdown

    CompanyLogos();                                        // InitCompanyLogos
    ~CompanyLogos();                                       // DestroyCompanyLogos
    void Run();                                            // RunCompanyLogos
    void LoadVvLogoGraphics();                             // LoadVvLogoGraphics
    void InitVvLogoPieces();                               // InitVvLogoPieces
    void UpdateVvLogoPieces();                             // UpdateVvLogoPieces
    void DrawVvLogoPieces();                               // DrawVvLogoPieces
    void LoadUniversalLogoBg();                            // LoadUniversalLogoBg
    void LoadAssetBuffered(const void *asset, void *dest); // LoadTaggedAssetBuffered
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(CompanyLogos) == 0x44C);

/* The company logos' 3D actor (gLogoActorVtable): Crash, who walks on
 * between the logos (UpdateLogoActor's states 0-4), double-buffering his
 * frames in the two VRAM tile blocks gLogoActorTiles. */

/* LogoActor's `state` (#843, docs/flags/actor_states_polar.md). */
#define LOGO_ACTOR_STATE_ENTER 0   // the first animation plays through
#define LOGO_ACTOR_STATE_WALK 1    // sequence 1, until its frame 0x12
#define LOGO_ACTOR_STATE_HIT 2     // sequence 7, frozen at its frame 7 (SFX_PLAYER_HURT)
#define LOGO_ACTOR_STATE_FLY_OFF 3 // towards the camera and up, for 16 frames
#define LOGO_ACTOR_STATE_GONE 4    // the end: Draw draws nothing
/* LOGO_ACTOR_STATE_FLY_OFF: he comes towards the camera by this much a
 * frame (Q8, z falls), and goes up by a pixel. */
#define LOGO_ACTOR_FLY_OFF_SPEED_Z Q8_FRAC(0, 0xe)
class LogoActor : public ActorSelf
{
public:
    LogoActor(const struct anim_table_record *anim); // InitLogoActor
    virtual ~LogoActor();                            // 1 DestroyLogoActor
    virtual void Update();                           // 2 UpdateLogoActor
    virtual void Draw();                             // 3 DrawLogoActor

    /* The current frame's graphics (GetAnimFrameData, inlined). */
    u8 *CurFrame()
    {
        s32 base = Q8_TO_INT(animTime);
        s32 idx = animIndex;
        const struct anim_frame_record *table = anims;
        s32 val = table[idx].frameIndex;

        val += base;
        return (u8 *)frameOffsets[val];
    }
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(LogoActor) == 0x54);

/* The language select at boot (OpenLanguageSelect/Run/CloseLanguageSelect,
 * called from MainLoop): up/down cycles `language` through the six
 * entries of gLanguageNames, A or START confirms, and MainLoop stores the
 * result in gLanguage. */
class LanguageSelect
{
public:
    s32 frame;             // 0x00 - frame counter, wraps at 0x100; bit 2 blinks the selection
    u8 done;               // 0x04
    s32 language;          // 0x08 - the selected entry, 0-5
    union dispcnt dispcnt; // 0x0C - the DISPCNT value CommitFrame writes (gfx.h)
    Starfield *starfield;  // 0x10

    LanguageSelect();     // InitLanguageSelect
    ~LanguageSelect();    // DestroyLanguageSelect
    static s32 Run();     // RunLanguageSelect
    static void Open();   // OpenLanguageSelect
    static void Close();  // CloseLanguageSelect
    void Input(u32 keys); // LanguageSelectInput
    void Draw();          // DrawLanguageSelect
    void InitGraphics();  // InitLanguageSelectGraphics
    void LoadBg();        // LoadLanguageSelectBg
    s32 Blink();          // LanguageSelectBlink
    void CommitFrame();   // CommitLanguageSelectFrame
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(LanguageSelect) == 0x14);

#endif /* GUARD_FRONTEND_HPP */

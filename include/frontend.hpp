#ifndef GUARD_FRONTEND_HPP
#define GUARD_FRONTEND_HPP

/* The front end's classes as C++ (#664, docs/cplusplus.md, parts 10b
 * and 10c):
 *
 *   Starfield       0x14                     src/frontend/starfield.cpp
 *   Credits         0x98                     src/frontend/credits.cpp
 *   ContinuePrompt  0x24                     src/frontend/credits.cpp,
 *                                            src/menus/continue_prompt*.cpp
 *   CompanyLogos    0x44C                    src/frontend/company_logos.cpp,
 *                                            language_select.cpp
 *                                            (and title_screen.c, still C)
 *   LogoActor       0x54   gLogoActorVtable  src/frontend/company_logos.cpp,
 *                                            language_select.cpp
 *   LanguageSelect  0x14                     src/frontend/language_select.cpp,
 *                                            language_select_setup.cpp
 *
 * The sizes are the ROM's (the allocations in RunCredits,
 * RunContinuePrompt, ShowCompanyLogos, RunCompanyLogos,
 * OpenLanguageSelect and InitLanguageSelect). Only the logo actor has a
 * vtable; the others are plain classes with a constructor and a
 * destructor (`delete` calls the destructor with 3, and it frees the
 * object when bit 0 is set).
 *
 * The C files see these objects through C structs (frontend.h's struct
 * logo_screen and struct actor_self; struct language_select and struct
 * credits_screen are opaque tags) and prototypes; cxx_symbols.txt maps
 * the methods to those names.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

#include "actor_self.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "logo_screen.h"
#include "math_util.h"
#include "frontend.h"
#include "menus.h"
}

/* One star: a 24.8 fixed-point position and its per-frame step. */
struct StarParticle {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
};

/* The starfield the language select, the credits and the company logos
 * draw behind their text (starfield.cpp). */
class Starfield
{
public:
    u32 tileVramBase;        // 0x00 - VRAM, where tileBuffer is DMA'd
    u32 mapVramBase;         // 0x04 - BG0's screen base
    StarParticle *particles; // 0x08 - 128 stars
    s32 count;               // 0x0C - stars alive
    u8 *tileBuffer;          // 0x10 - the 4bpp screen buffer

    Starfield();                           // InitStarfield
    ~Starfield();                          // DestroyStarfield
    void Draw();                           // DrawStarfield
    void SpawnStar(s32 idx);               // SpawnStar
    void PlotPixel(u32 x, s32 y, s32 val); // PlotStarfieldPixel (unused)
    void Plot(u32 x, s32 y, s32 val);      // inline: Draw's, PlotPixel's body
    void Update();                         // UpdateStarfield
    void WaitForButton();                  // StarfieldWaitForButton
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(Starfield) == 0x14);

/* One line of the credits on screen: a run of text, or one of the
 * logos, floating up from the bottom (0x18 bytes). */
struct CreditsPopup {
    CreditsPopup *next; // 0x00
    s32 x;              // 0x04
    s32 y;              // 0x08 - counts down while alive
    s32 height;         // 0x0C - it dies once y + height <= 0
    s32 mode;           // 0x10 - 0/1: a character of gSmallFont/gLargeFont, 2: a logo
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
 * ending, game_frame.c; 0x98 bytes): a starfield, with the credits text
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
    struct bg_setup *bg1Buf; // 0x00 - BG1
    struct bg_setup *bg0Buf; // 0x04 - BG0
    struct bg_setup *bg2Buf; // 0x08 - BG2
    union {
        u16 raw;
        struct dispcnt_bits bits;
    } dispcnt;         // 0x0C - REG_DISPCNT (a word: a struct is 4-aligned)
    union blend blend; // 0x10 - REG_BLDCNT/BLDALPHA; Loop pulses `eva`
    u8 unused_14[4];
    struct bitmap_font *icons; // 0x18 - gSmallFont
    s32 blinkCounter;          // 0x1C - the selected option's blink counter
    s32 selection;             // 0x20 - the Yes/No cursor, 0/1

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

/* The 0x44C-byte company-logo screen (ShowCompanyLogos, level_state.c):
 * the Vicarious Visions logo's 20 pieces (struct logo_piece), the frame
 * strip of its last one, then the Universal logo on BG2. Its constructor
 * is empty. */
class CompanyLogos
{
public:
    struct logo_piece slots[20]; // 0x000
    u8 sfxPending[0x12];         // 0x410 - per-slot "play the cue once" flags
    u8 pad_422[2];
    u32 tilesA;    // 0x424 - OBJ VRAM tile block (0x1200 bytes)
    u32 tilesB;    // 0x428 - OBJ VRAM tile block (0x400 bytes)
    u32 tilesC;    // 0x42C - OBJ VRAM tile block (0x1000 bytes)
    u8 *frames;    // 0x430 - unpacked frame strip, 0xa00 bytes a frame
    u8 *scratch;   // 0x434 - 0x1000-byte frame build buffer
    s32 frame;     // 0x438 - index into `frames` (0-9)
    s32 loops;     // 0x43C - `frames` passes played, stops at 2
    s32 frameTick; // 0x440 - ticks on the current frame (0-3)
    s32 fade;      // 0x444 - fade/zoom counter, -1 when idle
    s32 timer;     // 0x448 - -1 while the slots move, then the outro countdown

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

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(CompanyLogos) == sizeof(struct logo_screen));

/* The company logos' 3D actor (gLogoActorVtable): Crash, who walks on
 * between the logos (UpdateLogoActor's states 0-4), double-buffering his
 * frames in the two VRAM tile blocks gLogoActorTiles. */
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
        struct anim_frame_record *table = anims;
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
    s32 frame; // 0x00 - frame counter, wraps at 0x100; bit 2 blinks the selection
    u8 done;   // 0x04
    u8 pad_5[3];
    s32 language; // 0x08 - the selected entry, 0-5
    union {
        u16 raw;
        struct dispcnt_bits bits;
    } dispcnt;            // 0x0C - the DISPCNT value CommitFrame writes
    Starfield *starfield; // 0x10

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

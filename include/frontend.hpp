#ifndef GUARD_FRONTEND_HPP
#define GUARD_FRONTEND_HPP

/* The front end's classes as C++ (#664, docs/cplusplus.md, part 10b):
 *
 *   CompanyLogos    0x44C                    src/frontend/company_logos.cpp,
 *                                            language_select.cpp
 *                                            (and title_screen.c, still C)
 *   LogoActor       0x54   gLogoActorVtable  src/frontend/company_logos.cpp,
 *                                            language_select.cpp
 *   LanguageSelect  0x14                     src/frontend/language_select.cpp,
 *                                            language_select_setup.cpp
 *   Starfield       0x14                     src/frontend/starfield.c (still C)
 *
 * The sizes are the ROM's (ShowCompanyLogos's, RunCompanyLogos's,
 * OpenLanguageSelect's and InitLanguageSelect's allocations). Only the
 * logo actor has a vtable; the others are plain classes with a
 * constructor and a destructor (`delete` calls the destructor with 3,
 * and it frees the object when bit 0 is set).
 *
 * The C files see these objects through frontend.h's C structs (struct
 * logo_screen, struct actor_self, struct language_select) and
 * prototypes; cxx_symbols.txt maps the methods to those names.
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
}

/* The starfield the language select, the credits and the company logos
 * draw behind their text (starfield.c's struct particle_bg). */
class Starfield
{
public:
    u32 tileVramBase; // 0x00 - VRAM, where tileBuffer is DMA'd
    u32 mapVramBase;  // 0x04 - BG0's screen base
    void *particles;  // 0x08 - 128 stars, 16 bytes each
    s32 count;        // 0x0C - stars alive
    void *tileBuffer; // 0x10 - the 4bpp screen buffer

    Starfield();   // InitStarfield
    ~Starfield();  // DestroyStarfield
    void Update(); // UpdateStarfield
};

COMPILE_TIME_ASSERT(frontend_hpp, sizeof(Starfield) == 0x14);

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

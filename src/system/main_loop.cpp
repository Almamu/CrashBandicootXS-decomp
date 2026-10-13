#include "level_state.hpp"
#include "frontend.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* The game's top-level loop (called once from `AgbMain`, see
 * src/system/main.cpp): sets up the central per-level state object
 * (`gLevelState`, see docs/rom_map.md's "hud"/"game_loop"
 * investigations for what its fields mean), the boot language menu
 * (`OpenLanguageSelect`/`RunLanguageSelect`/`CloseLanguageSelect`, src/audio/
 * language_select*.c), then runs `UpdateGameFrame` forever, freeing
 * scratch memory before and after each frame. Never actually returns -
 * the `s32` return type only exists to match `AgbMain`'s
 * `if (MainLoop() != 0)` guard, which this loop never reaches. */
s32 MainLoop(void)
{
    gLevelState = LevelState::Get();
    gLevelState->PlayBootCutscene();
    gLevelState->ShowCompanyLogos();
    OpenLanguageSelect();
    gLanguage = RunLanguageSelect();
    CloseLanguageSelect();
    gLevelState->PlayIntroCutscene();

    for (;;) {
        mem_free_bytes(MEM_HEAP_BOTH);
        gLevelState->UpdateGameFrame();
        mem_free_bytes(MEM_HEAP_BOTH);
    }
}

/* UI string `index` in the current language: `gUiTextTables` holds one
 * string table per language (src/data/ui_text_172cd4.cpp), and
 * `gLanguage` (0-5, set above from the language selector
 * `RunLanguageSelect`'s return) picks one. */
const u8 *GetUiText(s32 index)
{
    return gUiTextTables[gLanguage][index];
}

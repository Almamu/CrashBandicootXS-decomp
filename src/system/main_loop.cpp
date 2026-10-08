extern "C" {
#include "core.h"
#include "system.h"
#include "frontend.h"
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
    gLevelState = GetLevelState();
    PlayBootCutscene(gLevelState);
    ShowCompanyLogos(gLevelState);
    OpenLanguageSelect();
    gLanguage = RunLanguageSelect();
    CloseLanguageSelect();
    PlayIntroCutscene(gLevelState);

    for (;;) {
        mem_free_bytes(MEM_HEAP_BOTH);
        UpdateGameFrame(gLevelState);
        mem_free_bytes(MEM_HEAP_BOTH);
    }
}

/* UI string `index` in the current language: `gUiTextTables` holds one
 * string table per language (src/data/ui_text_172cd4.c), and
 * `gLanguage` (0-5, set above from the language selector
 * `RunLanguageSelect`'s return) picks one. */
s32 GetUiText(s32 index)
{
    return (s32)gUiTextTables[gLanguage][index];
}

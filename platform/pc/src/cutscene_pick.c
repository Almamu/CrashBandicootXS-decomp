/* --cutscene N [--language L] (main.c): the boot's cutscene is cutscene N,
 * in language L (gLanguage: 0 English, 1 French, 2 German, 3 Spanish,
 * 4 Italian, 5 Dutch), instead of the legal notice; then the game boots
 * on as it does (the logos, the language select, ...). tools/preview.py's
 * --cutscene (the cutscene editor's "Play this cutscene", docs/editors.md)
 * plays a cutscene this way, without a level that plays it.
 *
 * The link wraps LevelState::PlayBootCutscene (platform.mk, --wrap; MainLoop
 * calls it first, main_loop.cpp), which plays CUTSCENE_LEGAL_NOTICE
 * through LevelState::PlayCutscene: with --cutscene, the wrapper plays N
 * through it instead. Nothing in the game changes. */

#include "gba/gba.h"
#include "host_platform.h"

extern s32 gLanguage;

void __real__ZN4Game5Level10LevelState16PlayBootCutsceneEv(void *self);
void _ZN4Game5Level10LevelState12PlayCutsceneEi(void *self, s32 idx);

void __wrap__ZN4Game5Level10LevelState16PlayBootCutsceneEv(void *self)
{
    if (gHostOptions.cutscene < 0) {
        __real__ZN4Game5Level10LevelState16PlayBootCutsceneEv(self);
        return;
    }
    if (gHostOptions.language >= 0) {
        gLanguage = gHostOptions.language;
    }
    _ZN4Game5Level10LevelState12PlayCutsceneEi(self, gHostOptions.cutscene);
}

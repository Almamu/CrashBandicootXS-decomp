#include "core.h"
#include "frontend.h"

/*
 * ROM 0x0817E714-0x0817E72C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gLanguageNameEnglish[];
extern const u8 gLanguageNameFrench[];
extern const u8 gLanguageNameGerman[];
extern const u8 gLanguageNameSpanish[];
extern const u8 gLanguageNameItalian[];
extern const u8 gLanguageNameDutch[];

/* The six language names the language menu draws (DrawLanguageSelect,
 * language_select.cpp), in countdown_17d7a4.c. */
const u8 *const gLanguageNames[6] = {
    gLanguageNameEnglish,
    gLanguageNameFrench,
    gLanguageNameGerman,
    gLanguageNameSpanish,
    gLanguageNameItalian,
    gLanguageNameDutch,
};

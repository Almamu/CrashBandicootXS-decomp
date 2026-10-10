#include "cutscene.hpp"

extern "C" {
#include "core.h"
#include "constants/sfx.h"
#include "constants/songs.h"
}

/*
 * ROM 0x0816D1C8-0x0816D4D8: the 11 cutscenes. PlayCutscene
 * (level_cutscene.cpp) plays cutscene `idx`: the slides of
 * gCutscenes[idx], each shown with the page of text at the same
 * index in the current language's `struct cutscene_page` array. The six
 * language tables (English first, then French, German, Spanish, Italian,
 * Dutch) are listed by the IWRAM pointer table gCutsceneTexts, indexed
 * by the language setting gLanguage.
 *
 * In ROM order, in four files: this one (the six language tables, the
 * slide lists, the English pages), cutscene_text_16d4d8.c (the English
 * text), cutscene_slides_16e064.cpp (the other five languages' pages and
 * the slides themselves) and cutscene_text_16e8a4.c (their text). Each
 * string comes before the array that lists it; a few are shared. The
 * text files are C (see their comment). The pictures are in
 * src/data/cutscene_pictures_5a9f70.cpp.
 *
 * The text is Latin-1, all in lower case. A page with no strings shows
 * the picture only (RunCutscenePlayer).
 */

extern const struct cutscene_page gCutscene01English[4];
extern const struct cutscene_page gCutscene02English[9];
extern const struct cutscene_page gCutscene03English[1];
extern const struct cutscene_page gCutscene04English[1];
extern const struct cutscene_page gCutscene05English[1];
extern const struct cutscene_page gCutscene06English[2];
extern const struct cutscene_page gCutscene07English[1];
extern const struct cutscene_page gCutscene08English[4];
extern const struct cutscene_page gCutscene09English[6];
extern const struct cutscene_page gCutscene10English[6];
extern const struct cutscene_page gCutscene00English[1];
extern const struct cutscene_slide *const gCutscene00Slides[1];
extern const struct cutscene_slide *const gCutscene01Slides[4];
extern const struct cutscene_slide *const gCutscene02Slides[9];
extern const struct cutscene_slide *const gCutscene03Slides[1];
extern const struct cutscene_slide *const gCutscene06Slides[2];
extern const struct cutscene_slide *const gCutscene04Slides[1];
extern const struct cutscene_slide *const gCutscene05Slides[1];
extern const struct cutscene_slide *const gCutscene07Slides[1];
extern const struct cutscene_slide *const gCutscene08Slides[4];
extern const struct cutscene_slide *const gCutscene09Slides[6];
extern const struct cutscene_slide *const gCutscene10Slides[6];
extern const u8 *const gCutscene01EnglishPage0[1];
extern const u8 *const gCutscene01EnglishPage1[1];
extern const u8 *const gCutscene01EnglishPage2[1];
extern const u8 *const gCutscene01EnglishPage3[2];
extern const u8 *const gCutscene02EnglishPage0[1];
extern const u8 *const gCutscene02EnglishPage1[1];
extern const u8 *const gCutscene02EnglishPage2[1];
extern const u8 *const gCutscene02EnglishPage5[1];
extern const u8 *const gCutscene02EnglishPage7[2];
extern const u8 *const gCutscene02EnglishPage6[1];
extern const u8 *const gCutscene02EnglishPage8[1];
extern const u8 *const gCutscene03EnglishPage0[1];
extern const u8 *const gCutscene04EnglishPage0[1];
extern const u8 *const gCutscene05EnglishPage0[1];
extern const u8 *const gCutscene06EnglishPage0[1];
extern const u8 *const gCutscene06EnglishPage1[1];
extern const u8 *const gCutscene07EnglishPage0[1];
extern const u8 *const gCutscene10EnglishPage0[1];
extern const u8 *const gCutscene08EnglishPage0[1];
extern const u8 *const gCutscene08EnglishPage1[1];
extern const u8 *const gCutscene08EnglishPage2[1];
extern const u8 *const gCutscene08EnglishPage3[1];
extern const u8 *const gCutscene09EnglishPage1[1];
extern const u8 *const gCutscene09EnglishPage5[1];
extern const u8 *const gCutscene10EnglishPage5[1];
extern const struct cutscene_page gCutscene01French[4];
extern const struct cutscene_page gCutscene02French[9];
extern const struct cutscene_page gCutscene03French[1];
extern const struct cutscene_page gCutscene04French[1];
extern const struct cutscene_page gCutscene05French[1];
extern const struct cutscene_page gCutscene06French[2];
extern const struct cutscene_page gCutscene07French[1];
extern const struct cutscene_page gCutscene08French[4];
extern const struct cutscene_page gCutscene09French[6];
extern const struct cutscene_page gCutscene10French[6];
extern const struct cutscene_page gCutscene00French[1];
extern const struct cutscene_page gCutscene01German[4];
extern const struct cutscene_page gCutscene02German[9];
extern const struct cutscene_page gCutscene03German[1];
extern const struct cutscene_page gCutscene04German[1];
extern const struct cutscene_page gCutscene05German[1];
extern const struct cutscene_page gCutscene06German[2];
extern const struct cutscene_page gCutscene07German[1];
extern const struct cutscene_page gCutscene08German[4];
extern const struct cutscene_page gCutscene09German[6];
extern const struct cutscene_page gCutscene10German[6];
extern const struct cutscene_page gCutscene00German[1];
extern const struct cutscene_page gCutscene01Spanish[4];
extern const struct cutscene_page gCutscene02Spanish[9];
extern const struct cutscene_page gCutscene03Spanish[1];
extern const struct cutscene_page gCutscene04Spanish[1];
extern const struct cutscene_page gCutscene05Spanish[1];
extern const struct cutscene_page gCutscene06Spanish[2];
extern const struct cutscene_page gCutscene07Spanish[1];
extern const struct cutscene_page gCutscene08Spanish[4];
extern const struct cutscene_page gCutscene09Spanish[6];
extern const struct cutscene_page gCutscene10Spanish[6];
extern const struct cutscene_page gCutscene00Spanish[1];
extern const struct cutscene_page gCutscene01Italian[4];
extern const struct cutscene_page gCutscene02Italian[9];
extern const struct cutscene_page gCutscene03Italian[1];
extern const struct cutscene_page gCutscene04Italian[1];
extern const struct cutscene_page gCutscene05Italian[1];
extern const struct cutscene_page gCutscene06Italian[2];
extern const struct cutscene_page gCutscene07Italian[1];
extern const struct cutscene_page gCutscene08Italian[4];
extern const struct cutscene_page gCutscene09Italian[6];
extern const struct cutscene_page gCutscene10Italian[6];
extern const struct cutscene_page gCutscene00Italian[1];
extern const struct cutscene_page gCutscene01Dutch[4];
extern const struct cutscene_page gCutscene02Dutch[9];
extern const struct cutscene_page gCutscene03Dutch[1];
extern const struct cutscene_page gCutscene04Dutch[1];
extern const struct cutscene_page gCutscene05Dutch[1];
extern const struct cutscene_page gCutscene06Dutch[2];
extern const struct cutscene_page gCutscene07Dutch[1];
extern const struct cutscene_page gCutscene08Dutch[4];
extern const struct cutscene_page gCutscene09Dutch[6];
extern const struct cutscene_page gCutscene10Dutch[6];
extern const struct cutscene_page gCutscene00Dutch[1];
extern const struct cutscene_slide gCutsceneSlide00;
extern const struct cutscene_slide gCutsceneSlide01;
extern const struct cutscene_slide gCutsceneSlide02;
extern const struct cutscene_slide gCutsceneSlide03;
extern const struct cutscene_slide gCutsceneSlide04;
extern const struct cutscene_slide gCutsceneSlide05;
extern const struct cutscene_slide gCutsceneSlide06;
extern const struct cutscene_slide gCutsceneSlide07;
extern const struct cutscene_slide gCutsceneSlide08;
extern const struct cutscene_slide gCutsceneSlide09;
extern const struct cutscene_slide gCutsceneSlide10;
extern const struct cutscene_slide gCutsceneSlide11;
extern const struct cutscene_slide gCutsceneSlide12;
extern const struct cutscene_slide gCutsceneSlide13;
extern const struct cutscene_slide gCutsceneSlide14;
extern const struct cutscene_slide gCutsceneSlide15;
extern const struct cutscene_slide gCutsceneSlide16;
extern const struct cutscene_slide gCutsceneSlide17;
extern const struct cutscene_slide gCutsceneSlide18;
extern const struct cutscene_slide gCutsceneSlide19;
extern const struct cutscene_slide gCutsceneSlide20;
extern const struct cutscene_slide gCutsceneSlide21;
extern const struct cutscene_slide gCutsceneSlide22;
extern const struct cutscene_slide gCutsceneSlide23;

/* The pages of each cutscene, per language. */
const struct cutscene_page *const gCutsceneTextEnglish[11] = {
    gCutscene00English, gCutscene01English, gCutscene02English, gCutscene03English,
    gCutscene04English, gCutscene05English, gCutscene06English, gCutscene07English,
    gCutscene08English, gCutscene09English, gCutscene10English,
};

/* The slides of each cutscene (PlayCutscene). */
const struct cutscene_slides gCutscenes[11] = {
    { gCutscene00Slides, ARRAY_COUNT(gCutscene00Slides) },
    { gCutscene01Slides, ARRAY_COUNT(gCutscene01Slides) },
    { gCutscene02Slides, ARRAY_COUNT(gCutscene02Slides) },
    { gCutscene03Slides, ARRAY_COUNT(gCutscene03Slides) },
    { gCutscene04Slides, ARRAY_COUNT(gCutscene04Slides) },
    { gCutscene05Slides, ARRAY_COUNT(gCutscene05Slides) },
    { gCutscene06Slides, ARRAY_COUNT(gCutscene06Slides) },
    { gCutscene07Slides, ARRAY_COUNT(gCutscene07Slides) },
    { gCutscene08Slides, ARRAY_COUNT(gCutscene08Slides) },
    { gCutscene09Slides, ARRAY_COUNT(gCutscene09Slides) },
    { gCutscene10Slides, ARRAY_COUNT(gCutscene10Slides) },
};

const struct cutscene_page gCutscene01English[4] = {
    { gCutscene01EnglishPage0, ARRAY_COUNT(gCutscene01EnglishPage0) },
    { gCutscene01EnglishPage1, ARRAY_COUNT(gCutscene01EnglishPage1) },
    { gCutscene01EnglishPage2, ARRAY_COUNT(gCutscene01EnglishPage2) },
    { gCutscene01EnglishPage3, ARRAY_COUNT(gCutscene01EnglishPage3) },
};
const struct cutscene_page gCutscene02English[9] = {
    { gCutscene02EnglishPage0, ARRAY_COUNT(gCutscene02EnglishPage0) },
    { gCutscene02EnglishPage1, ARRAY_COUNT(gCutscene02EnglishPage1) },
    { gCutscene02EnglishPage2, ARRAY_COUNT(gCutscene02EnglishPage2) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene02EnglishPage5, ARRAY_COUNT(gCutscene02EnglishPage5) },
    { gCutscene02EnglishPage6, ARRAY_COUNT(gCutscene02EnglishPage6) },
    { gCutscene02EnglishPage7, ARRAY_COUNT(gCutscene02EnglishPage7) },
    { gCutscene02EnglishPage8, ARRAY_COUNT(gCutscene02EnglishPage8) },
};
const struct cutscene_page gCutscene03English[1] = {
    { gCutscene03EnglishPage0, ARRAY_COUNT(gCutscene03EnglishPage0) },
};
const struct cutscene_page gCutscene04English[1] = {
    { gCutscene04EnglishPage0, ARRAY_COUNT(gCutscene04EnglishPage0) },
};
const struct cutscene_page gCutscene05English[1] = {
    { gCutscene05EnglishPage0, ARRAY_COUNT(gCutscene05EnglishPage0) },
};
const struct cutscene_page gCutscene06English[2] = {
    { gCutscene06EnglishPage0, ARRAY_COUNT(gCutscene06EnglishPage0) },
    { gCutscene06EnglishPage1, ARRAY_COUNT(gCutscene06EnglishPage1) },
};
const struct cutscene_page gCutscene07English[1] = {
    { gCutscene07EnglishPage0, ARRAY_COUNT(gCutscene07EnglishPage0) },
};
const struct cutscene_page gCutscene08English[4] = {
    { gCutscene08EnglishPage0, ARRAY_COUNT(gCutscene08EnglishPage0) },
    { gCutscene08EnglishPage1, ARRAY_COUNT(gCutscene08EnglishPage1) },
    { gCutscene08EnglishPage2, ARRAY_COUNT(gCutscene08EnglishPage2) },
    { gCutscene08EnglishPage3, ARRAY_COUNT(gCutscene08EnglishPage3) },
};
const struct cutscene_page gCutscene09English[6] = {
    { 0, 0 }, { gCutscene09EnglishPage1, ARRAY_COUNT(gCutscene09EnglishPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09EnglishPage5, ARRAY_COUNT(gCutscene09EnglishPage5) },
};
const struct cutscene_page gCutscene10English[6] = {
    { gCutscene10EnglishPage0, ARRAY_COUNT(gCutscene10EnglishPage0) },
    { 0, 0 },
    { gCutscene09EnglishPage1, ARRAY_COUNT(gCutscene09EnglishPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10EnglishPage5, ARRAY_COUNT(gCutscene10EnglishPage5) },
};
const struct cutscene_page gCutscene00English[1] = {
    { 0, 0 },
};
const struct cutscene_page *const gCutsceneTextFrench[11] = {
    gCutscene00French, gCutscene01French, gCutscene02French, gCutscene03French,
    gCutscene04French, gCutscene05French, gCutscene06French, gCutscene07French,
    gCutscene08French, gCutscene09French, gCutscene10French,
};

const struct cutscene_page *const gCutsceneTextGerman[11] = {
    gCutscene00German, gCutscene01German, gCutscene02German, gCutscene03German,
    gCutscene04German, gCutscene05German, gCutscene06German, gCutscene07German,
    gCutscene08German, gCutscene09German, gCutscene10German,
};

const struct cutscene_page *const gCutsceneTextSpanish[11] = {
    gCutscene00Spanish, gCutscene01Spanish, gCutscene02Spanish, gCutscene03Spanish,
    gCutscene04Spanish, gCutscene05Spanish, gCutscene06Spanish, gCutscene07Spanish,
    gCutscene08Spanish, gCutscene09Spanish, gCutscene10Spanish,
};

const struct cutscene_page *const gCutsceneTextItalian[11] = {
    gCutscene00Italian, gCutscene01Italian, gCutscene02Italian, gCutscene03Italian,
    gCutscene04Italian, gCutscene05Italian, gCutscene06Italian, gCutscene07Italian,
    gCutscene08Italian, gCutscene09Italian, gCutscene10Italian,
};

const struct cutscene_page *const gCutsceneTextDutch[11] = {
    gCutscene00Dutch, gCutscene01Dutch, gCutscene02Dutch, gCutscene03Dutch,
    gCutscene04Dutch, gCutscene05Dutch, gCutscene06Dutch, gCutscene07Dutch,
    gCutscene08Dutch, gCutscene09Dutch, gCutscene10Dutch,
};

const struct cutscene_slide *const gCutscene00Slides[1] = { &gCutsceneSlide23 };
const struct cutscene_slide *const gCutscene01Slides[4] = { &gCutsceneSlide00, &gCutsceneSlide01,
                                                            &gCutsceneSlide02, &gCutsceneSlide01 };
const struct cutscene_slide *const gCutscene02Slides[9] = {
    &gCutsceneSlide00, &gCutsceneSlide03, &gCutsceneSlide04, &gCutsceneSlide05, &gCutsceneSlide06,
    &gCutsceneSlide07, &gCutsceneSlide08, &gCutsceneSlide09, &gCutsceneSlide10
};
const struct cutscene_slide *const gCutscene03Slides[1] = { &gCutsceneSlide11 };
const struct cutscene_slide *const gCutscene06Slides[2] = { &gCutsceneSlide01, &gCutsceneSlide02 };
const struct cutscene_slide *const gCutscene04Slides[1] = { &gCutsceneSlide11 };
const struct cutscene_slide *const gCutscene05Slides[1] = { &gCutsceneSlide12 };
const struct cutscene_slide *const gCutscene07Slides[1] = { &gCutsceneSlide12 };
const struct cutscene_slide *const gCutscene08Slides[4] = { &gCutsceneSlide13, &gCutsceneSlide14,
                                                            &gCutsceneSlide15, &gCutsceneSlide16 };
const struct cutscene_slide *const gCutscene09Slides[6] = { &gCutsceneSlide17, &gCutsceneSlide18,
                                                            &gCutsceneSlide19, &gCutsceneSlide20,
                                                            &gCutsceneSlide21, &gCutsceneSlide22 };
const struct cutscene_slide *const gCutscene10Slides[6] = { &gCutsceneSlide13, &gCutsceneSlide17,
                                                            &gCutsceneSlide18, &gCutsceneSlide19,
                                                            &gCutsceneSlide20, &gCutsceneSlide10 };

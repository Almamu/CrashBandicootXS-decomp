#include "core.h"
#include "cutscene.h"

/*
 * ROM 0x0816D1C8-0x081725A8: the 11 cutscenes. PlayCutscene
 * (graphics_loading_22354.c) plays cutscene `idx`: the slides of
 * gCutscenes[idx], each shown with the page of text at the same
 * index in the current language's `struct cutscene_page` array. The six
 * language tables (English first, then French, German, Spanish, Italian,
 * Dutch) are listed by the IWRAM pointer table gCutsceneTexts, indexed
 * by the language setting gLanguage.
 *
 * In ROM order: the English table, the slide lists, the English pages,
 * the other five tables, the slide arrays, then per language its text
 * (each string before the array that lists it, a few strings shared),
 * with the slides themselves between the English and French text. The
 * pictures are in src/data/cutscene_pictures_5a9f70.c.
 *
 * The text is Latin-1, all in lower case. A page with no strings shows
 * the picture only (RunCutscenePlayer).
 */

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

extern const u16 gCutscenePicture00[], gCutscenePicture01[], gCutscenePicture02[], gCutscenePicture03[], gCutscenePicture04[], gCutscenePicture05[], gCutscenePicture06[], gCutscenePicture07[], gCutscenePicture08[], gCutscenePicture09[], gCutscenePicture10[], gCutscenePicture11[], gCutscenePicture12[], gCutscenePicture13[], gCutscenePicture14[], gCutscenePicture15[], gCutscenePicture16[], gCutscenePicture17[], gCutscenePicture18[], gCutscenePicture19[], gCutscenePicture20[], gCutscenePicture21[], gCutscenePicture22[], gCutscenePicture23[];

extern const struct cutscene_page *const gCutsceneTextEnglish[11];
extern const struct cutscene_slides gCutscenes[11];
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
extern const struct cutscene_page *const gCutsceneTextFrench[11];
extern const struct cutscene_page *const gCutsceneTextGerman[11];
extern const struct cutscene_page *const gCutsceneTextSpanish[11];
extern const struct cutscene_page *const gCutsceneTextItalian[11];
extern const struct cutscene_page *const gCutsceneTextDutch[11];
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
extern const u8 gCutscene01EnglishPage0Text[];
extern const u8 *const gCutscene01EnglishPage0[1];
extern const u8 gCutscene01EnglishPage1Text[];
extern const u8 *const gCutscene01EnglishPage1[1];
extern const u8 gCutscene01EnglishPage2Text[];
extern const u8 *const gCutscene01EnglishPage2[1];
extern const u8 gCutscene01EnglishPage3_0Text[];
extern const u8 gCutscene01EnglishPage3_1Text[];
extern const u8 *const gCutscene01EnglishPage3[2];
extern const u8 gCutscene02EnglishPage0Text[];
extern const u8 *const gCutscene02EnglishPage0[1];
extern const u8 gCutscene02EnglishPage1Text[];
extern const u8 *const gCutscene02EnglishPage1[1];
extern const u8 gCutscene02EnglishPage2Text[];
extern const u8 *const gCutscene02EnglishPage2[1];
extern const u8 gCutscene02EnglishPage5Text[];
extern const u8 *const gCutscene02EnglishPage5[1];
extern const u8 gCutscene02EnglishPage7_0Text[];
extern const u8 gCutscene02EnglishPage7_1Text[];
extern const u8 *const gCutscene02EnglishPage7[2];
extern const u8 gCutscene02EnglishPage6Text[];
extern const u8 *const gCutscene02EnglishPage6[1];
extern const u8 gCutscene02EnglishPage8Text[];
extern const u8 *const gCutscene02EnglishPage8[1];
extern const u8 gCutscene03EnglishPage0Text[];
extern const u8 *const gCutscene03EnglishPage0[1];
extern const u8 gCutscene04EnglishPage0Text[];
extern const u8 *const gCutscene04EnglishPage0[1];
extern const u8 gCutscene05EnglishPage0Text[];
extern const u8 *const gCutscene05EnglishPage0[1];
extern const u8 gCutscene06EnglishPage0Text[];
extern const u8 *const gCutscene06EnglishPage0[1];
extern const u8 gCutscene06EnglishPage1Text[];
extern const u8 *const gCutscene06EnglishPage1[1];
extern const u8 gCutscene07EnglishPage0Text[];
extern const u8 *const gCutscene07EnglishPage0[1];
extern const u8 gCutscene10EnglishPage0Text[];
extern const u8 *const gCutscene10EnglishPage0[1];
extern const u8 gCutscene08EnglishPage0Text[];
extern const u8 *const gCutscene08EnglishPage0[1];
extern const u8 gCutscene08EnglishPage1Text[];
extern const u8 *const gCutscene08EnglishPage1[1];
extern const u8 gCutscene08EnglishPage2Text[];
extern const u8 *const gCutscene08EnglishPage2[1];
extern const u8 gCutscene08EnglishPage3Text[];
extern const u8 *const gCutscene08EnglishPage3[1];
extern const u8 gCutscene09EnglishPage1Text[];
extern const u8 *const gCutscene09EnglishPage1[1];
extern const u8 gCutscene09EnglishPage5Text[];
extern const u8 *const gCutscene09EnglishPage5[1];
extern const u8 gCutscene10EnglishPage5Text[];
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
extern const u8 gCutscene01FrenchPage0Text[];
extern const u8 *const gCutscene01FrenchPage0[1];
extern const u8 gCutscene01FrenchPage1Text[];
extern const u8 *const gCutscene01FrenchPage1[1];
extern const u8 gCutscene01FrenchPage2Text[];
extern const u8 *const gCutscene01FrenchPage2[1];
extern const u8 gCutscene01FrenchPage3_0Text[];
extern const u8 gCutscene01FrenchPage3_1Text[];
extern const u8 *const gCutscene01FrenchPage3[2];
extern const u8 gCutscene02FrenchPage0Text[];
extern const u8 *const gCutscene02FrenchPage0[1];
extern const u8 gCutscene02FrenchPage1Text[];
extern const u8 *const gCutscene02FrenchPage1[1];
extern const u8 gCutscene02FrenchPage2Text[];
extern const u8 *const gCutscene02FrenchPage2[1];
extern const u8 gCutscene02FrenchPage5Text[];
extern const u8 *const gCutscene02FrenchPage5[1];
extern const u8 gCutscene02FrenchPage7_0Text[];
extern const u8 gCutscene02FrenchPage7_1Text[];
extern const u8 *const gCutscene02FrenchPage7[2];
extern const u8 gCutscene02FrenchPage6Text[];
extern const u8 *const gCutscene02FrenchPage6[1];
extern const u8 gCutscene02FrenchPage8Text[];
extern const u8 *const gCutscene02FrenchPage8[1];
extern const u8 gCutscene03FrenchPage0Text[];
extern const u8 *const gCutscene03FrenchPage0[1];
extern const u8 gCutscene04FrenchPage0Text[];
extern const u8 *const gCutscene04FrenchPage0[1];
extern const u8 gCutscene05FrenchPage0Text[];
extern const u8 *const gCutscene05FrenchPage0[1];
extern const u8 gCutscene06FrenchPage0Text[];
extern const u8 *const gCutscene06FrenchPage0[1];
extern const u8 gCutscene06FrenchPage1Text[];
extern const u8 *const gCutscene06FrenchPage1[1];
extern const u8 gCutscene07FrenchPage0Text[];
extern const u8 *const gCutscene07FrenchPage0[1];
extern const u8 gCutscene10FrenchPage0Text[];
extern const u8 *const gCutscene10FrenchPage0[1];
extern const u8 gCutscene08FrenchPage0Text[];
extern const u8 *const gCutscene08FrenchPage0[1];
extern const u8 *const gCutscene08FrenchPage1[1];
extern const u8 gCutscene08FrenchPage2Text[];
extern const u8 *const gCutscene08FrenchPage2[1];
extern const u8 gCutscene08FrenchPage3Text[];
extern const u8 *const gCutscene08FrenchPage3[1];
extern const u8 gCutscene09FrenchPage1Text[];
extern const u8 *const gCutscene09FrenchPage1[1];
extern const u8 gCutscene09FrenchPage5Text[];
extern const u8 *const gCutscene09FrenchPage5[1];
extern const u8 gCutscene10FrenchPage5Text[];
extern const u8 *const gCutscene10FrenchPage5[1];
extern const u8 gCutscene01GermanPage0Text[];
extern const u8 *const gCutscene01GermanPage0[1];
extern const u8 gCutscene01GermanPage1Text[];
extern const u8 *const gCutscene01GermanPage1[1];
extern const u8 gCutscene01GermanPage2Text[];
extern const u8 *const gCutscene01GermanPage2[1];
extern const u8 gCutscene01GermanPage3_0Text[];
extern const u8 gCutscene01GermanPage3_1Text[];
extern const u8 *const gCutscene01GermanPage3[2];
extern const u8 gCutscene02GermanPage0Text[];
extern const u8 *const gCutscene02GermanPage0[1];
extern const u8 gCutscene02GermanPage1Text[];
extern const u8 *const gCutscene02GermanPage1[1];
extern const u8 gCutscene02GermanPage2Text[];
extern const u8 *const gCutscene02GermanPage2[1];
extern const u8 gCutscene02GermanPage5Text[];
extern const u8 *const gCutscene02GermanPage5[1];
extern const u8 gCutscene02GermanPage7_0Text[];
extern const u8 gCutscene02GermanPage7_1Text[];
extern const u8 *const gCutscene02GermanPage7[2];
extern const u8 gCutscene02GermanPage6Text[];
extern const u8 *const gCutscene02GermanPage6[1];
extern const u8 gCutscene02GermanPage8Text[];
extern const u8 *const gCutscene02GermanPage8[1];
extern const u8 gCutscene03GermanPage0Text[];
extern const u8 *const gCutscene03GermanPage0[1];
extern const u8 gCutscene04GermanPage0Text[];
extern const u8 *const gCutscene04GermanPage0[1];
extern const u8 gCutscene05GermanPage0Text[];
extern const u8 *const gCutscene05GermanPage0[1];
extern const u8 gCutscene06GermanPage0Text[];
extern const u8 *const gCutscene06GermanPage0[1];
extern const u8 gCutscene06GermanPage1Text[];
extern const u8 *const gCutscene06GermanPage1[1];
extern const u8 gCutscene07GermanPage0Text[];
extern const u8 *const gCutscene07GermanPage0[1];
extern const u8 gCutscene10GermanPage0Text[];
extern const u8 *const gCutscene10GermanPage0[1];
extern const u8 gCutscene08GermanPage0Text[];
extern const u8 *const gCutscene08GermanPage0[1];
extern const u8 gCutscene08GermanPage1Text[];
extern const u8 *const gCutscene08GermanPage1[1];
extern const u8 gCutscene08GermanPage2Text[];
extern const u8 *const gCutscene08GermanPage2[1];
extern const u8 gCutscene08GermanPage3Text[];
extern const u8 *const gCutscene08GermanPage3[1];
extern const u8 gCutscene09GermanPage1Text[];
extern const u8 *const gCutscene09GermanPage1[1];
extern const u8 gCutscene09GermanPage5Text[];
extern const u8 *const gCutscene09GermanPage5[1];
extern const u8 gCutscene10GermanPage5Text[];
extern const u8 *const gCutscene10GermanPage5[1];
extern const u8 gCutscene01SpanishPage0Text[];
extern const u8 *const gCutscene01SpanishPage0[1];
extern const u8 gCutscene01SpanishPage1Text[];
extern const u8 *const gCutscene01SpanishPage1[1];
extern const u8 gCutscene01SpanishPage2Text[];
extern const u8 *const gCutscene01SpanishPage2[1];
extern const u8 gCutscene01SpanishPage3_0Text[];
extern const u8 gCutscene01SpanishPage3_1Text[];
extern const u8 *const gCutscene01SpanishPage3[2];
extern const u8 gCutscene02SpanishPage0Text[];
extern const u8 *const gCutscene02SpanishPage0[1];
extern const u8 gCutscene02SpanishPage1Text[];
extern const u8 *const gCutscene02SpanishPage1[1];
extern const u8 gCutscene02SpanishPage2Text[];
extern const u8 *const gCutscene02SpanishPage2[1];
extern const u8 gCutscene02SpanishPage5Text[];
extern const u8 *const gCutscene02SpanishPage5[1];
extern const u8 gCutscene02SpanishPage7_0Text[];
extern const u8 gCutscene02SpanishPage7_1Text[];
extern const u8 *const gCutscene02SpanishPage7[2];
extern const u8 gCutscene02SpanishPage6Text[];
extern const u8 *const gCutscene02SpanishPage6[1];
extern const u8 gCutscene02SpanishPage8Text[];
extern const u8 *const gCutscene02SpanishPage8[1];
extern const u8 gCutscene03SpanishPage0Text[];
extern const u8 *const gCutscene03SpanishPage0[1];
extern const u8 gCutscene04SpanishPage0Text[];
extern const u8 *const gCutscene04SpanishPage0[1];
extern const u8 gCutscene05SpanishPage0Text[];
extern const u8 *const gCutscene05SpanishPage0[1];
extern const u8 gCutscene06SpanishPage0Text[];
extern const u8 *const gCutscene06SpanishPage0[1];
extern const u8 gCutscene06SpanishPage1Text[];
extern const u8 *const gCutscene06SpanishPage1[1];
extern const u8 gCutscene07SpanishPage0Text[];
extern const u8 *const gCutscene07SpanishPage0[1];
extern const u8 gCutscene10SpanishPage0Text[];
extern const u8 *const gCutscene10SpanishPage0[1];
extern const u8 gCutscene08SpanishPage0Text[];
extern const u8 *const gCutscene08SpanishPage0[1];
extern const u8 gCutscene08SpanishPage1Text[];
extern const u8 *const gCutscene08SpanishPage1[1];
extern const u8 gCutscene08SpanishPage2Text[];
extern const u8 *const gCutscene08SpanishPage2[1];
extern const u8 gCutscene08SpanishPage3Text[];
extern const u8 *const gCutscene08SpanishPage3[1];
extern const u8 gCutscene09SpanishPage1Text[];
extern const u8 *const gCutscene09SpanishPage1[1];
extern const u8 gCutscene09SpanishPage5Text[];
extern const u8 *const gCutscene09SpanishPage5[1];
extern const u8 gCutscene10SpanishPage5Text[];
extern const u8 *const gCutscene10SpanishPage5[1];
extern const u8 gCutscene01ItalianPage0Text[];
extern const u8 *const gCutscene01ItalianPage0[1];
extern const u8 gCutscene01ItalianPage1Text[];
extern const u8 *const gCutscene01ItalianPage1[1];
extern const u8 gCutscene01ItalianPage2Text[];
extern const u8 *const gCutscene01ItalianPage2[1];
extern const u8 gCutscene01ItalianPage3_0Text[];
extern const u8 gCutscene01ItalianPage3_1Text[];
extern const u8 *const gCutscene01ItalianPage3[2];
extern const u8 gCutscene02ItalianPage0Text[];
extern const u8 *const gCutscene02ItalianPage0[1];
extern const u8 gCutscene02ItalianPage1Text[];
extern const u8 *const gCutscene02ItalianPage1[1];
extern const u8 gCutscene02ItalianPage2Text[];
extern const u8 *const gCutscene02ItalianPage2[1];
extern const u8 gCutscene02ItalianPage5Text[];
extern const u8 *const gCutscene02ItalianPage5[1];
extern const u8 gCutscene02ItalianPage7_0Text[];
extern const u8 gCutscene02ItalianPage7_1Text[];
extern const u8 *const gCutscene02ItalianPage7[2];
extern const u8 gCutscene02ItalianPage6Text[];
extern const u8 *const gCutscene02ItalianPage6[1];
extern const u8 gCutscene02ItalianPage8Text[];
extern const u8 *const gCutscene02ItalianPage8[1];
extern const u8 gCutscene03ItalianPage0Text[];
extern const u8 *const gCutscene03ItalianPage0[1];
extern const u8 gCutscene04ItalianPage0Text[];
extern const u8 *const gCutscene04ItalianPage0[1];
extern const u8 gCutscene05ItalianPage0Text[];
extern const u8 *const gCutscene05ItalianPage0[1];
extern const u8 gCutscene06ItalianPage0Text[];
extern const u8 *const gCutscene06ItalianPage0[1];
extern const u8 gCutscene06ItalianPage1Text[];
extern const u8 *const gCutscene06ItalianPage1[1];
extern const u8 gCutscene07ItalianPage0Text[];
extern const u8 *const gCutscene07ItalianPage0[1];
extern const u8 gCutscene10ItalianPage0Text[];
extern const u8 *const gCutscene10ItalianPage0[1];
extern const u8 gCutscene08ItalianPage0Text[];
extern const u8 *const gCutscene08ItalianPage0[1];
extern const u8 *const gCutscene08ItalianPage1[1];
extern const u8 gCutscene08ItalianPage2Text[];
extern const u8 *const gCutscene08ItalianPage2[1];
extern const u8 gCutscene08ItalianPage3Text[];
extern const u8 *const gCutscene08ItalianPage3[1];
extern const u8 gCutscene09ItalianPage1Text[];
extern const u8 *const gCutscene09ItalianPage1[1];
extern const u8 gCutscene09ItalianPage5Text[];
extern const u8 *const gCutscene09ItalianPage5[1];
extern const u8 gCutscene10ItalianPage5Text[];
extern const u8 *const gCutscene10ItalianPage5[1];
extern const u8 gCutscene01DutchPage0Text[];
extern const u8 *const gCutscene01DutchPage0[1];
extern const u8 gCutscene01DutchPage1Text[];
extern const u8 *const gCutscene01DutchPage1[1];
extern const u8 gCutscene01DutchPage2Text[];
extern const u8 *const gCutscene01DutchPage2[1];
extern const u8 gCutscene01DutchPage3_0Text[];
extern const u8 gCutscene01DutchPage3_1Text[];
extern const u8 *const gCutscene01DutchPage3[2];
extern const u8 gCutscene02DutchPage0Text[];
extern const u8 *const gCutscene02DutchPage0[1];
extern const u8 gCutscene02DutchPage1Text[];
extern const u8 *const gCutscene02DutchPage1[1];
extern const u8 *const gCutscene02DutchPage2[1];
extern const u8 gCutscene02DutchPage5Text[];
extern const u8 *const gCutscene02DutchPage5[1];
extern const u8 gCutscene02DutchPage7_0Text[];
extern const u8 gCutscene02DutchPage7_1Text[];
extern const u8 *const gCutscene02DutchPage7[2];
extern const u8 gCutscene02DutchPage6Text[];
extern const u8 *const gCutscene02DutchPage6[1];
extern const u8 gCutscene02DutchPage8Text[];
extern const u8 *const gCutscene02DutchPage8[1];
extern const u8 gCutscene03DutchPage0Text[];
extern const u8 *const gCutscene03DutchPage0[1];
extern const u8 gCutscene04DutchPage0Text[];
extern const u8 *const gCutscene04DutchPage0[1];
extern const u8 gCutscene05DutchPage0Text[];
extern const u8 *const gCutscene05DutchPage0[1];
extern const u8 gCutscene06DutchPage0Text[];
extern const u8 *const gCutscene06DutchPage0[1];
extern const u8 gCutscene06DutchPage1Text[];
extern const u8 *const gCutscene06DutchPage1[1];
extern const u8 gCutscene07DutchPage0Text[];
extern const u8 *const gCutscene07DutchPage0[1];
extern const u8 gCutscene10DutchPage0Text[];
extern const u8 *const gCutscene10DutchPage0[1];
extern const u8 gCutscene08DutchPage0Text[];
extern const u8 *const gCutscene08DutchPage0[1];
extern const u8 *const gCutscene08DutchPage1[1];
extern const u8 gCutscene08DutchPage2Text[];
extern const u8 *const gCutscene08DutchPage2[1];
extern const u8 gCutscene08DutchPage3Text[];
extern const u8 *const gCutscene08DutchPage3[1];
extern const u8 gCutscene09DutchPage1Text[];
extern const u8 *const gCutscene09DutchPage1[1];
extern const u8 *const gCutscene09DutchPage5[1];
extern const u8 gCutscene10DutchPage5Text[];
extern const u8 *const gCutscene10DutchPage5[1];

/* The pages of each cutscene, per language. */
const struct cutscene_page *const gCutsceneTextEnglish[11] = {
    gCutscene00English,
    gCutscene01English,
    gCutscene02English,
    gCutscene03English,
    gCutscene04English,
    gCutscene05English,
    gCutscene06English,
    gCutscene07English,
    gCutscene08English,
    gCutscene09English,
    gCutscene10English,
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
    { NULL, 0 },
    { NULL, 0 },
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
    { NULL, 0 },
    { gCutscene09EnglishPage1, ARRAY_COUNT(gCutscene09EnglishPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09EnglishPage5, ARRAY_COUNT(gCutscene09EnglishPage5) },
};
const struct cutscene_page gCutscene10English[6] = {
    { gCutscene10EnglishPage0, ARRAY_COUNT(gCutscene10EnglishPage0) },
    { NULL, 0 },
    { gCutscene09EnglishPage1, ARRAY_COUNT(gCutscene09EnglishPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10EnglishPage5, ARRAY_COUNT(gCutscene10EnglishPage5) },
};
const struct cutscene_page gCutscene00English[1] = {
    { NULL, 0 },
};
const struct cutscene_page *const gCutsceneTextFrench[11] = {
    gCutscene00French,
    gCutscene01French,
    gCutscene02French,
    gCutscene03French,
    gCutscene04French,
    gCutscene05French,
    gCutscene06French,
    gCutscene07French,
    gCutscene08French,
    gCutscene09French,
    gCutscene10French,
};

const struct cutscene_page *const gCutsceneTextGerman[11] = {
    gCutscene00German,
    gCutscene01German,
    gCutscene02German,
    gCutscene03German,
    gCutscene04German,
    gCutscene05German,
    gCutscene06German,
    gCutscene07German,
    gCutscene08German,
    gCutscene09German,
    gCutscene10German,
};

const struct cutscene_page *const gCutsceneTextSpanish[11] = {
    gCutscene00Spanish,
    gCutscene01Spanish,
    gCutscene02Spanish,
    gCutscene03Spanish,
    gCutscene04Spanish,
    gCutscene05Spanish,
    gCutscene06Spanish,
    gCutscene07Spanish,
    gCutscene08Spanish,
    gCutscene09Spanish,
    gCutscene10Spanish,
};

const struct cutscene_page *const gCutsceneTextItalian[11] = {
    gCutscene00Italian,
    gCutscene01Italian,
    gCutscene02Italian,
    gCutscene03Italian,
    gCutscene04Italian,
    gCutscene05Italian,
    gCutscene06Italian,
    gCutscene07Italian,
    gCutscene08Italian,
    gCutscene09Italian,
    gCutscene10Italian,
};

const struct cutscene_page *const gCutsceneTextDutch[11] = {
    gCutscene00Dutch,
    gCutscene01Dutch,
    gCutscene02Dutch,
    gCutscene03Dutch,
    gCutscene04Dutch,
    gCutscene05Dutch,
    gCutscene06Dutch,
    gCutscene07Dutch,
    gCutscene08Dutch,
    gCutscene09Dutch,
    gCutscene10Dutch,
};

const struct cutscene_slide *const gCutscene00Slides[1] = { &gCutsceneSlide23 };
const struct cutscene_slide *const gCutscene01Slides[4] = { &gCutsceneSlide00, &gCutsceneSlide01, &gCutsceneSlide02, &gCutsceneSlide01 };
const struct cutscene_slide *const gCutscene02Slides[9] = { &gCutsceneSlide00, &gCutsceneSlide03, &gCutsceneSlide04, &gCutsceneSlide05, &gCutsceneSlide06, &gCutsceneSlide07, &gCutsceneSlide08, &gCutsceneSlide09, &gCutsceneSlide10 };
const struct cutscene_slide *const gCutscene03Slides[1] = { &gCutsceneSlide11 };
const struct cutscene_slide *const gCutscene06Slides[2] = { &gCutsceneSlide01, &gCutsceneSlide02 };
const struct cutscene_slide *const gCutscene04Slides[1] = { &gCutsceneSlide11 };
const struct cutscene_slide *const gCutscene05Slides[1] = { &gCutsceneSlide12 };
const struct cutscene_slide *const gCutscene07Slides[1] = { &gCutsceneSlide12 };
const struct cutscene_slide *const gCutscene08Slides[4] = { &gCutsceneSlide13, &gCutsceneSlide14, &gCutsceneSlide15, &gCutsceneSlide16 };
const struct cutscene_slide *const gCutscene09Slides[6] = { &gCutsceneSlide17, &gCutsceneSlide18, &gCutsceneSlide19, &gCutsceneSlide20, &gCutsceneSlide21, &gCutsceneSlide22 };
const struct cutscene_slide *const gCutscene10Slides[6] = { &gCutsceneSlide13, &gCutsceneSlide17, &gCutsceneSlide18, &gCutsceneSlide19, &gCutsceneSlide20, &gCutsceneSlide10 };
const u8 gCutscene01EnglishPage0Text[] = "on a mysterious space station orbiting high above the earth ...";
const u8 *const gCutscene01EnglishPage0[1] = { gCutscene01EnglishPage0Text };
const u8 gCutscene01EnglishPage1Text[] =
    "uka uka:  cortex, remind me why i keep you around.  you have "
    "failed me one too many times.";
const u8 *const gCutscene01EnglishPage1[1] = { gCutscene01EnglishPage1Text };
const u8 gCutscene01EnglishPage2Text[] =
    "cortex:  uka uka!  forgive me ...  i've been wasting all these "
    "years trying to vanquish that brainless bandicoot!  he is of no "
    "importance to us ...  we want to take over the world!";
const u8 *const gCutscene01EnglishPage2[1] = { gCutscene01EnglishPage2Text };
const u8 gCutscene01EnglishPage3_0Text[] =
    "uka uka:  i've heard it all before, little scientist!  what is "
    "so different this time?";
const u8 gCutscene01EnglishPage3_1Text[] =
    "cortex:  trust me uka, i have a little plan to bring earth's "
    "puny inhabitants down to size ...";
const u8 *const gCutscene01EnglishPage3[2] = { gCutscene01EnglishPage3_0Text, gCutscene01EnglishPage3_1Text };
const u8 gCutscene02EnglishPage0Text[] = "cortex:  at last, my planetary minimizer is complete!";
const u8 *const gCutscene02EnglishPage0[1] = { gCutscene02EnglishPage0Text };
const u8 gCutscene02EnglishPage1Text[] = "cortex: earthlings will bow to my superior intellect!";
const u8 *const gCutscene02EnglishPage1[1] = { gCutscene02EnglishPage1Text };
const u8 gCutscene02EnglishPage2Text[] = "cortex:  muahahahahaha!";
const u8 *const gCutscene02EnglishPage2[1] = { gCutscene02EnglishPage2Text };
const u8 gCutscene02EnglishPage5Text[] =
    "cortex:  finally, after all these years of abuse, the tables "
    "have turned!  who's the little guy now?  i, neo cortex, am your "
    "ruler.  you will look up to me!  hahaha!";
const u8 *const gCutscene02EnglishPage5[1] = { gCutscene02EnglishPage5Text };
const u8 gCutscene02EnglishPage7_0Text[] =
    "aku:  crash, coco, the world needs your help.  cortex has shrunk "
    "our planet to the size of a grapefruit.  we must stop him.";
const u8 gCutscene02EnglishPage7_1Text[] =
    "coco:  it looks as if cortex is using the crystals to power his "
    "shrinking machine ...  crash, if you can find the proper "
    "crystals from around the world, i think i can build a device to "
    "reverse the effects.";
const u8 *const gCutscene02EnglishPage7[2] = { gCutscene02EnglishPage7_0Text, gCutscene02EnglishPage7_1Text };
const u8 gCutscene02EnglishPage6Text[] =
    "cortex:  i have finally won!  now nothing can stop me, not even "
    "that little bandicoot!";
const u8 *const gCutscene02EnglishPage6[1] = { gCutscene02EnglishPage6Text };
const u8 gCutscene02EnglishPage8Text[] =
    "aku:  good luck crash.  you must find the crystals so we can put "
    "an end to cortex's evil scheme.";
const u8 *const gCutscene02EnglishPage8[1] = { gCutscene02EnglishPage8Text };
const u8 gCutscene03EnglishPage0Text[] =
    "cortex:  i know what you are trying to do bandicoot - but it's "
    "not going to work.  my minions will be ready for you!";
const u8 *const gCutscene03EnglishPage0[1] = { gCutscene03EnglishPage0Text };
const u8 gCutscene04EnglishPage0Text[] =
    "cortex:  meddling marsupial!  you got lucky this time ...  my "
    "henchmen won't be so easy on you in the future.";
const u8 *const gCutscene04EnglishPage0[1] = { gCutscene04EnglishPage0Text };
const u8 gCutscene05EnglishPage0Text[] =
    "coco:  great job crash!  i have half the crystals.  now bring me "
    "the rest of them, so i can finish my machine - but be careful, "
    "cortex is watching you!";
const u8 *const gCutscene05EnglishPage0[1] = { gCutscene05EnglishPage0Text };
const u8 gCutscene06EnglishPage0Text[] =
    "uka:  cortex, i knew this would happen!  crash has defeated "
    "three of your stooges!  he must be stopped.  i will not tolerate "
    "another failure.";
const u8 *const gCutscene06EnglishPage0[1] = { gCutscene06EnglishPage0Text };
const u8 gCutscene06EnglishPage1Text[] =
    "cortex:  uka ...  i'm sorry!  however, do not be overly "
    "concerned - my planetary minimizer will stop him!";
const u8 *const gCutscene06EnglishPage1[1] = { gCutscene06EnglishPage1Text };
const u8 gCutscene07EnglishPage0Text[] =
    "coco:  okay crash, i have all of the crystals, but i need you to "
    "destroy the shrinking machine before i can return earth to its "
    "original state.  i'll use the enlarger to make you big enough to "
    "take on cortex!";
const u8 *const gCutscene07EnglishPage0[1] = { gCutscene07EnglishPage0Text };
const u8 gCutscene10EnglishPage0Text[] = "cortex:  you fool!  it will take me forever to fix this mess!";
const u8 *const gCutscene10EnglishPage0[1] = { gCutscene10EnglishPage0Text };
const u8 gCutscene08EnglishPage0Text[] =
    "cortex:  you idiot!  what have you done?  you have destroyed the "
    "stabilizer crystals!  the unrestrained power of the minimizer is "
    "highly unpredictable!";
const u8 *const gCutscene08EnglishPage0[1] = { gCutscene08EnglishPage0Text };
const u8 gCutscene08EnglishPage1Text[] = "arrgh!  ahhh!!!";
const u8 *const gCutscene08EnglishPage1[1] = { gCutscene08EnglishPage1Text };
const u8 gCutscene08EnglishPage2Text[] = "monster:  what have you done to us?";
const u8 *const gCutscene08EnglishPage2[1] = { gCutscene08EnglishPage2Text };
const u8 gCutscene08EnglishPage3Text[] = "monster:  you will pay little bandicoot!";
const u8 *const gCutscene08EnglishPage3[1] = { gCutscene08EnglishPage3Text };
const u8 gCutscene09EnglishPage1Text[] =
    "coco:  super!  you got all the crystals!  let's hope there's "
    "enough power in these to reverse cortex's dirty work.";
const u8 *const gCutscene09EnglishPage1[1] = { gCutscene09EnglishPage1Text };
const u8 gCutscene09EnglishPage5Text[] = "the end?!";
const u8 *const gCutscene09EnglishPage5[1] = { gCutscene09EnglishPage5Text };
const u8 gCutscene10EnglishPage5Text[] =
    "aku:  crash, you've done well ...  but cortex is still a threat "
    "to us, as his space station and planetary minimizer have not "
    "been destroyed.  you must go back and retrieve the gems and "
    "relics from each location.";
const u8 *const gCutscene10EnglishPage5[1] = { gCutscene10EnglishPage5Text };
const struct cutscene_page gCutscene01French[4] = {
    { gCutscene01FrenchPage0, ARRAY_COUNT(gCutscene01FrenchPage0) },
    { gCutscene01FrenchPage1, ARRAY_COUNT(gCutscene01FrenchPage1) },
    { gCutscene01FrenchPage2, ARRAY_COUNT(gCutscene01FrenchPage2) },
    { gCutscene01FrenchPage3, ARRAY_COUNT(gCutscene01FrenchPage3) },
};
const struct cutscene_page gCutscene02French[9] = {
    { gCutscene02FrenchPage0, ARRAY_COUNT(gCutscene02FrenchPage0) },
    { gCutscene02FrenchPage1, ARRAY_COUNT(gCutscene02FrenchPage1) },
    { gCutscene02FrenchPage2, ARRAY_COUNT(gCutscene02FrenchPage2) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene02FrenchPage5, ARRAY_COUNT(gCutscene02FrenchPage5) },
    { gCutscene02FrenchPage6, ARRAY_COUNT(gCutscene02FrenchPage6) },
    { gCutscene02FrenchPage7, ARRAY_COUNT(gCutscene02FrenchPage7) },
    { gCutscene02FrenchPage8, ARRAY_COUNT(gCutscene02FrenchPage8) },
};
const struct cutscene_page gCutscene03French[1] = {
    { gCutscene03FrenchPage0, ARRAY_COUNT(gCutscene03FrenchPage0) },
};
const struct cutscene_page gCutscene04French[1] = {
    { gCutscene04FrenchPage0, ARRAY_COUNT(gCutscene04FrenchPage0) },
};
const struct cutscene_page gCutscene05French[1] = {
    { gCutscene05FrenchPage0, ARRAY_COUNT(gCutscene05FrenchPage0) },
};
const struct cutscene_page gCutscene06French[2] = {
    { gCutscene06FrenchPage0, ARRAY_COUNT(gCutscene06FrenchPage0) },
    { gCutscene06FrenchPage1, ARRAY_COUNT(gCutscene06FrenchPage1) },
};
const struct cutscene_page gCutscene07French[1] = {
    { gCutscene07FrenchPage0, ARRAY_COUNT(gCutscene07FrenchPage0) },
};
const struct cutscene_page gCutscene08French[4] = {
    { gCutscene08FrenchPage0, ARRAY_COUNT(gCutscene08FrenchPage0) },
    { gCutscene08FrenchPage1, ARRAY_COUNT(gCutscene08FrenchPage1) },
    { gCutscene08FrenchPage2, ARRAY_COUNT(gCutscene08FrenchPage2) },
    { gCutscene08FrenchPage3, ARRAY_COUNT(gCutscene08FrenchPage3) },
};
const struct cutscene_page gCutscene09French[6] = {
    { NULL, 0 },
    { gCutscene09FrenchPage1, ARRAY_COUNT(gCutscene09FrenchPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09FrenchPage5, ARRAY_COUNT(gCutscene09FrenchPage5) },
};
const struct cutscene_page gCutscene10French[6] = {
    { gCutscene10FrenchPage0, ARRAY_COUNT(gCutscene10FrenchPage0) },
    { NULL, 0 },
    { gCutscene09FrenchPage1, ARRAY_COUNT(gCutscene09FrenchPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10FrenchPage5, ARRAY_COUNT(gCutscene10FrenchPage5) },
};
const struct cutscene_page gCutscene00French[1] = {
    { NULL, 0 },
};
const struct cutscene_page gCutscene01German[4] = {
    { gCutscene01GermanPage0, ARRAY_COUNT(gCutscene01GermanPage0) },
    { gCutscene01GermanPage1, ARRAY_COUNT(gCutscene01GermanPage1) },
    { gCutscene01GermanPage2, ARRAY_COUNT(gCutscene01GermanPage2) },
    { gCutscene01GermanPage3, ARRAY_COUNT(gCutscene01GermanPage3) },
};
const struct cutscene_page gCutscene02German[9] = {
    { gCutscene02GermanPage0, ARRAY_COUNT(gCutscene02GermanPage0) },
    { gCutscene02GermanPage1, ARRAY_COUNT(gCutscene02GermanPage1) },
    { gCutscene02GermanPage2, ARRAY_COUNT(gCutscene02GermanPage2) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene02GermanPage5, ARRAY_COUNT(gCutscene02GermanPage5) },
    { gCutscene02GermanPage6, ARRAY_COUNT(gCutscene02GermanPage6) },
    { gCutscene02GermanPage7, ARRAY_COUNT(gCutscene02GermanPage7) },
    { gCutscene02GermanPage8, ARRAY_COUNT(gCutscene02GermanPage8) },
};
const struct cutscene_page gCutscene03German[1] = {
    { gCutscene03GermanPage0, ARRAY_COUNT(gCutscene03GermanPage0) },
};
const struct cutscene_page gCutscene04German[1] = {
    { gCutscene04GermanPage0, ARRAY_COUNT(gCutscene04GermanPage0) },
};
const struct cutscene_page gCutscene05German[1] = {
    { gCutscene05GermanPage0, ARRAY_COUNT(gCutscene05GermanPage0) },
};
const struct cutscene_page gCutscene06German[2] = {
    { gCutscene06GermanPage0, ARRAY_COUNT(gCutscene06GermanPage0) },
    { gCutscene06GermanPage1, ARRAY_COUNT(gCutscene06GermanPage1) },
};
const struct cutscene_page gCutscene07German[1] = {
    { gCutscene07GermanPage0, ARRAY_COUNT(gCutscene07GermanPage0) },
};
const struct cutscene_page gCutscene08German[4] = {
    { gCutscene08GermanPage0, ARRAY_COUNT(gCutscene08GermanPage0) },
    { gCutscene08GermanPage1, ARRAY_COUNT(gCutscene08GermanPage1) },
    { gCutscene08GermanPage2, ARRAY_COUNT(gCutscene08GermanPage2) },
    { gCutscene08GermanPage3, ARRAY_COUNT(gCutscene08GermanPage3) },
};
const struct cutscene_page gCutscene09German[6] = {
    { NULL, 0 },
    { gCutscene09GermanPage1, ARRAY_COUNT(gCutscene09GermanPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09GermanPage5, ARRAY_COUNT(gCutscene09GermanPage5) },
};
const struct cutscene_page gCutscene10German[6] = {
    { gCutscene10GermanPage0, ARRAY_COUNT(gCutscene10GermanPage0) },
    { NULL, 0 },
    { gCutscene09GermanPage1, ARRAY_COUNT(gCutscene09GermanPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10GermanPage5, ARRAY_COUNT(gCutscene10GermanPage5) },
};
const struct cutscene_page gCutscene00German[1] = {
    { NULL, 0 },
};
const struct cutscene_page gCutscene01Spanish[4] = {
    { gCutscene01SpanishPage0, ARRAY_COUNT(gCutscene01SpanishPage0) },
    { gCutscene01SpanishPage1, ARRAY_COUNT(gCutscene01SpanishPage1) },
    { gCutscene01SpanishPage2, ARRAY_COUNT(gCutscene01SpanishPage2) },
    { gCutscene01SpanishPage3, ARRAY_COUNT(gCutscene01SpanishPage3) },
};
const struct cutscene_page gCutscene02Spanish[9] = {
    { gCutscene02SpanishPage0, ARRAY_COUNT(gCutscene02SpanishPage0) },
    { gCutscene02SpanishPage1, ARRAY_COUNT(gCutscene02SpanishPage1) },
    { gCutscene02SpanishPage2, ARRAY_COUNT(gCutscene02SpanishPage2) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene02SpanishPage5, ARRAY_COUNT(gCutscene02SpanishPage5) },
    { gCutscene02SpanishPage6, ARRAY_COUNT(gCutscene02SpanishPage6) },
    { gCutscene02SpanishPage7, ARRAY_COUNT(gCutscene02SpanishPage7) },
    { gCutscene02SpanishPage8, ARRAY_COUNT(gCutscene02SpanishPage8) },
};
const struct cutscene_page gCutscene03Spanish[1] = {
    { gCutscene03SpanishPage0, ARRAY_COUNT(gCutscene03SpanishPage0) },
};
const struct cutscene_page gCutscene04Spanish[1] = {
    { gCutscene04SpanishPage0, ARRAY_COUNT(gCutscene04SpanishPage0) },
};
const struct cutscene_page gCutscene05Spanish[1] = {
    { gCutscene05SpanishPage0, ARRAY_COUNT(gCutscene05SpanishPage0) },
};
const struct cutscene_page gCutscene06Spanish[2] = {
    { gCutscene06SpanishPage0, ARRAY_COUNT(gCutscene06SpanishPage0) },
    { gCutscene06SpanishPage1, ARRAY_COUNT(gCutscene06SpanishPage1) },
};
const struct cutscene_page gCutscene07Spanish[1] = {
    { gCutscene07SpanishPage0, ARRAY_COUNT(gCutscene07SpanishPage0) },
};
const struct cutscene_page gCutscene08Spanish[4] = {
    { gCutscene08SpanishPage0, ARRAY_COUNT(gCutscene08SpanishPage0) },
    { gCutscene08SpanishPage1, ARRAY_COUNT(gCutscene08SpanishPage1) },
    { gCutscene08SpanishPage2, ARRAY_COUNT(gCutscene08SpanishPage2) },
    { gCutscene08SpanishPage3, ARRAY_COUNT(gCutscene08SpanishPage3) },
};
const struct cutscene_page gCutscene09Spanish[6] = {
    { NULL, 0 },
    { gCutscene09SpanishPage1, ARRAY_COUNT(gCutscene09SpanishPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09SpanishPage5, ARRAY_COUNT(gCutscene09SpanishPage5) },
};
const struct cutscene_page gCutscene10Spanish[6] = {
    { gCutscene10SpanishPage0, ARRAY_COUNT(gCutscene10SpanishPage0) },
    { NULL, 0 },
    { gCutscene09SpanishPage1, ARRAY_COUNT(gCutscene09SpanishPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10SpanishPage5, ARRAY_COUNT(gCutscene10SpanishPage5) },
};
const struct cutscene_page gCutscene00Spanish[1] = {
    { NULL, 0 },
};
const struct cutscene_page gCutscene01Italian[4] = {
    { gCutscene01ItalianPage0, ARRAY_COUNT(gCutscene01ItalianPage0) },
    { gCutscene01ItalianPage1, ARRAY_COUNT(gCutscene01ItalianPage1) },
    { gCutscene01ItalianPage2, ARRAY_COUNT(gCutscene01ItalianPage2) },
    { gCutscene01ItalianPage3, ARRAY_COUNT(gCutscene01ItalianPage3) },
};
const struct cutscene_page gCutscene02Italian[9] = {
    { gCutscene02ItalianPage0, ARRAY_COUNT(gCutscene02ItalianPage0) },
    { gCutscene02ItalianPage1, ARRAY_COUNT(gCutscene02ItalianPage1) },
    { gCutscene02ItalianPage2, ARRAY_COUNT(gCutscene02ItalianPage2) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene02ItalianPage5, ARRAY_COUNT(gCutscene02ItalianPage5) },
    { gCutscene02ItalianPage6, ARRAY_COUNT(gCutscene02ItalianPage6) },
    { gCutscene02ItalianPage7, ARRAY_COUNT(gCutscene02ItalianPage7) },
    { gCutscene02ItalianPage8, ARRAY_COUNT(gCutscene02ItalianPage8) },
};
const struct cutscene_page gCutscene03Italian[1] = {
    { gCutscene03ItalianPage0, ARRAY_COUNT(gCutscene03ItalianPage0) },
};
const struct cutscene_page gCutscene04Italian[1] = {
    { gCutscene04ItalianPage0, ARRAY_COUNT(gCutscene04ItalianPage0) },
};
const struct cutscene_page gCutscene05Italian[1] = {
    { gCutscene05ItalianPage0, ARRAY_COUNT(gCutscene05ItalianPage0) },
};
const struct cutscene_page gCutscene06Italian[2] = {
    { gCutscene06ItalianPage0, ARRAY_COUNT(gCutscene06ItalianPage0) },
    { gCutscene06ItalianPage1, ARRAY_COUNT(gCutscene06ItalianPage1) },
};
const struct cutscene_page gCutscene07Italian[1] = {
    { gCutscene07ItalianPage0, ARRAY_COUNT(gCutscene07ItalianPage0) },
};
const struct cutscene_page gCutscene08Italian[4] = {
    { gCutscene08ItalianPage0, ARRAY_COUNT(gCutscene08ItalianPage0) },
    { gCutscene08ItalianPage1, ARRAY_COUNT(gCutscene08ItalianPage1) },
    { gCutscene08ItalianPage2, ARRAY_COUNT(gCutscene08ItalianPage2) },
    { gCutscene08ItalianPage3, ARRAY_COUNT(gCutscene08ItalianPage3) },
};
const struct cutscene_page gCutscene09Italian[6] = {
    { NULL, 0 },
    { gCutscene09ItalianPage1, ARRAY_COUNT(gCutscene09ItalianPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09ItalianPage5, ARRAY_COUNT(gCutscene09ItalianPage5) },
};
const struct cutscene_page gCutscene10Italian[6] = {
    { gCutscene10ItalianPage0, ARRAY_COUNT(gCutscene10ItalianPage0) },
    { NULL, 0 },
    { gCutscene09ItalianPage1, ARRAY_COUNT(gCutscene09ItalianPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10ItalianPage5, ARRAY_COUNT(gCutscene10ItalianPage5) },
};
const struct cutscene_page gCutscene00Italian[1] = {
    { NULL, 0 },
};
const struct cutscene_page gCutscene01Dutch[4] = {
    { gCutscene01DutchPage0, ARRAY_COUNT(gCutscene01DutchPage0) },
    { gCutscene01DutchPage1, ARRAY_COUNT(gCutscene01DutchPage1) },
    { gCutscene01DutchPage2, ARRAY_COUNT(gCutscene01DutchPage2) },
    { gCutscene01DutchPage3, ARRAY_COUNT(gCutscene01DutchPage3) },
};
const struct cutscene_page gCutscene02Dutch[9] = {
    { gCutscene02DutchPage0, ARRAY_COUNT(gCutscene02DutchPage0) },
    { gCutscene02DutchPage1, ARRAY_COUNT(gCutscene02DutchPage1) },
    { gCutscene02DutchPage2, ARRAY_COUNT(gCutscene02DutchPage2) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene02DutchPage5, ARRAY_COUNT(gCutscene02DutchPage5) },
    { gCutscene02DutchPage6, ARRAY_COUNT(gCutscene02DutchPage6) },
    { gCutscene02DutchPage7, ARRAY_COUNT(gCutscene02DutchPage7) },
    { gCutscene02DutchPage8, ARRAY_COUNT(gCutscene02DutchPage8) },
};
const struct cutscene_page gCutscene03Dutch[1] = {
    { gCutscene03DutchPage0, ARRAY_COUNT(gCutscene03DutchPage0) },
};
const struct cutscene_page gCutscene04Dutch[1] = {
    { gCutscene04DutchPage0, ARRAY_COUNT(gCutscene04DutchPage0) },
};
const struct cutscene_page gCutscene05Dutch[1] = {
    { gCutscene05DutchPage0, ARRAY_COUNT(gCutscene05DutchPage0) },
};
const struct cutscene_page gCutscene06Dutch[2] = {
    { gCutscene06DutchPage0, ARRAY_COUNT(gCutscene06DutchPage0) },
    { gCutscene06DutchPage1, ARRAY_COUNT(gCutscene06DutchPage1) },
};
const struct cutscene_page gCutscene07Dutch[1] = {
    { gCutscene07DutchPage0, ARRAY_COUNT(gCutscene07DutchPage0) },
};
const struct cutscene_page gCutscene08Dutch[4] = {
    { gCutscene08DutchPage0, ARRAY_COUNT(gCutscene08DutchPage0) },
    { gCutscene08DutchPage1, ARRAY_COUNT(gCutscene08DutchPage1) },
    { gCutscene08DutchPage2, ARRAY_COUNT(gCutscene08DutchPage2) },
    { gCutscene08DutchPage3, ARRAY_COUNT(gCutscene08DutchPage3) },
};
const struct cutscene_page gCutscene09Dutch[6] = {
    { NULL, 0 },
    { gCutscene09DutchPage1, ARRAY_COUNT(gCutscene09DutchPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene09DutchPage5, ARRAY_COUNT(gCutscene09DutchPage5) },
};
const struct cutscene_page gCutscene10Dutch[6] = {
    { gCutscene10DutchPage0, ARRAY_COUNT(gCutscene10DutchPage0) },
    { NULL, 0 },
    { gCutscene09DutchPage1, ARRAY_COUNT(gCutscene09DutchPage1) },
    { NULL, 0 },
    { NULL, 0 },
    { gCutscene10DutchPage5, ARRAY_COUNT(gCutscene10DutchPage5) },
};
const struct cutscene_page gCutscene00Dutch[1] = {
    { NULL, 0 },
};
const struct cutscene_slide gCutsceneSlide00 = {
    gCutscenePicture00, 0, 0, 0, 1, 0, 0, 14, 93,
};
const struct cutscene_slide gCutsceneSlide01 = {
    gCutscenePicture01, 0, 0, 0, 1, 0, 0, 14, 93,
};
const struct cutscene_slide gCutsceneSlide02 = {
    gCutscenePicture02, 0, 0, 0, 1, 0, 0, 14, 93,
};
const struct cutscene_slide gCutsceneSlide03 = {
    gCutscenePicture03, 0, 0, 0, 1, 0, 1, 14, 93,
};
const struct cutscene_slide gCutsceneSlide04 = {
    gCutscenePicture04, 140, 0, 0, 1, 0, 0, 14, 92,
};
const struct cutscene_slide gCutsceneSlide05 = {
    gCutscenePicture05, 90, 0, -1, 1, 0, 0, 14, 94,
};
const struct cutscene_slide gCutsceneSlide06 = {
    gCutscenePicture06, 120, 1, 0, 1, 0, 0, 14, 95,
};
const struct cutscene_slide gCutsceneSlide07 = {
    gCutscenePicture07, 0, 0, 0, 1, 0, 1, 14, 91,
};
const struct cutscene_slide gCutsceneSlide08 = {
    gCutscenePicture09, 0, 0, 0, 1, 0, 1, 14, 93,
};
const struct cutscene_slide gCutsceneSlide09 = {
    gCutscenePicture08, 0, 0, 0, 1, 0, 1, 15, 96,
};
const struct cutscene_slide gCutsceneSlide10 = {
    gCutscenePicture10, 0, 0, 0, 1, 0, 0, 15, 1,
};
const struct cutscene_slide gCutsceneSlide11 = {
    gCutscenePicture11, 0, 0, 0, 1, 0, 1, 15, 93,
};
const struct cutscene_slide gCutsceneSlide12 = {
    gCutscenePicture12, 0, 0, 0, 1, 0, 0, 15, 14,
};
const struct cutscene_slide gCutsceneSlide13 = {
    gCutscenePicture13, 0, 0, 0, 1, 0, 0, 14, 51,
};
const struct cutscene_slide gCutsceneSlide14 = {
    gCutscenePicture14, 0, 0, 0, 1, 0, 0, 14, 29,
};
const struct cutscene_slide gCutsceneSlide15 = {
    gCutscenePicture15, 0, 0, 0, 1, 0, 0, 14, 29,
};
const struct cutscene_slide gCutsceneSlide16 = {
    gCutscenePicture16, 0, 0, 0, 1, 0, 0, 14, 32,
};
const struct cutscene_slide gCutsceneSlide17 = {
    gCutscenePicture17, 90, 0, 0, 1, 0, 0, 15, 97,
};
const struct cutscene_slide gCutsceneSlide18 = {
    gCutscenePicture18, 0, 0, 0, 1, 0, 0, 15, 1,
};
const struct cutscene_slide gCutsceneSlide19 = {
    gCutscenePicture19, 120, 0, 0, 1, 0, 0, 15, 95,
};
const struct cutscene_slide gCutsceneSlide20 = {
    gCutscenePicture20, 120, 0, 0, 1, 0, 0, 15, 98,
};
const struct cutscene_slide gCutsceneSlide21 = {
    gCutscenePicture21, 120, 0, 0, 1, 0, 0, 15, 4,
};
const struct cutscene_slide gCutsceneSlide22 = {
    gCutscenePicture22, 0, 0, 0, 1, 0, 0, 15, 94,
};
const struct cutscene_slide gCutsceneSlide23 = {
    gCutscenePicture23, 120, 0, 0, 0, 0, 0, 13, 99,
};
const u8 gCutscene01FrenchPage0Text[] =
    "a bord d'une myst\351rieuse station spatiale en orbite autour de "
    "la terre ...";
const u8 *const gCutscene01FrenchPage0[1] = { gCutscene01FrenchPage0Text };
const u8 gCutscene01FrenchPage1Text[] =
    "uka uka: cortex, rappelle-moi pourquoi je te garde! tu m'as trop "
    "souvent d\351\347u!";
const u8 *const gCutscene01FrenchPage1[1] = { gCutscene01FrenchPage1Text };
const u8 gCutscene01FrenchPage2Text[] =
    "cortex: pardonne-moi uka uka! j'ai perdu tout ce temps \340 "
    "essayer de me d\351barrasser de ce bandicoot sans cervelle! mais "
    "il n'est rien ... c'est le monde que nous allons conqu\351rir!";
const u8 *const gCutscene01FrenchPage2[1] = { gCutscene01FrenchPage2Text };
const u8 gCutscene01FrenchPage3_0Text[] =
    "uka uka: j'ai d\351j\340 entendu \347a, monsieur le scientifique "
    "du dimanche! pourquoi r\351ussirais-tu cette fois?!";
const u8 gCutscene01FrenchPage3_1Text[] =
    "cortex: fais-moi confiance uka, j'ai un plan pour remettre ces "
    "minables terriens \340 leur place ... ";
const u8 *const gCutscene01FrenchPage3[2] = { gCutscene01FrenchPage3_0Text, gCutscene01FrenchPage3_1Text };
const u8 gCutscene02FrenchPage0Text[] = "cortex: mon r\351ducteur de plan\350te est enfin termin\351!";
const u8 *const gCutscene02FrenchPage0[1] = { gCutscene02FrenchPage0Text };
const u8 gCutscene02FrenchPage1Text[] =
    "cortex: les terriens devront s'incliner devant mon intelligence "
    "sup\351rieure!";
const u8 *const gCutscene02FrenchPage1[1] = { gCutscene02FrenchPage1Text };
const u8 gCutscene02FrenchPage2Text[] = "cortex: ahahahahaha!";
const u8 *const gCutscene02FrenchPage2[1] = { gCutscene02FrenchPage2Text };
const u8 gCutscene02FrenchPage5Text[] =
    "cortex: apr\350s toutes ces ann\351es, la roue a enfin "
    "tourn\351! on fait moins le malin maintenant, hein? moi, n\351o "
    "cortex, je suis le roi du monde. inclinez-vous devant moi! "
    "ahaha!";
const u8 *const gCutscene02FrenchPage5[1] = { gCutscene02FrenchPage5Text };
const u8 gCutscene02FrenchPage7_0Text[] =
    "aku: crash, coco, le monde a besoin de vous. cortex a r\351duit "
    "la plan\350te \340 la taille d'une orange. nous devons agir.";
const u8 gCutscene02FrenchPage7_1Text[] =
    "coco: on dirait que cortex utilise les cristaux pour alimenter "
    "son r\351ducteur ... crash, si tu me ram\350nes les cristaux "
    "ad\351quats, je pense pouvoir mettre au point un appareil "
    "capable d'inverser les effets de la machine de cortex.";
const u8 *const gCutscene02FrenchPage7[2] = { gCutscene02FrenchPage7_0Text, gCutscene02FrenchPage7_1Text };
const u8 gCutscene02FrenchPage6Text[] =
    "cortex: j'ai enfin gagn\351! rien ne peut plus m'arr\352ter, "
    "m\352me pas ce petit bandicoot!";
const u8 *const gCutscene02FrenchPage6[1] = { gCutscene02FrenchPage6Text };
const u8 gCutscene02FrenchPage8Text[] =
    "aku: bonne chance crash. il faut que tu trouves les cristaux "
    "pour mettre fin aux plans diaboliques de cortex.";
const u8 *const gCutscene02FrenchPage8[1] = { gCutscene02FrenchPage8Text };
const u8 gCutscene03FrenchPage0Text[] =
    "cortex: je sais ce que tu mijotes bandicoot, mais tu n'as aucune "
    "chance. mes serviteurs t'attendent de pied ferme!";
const u8 *const gCutscene03FrenchPage0[1] = { gCutscene03FrenchPage0Text };
const u8 gCutscene04FrenchPage0Text[] =
    "cortex: maudit marsupial! tu as eu de la chance cette fois ... "
    "mais mes hommes de main ne se laisseront plus berner si "
    "facilement.";
const u8 *const gCutscene04FrenchPage0[1] = { gCutscene04FrenchPage0Text };
const u8 gCutscene05FrenchPage0Text[] =
    "coco: bien jou\351 crash! j'ai r\351cup\351r\351 la moiti\351 "
    "des cristaux. apporte-moi le reste pour que je finisse de "
    "construire ma machine. mais sois prudent, cortex te surveille!";
const u8 *const gCutscene05FrenchPage0[1] = { gCutscene05FrenchPage0Text };
const u8 gCutscene06FrenchPage0Text[] =
    "uka: cortex, j'en \351tais s\373r! crash a vaincu trois de tes "
    "sous-fifres! il faut l'arr\352ter. je ne tol\351rerai pas un "
    "autre \351chec.";
const u8 *const gCutscene06FrenchPage0[1] = { gCutscene06FrenchPage0Text };
const u8 gCutscene06FrenchPage1Text[] =
    "cortex: uka ... je suis d\351sol\351! mais ne t'inqui\350te pas, "
    "mon r\351ducteur de plan\350te va l'arr\352ter!";
const u8 *const gCutscene06FrenchPage1[1] = { gCutscene06FrenchPage1Text };
const u8 gCutscene07FrenchPage0Text[] =
    "coco: okay crash, j'ai tous les cristaux mais il faut que tu "
    "d\351truises le r\351ducteur de plan\350te avant que je puisse "
    "ramener la terre \340 sa taille normale. j'utiliserai "
    "l'agrandisseur pour que tu sois assez grand pour affronter "
    "cortex!";
const u8 *const gCutscene07FrenchPage0[1] = { gCutscene07FrenchPage0Text };
const u8 gCutscene10FrenchPage0Text[] =
    "cortex: cr\351tin! il va me falloir des ann\351es pour "
    "r\351parer tout \347a!";
const u8 *const gCutscene10FrenchPage0[1] = { gCutscene10FrenchPage0Text };
const u8 gCutscene08FrenchPage0Text[] =
    "cortex: esp\350ce d'imb\351cile! regarde ce que tu as fait?! tu "
    "as d\351truit les cristaux stabilisateurs! si elle n'est pas "
    "ma\356tris\351e, la puissance du r\351ducteur est "
    "impr\351visible!";
const u8 *const gCutscene08FrenchPage0[1] = { gCutscene08FrenchPage0Text };
const u8 *const gCutscene08FrenchPage1[1] = { gCutscene08EnglishPage1Text };
const u8 gCutscene08FrenchPage2Text[] = "monstre: qu'est-ce que tu nous as fait?!";
const u8 *const gCutscene08FrenchPage2[1] = { gCutscene08FrenchPage2Text };
const u8 gCutscene08FrenchPage3Text[] = "monstre: tu vas nous le payer petit bandicoot!";
const u8 *const gCutscene08FrenchPage3[1] = { gCutscene08FrenchPage3Text };
const u8 gCutscene09FrenchPage1Text[] =
    "coco: super! tu as tous les cristaux! pourvu qu'ils soient assez "
    "puissants pour faire rentrer les choses dans l'ordre ...";
const u8 *const gCutscene09FrenchPage1[1] = { gCutscene09FrenchPage1Text };
const u8 gCutscene09FrenchPage5Text[] = "fin?!";
const u8 *const gCutscene09FrenchPage5[1] = { gCutscene09FrenchPage5Text };
const u8 gCutscene10FrenchPage5Text[] =
    "aku: bien jou\351 crash ... mais cortex constitue toujours une "
    "menace car sa station spatiale et son r\351ducteur n'ont pas "
    "\351t\351 d\351truits. tu dois repartir chercher les gemmes.";
const u8 *const gCutscene10FrenchPage5[1] = { gCutscene10FrenchPage5Text };
const u8 gCutscene01GermanPage0Text[] =
    "auf einer mysteri\366sen raumstation im orbit hoch \374ber der "
    "erde ...";
const u8 *const gCutscene01GermanPage0[1] = { gCutscene01GermanPage0Text };
const u8 gCutscene01GermanPage1Text[] =
    "uka uka: cortex, kannst du mir sagen, warum ich dich immer noch "
    "in meiner n\344he dulde? du hast bisher jedesmal versagt!";
const u8 *const gCutscene01GermanPage1[1] = { gCutscene01GermanPage1Text };
const u8 gCutscene01GermanPage2Text[] =
    "cortex: verzeih mir, uka uka! ich habe all diese jahre damit "
    "zugebracht, dieses hirnlose bandicoot zu jagen! aber jetzt haben "
    "wir gr\366\337ere aufgaben vor uns: es gilt, herrscher der welt "
    "zu werden!";
const u8 *const gCutscene01GermanPage2[1] = { gCutscene01GermanPage2Text };
const u8 gCutscene01GermanPage3_0Text[] =
    "uka uka:  das habe ich alles schon einmal geh\366rt, kleiner "
    "wissenschaftler! was ist denn diesmal anders als sonst?";
const u8 gCutscene01GermanPage3_1Text[] =
    "cortex: vertraue mir, uka. ich habe einen kleinen plan, wie ich "
    "die k\374mmerlichen erdbewohner zurechtstutzen kann ... ";
const u8 *const gCutscene01GermanPage3[2] = { gCutscene01GermanPage3_0Text, gCutscene01GermanPage3_1Text };
const u8 gCutscene02GermanPage0Text[] = "cortex: endlich ist mein planetenschrumpfer fertig!";
const u8 *const gCutscene02GermanPage0[1] = { gCutscene02GermanPage0Text };
const u8 gCutscene02GermanPage1Text[] = "cortex: die erdlinge werden sich meiner intelligenz unterwerfen!";
const u8 *const gCutscene02GermanPage1[1] = { gCutscene02GermanPage1Text };
const u8 gCutscene02GermanPage2Text[] = "cortex: hahahahahaha!";
const u8 *const gCutscene02GermanPage2[1] = { gCutscene02GermanPage2Text };
const u8 gCutscene02GermanPage5Text[] =
    "cortex: nach all diesen jahren, in denen ich beschimpft worden "
    "bin, hat sich das blatt endlich gewandelt! jetzt zeigt sich, wer "
    "hier klein gemacht wird. ich, neo cortex, habe jetzt das sagen, "
    "und alle werden zu mir aufblicken! hahaha!";
const u8 *const gCutscene02GermanPage5[1] = { gCutscene02GermanPage5Text };
const u8 gCutscene02GermanPage7_0Text[] =
    "aku: crash, coco, ihr m\374sst der welt zu hilfe kommen. cortex "
    "hat unseren planeten zur gr\366\337e einer pampelmuse "
    "geschrumpft. wir m\374ssen ihn aufhalten.";
const u8 gCutscene02GermanPage7_1Text[] =
    "coco:  sieht so aus, als w\374rde cortex die kristalle als "
    "treibstoff f\374r seine verkleinerungsmaschine benutzen. crash, "
    "wenn du die richtigen kristalle auf der welt findest, kann ich "
    "bestimmt ein ger\344t bauen, das die welt wieder "
    "vergr\366\337ert.";
const u8 *const gCutscene02GermanPage7[2] = { gCutscene02GermanPage7_0Text, gCutscene02GermanPage7_1Text };
const u8 gCutscene02GermanPage6Text[] =
    "cortex: ich habe es endlich geschafft! jetzt kann mich niemand "
    "mehr aufhalten, nicht einmal das kleine bandicoot!";
const u8 *const gCutscene02GermanPage6[1] = { gCutscene02GermanPage6Text };
const u8 gCutscene02GermanPage8Text[] =
    "aku: viel gl\374ck, crash. du musst die kristalle finden, damit "
    "wir cortex in seinem b\366sen vorhaben aufhalten k\366nnen.";
const u8 *const gCutscene02GermanPage8[1] = { gCutscene02GermanPage8Text };
const u8 gCutscene03GermanPage0Text[] =
    "cortex:  ich wei\337, was du vorhast, kleines bandicoot. aber du "
    "hast keine chance. meine helfer sind schon bereit!";
const u8 *const gCutscene03GermanPage0[1] = { gCutscene03GermanPage0Text };
const u8 gCutscene04GermanPage0Text[] =
    "cortex: verdammtes aufdringliches beuteltier! diesmal hast du "
    "noch gl\374ck gehabt. aber n\344chstes mal werden dich meine "
    "leute nicht so leicht entwischen lassen.";
const u8 *const gCutscene04GermanPage0[1] = { gCutscene04GermanPage0Text };
const u8 gCutscene05GermanPage0Text[] =
    "coco:  klasse arbeit, crash! ich habe die h\344lfte der "
    "kristalle. jetzt musst du mir noch die zweite h\344lfte bringen, "
    "damit ich meine maschine fertig stellen kann. aber sei "
    "vorsichtig: cortex beobachtet dich!";
const u8 *const gCutscene05GermanPage0[1] = { gCutscene05GermanPage0Text };
const u8 gCutscene06GermanPage0Text[] =
    "uka: ich habe es doch geahnt, cortex! crash hat drei deiner "
    "helfer geschlagen! er muss endlich aufgehalten werden. ich dulde "
    "es nicht, wieder zu verlieren.";
const u8 *const gCutscene06GermanPage0[1] = { gCutscene06GermanPage0Text };
const u8 gCutscene06GermanPage1Text[] =
    "cortex: tut mir leid, uka! aber mach dir keine sorgen. mein "
    "planetenschrumpfer wird ihn aufhalten.";
const u8 *const gCutscene06GermanPage1[1] = { gCutscene06GermanPage1Text };
const u8 gCutscene07GermanPage0Text[] =
    "coco:  gut, crash, ich habe jetzt alle kristalle. aber du musst "
    "die verkleinerungsmaschine zerst\366ren, bevor ich die "
    "urspr\374ngliche gr\366\337e der erde wieder herstellen kann. "
    "ich werde dich mit dem vergr\366\337erer gro\337 genug machen, "
    "um es mit cortex aufzunehmen!";
const u8 *const gCutscene07GermanPage0[1] = { gCutscene07GermanPage0Text };
const u8 gCutscene10GermanPage0Text[] =
    "cortex: du schwachkopf! ich werde ewig brauchen, das wieder in "
    "ordnung zu bringen!";
const u8 *const gCutscene10GermanPage0[1] = { gCutscene10GermanPage0Text };
const u8 gCutscene08GermanPage0Text[] =
    "cortex: du idiot! was hast du getan! meine "
    "stabilisierungs-kristalle sind zerst\366rt! wenn die "
    "schrumpfmaschine nicht gedrosselt wird, kann ungeahntes "
    "passieren!";
const u8 *const gCutscene08GermanPage0[1] = { gCutscene08GermanPage0Text };
const u8 gCutscene08GermanPage1Text[] = "aahh!  aaahhh!!!";
const u8 *const gCutscene08GermanPage1[1] = { gCutscene08GermanPage1Text };
const u8 gCutscene08GermanPage2Text[] = "monster: was hast du uns angetan?";
const u8 *const gCutscene08GermanPage2[1] = { gCutscene08GermanPage2Text };
const u8 gCutscene08GermanPage3Text[] = "monster: das wirst du b\374\337en, kleines bandicoot!";
const u8 *const gCutscene08GermanPage3[1] = { gCutscene08GermanPage3Text };
const u8 gCutscene09GermanPage1Text[] =
    "coco:  klasse! du hast alle kristalle gefunden. wollen wir "
    "hoffen, dass sie gen\374gend kraft enthalten, um die miese "
    "arbeit von cortex aufzuheben.";
const u8 *const gCutscene09GermanPage1[1] = { gCutscene09GermanPage1Text };
const u8 gCutscene09GermanPage5Text[] = "das ende?";
const u8 *const gCutscene09GermanPage5[1] = { gCutscene09GermanPage5Text };
const u8 gCutscene10GermanPage5Text[] =
    "aku: das hast du sehr gut gemacht, crash. aber cortex ist immer "
    "noch eine bedrohung f\374r uns, da seine raumstation und sein "
    "planetenschrumpfer noch heil sind. du musst noch einmal "
    "zur\374ckkehren und die edelsteine an jedem ort sammeln.";
const u8 *const gCutscene10GermanPage5[1] = { gCutscene10GermanPage5Text };
const u8 gCutscene01SpanishPage0Text[] =
    "en una misteriosa estaci\363n espacial en \363rbita alrededor de "
    "la tierra ...";
const u8 *const gCutscene01SpanishPage0[1] = { gCutscene01SpanishPage0Text };
const u8 gCutscene01SpanishPage1Text[] =
    "uka uka: no s\351 por qu\351 no te he hecho desaparecer, cortex. "
    "has resultado un verdadero fiasco.";
const u8 *const gCutscene01SpanishPage1[1] = { gCutscene01SpanishPage1Text };
const u8 gCutscene01SpanishPage2Text[] =
    "cortex: \241perd\363name, uka uka! he desperdiciado todos estos "
    "a\361os tratando de aniquilar a ese descerebrado de bandicoot "
    "... cuando lo que realmente importa es conquistar el mundo.";
const u8 *const gCutscene01SpanishPage2[1] = { gCutscene01SpanishPage2Text };
const u8 gCutscene01SpanishPage3_0Text[] =
    "uka uka:  \241a otro perro con ese hueso, cient\355fico de "
    "pacotilla! \277por qu\351 va a ser diferente esta vez?";
const u8 gCutscene01SpanishPage3_1Text[] =
    "cortex: conf\355a en m\355, uka. he estado maquinando un "
    "planecillo para bajarles los humos a esos insignificantes "
    "terr\355colas ... ";
const u8 *const gCutscene01SpanishPage3[2] = { gCutscene01SpanishPage3_0Text, gCutscene01SpanishPage3_1Text };
const u8 gCutscene02SpanishPage0Text[] = "cortex: \241por fin he terminado el miniaturizador planetario!";
const u8 *const gCutscene02SpanishPage0[1] = { gCutscene02SpanishPage0Text };
const u8 gCutscene02SpanishPage1Text[] =
    "cortex: \241los terr\355colas tendr\341n que rendirse ante la "
    "superioridad de mi intelecto!";
const u8 *const gCutscene02SpanishPage1[1] = { gCutscene02SpanishPage1Text };
const u8 gCutscene02SpanishPage2Text[] = "cortex: \241muahahahahaha!";
const u8 *const gCutscene02SpanishPage2[1] = { gCutscene02SpanishPage2Text };
const u8 gCutscene02SpanishPage5Text[] =
    "cortex: ahora s\355, despu\351s de todos estos a\361os de "
    "abusos, los papeles se han invertido. \277qui\351n es el "
    "peque\361\355n ahora? yo, neo cortex, soy vuestro soberano. "
    "\241mirad hacia arriba cuando os hable! \241ja ja ja!";
const u8 *const gCutscene02SpanishPage5[1] = { gCutscene02SpanishPage5Text };
const u8 gCutscene02SpanishPage7_0Text[] =
    "aku: crash, coco, el mundo os necesita. cortex ha encogido el "
    "planeta al tama\361o de una naranja. debemos detenerlo.";
const u8 gCutscene02SpanishPage7_1Text[] =
    "coco:  parece que cortex est\341 usando los cristales como "
    "fuente de energ\355a de su 'minimizadora' ... crash, si puedes "
    "encontrar los cristales adecuados por el mundo, creo que "
    "podr\351 construir un dispositivo para contrarrestar los efectos "
    "de la m\341quina de cortex.";
const u8 *const gCutscene02SpanishPage7[2] = { gCutscene02SpanishPage7_0Text, gCutscene02SpanishPage7_1Text };
const u8 gCutscene02SpanishPage6Text[] =
    "cortex: \241por fin he triunfado! \241nada podr\341 detenerme "
    "ahora, ni siquiera ese insignificante bandicoot!";
const u8 *const gCutscene02SpanishPage6[1] = { gCutscene02SpanishPage6Text };
const u8 gCutscene02SpanishPage8Text[] =
    "aku: buena suerte, crash. debes hacerte con los cristales para "
    "que podamos poner fin a los mal\351ficos planes de cortex.";
const u8 *const gCutscene02SpanishPage8[1] = { gCutscene02SpanishPage8Text };
const u8 gCutscene03SpanishPage0Text[] =
    "cortex:  s\351 lo que tratas de hacer, bandicoot, pero no lo "
    "lograr\341s. \241mis secuaces te estar\341n esperando!";
const u8 *const gCutscene03SpanishPage0[1] = { gCutscene03SpanishPage0Text };
const u8 gCutscene04SpanishPage0Text[] =
    "cortex: \241marsupial entrometido! esta vez has tenido suerte "
    "... ya me encargar\351 de que mis esbirros no vuelvan a fallar.";
const u8 *const gCutscene04SpanishPage0[1] = { gCutscene04SpanishPage0Text };
const u8 gCutscene05SpanishPage0Text[] =
    "coco: \241excelente labor, crash! ya tengo la mitad de los "
    "cristales. tr\341eme ahora el resto para que pueda terminar mi "
    "m\341quina, pero \341ndate con cuidado. cortex te estar\341 "
    "vigilando.";
const u8 *const gCutscene05SpanishPage0[1] = { gCutscene05SpanishPage0Text };
const u8 gCutscene06SpanishPage0Text[] =
    "uka: \241ya sab\355a que esto iba a pasar, cortex! crash ha "
    "derrotado a tres de tus engendros. debes detenerlo. no "
    "tolerar\351 otro fracaso.";
const u8 *const gCutscene06SpanishPage0[1] = { gCutscene06SpanishPage0Text };
const u8 gCutscene06SpanishPage1Text[] =
    "cortex: uka ... \241perd\363name! aunque no debes preocuparte "
    "m\341s. el miniaturizador planetario se encargar\341 de \351l.";
const u8 *const gCutscene06SpanishPage1[1] = { gCutscene06SpanishPage1Text };
const u8 gCutscene07SpanishPage0Text[] =
    "coco:  muy bien, crash, ya tengo todos los cristales, pero si no "
    "destruyes la 'minimizadora' no podr\351 devolver a la tierra a "
    "su estado original. voy a hacerte crecer con el 'maximizador' "
    "para que puedas enfrentarte a cortex.";
const u8 *const gCutscene07SpanishPage0[1] = { gCutscene07SpanishPage0Text };
const u8 gCutscene10SpanishPage0Text[] =
    "cortex: \241tonto! \241tardar\351 a\361os en reparar este "
    "l\355o!";
const u8 *const gCutscene10SpanishPage0[1] = { gCutscene10SpanishPage0Text };
const u8 gCutscene08SpanishPage0Text[] =
    "cortex: \241nooo!  \277qu\351 has hecho? \241has destruido los "
    "cristales estabilizantes! \241el miniaturizador est\341 fuera de "
    "control!";
const u8 *const gCutscene08SpanishPage0[1] = { gCutscene08SpanishPage0Text };
const u8 gCutscene08SpanishPage1Text[] = "\241arrgh!  \241\241\241ayyy!!!";
const u8 *const gCutscene08SpanishPage1[1] = { gCutscene08SpanishPage1Text };
const u8 gCutscene08SpanishPage2Text[] = "monstruo: \277qu\351 nos has hecho?";
const u8 *const gCutscene08SpanishPage2[1] = { gCutscene08SpanishPage2Text };
const u8 gCutscene08SpanishPage3Text[] = "monstruo: \241me las pagar\341s, peque\361a sabandija!";
const u8 *const gCutscene08SpanishPage3[1] = { gCutscene08SpanishPage3Text };
const u8 gCutscene09SpanishPage1Text[] =
    "coco: \241fant\341stico! \241te has hecho con todos los "
    "cristales! esperemos que haya la suficiente potencia en ellos "
    "para 'deshacer los entuertos' de cortex ...";
const u8 *const gCutscene09SpanishPage1[1] = { gCutscene09SpanishPage1Text };
const u8 gCutscene09SpanishPage5Text[] = "\277ser\341 el fin?";
const u8 *const gCutscene09SpanishPage5[1] = { gCutscene09SpanishPage5Text };
const u8 gCutscene10SpanishPage5Text[] =
    "aku: buen trabajo, crash, pero cortex sigue siendo una amenaza, "
    "ya que no hemos destruido ni su estaci\363n espacial ni su "
    "miniaturizador planetario. debes regresar y recolectar las gemas "
    "de todos los lugares.";
const u8 *const gCutscene10SpanishPage5[1] = { gCutscene10SpanishPage5Text };
const u8 gCutscene01ItalianPage0Text[] = "su una misteriosa stazione spaziale in orbita sulla terra ...";
const u8 *const gCutscene01ItalianPage0[1] = { gCutscene01ItalianPage0Text };
const u8 gCutscene01ItalianPage1Text[] =
    "uka uka: cortex, non so perch\351 ti permetto di restare. mi hai "
    "deluso una volta di troppo!";
const u8 *const gCutscene01ItalianPage1[1] = { gCutscene01ItalianPage1Text };
const u8 gCutscene01ItalianPage2Text[] =
    "cortex: perdonami, uka uka! per tanti anni ho tentato "
    "inutilmente di sconfiggere quell'inetto di un bandicoot! ma lui "
    "non conta ... domineremo il mondo!";
const u8 *const gCutscene01ItalianPage2[1] = { gCutscene01ItalianPage2Text };
const u8 gCutscene01ItalianPage3_0Text[] =
    "uka uka:  questa l'ho gi\340 sentita, scienziato! cosa c'\350 di "
    "diverso questa volta?!";
const u8 gCutscene01ItalianPage3_1Text[] =
    "cortex: fidati di me uka, ho un piano che ridimensioner\340 gli "
    "abitanti della terra ...";
const u8 *const gCutscene01ItalianPage3[2] = { gCutscene01ItalianPage3_0Text, gCutscene01ItalianPage3_1Text };
const u8 gCutscene02ItalianPage0Text[] = "cortex: il miniaturizzatore planetario \350 pronto!";
const u8 *const gCutscene02ItalianPage0[1] = { gCutscene02ItalianPage0Text };
const u8 gCutscene02ItalianPage1Text[] = "cortex: i terrestri dovranno inchinarsi al mio genio!";
const u8 *const gCutscene02ItalianPage1[1] = { gCutscene02ItalianPage1Text };
const u8 gCutscene02ItalianPage2Text[] = "cortex: muahahahahaha!";
const u8 *const gCutscene02ItalianPage2[1] = { gCutscene02ItalianPage2Text };
const u8 gCutscene02ItalianPage5Text[] =
    "cortex: dopo tante umiliazioni, \350 giunto il mio momento! "
    "dimenticate quel microbo ... io, neo cortex, sono il vostro "
    "padrone. inchinatevi al mio potere! hahaha!";
const u8 *const gCutscene02ItalianPage5[1] = { gCutscene02ItalianPage5Text };
const u8 gCutscene02ItalianPage7_0Text[] =
    "aku: crash, coco, il mondo ha bisogno del vostro aiuto. cortex "
    "ha rimpicciolito la terra. dobbiamo fermarlo!";
const u8 gCutscene02ItalianPage7_1Text[] =
    "coco: cortex sta usando i cristalli per alimentare il suo "
    "miniaturizzatore ... crash, se riesci a trovare i cristalli "
    "giusti, potr\362 costruire un apparecchio per contrastarne gli "
    "effetti.";
const u8 *const gCutscene02ItalianPage7[2] = { gCutscene02ItalianPage7_0Text, gCutscene02ItalianPage7_1Text };
const u8 gCutscene02ItalianPage6Text[] =
    "cortex: ho vinto io, alla fine! nulla potr\340 fermarmi, nemmeno "
    "quel microbo di bandicoot!";
const u8 *const gCutscene02ItalianPage6[1] = { gCutscene02ItalianPage6Text };
const u8 gCutscene02ItalianPage8Text[] =
    "aku: buona fortuna, crash. trova i cristalli e potremo sventare "
    "i diabolici piani di cortex.";
const u8 *const gCutscene02ItalianPage8[1] = { gCutscene02ItalianPage8Text };
const u8 gCutscene03ItalianPage0Text[] =
    "cortex: so cosa stai cercando di fare, bandicoot, ma non "
    "funzioner\340. i miei scagnozzi ti aspettano al varco!";
const u8 *const gCutscene03ItalianPage0[1] = { gCutscene03ItalianPage0Text };
const u8 gCutscene04ItalianPage0Text[] =
    "cortex: marsupiale impiccione! sei stato fortunato, ma la "
    "prossima volta non la passerai liscia.";
const u8 *const gCutscene04ItalianPage0[1] = { gCutscene04ItalianPage0Text };
const u8 gCutscene05ItalianPage0Text[] =
    "coco: ottimo lavoro, crash! abbiamo met\340 dei cristalli. "
    "portami gli altri e finir\362 la mia macchina. ma sta attento, "
    "cortex ti sorveglia!";
const u8 *const gCutscene05ItalianPage0[1] = { gCutscene05ItalianPage0Text };
const u8 gCutscene06ItalianPage0Text[] =
    "uka: cortex, sapevo che sarebbe successo! crash ha sconfitto tre "
    "dei tuoi scagnozzi! dobbiamo fermarlo, non tollerer\362 un altro "
    "fallimento.";
const u8 *const gCutscene06ItalianPage0[1] = { gCutscene06ItalianPage0Text };
const u8 gCutscene06ItalianPage1Text[] =
    "cortex: mi dispiace, uka! ma non preoccuparti, il mio "
    "miniaturizzatore planetario lo fermer\340!";
const u8 *const gCutscene06ItalianPage1[1] = { gCutscene06ItalianPage1Text };
const u8 gCutscene07ItalianPage0Text[] =
    "coco: bene crash, adesso ho tutti i cristalli, ma prima che io "
    "possa riportare la terra alle sue condizioni originali, dovrai "
    "distruggere il miniaturizzatore. user\362 l'ingranditore per "
    "renderti pi\371 potente!";
const u8 *const gCutscene07ItalianPage0[1] = { gCutscene07ItalianPage0Text };
const u8 gCutscene10ItalianPage0Text[] = "cortex: stupido! mi ci vorr\340 una vita per rimediare!";
const u8 *const gCutscene10ItalianPage0[1] = { gCutscene10ItalianPage0Text };
const u8 gCutscene08ItalianPage0Text[] =
    "cortex: idiota, cos'hai combinato?! hai distrutto i cristalli "
    "stabilizzatori! non c'\350 modo di sapere cosa accadr\340, ora!";
const u8 *const gCutscene08ItalianPage0[1] = { gCutscene08ItalianPage0Text };
const u8 *const gCutscene08ItalianPage1[1] = { gCutscene08EnglishPage1Text };
const u8 gCutscene08ItalianPage2Text[] = "mostro: cosa ci hai fatto?!";
const u8 *const gCutscene08ItalianPage2[1] = { gCutscene08ItalianPage2Text };
const u8 gCutscene08ItalianPage3Text[] = "mostro: la pagherai, piccolo bandicoot!";
const u8 *const gCutscene08ItalianPage3[1] = { gCutscene08ItalianPage3Text };
const u8 gCutscene09ItalianPage1Text[] =
    "coco: grandioso, hai trovato tutti i cristalli! speriamo siano "
    "abbastanza potenti da contrastare i malefici piani di cortex ...";
const u8 *const gCutscene09ItalianPage1[1] = { gCutscene09ItalianPage1Text };
const u8 gCutscene09ItalianPage5Text[] = "fine?!";
const u8 *const gCutscene09ItalianPage5[1] = { gCutscene09ItalianPage5Text };
const u8 gCutscene10ItalianPage5Text[] =
    "aku: sei stato bravo, crash ... ma cortex continuer\340 a "
    "minacciarci finch\351 non avremo distrutto la sua stazione "
    "spaziale e il miniaturizzatore planetario. devi tornare indietro "
    "e recuperare tutte le gemme.";
const u8 *const gCutscene10ItalianPage5[1] = { gCutscene10ItalianPage5Text };
const u8 gCutscene01DutchPage0Text[] =
    "op een geheimzinnig ruimtestation, in een baan hoog boven de "
    "aarde ...";
const u8 *const gCutscene01DutchPage0[1] = { gCutscene01DutchPage0Text };
const u8 gCutscene01DutchPage1Text[] =
    "uka uka: cortex, ik snap zelf niet waarom ik je niet wegstuur! "
    "nu heb je me voor het laatst teleurgesteld!";
const u8 *const gCutscene01DutchPage1[1] = { gCutscene01DutchPage1Text };
const u8 gCutscene01DutchPage2Text[] =
    "cortex: uka uka! vergeef me ... ik heb jaren verspild aan het "
    "uitschakelen van die stompzinnige bandicoot! maar hij is niet "
    "belangrijk ... wij zullen over de wereld heersen!";
const u8 *const gCutscene01DutchPage2[1] = { gCutscene01DutchPage2Text };
const u8 gCutscene01DutchPage3_0Text[] =
    "uka uka:  ach professortje, dat heb ik al zo vaak gehoord!  "
    "waarom zou ik je nu geloven?";
const u8 gCutscene01DutchPage3_1Text[] =
    "cortex: vertrouw me nu maar, uka, ik heb een duivels plan "
    "ontwikkeld om de aardebewoners een kopje kleiner te maken ...";
const u8 *const gCutscene01DutchPage3[2] = { gCutscene01DutchPage3_0Text, gCutscene01DutchPage3_1Text };
const u8 gCutscene02DutchPage0Text[] = "cortex: eindelijk is mijn planetenverkleiner klaar!";
const u8 *const gCutscene02DutchPage0[1] = { gCutscene02DutchPage0Text };
const u8 gCutscene02DutchPage1Text[] =
    "cortex: de bewoners van de aarde zullen het afleggen tegen mijn "
    "superieure intellect!";
const u8 *const gCutscene02DutchPage1[1] = { gCutscene02DutchPage1Text };
const u8 *const gCutscene02DutchPage2[1] = { gCutscene02ItalianPage2Text };
const u8 gCutscene02DutchPage5Text[] =
    "cortex: na vele jaren ellende ben ik nu eindelijk de baas! wie "
    "is nu de zwakkeling? ik, neo cortex, ben je heerser. je zult "
    "tegen mij opkijken!  hahaha!";
const u8 *const gCutscene02DutchPage5[1] = { gCutscene02DutchPage5Text };
const u8 gCutscene02DutchPage7_0Text[] =
    "aku: crash, coco, de wereld heeft jullie nodig.  cortex heeft "
    "onze planeet verkleind tot het formaat van een grapefruit.  we "
    "moeten hem tegenhouden.";
const u8 gCutscene02DutchPage7_1Text[] =
    "coco:  het lijkt erop dat cortex de kristallen gebruikt als "
    "brandstof voor zijn verkleiningsmachine. crash, als jij de "
    "juiste kristallen die over de wereld verspreid zijn op kunt "
    "halen, kan ik een machine bouwen om het effect tegen te gaan.";
const u8 *const gCutscene02DutchPage7[2] = { gCutscene02DutchPage7_0Text, gCutscene02DutchPage7_1Text };
const u8 gCutscene02DutchPage6Text[] =
    "cortex: eindelijk heb ik gewonnen!  niemand kan me nu meer "
    "stoppen, zelfs die kleine bandicoot niet!";
const u8 *const gCutscene02DutchPage6[1] = { gCutscene02DutchPage6Text };
const u8 gCutscene02DutchPage8Text[] =
    "aku: succes, crash.  we moeten de kristallen zoeken, zodat we "
    "een stokje kunnen steken voor de plannen van cortex.";
const u8 *const gCutscene02DutchPage8[1] = { gCutscene02DutchPage8Text };
const u8 gCutscene03DutchPage0Text[] =
    "cortex:  ik begrijp best wat je probeert te doen, bandicoot, "
    "maar het lukt je toch niet. mijn helpers staan al op je te "
    "wachten!";
const u8 *const gCutscene03DutchPage0[1] = { gCutscene03DutchPage0Text };
const u8 gCutscene04DutchPage0Text[] =
    "cortex: vervelend buideldier! je hebt gewoon geluk gehad, de "
    "volgende keer zullen mijn handlangers het je niet zo gemakkelijk "
    "maken.";
const u8 *const gCutscene04DutchPage0[1] = { gCutscene04DutchPage0Text };
const u8 gCutscene05DutchPage0Text[] =
    "coco:  prima, crash!  ik heb nu de helft van de kristallen. ga "
    "je de rest even zoeken, dan kan ik mijn machine afmaken. maar "
    "wees wel voorzichtig, want cortex houdt je in de gaten!";
const u8 *const gCutscene05DutchPage0[1] = { gCutscene05DutchPage0Text };
const u8 gCutscene06DutchPage0Text[] =
    "uka: cortex, ik wist dat dit zou gebeuren!  crash heeft drie van "
    "je knechten verslagen. we moeten hem tegenhouden. ik accepteer "
    "geen nederlaag meer.";
const u8 *const gCutscene06DutchPage0[1] = { gCutscene06DutchPage0Text };
const u8 gCutscene06DutchPage1Text[] =
    "cortex: uka ... het spijt me, maar maak je niet al te veel "
    "zorgen, mijn planetenvernietiger maakt hem wel een kopje "
    "kleiner.";
const u8 *const gCutscene06DutchPage1[1] = { gCutscene06DutchPage1Text };
const u8 gCutscene07DutchPage0Text[] =
    "coco:  ok\351 crash, ik heb alle kristallen, maar ik kan de "
    "aarde pas herstellen als je de verkleiningsmachine hebt "
    "vernietigd. ik zal de vergroter gebruiken, zodat je groot genoeg "
    "bent om het tegen cortex op te nemen!";
const u8 *const gCutscene07DutchPage0[1] = { gCutscene07DutchPage0Text };
const u8 gCutscene10DutchPage0Text[] =
    "cortex: idioot!  ik heb jaren nodig om dat allemaal weer recht "
    "te trekken!";
const u8 *const gCutscene10DutchPage0[1] = { gCutscene10DutchPage0Text };
const u8 gCutscene08DutchPage0Text[] =
    "cortex: stomkop!  wat heb je gedaan?!  je hebt de stabiliserende "
    "kristallen vernietigd! nu is de kracht van de verkleiner totaal "
    "onvoorspelbaar!";
const u8 *const gCutscene08DutchPage0[1] = { gCutscene08DutchPage0Text };
const u8 *const gCutscene08DutchPage1[1] = { gCutscene08EnglishPage1Text };
const u8 gCutscene08DutchPage2Text[] = "monster: wat heb je met ons gedaan?!";
const u8 *const gCutscene08DutchPage2[1] = { gCutscene08DutchPage2Text };
const u8 gCutscene08DutchPage3Text[] = "monster: daar zul je voor boeten, kleine bandicoot!";
const u8 *const gCutscene08DutchPage3[1] = { gCutscene08DutchPage3Text };
const u8 gCutscene09DutchPage1Text[] =
    "coco:  wauw! je hebt alle kristallen! nou maar hopen dat ze "
    "krachtig genoeg zijn om de acties van cortex ongedaan te maken "
    "...";
const u8 *const gCutscene09DutchPage1[1] = { gCutscene09DutchPage1Text };
const u8 *const gCutscene09DutchPage5[1] = { gCutscene09EnglishPage5Text };
const u8 gCutscene10DutchPage5Text[] =
    "aku: crash, prima ... maar we zijn nog niet van cortex af, want "
    "we hebben zijn ruimtestation en de planetenverkleiner nog niet "
    "vernietigd. je moet teruggaan en alle edelstenen ophalen.";
const u8 *const gCutscene10DutchPage5[1] = { gCutscene10DutchPage5Text };

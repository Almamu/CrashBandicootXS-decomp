#include "game/ui/cutscene.hpp"
#include "graphics.hpp"

extern "C" {
#include "core.h"
#include "constants/sfx.h"
#include "constants/songs.h"
}

using Engine::Gfx::CutscenePicture00;
using Engine::Gfx::CutscenePicture01;
using Engine::Gfx::CutscenePicture02;
using Engine::Gfx::CutscenePicture03;
using Engine::Gfx::CutscenePicture04;
using Engine::Gfx::CutscenePicture05;
using Engine::Gfx::CutscenePicture06;
using Engine::Gfx::CutscenePicture07;
using Engine::Gfx::CutscenePicture08;
using Engine::Gfx::CutscenePicture09;
using Engine::Gfx::CutscenePicture10;
using Engine::Gfx::CutscenePicture11;
using Engine::Gfx::CutscenePicture12;
using Engine::Gfx::CutscenePicture13;
using Engine::Gfx::CutscenePicture14;
using Engine::Gfx::CutscenePicture15;
using Engine::Gfx::CutscenePicture16;
using Engine::Gfx::CutscenePicture17;
using Engine::Gfx::CutscenePicture18;
using Engine::Gfx::CutscenePicture19;
using Engine::Gfx::CutscenePicture20;
using Engine::Gfx::CutscenePicture21;
using Engine::Gfx::CutscenePicture22;
using Engine::Gfx::CutscenePicture23;

/*
 * ROM 0x0816E064-0x0816E8A4: the cutscenes' French, German, Spanish,
 * Italian and Dutch pages (`struct cutscene_page` arrays, listed by the
 * language tables in cutscenes.cpp) and the 24 slides. The pages'
 * strings are in cutscene_text_translations.cpp, the pictures in
 * cutscene_pictures.cpp. Linked in ROM order by ldscript.txt.
 */

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
extern const u8 *const gCutscene01FrenchPage0[1];
extern const u8 *const gCutscene01FrenchPage1[1];
extern const u8 *const gCutscene01FrenchPage2[1];
extern const u8 *const gCutscene01FrenchPage3[2];
extern const u8 *const gCutscene02FrenchPage0[1];
extern const u8 *const gCutscene02FrenchPage1[1];
extern const u8 *const gCutscene02FrenchPage2[1];
extern const u8 *const gCutscene02FrenchPage5[1];
extern const u8 *const gCutscene02FrenchPage7[2];
extern const u8 *const gCutscene02FrenchPage6[1];
extern const u8 *const gCutscene02FrenchPage8[1];
extern const u8 *const gCutscene03FrenchPage0[1];
extern const u8 *const gCutscene04FrenchPage0[1];
extern const u8 *const gCutscene05FrenchPage0[1];
extern const u8 *const gCutscene06FrenchPage0[1];
extern const u8 *const gCutscene06FrenchPage1[1];
extern const u8 *const gCutscene07FrenchPage0[1];
extern const u8 *const gCutscene10FrenchPage0[1];
extern const u8 *const gCutscene08FrenchPage0[1];
extern const u8 *const gCutscene08FrenchPage1[1];
extern const u8 *const gCutscene08FrenchPage2[1];
extern const u8 *const gCutscene08FrenchPage3[1];
extern const u8 *const gCutscene09FrenchPage1[1];
extern const u8 *const gCutscene09FrenchPage5[1];
extern const u8 *const gCutscene10FrenchPage5[1];
extern const u8 *const gCutscene01GermanPage0[1];
extern const u8 *const gCutscene01GermanPage1[1];
extern const u8 *const gCutscene01GermanPage2[1];
extern const u8 *const gCutscene01GermanPage3[2];
extern const u8 *const gCutscene02GermanPage0[1];
extern const u8 *const gCutscene02GermanPage1[1];
extern const u8 *const gCutscene02GermanPage2[1];
extern const u8 *const gCutscene02GermanPage5[1];
extern const u8 *const gCutscene02GermanPage7[2];
extern const u8 *const gCutscene02GermanPage6[1];
extern const u8 *const gCutscene02GermanPage8[1];
extern const u8 *const gCutscene03GermanPage0[1];
extern const u8 *const gCutscene04GermanPage0[1];
extern const u8 *const gCutscene05GermanPage0[1];
extern const u8 *const gCutscene06GermanPage0[1];
extern const u8 *const gCutscene06GermanPage1[1];
extern const u8 *const gCutscene07GermanPage0[1];
extern const u8 *const gCutscene10GermanPage0[1];
extern const u8 *const gCutscene08GermanPage0[1];
extern const u8 *const gCutscene08GermanPage1[1];
extern const u8 *const gCutscene08GermanPage2[1];
extern const u8 *const gCutscene08GermanPage3[1];
extern const u8 *const gCutscene09GermanPage1[1];
extern const u8 *const gCutscene09GermanPage5[1];
extern const u8 *const gCutscene10GermanPage5[1];
extern const u8 *const gCutscene01SpanishPage0[1];
extern const u8 *const gCutscene01SpanishPage1[1];
extern const u8 *const gCutscene01SpanishPage2[1];
extern const u8 *const gCutscene01SpanishPage3[2];
extern const u8 *const gCutscene02SpanishPage0[1];
extern const u8 *const gCutscene02SpanishPage1[1];
extern const u8 *const gCutscene02SpanishPage2[1];
extern const u8 *const gCutscene02SpanishPage5[1];
extern const u8 *const gCutscene02SpanishPage7[2];
extern const u8 *const gCutscene02SpanishPage6[1];
extern const u8 *const gCutscene02SpanishPage8[1];
extern const u8 *const gCutscene03SpanishPage0[1];
extern const u8 *const gCutscene04SpanishPage0[1];
extern const u8 *const gCutscene05SpanishPage0[1];
extern const u8 *const gCutscene06SpanishPage0[1];
extern const u8 *const gCutscene06SpanishPage1[1];
extern const u8 *const gCutscene07SpanishPage0[1];
extern const u8 *const gCutscene10SpanishPage0[1];
extern const u8 *const gCutscene08SpanishPage0[1];
extern const u8 *const gCutscene08SpanishPage1[1];
extern const u8 *const gCutscene08SpanishPage2[1];
extern const u8 *const gCutscene08SpanishPage3[1];
extern const u8 *const gCutscene09SpanishPage1[1];
extern const u8 *const gCutscene09SpanishPage5[1];
extern const u8 *const gCutscene10SpanishPage5[1];
extern const u8 *const gCutscene01ItalianPage0[1];
extern const u8 *const gCutscene01ItalianPage1[1];
extern const u8 *const gCutscene01ItalianPage2[1];
extern const u8 *const gCutscene01ItalianPage3[2];
extern const u8 *const gCutscene02ItalianPage0[1];
extern const u8 *const gCutscene02ItalianPage1[1];
extern const u8 *const gCutscene02ItalianPage2[1];
extern const u8 *const gCutscene02ItalianPage5[1];
extern const u8 *const gCutscene02ItalianPage7[2];
extern const u8 *const gCutscene02ItalianPage6[1];
extern const u8 *const gCutscene02ItalianPage8[1];
extern const u8 *const gCutscene03ItalianPage0[1];
extern const u8 *const gCutscene04ItalianPage0[1];
extern const u8 *const gCutscene05ItalianPage0[1];
extern const u8 *const gCutscene06ItalianPage0[1];
extern const u8 *const gCutscene06ItalianPage1[1];
extern const u8 *const gCutscene07ItalianPage0[1];
extern const u8 *const gCutscene10ItalianPage0[1];
extern const u8 *const gCutscene08ItalianPage0[1];
extern const u8 *const gCutscene08ItalianPage1[1];
extern const u8 *const gCutscene08ItalianPage2[1];
extern const u8 *const gCutscene08ItalianPage3[1];
extern const u8 *const gCutscene09ItalianPage1[1];
extern const u8 *const gCutscene09ItalianPage5[1];
extern const u8 *const gCutscene10ItalianPage5[1];
extern const u8 *const gCutscene01DutchPage0[1];
extern const u8 *const gCutscene01DutchPage1[1];
extern const u8 *const gCutscene01DutchPage2[1];
extern const u8 *const gCutscene01DutchPage3[2];
extern const u8 *const gCutscene02DutchPage0[1];
extern const u8 *const gCutscene02DutchPage1[1];
extern const u8 *const gCutscene02DutchPage2[1];
extern const u8 *const gCutscene02DutchPage5[1];
extern const u8 *const gCutscene02DutchPage7[2];
extern const u8 *const gCutscene02DutchPage6[1];
extern const u8 *const gCutscene02DutchPage8[1];
extern const u8 *const gCutscene03DutchPage0[1];
extern const u8 *const gCutscene04DutchPage0[1];
extern const u8 *const gCutscene05DutchPage0[1];
extern const u8 *const gCutscene06DutchPage0[1];
extern const u8 *const gCutscene06DutchPage1[1];
extern const u8 *const gCutscene07DutchPage0[1];
extern const u8 *const gCutscene10DutchPage0[1];
extern const u8 *const gCutscene08DutchPage0[1];
extern const u8 *const gCutscene08DutchPage1[1];
extern const u8 *const gCutscene08DutchPage2[1];
extern const u8 *const gCutscene08DutchPage3[1];
extern const u8 *const gCutscene09DutchPage1[1];
extern const u8 *const gCutscene09DutchPage5[1];
extern const u8 *const gCutscene10DutchPage5[1];

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
    { 0, 0 },
    { 0, 0 },
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
    { 0, 0 }, { gCutscene09FrenchPage1, ARRAY_COUNT(gCutscene09FrenchPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09FrenchPage5, ARRAY_COUNT(gCutscene09FrenchPage5) },
};
const struct cutscene_page gCutscene10French[6] = {
    { gCutscene10FrenchPage0, ARRAY_COUNT(gCutscene10FrenchPage0) },
    { 0, 0 },
    { gCutscene09FrenchPage1, ARRAY_COUNT(gCutscene09FrenchPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10FrenchPage5, ARRAY_COUNT(gCutscene10FrenchPage5) },
};
const struct cutscene_page gCutscene00French[1] = {
    { 0, 0 },
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
    { 0, 0 },
    { 0, 0 },
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
    { 0, 0 }, { gCutscene09GermanPage1, ARRAY_COUNT(gCutscene09GermanPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09GermanPage5, ARRAY_COUNT(gCutscene09GermanPage5) },
};
const struct cutscene_page gCutscene10German[6] = {
    { gCutscene10GermanPage0, ARRAY_COUNT(gCutscene10GermanPage0) },
    { 0, 0 },
    { gCutscene09GermanPage1, ARRAY_COUNT(gCutscene09GermanPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10GermanPage5, ARRAY_COUNT(gCutscene10GermanPage5) },
};
const struct cutscene_page gCutscene00German[1] = {
    { 0, 0 },
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
    { 0, 0 },
    { 0, 0 },
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
    { 0, 0 }, { gCutscene09SpanishPage1, ARRAY_COUNT(gCutscene09SpanishPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09SpanishPage5, ARRAY_COUNT(gCutscene09SpanishPage5) },
};
const struct cutscene_page gCutscene10Spanish[6] = {
    { gCutscene10SpanishPage0, ARRAY_COUNT(gCutscene10SpanishPage0) },
    { 0, 0 },
    { gCutscene09SpanishPage1, ARRAY_COUNT(gCutscene09SpanishPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10SpanishPage5, ARRAY_COUNT(gCutscene10SpanishPage5) },
};
const struct cutscene_page gCutscene00Spanish[1] = {
    { 0, 0 },
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
    { 0, 0 },
    { 0, 0 },
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
    { 0, 0 }, { gCutscene09ItalianPage1, ARRAY_COUNT(gCutscene09ItalianPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09ItalianPage5, ARRAY_COUNT(gCutscene09ItalianPage5) },
};
const struct cutscene_page gCutscene10Italian[6] = {
    { gCutscene10ItalianPage0, ARRAY_COUNT(gCutscene10ItalianPage0) },
    { 0, 0 },
    { gCutscene09ItalianPage1, ARRAY_COUNT(gCutscene09ItalianPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10ItalianPage5, ARRAY_COUNT(gCutscene10ItalianPage5) },
};
const struct cutscene_page gCutscene00Italian[1] = {
    { 0, 0 },
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
    { 0, 0 },
    { 0, 0 },
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
    { 0, 0 }, { gCutscene09DutchPage1, ARRAY_COUNT(gCutscene09DutchPage1) },
    { 0, 0 }, { 0, 0 },
    { 0, 0 }, { gCutscene09DutchPage5, ARRAY_COUNT(gCutscene09DutchPage5) },
};
const struct cutscene_page gCutscene10Dutch[6] = {
    { gCutscene10DutchPage0, ARRAY_COUNT(gCutscene10DutchPage0) },
    { 0, 0 },
    { gCutscene09DutchPage1, ARRAY_COUNT(gCutscene09DutchPage1) },
    { 0, 0 },
    { 0, 0 },
    { gCutscene10DutchPage5, ARRAY_COUNT(gCutscene10DutchPage5) },
};
const struct cutscene_page gCutscene00Dutch[1] = {
    { 0, 0 },
};
const struct cutscene_slide gCutsceneSlide00 = {
    CutscenePicture00::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide01 = {
    CutscenePicture01::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide02 = {
    CutscenePicture02::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide03 = {
    CutscenePicture03::palette, 0, 0, 0, 1, 0, 1, SONG_CUTSCENES_SPOOKY, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide04 = {
    CutscenePicture04::palette, 140, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_CORTEX_TARGET_BLINK,
};
const struct cutscene_slide gCutsceneSlide05 = {
    CutscenePicture05::palette, 90, 0, -1, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_MINIMIZER_BEAM,
};
const struct cutscene_slide gCutsceneSlide06 = {
    CutscenePicture06::palette, 120, 1, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_UNKNOWN_5F,
};
const struct cutscene_slide gCutsceneSlide07 = {
    CutscenePicture07::palette, 0, 0, 0, 1, 0, 1, SONG_CUTSCENES_SPOOKY, SFX_UNKNOWN_5B,
};
const struct cutscene_slide gCutsceneSlide08 = {
    CutscenePicture09::palette, 0, 0, 0, 1, 0, 1, SONG_CUTSCENES_SPOOKY, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide09 = {
    CutscenePicture08::palette, 0, 0, 0, 1, 0, 1, SONG_INTRO, SFX_UNKNOWN_60,
};
const struct cutscene_slide gCutsceneSlide10 = {
    CutscenePicture10::palette, 0, 0, 0, 1, 0, 0, SONG_INTRO, SFX_AKU_AKU_GAIN,
};
const struct cutscene_slide gCutsceneSlide11 = {
    CutscenePicture11::palette, 0, 0, 0, 1, 0, 1, SONG_INTRO, SFX_SPACE_STATION_AMBIENCE,
};
const struct cutscene_slide gCutsceneSlide12 = {
    CutscenePicture12::palette, 0, 0, 0, 1, 0, 0, SONG_INTRO, SFX_HUD_COLLECT,
};
const struct cutscene_slide gCutsceneSlide13 = {
    CutscenePicture13::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_ELECTRIC_SHOCK,
};
const struct cutscene_slide gCutsceneSlide14 = {
    CutscenePicture14::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_UNKNOWN_1D,
};
const struct cutscene_slide gCutsceneSlide15 = {
    CutscenePicture15::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_UNKNOWN_1D,
};
const struct cutscene_slide gCutsceneSlide16 = {
    CutscenePicture16::palette, 0, 0, 0, 1, 0, 0, SONG_CUTSCENES_SPOOKY, SFX_YETI_CHASE,
};
const struct cutscene_slide gCutsceneSlide17 = {
    CutscenePicture17::palette, 90, 0, 0, 1, 0, 0, SONG_INTRO, SFX_UNKNOWN_61,
};
const struct cutscene_slide gCutsceneSlide18 = {
    CutscenePicture18::palette, 0, 0, 0, 1, 0, 0, SONG_INTRO, SFX_AKU_AKU_GAIN,
};
const struct cutscene_slide gCutsceneSlide19 = {
    CutscenePicture19::palette, 120, 0, 0, 1, 0, 0, SONG_INTRO, SFX_UNKNOWN_5F,
};
const struct cutscene_slide gCutsceneSlide20 = {
    CutscenePicture20::palette, 120, 0, 0, 1, 0, 0, SONG_INTRO, SFX_UNKNOWN_62,
};
const struct cutscene_slide gCutsceneSlide21 = {
    CutscenePicture21::palette, 120, 0, 0, 1, 0, 0, SONG_INTRO, SFX_EXPLOSION,
};
const struct cutscene_slide gCutsceneSlide22 = {
    CutscenePicture22::palette, 0, 0, 0, 1, 0, 0, SONG_INTRO, SFX_MINIMIZER_BEAM,
};
const struct cutscene_slide gCutsceneSlide23 = {
    CutscenePicture23::palette, 120, 0, 0, 0, 0, 0, SONG_CUTSCENES, SFX_NONE,
};

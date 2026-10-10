/*
 * ROM 0x0816D4D8-0x0816E064: the cutscenes' English text. Each string
 * is followed by the array of a page's strings (gCutsceneNNEnglishPageN)
 * that the English `struct cutscene_page` arrays point at
 * (cutscenes_16d1c8.cpp). Latin-1, all in lower case.
 *
 * C, not C++ like the rest of src/: each page array points at the string
 * defined just before it, and g++ 2.9 (decay_conversion, cp/typeck.c)
 * replaces a const array initialised from a string literal by that
 * literal wherever the array decays to a pointer, so the page arrays
 * would point at new copies of the strings. C's default_conversion
 * leaves arrays alone. The file includes nothing; see docs/data.md,
 * "Text tables in C". Linked in ROM order by ldscript.txt.
 */

typedef unsigned char u8;

const u8 gCutscene01EnglishPage0Text[] =
    "on a mysterious space station orbiting high above the earth ...";
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
const u8 *const gCutscene01EnglishPage3[2] = { gCutscene01EnglishPage3_0Text,
                                               gCutscene01EnglishPage3_1Text };
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
const u8 *const gCutscene02EnglishPage7[2] = { gCutscene02EnglishPage7_0Text,
                                               gCutscene02EnglishPage7_1Text };
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
const u8 gCutscene06EnglishPage1Text[] = "cortex:  uka ...  i'm sorry!  however, do not be overly "
                                         "concerned - my planetary minimizer will stop him!";
const u8 *const gCutscene06EnglishPage1[1] = { gCutscene06EnglishPage1Text };
const u8 gCutscene07EnglishPage0Text[] =
    "coco:  okay crash, i have all of the crystals, but i need you to "
    "destroy the shrinking machine before i can return earth to its "
    "original state.  i'll use the enlarger to make you big enough to "
    "take on cortex!";
const u8 *const gCutscene07EnglishPage0[1] = { gCutscene07EnglishPage0Text };
const u8 gCutscene10EnglishPage0Text[] =
    "cortex:  you fool!  it will take me forever to fix this mess!";
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

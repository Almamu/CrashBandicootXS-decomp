/*
 * ROM 0x0816E8A4-0x081725A8: the cutscenes' French, German, Spanish,
 * Italian and Dutch text, as cutscene_text_16d4d8.c has the English:
 * each string, then the array of a page's strings that the language's
 * `struct cutscene_page` arrays point at (cutscene_slides_16e064.cpp).
 * Four pages reuse an English string. Latin-1, all in lower case.
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

extern const u8 gCutscene08EnglishPage1Text[];
extern const u8 gCutscene09EnglishPage5Text[];

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
const u8 *const gCutscene01FrenchPage3[2] = { gCutscene01FrenchPage3_0Text,
                                              gCutscene01FrenchPage3_1Text };
const u8 gCutscene02FrenchPage0Text[] =
    "cortex: mon r\351ducteur de plan\350te est enfin termin\351!";
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
const u8 *const gCutscene02FrenchPage7[2] = { gCutscene02FrenchPage7_0Text,
                                              gCutscene02FrenchPage7_1Text };
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
const u8 gCutscene10FrenchPage0Text[] = "cortex: cr\351tin! il va me falloir des ann\351es pour "
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
const u8 *const gCutscene01GermanPage3[2] = { gCutscene01GermanPage3_0Text,
                                              gCutscene01GermanPage3_1Text };
const u8 gCutscene02GermanPage0Text[] = "cortex: endlich ist mein planetenschrumpfer fertig!";
const u8 *const gCutscene02GermanPage0[1] = { gCutscene02GermanPage0Text };
const u8 gCutscene02GermanPage1Text[] =
    "cortex: die erdlinge werden sich meiner intelligenz unterwerfen!";
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
const u8 *const gCutscene02GermanPage7[2] = { gCutscene02GermanPage7_0Text,
                                              gCutscene02GermanPage7_1Text };
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
const u8 gCutscene08GermanPage0Text[] = "cortex: du idiot! was hast du getan! meine "
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
const u8 *const gCutscene01SpanishPage3[2] = { gCutscene01SpanishPage3_0Text,
                                               gCutscene01SpanishPage3_1Text };
const u8 gCutscene02SpanishPage0Text[] =
    "cortex: \241por fin he terminado el miniaturizador planetario!";
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
const u8 *const gCutscene02SpanishPage7[2] = { gCutscene02SpanishPage7_0Text,
                                               gCutscene02SpanishPage7_1Text };
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
const u8 gCutscene01ItalianPage0Text[] =
    "su una misteriosa stazione spaziale in orbita sulla terra ...";
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
const u8 *const gCutscene01ItalianPage3[2] = { gCutscene01ItalianPage3_0Text,
                                               gCutscene01ItalianPage3_1Text };
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
const u8 *const gCutscene02ItalianPage7[2] = { gCutscene02ItalianPage7_0Text,
                                               gCutscene02ItalianPage7_1Text };
const u8 gCutscene02ItalianPage6Text[] =
    "cortex: ho vinto io, alla fine! nulla potr\340 fermarmi, nemmeno "
    "quel microbo di bandicoot!";
const u8 *const gCutscene02ItalianPage6[1] = { gCutscene02ItalianPage6Text };
const u8 gCutscene02ItalianPage8Text[] =
    "aku: buona fortuna, crash. trova i cristalli e potremo sventare "
    "i diabolici piani di cortex.";
const u8 *const gCutscene02ItalianPage8[1] = { gCutscene02ItalianPage8Text };
const u8 gCutscene03ItalianPage0Text[] = "cortex: so cosa stai cercando di fare, bandicoot, ma non "
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
const u8 gCutscene06ItalianPage1Text[] = "cortex: mi dispiace, uka! ma non preoccuparti, il mio "
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
const u8 *const gCutscene01DutchPage3[2] = { gCutscene01DutchPage3_0Text,
                                             gCutscene01DutchPage3_1Text };
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
const u8 *const gCutscene02DutchPage7[2] = { gCutscene02DutchPage7_0Text,
                                             gCutscene02DutchPage7_1Text };
const u8 gCutscene02DutchPage6Text[] = "cortex: eindelijk heb ik gewonnen!  niemand kan me nu meer "
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
const u8 gCutscene06DutchPage1Text[] = "cortex: uka ... het spijt me, maar maak je niet al te veel "
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

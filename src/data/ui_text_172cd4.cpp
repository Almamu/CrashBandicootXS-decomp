extern "C" {
#include "core.h"
#include "system.h"
}

/*
 * ROM 0x08172CD4-0x08174BE0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The game's own text (menus, level names, popups, credits labels) in
 * the six languages: per language an array of 70 strings, which
 * GetUiText (`text id -> string`) reads through the IWRAM table
 * gUiTextTables (src/iwram/iwram_data.cpp), indexed by the language
 * setting gLanguage. Each language's new strings come before its
 * array; strings that are the same in several languages (the level names,
 * mostly) are stored once, under the first language that uses them.
 * Latin-1, all lower case. */
extern const u8 gUiTextEnglish00[] = "level";
extern const u8 gUiTextEnglish01[] = "jungle jam";
extern const u8 gUiTextEnglish02[] = "shipwrecked";
extern const u8 gUiTextEnglish03[] = "temple of boom";
extern const u8 gUiTextEnglish04[] = "frostbite cavern";
extern const u8 gUiTextEnglish05[] = "just in slime";
extern const u8 gUiTextEnglish06[] = "snow crash";
extern const u8 gUiTextEnglish07[] = "rocket racket";
extern const u8 gUiTextEnglish08[] = "just hangin'";
extern const u8 gUiTextEnglish09[] = "shark attack";
extern const u8 gUiTextEnglish10[] = "ruined";
extern const u8 gUiTextEnglish11[] = "snow job";
extern const u8 gUiTextEnglish12[] = "ace of space";
extern const u8 gUiTextEnglish13[] = "sunken city";
extern const u8 gUiTextEnglish14[] = "down the hole";
extern const u8 gUiTextEnglish15[] = "blimp bonanza";
extern const u8 gUiTextEnglish16[] = "star to finish";
extern const u8 gUiTextEnglish17[] = "air supply";
extern const u8 gUiTextEnglish18[] = "no-fly zone";
extern const u8 gUiTextEnglish19[] = "drip, drip, drip";
extern const u8 gUiTextEnglish20[] = "final countdown";
extern const u8 gUiTextEnglish21[] = "tiny";
extern const u8 gUiTextEnglish22[] = "dingodile";
extern const u8 gUiTextEnglish23[] = "n. gin";
extern const u8 gUiTextEnglish24[] = "neo cortex";
extern const u8 gUiTextEnglish25[] = "mega-mix";
extern const u8 gUiTextEnglish26[] = "new game";
extern const u8 gUiTextEnglish27[] = "load game";
extern const u8 gUiTextEnglish28[] = "load link game";
extern const u8 gUiTextEnglish29[] = "delete game";
extern const u8 gUiTextEnglish30[] = "save game";
extern const u8 gUiTextEnglish31[] = "quit";
extern const u8 gUiTextEnglish32[] = "exit";
extern const u8 gUiTextEnglish33[] = "error saving data";
extern const u8 gUiTextEnglish34[] = "save ok";
extern const u8 gUiTextEnglish35[] = "cancel";
extern const u8 gUiTextEnglish36[] = "complete";
extern const u8 gUiTextEnglish37[] = "empty";
extern const u8 gUiTextEnglish38[] = "delete?";
extern const u8 gUiTextEnglish39[] = "overwrite?";
extern const u8 gUiTextEnglish40[] = "continue?";
extern const u8 gUiTextEnglish41[] = "yes";
extern const u8 gUiTextEnglish42[] = "no";
extern const u8 gUiTextEnglish43[] = "transferring data";
extern const u8 gUiTextEnglish44[] = "error transferring data";
extern const u8 gUiTextEnglish45[] = "b button to abort";
extern const u8 gUiTextEnglish46[] = "push start to continue";
extern const u8 gUiTextEnglish47[] = "start:  load-save";
extern const u8 gUiTextEnglish48[] = "music";
extern const u8 gUiTextEnglish49[] = "sound";
extern const u8 gUiTextEnglish50[] = "resume";
extern const u8 gUiTextEnglish51[] = "warp room";
extern const u8 gUiTextEnglish52[] = "restart trial";
extern const u8 gUiTextEnglish53[] = "powers";
extern const u8 gUiTextEnglish54[] = "crystals";
extern const u8 gUiTextEnglish55[] = "gems";
extern const u8 gUiTextEnglish56[] = "relics";
extern const u8 gUiTextEnglish57[] = "time trial";
extern const u8 gUiTextEnglish58[] = "none";
extern const u8 gUiTextEnglish59[] = "credits";
extern const u8 gUiTextEnglish60[] = "super body slam";
extern const u8 gUiTextEnglish61[] = "double jump";
extern const u8 gUiTextEnglish62[] = "tornado spin";
extern const u8 gUiTextEnglish63[] = "turbo run";
extern const u8 gUiTextEnglish64[] = "push R in mid-air to super body slam.  this destroys crates and nearby enemies.";
extern const u8 gUiTextEnglish65[] = "push A in mid-air to jump higher.";
extern const u8 gUiTextEnglish66[] = "push B repeatedly to spin longer and temporarily float.";
extern const u8 gUiTextEnglish67[] = "hold L for a burst of speed.";
extern const u8 gUiTextEnglish68[] = "bonus";
extern const u8 gUiTextEnglish69[] = "pause";

const u8 *const gUiTextEnglish[70] = {
    STRING_ADDR(gUiTextEnglish00),
    STRING_ADDR(gUiTextEnglish01),
    STRING_ADDR(gUiTextEnglish02),
    STRING_ADDR(gUiTextEnglish03),
    STRING_ADDR(gUiTextEnglish04),
    STRING_ADDR(gUiTextEnglish05),
    STRING_ADDR(gUiTextEnglish06),
    STRING_ADDR(gUiTextEnglish07),
    STRING_ADDR(gUiTextEnglish08),
    STRING_ADDR(gUiTextEnglish09),
    STRING_ADDR(gUiTextEnglish10),
    STRING_ADDR(gUiTextEnglish11),
    STRING_ADDR(gUiTextEnglish12),
    STRING_ADDR(gUiTextEnglish13),
    STRING_ADDR(gUiTextEnglish14),
    STRING_ADDR(gUiTextEnglish15),
    STRING_ADDR(gUiTextEnglish16),
    STRING_ADDR(gUiTextEnglish17),
    STRING_ADDR(gUiTextEnglish18),
    STRING_ADDR(gUiTextEnglish19),
    STRING_ADDR(gUiTextEnglish20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextEnglish26),
    STRING_ADDR(gUiTextEnglish27),
    STRING_ADDR(gUiTextEnglish28),
    STRING_ADDR(gUiTextEnglish29),
    STRING_ADDR(gUiTextEnglish30),
    STRING_ADDR(gUiTextEnglish31),
    STRING_ADDR(gUiTextEnglish32),
    STRING_ADDR(gUiTextEnglish33),
    STRING_ADDR(gUiTextEnglish34),
    STRING_ADDR(gUiTextEnglish35),
    STRING_ADDR(gUiTextEnglish36),
    STRING_ADDR(gUiTextEnglish37),
    STRING_ADDR(gUiTextEnglish38),
    STRING_ADDR(gUiTextEnglish39),
    STRING_ADDR(gUiTextEnglish40),
    STRING_ADDR(gUiTextEnglish41),
    STRING_ADDR(gUiTextEnglish42),
    STRING_ADDR(gUiTextEnglish43),
    STRING_ADDR(gUiTextEnglish44),
    STRING_ADDR(gUiTextEnglish45),
    STRING_ADDR(gUiTextEnglish46),
    STRING_ADDR(gUiTextEnglish47),
    STRING_ADDR(gUiTextEnglish48),
    STRING_ADDR(gUiTextEnglish49),
    STRING_ADDR(gUiTextEnglish50),
    STRING_ADDR(gUiTextEnglish51),
    STRING_ADDR(gUiTextEnglish52),
    STRING_ADDR(gUiTextEnglish53),
    STRING_ADDR(gUiTextEnglish54),
    STRING_ADDR(gUiTextEnglish55),
    STRING_ADDR(gUiTextEnglish56),
    STRING_ADDR(gUiTextEnglish57),
    STRING_ADDR(gUiTextEnglish58),
    STRING_ADDR(gUiTextEnglish59),
    STRING_ADDR(gUiTextEnglish60),
    STRING_ADDR(gUiTextEnglish61),
    STRING_ADDR(gUiTextEnglish62),
    STRING_ADDR(gUiTextEnglish63),
    STRING_ADDR(gUiTextEnglish64),
    STRING_ADDR(gUiTextEnglish65),
    STRING_ADDR(gUiTextEnglish66),
    STRING_ADDR(gUiTextEnglish67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};

extern const u8 gUiTextFrench00[] = "niveau";
extern const u8 gUiTextFrench01[] = "jungle en d\351lire";
extern const u8 gUiTextFrench02[] = "crash\351 coul\351";
extern const u8 gUiTextFrench03[] = "le temple maudit";
extern const u8 gUiTextFrench04[] = "caverne des glaces";
extern const u8 gUiTextFrench05[] = "univers gluant";
extern const u8 gUiTextFrench06[] = "crash-neige";
extern const u8 gUiTextFrench07[] = "crashta rocket";
extern const u8 gUiTextFrench08[] = "accroche-toi";
extern const u8 gUiTextFrench09[] = "gare au requin!";
extern const u8 gUiTextFrench10[] = "la cit\351 des ruines";
extern const u8 gUiTextFrench11[] = "tombe la neige";
extern const u8 gUiTextFrench12[] = "l'as de l'espace";
extern const u8 gUiTextFrench13[] = "la cit\351 engloutie";
extern const u8 gUiTextFrench14[] = "sous-terre";
extern const u8 gUiTextFrench15[] = "zeppelinland";
extern const u8 gUiTextFrench16[] = "\351toile crashante";
extern const u8 gUiTextFrench17[] = "aero-crash";
extern const u8 gUiTextFrench18[] = "no-man's-air";
extern const u8 gUiTextFrench19[] = "goutte \340 goutte";
extern const u8 gUiTextFrench20[] = "compte \340 rebours";
extern const u8 gUiTextFrench26[] = "nouvelle partie";
extern const u8 gUiTextFrench27[] = "charger partie";
extern const u8 gUiTextFrench28[] = "charg. partie link";
extern const u8 gUiTextFrench29[] = "supprimer partie";
extern const u8 gUiTextFrench30[] = "sauvegarder partie";
extern const u8 gUiTextFrench31[] = "quitter";
extern const u8 gUiTextFrench33[] = "erreur sauvegarde donn\351es";
extern const u8 gUiTextFrench34[] = "sauvegarde ok";
extern const u8 gUiTextFrench35[] = "annuler";
extern const u8 gUiTextFrench36[] = "termin\351";
extern const u8 gUiTextFrench37[] = "vide";
extern const u8 gUiTextFrench38[] = "supprimer?";
extern const u8 gUiTextFrench39[] = "ecraser?";
extern const u8 gUiTextFrench40[] = "continuer?";
extern const u8 gUiTextFrench41[] = "oui";
extern const u8 gUiTextFrench42[] = "non";
extern const u8 gUiTextFrench43[] = "transfert des donn\351es";
extern const u8 gUiTextFrench44[] = "erreur transfert donn\351es";
extern const u8 gUiTextFrench45[] = "appuyer sur B pour abandonner";
extern const u8 gUiTextFrench46[] = "appuyer sur start pour continuer";
extern const u8 gUiTextFrench47[] = "start pour charg-sauveg";
extern const u8 gUiTextFrench48[] = "musique";
extern const u8 gUiTextFrench49[] = "son";
extern const u8 gUiTextFrench50[] = "reprendre";
extern const u8 gUiTextFrench51[] = "s. spatio-t.";
extern const u8 gUiTextFrench52[] = "recom. clm";
extern const u8 gUiTextFrench53[] = "pouvoirs";
extern const u8 gUiTextFrench54[] = "cristaux";
extern const u8 gUiTextFrench55[] = "gemmes";
extern const u8 gUiTextFrench56[] = "reliques";
extern const u8 gUiTextFrench57[] = "contre la m.";
extern const u8 gUiTextFrench58[] = "aucun";
extern const u8 gUiTextFrench59[] = "cr\351dits";
extern const u8 gUiTextFrench60[] = "super \351crasement";
extern const u8 gUiTextFrench61[] = "double saut";
extern const u8 gUiTextFrench62[] = "tornade";
extern const u8 gUiTextFrench63[] = "turbo";
extern const u8 gUiTextFrench64[] = "super \351crasement: en l'air, appuyer sur R pour tout d\351truire";
extern const u8 gUiTextFrench65[] = "sauter plus haut: en l'air, appuyer sur A";
extern const u8 gUiTextFrench66[] = "tourbillonner longtemps + flotter: appuyer plusieurs fois sur B";
extern const u8 gUiTextFrench67[] = "acc\351l\351ration: maintenir L enfonc\351";

const u8 *const gUiTextFrench[70] = {
    STRING_ADDR(gUiTextFrench00),
    STRING_ADDR(gUiTextFrench01),
    STRING_ADDR(gUiTextFrench02),
    STRING_ADDR(gUiTextFrench03),
    STRING_ADDR(gUiTextFrench04),
    STRING_ADDR(gUiTextFrench05),
    STRING_ADDR(gUiTextFrench06),
    STRING_ADDR(gUiTextFrench07),
    STRING_ADDR(gUiTextFrench08),
    STRING_ADDR(gUiTextFrench09),
    STRING_ADDR(gUiTextFrench10),
    STRING_ADDR(gUiTextFrench11),
    STRING_ADDR(gUiTextFrench12),
    STRING_ADDR(gUiTextFrench13),
    STRING_ADDR(gUiTextFrench14),
    STRING_ADDR(gUiTextFrench15),
    STRING_ADDR(gUiTextFrench16),
    STRING_ADDR(gUiTextFrench17),
    STRING_ADDR(gUiTextFrench18),
    STRING_ADDR(gUiTextFrench19),
    STRING_ADDR(gUiTextFrench20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextFrench26),
    STRING_ADDR(gUiTextFrench27),
    STRING_ADDR(gUiTextFrench28),
    STRING_ADDR(gUiTextFrench29),
    STRING_ADDR(gUiTextFrench30),
    STRING_ADDR(gUiTextFrench31),
    STRING_ADDR(gUiTextFrench31),
    STRING_ADDR(gUiTextFrench33),
    STRING_ADDR(gUiTextFrench34),
    STRING_ADDR(gUiTextFrench35),
    STRING_ADDR(gUiTextFrench36),
    STRING_ADDR(gUiTextFrench37),
    STRING_ADDR(gUiTextFrench38),
    STRING_ADDR(gUiTextFrench39),
    STRING_ADDR(gUiTextFrench40),
    STRING_ADDR(gUiTextFrench41),
    STRING_ADDR(gUiTextFrench42),
    STRING_ADDR(gUiTextFrench43),
    STRING_ADDR(gUiTextFrench44),
    STRING_ADDR(gUiTextFrench45),
    STRING_ADDR(gUiTextFrench46),
    STRING_ADDR(gUiTextFrench47),
    STRING_ADDR(gUiTextFrench48),
    STRING_ADDR(gUiTextFrench49),
    STRING_ADDR(gUiTextFrench50),
    STRING_ADDR(gUiTextFrench51),
    STRING_ADDR(gUiTextFrench52),
    STRING_ADDR(gUiTextFrench53),
    STRING_ADDR(gUiTextFrench54),
    STRING_ADDR(gUiTextFrench55),
    STRING_ADDR(gUiTextFrench56),
    STRING_ADDR(gUiTextFrench57),
    STRING_ADDR(gUiTextFrench58),
    STRING_ADDR(gUiTextFrench59),
    STRING_ADDR(gUiTextFrench60),
    STRING_ADDR(gUiTextFrench61),
    STRING_ADDR(gUiTextFrench62),
    STRING_ADDR(gUiTextFrench63),
    STRING_ADDR(gUiTextFrench64),
    STRING_ADDR(gUiTextFrench65),
    STRING_ADDR(gUiTextFrench66),
    STRING_ADDR(gUiTextFrench67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};

extern const u8 gUiTextGerman01[] = "dschungelkoller";
extern const u8 gUiTextGerman02[] = "schiffbruch";
extern const u8 gUiTextGerman03[] = "schauertempel";
extern const u8 gUiTextGerman04[] = "frosth\366hle";
extern const u8 gUiTextGerman05[] = "alles in glibber";
extern const u8 gUiTextGerman06[] = "schnee-crash";
extern const u8 gUiTextGerman07[] = "raketen-radau";
extern const u8 gUiTextGerman08[] = "gut abgehangen";
extern const u8 gUiTextGerman09[] = "am hai vorbei";
extern const u8 gUiTextGerman10[] = "ruin\366s";
extern const u8 gUiTextGerman11[] = "schneegeschmiere";
extern const u8 gUiTextGerman12[] = "traum im raum";
extern const u8 gUiTextGerman13[] = "versunkene stadt";
extern const u8 gUiTextGerman14[] = "den ausguss hinab";
extern const u8 gUiTextGerman15[] = "blimp-gefecht";
extern const u8 gUiTextGerman16[] = "man sieht sterne";
extern const u8 gUiTextGerman17[] = "sch\366n luft holen";
extern const u8 gUiTextGerman18[] = "fliegen verboten";
extern const u8 gUiTextGerman19[] = "feucht und muffig";
extern const u8 gUiTextGerman20[] = "letzter countdown";
extern const u8 gUiTextGerman26[] = "neues spiel";
extern const u8 gUiTextGerman27[] = "spiel laden";
extern const u8 gUiTextGerman28[] = "link-spiel laden";
extern const u8 gUiTextGerman29[] = "spiel l\366schen";
extern const u8 gUiTextGerman30[] = "spiel speichern";
extern const u8 gUiTextGerman31[] = "beenden";
extern const u8 gUiTextGerman33[] = "datenspeicherungs-fehler";
extern const u8 gUiTextGerman34[] = "speicherung ok";
extern const u8 gUiTextGerman35[] = "abbrechen";
extern const u8 gUiTextGerman36[] = "fertig";
extern const u8 gUiTextGerman37[] = "leer";
extern const u8 gUiTextGerman38[] = "l\366schen?";
extern const u8 gUiTextGerman39[] = "\374berschreiben?";
extern const u8 gUiTextGerman40[] = "fortfahren?";
extern const u8 gUiTextGerman41[] = "ja";
extern const u8 gUiTextGerman42[] = "nein";
extern const u8 gUiTextGerman43[] = "daten werden \374bertragen";
extern const u8 gUiTextGerman44[] = "daten\374bertrag.-fehler";
extern const u8 gUiTextGerman45[] = "b-knopf: abbrechen";
extern const u8 gUiTextGerman46[] = "'start': fortfahren";
extern const u8 gUiTextGerman47[] = "'start': laden-speichern";
extern const u8 gUiTextGerman48[] = "musik";
extern const u8 gUiTextGerman50[] = "fortsetzen";
extern const u8 gUiTextGerman51[] = "zeitspr.-raum";
extern const u8 gUiTextGerman52[] = "zeitmodus neu";
extern const u8 gUiTextGerman53[] = "kr\344fte";
extern const u8 gUiTextGerman54[] = "kristalle";
extern const u8 gUiTextGerman55[] = "edelsteine";
extern const u8 gUiTextGerman56[] = "relikte";
extern const u8 gUiTextGerman57[] = "zeitmodus";
extern const u8 gUiTextGerman58[] = "keine";
extern const u8 gUiTextGerman59[] = "mitwirkende";
extern const u8 gUiTextGerman60[] = "super-bodenwurf";
extern const u8 gUiTextGerman61[] = "doppelsprung";
extern const u8 gUiTextGerman62[] = "tornado-drehung";
extern const u8 gUiTextGerman63[] = "turbo-lauf";
extern const u8 gUiTextGerman64[] = "super-bodenwurf: in der luft R dr\374cken. zerst\366rt kisten und gegner";
extern const u8 gUiTextGerman65[] = "h\366her springen: in der luft A dr\374cken";
extern const u8 gUiTextGerman66[] = "schneller drehen und zeitweilig gleiten: B wiederholt dr\374cken";
extern const u8 gUiTextGerman67[] = "geschwindigkeitsschub: L gedr\374ckt halten";

const u8 *const gUiTextGerman[70] = {
    STRING_ADDR(gUiTextEnglish00),
    STRING_ADDR(gUiTextGerman01),
    STRING_ADDR(gUiTextGerman02),
    STRING_ADDR(gUiTextGerman03),
    STRING_ADDR(gUiTextGerman04),
    STRING_ADDR(gUiTextGerman05),
    STRING_ADDR(gUiTextGerman06),
    STRING_ADDR(gUiTextGerman07),
    STRING_ADDR(gUiTextGerman08),
    STRING_ADDR(gUiTextGerman09),
    STRING_ADDR(gUiTextGerman10),
    STRING_ADDR(gUiTextGerman11),
    STRING_ADDR(gUiTextGerman12),
    STRING_ADDR(gUiTextGerman13),
    STRING_ADDR(gUiTextGerman14),
    STRING_ADDR(gUiTextGerman15),
    STRING_ADDR(gUiTextGerman16),
    STRING_ADDR(gUiTextGerman17),
    STRING_ADDR(gUiTextGerman18),
    STRING_ADDR(gUiTextGerman19),
    STRING_ADDR(gUiTextGerman20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextGerman26),
    STRING_ADDR(gUiTextGerman27),
    STRING_ADDR(gUiTextGerman28),
    STRING_ADDR(gUiTextGerman29),
    STRING_ADDR(gUiTextGerman30),
    STRING_ADDR(gUiTextGerman31),
    STRING_ADDR(gUiTextGerman31),
    STRING_ADDR(gUiTextGerman33),
    STRING_ADDR(gUiTextGerman34),
    STRING_ADDR(gUiTextGerman35),
    STRING_ADDR(gUiTextGerman36),
    STRING_ADDR(gUiTextGerman37),
    STRING_ADDR(gUiTextGerman38),
    STRING_ADDR(gUiTextGerman39),
    STRING_ADDR(gUiTextGerman40),
    STRING_ADDR(gUiTextGerman41),
    STRING_ADDR(gUiTextGerman42),
    STRING_ADDR(gUiTextGerman43),
    STRING_ADDR(gUiTextGerman44),
    STRING_ADDR(gUiTextGerman45),
    STRING_ADDR(gUiTextGerman46),
    STRING_ADDR(gUiTextGerman47),
    STRING_ADDR(gUiTextGerman48),
    STRING_ADDR(gUiTextEnglish49),
    STRING_ADDR(gUiTextGerman50),
    STRING_ADDR(gUiTextGerman51),
    STRING_ADDR(gUiTextGerman52),
    STRING_ADDR(gUiTextGerman53),
    STRING_ADDR(gUiTextGerman54),
    STRING_ADDR(gUiTextGerman55),
    STRING_ADDR(gUiTextGerman56),
    STRING_ADDR(gUiTextGerman57),
    STRING_ADDR(gUiTextGerman58),
    STRING_ADDR(gUiTextGerman59),
    STRING_ADDR(gUiTextGerman60),
    STRING_ADDR(gUiTextGerman61),
    STRING_ADDR(gUiTextGerman62),
    STRING_ADDR(gUiTextGerman63),
    STRING_ADDR(gUiTextGerman64),
    STRING_ADDR(gUiTextGerman65),
    STRING_ADDR(gUiTextGerman66),
    STRING_ADDR(gUiTextGerman67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};

extern const u8 gUiTextSpanish00[] = "nivel";
extern const u8 gUiTextSpanish01[] = "liado entre lianas";
extern const u8 gUiTextSpanish02[] = "n\341ufragos";
extern const u8 gUiTextSpanish03[] = "templo explosivo";
extern const u8 gUiTextSpanish04[] = "la caverna helada";
extern const u8 gUiTextSpanish05[] = "cieno que me hundo";
extern const u8 gUiTextSpanish06[] = "la nieve y yo";
extern const u8 gUiTextSpanish07[] = "como un cohete";
extern const u8 gUiTextSpanish08[] = "aqu\355, colgado";
extern const u8 gUiTextSpanish09[] = "tibur\363n a la carga";
extern const u8 gUiTextSpanish10[] = "en la ruina";
extern const u8 gUiTextSpanish11[] = "aventura nevada";
extern const u8 gUiTextSpanish12[] = "dadme espacio";
extern const u8 gUiTextSpanish13[] = "ciudad sumergida";
extern const u8 gUiTextSpanish14[] = "ca\355da libre";
extern const u8 gUiTextSpanish15[] = "bonanza en el aire";
extern const u8 gUiTextSpanish16[] = "estrellado";
extern const u8 gUiTextSpanish17[] = "a falta de aire";
extern const u8 gUiTextSpanish18[] = "zona de exclusi\363n";
extern const u8 gUiTextSpanish19[] = "gota a gota";
extern const u8 gUiTextSpanish20[] = "tres, dos, uno...";
extern const u8 gUiTextSpanish26[] = "partida nueva";
extern const u8 gUiTextSpanish27[] = "cargar partida";
extern const u8 gUiTextSpanish28[] = "c. part. conexi\363n ";
extern const u8 gUiTextSpanish29[] = "borrar partida";
extern const u8 gUiTextSpanish30[] = "grabar partida";
extern const u8 gUiTextSpanish31[] = "abandonar";
extern const u8 gUiTextSpanish32[] = "salir";
extern const u8 gUiTextSpanish33[] = "error al grabar datos";
extern const u8 gUiTextSpanish34[] = "grabar ok";
extern const u8 gUiTextSpanish35[] = "cancelar";
extern const u8 gUiTextSpanish36[] = "complet.";
extern const u8 gUiTextSpanish37[] = "vac\355a";
extern const u8 gUiTextSpanish38[] = "\277borrar?";
extern const u8 gUiTextSpanish39[] = "\277sobrescribir?";
extern const u8 gUiTextSpanish40[] = "\277continuar?";
extern const u8 gUiTextSpanish41[] = "s\355";
extern const u8 gUiTextSpanish43[] = "transfiriendo datos";
extern const u8 gUiTextSpanish44[] = "error de transferencia";
extern const u8 gUiTextSpanish45[] = "bot\363n b para detener";
extern const u8 gUiTextSpanish46[] = "pulsa start para continuar";
extern const u8 gUiTextSpanish47[] = "cargar-grabar -> start";
extern const u8 gUiTextSpanish48[] = "m\372sica";
extern const u8 gUiTextSpanish49[] = "sonido";
extern const u8 gUiTextSpanish50[] = "reanudar";
extern const u8 gUiTextSpanish51[] = "s. del tiempo";
extern const u8 gUiTextSpanish52[] = "contrarreloj";
extern const u8 gUiTextSpanish53[] = "poderes";
extern const u8 gUiTextSpanish54[] = "cristales";
extern const u8 gUiTextSpanish55[] = "gemas";
extern const u8 gUiTextSpanish56[] = "reliquias";
extern const u8 gUiTextSpanish58[] = "agotados";
extern const u8 gUiTextSpanish59[] = "ficha t\351cnica";
extern const u8 gUiTextSpanish60[] = "golpetazo corporal";
extern const u8 gUiTextSpanish61[] = "doble salto";
extern const u8 gUiTextSpanish62[] = "giro de tornado";
extern const u8 gUiTextSpanish63[] = "turbocarrera";
extern const u8 gUiTextSpanish64[] = "pulsa R en el aire para el golpetazo corporal que destruye cajas y enemigos";
extern const u8 gUiTextSpanish65[] = "pulsa A en el aire para saltar m\341s alto";
extern const u8 gUiTextSpanish66[] = "pulsa varias veces B para girar m\341s y flotar temporalmente";
extern const u8 gUiTextSpanish67[] = "mant\351n pulsado L para lograr un impulso turbo";

const u8 *const gUiTextSpanish[70] = {
    STRING_ADDR(gUiTextSpanish00),
    STRING_ADDR(gUiTextSpanish01),
    STRING_ADDR(gUiTextSpanish02),
    STRING_ADDR(gUiTextSpanish03),
    STRING_ADDR(gUiTextSpanish04),
    STRING_ADDR(gUiTextSpanish05),
    STRING_ADDR(gUiTextSpanish06),
    STRING_ADDR(gUiTextSpanish07),
    STRING_ADDR(gUiTextSpanish08),
    STRING_ADDR(gUiTextSpanish09),
    STRING_ADDR(gUiTextSpanish10),
    STRING_ADDR(gUiTextSpanish11),
    STRING_ADDR(gUiTextSpanish12),
    STRING_ADDR(gUiTextSpanish13),
    STRING_ADDR(gUiTextSpanish14),
    STRING_ADDR(gUiTextSpanish15),
    STRING_ADDR(gUiTextSpanish16),
    STRING_ADDR(gUiTextSpanish17),
    STRING_ADDR(gUiTextSpanish18),
    STRING_ADDR(gUiTextSpanish19),
    STRING_ADDR(gUiTextSpanish20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextSpanish26),
    STRING_ADDR(gUiTextSpanish27),
    STRING_ADDR(gUiTextSpanish28),
    STRING_ADDR(gUiTextSpanish29),
    STRING_ADDR(gUiTextSpanish30),
    STRING_ADDR(gUiTextSpanish31),
    STRING_ADDR(gUiTextSpanish32),
    STRING_ADDR(gUiTextSpanish33),
    STRING_ADDR(gUiTextSpanish34),
    STRING_ADDR(gUiTextSpanish35),
    STRING_ADDR(gUiTextSpanish36),
    STRING_ADDR(gUiTextSpanish37),
    STRING_ADDR(gUiTextSpanish38),
    STRING_ADDR(gUiTextSpanish39),
    STRING_ADDR(gUiTextSpanish40),
    STRING_ADDR(gUiTextSpanish41),
    STRING_ADDR(gUiTextEnglish42),
    STRING_ADDR(gUiTextSpanish43),
    STRING_ADDR(gUiTextSpanish44),
    STRING_ADDR(gUiTextSpanish45),
    STRING_ADDR(gUiTextSpanish46),
    STRING_ADDR(gUiTextSpanish47),
    STRING_ADDR(gUiTextSpanish48),
    STRING_ADDR(gUiTextSpanish49),
    STRING_ADDR(gUiTextSpanish50),
    STRING_ADDR(gUiTextSpanish51),
    STRING_ADDR(gUiTextSpanish52),
    STRING_ADDR(gUiTextSpanish53),
    STRING_ADDR(gUiTextSpanish54),
    STRING_ADDR(gUiTextSpanish55),
    STRING_ADDR(gUiTextSpanish56),
    STRING_ADDR(gUiTextSpanish52),
    STRING_ADDR(gUiTextSpanish58),
    STRING_ADDR(gUiTextSpanish59),
    STRING_ADDR(gUiTextSpanish60),
    STRING_ADDR(gUiTextSpanish61),
    STRING_ADDR(gUiTextSpanish62),
    STRING_ADDR(gUiTextSpanish63),
    STRING_ADDR(gUiTextSpanish64),
    STRING_ADDR(gUiTextSpanish65),
    STRING_ADDR(gUiTextSpanish66),
    STRING_ADDR(gUiTextSpanish67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};

extern const u8 gUiTextItalian00[] = "livello";
extern const u8 gUiTextItalian01[] = "caos nella giungla";
extern const u8 gUiTextItalian02[] = "naufragio";
extern const u8 gUiTextItalian03[] = "tempio di boom!";
extern const u8 gUiTextItalian04[] = "antro di ghiaccio";
extern const u8 gUiTextItalian05[] = "fango mortale";
extern const u8 gUiTextItalian06[] = "valanga arancione";
extern const u8 gUiTextItalian07[] = "la gang del razzo";
extern const u8 gUiTextItalian08[] = "col fiato sospeso";
extern const u8 gUiTextItalian09[] = "squali affamati";
extern const u8 gUiTextItalian10[] = "tra le rovine";
extern const u8 gUiTextItalian11[] = "scende la neve";
extern const u8 gUiTextItalian12[] = "spaziale!";
extern const u8 gUiTextItalian13[] = "citt\340 sommersa";
extern const u8 gUiTextItalian14[] = "sotto terra";
extern const u8 gUiTextItalian15[] = "al dirigibile!";
extern const u8 gUiTextItalian16[] = "spazi siderali";
extern const u8 gUiTextItalian17[] = "riserva d'aria";
extern const u8 gUiTextItalian18[] = "vietato volare";
extern const u8 gUiTextItalian19[] = "plic, plic, plic";
extern const u8 gUiTextItalian20[] = "meno 10... 9";
extern const u8 gUiTextItalian26[] = "nuova partita";
extern const u8 gUiTextItalian27[] = "carica partita";
extern const u8 gUiTextItalian28[] = "carica via link";
extern const u8 gUiTextItalian29[] = "cancella partita";
extern const u8 gUiTextItalian30[] = "salva partita";
extern const u8 gUiTextItalian31[] = "abbandona";
extern const u8 gUiTextItalian32[] = "esci";
extern const u8 gUiTextItalian33[] = "errore nel salvataggio";
extern const u8 gUiTextItalian34[] = "salvataggio ok";
extern const u8 gUiTextItalian35[] = "annulla";
extern const u8 gUiTextItalian36[] = "completo";
extern const u8 gUiTextItalian37[] = "vuoto";
extern const u8 gUiTextItalian38[] = "cancellare?";
extern const u8 gUiTextItalian39[] = "sovrascrivere?";
extern const u8 gUiTextItalian40[] = "continuare?";
extern const u8 gUiTextItalian41[] = "s\354";
extern const u8 gUiTextItalian43[] = "trasferimento in corso";
extern const u8 gUiTextItalian44[] = "errore nel trasferimento";
extern const u8 gUiTextItalian45[] = "pulsante B per interrompere";
extern const u8 gUiTextItalian46[] = "premi start per continuare";
extern const u8 gUiTextItalian47[] = "carica-salva con start";
extern const u8 gUiTextItalian48[] = "musica";
extern const u8 gUiTextItalian49[] = "suono";
extern const u8 gUiTextItalian50[] = "riprendi";
extern const u8 gUiTextItalian51[] = "teletraspo.";
extern const u8 gUiTextItalian52[] = "riprova gara";
extern const u8 gUiTextItalian53[] = "potenziatori";
extern const u8 gUiTextItalian54[] = "cristalli";
extern const u8 gUiTextItalian55[] = "gemme";
extern const u8 gUiTextItalian56[] = "reliquie";
extern const u8 gUiTextItalian57[] = "gara a tempo";
extern const u8 gUiTextItalian58[] = "nessuno";
extern const u8 gUiTextItalian59[] = "crediti";
extern const u8 gUiTextItalian60[] = "super panciata";
extern const u8 gUiTextItalian61[] = "salto doppio";
extern const u8 gUiTextItalian62[] = "giravolta tornado";
extern const u8 gUiTextItalian63[] = "corsa turbo";
extern const u8 gUiTextItalian64[] = "premi R in volo e la panciata distrugger\340 casse e nemici";
extern const u8 gUiTextItalian65[] = "premi A in volo per saltare pi\371 in alto";
extern const u8 gUiTextItalian66[] = "premi B ripetutamente per girare pi\371 a lungo e fluttuare";
extern const u8 gUiTextItalian67[] = "tieni premuto L per una rapida accelerata";

const u8 *const gUiTextItalian[70] = {
    STRING_ADDR(gUiTextItalian00),
    STRING_ADDR(gUiTextItalian01),
    STRING_ADDR(gUiTextItalian02),
    STRING_ADDR(gUiTextItalian03),
    STRING_ADDR(gUiTextItalian04),
    STRING_ADDR(gUiTextItalian05),
    STRING_ADDR(gUiTextItalian06),
    STRING_ADDR(gUiTextItalian07),
    STRING_ADDR(gUiTextItalian08),
    STRING_ADDR(gUiTextItalian09),
    STRING_ADDR(gUiTextItalian10),
    STRING_ADDR(gUiTextItalian11),
    STRING_ADDR(gUiTextItalian12),
    STRING_ADDR(gUiTextItalian13),
    STRING_ADDR(gUiTextItalian14),
    STRING_ADDR(gUiTextItalian15),
    STRING_ADDR(gUiTextItalian16),
    STRING_ADDR(gUiTextItalian17),
    STRING_ADDR(gUiTextItalian18),
    STRING_ADDR(gUiTextItalian19),
    STRING_ADDR(gUiTextItalian20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextItalian26),
    STRING_ADDR(gUiTextItalian27),
    STRING_ADDR(gUiTextItalian28),
    STRING_ADDR(gUiTextItalian29),
    STRING_ADDR(gUiTextItalian30),
    STRING_ADDR(gUiTextItalian31),
    STRING_ADDR(gUiTextItalian32),
    STRING_ADDR(gUiTextItalian33),
    STRING_ADDR(gUiTextItalian34),
    STRING_ADDR(gUiTextItalian35),
    STRING_ADDR(gUiTextItalian36),
    STRING_ADDR(gUiTextItalian37),
    STRING_ADDR(gUiTextItalian38),
    STRING_ADDR(gUiTextItalian39),
    STRING_ADDR(gUiTextItalian40),
    STRING_ADDR(gUiTextItalian41),
    STRING_ADDR(gUiTextEnglish42),
    STRING_ADDR(gUiTextItalian43),
    STRING_ADDR(gUiTextItalian44),
    STRING_ADDR(gUiTextItalian45),
    STRING_ADDR(gUiTextItalian46),
    STRING_ADDR(gUiTextItalian47),
    STRING_ADDR(gUiTextItalian48),
    STRING_ADDR(gUiTextItalian49),
    STRING_ADDR(gUiTextItalian50),
    STRING_ADDR(gUiTextItalian51),
    STRING_ADDR(gUiTextItalian52),
    STRING_ADDR(gUiTextItalian53),
    STRING_ADDR(gUiTextItalian54),
    STRING_ADDR(gUiTextItalian55),
    STRING_ADDR(gUiTextItalian56),
    STRING_ADDR(gUiTextItalian57),
    STRING_ADDR(gUiTextItalian58),
    STRING_ADDR(gUiTextItalian59),
    STRING_ADDR(gUiTextItalian60),
    STRING_ADDR(gUiTextItalian61),
    STRING_ADDR(gUiTextItalian62),
    STRING_ADDR(gUiTextItalian63),
    STRING_ADDR(gUiTextItalian64),
    STRING_ADDR(gUiTextItalian65),
    STRING_ADDR(gUiTextItalian66),
    STRING_ADDR(gUiTextItalian67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};

extern const u8 gUiTextDutch02[] = "schipbreuk";
extern const u8 gUiTextDutch03[] = "de tempel";
extern const u8 gUiTextDutch04[] = "ijzige grot";
extern const u8 gUiTextDutch05[] = "slijmerig";
extern const u8 gUiTextDutch06[] = "in de sneeuw";
extern const u8 gUiTextDutch08[] = "even hangen";
extern const u8 gUiTextDutch09[] = "agressieve haai";
extern const u8 gUiTextDutch10[] = "ru\357ne";
extern const u8 gUiTextDutch11[] = "ingesneeuwd";
extern const u8 gUiTextDutch12[] = "in de ruimte";
extern const u8 gUiTextDutch13[] = "verzonken stad";
extern const u8 gUiTextDutch14[] = "door het gat";
extern const u8 gUiTextDutch15[] = "zeppelinfeest";
extern const u8 gUiTextDutch16[] = "op naar de finish";
extern const u8 gUiTextDutch17[] = "luchttoevoer";
extern const u8 gUiTextDutch19[] = "drup, drup, drup";
extern const u8 gUiTextDutch26[] = "nieuw spel";
extern const u8 gUiTextDutch27[] = "spel laden";
extern const u8 gUiTextDutch28[] = "link-spel laden";
extern const u8 gUiTextDutch29[] = "spel verwijderen";
extern const u8 gUiTextDutch30[] = "spel opslaan";
extern const u8 gUiTextDutch31[] = "stoppen";
extern const u8 gUiTextDutch32[] = "afsluiten";
extern const u8 gUiTextDutch33[] = "fout bij gegevensopslag";
extern const u8 gUiTextDutch34[] = "opslaan ok";
extern const u8 gUiTextDutch35[] = "annuleren";
extern const u8 gUiTextDutch36[] = "voltooid";
extern const u8 gUiTextDutch37[] = "vrij";
extern const u8 gUiTextDutch38[] = "verwijderen?";
extern const u8 gUiTextDutch39[] = "overschrijven?";
extern const u8 gUiTextDutch40[] = "doorgaan?";
extern const u8 gUiTextDutch42[] = "nee";
extern const u8 gUiTextDutch43[] = "gegevensoverdracht";
extern const u8 gUiTextDutch44[] = "fout gegevensoverdracht";
extern const u8 gUiTextDutch45[] = "b-knop: afbreken";
extern const u8 gUiTextDutch46[] = "start: doorgaan";
extern const u8 gUiTextDutch47[] = "laden-opslaan: start";
extern const u8 gUiTextDutch48[] = "muziek";
extern const u8 gUiTextDutch49[] = "geluid";
extern const u8 gUiTextDutch50[] = "hervatten";
extern const u8 gUiTextDutch51[] = "zapkamer";
extern const u8 gUiTextDutch52[] = "race opnieuw";
extern const u8 gUiTextDutch53[] = "krachten";
extern const u8 gUiTextDutch54[] = "kristallen";
extern const u8 gUiTextDutch55[] = "edelstenen";
extern const u8 gUiTextDutch56[] = "relikwieen";
extern const u8 gUiTextDutch57[] = "tegen de tijd";
extern const u8 gUiTextDutch58[] = "geen";
extern const u8 gUiTextDutch64[] = "druk in de lucht op R: super body slam maakt dozen kapot en vijanden af";
extern const u8 gUiTextDutch65[] = "druk in de lucht op A voor een hogere sprong";
extern const u8 gUiTextDutch66[] = "druk herhaaldelijk op B voor langer draaien en tijdelijk zweven";
extern const u8 gUiTextDutch67[] = "houd L ingedrukt voor snelheidsinjectie";

const u8 *const gUiTextDutch[70] = {
    STRING_ADDR(gUiTextEnglish00),
    STRING_ADDR(gUiTextEnglish01),
    STRING_ADDR(gUiTextDutch02),
    STRING_ADDR(gUiTextDutch03),
    STRING_ADDR(gUiTextDutch04),
    STRING_ADDR(gUiTextDutch05),
    STRING_ADDR(gUiTextDutch06),
    STRING_ADDR(gUiTextEnglish07),
    STRING_ADDR(gUiTextDutch08),
    STRING_ADDR(gUiTextDutch09),
    STRING_ADDR(gUiTextDutch10),
    STRING_ADDR(gUiTextDutch11),
    STRING_ADDR(gUiTextDutch12),
    STRING_ADDR(gUiTextDutch13),
    STRING_ADDR(gUiTextDutch14),
    STRING_ADDR(gUiTextDutch15),
    STRING_ADDR(gUiTextDutch16),
    STRING_ADDR(gUiTextDutch17),
    STRING_ADDR(gUiTextEnglish18),
    STRING_ADDR(gUiTextDutch19),
    STRING_ADDR(gUiTextEnglish20),
    STRING_ADDR(gUiTextEnglish21),
    STRING_ADDR(gUiTextEnglish22),
    STRING_ADDR(gUiTextEnglish23),
    STRING_ADDR(gUiTextEnglish24),
    STRING_ADDR(gUiTextEnglish25),
    STRING_ADDR(gUiTextDutch26),
    STRING_ADDR(gUiTextDutch27),
    STRING_ADDR(gUiTextDutch28),
    STRING_ADDR(gUiTextDutch29),
    STRING_ADDR(gUiTextDutch30),
    STRING_ADDR(gUiTextDutch31),
    STRING_ADDR(gUiTextDutch32),
    STRING_ADDR(gUiTextDutch33),
    STRING_ADDR(gUiTextDutch34),
    STRING_ADDR(gUiTextDutch35),
    STRING_ADDR(gUiTextDutch36),
    STRING_ADDR(gUiTextDutch37),
    STRING_ADDR(gUiTextDutch38),
    STRING_ADDR(gUiTextDutch39),
    STRING_ADDR(gUiTextDutch40),
    STRING_ADDR(gUiTextGerman41),
    STRING_ADDR(gUiTextDutch42),
    STRING_ADDR(gUiTextDutch43),
    STRING_ADDR(gUiTextDutch44),
    STRING_ADDR(gUiTextDutch45),
    STRING_ADDR(gUiTextDutch46),
    STRING_ADDR(gUiTextDutch47),
    STRING_ADDR(gUiTextDutch48),
    STRING_ADDR(gUiTextDutch49),
    STRING_ADDR(gUiTextDutch50),
    STRING_ADDR(gUiTextDutch51),
    STRING_ADDR(gUiTextDutch52),
    STRING_ADDR(gUiTextDutch53),
    STRING_ADDR(gUiTextDutch54),
    STRING_ADDR(gUiTextDutch55),
    STRING_ADDR(gUiTextDutch56),
    STRING_ADDR(gUiTextDutch57),
    STRING_ADDR(gUiTextDutch58),
    STRING_ADDR(gUiTextEnglish59),
    STRING_ADDR(gUiTextEnglish60),
    STRING_ADDR(gUiTextEnglish61),
    STRING_ADDR(gUiTextEnglish62),
    STRING_ADDR(gUiTextEnglish63),
    STRING_ADDR(gUiTextDutch64),
    STRING_ADDR(gUiTextDutch65),
    STRING_ADDR(gUiTextDutch66),
    STRING_ADDR(gUiTextDutch67),
    STRING_ADDR(gUiTextEnglish68),
    STRING_ADDR(gUiTextEnglish69),
};


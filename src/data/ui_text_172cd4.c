#include "core.h"
#include "system.h"

/*
 * ROM 0x08172CD4-0x08174BE0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The game's own text (menus, level names, popups, credits labels) in
 * the six languages: per language an array of 70 strings, which
 * GetUiText (`text id -> string`) reads through the IWRAM table
 * gUiTextTables (src/iwram/iwram_data.c), indexed by the language
 * setting gLanguage. Each language's new strings come before its
 * array; strings that are the same in several languages (the level names,
 * mostly) are stored once, under the first language that uses them.
 * Latin-1, all lower case. */
const u8 gUiTextEnglish00[] = "level";
const u8 gUiTextEnglish01[] = "jungle jam";
const u8 gUiTextEnglish02[] = "shipwrecked";
const u8 gUiTextEnglish03[] = "temple of boom";
const u8 gUiTextEnglish04[] = "frostbite cavern";
const u8 gUiTextEnglish05[] = "just in slime";
const u8 gUiTextEnglish06[] = "snow crash";
const u8 gUiTextEnglish07[] = "rocket racket";
const u8 gUiTextEnglish08[] = "just hangin'";
const u8 gUiTextEnglish09[] = "shark attack";
const u8 gUiTextEnglish10[] = "ruined";
const u8 gUiTextEnglish11[] = "snow job";
const u8 gUiTextEnglish12[] = "ace of space";
const u8 gUiTextEnglish13[] = "sunken city";
const u8 gUiTextEnglish14[] = "down the hole";
const u8 gUiTextEnglish15[] = "blimp bonanza";
const u8 gUiTextEnglish16[] = "star to finish";
const u8 gUiTextEnglish17[] = "air supply";
const u8 gUiTextEnglish18[] = "no-fly zone";
const u8 gUiTextEnglish19[] = "drip, drip, drip";
const u8 gUiTextEnglish20[] = "final countdown";
const u8 gUiTextEnglish21[] = "tiny";
const u8 gUiTextEnglish22[] = "dingodile";
const u8 gUiTextEnglish23[] = "n. gin";
const u8 gUiTextEnglish24[] = "neo cortex";
const u8 gUiTextEnglish25[] = "mega-mix";
const u8 gUiTextEnglish26[] = "new game";
const u8 gUiTextEnglish27[] = "load game";
const u8 gUiTextEnglish28[] = "load link game";
const u8 gUiTextEnglish29[] = "delete game";
const u8 gUiTextEnglish30[] = "save game";
const u8 gUiTextEnglish31[] = "quit";
const u8 gUiTextEnglish32[] = "exit";
const u8 gUiTextEnglish33[] = "error saving data";
const u8 gUiTextEnglish34[] = "save ok";
const u8 gUiTextEnglish35[] = "cancel";
const u8 gUiTextEnglish36[] = "complete";
const u8 gUiTextEnglish37[] = "empty";
const u8 gUiTextEnglish38[] = "delete?";
const u8 gUiTextEnglish39[] = "overwrite?";
const u8 gUiTextEnglish40[] = "continue?";
const u8 gUiTextEnglish41[] = "yes";
const u8 gUiTextEnglish42[] = "no";
const u8 gUiTextEnglish43[] = "transferring data";
const u8 gUiTextEnglish44[] = "error transferring data";
const u8 gUiTextEnglish45[] = "b button to abort";
const u8 gUiTextEnglish46[] = "push start to continue";
const u8 gUiTextEnglish47[] = "start:  load-save";
const u8 gUiTextEnglish48[] = "music";
const u8 gUiTextEnglish49[] = "sound";
const u8 gUiTextEnglish50[] = "resume";
const u8 gUiTextEnglish51[] = "warp room";
const u8 gUiTextEnglish52[] = "restart trial";
const u8 gUiTextEnglish53[] = "powers";
const u8 gUiTextEnglish54[] = "crystals";
const u8 gUiTextEnglish55[] = "gems";
const u8 gUiTextEnglish56[] = "relics";
const u8 gUiTextEnglish57[] = "time trial";
const u8 gUiTextEnglish58[] = "none";
const u8 gUiTextEnglish59[] = "credits";
const u8 gUiTextEnglish60[] = "super body slam";
const u8 gUiTextEnglish61[] = "double jump";
const u8 gUiTextEnglish62[] = "tornado spin";
const u8 gUiTextEnglish63[] = "turbo run";
const u8 gUiTextEnglish64[] = "push R in mid-air to super body slam.  this destroys crates and nearby enemies.";
const u8 gUiTextEnglish65[] = "push A in mid-air to jump higher.";
const u8 gUiTextEnglish66[] = "push B repeatedly to spin longer and temporarily float.";
const u8 gUiTextEnglish67[] = "hold L for a burst of speed.";
const u8 gUiTextEnglish68[] = "bonus";
const u8 gUiTextEnglish69[] = "pause";

const u8 *const gUiTextEnglish[70] = {
    gUiTextEnglish00,
    gUiTextEnglish01,
    gUiTextEnglish02,
    gUiTextEnglish03,
    gUiTextEnglish04,
    gUiTextEnglish05,
    gUiTextEnglish06,
    gUiTextEnglish07,
    gUiTextEnglish08,
    gUiTextEnglish09,
    gUiTextEnglish10,
    gUiTextEnglish11,
    gUiTextEnglish12,
    gUiTextEnglish13,
    gUiTextEnglish14,
    gUiTextEnglish15,
    gUiTextEnglish16,
    gUiTextEnglish17,
    gUiTextEnglish18,
    gUiTextEnglish19,
    gUiTextEnglish20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextEnglish26,
    gUiTextEnglish27,
    gUiTextEnglish28,
    gUiTextEnglish29,
    gUiTextEnglish30,
    gUiTextEnglish31,
    gUiTextEnglish32,
    gUiTextEnglish33,
    gUiTextEnglish34,
    gUiTextEnglish35,
    gUiTextEnglish36,
    gUiTextEnglish37,
    gUiTextEnglish38,
    gUiTextEnglish39,
    gUiTextEnglish40,
    gUiTextEnglish41,
    gUiTextEnglish42,
    gUiTextEnglish43,
    gUiTextEnglish44,
    gUiTextEnglish45,
    gUiTextEnglish46,
    gUiTextEnglish47,
    gUiTextEnglish48,
    gUiTextEnglish49,
    gUiTextEnglish50,
    gUiTextEnglish51,
    gUiTextEnglish52,
    gUiTextEnglish53,
    gUiTextEnglish54,
    gUiTextEnglish55,
    gUiTextEnglish56,
    gUiTextEnglish57,
    gUiTextEnglish58,
    gUiTextEnglish59,
    gUiTextEnglish60,
    gUiTextEnglish61,
    gUiTextEnglish62,
    gUiTextEnglish63,
    gUiTextEnglish64,
    gUiTextEnglish65,
    gUiTextEnglish66,
    gUiTextEnglish67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};

const u8 gUiTextFrench00[] = "niveau";
const u8 gUiTextFrench01[] = "jungle en d\351lire";
const u8 gUiTextFrench02[] = "crash\351 coul\351";
const u8 gUiTextFrench03[] = "le temple maudit";
const u8 gUiTextFrench04[] = "caverne des glaces";
const u8 gUiTextFrench05[] = "univers gluant";
const u8 gUiTextFrench06[] = "crash-neige";
const u8 gUiTextFrench07[] = "crashta rocket";
const u8 gUiTextFrench08[] = "accroche-toi";
const u8 gUiTextFrench09[] = "gare au requin!";
const u8 gUiTextFrench10[] = "la cit\351 des ruines";
const u8 gUiTextFrench11[] = "tombe la neige";
const u8 gUiTextFrench12[] = "l'as de l'espace";
const u8 gUiTextFrench13[] = "la cit\351 engloutie";
const u8 gUiTextFrench14[] = "sous-terre";
const u8 gUiTextFrench15[] = "zeppelinland";
const u8 gUiTextFrench16[] = "\351toile crashante";
const u8 gUiTextFrench17[] = "aero-crash";
const u8 gUiTextFrench18[] = "no-man's-air";
const u8 gUiTextFrench19[] = "goutte \340 goutte";
const u8 gUiTextFrench20[] = "compte \340 rebours";
const u8 gUiTextFrench26[] = "nouvelle partie";
const u8 gUiTextFrench27[] = "charger partie";
const u8 gUiTextFrench28[] = "charg. partie link";
const u8 gUiTextFrench29[] = "supprimer partie";
const u8 gUiTextFrench30[] = "sauvegarder partie";
const u8 gUiTextFrench31[] = "quitter";
const u8 gUiTextFrench33[] = "erreur sauvegarde donn\351es";
const u8 gUiTextFrench34[] = "sauvegarde ok";
const u8 gUiTextFrench35[] = "annuler";
const u8 gUiTextFrench36[] = "termin\351";
const u8 gUiTextFrench37[] = "vide";
const u8 gUiTextFrench38[] = "supprimer?";
const u8 gUiTextFrench39[] = "ecraser?";
const u8 gUiTextFrench40[] = "continuer?";
const u8 gUiTextFrench41[] = "oui";
const u8 gUiTextFrench42[] = "non";
const u8 gUiTextFrench43[] = "transfert des donn\351es";
const u8 gUiTextFrench44[] = "erreur transfert donn\351es";
const u8 gUiTextFrench45[] = "appuyer sur B pour abandonner";
const u8 gUiTextFrench46[] = "appuyer sur start pour continuer";
const u8 gUiTextFrench47[] = "start pour charg-sauveg";
const u8 gUiTextFrench48[] = "musique";
const u8 gUiTextFrench49[] = "son";
const u8 gUiTextFrench50[] = "reprendre";
const u8 gUiTextFrench51[] = "s. spatio-t.";
const u8 gUiTextFrench52[] = "recom. clm";
const u8 gUiTextFrench53[] = "pouvoirs";
const u8 gUiTextFrench54[] = "cristaux";
const u8 gUiTextFrench55[] = "gemmes";
const u8 gUiTextFrench56[] = "reliques";
const u8 gUiTextFrench57[] = "contre la m.";
const u8 gUiTextFrench58[] = "aucun";
const u8 gUiTextFrench59[] = "cr\351dits";
const u8 gUiTextFrench60[] = "super \351crasement";
const u8 gUiTextFrench61[] = "double saut";
const u8 gUiTextFrench62[] = "tornade";
const u8 gUiTextFrench63[] = "turbo";
const u8 gUiTextFrench64[] = "super \351crasement: en l'air, appuyer sur R pour tout d\351truire";
const u8 gUiTextFrench65[] = "sauter plus haut: en l'air, appuyer sur A";
const u8 gUiTextFrench66[] = "tourbillonner longtemps + flotter: appuyer plusieurs fois sur B";
const u8 gUiTextFrench67[] = "acc\351l\351ration: maintenir L enfonc\351";

const u8 *const gUiTextFrench[70] = {
    gUiTextFrench00,
    gUiTextFrench01,
    gUiTextFrench02,
    gUiTextFrench03,
    gUiTextFrench04,
    gUiTextFrench05,
    gUiTextFrench06,
    gUiTextFrench07,
    gUiTextFrench08,
    gUiTextFrench09,
    gUiTextFrench10,
    gUiTextFrench11,
    gUiTextFrench12,
    gUiTextFrench13,
    gUiTextFrench14,
    gUiTextFrench15,
    gUiTextFrench16,
    gUiTextFrench17,
    gUiTextFrench18,
    gUiTextFrench19,
    gUiTextFrench20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextFrench26,
    gUiTextFrench27,
    gUiTextFrench28,
    gUiTextFrench29,
    gUiTextFrench30,
    gUiTextFrench31,
    gUiTextFrench31,
    gUiTextFrench33,
    gUiTextFrench34,
    gUiTextFrench35,
    gUiTextFrench36,
    gUiTextFrench37,
    gUiTextFrench38,
    gUiTextFrench39,
    gUiTextFrench40,
    gUiTextFrench41,
    gUiTextFrench42,
    gUiTextFrench43,
    gUiTextFrench44,
    gUiTextFrench45,
    gUiTextFrench46,
    gUiTextFrench47,
    gUiTextFrench48,
    gUiTextFrench49,
    gUiTextFrench50,
    gUiTextFrench51,
    gUiTextFrench52,
    gUiTextFrench53,
    gUiTextFrench54,
    gUiTextFrench55,
    gUiTextFrench56,
    gUiTextFrench57,
    gUiTextFrench58,
    gUiTextFrench59,
    gUiTextFrench60,
    gUiTextFrench61,
    gUiTextFrench62,
    gUiTextFrench63,
    gUiTextFrench64,
    gUiTextFrench65,
    gUiTextFrench66,
    gUiTextFrench67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};

const u8 gUiTextGerman01[] = "dschungelkoller";
const u8 gUiTextGerman02[] = "schiffbruch";
const u8 gUiTextGerman03[] = "schauertempel";
const u8 gUiTextGerman04[] = "frosth\366hle";
const u8 gUiTextGerman05[] = "alles in glibber";
const u8 gUiTextGerman06[] = "schnee-crash";
const u8 gUiTextGerman07[] = "raketen-radau";
const u8 gUiTextGerman08[] = "gut abgehangen";
const u8 gUiTextGerman09[] = "am hai vorbei";
const u8 gUiTextGerman10[] = "ruin\366s";
const u8 gUiTextGerman11[] = "schneegeschmiere";
const u8 gUiTextGerman12[] = "traum im raum";
const u8 gUiTextGerman13[] = "versunkene stadt";
const u8 gUiTextGerman14[] = "den ausguss hinab";
const u8 gUiTextGerman15[] = "blimp-gefecht";
const u8 gUiTextGerman16[] = "man sieht sterne";
const u8 gUiTextGerman17[] = "sch\366n luft holen";
const u8 gUiTextGerman18[] = "fliegen verboten";
const u8 gUiTextGerman19[] = "feucht und muffig";
const u8 gUiTextGerman20[] = "letzter countdown";
const u8 gUiTextGerman26[] = "neues spiel";
const u8 gUiTextGerman27[] = "spiel laden";
const u8 gUiTextGerman28[] = "link-spiel laden";
const u8 gUiTextGerman29[] = "spiel l\366schen";
const u8 gUiTextGerman30[] = "spiel speichern";
const u8 gUiTextGerman31[] = "beenden";
const u8 gUiTextGerman33[] = "datenspeicherungs-fehler";
const u8 gUiTextGerman34[] = "speicherung ok";
const u8 gUiTextGerman35[] = "abbrechen";
const u8 gUiTextGerman36[] = "fertig";
const u8 gUiTextGerman37[] = "leer";
const u8 gUiTextGerman38[] = "l\366schen?";
const u8 gUiTextGerman39[] = "\374berschreiben?";
const u8 gUiTextGerman40[] = "fortfahren?";
const u8 gUiTextGerman41[] = "ja";
const u8 gUiTextGerman42[] = "nein";
const u8 gUiTextGerman43[] = "daten werden \374bertragen";
const u8 gUiTextGerman44[] = "daten\374bertrag.-fehler";
const u8 gUiTextGerman45[] = "b-knopf: abbrechen";
const u8 gUiTextGerman46[] = "'start': fortfahren";
const u8 gUiTextGerman47[] = "'start': laden-speichern";
const u8 gUiTextGerman48[] = "musik";
const u8 gUiTextGerman50[] = "fortsetzen";
const u8 gUiTextGerman51[] = "zeitspr.-raum";
const u8 gUiTextGerman52[] = "zeitmodus neu";
const u8 gUiTextGerman53[] = "kr\344fte";
const u8 gUiTextGerman54[] = "kristalle";
const u8 gUiTextGerman55[] = "edelsteine";
const u8 gUiTextGerman56[] = "relikte";
const u8 gUiTextGerman57[] = "zeitmodus";
const u8 gUiTextGerman58[] = "keine";
const u8 gUiTextGerman59[] = "mitwirkende";
const u8 gUiTextGerman60[] = "super-bodenwurf";
const u8 gUiTextGerman61[] = "doppelsprung";
const u8 gUiTextGerman62[] = "tornado-drehung";
const u8 gUiTextGerman63[] = "turbo-lauf";
const u8 gUiTextGerman64[] = "super-bodenwurf: in der luft R dr\374cken. zerst\366rt kisten und gegner";
const u8 gUiTextGerman65[] = "h\366her springen: in der luft A dr\374cken";
const u8 gUiTextGerman66[] = "schneller drehen und zeitweilig gleiten: B wiederholt dr\374cken";
const u8 gUiTextGerman67[] = "geschwindigkeitsschub: L gedr\374ckt halten";

const u8 *const gUiTextGerman[70] = {
    gUiTextEnglish00,
    gUiTextGerman01,
    gUiTextGerman02,
    gUiTextGerman03,
    gUiTextGerman04,
    gUiTextGerman05,
    gUiTextGerman06,
    gUiTextGerman07,
    gUiTextGerman08,
    gUiTextGerman09,
    gUiTextGerman10,
    gUiTextGerman11,
    gUiTextGerman12,
    gUiTextGerman13,
    gUiTextGerman14,
    gUiTextGerman15,
    gUiTextGerman16,
    gUiTextGerman17,
    gUiTextGerman18,
    gUiTextGerman19,
    gUiTextGerman20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextGerman26,
    gUiTextGerman27,
    gUiTextGerman28,
    gUiTextGerman29,
    gUiTextGerman30,
    gUiTextGerman31,
    gUiTextGerman31,
    gUiTextGerman33,
    gUiTextGerman34,
    gUiTextGerman35,
    gUiTextGerman36,
    gUiTextGerman37,
    gUiTextGerman38,
    gUiTextGerman39,
    gUiTextGerman40,
    gUiTextGerman41,
    gUiTextGerman42,
    gUiTextGerman43,
    gUiTextGerman44,
    gUiTextGerman45,
    gUiTextGerman46,
    gUiTextGerman47,
    gUiTextGerman48,
    gUiTextEnglish49,
    gUiTextGerman50,
    gUiTextGerman51,
    gUiTextGerman52,
    gUiTextGerman53,
    gUiTextGerman54,
    gUiTextGerman55,
    gUiTextGerman56,
    gUiTextGerman57,
    gUiTextGerman58,
    gUiTextGerman59,
    gUiTextGerman60,
    gUiTextGerman61,
    gUiTextGerman62,
    gUiTextGerman63,
    gUiTextGerman64,
    gUiTextGerman65,
    gUiTextGerman66,
    gUiTextGerman67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};

const u8 gUiTextSpanish00[] = "nivel";
const u8 gUiTextSpanish01[] = "liado entre lianas";
const u8 gUiTextSpanish02[] = "n\341ufragos";
const u8 gUiTextSpanish03[] = "templo explosivo";
const u8 gUiTextSpanish04[] = "la caverna helada";
const u8 gUiTextSpanish05[] = "cieno que me hundo";
const u8 gUiTextSpanish06[] = "la nieve y yo";
const u8 gUiTextSpanish07[] = "como un cohete";
const u8 gUiTextSpanish08[] = "aqu\355, colgado";
const u8 gUiTextSpanish09[] = "tibur\363n a la carga";
const u8 gUiTextSpanish10[] = "en la ruina";
const u8 gUiTextSpanish11[] = "aventura nevada";
const u8 gUiTextSpanish12[] = "dadme espacio";
const u8 gUiTextSpanish13[] = "ciudad sumergida";
const u8 gUiTextSpanish14[] = "ca\355da libre";
const u8 gUiTextSpanish15[] = "bonanza en el aire";
const u8 gUiTextSpanish16[] = "estrellado";
const u8 gUiTextSpanish17[] = "a falta de aire";
const u8 gUiTextSpanish18[] = "zona de exclusi\363n";
const u8 gUiTextSpanish19[] = "gota a gota";
const u8 gUiTextSpanish20[] = "tres, dos, uno...";
const u8 gUiTextSpanish26[] = "partida nueva";
const u8 gUiTextSpanish27[] = "cargar partida";
const u8 gUiTextSpanish28[] = "c. part. conexi\363n ";
const u8 gUiTextSpanish29[] = "borrar partida";
const u8 gUiTextSpanish30[] = "grabar partida";
const u8 gUiTextSpanish31[] = "abandonar";
const u8 gUiTextSpanish32[] = "salir";
const u8 gUiTextSpanish33[] = "error al grabar datos";
const u8 gUiTextSpanish34[] = "grabar ok";
const u8 gUiTextSpanish35[] = "cancelar";
const u8 gUiTextSpanish36[] = "complet.";
const u8 gUiTextSpanish37[] = "vac\355a";
const u8 gUiTextSpanish38[] = "\277borrar?";
const u8 gUiTextSpanish39[] = "\277sobrescribir?";
const u8 gUiTextSpanish40[] = "\277continuar?";
const u8 gUiTextSpanish41[] = "s\355";
const u8 gUiTextSpanish43[] = "transfiriendo datos";
const u8 gUiTextSpanish44[] = "error de transferencia";
const u8 gUiTextSpanish45[] = "bot\363n b para detener";
const u8 gUiTextSpanish46[] = "pulsa start para continuar";
const u8 gUiTextSpanish47[] = "cargar-grabar -> start";
const u8 gUiTextSpanish48[] = "m\372sica";
const u8 gUiTextSpanish49[] = "sonido";
const u8 gUiTextSpanish50[] = "reanudar";
const u8 gUiTextSpanish51[] = "s. del tiempo";
const u8 gUiTextSpanish52[] = "contrarreloj";
const u8 gUiTextSpanish53[] = "poderes";
const u8 gUiTextSpanish54[] = "cristales";
const u8 gUiTextSpanish55[] = "gemas";
const u8 gUiTextSpanish56[] = "reliquias";
const u8 gUiTextSpanish58[] = "agotados";
const u8 gUiTextSpanish59[] = "ficha t\351cnica";
const u8 gUiTextSpanish60[] = "golpetazo corporal";
const u8 gUiTextSpanish61[] = "doble salto";
const u8 gUiTextSpanish62[] = "giro de tornado";
const u8 gUiTextSpanish63[] = "turbocarrera";
const u8 gUiTextSpanish64[] = "pulsa R en el aire para el golpetazo corporal que destruye cajas y enemigos";
const u8 gUiTextSpanish65[] = "pulsa A en el aire para saltar m\341s alto";
const u8 gUiTextSpanish66[] = "pulsa varias veces B para girar m\341s y flotar temporalmente";
const u8 gUiTextSpanish67[] = "mant\351n pulsado L para lograr un impulso turbo";

const u8 *const gUiTextSpanish[70] = {
    gUiTextSpanish00,
    gUiTextSpanish01,
    gUiTextSpanish02,
    gUiTextSpanish03,
    gUiTextSpanish04,
    gUiTextSpanish05,
    gUiTextSpanish06,
    gUiTextSpanish07,
    gUiTextSpanish08,
    gUiTextSpanish09,
    gUiTextSpanish10,
    gUiTextSpanish11,
    gUiTextSpanish12,
    gUiTextSpanish13,
    gUiTextSpanish14,
    gUiTextSpanish15,
    gUiTextSpanish16,
    gUiTextSpanish17,
    gUiTextSpanish18,
    gUiTextSpanish19,
    gUiTextSpanish20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextSpanish26,
    gUiTextSpanish27,
    gUiTextSpanish28,
    gUiTextSpanish29,
    gUiTextSpanish30,
    gUiTextSpanish31,
    gUiTextSpanish32,
    gUiTextSpanish33,
    gUiTextSpanish34,
    gUiTextSpanish35,
    gUiTextSpanish36,
    gUiTextSpanish37,
    gUiTextSpanish38,
    gUiTextSpanish39,
    gUiTextSpanish40,
    gUiTextSpanish41,
    gUiTextEnglish42,
    gUiTextSpanish43,
    gUiTextSpanish44,
    gUiTextSpanish45,
    gUiTextSpanish46,
    gUiTextSpanish47,
    gUiTextSpanish48,
    gUiTextSpanish49,
    gUiTextSpanish50,
    gUiTextSpanish51,
    gUiTextSpanish52,
    gUiTextSpanish53,
    gUiTextSpanish54,
    gUiTextSpanish55,
    gUiTextSpanish56,
    gUiTextSpanish52,
    gUiTextSpanish58,
    gUiTextSpanish59,
    gUiTextSpanish60,
    gUiTextSpanish61,
    gUiTextSpanish62,
    gUiTextSpanish63,
    gUiTextSpanish64,
    gUiTextSpanish65,
    gUiTextSpanish66,
    gUiTextSpanish67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};

const u8 gUiTextItalian00[] = "livello";
const u8 gUiTextItalian01[] = "caos nella giungla";
const u8 gUiTextItalian02[] = "naufragio";
const u8 gUiTextItalian03[] = "tempio di boom!";
const u8 gUiTextItalian04[] = "antro di ghiaccio";
const u8 gUiTextItalian05[] = "fango mortale";
const u8 gUiTextItalian06[] = "valanga arancione";
const u8 gUiTextItalian07[] = "la gang del razzo";
const u8 gUiTextItalian08[] = "col fiato sospeso";
const u8 gUiTextItalian09[] = "squali affamati";
const u8 gUiTextItalian10[] = "tra le rovine";
const u8 gUiTextItalian11[] = "scende la neve";
const u8 gUiTextItalian12[] = "spaziale!";
const u8 gUiTextItalian13[] = "citt\340 sommersa";
const u8 gUiTextItalian14[] = "sotto terra";
const u8 gUiTextItalian15[] = "al dirigibile!";
const u8 gUiTextItalian16[] = "spazi siderali";
const u8 gUiTextItalian17[] = "riserva d'aria";
const u8 gUiTextItalian18[] = "vietato volare";
const u8 gUiTextItalian19[] = "plic, plic, plic";
const u8 gUiTextItalian20[] = "meno 10... 9";
const u8 gUiTextItalian26[] = "nuova partita";
const u8 gUiTextItalian27[] = "carica partita";
const u8 gUiTextItalian28[] = "carica via link";
const u8 gUiTextItalian29[] = "cancella partita";
const u8 gUiTextItalian30[] = "salva partita";
const u8 gUiTextItalian31[] = "abbandona";
const u8 gUiTextItalian32[] = "esci";
const u8 gUiTextItalian33[] = "errore nel salvataggio";
const u8 gUiTextItalian34[] = "salvataggio ok";
const u8 gUiTextItalian35[] = "annulla";
const u8 gUiTextItalian36[] = "completo";
const u8 gUiTextItalian37[] = "vuoto";
const u8 gUiTextItalian38[] = "cancellare?";
const u8 gUiTextItalian39[] = "sovrascrivere?";
const u8 gUiTextItalian40[] = "continuare?";
const u8 gUiTextItalian41[] = "s\354";
const u8 gUiTextItalian43[] = "trasferimento in corso";
const u8 gUiTextItalian44[] = "errore nel trasferimento";
const u8 gUiTextItalian45[] = "pulsante B per interrompere";
const u8 gUiTextItalian46[] = "premi start per continuare";
const u8 gUiTextItalian47[] = "carica-salva con start";
const u8 gUiTextItalian48[] = "musica";
const u8 gUiTextItalian49[] = "suono";
const u8 gUiTextItalian50[] = "riprendi";
const u8 gUiTextItalian51[] = "teletraspo.";
const u8 gUiTextItalian52[] = "riprova gara";
const u8 gUiTextItalian53[] = "potenziatori";
const u8 gUiTextItalian54[] = "cristalli";
const u8 gUiTextItalian55[] = "gemme";
const u8 gUiTextItalian56[] = "reliquie";
const u8 gUiTextItalian57[] = "gara a tempo";
const u8 gUiTextItalian58[] = "nessuno";
const u8 gUiTextItalian59[] = "crediti";
const u8 gUiTextItalian60[] = "super panciata";
const u8 gUiTextItalian61[] = "salto doppio";
const u8 gUiTextItalian62[] = "giravolta tornado";
const u8 gUiTextItalian63[] = "corsa turbo";
const u8 gUiTextItalian64[] = "premi R in volo e la panciata distrugger\340 casse e nemici";
const u8 gUiTextItalian65[] = "premi A in volo per saltare pi\371 in alto";
const u8 gUiTextItalian66[] = "premi B ripetutamente per girare pi\371 a lungo e fluttuare";
const u8 gUiTextItalian67[] = "tieni premuto L per una rapida accelerata";

const u8 *const gUiTextItalian[70] = {
    gUiTextItalian00,
    gUiTextItalian01,
    gUiTextItalian02,
    gUiTextItalian03,
    gUiTextItalian04,
    gUiTextItalian05,
    gUiTextItalian06,
    gUiTextItalian07,
    gUiTextItalian08,
    gUiTextItalian09,
    gUiTextItalian10,
    gUiTextItalian11,
    gUiTextItalian12,
    gUiTextItalian13,
    gUiTextItalian14,
    gUiTextItalian15,
    gUiTextItalian16,
    gUiTextItalian17,
    gUiTextItalian18,
    gUiTextItalian19,
    gUiTextItalian20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextItalian26,
    gUiTextItalian27,
    gUiTextItalian28,
    gUiTextItalian29,
    gUiTextItalian30,
    gUiTextItalian31,
    gUiTextItalian32,
    gUiTextItalian33,
    gUiTextItalian34,
    gUiTextItalian35,
    gUiTextItalian36,
    gUiTextItalian37,
    gUiTextItalian38,
    gUiTextItalian39,
    gUiTextItalian40,
    gUiTextItalian41,
    gUiTextEnglish42,
    gUiTextItalian43,
    gUiTextItalian44,
    gUiTextItalian45,
    gUiTextItalian46,
    gUiTextItalian47,
    gUiTextItalian48,
    gUiTextItalian49,
    gUiTextItalian50,
    gUiTextItalian51,
    gUiTextItalian52,
    gUiTextItalian53,
    gUiTextItalian54,
    gUiTextItalian55,
    gUiTextItalian56,
    gUiTextItalian57,
    gUiTextItalian58,
    gUiTextItalian59,
    gUiTextItalian60,
    gUiTextItalian61,
    gUiTextItalian62,
    gUiTextItalian63,
    gUiTextItalian64,
    gUiTextItalian65,
    gUiTextItalian66,
    gUiTextItalian67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};

const u8 gUiTextDutch02[] = "schipbreuk";
const u8 gUiTextDutch03[] = "de tempel";
const u8 gUiTextDutch04[] = "ijzige grot";
const u8 gUiTextDutch05[] = "slijmerig";
const u8 gUiTextDutch06[] = "in de sneeuw";
const u8 gUiTextDutch08[] = "even hangen";
const u8 gUiTextDutch09[] = "agressieve haai";
const u8 gUiTextDutch10[] = "ru\357ne";
const u8 gUiTextDutch11[] = "ingesneeuwd";
const u8 gUiTextDutch12[] = "in de ruimte";
const u8 gUiTextDutch13[] = "verzonken stad";
const u8 gUiTextDutch14[] = "door het gat";
const u8 gUiTextDutch15[] = "zeppelinfeest";
const u8 gUiTextDutch16[] = "op naar de finish";
const u8 gUiTextDutch17[] = "luchttoevoer";
const u8 gUiTextDutch19[] = "drup, drup, drup";
const u8 gUiTextDutch26[] = "nieuw spel";
const u8 gUiTextDutch27[] = "spel laden";
const u8 gUiTextDutch28[] = "link-spel laden";
const u8 gUiTextDutch29[] = "spel verwijderen";
const u8 gUiTextDutch30[] = "spel opslaan";
const u8 gUiTextDutch31[] = "stoppen";
const u8 gUiTextDutch32[] = "afsluiten";
const u8 gUiTextDutch33[] = "fout bij gegevensopslag";
const u8 gUiTextDutch34[] = "opslaan ok";
const u8 gUiTextDutch35[] = "annuleren";
const u8 gUiTextDutch36[] = "voltooid";
const u8 gUiTextDutch37[] = "vrij";
const u8 gUiTextDutch38[] = "verwijderen?";
const u8 gUiTextDutch39[] = "overschrijven?";
const u8 gUiTextDutch40[] = "doorgaan?";
const u8 gUiTextDutch42[] = "nee";
const u8 gUiTextDutch43[] = "gegevensoverdracht";
const u8 gUiTextDutch44[] = "fout gegevensoverdracht";
const u8 gUiTextDutch45[] = "b-knop: afbreken";
const u8 gUiTextDutch46[] = "start: doorgaan";
const u8 gUiTextDutch47[] = "laden-opslaan: start";
const u8 gUiTextDutch48[] = "muziek";
const u8 gUiTextDutch49[] = "geluid";
const u8 gUiTextDutch50[] = "hervatten";
const u8 gUiTextDutch51[] = "zapkamer";
const u8 gUiTextDutch52[] = "race opnieuw";
const u8 gUiTextDutch53[] = "krachten";
const u8 gUiTextDutch54[] = "kristallen";
const u8 gUiTextDutch55[] = "edelstenen";
const u8 gUiTextDutch56[] = "relikwieen";
const u8 gUiTextDutch57[] = "tegen de tijd";
const u8 gUiTextDutch58[] = "geen";
const u8 gUiTextDutch64[] = "druk in de lucht op R: super body slam maakt dozen kapot en vijanden af";
const u8 gUiTextDutch65[] = "druk in de lucht op A voor een hogere sprong";
const u8 gUiTextDutch66[] = "druk herhaaldelijk op B voor langer draaien en tijdelijk zweven";
const u8 gUiTextDutch67[] = "houd L ingedrukt voor snelheidsinjectie";

const u8 *const gUiTextDutch[70] = {
    gUiTextEnglish00,
    gUiTextEnglish01,
    gUiTextDutch02,
    gUiTextDutch03,
    gUiTextDutch04,
    gUiTextDutch05,
    gUiTextDutch06,
    gUiTextEnglish07,
    gUiTextDutch08,
    gUiTextDutch09,
    gUiTextDutch10,
    gUiTextDutch11,
    gUiTextDutch12,
    gUiTextDutch13,
    gUiTextDutch14,
    gUiTextDutch15,
    gUiTextDutch16,
    gUiTextDutch17,
    gUiTextEnglish18,
    gUiTextDutch19,
    gUiTextEnglish20,
    gUiTextEnglish21,
    gUiTextEnglish22,
    gUiTextEnglish23,
    gUiTextEnglish24,
    gUiTextEnglish25,
    gUiTextDutch26,
    gUiTextDutch27,
    gUiTextDutch28,
    gUiTextDutch29,
    gUiTextDutch30,
    gUiTextDutch31,
    gUiTextDutch32,
    gUiTextDutch33,
    gUiTextDutch34,
    gUiTextDutch35,
    gUiTextDutch36,
    gUiTextDutch37,
    gUiTextDutch38,
    gUiTextDutch39,
    gUiTextDutch40,
    gUiTextGerman41,
    gUiTextDutch42,
    gUiTextDutch43,
    gUiTextDutch44,
    gUiTextDutch45,
    gUiTextDutch46,
    gUiTextDutch47,
    gUiTextDutch48,
    gUiTextDutch49,
    gUiTextDutch50,
    gUiTextDutch51,
    gUiTextDutch52,
    gUiTextDutch53,
    gUiTextDutch54,
    gUiTextDutch55,
    gUiTextDutch56,
    gUiTextDutch57,
    gUiTextDutch58,
    gUiTextEnglish59,
    gUiTextEnglish60,
    gUiTextEnglish61,
    gUiTextEnglish62,
    gUiTextEnglish63,
    gUiTextDutch64,
    gUiTextDutch65,
    gUiTextDutch66,
    gUiTextDutch67,
    gUiTextEnglish68,
    gUiTextEnglish69,
};


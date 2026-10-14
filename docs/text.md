# The game's text

The game's text is in `text/` as gettext files (#1070): the menus, the
level names and the other UI strings (`GetUiText`'s text ids), and the
cutscenes' pages, in the six languages of the language select (English,
French, German, Spanish, Italian, Dutch). The build generates the ROM's
three string tables from them (docs/data.md, "Text tables").

| File | What |
|---|---|
| `text/crashxs.pot` | the template: every string's key, its English text and the translator comments |
| `text/fr.po`, `de.po`, `es.po`, `it.po`, `nl.po` | a language's text, one entry per template entry |
| `tools/text.py` | the generator, the checks and the extractor |
| `text.mk` | the build rules and the `text-check`/`text-update` targets |

The gettext tools (msgfmt, msgcmp, msgmerge) are part of the build: the
devshell has them, and on Debian/Ubuntu they are the `gettext` package
(INSTALL.md).

## The entries

An entry is one string:

```po
#. Text id 27 (0x1B). Title screen menu item, and the save menu's first option
#. and the title of its screen (large font).
#. Widest shipped text: 92 px in the small font, 147 px in the large font; the
#. screen is 240 px wide.
msgctxt "ui.menu.load_game"
msgid "load game"
msgstr "charger partie"
```

- `msgctxt` is the key. The English text can be the same for two keys
  (and a language's text the same for several), so the key, not the
  English, says which string it is.
- `msgid` is the English text, the template's. English has no `.po`.
- `msgstr` is the language's text. Where the game keeps the English in
  a language (most level names, a few labels, two cutscene pages), it is
  the English text again: an entry is never left empty.
- The `#.` comments say where the game shows the string and, for the UI
  text, how wide the shipped texts are, in pixels of the font (the screen
  is 240 pixels wide). The game draws some UI strings in the large font
  too: the level names (the level select's title), the save menu's
  titles and the powers' names (the power dialog's title). The comments
  come from the template, and `make text-update` keeps them up to date in
  the `.po` files.

The keys:

- `ui.<group>.<name>`: a UI string. The `ui.*` entries are in text id
  order: the first is text id 0, and the id is GetUiText's argument
  (`GetUiText(0x1b)` is `ui.menu.load_game`). A new one goes at the end
  of them. The level names are `ui.level.<level>`, the level's constant
  in levels.json (`LEVEL_JUNGLE_JAM` is `ui.level.jungle_jam`), and a
  level's `name_text` is its entry's text id: tools/text.py stops if it
  isn't a `ui.level.*` entry, which catches an entry inserted in the
  middle. There are `UI_TEXT_COUNT` of them (levels.py's
  `constants/levels.h`), and the build stops if the template has
  another number.
- `cutscene<NN>.page<P>.<I>`: string `I` of page `P` of cutscene `NN`, the
  page that shows with slide `P` of the cutscene. A page's strings show
  one after the other. A page used twice (cutscene 10's page 2 is
  cutscene 9's page 1) has one key.

## The text

The game draws the text with its two HUD fonts, whose characters are
`gSmallFontChars` and `gLargeFontChars` (src/game/ui/data/hud_font_tables.cpp,
the glyphs are graphics/fonts/): the 26 lower case letters, the digits,
`- + ! : , ? . ' %` and `< >` (the small font only), the accented letters
`ä à á â ë è é ê ï ì í î ö ò ó ô ü ù ú û ñ ß ç ¿ ¡ ´ œ`, and `A`, `B`,
`L`, `R`, which draw the GBA's buttons (the small font only). So the text
is lower case. The files are UTF-8, and tools/text.py writes the ROM's
bytes in Latin-1 (`œ` is 0x9C, Windows-1252's code for it, where the
font has it). A character the font lacks stops the build: the game would
draw it as an `a`. A string the game also draws in the large font
(tools/text.py's `LARGE_FONT_KEYS` and the level names) must be in both
fonts.

The cutscene text is word-wrapped into a 228x30 pixel box (three lines
of the small font) under the picture, and a longer string goes on after
a key press, so it has no length limit. A UI string isn't wrapped (the
power dialog's help text is), so it has to fit where the game draws it:
the comment's widest shipped text is a safe width.

## Translating

Edit the language's `.po` file (a PO editor such as Poedit or Lokalize,
or any text editor; keep it UTF-8), then:

    make text-check      # the checks below
    make                 # builds the ROM with the new text

`make` rebuilds the tables from the `.po` files; only `make compare`
checks the ROM against the original, so changed text builds a working
ROM that `make compare` rejects.

`make text-check` (CI runs it) fails on:

- an untranslated or fuzzy entry (`msgfmt --check --statistics`): a
  fuzzy entry is one whose English changed since it was translated (see
  below). Check the translation, then delete its `#, fuzzy` line and its
  `#|` lines;
- a key the language lacks or one the template doesn't have (`msgcmp`
  against the template, both ways): run `make text-update`;
- a character the font lacks, a character Latin-1 lacks, a level name
  that isn't at its text id (tools/text.py's own checks, the same as the
  build's).

## Changing the template

To change the English text or add an entry, edit `text/crashxs.pot`
(the English, the key, the comments), then

    make text-update

which merges the template into every `.po` file (`msgmerge --update
--previous`): a new key comes in untranslated, a changed English text
marks its entry fuzzy, with the old English as `#| msgid`, and a removed
key becomes an obsolete `#~` entry. `make text-check` fails until each
language has been seen to. Where the game should keep the English, copy
it into `msgstr`.

A new UI string is a new `ui.*` entry after the last one, and its text
id is its position (for a level's name, set the level's `name_text` in
levels.json to it, see docs/levels.md). A new cutscene string needs a
page that the cutscene tables (`cutscenes.cpp`,
`cutscene_slides.cpp`) point at with that many strings.

## How the tables are built

- `msgfmt --check` compiles each `.po` into
  `build/crashbandicootxs/text/<lang>.mo` (an untranslated or fuzzy entry
  isn't in the `.mo`).
- `tools/text.py cpp` reads the template (the keys, their order and the
  English) and each language's `.mo` with Python's gettext module
  (`pgettext`, the key being the context), and writes
  `build/crashbandicootxs/text/<table>.inc`, which
  `src/*/data/<table>.cpp` includes. It stops on a key a `.mo` lacks.
- The tables store each distinct string once, at its first use, in the
  order docs/data.md, "Text tables", gives, as the ROM does.

## Extracting the text again

`tools/text.py extract [baserom.gba]` writes `text/` from the ROM's
tables, with the keys and the comments; this is how the files were made.
`tools/text.py extract --check` writes them into a temporary directory
and compares them with `text/` byte for byte: it passes as long as
`text/` is the ROM's text.

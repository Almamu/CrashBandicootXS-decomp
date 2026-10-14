# The game's text (docs/text.md, #1070): text/crashxs.pot is the template
# (the keys in msgctxt, the English as msgid), text/<lang>.po a language's
# text. msgfmt compiles each .po into a .mo, and tools/text.py writes the
# C++ of the three string tables from the template and the .mo files, which
# src/data/<table>.cpp #include:
# - ui_text_172cd4: the UI text of the six languages;
# - cutscene_text_16d4d8: the cutscenes' English text (the template only);
# - cutscene_text_16e8a4: the other five languages' cutscene text.
# The generator stops on a missing translation and on a character the
# game's font lacks (src/data/hud_fonts_174be0.cpp's character sets), and
# checks the level names' ids against levels.json.
TEXT_DIR := text
TEXT_POT := $(TEXT_DIR)/crashxs.pot
TEXT_LANGS := fr de es it nl
TEXT_POS := $(TEXT_LANGS:%=$(TEXT_DIR)/%.po)
TEXT_BUILDDIR := $(OBJ_DIR)/text
TEXT_MOS := $(TEXT_LANGS:%=$(TEXT_BUILDDIR)/%.mo)
TEXT_TABLES := ui_text_172cd4 cutscene_text_16d4d8 cutscene_text_16e8a4
TEXT_DEPS := $(TEXT_POT) tools/text.py src/data/hud_fonts_174be0.cpp

MSGFMT := msgfmt
MSGMERGE := msgmerge

$(TEXT_BUILDDIR)/%.mo: $(TEXT_DIR)/%.po
	@mkdir -p $(dir $@)
	$(MSGFMT) --check -o $@ $<

$(TEXT_BUILDDIR)/ui_text_172cd4.inc: $(TEXT_DEPS) $(TEXT_MOS) data/levels/levels.json
	python3 tools/text.py cpp ui_text_172cd4 $@ --mo $(TEXT_BUILDDIR)
$(TEXT_BUILDDIR)/cutscene_text_16d4d8.inc: $(TEXT_DEPS)
	python3 tools/text.py cpp cutscene_text_16d4d8 $@
$(TEXT_BUILDDIR)/cutscene_text_16e8a4.inc: $(TEXT_DEPS) $(TEXT_MOS)
	python3 tools/text.py cpp cutscene_text_16e8a4 $@ --mo $(TEXT_BUILDDIR)

$(foreach t,$(TEXT_TABLES),$(eval $(C_BUILDDIR)/data/$(t).o: $(TEXT_BUILDDIR)/$(t).inc))
$(foreach t,$(TEXT_TABLES),$(eval $(C_BUILDDIR)/data/$(t).o: CPPFLAGS += -iquote $(TEXT_BUILDDIR)))

# The translations' checks (CI runs them): `msgfmt --check --statistics`
# on each .po, failing on an untranslated or fuzzy entry; msgcmp against
# the template both ways, failing on a missing or extra key; and the
# generator's checks (the font, the encoding, the level names' ids).
.PHONY: text-check text-update
text-check:
	python3 tools/text.py check

# After a template change: merges text/crashxs.pot into every .po. A new
# key comes in untranslated and a changed English text marks its entry
# fuzzy (with the old text as `#| msgid`), so text-check fails until a
# translator has seen them (docs/text.md).
text-update:
	for po in $(TEXT_POS); do $(MSGMERGE) --quiet --update --backup=none --previous $$po $(TEXT_POT) || exit 1; done

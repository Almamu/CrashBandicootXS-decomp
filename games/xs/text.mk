# The game's text (docs/text.md, #1070): games/xs/text/crashxs.pot is the template
# (the keys in msgctxt, the English as msgid), games/xs/text/<lang>.po a language's
# text. msgfmt compiles each .po into a .mo, and tools/text.py writes the
# C++ of the three string tables from the template and the .mo files, which
# game/ui/data/<table>.cpp #include:
# - ui_text: the UI text of the six languages;
# - cutscene_text_english: the cutscenes' English text (the template only);
# - cutscene_text_translations: the other five languages' cutscene text;
# - credits_text: the credits (gCreditsText), from the template's credits.*
#   entries (the English: every language shows it, so each .po's copies
#   must be the English, which text-check checks) and games/xs/data/credits.json
#   (the order, the fonts and the logos).
# The generator stops on a missing translation and on a character the
# game's font lacks (engine/src/game/ui/data/hud_font_tables.cpp's character sets), and
# checks the level names' ids against levels.json.
TEXT_DIR := $(XS_DIR)/text
TEXT_POT := $(TEXT_DIR)/crashxs.pot
TEXT_LANGS := fr de es it nl
TEXT_POS := $(TEXT_LANGS:%=$(TEXT_DIR)/%.po)
TEXT_BUILDDIR := $(OBJ_DIR)/text
TEXT_MOS := $(TEXT_LANGS:%=$(TEXT_BUILDDIR)/%.mo)
TEXT_TABLES := ui_text cutscene_text_english cutscene_text_translations credits_text
TEXT_DEPS := $(TEXT_POT) $(PY_TEXT) engine/src/game/ui/data/hud_font_tables.cpp

MSGFMT := msgfmt
MSGMERGE := msgmerge

$(TEXT_BUILDDIR)/%.mo: $(TEXT_DIR)/%.po
	@mkdir -p $(dir $@)
	$(MSGFMT) --check -o $@ $<

$(TEXT_BUILDDIR)/ui_text.inc: $(TEXT_DEPS) $(TEXT_MOS) $(XS_DIR)/data/levels/levels.json
	python3 tools/text.py cpp ui_text $@ --mo $(TEXT_BUILDDIR)
$(TEXT_BUILDDIR)/cutscene_text_english.inc: $(TEXT_DEPS)
	python3 tools/text.py cpp cutscene_text_english $@
$(TEXT_BUILDDIR)/cutscene_text_translations.inc: $(TEXT_DEPS) $(TEXT_MOS)
	python3 tools/text.py cpp cutscene_text_translations $@ --mo $(TEXT_BUILDDIR)
$(TEXT_BUILDDIR)/credits_text.inc: $(TEXT_DEPS) $(XS_DIR)/data/credits.json
	python3 tools/text.py cpp credits_text $@

$(foreach t,$(TEXT_TABLES),$(eval $(call data_obj,$(t)): $(TEXT_BUILDDIR)/$(t).inc))
$(foreach t,$(TEXT_TABLES),$(eval $(call data_obj,$(t)): CPPFLAGS += -iquote $(TEXT_BUILDDIR)))

# The translations' checks (CI runs them): `msgfmt --check --statistics`
# on each .po, failing on an untranslated or fuzzy entry; msgcmp against
# the template both ways, failing on a missing or extra key; and the
# generator's checks (the font, the encoding, the level names' ids, the
# credits' keys and fonts, and each .po's credits.* entries being the
# English, unchanged and not fuzzy), and
# the cutscenes' (every page in the template, cutscenes.mk).
.PHONY: text-check text-update
text-check:
	python3 tools/text.py check
	python3 tools/cutscenes.py check

# After a template change: merges games/xs/text/crashxs.pot into every .po. A new
# key comes in untranslated and a changed English text marks its entry
# fuzzy (with the old text as `#| msgid`), so text-check fails until a
# translator has seen them (docs/text.md).
text-update:
	for po in $(TEXT_POS); do $(MSGMERGE) --quiet --update --backup=none --previous $$po $(TEXT_POT) || exit 1; done

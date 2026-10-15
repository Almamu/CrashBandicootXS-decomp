# N-Tranced's text (docs/n_tranced.md, "Text"), as text.mk builds XS's:
# games/n2/text/ntranced.pot is the template, games/n2/text/<lang>.po a
# language's text; msgfmt compiles each .po, and tools/text.py --game n2
# writes the C++ of the UI text (ui_text.inc), which
# games/n2/src/game/ui/data/ui_text.cpp #includes. Included by
# games/n2/game.mk.
N2_TEXT_DIR      := $(N2_DIR)/text
N2_TEXT_POT      := $(N2_TEXT_DIR)/ntranced.pot
N2_TEXT_LANGS    := fr de es it nl
N2_TEXT_BUILDDIR := $(OBJ_DIR)/text
N2_TEXT_MOS      := $(N2_TEXT_LANGS:%=$(N2_TEXT_BUILDDIR)/%.mo)

# A text's final newline is written ␤ in N-Tranced's .po files (tools/text.py):
# its French, German and Spanish texts 36 and 37 end in a newline the
# English doesn't, or the other way round, which msgfmt rejects.
$(N2_TEXT_BUILDDIR)/%.mo: $(N2_TEXT_DIR)/%.po
	@mkdir -p $(dir $@)
	msgfmt --check -o $@ $<

$(N2_TEXT_BUILDDIR)/ui_text.inc: $(N2_TEXT_POT) $(N2_TEXT_MOS) $(PY_TEXT)
	python3 tools/text.py --game n2 cpp ui_text $@ --mo $(N2_TEXT_BUILDDIR)

$(C_BUILDDIR)/game/ui/data/ui_text.o: $(N2_TEXT_BUILDDIR)/ui_text.inc
$(C_BUILDDIR)/game/ui/data/ui_text.o: CPPFLAGS += -iquote $(N2_TEXT_BUILDDIR)

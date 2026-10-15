# The cutscenes (docs/data.md, "Cutscenes", #1092): games/xs/data/cutscenes.json
# lists them, their slides and the page of text each slide shows, and
# tools/cutscenes.py writes, from it and the text template (the pages'
# strings, text.mk), the C++ that game/ui/data/<table>.cpp #include:
# - cutscenes: the language tables, gCutscenes, the English pages and the
#   slide lists;
# - cutscene_slides: the other five languages' pages and the slides.
# The generator stops on a slide whose picture isn't a cutscene picture of
# games/xs/graphics/manifest.json, on a page the template lacks and on a page or
# slide that no cutscene uses.
CUTSCENES_JSON := $(XS_DIR)/data/cutscenes.json
CUTSCENES_BUILDDIR := $(OBJ_DIR)/cutscenes
CUTSCENE_TABLES := cutscenes cutscene_slides
CUTSCENE_DEPS := $(CUTSCENES_JSON) $(TEXT_POT) $(XS_DIR)/graphics/manifest.json $(PY_CUTSCENES) $(PY_TEXT)

$(CUTSCENES_BUILDDIR)/%.inc: $(CUTSCENE_DEPS)
	python3 tools/cutscenes.py cpp $* $@

$(foreach t,$(CUTSCENE_TABLES),$(eval $(call data_obj,$(t)): $(CUTSCENES_BUILDDIR)/$(t).inc))
$(foreach t,$(CUTSCENE_TABLES),$(eval $(call data_obj,$(t)): CPPFLAGS += -iquote $(CUTSCENES_BUILDDIR)))

# The cutscene ids (CUTSCENE_*, CUTSCENE_COUNT), from cutscenes.json's
# "cutscenes". Written only if it changed, behind a stamp, so an edit that
# keeps them (a slide's timing, a page) doesn't rebuild every object.
GENERATED_HEADERS += $(GENERATED_INCLUDE_DIR)/constants/cutscenes.h

$(GENERATED_INCLUDE_DIR)/constants/cutscenes.h.stamp: $(CUTSCENE_DEPS)
	@mkdir -p $(dir $@)
	python3 tools/cutscenes.py constants $(GENERATED_INCLUDE_DIR)/constants/cutscenes.h
	@touch $@
$(GENERATED_INCLUDE_DIR)/constants/cutscenes.h: $(GENERATED_INCLUDE_DIR)/constants/cutscenes.h.stamp
	@test -f $@ || python3 tools/cutscenes.py constants $@

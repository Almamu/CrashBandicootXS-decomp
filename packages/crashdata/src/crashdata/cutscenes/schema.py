"""cutscenes.json's and credits.json's JSON Schemas (#1167, docs/editors.md).

From crashdata.cutscenes' slide fields (SLIDE_FIELDS, NUMBER_FIELDS,
ROM_ORDERS) and crashdata.text's credits items (KEY_CREDITS,
CREDITS_LOGOS); cutscenes.load and text.read_credits validate the files
with them before their own checks (the slides' pictures, the pages in the
template, ...).
"""
from crashdata.common import schema as S


def cutscenes():
    from crashdata import cutscenes as C
    slide = {"name": S.string(min_length=1), "picture": S.string(r"^cutscenes/.+\.png$",
                                                                 description="a cutscene picture of the graphics "
                                                                             "manifest, by path")}
    for k in C.SLIDE_FIELDS:
        if k in C.NUMBER_FIELDS:
            slide[k] = S.integer(*C.NUMBER_FIELDS[k])
        else:
            slide[k] = S.string(r"^[A-Za-z_][A-Za-z0-9_]*$", description="a constant's name (%s)" % (
                "SONG_*" if k == "song" else "SFX_*" if k == "sfx" else k))
    cutscene = S.obj({
        "constant": S.string(r"^CUTSCENE_[A-Z0-9_]+$"),
        "about": S.string(),
        "slides": S.array(S.obj({"slide": S.string(min_length=1, description="a slide of \"slides\", by name"),
                                 "text": S.string(r"^cutscene[0-9]{2}\.page[0-9]+$",
                                                  description="its page of text, cutsceneNN.pageP")},
                                required=["slide"]), min_items=1),
    }, required=["constant", "slides"])
    root = S.obj({
        "cutscenes": S.array(cutscene, min_items=1, description="in id order: PlayCutscene's argument"),
        "slides": S.array(S.obj(slide, required=["name", "picture"] + C.SLIDE_FIELDS),
                          description="the slide records (struct cutscene_slide)"),
        "rom_order": S.obj({k: S.array(S.string(r"^CUTSCENE_[A-Z0-9_]+$")) for k in C.ROM_ORDERS},
                           description="the ROM's order of the tables"),
    }, required=["cutscenes", "slides"])
    return S.document("cutscenes.schema.json", "The cutscenes (cutscenes.json)", root,
                      description="games/xs/data/cutscenes.json (docs/data.md, \"Cutscenes\")")


def credits():
    from crashdata import text as T
    key = S.string(T.KEY_CREDITS.pattern, description="a credits.* key of the text template")
    item = S.one_of(
        S.obj({"large": key}, required="all", description="the large font, then the text"),
        S.obj({"small": key}, required="all", description="the small font, then the text"),
        S.obj({"text": key}, required="all", description="the text, in the font so far"),
        S.obj({"picture": S.integer(0, T.CREDITS_LOGOS - 1)}, required="all", description="a logo"),
        S.obj({"blank": S.integer(1, None)}, required="all", description="empty lines"))
    root = S.obj({"credits": S.array(item, min_items=1)}, required="all")
    return S.document("credits.schema.json", "The credits (credits.json)", root,
                      description="games/xs/data/credits.json (docs/text.md, \"The credits\")")


# schema name -> (builder, the files' globs under the game's data directory)
SCHEMAS = {
    "cutscenes": (cutscenes, ["cutscenes.json"]),
    "credits": (credits, ["credits.json"]),
}

// The cutscene editor's logic (docs/editors.md, "The cutscene editor"): the
// text as the game draws it (the encoding, the font's characters,
// DrawWrappedText's screens, checked against XS's own text and font), the
// screen's rendering, the timeline's edits and their rules, the undo stack,
// and what a save sends.
import { readFileSync } from "node:fs";
import { join } from "node:path";
import { describe, expect, it } from "vitest";
import { Snapshots } from "../src/lib/cutscene/history.ts";
import * as M from "../src/lib/cutscene/model.ts";
import { gbaPixel, renderScreen, SCREEN_W } from "../src/lib/cutscene/render.ts";
import { checkString, encode, missingGlyphs, screens, type FontMetrics } from "../src/lib/cutscene/text.ts";

const ROOT = join(__dirname, "..", "..", "..");
const BOX = { x: 7, y: 126, w: 228, h: 30 };

/** XS's small font, from engine/src/game/ui/data/hud_font_tables.cpp (as crashdata.cutscenes.edit reads it). */
function xsFont(): FontMetrics {
  const src = readFileSync(join(ROOT, "engine/src/game/ui/data/hud_font_tables.cpp"), "latin1");
  const lit = /gSmallFontChars\[\] = "((?:[^"\\]|\\.)*)";/.exec(src)![1]!;
  const chars: number[] = [];
  for (let i = 0; i < lit.length; i++) {
    if (lit[i] === "\\") {
      const m = /^[0-7]{1,3}/.exec(lit.slice(i + 1))!;
      chars.push(parseInt(m[0], 8));
      i += m[0].length;
    } else chars.push(lit.charCodeAt(i));
  }
  const body = /gSmallFontGlyphs\[\d+\] = \{([\s\S]*?)\n\};/.exec(src)![1]!;
  const recs = [...body.matchAll(/^\s*\{\s*(\d+),\s*(\d+),\s*(\d+),/gm)].map((m) => [Number(m[1]), Number(m[2]), Number(m[3])]);
  return { chars, widths: recs.map((r) => r[0]!), shapes: recs.map((r) => r[1]!), yOffsets: recs.map((r) => r[2]!), space: 4, lineHeight: 9 };
}

/** The template's cutscene strings, by key (msgctxt -> msgid). */
function templateStrings(): Map<string, string> {
  const text = readFileSync(join(ROOT, "games/xs/text/crashxs.pot"), "utf8");
  const out = new Map<string, string>();
  for (const block of text.split("\n\n")) {
    const ctx = /^msgctxt "(.*)"$/m.exec(block)?.[1];
    if (!ctx?.startsWith("cutscene")) continue;
    const id = /^msgid ((?:".*"\n?)+)/m.exec(block)![1]!;
    out.set(ctx, [...id.matchAll(/"((?:[^"\\]|\\.)*)"/g)].map((m) => JSON.parse(`"${m[1]}"`) as string).join(""));
  }
  return out;
}

const font = xsFont();

describe("the text as the game draws it", () => {
  it("is written in Windows-1252, as tools/text.py writes it", () => {
    expect([...encode("œé€").bytes]).toEqual([0x9c, 0xe9, 0x80]);
    expect(encode("a☃b").bad).toEqual(["☃"]);
    expect(missingGlyphs(encode("ab Qz\n").bytes, font)).toEqual(["Q"]);
    expect(missingGlyphs(encode("abLR!?").bytes, font)).toEqual([]);
  });

  it("wraps a word at a time into three lines a screen", () => {
    const one = screens(encode("abc def").bytes, font, BOX);
    expect(one.length).toBe(1);
    expect(one[0]!.slice(0, 4)).toEqual([{ g: 0, x: 7, y: 118 }, { g: 1, x: 14, y: 118 }, { g: 2, x: 21, y: 118 }, { g: 3, x: 32, y: 118 }]);
    const long = screens(encode(Array(40).fill("abcdefg").join(" ")).bytes, font, BOX);
    expect(long.map((s) => s.length)).toEqual([84, 84, 84, 28]);
    expect([...new Set(long[0]!.map((g) => g.y))]).toEqual([118, 127, 136]);
    // a word wider than the box at a screen's start: on the second line, overflowing
    expect([...new Set(screens(encode("a".repeat(40)).bytes, font, BOX)[0]!.map((g) => g.y))]).toEqual([127]);
  });

  it("gives XS's English pages the screens crashdata gives (and the PC port shows)", () => {
    const want: Record<string, number> = {
      "cutscene01.page0.0": 1, "cutscene01.page2.0": 2, "cutscene01.page3.0": 1, "cutscene01.page3.1": 1, "cutscene02.page5.0": 2,
      "cutscene02.page7.0": 2, "cutscene02.page7.1": 2, "cutscene08.page1.0": 1, "cutscene09.page1.0": 2, "cutscene10.page5.0": 2,
    };
    const strings = templateStrings();
    for (const [k, n] of Object.entries(want)) expect([k, screens(encode(strings.get(k)!).bytes, font, BOX).length]).toEqual([k, n]);
    const sc = screens(encode(strings.get("cutscene02.page7.1")!).bytes, font, BOX);
    expect(sc.map((s) => s.length)).toEqual([90, 75]);
    expect([sc[0]![0], sc[0]![1], sc[0]![89], sc[1]![0], sc[1]![74]]).toEqual([
      { g: 2, x: 7, y: 118 }, { g: 14, x: 14, y: 118 }, { g: 4, x: 221, y: 136 }, { g: 15, x: 7, y: 118 }, { g: 42, x: 134, y: 136 },
    ]);
  });

  it("checks a string as the save does, and measures it", () => {
    const ok = checkString("on a mysterious space station orbiting high above the earth ...", font, BOX);
    expect(ok.problems).toEqual([]);
    expect(ok.screens).toBe(1);
    expect(ok.widest).toBeLessThanOrEqual(BOX.w);
    expect(checkString("", font, BOX).problems[0]!.severity).toBe("error");
    expect(checkString("Quiet", font, BOX).problems.map((p) => p.severity)).toEqual(["error"]);
    expect(checkString("a\nb", font, BOX).problems.map((p) => p.severity)).toEqual(["warning"]);
    expect(checkString("w".repeat(40), font, BOX).problems.map((p) => p.severity)).toEqual(["warning"]);
  });
});

describe("the screen", () => {
  it("draws the picture in the hardware's colours, and an earlier glyph in front", () => {
    expect(gbaPixel(0x7fff) >>> 0).toBe(0xffffffff);
    expect(gbaPixel(0x0010) & 0xff).toBe((16 << 3) | (16 >> 2));
    const pixels = new Uint8Array(240 * 160).fill(1);
    const palette = Array(256).fill(0);
    palette[1] = 0x001f;
    const cell = 16;
    const glyphs = new Uint8Array(2 * cell * cell);
    glyphs.fill(1, 0, cell * cell); // glyph 0: colour 1 everywhere
    glyphs.fill(2, cell * cell); // glyph 1: colour 2
    const out = new Uint32Array(240 * 160);
    const fp = { glyphs, cell, shapes: [0, 0], palette: [0, 0x03e0, 0x7c00] };
    renderScreen(out, { palette, pixels }, fp, [{ g: 0, x: 10, y: 10 }, { g: 1, x: 14, y: 10 }]);
    expect(out[0]! >>> 0).toBe(gbaPixel(0x001f) >>> 0);
    expect(out[10 * SCREEN_W + 15]! >>> 0).toBe(gbaPixel(0x03e0) >>> 0); // glyph 0's, though glyph 1 covers it too
    expect(out[10 * SCREEN_W + 19]! >>> 0).toBe(gbaPixel(0x7c00) >>> 0);
  });
});

function rec(name: string, picture = "cutscenes/00_a.png"): M.SlideRecord {
  return { name, picture, wait: 0, fade: 0, fade_after: 0, buttons: 1, duck_music: 0, rearm_sfx: 0, song: "SONG_INTRO", sfx: "SFX_NONE" };
}

function fixture(): M.EditState {
  return {
    doc: {
      cutscenes: [
        { constant: "CUTSCENE_A", slides: [{ slide: "one", text: "cutscene00.page0" }, { slide: "two" }, { slide: "one", text: "cutscene00.page2" }] },
        { constant: "CUTSCENE_B", slides: [{ slide: "two", text: "cutscene01.page0" }] },
      ],
      slides: [rec("one"), rec("two", "cutscenes/01_b.png")],
      rom_order: { pages: ["CUTSCENE_B", "CUTSCENE_A"] },
    },
    pages: { "cutscene00.page0": [M.placeholder("hello")], "cutscene00.page2": [M.placeholder("x"), M.placeholder("y")], "cutscene01.page0": [M.placeholder("b")] },
    pictures: {},
    newPictures: {},
  };
}

describe("the timeline", () => {
  it("moves, duplicates and adds slides", () => {
    const s = fixture();
    expect(M.moveSlide(s, 0, 0, 2).doc.cutscenes[0]!.slides.map((x) => x.slide)).toEqual(["two", "one", "one"]);
    expect(M.moveSlide(s, 0, 0, 9)).toBe(s);
    const d = M.duplicateSlide(s, 0, 0);
    expect(d.doc.cutscenes[0]!.slides[1]).toEqual({ slide: "one", text: "cutscene00.page1" });
    expect(d.pages["cutscene00.page1"]).toEqual(s.pages["cutscene00.page0"]);
    expect(d.pages["cutscene00.page1"]).not.toBe(s.pages["cutscene00.page0"]);
    const a = M.addSlide(s, 1, 1, { like: rec("two"), text: true });
    expect(a.doc.cutscenes[1]!.slides[1]).toEqual({ slide: "two_2", text: "cutscene01.page1" });
    expect(M.record(a.doc, "two_2")).toEqual({ ...rec("two"), name: "two_2" });
    expect(a.pages["cutscene01.page1"]).toEqual([M.placeholder()]);
    expect(M.addSlide(s, 1, 0, { record: "one" }).doc.cutscenes[1]!.slides[0]).toEqual({ slide: "one" });
  });

  it("deletes a slide, and what no slide shows any more", () => {
    const s = fixture();
    const d = M.deleteSlide(s, 0, 2);
    expect(Object.keys(d.pages).sort()).toEqual(["cutscene00.page0", "cutscene01.page0"]);
    expect(d.doc.slides.map((r) => r.name)).toEqual(["one", "two"]); // still shown
    const e = M.deleteSlide(d, 0, 0);
    expect(e.doc.cutscenes[0]!.slides).toEqual([{ slide: "two" }]);
    expect(e.doc.slides.map((r) => r.name)).toEqual(["two"]);
    expect(() => M.deleteSlide(s, 1, 0)).toThrow(/one slide or more/);
  });

  it("shares and unshares records and pages", () => {
    const s = fixture();
    expect(M.recordUsers(s.doc, "one")).toEqual([{ ci: 0, si: 0 }, { ci: 0, si: 2 }]);
    const o = M.ownRecord(s, 0, 2);
    expect(o.doc.cutscenes[0]!.slides[2]!.slide).toBe("one_2");
    expect(M.recordUsers(o.doc, "one")).toEqual([{ ci: 0, si: 0 }]);
    const r = M.setRecord(s, "one", { name: "first", wait: 90 });
    expect(r.doc.cutscenes[0]!.slides.map((x) => x.slide)).toEqual(["first", "two", "first"]);
    expect(M.record(r.doc, "first")!.wait).toBe(90);
    expect(() => M.setRecord(s, "one", { name: "two" })).toThrow();
    const t = M.setHasText(s, 0, 1, true);
    expect(t.doc.cutscenes[0]!.slides[1]!.text).toBe("cutscene00.page1");
    expect(M.setHasText(t, 0, 1, false).pages["cutscene00.page1"]).toBeUndefined();
    const p = M.ownPage({ ...s, doc: { ...s.doc, cutscenes: [{ ...s.doc.cutscenes[0]!, slides: [...s.doc.cutscenes[0]!.slides, { slide: "two", text: "cutscene00.page0" }] }, s.doc.cutscenes[1]!] } }, 0, 3);
    expect(p.doc.cutscenes[0]!.slides[3]!.text).toBe("cutscene00.page1");
  });

  it("edits a page's strings", () => {
    const s = fixture();
    const e = M.setString(s, "cutscene00.page0", 0, "fr", "bonjour");
    expect(e.pages["cutscene00.page0"]![0]!.fr).toBe("bonjour");
    expect(M.setString(e, "cutscene00.page0", 0, "fr", "bonjour")).toBe(e);
    const a = M.addString(s, "cutscene00.page2", 1);
    expect(a.pages["cutscene00.page2"]!.map((x) => x.en)).toEqual(["x", "...", "y"]);
    expect(M.moveString(s, "cutscene00.page2", 0, 1).pages["cutscene00.page2"]!.map((x) => x.en)).toEqual(["y", "x"]);
    expect(() => M.deleteString(s, "cutscene00.page0", 0)).toThrow(/one string or more/);
  });

  it("adds a cutscene at the end; only an unplayed last one goes", () => {
    const s = M.addCutscene(fixture(), "CUTSCENE_NEW", "two", "a test");
    expect(s.doc.cutscenes[2]).toEqual({ constant: "CUTSCENE_NEW", about: "a test", slides: [{ slide: "two" }] });
    expect(() => M.addCutscene(s, "CUTSCENE_NEW", "two")).toThrow(/already/);
    expect(() => M.addCutscene(s, "cutscene_x", "two")).toThrow();
    expect(M.cannotDelete(s, 0, [])).toMatch(/only the last/);
    expect(M.cannotDelete(s, 2, ["levels.json: X's on_clear"])).toMatch(/played by/);
    const d = M.deleteCutscene(s, 2, []);
    expect(d.doc.cutscenes.length).toBe(2);
    expect(M.deleteCutscene(fixture(), 1, []).doc.rom_order).toEqual({ pages: ["CUTSCENE_A"] });
  });

  it("names a new picture after the last one's number", () => {
    expect(M.newPicturePath(["cutscenes/00_a.png", "cutscenes/23_legal_notice.png"], "My Lab!")).toBe("cutscenes/24_my_lab.png");
  });
});

describe("undo", () => {
  it("steps back and forth, merges typing, and knows what was saved", () => {
    const h = new Snapshots<number>(0);
    expect(h.dirty).toBe(false);
    h.push("a", 1, "k", 1000);
    h.push("a", 2, "k", 1500); // the same field within a second: one step
    expect(h.current).toBe(2);
    expect(h.undo()).toBe(0);
    expect(h.dirty).toBe(false);
    expect(h.redo()).toBe(2);
    h.markSaved();
    h.push("b", 3, "k", 1600); // after a save: a step of its own
    expect(h.undo()).toBe(2);
    expect(h.dirty).toBe(false);
    h.push("c", 4, null, 5000);
    expect(h.canRedo).toBe(false);
    expect(h.undoLabel).toBe("c");
    expect(h.dirty).toBe(true);
    const small = new Snapshots<number>(0, 3);
    for (let i = 1; i <= 5; i++) small.push(`s${i}`, i);
    let n = 0;
    while (small.undo() !== undefined) n++;
    expect([n, small.current]).toEqual([3, 2]);
  });
});

describe("saving", () => {
  it("sends what changed: the document, pages (null: gone), pictures that differ, new pictures shown", () => {
    const saved = fixture();
    const pixels = new Uint8Array(4);
    const originals = { "cutscenes/00_a.png": { palette: [1, 2], pixels }, "cutscenes/01_b.png": { palette: [1, 2], pixels } };
    const existing = new Set(Object.keys(originals));
    expect(M.isEmptyBody(M.saveBody(saved, saved, originals, existing))).toBe(true);
    let now = M.setString(saved, "cutscene00.page0", 0, "de", "hallo");
    now = M.deleteSlide(now, 0, 2);
    now = { ...now, pictures: { "cutscenes/00_a.png": { palette: [1, 2], pixels: pixels.slice() }, "cutscenes/01_b.png": { palette: [1, 3], pixels } } };
    now = { ...now, newPictures: { "cutscenes/02_c.png": { palette: [0], pixels: new Uint8Array([5]), about: "c" }, "cutscenes/03_d.png": { palette: [0], pixels, about: "d" } } };
    now = M.setRecord(now, "two", { picture: "cutscenes/02_c.png" });
    const b = M.saveBody(saved, now, originals, existing);
    expect(b.cutscenes).toBe(now.doc);
    expect(b.pages).toEqual({ "cutscene00.page0": now.pages["cutscene00.page0"], "cutscene00.page2": null });
    expect(Object.keys(b.pictures!)).toEqual(["cutscenes/01_b.png"]); // 00_a's is the same
    expect(b.newPictures!.map((p) => [p.path, p.pixels])).toEqual([["cutscenes/02_c.png", "BQ=="]]); // 03_d isn't shown
  });
});

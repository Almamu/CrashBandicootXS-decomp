// A cutscene's text as the game draws it (docs/editors.md, "The cutscene
// editor"): the bytes of the game's encoding (Windows-1252, as tools/text.py
// writes them), the small font's characters, and DrawWrappedText's screens
// (engine/src/engine/text/wrapped_text.cpp, as CutscenePlayer::Run calls it:
// a word at a time into the text box, three lines a screen, the rest after a
// key press). crashdata.cutscenes.edit.screens is the same in Python; both
// give the PC port's screens pixel for pixel.

/** The font's metrics and characters (the backend's CutsceneFont). */
export interface FontMetrics {
  chars: number[];
  widths: number[];
  shapes: number[];
  yOffsets: number[];
  space: number;
  lineHeight: number;
}

export interface Box {
  x: number;
  y: number;
  w: number;
  h: number;
}

/** A glyph drawn: its index, its OBJ's top-left corner (y in OAM's 8 bits). */
export interface Glyph {
  g: number;
  x: number;
  y: number;
}

/** Windows-1252's characters at 0x80-0x9F (the five holes have none). */
const CP1252_HIGH: Record<number, number> = {
  0x20ac: 0x80, 0x201a: 0x82, 0x0192: 0x83, 0x201e: 0x84, 0x2026: 0x85, 0x2020: 0x86, 0x2021: 0x87, 0x02c6: 0x88,
  0x2030: 0x89, 0x0160: 0x8a, 0x2039: 0x8b, 0x0152: 0x8c, 0x017d: 0x8e, 0x2018: 0x91, 0x2019: 0x92, 0x201c: 0x93,
  0x201d: 0x94, 0x2022: 0x95, 0x2013: 0x96, 0x2014: 0x97, 0x02dc: 0x98, 0x2122: 0x99, 0x0161: 0x9a, 0x203a: 0x9b,
  0x0153: 0x9c, 0x017e: 0x9e, 0x0178: 0x9f,
};

/** A text's bytes in the game's encoding, and the characters it can't write. */
export function encode(text: string): { bytes: Uint8Array; bad: string[] } {
  const out: number[] = [];
  const bad: string[] = [];
  for (const ch of text) {
    const c = ch.codePointAt(0)!;
    if (c < 0x80 || (c >= 0xa0 && c <= 0xff)) out.push(c);
    else if (CP1252_HIGH[c] !== undefined) out.push(CP1252_HIGH[c]);
    else {
      bad.push(ch);
      out.push(0x3f);
    }
  }
  return { bytes: Uint8Array.from(out), bad };
}

/** A byte as the character it stands for (for messages). */
export function decodeByte(b: number): string {
  for (const [u, v] of Object.entries(CP1252_HIGH)) if (v === b) return String.fromCodePoint(Number(u));
  return String.fromCharCode(b);
}

/** InitSmallFont's charLookup: a byte's glyph (the first of the font's characters it is; 0 if none). */
export function glyphLookup(font: FontMetrics): Int16Array {
  const lookup = new Int16Array(256).fill(-1);
  [...font.chars, 0].forEach((c, j) => {
    if (lookup[c] === -1) lookup[c] = j;
  });
  return lookup;
}

function glyphOf(font: FontMetrics, lookup: Int16Array, c: number): number {
  const g = lookup[c]!;
  return g >= 0 && g < font.widths.length ? g : 0;
}

/** The characters of `bytes` the font has no glyph for (a space and a newline need none). */
export function missingGlyphs(bytes: Uint8Array, font: FontMetrics): string[] {
  const have = new Set(font.chars);
  const out = new Set<string>();
  for (const b of bytes) if (b !== 0x20 && b !== 0x0a && !have.has(b)) out.add(decodeByte(b));
  return [...out];
}

/** Font::MeasureChars. */
export function measure(bytes: Uint8Array, font: FontMetrics, lookup = glyphLookup(font)): number {
  let w = 0;
  for (const c of bytes) {
    if (c === 0x20) w += font.space;
    else if (c !== 0x0a) w += font.widths[glyphOf(font, lookup, c)]!;
  }
  return w;
}

/** GetWordLength: the token at i, up to and including a space. */
export function wordLength(text: Uint8Array, i: number): number {
  let n = 0;
  while (i + n < text.length) {
    n++;
    if (text[i + n - 1] === 0x20) break;
  }
  return n;
}

/** DrawWrappedText (per word): the bytes it consumes and the glyphs it draws, from text's start. */
export function drawWrapped(text: Uint8Array, font: FontMetrics, box: Box, lookup = glyphLookup(font)): { used: number; glyphs: Glyph[] } {
  const limit = Math.floor(box.h / font.lineHeight);
  let x = box.x;
  let y = box.y;
  let width = 0;
  let lines = 0;
  let used = 0;
  let i = 0;
  const glyphs: Glyph[] = [];
  const put = (c: number) => {
    if (c === 0x20) x += font.space;
    else if (c === 0x0a) {
      x = box.x;
      y += font.lineHeight;
    } else {
      const g = glyphOf(font, lookup, c);
      glyphs.push({ g, x, y: (y + font.yOffsets[g]!) & 0xff });
      x += font.widths[g]!;
    }
  };
  while (i < text.length && lines < limit) {
    const n = wordLength(text, i);
    const tok = text.subarray(i, i + n);
    i += n;
    if (tok[0] === 0x2f) {
      // "/b": half a line down, then a newline; "/n": a newline; any other word starting with "/" is skipped
      if (tok[1] === 0x62) y += 4;
      if (tok[1] === 0x62 || tok[1] === 0x6e) {
        put(0x0a);
        width = 0;
        lines++;
      }
      used += n;
      continue;
    }
    const w = measure(tok, font, lookup);
    if (width + w <= box.w) {
      for (const c of tok) put(c);
      width += w;
      used += n;
    } else {
      lines++;
      if (lines >= limit) break;
      put(0x0a);
      for (const c of tok) put(c);
      width = w;
      used += n;
    }
  }
  return { used, glyphs };
}

/** A string's screens: what each key press shows, each its glyphs. */
export function screens(text: Uint8Array, font: FontMetrics, box: Box): Glyph[][] {
  const lookup = glyphLookup(font);
  const out: Glyph[][] = [];
  let pos = 0;
  while (pos < text.length) {
    const r = drawWrapped(text.subarray(pos), font, box, lookup);
    out.push(r.glyphs);
    if (r.used === 0) break; // the game would show this screen for ever
    pos += r.used;
  }
  return out;
}

export interface Problem {
  severity: "error" | "warning";
  message: string;
}

export interface StringInfo {
  screens: number;
  /** the widest line, in pixels */
  widest: number;
  problems: Problem[];
}

/** A string's checks, as the save's and tools/text.py's (errors) and what the box shows (warnings). */
export function checkString(text: string, font: FontMetrics, box: Box): StringInfo {
  const problems: Problem[] = [];
  if (!text) {
    problems.push({ severity: "error", message: "empty (where the game keeps the English, write the English)" });
    return { screens: 0, widest: 0, problems };
  }
  const { bytes, bad } = encode(text);
  if (bad.length) problems.push({ severity: "error", message: `not in the game's encoding (Latin-1): ${[...new Set(bad)].join(" ")}` });
  const missing = missingGlyphs(bytes, font).filter((c) => !bad.includes(c) && c !== "?");
  if (missing.length) problems.push({ severity: "error", message: `the small font has no ${missing.map((c) => JSON.stringify(c)).join(", ")} (lower case, digits, A B L R and its punctuation)` });
  if (bytes.includes(0x0a)) problems.push({ severity: "warning", message: "a newline moves the text down without counting a line: the box can overflow (the game wraps the words itself)" });
  const lookup = glyphLookup(font);
  let widest = 0;
  for (let i = 0; i < bytes.length; ) {
    const n = wordLength(bytes, i);
    let tok = bytes.subarray(i, i + n);
    if (tok[tok.length - 1] === 0x20) tok = tok.subarray(0, tok.length - 1);
    const w = measure(tok, font, lookup);
    if (w > box.w) problems.push({ severity: "warning", message: `a word is ${w} pixels wide: wider than the box (${box.w}), it overflows it` });
    i += n;
  }
  const sc = screens(bytes, font, box);
  for (const s of sc) {
    const rows = new Map<number, number>();
    for (const gl of s) rows.set(gl.y, Math.max(rows.get(gl.y) ?? 0, gl.x + font.widths[gl.g]! - box.x));
    for (const w of rows.values()) widest = Math.max(widest, w);
  }
  return { screens: sc.length, widest, problems };
}

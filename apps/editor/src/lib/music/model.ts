// The music editor's model (docs/editors.md, "The music editor"): the
// edit state (the tables, the songs' cell edits, the samples changed),
// what a save sends, the XM cells as a tracker shows them and what the
// build makes of them (crashdata.audio.gax's XM import), a ROM song's
// shared patterns, and the sound effects' rules. No DOM: the tests run it
// in node.
import type { GaxManifestDefs, Instrument, Music, MusicCellEdit, MusicInstrument, MusicJsonEdit, MusicPatterns, MusicSave, MusicSong, MusicSongSettings, MusicStructure, MusicUnsaved, MusicView, MusicXmInstrument, MusicXmSample, SfxTable } from "@crash/schema-types";

export type InstrumentRecord = GaxManifestDefs.Instrument;

export type Cell = [number, number, number, number, number];
export const CELL = 5;
export const KEY_OFF = 97;
const NOTES = ["C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "A-", "A#", "B-"];
export const EFFECTS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/** A sample replaced: its new WAV (8-bit mono, base64), length and rate. */
export interface SampleEdit {
  wav: string;
  length: number;
  rate: number;
}

/** A new sound-effect instrument: instruments/<name>.json and sfx_samples/<name>.wav. */
export interface NewInstrument extends SampleEdit {
  volume?: number;
  comment?: string;
}

export interface NewSong {
  key: string;
  xm: string;
  comment: string;
}

/** What the music editor edits; every part immutable, shared between undo states. */
export interface EditState {
  sfx: SfxTable;
  music: Music;
  /** THEME_<NAME> -> song */
  themes: Record<string, string>;
  /** song -> comment */
  comments: Record<string, string>;
  /** song -> "pattern:row:channel" -> the cell, the edits made since the XM was saved */
  cells: Record<string, Record<string, Cell>>;
  /** samples/NN.wav, sfx_samples/NN.wav -> the new sound */
  samples: Record<string, SampleEdit>;
  newInstruments: Record<string, NewInstrument>;
  newSongs: NewSong[];
  removeSongs: string[];
  /** song -> its new structure (its cells with it: the song has no cell edits then) */
  structures: Record<string, Structure>;
  /** the music pool: instrument number -> its edited record (gax_manifest.json's) */
  instruments: Record<string, InstrumentRecord>;
  /** the music pool's new samples, by their numbers (the pool's next ones) */
  newSamples: Record<string, SampleEdit>;
  /** music pool samples taken out (no instrument plays them) */
  deleteSamples: number[];
  /** music pool instruments taken out (no song's XM names them): emptied, the pool's last ones removed */
  deleteInstruments: number[];
  /** song -> the pool instruments added to its XM's instruments (`Instrument N`), for its cells to play */
  songInstruments: Record<string, number[]>;
  /** song -> its settings (a ROM song's mix rate, volume, half-rate sound effects, echo taps; a new song's title too) */
  songSettings: Record<string, SongSettings>;
  /** the sound-effect set: instrument number -> its edited record (gax_sfx_manifest.json's; a number after the set's: a new one) */
  sfxInstruments: Record<string, InstrumentRecord>;
  /** the sound-effect set's new samples, by their numbers (the set's next ones) */
  newSfxSamples: Record<string, SampleEdit>;
  /** the set's samples taken out (no instrument plays them) */
  deleteSfxSamples: number[];
  /** the set's instruments taken out (no sound effect plays them): emptied, the set's last ones removed */
  deleteSfxInstruments: number[];
  /** song -> XM instrument number (1-based) -> its own instrument's fields, edited (crashdata.audio.edit.xm_with_own_instrument;
   * one `added`, after the XM's instruments: a new one) */
  xmInstruments: Record<string, Record<string, XmInstrumentEdit>>;
  /** instruments/<name>.json -> its value and its WAV's new sound, edited (one `added`: a new one) */
  jsonInstruments: Record<string, JsonInstrumentEdit>;
}

/** An XM sample's fields without a new sound. */
export type XmSample = Omit<MusicXmSample, "wav">;

/** A song's own XM instrument's fields (its samples' without their sounds). */
export type XmSpec = Omit<MusicXmInstrument, "samples"> & { samples: XmSample[] };

/** A song's own XM instrument's fields as edited, new sounds for its samples (8-bit, written as 8-bit; by sample
 * number: one after its samples is a new sample), and whether it is a new instrument. */
export interface XmInstrumentEdit {
  spec: XmSpec;
  sounds?: Record<string, SampleEdit>;
  added?: boolean;
}

/** An instruments/<name>.json instrument as edited: its value, a new sound for its WAV, and whether it is new. */
export interface JsonInstrumentEdit {
  json: Instrument;
  wav?: SampleEdit;
  added?: boolean;
}

/** A song's settings as the Music tab edits them (crashdata.audio.edit.ROM_SETTINGS, NEW_SETTINGS). */
export type SongSettings = MusicSongSettings;

export function initialState(v: MusicView, sfx: SfxTable, music: Music): EditState {
  return {
    sfx,
    music,
    themes: Object.fromEntries(v.themes.map((t) => [t.constant, t.song])),
    comments: Object.fromEntries(v.songs.map((s) => [s.key, s.comment])),
    cells: {},
    samples: {},
    newInstruments: {},
    newSongs: [],
    removeSongs: [],
    structures: {},
    instruments: {},
    newSamples: {},
    deleteSamples: [],
    deleteInstruments: [],
    songInstruments: {},
    songSettings: {},
    sfxInstruments: {},
    newSfxSamples: {},
    deleteSfxSamples: [],
    deleteSfxInstruments: [],
    xmInstruments: {},
    jsonInstruments: {},
  };
}

const same = (a: unknown, b: unknown) => JSON.stringify(a) === JSON.stringify(b);

/** What a save sends: the parts that differ from the state last saved. */
export function saveBody(saved: EditState, now: EditState): MusicSave {
  const body: MusicSave = {};
  if (!same(saved.sfx, now.sfx)) body.sfxTable = now.sfx;
  if (!same(saved.music, now.music)) body.music = now.music;
  const themes = Object.fromEntries(Object.entries(now.themes).filter(([k, s]) => saved.themes[k] !== s));
  if (Object.keys(themes).length) body.themes = themes;
  const comments = Object.fromEntries(Object.entries(now.comments).filter(([k, c]) => saved.comments[k] !== c && !now.newSongs.some((n) => n.key === k)));
  if (Object.keys(comments).length) body.songComments = comments;
  const patterns: Record<string, MusicCellEdit[]> = {};
  for (const [key, cells] of Object.entries(now.cells)) {
    const edits = Object.entries(cells).map(([k, cell]) => {
      const [pattern, row, channel] = k.split(":").map(Number) as [number, number, number];
      return { pattern, row, channel, cell };
    });
    if (edits.length) patterns[key] = edits;
  }
  if (Object.keys(patterns).length) body.patterns = patterns;
  if (Object.keys(now.samples).length) body.samples = Object.fromEntries(Object.entries(now.samples).map(([p, s]) => [p, s.wav]));
  const ni = Object.entries(now.newInstruments);
  if (ni.length) body.newInstruments = ni.map(([name, n]) => ({ name, wav: n.wav, rate: n.rate, ...(n.volume !== undefined ? { volume: n.volume } : {}), ...(n.comment ? { comment: n.comment } : {}) }));
  if (now.newSongs.length) body.newSongs = now.newSongs.map((s) => ({ key: s.key, xm: s.xm, comment: now.comments[s.key] ?? s.comment }));
  if (now.removeSongs.length) body.removeSongs = now.removeSongs;
  const structures = Object.entries(now.structures).filter(([k, st]) => saved.structures[k] !== st);
  if (structures.length) body.structures = Object.fromEntries(structures.map(([k, st]) => [k, structureBody(st)]));
  const instruments = Object.entries(now.instruments).filter(([k, r]) => saved.instruments[k] !== r);
  if (instruments.length) body.instruments = Object.fromEntries(instruments);
  if (Object.keys(now.newSamples).length) body.newSamples = Object.fromEntries(Object.entries(now.newSamples).map(([k, s]) => [k, s.wav]));
  if (now.deleteSamples.length) body.deleteSamples = now.deleteSamples;
  Object.assign(body, songAndPoolParts(saved, now));
  return body;
}

/** The parts of a save (and a player request) for the songs' added instruments, settings and own instruments, the
 * pool instruments taken out and the sound-effect set's instruments and samples: those that differ from the state
 * last saved. */
function songAndPoolParts(saved: EditState, now: EditState): MusicUnsaved {
  const body: MusicUnsaved = {};
  if (now.deleteInstruments.length) body.deleteInstruments = now.deleteInstruments;
  const added = Object.entries(now.songInstruments).filter(([, ns]) => ns.length);
  if (added.length) body.songInstruments = Object.fromEntries(added);
  const settings = Object.entries(now.songSettings).filter(([k, st]) => saved.songSettings[k] !== st);
  if (settings.length) body.songSettings = Object.fromEntries(settings);
  const sfx = Object.entries(now.sfxInstruments).filter(([k, r]) => saved.sfxInstruments[k] !== r);
  if (sfx.length) body.sfxInstruments = Object.fromEntries(sfx);
  if (Object.keys(now.newSfxSamples).length) body.newSfxSamples = Object.fromEntries(Object.entries(now.newSfxSamples).map(([k, s]) => [k, s.wav]));
  if (now.deleteSfxSamples.length) body.deleteSfxSamples = now.deleteSfxSamples;
  if (now.deleteSfxInstruments.length) body.deleteSfxInstruments = now.deleteSfxInstruments;
  const own = Object.entries(now.xmInstruments).filter(([k, x]) => saved.xmInstruments[k] !== x && Object.keys(x).length);
  if (own.length) body.xmInstruments = Object.fromEntries(own.map(([k, x]) => [k, Object.fromEntries(Object.entries(x).map(([n, e]) => [n, xmInstrumentBody(e)]))]));
  const json = Object.entries(now.jsonInstruments).filter(([k, j]) => saved.jsonInstruments[k] !== j);
  if (json.length) body.jsonInstruments = Object.fromEntries(json.map(([k, j]) => [k, jsonInstrumentBody(j)]));
  return body;
}

/** An own XM instrument's edit as a save or a player request sends it (MusicXmInstrument: its samples' new sounds as
 * their `wav`). */
export function xmInstrumentBody(e: XmInstrumentEdit): MusicXmInstrument {
  return { ...e.spec, samples: e.spec.samples.map((x, i) => (e.sounds?.[String(i)] ? { ...x, wav: e.sounds[String(i)]!.wav } : { ...x })) };
}

/** A JSON instrument's edit as a save or a player request sends it. */
export function jsonInstrumentBody(e: JsonInstrumentEdit): MusicJsonEdit {
  return e.wav ? { json: e.json, wav: e.wav.wav } : { json: e.json };
}

/** The songs' unsaved edits as the player asks the backend for them (POST music/song, music/pool): every new
 * song, structure and cell edit, so the songs (and the new instruments they add to the pool) are as the build
 * would build them. */
export function unsavedBody(now: EditState): MusicUnsaved {
  const body: MusicUnsaved = {};
  if (now.newSongs.length) body.newSongs = now.newSongs.map((s) => ({ key: s.key, xm: s.xm }));
  if (Object.keys(now.structures).length) body.structures = Object.fromEntries(Object.entries(now.structures).map(([k, st]) => [k, structureBody(st)]));
  const patterns = saveBody({ ...now, cells: {} }, now).patterns;
  if (patterns) body.patterns = patterns;
  // the music pool's edits (a music sample's new sound too: an instrument's loop is checked against it)
  if (Object.keys(now.instruments).length) body.instruments = now.instruments;
  const samples = Object.entries(now.samples).filter(([p]) => p.startsWith("samples/"));
  if (samples.length) body.samples = Object.fromEntries(samples.map(([p, s]) => [p, s.wav]));
  if (Object.keys(now.newSamples).length) body.newSamples = Object.fromEntries(Object.entries(now.newSamples).map(([k, s]) => [k, s.wav]));
  if (now.deleteSamples.length) body.deleteSamples = now.deleteSamples;
  const saved = { ...now, songSettings: {}, sfxInstruments: {}, xmInstruments: {}, jsonInstruments: {} };
  Object.assign(body, songAndPoolParts(saved, now));
  return body;
}

export function isEmptyBody(b: MusicSave): boolean {
  return Object.keys(b).length === 0;
}

// -- the XM's cells ------------------------------------------------------------------

/** A note as a tracker writes it: C-4 (XM's 1-96 are C-0 to B-7), === (key off), --- (none). */
export function noteName(n: number): string {
  if (n === 0) return "---";
  if (n === KEY_OFF) return "===";
  if (n < 1 || n > 96) return `?${n.toString(16).toUpperCase().padStart(2, "0")}`;
  return NOTES[(n - 1) % 12]! + Math.floor((n - 1) / 12);
}

/** C-4, c#4, ===, --- (or empty) as a note; null if it isn't one. */
export function parseNote(s: string): number | null {
  const t = s.trim().toUpperCase();
  if (t === "" || t === "---") return 0;
  if (t === "===" || t === "OFF") return KEY_OFF;
  const m = /^([A-G])([-#]?)([0-7])$/.exec(t);
  if (!m) return null;
  const i = NOTES.indexOf(m[1]! + (m[2] === "#" ? "#" : "-"));
  if (i < 0) return null;
  return Number(m[3]) * 12 + i + 1;
}

const hex2 = (v: number) => v.toString(16).toUpperCase().padStart(2, "0");

/** A cell's columns as a tracker shows them: note, instrument, volume column, effect and parameter. */
export function cellText(c: ArrayLike<number>): { note: string; ins: string; vol: string; fx: string } {
  const [note, ins, vol, eff, param] = [c[0]!, c[1]!, c[2]!, c[3]!, c[4]!];
  return {
    note: noteName(note),
    ins: ins ? String(ins).padStart(2, "0") : "..",
    vol: vol ? hex2(vol) : "..",
    fx: eff || param ? (EFFECTS[eff] ?? "?") + hex2(param) : "...",
  };
}

/** The volume column's byte that makes the effect column GAX2's own command and parameter (gax.GAX_RAW_VOLUME). */
export const RAW_VOLUME = 0x01;

/** What the build makes of a cell's effect and volume column: GAX2's one
 * effect of the row (crashdata.audio.gax._xm_effect_to_gax), [cmd, param]. */
export function gaxEffect(effect: number, param: number, volume: number): [number, number] {
  if (volume === RAW_VOLUME) return [effect, param];
  if ([1, 2, 3, 7, 13, 14].includes(effect)) return [effect, param];
  if (effect === 10 && param) {
    if (!(param & 0x0f)) return [10, param >> 4];
    if (!(param & 0xf0)) return [11, param];
  }
  if (effect === 12) return [12, param <= 0x40 ? Math.min(255, param * 4) : param];
  if (effect === 15 && param < 0x20) return [15, param];
  if (volume) {
    if (volume >= 0x90 && volume <= 0x9f) return [10, (volume - 0x90) << 4];
    if (volume >= 0x80 && volume <= 0x8f) return [11, volume - 0x80];
    if (volume >= 0x10 && volume <= 0x50) return [12, Math.min(255, (volume - 0x10) * 4)];
  }
  return [0, 0];
}

const GAX_EFFECTS: Record<number, string> = {
  1: "pitch slide up",
  2: "pitch slide down",
  3: "portamento to the note",
  7: "alternating speeds",
  10: "volume slide up",
  11: "volume slide down",
  12: "set the volume",
  13: "pattern break (to the next pattern's first row)",
  14: "E-commands (EDx: note delay)",
  15: "set the speed",
};

/** What GAX2 plays of a cell, and what the build drops (docs/audio.md, "Adding a song"). */
export function cellNotes(c: ArrayLike<number>): { plays: string; dropped: string[] } {
  const [note, , vol, eff, param] = [c[0]!, c[1]!, c[2]!, c[3]!, c[4]!];
  const [cmd, p] = gaxEffect(eff, param, vol);
  const dropped: string[] = [];
  const raw = vol === RAW_VOLUME;
  if ((eff || param) && !raw && gaxEffect(eff, param, 0)[0] === 0) {
    dropped.push(
      eff === 15
        ? `F${hex2(param)}: a BPM (0x20 or more); GAX2 steps a tick a frame`
        : eff === 10
          ? `A${hex2(param)}: a volume slide is up (x0) or down (0y), not both`
          : `effect ${EFFECTS[eff]}${hex2(param)}: GAX2 has no such effect`,
    );
  }
  if (vol && !raw) {
    if (gaxEffect(eff, param, 0)[0] !== 0) dropped.push(`the volume column ${hex2(vol)}: a GAX2 row has one effect, the effect column's`);
    else if (gaxEffect(0, 0, vol)[0] === 0) dropped.push(`the volume column ${hex2(vol)}: not one GAX2 has (01 GAX2's own effect, 10-50, 80-9F)`);
  }
  const parts: string[] = [];
  if (note === KEY_OFF) parts.push("key off");
  else if (note >= 1 && note <= 96) parts.push(noteName(note));
  if (cmd) parts.push(`${raw ? "GAX2's own " : ""}${GAX_EFFECTS[cmd] ?? `effect ${cmd}`} ${hex2(p)}${cmd === 7 ? ` (${p >> 4} ticks, then ${p & 15})` : ""}`);
  return { plays: parts.join(", ") || "nothing", dropped };
}

/** A pattern's cells, decoded from the API's base64. */
export function decodeCells(p: MusicPatterns): Uint8Array[] {
  return p.cells.map((b) => fromBase64(b));
}

export function cellKey(pattern: number, row: number, channel: number): string {
  return `${pattern}:${row}:${channel}`;
}

export function cellAt(cells: Uint8Array[], channels: number, pattern: number, row: number, channel: number): Cell {
  const k = (row * channels + channel) * CELL;
  const c = cells[pattern]!;
  return [c[k]!, c[k + 1]!, c[k + 2]!, c[k + 3]!, c[k + 4]!];
}

/** A ROM song's pattern sharing: [order position, channel, transpose difference] of every
 * pattern stored once with channel `ch`'s at `pi` (crashdata.audio.edit.shared_cells). */
export function sharedCells(p: MusicPatterns, pi: number, ch: number): [number, number, number][] {
  if (!p.groups || !p.transposes) return [[pi, ch, 0]];
  const gid = p.groups[ch]![pi]!;
  const t0 = p.transposes[ch]![pi]!;
  const out: [number, number, number][] = [];
  p.groups.forEach((row, cj) => row.forEach((g, pj) => g === gid && out.push([pj, cj, p.transposes![cj]![pj]! - t0])));
  return out;
}

/** The cells after the edits, each copied to the patterns it shares its bytes with
 * (crashdata.audio.edit.expand_edits: what the save writes). */
export function editedCells(p: MusicPatterns, base: Uint8Array[], edits: Record<string, Cell> | undefined): Uint8Array[] {
  if (!edits || !Object.keys(edits).length) return base;
  const out = base.map((c) => c);
  const copied = new Set<number>();
  const put = (pi: number, row: number, ch: number, cell: Cell) => {
    if (!copied.has(pi)) {
      out[pi] = new Uint8Array(base[pi]!);
      copied.add(pi);
    }
    out[pi]!.set(cell, (row * p.channels + ch) * CELL);
  };
  for (const [k, cell] of Object.entries(edits)) {
    const [pi, row, ch] = k.split(":").map(Number) as [number, number, number];
    if (!p.rom) {
      put(pi, row, ch, cell);
      continue;
    }
    if (same(cellAt(base, p.channels, pi, row, ch), cell)) continue;
    for (const [pj, cj, dt] of sharedCells(p, pi, ch)) {
      const c = [...cell] as Cell;
      if (c[0] >= 1 && c[0] <= 96) c[0] = Math.max(1, Math.min(96, c[0] + dt));
      put(pj, row, cj, c);
    }
  }
  return out;
}

// -- a song's structure ------------------------------------------------------------------

/** An XM pattern: its rows and cells (rows * channels * 5 bytes). */
export interface Pattern {
  rows: number;
  cells: Uint8Array;
}

/** An XM's structure (crashdata.audio.edit.structure): what a structural edit changes. Immutable:
 * every edit makes a new one, sharing the patterns it doesn't change. */
export interface Structure {
  channels: number;
  patterns: Pattern[];
  /** the XM patterns the order positions play */
  order: number[];
  /** the order position the song loops back to */
  restart: number;
  /** the speed the song starts at (the module's; the build puts an Fxx with it on the first row, unless that row sets one) */
  speed: number;
}

// FT2's limits (crashdata.audio.edit.MAX_*)
export const MAX_CHANNELS = 32;
export const MAX_PATTERNS = 256;
export const MAX_ROWS = 256;
export const MAX_ORDER = 256;
export const MAX_SPEED = 31;

/** A song's structure as its XM is, with the cell edits made (a ROM song's shared patterns are copied out:
 * from here on each XM pattern is its own). */
export function structureOf(p: MusicPatterns, cells: Uint8Array[]): Structure {
  return {
    channels: p.channels,
    patterns: p.rows.map((rows, i) => ({ rows, cells: cells[i]! })),
    order: [...p.order],
    restart: p.restart,
    speed: p.speed,
  };
}

export function structureBody(s: Structure): MusicStructure {
  return { channels: s.channels, patterns: s.patterns.map((p) => ({ rows: p.rows, cells: toBase64(p.cells) })), order: [...s.order], restart: s.restart, speed: s.speed };
}

const sameBytes = (a: Uint8Array, b: Uint8Array) => a === b || (a.length === b.length && a.every((v, i) => v === b[i]));

export function sameStructure(a: Structure, b: Structure): boolean {
  return (
    a.channels === b.channels &&
    a.restart === b.restart &&
    a.speed === b.speed &&
    same(a.order, b.order) &&
    a.patterns.length === b.patterns.length &&
    a.patterns.every((p, i) => p.rows === b.patterns[i]!.rows && sameBytes(p.cells, b.patterns[i]!.cells))
  );
}

/** The pattern view's form of a structure: what MusicPatterns says of a song built from its XM. */
export function structureView(base: MusicPatterns, s: Structure): MusicPatterns {
  const { groups: _g, transposes: _t, ...rest } = base;
  void _g;
  void _t;
  return { ...rest, scaffold: false, channels: s.channels, rows: s.patterns.map((p) => p.rows), cells: [], order: s.order, restart: s.restart, speed: s.speed, playOrder: s.order, loopOrder: s.restart };
}

export function emptyPattern(rows: number, channels: number): Pattern {
  return { rows, cells: new Uint8Array(rows * channels * CELL) };
}

function check(ok: boolean, why: string): void {
  if (!ok) throw new Error(why);
}

function withPattern(s: Structure, i: number, p: Pattern): Structure {
  const patterns = [...s.patterns];
  patterns[i] = p;
  return { ...s, patterns };
}

/** A new empty pattern at the end, of `rows` rows. */
export function addPattern(s: Structure, rows: number): Structure {
  check(s.patterns.length < MAX_PATTERNS, `an XM has at most ${MAX_PATTERNS} patterns`);
  return { ...s, patterns: [...s.patterns, emptyPattern(rows, s.channels)] };
}

/** A copy of pattern i at the end. */
export function duplicatePattern(s: Structure, i: number): Structure {
  check(s.patterns.length < MAX_PATTERNS, `an XM has at most ${MAX_PATTERNS} patterns`);
  const p = s.patterns[i]!;
  return { ...s, patterns: [...s.patterns, { rows: p.rows, cells: new Uint8Array(p.cells) }] };
}

export function clearPattern(s: Structure, i: number): Structure {
  const p = s.patterns[i]!;
  return withPattern(s, i, emptyPattern(p.rows, s.channels));
}

/** Pattern i taken out: the order positions that play it go too, and the later patterns' numbers move down. */
export function deletePattern(s: Structure, i: number): Structure {
  check(s.patterns.length > 1, "an XM has at least one pattern");
  const keep = s.order.map((p, pos) => [p, pos] as const).filter(([p]) => p !== i);
  check(keep.length > 0, `every order position plays pattern ${i}: the order list can't be empty`);
  // the loop point: the first position kept from the one it marked on
  const at = keep.findIndex(([, pos]) => pos >= s.restart);
  return {
    ...s,
    patterns: s.patterns.filter((_, k) => k !== i),
    order: keep.map(([p]) => (p > i ? p - 1 : p)),
    restart: at >= 0 ? at : keep.length - 1,
  };
}

/** Pattern i with n rows: rows cut from its end, or empty rows added there. */
export function setRows(s: Structure, i: number, n: number): Structure {
  check(Number.isInteger(n) && n >= 1 && n <= MAX_ROWS, `a pattern has 1-${MAX_ROWS} rows`);
  const p = s.patterns[i]!;
  const cells = new Uint8Array(n * s.channels * CELL);
  cells.set(p.cells.subarray(0, Math.min(p.cells.length, cells.length)));
  return withPattern(s, i, { rows: n, cells });
}

/** An empty row inserted at `at` in pattern i (the rows after it move down; the pattern grows by one). */
export function insertRow(s: Structure, i: number, at: number): Structure {
  const p = s.patterns[i]!;
  check(p.rows < MAX_ROWS, `a pattern has at most ${MAX_ROWS} rows`);
  const w = s.channels * CELL;
  const cells = new Uint8Array((p.rows + 1) * w);
  cells.set(p.cells.subarray(0, at * w));
  cells.set(p.cells.subarray(at * w), (at + 1) * w);
  return withPattern(s, i, { rows: p.rows + 1, cells });
}

/** Row `at` of pattern i taken out (the rows after it move up). */
export function deleteRow(s: Structure, i: number, at: number): Structure {
  const p = s.patterns[i]!;
  check(p.rows > 1, "a pattern has at least one row");
  const w = s.channels * CELL;
  const cells = new Uint8Array((p.rows - 1) * w);
  cells.set(p.cells.subarray(0, at * w));
  cells.set(p.cells.subarray((at + 1) * w), at * w);
  return withPattern(s, i, { rows: p.rows - 1, cells });
}

/** An order position playing `pattern` inserted at `pos`. */
export function insertOrder(s: Structure, pos: number, pattern: number): Structure {
  check(s.order.length < MAX_ORDER, `an order list has at most ${MAX_ORDER} positions`);
  const order = [...s.order];
  order.splice(pos, 0, pattern);
  return { ...s, order, restart: pos <= s.restart ? s.restart + 1 : s.restart };
}

export function removeOrder(s: Structure, pos: number): Structure {
  check(s.order.length > 1, "the order list can't be empty");
  const order = s.order.filter((_, k) => k !== pos);
  return { ...s, order, restart: Math.min(order.length - 1, s.restart > pos ? s.restart - 1 : s.restart) };
}

/** Order position `pos` moved to `to` (the loop point stays on the position it marks). */
export function moveOrder(s: Structure, pos: number, to: number): Structure {
  if (to < 0 || to >= s.order.length || to === pos) return s;
  const idx = s.order.map((_, k) => k);
  idx.splice(to, 0, idx.splice(pos, 1)[0]!);
  return { ...s, order: idx.map((k) => s.order[k]!), restart: idx.indexOf(s.restart) };
}

export function setOrder(s: Structure, pos: number, pattern: number): Structure {
  check(pattern >= 0 && pattern < s.patterns.length, `there is no pattern ${pattern}`);
  const order = [...s.order];
  order[pos] = pattern;
  return { ...s, order };
}

export function setRestart(s: Structure, pos: number): Structure {
  check(pos >= 0 && pos < s.order.length, `there is no order position ${pos}`);
  return { ...s, restart: pos };
}

export function setSpeed(s: Structure, speed: number): Structure {
  check(Number.isInteger(speed) && speed >= 1 && speed <= MAX_SPEED, `a speed is 1-${MAX_SPEED}`);
  return { ...s, speed };
}

/** An empty channel inserted at `at` (the end by default) in every pattern. */
export function addChannel(s: Structure, at = s.channels): Structure {
  check(s.channels < MAX_CHANNELS, `an XM has at most ${MAX_CHANNELS} channels`);
  const n = s.channels + 1;
  const patterns = s.patterns.map((p) => {
    const cells = new Uint8Array(p.rows * n * CELL);
    for (let r = 0; r < p.rows; r++) {
      const src = p.cells.subarray(r * s.channels * CELL, (r + 1) * s.channels * CELL);
      cells.set(src.subarray(0, at * CELL), r * n * CELL);
      cells.set(src.subarray(at * CELL), (r * n + at + 1) * CELL);
    }
    return { rows: p.rows, cells };
  });
  return { ...s, channels: n, patterns };
}

/** Channel `ch` taken out of every pattern. */
export function removeChannel(s: Structure, ch: number): Structure {
  check(s.channels > 1, "an XM has at least one channel");
  const n = s.channels - 1;
  const patterns = s.patterns.map((p) => {
    const cells = new Uint8Array(p.rows * n * CELL);
    for (let r = 0; r < p.rows; r++) {
      const src = p.cells.subarray(r * s.channels * CELL, (r + 1) * s.channels * CELL);
      cells.set(src.subarray(0, ch * CELL), r * n * CELL);
      cells.set(src.subarray((ch + 1) * CELL), (r * n + ch) * CELL);
    }
    return { rows: p.rows, cells };
  });
  return { ...s, channels: n, patterns };
}

export function setCellIn(s: Structure, pattern: number, row: number, channel: number, cell: Cell): Structure {
  const p = s.patterns[pattern]!;
  const cells = new Uint8Array(p.cells);
  cells.set(cell, (row * s.channels + channel) * CELL);
  return withPattern(s, pattern, { rows: p.rows, cells });
}

/** The speed the first row sets (its Fxx or 7xy), which the module's speed doesn't override; null if none. */
export function firstRowSpeed(s: Structure): number | null {
  const p = s.patterns[s.order[0]!];
  if (!p) return null;
  for (let c = 0; c < s.channels; c++) {
    const k = c * CELL;
    const [cmd, param] = gaxEffect(p.cells[k + 3]!, p.cells[k + 4]!, p.cells[k + 2]!);
    if (cmd === 15) return param;
    if (cmd === 7) return param >> 4;
  }
  return null;
}

// -- the music pool's instruments ------------------------------------------------------------------

export type WaveRow = InstrumentRecord["samples"][number];
export type SeqStep = InstrumentRecord["rows"][number];
export type LoopKind = "none" | "forward" | "pingpong" | "sweep";

/** 0xff in an envelope's sustain or loop point: none (the manifest writes 255 or null). */
export const NO_POINT = 0xff;

/** A wave row's tune (1/32 semitones) as a tracker's relative note (semitones) and finetune (32nds, 0-31). */
export function tuneParts(pitch: number): { note: number; fine: number } {
  const note = Math.floor(pitch / 32);
  return { note, fine: pitch - note * 32 };
}

export function loopKind(w: WaveRow): LoopKind {
  if (w.byte_02) return "sweep";
  if (w.loop_start < w.loop_end) return w.is_bidirectional ? "pingpong" : "forward";
  return "none";
}

/** A wave row with a loop kind: none (no loop points), forward or ping-pong over [start, end). */
export function withLoop(w: WaveRow, kind: Exclude<LoopKind, "sweep">, start: number, end: number): WaveRow {
  if (kind === "none") return { ...w, byte_02: 0, is_bidirectional: false, loop_start: 0, loop_end: 0 };
  return { ...w, byte_02: 0, is_bidirectional: kind === "pingpong", loop_start: start, loop_end: end };
}

/** The shortest sample a wave row can play (crashdata.audio.pool.wave_row_needs). */
export function waveRowNeeds(w: WaveRow): number {
  let need = w.start_position + 1;
  if (w.loop_start < w.loop_end) need = Math.max(need, w.loop_end);
  if (w.byte_02) need = Math.max(need, w.start_position + w.int_10, w.loop_end);
  return need;
}

export interface EnvPoint {
  x: number;
  y: number;
}

/** An envelope's points as the graph draws them: ticks and volumes (0-255). */
export function envPoints(r: InstrumentRecord): EnvPoint[] {
  return (r.envelope?.points ?? []).map((p) => ({ x: p.x, y: p.y }));
}

const pointIndex = (v: number | null | undefined) => (v === null || v === undefined || v === NO_POINT ? null : v);

/** The record with an envelope of these points (and sustain, loop: point indices or null); the backend works out
 * the slopes (crashdata.audio.pool.edited_envelope), the player is given them by it. */
export function withEnvelope(r: InstrumentRecord, points: EnvPoint[], sustain: number | null, loopStart: number | null, loopEnd: number | null): InstrumentRecord {
  const old = r.envelope?.points ?? [];
  return {
    ...r,
    envelope: {
      sustain: sustain ?? (r.envelope?.sustain === NO_POINT ? NO_POINT : null),
      loop_start: loopStart ?? (r.envelope?.loop_start === NO_POINT ? NO_POINT : null),
      loop_end: loopEnd ?? (r.envelope?.loop_end === NO_POINT ? NO_POINT : null),
      points: points.map((p, i) => ({ x: p.x, y: p.y, interpolation: old[i]?.x === p.x && old[i]?.y === p.y ? old[i]!.interpolation : 0, byte_05: 0, ushort_06: 0 })),
    },
  } as InstrumentRecord;
}

export function envMarks(r: InstrumentRecord): { sustain: number | null; loopStart: number | null; loopEnd: number | null } {
  return { sustain: pointIndex(r.envelope?.sustain), loopStart: pointIndex(r.envelope?.loop_start), loopEnd: pointIndex(r.envelope?.loop_end) };
}

/** Why an envelope's points can't be (null if they can): the first at tick 0, the ticks increasing, volumes 0-255. */
export function envelopeProblem(points: EnvPoint[]): string | null {
  if (!points.length) return "an envelope has a point";
  if (points[0]!.x !== 0) return "the first point is at tick 0";
  for (let i = 1; i < points.length; i++) if (points[i]!.x <= points[i - 1]!.x) return `point ${i}'s tick isn't after point ${i - 1}'s`;
  if (points.some((p) => p.y < 0 || p.y > 255 || p.x > 0xffff)) return "a volume is 0-255, a tick 0-65535";
  return null;
}

/** A music pool instrument as the Instruments tab lists it after the edits: its number, its record, whether it is
 * new (not saved yet) or taken out (emptied: a later one keeps its number). */
export interface PoolInstrument {
  index: number;
  record: InstrumentRecord;
  added: boolean;
  deleted: boolean;
}

/** The music pool's instruments after the edits (crashdata.audio.edit.apply_pool_edits): the saved ones, edited,
 * then the new ones (the pool's next numbers), those taken out emptied and the empty ones at the end gone
 * (crashdata.audio.pool.taken_out). */
export function poolInstruments(v: MusicView, s: EditState): PoolInstrument[] {
  const saved = new Set(v.poolInstruments.map((i) => i.index));
  const out: PoolInstrument[] = v.poolInstruments.map((i) => {
    const deleted = s.deleteInstruments.includes(i.index);
    return { index: i.index, record: deleted ? v.instrumentTemplates.empty : (s.instruments[String(i.index)] ?? i.record), added: false, deleted };
  });
  for (const k of Object.keys(s.instruments)
    .map(Number)
    .filter((n) => !saved.has(n))
    .sort((a, b) => a - b))
    out.push({ index: k, record: s.instruments[String(k)]!, added: true, deleted: false });
  while (out.length > 1 && out[out.length - 1]!.deleted) out.pop();
  return out;
}

/** The number a new pool instrument gets: after the saved ones and the new ones (crashdata.audio.edit: the pool's
 * next numbers). */
export function nextInstrument(v: MusicView, s: EditState): number {
  return Math.max(...v.poolInstruments.map((i) => i.index), ...Object.keys(s.instruments).map(Number)) + 1;
}

/** A new pool instrument's record: blank (no sample yet), from a sample of the pool (at 8363 Hz on C-4, as the
 * build makes one from an XM's sample with no relative note or finetune), or a copy of a record. */
export function newInstrumentRecord(v: MusicView, from: { sample: number } | { copy: InstrumentRecord } | null): InstrumentRecord {
  const blank = v.instrumentTemplates.blank;
  if (from && "copy" in from) return structuredClone(from.copy);
  const r = structuredClone(blank);
  if (from) r.sample_indices = [from.sample, 0, 0, 0] as InstrumentRecord["sample_indices"];
  return r;
}

/** A song's XM instruments after the edits: its own, then the pool instruments added to it (`Instrument N`). */
export function songInstruments(p: MusicPatterns, added: number[] | undefined, own?: Record<string, XmInstrumentEdit>): { name: string; pool: number | null; added: boolean }[] {
  // the new ones after the XM's: its own new ones at their numbers, the pool's in the others, in their order
  // (crashdata.audio.edit.xm_with_instruments)
  const out = p.instruments.map((i, k) => ({ name: own?.[String(k + 1)]?.spec.name ?? i.name, pool: i.pool, added: false })); // an own one renamed
  const fresh = new Map(Object.entries(own ?? {}).flatMap(([n, e]) => (e.added ? [[Number(n), e.spec.name] as const] : [])));
  const pool = [...(added ?? [])];
  for (let n = out.length + 1, end = out.length + pool.length + fresh.size; n <= end; n++) {
    const name = fresh.get(n);
    if (name !== undefined) out.push({ name, pool: null, added: true });
    else {
      const k = pool.shift()!;
      out.push({ name: `Instrument ${k}`, pool: k, added: true });
    }
  }
  return out;
}

// -- the sound-effect set's instruments -------------------------------------------------------------

/** The sound-effect set's instruments after the edits (crashdata.audio.edit.apply_sfx_edits), as poolInstruments
 * lists the music pool's: the saved ones, edited, then the new ones (the set's next numbers), those taken out emptied
 * and the empty ones at the end gone. */
export function sfxInstruments(v: MusicView, s: EditState): PoolInstrument[] {
  const saved = new Set(v.sfxInstruments.map((i) => i.index));
  const out: PoolInstrument[] = v.sfxInstruments.map((i) => {
    const deleted = s.deleteSfxInstruments.includes(i.index);
    return { index: i.index, record: deleted ? v.instrumentTemplates.empty : (s.sfxInstruments[String(i.index)] ?? i.record), added: false, deleted };
  });
  for (const k of Object.keys(s.sfxInstruments)
    .map(Number)
    .filter((n) => !saved.has(n))
    .sort((a, b) => a - b))
    out.push({ index: k, record: s.sfxInstruments[String(k)]!, added: true, deleted: false });
  while (out.length > 1 && out[out.length - 1]!.deleted) out.pop();
  return out;
}

/** The number a new sound-effect instrument gets: after the set's saved and new ones. */
export function nextSfxInstrument(v: MusicView, s: EditState): number {
  return Math.max(...v.sfxInstruments.map((i) => i.index), ...Object.keys(s.sfxInstruments).map(Number)) + 1;
}

/** A new sound-effect instrument's record: blank (as the ROM's are: a fixed pitch at the set's one rate, no sample
 * yet), from a sample of the set, or a copy of a record. */
export function newSfxInstrumentRecord(v: MusicView, from: { sample: number } | { copy: InstrumentRecord } | null): InstrumentRecord {
  if (from && "copy" in from) return structuredClone(from.copy);
  const r = structuredClone(v.instrumentTemplates.sfxBlank);
  if (from) r.sample_indices = [from.sample, 0, 0, 0] as InstrumentRecord["sample_indices"];
  return r;
}

/** The rate a wave row's tune plays a fixed-pitch sample at (gGaxPeriodTable[(note - 2) * 32 + tune] / 2048). */
export function fixedRate(period: number[], r: InstrumentRecord): number {
  const i = ((r.rows[0]?.relative_note_number ?? 2) & 0xff) * 32 - 64 + (r.samples[0]?.pitch ?? 0);
  return period[Math.max(0, Math.min(period.length - 1, i))]! / 2048;
}

// -- a song's own XM instruments -------------------------------------------------------------------

/** XM's limits on an instrument's envelope (FT2's), its volumes and loops (crashdata.audio.edit). */
export const XM_ENVELOPE_POINTS = 12;
export const XM_C4 = 49;
const XM_C4_RATE = 8363;

export const XM_KEYS = 96;
export const XM_NAME = 22;
export const XM_SAMPLES = 16;
/** a new XM sample's fields: FT2's (full volume, C-4 at 8363 Hz, no loop) */
export const XM_NEW_SAMPLE: XmSample = { volume: 64, relative_note: 0, finetune: 0, loop: "none", loop_start: 0, loop_end: 0 };

/** An own XM instrument's fields without its samples' new sounds (`wav`). */
export function xmSpec(x: MusicXmInstrument): XmSpec {
  return { ...x, samples: x.samples.map(({ wav: _w, ...s }) => (void _w, s)) };
}

/** {XM instrument: the notes its cells play} over the order list, a note with no instrument the channel's last one's
 * (crashdata.audio.edit.played_notes). */
export function playedNotes(p: MusicPatterns, cells: Uint8Array[]): Map<number, Set<number>> {
  const out = new Map<number, Set<number>>();
  for (const pi of p.playOrder) {
    const c = cells[pi];
    if (!c) continue;
    const current = new Array<number>(p.channels).fill(0);
    for (let r = 0; r < (p.rows[pi] ?? 0); r++)
      for (let ch = 0; ch < p.channels; ch++) {
        const k = (r * p.channels + ch) * CELL;
        if (c[k + 1]) current[ch] = c[k + 1]!;
        const note = c[k]!;
        if (note >= 1 && note <= 96 && current[ch]) {
          const set = out.get(current[ch]!) ?? new Set<number>();
          set.add(note);
          out.set(current[ch]!, set);
        }
      }
  }
  return out;
}

/** The samples an XM instrument's key map plays on `notes` (C-4's if none; none with no sample): one is the sample
 * the build makes its GAX2 instrument from, more than one it refuses (crashdata.audio.gax.new_song_instruments). */
export function keymapPlays(spec: XmSpec, notes: number[]): number[] {
  if (!spec.samples.length) return [];
  const plays = [...new Set(notes.map((k) => spec.keymap[k - 1] ?? 0))].sort((a, b) => a - b);
  return plays.length ? plays : [spec.keymap[XM_C4 - 1] ?? 0];
}

/** A name an XM instrument can have, made of `s` (a file's name): printable ASCII, at most 22 characters, not a pool
 * instrument's. */
export function xmName(s: string): string {
  const name = [...s].map((c) => (c >= " " && c <= "~" ? c : "_")).join("").slice(0, XM_NAME);
  return /^Instrument \d+$/.test(name) ? `${name} copy`.slice(0, XM_NAME) : name;
}

/** Why an own XM instrument's name can't be (null if it can): crashdata.audio.edit.xm_with_own_instrument's checks. */
export function xmNameProblem(name: string, samples: number, jsonNames: Iterable<string>): string | null {
  if (name.length > XM_NAME || ![...name].every((c) => c >= " " && c <= "~")) return `a name is at most ${XM_NAME} ASCII characters`;
  if (/^Instrument \d+$/.test(name)) return `"${name}" names the pool's instrument: a cell plays one from the pool's list`;
  if (samples && [...jsonNames].includes(name)) return `"${name}" is instruments/${name}.json's: an instrument of that name plays it, not its own samples`;
  return null;
}

/** Python's round: halves to even (the build's arithmetic is Python's). */
export function roundEven(x: number): number {
  const r = Math.round(x);
  return Math.abs(x % 1) === 0.5 && r % 2 !== 0 ? r - 1 : r;
}

/** The wave row's tune the build makes of an XM sample's relative note and finetune (crashdata.audio.gax.xm_instrument_spec). */
export function xmTune(relativeNote: number, finetune: number): number {
  return roundEven(384 * Math.log2(XM_C4_RATE / PERIOD_0_HZ) + 32 * relativeNote + finetune / 4) - (XM_C4 - 2) * 32;
}

/** The relative note and finetune that play a sample at `rate` Hz on C-4 (XM's: 8363 Hz with neither). */
export function xmTuneForRate(rate: number): { relative_note: number; finetune: number } {
  const semis = 12 * Math.log2(rate / XM_C4_RATE);
  let relative_note = Math.round(semis);
  let finetune = Math.round((semis - relative_note) * 128);
  if (finetune > 127) [relative_note, finetune] = [relative_note + 1, finetune - 128];
  return { relative_note: Math.max(-128, Math.min(127, relative_note)), finetune: Math.max(-128, Math.min(127, finetune)) };
}

/** The vibrato speed the build makes of an XM auto-vibrato's rate (XM steps 256 phases a cycle, GAX2 64). */
export function xmVibratoSpeed(rate: number): number {
  return Math.max(1, Math.min(63, roundEven(rate / 4)));
}

/** Why an XM sample's fields can't be (null if they can), `length` samples long. */
export function xmSampleProblem(x: XmSample, length: number): string | null {
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
  if (!int(x.volume, 0, 64)) return "the volume is 0-64";
  if (!int(x.relative_note, -128, 127) || !int(x.finetune, -128, 127)) return "the relative note and finetune are -128 to 127";
  if (x.loop !== "none" && !(int(x.loop_start, 0, length - 1) && int(x.loop_end, x.loop_start + 1, length))) return `the loop ${x.loop_start}-${x.loop_end} isn't inside the sample (0-${length})`;
  return null;
}

/** Why an own XM instrument's fields can't be (null if it can): what crashdata.audio.edit.xm_with_own_instrument
 * and the build refuse. lengths: its samples' lengths. */
export function xmInstrumentProblem(x: XmSpec, lengths: number[]): string | null {
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
  for (const [i, smp] of x.samples.entries()) {
    const why = xmSampleProblem(smp, lengths[i] ?? 0);
    if (why) return x.samples.length > 1 ? `sample ${i}: ${why}` : why;
  }
  if (x.samples.length && (x.keymap.length !== XM_KEYS || x.keymap.some((k) => !int(k, 0, x.samples.length - 1)))) return `the key map plays samples 0-${x.samples.length - 1}`;
  const e = x.envelope;
  if (e) {
    if (e.points.length < 1 || e.points.length > XM_ENVELOPE_POINTS) return `an XM envelope has 1-${XM_ENVELOPE_POINTS} points`;
    if (e.points[0]![0] !== 0) return "the first point is at tick 0";
    for (let i = 1; i < e.points.length; i++) if (e.points[i]![0] <= e.points[i - 1]![0]) return `point ${i}'s tick isn't after point ${i - 1}'s`;
    if (e.points.some(([t, v]) => !int(t, 0, 0xffff) || !int(v, 0, 64))) return "a volume is 0-64, a tick 0-65535";
  }
  if (!int(x.vibrato.depth, 0, 255) || !int(x.vibrato.rate, 0, 255)) return "the vibrato's depth and rate are 0-255";
  return null;
}

/** What the build makes of an own XM instrument with its sample k (the one its key map plays on the song's notes:
 * crashdata.audio.gax.instrument_entry), as the panel shows it. */
export function xmInstrumentGax(x: XmSpec, k = 0): { tune: number; volume: number; envelope: number[] | null; vibrato: { depth: number; speed: number } } {
  const vib = x.vibrato.depth && x.vibrato.rate ? { depth: x.vibrato.depth, speed: xmVibratoSpeed(x.vibrato.rate) } : { depth: 0, speed: 0 };
  const smp = x.samples[k] ?? XM_NEW_SAMPLE;
  return { tune: xmTune(smp.relative_note, smp.finetune), volume: smp.volume * 4, envelope: x.envelope ? x.envelope.points.map(([, v]) => Math.min(255, v * 4)) : null, vibrato: vib };
}

/** A new own XM instrument's fields: blank (a name, no sample yet), or from a sound at `rate` Hz (C-4 plays it there). */
export function newXmSpec(name: string, rate: number | null): XmSpec {
  const samples = rate === null ? [] : [{ ...XM_NEW_SAMPLE, ...xmTuneForRate(rate) }];
  return { name: name.slice(0, XM_NAME), keymap: new Array<number>(XM_KEYS).fill(0), envelope: null, vibrato: { depth: 0, rate: 0 }, samples };
}

// -- instruments/<name>.json ------------------------------------------------------------------------

/** GAX2's limits on a JSON instrument's envelope (crashdata.audio.gax.MAX_ENVELOPE_POINTS) and its vibrato. */
export const JSON_ENVELOPE_POINTS = 255;

/** Why a JSON instrument can't be (null if it can), its WAV `length` samples long: crashdata.audio.gax.json_instrument_spec's checks. */
export function jsonInstrumentProblem(j: Instrument, length: number): string | null {
  const int = (v: unknown, lo: number, hi: number) => Number.isInteger(v) && (v as number) >= lo && (v as number) <= hi;
  if (j.rate !== undefined && !(typeof j.rate === "number" && j.rate > 0)) return "the rate is a number of Hz";
  if (j.volume !== undefined && !int(j.volume, 0, 64)) return "the volume is 0-64";
  const loop = j.loop ?? "none";
  if (loop !== "none") {
    const ls = j.loop_start ?? 0;
    const le = j.loop_end ?? length;
    if (!(int(ls, 0, length - 1) && int(le, ls + 1, length))) return `the loop ${ls}-${le} isn't inside the sample (0-${length})`;
  }
  const v = j.vibrato ?? {};
  if (!int(v.delay ?? 0, 0, 255) || !int(v.depth ?? 0, 0, 255) || !int(v.speed ?? 0, 0, 63)) return "the vibrato's delay and depth are 0-255, its speed 0-63";
  const e = j.envelope;
  if (e) {
    if (e.points.length < 1 || e.points.length > JSON_ENVELOPE_POINTS) return `an envelope has 1-${JSON_ENVELOPE_POINTS} points`;
    if (e.points[0]![0] !== 0) return "the first point is at tick 0";
    for (let i = 1; i < e.points.length; i++) if (e.points[i]![0] <= e.points[i - 1]![0]) return `point ${i}'s tick isn't after point ${i - 1}'s`;
    if (e.points.some(([t, y]) => !int(t, 0, 0xffff) || !int(y, 0, 64))) return "a volume is 0-64, a tick 0-65535";
    for (const f of ["sustain", "loop_start", "loop_end"] as const) if (e[f] !== undefined && !int(e[f], 0, e.points.length - 1)) return `the ${f.replace("_", " ")} is one of the points`;
  }
  return null;
}

/** The wave row's tune the build makes of a JSON instrument's rate for a song (C-4 plays it at `rate` Hz:
 * crashdata.audio.gax.tune_for_rate). */
export function rateTune(rate: number): number {
  return roundEven(384 * Math.log2(rate / PERIOD_0_HZ)) - (XM_C4 - 2) * 32;
}

// -- a song's settings ---------------------------------------------------------------------------

/** The mixer's data (struct GaxDspParams, 7 words of mixer_data_bytes): the channels mixed before the echo, then the
 * three taps' delay (ms) and gain (x/16); the third tap's gain isn't read (crashdata.audio.edit.echo_taps). */
export function echoTaps(bytes: number[]): number[] {
  const dv = new DataView(Uint8Array.from(bytes).buffer);
  return Array.from({ length: 7 }, (_, i) => dv.getUint32(i * 4, true));
}

export function withEchoTaps(taps: number[]): number[] {
  const dv = new DataView(new ArrayBuffer(28));
  taps.forEach((v, i) => dv.setUint32(i * 4, v >>> 0, true));
  return [...new Uint8Array(dv.buffer)];
}

/** GAX2_estimate's work RAM for a song StartSong plays (crashdata.audio.gax.work_size). */
export function workSize(channels: number, sampleRate: number, halfRateFx: number, mixer: number[], mixRates: number[]): number {
  const fx = 3;
  const rate = mixRates.find((r) => r >= sampleRate) ?? mixRates[mixRates.length - 1]!;
  let size = 0x18c + (3 + channels + fx) * 4 + 8;
  size += 0xc + 0xc + channels * 4 + fx * 4;
  size += 0xc + 0x1c;
  size += channels * (0xc + 0x48 + 4);
  size += fx * 0x58;
  const taps = echoTaps(mixer);
  const longest = Math.max(taps[1]!, taps[3]!, taps[5]!);
  if (longest) size += Math.floor((longest * rate) / 1000) * 2 + 0x18;
  size += halfRateFx ? 0x130 : 0xdc;
  return size + Math.floor((rate * 1000) / 0xe94f) * 4 + 0x20;
}

/** Why GAX2 or the game can't play a song with these settings (crashdata.audio.edit.check_settings), or null. */
export function settingsProblem(st: SongSettings, channels: number, v: MusicView): string | null {
  if (!v.mixRates.includes(st.sample_rate)) return `GAX2 mixes at ${v.mixRates.join(", ")} Hz`;
  if (!Number.isInteger(st.volume) || st.volume < 0 || st.volume > 0xffff) return "the volume is 0-65535 (256 is 1.0)";
  const taps = echoTaps(st.mixer_data_bytes);
  if (taps[0]! > channels) return `${taps[0]} channels mixed before the echo, but the song has ${channels}`;
  if (taps[2]! > v.limits.maxEchoGain || taps[4]! > v.limits.maxEchoGain) return `an echo tap's gain is 0-${v.limits.maxEchoGain}`;
  const need = workSize(channels, st.sample_rate, st.half_rate_fx, st.mixer_data_bytes, v.mixRates);
  if (need > v.limits.workSize) return `GAX2 needs 0x${need.toString(16)} bytes of work RAM, StartSong gives it 0x${v.limits.workSize.toString(16)}: a lower mix rate or shorter echo delays`;
  return null;
}

/** A one-channel song that plays instrument `ins` of the music pool: `note` (XM's: 49 is C-4) on its first row,
 * a key off after `hold` rows, `rows` rows at speed 6 (GAX2's packed rows: a note, rests, the key off). */
export function instrumentSong(ins: number, note: number, hold: number, rows = 64, mixRate = 15769): MusicSong {
  const rest = (n: number): number[] => (n <= 0 ? [] : n === 1 ? [0x80] : [0xff, n]);
  const bytes = [0x00, 0x80 | (note & 0x7f), ins, ...rest(hold - 1), 0x81, 0x00, ...rest(rows - hold - 1)];
  return {
    key: `instrument_${ins}`,
    id: -1,
    channelCount: 1,
    patternRows: rows,
    orderCount: 1,
    loopOrder: 0,
    volume: 256,
    mixRate,
    numSfx: 3,
    halfRateFx: 1,
    dsp: [0, 0, 0, 0, 0, 0, 0],
    patterns: toBase64(Uint8Array.from(bytes)),
    orders: [[[0, 0]]],
  };
}

// -- the sound effects ------------------------------------------------------------------

export type SfxEntry = SfxTable["entries"][number];

/** The volume PlaySfx gives the voice: Q16_TO_INT(baseVolume * sfxVolume (0x100) * 0x100), clamped by GAX_set_fx_volume. */
export function sfxVoiceVolume(e: SfxEntry): number {
  return Math.max(0, Math.min(0xff, e.volume));
}

export const SFX_NAME = /^[A-Z0-9_]+$/;
export const KEY = /^[a-z0-9_]+$/;

/** A name no entry has: NEW_SOUND, NEW_SOUND_2, ... */
export function freeName(entries: SfxEntry[], base = "NEW_SOUND"): string {
  const names = new Set(entries.map((e) => e.name));
  if (!names.has(base)) return base;
  for (let i = 2; ; i++) if (!names.has(`${base}_${i}`)) return `${base}_${i}`;
}

/** Why the last entry can't be deleted (a sound effect's id is its place), or null. */
export function cannotDeleteLast(v: MusicView, s: EditState): string | null {
  const i = s.sfx.entries.length - 1;
  const e = s.sfx.entries[i];
  if (!e) return "there is none";
  const old = v.sfx[i];
  if (old && old.name === e.name && old.uses.length) return `SFX_${e.name} is used by ${old.uses.slice(0, 3).join(", ")}`;
  return null;
}

// GAX2's period table's first entry, Hz (gGaxPeriodTable[0] / 2048)
export const PERIOD_0_HZ = 0x105a02 / 2048;

/** The wave row's tune that plays a fixed-pitch sample at `rate` Hz (crashdata.audio.gax.tune_for_rate). */
export function tuneForRate(rate: number): number {
  return Math.round(384 * Math.log2(rate / PERIOD_0_HZ));
}

/** A new sound effect's instrument as the build makes it from instruments/<name>.json (gax.instrument_entry,
 * fixed pitch, no envelope): what the player plays before it is saved. */
export function sfxInstrument(wave: number, rate: number, volume = 64): MusicInstrument {
  const row = { sweep: 0, pingPong: 0, start: 0, sweepMin: 0, sweepMax: 0, sweepLen: 0, sweepStep: 0, sweepRate: 0, tune: 0 };
  return {
    empty: 0,
    waveIdx: [wave, 0, 0, 0],
    vibratoDelay: 0,
    vibratoDepth: 0,
    vibratoSpeed: 0,
    rows: [{ ...row, tune: tuneForRate(rate) }, { ...row }, { ...row }, { ...row }],
    envelope: { sustain: 0xff, loopStart: 0xff, loopEnd: 0xff, points: [{ pos: 0, slope: 0, value: 255 }] },
    seqSpeed: 1,
    seq: [{ note: 2, fixedPitch: 1, wave: 1, fx: [volume < 64 ? (12 << 8) | (volume * 4) : 0, 0] }],
  };
}

// -- WAVs and base64 -------------------------------------------------------------------------

export function fromBase64(b64: string): Uint8Array {
  const s = atob(b64);
  const out = new Uint8Array(s.length);
  for (let i = 0; i < s.length; i++) out[i] = s.charCodeAt(i);
  return out;
}

export function toBase64(bytes: Uint8Array): string {
  let s = "";
  for (let i = 0; i < bytes.length; i += 0x8000) s += String.fromCharCode(...bytes.subarray(i, i + 0x8000));
  return btoa(s);
}

/** An 8-bit mono WAV's samples (signed) and rate, as the tree's samples are. */
export function parseWav8(buf: ArrayBuffer | Uint8Array): { rate: number; data: Int8Array } {
  const b = buf instanceof Uint8Array ? buf : new Uint8Array(buf);
  const dv = new DataView(b.buffer, b.byteOffset, b.byteLength);
  const tag = (o: number) => String.fromCharCode(b[o]!, b[o + 1]!, b[o + 2]!, b[o + 3]!);
  if (tag(0) !== "RIFF" || tag(8) !== "WAVE") throw new Error("not a WAV file");
  let pos = 12;
  let rate = 0;
  let bits = 0;
  let channels = 0;
  while (pos + 8 <= b.length) {
    const id = tag(pos);
    const size = dv.getUint32(pos + 4, true);
    if (id === "fmt ") {
      channels = dv.getUint16(pos + 10, true);
      rate = dv.getUint32(pos + 12, true);
      bits = dv.getUint16(pos + 22, true);
    } else if (id === "data") {
      if (bits !== 8 || channels !== 1) throw new Error(`a ${bits}-bit WAV of ${channels} channels: a sample is 8-bit mono`);
      const data = new Int8Array(size);
      for (let i = 0; i < size; i++) data[i] = b[pos + 8 + i]! - 128;
      return { rate, data };
    }
    pos += 8 + size + (size & 1);
  }
  throw new Error("the WAV has no data");
}

/** An 8-bit mono WAV of signed samples (the PC port's --wav writes the same). */
export function wavFile(samples: Int8Array, rate: number): Uint8Array<ArrayBuffer> {
  const out = new Uint8Array(new ArrayBuffer(44 + samples.length));
  const dv = new DataView(out.buffer);
  const put = (o: number, s: string) => [...s].forEach((ch, i) => (out[o + i] = ch.charCodeAt(0)));
  put(0, "RIFF");
  dv.setUint32(4, 36 + samples.length, true);
  put(8, "WAVEfmt ");
  dv.setUint32(16, 16, true);
  dv.setUint16(20, 1, true);
  dv.setUint16(22, 1, true);
  dv.setUint32(24, rate, true);
  dv.setUint32(28, rate, true);
  dv.setUint16(32, 1, true);
  dv.setUint16(34, 8, true);
  put(36, "data");
  dv.setUint32(40, samples.length, true);
  for (let i = 0; i < samples.length; i++) out[44 + i] = (samples[i]! + 128) & 0xff;
  return out;
}

/** m:ss.s of a number of frames at the GBA's 59.73 frames a second. */
export function frameTime(frames: number): string {
  const s = (frames * 1000) / 0xe94f;
  const m = Math.floor(s / 60);
  return `${m}:${(s - m * 60).toFixed(1).padStart(4, "0")}`;
}

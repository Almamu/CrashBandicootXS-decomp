// The music editor's model (docs/editors.md, "The music editor"): the
// edit state (the tables, the songs' cell edits, the samples changed),
// what a save sends, the XM cells as a tracker shows them and what the
// build makes of them (crashdata.audio.gax's XM import), a ROM song's
// shared patterns, and the sound effects' rules. No DOM: the tests run it
// in node.
import type { Music, MusicCellEdit, MusicInstrument, MusicPatterns, MusicSave, MusicView, SfxTable } from "@crash/schema-types";

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
}

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

/** What the build makes of a cell's effect and volume column: GAX2's one
 * effect of the row (crashdata.audio.gax._xm_effect_to_gax), [cmd, param]. */
export function gaxEffect(effect: number, param: number, volume: number): [number, number] {
  if (effect === 1 || effect === 2 || effect === 3) return [effect, param];
  if (effect === 12) return [12, Math.min(255, param * 4)];
  if (effect === 13 || effect === 14) return [effect, param];
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
  if (eff || param) {
    const kept = [1, 2, 3, 12, 13, 14].includes(eff) || (eff === 15 && param < 0x20);
    if (!kept) dropped.push(eff === 15 ? `F${hex2(param)}: a BPM (0x20 or more); GAX2 steps a tick a frame` : `effect ${EFFECTS[eff]}${hex2(param)}: GAX2 has no such effect`);
  }
  if (vol) {
    if (gaxEffect(eff, param, 0)[0] !== 0) dropped.push(`the volume column ${hex2(vol)}: a GAX2 row has one effect, the effect column's`);
    else if (gaxEffect(0, 0, vol)[0] === 0) dropped.push(`the volume column ${hex2(vol)}: not one GAX2 has (10-50, 80-9F)`);
  }
  const parts: string[] = [];
  if (note === KEY_OFF) parts.push("key off");
  else if (note >= 1 && note <= 96) parts.push(noteName(note));
  if (cmd) parts.push(`${GAX_EFFECTS[cmd] ?? `effect ${cmd}`} ${hex2(p)}`);
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

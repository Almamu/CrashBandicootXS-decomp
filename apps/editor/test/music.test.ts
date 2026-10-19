// The music editor's logic (docs/editors.md, "The music editor"): its GAX2
// player against the PC port's (gax_reference.json, which tools/gax_render.py
// renders with the PC port: every song's first 1200 frames and three
// sound-effect sequences, sample for sample), on the data crashdata gives
// the backend; the XM cells as a tracker shows them and what the build
// makes of them; the shared ROM patterns' edits as the save makes them;
// what a save sends; a new sound effect's instrument as the build makes it.
import { createHash } from "node:crypto";
import { execFileSync } from "node:child_process";
import { readFileSync } from "node:fs";
import { delimiter, join } from "node:path";
import { describe, expect, it } from "vitest";
import type { MusicInstrument, MusicPatterns, MusicPool, MusicSong, MusicStructure, MusicView, MusicXmInstrument } from "@crash/schema-types";
import { GaxPlayer, decodePool } from "../src/lib/music/gax.ts";
import * as M from "../src/lib/music/model.ts";

const ROOT = join(__dirname, "..", "..", "..");

/** crashdata.audio.edit, as the backend runs it. */
function python<T>(code: string): T {
  const out = execFileSync("python3", ["-c", `import json, sys\nfrom crashdata.audio import edit as E\nfrom crashdata.audio import gax as G\nfrom crashdata.common.game import Game\ng = Game("xs", ".")\n${code}`], {
    cwd: ROOT,
    env: { ...process.env, PYTHONPATH: [join(ROOT, "packages", "crashdata", "src"), join(ROOT, "packages", "buildtools", "src")].join(delimiter) },
    maxBuffer: 256 * 1024 * 1024,
    encoding: "utf8",
  });
  return JSON.parse(out) as T;
}

interface Reference {
  frames: number;
  songs: Record<string, string>;
  sfx: { song: string; musicVolume?: number; frames: number; events: [number, number, number, number, number][]; sha1: string }[];
  edited?: { songs: Record<string, string>; sfx?: { song: string; musicVolume?: number; frames: number; events: [number, number, number, number, number][]; sha1: string }[] };
}

const ref = JSON.parse(readFileSync(join(__dirname, "gax_reference.json"), "utf8")) as Reference;
const data = python<{ pool: MusicPool; songs: Record<string, MusicSong> }>(
  `print(json.dumps({"pool": E.player_pool(g), "songs": {k: E.player_song(g, k) for k in ${JSON.stringify(Object.keys(ref.songs))}}}))`,
);

function player(key: string): GaxPlayer {
  return new GaxPlayer(data.pool.tables, decodePool(data.pool.music), decodePool(data.pool.sfx), data.songs[key]!);
}

const sha1 = (b: Int8Array) => createHash("sha1").update(new Uint8Array(b.buffer, b.byteOffset, b.byteLength)).digest("hex");

describe("the player", () => {
  it("mixes every song as the PC port's GAX2 does, sample for sample", () => {
    for (const [key, digest] of Object.entries(ref.songs)) {
      const p = player(key);
      expect([key, sha1(p.render(ref.frames))]).toEqual([key, digest]);
    }
  });

  it("mixes sound effects as PlaySfx plays them (GAX_fx_ex, GAX_set_fx_volume), over a song", () => {
    for (const s of ref.sfx) {
      const p = player(s.song);
      if (s.musicVolume !== undefined) p.setMusicVolume(-1, s.musicVolume);
      const out = new Int8Array(s.frames * p.frames);
      for (let f = 0; f < s.frames; f++) {
        for (const [at, slot, voice, prio, vol] of s.events) {
          if (at !== f) continue;
          p.fx(slot, voice, prio, -1);
          p.setFxVolume(voice, vol);
        }
        p.frame(out.subarray(f * p.frames, (f + 1) * p.frames));
      }
      expect([s.song, sha1(out)]).toEqual([s.song, s.sha1]);
    }
  });

  it("mixes the edited songs, played before they are saved, as the PC port built from the saved edits", () => {
    // tools/gax_render.py edited_body: a ROM song restructured, cells with alternating speeds and GAX2's own
    // slides, a channel more, ...; the backend builds them unsaved (POST music/song, music/pool with the edits)
    const songs = Object.keys(ref.edited!.songs);
    const edited = python<{ pool: MusicPool; songs: Record<string, MusicSong> }>(
      `from buildtools import gax_render as R\nb = R.edited_body(".")\nprint(json.dumps({"pool": E.player_pool(g, b), "songs": {k: E.player_song(g, k, unsaved=b) for k in ${JSON.stringify(songs)}}}))`,
    );
    for (const [key, digest] of Object.entries(ref.edited!.songs)) {
      const p = new GaxPlayer(edited.pool.tables, decodePool(edited.pool.music), decodePool(edited.pool.sfx), edited.songs[key]!);
      const got = sha1(p.render(ref.frames));
      expect([key, got]).toEqual([key, digest]);
      // and they are edited: not the song as it was
      expect(got).not.toBe(ref.songs[key]);
    }
    // the edited sound-effect instrument, over jungle and over warp_room with its edited settings
    for (const s of ref.edited!.sfx ?? []) {
      const p = new GaxPlayer(edited.pool.tables, decodePool(edited.pool.music), decodePool(edited.pool.sfx), edited.songs[s.song] ?? data.songs[s.song]!);
      if (s.musicVolume !== undefined) p.setMusicVolume(-1, s.musicVolume);
      const out = new Int8Array(s.frames * p.frames);
      for (let f = 0; f < s.frames; f++) {
        for (const [at, slot, voice, prio, vol] of s.events) {
          if (at !== f) continue;
          p.fx(slot, voice, prio, -1);
          p.setFxVolume(voice, vol);
        }
        p.frame(out.subarray(f * p.frames, (f + 1) * p.frames));
      }
      expect([s.song, sha1(out)]).toEqual([s.song, s.sha1]);
    }
    // "Play the note": the one-channel song the editor makes for a new pool instrument, as the PC port plays the
    // song of that note the backend built (tools/gax_render.py note_xm)
    const note = python<{ n: number; note: number; hold: number; rows: number }>(
      `from buildtools import gax_render as R\nb = R.edited_body(".")\nprint(json.dumps({"n": b["songInstruments"]["cutscenes"][0], "note": R.NOTE, "hold": R.NOTE_HOLD, "rows": R.NOTE_ROWS}))`,
    );
    const p = new GaxPlayer(edited.pool.tables, decodePool(edited.pool.music), decodePool(edited.pool.sfx), M.instrumentSong(note.n, note.note, note.hold, note.rows));
    const out = p.render(ref.frames);
    expect(out.some((v) => v !== 0)).toBe(true);
    expect(sha1(out)).toBe(ref.edited!.songs.instrument_note);
    // and on a new song's own XM instrument, edited (tools/gax_render.py own_note_xm): the pool instrument the build
    // makes of it (the pool's xmPool)
    const own = edited.pool.xmPool.own_note![0]!;
    const q = new GaxPlayer(edited.pool.tables, decodePool(edited.pool.music), decodePool(edited.pool.sfx), M.instrumentSong(own, note.note, note.hold, note.rows));
    expect(sha1(q.render(ref.frames))).toBe(ref.edited!.songs.own_note);
  });

  it("follows the song's position: order, row, speed, loop", () => {
    const p = player("drums");
    const song = data.songs.drums!;
    p.frame();
    // its first row sets speed 3 (F03): GAX2 starts every song at 6
    expect([p.info.orderPos, p.info.row, p.info.speed]).toEqual([0, 0, 3]);
    let frames = 1;
    while (!p.info.songEnded) {
      p.frame();
      frames++;
    }
    // the last order played to its end, then back to the loop point
    expect(p.info.orderPos).toBe(song.loopOrder);
    expect(frames).toBeGreaterThan(song.orderCount * song.patternRows);
  });
});

describe("the cells", () => {
  it("are written as a tracker writes them", () => {
    expect([M.noteName(0), M.noteName(1), M.noteName(49), M.noteName(96), M.noteName(97), M.noteName(243)]).toEqual(["---", "C-0", "C-4", "B-7", "===", "?F3"]);
    for (let n = 0; n <= 97; n++) expect(M.parseNote(M.noteName(n))).toBe(n);
    expect([M.parseNote("c#4"), M.parseNote(""), M.parseNote("H-4")]).toEqual([50, 0, null]);
    expect(M.cellText([49, 3, 0x40, 0xf, 0x07])).toEqual({ note: "C-4", ins: "03", vol: "40", fx: "F07" });
    expect(M.cellText([0, 0, 0, 0, 0])).toEqual({ note: "---", ins: "..", vol: "..", fx: "..." });
  });

  it("are what the build makes of them (crashdata.audio.gax's XM import)", () => {
    const cases: [number, number, number][] = [
      [1, 0x20, 0], [3, 0x08, 0x40], [0xc, 0x40, 0], [0xc, 0x50, 0], [0xd, 0, 0], [0xe, 0xd3, 0], [0xf, 0x07, 0], [0xf, 0x80, 0x30],
      [0, 0, 0x30], [0, 0, 0x95], [0, 0, 0x83], [0, 0, 0x60], [4, 0x44, 0], [0xa, 0x0f, 0x20],
      // the ROM's: alternating speeds, volume slides, volumes over C40, GAX2's own command (volume column 01)
      [7, 0x96, 0], [0xa, 0x20, 0x92], [0xa, 0x01, 0x81], [0xa, 0x11, 0], [0xc, 0x62, 0], [0xb, 0xff, 1], [0xa, 0x40, 1],
    ];
    const py = python<[number, number][]>(`print(json.dumps([G._xm_effect_to_gax(e, p, v) for e, p, v in ${JSON.stringify(cases)}]))`);
    expect(cases.map(([e, p, v]) => M.gaxEffect(e, p, v))).toEqual(py);
    expect(M.cellNotes([49, 1, 0x30, 0xf, 0x80]).dropped.length).toBe(1); // a BPM: the volume column plays instead
    expect(M.cellNotes([49, 1, 0x30, 0xf, 0x80]).plays).toBe("C-4, set the volume 80");
    expect(M.cellNotes([49, 1, 0x30, 0xf, 0x06]).dropped[0]).toMatch(/one effect/);
    expect(M.cellNotes([0, 0, 0, 4, 0x44]).dropped[0]).toMatch(/no such effect/);
    expect(M.cellNotes([0, 0, 0, 7, 0x96]).plays).toBe("alternating speeds 96 (9 ticks, then 6)");
    expect(M.cellNotes([0, 0, 1, 0xb, 0xff])).toEqual({ plays: "GAX2's own volume slide down FF", dropped: [] });
  });

  it("are edited in every pattern a ROM pattern shares its bytes with, as the save writes them", () => {
    const edit = { pattern: 0, row: 3, channel: 0, cell: [50, 1, 0x30, 0, 0] as M.Cell };
    const got = python<{ p: MusicPatterns; after: Record<string, string> }>(
      `p = E.patterns(g, "jungle")\nfiles = E.Files(g)\nm = json.load(open(files.manifest))\nxm, _, changed = E.edited_song(files, m, "jungle", [${JSON.stringify(edit)}])\nlay = E.XmLayout(xm)\nimport base64\nprint(json.dumps({"p": p, "after": {i: base64.b64encode(bytes(E.unpack_cells(xm, lay, i))).decode() for i in changed}}))`,
    );
    const base = M.decodeCells(got.p);
    const mine = M.editedCells(got.p, base, { [M.cellKey(edit.pattern, edit.row, edit.channel)]: edit.cell });
    const changed = mine.map((c, i) => (c !== base[i] ? i : -1)).filter((i) => i >= 0);
    expect(changed.map(String).sort()).toEqual(Object.keys(got.after).sort());
    for (const [i, b64] of Object.entries(got.after)) expect(Buffer.from(mine[Number(i)]!).toString("base64")).toBe(b64);
    expect(changed.length).toBeGreaterThan(1);
    // an edit that changes nothing changes nothing
    const same = M.cellAt(base, got.p.channels, 0, 0, 0);
    expect(M.editedCells(got.p, base, { [M.cellKey(0, 0, 0)]: same }).every((c, i) => c === base[i] || Buffer.from(c).equals(Buffer.from(base[i]!)))).toBe(true);
  });
});

describe("a song's structure", () => {
  const drums = python<{ p: MusicPatterns; st: MusicStructure }>(`buf = open("games/xs/sound/songs/drums.xm", "rb").read()\nprint(json.dumps({"p": E.patterns(g, "drums"), "st": E.structure(buf)}))`);
  const base = M.structureOf(drums.p, M.decodeCells(drums.p));

  it("is the XM's (crashdata.audio.edit.structure)", () => {
    expect(M.structureBody(base)).toEqual(drums.st);
  });

  it("is edited and undone", () => {
    const ch = M.removeChannel(M.addChannel(base, 1), 1);
    expect(M.sameStructure(ch, base)).toBe(true);
    expect(M.addChannel(base).channels).toBe(base.channels + 1);
    const rows = M.deleteRow(M.insertRow(base, 1, 4), 1, 4);
    expect(M.sameStructure(rows, base)).toBe(true);
    expect(M.insertRow(base, 1, 4).patterns[1]!.rows).toBe(base.patterns[1]!.rows + 1);
    expect(M.setRows(base, 1, 8).patterns[1]!.cells.length).toBe(8 * base.channels * M.CELL);
    const ord = M.removeOrder(M.insertOrder(base, 1, 0), 1);
    expect(M.sameStructure(ord, base)).toBe(true);
    // the loop point stays on the position it marks
    expect(M.insertOrder(base, 0, 0).restart).toBe(base.restart + 1);
    const moved = M.moveOrder(base, base.restart, 0);
    expect([moved.restart, moved.order[0]]).toEqual([0, base.order[base.restart]]);
  });

  it("loses a pattern with the positions that play it, the later ones renumbered", () => {
    const s = M.duplicatePattern(base, 2);
    const n = s.patterns.length - 1;
    const t = M.insertOrder(s, 0, n);
    const d = M.deletePattern(t, 0);
    expect(d.patterns.length).toBe(t.patterns.length - 1);
    expect(d.order).toEqual(t.order.filter((p) => p !== 0).map((p) => (p > 0 ? p - 1 : p)));
    expect(() => M.deletePattern({ ...base, patterns: [base.patterns[0]!] }, 0)).toThrow();
    expect(() => M.setSpeed(base, 0)).toThrow();
  });

  it("is what a save and the player send", () => {
    const st = M.setSpeed(base, 4);
    const s0: M.EditState = { ...M.initialState({ themes: [], songs: [] } as unknown as MusicView, { entries: [] } as never, { screen_music: [] }) };
    const s1 = { ...s0, structures: { drums: st } };
    expect(M.saveBody(s0, s1).structures?.drums?.speed).toBe(4);
    expect(M.saveBody(s1, s1).structures).toBeUndefined();
    expect(M.unsavedBody(s1).structures?.drums).toEqual(M.structureBody(st));
  });
});

describe("the music pool's instruments", () => {
  const view = python<MusicView>(`print(json.dumps(E.view(g)))`);
  const rec = view.poolInstruments.find((i) => i.index === 7)!.record;

  it("are shown as a tracker shows them: tune, loop", () => {
    expect(M.tuneParts(1860)).toEqual({ note: 58, fine: 4 });
    expect(M.tuneParts(-33)).toEqual({ note: -2, fine: 31 });
    expect(M.loopKind(rec.samples[0]!)).toBe("pingpong");
    const f = M.withLoop(rec.samples[0]!, "forward", 10, 20);
    expect([M.loopKind(f), f.loop_start, f.loop_end, M.waveRowNeeds(f)]).toEqual(["forward", 10, 20, 20]);
    expect(M.loopKind(M.withLoop(f, "none", 0, 0))).toBe("none");
  });

  it("keep their envelope's recorded slopes where the points are (the backend gives the others theirs)", () => {
    const pts = M.envPoints(rec);
    const same = M.withEnvelope(rec, pts, null, null, null);
    expect(same.envelope).toEqual(rec.envelope);
    const moved = M.withEnvelope(rec, pts.map((p, i) => (i === 1 ? { ...p, y: 100 } : p)), 1, null, null);
    expect(moved.envelope!.sustain).toBe(1);
    expect(M.envelopeProblem([{ x: 0, y: 0 }, { x: 0, y: 3 }])).toMatch(/after/);
    // the backend's record of the same edit plays: its slopes are crashdata.audio.pool's
    const back = python<{ envelope: { points: { interpolation: number }[] } }>(
      `from crashdata.audio import pool as P\nm = json.load(open(E.Files(g).manifest))\nr = json.loads(${JSON.stringify(JSON.stringify(moved))})\nprint(json.dumps(P.edited_instrument(m["instruments"]["7"], r, {6: 20000}, "7")))`,
    );
    // (255 at 0, 100 at 14, 0 at 60: Q8 a tick, rounded toward 0)
    expect(back.envelope.points.map((p) => p.interpolation)).toEqual([0, -Math.floor((155 * 256) / 14), -Math.floor((100 * 256) / 46)]);
  });

  it("play a note as a song plays it", () => {
    const song = M.instrumentSong(7, 49, 4, 8);
    expect(Array.from(M.fromBase64(song.patterns))).toEqual([0x00, 0x80 | 49, 7, 0xff, 3, 0x81, 0x00, 0xff, 3]);
    const p = new GaxPlayer(data.pool.tables, decodePool(data.pool.music), decodePool(data.pool.sfx), song);
    const out = p.render(60);
    expect(out.some((v) => v !== 0)).toBe(true);
  });
});

describe("new pool instruments, a song's settings", () => {
  const view = python<MusicView>(`print(json.dumps(E.view(g)))`);
  const sfx = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/sfx_table.json"), "utf8"));
  const music = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/music.json"), "utf8"));
  const s0 = M.initialState(view, sfx, music);
  const top = Math.max(...view.poolInstruments.map((i) => i.index));

  it("number new instruments after the pool's; one taken out is emptied, the last ones leave", () => {
    expect(M.nextInstrument(view, s0)).toBe(top + 1);
    const copy = M.newInstrumentRecord(view, { copy: view.poolInstruments[7]!.record });
    const fromSample = M.newInstrumentRecord(view, { sample: 6 });
    expect(fromSample.sample_indices).toEqual([6, 0, 0, 0]);
    const s1 = { ...s0, instruments: { [String(top + 1)]: copy, [String(top + 2)]: fromSample } };
    expect(M.nextInstrument(view, s1)).toBe(top + 3);
    const list = M.poolInstruments(view, { ...s1, deleteInstruments: [26] });
    expect(list.map((i) => i.index).slice(-3)).toEqual([top, top + 1, top + 2]);
    expect(list.find((i) => i.index === 26)!.record).toEqual(view.instrumentTemplates.empty);
    // the pool's last taken out: it leaves the pool
    expect(M.poolInstruments(view, { ...s0, deleteInstruments: [top] }).length).toBe(top);
    // what the backend makes of the same edits
    const py = python<{ len: number; empty: number }>(
      `b = json.loads(${JSON.stringify(JSON.stringify({ instruments: s1.instruments, deleteInstruments: [26] }))})\np = E.player_pool(g, b)\nprint(json.dumps({"len": len(p["music"]["instruments"]), "empty": p["music"]["instruments"][26]["empty"]}))`,
    );
    expect(py).toEqual({ len: top + 3, empty: 1 });
    const b = M.saveBody(s0, { ...s1, deleteInstruments: [26], songInstruments: { intro: [top + 1] } });
    expect([Object.keys(b.instruments ?? {}), b.deleteInstruments, b.songInstruments]).toEqual([[String(top + 1), String(top + 2)], [26], { intro: [top + 1] }]);
  });

  it("add a pool instrument to a song's XM instruments", () => {
    const p = python<MusicPatterns>(`print(json.dumps(E.patterns(g, "intro")))`);
    const list = M.songInstruments(p, [top]);
    expect(list.slice(-1)).toEqual([{ name: `Instrument ${top}`, pool: top, added: true }]);
    expect(list.length).toBe(p.instruments.length + 1);
  });

  it("check a song's settings as the backend does: GAX2's mix rates, its work RAM, the echo", () => {
    const jungle = view.songs.find((s) => s.key === "jungle")!;
    const st: M.SongSettings = { sample_rate: jungle.settings!.sample_rate, volume: jungle.settings!.volume, half_rate_fx: jungle.settings!.half_rate_fx, mixer_data_bytes: jungle.settings!.mixer_data_bytes };
    expect(M.settingsProblem(st, 4, view)).toBeNull();
    expect(M.workSize(4, st.sample_rate, st.half_rate_fx, st.mixer_data_bytes, view.mixRates)).toBe(jungle.workSize);
    const taps = [3, 40, 10, 15, 6, 0, 0];
    expect(M.echoTaps(M.withEchoTaps(taps))).toEqual(taps);
    const cases: [number, number, number, number[]][] = [
      [4, 18158, 1, taps],
      [7, 42049, 0, [4, 120, 9, 0, 0, 0, 0]],
      [16, 42049, 1, [0, 100, 8, 0, 0, 0, 0]],
      [2, 5735, 0, [0, 0, 0, 0, 0, 0, 0]],
    ];
    const py = python<number[]>(`import struct\nprint(json.dumps([G.work_size(c, r, h, list(struct.pack("<7I", *t))) for c, r, h, t in ${JSON.stringify(cases)}]))`);
    expect(cases.map(([c, r, h, t]) => M.workSize(c, r, h, M.withEchoTaps(t), view.mixRates))).toEqual(py);
    expect(M.settingsProblem({ ...st, sample_rate: 22050 }, 4, view)).toMatch(/mixes at/);
    expect(M.settingsProblem({ ...st, mixer_data_bytes: M.withEchoTaps([5, 10, 8, 0, 0, 0, 0]) }, 4, view)).toMatch(/channels/);
    expect(M.settingsProblem({ ...st, sample_rate: 42049, mixer_data_bytes: M.withEchoTaps([4, 500, 8, 0, 0, 0, 0]) }, 4, view)).toMatch(/work RAM/);
    const b = M.saveBody(s0, { ...s0, songSettings: { jungle: { ...st, volume: 300 } } });
    expect(b.songSettings).toEqual({ jungle: { ...st, volume: 300 } });
  });
});

describe("the sound-effect set's new instruments, a song's own instruments", () => {
  const view = python<MusicView>(`print(json.dumps(E.view(g)))`);
  const sfx = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/sfx_table.json"), "utf8"));
  const music = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/music.json"), "utf8"));
  const s0 = M.initialState(view, sfx, music);
  const top = Math.max(...view.sfxInstruments.map((i) => i.index));

  it("number the set's new instruments after its own; one taken out is emptied, the last ones leave", () => {
    expect(M.nextSfxInstrument(view, s0)).toBe(top + 1);
    const blank = M.newSfxInstrumentRecord(view, { sample: 40 });
    // a blank one is as the ROM's are: instrument 40 plays sample 40 so
    expect(blank).toEqual(view.sfxInstruments.find((i) => i.index === 40)!.record);
    expect(M.fixedRate(data.pool.tables.period, blank)).toBeCloseTo(view.sfxRate, 1);
    const s1 = { ...s0, sfxInstruments: { [String(top + 1)]: blank, [String(top + 2)]: M.newSfxInstrumentRecord(view, null) } };
    expect(M.nextSfxInstrument(view, s1)).toBe(top + 3);
    const list = M.sfxInstruments(view, { ...s1, deleteSfxInstruments: [19] });
    expect(list.map((i) => [i.index, i.added]).slice(-2)).toEqual([
      [top + 1, true],
      [top + 2, true],
    ]);
    expect(list.find((i) => i.index === 19)!.record).toEqual(view.instrumentTemplates.empty);
    expect(M.sfxInstruments(view, { ...s0, deleteSfxInstruments: [top] }).length).toBe(top);
    const wav = { wav: "UklGRg==", length: 4, rate: 8003 };
    const b = M.saveBody(s0, { ...s1, newSfxSamples: { "88": wav }, deleteSfxSamples: [19], deleteSfxInstruments: [19] });
    expect([Object.keys(b.sfxInstruments ?? {}), b.newSfxSamples, b.deleteSfxSamples, b.deleteSfxInstruments]).toEqual([[String(top + 1), String(top + 2)], { "88": "UklGRg==" }, [19], [19]]);
    expect(M.unsavedBody({ ...s1, newSfxSamples: { "88": wav } }).newSfxSamples).toEqual({ "88": "UklGRg==" });
  });

  it("tune an own XM instrument as the build does", () => {
    const cases = [
      [0, 0],
      [12, -20],
      [-7, 127],
      [6, 32],
      [-128, -128],
      [3, 2],
    ];
    const py = python<number[]>(`print(json.dumps([round(384 * __import__("math").log2(G.XM_C4_RATE / G.PERIOD_0_HZ) + 32 * r + f / 4) - (G.XM_C4 - 2) * 32 for r, f in ${JSON.stringify(cases)}]))`);
    expect(cases.map(([r, f]) => M.xmTune(r!, f!))).toEqual(py);
    const rates = [0, 1, 2, 6, 10, 18, 22, 255];
    const pyRates = python<number[]>(`print(json.dumps([max(1, min(63, round(r / 4))) for r in ${JSON.stringify(rates)}]))`);
    expect(rates.map(M.xmVibratoSpeed)).toEqual(pyRates);
    // C-4 at 12000 Hz: 6 semitones and 32/128 up, within a cent
    const t = M.xmTuneForRate(12000);
    expect(t).toEqual({ relative_note: 6, finetune: 32 });
    expect(8363 * 2 ** ((t.relative_note + t.finetune / 128) / 12)).toBeCloseTo(12000, -1);
  });

  it("show what the build makes of an own XM instrument, and refuse what it refuses", () => {
    // tools/gax_render.py edited_body's own_note, its instrument as the backend builds it unsaved
    const r = python<{ spec: M.XmSpec; ins: MusicInstrument }>(
      `from buildtools import gax_render as R\nb = R.edited_body(".")\np = E.player_pool(g, b)\nxm = G.read_xm("own.xm", R.own_note_xm())\nspec = E.own_instruments(E.Files(g), xm)[1]["spec"]\nedit = b["xmInstruments"]["own_note"]["1"]\nspec.update({k: v for k, v in edit.items() if k != "samples"})\nspec["samples"] = [{k: v for k, v in s.items() if k != "wav"} for s in edit["samples"]]\nprint(json.dumps({"spec": spec, "ins": p["music"]["instruments"][p["xmPool"]["own_note"][0]]}))`,
    );
    const gax = M.xmInstrumentGax(r.spec);
    expect([gax.tune, gax.envelope, gax.vibrato.depth, gax.vibrato.speed]).toEqual([r.ins.rows[0]!.tune, r.ins.envelope!.points.map((p) => p.value), r.ins.vibratoDepth, r.ins.vibratoSpeed]);
    expect(r.ins.seq[0]!.fx[0]).toBe((12 << 8) | gax.volume);
    const smp = r.spec.samples[0]!;
    expect(M.xmInstrumentProblem(r.spec, [1200])).toBeNull();
    expect(M.xmInstrumentProblem({ ...r.spec, samples: [{ ...smp, volume: 65 }] }, [1200])).toMatch(/volume/);
    expect(M.xmInstrumentProblem({ ...r.spec, samples: [{ ...smp, loop_end: 1300 }] }, [1200])).toMatch(/loop/);
    expect(M.xmInstrumentProblem({ ...r.spec, envelope: { ...r.spec.envelope!, points: Array.from({ length: 13 }, (_, k) => [k, 1] as [number, number]) } }, [1200])).toMatch(/12 points/);
    expect(M.xmInstrumentProblem({ ...r.spec, keymap: [...r.spec.keymap.slice(1), 1] }, [1200])).toMatch(/key map/);
    expect(M.xmNameProblem("Instrument 4", 1, [])).toMatch(/pool/);
    expect(M.xmNameProblem("lead", 1, ["lead"])).toMatch(/json/);
    expect(M.xmNameProblem("lead", 0, ["lead"])).toBeNull();
    expect(M.xmNameProblem("x".repeat(23), 1, [])).toMatch(/22/);
    expect(M.xmName("Instrument 4.wav")).toBe("Instrument 4.wav");
    expect(M.xmName("Instrument 4")).toBe("Instrument 4 copy");
    expect(M.xmName("café crème brûlée, a long one")).toBe("caf_ cr_me br_l_e, a l");
    // a save sends the fields and the samples' new sounds; the player request too
    const e: M.XmInstrumentEdit = { spec: r.spec, sounds: { "0": { wav: "UklGRg==", length: 4, rate: 12000 } } };
    const s1 = { ...s0, xmInstruments: { own_note: { "1": e } } };
    const sent = { ...r.spec, samples: [{ ...smp, wav: "UklGRg==" }] };
    expect(M.saveBody(s0, s1).xmInstruments).toEqual({ own_note: { "1": sent } });
    expect(M.unsavedBody(s1).xmInstruments).toEqual({ own_note: { "1": sent } });
    expect(M.saveBody(s1, s1).xmInstruments).toBeUndefined();
  });

  it("number a song's new instruments and play its key map as the build does", () => {
    // tools/gax_render.py edited_body's own_keys: instrument 1's key map and samples, new own instruments 2 and 4
    // around pool instrument 7 (3), and cells on 2 and 3
    const r = python<{ p: MusicPatterns; names: string[]; edits: Record<string, Partial<MusicXmInstrument>>; added: number[]; own: Record<string, { notes: number[]; plays: number[] }>; xm: string }>(
      `from buildtools import gax_render as R\nimport base64\nb = R.edited_body(".")\nkey = R.KEYS_SONG\nbuf = base64.b64decode(next(s["xm"] for s in b["newSongs"] if s["key"] == key))\nfiles = E.Files(g)\nm = json.load(open(files.manifest))\n_, xms = E.apply_song_edits(files, m, [key], {key: buf}, {k: {key: b[k][key]} for k in ("songInstruments", "xmInstruments", "patterns")})\nxm = G.read_xm("x.xm", xms[key])\nown = E.own_instruments(files, xm)\nprint(json.dumps({"p": E.patterns(g, key, base64.b64encode(buf).decode()), "names": E.xm_instrument_names(xms[key]), "edits": b["xmInstruments"][key], "added": b["songInstruments"][key], "own": {str(n): {"notes": o["notes"], "plays": o["plays"]} for n, o in own.items()}, "xm": base64.b64encode(xms[key]).decode()}))`,
    );
    const count = r.p.instruments.length;
    const own = Object.fromEntries(
      Object.entries(r.edits).map(([n, spec]) => {
        const base = Number(n) <= count ? M.xmSpec(r.p.instruments[Number(n) - 1]!.own!.spec) : M.newXmSpec(spec.name ?? "", null);
        const merged: MusicXmInstrument = { ...base, ...spec, samples: (spec.samples ?? base.samples).map((x, i) => ({ ...(base.samples[i] ?? M.XM_NEW_SAMPLE), ...x })) };
        return [n, { spec: M.xmSpec(merged), added: Number(n) > count }];
      }),
    );
    expect(M.songInstruments(r.p, r.added, own).map((i) => i.name)).toEqual(r.names);
    // the notes the song plays on each instrument now (its cells edited), and the samples its key map plays there
    const after = python<MusicPatterns>(`print(json.dumps(E.patterns(g, "own_keys", ${JSON.stringify(r.xm)})))`);
    const notes = M.playedNotes(after, M.decodeCells(after));
    for (const [n, o] of Object.entries(r.own)) {
      expect([...(notes.get(Number(n)) ?? [])].sort((a, b) => a - b)).toEqual(o.notes);
      expect(M.keymapPlays(M.xmSpec(after.instruments[Number(n) - 1]!.own!.spec), o.notes)).toEqual(o.plays);
    }
    expect(r.own["1"]!.plays).toEqual([1]);
    // a new one is sent with its number: blank (no sample), from a WAV (its tune for C-4 at the WAV's rate)
    const blank = M.newXmSpec("a blank one", null);
    expect([blank.samples, blank.keymap.length]).toEqual([[], 96]);
    expect(M.newXmSpec("x", 12000).samples[0]).toEqual({ ...M.XM_NEW_SAMPLE, relative_note: 6, finetune: 32 });
    expect(M.keymapPlays(blank, [49])).toEqual([]);
    const s1 = { ...s0, xmInstruments: { own_keys: { "5": { spec: blank, added: true } } } };
    expect(M.saveBody(s0, s1).xmInstruments).toEqual({ own_keys: { "5": blank } });
  });

  it("check a JSON instrument as the build does, and send it", () => {
    const lead = { sample: "samples/lead.wav", loop: "forward" as const, loop_start: 10, loop_end: 100, volume: 40, envelope: { points: [[0, 64], [10, 0]] as [[number, number], [number, number]], sustain: 1 }, vibrato: { delay: 2, depth: 8, speed: 4 } };
    expect(M.jsonInstrumentProblem(lead, 100)).toBeNull();
    expect(M.jsonInstrumentProblem({ ...lead, loop_end: 101 }, 100)).toMatch(/loop/);
    expect(M.jsonInstrumentProblem({ ...lead, volume: 65 }, 100)).toMatch(/volume/);
    expect(M.jsonInstrumentProblem({ ...lead, vibrato: { speed: 64 } }, 100)).toMatch(/speed/);
    expect(M.jsonInstrumentProblem({ ...lead, envelope: { points: [[2, 64]] } }, 100)).toMatch(/tick 0/);
    expect(M.jsonInstrumentProblem({ ...lead, envelope: { points: [[0, 64]], sustain: 3 } }, 100)).toMatch(/sustain/);
    const rates = [8000, 11025, 16000, 22050, 8363, 31537];
    const py = python<number[]>(`print(json.dumps([G.tune_for_rate(r, False) for r in ${JSON.stringify(rates)}]))`);
    expect(rates.map(M.rateTune)).toEqual(py);
    // a save sends its value and its WAV's new sound; the player request too
    const e: M.JsonInstrumentEdit = { json: lead, wav: { wav: "UklGRg==", length: 4, rate: 16000 }, added: true };
    const s1 = { ...s0, jsonInstruments: { lead: e } };
    expect(M.saveBody(s0, s1).jsonInstruments).toEqual({ lead: { json: lead, wav: "UklGRg==" } });
    expect(M.unsavedBody(s1).jsonInstruments).toEqual({ lead: { json: lead, wav: "UklGRg==" } });
    expect(M.saveBody(s1, s1).jsonInstruments).toBeUndefined();
    expect(M.saveBody(s1, { ...s1, jsonInstruments: { lead: { json: { ...lead, volume: 30 } } } }).jsonInstruments).toEqual({ lead: { json: { ...lead, volume: 30 } } });
  });
});

describe("the edits", () => {
  const view = python<MusicView>(`print(json.dumps(E.view(g)))`);
  const sfx = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/sfx_table.json"), "utf8"));
  const music = JSON.parse(readFileSync(join(ROOT, "games/xs/sound/music.json"), "utf8"));
  const s0 = M.initialState(view, sfx, music);

  it("send nothing when nothing changed", () => {
    expect(M.saveBody(s0, s0)).toEqual({});
    expect(M.saveBody(s0, { ...s0, themes: { ...s0.themes } })).toEqual({});
  });

  it("send what changed", () => {
    const entries = s0.sfx.entries.map((e, i) => (i === 3 ? { ...e, volume: 150 } : e));
    const s1: M.EditState = {
      ...s0,
      sfx: { ...s0.sfx, entries },
      themes: { ...s0.themes, THEME_JUNGLE: "drums" },
      cells: { jungle: { "0:3:0": [50, 1, 0, 0, 0] } },
      samples: { "sfx_samples/09.wav": { wav: "UklGRg==", length: 4, rate: 8000 } },
    };
    const b = M.saveBody(s0, s1);
    expect(Object.keys(b).sort()).toEqual(["patterns", "samples", "sfxTable", "themes"]);
    expect(b.themes).toEqual({ THEME_JUNGLE: "drums" });
    expect(b.patterns).toEqual({ jungle: [{ pattern: 0, row: 3, channel: 0, cell: [50, 1, 0, 0, 0] }] });
    expect(b.samples).toEqual({ "sfx_samples/09.wav": "UklGRg==" });
  });

  it("keep a sound effect the code names", () => {
    const last = view.sfx[view.sfx.length - 1]!;
    expect(M.cannotDeleteLast(view, s0)).toEqual(last.uses.length ? expect.stringContaining(last.name!) : null);
    const added = { ...s0, sfx: { ...s0.sfx, entries: [...s0.sfx.entries, { slot_id: 1, pitch_offset: 0, volume: 256, name: "NEW_SOUND" } as M.SfxEntry] } };
    expect(M.cannotDeleteLast(view, added)).toBeNull();
    expect(M.freeName(added.sfx.entries)).toBe("NEW_SOUND_2");
  });

  it("make a new sound effect's instrument as the build does", () => {
    const py = python<MusicInstrument>(
      `spec = {"sample": b"x", "loop": "none", "loop_start": 0, "loop_end": 1, "volume": 40, "vibrato": (0, 0, 0), "tune": G.tune_for_rate(8000, True), "fixed_pitch": True, "envelope": None}\nprint(json.dumps(E.instrument_struct(G.Instrument(G.instrument_entry(spec, 88, None)))))`,
    );
    expect(M.sfxInstrument(88, 8000, 40)).toEqual(py);
  });

  it("read and write the tree's 8-bit WAVs", () => {
    const wav = readFileSync(join(ROOT, "games/xs/sound/sfx_samples/01.wav"));
    const { rate, data: pcm } = M.parseWav8(new Uint8Array(wav));
    expect(rate).toBe(7994);
    expect(Buffer.from(M.wavFile(pcm, rate)).equals(wav)).toBe(true);
    expect(M.frameTime(59727)).toBe("16:40.0");
  });
});

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
import { join } from "node:path";
import { describe, expect, it } from "vitest";
import type { MusicInstrument, MusicPatterns, MusicPool, MusicSong, MusicView } from "@crash/schema-types";
import { GaxPlayer, decodePool } from "../src/lib/music/gax.ts";
import * as M from "../src/lib/music/model.ts";

const ROOT = join(__dirname, "..", "..", "..");

/** crashdata.audio.edit, as the backend runs it. */
function python<T>(code: string): T {
  const out = execFileSync("python3", ["-c", `import json, sys\nfrom crashdata.audio import edit as E\nfrom crashdata.audio import gax as G\nfrom crashdata.common.game import Game\ng = Game("xs", ".")\n${code}`], {
    cwd: ROOT,
    env: { ...process.env, PYTHONPATH: join(ROOT, "packages", "crashdata", "src") },
    maxBuffer: 256 * 1024 * 1024,
    encoding: "utf8",
  });
  return JSON.parse(out) as T;
}

interface Reference {
  frames: number;
  songs: Record<string, string>;
  sfx: { song: string; musicVolume?: number; frames: number; events: [number, number, number, number, number][]; sha1: string }[];
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

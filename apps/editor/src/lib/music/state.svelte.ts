// The music editor's state (docs/editors.md, "The music editor"): the
// project's songs, sound effects and samples as the backend gave them
// (crashdata.audio.edit), the edit state now (model.ts), the undo stack,
// what is selected and played, and saving it all back through the
// backend's music save, which checks it like any save (and builds the
// music and the sound effects) and keeps it all or none.
import { ApiError, type Music, type MusicPatterns, type MusicPool, type MusicSong, type MusicView, type SfxTable } from "@crash/schema-types";
import { client } from "../api.ts";
import { editor } from "../state.svelte.ts";
import { Snapshots } from "../cutscene/history.ts";
import { Output, playSample, type Position } from "./audio.ts";
import { GaxPlayer, decodePool, type GaxPool } from "./gax.ts";
import * as M from "./model.ts";

export type Kind = "song" | "sfx" | "sample";

function emptyState(): M.EditState {
  return { sfx: { entries: [] } as unknown as SfxTable, music: { screen_music: [] }, themes: {}, comments: {}, cells: {}, samples: {}, newInstruments: {}, newSongs: [], removeSongs: [] };
}

/** The NO_NOTE sentinel of a channel's note (GaxChannelState.note 0x8AD0). */
const NO_NOTE = (0x8ad0 << 16) >> 16;

class MusicEditorState {
  view = $state.raw<MusicView | null>(null);
  pool = $state.raw<MusicPool | null>(null);
  /** why there is no view (N-Tranced's sound isn't data) */
  unavailable = $state<string | null>(null);
  state = $state.raw<M.EditState>(emptyState());
  saved = $state.raw<M.EditState>(emptyState());
  history = new Snapshots<M.EditState>(emptyState());
  hv = $state(0);
  loading = $state(false);

  kind = $state<Kind>("song");
  song = $state("");
  sfx = $state(0);
  sample = $state("");
  /** the songs' XMs, by key, as saved: their patterns and decoded cells */
  patterns = $state.raw<Record<string, { p: MusicPatterns; cells: Uint8Array[] }>>({});
  /** the order position the pattern view shows, and the cell selected */
  order = $state(0);
  cell = $state<{ row: number; channel: number } | null>(null);
  follow = $state(true);

  // playing
  playing = $state<"song" | "sfx" | "sample" | null>(null);
  position = $state<Position | null>(null);
  musicVolume = $state(255);
  /** the song a sound effect plays over (its mix rate, its voices' half rate) and whether its music is heard */
  sfxSong = $state("jungle");
  sfxWithMusic = $state(false);
  songLength = $state<Record<string, { loop: number; order: number } | null>>({});
  private output = new Output(
    (p) => {
      this.position = p;
      if (this.follow && this.playing === "song" && p.order >= 0 && p.order !== this.order) this.order = p.order;
    },
    () => {
      this.playing = null;
    },
  );
  private songCache = new Map<string, MusicSong>();
  private projectKey = "";

  get dirty(): boolean {
    void this.hv;
    return this.history.dirty;
  }

  get canUndo(): boolean {
    void this.hv;
    return this.history.canUndo;
  }

  get canRedo(): boolean {
    void this.hv;
    return this.history.canRedo;
  }

  get undoLabel(): string {
    void this.hv;
    return this.history.undoLabel ?? "";
  }

  get redoLabel(): string {
    void this.hv;
    return this.history.redoLabel ?? "";
  }

  confirmDiscard(): boolean {
    return !this.dirty || confirm("The music has changes that aren't saved. Discard them?");
  }

  /** The songs of the song table now: the saved ones (less the removed), then the new ones. */
  get songs(): { key: string; id: number; info: MusicView["songs"][number] | null; added: boolean }[] {
    const v = this.view;
    if (!v) return [];
    const out: { key: string; id: number; info: MusicView["songs"][number] | null; added: boolean }[] = v.songs
      .filter((s) => !this.state.removeSongs.includes(s.key))
      .map((s) => ({ key: s.key, id: 0, info: s, added: false }));
    for (const n of this.state.newSongs) out.push({ key: n.key, id: 0, info: null, added: true });
    out.forEach((s, i) => (s.id = i));
    return out;
  }

  get songKeys(): string[] {
    return this.songs.map((s) => s.key);
  }

  get songInfo(): MusicView["songs"][number] | undefined {
    return this.view?.songs.find((s) => s.key === this.song);
  }

  // -- opening ------------------------------------------------------------------------

  async syncProject(id: string, ready: boolean): Promise<void> {
    const key = `${id}:${ready}`;
    if (key === this.projectKey) return;
    this.projectKey = key;
    this.stop();
    this.view = null;
    this.pool = null;
    this.unavailable = null;
    this.patterns = {};
    this.songCache.clear();
    this.reset(emptyState());
    if (ready) await this.load();
  }

  private reset(s: M.EditState): void {
    this.state = s;
    this.saved = s;
    this.history.reset(s);
    this.hv = this.history.version;
  }

  async load(): Promise<void> {
    const id = editor.projectId;
    this.loading = true;
    try {
      await editor.run("Reading the music", async () => {
        let view: MusicView;
        try {
          view = await client.music(id);
        } catch (e) {
          if (e instanceof ApiError && e.status === 404) {
            this.unavailable = e.message;
            return;
          }
          throw e;
        }
        const [pool, sfx, music] = await Promise.all([
          client.musicPool(id),
          client.read<"sfx_table.schema.json">(id, view.files.sfxTable),
          client.read<"music.schema.json">(id, view.files.music),
        ]);
        this.view = view;
        this.pool = pool;
        this.patterns = {};
        this.wavs = {};
        this.songLength = {};
        this.songCache.clear();
        this.reset(M.initialState(view, sfx.data as SfxTable, music.data as Music));
        const q = new URLSearchParams(location.hash.slice(1));
        const want = q.get("song");
        this.song = want && view.songs.some((s) => s.key === want) ? want : (view.songs[0]?.key ?? "");
        if (!view.songs.some((s) => s.key === this.sfxSong)) this.sfxSong = view.songs[0]?.key ?? "";
        this.order = 0;
        this.cell = null;
        if (q.get("sfx")) {
          this.kind = "sfx";
          this.sfx = Math.max(0, Math.min(Number(q.get("sfx")) || 0, view.sfx.length - 1));
        }
        await this.loadPatterns(this.song);
      });
    } finally {
      this.loading = false;
    }
  }

  async loadPatterns(key: string): Promise<void> {
    if (!key || this.patterns[key] || !this.view?.songs.some((s) => s.key === key)) return;
    const p = await client.musicPatterns(editor.projectId, key);
    this.patterns = { ...this.patterns, [key]: { p, cells: M.decodeCells(p) } };
  }

  async selectSong(key: string): Promise<void> {
    this.kind = "song";
    if (key === this.song) return;
    this.song = key;
    this.order = 0;
    this.cell = null;
    window.history.replaceState(null, "", `#${new URLSearchParams({ ...Object.fromEntries(new URLSearchParams(location.hash.slice(1))), mode: "music", song: key })}`);
    await editor.run(`Reading ${key}`, () => this.loadPatterns(key));
  }

  /** The selected song's cells now: as saved, with the edits (copied to the patterns they share bytes with). */
  get cells(): Uint8Array[] | null {
    const x = this.patterns[this.song];
    if (!x) return null;
    return M.editedCells(x.p, x.cells, this.state.cells[this.song]);
  }

  // -- edits --------------------------------------------------------------------------

  apply(label: string, f: (s: M.EditState) => M.EditState, key: string | null = null): boolean {
    let next: M.EditState;
    try {
      next = f(this.state);
    } catch (e) {
      editor.error = e instanceof Error ? e.message : String(e);
      return false;
    }
    if (next === this.state) return false;
    this.state = next;
    this.history.push(label, next, key);
    this.hv = this.history.version;
    this.clamp();
    return true;
  }

  undo(): void {
    const s = this.history.undo();
    if (s) this.state = s;
    this.hv = this.history.version;
    this.clamp();
  }

  redo(): void {
    const s = this.history.redo();
    if (s) this.state = s;
    this.hv = this.history.version;
    this.clamp();
  }

  private clamp(): void {
    if (this.sfx >= this.state.sfx.entries.length) this.sfx = Math.max(0, this.state.sfx.entries.length - 1);
    if (!this.songKeys.includes(this.song)) this.song = this.songKeys[0] ?? "";
  }

  setCell(pattern: number, row: number, channel: number, cell: M.Cell, key: string | null = null): void {
    const song = this.song;
    this.apply(
      `Edit ${song}'s pattern ${pattern}, row ${row}, channel ${channel + 1}`,
      (s) => ({ ...s, cells: { ...s.cells, [song]: { ...(s.cells[song] ?? {}), [M.cellKey(pattern, row, channel)]: cell } } }),
      key ? `${song}:${pattern}:${row}:${channel}:${key}` : null,
    );
  }

  setSfx(i: number, patch: Partial<M.SfxEntry> & { slot_id?: number; instrument?: string }, key: string | null = null): void {
    this.apply(
      `Edit sound effect ${i}`,
      (s) => {
        const entries = s.sfx.entries.map((e, k) => {
          if (k !== i) return e;
          const next = { ...e, ...patch } as M.SfxEntry & { slot_id?: number; instrument?: string };
          if ("slot_id" in patch && patch.slot_id !== undefined) delete next.instrument;
          if ("instrument" in patch && patch.instrument !== undefined) delete next.slot_id;
          return next as M.SfxEntry;
        });
        return { ...s, sfx: { ...s.sfx, entries } };
      },
      key ? `sfx:${i}:${key}` : null,
    );
  }

  addSfx(entry: M.SfxEntry): void {
    if (this.apply(`New sound effect ${entry.name ?? ""}`, (s) => ({ ...s, sfx: { ...s.sfx, entries: [...s.sfx.entries, entry] } }))) {
      this.kind = "sfx";
      this.sfx = this.state.sfx.entries.length - 1;
    }
  }

  deleteLastSfx(): void {
    const why = this.view ? M.cannotDeleteLast(this.view, this.state) : "no view";
    if (why) {
      editor.error = why;
      return;
    }
    this.apply("Delete the last sound effect", (s) => {
      const entries = s.sfx.entries.slice(0, -1);
      const gone = s.sfx.entries[s.sfx.entries.length - 1] as { instrument?: string };
      const newInstruments = { ...s.newInstruments };
      if (gone.instrument && !entries.some((e) => (e as { instrument?: string }).instrument === gone.instrument)) delete newInstruments[gone.instrument];
      return { ...s, sfx: { ...s.sfx, entries }, newInstruments };
    });
  }

  setScreenSong(screen: string, song: string): void {
    this.apply(`${screen} plays ${song}`, (s) => ({ ...s, music: { ...s.music, screen_music: s.music.screen_music.map((x) => (x.screen === screen ? { ...x, song } : x)) } }));
  }

  setThemeSong(theme: string, song: string): void {
    this.apply(`${theme} plays ${song}`, (s) => ({ ...s, themes: { ...s.themes, [theme]: song } }));
  }

  setComment(song: string, comment: string): void {
    this.apply(`${song}'s comment`, (s) => ({ ...s, comments: { ...s.comments, [song]: comment } }), `comment:${song}`);
  }

  /** A WAV file made into a sample by the backend (8-bit mono, at `rate`; the WAV's own rate if none). */
  async importWav(file: File, rate?: number): Promise<M.SampleEdit | undefined> {
    const buf = new Uint8Array(await file.arrayBuffer());
    const r = await editor.run(`Importing ${file.name}`, () => client.importSample(editor.projectId, M.toBase64(buf), rate));
    if (!r) return undefined;
    if (r.clipped) editor.error = `${file.name}: ${r.clipped} samples clipped to 8 bits`;
    return { wav: r.wav, length: r.length, rate: r.rate };
  }

  replaceSample(path: string, s: M.SampleEdit): void {
    this.apply(`Replace ${path}`, (st) => ({ ...st, samples: { ...st.samples, [path]: s } }));
  }

  /** A new sound effect with its own sample: instruments/<name>.json, sfx_samples/<name>.wav, and its table entry. */
  newSfxWithSample(name: string, sfxName: string, s: M.SampleEdit, comment: string): void {
    const entry = { instrument: name, pitch_offset: 0, volume: 256, name: sfxName, comment } as unknown as M.SfxEntry;
    if (this.apply(`New sound effect ${sfxName}`, (st) => ({ ...st, newInstruments: { ...st.newInstruments, [name]: { ...s, comment } }, sfx: { ...st.sfx, entries: [...st.sfx.entries, entry] } }))) {
      this.kind = "sfx";
      this.sfx = this.state.sfx.entries.length - 1;
    }
  }

  async addSong(file: File, key: string, comment: string): Promise<void> {
    const xm = M.toBase64(new Uint8Array(await file.arrayBuffer()));
    this.apply(`New song ${key}`, (s) => ({ ...s, newSongs: [...s.newSongs, { key, xm, comment }], comments: { ...s.comments, [key]: comment } }));
  }

  removeSong(key: string): void {
    this.apply(`Take ${key} out of the song table`, (s) => {
      if (s.newSongs.some((n) => n.key === key)) return { ...s, newSongs: s.newSongs.filter((n) => n.key !== key) };
      return { ...s, removeSongs: [...s.removeSongs, key] };
    });
  }

  /** What still plays a song, after the edits: the themes, the screens, the cutscene slides. */
  usesOf(key: string): string[] {
    const out: string[] = [];
    for (const [t, s] of Object.entries(this.state.themes)) if (s === key) out.push(t);
    for (const s of this.state.music.screen_music) if (s.song === key) out.push(`screen ${s.screen}`);
    for (const u of this.view?.songs.find((s) => s.key === key)?.uses ?? []) if (u.kind === "cutscene") out.push(`cutscene slide ${u.name}`);
    return out;
  }

  // -- playing ---------------------------------------------------------------------------

  /** The pools the player plays, with the unsaved samples and new instruments. */
  private pools(): { music: GaxPool; sfx: GaxPool; slots: Record<string, number> } {
    const pool = this.pool!;
    const music = decodePool(pool.music);
    const sfx = decodePool(pool.sfx);
    const slots = { ...pool.sfxSlots };
    for (const [path, s] of Object.entries(this.state.samples)) {
      const m = /^(samples|sfx_samples)\/(\d+)\.wav$/.exec(path);
      if (!m) continue;
      const data = M.parseWav8(M.fromBase64(s.wav)).data;
      (m[1] === "samples" ? music : sfx).waves.set(Number(m[2]), { data, offset: 0, length: data.length });
    }
    for (const [name, n] of Object.entries(this.state.newInstruments)) {
      if (slots[name] !== undefined) continue;
      const data = M.parseWav8(M.fromBase64(n.wav)).data;
      const wave = Math.max(...sfx.waves.keys()) + 1;
      sfx.waves.set(wave, { data, offset: 0, length: data.length });
      slots[name] = sfx.instruments.length;
      sfx.instruments.push(M.sfxInstrument(wave, n.rate, n.volume));
    }
    return { music, sfx, slots };
  }

  private async program(key: string): Promise<MusicSong | undefined> {
    const edits = this.state.cells[key];
    const list = edits ? Object.entries(edits).map(([k, cell]) => {
      const [pattern, row, channel] = k.split(":").map(Number) as [number, number, number];
      return { pattern, row, channel, cell };
    }) : [];
    const ck = `${key}:${JSON.stringify(list)}`;
    const hit = this.songCache.get(ck);
    if (hit) return hit;
    const song = await editor.run(list.length ? `Building ${key} with its edits` : `Reading ${key}`, () => client.musicSong(editor.projectId, key, list));
    if (song) this.songCache.set(ck, song);
    return song;
  }

  /** A player of the song, run on to order position `from` (GAX2 has no seek: the frames before it are mixed and dropped). */
  async songPlayer(key: string, from = 0): Promise<GaxPlayer | undefined> {
    if (!this.pool) return undefined;
    const song = await this.program(key);
    if (!song) return undefined;
    const { music, sfx } = this.pools();
    const p = new GaxPlayer(this.pool.tables, music, sfx, song);
    p.setMusicVolume(-1, this.musicVolume);
    if (from > 0) {
      const scratch = new Int8Array(p.frames);
      for (let i = 0; i < 200000 && !(p.info.orderPos === from && p.info.row === 0 && p.info.newRow); i++) p.frame(scratch);
    }
    return p;
  }

  async playSong(from = this.order): Promise<void> {
    const p = await this.songPlayer(this.song, from);
    if (!p) return;
    this.output.start(p);
    this.playing = "song";
  }

  /** A sound effect as PlaySfx plays it: its slot on voice 0, its priority and volume, over the chosen song. */
  async playSfx(i = this.sfx): Promise<void> {
    const e = this.state.sfx.entries[i] as M.SfxEntry & { slot_id?: number; instrument?: string };
    if (!e) return;
    const p = await this.songPlayer(this.sfxSong);
    if (!p) return;
    const slot = e.slot_id ?? this.pools().slots[e.instrument ?? ""];
    if (slot === undefined) {
      editor.error = `${e.instrument}: no such instrument`;
      return;
    }
    if (!this.sfxWithMusic) p.setMusicVolume(-1, 0);
    let quiet = 0;
    this.output.start(p, {
      frames: 60 * 30,
      before: (f, pl) => {
        if (f === 0) {
          const v = pl.fx(slot, 0, e.pitch_offset, -1);
          pl.setFxVolume(v, M.sfxVoiceVolume(e));
        } else if (pl.voices[0]!.note === NO_NOTE || pl.voices[0]!.instrument === null) {
          if (++quiet === 6 && !this.sfxWithMusic) {
            this.output.stop();
            this.playing = null;
          }
        }
      },
    });
    this.playing = "sfx";
  }

  /** The tree's samples read so far (sfx_samples/NN.wav, ...), as saved. */
  wavs = $state.raw<Record<string, { data: Int8Array; rate: number }>>({});

  /** A sample's sound now: its unsaved replacement (or a new instrument's), or the file's. */
  sound(path: string): { data: Int8Array; rate: number } | null {
    const edit = this.state.samples[path] ?? this.state.newInstruments[path.replace(/^sfx_samples\/(.*)\.wav$/, "$1")];
    if (edit) return M.parseWav8(M.fromBase64(edit.wav));
    return this.wavs[path] ?? null;
  }

  async loadWav(path: string): Promise<void> {
    if (this.wavs[path] || this.state.samples[path]) return;
    const buf = await client.raw(editor.projectId, `games/xs/sound/${path}`).catch(() => null);
    if (buf) this.wavs = { ...this.wavs, [path]: M.parseWav8(buf) };
  }

  /** A sample as it is, at its rate (not through GAX2). */
  async playWav(path: string): Promise<void> {
    await this.loadWav(path);
    const s = this.sound(path);
    if (!s) return;
    const { data, rate } = s;
    this.stop();
    this.playing = "sample";
    try {
      await playSample(data, rate);
    } finally {
      if (this.playing === "sample") this.playing = null;
    }
  }

  stop(): void {
    this.output.stop();
    this.playing = null;
  }

  setMusicVolume(v: number): void {
    this.musicVolume = v;
  }

  /** How long a song plays before it loops (its Info handler's songEnded), mixing nothing. */
  async measure(key: string): Promise<void> {
    if (key in this.songLength) return;
    const p = await this.songPlayer(key);
    if (!p) return;
    const scratch = new Int8Array(p.frames);
    let f = 0;
    for (; f < 60 * 60 * 20 && !p.info.songEnded; f++) p.frame(scratch);
    this.songLength = { ...this.songLength, [key]: p.info.songEnded ? { loop: f, order: p.song.loopOrder } : null };
  }

  /** The song rendered as the PC port's --wav writes it: 8-bit mono at its mix rate. */
  async exportWav(seconds: number): Promise<Blob | undefined> {
    const p = await this.songPlayer(this.song);
    if (!p) return undefined;
    const frames = Math.round((seconds * 0xe94f) / 1000);
    return new Blob([M.wavFile(p.render(frames), p.mixRate)], { type: "audio/wav" });
  }

  // -- saving ------------------------------------------------------------------------------

  saveBody() {
    return M.saveBody(this.saved, this.state);
  }

  async save(): Promise<void> {
    const body = this.saveBody();
    const ok = await editor.run("Saving the music", async () => {
      const r = await client.saveMusic(editor.projectId, body);
      const errors = r.diagnostics.filter((d) => d.severity === "error").length;
      editor.diagnostics = {
        source: "save",
        title: r.saved.length ? `Saved ${r.saved.join(", ")}` + (errors ? ` (the data's other ${errors} error(s) are as they were)` : "") : "The music: nothing changed",
        ok: true,
        diagnostics: r.diagnostics,
        at: new Date(),
      };
      return true;
    });
    if (!ok) return;
    // what is saved is now the files: read them again (the XMs, the samples, the new songs and instruments)
    const keep = { kind: this.kind, song: this.song, sfx: this.sfx, order: this.order };
    await this.load();
    Object.assign(this, keep);
    await editor.run(`Reading ${this.song}`, () => this.loadPatterns(this.song));
  }

  async revert(): Promise<void> {
    if (!this.confirmDiscard()) return;
    this.history.reset(this.saved);
    await this.load();
  }
}

export const music = new MusicEditorState();

editor.projectGuards.push(() => music.confirmDiscard());

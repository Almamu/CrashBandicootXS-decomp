// The music editor's state (docs/editors.md, "The music editor"): the
// project's songs, sound effects and samples as the backend gave them
// (crashdata.audio.edit), the edit state now (model.ts), the undo stack,
// what is selected and played, and saving it all back through the
// backend's music save, which checks it like any save (and builds the
// music and the sound effects) and keeps it all or none.
import { ApiError, type Instrument, type Music, type MusicJsonInstrument, type MusicOwnInstrument, type MusicPatterns, type MusicPool, type MusicSong, type MusicUnsaved, type MusicView, type SfxTable } from "@crash/schema-types";
import { client } from "../api.ts";
import { editor } from "../state.svelte.ts";
import { Snapshots } from "../cutscene/history.ts";
import { Output, playSample, type Position } from "./audio.ts";
import { GaxPlayer, decodePool, type GaxPool } from "./gax.ts";
import * as M from "./model.ts";

export type Kind = "song" | "sfx" | "sample" | "instrument";

function emptyState(): M.EditState {
  return { sfx: { entries: [] } as unknown as SfxTable, music: { screen_music: [] }, themes: {}, comments: {}, cells: {}, samples: {}, newInstruments: {}, newSongs: [], removeSongs: [], structures: {}, instruments: {}, newSamples: {}, deleteSamples: [], deleteInstruments: [], songInstruments: {}, songSettings: {}, sfxInstruments: {}, newSfxSamples: {}, deleteSfxSamples: [], deleteSfxInstruments: [], xmInstruments: {}, jsonInstruments: {} };
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
  /** the instrument selected: the music pool's, the sound-effect set's, a song's own XM instrument (xmSong's
   * instrument N, 1-based), or instruments/<jsonName>.json's */
  instrument = $state(1);
  instrumentPool = $state<"music" | "sfx" | "xm" | "json">("music");
  xmSong = $state("");
  /** the sample of the own instrument selected that its panel shows */
  ownSample = $state(0);
  jsonName = $state("");
  /** the songs' XMs, by key, as saved: their patterns and decoded cells */
  patterns = $state.raw<Record<string, { p: MusicPatterns; cells: Uint8Array[] }>>({});
  /** the order position the pattern view shows, and the cell selected */
  order = $state(0);
  cell = $state<{ row: number; channel: number } | null>(null);
  follow = $state(true);

  // playing
  playing = $state<"song" | "sfx" | "sample" | "instrument" | null>(null);
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
  private poolCache = new Map<string, MusicPool>();
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
    this.poolCache.clear();
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
        this.poolCache.clear();
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
    if (!key || this.patterns[key]) return;
    // a saved song's XM, or an unsaved new song's (the backend reads it as it reads the files)
    const added = this.state.newSongs.find((n) => n.key === key);
    if (!added && !this.view?.songs.some((s) => s.key === key)) return;
    const p = await client.musicPatterns(editor.projectId, key, added?.xm);
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

  /** The selected song as the pattern view shows it now: its structure if it was changed, else its XM's
   * patterns as saved with the cell edits (copied to the patterns they share bytes with). */
  get songView(): { p: MusicPatterns; cells: Uint8Array[]; structure: M.Structure | null } | null {
    return this.viewOf(this.song);
  }

  viewOf(key: string): { p: MusicPatterns; cells: Uint8Array[]; structure: M.Structure | null } | null {
    const x = this.patterns[key];
    if (!x) return null;
    const st = this.state.structures[key];
    if (st) return { p: M.structureView(x.p, st), cells: st.patterns.map((p) => p.cells), structure: st };
    return { p: x.p, cells: M.editedCells(x.p, x.cells, this.state.cells[key]), structure: null };
  }

  /** The selected song's cells now. */
  get cells(): Uint8Array[] | null {
    return this.songView?.cells ?? null;
  }

  /** A structural edit of the selected song (model.ts's structure operations). Its first one makes the song's
   * structure from its XM and cell edits; a ROM song's shared patterns are then each their own, and the save
   * takes its scaffold out of the manifest (it is built from its XM, crashdata.audio.gax.new_song). */
  structural(label: string, f: (s: M.Structure) => M.Structure, key: string | null = null): boolean {
    const song = this.song;
    const x = this.patterns[song];
    if (!x) return false;
    return this.apply(
      `${label} (${song})`,
      (s) => {
        const base = s.structures[song] ?? M.structureOf(x.p, M.editedCells(x.p, x.cells, s.cells[song]));
        const next = f(base);
        if (next === base) return s;
        const cells = { ...s.cells };
        delete cells[song];
        return { ...s, structures: { ...s.structures, [song]: next }, cells };
      },
      key ? `${song}:structure:${key}` : null,
    );
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
    if (this.instrumentPool === "xm" && (!this.songKeys.includes(this.xmSong) || (this.patterns[this.xmSong] && !this.own(this.xmSong, this.instrument)))) [this.instrumentPool, this.instrument] = ["music", 1];
    if (this.instrumentPool === "json" && !this.json(this.jsonName)) [this.instrumentPool, this.instrument] = ["music", 1];
    // a structural edit (or its undo) can take away the order position or the cell shown
    const v = this.songView;
    if (v) {
      if (this.order >= v.p.playOrder.length) this.order = Math.max(0, v.p.playOrder.length - 1);
      const rows = v.p.rows[v.p.playOrder[this.order] ?? 0] ?? 0;
      if (this.cell && (this.cell.row >= rows || this.cell.channel >= v.p.channels)) this.cell = null;
    }
  }

  setCell(pattern: number, row: number, channel: number, cell: M.Cell, key: string | null = null): void {
    const song = this.song;
    if (this.state.structures[song]) {
      this.structural(`Edit pattern ${pattern}, row ${row}, channel ${channel + 1}`, (st) => M.setCellIn(st, pattern, row, channel, cell), key ? `${pattern}:${row}:${channel}:${key}` : null);
      return;
    }
    this.apply(
      `Edit ${song}'s pattern ${pattern}, row ${row}, channel ${channel + 1}`,
      (s) => ({ ...s, cells: { ...s.cells, [song]: { ...(s.cells[song] ?? {}), [M.cellKey(pattern, row, channel)]: cell } } }),
      key ? `${song}:${pattern}:${row}:${channel}:${key}` : null,
    );
  }

  /** The selected song's XM instruments now: its own, then those added to it (the pool's, and its own new ones). */
  xmInstruments(key = this.song): { name: string; pool: number | null; added: boolean }[] {
    const p = this.patterns[key]?.p;
    return p ? M.songInstruments(p, this.state.songInstruments[key], this.state.xmInstruments[key]) : [];
  }

  /** A cell playing pool instrument `n`: the XM instrument that is `Instrument n`, added to the song's XM if it has
   * none (one step of the undo stack with the cell). */
  setCellPool(pattern: number, row: number, channel: number, cell: M.Cell, n: number): void {
    const song = this.song;
    const list = this.xmInstruments(song);
    const at = list.findIndex((i) => i.pool === n);
    if (at >= 0) {
      this.setCell(pattern, row, channel, [cell[0], at + 1, cell[2], cell[3], cell[4]]);
      return;
    }
    const most = this.view?.limits.xmInstruments ?? 128;
    if (list.length >= most) {
      editor.error = `an XM has at most ${most} instruments`;
      return;
    }
    const c: M.Cell = [cell[0], list.length + 1, cell[2], cell[3], cell[4]];
    this.apply(`${song} plays instrument ${n} (pattern ${pattern}, row ${row}, channel ${channel + 1})`, (s) => {
      const songInstruments = { ...s.songInstruments, [song]: [...(s.songInstruments[song] ?? []), n] };
      const st = s.structures[song];
      if (st) return { ...s, songInstruments, structures: { ...s.structures, [song]: M.setCellIn(st, pattern, row, channel, c) } };
      return { ...s, songInstruments, cells: { ...s.cells, [song]: { ...(s.cells[song] ?? {}), [M.cellKey(pattern, row, channel)]: c } } };
    });
  }

  // -- a song's settings -------------------------------------------------------------------

  /** Song `key`'s settings now: its edit, or what the build reads (the manifest's, songs/<key>.json's, the defaults). */
  settingsOf(key: string): M.SongSettings | null {
    const v = this.view;
    if (!v) return null;
    const edit = this.state.songSettings[key];
    if (edit) return edit;
    const info = v.songs.find((x) => x.key === key);
    const s = info?.settings ?? (this.state.newSongs.some((n) => n.key === key) ? v.songDefaults : null);
    if (!s) return null;
    const out: M.SongSettings = { sample_rate: s.sample_rate, volume: s.volume, half_rate_fx: s.half_rate_fx, mixer_data_bytes: [...s.mixer_data_bytes] };
    if (!info?.rom) Object.assign(out, { title: s.title ?? null, artist: s.artist ?? null });
    return out;
  }

  setSettings(key: string, label: string, patch: Partial<M.SongSettings>, coalesce: string | null = null): void {
    const base = this.settingsOf(key);
    if (!base) return;
    this.apply(`${key}'s ${label}`, (s) => ({ ...s, songSettings: { ...s.songSettings, [key]: { ...(s.songSettings[key] ?? base), ...patch } } }), coalesce ? `settings:${key}:${coalesce}` : null);
  }

  revertSettings(key: string): void {
    this.apply(`${key}'s settings as saved`, (s) => {
      const songSettings = { ...s.songSettings };
      delete songSettings[key];
      return { ...s, songSettings };
    });
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
    // a new sample (not saved yet) is its new sound
    const m = /^(sfx_)?samples\/(\d+)\.wav$/.exec(path);
    const field = m?.[1] ? "newSfxSamples" : "newSamples";
    const k = m ? String(Number(m[2])) : "";
    this.apply(`Replace ${path}`, (st) => (m && st[field][k] ? { ...st, [field]: { ...st[field], [k]: s } } : { ...st, samples: { ...st.samples, [path]: s } }));
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
    if (this.apply(`New song ${key}`, (s) => ({ ...s, newSongs: [...s.newSongs, { key, xm, comment }], comments: { ...s.comments, [key]: comment } }))) await this.selectSong(key);
  }

  removeSong(key: string): void {
    this.apply(`Take ${key} out of the song table`, (s) => {
      // its own instruments' edits go with it (no song of the table to write them in)
      const xmInstruments = { ...s.xmInstruments };
      delete xmInstruments[key];
      if (s.newSongs.some((n) => n.key === key)) {
        const structures = { ...s.structures };
        delete structures[key];
        return { ...s, newSongs: s.newSongs.filter((n) => n.key !== key), structures, xmInstruments };
      }
      return { ...s, removeSongs: [...s.removeSongs, key], xmInstruments };
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
  private pools(base: MusicPool | null = this.pool): { music: GaxPool; sfx: GaxPool; slots: Record<string, number> } {
    const pool = base!;
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

  /** Song `key` and the music pool as the build would build them with every song's unsaved edits (new songs,
   * structures, cells: the backend builds them, as it builds the saved ones; a new song's new instruments join
   * the pool). */
  private async program(key: string): Promise<{ song: MusicSong; pool: MusicPool } | undefined> {
    const unsaved = M.unsavedBody(this.state);
    const edited = Object.keys(unsaved).length > 0;
    const uk = JSON.stringify(unsaved);
    const ck = `${key}:${uk}`;
    let song = this.songCache.get(ck);
    if (!song) {
      song = await editor.run(edited ? `Building ${key} with the edits` : `Reading ${key}`, () => client.musicSong(editor.projectId, key, undefined, unsaved));
      if (!song) return undefined;
      this.songCache.set(ck, song);
    }
    const pool = await this.builtPool(unsaved);
    return pool ? { song, pool } : undefined;
  }

  /** The music pool as the build would build it with the unsaved edits: the backend builds it when they change it
   * (an instrument's record, a sample added or taken out, a new song's or a structure's instruments). */
  private async builtPool(unsaved: MusicUnsaved = M.unsavedBody(this.state)): Promise<MusicPool | undefined> {
    // the cells of a song with instruments of its own: which of them its cells play is which the build adds
    const own = Object.entries(unsaved.patterns ?? {}).filter(([k]) => this.state.xmInstruments[k] || this.patterns[k]?.p.instruments.some((i) => i.own));
    const parts = [unsaved.newSongs, unsaved.structures, unsaved.instruments, unsaved.newSamples, unsaved.deleteSamples, unsaved.deleteInstruments, unsaved.sfxInstruments, unsaved.songSettings, unsaved.xmInstruments, unsaved.newSfxSamples, unsaved.deleteSfxSamples, unsaved.deleteSfxInstruments, unsaved.jsonInstruments, own.length ? own : undefined];
    if (!parts.some(Boolean)) return this.pool!;
    const pk = JSON.stringify([...parts, unsaved.samples]);
    const hit = this.poolCache.get(pk) ?? (await editor.run("Building the music pool with the edits", () => client.musicPool(editor.projectId, unsaved)));
    if (hit) this.poolCache.set(pk, hit);
    return hit;
  }

  // -- the music pool's instruments ------------------------------------------------------

  /** The music pool's instruments now (model.ts poolInstruments: the new ones too, those taken out emptied). */
  get poolList(): M.PoolInstrument[] {
    return this.view ? M.poolInstruments(this.view, this.state) : [];
  }

  /** The sound-effect set's instruments now (model.ts sfxInstruments: the new ones too, those taken out emptied). */
  get sfxList(): M.PoolInstrument[] {
    return this.view ? M.sfxInstruments(this.view, this.state) : [];
  }

  /** Instrument n's record now (of the music pool or the sound-effect set): its edit, or the manifest's. */
  record(n: number, pool: "music" | "sfx" | "xm" | "json" = "music"): M.InstrumentRecord | null {
    if (pool === "xm" || pool === "json") return null;
    return (pool === "sfx" ? this.sfxList : this.poolList).find((i) => i.index === n)?.record ?? null;
  }

  setRecord(n: number, label: string, f: (r: M.InstrumentRecord) => M.InstrumentRecord, key: string | null = null, pool: "music" | "sfx" = "music"): void {
    const base = this.record(n, pool);
    if (!base || (pool === "music" ? this.state.deleteInstruments : this.state.deleteSfxInstruments).includes(n)) return;
    const field = pool === "sfx" ? "sfxInstruments" : "instruments";
    this.apply(
      `${pool === "sfx" ? "Sound-effect instrument" : "Instrument"} ${n}: ${label}`,
      (s) => ({ ...s, [field]: { ...s[field], [String(n)]: f(s[field][String(n)] ?? base) } }),
      key ? `${pool}:instrument:${n}:${key}` : null,
    );
  }

  revertRecord(n: number, pool: "music" | "sfx" = "music"): void {
    const field = pool === "sfx" ? "sfxInstruments" : "instruments";
    this.apply(`${pool === "sfx" ? "Sound-effect instrument" : "Instrument"} ${n} as saved`, (s) => {
      const records = { ...s[field] };
      delete records[String(n)];
      return { ...s, [field]: records };
    });
  }

  /** The songs whose XMs name pool instrument n now (`Instrument n`): the saved ones', the instruments added to them,
   * the new songs' read so far. */
  instrumentSongs(n: number): string[] {
    const out = [
      ...(this.view?.poolInstruments.find((i) => i.index === n)?.songs ?? []),
      ...Object.entries(this.state.songInstruments)
        .filter(([, ns]) => ns.includes(n))
        .map(([k]) => k),
      ...this.state.newSongs.filter((ns) => this.patterns[ns.key]?.p.instruments.some((i) => i.pool === n)).map((ns) => ns.key),
    ];
    return [...new Set(out)].filter((k) => !this.state.removeSongs.includes(k)).sort();
  }

  /** A new instrument at the end of the music pool: blank, from a pool sample, or a copy of a record. */
  newPoolInstrument(from: { sample: number } | { copy: M.InstrumentRecord } | null, label: string): void {
    const v = this.view;
    if (!v) return;
    const n = M.nextInstrument(v, this.state);
    if (n > v.limits.maxIndex) {
      editor.error = `a pattern row names an instrument by a byte: at most ${v.limits.maxIndex}`;
      return;
    }
    const rec = M.newInstrumentRecord(v, from);
    if (this.apply(`New instrument ${n} (${label})`, (s) => ({ ...s, instruments: { ...s.instruments, [String(n)]: rec } }))) {
      this.kind = "instrument";
      this.instrumentPool = "music";
      this.instrument = n;
    }
  }

  /** Why pool instrument n can't be taken out (null if it can): a song's XM names it, it is the empty one, or it is a
   * new one with new ones after it (they keep their numbers). */
  cannotDeleteInstrument(n: number): string | null {
    const list = this.poolList;
    const it = list.find((i) => i.index === n);
    if (!it || it.deleted || n === 0 || it.record.byte_00) return "the empty instrument stays";
    const who = this.instrumentSongs(n);
    if (who.length) return `${who.join(", ")} name Instrument ${n}`;
    if (it.added && list[list.length - 1]!.index !== n) return "take the new instruments after it out first";
    return null;
  }

  deletePoolInstrument(n: number): void {
    const why = this.cannotDeleteInstrument(n);
    if (why) {
      editor.error = why;
      return;
    }
    const added = this.poolList.find((i) => i.index === n)!.added;
    this.apply(`Delete instrument ${n}`, (s) => {
      const instruments = { ...s.instruments };
      delete instruments[String(n)];
      return { ...s, instruments, deleteInstruments: added ? s.deleteInstruments : [...s.deleteInstruments, n] };
    });
    const list = this.poolList;
    if (!list.some((i) => i.index === this.instrument)) this.instrument = list[list.length - 1]?.index ?? 0;
  }

  /** The music pool's samples now: the saved ones less those taken out, then the new ones; {index, length, path}. */
  get poolSamples(): { index: number; path: string; length: number; added: boolean }[] {
    const out = (this.view?.samples ?? []).filter((s) => !this.state.deleteSamples.includes(s.index)).map((s) => ({ index: s.index, path: s.path, length: this.state.samples[s.path]?.length ?? s.length, added: false }));
    for (const [k, s] of Object.entries(this.state.newSamples)) out.push({ index: Number(k), path: `samples/${k.padStart(2, "0")}.wav`, length: s.length, added: true });
    return out;
  }

  /** The instruments whose wave rows name sample idx, now. */
  sampleUsers(idx: number): number[] {
    return this.poolList.filter((i) => i.record.sample_indices.includes(idx)).map((i) => i.index);
  }

  /** The sound-effect set's samples now (sfx_samples/NN.wav: the ROM's slot N plays sample N): the saved ones less
   * those taken out, then the new ones. */
  get sfxSamples(): { index: number; path: string; length: number; added: boolean }[] {
    const out = (this.view?.sfxSamples ?? []).filter((s) => !this.state.deleteSfxSamples.includes(s.index)).map((s) => ({ index: s.index, path: s.path, length: this.state.samples[s.path]?.length ?? s.length, added: false }));
    for (const [k, s] of Object.entries(this.state.newSfxSamples)) out.push({ index: Number(k), path: `sfx_samples/${k.padStart(2, "0")}.wav`, length: s.length, added: true });
    return out;
  }

  /** The set's instruments whose wave rows name sample idx, now. */
  sfxSampleUsers(idx: number): number[] {
    return this.sfxList.filter((i) => i.record.sample_indices.includes(idx)).map((i) => i.index);
  }

  /** The sound effects that play slot n now (sfx_table.json's slot_id). */
  slotUsers(n: number): string[] {
    return this.state.sfx.entries.filter((e) => (e as { slot_id?: number }).slot_id === n).map((e) => `SFX_${e.name ?? "?"}`);
  }

  /** The set's slots a sound effect can play now: its instruments with a sample (new ones too), the rate each plays
   * it at and its sample's path. */
  get sfxSlotList(): { slot: number; sample: number; path: string; rate: number; added: boolean }[] {
    const period = this.pool?.tables.period ?? [];
    return this.sfxList
      .filter((i) => !i.deleted && !i.record.byte_00 && i.record.sample_indices[0])
      .map((i) => ({ slot: i.index, sample: i.record.sample_indices[0]!, path: `sfx_samples/${String(i.record.sample_indices[0]).padStart(2, "0")}.wav`, rate: Math.round(M.fixedRate(period, i.record) * 10) / 10, added: i.added }));
  }

  /** A new instrument at the end of the sound-effect set: blank, from a sample of the set, or a copy of a record. */
  newSfxInstrument(from: { sample: number } | { copy: M.InstrumentRecord } | null, label: string): void {
    const v = this.view;
    if (!v) return;
    const n = M.nextSfxInstrument(v, this.state);
    if (n > v.limits.maxIndex) {
      editor.error = `GAX_fx_ex names an instrument by a byte: at most ${v.limits.maxIndex}`;
      return;
    }
    const rec = M.newSfxInstrumentRecord(v, from);
    if (this.apply(`New sound-effect instrument ${n} (${label})`, (s) => ({ ...s, sfxInstruments: { ...s.sfxInstruments, [String(n)]: rec } }))) {
      this.kind = "instrument";
      this.instrumentPool = "sfx";
      this.instrument = n;
    }
  }

  /** Why sound-effect instrument n can't be taken out (null if it can): a sound effect plays it, it is the empty one,
   * or it is a new one with new ones after it. */
  cannotDeleteSfxInstrument(n: number): string | null {
    const list = this.sfxList;
    const it = list.find((i) => i.index === n);
    if (!it || it.deleted || n === 0 || it.record.byte_00) return "the empty instrument stays";
    const who = this.slotUsers(n);
    if (who.length) return `${who.slice(0, 3).join(", ")} play slot ${n}`;
    if (it.added && list[list.length - 1]!.index !== n) return "take the new instruments after it out first";
    return null;
  }

  deleteSfxInstrument(n: number): void {
    const why = this.cannotDeleteSfxInstrument(n);
    if (why) {
      editor.error = why;
      return;
    }
    const added = this.sfxList.find((i) => i.index === n)!.added;
    this.apply(`Delete sound-effect instrument ${n}`, (s) => {
      const sfxInstruments = { ...s.sfxInstruments };
      delete sfxInstruments[String(n)];
      return { ...s, sfxInstruments, deleteSfxInstruments: added ? s.deleteSfxInstruments : [...s.deleteSfxInstruments, n] };
    });
    const list = this.sfxList;
    if (!list.some((i) => i.index === this.instrument)) this.instrument = list[list.length - 1]?.index ?? 0;
  }

  /** A WAV as the sound-effect set's next sample (sfx_samples/NN.wav, NN after the set's); returns its number. */
  addSfxSample(s: M.SampleEdit): number {
    const have = [...(this.view?.sfxSamples ?? []).map((x) => x.index), ...Object.keys(this.state.newSfxSamples).map(Number)];
    const n = Math.max(0, ...have) + 1;
    if (this.apply(`New sound-effect sample ${n}`, (st) => ({ ...st, newSfxSamples: { ...st.newSfxSamples, [String(n)]: s } }))) {
      this.kind = "sample";
      this.sample = `sfx_samples/${String(n).padStart(2, "0")}.wav`;
    }
    return n;
  }

  /** Sample idx taken out of the sound-effect set (no instrument may play it). */
  deleteSfxSample(idx: number): void {
    const who = this.sfxSampleUsers(idx);
    if (who.length) {
      editor.error = `instruments ${who.join(", ")} play sample ${idx}`;
      return;
    }
    this.apply(`Delete sound-effect sample ${idx}`, (st) => {
      if (st.newSfxSamples[String(idx)]) {
        const newSfxSamples = { ...st.newSfxSamples };
        delete newSfxSamples[String(idx)];
        return { ...st, newSfxSamples };
      }
      return { ...st, deleteSfxSamples: [...st.deleteSfxSamples, idx] };
    });
    this.sample = "";
  }

  // -- a song's own XM instruments --------------------------------------------------------

  /** The JSON instruments' names now (instruments/<name>.json: the saved ones and the new ones). */
  get jsonNames(): Set<string> {
    return new Set([...(this.view?.instruments.map((i) => i.name) ?? []), ...Object.keys(this.state.jsonInstruments)]);
  }

  /** The songs' own XM instruments now (not `Instrument N`, not instruments/<name>.json's: made by the build from the
   * XM's samples and envelopes): the saved songs', the new songs' read so far, and the new ones. */
  get ownList(): { song: string; n: number; name: string; added: boolean; edited: boolean }[] {
    const out: { song: string; n: number; name: string; added: boolean; edited: boolean }[] = [];
    const json = this.jsonNames;
    for (const s of this.songs) {
      const p = this.patterns[s.key]?.p;
      const edits = this.state.xmInstruments[s.key] ?? {};
      const own = p ? p.instruments.flatMap((i, k) => (i.own ? [k + 1] : [])) : (s.info?.xm?.own ?? []);
      const added = Object.entries(edits).flatMap(([n, e]) => (e.added ? [Number(n)] : []));
      for (const n of [...own, ...added.sort((a, b) => a - b)]) {
        const e = edits[String(n)];
        const name = e?.spec.name ?? p?.instruments[n - 1]?.name ?? s.info?.xm?.instruments[n - 1] ?? "";
        // one named after a JSON instrument, with no sample, is that one (the song plays it)
        if (json.has(name) && !(e?.spec.samples.length ?? p?.instruments[n - 1]?.own?.spec.samples.length)) continue;
        out.push({ song: s.key, n, name, added: !!e?.added, edited: !!e });
      }
    }
    return out;
  }

  /** Song `song`'s own instrument n: as its XM has it (the pattern view's reading; none for a new one), and its fields,
   * its samples' sounds (or the problem the build has with one), the notes the song plays on it now and the samples
   * its key map plays on them. */
  own(song: string, n: number): { saved: MusicOwnInstrument | null; spec: M.XmSpec; sounds: (Int8Array | null)[]; problems: (string | null)[]; notes: number[]; plays: number[]; edited: boolean; added: boolean } | null {
    const saved = this.patterns[song]?.p.instruments[n - 1]?.own ?? null;
    const e = this.state.xmInstruments[song]?.[String(n)];
    if (!saved && !e?.added) return null;
    const spec = e?.spec ?? M.xmSpec(saved!.spec);
    const sounds = spec.samples.map((_, i) => {
      const edit = e?.sounds?.[String(i)];
      if (edit) return M.parseWav8(M.fromBase64(edit.wav)).data;
      const s = saved?.sounds[i];
      return s && !s.problem ? new Int8Array(M.fromBase64(s.data).buffer) : null;
    });
    const problems = spec.samples.map((_, i) => (e?.sounds?.[String(i)] ? null : (saved?.sounds[i]?.problem ?? null)));
    const v = this.viewOf(song);
    const notes = v ? [...(M.playedNotes(v.p, v.cells).get(n) ?? [])].sort((a, b) => a - b) : (saved?.notes ?? []);
    return { saved, spec, sounds, problems, notes, plays: M.keymapPlays(spec, notes), edited: !!e, added: !!e?.added };
  }

  /** Song `song`'s own instrument n shown, with the sample the build makes it from. */
  selectOwn(song: string, n: number): void {
    this.kind = "instrument";
    this.instrumentPool = "xm";
    this.xmSong = song;
    this.instrument = n;
    this.ownSample = this.own(song, n)?.plays[0] ?? 0;
    void editor.run(`Reading ${song}`, async () => {
      await this.loadPatterns(song);
      if (this.xmSong === song && this.instrument === n) this.ownSample = this.own(song, n)?.plays[0] ?? 0;
    });
  }

  /** An edit of song `song`'s own instrument n's fields (one step of the undo stack). */
  setOwn(song: string, n: number, label: string, f: (x: M.XmInstrumentEdit) => M.XmInstrumentEdit, key: string | null = null): void {
    const o = this.own(song, n);
    if (!o) return;
    this.apply(
      `${song}'s instrument ${n}: ${label}`,
      (s) => {
        const was = s.xmInstruments[song]?.[String(n)] ?? { spec: o.spec };
        return { ...s, xmInstruments: { ...s.xmInstruments, [song]: { ...(s.xmInstruments[song] ?? {}), [String(n)]: f(was) } } };
      },
      key ? `xm:${song}:${n}:${key}` : null,
    );
  }

  revertOwn(song: string, n: number): void {
    this.apply(`${song}'s instrument ${n} as saved`, (s) => {
      const mine = { ...(s.xmInstruments[song] ?? {}) };
      delete mine[String(n)];
      const xmInstruments = { ...s.xmInstruments, [song]: mine };
      if (!Object.keys(mine).length) delete xmInstruments[song];
      return { ...s, xmInstruments };
    });
  }

  /** Why song `song` can't have a new own instrument (null if it can): a ROM song built with its recorded scaffold
   * plays only the pool's (crashdata.audio.gax.rom_song), and an XM has at most 128. */
  cannotAddOwn(song: string): string | null {
    const p = this.patterns[song]?.p;
    if (!p) return `${song} isn't read yet`;
    if (p.scaffold && !this.state.structures[song]) return `${song} is built with the ROM's scaffold, which plays only the pool's instruments: change its structure first (it is then built from its XM)`;
    const most = this.view?.limits.xmInstruments ?? 128;
    if (this.xmInstruments(song).length >= most) return `an XM has at most ${most} instruments`;
    return null;
  }

  /** A new instrument at the end of song `song`'s XM: blank (a name, no sound yet), a copy of another song's own
   * instrument (its fields and its samples' sounds as the build reads them), one from a WAV, or one named after a
   * JSON instrument (the song then plays that one). Selects it, unless it is a JSON's. */
  async newOwn(song: string, from: { blank: string } | { copy: { song: string; n: number } } | { wav: M.SampleEdit; name: string } | { json: string }): Promise<void> {
    if (!this.patterns[song]) await editor.run(`Reading ${song}`, () => this.loadPatterns(song));
    const why = this.cannotAddOwn(song);
    if (why) {
      editor.error = why;
      return;
    }
    let edit: M.XmInstrumentEdit;
    let label: string;
    if ("blank" in from) [edit, label] = [{ spec: M.newXmSpec(from.blank, null), added: true }, "blank"];
    else if ("json" in from) [edit, label] = [{ spec: M.newXmSpec(from.json, null), added: true }, `instruments/${from.json}.json's`];
    else if ("wav" in from) [edit, label] = [{ spec: M.newXmSpec(from.name, from.wav.rate), sounds: { "0": from.wav }, added: true }, "from a WAV"];
    else {
      const o = this.own(from.copy.song, from.copy.n);
      if (!o) return;
      const bad = o.problems.findIndex((p) => p);
      if (bad >= 0) {
        editor.error = `sample ${bad}: ${o.problems[bad]}`;
        return;
      }
      const sounds = Object.fromEntries(o.sounds.map((d, i) => [String(i), { wav: M.toBase64(M.wavFile(d!, 8363)), length: d!.length, rate: 8363 }]));
      const spec = { ...structuredClone(o.spec), name: `${o.spec.name} copy`.slice(0, M.XM_NAME) };
      [edit, label] = [{ spec, sounds, added: true }, `a copy of ${from.copy.song}'s ${from.copy.n}`];
    }
    const n = this.xmInstruments(song).length + 1;
    if (this.apply(`${song}'s new instrument ${n} (${label})`, (s) => ({ ...s, xmInstruments: { ...s.xmInstruments, [song]: { ...(s.xmInstruments[song] ?? {}), [String(n)]: edit } } })) && !("json" in from))
      this.selectOwn(song, n);
  }

  /** Why song `song`'s instrument n can't be taken out (null if it can): only a new one, the last of the song's list,
   * that no cell plays. */
  cannotDeleteOwn(song: string, n: number): string | null {
    if (!this.state.xmInstruments[song]?.[String(n)]?.added) return "only a new instrument is taken out here (the XM's are a tracker's to take out)";
    if (n !== this.xmInstruments(song).length) return "take the instruments after it out first";
    const v = this.viewOf(song);
    if (v && v.cells.some((c) => c.some((b, i) => i % M.CELL === 1 && b === n))) return `a cell of ${song} plays it`;
    return null;
  }

  deleteOwn(song: string, n: number): void {
    const why = this.cannotDeleteOwn(song, n);
    if (why) {
      editor.error = why;
      return;
    }
    this.apply(`Take ${song}'s new instrument ${n} out`, (s) => {
      const mine = { ...(s.xmInstruments[song] ?? {}) };
      delete mine[String(n)];
      const xmInstruments = { ...s.xmInstruments, [song]: mine };
      if (!Object.keys(mine).length) delete xmInstruments[song];
      return { ...s, xmInstruments };
    });
    if (this.instrumentPool === "xm" && this.xmSong === song && this.instrument === n) [this.instrumentPool, this.instrument] = ["music", 1];
  }

  /** Song `song`'s own instrument n played as a song plays it: the pool instrument the build makes of it. */
  async playOwn(song: string, n: number, note: number, hold: number): Promise<void> {
    const pool = await this.builtPool();
    if (!pool) return;
    const idx = pool.xmPool[song]?.[n - 1];
    if (idx === null || idx === undefined) {
      editor.error = `no cell of ${song} plays its instrument ${n}: the build doesn't add it to the pool`;
      return;
    }
    await this.playInstrument(idx, note, hold);
  }

  // -- instruments/<name>.json ------------------------------------------------------------

  /** The JSON instruments now: the saved ones, then the new ones. */
  get jsonList(): { name: string; added: boolean; edited: boolean }[] {
    const saved = (this.view?.instruments ?? []).map((i) => ({ name: i.name, added: false, edited: !!this.state.jsonInstruments[i.name] }));
    const added = Object.entries(this.state.jsonInstruments).flatMap(([name, e]) => (e.added ? [{ name, added: true, edited: true }] : []));
    return [...saved, ...added];
  }

  /** JSON instrument `name` now: its value, its sound (as the build reads it) or the problem with it, the songs whose
   * XMs name it (an unsaved new instrument of that name too) and the sound effects that name it. */
  json(name: string): { saved: MusicJsonInstrument | null; json: Instrument; sound: { data: Int8Array; rate: number } | null; problem: string | null; songs: string[]; sfx: string[]; edited: boolean; added: boolean } | null {
    const saved = this.view?.instruments.find((i) => i.name === name) ?? null;
    const e = this.state.jsonInstruments[name];
    if (!saved && !e) return null;
    let sound: { data: Int8Array; rate: number } | null = null;
    let problem: string | null = null;
    if (e?.wav) sound = M.parseWav8(M.fromBase64(e.wav.wav));
    else if (saved && "data" in saved.sound) sound = { data: new Int8Array(M.fromBase64(saved.sound.data).buffer), rate: saved.sound.rate };
    else problem = saved && "problem" in saved.sound ? saved.sound.problem : "no sound yet";
    const added = Object.entries(this.state.xmInstruments).flatMap(([song, edits]) => (Object.values(edits).some((x) => x.added && x.spec.name === name) ? [song] : []));
    const songs = [...(saved?.songs ?? []), ...added.filter((k) => !saved?.songs.includes(k))];
    const sfx = this.state.sfx.entries.filter((x) => (x as { instrument?: string }).instrument === name).map((x) => `SFX_${x.name ?? "?"}`);
    return { saved, json: e?.json ?? saved!.data, sound, problem, songs: songs.filter((k) => this.songKeys.includes(k)), sfx, edited: !!e, added: !!e?.added };
  }

  selectJson(name: string): void {
    this.kind = "instrument";
    this.instrumentPool = "json";
    this.jsonName = name;
  }

  /** An edit of JSON instrument `name` (one step of the undo stack). */
  setJson(name: string, label: string, f: (j: M.JsonInstrumentEdit) => M.JsonInstrumentEdit, key: string | null = null): void {
    const j = this.json(name);
    if (!j) return;
    this.apply(`instruments/${name}.json: ${label}`, (s) => ({ ...s, jsonInstruments: { ...s.jsonInstruments, [name]: f(s.jsonInstruments[name] ?? { json: j.json }) } }), key ? `json:${name}:${key}` : null);
  }

  revertJson(name: string): void {
    this.apply(`instruments/${name}.json as saved`, (s) => {
      const jsonInstruments = { ...s.jsonInstruments };
      delete jsonInstruments[name];
      return { ...s, jsonInstruments };
    });
  }

  /** Why `name` can't be a new JSON instrument's (null if it can). */
  cannotNameJson(name: string): string | null {
    if (!M.KEY.test(name)) return "a name is lower case letters, digits and _";
    if (this.jsonNames.has(name) || name in this.state.newInstruments) return `there is an instrument ${name} already`;
    return null;
  }

  /** A new JSON instrument with a WAV (samples/<name>.wav, C-4 playing it at the WAV's rate). */
  newJson(name: string, wav: M.SampleEdit): void {
    const why = this.cannotNameJson(name);
    if (why) {
      editor.error = why;
      return;
    }
    const json: Instrument = { comment: "an instrument (the music editor's)", sample: `samples/${name}.wav` };
    if (this.apply(`New instrument instruments/${name}.json`, (s) => ({ ...s, jsonInstruments: { ...s.jsonInstruments, [name]: { json, wav, added: true } } }))) this.selectJson(name);
  }

  /** JSON instrument `name` played: as a song plays it if a song's XM names it (the pool instrument the build makes of
   * it), else as a sound effect plays it. */
  async playJson(name: string, note: number, hold: number): Promise<void> {
    const j = this.json(name);
    if (!j) return;
    const pool = await this.builtPool();
    if (!pool) return;
    for (const song of j.songs) {
      if (!this.patterns[song]) await this.loadPatterns(song);
      const n = this.xmInstruments(song).findIndex((i) => i.name === name) + 1;
      const idx = n ? pool.xmPool[song]?.[n - 1] : null;
      if (idx !== null && idx !== undefined) return this.playInstrument(idx, note, hold);
    }
    if (j.sfx.length) return this.playSfxEntry({ instrument: name, pitch_offset: 0, volume: 256 } as unknown as M.SfxEntry, "instrument");
    editor.error = `no cell of a song and no sound effect plays instruments/${name}.json: the build doesn't add it to a pool`;
  }

  /** A WAV as the music pool's next sample (samples/NN.wav, NN after the pool's); returns its number. */
  addPoolSample(s: M.SampleEdit): number {
    const have = [...(this.view?.samples ?? []).map((x) => x.index), ...Object.keys(this.state.newSamples).map(Number)];
    const n = Math.max(0, ...have) + 1;
    if (this.apply(`New sample ${n}`, (st) => ({ ...st, newSamples: { ...st.newSamples, [String(n)]: s } }))) {
      this.kind = "sample";
      this.sample = `samples/${String(n).padStart(2, "0")}.wav`;
    }
    return n;
  }

  /** Sample idx taken out of the music pool (no instrument may play it). */
  deletePoolSample(idx: number): void {
    const who = this.sampleUsers(idx);
    if (who.length) {
      editor.error = `instruments ${who.join(", ")} play sample ${idx}`;
      return;
    }
    this.apply(`Delete sample ${idx}`, (st) => {
      if (st.newSamples[String(idx)]) {
        const newSamples = { ...st.newSamples };
        delete newSamples[String(idx)];
        return { ...st, newSamples };
      }
      return { ...st, deleteSamples: [...st.deleteSamples, idx] };
    });
    this.sample = "";
  }

  /** Instrument n played as a song plays it: `note` (XM's) on a channel, a key off after `hold` rows (speed 6). */
  async playInstrument(n: number, note: number, hold: number): Promise<void> {
    if (!this.pool) return;
    const pool = await this.builtPool();
    if (!pool) return;
    const rows = Math.max(hold + 24, 32);
    const { music, sfx } = this.pools(pool);
    const p = new GaxPlayer(pool.tables, music, sfx, M.instrumentSong(n, note, hold, rows));
    p.setMusicVolume(-1, this.musicVolume);
    this.output.start(p, { frames: rows * 6 });
    this.playing = "instrument";
  }

  /** Sound-effect instrument n played as PlaySfx plays a sound effect on it (priority 0, volume 1.0), over the song
   * chosen for the sound effects. */
  async playSfxInstrument(n: number): Promise<void> {
    await this.playSfxEntry({ slot_id: n, pitch_offset: 0, volume: 256 } as unknown as M.SfxEntry & { slot_id: number }, "instrument");
  }

  /** A player of the song, run on to order position `from` (GAX2 has no seek: the frames before it are mixed and dropped). */
  async songPlayer(key: string, from = 0): Promise<GaxPlayer | undefined> {
    if (!this.pool) return undefined;
    const built = await this.program(key);
    if (!built) return undefined;
    const { song, pool } = built;
    const { music, sfx } = this.pools(pool);
    const p = new GaxPlayer(pool.tables, music, sfx, song);
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
    await this.playSfxEntry(e, "sfx");
  }

  private async playSfxEntry(e: M.SfxEntry & { slot_id?: number; instrument?: string }, what: "sfx" | "instrument"): Promise<void> {
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
    this.playing = what;
  }

  /** The tree's samples read so far (sfx_samples/NN.wav, ...), as saved. */
  wavs = $state.raw<Record<string, { data: Int8Array; rate: number }>>({});

  /** A sample's sound now: its unsaved replacement (or a new instrument's), or the file's. */
  sound(path: string): { data: Int8Array; rate: number } | null {
    const added = /^(sfx_)?samples\/(\d+)\.wav$/.exec(path);
    const edit = this.state.samples[path] ?? this.state.newInstruments[path.replace(/^sfx_samples\/(.*)\.wav$/, "$1")] ?? (added ? (added[1] ? this.state.newSfxSamples : this.state.newSamples)[String(Number(added[2]))] : undefined);
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
    const lk = this.lengthKey(key);
    if (lk in this.songLength) return;
    // a burst of edits is measured once, after it
    await new Promise((r) => setTimeout(r, 300));
    if (lk !== this.lengthKey(key)) return;
    const p = await this.songPlayer(key);
    if (!p) return;
    const scratch = new Int8Array(p.frames);
    let f = 0;
    for (; f < 60 * 60 * 20 && !p.info.songEnded; f++) p.frame(scratch);
    this.songLength = { ...this.songLength, [lk]: p.info.songEnded ? { loop: f, order: p.song.loopOrder } : null };
  }

  private ids = new WeakMap<object, number>();
  private nextId = 1;

  private idOf(o: object | undefined): number {
    if (!o) return 0;
    let v = this.ids.get(o);
    if (!v) this.ids.set(o, (v = this.nextId++));
    return v;
  }

  /** What a song's length is measured for: the song with its edits now (its structure, its cells, the new songs). */
  lengthKey(key: string): string {
    return `${key}:${this.idOf(this.state.structures[key])}:${this.idOf(this.state.cells[key])}:${this.idOf(this.state.newSongs)}`;
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

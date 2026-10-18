// A typed client of the editors' backend, tools/editor_server.py (#1167,
// docs/editors.md "The editor's backend"). The browser editor calls it on
// its own origin; the desktop app gives it the backend's URL and token.
import type { Cutscenes, Music, Room, SchemaFiles, SchemaName, SfxTable } from "./generated/index.ts";

/** One problem with the data (crashdata.common.diagnostics.Diagnostic). */
export interface Diagnostic {
  file: string | null;
  /** a JSON pointer (RFC 6901) into the file, or null */
  pointer: string | null;
  line: number | null;
  col: number | null;
  severity: "error" | "warning";
  code: string;
  message: string;
}

/** A data tool's `--json` report (docs/editors.md, "Errors"). */
export interface Report {
  version: number;
  tool: string;
  ok: boolean;
  diagnostics: Diagnostic[];
  output: string;
}

export interface PreviewReport extends Report {
  preview?: Record<string, unknown>;
  /** a headless preview's picture, on this server */
  image?: string | null;
  /** a headless cutscene's pictures, one a key press (on this server) */
  images?: string[];
}

export interface PreviewStarted {
  ok: true;
  started: true;
  pid: number;
  command: string[];
  log: string;
}

export interface PreviewOptions {
  /** gLevelTable's index, a constant, a name or a room folder */
  level?: string | number;
  /** an index in the level's "rooms" or a folder */
  room?: string | number;
  /** run headless and give back a picture, instead of opening the game's window */
  headless?: boolean;
  frames?: number;
  noBuild?: boolean;
  /** a cutscene to play at boot instead of a level (its id or constant) */
  cutscene?: string | number;
  /** with `cutscene`: its text's language (en, fr, de, es, it, nl) */
  language?: string;
}

export interface Health {
  ok: boolean;
  version: number;
  tree: string;
  python: string;
  /** whether the server has the built web editor to serve */
  editor: boolean;
}

export interface Project {
  /** "xs", "n2", or "mod:<name>" */
  id: string;
  kind: "game" | "mod";
  game: "xs" | "n2";
  gameTitle: string;
  title: string;
  /** the project's tree, from the server's tree's root */
  root: string;
  /** a mod's tree exists (it has been applied) */
  ready: boolean;
  mod?: string;
}

export interface SchemaIndex {
  $comment?: string;
  schemas: { schema: SchemaName; fileMatch: string[] }[];
}

export interface FileEntry {
  path: string;
  schema: SchemaName;
}

export interface DataFile<N extends SchemaName = SchemaName> {
  path: string;
  schema: N;
  data: SchemaFiles[N];
}

export interface SaveResult {
  ok: true;
  /** the files that changed (a file saved as it was isn't written) */
  saved: string[];
  /** the model's checks after the save (warnings, and errors that were there before) */
  diagnostics: Diagnostic[];
}

export interface ApplyResult {
  ok: true;
  project: Project;
  report: Report;
}

/** A room layer's picture as the level editor draws it (crashdata.levels.room_edit.layer_info). */
export interface RoomLayerInfo {
  /** the picture, in the room's folder */
  map: string;
  /** in pixels: the chunk grid's (128x64 pixels a chunk) */
  width: number;
  height: number;
  /** [width, height] in 8x8 cells */
  cells: [number, number];
  /** the scrollable part, in cells */
  sizeTiles: [number, number];
  /** the parallax factors (Q8: 256 is the camera's speed) */
  scale: [number, number];
  /** BGnCNT's priority: 0 is drawn over 3 */
  priority: number;
  tileset: string | null;
  /** 8 for BG0, 4 for BG1-3, null for the collision layer */
  bpp: 8 | 4 | null;
}

/** A collision value's colour (collision_legend.json). */
export interface LegendEntry {
  value: number;
  /** "#RRGGBB" */
  colour: string;
  name: string;
}

/** An entity type (entity_types.json's record). */
export interface EntityTypeInfo {
  type: number;
  name: string | null;
  spawner?: string;
  params?: string;
  enemy_kind?: number;
  comment?: string;
}

/** The sprite frame the editor draws an entity type with. */
export interface SpriteInfo {
  bank: number;
  frame: number;
  /** an OBJ palette id, or null: the frame's own */
  palette: number | null;
  /** the sprite's position in the frame's picture */
  origin: [number, number];
  size: [number, number];
}

/** GET /api/projects/ID/room: a room, as the level editor opens it. */
export interface RoomView {
  path: string;
  room: Room;
  /** the room's 256 colours, BGR555 (bit 15 kept, docs/graphics.md "Palettes") */
  palette: number[];
  layers: Partial<Record<"bg0" | "bg1" | "bg2" | "bg3" | "collision", RoomLayerInfo>>;
  legend: LegendEntry[];
  entityTypes: EntityTypeInfo[];
  /** by entity type (its number, as a string) */
  sprites: Record<string, SpriteInfo>;
}

/** A layer's data: a BG picture's palette indices (a byte a pixel), or the collision layer's cells (a u16 a cell). */
export interface RoomLayerData {
  width: number;
  height: number;
  data: Uint8Array | Uint16Array;
}

/** GET /api/projects/ID/room/tiles: a BG layer's tile set and its cells in it. */
export interface RoomTiles {
  tileset: string;
  bpp: 8 | 4;
  count: number;
  /** base64: 64 bytes a tile, a pixel a byte (4bpp: 0-15) */
  tiles: string;
  /** each tile's palette bank where it is first drawn */
  banks: number[];
  /** base64 of the layer's cells, u16 LE: BG0 tile | flip << 14, BG1-3 tile | flip << 10 | bank << 12 */
  cells: string;
}

/** POST /api/projects/ID/room/save: a room's edits, saved together. */
export interface RoomSave {
  path: string;
  room?: Room;
  /** per BG layer: [cx, cy, base64 of the cell's 64 palette indices] */
  layers?: Partial<Record<"bg0" | "bg1" | "bg2" | "bg3", [number, number, string][]>>;
  /** [cx, cy, value] */
  collision?: [number, number, number][];
  /** 256 BGR555 entries: every layer picture's palette */
  palette?: number[];
}

/** A string of a cutscene page in the six languages (the template's msgid, each .po's msgstr). */
export interface CutsceneString {
  /** cutsceneNN.pageP.I */
  key?: string;
  en: string;
  fr: string;
  de: string;
  es: string;
  it: string;
  nl: string;
}

/** A cutscene picture of the graphics manifest. */
export interface CutscenePictureInfo {
  /** from the graphics directory: cutscenes/NN_<name>.png */
  path: string;
  symbol: string;
  class: string;
  palette: string;
  about?: string;
  /** the ROM's (it has an address) or a new one */
  rom: boolean;
}

/** The small font as PlayCutscene draws the text (crashdata.cutscenes.edit.font). */
export interface CutsceneFont {
  /** the characters (Latin-1 codes): glyph g is chars[g] */
  chars: number[];
  /** each glyph's advance, OBJ shape (0 8x8, 1 16x8, 2 8x16) and Y offset (OAM's 8 bits) */
  widths: number[];
  shapes: number[];
  yOffsets: number[];
  space: number;
  lineHeight: number;
  /** each glyph's cell, cell x cell pixels */
  cell: number;
  /** base64: the glyphs' cells, a byte (a colour index 0-15) a pixel */
  glyphs: string;
  /** its 16 colours, BGR555 */
  palette: number[];
}

/** GET /api/projects/ID/cutscenes: the cutscenes, as the cutscene editor opens them. */
export interface CutscenesView {
  cutscenes: Cutscenes;
  pictures: CutscenePictureInfo[];
  /** every page's strings, by page key (cutsceneNN.pageP) */
  pages: Record<string, CutsceneString[]>;
  /** the pages in the template's order */
  pageOrder: string[];
  languages: { code: string; name: string }[];
  songs: { name: string; comment: string }[];
  sfx: { name: string; comment: string }[];
  font: CutsceneFont;
  /** the text box: PlayCutscene's */
  box: { x: number; y: number; w: number; h: number };
  screen: [number, number];
  /** the text's encoding (Python's name: cp1252) */
  encoding: string;
  /** what plays each cutscene, by constant: code, levels.json's on_clear */
  references: Record<string, string[]>;
  /** the slide records' number fields' ranges */
  numberFields: Record<string, [number, number]>;
}

/** A cutscene picture's data: 240x160, its 256 colours. */
export interface CutscenePictureData {
  width: number;
  height: number;
  /** 256 BGR555 entries */
  palette: number[];
  /** a byte a pixel */
  pixels: Uint8Array;
}

/** POST /api/projects/ID/cutscenes/import's answer: a PNG made into a cutscene picture. */
export interface CutsceneImport {
  width: number;
  height: number;
  palette: number[];
  /** base64, a byte a pixel */
  pixels: string;
  /** the colours it uses */
  colours: number;
  /** whether every pixel kept its colour */
  exact: boolean;
  /** what was done */
  how: string;
}

/** POST /api/projects/ID/cutscenes/save: the cutscene editor's edits, saved together. */
export interface CutscenesSave {
  cutscenes?: Cutscenes;
  /** pages whose strings change (null: the page goes) */
  pages?: Record<string, Omit<CutsceneString, "key">[] | null>;
  /** pictures whose pixels or colours change: base64 pixels */
  pictures?: Record<string, { palette: number[]; pixels: string }>;
  /** new pictures (their manifest entries are made) */
  newPictures?: { path: string; about?: string; palette: number[]; pixels: string }[];
}

/** What plays a song: a level theme (levels.json), a screen (music.json), a cutscene slide record. */
export interface MusicSongUse {
  kind: "theme" | "screen" | "cutscene";
  /** THEME_<NAME>, the screen, the slide record's name */
  name: string;
  /** the theme's name, the screen's comment, the cutscenes that show the record */
  comment: string;
}

/** A song of the song table, as the music editor lists it. */
export interface MusicSongInfo {
  key: string;
  /** its song id (gSongTable's index) and SONG_<KEY> */
  id: number;
  constant: string;
  comment: string;
  /** one of the ROM's (its scaffold is the manifest's) or a new one */
  rom: boolean;
  /** its XM, from the tree's root */
  file: string;
  uses: MusicSongUse[];
  /** the XM in brief (absent with `error`) */
  xm?: { name: string; channels: number; patterns: number; order: number; speed: number; bpm: number; instruments: string[] };
  error?: string;
  mixRate?: number;
  volume?: number;
  halfRateFx?: number;
  title?: string;
  artist?: string | null;
}

/** A sound effect: sfx_table.json's entry, its id and where it is named. */
export interface MusicSfx {
  id: number;
  constant: string | null;
  name?: string;
  slot_id?: number;
  instrument?: string;
  pitch_offset: number;
  volume: number;
  comment?: string;
  /** code (engine/) and cutscene slides that name SFX_<NAME> */
  uses: string[];
}

/** A sound-effect instrument of the set: slot N plays sample N, at one rate. */
export interface MusicSfxSlot {
  slot: number;
  sample: number;
  /** from the sound directory: sfx_samples/NN.wav */
  path: string;
  length: number;
  /** the rate the WAV declares (cosmetic: the build doesn't read it) */
  wavRate: number | null;
  /** the rate GAX2 plays it at (gGaxPeriodTable / 2048), Hz */
  rate: number;
  fixedPitch: boolean;
  /** the shortest sample its instrument's loop and start can take */
  minLength: number;
}

/** A sample of the music pool (samples/NN.wav) and what plays it. */
export interface MusicSample {
  index: number;
  path: string;
  length: number;
  rate: number;
  minLength: number;
  instruments: number[];
  songs: string[];
}

/** GET /api/projects/ID/music: the songs, the screens' and themes' songs, the sound effects, the samples. */
export interface MusicView {
  songs: MusicSongInfo[];
  screens: { screen: string; song: string; comment?: string }[];
  themes: { constant: string; name: string; song: string }[];
  sfx: MusicSfx[];
  sfxNoneComment: string;
  sfxSlots: MusicSfxSlot[];
  samples: MusicSample[];
  /** the new instruments, instruments/<name>.json */
  instruments: { name: string; file: string; data: Record<string, unknown> }[];
  mixRates: number[];
  limits: { maxIndex: number; maxSampleLength: number; romSfxSlots: number };
  files: { manifest: string; music: string; sfxTable: string; levels: string };
}

/** GAX2's tables (lib/gax/data/gax_tables.c). */
export interface MusicTables {
  /** gGaxPeriodTable: Hz * 2048 per 1/32 semitone */
  period: number[];
  /** gGaxVibratoTable */
  vibrato: number[];
  /** gGaxMixRates' rates */
  mixRates: number[];
}

/** struct GaxInstrumentRow: a wave row of an instrument. */
export interface MusicWaveRow {
  sweep: number;
  pingPong: number;
  start: number;
  sweepMin: number;
  sweepMax: number;
  sweepLen: number;
  sweepStep: number;
  sweepRate: number;
  tune: number;
}

/** struct GaxEnvelope (0xff: no sustain or loop point). */
export interface MusicEnvelope {
  sustain: number;
  loopStart: number;
  loopEnd: number;
  points: { pos: number; slope: number; value: number }[];
}

/** struct GaxChannelInstrument. */
export interface MusicInstrument {
  empty: number;
  waveIdx: number[];
  vibratoDelay: number;
  vibratoDepth: number;
  vibratoSpeed: number;
  rows: MusicWaveRow[];
  envelope: MusicEnvelope | null;
  seqSpeed: number;
  /** struct GaxInstrumentSeqEntry: fx are `cmd << 8 | param` */
  seq: { note: number; fixedPitch: number; wave: number; fx: number[] }[];
}

/** A pool of instruments and samples (the music's or the sound effects'). */
export interface MusicPoolData {
  /** by number; null where there is none */
  instruments: (MusicInstrument | null)[];
  /** sample number -> [offset, length] in `samples` */
  waves: Record<string, [number, number]>;
  /** base64: the samples, signed 8-bit */
  samples: string;
}

/** GET /api/projects/ID/music/pool: what the player needs besides a song. */
export interface MusicPool {
  tables: MusicTables;
  music: MusicPoolData;
  /** the sound-effect set, its song header's volume and voice types */
  sfx: MusicPoolData & { volume: number; voiceTypes: number };
  /** a new sound effect's instrument number, by instruments/<name>.json's name */
  sfxSlots: Record<string, number>;
}

/** GET/POST /api/projects/ID/music/song: a song as GAX2_init sees it (GaxSongData, its channels' data). */
export interface MusicSong {
  key: string;
  id: number;
  channelCount: number;
  patternRows: number;
  orderCount: number;
  loopOrder: number;
  /** Q8 */
  volume: number;
  mixRate: number;
  numSfx: number;
  halfRateFx: number;
  /** the mixer's data: the channels mixed before the echo, then three taps' {delay ms, gain} */
  dsp: number[];
  /** base64: the packed pattern streams */
  patterns: string;
  /** each channel's order list: [the pattern's offset in `patterns`, transpose] */
  orders: [number, number][][];
}

/** A cell edit of a song's XM: [note, instrument, volume column, effect, parameter]. */
export interface MusicCellEdit {
  pattern: number;
  row: number;
  channel: number;
  cell: [number, number, number, number, number];
}

/** GET /api/projects/ID/music/patterns: a song's XM for the pattern view. */
export interface MusicPatterns {
  key: string;
  rom: boolean;
  name: string;
  channels: number;
  /** each XM pattern's rows */
  rows: number[];
  /** base64 per pattern: 5 bytes a cell, row by row */
  cells: string[];
  order: number[];
  restart: number;
  speed: number;
  bpm: number;
  /** the XM's instruments: their names and the pool instrument an `Instrument N` is */
  instruments: { name: string; pool: number | null }[];
  /** order position p plays XM pattern playOrder[p] */
  playOrder: number[];
  loopOrder: number;
  /** a ROM song's: the pattern group of each channel's order position (one group, one copy) */
  groups?: number[][];
  transposes?: number[][];
  /** the groups whose ROM bytes are kept while their notes and instruments are the ROM's */
  recorded?: number[];
}

/** POST /api/projects/ID/music/import's answer: a WAV made into a GAX2 sample. */
export interface MusicImport {
  /** base64: an 8-bit mono WAV */
  wav: string;
  length: number;
  rate: number;
  source: { channels: number; bits: number; rate: number; float: boolean };
  peak: number;
  clipped: number;
  how: string;
}

/** POST /api/projects/ID/music/save: the music editor's edits, saved together. */
export interface MusicSave {
  sfxTable?: SfxTable;
  music?: Music;
  /** THEME_<NAME> -> song key */
  themes?: Record<string, string>;
  songComments?: Record<string, string>;
  newSongs?: { key: string; xm: string; comment?: string }[];
  removeSongs?: string[];
  patterns?: Record<string, MusicCellEdit[]>;
  /** samples/NN.wav or sfx_samples/NN.wav -> base64 8-bit mono WAV */
  samples?: Record<string, string>;
  /** new sound-effect instruments: instruments/<name>.json and sfx_samples/<name>.wav */
  newInstruments?: { name: string; wav: string; rate?: number; volume?: number; comment?: string }[];
}

/** A JSON Schema, as the backend serves it. */
export type JsonSchema = Record<string, unknown>;

/** The backend's answer to a request it refused. */
export class ApiError extends Error {
  readonly status: number;
  readonly diagnostics: Diagnostic[];

  constructor(status: number, message: string, diagnostics: Diagnostic[] = []) {
    super(message);
    this.name = "ApiError";
    this.status = status;
    this.diagnostics = diagnostics;
  }
}

export interface ClientOptions {
  /** the backend's URL ("" or omitted: the page's own origin) */
  baseUrl?: string;
  /** the desktop app's token (tools/editor_server.py --token) */
  token?: string;
  fetch?: typeof fetch;
}

export class EditorClient {
  readonly baseUrl: string;
  readonly token: string | undefined;
  private readonly fetchImpl: typeof fetch;

  constructor(options: ClientOptions = {}) {
    this.baseUrl = (options.baseUrl ?? "").replace(/\/+$/, "");
    this.token = options.token;
    this.fetchImpl = options.fetch ?? ((input, init) => globalThis.fetch(input, init));
  }

  /** A URL of the backend, with its query. */
  url(path: string, query: Record<string, string | undefined> = {}): string {
    const q = Object.entries(query)
      .filter((kv): kv is [string, string] => kv[1] !== undefined)
      .map(([k, v]) => `${encodeURIComponent(k)}=${encodeURIComponent(v)}`)
      .join("&");
    return `${this.baseUrl}${path}${q ? `?${q}` : ""}`;
  }

  private headers(write: boolean): Record<string, string> {
    const h: Record<string, string> = { Accept: "application/json" };
    if (write) {
      h["Content-Type"] = "application/json";
      h["X-Crash-Editor"] = "1";
    }
    if (this.token) h["Authorization"] = `Bearer ${this.token}`;
    return h;
  }

  async request<T>(method: "GET" | "PUT" | "POST", path: string, body?: unknown, query?: Record<string, string | undefined>): Promise<T> {
    const write = method !== "GET";
    const res = await this.fetchImpl(this.url(path, query), {
      method,
      headers: this.headers(write),
      body: body === undefined ? undefined : JSON.stringify(body),
    });
    const text = await res.text();
    let json: unknown = undefined;
    try {
      json = text ? JSON.parse(text) : undefined;
    } catch {
      throw new ApiError(res.status, `${method} ${path}: not JSON (${res.status})`);
    }
    if (!res.ok) {
      const e = (json ?? {}) as { error?: string; diagnostics?: Diagnostic[] };
      throw new ApiError(res.status, e.error ?? `${method} ${path}: ${res.status}`, e.diagnostics ?? []);
    }
    return json as T;
  }

  /** A binary answer (a room's layer). */
  async requestBytes(path: string, query?: Record<string, string | undefined>): Promise<ArrayBuffer> {
    const res = await this.fetchImpl(this.url(path, query), { method: "GET", headers: this.headers(false) });
    if (!res.ok) {
      let e: { error?: string; diagnostics?: Diagnostic[] } = {};
      try {
        e = (await res.json()) as typeof e;
      } catch {
        // not JSON
      }
      throw new ApiError(res.status, e.error ?? `GET ${path}: ${res.status}`, e.diagnostics ?? []);
    }
    return res.arrayBuffer();
  }

  private project(id: string, rest: string): string {
    return `/api/projects/${encodeURIComponent(id)}/${rest}`;
  }

  health(): Promise<Health> {
    return this.request("GET", "/api/health");
  }

  async projects(): Promise<Project[]> {
    return (await this.request<{ projects: Project[] }>("GET", "/api/projects")).projects;
  }

  /** Adds a mod (a folder with a mod.py) to the projects. */
  addMod(dir: string): Promise<Project> {
    return this.request("POST", "/api/projects", { mod: dir });
  }

  /** Applies a mod to its tree (build/mods/<name>), as `make mod`: the tree's edits are lost. */
  applyMod(id: string): Promise<ApplyResult> {
    return this.request("POST", this.project(id, "apply"), {});
  }

  schemaIndex(): Promise<SchemaIndex> {
    return this.request("GET", "/api/schemas");
  }

  schema(name: SchemaName): Promise<JsonSchema> {
    return this.request("GET", `/api/schemas/${encodeURIComponent(name)}`);
  }

  async files(id: string): Promise<FileEntry[]> {
    return (await this.request<{ files: FileEntry[] }>("GET", this.project(id, "files"))).files;
  }

  read<N extends SchemaName = SchemaName>(id: string, path: string): Promise<DataFile<N>> {
    return this.request("GET", this.project(id, "file"), undefined, { path });
  }

  /** Saves one file: rejected (an ApiError with diagnostics) if its schema or the model's checks refuse it. */
  save<N extends SchemaName>(id: string, path: string, data: SchemaFiles[N]): Promise<SaveResult> {
    return this.request("PUT", this.project(id, "file"), { data }, { path });
  }

  /** Saves several files together: all, or none. */
  saveMany(id: string, files: { path: string; data: unknown }[]): Promise<SaveResult> {
    return this.request("POST", this.project(id, "save"), { files });
  }

  /** `make data-check`'s report for the project (text: the translations too, which needs gettext). */
  check(id: string, options: { text?: boolean } = {}): Promise<Report> {
    return this.request("GET", this.project(id, "check"), undefined, { text: options.text === false ? "0" : "1" });
  }

  preview(id: string, options: PreviewOptions & { headless: true }): Promise<PreviewReport>;
  preview(id: string, options: PreviewOptions): Promise<PreviewReport | PreviewStarted>;
  preview(id: string, options: PreviewOptions): Promise<PreviewReport | PreviewStarted> {
    return this.request("POST", this.project(id, "preview"), options);
  }

  /** The URL of any file of the project's game directory (a picture, a palette), as it is. */
  rawUrl(id: string, path: string): string {
    return this.url(this.project(id, "raw"), { path });
  }

  /** A room's folder (games/<game>/data/levels/<level>/<slot>), as the level editor opens it. */
  room(id: string, path: string): Promise<RoomView> {
    return this.request("GET", this.project(id, "room"), undefined, { path });
  }

  /** A layer of a room: a BG picture's palette indices, or the collision layer's cells. */
  async roomLayer(id: string, path: string, layer: string): Promise<RoomLayerData> {
    return parseRoomLayer(await this.requestBytes(this.project(id, "room/layer"), { path, layer }), layer === "collision");
  }

  /** The tile set a room's BG layer names, as the build makes it, and the layer's cells in it. */
  roomTiles(id: string, path: string, layer: string): Promise<RoomTiles> {
    return this.request("GET", this.project(id, "room/tiles"), undefined, { path, layer });
  }

  /** Saves a room's edits (room.json, painted cells, the palette) together: all, or none. */
  saveRoom(id: string, body: RoomSave): Promise<SaveResult> {
    return this.request("POST", this.project(id, "room/save"), body);
  }

  /** The cutscenes, their pages' text, their pictures, songs and sound effects, the font. */
  cutscenes(id: string): Promise<CutscenesView> {
    return this.request("GET", this.project(id, "cutscenes"));
  }

  /** A cutscene picture's colours and pixels. */
  async cutscenePicture(id: string, path: string): Promise<CutscenePictureData> {
    return parseCutscenePicture(await this.requestBytes(this.project(id, "cutscenes/picture"), { path }));
  }

  /** A PNG (base64) made into a cutscene picture: on its own colours, or mapped to `palette`. */
  importCutscenePicture(id: string, png: string, palette?: number[]): Promise<CutsceneImport> {
    return this.request("POST", this.project(id, "cutscenes/import"), palette ? { png, palette } : { png });
  }

  /** Saves the cutscene editor's edits together (cutscenes.json, the text, the pictures): all, or none. */
  saveCutscenes(id: string, body: CutscenesSave): Promise<SaveResult> {
    return this.request("POST", this.project(id, "cutscenes/save"), body);
  }

  /** The songs, what plays them, the sound effects and the samples. */
  music(id: string): Promise<MusicView> {
    return this.request("GET", this.project(id, "music"));
  }

  /** GAX2's tables and the two pools of instruments and samples, for the player. */
  musicPool(id: string): Promise<MusicPool> {
    return this.request("GET", this.project(id, "music/pool"));
  }

  /** A song as the build builds it; with `edits`, as it would build them (unsaved cell edits). */
  musicSong(id: string, key: string, edits?: MusicCellEdit[]): Promise<MusicSong> {
    if (edits?.length) return this.request("POST", this.project(id, "music/song"), { key, edits });
    return this.request("GET", this.project(id, "music/song"), undefined, { key });
  }

  /** A song's XM: its patterns' cells, order list and instruments. */
  musicPatterns(id: string, key: string): Promise<MusicPatterns> {
    return this.request("GET", this.project(id, "music/patterns"), undefined, { key });
  }

  /** A WAV (base64) made into a GAX2 sample: mono, 8-bit, at `rate` (the WAV's own by default). */
  importSample(id: string, wav: string, rate?: number): Promise<MusicImport> {
    return this.request("POST", this.project(id, "music/import"), rate === undefined ? { wav } : { wav, rate });
  }

  /** A file of the game's directory as it is (a sample's WAV: games/xs/sound/...). */
  raw(id: string, path: string): Promise<ArrayBuffer> {
    return this.requestBytes(this.project(id, "raw"), { path });
  }

  /** Saves the music editor's edits together (the tables, the songs, the samples): all, or none. */
  saveMusic(id: string, body: MusicSave): Promise<SaveResult> {
    return this.request("POST", this.project(id, "music/save"), body);
  }

  /** A sprite bank's frame as a PNG (index 0 transparent), in an OBJ palette or its own. */
  spriteUrl(id: string, bank: number, frame: number, palette: number | null = null): string {
    return this.url(this.project(id, "sprite"), {
      bank: String(bank),
      frame: String(frame),
      palette: palette === null ? undefined : String(palette),
    });
  }
}

/** GET .../room/layer's answer: width and height (u32 LE), then a byte a pixel or a u16 a cell. */
export function parseRoomLayer(buf: ArrayBuffer, cells: boolean): RoomLayerData {
  const view = new DataView(buf);
  if (buf.byteLength < 8) throw new Error("a room layer's answer is at least 8 bytes");
  const width = view.getUint32(0, true);
  const height = view.getUint32(4, true);
  const n = width * height;
  if (cells) {
    if (buf.byteLength !== 8 + n * 2) throw new Error(`a collision layer of ${width}x${height} cells is ${8 + n * 2} bytes, not ${buf.byteLength}`);
    const data = new Uint16Array(n);
    for (let i = 0; i < n; i++) data[i] = view.getUint16(8 + i * 2, true);
    return { width, height, data };
  }
  if (buf.byteLength !== 8 + n) throw new Error(`a layer of ${width}x${height} pixels is ${8 + n} bytes, not ${buf.byteLength}`);
  return { width, height, data: new Uint8Array(buf, 8, n) };
}

/** GET .../cutscenes/picture's answer: width and height (u32 LE), 256 colours (u16 LE), then a byte a pixel. */
export function parseCutscenePicture(buf: ArrayBuffer): CutscenePictureData {
  const view = new DataView(buf);
  if (buf.byteLength < 8 + 512) throw new Error("a cutscene picture's answer is at least 520 bytes");
  const width = view.getUint32(0, true);
  const height = view.getUint32(4, true);
  const palette: number[] = [];
  for (let i = 0; i < 256; i++) palette.push(view.getUint16(8 + i * 2, true));
  const n = width * height;
  if (buf.byteLength !== 8 + 512 + n) throw new Error(`a picture of ${width}x${height} pixels is ${8 + 512 + n} bytes, not ${buf.byteLength}`);
  return { width, height, palette, pixels: new Uint8Array(buf, 8 + 512, n) };
}

// The level editor's state (docs/editors.md, "The level editor"): the open
// room (room.json's value, its layers' pictures, its collision cells, its
// palette), what is shown, the tool, the undo stack, and saving it all back
// through the backend's room save (crashdata.levels.room_edit), which checks
// it like any save and keeps it all or none.
import type { EntityTypeInfo, LegendEntry, Room, RoomSave, RoomTiles, RoomView } from "@crash/schema-types";
import { client } from "../api.ts";
import { editor } from "../state.svelte.ts";
import { History, type Edit } from "./history.ts";
import {
  applyBlocks,
  applyValues,
  base64,
  brushOfBlock,
  changedCells,
  fromBase64,
  paintBlock,
  paintValue,
  readCell,
  tileBlock,
  tileIndex,
  type CellChange,
  type PixelLayer,
  type TileBrush,
} from "./paint.ts";
import type { Point } from "./coords.ts";
import { addEntity, addLink, deleteEntity, deleteLink, moveEntity, newRecord, ownRecord, type Entity, type ParamRecord } from "./entities.ts";

export type BgName = "bg0" | "bg1" | "bg2" | "bg3";
export type LayerName = BgName | "collision";
export const BG_NAMES: BgName[] = ["bg0", "bg1", "bg2", "bg3"];

export type Tool = "select" | "add" | "link" | "tile" | "collision";

export interface TileSet {
  name: string;
  bpp: 4 | 8;
  count: number;
  tiles: Uint8Array;
  banks: number[];
  index: Map<string, TileBrush>;
}

/** A room of the level table, for the navigation. */
export interface RoomEntry {
  folder: string;
  path: string;
  /** its slot: "main", "part1", "bonus", "gem path", ... */
  label: string;
  /** gLevelTable's index and the room's in its "rooms" (or "bonus", "gem_path": its extra rooms), when it can be previewed */
  level?: number;
  room?: number | "bonus" | "gem_path";
}

export interface LevelEntry {
  index: number;
  name: string;
  rooms: RoomEntry[];
  stages: string[];
}

export interface Show {
  bg0: boolean;
  bg1: boolean;
  bg2: boolean;
  bg3: boolean;
  collision: boolean;
  entities: boolean;
  labels: boolean;
  links: boolean;
  columns: boolean;
  screens: boolean;
  camera: boolean;
}

function same(a: unknown, b: unknown): boolean {
  return JSON.stringify(a) === JSON.stringify(b);
}

class LevelEditorState {
  levels = $state.raw<LevelEntry[]>([]);
  others = $state.raw<RoomEntry[]>([]);
  /** the open room as the backend gave it (as last saved) */
  view = $state.raw<RoomView | null>(null);
  /** room.json's value now */
  room = $state.raw<Room | null>(null);
  palette = $state.raw<number[]>([]);
  /** bumped when a layer's pixels or the collision cells change (the canvas redraws `damage`) */
  pixels = $state(0);
  /** the cells changed since the canvas last drew: (layer, cell index) */
  damage: { layer: LayerName; index: number }[] = [];
  loading = $state<string | null>(null);

  // the layers' data: not reactive (megabytes), `pixels` says when they change
  layers: Partial<Record<BgName, PixelLayer>> = {};
  originals: Partial<Record<BgName, Uint8Array>> = {};
  touched: Partial<Record<BgName, Set<number>>> = {};
  collision: { cols: number; rows: number; cells: Uint16Array; original: Uint16Array } | null = null;
  collisionTouched = new Set<number>();
  tileSets = $state.raw<Partial<Record<BgName, TileSet | string>>>({});

  history = new History();
  /** the history's version, reactively */
  hv = $state(0);

  selected = $state<number | null>(null);
  tool = $state<Tool>("select");
  paintLayer = $state<BgName>("bg0");
  brush = $state<TileBrush>({ tile: 0, hflip: false, vflip: false, bank: 0 });
  collisionValue = $state(0x2b);
  addType = $state<string | number>("WUMPA");
  show = $state<Show>({
    bg0: true,
    bg1: true,
    bg2: true,
    bg3: true,
    collision: false,
    entities: true,
    labels: false,
    links: true,
    columns: false,
    screens: false,
    camera: true,
  });
  parallax = $state(true);
  collisionOpacity = $state(0.55);
  /** the link tool's first entity */
  linkFrom = $state<number | null>(null);

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

  get legend(): LegendEntry[] {
    return this.view?.legend ?? [];
  }

  get types(): EntityTypeInfo[] {
    return this.view?.entityTypes ?? [];
  }

  /** An entity type (a name, or an unnamed type's number) as its record. */
  typeOf(t: string | number): EntityTypeInfo | undefined {
    return typeof t === "number" ? this.types.find((x) => x.type === t) : this.types.find((x) => x.name === t);
  }

  private bump(): void {
    this.hv = this.history.version;
  }

  confirmDiscard(): boolean {
    return !this.dirty || confirm(`${this.view?.path} has changes that aren't saved. Discard them?`);
  }

  // -- the level table ----------------------------------------------------------

  private projectKey = "";

  /** Follows the editor's project: a new one closes the room and reads its level table. */
  async syncProject(id: string, ready: boolean): Promise<void> {
    const key = `${id}:${ready}`;
    if (key === this.projectKey) return;
    this.projectKey = key;
    this.close();
    await this.loadLevels();
  }

  async loadLevels(): Promise<void> {
    const p = editor.project;
    this.levels = [];
    this.others = [];
    if (!p?.ready) return;
    const base = `games/${p.game}/data/levels/`;
    await editor.run("Reading the level table", async () => {
      const f = await client.read<"levels.schema.json">(editor.projectId, `${base}levels.json`);
      const data = f.data as unknown as { rooms?: string[]; levels?: { name?: string; constant?: string; rooms?: (string | Record<string, unknown>)[]; bonus?: string; gem_path?: string }[] };
      const listed: Record<string, boolean> = {};
      const entry = (folder: string, label: string, level?: number, room?: RoomEntry["room"]): RoomEntry => {
        listed[folder] = true;
        return { folder, path: base + folder, label, level, room };
      };
      this.levels = (data.levels ?? []).map((l, i) => {
        const rooms: RoomEntry[] = [];
        const stages: string[] = [];
        (l.rooms ?? []).forEach((r, k) => {
          if (typeof r === "string") rooms.push(entry(r, r.split("/").pop() ?? r, i, k));
          else stages.push(typeof r["stage"] === "string" ? r["stage"] : Object.entries(r).map(([key, v]) => `${key} ${String(v)}`).join(", "));
        });
        if (l.bonus) rooms.push(entry(l.bonus, "bonus", i, "bonus"));
        if (l.gem_path) rooms.push(entry(l.gem_path, "gem path", i, "gem_path"));
        return { index: i, name: l.name ?? l.constant ?? String(i), rooms, stages };
      });
      this.others = (data.rooms ?? []).filter((r) => !listed[r]).map((r) => ({ folder: r, path: base + r, label: r }));
    });
  }

  roomEntry(path: string | undefined): RoomEntry | undefined {
    for (const l of this.levels) for (const r of l.rooms) if (r.path === path) return r;
    return this.others.find((r) => r.path === path);
  }

  // -- opening ------------------------------------------------------------------

  close(): void {
    this.view = null;
    this.room = null;
    this.palette = [];
    this.layers = {};
    this.originals = {};
    this.touched = {};
    this.collision = null;
    this.collisionTouched = new Set();
    this.tileSets = {};
    this.selected = null;
    this.linkFrom = null;
    this.history.clear();
    this.bump();
  }

  async openRoom(path: string): Promise<boolean> {
    if (this.view?.path === path) return true;
    if (!this.confirmDiscard()) return false;
    const id = editor.projectId;
    this.loading = path;
    try {
      const ok = await editor.run(`Opening ${path}`, async () => {
        const view = await client.room(id, path);
        const names = Object.keys(view.layers) as LayerName[];
        const data = await Promise.all(names.map((n) => client.roomLayer(id, path, n)));
        this.close();
        names.forEach((n, i) => {
          const d = data[i]!;
          if (n === "collision") {
            const cells = d.data as Uint16Array;
            this.collision = { cols: d.width, rows: d.height, cells, original: cells.slice() };
          } else {
            const px = d.data as Uint8Array;
            this.layers[n] = { width: d.width, height: d.height, data: px.slice() };
            this.originals[n] = px.slice();
            this.touched[n] = new Set();
          }
        });
        this.view = view;
        this.room = structuredClone(view.room);
        this.palette = [...view.palette];
        this.paintLayer = (BG_NAMES.find((n) => n in view.layers) ?? "bg0") as BgName;
        const firstNamed = view.entityTypes.find((t) => t.name);
        if (!this.typeOf(this.addType) && firstNamed?.name) this.addType = firstNamed.name;
        this.pixels++;
        return true;
      });
      return !!ok;
    } finally {
      this.loading = null;
    }
  }

  /** The tile set of a BG layer, fetched once (the build's: a few seconds for the largest). */
  async loadTiles(layer: BgName): Promise<TileSet | null> {
    const have = this.tileSets[layer];
    if (have && typeof have !== "string") return have;
    if (!this.view) return null;
    const path = this.view.path;
    this.tileSets = { ...this.tileSets, [layer]: "loading" };
    try {
      const t: RoomTiles = await client.roomTiles(editor.projectId, path, layer);
      if (this.view?.path !== path) return null;
      const tiles = fromBase64(t.tiles);
      const set: TileSet = { name: t.tileset, bpp: t.bpp, count: t.count, tiles, banks: t.banks, index: tileIndex(tiles, t.count, t.bpp) };
      this.tileSets = { ...this.tileSets, [layer]: set };
      return set;
    } catch (e) {
      this.tileSets = { ...this.tileSets, [layer]: e instanceof Error ? e.message : String(e) };
      return null;
    }
  }

  // -- edits ----------------------------------------------------------------------

  private damaged(layer: LayerName, changes: { index: number }[]): void {
    if (!changes.length) return;
    for (const c of changes) this.damage.push({ layer, index: c.index });
    this.pixels++;
  }

  private record(e: Edit): void {
    this.history.push(e);
    this.bump();
  }

  /** Replaces room.json's value (an edit, undoable). */
  setRoom(label: string, next: Room, select: number | null = this.selected): void {
    const before = this.room;
    const sel = this.selected;
    if (!before || same(before, next)) return;
    this.room = next;
    this.selected = select;
    this.record({
      label,
      undo: () => {
        this.room = before;
        this.selected = sel;
      },
      redo: () => {
        this.room = next;
        this.selected = select;
      },
    });
  }

  moveEntity(i: number, x: number, y: number): void {
    if (!this.room) return;
    const r = moveEntity(this.room, i, Math.max(0, Math.round(x)), Math.max(0, Math.round(y)));
    this.setRoom("Move an entity", r.room, r.id);
  }

  /** Adds an entity of a type at a point: its own parameter record, a copy of one its type uses here, or a new one. */
  addEntity(type: string | number, x: number, y: number): void {
    if (!this.room || !this.view) return;
    const room = structuredClone(this.room);
    const like = room.entities.find((e) => e.type === type);
    let rec: ParamRecord;
    if (like) rec = structuredClone(room.params[like.param]!);
    else {
      const zero = this.types.find((t) => t.type === 0)?.name ?? null;
      rec = newRecord(editor.schemas["room.schema.json"] as Record<string, unknown>, this.typeOf(type)?.params, zero);
    }
    room.params.push(rec);
    const e: Entity = { type, x: Math.max(0, Math.round(x)), y: Math.max(0, Math.round(y)), param: room.params.length - 1 };
    const r = addEntity(room, e);
    this.setRoom("Add an entity", r.room, r.id);
  }

  deleteEntity(i: number): void {
    if (!this.room) return;
    this.setRoom("Delete an entity", deleteEntity(this.room, i), null);
  }

  updateEntity(i: number, patch: Partial<Entity>): void {
    if (!this.room) return;
    const e = this.room.entities[i];
    if (!e) return;
    if ((patch.x !== undefined && patch.x !== e.x) || (patch.y !== undefined && patch.y !== e.y)) {
      const { x, y, ...rest } = patch;
      const r = moveEntity(this.room, i, x ?? e.x, y ?? e.y);
      const out = r.room;
      Object.assign(out.entities[r.id]!, rest);
      this.setRoom("Edit an entity", out, r.id);
      return;
    }
    const out = structuredClone(this.room);
    Object.assign(out.entities[i]!, patch);
    this.setRoom("Edit an entity", out, i);
  }

  setRecord(p: number, rec: ParamRecord): void {
    if (!this.room) return;
    const out = structuredClone(this.room);
    out.params[p] = rec;
    this.setRoom("Edit a parameter record", out);
  }

  ownRecord(i: number): void {
    if (!this.room) return;
    this.setRoom("Give an entity its own record", ownRecord(this.room, i), i);
  }

  addLink(from: number, to: number): void {
    if (!this.room) return;
    this.setRoom("Link two entities", addLink(this.room, from, to));
  }

  deleteLink(index: number): void {
    if (!this.room) return;
    this.setRoom("Delete a link", deleteLink(this.room, index));
  }

  /** The block the brush paints on the paint layer, or null without its tile set. */
  brushBlock(): Uint8Array | null {
    const set = this.tileSets[this.paintLayer];
    if (!set || typeof set === "string") return null;
    return tileBlock(set.tiles, this.brush, set.bpp);
  }

  /** Paints the brush's tile into cells of the paint layer: the changes, applied (a stroke gathers them). */
  paintTiles(at: Point[]): CellChange<Uint8Array>[] {
    const layer = this.layers[this.paintLayer];
    const block = this.brushBlock();
    if (!layer || !block) return [];
    const changes = paintBlock(layer, at, block);
    for (const c of changes) this.touched[this.paintLayer]!.add(c.index);
    this.damaged(this.paintLayer, changes);
    return changes;
  }

  /** Records a stroke's tile changes as one edit. */
  commitTiles(layerName: BgName, changes: CellChange<Uint8Array>[]): void {
    const layer = this.layers[layerName];
    if (!layer || !changes.length) return;
    this.record({
      label: `Paint ${layerName}`,
      undo: () => {
        // a saved stroke undone is a change again: the cells go in the next save
        applyBlocks(layer, changes, true);
        for (const c of changes) this.touched[layerName]!.add(c.index);
        this.damaged(layerName, changes);
      },
      redo: () => {
        applyBlocks(layer, changes, false);
        for (const c of changes) this.touched[layerName]!.add(c.index);
        this.damaged(layerName, changes);
      },
    });
  }

  paintCollision(at: Point[]): CellChange<number>[] {
    const c = this.collision;
    if (!c) return [];
    const changes = paintValue(c.cells, c.cols, at, this.collisionValue);
    for (const x of changes) this.collisionTouched.add(x.index);
    this.damaged("collision", changes);
    return changes;
  }

  commitCollision(changes: CellChange<number>[]): void {
    const c = this.collision;
    if (!c || !changes.length) return;
    this.record({
      label: "Paint the collision",
      undo: () => {
        applyValues(c.cells, changes, true);
        for (const x of changes) this.collisionTouched.add(x.index);
        this.damaged("collision", changes);
      },
      redo: () => {
        applyValues(c.cells, changes, false);
        for (const x of changes) this.collisionTouched.add(x.index);
        this.damaged("collision", changes);
      },
    });
  }

  /** The eyedropper: the brush from a paint layer cell, or the collision value. */
  pickTile(cell: Point): boolean {
    const layer = this.layers[this.paintLayer];
    const set = this.tileSets[this.paintLayer];
    if (!layer || !set || typeof set === "string") return false;
    const b = brushOfBlock(set.index, readCell(layer, cell.x, cell.y), set.bpp);
    if (b) this.brush = b;
    return !!b;
  }

  pickCollision(cell: Point): void {
    const c = this.collision;
    if (c) this.collisionValue = c.cells[cell.y * c.cols + cell.x] ?? 0;
  }

  setPaletteEntry(i: number, v: number): void {
    if (this.palette[i] === v) return;
    const before = this.palette;
    const after = [...before];
    after[i] = v;
    this.palette = after;
    this.record({
      label: `Colour ${i}`,
      undo: () => (this.palette = before),
      redo: () => (this.palette = after),
    });
  }

  undo(): void {
    if (this.history.undo()) this.bump();
  }

  redo(): void {
    if (this.history.redo()) this.bump();
  }

  // -- saving -------------------------------------------------------------------------

  /** What a save sends: only what differs from the room as opened (or last saved). */
  saveBody(): RoomSave | null {
    if (!this.view || !this.room) return null;
    const body: RoomSave = { path: this.view.path };
    if (!same(this.room, this.view.room)) body.room = this.room;
    if (!same(this.palette, this.view.palette)) body.palette = this.palette;
    const layers: NonNullable<RoomSave["layers"]> = {};
    for (const n of BG_NAMES) {
      const l = this.layers[n];
      if (!l) continue;
      const cells = changedCells(l, this.originals[n]!, this.touched[n]!);
      if (cells.length) layers[n] = cells.map(([x, y, px]) => [x, y, base64(px)]);
    }
    if (Object.keys(layers).length) body.layers = layers;
    const c = this.collision;
    if (c) {
      const out: [number, number, number][] = [];
      for (const i of [...this.collisionTouched].sort((a, b) => a - b)) {
        if (c.cells[i] !== c.original[i]) out.push([i % c.cols, Math.floor(i / c.cols), c.cells[i]!]);
      }
      if (out.length) body.collision = out;
    }
    return body;
  }

  async save(): Promise<void> {
    const body = this.saveBody();
    if (!body) return;
    await editor.run(`Saving ${body.path}`, async () => {
      const r = await client.saveRoom(editor.projectId, body);
      // what is saved is now the room as opened
      const view = this.view!;
      this.view = { ...view, room: structuredClone(this.room!), palette: [...this.palette] };
      for (const n of BG_NAMES) {
        const l = this.layers[n];
        if (!l) continue;
        this.originals[n] = l.data.slice();
        this.touched[n] = new Set();
      }
      if (this.collision) {
        this.collision.original = this.collision.cells.slice();
        this.collisionTouched = new Set();
      }
      // painted tiles may be new to the set's order: fetch it again when needed
      if (body.layers) this.tileSets = {};
      this.history.markSaved();
      this.bump();
      const errors = r.diagnostics.filter((d) => d.severity === "error").length;
      editor.diagnostics = {
        source: "save",
        title: r.saved.length ? `Saved ${r.saved.join(", ")}` + (errors ? ` (the data's other ${errors} error(s) are as they were)` : "") : `${body.path}: nothing changed`,
        ok: true,
        diagnostics: r.diagnostics,
        at: new Date(),
      };
    });
  }

  /** Drops the edits: the room as last saved. */
  async revert(): Promise<void> {
    const path = this.view?.path;
    if (!path) return;
    this.history.clear();
    this.view = null;
    await this.openRoom(path);
  }
}

export const level = new LevelEditorState();

// switching projects asks about the room's changes too
editor.projectGuards.push(() => level.confirmDiscard());

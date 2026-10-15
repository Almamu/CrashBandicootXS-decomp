// The level editor's state (docs/editors.md, "The level editor"): the open
// room (room.json's value, its layers' pictures, its collision cells, its
// palette), what is shown, the tool, the undo stack, and saving it back
// through the backend's room save (crashdata.levels.room_edit), which checks
// it like any save and keeps it all or none.
import type { EntityTypeInfo, LegendEntry, Room, RoomSave, RoomView } from "@crash/schema-types";
import { client } from "../api.ts";
import { editor } from "../state.svelte.ts";
import { History, type Edit } from "./history.ts";
import { addEntity, addLink, deleteEntity, deleteLink, moveEntity, newRecord, ownRecord, type Entity, type ParamRecord } from "./entities.ts";

export type BgName = "bg0" | "bg1" | "bg2" | "bg3";
export type LayerName = BgName | "collision";
export const BG_NAMES: BgName[] = ["bg0", "bg1", "bg2", "bg3"];

export type Tool = "select" | "add" | "link";

/** A layer's picture: its palette indices, row by row. */
export interface PixelLayer {
  width: number;
  height: number;
  data: Uint8Array;
}

/** A room of the level table, for the navigation. */
export interface RoomEntry {
  folder: string;
  path: string;
  /** its slot: "main", "part1", "bonus", "gem path", ... */
  label: string;
  /** gLevelTable's index and the room's in its "rooms", when it can be previewed */
  level?: number;
  room?: number;
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
  /** bumped when a layer's pixels or the collision cells change (the canvas redraws) */
  pixels = $state(0);
  loading = $state<string | null>(null);

  // the layers' data: not reactive (megabytes), `pixels` says when they change
  layers: Partial<Record<BgName, PixelLayer>> = {};
  collision: { cols: number; rows: number; cells: Uint16Array } | null = null;

  history = new History();
  /** the history's version, reactively */
  hv = $state(0);

  selected = $state<number | null>(null);
  tool = $state<Tool>("select");
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
      const entry = (folder: string, label: string, level?: number, room?: number): RoomEntry => {
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
        if (l.bonus) rooms.push(entry(l.bonus, "bonus"));
        if (l.gem_path) rooms.push(entry(l.gem_path, "gem path"));
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
    this.collision = null;
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
          if (n === "collision") this.collision = { cols: d.width, rows: d.height, cells: d.data as Uint16Array };
          else this.layers[n] = { width: d.width, height: d.height, data: d.data as Uint8Array };
        });
        this.view = view;
        this.room = structuredClone(view.room);
        this.palette = [...view.palette];
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

  // -- edits ----------------------------------------------------------------------

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
    return body;
  }

  async save(): Promise<void> {
    const body = this.saveBody();
    if (!body) return;
    await editor.run(`Saving ${body.path}`, async () => {
      const r = await client.saveRoom(editor.projectId, body);
      // what is saved is now the room as opened
      const view = this.view!;
      this.view = { ...view, room: structuredClone(this.room!) };
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

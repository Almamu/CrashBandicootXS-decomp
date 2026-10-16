// A room's entities, links and parameter records (docs/levels.md,
// "Entities"). An entity's id is its index in room.json's "entities", which
// are stored in id order: the last 256-pixel column first (SpawnRoomEntities
// walks the columns from the last), any order within a column. The links name
// entities by id, so an edit that moves an entity to another column, adds or
// deletes one renumbers the links with it. Each function returns a new room;
// the one given is left as it was.
import type { Room, RoomDefs } from "@crash/schema-types";
import { columnOf } from "./coords.ts";

export type Entity = RoomDefs.Entity;
export type ParamRecord = RoomDefs.ParamRecord;
export type Link = [number, number];

function clone<T>(v: T): T {
  return structuredClone(v);
}

/** Whether the entities are in id order (each column no later than the one before it). */
export function inIdOrder(entities: readonly Entity[]): boolean {
  for (let i = 1; i < entities.length; i++) if (columnOf(entities[i]!.x) > columnOf(entities[i - 1]!.x)) return false;
  return true;
}

/** Where an entity of column `col` goes: after the last one of its column (or of a later one). */
export function insertionIndex(entities: readonly Entity[], col: number): number {
  let i = 0;
  while (i < entities.length && columnOf(entities[i]!.x) >= col) i++;
  return i;
}

/** The links with each id mapped (`map[id]`, -1: the entity is gone, and so is the link). */
export function remapLinks(links: readonly Link[] | null | undefined, map: readonly number[]): Link[] | null | undefined {
  if (!links) return links;
  const out: Link[] = [];
  for (const [a, b] of links) {
    const na = map[a] ?? a;
    const nb = map[b] ?? b;
    if (na < 0 || nb < 0) continue;
    out.push([na, nb]);
  }
  return out;
}

/** An entity moved from `from` to `to`: every id's new value. */
function moveMap(n: number, from: number, to: number): number[] {
  const map = Array.from({ length: n }, (_, i) => i);
  if (from === to) return map;
  map[from] = to;
  if (from < to) for (let i = from + 1; i <= to; i++) map[i] = i - 1;
  else for (let i = to; i < from; i++) map[i] = i + 1;
  return map;
}

/**
 * The room with entity `i` at (x, y): it stays where it is in the list while
 * it stays in its column, and goes to the end of its new column's entities
 * otherwise (the links follow). Also gives its new id.
 */
export function moveEntity(room: Room, i: number, x: number, y: number): { room: Room; id: number } {
  const out = clone(room);
  const e = out.entities[i];
  if (!e) throw new Error(`no entity ${i}`);
  const oldCol = columnOf(e.x);
  e.x = x;
  e.y = y;
  if (columnOf(x) === oldCol) return { room: out, id: i };
  out.entities.splice(i, 1);
  const to = insertionIndex(out.entities, columnOf(x));
  out.entities.splice(to, 0, e);
  out.links = remapLinks(out.links, moveMap(room.entities.length, i, to));
  return { room: out, id: to };
}

/** The room with an entity added in id order (the links follow). Also gives its id. */
export function addEntity(room: Room, e: Entity): { room: Room; id: number } {
  const out = clone(room);
  const to = insertionIndex(out.entities, columnOf(e.x));
  out.entities.splice(to, 0, clone(e));
  const map = Array.from({ length: room.entities.length }, (_, k) => (k >= to ? k + 1 : k));
  out.links = remapLinks(out.links, map);
  return { room: out, id: to };
}

/** The room without entity `i`, and without its links. Its record stays (another may use it). */
export function deleteEntity(room: Room, i: number): Room {
  const out = clone(room);
  out.entities.splice(i, 1);
  const map = Array.from({ length: room.entities.length }, (_, k) => (k === i ? -1 : k > i ? k - 1 : k));
  out.links = remapLinks(out.links, map);
  return out;
}

/** The room with a link added (a crate `from`, the crate on top of it `to`); none if it has it. */
export function addLink(room: Room, from: number, to: number): Room {
  const out = clone(room);
  if (from === to) return out;
  const links = out.links ?? [];
  if (!links.some(([a, b]) => a === from && b === to)) links.push([from, to]);
  out.links = links;
  return out;
}

export function deleteLink(room: Room, index: number): Room {
  const out = clone(room);
  out.links?.splice(index, 1);
  return out;
}

/** The entities that use record `p`. */
export function recordUsers(room: Room, p: number): number[] {
  const out: number[] = [];
  room.entities.forEach((e, i) => {
    if (e.param === p) out.push(i);
  });
  return out;
}

/** The room with a copy of entity i's record, which i then uses alone. */
export function ownRecord(room: Room, i: number): Room {
  const out = clone(room);
  const e = out.entities[i];
  if (!e) throw new Error(`no entity ${i}`);
  out.params.push(clone(out.params[e.param]!));
  e.param = out.params.length - 1;
  return out;
}

/** The fields of a record's view, as the schema's $defs/param_<layout> lists them. */
export interface FieldSpec {
  name: string;
  kind: "bits" | "entity" | "int";
  /** the bits' names */
  bits?: string[];
  min?: number;
  max?: number;
}

type Schema = Record<string, unknown>;

/** A view's fields, from room.schema.json (crashdata.levels.params.record_schemas). */
export function layoutFields(roomSchema: Schema, layout: string): FieldSpec[] {
  const defs = (roomSchema["$defs"] ?? {}) as Record<string, Schema>;
  const def = defs[`param_${layout}`];
  if (!def) return [];
  const props = (def["properties"] ?? {}) as Record<string, Schema>;
  const out: FieldSpec[] = [];
  for (const [name, s] of Object.entries(props)) {
    if (name === "layout" || name === "extra") continue;
    if (s["type"] === "array") {
      const items = (s["items"] ?? {}) as Schema;
      out.push({ name, kind: "bits", bits: (items["enum"] as string[] | undefined) ?? [] });
    } else if (s["anyOf"]) out.push({ name, kind: "entity" });
    else out.push({ name, kind: "int", min: s["minimum"] as number | undefined, max: s["maximum"] as number | undefined });
  }
  return out;
}

/** The views' names, from room.schema.json. */
export function layouts(roomSchema: Schema): string[] {
  const defs = (roomSchema["$defs"] ?? {}) as Record<string, Schema>;
  return Object.keys(defs)
    .filter((k) => k.startsWith("param_") && k !== "param_record")
    .map((k) => k.slice("param_".length));
}

/**
 * A new record for a view: every field, all zero (the ROM's records are as
 * long as what their entities read, so a new one has every field). An entity
 * field is named after type 0, as the build names a zero (`zeroName`). A type
 * with no view gets a raw record of one zero word.
 */
export function newRecord(roomSchema: Schema, layout: string | null | undefined, zeroName: string | null): ParamRecord {
  if (!layout) return [0];
  const rec: Record<string, unknown> = { layout };
  for (const f of layoutFields(roomSchema, layout)) {
    rec[f.name] = f.kind === "bits" ? [] : f.kind === "entity" ? (zeroName ?? 0) : 0;
  }
  return rec as unknown as ParamRecord;
}

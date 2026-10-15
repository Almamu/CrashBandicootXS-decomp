// The level editor's logic (docs/editors.md, "The level editor"): the view's
// coordinates and the game's parallax, the undo stack, the entities' id order
// and links, the palette's colour rule, the parameter record views.
import { readFileSync } from "node:fs";
import { join } from "node:path";
import type { Room } from "@crash/schema-types";
import { describe, expect, it } from "vitest";
import { cameraFor, cellAt, clampCamera, fit, layerOrigin, lineCells, rectCells, scaleQ8, toScreen, toWorld, zoomAt } from "../src/lib/level/coords.ts";
import { addEntity, addLink, deleteEntity, inIdOrder, insertionIndex, layoutFields, layouts, moveEntity, newRecord, ownRecord, recordUsers, remapLinks } from "../src/lib/level/entities.ts";
import { History } from "../src/lib/level/history.ts";
import { channels, fromChannels, fromHex, fromRgb, hex, pixelTable, toRgb } from "../src/lib/level/palette.ts";
import { visibleTiles } from "../src/lib/level/render.ts";

const ROOT = join(import.meta.dirname, "../../..");
const json = (p: string) => JSON.parse(readFileSync(join(ROOT, p), "utf8")) as unknown;
const roomSchema = json("packages/crashdata/schemas/room.schema.json") as Record<string, unknown>;
const jungle = () => json("games/xs/data/levels/jungle_jam/main/room.json") as Room;

/** An indexed PNG's palette (its PLTE chunk): a room picture's colours, as the build reads them. */
function pngPalette(path: string): [number, number, number][] {
  const b = readFileSync(join(ROOT, path));
  const out: [number, number, number][] = [];
  for (let i = 8; i < b.length; ) {
    const n = b.readUInt32BE(i);
    if (b.toString("latin1", i + 4, i + 8) === "PLTE") for (let k = i + 8; k < i + 8 + n; k += 3) out.push([b[k]!, b[k + 1]!, b[k + 2]!]);
    i += 12 + n;
  }
  return out;
}

describe("the view's coordinates", () => {
  it("map the canvas to the world and back", () => {
    const v = { zoom: 2, x: 100, y: 50 };
    expect(toWorld(v, 40, 20)).toEqual({ x: 120, y: 60 });
    expect(toScreen(v, 120, 60)).toEqual({ x: 40, y: 20 });
  });

  it("zoom about the cursor, which stays put", () => {
    const v = { zoom: 1, x: 0, y: 0 };
    const z = zoomAt(v, 200, 100, 2);
    expect(z.zoom).toBe(2);
    expect(toWorld(z, 200, 100)).toEqual(toWorld(v, 200, 100));
    expect(zoomAt(v, 0, 0, 1e9).zoom).toBe(16);
  });

  it("fit a room at a power of two", () => {
    const v = fit(800, 600, 4096, 512);
    expect(v.zoom).toBe(0.125);
    expect(toWorld(v, 400, 300)).toEqual({ x: 2048, y: 256 });
  });

  it("scroll a layer as ScaleBgLayerScroll does: rounded toward zero", () => {
    expect(scaleQ8(100, 128)).toBe(50);
    expect(scaleQ8(101, 128)).toBe(50);
    expect(scaleQ8(-101, 128)).toBe(-50);
    expect(scaleQ8(1000, 384)).toBe(1500);
    // bg1 of jungle jam, at half speed: at camera 1000 its pixel 500 is at the screen's left
    const o = layerOrigin({ x: 1000, y: 200 }, [128, 128], true);
    expect(o).toEqual({ x: 500, y: 100 });
    expect(500 + o.x).toBe(1000);
    expect(layerOrigin({ x: 1000, y: 200 }, [128, 128], false)).toEqual({ x: 0, y: 0 });
    expect(layerOrigin({ x: 1000, y: 200 }, [256, 256], true)).toEqual({ x: 0, y: 0 });
  });

  it("keep the camera in the room", () => {
    expect(clampCamera(-5, 10, 4016, 480)).toEqual({ x: 0, y: 10 });
    expect(clampCamera(5000, 1000, 4016, 480)).toEqual({ x: 4016 - 240, y: 480 - 160 });
    expect(cameraFor({ zoom: 1, x: 1000, y: 0 }, 400, 300, 4016, 480)).toEqual({ x: 1080, y: 70 });
  });

  it("find cells, lines and rectangles of them", () => {
    expect(cellAt({ x: 17, y: 9 }, { x: 0, y: 0 }, 10, 10)).toEqual({ x: 2, y: 1 });
    expect(cellAt({ x: 17, y: 9 }, { x: 16, y: 0 }, 10, 10)).toEqual({ x: 0, y: 1 });
    expect(cellAt({ x: -1, y: 0 }, { x: 0, y: 0 }, 10, 10)).toBeNull();
    expect(cellAt({ x: 80, y: 0 }, { x: 0, y: 0 }, 10, 10)).toBeNull();
    expect(lineCells({ x: 0, y: 0 }, { x: 3, y: 1 })).toEqual([
      { x: 0, y: 0 },
      { x: 1, y: 0 },
      { x: 2, y: 1 },
      { x: 3, y: 1 },
    ]);
    expect(rectCells({ x: 2, y: 1 }, { x: 1, y: 2 })).toHaveLength(4);
  });

  it("draw only the visible tiles", () => {
    expect(visibleTiles({ zoom: 1, x: 0, y: 0 }, 600, 400, { x: 0, y: 0 }, 3328, 512)).toEqual([
      [0, 0],
      [1, 0],
    ]);
    expect(visibleTiles({ zoom: 1, x: 5000, y: 0 }, 600, 400, { x: 0, y: 0 }, 3328, 512)).toEqual([]);
    // the collision bitmap: a pixel a cell, 8 world pixels each
    expect(visibleTiles({ zoom: 1, x: 4000, y: 0 }, 600, 400, { x: 0, y: 0 }, 512, 64, 8)).toEqual([[0, 0]]);
  });
});

describe("the undo stack", () => {
  function counter() {
    const s = { v: 0 };
    const add = (n: number) => ({ label: `+${n}`, undo: () => (s.v -= n), redo: () => (s.v += n) });
    return { s, add };
  }

  it("undoes and redoes, and knows the saved state", () => {
    const { s, add } = counter();
    const h = new History();
    expect(h.dirty).toBe(false);
    h.apply(add(1));
    h.apply(add(2));
    expect(s.v).toBe(3);
    expect(h.dirty).toBe(true);
    expect(h.undoLabel).toBe("+2");
    h.markSaved();
    expect(h.dirty).toBe(false);
    h.undo();
    expect(s.v).toBe(1);
    expect(h.dirty).toBe(true);
    h.redo();
    expect(s.v).toBe(3);
    expect(h.dirty).toBe(false);
    expect(h.redo()).toBe(false);
  });

  it("drops the redo branch on a new edit; the saved state may become unreachable", () => {
    const { s, add } = counter();
    const h = new History();
    h.apply(add(1));
    h.markSaved();
    h.undo();
    h.apply(add(5));
    expect(s.v).toBe(5);
    expect(h.canRedo).toBe(false);
    h.undo();
    expect(h.dirty).toBe(true); // the saved state had +1, which is gone
  });

  it("keeps at most its limit", () => {
    const { s, add } = counter();
    const h = new History(3);
    for (let i = 0; i < 5; i++) h.apply(add(1));
    while (h.undo());
    expect(s.v).toBe(2);
  });
});

describe("the palette", () => {
  it("reads and writes colours as jasc_pal does, bit 15 included", () => {
    expect(toRgb(0x7fff)).toEqual([255, 255, 255]);
    expect(toRgb(0x801f)).toEqual([254, 0, 0]); // bit 15: red one step off
    expect(fromRgb(254, 0, 0)).toBe(0x801f);
    expect(fromRgb(255, 0, 0)).toBe(0x001f);
    expect(fromHex(hex(0x801f))).toBe(0x801f);
    const c = channels(0x8000 | (3 << 10) | (2 << 5) | 1);
    expect(c).toEqual({ r: 1, g: 2, b: 3, bit15: 1 });
    expect(fromChannels(c.r, c.g, c.b, c.bit15)).toBe(0x8000 | (3 << 10) | (2 << 5) | 1);
  });

  it("gives back a room picture's every colour", () => {
    const palette = pngPalette("games/xs/data/levels/jungle_jam/main/bg1.png");
    expect(palette).toHaveLength(256);
    for (const rgb of palette) expect(toRgb(fromRgb(...rgb))).toEqual(rgb);
  });

  it("makes the transparent indices see-through", () => {
    const t = pixelTable([0x7fff, 0x001f], (i) => i === 0);
    expect(t[0]).toBe(0);
    expect(t[1]! >>> 0).toBe(0xff0000ff);
  });
});

describe("the entities", () => {
  it("are in id order in the tree's rooms", () => {
    expect(inIdOrder(jungle().entities)).toBe(true);
  });

  it("stay in place when moved within their group", () => {
    const room = jungle();
    const e = room.entities[1]!;
    const r = moveEntity(room, 1, e.x + 3, e.y - 8);
    expect(r.id).toBe(1);
    expect(r.room.entities[1]).toEqual({ ...e, x: e.x + 3, y: e.y - 8 });
    expect(r.room.links).toEqual(room.links);
    expect(room.entities[1]).toEqual(e); // the room given is left as it was
  });

  it("move to their new group's place, the links following", () => {
    const room = jungle();
    // entity 1 is the lower crate of link [2, 1]; take it to the first group
    const r = moveEntity(room, 1, 10, 100);
    expect(inIdOrder(r.room.entities)).toBe(true);
    expect(r.id).toBe(room.entities.length - 1);
    expect(r.room.entities[r.id]).toMatchObject({ x: 10, y: 100, type: room.entities[1]!.type });
    // each link still joins the same two entities
    const name = (rm: Room, i: number) => JSON.stringify(rm.entities[i]);
    const before = room.links!.map(([a, b]) => [a === 1 ? "moved" : name(room, a), b === 1 ? "moved" : name(room, b)]);
    const after = r.room.links!.map(([a, b]) => [a === r.id ? "moved" : name(r.room, a), b === r.id ? "moved" : name(r.room, b)]);
    expect(after).toEqual(before);
  });

  it("are added in id order and deleted with their links", () => {
    const room = jungle();
    const n = room.entities.length;
    const r = addEntity(room, { type: "WUMPA", x: 300, y: 50, param: 1 });
    expect(r.room.entities).toHaveLength(n + 1);
    expect(inIdOrder(r.room.entities)).toBe(true);
    expect(r.id).toBe(insertionIndex(room.entities, 1));
    const d = deleteEntity(room, 2);
    expect(d.entities).toHaveLength(n - 1);
    expect(d.links!.some(([a, b]) => a === 2 || b === 2)).toBe(false);
    expect(d.links).toEqual(remapLinks(room.links, Array.from({ length: n }, (_, k) => (k === 2 ? -1 : k > 2 ? k - 1 : k))));
    expect(addLink(room, 5, 6).links!.at(-1)).toEqual([5, 6]);
    expect(addLink(room, 2, 1).links).toEqual(room.links);
  });

  it("can have a record of their own", () => {
    const room = jungle();
    const p = room.entities[1]!.param;
    expect(recordUsers(room, p).length).toBeGreaterThan(1);
    const r = ownRecord(room, 1);
    expect(r.entities[1]!.param).toBe(room.params.length);
    expect(r.params.at(-1)).toEqual(room.params[p]);
    expect(recordUsers(r, r.entities[1]!.param)).toEqual([1]);
  });
});

describe("the parameter records' views", () => {
  it("come from the room schema", () => {
    expect(layouts(roomSchema)).toEqual(expect.arrayContaining(["flags", "patrol", "crate", "platform", "outline_crate"]));
    const crate = layoutFields(roomSchema, "crate");
    expect(crate.map((f) => [f.name, f.kind])).toEqual([
      ["flags", "bits"],
      ["options", "bits"],
      ["unk_02", "int"],
      ["trial_kind", "entity"],
      ["param6", "int"],
      ["param7", "int"],
    ]);
    expect(crate[0]!.bits).toContain("TRIAL_KIND");
    expect(layoutFields(roomSchema, "platform").find((f) => f.name === "dir_x")).toMatchObject({ min: -32768, max: 32767 });
  });

  it("make a new record with every field", () => {
    expect(newRecord(roomSchema, "crate", "START_MARKER")).toEqual({
      layout: "crate",
      flags: [],
      options: [],
      unk_02: 0,
      trial_kind: "START_MARKER",
      param6: 0,
      param7: 0,
    });
    expect(newRecord(roomSchema, null, null)).toEqual([0]);
  });
});

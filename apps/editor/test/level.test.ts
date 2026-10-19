// The level editor's logic (docs/editors.md, "The level editor"): the view's
// coordinates and the game's parallax, the undo stack, painting cells, the
// entities' id order and links, the palette's colour rule, the parameter
// record views.
import { readFileSync } from "node:fs";
import { join } from "node:path";
import type { EntityTypeInfo, Room } from "@crash/schema-types";
import { describe, expect, it } from "vitest";
import { cameraFor, cellAt, clampCamera, fit, layerOrigin, lineCells, rectCells, scaleQ8, toScreen, toWorld, zoomAt } from "../src/lib/level/coords.ts";
import { addEntity, addLink, deleteEntity, inIdOrder, insertionIndex, layoutFields, layouts, moveEntity, newRecord, ownRecord, recordUsers, remapLinks } from "../src/lib/level/entities.ts";
import { History } from "../src/lib/level/history.ts";
import {
  addTile,
  blockKey,
  layerBanks,
  newBlocks,
  pasteCells,
  replaceTile,
  tileUses,
  applyBlocks,
  applyValues,
  base64,
  brushOf,
  brushOfBlock,
  cellValue,
  changedCells,
  fromBase64,
  oneBank,
  paintBlock,
  paintValue,
  readCell,
  tileBlock,
  tileIndex,
  u16le,
  type PixelLayer,
} from "../src/lib/level/paint.ts";
import { channels, fromChannels, fromHex, fromRgb, hex, pixelTable, toRgb } from "../src/lib/level/palette.ts";
import { blendPixel, blends, compositeBlend, visibleTiles } from "../src/lib/level/render.ts";
import { startMarkerFor } from "../src/lib/level/room.ts";
import { composeValue, valueName, freeColour, newValueErrors, typeKind, valueParts } from "../src/lib/level/terrain.ts";

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

describe("painting", () => {
  const tiles = new Uint8Array(64 * 2);
  for (let i = 0; i < 64; i++) tiles[64 + i] = (i % 8) + 1; // tile 1: columns 1-8 (4bpp: 1-8)

  it("draws a tile flipped, in a bank, and reads the cell back", () => {
    const b = tileBlock(tiles, { tile: 1, hflip: true, vflip: false, bank: 3 }, 4);
    expect([...b.subarray(0, 8)]).toEqual([56, 55, 54, 53, 52, 51, 50, 49]);
    expect(oneBank(b)).toBe(true);
    expect(tileBlock(tiles, { tile: 1, hflip: false, vflip: false, bank: 3 }, 8)[0]).toBe(1);
    const v = cellValue({ tile: 5, hflip: true, vflip: true, bank: 2 }, false);
    expect(v).toBe(5 | (3 << 10) | (2 << 12));
    expect(brushOf(v, false)).toEqual({ tile: 5, hflip: true, vflip: true, bank: 2 });
    expect(brushOf(cellValue({ tile: 300, hflip: false, vflip: true, bank: 0 }, true), true)).toEqual({ tile: 300, hflip: false, vflip: true, bank: 0 });
  });

  it("paints cells, undoes them, and sends only what changed", () => {
    const layer: PixelLayer = { width: 32, height: 16, data: new Uint8Array(32 * 16) };
    const original = layer.data.slice();
    const block = tileBlock(tiles, { tile: 1, hflip: false, vflip: false, bank: 1 }, 4);
    const ch = paintBlock(layer, [{ x: 1, y: 1 }, { x: 1, y: 1 }, { x: 3, y: 0 }], block);
    expect(ch.map((c) => c.index)).toEqual([5, 3]);
    expect([...readCell(layer, 1, 1)]).toEqual([...block]);
    expect(paintBlock(layer, [{ x: 1, y: 1 }], block)).toEqual([]);
    const sent = changedCells(layer, original, [5, 3, 0]);
    expect(sent.map(([x, y]) => [x, y])).toEqual([
      [3, 0],
      [1, 1],
    ]);
    applyBlocks(layer, ch, true);
    expect(layer.data).toEqual(original);
    expect(changedCells(layer, original, [5, 3])).toEqual([]);
    applyBlocks(layer, ch, false);
    expect([...readCell(layer, 3, 0)]).toEqual([...block]);
  });

  it("paints collision values", () => {
    const cells = new Uint16Array(6);
    const ch = paintValue(cells, 3, [{ x: 1, y: 1 }, { x: 2, y: 0 }], 0x2b);
    expect([...cells]).toEqual([0, 0, 0x2b, 0, 0x2b, 0]);
    applyValues(cells, ch, true);
    expect([...cells]).toEqual([0, 0, 0, 0, 0, 0]);
  });

  it("finds a block's tile under its flips (the eyedropper)", () => {
    const index = tileIndex(tiles, 2, 4);
    const b = tileBlock(tiles, { tile: 1, hflip: true, vflip: true, bank: 7 }, 4);
    const got = brushOfBlock(index, b, 4)!;
    expect(got.tile).toBe(1);
    expect(got.bank).toBe(7);
    expect(tileBlock(tiles, got, 4)).toEqual(b);
    expect(brushOfBlock(index, new Uint8Array(64).fill(9), 4)).toBeNull();
  });

  it("round-trips base64 and u16 cells", () => {
    const b = new Uint8Array([0, 1, 254, 255, 16]);
    expect(fromBase64(base64(b))).toEqual(b);
    expect([...u16le(new Uint8Array([0x34, 0x12, 0xff, 0xff]))]).toEqual([0x1234, 0xffff]);
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

describe("the room's kind and blend", () => {
  const W = 4;
  const layer = (fill: number, first: boolean, second: boolean, table: Uint32Array) => ({
    data: new Uint8Array(W * W).fill(fill),
    width: W,
    height: W,
    origin: { x: 0, y: 0 },
    table,
    first,
    second,
  });
  // entry 1 is red, 2 is grey, 0 transparent
  const table = pixelTable([0, 0x001f, 0x4210]);
  const red = table[1]!;
  const grey = table[2]!;

  it("blend two pixels as the PPU does: (a * eva + b * evb) / 16, at most 255, the weights at most 16", () => {
    expect(blendPixel(0xff0000ff, 4, 0xff000000, 16)).toBe((0xff000000 | 63) >>> 0);
    expect(blendPixel(0xff808080, 16, 0xff808080, 16)).toBe(0xffffffff);
    expect(blendPixel(0xff0000ff, 31, 0xff000000, 0)).toBe((0xff000000 | 255) >>> 0);
  });

  it("blend BG3 over BG0-2 only, not over the backdrop, and only with effect 1", () => {
    const out = new Uint32Array(W * W);
    const blend = { effect: 1, eva: 4, evb: 16 };
    const bg3 = layer(1, true, false, table);
    const bg0 = layer(2, false, true, table);
    compositeBlend([bg3, bg0], 0xff000000, blend, W, W, 0, 0, 1, W, W, out);
    expect(out[0]).toBe(blendPixel(red, 4, grey, 16));
    // BG3 over the backdrop: as it is
    compositeBlend([bg3], 0xff000000, blend, W, W, 0, 0, 1, W, W, out);
    expect(out[0]).toBe(red);
    // BG0 in front of BG3: no blend
    compositeBlend([bg0, bg3], 0xff000000, blend, W, W, 0, 0, 1, W, W, out);
    expect(out[0]).toBe(grey);
    // no effect, or brighten/darken (BLDY, which the room doesn't set): BG3 as it is
    for (const effect of [0, 2, 3]) {
      compositeBlend([bg3, bg0], 0xff000000, { ...blend, effect }, W, W, 0, 0, 1, W, W, out);
      expect(out[0]).toBe(red);
    }
    // outside the room: nothing
    compositeBlend([bg3, bg0], 0xff000000, blend, W, W, -2, 0, 1, W, W, out);
    expect(out[0]).toBe(0);
    expect(blends({ effect: 1, eva: 0, evb: 0 })).toBe(true);
    expect(blends({ effect: 0, eva: 31, evb: 7 })).toBe(false);
  });

  it("know each kind's start marker", () => {
    const room = jungle();
    const types = (json("games/xs/data/levels/entity_types.json") as { types: EntityTypeInfo[] }).types;
    expect(startMarkerFor(room, types)).toEqual({ want: "START_MARKER", have: "START_MARKER" });
    const wet = { ...room, record: { ...room.record!, kind: "UNDERWATER" as const } };
    expect(startMarkerFor(wet, types)).toEqual({ want: "UNDERWATER_START_MARKER", have: "START_MARKER" });
    // N-Tranced's types don't name the markers
    const n2 = (json("games/n2/data/levels/entity_types.json") as { types: EntityTypeInfo[] }).types;
    expect(startMarkerFor(room, n2)).toBeNull();
  });
});

describe("drawing tiles", () => {
  // a 4bpp set: tile 0 blank, tile 1 a diagonal, tile 2 a column on the left
  const tiles = new Uint8Array(3 * 64);
  for (let i = 0; i < 8; i++) tiles[64 + i * 8 + i] = 3;
  for (let i = 0; i < 8; i++) tiles[128 + i * 8] = 5;
  const layer = (): PixelLayer => ({ width: 32, height: 8, data: new Uint8Array(32 * 8) });

  it("redraws the cells that draw a tile, each in its flips and bank, and clears them", () => {
    const l = layer();
    const index = tileIndex(tiles, 3, 4);
    paintBlock(l, [{ x: 0, y: 0 }], tileBlock(tiles, { tile: 2, hflip: false, vflip: false, bank: 1 }, 4));
    paintBlock(l, [{ x: 2, y: 0 }], tileBlock(tiles, { tile: 2, hflip: true, vflip: false, bank: 3 }, 4));
    paintBlock(l, [{ x: 3, y: 0 }], tileBlock(tiles, { tile: 1, hflip: false, vflip: false, bank: 0 }, 4));
    expect(tileUses(l, index, 4, 2)).toBe(2);
    expect(layerBanks(l)).toEqual([0, 1, 3]);
    // tile 2 becomes a row along the top
    const row = new Uint8Array(64);
    for (let x = 0; x < 8; x++) row[x] = 7;
    const changes = replaceTile(l, index, 4, 2, row);
    expect(changes.map((c) => c.index)).toEqual([0, 2]);
    expect(readCell(l, 0, 0)[0]).toBe(16 + 7);
    expect(readCell(l, 2, 0)[7]).toBe(48 + 7); // flipped across: still the top row, in bank 3
    expect(readCell(l, 2, 0)[8]).toBe(48);
    expect(readCell(l, 3, 0)[0]).toBe(3); // tile 1's cell is left
    // cleared: blank, in the cell's bank
    const cleared = replaceTile(l, tileIndex(new Uint8Array([...tiles.subarray(0, 128), ...row]), 3, 4), 4, 2, new Uint8Array(64));
    expect(cleared.length).toBe(2);
    expect([...readCell(l, 2, 0)].every((p) => p === 48)).toBe(true);
  });

  it("adds a new tile to the set, or finds the one it is (flipped too)", () => {
    const index = tileIndex(tiles, 3, 4);
    const flipped = tileBlock(tiles, { tile: 2, hflip: true, vflip: false, bank: 0 }, 4);
    const same = addTile(tiles, 3, index, flipped, 4);
    expect(same.added).toBe(false);
    expect(same.brush).toMatchObject({ tile: 2, hflip: true });
    const dot = new Uint8Array(64);
    dot[9] = 2;
    const r = addTile(tiles, 3, index, dot, 4);
    expect(r).toMatchObject({ count: 4, added: true, brush: { tile: 3 } });
    expect(tileBlock(r.tiles, { tile: 3, hflip: false, vflip: false, bank: 0 }, 4)[9]).toBe(2);
    // its flips are it now
    const dotX = tileBlock(r.tiles, { tile: 3, hflip: true, vflip: false, bank: 0 }, 4);
    expect(index.get(blockKey(dotX, 4))).toMatchObject({ tile: 3, hflip: true });
  });

  it("pastes imported cells and counts the new tiles they make", () => {
    const l = layer();
    const index = tileIndex(tiles, 3, 4);
    const data = new Uint8Array(3 * 64);
    data.set(tileBlock(tiles, { tile: 1, hflip: false, vflip: false, bank: 2 }, 4), 0);
    data[64 + 5] = 4; // a new block
    data.set(data.subarray(64, 128).slice().reverse(), 128); // the same block turned around: the same tile
    const changes = pasteCells(l, { x: 1, y: 0 }, 3, 1, data);
    expect(changes.map((c) => c.index)).toEqual([1, 2, 3]);
    expect(newBlocks(l, index, 4, [1, 2, 3])).toBe(1);
    // outside the layer: left out
    expect(pasteCells(l, { x: 3, y: 0 }, 3, 1, data).map((c) => c.index)).toEqual([3]); // the rest is outside
  });
});

describe("the collision legend's values", () => {
  const legend = (game: string) =>
    (json(`games/${game}/data/levels/collision_legend.json`) as { cells: { value: string; colour: string; name: string }[] }).cells.map((e) => ({
      value: parseInt(e.value, 16),
      colour: `#${e.colour}`,
      name: e.name,
    }));

  it("spells a value as the legend does, and puts its bits together", () => {
    for (const e of legend("xs")) expect(valueName(e.value)).toBe(e.name);
    expect(composeValue({ type: 0x2b, flags: 5, open: 3 })).toBe(0x352b);
    expect(valueParts(0x352b)).toEqual({ type: 0x2b, flags: 5, open: 3 });
    expect(valueName(0x812e)).toBe("solid 0x2E, flags 0x1, TERRAIN_FLAG_OPEN_UP");
    expect(typeKind(0x24)).toMatch(/^solid/);
    expect(typeKind(0x23)).toMatch(/height/);
  });

  it("takes a new value in a colour of its own, of a type gTerrainTypes has or the legend's values do", () => {
    for (const game of ["xs", "n2"]) {
      const l = legend(game);
      expect(newValueErrors(0x0632, "#010203", l)).toEqual([]);
      expect(newValueErrors(0x05ab, "#010203", l)).toEqual([]); // 0xAB is past the table, but the ROM's rooms have it
      expect(newValueErrors(0x002b, "#010203", l)).toHaveLength(1); // there already
      expect(newValueErrors(0x0632, l[3]!.colour.toLowerCase(), l)).toHaveLength(1); // its colour is taken
      expect(newValueErrors(0x0040, "#010203", l)).toHaveLength(1);
      expect(newValueErrors(0x10000, "#010203", l)).toHaveLength(1);
      // a free colour is free
      const c = freeColour(0x0632, l);
      expect(c).toMatch(/^#[0-9A-F]{6}$/);
      expect(l.some((e) => e.colour.toUpperCase() === c)).toBe(false);
      expect(freeColour(0x0632, [...l, { value: 1, colour: c, name: "" }])).not.toBe(c);
    }
  });
});

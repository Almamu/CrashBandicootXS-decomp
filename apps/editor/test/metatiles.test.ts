// Meta-tiles (docs/editors.md, "Meta-tiles"): their file read and written
// back, a stamp's cells (flipped as a whole, banks shifted, blank cells
// kept or left out) as crashdata.levels.metatiles.stamp draws them, and a
// meta-tile made of a layer's cells.
import { describe, expect, it } from "vitest";
import type { Metatiles } from "@crash/schema-types";
import { badShift, blankCell, freeName, fromCandidate, fromCells, hexPixels, pixelsHex, readFile, sameMetaTile, stamp, stampBlocks, stampStep, tileNumbers, writeFile } from "../src/lib/level/metatiles.ts";
import { changedCells, readCell, tileBlock, tileIndex, type PixelLayer } from "../src/lib/level/paint.ts";

const ramp = Uint8Array.from({ length: 64 }, (_, i) => i % 16);
const doc: Metatiles = {
  metatiles: [
    { name: "ledge", size: [2, 1], cells: [[1, 0, 3], [1, 1, 3]] },
    { name: "post", size: [1, 2], cells: [[0, 0, 2], [1, 2, 2]] },
  ],
  tiles: ["0".repeat(64), Array.from(ramp, (p) => p.toString(16)).join("")],
};

function flipped(px: Uint8Array, f: number): Uint8Array {
  return tileBlock(px, { tile: 0, hflip: !!(f & 1), vflip: !!(f & 2), bank: 0 }, 8);
}

function layer(cols: number, rows: number, fill = 0): PixelLayer {
  return { width: cols * 8, height: rows * 8, data: new Uint8Array(cols * rows * 64).fill(fill) };
}

describe("the file", () => {
  it("reads and writes back", () => {
    const list = readFile(doc);
    expect(list.map((m) => [m.name, m.w, m.h])).toEqual([
      ["ledge", 2, 1],
      ["post", 1, 2],
    ]);
    expect(list[0]!.cells[1]!.pixels).toEqual(ramp);
    // written: the tiles in the order the cells name them, the same meta-tiles
    const out = writeFile(list, 4);
    expect(out.tiles).toEqual([doc.tiles[1], doc.tiles[0]]);
    readFile(out).forEach((m, i) => expect(sameMetaTile(m, list[i]!)).toBe(true));
    expect(writeFile(readFile(out), 4)).toEqual(out);
    expect(hexPixels(pixelsHex(ramp, 8))).toEqual(ramp);
    expect(pixelsHex(ramp, 8)).toHaveLength(128);
  });

  it("names each tile's pixels once, in the order the cells name them", () => {
    const list = readFile(doc).reverse();
    const out = writeFile(list, 4);
    expect(out.tiles).toEqual([doc.tiles[0], doc.tiles[1]]);
    expect(out.metatiles[1]!.cells).toEqual([
      [1, 0, 3],
      [1, 1, 3],
    ]);
  });
});

describe("stamping", () => {
  const [ledge, post] = readFile(doc) as [ReturnType<typeof readFile>[number], ReturnType<typeof readFile>[number]];

  it("draws as crashdata's stamp does: BG1-3 cells in their bank", () => {
    const s = stampBlocks(ledge, { flip: 0, shift: 0, skipBlank: false }, 4);
    expect(s.map((c) => [c.x, c.y])).toEqual([
      [0, 0],
      [1, 0],
    ]);
    expect(s[0]!.block).toEqual(ramp.map((p) => p + 48));
    expect(s[1]!.block).toEqual(flipped(ramp, 1).map((p) => p + 48));
  });

  it("flips as a whole and shifts every bank", () => {
    const s = stampBlocks(ledge, { flip: 1, shift: 2, skipBlank: false }, 4);
    expect(s.map((c) => [c.x, c.y])).toEqual([
      [1, 0],
      [0, 0],
    ]);
    expect(s[0]!.block).toEqual(flipped(ramp, 1).map((p) => p + 80));
    expect(s[1]!.block).toEqual(ramp.map((p) => p + 80));
    for (const c of stampBlocks(post, { flip: 2, shift: -2, skipBlank: false }, 4)) expect(new Set(Array.from(c.block, (p) => p >> 4))).toEqual(new Set([0]));
    expect(badShift(ledge, 13, 4)).toEqual([16]);
    expect(badShift(ledge, 13, 8)).toEqual([]);
  });

  it("writes the layer's cells, an undoable change each, inside the layer", () => {
    const l = layer(3, 3, 0x10);
    const changes = stamp(l, { x: 2, y: 1 }, ledge, { flip: 0, shift: 0, skipBlank: false }, 4);
    expect(changes.map((c) => c.index)).toEqual([1 * 3 + 2]);
    expect(readCell(l, 2, 1)).toEqual(ramp.map((p) => p + 48));
    expect(changedCells(l, new Uint8Array(l.data.length).fill(0x10), [5])).toHaveLength(1);
    // stamped again: nothing changes
    expect(stamp(l, { x: 2, y: 1 }, ledge, { flip: 0, shift: 0, skipBlank: false }, 4)).toEqual([]);
    // a bank past 15: nothing is stamped
    expect(stamp(l, { x: 0, y: 0 }, ledge, { flip: 0, shift: 13, skipBlank: false }, 4)).toEqual([]);
  });

  it("keeps the cells under blank ones when asked", () => {
    expect(blankCell(post.cells[0]!)).toBe(true);
    const l = layer(1, 2, 0x35);
    const kept = stamp(l, { x: 0, y: 0 }, post, { flip: 0, shift: 0, skipBlank: true }, 4);
    expect(kept.map((c) => c.index)).toEqual([1]);
    expect(readCell(l, 0, 0)).toEqual(new Uint8Array(64).fill(0x35));
    const all = stamp(layer(1, 2, 0x35), { x: 0, y: 0 }, post, { flip: 0, shift: 0, skipBlank: false }, 4);
    expect(all[0]!.after).toEqual(new Uint8Array(64).fill(0x20));
  });

  it("steps a drag a whole meta-tile at a time", () => {
    expect(stampStep({ x: 4, y: 4 }, { x: 5, y: 4 }, 2, 2)).toEqual({ x: 4, y: 4 });
    expect(stampStep({ x: 4, y: 4 }, { x: 6, y: 7 }, 2, 2)).toEqual({ x: 6, y: 6 });
    expect(stampStep({ x: 4, y: 4 }, { x: 3, y: 4 }, 2, 2)).toEqual({ x: 2, y: 4 });
  });
});

describe("making meta-tiles", () => {
  const tiles = new Uint8Array(3 * 64);
  tiles.set(ramp, 64);
  tiles.set(Uint8Array.from({ length: 64 }, (_, i) => (i * 7) % 16), 128);
  const index = tileIndex(tiles, 3, 4);

  it("of a layer's cells: their tiles, flips and banks, as stamped", () => {
    const m = readFile(doc)[0]!;
    const l = layer(4, 2);
    stamp(l, { x: 1, y: 1 }, m, { flip: 1, shift: 1, skipBlank: false }, 4);
    const back = fromCells(l, 1, 1, 2, 1, index, tiles, 4, "back");
    // the ledge is its own mirror image: flipped as a whole, its cells' flips swap places
    expect(back.cells.map((c) => [c.flip, c.bank])).toEqual([
      [0, 4],
      [1, 4],
    ]);
    expect(tileNumbers(back, index, 4)).toEqual([1, 1]);
    // stamped again unflipped, it draws the same cells
    const l2 = layer(4, 2);
    stamp(l2, { x: 1, y: 1 }, back, { flip: 0, shift: 0, skipBlank: false }, 4);
    expect(l2.data).toEqual(l.data);
    // a block no tile is: its own pixels
    const odd = layer(1, 1);
    odd.data.set(Uint8Array.from({ length: 64 }, (_, i) => 0x50 | (i % 3)));
    const own = fromCells(odd, 0, 0, 1, 1, index, tiles, 4, "own");
    expect(tileNumbers(own, index, 4)).toEqual([-1]);
    expect(own.cells[0]!.bank).toBe(5);
  });

  it("of a detected block", () => {
    const c = { size: [2, 1] as [number, number], cells: [[2, 3, 1], [0, 0, 1]] as [number, number, number][], count: 3, flipped: 0, shifted: 0, rooms: {}, at: [], score: 3 };
    const m = fromCandidate(c, tiles, "found");
    expect(tileNumbers(m, index, 4)).toEqual([2, 0]);
    expect(sameMetaTile(m, readFile(writeFile([m], 4))[0]!)).toBe(true);
  });

  it("names them apart", () => {
    const list = readFile(doc);
    expect(freeName(list, "ledge")).toBe("ledge 2");
    expect(freeName(list, "rock")).toBe("rock");
  });
});

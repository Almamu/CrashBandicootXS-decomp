// Meta-tiles (docs/editors.md, "Meta-tiles"): blocks of a tile set's cells,
// each cell a tile, its flips and (BG1-3) its palette bank, stamped onto a
// BG layer as a brush. They are the editor's own data, a file per tile set
// (games/<game>/data/levels/metatiles/<set>.json) that names its tiles by
// their pixels, so a renumbered set still draws them. A stamp is cells of
// the layer's picture like any painting: BG1-3's `bank * 16 + p` for all 64
// pixels of a cell, which the build reads back as the tile, flips and bank.
import type { MetatileCandidate, Metatiles } from "@crash/schema-types";
import { CELL, CELL_PIXELS, blockKey, brushOfBlock, readCell, sameBlock, tileBlock, writeCell, type CellChange, type PixelLayer, type TileBrush } from "./paint.ts";
import type { Point } from "./coords.ts";

/** A cell of a meta-tile: its tile's 64 pixels as the set has them (4bpp: 0-15), its flip (1 X, 2 Y) and bank. */
export interface MetaCell {
  pixels: Uint8Array;
  flip: number;
  bank: number;
}

export interface MetaTile {
  name: string;
  w: number;
  h: number;
  /** row by row */
  cells: MetaCell[];
}

/** How a meta-tile is stamped: flipped as a whole (1 X, 2 Y), its banks shifted, its blank cells left out. */
export interface StampOptions {
  flip: number;
  shift: number;
  skipBlank: boolean;
}

export const MAX_SIZE = 32;

export function pixelsHex(px: Uint8Array, bpp: 4 | 8): string {
  let s = "";
  for (let i = 0; i < CELL_PIXELS; i++) s += bpp === 4 ? (px[i]! & 15).toString(16) : px[i]!.toString(16).padStart(2, "0");
  return s;
}

export function hexPixels(hex: string): Uint8Array {
  const out = new Uint8Array(CELL_PIXELS);
  const two = hex.length === 2 * CELL_PIXELS;
  for (let i = 0; i < CELL_PIXELS; i++) out[i] = parseInt(two ? hex.slice(2 * i, 2 * i + 2) : hex[i]!, 16);
  return out;
}

/** A meta-tiles file's value as meta-tiles. */
export function readFile(doc: Metatiles): MetaTile[] {
  const tiles = doc.tiles.map(hexPixels);
  return doc.metatiles.map((m) => ({
    name: m.name,
    w: m.size[0],
    h: m.size[1],
    cells: m.cells.map(([t, flip, bank]) => ({ pixels: tiles[t] ?? new Uint8Array(CELL_PIXELS), flip, bank })),
  }));
}

/** Meta-tiles as their file's value: each tile's pixels once, in the order the cells first name them. */
export function writeFile(list: readonly MetaTile[], bpp: 4 | 8): Metatiles {
  const tiles: string[] = [];
  const at = new Map<string, number>();
  const metatiles = list.map((m) => {
    const cells = m.cells.map((c): [number, number, number] => {
      const hex = pixelsHex(c.pixels, bpp);
      let k = at.get(hex);
      if (k === undefined) {
        k = tiles.length;
        tiles.push(hex);
        at.set(hex, k);
      }
      return [k, c.flip & 3, bpp === 4 ? c.bank & 15 : 0];
    });
    return { name: m.name, size: [m.w, m.h] as [number, number], cells: cells as Metatiles["metatiles"][number]["cells"] };
  });
  return { metatiles, tiles };
}

/** Whether a cell is blank (tile 0: every pixel 0, the bank apart). */
export function blankCell(c: MetaCell): boolean {
  for (let i = 0; i < CELL_PIXELS; i++) if (c.pixels[i] !== 0) return false;
  return true;
}

/** The banks a stamp shifted by `shift` would draw outside 0-15 (an empty list: it can be stamped). */
export function badShift(m: MetaTile, shift: number, bpp: 4 | 8): number[] {
  if (bpp === 8) return [];
  return [...new Set(m.cells.map((c) => c.bank + shift).filter((b) => b < 0 || b > 15))];
}

/**
 * The cells a meta-tile draws, flipped as a whole (the cells change places
 * and each one's flips turn too) and its banks shifted: [x, y, the 64
 * palette indices] (BG1-3: `bank * 16 + p`, one bank a cell).
 */
export function stampBlocks(m: MetaTile, o: StampOptions, bpp: 4 | 8): { x: number; y: number; block: Uint8Array; blank: boolean }[] {
  const out: { x: number; y: number; block: Uint8Array; blank: boolean }[] = [];
  for (let j = 0; j < m.h; j++) {
    for (let i = 0; i < m.w; i++) {
      const c = m.cells[j * m.w + i]!;
      const f = c.flip ^ o.flip;
      const brush: TileBrush = { tile: 0, hflip: !!(f & 1), vflip: !!(f & 2), bank: bpp === 4 ? c.bank + o.shift : 0 };
      out.push({ x: o.flip & 1 ? m.w - 1 - i : i, y: o.flip & 2 ? m.h - 1 - j : j, block: tileBlock(c.pixels, brush, bpp), blank: blankCell(c) });
    }
  }
  return out;
}

/** Stamps a meta-tile with its top left at cell `at`: the cells that change, those inside the layer (a blank cell left out with skipBlank). */
export function stamp(layer: PixelLayer, at: Point, m: MetaTile, o: StampOptions, bpp: 4 | 8): CellChange<Uint8Array>[] {
  if (badShift(m, o.shift, bpp).length) return [];
  const cols = layer.width / CELL;
  const rows = layer.height / CELL;
  const out: CellChange<Uint8Array>[] = [];
  for (const s of stampBlocks(m, o, bpp)) {
    if (o.skipBlank && s.blank) continue;
    const cx = at.x + s.x;
    const cy = at.y + s.y;
    if (cx < 0 || cy < 0 || cx >= cols || cy >= rows) continue;
    const before = readCell(layer, cx, cy);
    if (sameBlock(before, s.block)) continue;
    writeCell(layer, cx, cy, s.block);
    out.push({ index: cy * cols + cx, before, after: s.block });
  }
  return out;
}

/**
 * A meta-tile of a layer's cells (a rectangle of them): each cell's tile
 * and flips as the set reads it back (a block no tile is yet is its own
 * pixels, unflipped) and its bank.
 */
export function fromCells(layer: PixelLayer, x: number, y: number, w: number, h: number, index: Map<string, TileBrush>, tiles: Uint8Array, bpp: 4 | 8, name: string): MetaTile {
  const cells: MetaCell[] = [];
  for (let j = 0; j < h; j++) {
    for (let i = 0; i < w; i++) {
      const block = readCell(layer, x + i, y + j);
      const bank = bpp === 4 ? block[0]! >> 4 : 0;
      const b = brushOfBlock(index, block, bpp);
      if (b) cells.push({ pixels: tiles.slice(b.tile * CELL_PIXELS, (b.tile + 1) * CELL_PIXELS), flip: (b.hflip ? 1 : 0) | (b.vflip ? 2 : 0), bank });
      else cells.push({ pixels: block.map((p) => (bpp === 4 ? p & 15 : p)), flip: 0, bank });
    }
  }
  return { name, w, h, cells };
}

/** A detected block (the set's tile numbers) as a meta-tile. */
export function fromCandidate(c: MetatileCandidate, tiles: Uint8Array, name: string): MetaTile {
  return {
    name,
    w: c.size[0],
    h: c.size[1],
    cells: c.cells.map(([t, flip, bank]) => ({ pixels: tiles.slice(t * CELL_PIXELS, (t + 1) * CELL_PIXELS), flip, bank })),
  };
}

/** Each cell's tile number in the set now (-1: no tile of it draws the cell; a stamp adds it). */
export function tileNumbers(m: MetaTile, index: Map<string, TileBrush>, bpp: 4 | 8): number[] {
  return m.cells.map((c) => index.get(blockKey(c.pixels, bpp))?.tile ?? -1);
}

/** Whether two meta-tiles draw the same cells. */
export function sameMetaTile(a: MetaTile, b: MetaTile): boolean {
  return a.w === b.w && a.h === b.h && a.cells.every((c, i) => c.flip === b.cells[i]!.flip && c.bank === b.cells[i]!.bank && sameBlock(c.pixels, b.cells[i]!.pixels));
}

/** A name no meta-tile of the list has: `base`, else `base 2`, `base 3`, ... */
export function freeName(list: readonly MetaTile[], base: string): string {
  const names = new Set(list.map((m) => m.name));
  if (!names.has(base)) return base;
  for (let k = 2; ; k++) if (!names.has(`${base} ${k}`)) return `${base} ${k}`;
}

/** The cell positions a drag stamps at: the start, then every whole meta-tile's step from it towards the cursor. */
export function stampStep(start: Point, cursor: Point, w: number, h: number): Point {
  return { x: start.x + Math.floor((cursor.x - start.x) / w) * w, y: start.y + Math.floor((cursor.y - start.y) / h) * h };
}

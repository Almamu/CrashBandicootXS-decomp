// Painting a room's layers (docs/levels.md, "BG layers", "Collision layers").
//
// A BG layer is its picture's palette indices, 8x8 pixels a cell. Painting a
// cell draws a tile of the layer's tile set into it, flipped as asked: on BG0
// (8bpp) the tile's own indices; on BG1-3 (4bpp) `bank * 16 + pixel` for all
// 64 pixels, the transparent ones too, so the cell keeps its bank. The build
// reads the block back as that tile (or its flip), so painting with the set's
// tiles never adds one. The collision layer is one value a cell.
import type { Point } from "./coords.ts";

export const CELL = 8;
export const CELL_PIXELS = CELL * CELL;

/** A cell's brush: a tile of the set, its flips and (BG1-3) its palette bank. */
export interface TileBrush {
  tile: number;
  hflip: boolean;
  vflip: boolean;
  bank: number;
}

/** A layer's picture: its palette indices, row by row. */
export interface PixelLayer {
  width: number;
  height: number;
  data: Uint8Array;
}

/** The 64 pixels of a tile as a cell draws it (`tiles`: 64 bytes a tile; 4bpp tiles' pixels are 0-15). */
export function tileBlock(tiles: Uint8Array, b: TileBrush, bpp: 4 | 8): Uint8Array {
  const out = new Uint8Array(CELL_PIXELS);
  const base = b.tile * CELL_PIXELS;
  const add = bpp === 4 ? (b.bank & 15) * 16 : 0;
  for (let y = 0; y < CELL; y++) {
    const sy = b.vflip ? CELL - 1 - y : y;
    for (let x = 0; x < CELL; x++) {
      const sx = b.hflip ? CELL - 1 - x : x;
      out[y * CELL + x] = (tiles[base + sy * CELL + sx] ?? 0) + add;
    }
  }
  return out;
}

/** A cell's value in the layer's cells (the build's): BG0 tile | flip << 14, BG1-3 tile | flip << 10 | bank << 12. */
export function cellValue(b: TileBrush, bg0: boolean): number {
  const flip = (b.hflip ? 1 : 0) | (b.vflip ? 2 : 0);
  return bg0 ? (b.tile & 0x3fff) | (flip << 14) : (b.tile & 0x3ff) | (flip << 10) | ((b.bank & 15) << 12);
}

/** A cell value as a brush. */
export function brushOf(v: number, bg0: boolean): TileBrush {
  if (bg0) return { tile: v & 0x3fff, hflip: !!(v & 0x4000), vflip: !!(v & 0x8000), bank: 0 };
  return { tile: v & 0x3ff, hflip: !!(v & 0x400), vflip: !!(v & 0x800), bank: (v >> 12) & 15 };
}

export function readCell(layer: PixelLayer, cx: number, cy: number): Uint8Array {
  const out = new Uint8Array(CELL_PIXELS);
  for (let y = 0; y < CELL; y++) {
    const o = (cy * CELL + y) * layer.width + cx * CELL;
    out.set(layer.data.subarray(o, o + CELL), y * CELL);
  }
  return out;
}

export function writeCell(layer: PixelLayer, cx: number, cy: number, block: Uint8Array): void {
  for (let y = 0; y < CELL; y++) {
    layer.data.set(block.subarray(y * CELL, y * CELL + CELL), (cy * CELL + y) * layer.width + cx * CELL);
  }
}

export function sameBlock(a: Uint8Array, b: Uint8Array): boolean {
  if (a.length !== b.length) return false;
  for (let i = 0; i < a.length; i++) if (a[i] !== b[i]) return false;
  return true;
}

/** One cell's change: its index (cy * cols + cx), what it was, what it is. */
export interface CellChange<T> {
  index: number;
  before: T;
  after: T;
}

/** Paints cells of a BG layer with a block: the cells that change (a cell that already has the block is left out). */
export function paintBlock(layer: PixelLayer, at: readonly Point[], block: Uint8Array): CellChange<Uint8Array>[] {
  const cols = layer.width / CELL;
  const out: CellChange<Uint8Array>[] = [];
  const seen = new Set<number>();
  for (const p of at) {
    const index = p.y * cols + p.x;
    if (seen.has(index)) continue;
    seen.add(index);
    const before = readCell(layer, p.x, p.y);
    if (sameBlock(before, block)) continue;
    writeCell(layer, p.x, p.y, block);
    out.push({ index, before, after: block });
  }
  return out;
}

/** Puts cell changes back (`undo`) or again. */
export function applyBlocks(layer: PixelLayer, changes: readonly CellChange<Uint8Array>[], undo: boolean): void {
  const cols = layer.width / CELL;
  const list = undo ? [...changes].reverse() : changes;
  for (const c of list) writeCell(layer, c.index % cols, Math.floor(c.index / cols), undo ? c.before : c.after);
}

/** Sets collision cells to a value: the cells that change. */
export function paintValue(cells: Uint16Array, cols: number, at: readonly Point[], value: number): CellChange<number>[] {
  const out: CellChange<number>[] = [];
  for (const p of at) {
    const index = p.y * cols + p.x;
    const before = cells[index]!;
    if (before === value) continue;
    cells[index] = value;
    out.push({ index, before, after: value });
  }
  return out;
}

export function applyValues(cells: Uint16Array, changes: readonly CellChange<number>[], undo: boolean): void {
  const list = undo ? [...changes].reverse() : changes;
  for (const c of list) cells[c.index] = undo ? c.before : c.after;
}

/** Whether a BG1-3 block's 64 pixels are all of one palette bank (what the build requires). */
export function oneBank(block: Uint8Array): boolean {
  const bank = block[0]! >> 4;
  for (let i = 1; i < block.length; i++) if (block[i]! >> 4 !== bank) return false;
  return true;
}

/**
 * The cells of a layer that differ from the picture as it was opened: what a
 * save sends, as [cx, cy, the 64 pixels].
 */
export function changedCells(layer: PixelLayer, original: Uint8Array, touched: Iterable<number>): [number, number, Uint8Array][] {
  const cols = layer.width / CELL;
  const out: [number, number, Uint8Array][] = [];
  const orig: PixelLayer = { width: layer.width, height: layer.height, data: original };
  for (const i of [...touched].sort((a, b) => a - b)) {
    const cx = i % cols;
    const cy = Math.floor(i / cols);
    const now = readCell(layer, cx, cy);
    if (!sameBlock(now, readCell(orig, cx, cy))) out.push([cx, cy, now]);
  }
  return out;
}

/** Bytes as base64 (the save's cells). */
export function base64(bytes: Uint8Array): string {
  let s = "";
  for (let i = 0; i < bytes.length; i++) s += String.fromCharCode(bytes[i]!);
  return btoa(s);
}

export function fromBase64(s: string): Uint8Array {
  const bin = atob(s);
  const out = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; i++) out[i] = bin.charCodeAt(i);
  return out;
}

/** u16 little-endian bytes (the tile set's cells) as numbers. */
export function u16le(bytes: Uint8Array): Uint16Array {
  const out = new Uint16Array(bytes.length >> 1);
  for (let i = 0; i < out.length; i++) out[i] = bytes[2 * i]! | (bytes[2 * i + 1]! << 8);
  return out;
}

/** A block's key in a tile index: its pixels (4bpp: their low nibbles). */
export function blockKey(block: Uint8Array, bpp: 4 | 8): string {
  let s = "";
  for (let i = 0; i < block.length; i++) s += String.fromCharCode(bpp === 4 ? block[i]! & 15 : block[i]!);
  return s;
}

/**
 * Every tile of a set under its four flips, by block: what a cell of a layer
 * is (the eyedropper), as the build reads it back (the first tile, in the
 * first flip, that gives the block: levels.py's generate_tileset).
 */
export function tileIndex(tiles: Uint8Array, count: number, bpp: 4 | 8): Map<string, TileBrush> {
  const out = new Map<string, TileBrush>();
  for (let t = 0; t < count; t++) {
    for (const [hflip, vflip] of [[false, false], [true, false], [false, true], [true, true]] as const) {
      const key = blockKey(tileBlock(tiles, { tile: t, hflip, vflip, bank: 0 }, bpp), bpp);
      if (!out.has(key)) out.set(key, { tile: t, hflip, vflip, bank: 0 });
    }
  }
  return out;
}

/** The brush that draws a block (its tile, flips and bank), or null if no tile of the set is it. */
export function brushOfBlock(index: Map<string, TileBrush>, block: Uint8Array, bpp: 4 | 8): TileBrush | null {
  const b = index.get(blockKey(block, bpp));
  if (!b) return null;
  return { ...b, bank: bpp === 4 ? block[0]! >> 4 : 0 };
}

// -- drawing tiles (docs/editors.md, "Tiles") -----------------------------------
//
// A tile set is made from its rooms' pictures, so a tile is drawn by drawing
// cells: an edited tile replaces the cells that draw it (each in its own
// flips and bank), a new tile is a block no tile of the set is yet, which
// the build adds at the end of the set once a cell draws it, and a tile no
// cell draws any more is gone from the set.

/** A tile's 64 pixels (4bpp: 0-15) drawn as a cell does: flipped, and on BG1-3 in a bank. */
export function drawTile(pixels: Uint8Array, b: TileBrush, bpp: 4 | 8): Uint8Array {
  return tileBlock(pixels, { ...b, tile: 0 }, bpp);
}

/**
 * Replaces every cell of a layer that draws `tile` (under any flip, in any
 * bank) with `pixels` drawn in that cell's flips and bank: the cells that
 * change. `pixels` of zeros clears them (BG1-3: blank in their bank).
 */
export function replaceTile(layer: PixelLayer, index: Map<string, TileBrush>, bpp: 4 | 8, tile: number, pixels: Uint8Array): CellChange<Uint8Array>[] {
  const cols = layer.width / CELL;
  const rows = layer.height / CELL;
  const out: CellChange<Uint8Array>[] = [];
  for (let cy = 0; cy < rows; cy++) {
    for (let cx = 0; cx < cols; cx++) {
      const before = readCell(layer, cx, cy);
      const b = brushOfBlock(index, before, bpp);
      if (!b || b.tile !== tile) continue;
      const after = drawTile(pixels, b, bpp);
      if (sameBlock(before, after)) continue;
      writeCell(layer, cx, cy, after);
      out.push({ index: cy * cols + cx, before, after });
    }
  }
  return out;
}

/** How many cells of a layer draw `tile`. */
export function tileUses(layer: PixelLayer, index: Map<string, TileBrush>, bpp: 4 | 8, tile: number): number {
  const cols = layer.width / CELL;
  const rows = layer.height / CELL;
  let n = 0;
  for (let cy = 0; cy < rows; cy++) for (let cx = 0; cx < cols; cx++) if (brushOfBlock(index, readCell(layer, cx, cy), bpp)?.tile === tile) n++;
  return n;
}

/**
 * A set with a tile added at its end (a block of 64 pixels, 4bpp: 0-15), or
 * the brush of the tile that already is it (flips included): {tiles, count,
 * index, brush, added}.
 */
export function addTile(
  tiles: Uint8Array,
  count: number,
  index: Map<string, TileBrush>,
  pixels: Uint8Array,
  bpp: 4 | 8,
): { tiles: Uint8Array; count: number; brush: TileBrush; added: boolean } {
  const have = index.get(blockKey(pixels, bpp));
  if (have) return { tiles, count, brush: { ...have }, added: false };
  const out = new Uint8Array((count + 1) * CELL_PIXELS);
  out.set(tiles.subarray(0, count * CELL_PIXELS));
  out.set(pixels.subarray(0, CELL_PIXELS), count * CELL_PIXELS);
  for (const [hflip, vflip] of [[false, false], [true, false], [false, true], [true, true]] as const) {
    const key = blockKey(tileBlock(out, { tile: count, hflip, vflip, bank: 0 }, bpp), bpp);
    if (!index.has(key)) index.set(key, { tile: count, hflip, vflip, bank: 0 });
  }
  return { tiles: out, count: count + 1, brush: { tile: count, hflip: false, vflip: false, bank: 0 }, added: true };
}

/**
 * Writes imported cells (`data`: cols * rows blocks of 64 indices, row by
 * row) into a layer at cell `at`, those inside it: the cells that change.
 */
export function pasteCells(layer: PixelLayer, at: Point, cols: number, rows: number, data: Uint8Array): CellChange<Uint8Array>[] {
  const lc = layer.width / CELL;
  const lr = layer.height / CELL;
  const out: CellChange<Uint8Array>[] = [];
  for (let y = 0; y < rows; y++) {
    for (let x = 0; x < cols; x++) {
      const cx = at.x + x;
      const cy = at.y + y;
      if (cx < 0 || cy < 0 || cx >= lc || cy >= lr) continue;
      const after = data.slice((y * cols + x) * CELL_PIXELS, (y * cols + x + 1) * CELL_PIXELS);
      const before = readCell(layer, cx, cy);
      if (sameBlock(before, after)) continue;
      writeCell(layer, cx, cy, after);
      out.push({ index: cy * lc + cx, before, after });
    }
  }
  return out;
}

/** The new tiles a layer's cells would add to its set: the blocks no tile is, each once (flips included). */
export function newBlocks(layer: PixelLayer, index: Map<string, TileBrush>, bpp: 4 | 8, cells: Iterable<number>): number {
  const cols = layer.width / CELL;
  const seen = new Set<string>();
  let n = 0;
  for (const i of cells) {
    const block = readCell(layer, i % cols, Math.floor(i / cols));
    const key = blockKey(block, bpp);
    if (index.has(key) || seen.has(key)) continue;
    n++;
    // its flips are the same tile
    for (const [hflip, vflip] of [[false, false], [true, false], [false, true], [true, true]] as const) {
      seen.add(blockKey(tileBlock(block.map((p) => (bpp === 4 ? p & 15 : p)), { tile: 0, hflip, vflip, bank: 0 }, bpp), bpp));
    }
  }
  return n;
}

/** The palette banks a BG1-3 layer's cells draw in (a cell's pixels are all of one bank). */
export function layerBanks(layer: PixelLayer): number[] {
  const seen = new Array<boolean>(16).fill(false);
  for (let y = 0; y < layer.height; y += CELL) for (let x = 0; x < layer.width; x += CELL) seen[layer.data[y * layer.width + x]! >> 4] = true;
  return seen.flatMap((b, i) => (b ? [i] : []));
}

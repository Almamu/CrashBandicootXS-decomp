// Drawing a room's layers on a canvas. A layer's picture (a BG layer's palette
// indices, or the collision layer's cells) is turned into RGBA in tiles of
// TILE x TILE bitmap pixels, each its own canvas, made when first shown and
// again when its part of the picture (or the palette) changes; a frame draws
// the visible tiles scaled to the view, unsmoothed.
import type { Point, View } from "./coords.ts";

export const TILE = 512;

/** The tiles (tx, ty) of a bitmap (width x height, `scale` world pixels a bitmap pixel, at `origin`) a view shows. */
export function visibleTiles(view: View, cw: number, ch: number, origin: Point, width: number, height: number, scale = 1): [number, number][] {
  const span = TILE * scale;
  const x0 = Math.max(0, Math.floor((view.x - origin.x) / span));
  const y0 = Math.max(0, Math.floor((view.y - origin.y) / span));
  const x1 = Math.min(Math.ceil(width / TILE) - 1, Math.floor((view.x + cw / view.zoom - origin.x) / span));
  const y1 = Math.min(Math.ceil(height / TILE) - 1, Math.floor((view.y + ch / view.zoom - origin.y) / span));
  const out: [number, number][] = [];
  for (let ty = y0; ty <= y1; ty++) for (let tx = x0; tx <= x1; tx++) out.push([tx, ty]);
  return out;
}

/** Fills a tile's pixels (32-bit ABGR words, `w` x `h`) from the bitmap's (x0, y0). */
export type TileFill = (out: Uint32Array, x0: number, y0: number, w: number, h: number) => void;

export class TiledBitmap {
  readonly width: number;
  readonly height: number;
  /** world pixels a bitmap pixel */
  readonly scale: number;
  private fill: TileFill;
  private readonly cols: number;
  private readonly canvases: (HTMLCanvasElement | null)[];
  private readonly stale: Uint8Array;

  constructor(width: number, height: number, fill: TileFill, scale = 1) {
    this.width = width;
    this.height = height;
    this.scale = scale;
    this.fill = fill;
    this.cols = Math.ceil(width / TILE);
    const n = this.cols * Math.ceil(height / TILE);
    this.canvases = new Array<HTMLCanvasElement | null>(n).fill(null);
    this.stale = new Uint8Array(n);
  }

  setFill(fill: TileFill): void {
    this.fill = fill;
    this.invalidateAll();
  }

  invalidateAll(): void {
    this.stale.fill(1);
  }

  /** Marks the tiles under a bitmap rectangle to be made again. */
  invalidate(x: number, y: number, w: number, h: number): void {
    const tx0 = Math.max(0, Math.floor(x / TILE));
    const ty0 = Math.max(0, Math.floor(y / TILE));
    const tx1 = Math.floor((x + w - 1) / TILE);
    const ty1 = Math.floor((y + h - 1) / TILE);
    for (let ty = ty0; ty <= ty1; ty++) for (let tx = tx0; tx <= Math.min(tx1, this.cols - 1); tx++) this.stale[ty * this.cols + tx] = 1;
  }

  private tile(tx: number, ty: number): HTMLCanvasElement | null {
    const i = ty * this.cols + tx;
    if (i < 0 || i >= this.canvases.length) return null;
    let c = this.canvases[i] ?? null;
    if (c && !this.stale[i]) return c;
    const w = Math.min(TILE, this.width - tx * TILE);
    const h = Math.min(TILE, this.height - ty * TILE);
    if (!c) {
      c = document.createElement("canvas");
      c.width = w;
      c.height = h;
      this.canvases[i] = c;
    }
    const ctx = c.getContext("2d");
    if (!ctx) return null;
    const img = ctx.createImageData(w, h);
    this.fill(new Uint32Array(img.data.buffer), tx * TILE, ty * TILE, w, h);
    ctx.putImageData(img, 0, 0);
    this.stale[i] = 0;
    return c;
  }

  /** Draws the bitmap with its (0, 0) at world `origin`. */
  draw(ctx: CanvasRenderingContext2D, view: View, cw: number, ch: number, origin: Point): void {
    for (const [tx, ty] of visibleTiles(view, cw, ch, origin, this.width, this.height, this.scale)) {
      const c = this.tile(tx, ty);
      if (!c) continue;
      const wx = origin.x + tx * TILE * this.scale;
      const wy = origin.y + ty * TILE * this.scale;
      ctx.drawImage(c, (wx - view.x) * view.zoom, (wy - view.y) * view.zoom, c.width * this.scale * view.zoom, c.height * this.scale * view.zoom);
    }
  }
}

/** A BG layer's fill: its indices through the palette's pixel table. */
export function indexFill(data: Uint8Array, width: number, table: () => Uint32Array): TileFill {
  return (out, x0, y0, w, h) => {
    const t = table();
    for (let y = 0; y < h; y++) {
      const row = (y0 + y) * width + x0;
      for (let x = 0; x < w; x++) out[y * w + x] = t[data[row + x]!]!;
    }
  };
}

/** The collision layer's fill: a bitmap pixel a cell, its legend colour (0: none). */
export function cellFill(cells: Uint16Array, cols: number, colour: (v: number) => number): TileFill {
  return (out, x0, y0, w, h) => {
    for (let y = 0; y < h; y++) {
      const row = (y0 + y) * cols + x0;
      for (let x = 0; x < w; x++) out[y * w + x] = colour(cells[row + x]!);
    }
  };
}

/** Whether a BG layer's index is transparent: BG0's (8bpp) 0, BG1-3's (4bpp) pixel 0 of each bank. */
export function transparentIndex(bg0: boolean): (i: number) => boolean {
  return bg0 ? (i) => i === 0 : (i) => (i & 15) === 0;
}


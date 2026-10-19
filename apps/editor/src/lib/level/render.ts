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


/** A BG layer as the blend composite reads it: its picture, where it is, and its pixels' colours (0: transparent). */
export interface BlendLayer {
  data: Uint8Array;
  width: number;
  height: number;
  /** the layer's (0, 0) in the world */
  origin: Point;
  /** palette index -> 32-bit pixel (ABGR words), 0 for a transparent index */
  table: Uint32Array;
  /** BLDCNT's first / second target bits for this layer */
  first: boolean;
  second: boolean;
}

/** The room's colour effect (room.json's record "blend", SetupRoomBlend): BLDCNT's effect, BLDALPHA's weights. */
export interface Blend {
  effect: number;
  eva: number;
  evb: number;
}

/** The backdrop: black, as a pixel (LoadRoom sets BACKDROP_COLOR to 0 after the room's palette: its entry 0 is never shown). */
export const BACKDROP = 0xff000000;

/** BLDCNT's targets for a room's blend (SetupRoomBlend): BG3 first, BG0-2 (and the sprites) second. */
export const BLEND_FIRST: Record<string, boolean> = { bg3: true };
export const BLEND_SECOND: Record<string, boolean> = { bg0: true, bg1: true, bg2: true };

/** Whether the composite is needed: alpha blending (effect 1) changes the picture; 0 doesn't, 2 and 3 use BLDY, which the room doesn't set. */
export function blends(b: Blend | undefined | null): boolean {
  return !!b && (b.effect & 3) === 1;
}

/** Two pixels (ABGR words) blended as the PPU does (platform/pc's Blend): each channel (a * eva + b * evb) / 16, at most 255; the weights at most 16. */
export function blendPixel(a: number, eva: number, b: number, evb: number): number {
  const ea = Math.min(eva & 31, 16);
  const eb = Math.min(evb & 31, 16);
  let out = 0xff000000;
  for (let shift = 0; shift <= 16; shift += 8) {
    const c = Math.floor((((a >>> shift) & 0xff) * ea + ((b >>> shift) & 0xff) * eb) / 16);
    out |= Math.min(c, 255) << shift;
  }
  return out >>> 0;
}

/**
 * The room's BG layers composited with its alpha blend, as the PPU does it:
 * per pixel the front layer's colour, unless it is a first target and the
 * next opaque layer under it is a second target, when the two are blended
 * (the backdrop isn't a second target: SetupRoomBlend leaves BLDCNT's bit
 * 13 clear). `layers` front to back; the output is `w` x `h` pixels, pixel
 * (i, j) the world's (x0 + i * step, y0 + j * step); outside the room's
 * picture (`roomW` x `roomH`) it is transparent, else the backdrop where no
 * layer is.
 */
export function compositeBlend(
  layers: BlendLayer[],
  backdrop: number,
  blend: Blend,
  roomW: number,
  roomH: number,
  x0: number,
  y0: number,
  step: number,
  w: number,
  h: number,
  out: Uint32Array,
): void {
  const alpha = blends(blend);
  for (let j = 0; j < h; j++) {
    const wy = Math.floor(y0 + j * step);
    for (let i = 0; i < w; i++) {
      const wx = Math.floor(x0 + i * step);
      const o = j * w + i;
      if (wx < 0 || wy < 0 || wx >= roomW || wy >= roomH) {
        out[o] = 0;
        continue;
      }
      let top = backdrop;
      let topLayer: BlendLayer | null = null;
      let under = backdrop;
      let underLayer: BlendLayer | null = null;
      for (const l of layers) {
        const lx = wx - l.origin.x;
        const ly = wy - l.origin.y;
        if (lx < 0 || ly < 0 || lx >= l.width || ly >= l.height) continue;
        const px = l.table[l.data[ly * l.width + lx]!]!;
        if (!px) continue;
        if (!topLayer) {
          top = px;
          topLayer = l;
          if (!alpha || !l.first) break;
        } else {
          under = px;
          underLayer = l;
          break;
        }
      }
      out[o] = topLayer && underLayer?.second ? blendPixel(top, blend.eva, under, blend.evb) : top;
    }
  }
}

// A cutscene screen as the GBA shows it (docs/editors.md, "The cutscene
// editor"): the slide's picture on BG2 in Mode 4 (its 256 colours, at the
// screen's top left), and the text's glyphs as OBJs of the small font over
// it, a lower OAM entry (an earlier glyph) in front where two overlap.
// Colours are widened as the hardware's (and the PC port's) c << 3 | c >> 2.

import type { Glyph } from "./text.ts";

export const SCREEN_W = 240;
export const SCREEN_H = 160;

/** The OBJ sizes of attribute 0's shape at size 0 (the small font's). */
export const SHAPE_SIZE: Record<number, [number, number]> = { 0: [8, 8], 1: [16, 8], 2: [8, 16] };

export interface Picture {
  palette: ArrayLike<number>;
  pixels: Uint8Array;
}

export interface FontPixels {
  /** each glyph's cell, cell x cell bytes (colour indices 0-15) */
  glyphs: Uint8Array;
  cell: number;
  shapes: number[];
  /** 16 BGR555 colours */
  palette: ArrayLike<number>;
}

/** A BGR555 colour as a 32-bit pixel (RGBA in memory), the hardware's widening. */
export function gbaPixel(v: number): number {
  const r = v & 31;
  const g = (v >> 5) & 31;
  const b = (v >> 10) & 31;
  return (0xff << 24) | (((b << 3) | (b >> 2)) << 16) | (((g << 3) | (g >> 2)) << 8) | ((r << 3) | (r >> 2));
}

/** Draws a screen into `out` (240x160 RGBA words): the picture (black without one), then the glyphs. */
export function renderScreen(out: Uint32Array, picture: Picture | null, font: FontPixels, glyphs: Glyph[]): void {
  if (picture) {
    const table = new Uint32Array(256);
    for (let i = 0; i < 256; i++) table[i] = gbaPixel(picture.palette[i] ?? 0);
    for (let i = 0; i < SCREEN_W * SCREEN_H; i++) out[i] = table[picture.pixels[i] ?? 0]!;
  } else out.fill(0xff000000);
  const colours = new Uint32Array(16);
  for (let i = 0; i < 16; i++) colours[i] = gbaPixel(font.palette[i] ?? 0);
  const covered = new Uint8Array(SCREEN_W * SCREEN_H);
  // OAM's order: the first glyph is in front
  for (const gl of glyphs) {
    const [w, h] = SHAPE_SIZE[font.shapes[gl.g] ?? 0] ?? [8, 8];
    const base = gl.g * font.cell * font.cell;
    for (let yy = 0; yy < h; yy++) {
      const y = (gl.y + yy) & 0xff;
      if (y >= SCREEN_H) continue;
      for (let xx = 0; xx < w; xx++) {
        const x = (gl.x + xx) & 0x1ff;
        if (x >= SCREEN_W) continue;
        const v = font.glyphs[base + yy * font.cell + xx]!;
        const at = y * SCREEN_W + x;
        if (v && !covered[at]) {
          out[at] = colours[v]!;
          covered[at] = 1;
        }
      }
    }
  }
}

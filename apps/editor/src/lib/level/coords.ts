// The room view's coordinates. The world is the room's BG0 and collision
// layers' pixels (the entities' x and y, 8 pixels a cell); the canvas shows
// it at a zoom, panned. The game's camera is a 240x160 screen inside the
// room's scrollable size, and each BG layer scrolls by its parallax factor
// (BgLayerBase::ScaleScroll: camera * scale / 256, rounded toward zero), so a
// layer drawn for one camera position is shifted against the world by
// `camera - scroll(camera)`.

export const SCREEN_W = 240;
export const SCREEN_H = 160;
export const CELL = 8;
/** An entity group (a 256-pixel column, docs/levels.md "Entities"). */
export const COLUMN = 256;

export interface View {
  /** canvas pixels per world pixel */
  zoom: number;
  /** the world point at the canvas's top left */
  x: number;
  y: number;
}

export interface Point {
  x: number;
  y: number;
}

export const ZOOM_MIN = 1 / 16;
export const ZOOM_MAX = 16;

export function toWorld(v: View, sx: number, sy: number): Point {
  return { x: v.x + sx / v.zoom, y: v.y + sy / v.zoom };
}

export function toScreen(v: View, wx: number, wy: number): Point {
  return { x: (wx - v.x) * v.zoom, y: (wy - v.y) * v.zoom };
}

/** The view zoomed by `factor` about a canvas point, which stays where it is. */
export function zoomAt(v: View, sx: number, sy: number, factor: number): View {
  const zoom = Math.min(ZOOM_MAX, Math.max(ZOOM_MIN, v.zoom * factor));
  const w = toWorld(v, sx, sy);
  return { zoom, x: w.x - sx / zoom, y: w.y - sy / zoom };
}

/** The view showing a world rectangle in a canvas of (cw, ch), centred, at a power-of-two zoom. */
export function fit(cw: number, ch: number, w: number, h: number): View {
  const z = Math.min(cw / w, ch / h);
  const zoom = Math.min(ZOOM_MAX, Math.max(ZOOM_MIN, 2 ** Math.floor(Math.log2(z))));
  return { zoom, x: (w - cw / zoom) / 2, y: (h - ch / zoom) / 2 };
}

/** Q8 times an integer, rounded toward zero (ScaleBgLayerScroll). */
export function scaleQ8(v: number, scale: number): number {
  return Math.trunc((v * scale) / 256);
}

/** The camera (the screen's top left) clamped to the room's scrollable size (CAMERA_MAX_X/Y). */
export function clampCamera(x: number, y: number, roomW: number, roomH: number): Point {
  return {
    x: Math.max(0, Math.min(Math.round(x), roomW - SCREEN_W)),
    y: Math.max(0, Math.min(Math.round(y), roomH - SCREEN_H)),
  };
}

/** The camera for a view: the screen centred on the canvas's centre, clamped. */
export function cameraFor(v: View, cw: number, ch: number, roomW: number, roomH: number): Point {
  const c = toWorld(v, cw / 2, ch / 2);
  return clampCamera(c.x - SCREEN_W / 2, c.y - SCREEN_H / 2, roomW, roomH);
}

/**
 * Where a layer's pixel (0, 0) is in the world: the layer scrolled for
 * `camera` at its scale lines up with the world at the camera's screen.
 * Without parallax (or at scale 256) it is (0, 0).
 */
export function layerOrigin(camera: Point, scale: readonly [number, number], parallax: boolean): Point {
  if (!parallax) return { x: 0, y: 0 };
  return { x: camera.x - scaleQ8(camera.x, scale[0]), y: camera.y - scaleQ8(camera.y, scale[1]) };
}

/** The layer cell under a world point, or null outside the layer. */
export function cellAt(p: Point, origin: Point, cols: number, rows: number): Point | null {
  const cx = Math.floor((p.x - origin.x) / CELL);
  const cy = Math.floor((p.y - origin.y) / CELL);
  if (cx < 0 || cy < 0 || cx >= cols || cy >= rows) return null;
  return { x: cx, y: cy };
}

/** The cells of a line from a to b (Bresenham): a drag's samples joined up. */
export function lineCells(a: Point, b: Point): Point[] {
  const out: Point[] = [];
  let x = a.x;
  let y = a.y;
  const dx = Math.abs(b.x - x);
  const dy = -Math.abs(b.y - y);
  const sx = x < b.x ? 1 : -1;
  const sy = y < b.y ? 1 : -1;
  let err = dx + dy;
  for (;;) {
    out.push({ x, y });
    if (x === b.x && y === b.y) break;
    const e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y += sy;
    }
  }
  return out;
}

/** The cells of the rectangle with corners a and b, row by row. */
export function rectCells(a: Point, b: Point): Point[] {
  const out: Point[] = [];
  for (let y = Math.min(a.y, b.y); y <= Math.max(a.y, b.y); y++) {
    for (let x = Math.min(a.x, b.x); x <= Math.max(a.x, b.x); x++) out.push({ x, y });
  }
  return out;
}

/** An entity's group: its 256-pixel column (SpawnRoomEntities, `x >> 8`). */
export function columnOf(x: number): number {
  return x >> 8;
}

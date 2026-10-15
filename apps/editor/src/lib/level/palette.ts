// A room's palette: 256 GBA colours, BGR555 with bit 15 kept, written into the
// layer pictures as crashdata.common.jasc_pal writes them (docs/graphics.md,
// "Palettes"): a channel's 8-bit value is c * 255 / 31, and bit 15 flips red's
// lowest bit. The same rule both ways, so a colour read from a picture and
// written back is the same entry.

/** A 5-bit channel's 8-bit value (gbagfx's). */
export function standard(c: number): number {
  return Math.floor((c * 255) / 31);
}

/** A palette entry's [r, g, b], 8 bits each (jasc_pal.to_rgb). */
export function toRgb(v: number): [number, number, number] {
  const r = v & 31;
  const g = (v >> 5) & 31;
  const b = (v >> 10) & 31;
  return [standard(r) ^ ((v >> 15) & 1), standard(g), standard(b)];
}

/** [r, g, b] (8 bits each) as a palette entry (jasc_pal.from_rgb). */
export function fromRgb(r: number, g: number, b: number): number {
  const r5 = r >> 3;
  const g5 = g >> 3;
  const b5 = b >> 3;
  const bit15 = (r ^ standard(r5)) & 1;
  return (bit15 << 15) | (b5 << 10) | (g5 << 5) | r5;
}

/** An entry's 5-bit channels. */
export function channels(v: number): { r: number; g: number; b: number; bit15: number } {
  return { r: v & 31, g: (v >> 5) & 31, b: (v >> 10) & 31, bit15: (v >> 15) & 1 };
}

export function fromChannels(r: number, g: number, b: number, bit15 = 0): number {
  return ((bit15 & 1) << 15) | ((b & 31) << 10) | ((g & 31) << 5) | (r & 31);
}

/** "#rrggbb" of an entry, as the pictures show it. */
export function hex(v: number): string {
  return "#" + toRgb(v).map((c) => c.toString(16).padStart(2, "0")).join("");
}

/** An "#rrggbb" colour as an entry (bit 15 from red's lowest bit, as jasc_pal reads a picture). */
export function fromHex(h: string): number {
  const m = /^#?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})$/i.exec(h);
  if (!m) throw new Error(`not a colour: ${h}`);
  return fromRgb(parseInt(m[1]!, 16), parseInt(m[2]!, 16), parseInt(m[3]!, 16));
}

/**
 * The palette as 32-bit pixels for an ImageData (RGBA in memory: little-endian
 * ABGR words), with the entries `transparent(i)` says are see-through.
 */
export function pixelTable(palette: ArrayLike<number>, transparent: (i: number) => boolean = () => false): Uint32Array {
  const out = new Uint32Array(256);
  for (let i = 0; i < 256; i++) {
    if (transparent(i)) continue;
    const [r, g, b] = toRgb(palette[i] ?? 0);
    out[i] = (0xff << 24) | (b << 16) | (g << 8) | r;
  }
  return out;
}

/** A "#RRGGBB" colour as a 32-bit pixel (ABGR words), with an alpha. */
export function pixelOf(colour: string, alpha = 255): number {
  const m = /^#?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})$/i.exec(colour);
  if (!m) return 0;
  return ((alpha & 255) << 24) | (parseInt(m[3]!, 16) << 16) | (parseInt(m[2]!, 16) << 8) | parseInt(m[1]!, 16);
}

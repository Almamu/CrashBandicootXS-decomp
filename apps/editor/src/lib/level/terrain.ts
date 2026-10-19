// A collision cell's value (docs/flags/terrain.md, engine/include/game/level/level.h):
// its terrain type in bits 0-7, a flag nibble in bits 8-11, the open bits in
// 12-15. The backend's crashdata.levels.collision names and checks a new
// value the same way (describe and new_entry_errors, valueName and
// newValueErrors here); this is what the Collision tab shows while one is
// made.

import type { LegendEntry } from "@crash/schema-types";

/** gTerrainTypes' entries (engine/src/game/level/data/terrain_types.cpp): a type past them reads its rows from the bytes after the table. */
export const TERRAIN_TYPES = 51;
/** the last type GetTerrainHeights reads (1-0x23 have a height per column); 0x24 and above are solid */
export const TERRAIN_LAST_HEIGHTS = 0x23;
/** the open bits: set, a solid cell lets one collision mode's probes through */
export const OPEN_BITS = [
  { bit: 1, name: "TERRAIN_FLAG_OPEN_RIGHT", label: "right" },
  { bit: 2, name: "TERRAIN_FLAG_OPEN_LEFT", label: "left" },
  { bit: 4, name: "TERRAIN_FLAG_OPEN_DOWN", label: "down" },
  { bit: 8, name: "TERRAIN_FLAG_OPEN_UP", label: "up" },
] as const;
/** What XS's player does on a cell's flag nibble (Player::CheckPlayerContact, engine/src/game/crash/player_collide.cpp); the others do nothing. */
export const XS_FLAGS: Record<number, string> = {
  1: "kills, mask or not",
  5: "slippery",
  6: "a bar to hang from",
  7: "pushes left",
  10: "pushes right",
};

export interface CellParts {
  type: number;
  flags: number;
  open: number;
}

export function composeValue(p: CellParts): number {
  return (p.type & 0xff) | ((p.flags & 0xf) << 8) | ((p.open & 0xf) << 12);
}

export function valueParts(v: number): CellParts {
  return { type: v & 0xff, flags: (v >> 8) & 0xf, open: (v >> 12) & 0xf };
}

const hex = (v: number, n: number) => v.toString(16).toUpperCase().padStart(n, "0");

/** A value's name as the legend spells them: its type, its flag nibble, its open bits. */
export function valueName(v: number): string {
  const { type, flags, open } = valueParts(v);
  const parts = [type === 0 ? "TERRAIN_TYPE_NONE" : `${type <= TERRAIN_LAST_HEIGHTS ? "heights" : "solid"} 0x${hex(type, 2)}`];
  if (flags) parts.push(`flags 0x${hex(flags, 1)}`);
  if (open)
    parts.push(
      OPEN_BITS.filter((b) => open & b.bit)
        .map((b) => b.name)
        .join("|"),
    );
  return parts.join(", ");
}

/** What a terrain type is to the lookups. */
export function typeKind(type: number): string {
  if (type === 0) return "empty";
  if (type <= TERRAIN_LAST_HEIGHTS) return "a height per column (GetTerrainHeights)";
  if (type < TERRAIN_TYPES) return "solid (GetSolidTerrainHeights)";
  return "solid, past gTerrainTypes";
}

/** Why the legend can't take a value in a colour ("#RRGGBB"): none when it can. */
export function newValueErrors(v: number, colour: string, legend: LegendEntry[]): string[] {
  const out: string[] = [];
  if (!Number.isInteger(v) || v < 0 || v > 0xffff) return ["a cell value is 0-0xFFFF"];
  if (legend.length >= 255) out.push("the legend has 255 values, the most a collision image's palette holds");
  const have = legend.find((e) => e.value === v);
  if (have) out.push(`0x${hex(v, 4)} is in the legend already (${have.name})`);
  const c = colour.toUpperCase();
  const taken = legend.find((e) => e.colour.toUpperCase() === c);
  if (taken) out.push(`${c} is 0x${hex(taken.value, 4)}'s colour: each value has its own`);
  const type = v & 0xff;
  if (type >= TERRAIN_TYPES && !legend.some((e) => (e.value & 0xff) === type))
    out.push(`terrain type 0x${hex(type, 2)}: gTerrainTypes has 0-0x${hex(TERRAIN_TYPES - 1, 2)}, and no value of the legend has it`);
  return out;
}

/** A colour no value of the legend has, picked from the value's bits. */
export function freeColour(v: number, legend: LegendEntry[]): string {
  const taken = new Set(legend.map((e) => e.colour.toUpperCase()));
  for (let k = 0; ; k++) {
    // a hue from the bits, turned by the golden angle until it's free
    const h = ((v * 47 + k * 137.508) % 360) / 60;
    const x = 1 - Math.abs((h % 2) - 1);
    const [r, g, b] = h < 1 ? [1, x, 0] : h < 2 ? [x, 1, 0] : h < 3 ? [0, 1, x] : h < 4 ? [0, x, 1] : h < 5 ? [x, 0, 1] : [1, 0, x];
    const light = 0.35 + ((v >> 8) % 4) * 0.15;
    const c = "#" + [r, g, b].map((ch) => hex(Math.round((ch * 0.8 + 0.2) * light * 255), 2)).join("");
    if (!taken.has(c)) return c;
  }
}

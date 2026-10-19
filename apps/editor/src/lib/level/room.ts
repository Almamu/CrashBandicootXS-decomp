// A room's record in the level table (room.json's "record", docs/levels.md
// "The level table"): its kind and its colour effect (SetupRoomBlend).
import type { EntityTypeInfo, Room } from "@crash/schema-types";

export type RoomRecord = NonNullable<Room["record"]>;
export type RoomKind = RoomRecord["kind"];

/** ROOM_KIND_*: what PlayRoom gives the player (level_room.kind). */
export const KINDS: { id: RoomKind; about: string; marker: string }[] = [
  { id: "ON_FOOT", about: "On foot: the player's action controller (running, jumping, spinning).", marker: "START_MARKER" },
  { id: "UNDERWATER", about: "Underwater: the swim controller (gSwimCtrl); with a blend, the sprites are raised over BG0-2.", marker: "UNDERWATER_START_MARKER" },
  { id: "HOVER", about: "Hover vehicle: the input controller of the hover levels.", marker: "HOVER_START_MARKER" },
];

/** BLDCNT's colour effects (bits 6-7): what SetupRoomBlend writes from "blend"."effect". */
export const BLEND_EFFECTS: { name: string; about: string }[] = [
  { name: "none", about: "No colour effect: SetupRoomBlend writes BLDALPHA 16/16 with the effect off (EVA and EVB aren't used)." },
  { name: "alpha blend", about: "Alpha blending: BG3 (first target) over BG0-2 and the sprites (second targets), weighted by EVA and EVB." },
  { name: "brighten", about: "Brighten the first target (BG3) by BLDY, which the room doesn't set (the fades do): drawn unchanged here." },
  { name: "darken", about: "Darken the first target (BG3) by BLDY, which the room doesn't set (the fades do): drawn unchanged here." },
];

/**
 * The start marker a room's kind places (docs/levels.md, "Entity types": every
 * room places one, of its kind's type) and the one it has, or null when the
 * game's entity types don't name them (N-Tranced's).
 */
export function startMarkerFor(room: Room | null | undefined, types: EntityTypeInfo[]): { want: string; have: string | null } | null {
  if (!room) return null;
  const names = new Set(types.map((t) => t.name));
  if (!KINDS.every((k) => names.has(k.marker))) return null;
  const want = KINDS.find((k) => k.id === room.record?.kind)?.marker;
  if (!want) return null;
  const markers = new Set(KINDS.map((k) => k.marker));
  const have = room.entities.find((e) => typeof e.type === "string" && markers.has(e.type))?.type as string | undefined;
  return { want, have: have ?? null };
}

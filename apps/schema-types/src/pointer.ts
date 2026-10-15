// JSON pointers (RFC 6901), as the diagnostics give them (/entities/3/param).

export type PathPart = string | number;

export function parsePointer(pointer: string | null | undefined): string[] {
  if (!pointer) return [];
  if (!pointer.startsWith("/")) throw new Error(`not a JSON pointer: ${pointer}`);
  return pointer
    .slice(1)
    .split("/")
    .map((p) => p.replace(/~1/g, "/").replace(/~0/g, "~"));
}

export function formatPointer(path: readonly PathPart[]): string {
  return path.map((p) => "/" + String(p).replace(/~/g, "~0").replace(/\//g, "~1")).join("");
}

/** The value at the pointer's place, or undefined. */
export function getAt(value: unknown, pointer: string | readonly PathPart[]): unknown {
  const parts = typeof pointer === "string" ? parsePointer(pointer) : pointer;
  let v: unknown = value;
  for (const p of parts) {
    if (Array.isArray(v)) v = v[Number(p)];
    else if (v !== null && typeof v === "object") v = (v as Record<string, unknown>)[String(p)];
    else return undefined;
  }
  return v;
}

// What the generic editor needs from a JSON Schema (draft 2020-12, as
// crashdata writes them: $ref to #/$defs, anyOf/oneOf, enum, const,
// properties/patternProperties/additionalProperties, items/prefixItems).

export type Schema = {
  $ref?: string;
  $defs?: Record<string, Schema>;
  $comment?: string;
  title?: string;
  description?: string;
  type?: string | string[];
  enum?: unknown[];
  const?: unknown;
  anyOf?: Schema[];
  oneOf?: Schema[];
  properties?: Record<string, Schema>;
  patternProperties?: Record<string, Schema>;
  additionalProperties?: boolean | Schema;
  required?: string[];
  items?: Schema | boolean;
  prefixItems?: Schema[];
  minItems?: number;
  maxItems?: number;
  minimum?: number;
  maximum?: number;
  pattern?: string;
  minLength?: number;
  default?: unknown;
  [k: string]: unknown;
};

export type Json = null | boolean | number | string | Json[] | { [k: string]: Json };

/** The schema a $ref names, followed until it isn't one (merging the referrer's description). */
export function resolve(root: Schema, s: Schema | boolean | undefined): Schema {
  if (s === undefined || s === true) return {};
  if (s === false) return { not: {} };
  let cur = s;
  const seen = new Set<string>();
  while (cur.$ref) {
    const ref = cur.$ref;
    if (seen.has(ref) || !ref.startsWith("#/")) break;
    seen.add(ref);
    const target = ref
      .slice(2)
      .split("/")
      .map((p) => p.replace(/~1/g, "/").replace(/~0/g, "~"))
      .reduce<unknown>((v, k) => (v && typeof v === "object" ? (v as Record<string, unknown>)[k] : undefined), root);
    if (!target || typeof target !== "object") break;
    const { $ref: _ref, ...rest } = cur;
    cur = { ...(target as Schema), ...rest };
  }
  return cur;
}

/** A schema's alternatives (anyOf or oneOf), or none. */
export function alternatives(s: Schema): Schema[] | undefined {
  return s.oneOf ?? s.anyOf;
}

export function types(s: Schema): string[] {
  if (s.type) return Array.isArray(s.type) ? s.type : [s.type];
  if (s.const !== undefined) return [jsonType(s.const as Json)];
  if (s.enum) return [...new Set(s.enum.map((v) => jsonType(v as Json)))];
  if (s.properties || s.patternProperties || s.additionalProperties !== undefined) return ["object"];
  if (s.items !== undefined || s.prefixItems) return ["array"];
  return [];
}

export function jsonType(v: Json | undefined): string {
  if (v === null) return "null";
  if (Array.isArray(v)) return "array";
  if (typeof v === "number") return Number.isInteger(v) ? "integer" : "number";
  return typeof v;
}

/** Whether v is of one of the types (an integer is a number too). */
export function typeFits(v: Json | undefined, ts: string[]): boolean {
  if (!ts.length) return true;
  const t = jsonType(v);
  return ts.includes(t) || (t === "integer" && ts.includes("number"));
}

/** A short name of an alternative, for the picker. */
export function label(root: Schema, s: Schema, i: number): string {
  if (s.title) return s.title;
  if (s.$ref) return s.$ref.split("/").pop()!.replace(/^param_/, "");
  const r = resolve(root, s);
  if (r.const !== undefined) return JSON.stringify(r.const);
  if (r.enum) return r.enum.length <= 3 ? r.enum.map((v) => JSON.stringify(v)).join(" | ") : `one of ${r.enum.length}`;
  const ts = types(r);
  if (ts.length === 1 && ts[0] === "array") {
    const it = typeof r.items === "object" ? resolve(root, r.items) : undefined;
    return it ? `list of ${types(it).join("/") || "values"}` : "list";
  }
  if (ts.length === 1 && ts[0] === "object" && r.properties) {
    const keys = Object.keys(r.properties);
    return `{${keys.slice(0, 3).join(", ")}${keys.length > 3 ? ", …" : ""}}`;
  }
  return ts.join(" | ") || `option ${i + 1}`;
}

/** A value for a new item: the schema's default, const, first enum value, or its type's empty value. */
export function defaultValue(root: Schema, s0: Schema | boolean | undefined, depth = 0): Json {
  const s = resolve(root, s0);
  if (s.default !== undefined) return structuredClone(s.default) as Json;
  if (s.const !== undefined) return structuredClone(s.const) as Json;
  if (s.enum?.length) return structuredClone(s.enum[0]) as Json;
  const alts = alternatives(s);
  if (alts?.length) return defaultValue(root, alts[0], depth + 1);
  const t = types(s)[0];
  switch (t) {
    case "object": {
      const out: Record<string, Json> = {};
      if (depth < 8) for (const k of s.required ?? []) out[k] = defaultValue(root, s.properties?.[k], depth + 1);
      return out;
    }
    case "array": {
      const out: Json[] = [];
      const n = s.minItems ?? 0;
      for (let i = 0; i < n && depth < 8; i++) {
        out.push(defaultValue(root, s.prefixItems?.[i] ?? (s.items as Schema | undefined), depth + 1));
      }
      return out;
    }
    case "integer":
    case "number":
      return typeof s.minimum === "number" && s.minimum > 0 ? s.minimum : 0;
    case "boolean":
      return false;
    case "string":
      return "";
    case "null":
      return null;
    default:
      return null;
  }
}

/** The schema of an object's key: its property, a matching patternProperties, or additionalProperties. */
export function propertySchema(root: Schema, s: Schema, key: string): Schema | undefined {
  if (s.properties && key in s.properties) return resolve(root, s.properties[key]);
  for (const [p, ps] of Object.entries(s.patternProperties ?? {})) {
    if (new RegExp(p, "u").test(key)) return resolve(root, ps);
  }
  if (s.additionalProperties === false) return undefined;
  if (s.additionalProperties && s.additionalProperties !== true) return resolve(root, s.additionalProperties);
  return {};
}

/** The schema of an array's i'th item. */
export function itemSchema(root: Schema, s: Schema, i: number): Schema {
  if (s.prefixItems && i < s.prefixItems.length) return resolve(root, s.prefixItems[i]);
  return resolve(root, s.items as Schema | boolean | undefined);
}

/** An object with `key` added where the schema's property order puts it. */
export function withKey(root: Schema, s: Schema, obj: Record<string, Json>, key: string, value: Json): Record<string, Json> {
  const order = Object.keys(s.properties ?? {});
  const at = order.indexOf(key);
  const out: Record<string, Json> = {};
  let placed = false;
  for (const [k, v] of Object.entries(obj)) {
    const kAt = order.indexOf(k);
    if (!placed && at >= 0 && kAt > at) {
      out[key] = value;
      placed = true;
    }
    out[k] = v;
  }
  if (!placed) out[key] = value;
  void root;
  return out;
}

/** An object without `key`. */
export function withoutKey(obj: Record<string, Json>, key: string): Record<string, Json> {
  const out: Record<string, Json> = {};
  for (const [k, v] of Object.entries(obj)) if (k !== key) out[k] = v;
  return out;
}

/** Whether the schema says nothing (a key the model reads that the schema doesn't describe yet). */
export function isOpen(s: Schema): boolean {
  return !types(s).length && !alternatives(s) && s.enum === undefined && s.const === undefined;
}

/** A path's JSON pointer. */
export function pointerOf(path: readonly (string | number)[]): string {
  return path.map((p) => "/" + String(p).replace(/~/g, "~0").replace(/\//g, "~1")).join("");
}

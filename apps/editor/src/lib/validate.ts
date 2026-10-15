// The editor's own check of a value against its schema, as the user types
// (Ajv, draft 2020-12). The backend checks again on save, with crashdata's
// validator and the model's checks: this one only marks the fields.
import Ajv2020, { type ErrorObject, type ValidateFunction } from "ajv/dist/2020.js";
import type { Schema } from "./schema.ts";

const ajv = new Ajv2020({ allErrors: true, strict: false, validateFormats: false });
const compiled = new Map<string, ValidateFunction>();

export interface FieldError {
  pointer: string;
  message: string;
}

function messageOf(e: ErrorObject): string {
  switch (e.keyword) {
    case "additionalProperties":
      return `unknown key "${(e.params as { additionalProperty: string }).additionalProperty}"`;
    case "required":
      return `"${(e.params as { missingProperty: string }).missingProperty}" is missing`;
    case "enum":
      return `not one of ${((e.params as { allowedValues: unknown[] }).allowedValues ?? []).slice(0, 8).map((v) => JSON.stringify(v)).join(", ")}`;
    default:
      return e.message ?? e.keyword;
  }
}

/** The value's problems, each at its JSON pointer (an unknown key's at the key). */
export function validate(name: string, schema: Schema, value: unknown): FieldError[] {
  let fn = compiled.get(name);
  if (!fn) {
    const { $id: _id, ...rest } = schema;
    fn = ajv.compile(rest);
    compiled.set(name, fn);
  }
  if (fn(value)) return [];
  const out: FieldError[] = [];
  const seen = new Set<string>();
  for (const e of fn.errors ?? []) {
    let pointer = e.instancePath;
    if (e.keyword === "additionalProperties") {
      pointer += "/" + (e.params as { additionalProperty: string }).additionalProperty.replace(/~/g, "~0").replace(/\//g, "~1");
    }
    // an anyOf's branches each give theirs: keep the anyOf's own and the first of a place's
    if (e.keyword === "anyOf" || e.keyword === "oneOf") {
      pointer = e.instancePath;
    }
    const message = e.keyword === "anyOf" || e.keyword === "oneOf" ? "matches none of the forms" : messageOf(e);
    const key = pointer + "\0" + message;
    if (seen.has(key)) continue;
    seen.add(key);
    out.push({ pointer, message });
  }
  return out;
}

/** The errors by pointer. */
export function byPointer(errors: readonly FieldError[]): Map<string, string[]> {
  const m = new Map<string, string[]>();
  for (const e of errors) {
    const l = m.get(e.pointer);
    if (l) l.push(e.message);
    else m.set(e.pointer, [e.message]);
  }
  return m;
}

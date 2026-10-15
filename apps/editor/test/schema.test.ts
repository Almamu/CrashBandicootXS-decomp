import { readFileSync } from "node:fs";
import { join } from "node:path";
import { describe, expect, it } from "vitest";
import { alternatives, defaultValue, itemSchema, label, propertySchema, resolve, typeFits, withKey, type Schema } from "../src/lib/schema.ts";
import { byPointer, validate } from "../src/lib/validate.ts";

const SCHEMAS = join(import.meta.dirname, "../../../packages/crashdata/schemas");
const load = (n: string) => JSON.parse(readFileSync(join(SCHEMAS, n), "utf8")) as Schema;
const read = (p: string) => JSON.parse(readFileSync(join(import.meta.dirname, "../../..", p), "utf8")) as unknown;

describe("the schema helpers", () => {
  const room = load("room.schema.json");

  it("follow $refs into $defs", () => {
    const entities = resolve(room, room.properties!["entities"]);
    const entity = itemSchema(room, entities, 0);
    expect(entity.type).toBe("object");
    expect(Object.keys(entity.properties ?? {})).toEqual(expect.arrayContaining(["type", "x", "y", "param"]));
  });

  it("give each anyOf branch a name", () => {
    const params = resolve(room, room.properties!["params"]);
    const record = itemSchema(room, params, 0);
    const alts = alternatives(record)!;
    expect(alts.length).toBeGreaterThan(3);
    expect(alts.map((a, i) => label(room, a, i))).toContain("flags");
  });

  it("make a new value the schema accepts", () => {
    const entities = resolve(room, room.properties!["entities"]);
    const v = defaultValue(room, itemSchema(room, entities, 0));
    expect(Object.keys(v as object)).toEqual(expect.arrayContaining((itemSchema(room, entities, 0).required ?? []) as string[]));
  });

  it("insert a key where the schema has it", () => {
    const s: Schema = { properties: { a: {}, b: {}, c: {} } };
    expect(Object.keys(withKey(s, s, { a: 1, c: 3 }, "b", 2))).toEqual(["a", "b", "c"]);
    expect(Object.keys(withKey(s, s, { c: 3 }, "a", 1))).toEqual(["a", "c"]);
    expect(Object.keys(withKey(s, s, { a: 1 }, "z", 1))).toEqual(["a", "z"]);
  });

  it("know an object's other keys", () => {
    const s: Schema = { properties: { a: { type: "integer" } }, patternProperties: { "^x_": { type: "string" } }, additionalProperties: false };
    expect(propertySchema(s, s, "a")?.type).toBe("integer");
    expect(propertySchema(s, s, "x_1")?.type).toBe("string");
    expect(propertySchema(s, s, "b")).toBeUndefined();
    expect(typeFits(3, ["number"])).toBe(true);
    expect(typeFits(3.5, ["integer"])).toBe(false);
  });
});

describe("the editor's validation", () => {
  it("accepts the tree's files as they are", () => {
    for (const [schema, file] of [
      ["room.schema.json", "games/xs/data/levels/jungle_jam/main/room.json"],
      ["levels.schema.json", "games/xs/data/levels/levels.json"],
      ["cutscenes.schema.json", "games/xs/data/cutscenes.json"],
      ["graphics_manifest.schema.json", "games/n2/graphics/manifest.json"],
    ] as const) {
      expect(validate(schema, load(schema), read(file)), file).toEqual([]);
    }
  });

  it("puts an unknown key's error at the key", () => {
    const music = read("games/xs/sound/music.json") as { screen_music: Record<string, unknown>[] };
    music.screen_music[0]!["bogus"] = 1;
    const errs = validate("music.schema.json", load("music.schema.json"), music);
    expect(byPointer(errs).get("/screen_music/0/bogus")).toEqual(['unknown key "bogus"']);
  });
});

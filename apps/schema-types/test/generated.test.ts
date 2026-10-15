import { readFileSync, readdirSync } from "node:fs";
import { join } from "node:path";
import { describe, expect, it } from "vitest";
// @ts-expect-error: a plain .mjs script, no types
import { OUT_DIR, SCHEMAS_DIR, render, typeName } from "../scripts/generate.mjs";
import { FILE_MATCH, SCHEMA_NAMES } from "../src/index.ts";

describe("the generated types", () => {
  it("are what packages/crashdata/schemas/ gives (pnpm gen)", async () => {
    const want: Record<string, string> = await render();
    const have = readdirSync(OUT_DIR).filter((f) => f.endsWith(".ts"));
    expect(have.sort()).toEqual(Object.keys(want).sort());
    for (const [name, text] of Object.entries(want)) {
      expect(readFileSync(join(OUT_DIR, name), "utf8"), name).toBe(text);
    }
  }, 60_000);

  it("have a type per schema, named after its file", () => {
    const files = readdirSync(SCHEMAS_DIR).filter((f) => f.endsWith(".schema.json")).sort();
    expect([...SCHEMA_NAMES]).toEqual(files);
    expect(typeName("room.schema.json")).toBe("Room");
    expect(typeName("gax_sfx_manifest.schema.json")).toBe("GaxSfxManifest");
  });

  it("carry index.json's globs", () => {
    const index = JSON.parse(readFileSync(join(SCHEMAS_DIR, "index.json"), "utf8")) as {
      schemas: { schema: keyof typeof FILE_MATCH; fileMatch: string[] }[];
    };
    for (const s of index.schemas) expect(FILE_MATCH[s.schema]).toEqual(s.fileMatch);
  });
});

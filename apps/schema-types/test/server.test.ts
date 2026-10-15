// Against a running backend (CI's smoke test, docs/editors.md): set
// CRASH_EDITOR_URL (http://127.0.0.1:8765) to run it; skipped otherwise.
import { describe, expect, it } from "vitest";
import { EditorClient, SCHEMA_NAMES, schemaFor } from "../src/index.ts";

const url = process.env.CRASH_EDITOR_URL;

describe.skipIf(!url)("the backend", () => {
  const c = new EditorClient({ baseUrl: url, token: process.env.CRASH_EDITOR_TOKEN });

  it("answers", async () => {
    const h = await c.health();
    expect(h.ok).toBe(true);
    expect(h.version).toBe(1);
  });

  it("serves the schemas the types come from", async () => {
    const index = await c.schemaIndex();
    expect(index.schemas.map((s) => s.schema).sort()).toEqual([...SCHEMA_NAMES]);
    const room = await c.schema("room.schema.json");
    expect(room["$schema"]).toBe("https://json-schema.org/draft/2020-12/schema");
  });

  it("lists XS's files with the schemas the globs give", async () => {
    const projects = await c.projects();
    expect(projects.map((p) => p.id)).toEqual(expect.arrayContaining(["xs", "n2"]));
    const files = await c.files("xs");
    expect(files.length).toBeGreaterThan(100);
    for (const f of files) expect(schemaFor(f.path), f.path).toBe(f.schema);
  });

  it("saves a file unchanged without writing it", async () => {
    for (const path of ["games/xs/data/cutscenes.json", "games/xs/data/levels/jungle_jam/main/room.json"]) {
      const file = await c.read("xs", path);
      const r = await c.save("xs", path, file.data);
      expect(r.saved).toEqual([]);
    }
  });

  it("refuses a file the schema rejects", async () => {
    const path = "games/xs/sound/music.json";
    const file = await c.read<"music.schema.json">("xs", path);
    const bad = structuredClone(file.data) as unknown as { screen_music: Record<string, unknown>[] };
    bad.screen_music[0]!["bogus"] = 1;
    const e = await c.save("xs", path, bad as never).catch((x: unknown) => x);
    expect((e as { status: number }).status).toBe(422);
    expect((e as { diagnostics: { pointer: string }[] }).diagnostics[0]!.pointer).toBe("/screen_music/0/bogus");
  });

  it("opens a room for the level editor, and saves it unchanged without writing it", async () => {
    const path = "games/xs/data/levels/jungle_jam/main";
    const v = await c.room("xs", path);
    expect(v.palette).toHaveLength(256);
    const bg1 = await c.roomLayer("xs", path, "bg1");
    expect([bg1.width, bg1.height]).toEqual([v.layers.bg1!.width, v.layers.bg1!.height]);
    const col = await c.roomLayer("xs", path, "collision");
    expect([col.width, col.height]).toEqual(v.layers.collision!.cells);
    const r = await c.saveRoom("xs", { path, room: v.room, palette: v.palette });
    expect(r.saved).toEqual([]);
  });

  it("gives data-check's report", async () => {
    const r = await c.check("xs", { text: false });
    expect(r.version).toBe(1);
    expect(r.tool).toBe("data-check");
    expect(Array.isArray(r.diagnostics)).toBe(true);
  }, 120_000);
});

import { describe, expect, it } from "vitest";
import { globToRegExp, schemaFor } from "../src/glob.ts";
import { formatPointer, getAt, parsePointer } from "../src/pointer.ts";

describe("schemaFor", () => {
  it("finds each kind of file's schema, as crashdata.schemas.schema_for does", () => {
    expect(schemaFor("games/xs/data/levels/jungle_jam/main/room.json")).toBe("room.schema.json");
    expect(schemaFor("games/n2/data/levels/levels.json")).toBe("levels.schema.json");
    expect(schemaFor("games/xs/data/cutscenes.json")).toBe("cutscenes.schema.json");
    expect(schemaFor("games/xs/graphics/manifest.json")).toBe("graphics_manifest.schema.json");
    expect(schemaFor("games/xs/sound/music.json")).toBe("music.schema.json");
    expect(schemaFor("games/xs/data/levels/spawns/blimp_bonanza.json")).toBe("spawn_list.schema.json");
  });

  it("finds none for other files", () => {
    expect(schemaFor("games/xs/data/data.s")).toBeUndefined();
    expect(schemaFor("games/xs/text/crashxs.pot")).toBeUndefined();
    expect(schemaFor("README.md")).toBeUndefined();
  });

  it("matches fnmatch's way: * crosses /", () => {
    expect(globToRegExp("games/*/x.json").test("games/a/b/x.json")).toBe(true);
    expect(globToRegExp("a?c").test("abc")).toBe(true);
    expect(globToRegExp("a[!b]c").test("abc")).toBe(false);
    expect(globToRegExp("a[!b]c").test("axc")).toBe(true);
    expect(globToRegExp("a.json").test("aXjson")).toBe(false);
  });
});

describe("JSON pointers", () => {
  it("round-trip, with ~ and / escaped", () => {
    expect(parsePointer("/entities/3/param")).toEqual(["entities", "3", "param"]);
    expect(parsePointer("/a~1b/c~0d")).toEqual(["a/b", "c~d"]);
    expect(formatPointer(["a/b", "c~d", 3])).toBe("/a~1b/c~0d/3");
    expect(parsePointer("")).toEqual([]);
    expect(parsePointer(null)).toEqual([]);
  });

  it("find the value", () => {
    const v = { entities: [{ x: 1 }, { x: 2, param: [5, 6] }] };
    expect(getAt(v, "/entities/1/param/0")).toBe(5);
    expect(getAt(v, "/entities/7/x")).toBeUndefined();
    expect(getAt(v, "")).toBe(v);
  });
});

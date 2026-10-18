import { describe, expect, it } from "vitest";
import { ApiError, EditorClient, parseCutscenePicture, parseRoomLayer } from "../src/client.ts";

interface Call {
  url: string;
  init: RequestInit;
}

function fake(status: number, body: unknown) {
  const calls: Call[] = [];
  const fetch = (async (url: string, init: RequestInit) => {
    calls.push({ url, init });
    return new Response(JSON.stringify(body), { status, headers: { "Content-Type": "application/json" } });
  }) as unknown as typeof globalThis.fetch;
  return { calls, fetch };
}

describe("EditorClient", () => {
  it("reads with plain GETs on the page's origin", async () => {
    const f = fake(200, { path: "games/xs/sound/music.json", schema: "music.schema.json", data: { screen_music: [] } });
    const c = new EditorClient({ fetch: f.fetch });
    const file = await c.read<"music.schema.json">("xs", "games/xs/sound/music.json");
    expect(file.data.screen_music).toEqual([]);
    expect(f.calls[0]!.url).toBe("/api/projects/xs/file?path=games%2Fxs%2Fsound%2Fmusic.json");
    expect(f.calls[0]!.init.method).toBe("GET");
    const h = f.calls[0]!.init.headers as Record<string, string>;
    expect(h["X-Crash-Editor"]).toBeUndefined();
    expect(h["Authorization"]).toBeUndefined();
  });

  it("marks a change, and gives the desktop app's token", async () => {
    const f = fake(200, { ok: true, saved: [], diagnostics: [] });
    const c = new EditorClient({ baseUrl: "http://127.0.0.1:4000/", token: "t0k", fetch: f.fetch });
    await c.save<"music.schema.json">("mod:sample", "games/xs/sound/music.json", { screen_music: [] });
    const call = f.calls[0]!;
    expect(call.url).toBe("http://127.0.0.1:4000/api/projects/mod%3Asample/file?path=games%2Fxs%2Fsound%2Fmusic.json");
    expect(call.init.method).toBe("PUT");
    expect(JSON.parse(call.init.body as string)).toEqual({ data: { screen_music: [] } });
    const h = call.init.headers as Record<string, string>;
    expect(h["X-Crash-Editor"]).toBe("1");
    expect(h["Authorization"]).toBe("Bearer t0k");
  });

  it("gives a refusal's diagnostics as an ApiError", async () => {
    const d = { file: "games/xs/sound/music.json", pointer: "/screen_music/0/bogus", line: 3, col: 9,
      severity: "error", code: "schema", message: "unknown key" };
    const f = fake(422, { ok: false, error: "the schema rejects it", diagnostics: [d] });
    const c = new EditorClient({ fetch: f.fetch });
    const e = await c.save("xs", "games/xs/sound/music.json", {} as never).catch((x: unknown) => x);
    expect(e).toBeInstanceOf(ApiError);
    expect((e as ApiError).status).toBe(422);
    expect((e as ApiError).message).toBe("the schema rejects it");
    expect((e as ApiError).diagnostics).toEqual([d]);
  });

  it("reads a room's layers as bytes", async () => {
    const calls: string[] = [];
    const body = new Uint8Array([2, 0, 0, 0, 1, 0, 0, 0, 0x34, 0x12, 0xff, 0x00]);
    const fetch = (async (url: string) => {
      calls.push(url);
      return new Response(body, { status: 200 });
    }) as unknown as typeof globalThis.fetch;
    const c = new EditorClient({ fetch });
    const l = await c.roomLayer("xs", "games/xs/data/levels/jungle_jam/main", "collision");
    expect(calls[0]).toBe("/api/projects/xs/room/layer?path=games%2Fxs%2Fdata%2Flevels%2Fjungle_jam%2Fmain&layer=collision");
    expect([l.width, l.height, ...l.data]).toEqual([2, 1, 0x1234, 0xff]);
    const px = parseRoomLayer(new Uint8Array([2, 0, 0, 0, 1, 0, 0, 0, 7, 9]).buffer, false);
    expect([...px.data]).toEqual([7, 9]);
    expect(() => parseRoomLayer(new Uint8Array([2, 0, 0, 0, 2, 0, 0, 0, 7]).buffer, false)).toThrow();
    expect(c.spriteUrl("n2", 23, 43)).toBe("/api/projects/n2/sprite?bank=23&frame=43");
    expect(c.spriteUrl("xs", 32, 0, 53)).toBe("/api/projects/xs/sprite?bank=32&frame=0&palette=53");
  });

  it("gives a refused binary request's error", async () => {
    const f = fake(404, { ok: false, error: "the room has no layer 'bg9'" });
    const c = new EditorClient({ fetch: f.fetch });
    const e = await c.roomLayer("xs", "x", "bg9").catch((x: unknown) => x);
    expect((e as ApiError).status).toBe(404);
    expect((e as ApiError).message).toBe("the room has no layer 'bg9'");
  });

  it("asks for the check without the text when told", async () => {
    const f = fake(200, { version: 1, tool: "data_check", ok: true, diagnostics: [], output: "" });
    const c = new EditorClient({ fetch: f.fetch });
    await c.check("n2", { text: false });
    expect(f.calls[0]!.url).toBe("/api/projects/n2/check?text=0");
    expect(c.rawUrl("xs", "games/xs/graphics/a b.png")).toBe("/api/projects/xs/raw?path=games%2Fxs%2Fgraphics%2Fa%20b.png");
  });

  it("reads a cutscene picture's colours and pixels", () => {
    const buf = new Uint8Array(8 + 512 + 2);
    new DataView(buf.buffer).setUint32(0, 2, true);
    new DataView(buf.buffer).setUint32(4, 1, true);
    new DataView(buf.buffer).setUint16(8 + 2, 0x7fff, true);
    buf.set([1, 0], 520);
    const p = parseCutscenePicture(buf.buffer);
    expect([p.width, p.height, p.palette[1], [...p.pixels]]).toEqual([2, 1, 0x7fff, [1, 0]]);
    expect(() => parseCutscenePicture(buf.buffer.slice(0, 521))).toThrow();
  });
});

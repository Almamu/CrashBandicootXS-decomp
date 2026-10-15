import { describe, expect, it } from "vitest";
import { ApiError, EditorClient } from "../src/client.ts";

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

  it("asks for the check without the text when told", async () => {
    const f = fake(200, { version: 1, tool: "data_check", ok: true, diagnostics: [], output: "" });
    const c = new EditorClient({ fetch: f.fetch });
    await c.check("n2", { text: false });
    expect(f.calls[0]!.url).toBe("/api/projects/n2/check?text=0");
    expect(c.rawUrl("xs", "games/xs/graphics/a b.png")).toBe("/api/projects/xs/raw?path=games%2Fxs%2Fgraphics%2Fa%20b.png");
  });
});

<script lang="ts">
  // The room's record in the level table (room.json's "record", docs/levels.md
  // "The level table"): its kind, which picks the player's controller, and its
  // colour effect, which SetupRoomBlend turns into BLDCNT/BLDALPHA (BG3 the
  // first target, BG0-2 and the sprites the second). The view draws the blend
  // as the PPU does ("blend" in the layer bar).
  import type { ResizeReport, RoomEdges } from "@crash/schema-types";
  import { BLEND_EFFECTS, KINDS, startMarkerFor } from "../../lib/level/room.ts";
  import { level } from "../../lib/level/state.svelte.ts";
  import { editor } from "../../lib/state.svelte.ts";

  const record = $derived(level.room?.record);
  const blend = $derived({ effect: record?.blend?.effect ?? 0, eva: record?.blend?.eva ?? 0, evb: record?.blend?.evb ?? 0 });
  const marker = $derived(startMarkerFor(level.room, level.view?.entityTypes ?? []));

  // the room's size: cells added (negative: taken) at each edge
  let edges = $state<RoomEdges>({ left: 0, right: 0, top: 0, bottom: 0 });
  let checked = $state<ResizeReport | null>(null);
  const size = $derived((level.view?.layers.collision?.sizeTiles ?? [0, 0]) as [number, number]);
  const played = $derived(level.roomEntry(level.view?.path)?.level !== undefined);
  const anyEdge = $derived(edges.left !== 0 || edges.right !== 0 || edges.top !== 0 || edges.bottom !== 0);

  function setEdge(k: keyof RoomEdges, v: string) {
    const n = Number(v);
    if (!Number.isInteger(n)) return;
    edges = { ...edges, [k]: n };
    checked = null;
  }

  async function check() {
    checked = await level.checkResize(edges);
  }

  async function resize() {
    if (level.dirty) return;
    const r = checked ?? (await level.checkResize(edges));
    if (!r) return;
    const cells = Object.values(r.layers).reduce((n, l) => n + l.cut, 0);
    if ((r.entities.length || cells) && !confirm(`Resizing cuts ${r.entities.length} entities and ${cells} cells that aren't blank. Resize?`)) return;
    if (await level.resize(edges)) {
      edges = { left: 0, right: 0, top: 0, bottom: 0 };
      checked = null;
    }
  }

  async function remove() {
    if (confirm(`Delete ${level.view?.path}: its folder, and its place in levels.json? (No level plays it.)`)) await level.deleteRoom();
  }

  function setBlend(k: "effect" | "eva" | "evb", s: string) {
    const n = Number(s);
    const max = k === "effect" ? 3 : 31;
    if (!Number.isInteger(n) || n < 0 || n > max) return;
    level.setRoomRecord({ blend: { ...blend, [k]: n } });
  }
</script>

{#if record}
  <div class="panel">
    <div class="muted small">{record.symbol}</div>
    <label class="row">Kind
      <select value={record.kind} onchange={(e) => level.setRoomRecord({ kind: e.currentTarget.value as typeof record.kind })}>
        {#each KINDS as k (k.id)}<option value={k.id}>{k.id}</option>{/each}
      </select>
    </label>
    <p class="muted small">{KINDS.find((k) => k.id === record.kind)?.about}</p>
    {#if marker && marker.want && marker.have !== marker.want}
      <p class="warn small">The room's start marker is {marker.have ?? "missing"}; a {record.kind} room places {marker.want} (docs/levels.md, "Entity types").</p>
    {/if}

    <h3>Blend</h3>
    <label class="row">Effect
      <select value={blend.effect & 3} onchange={(e) => setBlend("effect", e.currentTarget.value)}>
        {#each BLEND_EFFECTS as b, i (i)}<option value={i}>{i}: {b.name}</option>{/each}
      </select>
    </label>
    <p class="muted small">{BLEND_EFFECTS[blend.effect & 3]?.about}</p>
    <label class="row" title="BLDALPHA's EVA: BG3's weight, in 16ths (16 and up: all of it)">EVA (BG3)
      <input type="range" min="0" max="16" value={Math.min(blend.eva, 16)} oninput={(e) => setBlend("eva", e.currentTarget.value)} />
      <input type="number" min="0" max="31" value={blend.eva} onchange={(e) => setBlend("eva", e.currentTarget.value)} />
    </label>
    <label class="row" title="BLDALPHA's EVB: the weight of what is under BG3 (BG0-2, the sprites), in 16ths">EVB (under)
      <input type="range" min="0" max="16" value={Math.min(blend.evb, 16)} oninput={(e) => setBlend("evb", e.currentTarget.value)} />
      <input type="number" min="0" max="31" value={blend.evb} onchange={(e) => setBlend("evb", e.currentTarget.value)} />
    </label>
    <p class="muted small">
      A pixel of BG3 in front of BG0-2 or a sprite is (BG3 × EVA + under × EVB) / 16 a channel, at most full (the PPU's weights stop at
      16). Where BG3 is over the backdrop alone it isn't blended. The ROM's blended rooms use 4 and 16: BG3 a quarter as bright, added.
      {#if record.kind === "UNDERWATER" && blend.effect !== 0}With a blend, an underwater room also raises the sprites' priority (SetupRoomBlend).{/if}
    </p>
    <label><input type="checkbox" bind:checked={level.show.blend} /> draw the blend</label>

    <h3>Size</h3>
    <p class="small">{size[0]}x{size[1]} cells ({size[0] * 8}x{size[1] * 8} pixels), BG0's and the collision layer's scrollable size.</p>
    <div class="edges">
      {#each ["left", "right", "top", "bottom"] as const as k (k)}
        <label>{k} <input type="number" value={edges[k]} onchange={(e) => setEdge(k, e.currentTarget.value)} /></label>
      {/each}
    </div>
    <p class="muted small">
      Cells to add at each edge (negative: to take). BG0, the collision layer and the entities move with the left and top edges; each BG1-3 layer
      grows by its parallax share (cells × scale / 256), so it lines up as before. A room is at least a screen (30x20 cells) and at most 8191
      cells each way.
    </p>
    <div class="row">
      <button disabled={!anyEdge || !!editor.busy} onclick={check}>Check</button>
      <button disabled={!anyEdge || level.dirty || !!editor.busy} title={level.dirty ? "save the room's changes first" : "resize and save"} onclick={resize}>Resize</button>
    </div>
    {#if checked}
      <div class="report small">
        <div>{checked.size[0].join("x")} → {checked.size[1].join("x")} cells</div>
        {#each Object.entries(checked.layers) as [n, l] (n)}
          <div>{n}: {l.size[0].join("x")} → {l.size[1].join("x")}, moved {l.shift.join(", ")}{l.cut ? `, ${l.cut} cells cut` : ""}</div>
        {/each}
        {#if checked.entities.length}<div class="warn">{checked.entities.length} entities cut ({checked.entities.map((e) => `${e.id} ${e.type}`).join(", ")}){checked.links ? `, ${checked.links} links` : ""}</div>{/if}
        {#each checked.warnings as w (w)}<div class="warn">{w}</div>{/each}
      </div>
    {/if}

    <h3>Delete</h3>
    <button disabled={played || !!editor.busy} title={played ? "a level plays it: take it out of the level first (levels.json)" : "delete its folder and its place in levels.json"} onclick={remove}>Delete this room</button>
  </div>
{/if}

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
    padding: 0.5rem;
    font-size: 0.85rem;
  }
  h3 {
    margin: 0.5rem 0 0;
    font-size: 0.9rem;
  }
  .row {
    display: flex;
    align-items: center;
    gap: 0.4rem;
  }
  .row input[type="number"] {
    width: 4rem;
  }
  .row input[type="range"] {
    flex: 1;
  }
  p {
    margin: 0;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
  }
  .edges {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 0.3rem;
  }
  .edges input {
    width: 4.5rem;
  }
  .report {
    background: var(--panel);
    padding: 0.4rem;
    border-radius: 4px;
  }
  .warn {
    color: var(--error);
  }
</style>

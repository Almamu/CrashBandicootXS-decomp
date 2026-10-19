<script lang="ts">
  // A new room (docs/editors.md, "The level editor", "Rooms"): a copy of a
  // room, or an empty room laid out like it (its tile sets, scroll scales,
  // palette and record; blank pictures and collision, its start marker),
  // under the next free gRoomNN, listed in levels.json and, if asked, played
  // by a level. The backend saves it like any save: checked, all or nothing.
  import type { NewRoom } from "@crash/schema-types";
  import { editor } from "../../lib/state.svelte.ts";
  import { level } from "../../lib/level/state.svelte.ts";

  let { onclose }: { onclose: () => void } = $props();

  const all = $derived([...level.levels.flatMap((l) => l.rooms), ...level.others]);
  const base = $derived(`games/${editor.project?.game ?? "xs"}/data/levels/`);
  let from = $state(level.view?.path ?? "");
  let folder = $state(level.view ? `${level.view.path.split("/").slice(-2, -1)[0]}/extra` : "");
  let empty = $state(false);
  let width = $state(60);
  let height = $state(20);
  let attachLevel = $state("");
  let attachAs = $state<"rooms" | "bonus" | "gem_path">("rooms");

  const target = $derived(level.levels.find((l) => l.constant === attachLevel));
  const valid = $derived(/^[a-z0-9_]+\/[a-z0-9_]+$/.test(folder) && !!from && !all.some((r) => r.folder === folder));

  async function create() {
    const body: NewRoom = { from: from.startsWith(base) ? from.slice(base.length) : from, folder, empty };
    if (empty) body.size = [width, height];
    if (target) body.attach = { level: target.constant, as: attachAs };
    if (await level.newRoom(body)) onclose();
  }
</script>

<div class="backdrop" role="presentation" onclick={(e) => e.target === e.currentTarget && onclose()}>
  <div class="dialog" role="dialog" aria-modal="true" aria-label="New room">
    <h2>New room</h2>
    <label>From
      <select bind:value={from}>
        {#each all as r (r.path)}<option value={r.path}>{r.folder}</option>{/each}
      </select>
    </label>
    <label>Folder <input bind:value={folder} placeholder="level/slot" aria-label="the new room's folder" /></label>
    <label><input type="checkbox" bind:checked={empty} /> empty: the source's layers, tile sets, palette and kind, blank, with its start marker</label>
    {#if empty}
      <div class="row">
        <label>Width <input type="number" min="30" max="8191" bind:value={width} /></label>
        <label>Height <input type="number" min="20" max="8191" bind:value={height} /></label>
        <span class="muted">cells ({width * 8}x{height * 8} pixels; at least a screen, 30x20)</span>
      </div>
    {/if}
    <label>Played by
      <select bind:value={attachLevel}>
        <option value="">no level (yet)</option>
        {#each level.levels as l (l.constant)}<option value={l.constant}>{l.name}</option>{/each}
      </select>
      {#if target}
        <select bind:value={attachAs}>
          <option value="rooms">after its rooms</option>
          <option value="bonus">as its bonus room{target.bonus ? " (replacing it)" : ""}</option>
          <option value="gem_path">as its gem-path room{target.gemPath ? " (replacing it)" : ""}</option>
        </select>
      {/if}
    </label>
    <p class="muted">The room gets the next free gRoomNN symbols and is listed in levels.json; the build lays it out after the ROM's rooms (docs/levels.md, "Adding a room").</p>
    <div class="actions">
      <button onclick={onclose}>Cancel</button>
      <button class="primary" disabled={!valid || !!editor.busy} onclick={create}>Make the room</button>
    </div>
  </div>
</div>

<style>
  .backdrop {
    position: fixed;
    inset: 0;
    background: rgb(0 0 0 / 40%);
    display: grid;
    place-items: center;
    z-index: 10;
  }
  .dialog {
    background: var(--bg);
    border: 1px solid var(--line);
    border-radius: 6px;
    padding: 1rem 1.25rem;
    width: min(34rem, 94vw);
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
    font-size: 0.9rem;
  }
  h2 {
    margin: 0;
    font-size: 1.05rem;
  }
  .row {
    display: flex;
    gap: 0.6rem;
    align-items: center;
    flex-wrap: wrap;
  }
  .row input {
    width: 5rem;
  }
  .muted {
    color: var(--muted);
    font-size: 0.8rem;
    margin: 0;
  }
  .actions {
    display: flex;
    justify-content: flex-end;
    gap: 0.4rem;
  }
</style>

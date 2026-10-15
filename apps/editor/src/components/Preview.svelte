<script lang="ts">
  // `make preview`: tools/preview.py in the project's tree, the PC port
  // started at a level's room (docs/editors.md, "Preview").
  import { onMount } from "svelte";
  import { assetUrl, client } from "../lib/api.ts";
  import { editor } from "../lib/state.svelte.ts";

  let { onclose }: { onclose: () => void } = $props();

  interface Level {
    constant?: string;
    name?: string;
    rooms?: (string | { stage: string })[];
  }

  let levels = $state<Level[]>([]);
  let level = $state("0");
  let room = $state("0");
  let headless = $state(true);
  let frames = $state(700);
  let noBuild = $state(false);
  let running = $state(false);
  let result = $state<{ ok: boolean; text: string; image?: string | null } | null>(null);

  const rooms = $derived(levels[Number(level)]?.rooms ?? []);

  onMount(async () => {
    const game = editor.project?.game ?? "xs";
    try {
      const f = await client.read<"levels.schema.json">(editor.projectId, `games/${game}/data/levels/levels.json`);
      levels = ((f.data as unknown as { levels?: Level[] }).levels ?? []) as Level[];
    } catch {
      levels = [];
    }
  });

  async function run() {
    running = true;
    result = null;
    try {
      const r = await client.preview(editor.projectId, { level: Number(level), room: Number(room), headless, frames, noBuild });
      if ("started" in r) result = { ok: true, text: `Started: ${r.command.join(" ")} (its output: ${r.log})` };
      else {
        const errs = r.diagnostics.filter((d) => d.severity === "error");
        result = {
          ok: r.ok,
          text: r.ok ? `${JSON.stringify(r.preview ?? {})}` : errs.map((d) => d.message).join("\n") || r.output.slice(-2000),
          image: r.image ? assetUrl(r.image) : null,
        };
      }
    } catch (e) {
      result = { ok: false, text: e instanceof Error ? e.message : String(e) };
    } finally {
      running = false;
    }
  }
</script>

<div class="backdrop" role="presentation" onclick={(e) => e.target === e.currentTarget && !running && onclose()}>
  <div class="dialog" role="dialog" aria-modal="true" aria-label="Preview">
    <h2>Preview a room</h2>
    <p class="muted">Builds the PC port from {editor.project?.root === "." ? "the tree" : editor.project?.root}'s data (<code>make PLATFORM=pc</code>) and starts it at the room. Unsaved changes aren't in it.</p>
    <div class="grid">
      <label for="pv-level">Level</label>
      <select id="pv-level" bind:value={level} onchange={() => (room = "0")}>
        {#each levels as l, i (i)}<option value={String(i)}>{i}: {l.name ?? l.constant}</option>{/each}
        {#if !levels.length}<option value="0">0</option>{/if}
      </select>
      <label for="pv-room">Room</label>
      <select id="pv-room" bind:value={room}>
        {#each rooms as r, i (i)}<option value={String(i)}>{i}: {typeof r === "string" ? r : `stage ${r.stage}`}</option>{/each}
        {#if !rooms.length}<option value="0">0</option>{/if}
      </select>
      <label for="pv-headless">Headless</label>
      <span><input id="pv-headless" type="checkbox" bind:checked={headless} /> a picture after
        <input type="number" min="1" bind:value={frames} disabled={!headless} style="width:6rem" /> frames (otherwise the game's window opens)</span>
      <label for="pv-nobuild">Don't build</label>
      <span><input id="pv-nobuild" type="checkbox" bind:checked={noBuild} /> run the PC port as it is</span>
    </div>
    <div class="actions">
      <button onclick={onclose} disabled={running}>Close</button>
      <button class="primary" onclick={run} disabled={running}>{running ? "Running…" : "Run"}</button>
    </div>
    {#if result}
      <pre class:bad={!result.ok}>{result.text}</pre>
      {#if result.image}<img src={result.image} alt="the room, from the PC port" />{/if}
    {/if}
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
    width: min(44rem, 94vw);
    max-height: 92vh;
    overflow: auto;
  }
  h2 {
    margin: 0 0 0.4rem;
    font-size: 1.1rem;
  }
  .muted {
    color: var(--muted);
    font-size: 0.85rem;
  }
  .grid {
    display: grid;
    grid-template-columns: max-content 1fr;
    gap: 0.4rem 0.75rem;
    align-items: center;
    margin: 0.75rem 0;
  }
  .actions {
    display: flex;
    justify-content: flex-end;
    gap: 0.5rem;
  }
  pre {
    white-space: pre-wrap;
    font-size: 0.8rem;
    background: var(--panel);
    padding: 0.5rem;
    border-radius: 4px;
  }
  pre.bad {
    color: var(--error);
  }
  img {
    image-rendering: pixelated;
    width: 100%;
    max-width: 720px;
    display: block;
    margin: 0.5rem auto 0;
  }
</style>

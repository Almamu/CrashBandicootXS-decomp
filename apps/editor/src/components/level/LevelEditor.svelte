<script lang="ts">
  // The level editor (docs/editors.md, "The level editor"): the level table
  // on the left, the room in the middle, the tools' panels on the right.
  import { onMount } from "svelte";
  import { assetUrl, client, desktop } from "../../lib/api.ts";
  import { editor } from "../../lib/state.svelte.ts";
  import { BG_NAMES, level, type Tool } from "../../lib/level/state.svelte.ts";
  import CollisionPanel from "./CollisionPanel.svelte";
  import EntityPanel from "./EntityPanel.svelte";
  import LevelNav from "./LevelNav.svelte";
  import MetaTilePanel from "./MetaTilePanel.svelte";
  import PalettePanel from "./PalettePanel.svelte";
  import RoomPanel from "./RoomPanel.svelte";
  import RoomCanvas from "./RoomCanvas.svelte";
  import TilePanel from "./TilePanel.svelte";

  let canvas = $state<RoomCanvas>();
  let cam = $state({ x: 0, y: 0 });
  let hover = $state<{ world: { x: number; y: number }; cell: { x: number; y: number } | null } | null>(null);
  let panel = $state<"entity" | "tiles" | "meta" | "collision" | "palette" | "room">("entity");
  let play = $state<{ running: boolean; text: string; ok: boolean; image?: string | null } | null>(null);

  const TOOLS: { id: Tool; label: string; key: string; title: string }[] = [
    { id: "select", label: "Select", key: "v", title: "select and drag entities (shift: snap to 8); drag the room to pan" },
    { id: "add", label: "Add", key: "a", title: "click to add an entity of the type chosen" },
    { id: "link", label: "Link", key: "l", title: "click a crate, then the crate on top of it" },
    { id: "tile", label: "Tiles", key: "t", title: "paint a BG layer with its tile set's tiles" },
    { id: "meta", label: "Meta-tiles", key: "m", title: "stamp blocks of a tile set's cells; shift-drag to select cells to make one of" },
    { id: "collision", label: "Collision", key: "c", title: "paint the collision layer's cells" },
  ];

  const entry = $derived(level.roomEntry(level.view?.path));
  const playable = $derived(entry?.level !== undefined && entry?.room !== undefined);

  $effect(() => {
    // the project changed (or a mod's tree was made again): its level table
    void level.syncProject(editor.projectId, !!editor.project?.ready);
  });

  function setTool(t: Tool) {
    level.tool = t;
    level.linkFrom = null;
    if (t === "tile") panel = "tiles";
    else if (t === "meta") panel = "meta";
    else if (t === "collision") {
      panel = "collision";
      level.show.collision = true;
    } else if (panel === "tiles" || panel === "meta" || panel === "collision") panel = "entity";
  }

  async function runPreview() {
    if (!entry || entry.level === undefined) return;
    if (level.dirty && !confirm("The preview builds the saved data: this room's changes aren't saved. Preview anyway?")) return;
    const headless = !desktop;
    play = { running: true, ok: true, text: headless ? "Building the PC port and running it headless…" : "Building the PC port and starting it…" };
    try {
      const r = await client.preview(editor.projectId, { level: entry.level, room: entry.room, headless, frames: 700 });
      if ("started" in r) play = { running: false, ok: true, text: `Started in its window (${r.command.join(" ")}); its output: ${r.log}` };
      else {
        const errs = r.diagnostics.filter((d) => d.severity === "error");
        play = { running: false, ok: r.ok, text: r.ok ? "The room after 700 frames, from the PC port:" : errs.map((d) => d.message).join("\n") || r.output.slice(-2000), image: r.image ? assetUrl(r.image) : null };
      }
    } catch (e) {
      play = { running: false, ok: false, text: e instanceof Error ? e.message : String(e) };
    }
  }

  function onkeydown(e: KeyboardEvent) {
    if (!level.view) return;
    const t = e.target;
    const typing = t instanceof HTMLInputElement || t instanceof HTMLTextAreaElement || t instanceof HTMLSelectElement;
    const mod = e.ctrlKey || e.metaKey;
    if (mod && e.key.toLowerCase() === "s") {
      e.preventDefault();
      if (level.dirty) void level.save();
    } else if (mod && !typing && e.key.toLowerCase() === "z") {
      e.preventDefault();
      if (e.shiftKey) level.redo();
      else level.undo();
    } else if (mod && !typing && e.key.toLowerCase() === "y") {
      e.preventDefault();
      level.redo();
    } else if (!mod && !typing) {
      if ((e.key === "Delete" || e.key === "Backspace") && level.selected !== null) {
        e.preventDefault();
        level.deleteEntity(level.selected);
      } else if (e.key === "Escape") {
        level.selected = null;
        level.linkFrom = null;
        level.metaSelection = null;
      } else {
        const tool = TOOLS.find((x) => x.key === e.key.toLowerCase());
        if (tool) setTool(tool.id);
      }
    }
  }

  onMount(() => {
    const before = (e: BeforeUnloadEvent) => {
      if (level.dirty) e.preventDefault();
    };
    window.addEventListener("beforeunload", before);
    return () => window.removeEventListener("beforeunload", before);
  });
</script>

<svelte:window {onkeydown} />

<div class="le">
  <aside class="left"><LevelNav /></aside>
  <section class="middle">
    {#if level.view}
      <div class="toolbar">
        <div class="seg" role="group" aria-label="tool">
          {#each TOOLS as t (t.id)}
            <button class:on={level.tool === t.id} title="{t.title} ({t.key})" onclick={() => setTool(t.id)}>{t.label}</button>
          {/each}
        </div>
        {#if level.tool === "add"}
          <select bind:value={level.addType} title="the type the Add tool places">
            {#each level.types as t (t.type)}<option value={t.name ?? t.type}>{t.name ?? `type ${t.type}`}</option>{/each}
          </select>
        {/if}
        <span class="sep"></span>
        <button disabled={!level.canUndo || !!editor.busy} title="undo {level.undoLabel} (ctrl+z)" onclick={() => level.undo()}>Undo</button>
        <button disabled={!level.canRedo || !!editor.busy} title="redo {level.redoLabel} (ctrl+shift+z)" onclick={() => level.redo()}>Redo</button>
        <span class="sep"></span>
        <button onclick={() => canvas?.zoomBy(0.5)} title="zoom out">−</button>
        <button onclick={() => canvas?.zoomBy(2)} title="zoom in">+</button>
        <button onclick={() => canvas?.fitRoom()} title="the whole room">Fit</button>
        <span class="spacer"></span>
        <button disabled={!playable || play?.running} title={playable ? `run the PC port at this room (level ${entry?.level}, room ${entry?.room})` : "a room no level plays can't be started: make it a level's room, bonus or gem-path room first"} onclick={runPreview}>Play this room</button>
        <button disabled={!level.dirty || !!editor.busy} onclick={() => level.revert()}>Revert</button>
        <button class="primary" disabled={!level.dirty || !!editor.busy} onclick={() => level.save()} title="save (ctrl+s): checked like any save, all or nothing">Save</button>
      </div>
      <div class="layers">
        {#each BG_NAMES as n (n)}
          {#if level.view.layers[n]}
            <label title="priority {level.view.layers[n]?.priority}, scroll {level.view.layers[n]?.scale.join('/')}/256, {level.view.layers[n]?.tileset}">
              <input type="checkbox" bind:checked={level.show[n]} /> {n}
            </label>
          {/if}
        {/each}
        <label><input type="checkbox" bind:checked={level.show.collision} /> collision</label>
        <label><input type="checkbox" bind:checked={level.show.entities} /> entities</label>
        <label><input type="checkbox" bind:checked={level.show.labels} /> labels</label>
        <label><input type="checkbox" bind:checked={level.show.links} /> links</label>
        <label title="the entity groups: 256-pixel columns"><input type="checkbox" bind:checked={level.show.columns} /> groups</label>
        <label title="240x160 screens"><input type="checkbox" bind:checked={level.show.screens} /> screens</label>
        <label title="the game's camera at the view's centre"><input type="checkbox" bind:checked={level.show.camera} /> camera</label>
        <label title="scroll each BG layer by its parallax factor for the camera, as the game does"><input type="checkbox" bind:checked={level.parallax} /> parallax</label>
        {#if (level.room?.record?.blend?.effect ?? 0) % 4 === 1}
          <label title="the room's alpha blend: BG3 over BG0-2 (the Room tab)"><input type="checkbox" bind:checked={level.show.blend} /> blend</label>
        {/if}
      </div>
      <div class="canvas">
        <RoomCanvas bind:this={canvas} onhover={(h) => (hover = h)} oncamera={(c) => { if (c.x !== cam.x || c.y !== cam.y) cam = c; }} />
        <div class="status">
          <span title="the game's camera (the screen's top left) at the view's centre">camera {cam.x}, {cam.y}</span>
          {#if hover} — ({hover.world.x}, {hover.world.y}){#if hover.cell} cell {hover.cell.x}, {hover.cell.y}{/if}{/if}
          {#if level.dirty}<strong> — not saved</strong>{/if}
        </div>
      </div>
    {:else}
      <div class="empty">
        {#if level.loading}<p>Opening {level.loading}…</p>
        {:else}
          <p>Pick a room on the left. The editor draws it from its folder (room.json, its layer pictures, collision.png) as the game does, and saves your edits back to them through the backend's checks.</p>
        {/if}
      </div>
    {/if}
  </section>
  <aside class="right">
    {#if level.view}
      <div class="tabs" role="tablist">
        <button class:on={panel === "entity"} onclick={() => (panel = "entity")}>Entities</button>
        <button class:on={panel === "tiles"} onclick={() => setTool("tile")}>Tiles</button>
        <button class:on={panel === "meta"} onclick={() => setTool("meta")}>Meta-tiles</button>
        <button class:on={panel === "collision"} onclick={() => setTool("collision")}>Collision</button>
        <button class:on={panel === "palette"} onclick={() => (panel = "palette")}>Palette</button>
        <button class:on={panel === "room"} onclick={() => (panel = "room")} title="the room's record: its kind and blend">Room</button>
      </div>
      <div class="panel">
        {#if panel === "entity"}<EntityPanel oncentre={(x, y) => canvas?.centreOn(x, y)} />
        {:else if panel === "tiles"}<TilePanel />
        {:else if panel === "meta"}<MetaTilePanel />
        {:else if panel === "collision"}<CollisionPanel />
        {:else if panel === "palette"}<PalettePanel />
        {:else}<RoomPanel />{/if}
      </div>
    {/if}
  </aside>
</div>

{#if play}
  <div class="backdrop" role="presentation" onclick={(e) => e.target === e.currentTarget && !play?.running && (play = null)}>
    <div class="dialog" role="dialog" aria-modal="true" aria-label="Play this room">
      <h2>Play {level.view?.path}</h2>
      <pre class:bad={!play.ok}>{play.text}</pre>
      {#if play.image}<img src={play.image} alt="the room, from the PC port" />{/if}
      <div class="actions"><button disabled={play.running} onclick={() => (play = null)}>Close</button></div>
    </div>
  </div>
{/if}

<style>
  .le {
    display: grid;
    grid-template-columns: minmax(12rem, 16rem) 1fr minmax(18rem, 22rem);
    height: 100%;
    min-height: 0;
  }
  .left,
  .right {
    overflow: auto;
    min-height: 0;
    border-right: 1px solid var(--line);
  }
  .right {
    border-right: none;
    border-left: 1px solid var(--line);
    display: flex;
    flex-direction: column;
  }
  .middle {
    display: grid;
    grid-template-rows: auto auto 1fr;
    min-width: 0;
    min-height: 0;
  }
  .toolbar,
  .layers {
    display: flex;
    flex-wrap: wrap;
    gap: 0.35rem;
    align-items: center;
    padding: 0.3rem 0.5rem;
    border-bottom: 1px solid var(--line);
    background: var(--panel);
    font-size: 0.85rem;
  }
  .layers label {
    white-space: nowrap;
  }
  .seg {
    display: flex;
  }
  .seg button {
    border-radius: 0;
    margin-left: -1px;
  }
  .seg button:first-child {
    border-radius: 4px 0 0 4px;
  }
  .seg button:last-child {
    border-radius: 0 4px 4px 0;
  }
  .seg button.on,
  .tabs button.on {
    background: var(--accent);
    color: var(--accent-text);
    border-color: var(--accent);
  }
  .sep {
    width: 0.4rem;
  }
  .spacer {
    flex: 1;
  }
  .status {
    position: absolute;
    left: 0;
    bottom: 0;
    padding: 0.1rem 0.5rem;
    background: rgb(0 0 0 / 55%);
    color: #fff;
    font-size: 0.8rem;
    font-variant-numeric: tabular-nums;
    pointer-events: none;
    border-top-right-radius: 4px;
  }
  .canvas {
    position: relative;
    min-height: 0;
    min-width: 0;
  }
  .empty {
    padding: 2rem;
    max-width: 40rem;
    color: var(--muted);
  }
  .tabs {
    display: flex;
    flex-wrap: wrap;
    gap: 0.2rem;
    padding: 0.4rem;
    border-bottom: 1px solid var(--line);
  }
  .panel {
    overflow: auto;
    min-height: 0;
    flex: 1;
  }
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
    font-size: 1.05rem;
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
    margin: 0.5rem auto;
  }
  .actions {
    display: flex;
    justify-content: flex-end;
  }
</style>

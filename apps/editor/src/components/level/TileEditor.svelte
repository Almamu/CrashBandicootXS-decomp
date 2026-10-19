<script lang="ts">
  // Drawing tiles (docs/editors.md, "Tiles"): a pixel editor for a tile of
  // the paint layer's set, in the room's palette (a BG1-3 tile in the brush's
  // bank), and a picture imported as cells. The set is made from its rooms'
  // pictures, so a tile is drawn by drawing cells: "Redraw" replaces the
  // cells that draw the tile, "New tile" adds the drawing to the set as the
  // brush (the build keeps it once a cell draws it), "Clear" blanks the cells
  // that draw it (a tile no cell draws is gone from the set). With "every
  // room", Redraw and Clear go to every room of the set at once (room/redraw,
  // saved).
  import type { TileImport } from "@crash/schema-types";
  import { client } from "../../lib/api.ts";
  import { editor } from "../../lib/state.svelte.ts";
  import { pixelTable } from "../../lib/level/palette.ts";
  import { transparentIndex } from "../../lib/level/render.ts";
  import { CELL, CELL_PIXELS, fromBase64, layerBanks } from "../../lib/level/paint.ts";
  import { level, type TileSet } from "../../lib/level/state.svelte.ts";

  let { set }: { set: TileSet } = $props();

  const SCALE = 20;
  let canvas = $state<HTMLCanvasElement>();
  let pixels = $state(new Uint8Array(CELL_PIXELS));
  let source = $state<number | null>(null);
  let colour = $state(1);
  let painting = false;
  /** Redraw and Clear in every room of the set (saved at once), not the layer here */
  let everywhere = $state(false);

  const bg0 = $derived(set.bpp === 8);
  const bank = $derived(level.brush.bank);
  const here = $derived.by(() => {
    void level.pixels;
    return source === null ? 0 : level.tileUses(source);
  });
  const pending = $derived.by(() => {
    void level.pixels;
    void level.hv;
    return level.pendingTiles();
  });

  function load() {
    const t = level.brush.tile;
    pixels = set.tiles.slice(t * CELL_PIXELS, (t + 1) * CELL_PIXELS);
    source = t;
  }

  /** The palette entry a pixel value shows: BG0's own index, BG1-3's bank * 16 + p. */
  function entry(p: number): number {
    return bg0 ? p : bank * 16 + p;
  }

  $effect(() => {
    const c = canvas;
    void level.palette;
    const px = pixels;
    const b = bank;
    if (!c) return;
    c.width = CELL;
    c.height = CELL;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    const img = ctx.createImageData(CELL, CELL);
    const out = new Uint32Array(img.data.buffer);
    const t = pixelTable(level.palette, transparentIndex(bg0));
    for (let i = 0; i < CELL_PIXELS; i++) out[i] = t[bg0 ? px[i]! : b * 16 + px[i]!]!;
    ctx.putImageData(img, 0, 0);
  });

  function at(e: PointerEvent): number | null {
    if (!canvas) return null;
    const r = canvas.getBoundingClientRect();
    const x = Math.floor(((e.clientX - r.left) / r.width) * CELL);
    const y = Math.floor(((e.clientY - r.top) / r.height) * CELL);
    return x >= 0 && y >= 0 && x < CELL && y < CELL ? y * CELL + x : null;
  }

  function paint(e: PointerEvent) {
    const i = at(e);
    if (i === null) return;
    if (e.altKey) {
      colour = pixels[i]!;
      return;
    }
    if (pixels[i] === colour) return;
    const next = pixels.slice();
    next[i] = colour;
    pixels = next;
  }

  const colours = $derived(pixelTable(level.palette));

  function swatch(p: number): string {
    if (p === 0) return "transparent";
    const v = colours[entry(p)]!;
    return `rgb(${v & 255},${(v >> 8) & 255},${(v >> 16) & 255})`;
  }

  // every room: a tile the build made, other than the blank tile 0, with the room's changes saved
  const whyNotEverywhere = $derived(
    source === null
      ? "load a tile first"
      : source === 0
        ? "tile 0 is the blank tile every set starts with"
        : source >= set.built
          ? "a new tile isn't in the other rooms: save the room first"
          : level.dirty
            ? "save the room's changes first"
            : null,
  );

  async function redraw() {
    if (source === null) return;
    if (everywhere) {
      if (whyNotEverywhere) return;
      if (!confirm(`Redraw tile ${source} in the ${set.uses[source] ?? 0} cells of all of ${set.name}'s rooms, and save them?`)) return;
      if (await level.redrawEverywhere(source, pixels)) source = null;
      return;
    }
    const n = level.replaceTile(source, pixels);
    editor.error = n ? null : `no cell of ${level.paintLayer} draws tile ${source}`;
  }

  async function clear() {
    if (source === null) return;
    if (everywhere) {
      if (whyNotEverywhere) return;
      if (!confirm(`Blank the ${set.uses[source] ?? 0} cells of all of ${set.name}'s rooms that draw tile ${source}, and save them?`)) return;
      if (await level.redrawEverywhere(source, new Uint8Array(CELL_PIXELS))) source = null;
      return;
    }
    if (!confirm(`Blank the ${here} cells of ${level.paintLayer} that draw tile ${source}?`)) return;
    level.replaceTile(source, new Uint8Array(CELL_PIXELS), `Clear tile ${source}`);
  }

  function flip(h: boolean) {
    const next = new Uint8Array(CELL_PIXELS);
    for (let y = 0; y < CELL; y++) for (let x = 0; x < CELL; x++) next[y * CELL + x] = pixels[h ? y * CELL + CELL - 1 - x : (CELL - 1 - y) * CELL + x]!;
    pixels = next;
  }

  // -- importing a picture ------------------------------------------------------------
  let file = $state<File | null>(null);
  let ix = $state(0);
  let iy = $state(0);
  let imported = $state<TileImport | null>(null);

  function banksOfLayer(): number[] {
    const l = level.layers[level.paintLayer];
    return !l || bg0 ? [] : layerBanks(l);
  }

  async function importPicture() {
    const f = file;
    const path = level.view?.path;
    if (!f || !path) return;
    const bytes = new Uint8Array(await f.arrayBuffer());
    let bin = "";
    for (let i = 0; i < bytes.length; i++) bin += String.fromCharCode(bytes[i]!);
    const r = await editor.run(`Importing ${f.name}`, () =>
      client.importTiles(editor.projectId, { path, layer: level.paintLayer, png: btoa(bin), palette: level.palette, banks: banksOfLayer() }),
    );
    if (!r) return;
    const total = set.count + r.stats.new;
    if (total > set.limit && !confirm(`${set.name} would have ${total} tiles, past the ${set.limit} the engine holds: the save will be refused. Paste anyway?`)) return;
    imported = r;
    level.pasteCells({ x: ix, y: iy }, r.cols, r.rows, fromBase64(r.cells));
  }
</script>

<div class="te">
  <div class="head">
    <strong>Tile editor</strong>
    <button onclick={load} title="edit the brush's tile">Load tile {level.brush.tile}</button>
    <button onclick={() => flip(true)} title="flip the drawing across">↔</button>
    <button onclick={() => flip(false)} title="flip the drawing down">↕</button>
  </div>
  <div class="body">
    <canvas
      bind:this={canvas}
      style="width:{CELL * SCALE}px;height:{CELL * SCALE}px"
      onpointerdown={(e) => {
        painting = true;
        (e.currentTarget as HTMLCanvasElement).setPointerCapture(e.pointerId);
        paint(e);
      }}
      onpointermove={(e) => painting && paint(e)}
      onpointerup={() => (painting = false)}
      data-testid="tile-editor"
    ></canvas>
    <div class="pal" class:wide={bg0} role="listbox" aria-label="the colours">
      {#each { length: bg0 ? 256 : 16 } as _, p (p)}
        <button class="sw" class:current={p === colour} style="background:{swatch(p)}" title="{p === 0 ? 'transparent' : `entry ${entry(p)}`}" aria-label="colour {p}" onclick={() => (colour = p)}></button>
      {/each}
    </div>
  </div>
  <p class="muted small">
    {#if source !== null}Tile {source}: {here} cells of {level.paintLayer} here, {set.uses[source] ?? 0} in all of {set.name}'s rooms (as saved).{/if}
    {set.name}: {set.count} of {set.limit} tiles{#if pending}, {pending} new in this room's changes{/if}. Alt-click picks a pixel's colour.
  </p>
  <div class="row">
    <button onclick={() => level.addTile(pixels)} title="add the drawing to the set and paint with it">New tile</button>
    <button
      disabled={source === null || (everywhere && !!whyNotEverywhere) || !!editor.busy}
      onclick={redraw}
      title={everywhere
        ? (whyNotEverywhere ?? "redraw every cell of the set's rooms that draws the tile, in its flips and bank, and save")
        : "redraw every cell of this layer that draws the tile, in its flips and bank"}>Redraw tile {source ?? ""}</button
    >
    <button
      disabled={source === null || (everywhere ? !!whyNotEverywhere || !set.uses[source] : !here) || !!editor.busy}
      onclick={clear}
      title={everywhere ? (whyNotEverywhere ?? "blank the cells of the set's rooms that draw the tile, and save") : "blank the cells of this layer that draw the tile"}
      >Clear tile {source ?? ""}</button
    >
    <label title="Redraw and Clear the tile in every room of {set.name} at once (the saved pictures; saved, all or nothing)"
      ><input type="checkbox" bind:checked={everywhere} /> every room</label
    >
  </div>
  {#if everywhere && whyNotEverywhere && source !== null}<p class="muted small">Every room: {whyNotEverywhere}.</p>{/if}

  <strong>Import a picture</strong>
  <input type="file" accept="image/*" onchange={(e) => (file = e.currentTarget.files?.[0] ?? null)} aria-label="a picture to import" />
  <div class="row">
    <label>at cell <input type="number" min="0" bind:value={ix} /></label>
    <label><input type="number" min="0" bind:value={iy} /></label>
    <button disabled={!file || !!editor.busy} onclick={importPicture}>Import</button>
  </div>
  {#if imported}
    <p class="small">
      {imported.stats.cells} cells: {imported.stats.tiles} of the set's tiles, {imported.stats.flipped} flipped, {imported.stats.blank} blank,
      {imported.stats.new} new tiles ({imported.stats.setCount + imported.stats.new} of {imported.stats.limit}).
    </p>
  {/if}
  <p class="muted small">
    The picture is quantised to the room's palette (BG1-3: each cell to the bank of this layer's whose 15 colours draw it closest), cut into
    cells and written at the cell given; a cell that is a tile of the set, flipped or not, costs nothing.
  </p>
</div>

<style>
  .te {
    display: flex;
    flex-direction: column;
    gap: 0.35rem;
    border-top: 1px solid var(--line);
    padding-top: 0.4rem;
    font-size: 0.85rem;
  }
  .head,
  .row {
    display: flex;
    gap: 0.35rem;
    align-items: center;
    flex-wrap: wrap;
  }
  .row input[type="number"] {
    width: 4rem;
  }
  .body {
    display: flex;
    gap: 0.5rem;
    align-items: flex-start;
  }
  canvas {
    image-rendering: pixelated;
    background: repeating-conic-gradient(#888 0 25%, #bbb 0 50%) 0 0 / 20px 20px;
    border: 1px solid var(--line);
    cursor: crosshair;
    touch-action: none;
  }
  .pal {
    display: grid;
    grid-template-columns: repeat(4, 1.2rem);
    gap: 2px;
  }
  .pal.wide {
    grid-template-columns: repeat(16, 0.6rem);
    gap: 1px;
  }
  .sw {
    aspect-ratio: 1;
    padding: 0;
    border: 1px solid rgb(0 0 0 / 30%);
    border-radius: 0;
    min-width: 0;
  }
  .sw.current {
    outline: 2px solid #00e5ff;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
</style>

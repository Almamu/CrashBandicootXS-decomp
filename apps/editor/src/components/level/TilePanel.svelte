<script lang="ts">
  // The tile tool's brush: a BG layer to paint, a tile of its tile set (the
  // build's, from every room that names the set), its flips and, on BG1-3,
  // its palette bank. Alt-click on the room picks a cell's tile.
  import { pixelTable } from "../../lib/level/palette.ts";
  import { transparentIndex } from "../../lib/level/render.ts";
  import { tileBlock } from "../../lib/level/paint.ts";
  import { BG_NAMES, level, type BgName } from "../../lib/level/state.svelte.ts";

  const COLS = 16;
  const SCALE = 2;
  let sheet = $state<HTMLCanvasElement>();
  let preview = $state<HTMLCanvasElement>();

  const layers = $derived(BG_NAMES.filter((n) => level.view?.layers[n]));
  const set = $derived(level.tileSets[level.paintLayer]);
  const info = $derived(level.view?.layers[level.paintLayer]);

  $effect(() => {
    if (level.view && level.tool === "tile" && !level.tileSets[level.paintLayer]) void level.loadTiles(level.paintLayer);
  });

  function table(bank: number): Uint32Array {
    const bg0 = level.paintLayer === "bg0";
    const t = pixelTable(level.palette, transparentIndex(bg0));
    if (bg0) return t;
    // a 4bpp tile's pixel p in a bank is entry bank * 16 + p
    const out = new Uint32Array(256);
    for (let i = 0; i < 16; i++) out[i] = t[bank * 16 + i]!;
    return out;
  }

  $effect(() => {
    // the sheet: every tile, 16 a row, in the brush's bank
    const s = set;
    const c = sheet;
    void level.palette;
    const bank = level.brush.bank;
    if (!c || !s || typeof s === "string") return;
    const rows = Math.ceil(s.count / COLS);
    c.width = COLS * 8;
    c.height = rows * 8;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    const img = ctx.createImageData(c.width, c.height);
    const px = new Uint32Array(img.data.buffer);
    const t = table(bank);
    for (let k = 0; k < s.count; k++) {
      const ox = (k % COLS) * 8;
      const oy = Math.floor(k / COLS) * 8;
      for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x++) px[(oy + y) * c.width + ox + x] = t[s.tiles[k * 64 + y * 8 + x]!]!;
    }
    ctx.putImageData(img, 0, 0);
  });

  $effect(() => {
    // the brush, flipped, at 4x
    const c = preview;
    const s = set;
    void level.palette;
    const b = level.brush;
    if (!c || !s || typeof s === "string") return;
    c.width = 8;
    c.height = 8;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    const img = ctx.createImageData(8, 8);
    const px = new Uint32Array(img.data.buffer);
    const block = tileBlock(s.tiles, b, s.bpp);
    const t = pixelTable(level.palette, transparentIndex(level.paintLayer === "bg0"));
    for (let i = 0; i < 64; i++) px[i] = t[block[i]!]!;
    ctx.putImageData(img, 0, 0);
  });

  function pick(e: MouseEvent) {
    const s = set;
    if (!s || typeof s === "string" || !sheet) return;
    const r = sheet.getBoundingClientRect();
    const k = Math.floor((e.clientY - r.top) / (8 * SCALE)) * COLS + Math.floor((e.clientX - r.left) / (8 * SCALE));
    if (k < 0 || k >= s.count) return;
    level.brush = { ...level.brush, tile: k, bank: s.bpp === 4 ? (s.banks[k] ?? 0) : 0 };
  }

  function setLayer(n: BgName) {
    level.paintLayer = n;
    level.brush = { tile: 0, hflip: false, vflip: false, bank: 0 };
  }
</script>

<div class="panel">
  <label class="row">
    <span>layer</span>
    <select value={level.paintLayer} onchange={(e) => setLayer(e.currentTarget.value as BgName)}>
      {#each layers as n (n)}<option value={n}>{n} ({level.view?.layers[n]?.tileset})</option>{/each}
    </select>
  </label>
  {#if info && (info.scale[0] !== 256 || info.scale[1] !== 256) && level.parallax}
    <p class="note">{level.paintLayer} scrolls at {info.scale[0]}/256 × {info.scale[1]}/256: with parallax on it moves under the cursor as the view pans. Turn parallax off to paint it at its own place.</p>
  {/if}
  {#if !set || set === "loading"}
    <p class="muted">Loading {info?.tileset}… (the build's tile set, from all of its rooms' pictures)</p>
  {:else if typeof set === "string"}
    <p class="error">{set}</p>
  {:else}
    <div class="brush">
      <canvas class="preview" bind:this={preview}></canvas>
      <div>
        <div><strong>tile {level.brush.tile}</strong> <span class="muted">of {set.count} ({set.name}, {set.bpp}bpp)</span></div>
        <label><input type="checkbox" checked={level.brush.hflip} onchange={(e) => (level.brush = { ...level.brush, hflip: e.currentTarget.checked })} /> flip X</label>
        <label><input type="checkbox" checked={level.brush.vflip} onchange={(e) => (level.brush = { ...level.brush, vflip: e.currentTarget.checked })} /> flip Y</label>
        {#if set.bpp === 4}
          <label>bank <input type="number" min="0" max="15" value={level.brush.bank} onchange={(e) => (level.brush = { ...level.brush, bank: Math.max(0, Math.min(15, Number(e.currentTarget.value) | 0)) })} /></label>
        {/if}
      </div>
    </div>
    <p class="muted small">Click to paint, shift-drag for a rectangle, alt-click to pick a cell's tile. Tile 0 is blank.</p>
    <div class="sheet">
      <canvas bind:this={sheet} onclick={pick} style="width:{COLS * 8 * SCALE}px"></canvas>
      <div class="sel" style="left:{(level.brush.tile % COLS) * 8 * SCALE}px;top:{Math.floor(level.brush.tile / COLS) * 8 * SCALE}px;width:{8 * SCALE}px;height:{8 * SCALE}px"></div>
    </div>
  {/if}
</div>

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
    padding: 0.5rem;
    min-height: 0;
  }
  .row {
    display: grid;
    grid-template-columns: 4rem 1fr;
    gap: 0.4rem;
    align-items: center;
    font-size: 0.85rem;
  }
  .brush {
    display: flex;
    gap: 0.6rem;
    align-items: center;
    font-size: 0.85rem;
  }
  .brush label {
    margin-right: 0.5rem;
  }
  .brush input[type="number"] {
    width: 3.5rem;
  }
  canvas.preview {
    width: 48px;
    height: 48px;
    image-rendering: pixelated;
    background: repeating-conic-gradient(#888 0 25%, #bbb 0 50%) 0 0 / 12px 12px;
    border: 1px solid var(--line);
  }
  .sheet {
    position: relative;
    overflow: auto;
    max-height: 55vh;
    background: repeating-conic-gradient(#888 0 25%, #bbb 0 50%) 0 0 / 16px 16px;
    align-self: flex-start;
  }
  .sheet canvas {
    display: block;
    image-rendering: pixelated;
    cursor: pointer;
  }
  .sel {
    position: absolute;
    outline: 2px solid #00e5ff;
    pointer-events: none;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
  .note {
    font-size: 0.8rem;
    background: var(--warning-bg);
    color: var(--warning);
    padding: 0.3rem;
    border-radius: 4px;
    margin: 0;
  }
  .error {
    color: var(--error);
  }
</style>

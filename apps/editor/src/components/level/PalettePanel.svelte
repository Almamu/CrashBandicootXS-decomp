<script lang="ts">
  // The room's palette: its 256 BG colours, which every layer picture carries
  // (docs/levels.md, "The room palette"). A colour is 5 bits a channel; bit 15
  // is kept as the ROM has it (docs/graphics.md, "Palettes"). A change redraws
  // the room at once and, saved, is written into every layer picture.
  import { channels, fromChannels, fromHex, hex } from "../../lib/level/palette.ts";
  import { level } from "../../lib/level/state.svelte.ts";

  let index = $state(0);
  const v = $derived(level.palette[index] ?? 0);
  const ch = $derived(channels(v));

  function setChannel(k: "r" | "g" | "b", s: string) {
    const n = Number(s);
    if (!Number.isInteger(n) || n < 0 || n > 31) return;
    const c = { ...ch, [k]: n };
    level.setPaletteEntry(index, fromChannels(c.r, c.g, c.b, c.bit15));
  }
</script>

<div class="panel">
  <div class="grid" role="listbox" aria-label="the room's palette">
    {#each level.palette as c, i (i)}
      <button class="sw" class:current={i === index} style="background:{hex(c)}" title="{i} (bank {i >> 4}, {i & 15}): {c.toString(16).toUpperCase().padStart(4, '0')}"
        onclick={() => (index = i)} aria-label="colour {i}"></button>
    {/each}
  </div>
  <div class="edit">
    <div class="big" style="background:{hex(v)}"></div>
    <div>
      <div><strong>{index}</strong> <span class="muted">bank {index >> 4}, colour {index & 15} — <code>{v.toString(16).toUpperCase().padStart(4, "0")}</code></span></div>
      <div class="rgb">
        {#each ["r", "g", "b"] as const as k (k)}
          <label>{k.toUpperCase()} <input type="number" min="0" max="31" value={ch[k]} onchange={(e) => setChannel(k, e.currentTarget.value)} /></label>
        {/each}
        <input type="color" value={hex(v)} onchange={(e) => level.setPaletteEntry(index, fromHex(e.currentTarget.value))} />
      </div>
    </div>
  </div>
  <p class="muted small">BG0 draws with all 256 (8bpp); a BG1-3 cell with one bank of 16 (its colour 0 is transparent, entry 0 is the backdrop). Rooms that share a tile set may have different palettes: the pictures keep indices, not colours.</p>
</div>

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
    padding: 0.5rem;
  }
  .grid {
    display: grid;
    grid-template-columns: repeat(16, 1fr);
    gap: 1px;
    max-width: 20rem;
  }
  .sw {
    aspect-ratio: 1;
    padding: 0;
    border: 1px solid rgb(0 0 0 / 25%);
    border-radius: 0;
    min-width: 0;
  }
  .sw.current {
    outline: 2px solid #00e5ff;
    z-index: 1;
  }
  .edit {
    display: flex;
    gap: 0.6rem;
    align-items: center;
    font-size: 0.85rem;
  }
  .big {
    width: 3rem;
    height: 3rem;
    border: 1px solid var(--line);
  }
  .rgb {
    display: flex;
    gap: 0.4rem;
    align-items: center;
    margin-top: 0.3rem;
  }
  .rgb input[type="number"] {
    width: 3.2rem;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
</style>

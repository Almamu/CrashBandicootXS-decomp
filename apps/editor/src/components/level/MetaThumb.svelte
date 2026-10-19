<script lang="ts">
  // A meta-tile drawn in the room's palette (stamped as asked: flipped,
  // its banks shifted), transparent where the layer is.
  import { pixelTable } from "../../lib/level/palette.ts";
  import { transparentIndex } from "../../lib/level/render.ts";
  import { stampBlocks, type MetaTile, type StampOptions } from "../../lib/level/metatiles.ts";

  let { meta, bpp, palette, options = { flip: 0, shift: 0, skipBlank: false }, max = 96 }: { meta: MetaTile; bpp: 4 | 8; palette: number[]; options?: StampOptions; max?: number } = $props();

  let canvas = $state<HTMLCanvasElement>();
  const scale = $derived(Math.max(1, Math.min(4, Math.floor(max / (8 * Math.max(meta.w, meta.h))))));

  $effect(() => {
    const c = canvas;
    if (!c) return;
    c.width = meta.w * 8;
    c.height = meta.h * 8;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    const img = ctx.createImageData(c.width, c.height);
    const px = new Uint32Array(img.data.buffer);
    const table = pixelTable(palette, transparentIndex(bpp === 8));
    const shift = options.shift;
    for (const s of stampBlocks(meta, { ...options, shift: bpp === 4 && meta.cells.some((x) => x.bank + shift < 0 || x.bank + shift > 15) ? 0 : shift }, bpp)) {
      for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x++) px[(s.y * 8 + y) * c.width + s.x * 8 + x] = table[s.block[y * 8 + x]!]!;
    }
    ctx.putImageData(img, 0, 0);
  });
</script>

<canvas bind:this={canvas} style="width:{meta.w * 8 * scale}px;height:{meta.h * 8 * scale}px" title="{meta.name}: {meta.w}x{meta.h} cells"></canvas>

<style>
  canvas {
    display: block;
    image-rendering: pixelated;
    background: repeating-conic-gradient(#888 0 25%, #bbb 0 50%) 0 0 / 12px 12px;
    border: 1px solid var(--line);
  }
</style>

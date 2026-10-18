<script lang="ts">
  // A 240x160 screen as the GBA shows it (lib/cutscene/render.ts): a picture
  // and the text's glyphs, drawn `width` pixels wide (pixelated).
  import type { CutsceneFont } from "@crash/schema-types";
  import type { PictureEdit } from "../../lib/cutscene/model.ts";
  import { fromBase64 } from "../../lib/cutscene/model.ts";
  import { renderScreen, SCREEN_H, SCREEN_W } from "../../lib/cutscene/render.ts";
  import type { Glyph } from "../../lib/cutscene/text.ts";

  let { picture, font, glyphs = [], width = SCREEN_W, label = "the screen" }: { picture: PictureEdit | null; font: CutsceneFont; glyphs?: Glyph[]; width?: number; label?: string } = $props();

  let canvas = $state<HTMLCanvasElement>();
  const glyphPixels = $derived(fromBase64(font.glyphs));

  $effect(() => {
    const c = canvas;
    if (!c) return;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    const img = ctx.createImageData(SCREEN_W, SCREEN_H);
    const out = new Uint32Array(img.data.buffer);
    renderScreen(out, picture, { glyphs: glyphPixels, cell: font.cell, shapes: font.shapes, palette: font.palette }, glyphs);
    ctx.putImageData(img, 0, 0);
  });
</script>

<canvas bind:this={canvas} width={SCREEN_W} height={SCREEN_H} style:width="{width}px" style:height="{(width * SCREEN_H) / SCREEN_W}px" aria-label={label} data-screen></canvas>

<style>
  canvas {
    image-rendering: pixelated;
    display: block;
    background: #000;
  }
</style>

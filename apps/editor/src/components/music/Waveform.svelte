<script lang="ts">
  // A sample's waveform: each column the lowest and highest of its samples.
  let { data, height = 120, label = "the sample's waveform" }: { data: Int8Array | null; height?: number; label?: string } = $props();
  let canvas = $state<HTMLCanvasElement>();
  let width = $state(600);

  $effect(() => {
    const c = canvas;
    if (!c) return;
    const ctx = c.getContext("2d");
    if (!ctx) return;
    c.width = width;
    c.height = height;
    const style = getComputedStyle(c);
    ctx.fillStyle = style.getPropertyValue("--panel") || "#eee";
    ctx.fillRect(0, 0, width, height);
    ctx.strokeStyle = style.getPropertyValue("--line") || "#ccc";
    ctx.beginPath();
    ctx.moveTo(0, height / 2);
    ctx.lineTo(width, height / 2);
    ctx.stroke();
    if (!data || !data.length) return;
    ctx.fillStyle = style.getPropertyValue("--accent") || "#b5600f";
    const per = data.length / width;
    for (let x = 0; x < width; x++) {
      const a = Math.floor(x * per);
      const b = Math.max(a + 1, Math.floor((x + 1) * per));
      let lo = 127;
      let hi = -128;
      for (let i = a; i < b && i < data.length; i++) {
        const v = data[i]!;
        if (v < lo) lo = v;
        if (v > hi) hi = v;
      }
      const y0 = height / 2 - (hi / 128) * (height / 2);
      const y1 = height / 2 - (lo / 128) * (height / 2);
      ctx.fillRect(x, y0, 1, Math.max(1, y1 - y0));
    }
  });
</script>

<div class="wave" bind:clientWidth={width}>
  <canvas bind:this={canvas} style="height: {height}px" aria-label={label}></canvas>
</div>

<style>
  .wave {
    width: 100%;
  }
  canvas {
    width: 100%;
    display: block;
    border: 1px solid var(--line);
    border-radius: 3px;
  }
</style>

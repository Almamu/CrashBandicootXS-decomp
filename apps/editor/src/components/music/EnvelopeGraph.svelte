<script lang="ts">
  // An instrument's volume envelope (GaxEnvelope; an XM instrument's,
  // volume 0-64, with top 64): its points, volume 0-255 over ticks, the
  // sustain point and the loop marked. A point is
  // dragged (the first stays at tick 0, the others between their
  // neighbours), a click on the line adds one, a double click on a point
  // takes it out; each change is given to `onchange` when the drag ends.
  import * as M from "../../lib/music/model.ts";

  let {
    points,
    sustain,
    loopStart,
    loopEnd,
    onchange,
    height = 180,
    top = 255,
    scale = "GAX2's",
  }: {
    points: M.EnvPoint[];
    sustain: number | null;
    loopStart: number | null;
    loopEnd: number | null;
    onchange: (points: M.EnvPoint[], label: string) => void;
    height?: number;
    /** the highest volume, and whose scale it is */
    top?: number;
    scale?: string;
  } = $props();

  let width = $state(600);
  let drag = $state<{ i: number; pts: M.EnvPoint[] } | null>(null);
  let svg = $state<SVGSVGElement>();

  const shown = $derived(drag ? drag.pts : points);
  const maxX = $derived(Math.max(16, ...shown.map((p) => p.x)) * 1.1);
  const pad = 8;
  const sx = (x: number) => pad + (x / maxX) * (width - 2 * pad);
  const sy = (y: number) => height - pad - (y / top) * (height - 2 * pad);
  const path = $derived(shown.map((p, i) => `${i ? "L" : "M"}${sx(p.x).toFixed(1)},${sy(p.y).toFixed(1)}`).join(" ") + (shown.length ? ` L${sx(maxX).toFixed(1)},${sy(shown[shown.length - 1]!.y).toFixed(1)}` : ""));

  function at(e: PointerEvent): M.EnvPoint {
    const r = svg!.getBoundingClientRect();
    const x = Math.round(((e.clientX - r.left - pad) / (width - 2 * pad)) * maxX);
    const y = Math.round(((height - pad - (e.clientY - r.top)) / (height - 2 * pad)) * top);
    return { x: Math.max(0, x), y: Math.max(0, Math.min(top, y)) };
  }

  function down(e: PointerEvent, i: number) {
    e.stopPropagation();
    (e.currentTarget as Element).setPointerCapture(e.pointerId);
    drag = { i, pts: points.map((p) => ({ ...p })) };
  }

  function move(e: PointerEvent) {
    if (!drag) return;
    const p = at(e);
    const pts = drag.pts.map((q) => ({ ...q }));
    const i = drag.i;
    const lo = i === 0 ? 0 : pts[i - 1]!.x + 1;
    const hi = i === 0 ? 0 : i + 1 < pts.length ? pts[i + 1]!.x - 1 : 0xffff;
    pts[i] = { x: Math.max(lo, Math.min(hi, p.x)), y: p.y };
    drag = { i, pts };
  }

  function up() {
    if (!drag) return;
    const d = drag;
    drag = null;
    if (d.pts.some((p, k) => p.x !== points[k]!.x || p.y !== points[k]!.y)) onchange(d.pts, `envelope point ${d.i}`);
  }

  function add(e: PointerEvent) {
    if (drag || e.button !== 0) return;
    const p = at(e);
    if (points.some((q) => q.x === p.x)) return;
    const pts = [...points, p].sort((a, b) => a.x - b.x);
    onchange(pts, `envelope point at tick ${p.x}`);
  }

  function remove(i: number) {
    if (i === 0 || points.length <= 1) return;
    onchange(
      points.filter((_, k) => k !== i),
      `envelope point ${i} out`,
    );
  }
</script>

<div class="env" bind:clientWidth={width}>
  <svg bind:this={svg} {width} {height} role="img" aria-label="the volume envelope" onpointerdown={add} onpointermove={move} onpointerup={up}>
    <rect x="0" y="0" {width} {height} class="bg" />
    {#each [top / 4, top / 2, (top * 3) / 4] as v (v)}<line x1={pad} x2={width - pad} y1={sy(v)} y2={sy(v)} class="grid" />{/each}
    {#if loopStart !== null && loopEnd !== null && shown[loopStart] && shown[loopEnd]}
      <rect x={sx(shown[loopStart]!.x)} y={pad} width={Math.max(2, sx(shown[loopEnd]!.x) - sx(shown[loopStart]!.x))} height={height - 2 * pad} class="loop" />
    {/if}
    {#if sustain !== null && shown[sustain]}
      <line x1={sx(shown[sustain]!.x)} x2={sx(shown[sustain]!.x)} y1={pad} y2={height - pad} class="sustain" />
    {/if}
    <path d={path} class="line" />
    {#each shown as p, i (i)}
      <circle cx={sx(p.x)} cy={sy(p.y)} r="5" class="pt" class:on={drag?.i === i} role="button" tabindex="-1" aria-label="envelope point {i}" onpointerdown={(e) => down(e, i)} ondblclick={() => remove(i)}>
        <title>point {i}: tick {p.x}, volume {p.y}{i === sustain ? " (sustain)" : ""}{i === loopStart ? " (loop start)" : ""}{i === loopEnd ? " (loop end)" : ""}</title>
      </circle>
    {/each}
  </svg>
  <p class="hint">Drag a point; click the graph to add one, double-click a point to take it out. Volume 0-{top} ({scale}), ticks of 1/60 s; {#if sustain !== null}the dashed line is the sustain point (held until the key off){:else}no sustain point: a key off cuts the note{/if}{#if loopStart !== null}, the band the loop{/if}.</p>
</div>

<style>
  .env {
    width: 100%;
  }
  svg {
    display: block;
    touch-action: none;
    cursor: crosshair;
  }
  .bg {
    fill: var(--panel);
    stroke: var(--line);
  }
  .grid {
    stroke: var(--line);
    stroke-dasharray: 2 4;
  }
  .loop {
    fill: color-mix(in srgb, var(--accent) 15%, transparent);
  }
  .sustain {
    stroke: var(--accent);
    stroke-dasharray: 5 3;
  }
  .line {
    fill: none;
    stroke: var(--accent);
    stroke-width: 2;
  }
  .pt {
    fill: var(--bg);
    stroke: var(--accent);
    stroke-width: 2;
    cursor: grab;
  }
  .pt.on {
    fill: var(--accent);
  }
  .hint {
    color: var(--muted);
    font-size: 0.75rem;
    margin: 0.2rem 0;
  }
</style>

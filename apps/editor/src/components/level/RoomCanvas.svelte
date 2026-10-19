<script lang="ts">
  // The room, drawn as the game draws it: the BG layers back to front by
  // priority, each scrolled by its parallax factor for the camera (the
  // 240x160 screen at the view's centre), the collision layer over them,
  // the entities' sprites (a labelled box where none is known) and their
  // links. Zoom with the wheel, pan with the middle or right button (or
  // space and drag); the tools edit (docs/editors.md, "The level editor").
  import { onMount } from "svelte";
  import { EditorClient } from "@crash/schema-types/client";
  import { assetUrl } from "../../lib/api.ts";
  import { editor } from "../../lib/state.svelte.ts";
  import {
    CELL,
    COLUMN,
    SCREEN_H,
    SCREEN_W,
    cameraFor,
    cellAt,
    fit,
    layerOrigin,
    lineCells,
    rectCells,
    toScreen,
    toWorld,
    zoomAt,
    type Point,
    type View,
  } from "../../lib/level/coords.ts";
  import { pixelOf, pixelTable, toRgb } from "../../lib/level/palette.ts";
  import type { CellChange } from "../../lib/level/paint.ts";
  import {
    BLEND_FIRST,
    BLEND_SECOND,
    TiledBitmap,
    blends,
    cellFill,
    compositeBlend,
    indexFill,
    transparentIndex,
    type Blend,
    type BlendLayer,
  } from "../../lib/level/render.ts";
  import { BG_NAMES, level, type BgName, type LayerName } from "../../lib/level/state.svelte.ts";

  let { onhover, oncamera }: { onhover?: (p: { world: Point; cell: Point | null } | null) => void; oncamera?: (p: Point) => void } = $props();

  let host: HTMLDivElement;
  let canvas: HTMLCanvasElement;
  let cw = $state(800);
  let ch = $state(500);
  let view = $state<View>({ zoom: 1, x: 0, y: 0 });
  let frame = 0;

  // -- what is drawn ------------------------------------------------------------

  let bitmaps: Partial<Record<LayerName, TiledBitmap>> = {};
  let tables: Partial<Record<BgName, Uint32Array>> = {};
  let sprites: Record<string, HTMLImageElement> = {};
  let builtFor: unknown = null;

  function build() {
    const v = level.view;
    bitmaps = {};
    tables = {};
    builtFor = v;
    if (!v) return;
    for (const n of BG_NAMES) {
      const l = level.layers[n];
      if (!l) continue;
      tables[n] = pixelTable(level.palette, transparentIndex(n === "bg0"));
      bitmaps[n] = new TiledBitmap(l.width, l.height, indexFill(l.data, l.width, () => tables[n]!));
    }
    const c = level.collision;
    if (c) {
      const colours = new Map(level.legend.map((e) => [e.value, e.value === 0 ? 0 : pixelOf(e.colour)]));
      bitmaps.collision = new TiledBitmap(c.cols, c.rows, cellFill(c.cells, c.cols, (x) => colours.get(x) ?? pixelOf("#ff00ff")), CELL);
    }
    sprites = {};
  }

  let paletteSeen: number[] = [];
  function syncPalette() {
    if (paletteSeen === level.palette) return;
    paletteSeen = level.palette;
    for (const n of BG_NAMES) {
      if (!bitmaps[n]) continue;
      tables[n] = pixelTable(level.palette, transparentIndex(n === "bg0"));
      bitmaps[n]!.invalidateAll();
    }
  }

  function takeDamage() {
    for (const d of level.damage.splice(0)) {
      const b = bitmaps[d.layer];
      if (!b) continue;
      const cols = d.layer === "collision" ? level.collision!.cols : level.layers[d.layer]!.width / CELL;
      const cx = d.index % cols;
      const cy = Math.floor(d.index / cols);
      if (d.layer === "collision") b.invalidate(cx, cy, 1, 1);
      else b.invalidate(cx * CELL, cy * CELL, CELL, CELL);
    }
  }

  function sprite(type: number): HTMLImageElement | null {
    const s = level.view?.sprites[String(type)];
    if (!s) return null;
    const key = `${s.bank}/${s.frame}/${s.palette}`;
    let img = sprites[key];
    if (!img) {
      img = new Image();
      img.onload = () => redraw();
      img.src = assetUrl(new EditorClient().spriteUrl(editor.projectId, s.bank, s.frame, s.palette));
      sprites[key] = img;
    }
    return img.complete && img.naturalWidth ? img : null;
  }

  function typeNumber(t: string | number): number {
    return typeof t === "number" ? t : (level.typeOf(t)?.type ?? -1);
  }

  /** An entity's box in the world: its sprite's, or 16x16 about its point. */
  function entityBox(e: { type: string | number; x: number; y: number }): { x: number; y: number; w: number; h: number } {
    const s = level.view?.sprites[String(typeNumber(e.type))];
    if (s) return { x: e.x - s.origin[0], y: e.y - s.origin[1], w: s.size[0], h: s.size[1] };
    return { x: e.x - 8, y: e.y - 8, w: 16, h: 16 };
  }

  // -- the room's numbers ---------------------------------------------------------

  const roomSize = $derived.by(() => {
    const c = level.view?.layers.collision;
    if (!c) return { w: 240, h: 160, pw: 240, ph: 160 };
    return { w: c.sizeTiles[0] * CELL, h: c.sizeTiles[1] * CELL, pw: c.width, ph: c.height };
  });

  function camera(): Point {
    return cameraFor(view, cw, ch, roomSize.w, roomSize.h);
  }

  function origin(n: BgName, cam: Point): Point {
    const info = level.view?.layers[n];
    return info ? layerOrigin(cam, info.scale, level.parallax) : { x: 0, y: 0 };
  }

  // -- drawing ------------------------------------------------------------------------

  function redraw() {
    if (frame) return;
    frame = requestAnimationFrame(draw);
  }

  function draw() {
    frame = 0;
    const ctx = canvas?.getContext("2d");
    if (!ctx) return;
    if (builtFor !== level.view) build();
    syncPalette();
    takeDamage();
    ctx.setTransform(1, 0, 0, 1, 0, 0);
    ctx.imageSmoothingEnabled = false;
    ctx.fillStyle = "#3a3a42";
    ctx.fillRect(0, 0, cw, ch);
    const v = level.view;
    if (!v) return;
    const z = view.zoom;
    const tl = toScreen(view, 0, 0);
    // the backdrop: palette entry 0, behind every layer
    const [r, g, b] = toRgb(level.palette[0] ?? 0);
    ctx.fillStyle = `rgb(${r},${g},${b})`;
    ctx.fillRect(tl.x, tl.y, roomSize.pw * z, roomSize.ph * z);
    const cam = camera();
    oncamera?.(cam);
    // back to front: priority 3 first; at one priority the lower BG is in front
    const order = BG_NAMES.filter((n) => v.layers[n] && level.show[n]).sort(
      (a, b2) => v.layers[b2]!.priority - v.layers[a]!.priority || BG_NAMES.indexOf(b2) - BG_NAMES.indexOf(a),
    );
    const rb = level.room?.record?.blend;
    const blend: Blend = { effect: rb?.effect ?? 0, eva: rb?.eva ?? 0, evb: rb?.evb ?? 0 };
    if (level.show.blend && blends(blend)) drawBlended(ctx, order, cam, blend);
    else for (const n of order) bitmaps[n]?.draw(ctx, view, cw, ch, origin(n, cam));
    if (level.show.collision && bitmaps.collision) {
      ctx.globalAlpha = level.collisionOpacity;
      bitmaps.collision.draw(ctx, view, cw, ch, { x: 0, y: 0 });
      ctx.globalAlpha = 1;
    }
    // the scrollable size
    ctx.strokeStyle = "rgba(255,255,255,0.5)";
    ctx.lineWidth = 1;
    ctx.strokeRect(tl.x + 0.5, tl.y + 0.5, roomSize.w * z, roomSize.h * z);
    if (level.show.columns) drawColumns(ctx);
    if (level.show.screens) drawScreens(ctx);
    if (level.show.camera) {
      const s = toScreen(view, cam.x, cam.y);
      ctx.strokeStyle = "#ffd54f";
      ctx.setLineDash([6, 4]);
      ctx.strokeRect(s.x + 0.5, s.y + 0.5, SCREEN_W * z, SCREEN_H * z);
      ctx.setLineDash([]);
    }
    if (level.show.links) drawLinks(ctx);
    if (level.show.entities) drawEntities(ctx);
    drawCursor(ctx, cam);
  }

  let blendCanvas: HTMLCanvasElement | null = null;

  /**
   * The BG layers with the room's alpha blend (SetupRoomBlend: BG3 over
   * BG0-2), composited pixel by pixel for the part of the room in view, a
   * world pixel an output pixel (zoomed out, a canvas pixel one), then drawn
   * scaled like the layers' tiles.
   */
  function drawBlended(ctx: CanvasRenderingContext2D, order: BgName[], cam: Point, blend: Blend) {
    const step = view.zoom >= 1 ? 1 : 1 / view.zoom;
    const x0 = Math.max(0, Math.floor(view.x));
    const y0 = Math.max(0, Math.floor(view.y));
    const x1 = Math.min(roomSize.pw, view.x + cw / view.zoom);
    const y1 = Math.min(roomSize.ph, view.y + ch / view.zoom);
    const w = Math.ceil((x1 - x0) / step);
    const h = Math.ceil((y1 - y0) / step);
    if (w <= 0 || h <= 0) return;
    const layers: BlendLayer[] = [];
    for (const n of [...order].reverse()) {
      const l = level.layers[n];
      if (!l || !tables[n]) continue;
      layers.push({ data: l.data, width: l.width, height: l.height, origin: origin(n, cam), table: tables[n]!, first: !!BLEND_FIRST[n], second: !!BLEND_SECOND[n] });
    }
    blendCanvas ??= document.createElement("canvas");
    if (blendCanvas.width !== w || blendCanvas.height !== h) {
      blendCanvas.width = w;
      blendCanvas.height = h;
    }
    const bctx = blendCanvas.getContext("2d");
    if (!bctx) return;
    const img = bctx.createImageData(w, h);
    const backdrop = pixelTable([level.palette[0] ?? 0])[0]!;
    compositeBlend(layers, backdrop, blend, roomSize.pw, roomSize.ph, x0, y0, step, w, h, new Uint32Array(img.data.buffer));
    bctx.putImageData(img, 0, 0);
    const s = toScreen(view, x0, y0);
    ctx.drawImage(blendCanvas, s.x, s.y, w * step * view.zoom, h * step * view.zoom);
  }

  function drawColumns(ctx: CanvasRenderingContext2D) {
    ctx.strokeStyle = "rgba(120,200,255,0.55)";
    ctx.fillStyle = "rgba(120,200,255,0.9)";
    ctx.font = "11px system-ui, sans-serif";
    for (let x = 0, k = 0; x <= roomSize.pw; x += COLUMN, k++) {
      const s = toScreen(view, x, 0);
      ctx.beginPath();
      ctx.moveTo(Math.round(s.x) + 0.5, s.y);
      ctx.lineTo(Math.round(s.x) + 0.5, s.y + roomSize.ph * view.zoom);
      ctx.stroke();
      if (view.zoom >= 0.25) ctx.fillText(`group ${k}`, s.x + 3, Math.max(12, s.y + 12));
    }
  }

  function drawScreens(ctx: CanvasRenderingContext2D) {
    ctx.strokeStyle = "rgba(255,255,255,0.18)";
    ctx.beginPath();
    for (let x = 0; x <= roomSize.w; x += SCREEN_W) {
      const s = toScreen(view, x, 0);
      ctx.moveTo(Math.round(s.x) + 0.5, s.y);
      ctx.lineTo(Math.round(s.x) + 0.5, s.y + roomSize.h * view.zoom);
    }
    for (let y = 0; y <= roomSize.h; y += SCREEN_H) {
      const s = toScreen(view, 0, y);
      ctx.moveTo(s.x, Math.round(s.y) + 0.5);
      ctx.lineTo(s.x + roomSize.w * view.zoom, Math.round(s.y) + 0.5);
    }
    ctx.stroke();
  }

  function shown(i: number) {
    const e = level.room!.entities[i]!;
    if (drag && drag.kind === "entity" && drag.id === i) return { ...e, x: drag.x, y: drag.y };
    return e;
  }

  function drawLinks(ctx: CanvasRenderingContext2D) {
    const room = level.room;
    if (!room?.links) return;
    ctx.strokeStyle = "#ff5ab4";
    ctx.fillStyle = "#ff5ab4";
    ctx.lineWidth = 2;
    for (const [a, b] of room.links) {
      if (!room.entities[a] || !room.entities[b]) continue;
      const p = toScreen(view, shown(a).x, shown(a).y);
      const q = toScreen(view, shown(b).x, shown(b).y);
      ctx.beginPath();
      ctx.moveTo(p.x, p.y);
      ctx.lineTo(q.x, q.y);
      ctx.stroke();
      const ang = Math.atan2(q.y - p.y, q.x - p.x);
      ctx.beginPath();
      ctx.moveTo(q.x, q.y);
      ctx.lineTo(q.x - 8 * Math.cos(ang - 0.4), q.y - 8 * Math.sin(ang - 0.4));
      ctx.lineTo(q.x - 8 * Math.cos(ang + 0.4), q.y - 8 * Math.sin(ang + 0.4));
      ctx.fill();
    }
    ctx.lineWidth = 1;
  }

  function drawEntities(ctx: CanvasRenderingContext2D) {
    const room = level.room;
    if (!room) return;
    ctx.font = "11px system-ui, sans-serif";
    room.entities.forEach((_, i) => {
      const e = shown(i);
      const box = entityBox(e);
      const s = toScreen(view, box.x, box.y);
      const img = sprite(typeNumber(e.type));
      const sel = level.selected === i || level.linkFrom === i;
      if (img) {
        ctx.drawImage(img, s.x, s.y, box.w * view.zoom, box.h * view.zoom);
      } else {
        ctx.fillStyle = "rgba(255,140,0,0.35)";
        ctx.fillRect(s.x, s.y, box.w * view.zoom, box.h * view.zoom);
        ctx.strokeStyle = "#ff8c00";
        ctx.strokeRect(s.x + 0.5, s.y + 0.5, box.w * view.zoom, box.h * view.zoom);
      }
      if (sel) {
        ctx.strokeStyle = level.linkFrom === i ? "#ff5ab4" : "#00e5ff";
        ctx.lineWidth = 2;
        ctx.strokeRect(s.x - 1, s.y - 1, box.w * view.zoom + 2, box.h * view.zoom + 2);
        ctx.lineWidth = 1;
      }
      if (level.show.labels || (!img && view.zoom >= 0.5) || sel) {
        const label = `${i} ${typeof e.type === "string" ? e.type : `type ${e.type}`}`;
        ctx.fillStyle = "rgba(0,0,0,0.65)";
        const w = ctx.measureText(label).width + 6;
        ctx.fillRect(s.x, s.y - 14, w, 14);
        ctx.fillStyle = "#fff";
        ctx.fillText(label, s.x + 3, s.y - 3);
      }
      // the entity's point
      const p = toScreen(view, e.x, e.y);
      ctx.fillStyle = sel ? "#00e5ff" : "#fff";
      ctx.fillRect(p.x - 1.5, p.y - 1.5, 3, 3);
    });
  }

  let hoverCell = $state<Point | null>(null);

  function paintTarget(cam: Point): { origin: Point; cols: number; rows: number } | null {
    if (level.tool === "collision" && level.collision) return { origin: { x: 0, y: 0 }, cols: level.collision.cols, rows: level.collision.rows };
    if (level.tool === "tile") {
      const l = level.layers[level.paintLayer];
      if (l) return { origin: origin(level.paintLayer, cam), cols: l.width / CELL, rows: l.height / CELL };
    }
    return null;
  }

  function drawCursor(ctx: CanvasRenderingContext2D, cam: Point) {
    const t = paintTarget(cam);
    if (!t) return;
    const cells: Point[] = drag?.kind === "rect" ? rectCells(drag.from, drag.to) : hoverCell ? [hoverCell] : [];
    if (!cells.length) return;
    const xs = cells.map((c) => c.x);
    const ys = cells.map((c) => c.y);
    const a = toScreen(view, t.origin.x + Math.min(...xs) * CELL, t.origin.y + Math.min(...ys) * CELL);
    const w = (Math.max(...xs) - Math.min(...xs) + 1) * CELL * view.zoom;
    const h = (Math.max(...ys) - Math.min(...ys) + 1) * CELL * view.zoom;
    ctx.strokeStyle = "#00e5ff";
    ctx.lineWidth = 2;
    ctx.strokeRect(a.x, a.y, w, h);
    ctx.lineWidth = 1;
  }

  // -- the mouse ------------------------------------------------------------------------

  type Drag =
    | { kind: "pan"; sx: number; sy: number; vx: number; vy: number }
    | { kind: "entity"; id: number; dx: number; dy: number; x: number; y: number; moved: boolean }
    | { kind: "stroke"; last: Point; tiles: CellChange<Uint8Array>[]; values: CellChange<number>[]; layer: BgName }
    | { kind: "rect"; from: Point; to: Point };
  let drag: Drag | null = null;
  let space = false;

  function local(e: MouseEvent): Point {
    const r = canvas.getBoundingClientRect();
    return { x: e.clientX - r.left, y: e.clientY - r.top };
  }

  function hitEntity(w: Point): number | null {
    const room = level.room;
    if (!room || !level.show.entities) return null;
    for (let i = room.entities.length - 1; i >= 0; i--) {
      const b = entityBox(room.entities[i]!);
      if (w.x >= b.x && w.x < b.x + b.w && w.y >= b.y && w.y < b.y + b.h) return i;
    }
    return null;
  }

  function cellUnder(w: Point): Point | null {
    const t = paintTarget(camera());
    return t ? cellAt(w, t.origin, t.cols, t.rows) : null;
  }

  function paintAt(cells: Point[], d: Extract<Drag, { kind: "stroke" }>) {
    if (level.tool === "tile") d.tiles.push(...level.paintTiles(cells));
    else d.values.push(...level.paintCollision(cells));
  }

  function onpointerdown(e: PointerEvent) {
    if (!level.view) return;
    canvas.setPointerCapture(e.pointerId);
    const p = local(e);
    const w = toWorld(view, p.x, p.y);
    if (e.button === 1 || e.button === 2 || (e.button === 0 && space)) {
      drag = { kind: "pan", sx: p.x, sy: p.y, vx: view.x, vy: view.y };
      return;
    }
    if (e.button !== 0) return;
    const tool = level.tool;
    if (tool === "tile" || tool === "collision") {
      const c = cellUnder(w);
      if (!c) return;
      if (e.altKey) {
        if (tool === "tile") level.pickTile(c);
        else level.pickCollision(c);
        return;
      }
      if (tool === "tile" && !level.brushBlock()) {
        editor.error = "Pick a tile first (the tile set is still loading, or didn't load)";
        return;
      }
      if (e.shiftKey) drag = { kind: "rect", from: c, to: c };
      else {
        const d: Drag = { kind: "stroke", last: c, tiles: [], values: [], layer: level.paintLayer };
        drag = d;
        paintAt([c], d);
      }
      redraw();
      return;
    }
    const hit = hitEntity(w);
    if (tool === "add") {
      level.addEntity(level.addType, w.x, w.y);
      return;
    }
    if (tool === "link") {
      if (hit === null) level.linkFrom = null;
      else if (level.linkFrom === null) level.linkFrom = hit;
      else {
        level.addLink(level.linkFrom, hit);
        level.linkFrom = null;
      }
      redraw();
      return;
    }
    // select
    level.selected = hit;
    if (hit !== null) {
      const ent = level.room!.entities[hit]!;
      drag = { kind: "entity", id: hit, dx: w.x - ent.x, dy: w.y - ent.y, x: ent.x, y: ent.y, moved: false };
    } else drag = { kind: "pan", sx: p.x, sy: p.y, vx: view.x, vy: view.y };
    redraw();
  }

  function onpointermove(e: PointerEvent) {
    const p = local(e);
    const w = toWorld(view, p.x, p.y);
    const c = level.view ? cellUnder(w) : null;
    hoverCell = c;
    onhover?.({ world: { x: Math.floor(w.x), y: Math.floor(w.y) }, cell: c });
    if (!drag) {
      if (level.tool === "tile" || level.tool === "collision") redraw();
      return;
    }
    if (drag.kind === "pan") {
      view = { ...view, x: drag.vx - (p.x - drag.sx) / view.zoom, y: drag.vy - (p.y - drag.sy) / view.zoom };
    } else if (drag.kind === "entity") {
      let x = Math.max(0, Math.round(w.x - drag.dx));
      let y = Math.max(0, Math.round(w.y - drag.dy));
      if (e.shiftKey) {
        x = Math.round(x / CELL) * CELL;
        y = Math.round(y / CELL) * CELL;
      }
      drag.moved ||= x !== drag.x || y !== drag.y;
      drag.x = x;
      drag.y = y;
    } else if (drag.kind === "stroke" && c) {
      if (c.x !== drag.last.x || c.y !== drag.last.y) paintAt(lineCells(drag.last, c), drag);
      drag.last = c;
    } else if (drag.kind === "rect" && c) {
      drag.to = c;
    }
    redraw();
  }

  function onpointerup() {
    const d = drag;
    drag = null;
    if (!d) return;
    if (d.kind === "entity" && d.moved) level.moveEntity(d.id, d.x, d.y);
    else if (d.kind === "stroke") {
      if (level.tool === "tile") level.commitTiles(d.layer, d.tiles);
      else level.commitCollision(d.values);
    } else if (d.kind === "rect") {
      const s: Extract<Drag, { kind: "stroke" }> = { kind: "stroke", last: d.to, tiles: [], values: [], layer: level.paintLayer };
      paintAt(rectCells(d.from, d.to), s);
      if (level.tool === "tile") level.commitTiles(s.layer, s.tiles);
      else level.commitCollision(s.values);
    }
    redraw();
  }

  function onpointerleave() {
    hoverCell = null;
    onhover?.(null);
    redraw();
  }

  function onwheel(e: WheelEvent) {
    e.preventDefault();
    const p = local(e);
    view = zoomAt(view, p.x, p.y, Math.pow(2, -e.deltaY / (e.deltaMode === 1 ? 6 : 240)));
  }

  function onkeydown(e: KeyboardEvent) {
    if (e.key === " " && !(e.target instanceof HTMLInputElement || e.target instanceof HTMLTextAreaElement)) space = true;
  }
  function onkeyup(e: KeyboardEvent) {
    if (e.key === " ") space = false;
  }

  // -- the view's own controls ------------------------------------------------------------

  export function fitRoom() {
    view = fit(cw, ch, roomSize.pw, roomSize.ph);
  }

  export function zoomBy(f: number) {
    view = zoomAt(view, cw / 2, ch / 2, f);
  }

  /** Centres the view on a world point. */
  export function centreOn(x: number, y: number) {
    view = { ...view, x: x - cw / 2 / view.zoom, y: y - ch / 2 / view.zoom };
  }

  export function zoomLevel(): number {
    return view.zoom;
  }

  // a room just opened: show its start marker (or all of it)
  let openedPath: string | null = null;
  $effect(() => {
    const p = level.view?.path ?? null;
    if (p && p !== openedPath) {
      openedPath = p;
      const start = level.room?.entities.find((e) => typeof e.type === "string" && e.type.endsWith("START_MARKER"));
      view = { zoom: 2, x: 0, y: 0 };
      if (start) centreOn(start.x, start.y);
      else {
        // no start marker (N-Tranced's types are partly named): the room's left end, its height filling the view
        const zoom = Math.min(4, Math.max(1, 2 ** Math.floor(Math.log2(ch / roomSize.ph))));
        view = { zoom, x: 0, y: (roomSize.ph - ch / zoom) / 2 };
      }
    }
  });

  $effect(() => {
    // what the drawing reads
    void level.view;
    void level.room;
    void level.palette;
    void level.pixels;
    void level.selected;
    void level.linkFrom;
    void level.tool;
    void level.parallax;
    void level.collisionOpacity;
    void JSON.stringify(level.show);
    void view;
    void cw;
    void ch;
    redraw();
  });

  onMount(() => {
    const ro = new ResizeObserver(() => {
      const r = host.getBoundingClientRect();
      const dpr = 1;
      cw = Math.max(1, Math.floor(r.width));
      ch = Math.max(1, Math.floor(r.height));
      canvas.width = cw * dpr;
      canvas.height = ch * dpr;
      redraw();
    });
    ro.observe(host);
    return () => {
      ro.disconnect();
      if (frame) cancelAnimationFrame(frame);
    };
  });
</script>

<svelte:window {onkeydown} {onkeyup} />

<div class="host" bind:this={host} class:paint={level.tool === "tile" || level.tool === "collision"}>
  <canvas
    bind:this={canvas}
    {onpointerdown}
    {onpointermove}
    {onpointerup}
    {onpointerleave}
    {onwheel}
    oncontextmenu={(e) => e.preventDefault()}
    data-testid="room-canvas"
  ></canvas>
  {#if level.loading}<div class="loading">Opening {level.loading}…</div>{/if}
</div>

<style>
  .host {
    position: relative;
    width: 100%;
    height: 100%;
    overflow: hidden;
    background: #2a2a30;
  }
  canvas {
    display: block;
    cursor: default;
    touch-action: none;
  }
  .paint canvas {
    cursor: crosshair;
  }
  .loading {
    position: absolute;
    left: 0.75rem;
    top: 0.5rem;
    color: #fff;
    background: rgb(0 0 0 / 60%);
    padding: 0.2rem 0.5rem;
    border-radius: 4px;
  }
</style>

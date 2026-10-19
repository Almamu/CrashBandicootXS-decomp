<script lang="ts">
  // The Meta-tile tool (docs/editors.md, "Meta-tiles"): the paint layer's
  // tile set's meta-tiles, the one to stamp and how (flipped, its banks
  // shifted, its blank cells left out), a new one from the cells selected
  // (shift-drag), and the blocks that recur in the set's rooms, to accept.
  import type { MetatileCandidate } from "@crash/schema-types";
  import { fromCandidate, MAX_SIZE, tileNumbers } from "../../lib/level/metatiles.ts";
  import { BG_NAMES, level, type BgName } from "../../lib/level/state.svelte.ts";
  import MetaThumb from "./MetaThumb.svelte";

  const layers = $derived(BG_NAMES.filter((n) => level.view?.layers[n]));
  const setName = $derived(level.view?.layers[level.paintLayer]?.tileset ?? "");
  const set = $derived(level.tileSets[level.paintLayer]);
  const lib = $derived(level.metaLibs[setName]);
  const detection = $derived(level.detections[setName]);
  const sel = $derived(level.metaSelection);
  const tooBig = $derived(!!sel && (sel.w > MAX_SIZE || sel.h > MAX_SIZE));
  const here = $derived(`${level.view?.path.split("/").slice(-2).join("/")}/${level.paintLayer}`);

  let scope = $state<"set" | "room">("set");
  let newName = $state("");
  /** the candidates dismissed or accepted, by their index in the detection */
  let gone = $state<Set<number>>(new Set());
  let shown = $state<number | null>(null);

  $effect(() => {
    if (level.view && level.tool === "meta") {
      if (!level.tileSets[level.paintLayer]) void level.loadTiles(level.paintLayer);
      if (!level.metaLibs[setName]) void level.loadMetatiles(level.paintLayer);
    }
  });

  $effect(() => {
    // a new detection: every candidate offered again
    void detection;
    gone = new Set();
    shown = null;
    level.metaHighlight = [];
  });

  function setLayer(n: BgName) {
    level.paintLayer = n;
    level.metaPick = null;
    level.metaSelection = null;
    level.metaHighlight = [];
  }

  function makeFromSelection() {
    if (level.metaFromSelection(newName || `Block ${sel?.w}x${sel?.h}`)) newName = "";
  }

  function candidateName(c: MetatileCandidate, k: number): string {
    return `Detected ${c.size[0]}x${c.size[1]} #${k + 1}`;
  }

  function accept(c: MetatileCandidate, k: number) {
    if (level.acceptCandidate(c, candidateName(c, k))) gone = new Set([...gone, k]);
  }

  function acceptAll(cs: MetatileCandidate[]) {
    const take = cs.map((c, k) => ({ c, k })).filter(({ k }) => !gone.has(k));
    level.acceptCandidates(take.map(({ c, k }) => ({ c, name: candidateName(c, k) })));
    gone = new Set(cs.map((_, k) => k));
  }

  function show(c: MetatileCandidate, k: number) {
    if (shown === k) {
      shown = null;
      level.metaHighlight = [];
      return;
    }
    shown = k;
    level.metaHighlight = c.at.filter((a) => a[0] === here).map((a) => ({ x: a[1], y: a[2], w: c.size[0], h: c.size[1] }));
  }

  function roomsOf(c: MetatileCandidate): string {
    const rooms = new Set(Object.keys(c.rooms).map((r) => r.split("/").slice(0, 2).join("/")));
    return rooms.size === 1 ? [...rooms][0]! : `${rooms.size} rooms`;
  }
</script>

<div class="panel">
  <label class="row">
    <span>layer</span>
    <select value={level.paintLayer} onchange={(e) => setLayer(e.currentTarget.value as BgName)}>
      {#each layers as n (n)}<option value={n}>{n} ({level.view?.layers[n]?.tileset})</option>{/each}
    </select>
  </label>
  {#if !set || set === "loading" || !lib || lib === "loading"}
    <p class="muted">Loading {setName}'s tiles and meta-tiles…</p>
  {:else if typeof set === "string"}
    <p class="error">{set}</p>
  {:else if typeof lib === "string"}
    <p class="error">{lib}</p>
  {:else}
    <section>
      <h3>Stamp</h3>
      <div class="opts">
        <label><input type="checkbox" checked={!!(level.metaStamp.flip & 1)} onchange={(e) => (level.metaStamp = { ...level.metaStamp, flip: (level.metaStamp.flip & 2) | (e.currentTarget.checked ? 1 : 0) })} /> flip X</label>
        <label><input type="checkbox" checked={!!(level.metaStamp.flip & 2)} onchange={(e) => (level.metaStamp = { ...level.metaStamp, flip: (level.metaStamp.flip & 1) | (e.currentTarget.checked ? 2 : 0) })} /> flip Y</label>
        {#if lib.bpp === 4}
          <label title="every cell's palette bank moves by this much (0-15)">banks <input type="number" min="-15" max="15" value={level.metaStamp.shift} onchange={(e) => (level.metaStamp = { ...level.metaStamp, shift: Math.max(-15, Math.min(15, Number(e.currentTarget.value) | 0)) })} /></label>
        {/if}
        <label title="leave the cells under its blank cells as they are"><input type="checkbox" checked={level.metaStamp.skipBlank} onchange={(e) => (level.metaStamp = { ...level.metaStamp, skipBlank: e.currentTarget.checked })} /> keep under blank</label>
      </div>
      {#if level.metaBrush() && level.stampProblem()}<p class="error small">{level.stampProblem()}</p>{/if}
      <p class="muted small">Click to stamp the meta-tile picked (its top left at the cell), drag to stamp it again a whole meta-tile on; shift-drag selects cells to make one of.</p>
    </section>

    <section>
      <h3>{lib.tileset}'s meta-tiles <span class="muted">({lib.list.length})</span></h3>
      {#if !lib.list.length}<p class="muted small">None yet: make one of a selection, or accept the blocks detection finds. They are saved with the room, in {lib.path}.</p>{/if}
      <ul class="list" data-testid="metatiles">
        {#each lib.list as m, i (i)}
          {@const nums = tileNumbers(m, set.index, set.bpp)}
          <li class:on={level.metaPick === i}>
            <button class="thumb" onclick={() => (level.metaPick = level.metaPick === i ? null : i)} title="stamp this one">
              <MetaThumb meta={m} bpp={lib.bpp} palette={level.palette} options={level.metaPick === i ? level.metaStamp : undefined} />
            </button>
            <div class="meta">
              <input value={m.name} aria-label="name" onchange={(e) => level.renameMetaTile(i, e.currentTarget.value)} />
              <span class="muted small">{m.w}x{m.h}{#if nums.includes(-1)} · {nums.filter((n) => n < 0).length} tile(s) no room draws (a stamp adds them){/if}</span>
            </div>
            <button class="del" title="delete it" onclick={() => level.deleteMetaTile(i)}>×</button>
          </li>
        {/each}
      </ul>
    </section>

    <section>
      <h3>From the selection</h3>
      {#if sel}
        <div class="opts">
          <span>{sel.w}x{sel.h} cells at {sel.x}, {sel.y}</span>
          <input placeholder="Block {sel.w}x{sel.h}" bind:value={newName} aria-label="the new meta-tile's name" />
          <button disabled={tooBig} title={tooBig ? `a meta-tile is at most ${MAX_SIZE}x${MAX_SIZE}` : "a meta-tile of these cells: their tiles, flips and banks"} onclick={makeFromSelection}>New meta-tile</button>
          <button onclick={() => (level.metaSelection = null)}>Clear</button>
        </div>
      {:else}
        <p class="muted small">Shift-drag on the room to select cells.</p>
      {/if}
    </section>

    <section>
      <h3>Detect</h3>
      <div class="opts">
        <select bind:value={scope} aria-label="where to look">
          <option value="set">every room of {lib.tileset}</option>
          <option value="room">this room</option>
        </select>
        <button onclick={() => level.detect(scope)} title="the blocks that recur (flipped and in other banks too), biggest savings first; as saved">Find recurring blocks</button>
      </div>
      {#if typeof detection === "string"}<p class="error">{detection}</p>
      {:else if detection}
        <p class="muted small">{detection.candidates.length} block(s) recur in {detection.scope === "set" ? `${detection.tileset}'s rooms` : "this room"}, as saved{level.dirty ? " (not this room's changes)" : ""}.</p>
        {#if detection.candidates.length}<button onclick={() => acceptAll(detection.candidates)} disabled={gone.size === detection.candidates.length}>Accept all</button>{/if}
        <ul class="list" data-testid="candidates">
          {#each detection.candidates as c, k (k)}
            {#if !gone.has(k)}
              <li class:on={shown === k}>
                <div class="thumb"><MetaThumb meta={fromCandidate(c, set.tiles, candidateName(c, k))} bpp={detection.bpp} palette={level.palette} /></div>
                <div class="meta">
                  <span><strong>{c.size[0]}x{c.size[1]}</strong>, {c.count} times in {roomsOf(c)}</span>
                  <span class="muted small">{c.flipped ? `${c.flipped} flipped` : "none flipped"}{c.shifted ? `, ${c.shifted} in other banks` : ""}; {c.score} cells drawn by its repeats</span>
                  <span class="acts">
                    <button onclick={() => accept(c, k)}>Accept</button>
                    <button onclick={() => (gone = new Set([...gone, k]))}>Dismiss</button>
                    <button disabled={!c.at.some((a) => a[0] === here)} title="outline its repeats in this layer" onclick={() => show(c, k)}>{shown === k ? "Hide" : "Show"}</button>
                  </span>
                </div>
              </li>
            {/if}
          {/each}
        </ul>
      {/if}
    </section>
  {/if}
</div>

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
    padding: 0.5rem;
    font-size: 0.85rem;
  }
  .row {
    display: grid;
    grid-template-columns: 4rem 1fr;
    gap: 0.4rem;
    align-items: center;
  }
  h3 {
    font-size: 0.85rem;
    margin: 0.2rem 0;
  }
  .opts {
    display: flex;
    flex-wrap: wrap;
    gap: 0.4rem;
    align-items: center;
  }
  .opts input[type="number"] {
    width: 3.5rem;
  }
  .list {
    list-style: none;
    margin: 0;
    padding: 0;
    display: flex;
    flex-direction: column;
    gap: 0.3rem;
    max-height: 40vh;
    overflow: auto;
  }
  .list li {
    display: flex;
    gap: 0.4rem;
    align-items: flex-start;
    padding: 0.2rem;
    border: 1px solid transparent;
    border-radius: 4px;
  }
  .list li.on {
    border-color: var(--accent);
  }
  .thumb {
    padding: 0;
    border: none;
    background: none;
    flex: none;
  }
  .meta {
    display: flex;
    flex-direction: column;
    gap: 0.15rem;
    flex: 1;
    min-width: 0;
  }
  .acts {
    display: flex;
    gap: 0.25rem;
  }
  .del {
    flex: none;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
  .error {
    color: var(--error);
  }
</style>

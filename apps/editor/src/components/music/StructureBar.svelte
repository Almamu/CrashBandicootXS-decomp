<script lang="ts">
  // A song's structure (docs/editors.md, "The music editor"): the order
  // position shown (which pattern it plays, a new one before or after it,
  // moved, taken out, the loop point), the pattern it plays (new, a copy,
  // cleared, taken out, its rows, a row inserted or taken out at the cell
  // selected), the channels and the speed the song starts at. Each is a
  // step of the undo stack (model.ts's structure operations); the player
  // plays the song as the build will build it.
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";

  let { pattern }: { pattern: number } = $props();

  const sv = $derived(mu.songView);
  const p = $derived(sv?.p);
  const rows = $derived(p ? (p.rows[pattern] ?? 0) : 0);
  const patternCount = $derived(p ? p.rows.length : 0);
  const firstSpeed = $derived(sv?.structure ? M.firstRowSpeed(sv.structure) : null);
  const scaffold = $derived(!!p?.scaffold && !mu.state.structures[mu.song]);
  let open = $state(false);

  function op(label: string, f: (s: M.Structure) => M.Structure, key: string | null = null) {
    mu.structural(label, f, key);
  }

  function goTo(pos: number) {
    mu.order = Math.max(0, pos);
  }

  function insertAfter(pat: number) {
    const pos = mu.order + 1;
    op(`Insert order position ${pos}`, (s) => M.insertOrder(s, pos, pat));
    goTo(pos);
  }

  function newPattern() {
    const n = patternCount;
    op(`New pattern ${n}`, (s) => M.insertOrder(M.addPattern(s, rows || 64), mu.order + 1, n));
    goTo(mu.order + 1);
  }

  function duplicate() {
    const n = patternCount;
    op(`Copy pattern ${pattern} as ${n}`, (s) => M.insertOrder(M.duplicatePattern(s, pattern), mu.order + 1, n));
    goTo(mu.order + 1);
  }

  function removePattern() {
    const uses = p ? p.playOrder.filter((x) => x === pattern).length : 0;
    if (uses > 1 && !confirm(`Pattern ${pattern} plays at ${uses} order positions: take it and them out?`)) return;
    op(`Delete pattern ${pattern}`, (s) => M.deletePattern(s, pattern));
  }

  const num = (s: string) => (s.trim() === "" ? NaN : Number(s));
</script>

{#if p}
  <div class="bar" aria-label="the song's structure">
    <span class="group">
      <strong>Order {mu.order}</strong>
      <label title="the pattern this order position plays">
        plays
        <select value={pattern} onchange={(e) => op(`Order position ${mu.order} plays pattern ${e.currentTarget.value}`, (s) => M.setOrder(s, mu.order, Number(e.currentTarget.value)))}>
          {#each Array.from({ length: patternCount }, (_, i) => i) as i (i)}<option value={i}>pattern {i}</option>{/each}
        </select>
      </label>
      <button title="a new order position after this one, playing the same pattern" onclick={() => insertAfter(pattern)}>+ position</button>
      <button title="take this order position out" disabled={p.playOrder.length <= 1} onclick={() => op(`Remove order position ${mu.order}`, (s) => M.removeOrder(s, mu.order))}>− position</button>
      <button title="move it earlier" disabled={mu.order === 0} onclick={() => { op(`Move order position ${mu.order}`, (s) => M.moveOrder(s, mu.order, mu.order - 1)); goTo(mu.order - 1); }}>◀</button>
      <button title="move it later" disabled={mu.order >= p.playOrder.length - 1} onclick={() => { op(`Move order position ${mu.order}`, (s) => M.moveOrder(s, mu.order, mu.order + 1)); goTo(mu.order + 1); }}>▶</button>
      <button title="the song loops back to this position when it ends" disabled={p.loopOrder === mu.order} onclick={() => op(`Loop to order position ${mu.order}`, (s) => M.setRestart(s, mu.order))}>loop here</button>
    </span>
    <span class="group">
      <strong>Pattern {pattern}</strong>
      <label title="its rows: a GAX2 song has one pattern length (the longest); a shorter pattern ends with a pattern break (D00) the build puts on its last row">
        rows <input type="number" min="1" max={M.MAX_ROWS} value={rows} onchange={(e) => { const n = num(e.currentTarget.value); if (Number.isInteger(n)) op(`Pattern ${pattern} has ${n} rows`, (s) => M.setRows(s, pattern, n)); }} />
      </label>
      <button title="an empty row at the cell selected (the rows after it move down)" disabled={!mu.cell} onclick={() => op(`Insert row ${mu.cell!.row} in pattern ${pattern}`, (s) => M.insertRow(s, pattern, mu.cell!.row))}>+ row</button>
      <button title="take out the row of the cell selected" disabled={!mu.cell || rows <= 1} onclick={() => op(`Delete row ${mu.cell!.row} of pattern ${pattern}`, (s) => M.deleteRow(s, pattern, mu.cell!.row))}>− row</button>
      <button title="a new empty pattern, played after this position" onclick={newPattern}>New</button>
      <button title="a copy of this pattern, played after this position" onclick={duplicate}>Duplicate</button>
      <button title="every cell of this pattern emptied" onclick={() => op(`Clear pattern ${pattern}`, (s) => M.clearPattern(s, pattern))}>Clear</button>
      <button title="take this pattern out (and the order positions that play it)" disabled={patternCount <= 1} onclick={removePattern}>Delete</button>
    </span>
    <span class="group">
      <strong>Song</strong>
      <button title="a new empty channel at the end" onclick={() => op("Add a channel", (s) => M.addChannel(s))}>+ channel</button>
      <button title="take out the channel of the cell selected" disabled={!mu.cell || p.channels <= 1} onclick={() => op(`Remove channel ${mu.cell!.channel + 1}`, (s) => M.removeChannel(s, mu.cell!.channel))}>− channel {mu.cell ? mu.cell.channel + 1 : ""}</button>
      <label title="the speed (ticks a row) the song starts at: GAX2 starts every song at 6, so the build puts an Fxx with another one on the first row, unless the first row sets its own">
        speed <input type="number" min="1" max={M.MAX_SPEED} value={p.speed} onchange={(e) => { const n = num(e.currentTarget.value); if (Number.isInteger(n)) op(`Speed ${n}`, (s) => M.setSpeed(s, n)); }} />
      </label>
      {#if firstSpeed !== null && firstSpeed !== p.speed}<span class="hint">(its first row sets {firstSpeed})</span>{/if}
      <button class="link" onclick={() => (open = !open)}>{open ? "less" : "about"}</button>
    </span>
  </div>
  {#if scaffold && open}
    <p class="note">
      {mu.song} is one of the ROM's songs, built with the scaffold the ROM recorded (its patterns shared between order positions and channels, with transposes), which gives back the ROM's bytes. A change of its structure builds it from its XM, like a new song: each order position's pattern its own, the sharing found again by content. The save takes the scaffold out of the manifest; its settings (mix rate, volume, echo) stay.
    </p>
  {:else if scaffold}
    <p class="hint about">One of the ROM's songs: a change of its structure builds it from its XM, without the ROM's pattern sharing ("about").</p>
  {:else if open}
    <p class="hint about">
      The order list's positions each play one of the XM's patterns; a pattern can play at several. A GAX2 song has one pattern length, the longest pattern's: a shorter one ends with a pattern break the build adds, on a channel with no effect on its last row. The BPM isn't used: GAX2 steps one tick a frame.
    </p>
  {/if}
{/if}

<style>
  .bar {
    display: flex;
    flex-wrap: wrap;
    gap: 0.3rem 1rem;
    padding: 0.3rem 0.5rem;
    border-bottom: 1px solid var(--line);
    font-size: 0.78rem;
    align-items: center;
  }
  .group {
    display: inline-flex;
    flex-wrap: wrap;
    gap: 0.25rem;
    align-items: center;
  }
  .bar button {
    font-size: 0.75rem;
    padding: 0.05rem 0.35rem;
  }
  .bar input[type="number"] {
    width: 3.5rem;
  }
  .hint {
    color: var(--muted);
  }
  .about,
  .note {
    margin: 0.3rem 0.5rem;
    font-size: 0.78rem;
    max-width: 60rem;
  }
  .note {
    background: var(--warning-bg);
    color: var(--warning);
    padding: 0.3rem 0.4rem;
    border-radius: 4px;
  }
</style>

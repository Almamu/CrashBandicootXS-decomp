<script lang="ts">
  // A song's patterns as a tracker shows them: the order list along the
  // top (each order position plays an XM pattern: a ROM song's position p
  // its pattern p, a new song's its order list's), the pattern of the
  // position shown as rows of cells (note, instrument, volume column,
  // effect), the row playing marked. A cell is selected with the mouse or
  // the arrow keys, and edited on the right; Delete clears it.
  //
  // The structure (StructureBar): the order list's positions, the
  // patterns, their rows, the channels and the speed. A ROM song's first
  // structural edit builds it from its XM from then on (its shared
  // patterns each their own).
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import StructureBar from "./StructureBar.svelte";

  const sv = $derived(mu.songView);
  const p = $derived(sv?.p);
  const cells = $derived(sv?.cells ?? null);
  const xmPattern = $derived(p ? (p.playOrder[mu.order] ?? 0) : 0);
  const rows = $derived(p ? (p.rows[xmPattern] ?? 0) : 0);
  const playingRow = $derived(mu.playing === "song" && mu.position && mu.position.order === mu.order ? mu.position.row : -1);
  // the cells edited: the cell edits made, or (a structure changed) the cells that aren't the saved pattern's
  const edited = $derived.by(() => {
    if (!sv?.structure) return new Set(Object.keys(mu.state.cells[mu.song] ?? {}));
    const saved = mu.patterns[mu.song];
    const out: string[] = [];
    const now = sv.cells[xmPattern];
    const was = saved && saved.p.channels === sv.p.channels ? saved.cells[xmPattern] : undefined;
    if (!now) return new Set(out);
    for (let k = 0; k < now.length; k += M.CELL) {
      if (was && k + M.CELL <= was.length && now.subarray(k, k + M.CELL).every((v, i) => v === was[k + i])) continue;
      if (!was && now.subarray(k, k + M.CELL).every((v) => v === 0)) continue;
      out.push(M.cellKey(xmPattern, Math.floor(k / M.CELL / sv.p.channels), (k / M.CELL) % sv.p.channels));
    }
    return new Set(out);
  });

  function sharedWith(ch: number): string {
    if (!p?.groups) return "";
    const list = M.sharedCells(p, mu.order, ch).filter(([pj, cj]) => pj !== mu.order || cj !== ch);
    if (!list.length) return "";
    return "stored once with " + list.map(([pj, cj, dt]) => `${pj}${cj !== ch ? ` ch${cj + 1}` : ""}${dt ? ` (${dt > 0 ? "+" : ""}${dt})` : ""}`).join(", ");
  }

  function select(row: number, channel: number) {
    mu.cell = { row, channel };
  }

  function onkeydown(e: KeyboardEvent) {
    if (!p || !mu.cell) return;
    const t = e.target;
    if (t instanceof HTMLInputElement || t instanceof HTMLTextAreaElement || t instanceof HTMLSelectElement) return;
    const { row, channel } = mu.cell;
    if (e.key === "ArrowDown") mu.cell = { row: Math.min(rows - 1, row + 1), channel };
    else if (e.key === "ArrowUp") mu.cell = { row: Math.max(0, row - 1), channel };
    else if (e.key === "ArrowRight") mu.cell = { row, channel: Math.min(p.channels - 1, channel + 1) };
    else if (e.key === "ArrowLeft") mu.cell = { row, channel: Math.max(0, channel - 1) };
    else if (e.key === "Delete" || e.key === "Backspace") mu.setCell(xmPattern, row, channel, [0, 0, 0, 0, 0]);
    else if (e.key === "PageDown") mu.order = Math.min(p.playOrder.length - 1, mu.order + 1);
    else if (e.key === "PageUp") mu.order = Math.max(0, mu.order - 1);
    else return;
    e.preventDefault();
  }
</script>

<svelte:window {onkeydown} />

{#if !p || !cells}
  <div class="nothing">
    <p>Reading {mu.song}…</p>
  </div>
{:else}
  <StructureBar pattern={xmPattern} />
  <div class="orders" aria-label="the order list">
    {#each p.playOrder as xp, pos (pos)}
      <button
        class:on={pos === mu.order}
        class:playing={mu.playing === "song" && mu.position?.order === pos}
        class:loop={pos === p.loopOrder}
        title="order position {pos}: XM pattern {xp}{pos === p.loopOrder ? ' (the song loops back here)' : ''} — double-click to play from here"
        onclick={() => (mu.order = pos)}
        ondblclick={() => mu.playSong(pos)}>{pos}</button
      >
    {/each}
  </div>
  <div class="grid-wrap">
    <table class="grid">
      <thead>
        <tr>
          <th class="rn">{xmPattern}</th>
          {#each Array.from({ length: p.channels }, (_, c) => c) as c (c)}
            {@const sh = sharedWith(c)}
            <th title={sh}>ch {c + 1}{#if sh}<span class="shared"> ⇄</span>{/if}</th>
          {/each}
        </tr>
      </thead>
      <tbody>
        {#each Array.from({ length: rows }, (_, r) => r) as r (r)}
          <tr class:beat={r % 4 === 0} class:now={r === playingRow}>
            <td class="rn">{r.toString(16).toUpperCase().padStart(2, "0")}</td>
            {#each Array.from({ length: p.channels }, (_, c) => c) as c (c)}
              {@const cell = M.cellAt(cells, p.channels, xmPattern, r, c)}
              {@const t = M.cellText(cell)}
              {@const n = M.cellNotes(cell)}
              <td
                class="cell"
                class:sel={mu.cell?.row === r && mu.cell?.channel === c}
                class:edited={edited.has(M.cellKey(xmPattern, r, c))}
                class:dropped={n.dropped.length > 0}
                title={n.dropped.length ? `GAX2 plays: ${n.plays}; the build drops ${n.dropped.join("; ")}` : `GAX2 plays: ${n.plays}`}
                onclick={() => select(r, c)}
              >
                <span class="note" class:faint={cell[0] === 0}>{t.note}</span>
                <span class="ins" class:faint={!cell[1]}>{t.ins}</span>
                <span class="vol" class:faint={!cell[2]}>{t.vol}</span>
                <span class="fx" class:faint={!cell[3] && !cell[4]}>{t.fx}</span>
              </td>
            {/each}
          </tr>
        {/each}
      </tbody>
    </table>
  </div>
{/if}

<style>
  .nothing {
    padding: 2rem;
    color: var(--muted);
  }
  .orders {
    display: flex;
    flex-wrap: wrap;
    gap: 2px;
    padding: 0.35rem 0.5rem;
    border-bottom: 1px solid var(--line);
  }
  .orders button {
    font-family: ui-monospace, monospace;
    font-size: 0.75rem;
    padding: 0 0.3rem;
    min-width: 1.8rem;
  }
  .orders button.loop {
    border-left: 3px solid var(--accent);
  }
  .orders button.on {
    background: var(--accent);
    color: var(--accent-text);
    border-color: var(--accent);
  }
  .orders button.playing {
    outline: 2px solid var(--ok);
  }
  .grid-wrap {
    padding: 0.4rem 0.5rem;
    overflow: auto;
  }
  .grid {
    border-collapse: collapse;
    font-family: ui-monospace, "SF Mono", Menlo, Consolas, monospace;
    font-size: 0.8rem;
  }
  th {
    font-weight: 600;
    text-align: left;
    padding: 0.1rem 0.4rem;
    border-bottom: 1px solid var(--line);
    position: sticky;
    top: 0;
    background: var(--bg);
  }
  .shared {
    color: var(--accent);
  }
  td {
    padding: 0.05rem 0.4rem;
    white-space: nowrap;
  }
  .rn {
    color: var(--muted);
    text-align: right;
  }
  tr.beat td {
    background: color-mix(in srgb, var(--panel) 70%, transparent);
  }
  tr.now td {
    background: var(--focus);
  }
  .cell {
    cursor: pointer;
    border-left: 1px solid var(--line);
  }
  .cell.sel {
    outline: 2px solid var(--accent);
    outline-offset: -2px;
  }
  .cell.edited .note,
  .cell.edited .ins {
    font-weight: 700;
    color: var(--accent);
  }
  .cell.dropped .fx,
  .cell.dropped .vol {
    text-decoration: underline wavy var(--warning);
  }
  .note {
    display: inline-block;
    width: 3ch;
  }
  .ins,
  .vol {
    display: inline-block;
    width: 2ch;
    margin-left: 0.5ch;
  }
  .fx {
    display: inline-block;
    width: 3ch;
    margin-left: 0.5ch;
  }
  .faint,
  span.faint {
    color: var(--muted);
    opacity: 0.6;
  }
</style>

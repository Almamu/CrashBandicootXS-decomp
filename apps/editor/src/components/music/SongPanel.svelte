<script lang="ts">
  // The selected song: what it is (its id, XM, mix rate), its comment, what
  // plays it, the cell selected in the pattern view (and what GAX2 makes of
  // it), and where every song plays: the screens (music.json) and the level
  // themes (levels.json).
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";

  const info = $derived(mu.songInfo);
  const entry = $derived(mu.songs.find((s) => s.key === mu.song));
  const x = $derived(mu.songView);
  const p = $derived(x?.p);
  const xmPattern = $derived(p ? (p.playOrder[mu.order] ?? 0) : 0);
  const inside = $derived(!!p && !!mu.cell && mu.cell.row < (p.rows[xmPattern] ?? 0) && mu.cell.channel < p.channels);
  const cell = $derived(p && x && mu.cell && inside ? M.cellAt(x.cells, p.channels, xmPattern, mu.cell.row, mu.cell.channel) : null);
  const notes = $derived(cell ? M.cellNotes(cell) : null);
  const shared = $derived(p && mu.cell ? M.sharedCells(p, mu.order, mu.cell.channel).filter(([pj, cj]) => pj !== mu.order || cj !== mu.cell!.channel) : []);
  const uses = $derived(mu.usesOf(mu.song));
  const canRemove = $derived(!!entry && !info?.rom && uses.length === 0);

  let noteText = $derived(cell ? M.noteName(cell[0]) : "");

  function set(i: number, v: number, key: string) {
    if (!cell || !mu.cell || !Number.isInteger(v) || v < 0 || v > 255) return;
    const c = [...cell] as M.Cell;
    c[i] = v;
    mu.setCell(xmPattern, mu.cell.row, mu.cell.channel, c, key);
  }

  function setNote(s: string) {
    noteText = s;
    const n = M.parseNote(s);
    if (n !== null) set(0, n, "note");
  }

  const hex = (s: string) => (/^[0-9a-fA-F]{1,2}$/.test(s.trim()) ? parseInt(s.trim(), 16) : s.trim() === "" ? 0 : NaN);
</script>

<div class="panel">
  {#if entry}
    <section>
      <h3>{entry.key} <span class="hint">SONG_{entry.key.toUpperCase()}, id {entry.id} (0x{entry.id.toString(16)})</span></h3>
      {#if info?.xm}
        <p class="hint">
          {info.file} · “{info.xm.name}” · {info.xm.channels} channels, {info.xm.patterns} patterns, speed {info.xm.speed} · {info.mixRate} Hz, volume {info.volume}/256{info.halfRateFx ? ", its sound effects at half rate" : ""}
          · {info.rom ? "one of the ROM's songs" : "a new song"}
        </p>
      {:else if info?.error}
        <p class="note">{info.error}</p>
      {:else}
        <p class="hint">A new song, not saved yet.</p>
      {/if}
      <label class="wide">
        Comment
        <textarea rows="2" value={mu.state.comments[entry.key] ?? ""} oninput={(e) => mu.setComment(entry.key, e.currentTarget.value)}></textarea>
      </label>
      <p class="hint">Plays: {uses.length ? uses.join(", ") : "nothing (pick it below for a screen or a theme)"}</p>
      {#if !info?.rom}
        <button disabled={!canRemove} title={canRemove ? "take it out of the song table (its XM stays, unbuilt)" : "only a new song nothing plays"} onclick={() => mu.removeSong(entry.key)}>Take it out of the song table</button>
      {/if}
    </section>
  {/if}

  {#if p && mu.cell && cell && notes}
    <section>
      <h3>Pattern {xmPattern}, row {mu.cell.row}, channel {mu.cell.channel + 1}</h3>
      <div class="cellgrid">
        <label>Note <input value={noteText} spellcheck="false" class:invalid={M.parseNote(noteText) === null} oninput={(e) => setNote(e.currentTarget.value)} title="C-4, C#4, === (key off), --- or empty (none)" /></label>
        <label>
          Instrument
          <select value={cell[1]} onchange={(e) => set(1, Number(e.currentTarget.value), "")}>
            <option value={0}>none</option>
            {#each p.instruments as ins, i (i)}<option value={i + 1}>{String(i + 1).padStart(2, "0")} {ins.name}</option>{/each}
          </select>
        </label>
        <label>Volume <input value={cell[2] ? cell[2].toString(16).toUpperCase() : ""} placeholder=".." spellcheck="false" oninput={(e) => set(2, hex(e.currentTarget.value), "vol")} title="the volume column, hex: 10-50 set, 80-8F slide down, 90-9F up; 01 makes the effect column GAX2's own command and parameter" /></label>
        <label>
          Effect
          <select value={cell[3]} onchange={(e) => set(3, Number(e.currentTarget.value), "")}>
            {#each M.EFFECTS.split("") as ch, i (i)}<option value={i}>{ch}</option>{/each}
          </select>
        </label>
        <label>Parameter <input value={cell[3] || cell[4] ? cell[4].toString(16).toUpperCase().padStart(2, "0") : ""} placeholder=".." spellcheck="false" oninput={(e) => set(4, hex(e.currentTarget.value), "param")} /></label>
      </div>
      <div class="row">
        <button onclick={() => mu.setCell(xmPattern, mu.cell!.row, mu.cell!.channel, [0, 0, 0, 0, 0])} title="Delete">Clear</button>
        <button onclick={() => mu.setCell(xmPattern, mu.cell!.row, mu.cell!.channel, [M.KEY_OFF, 0, cell[2], cell[3], cell[4]])}>Key off</button>
      </div>
      <p class="hint">GAX2 plays: {notes.plays}.</p>
      {#each notes.dropped as d (d)}<p class="warn">The build drops {d}.</p>{/each}
      {#if shared.length}
        <p class="note">
          The ROM stores this channel's pattern once for order positions {shared.map(([pj, cj, dt]) => `${pj}${cj !== mu.cell!.channel ? ` (channel ${cj + 1})` : ""}${dt ? ` (transposed ${dt > 0 ? "+" : ""}${dt})` : ""}`).join(", ")}: an edit here is made there too.
        </p>
      {/if}
    </section>
  {:else if p}
    <section><p class="hint">Click a cell of the pattern to edit it (arrow keys move, Delete clears, Page Up/Down change the order position).</p></section>
  {/if}

  <section>
    <h3>The screens' songs <span class="hint">music.json</span></h3>
    {#each mu.state.music.screen_music as s (s.screen)}
      <label class="row" title={s.comment}>
        <span>{s.screen}</span>
        <select value={s.song} onchange={(e) => mu.setScreenSong(s.screen, e.currentTarget.value)}>
          {#each mu.songKeys as k (k)}<option value={k}>{k}</option>{/each}
        </select>
      </label>
    {/each}
  </section>
  <section>
    <h3>The level themes' songs <span class="hint">levels.json</span></h3>
    {#each mu.view?.themes ?? [] as t (t.constant)}
      <label class="row">
        <span>{t.name}</span>
        <select value={mu.state.themes[t.constant]} onchange={(e) => mu.setThemeSong(t.constant, e.currentTarget.value)}>
          {#each mu.songKeys as k (k)}<option value={k}>{k}</option>{/each}
        </select>
      </label>
    {/each}
    <p class="hint">A cutscene slide's song is the Cutscenes tab's.</p>
  </section>
</div>

<style>
  .panel {
    padding: 0.5rem 0.6rem;
    font-size: 0.85rem;
  }
  section {
    border-bottom: 1px solid var(--line);
    padding-bottom: 0.5rem;
    margin-bottom: 0.5rem;
  }
  h3 {
    font-size: 0.85rem;
    margin: 0.2rem 0 0.35rem;
  }
  .row {
    display: grid;
    grid-template-columns: 7rem 1fr;
    gap: 0.35rem;
    align-items: center;
    margin: 0.15rem 0;
  }
  .row select {
    min-width: 0;
  }
  div.row {
    display: flex;
  }
  .wide {
    display: block;
  }
  .wide textarea {
    width: 100%;
  }
  .cellgrid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 0.3rem 0.5rem;
  }
  .cellgrid label {
    display: grid;
    gap: 0.1rem;
    font-size: 0.78rem;
  }
  .cellgrid input,
  .cellgrid select {
    font-family: ui-monospace, monospace;
    min-width: 0;
  }
  .hint {
    color: var(--muted);
    font-size: 0.78rem;
    margin: 0.2rem 0;
    font-weight: normal;
  }
  .note {
    background: var(--warning-bg);
    color: var(--warning);
    padding: 0.3rem 0.4rem;
    border-radius: 4px;
    font-size: 0.78rem;
  }
  .warn {
    color: var(--warning);
    font-size: 0.78rem;
    margin: 0.15rem 0;
  }
</style>

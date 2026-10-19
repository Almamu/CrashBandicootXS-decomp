<script lang="ts">
  // The collision tool's value: one of the legend's (collision_legend.json,
  // docs/levels.md "Collision layers"). "New value" adds one to the legend
  // (its bits: terrain type, flag nibble, open bits, docs/flags/terrain.md)
  // with a colour of its own, saved at once.
  import { level } from "../../lib/level/state.svelte.ts";
  import { editor } from "../../lib/state.svelte.ts";
  import { OPEN_BITS, XS_FLAGS, composeValue, freeColour, newValueErrors, typeKind, valueName } from "../../lib/level/terrain.ts";

  let filter = $state("");

  const hex = (v: number) => v.toString(16).toUpperCase().padStart(4, "0");
  const shown = $derived(level.legend.filter((e) => !filter || e.name.toLowerCase().includes(filter.toLowerCase()) || hex(e.value).includes(filter.toUpperCase())));

  // -- a new value ----------------------------------------------------------------------
  let adding = $state(false);
  let typeText = $state("2B");
  let flags = $state(0);
  let open = $state(0);
  let colour = $state("");
  let name = $state("");

  const type = $derived(/^[0-9a-f]{1,2}$/i.test(typeText) ? parseInt(typeText, 16) : NaN);
  const value = $derived(Number.isNaN(type) ? NaN : composeValue({ type, flags, open }));
  const shownColour = $derived(colour || (Number.isNaN(value) ? "#000000" : freeColour(value, level.legend)));
  const errors = $derived(Number.isNaN(value) ? ["the terrain type is 2 hex digits, 00-FF"] : newValueErrors(value, shownColour, level.legend));
  const xs = $derived((editor.project?.game ?? "xs") === "xs");

  async function add() {
    if (errors.length || Number.isNaN(value)) return;
    if (await level.addLegendValue(value, shownColour, name.trim() || undefined)) {
      adding = false;
      colour = "";
      name = "";
    }
  }
</script>

<div class="panel">
  <label class="row">
    <span>overlay</span>
    <input type="range" min="0.1" max="1" step="0.05" bind:value={level.collisionOpacity} />
  </label>
  <p class="muted small">Click to paint, shift-drag for a rectangle, alt-click to pick a cell's value. A cell is 8x8 pixels of BG0's.</p>
  <input type="search" placeholder="Filter the values" bind:value={filter} />
  <ul>
    {#each shown as e (e.value)}
      <li>
        <button class:current={level.collisionValue === e.value} onclick={() => (level.collisionValue = e.value)}>
          <span class="swatch" style="background:{e.colour}"></span>
          <code>{hex(e.value)}</code>
          <span class="name">{e.name}</span>
        </button>
      </li>
    {/each}
  </ul>

  {#if !adding}
    <button onclick={() => (adding = true)} title="add a value no room has yet to the legend">New value…</button>
  {:else}
    <div class="new">
      <strong>A new value</strong>
      <label class="field">
        <span>terrain type</span>
        <span>0x<input class="hexin" maxlength="2" bind:value={typeText} aria-label="terrain type" /></span>
      </label>
      <p class="muted small">{Number.isNaN(type) ? "" : typeKind(type)}</p>
      <label class="field">
        <span>flags</span>
        <select bind:value={flags} aria-label="the flag nibble">
          {#each { length: 16 } as _, f (f)}
            <option value={f}>0x{f.toString(16).toUpperCase()}{f === 0 ? ": none" : xs ? `: ${XS_FLAGS[f] ?? "nothing (XS's player)"}` : ""}</option>
          {/each}
        </select>
      </label>
      <div class="field">
        <span>open to</span>
        <span>
          {#each OPEN_BITS as b (b.bit)}
            <label title={b.name}
              ><input type="checkbox" checked={(open & b.bit) !== 0} onchange={(e) => (open = e.currentTarget.checked ? open | b.bit : open & ~b.bit)} />
              {b.label}</label
            >
          {/each}
        </span>
      </div>
      <label class="field">
        <span>colour</span>
        <input type="color" value={shownColour} oninput={(e) => (colour = e.currentTarget.value.toUpperCase())} />
      </label>
      <label class="field">
        <span>name</span>
        <input placeholder={Number.isNaN(value) ? "" : valueName(value)} bind:value={name} />
      </label>
      <p class="small">
        {#if !Number.isNaN(value)}<code>{hex(value)}</code>{/if}
        {#each errors as e (e)}<span class="warn"> {e}.</span>{/each}
      </p>
      <p class="muted small">
        The probes read a solid type's shape (0x24 and up) and its open bits, a height type's (1-0x23) surface; the flag nibble goes to the
        player when a probe meets the cell. The legend is every room's: adding saves it at once.
      </p>
      <div class="row2">
        <button disabled={errors.length > 0 || !!editor.busy} onclick={add}>Add to the legend</button>
        <button onclick={() => (adding = false)}>Cancel</button>
      </div>
    </div>
  {/if}
</div>

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
    padding: 0.5rem;
  }
  .row {
    display: grid;
    grid-template-columns: 4rem 1fr;
    gap: 0.4rem;
    align-items: center;
    font-size: 0.85rem;
  }
  ul {
    list-style: none;
    margin: 0;
    padding: 0;
    max-height: 60vh;
    overflow: auto;
  }
  li button {
    display: flex;
    gap: 0.4rem;
    align-items: center;
    width: 100%;
    border: 1px solid transparent;
    background: none;
    text-align: left;
    font-size: 0.8rem;
    padding: 0.1rem 0.25rem;
  }
  li button.current {
    border-color: var(--accent);
    background: var(--panel);
  }
  .swatch {
    width: 1rem;
    height: 1rem;
    border: 1px solid var(--line);
    flex: none;
  }
  .name {
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  .new {
    display: flex;
    flex-direction: column;
    gap: 0.3rem;
    border-top: 1px solid var(--line);
    padding-top: 0.4rem;
    font-size: 0.85rem;
  }
  .field {
    display: grid;
    grid-template-columns: 6rem 1fr;
    gap: 0.4rem;
    align-items: center;
  }
  .hexin {
    width: 2.5rem;
    font-family: monospace;
  }
  .row2 {
    display: flex;
    gap: 0.4rem;
  }
  .muted {
    color: var(--muted);
  }
  .warn {
    color: var(--error);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
</style>

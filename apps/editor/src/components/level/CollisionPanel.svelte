<script lang="ts">
  // The collision tool's value: one of the legend's (collision_legend.json,
  // docs/levels.md "Collision layers"). A value no room has yet needs its
  // own colour in the legend first.
  import { level } from "../../lib/level/state.svelte.ts";

  let filter = $state("");

  const hex = (v: number) => v.toString(16).toUpperCase().padStart(4, "0");
  const shown = $derived(level.legend.filter((e) => !filter || e.name.toLowerCase().includes(filter.toLowerCase()) || hex(e.value).includes(filter.toUpperCase())));
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
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
</style>

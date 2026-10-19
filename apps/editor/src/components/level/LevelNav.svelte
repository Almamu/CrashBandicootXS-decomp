<script lang="ts">
  // The level table (levels.json): each level's rooms, in play order, and its
  // bonus and gem-path rooms; a stage played in an actor category has no room.
  import { level } from "../../lib/level/state.svelte.ts";
  import NewRoomDialog from "./NewRoomDialog.svelte";

  let filter = $state("");
  let making = $state(false);
  const q = $derived(filter.trim().toLowerCase());
  const shown = $derived(level.levels.filter((l) => !q || l.name.toLowerCase().includes(q) || l.rooms.some((r) => r.folder.includes(q))));
</script>

<div class="nav">
  <input type="search" placeholder="Filter the levels" bind:value={filter} aria-label="filter the levels" />
  <ul>
    {#each shown as l (l.index)}
      <li>
        <div class="level"><span class="n">{l.index}</span> {l.name}</div>
        <ul>
          {#each l.rooms as r (r.path)}
            <li>
              <button class:current={level.view?.path === r.path} title={r.path} onclick={() => level.openRoom(r.path)}>{r.label}
                <span class="muted">{r.folder}</span></button>
            </li>
          {/each}
          {#each l.stages as s (s)}
            <li class="stage" title="a stage of the actor category {s}: no room data (docs/levels.md, 'Actor categories')">stage {s}</li>
          {/each}
        </ul>
      </li>
    {/each}
    {#if level.others.length}
      <li>
        <div class="level">Rooms no level plays</div>
        <ul>
          {#each level.others as r (r.path)}
            <li><button class:current={level.view?.path === r.path} onclick={() => level.openRoom(r.path)}>{r.label}</button></li>
          {/each}
        </ul>
      </li>
    {/if}
  </ul>
  <button class="new" disabled={!level.levels.length} onclick={() => (making = true)} title="a copy of a room, or an empty room laid out like it">New room…</button>
</div>
{#if making}<NewRoomDialog onclose={() => (making = false)} />{/if}

<style>
  .nav {
    padding: 0.5rem;
  }
  input[type="search"] {
    width: 100%;
    margin-bottom: 0.4rem;
  }
  ul {
    list-style: none;
    margin: 0;
    padding: 0;
  }
  ul ul {
    padding-left: 0.8rem;
    margin-bottom: 0.3rem;
  }
  .level {
    font-weight: 600;
    font-size: 0.85rem;
    padding: 0.15rem 0;
  }
  .n {
    color: var(--muted);
    font-weight: normal;
    display: inline-block;
    min-width: 1.4rem;
  }
  li button {
    border: none;
    background: none;
    padding: 0.05rem 0.25rem;
    text-align: left;
    width: 100%;
    font-size: 0.85rem;
    border-radius: 3px;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
  li button:hover {
    background: var(--panel);
  }
  li button.current {
    background: var(--accent);
    color: var(--accent-text);
  }
  li button.current .muted {
    color: inherit;
  }
  .stage {
    color: var(--muted);
    font-size: 0.8rem;
    padding: 0.05rem 0.25rem;
  }
  .muted {
    color: var(--muted);
    font-size: 0.8em;
  }
  .new {
    margin-top: 0.6rem;
    width: 100%;
  }
</style>

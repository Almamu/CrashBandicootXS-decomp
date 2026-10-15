<script lang="ts">
  // The selected entity: its type, position and parameter record, its links;
  // or, with none selected, the room's entities as a list.
  import { editor } from "../../lib/state.svelte.ts";
  import { columnOf } from "../../lib/level/coords.ts";
  import { recordUsers, type ParamRecord } from "../../lib/level/entities.ts";
  import { level } from "../../lib/level/state.svelte.ts";
  import ParamForm from "./ParamForm.svelte";

  let { oncentre }: { oncentre?: (x: number, y: number) => void } = $props();

  const room = $derived(level.room);
  const i = $derived(level.selected);
  const e = $derived(i !== null && room ? room.entities[i] : undefined);
  const info = $derived(e ? level.typeOf(e.type) : undefined);
  const users = $derived(e && room ? recordUsers(room, e.param) : []);
  const links = $derived(room?.links ?? []);
  const roomSchema = $derived((editor.schemas["room.schema.json"] ?? {}) as Record<string, unknown>);
  let filter = $state("");

  const types = $derived(level.types);

  function typeValue(t: string | number): string {
    return typeof t === "number" ? `#${t}` : t;
  }

  function parseType(s: string): string | number {
    return s.startsWith("#") ? Number(s.slice(1)) : s;
  }

  function summary(r: ParamRecord): string {
    if (Array.isArray(r)) return `raw [${r.join(", ")}]`;
    const o = r as unknown as Record<string, unknown>;
    return `${o["layout"]}` + Object.entries(o).filter(([k, v]) => k !== "layout" && !(Array.isArray(v) && !v.length) && v !== 0).map(([k, v]) => ` ${k}=${JSON.stringify(v)}`).join("").slice(0, 80);
  }

  function num(s: string): number | null {
    const v = Number(s);
    return Number.isInteger(v) && v >= 0 && v <= 0xffff ? v : null;
  }

  const listed = $derived.by(() => {
    const q = filter.trim().toLowerCase();
    return (room?.entities ?? []).map((x, k) => ({ x, k })).filter(({ x, k }) => !q || String(x.type).toLowerCase().includes(q) || String(k) === q);
  });
</script>

{#if !room}
  <p class="muted">No room open.</p>
{:else if e && i !== null}
  <div class="panel">
    <div class="head">
      <strong>Entity {i}</strong>
      <span class="muted">group {columnOf(e.x)}</span>
      <span class="spacer"></span>
      <button class="icon" title="show it" onclick={() => oncentre?.(e.x, e.y)}>⌖</button>
      <button class="icon" onclick={() => (level.selected = null)}>✕</button>
    </div>
    <label class="row">
      <span>type</span>
      <select value={typeValue(e.type)} onchange={(ev) => level.updateEntity(i, { type: parseType(ev.currentTarget.value) })}>
        {#if !info}<option value={typeValue(e.type)}>{e.type} (unknown)</option>{/if}
        {#each types as t (t.type)}
          <option value={t.name ?? `#${t.type}`}>{t.name ?? `type ${t.type}`}{t.name ? "" : ` (${t.spawner ?? "unnamed"})`}</option>
        {/each}
      </select>
    </label>
    {#if info?.spawner}<p class="muted small">{info.spawner}{info.comment ? ` — ${info.comment}` : ""}</p>{/if}
    <div class="row">
      <span>x, y</span>
      <span class="xy">
        <input type="number" min="0" max="65535" value={e.x} onchange={(ev) => { const v = num(ev.currentTarget.value); if (v !== null) level.updateEntity(i, { x: v }); }} />
        <input type="number" min="0" max="65535" value={e.y} onchange={(ev) => { const v = num(ev.currentTarget.value); if (v !== null) level.updateEntity(i, { y: v }); }} />
      </span>
    </div>
    <label class="row">
      <span>record</span>
      <select value={String(e.param)} onchange={(ev) => level.updateEntity(i, { param: Number(ev.currentTarget.value) })}>
        {#each room.params as r, k (k)}<option value={String(k)}>{k}: {summary(r)}</option>{/each}
      </select>
    </label>
    {#if users.length > 1}
      <p class="note">
        Record {e.param} is shared by {users.length} entities ({users.slice(0, 8).join(", ")}{users.length > 8 ? ", …" : ""}): a change is theirs too.
        <button class="link" onclick={() => level.ownRecord(i)}>Give this one its own copy</button>
      </p>
    {/if}
    {#if room.params[e.param] !== undefined}
      <ParamForm record={room.params[e.param]!} {roomSchema} types={types} expected={info?.params ?? null}
        onchange={(r) => level.setRecord(e.param, r)} />
    {/if}
    <h4>Links</h4>
    <ul class="links">
      {#each links as l, k (k)}
        {#if l[0] === i || l[1] === i}
          <li>
            {l[0] === i ? "under" : "on top of"}
            <button class="link" onclick={() => (level.selected = l[0] === i ? l[1] : l[0])}>entity {l[0] === i ? l[1] : l[0]}</button>
            <button class="icon" title="delete the link" onclick={() => level.deleteLink(k)}>✕</button>
          </li>
        {/if}
      {/each}
    </ul>
    <p class="muted small">A link stacks crates: the first is the lower one. The link tool adds one: click the lower crate, then the one on top.</p>
    <div class="actions">
      <button onclick={() => level.addEntity(e.type, e.x + 16, e.y)}>Duplicate</button>
      <button onclick={() => level.deleteEntity(i)}>Delete</button>
    </div>
  </div>
{:else}
  <div class="panel">
    <input type="search" placeholder="Filter the entities" bind:value={filter} />
    <ul class="list">
      {#each listed as { x, k } (k)}
        <li><button onclick={() => { level.selected = k; oncentre?.(x.x, x.y); }}>{k}: {x.type} <span class="muted">({x.x}, {x.y})</span></button></li>
      {/each}
    </ul>
    <p class="muted small">{room.entities.length} entities, {room.params.length} parameter records, {links.length} links. They are in id order (the last group first); a move to another group renumbers them, and the links follow.</p>
  </div>
{/if}

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.35rem;
    padding: 0.5rem;
  }
  .head {
    display: flex;
    gap: 0.4rem;
    align-items: center;
  }
  .spacer {
    flex: 1;
  }
  .row {
    display: grid;
    grid-template-columns: 7rem 1fr;
    gap: 0.4rem;
    align-items: center;
    font-size: 0.85rem;
  }
  .row > span:first-child {
    color: var(--muted);
  }
  select,
  input {
    width: 100%;
    min-width: 0;
  }
  .xy {
    display: flex;
    gap: 0.3rem;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
    margin: 0;
  }
  .note {
    font-size: 0.8rem;
    background: var(--warning-bg);
    color: var(--warning);
    padding: 0.3rem;
    border-radius: 4px;
    margin: 0;
  }
  h4 {
    margin: 0.4rem 0 0;
    font-size: 0.85rem;
  }
  ul {
    list-style: none;
    margin: 0;
    padding: 0;
    font-size: 0.85rem;
  }
  .list {
    max-height: 50vh;
    overflow: auto;
  }
  .list button {
    border: none;
    background: none;
    width: 100%;
    text-align: left;
    padding: 0.05rem 0.25rem;
  }
  .list button:hover {
    background: var(--panel);
  }
  .actions {
    display: flex;
    gap: 0.4rem;
    margin-top: 0.4rem;
  }
</style>

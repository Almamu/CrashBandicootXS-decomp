<script lang="ts">
  // A parameter record (docs/levels.md, "Parameter records"): a named view's
  // fields (the flags and options as their bits, a time-trial kind or solid
  // kind as an entity type, the numbers in their ranges), or a raw record's
  // words. Each change is the whole record, given to `onchange`.
  import type { EntityTypeInfo } from "@crash/schema-types";
  import { layoutFields, layouts, newRecord, type ParamRecord } from "../../lib/level/entities.ts";

  interface Props {
    record: ParamRecord;
    roomSchema: Record<string, unknown>;
    types: EntityTypeInfo[];
    /** the view the record's entities' type reads (entity_types.json's "params"), if it says */
    expected?: string | null;
    onchange: (r: ParamRecord) => void;
  }
  let { record, roomSchema, types, expected = null, onchange }: Props = $props();

  const raw = $derived(Array.isArray(record));
  const rec = $derived(raw ? null : (record as unknown as Record<string, unknown>));
  const layout = $derived(raw ? "raw" : String(rec!["layout"]));
  const fields = $derived(raw ? [] : layoutFields(roomSchema, layout));
  const named = $derived(types.filter((t) => t.name));
  const zero = $derived(types.find((t) => t.type === 0)?.name ?? null);

  function set(name: string, v: unknown) {
    const out = { ...rec!, [name]: v };
    onchange(out as unknown as ParamRecord);
  }

  function toggleBit(name: string, bit: string, on: boolean) {
    const cur = (rec![name] as string[] | undefined) ?? [];
    const all = fields.find((f) => f.name === name)?.bits ?? [];
    const next = all.filter((b) => (b === bit ? on : cur.includes(b)));
    set(name, next);
  }

  function int(name: string, s: string, min?: number, max?: number) {
    const v = Number(s);
    if (!Number.isInteger(v) || (min !== undefined && v < min) || (max !== undefined && v > max)) return;
    set(name, v);
  }

  function entity(name: string, s: string) {
    set(name, /^-?\d+$/.test(s) ? Number(s) : s);
  }

  function setLayout(l: string) {
    if (l === layout) return;
    if (l === "raw") onchange([0]);
    else onchange(newRecord(roomSchema, l, zero));
  }

  function word(i: number, s: string) {
    const v = s.trim().toLowerCase().startsWith("0x") ? parseInt(s, 16) : Number(s);
    if (!Number.isInteger(v) || v < 0 || v > 0xffffffff) return;
    const out = [...(record as number[])];
    out[i] = v;
    onchange(out as unknown as ParamRecord);
  }
</script>

<div class="form">
  <label class="row">
    <span>view</span>
    <select value={layout} onchange={(e) => setLayout(e.currentTarget.value)}>
      {#each layouts(roomSchema) as l (l)}<option value={l}>{l}{l === expected ? " (its type's)" : ""}</option>{/each}
      <option value="raw">raw words</option>
    </select>
  </label>
  {#if expected && layout !== expected}
    <p class="warn">Its entities' type reads the <code>{expected}</code> view: data-check refuses another one.</p>
  {/if}
  {#if raw}
    {#each record as number[] as w, i (i)}
      <label class="row">
        <span>word {i}</span>
        <input value={"0x" + w.toString(16).toUpperCase()} onchange={(e) => word(i, e.currentTarget.value)} />
      </label>
    {/each}
    <div class="row">
      <span></span>
      <span>
        <button class="icon" onclick={() => onchange([...(record as number[]), 0] as unknown as ParamRecord)}>+ word</button>
        <button class="icon" disabled={(record as number[]).length <= 1} onclick={() => onchange((record as number[]).slice(0, -1) as unknown as ParamRecord)}>− word</button>
      </span>
    </div>
  {:else}
    {#each fields as f (f.name)}
      {#if f.kind === "bits"}
        <div class="row">
          <span>{f.name}</span>
          <span class="bits">
            {#each f.bits ?? [] as b (b)}
              <label title={b}>
                <input type="checkbox" checked={((rec![f.name] as string[] | undefined) ?? []).includes(b)} onchange={(e) => toggleBit(f.name, b, e.currentTarget.checked)} />
                {b}
              </label>
            {/each}
          </span>
        </div>
      {:else if f.kind === "entity"}
        <label class="row">
          <span>{f.name}</span>
          <select value={String(rec![f.name] ?? "")} onchange={(e) => entity(f.name, e.currentTarget.value)}>
            {#if typeof rec![f.name] === "number"}<option value={String(rec![f.name])}>type {rec![f.name]}</option>{/if}
            {#if rec![f.name] === undefined}<option value="">(not given)</option>{/if}
            {#each named as t (t.type)}<option value={t.name}>{t.name}</option>{/each}
          </select>
        </label>
      {:else}
        <label class="row">
          <span>{f.name}</span>
          <input type="number" min={f.min} max={f.max} value={rec![f.name] ?? ""} placeholder="(not given)"
            onchange={(e) => int(f.name, e.currentTarget.value, f.min, f.max)} />
        </label>
      {/if}
    {/each}
    {#if rec!["extra"]}
      <p class="muted">extra words: {JSON.stringify(rec!["extra"])} (edit them in room.json's form)</p>
    {/if}
  {/if}
</div>

<style>
  .form {
    display: flex;
    flex-direction: column;
    gap: 0.25rem;
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
    overflow: hidden;
    text-overflow: ellipsis;
  }
  input:not([type="checkbox"]),
  select {
    width: 100%;
    min-width: 0;
  }
  .bits {
    display: flex;
    flex-wrap: wrap;
    gap: 0.1rem 0.6rem;
    font-size: 0.75rem;
  }
  .warn {
    color: var(--warning);
    font-size: 0.8rem;
    margin: 0;
  }
  .muted {
    color: var(--muted);
    font-size: 0.8rem;
    margin: 0;
  }
</style>

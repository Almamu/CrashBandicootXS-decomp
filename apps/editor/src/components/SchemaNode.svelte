<script lang="ts">
  // One value of a data file, edited by what its schema says it is: an object
  // (its properties), an array (its items), one of several forms (anyOf/oneOf),
  // an enum, a number, a string, a boolean. Recursive.
  import { tick } from "svelte";
  import Self from "./SchemaNode.svelte";
  import {
    alternatives,
    defaultValue,
    isOpen,
    itemSchema,
    jsonType,
    label,
    pointerOf,
    propertySchema,
    resolve,
    typeFits,
    types,
    withKey,
    withoutKey,
    type Json,
    type Schema,
  } from "../lib/schema.ts";

  const PAGE = 100;

  interface Props {
    root: Schema;
    schema: Schema | boolean | undefined;
    value: Json;
    path: (string | number)[];
    errors: Map<string, string[]>;
    focus: string | null;
    focusSeq: number;
    name: string;
    depth?: number;
    required?: boolean;
    /** just the control: no row, no name (an inline object's field) */
    bare?: boolean;
    /** the alternative's own editor: the parent showed the name */
    nested?: boolean;
    onremove?: () => void;
    onmove?: (by: number) => void;
  }

  let {
    root,
    schema,
    value = $bindable(),
    path,
    errors,
    focus,
    focusSeq,
    name,
    depth = 0,
    required = true,
    bare = false,
    nested = false,
    onremove,
    onmove,
  }: Props = $props();

  const s = $derived(resolve(root, schema));
  const ptr = $derived(pointerOf(path));
  const here = $derived(errors.get(ptr) ?? []);
  const under = $derived.by(() => {
    let n = 0;
    for (const [k, v] of errors) if (k.startsWith(ptr + "/")) n += v.length;
    return n;
  });

  // -- one of several forms ---------------------------------------------------
  const alts = $derived(alternatives(s));
  let chosen = $state<number | null>(null);

  function fits(alt: Schema, v: Json): boolean {
    const r = resolve(root, alt);
    if (r.const !== undefined) return JSON.stringify(r.const) === JSON.stringify(v);
    if (r.enum) return r.enum.some((e) => JSON.stringify(e) === JSON.stringify(v));
    const sub = alternatives(r);
    if (sub) return sub.some((x) => fits(x, v));
    if (!typeFits(v, types(r))) return false;
    if (v !== null && typeof v === "object" && !Array.isArray(v)) {
      if (!(r.required ?? []).every((k) => k in v)) return false;
      return Object.keys(v).every((k) => propertySchema(root, r, k) !== undefined);
    }
    if (Array.isArray(v) && r.prefixItems) return v.length >= (r.minItems ?? 0) && v.length <= (r.maxItems ?? Infinity);
    return true;
  }

  const altIndex = $derived.by(() => {
    if (!alts) return -1;
    if (chosen !== null && chosen < alts.length) return chosen;
    const i = alts.findIndex((a) => fits(a, value));
    return i < 0 ? 0 : i;
  });

  function chooseAlt(i: number) {
    chosen = i;
    if (alts && !fits(alts[i]!, value)) value = defaultValue(root, alts[i]);
  }

  // -- what it is -------------------------------------------------------------
  const allowed = $derived(types(s));
  const kind = $derived.by(() => {
    if (alts) return "alts";
    if (s.const !== undefined) return "const";
    if (s.enum) return "enum";
    if (isOpen(s)) return "open";
    const t = jsonType(value);
    if (typeFits(value, allowed)) return t === "integer" && !allowed.includes("integer") ? "number" : t;
    return "mismatch";
  });

  // -- containers -------------------------------------------------------------
  const obj = $derived(kind === "object" ? (value as Record<string, Json>) : null);
  const arr = $derived(kind === "array" ? (value as Json[]) : null);
  const scalarish = (v: Json) => v === null || typeof v !== "object" || (Array.isArray(v) && v.length <= 8 && v.every((x) => x === null || typeof x !== "object"));
  const inline = $derived(
    (obj !== null && Object.keys(obj).length <= 10 && Object.values(obj).every(scalarish)) ||
      (arr !== null && arr.length <= 24 && arr.every((x) => x === null || typeof x !== "object")),
  );
  const size = $derived(kind === "object" || kind === "array" ? JSON.stringify(value).length : 0);
  let open = $state(false);
  let opened = false;
  $effect.pre(() => {
    if (!opened) {
      opened = true;
      open = depth === 0 || inline || (depth <= 2 && size < 4000);
    }
  });
  let page = $state(0);
  const pages = $derived(arr ? Math.max(1, Math.ceil(arr.length / PAGE)) : 1);
  const missing = $derived(obj ? Object.keys(s.properties ?? {}).filter((k) => !(k in obj)) : []);
  const freeKeys = $derived(s.additionalProperties !== false && (s.additionalProperties !== undefined || !!s.patternProperties));
  const fixedLength = $derived(s.minItems !== undefined && s.minItems === s.maxItems);
  let newKey = $state("");

  function addKey(k: string) {
    if (!obj || !k || k in obj) return;
    value = withKey(root, s, obj, k, defaultValue(root, propertySchema(root, s, k)));
  }

  function addItem() {
    if (!arr) return;
    const item = arr.length ? structuredClone($state.snapshot(arr[arr.length - 1] as unknown)) : defaultValue(root, itemSchema(root, s, arr.length));
    arr.push(item as Json);
    page = Math.floor((arr.length - 1) / PAGE);
  }

  function preview(v: Json): string {
    const t = JSON.stringify(v);
    return t.length > 90 ? t.slice(0, 89) + "…" : t;
  }

  // -- scalars ------------------------------------------------------------------
  let openText = $state("");
  let openBad = $state(false);
  $effect(() => {
    if (kind === "open" || kind === "mismatch") {
      openText = JSON.stringify(value);
      openBad = false;
    }
  });
  function setOpen() {
    try {
      value = JSON.parse(openText) as Json;
      openBad = false;
    } catch {
      openBad = true;
    }
  }

  // -- a diagnostic's place ---------------------------------------------------------
  let el = $state<HTMLElement | undefined>();
  let flash = $state(false);
  $effect(() => {
    void focusSeq;
    if (!focus) return;
    if (focus === ptr || focus.startsWith(ptr + "/")) {
      open = true;
      if (arr && focus !== ptr) {
        const next = Number(focus.slice(ptr.length + 1).split("/")[0]);
        if (Number.isFinite(next)) page = Math.floor(next / PAGE);
      }
    }
    if (focus === ptr && !nested) {
      void tick().then(() => {
        el?.scrollIntoView({ block: "center", behavior: "smooth" });
        flash = true;
        setTimeout(() => (flash = false), 1600);
      });
    }
  });

  const desc = $derived((s.description as string | undefined) ?? "");
</script>

{#snippet controls()}
  {#if onmove}
    <button class="icon" title="up" onclick={() => onmove?.(-1)}>↑</button>
    <button class="icon" title="down" onclick={() => onmove?.(1)}>↓</button>
  {/if}
  {#if onremove && !required}
    <button class="icon" title="remove" onclick={() => onremove?.()}>✕</button>
  {/if}
{/snippet}

{#snippet messages()}
  {#each here as m (m)}<div class="msg">{m}</div>{/each}
{/snippet}

{#snippet scalar()}
  {#if kind === "string"}
    <input type="text" bind:value class:invalid={here.length} size={Math.min(40, Math.max(8, String(value).length + 2))} aria-label={name} />
  {:else if kind === "integer" || kind === "number"}
    <input type="number" step={kind === "integer" && !allowed.includes("number") ? 1 : "any"} min={s.minimum} max={s.maximum}
      bind:value class:invalid={here.length} aria-label={name} title={s.minimum !== undefined ? `${s.minimum} to ${s.maximum ?? "…"}` : undefined} />
  {:else if kind === "boolean"}
    <input type="checkbox" bind:checked={value as boolean} aria-label={name} />
  {:else if kind === "null"}
    <code>null</code>
  {:else if kind === "const"}
    <code title="always this">{JSON.stringify(s.const)}</code>
  {:else if kind === "enum"}
    <select class:invalid={here.length} aria-label={name}
      value={String(s.enum!.findIndex((e) => JSON.stringify(e) === JSON.stringify(value)))}
      onchange={(e) => (value = structuredClone(s.enum![Number((e.currentTarget as HTMLSelectElement).value)]) as Json)}>
      {#if !s.enum!.some((e) => JSON.stringify(e) === JSON.stringify(value))}<option value="-1">{JSON.stringify(value)} (not one of them)</option>{/if}
      {#each s.enum! as e, i (i)}<option value={String(i)}>{typeof e === "string" ? e : JSON.stringify(e)}</option>{/each}
    </select>
  {:else}
    <input type="text" class="jsonish" class:invalid={openBad || here.length} bind:value={openText} onchange={setOpen} aria-label={name}
      title={kind === "mismatch" ? `the schema wants ${allowed.join(" or ")}: give it as JSON` : "any JSON value (the model reads it; the schema doesn't describe it yet)"} />
    {#if kind === "mismatch" && allowed.length}
      <button class="icon" onclick={() => (value = defaultValue(root, s))} title="replace it with a {allowed[0]}">reset</button>
    {/if}
  {/if}
{/snippet}

{#if kind === "alts"}
  <div class="alts" class:bare bind:this={el} class:flash>
    <div class="row">
      {#if !bare && !nested}<span class="name" title={desc}>{name}</span>{/if}
      <select class="alt" value={String(altIndex)} onchange={(e) => chooseAlt(Number((e.currentTarget as HTMLSelectElement).value))}
        aria-label="{name}'s form" title="which form it has">
        {#each alts! as a, i (i)}<option value={String(i)}>{label(root, a, i)}</option>{/each}
      </select>
      {#if !bare}{@render controls()}{/if}
    </div>
    {@render messages()}
    <div class="alt-body">
      <Self {root} schema={alts![altIndex]} bind:value {path} {errors} {focus} {focusSeq} {name} depth={depth} nested {bare} />
    </div>
  </div>
{:else if obj || arr}
  {#if bare}
    <code class="preview">{preview(value)}</code>
  {:else}
    <div class="node" bind:this={el} class:flash>
      <div class="row head">
        {#if !(nested && inline && open)}
          <button class="toggle" onclick={() => (open = !open)} aria-expanded={open}>{open ? "▾" : "▸"}</button>
        {/if}
        {#if !nested}<span class="name" title={desc}>{name}</span>{/if}
        {#if inline && open}
          <span class="inline">
            {#if obj}
              {#each Object.keys(obj) as k (k)}
                <span class="field" class:unknown={propertySchema(root, s, k) === undefined}>
                  <span class="k" title={(propertySchema(root, s, k)?.description as string) ?? ""}>{k}</span>
                  <Self {root} schema={propertySchema(root, s, k) ?? {}} bind:value={obj[k]!} path={[...path, k]} {errors} {focus} {focusSeq}
                    name={k} depth={depth + 1} bare required={s.required?.includes(k) ?? false} />
                  {#if !(s.required?.includes(k) ?? false)}<button class="icon" title="remove {k}" onclick={() => (value = withoutKey(obj, k))}>✕</button>{/if}
                </span>
              {/each}
              {#if missing.length}
                <select class="add" value="" onchange={(e) => { addKey((e.currentTarget as HTMLSelectElement).value); (e.currentTarget as HTMLSelectElement).value = ""; }} aria-label="add a key">
                  <option value="">+</option>
                  {#each missing as k (k)}<option value={k}>{k}</option>{/each}
                </select>
              {/if}
            {:else if arr}
              {#each arr as _, i (i)}
                <span class="field">
                  <Self {root} schema={itemSchema(root, s, i)} bind:value={arr[i]!} path={[...path, i]} {errors} {focus} {focusSeq} name={String(i)} depth={depth + 1} bare />
                </span>
              {/each}
              {#if !fixedLength}
                <button class="icon" title="add an item" onclick={addItem}>+</button>
                {#if arr.length}<button class="icon" title="remove the last item" onclick={() => arr.pop()}>−</button>{/if}
              {/if}
            {/if}
          </span>
        {:else if !open}
          <code class="preview">{preview(value)}</code>
        {:else}
          <span class="muted">{arr ? `${arr.length} item${arr.length === 1 ? "" : "s"}` : ""}</span>
        {/if}
        {#if under && !open}<span class="badge" title="problems inside">{under}</span>{/if}
        {@render controls()}
      </div>
      {@render messages()}
      {#if open && !inline}
        <div class="children">
          {#if obj}
            {#each Object.keys(obj) as k (k)}
              <Self {root} schema={propertySchema(root, s, k) ?? {}} bind:value={obj[k]!} path={[...path, k]} {errors} {focus} {focusSeq}
                name={k} depth={depth + 1} required={s.required?.includes(k) ?? false} onremove={() => (value = withoutKey(obj, k))} />
            {/each}
            {#if missing.length || freeKeys}
              <div class="row add-row">
                {#if missing.length}
                  <select value="" onchange={(e) => { addKey((e.currentTarget as HTMLSelectElement).value); (e.currentTarget as HTMLSelectElement).value = ""; }} aria-label="add a key">
                    <option value="">+ add a key…</option>
                    {#each missing as k (k)}<option value={k}>{k}</option>{/each}
                  </select>
                {/if}
                {#if freeKeys}
                  <input bind:value={newKey} placeholder="new key" size="12" aria-label="a new key" />
                  <button class="icon" disabled={!newKey || newKey in obj} onclick={() => { addKey(newKey); newKey = ""; }}>+</button>
                {/if}
              </div>
            {/if}
          {:else if arr}
            {#if pages > 1}
              <div class="pager">
                <button class="icon" disabled={page === 0} onclick={() => page--}>‹</button>
                items {page * PAGE}–{Math.min(arr.length, (page + 1) * PAGE) - 1} of {arr.length}
                <button class="icon" disabled={page >= pages - 1} onclick={() => page++}>›</button>
              </div>
            {/if}
            {#each arr.slice(page * PAGE, (page + 1) * PAGE) as _, j (page * PAGE + j)}
              {@const i = page * PAGE + j}
              <Self {root} schema={itemSchema(root, s, i)} bind:value={arr[i]!} path={[...path, i]} {errors} {focus} {focusSeq}
                name={String(i)} depth={depth + 1} required={fixedLength}
                onremove={() => arr.splice(i, 1)}
                onmove={fixedLength ? undefined : (by) => { const k = i + by; if (k >= 0 && k < arr.length) { const t = arr[i]!; arr[i] = arr[k]!; arr[k] = t; } }} />
            {/each}
            {#if !fixedLength}
              <div class="row add-row"><button class="icon" onclick={addItem}>+ add an item</button></div>
            {/if}
          {/if}
        </div>
      {/if}
    </div>
  {/if}
{:else if bare}
  <span class="bare-scalar" bind:this={el} class:flash title={here.join("\n") || desc}>{@render scalar()}</span>
{:else}
  <div class="row leaf" bind:this={el} class:flash>
    {#if !nested}<span class="name" title={desc}>{name}</span>{/if}
    {@render scalar()}
    {@render controls()}
    {#if desc && !nested}<span class="desc">{desc}</span>{/if}
  </div>
  {@render messages()}
{/if}

<style>
  .row {
    display: flex;
    align-items: center;
    gap: 0.35rem;
    min-height: 1.7rem;
    flex-wrap: wrap;
  }
  .name {
    font-weight: 600;
    min-width: 2rem;
    color: var(--text);
  }
  .leaf .name::after,
  .alts .name::after {
    content: ":";
    font-weight: normal;
    color: var(--muted);
  }
  .desc {
    color: var(--muted);
    font-size: 0.8rem;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    max-width: 40rem;
  }
  .children {
    margin-left: 0.55rem;
    padding-left: 0.6rem;
    border-left: 1px solid var(--line);
  }
  .toggle {
    border: none;
    background: none;
    padding: 0 0.1rem;
    width: 1.1rem;
    color: var(--muted);
  }
  .preview {
    color: var(--muted);
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    max-width: 60rem;
  }
  .inline {
    display: inline-flex;
    flex-wrap: wrap;
    gap: 0.25rem 0.6rem;
    align-items: center;
  }
  .field {
    display: inline-flex;
    gap: 0.25rem;
    align-items: center;
  }
  .field .k {
    color: var(--muted);
    font-size: 0.85rem;
  }
  .field.unknown .k {
    color: var(--error);
    text-decoration: line-through;
  }
  .field :global(input[type="number"]) {
    width: 6.5rem;
  }
  input[type="number"] {
    width: 8rem;
  }
  .jsonish {
    font-family: ui-monospace, monospace;
    min-width: 12rem;
  }
  .msg {
    color: var(--error);
    font-size: 0.8rem;
    margin: 0 0 0.2rem 1.5rem;
  }
  .badge {
    background: var(--error);
    color: var(--bg);
    border-radius: 8px;
    font-size: 0.7rem;
    padding: 0 0.4rem;
  }
  .muted {
    color: var(--muted);
    font-size: 0.85rem;
  }
  .pager {
    display: flex;
    gap: 0.4rem;
    align-items: center;
    color: var(--muted);
    font-size: 0.85rem;
    padding: 0.2rem 0;
  }
  .alt {
    font-size: 0.8rem;
    color: var(--muted);
  }
  .alts.bare,
  .alts.bare .row {
    display: inline-flex;
    min-height: 0;
  }
  .alt-body {
    margin-left: 0.2rem;
  }
  .alts.bare .alt-body {
    display: inline;
  }
  .flash {
    background: var(--focus);
    transition: background 0.4s;
    border-radius: 3px;
  }
</style>

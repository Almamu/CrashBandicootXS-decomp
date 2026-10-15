<script lang="ts">
  import { editor } from "../lib/state.svelte.ts";
  import type { Json, Schema } from "../lib/schema.ts";
  import { byPointer, validate } from "../lib/validate.ts";
  import SchemaNode from "./SchemaNode.svelte";

  let mode = $state<"form" | "json">("form");
  let jsonText = $state("");
  let jsonError = $state<string | null>(null);

  const schema = $derived<Schema | undefined>(editor.open ? editor.schemas[editor.open.schema] : undefined);
  const errors = $derived.by(() => {
    if (!editor.open || !schema) return [];
    return validate(editor.open.schema, schema, $state.snapshot(editor.doc as unknown));
  });
  const errorMap = $derived(byPointer(errors));

  function toJson() {
    jsonText = JSON.stringify(editor.doc, null, 2);
    jsonError = null;
    mode = "json";
  }

  function fromJson(): boolean {
    try {
      editor.doc = JSON.parse(jsonText) as Json;
      jsonError = null;
      return true;
    } catch (e) {
      jsonError = (e as Error).message;
      return false;
    }
  }

  function onkeydown(e: KeyboardEvent) {
    if ((e.ctrlKey || e.metaKey) && e.key === "s") {
      e.preventDefault();
      if (mode === "json" && !fromJson()) return;
      if (editor.dirty) void editor.save();
    }
  }
</script>

<svelte:window {onkeydown} />

{#if !editor.open || !schema}
  <div class="empty">
    {#if editor.project && !editor.project.ready}
      <p>{editor.project.title} isn't applied yet.</p>
    {:else}
      <p>Pick a data file on the left. Every file the schemas describe (packages/crashdata/schemas/index.json) is there.</p>
      <p class="muted">The editor checks the value against its schema as you type; the backend checks it again on save, with the model's checks, and refuses what they reject.</p>
    {/if}
  </div>
{:else}
  <div class="file">
    <div class="bar">
      <div class="title">
        <strong>{editor.open.path}</strong>
        <span class="muted">{schema.title ?? editor.open.schema}</span>
      </div>
      <div class="actions">
        {#if errors.length}
          <span class="err-count" title="what the schema rejects (the save would be refused)">{errors.length} problem{errors.length === 1 ? "" : "s"}</span>
        {/if}
        <div class="seg" role="group" aria-label="view">
          <button class:on={mode === "form"} onclick={() => (mode === "json" ? fromJson() && (mode = "form") : null)}>Form</button>
          <button class:on={mode === "json"} onclick={() => mode === "form" && toJson()}>JSON</button>
        </div>
        <button disabled={!editor.dirty || !!editor.busy} onclick={() => editor.revert()}>Revert</button>
        <button class="primary" disabled={!editor.dirty || !!editor.busy}
          onclick={() => (mode === "json" ? fromJson() && editor.save() : editor.save())}>Save</button>
      </div>
    </div>
    {#if schema.description}<p class="desc">{schema.description}</p>{/if}
    {#if errors.length}
      <ul class="errors">
        {#each errors.slice(0, 8) as e (e.pointer + e.message)}
          <li><button class="link" onclick={() => editor.jump(e.pointer)}>{e.pointer || "/"}</button>: {e.message}</li>
        {/each}
        {#if errors.length > 8}<li class="muted">and {errors.length - 8} more</li>{/if}
      </ul>
    {/if}
    {#if mode === "form"}
      {#key editor.open.path}
        <div class="form">
          <SchemaNode root={schema} schema={schema} bind:value={editor.doc} path={[]} errors={errorMap}
            focus={editor.focus} focusSeq={editor.focusSeq} name={editor.open.path.split("/").pop() ?? ""} />
        </div>
      {/key}
    {:else}
      <textarea class="json" bind:value={jsonText} spellcheck="false" oninput={() => (jsonError = null)}></textarea>
      {#if jsonError}<p class="json-error">{jsonError}</p>{/if}
      <p class="muted">Saved in the file's own layout either way: only what changed is written again.</p>
    {/if}
  </div>
{/if}

<style>
  .empty {
    padding: 2rem;
    max-width: 44rem;
  }
  .muted {
    color: var(--muted);
  }
  .file {
    padding: 0.5rem 0.75rem 2rem;
  }
  .bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    gap: 1rem;
    position: sticky;
    top: 0;
    background: var(--bg);
    padding: 0.3rem 0;
    z-index: 2;
    border-bottom: 1px solid var(--line);
  }
  .title {
    display: flex;
    flex-direction: column;
    min-width: 0;
  }
  .title strong {
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  .title span {
    font-size: 0.8rem;
  }
  .actions {
    display: flex;
    gap: 0.4rem;
    align-items: center;
  }
  .seg {
    display: flex;
  }
  .seg button {
    border-radius: 0;
  }
  .seg button:first-child {
    border-radius: 4px 0 0 4px;
  }
  .seg button:last-child {
    border-radius: 0 4px 4px 0;
    border-left: none;
  }
  .seg button.on {
    background: var(--panel);
    font-weight: 600;
  }
  .err-count {
    color: var(--error);
    font-size: 0.85rem;
  }
  .desc {
    color: var(--muted);
    margin: 0.4rem 0;
    font-size: 0.85rem;
  }
  .errors {
    margin: 0.3rem 0;
    padding: 0.3rem 0.3rem 0.3rem 1.4rem;
    background: var(--error-bg);
    color: var(--error);
    font-size: 0.85rem;
    border-radius: 4px;
  }
  .form {
    margin-top: 0.5rem;
  }
  textarea.json {
    width: 100%;
    min-height: 60vh;
    margin-top: 0.5rem;
  }
  .json-error {
    color: var(--error);
  }
</style>

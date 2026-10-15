<script lang="ts">
  import type { FileEntry } from "@crash/schema-types";
  import { editor } from "../lib/state.svelte.ts";

  let filter = $state("");

  // index.json's order: one group per schema, with the files it describes
  const groups = $derived.by(() => {
    const q = filter.trim().toLowerCase();
    const prefix = `games/${editor.project?.game ?? ""}/`;
    const out: { schema: string; title: string; globs: string[]; files: FileEntry[] }[] = [];
    for (const s of editor.index?.schemas ?? []) {
      const files = editor.files.filter((f) => f.schema === s.schema && (!q || f.path.toLowerCase().includes(q)));
      if (!files.length) continue;
      const title = (editor.schemas[s.schema]?.title as string | undefined) ?? s.schema;
      out.push({ schema: s.schema, title, globs: s.fileMatch, files: files.map((f) => ({ ...f, path: f.path })) });
    }
    return { prefix, out };
  });

  function short(path: string, prefix: string) {
    return path.startsWith(prefix) ? path.slice(prefix.length) : path;
  }
</script>

<div class="browser">
  <input type="search" bind:value={filter} placeholder="Filter the files" aria-label="filter the files" />
  {#if editor.project && !editor.project.ready}
    <p class="note">This mod isn't applied yet: apply it to edit its tree.</p>
  {/if}
  {#each groups.out as g (g.schema)}
    <details open={!!filter || g.files.length <= 12 || g.files.some((f) => f.path === editor.open?.path)}>
      <summary title="{g.schema}: {g.globs.join(', ')}">
        {g.title} <span class="count">{g.files.length}</span>
      </summary>
      <ul>
        {#each g.files as f (f.path)}
          <li>
            <button class:current={editor.open?.path === f.path} title={f.path} onclick={() => editor.openFile(f.path)}>
              {short(f.path, groups.prefix)}
            </button>
          </li>
        {/each}
      </ul>
    </details>
  {/each}
</div>

<style>
  .browser {
    padding: 0.5rem;
  }
  input[type="search"] {
    width: 100%;
    margin-bottom: 0.4rem;
  }
  summary {
    cursor: pointer;
    padding: 0.15rem 0;
    font-weight: 600;
    font-size: 0.85rem;
  }
  .count {
    color: var(--muted);
    font-weight: normal;
  }
  ul {
    list-style: none;
    margin: 0 0 0.3rem 0;
    padding: 0 0 0 0.6rem;
  }
  li button {
    border: none;
    background: none;
    padding: 0.05rem 0.25rem;
    text-align: left;
    width: 100%;
    font-size: 0.85rem;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    border-radius: 3px;
  }
  li button:hover {
    background: var(--panel);
  }
  li button.current {
    background: var(--accent);
    color: var(--accent-text);
  }
  .note {
    color: var(--muted);
    font-size: 0.85rem;
  }
</style>

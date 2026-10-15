<script lang="ts">
  import type { Diagnostic } from "@crash/schema-types";
  import { editor } from "../lib/state.svelte.ts";

  let showWarnings = $state(true);
  let showOutput = $state(false);

  const set = $derived(editor.diagnostics);
  const shown = $derived((set?.diagnostics ?? []).filter((d) => showWarnings || d.severity === "error"));
  const known = $derived(new Set(editor.files.map((f) => f.path)));

  function where(d: Diagnostic): string {
    let s = d.file ?? "";
    if (d.line !== null) s += `:${d.line}` + (d.col !== null ? `:${d.col}` : "");
    return s;
  }

  async function go(d: Diagnostic) {
    if (!d.file || !known.has(d.file)) return;
    await editor.openFile(d.file, d.pointer ?? "");
  }
</script>

<div class="diags">
  <div class="bar">
    <strong>Diagnostics</strong>
    {#if set}
      <span class:ok={set.ok} class:bad={!set.ok}>{set.title}</span>
      <span class="muted">{set.at.toLocaleTimeString()}</span>
    {:else}
      <span class="muted">"Check the data" runs <code>make data-check</code> on the project (tools/data_check.py --json); a refused save shows why here.</span>
    {/if}
    <div class="spacer"></div>
    <label><input type="checkbox" bind:checked={showWarnings} /> warnings</label>
    {#if set?.output}<label><input type="checkbox" bind:checked={showOutput} /> output</label>{/if}
  </div>
  {#if shown.length}
    <table>
      <tbody>
        {#each shown as d, i (i)}
          <tr class={d.severity}>
            <td class="sev">{d.severity === "error" ? "●" : "▲"}</td>
            <td class="where">
              {#if d.file && known.has(d.file)}
                <button class="link" onclick={() => go(d)} title="open it there">{where(d)}</button>
              {:else}
                {where(d)}
              {/if}
            </td>
            <td class="ptr"><code>{d.pointer ?? ""}</code></td>
            <td class="code">{d.code}</td>
            <td class="message">{d.message}</td>
          </tr>
        {/each}
      </tbody>
    </table>
  {:else if set}
    <p class="muted pad">No problems{showWarnings ? "" : " (warnings hidden)"}.</p>
  {/if}
  {#if showOutput && set?.output}<pre>{set.output}</pre>{/if}
</div>

<style>
  .diags {
    font-size: 0.85rem;
  }
  .bar {
    display: flex;
    gap: 0.6rem;
    align-items: center;
    padding: 0.3rem 0.75rem;
    position: sticky;
    top: 0;
    background: var(--panel);
    border-bottom: 1px solid var(--line);
  }
  .spacer {
    flex: 1;
  }
  .ok {
    color: var(--ok);
  }
  .bad {
    color: var(--error);
  }
  .muted {
    color: var(--muted);
  }
  .pad {
    padding: 0 0.75rem;
  }
  table {
    border-collapse: collapse;
    width: 100%;
  }
  td {
    padding: 0.15rem 0.5rem;
    vertical-align: top;
    border-bottom: 1px solid var(--line);
  }
  tr.error .sev {
    color: var(--error);
  }
  tr.warning .sev {
    color: var(--warning);
  }
  .where {
    white-space: nowrap;
  }
  .code {
    color: var(--muted);
    white-space: nowrap;
  }
  pre {
    margin: 0;
    padding: 0.5rem 0.75rem;
    white-space: pre-wrap;
  }
</style>

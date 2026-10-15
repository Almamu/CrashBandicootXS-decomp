<script lang="ts">
  import { editor } from "../lib/state.svelte.ts";

  let adding = $state(false);
  let modDir = $state("");

  function onchange(e: Event) {
    const select = e.currentTarget as HTMLSelectElement;
    if (select.value === "+mod") {
      adding = true;
      select.value = editor.projectId;
      return;
    }
    void editor.selectProject(select.value);
    if (select.value !== editor.projectId) select.value = editor.projectId;
  }

  async function add(e: SubmitEvent) {
    e.preventDefault();
    await editor.addMod(modDir.trim());
    if (!editor.error) {
      adding = false;
      modDir = "";
    }
  }
</script>

<div class="picker">
  <label>
    Project
    <select value={editor.projectId} {onchange} aria-label="the project">
      <optgroup label="Games">
        {#each editor.projects.filter((p) => p.kind === "game") as p (p.id)}
          <option value={p.id}>{p.title}</option>
        {/each}
      </optgroup>
      <optgroup label="Mods (edited in their applied tree)">
        {#each editor.projects.filter((p) => p.kind === "mod") as p (p.id)}
          <option value={p.id}>{p.mod} → {p.root}{p.ready ? "" : " (not applied)"}</option>
        {/each}
        <option value="+mod">Another mod folder…</option>
      </optgroup>
    </select>
  </label>
  {#if editor.project?.kind === "mod"}
    <button onclick={() => editor.applyMod()} disabled={!!editor.busy}
      title="tools/apply_mod.py: copy the tree to {editor.project.root} and apply the mod there (make mod)">
      {editor.project.ready ? "Apply again" : "Apply the mod"}
    </button>
  {/if}
  {#if adding}
    <form onsubmit={add}>
      <input bind:value={modDir} placeholder="mods/my_mod, or an absolute path" size="28" aria-label="the mod's folder" />
      <button type="submit" disabled={!modDir.trim()}>Add</button>
      <button type="button" onclick={() => (adding = false)}>Cancel</button>
    </form>
  {/if}
</div>

<style>
  .picker {
    display: flex;
    align-items: center;
    gap: 0.5rem;
  }
  label {
    display: flex;
    gap: 0.35rem;
    align-items: center;
    color: var(--muted);
  }
  form {
    display: flex;
    gap: 0.3rem;
  }
</style>

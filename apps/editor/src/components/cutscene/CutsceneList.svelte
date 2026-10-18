<script lang="ts">
  // The cutscenes, in id order (PlayCutscene's argument), with what plays
  // each; a new one at the end, and the last one deleted when nothing plays it.
  import { cutscenes as cs } from "../../lib/cutscene/state.svelte.ts";
  import * as M from "../../lib/cutscene/model.ts";

  let adding = $state(false);
  let constant = $state("CUTSCENE_");
  let about = $state("");

  const refs = (c: string) => cs.view?.references[c] ?? [];

  function add() {
    const first = cs.record?.name ?? cs.doc.slides[0]?.name ?? "";
    if (cs.apply(`New cutscene ${constant}`, (s) => M.addCutscene(s, constant.trim(), first, about.trim() || undefined))) {
      cs.select(cs.doc.cutscenes.length - 1);
      adding = false;
      constant = "CUTSCENE_";
      about = "";
    }
  }

  function remove(ci: number) {
    const c = cs.doc.cutscenes[ci];
    if (!c || !confirm(`Delete ${c.constant}?`)) return;
    cs.apply(`Delete ${c.constant}`, (s) => M.deleteCutscene(s, ci, refs(c.constant)));
  }
</script>

<nav aria-label="cutscenes">
  <h2>Cutscenes</h2>
  <ol>
    {#each cs.doc.cutscenes as c, ci (ci)}
      {@const r = refs(c.constant)}
      <li>
        <button class:on={ci === cs.cutscene} onclick={() => cs.select(ci)} title={r.length ? `played by ${r.join(", ")}` : "nothing plays it"}>
          <span class="id">{ci}</span>
          <span class="name">{c.constant.replace(/^CUTSCENE_/, "").toLowerCase().replace(/_/g, " ")}</span>
          <span class="count">{c.slides.length}</span>
          {#if c.about}<span class="about">{c.about}</span>{/if}
          {#if !r.length}<span class="about unused">nothing plays it</span>{/if}
        </button>
      </li>
    {/each}
  </ol>
  {#if adding}
    <div class="new">
      <label>Constant <input bind:value={constant} spellcheck="false" /></label>
      <label>About <input bind:value={about} placeholder="what plays it" /></label>
      <p class="hint">Its first slide shows the selected slide's record, without text. Something must play it: a level's <code>"on_clear"</code> (levels.json) names its id, {cs.doc.cutscenes.length}.</p>
      <div class="row">
        <button class="primary" disabled={!M.CONSTANT.test(constant)} onclick={add}>Add</button>
        <button onclick={() => (adding = false)}>Cancel</button>
      </div>
    </div>
  {:else}
    <div class="row">
      <button onclick={() => (adding = true)}>New cutscene…</button>
      {#if cs.doc.cutscenes.length}
        {@const last = cs.doc.cutscenes.length - 1}
        {@const why = M.cannotDelete(cs.state, last, refs(cs.doc.cutscenes[last]!.constant))}
        <button disabled={!!why} title={why ?? `delete ${cs.doc.cutscenes[last]!.constant}`} onclick={() => remove(last)}>Delete the last</button>
      {/if}
    </div>
  {/if}
</nav>

<style>
  nav {
    padding: 0.5rem;
  }
  h2 {
    font-size: 0.95rem;
    margin: 0 0 0.4rem;
  }
  ol {
    list-style: none;
    margin: 0;
    padding: 0;
  }
  li button {
    display: grid;
    grid-template-columns: 1.6rem 1fr auto;
    width: 100%;
    text-align: left;
    border: none;
    border-radius: 4px;
    background: transparent;
    padding: 0.25rem 0.35rem;
    gap: 0 0.3rem;
  }
  li button:hover {
    background: var(--panel);
  }
  li button.on {
    background: var(--accent);
    color: var(--accent-text);
  }
  .id,
  .count {
    font-variant-numeric: tabular-nums;
    opacity: 0.75;
  }
  .about {
    grid-column: 2 / 4;
    font-size: 0.75rem;
    opacity: 0.75;
  }
  .unused {
    font-style: italic;
  }
  .row {
    display: flex;
    gap: 0.35rem;
    margin-top: 0.5rem;
    flex-wrap: wrap;
  }
  .new label {
    display: block;
    font-size: 0.85rem;
    margin-top: 0.4rem;
  }
  .new input {
    width: 100%;
  }
  .hint {
    font-size: 0.75rem;
    color: var(--muted);
  }
</style>

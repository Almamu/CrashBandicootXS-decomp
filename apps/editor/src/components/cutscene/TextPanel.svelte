<script lang="ts">
  // The slide's page of text: its strings, in the six languages side by side
  // (the English is the template's msgid, the others each .po's msgstr). Each
  // is checked as it is typed, as the save and `make text-check` check it
  // (the game's encoding, the small font's characters), and measured as the
  // game draws it: its screens (three lines of the 228-pixel box each, a key
  // press between them) and its widest line.
  import { cutscenes as cs } from "../../lib/cutscene/state.svelte.ts";
  import * as M from "../../lib/cutscene/model.ts";

  const names: Record<M.Lang, string> = { en: "English", fr: "French", de: "German", es: "Spanish", it: "Italian", nl: "Dutch" };
  const langs = $derived(M.LANGS.filter((l) => cs.shown[l]));

  function edit(page: string, i: number, lang: M.Lang, text: string) {
    cs.apply(`Edit ${page}.${i} (${names[lang]})`, (s) => M.setString(s, page, i, lang, text), `${page}.${i}.${lang}`);
  }

  function focus(i: number, lang: M.Lang) {
    if (cs.string !== i || cs.language !== lang) cs.screen = 0;
    cs.string = i;
    cs.language = lang;
  }
</script>

{#if cs.ref && cs.view}
  <div class="text">
    <div class="head">
      <strong>{cs.ref.text ?? "No text"}</strong>
      {#if cs.ref.text}
        <span class="hint">the small font, word-wrapped into {cs.view.box.w}x{cs.view.box.h} px (3 lines) at ({cs.view.box.x}, {cs.view.box.y}); longer text goes on after a key press</span>
      {/if}
      <span class="spacer"></span>
      {#each M.LANGS as l (l)}
        <label class="lang"><input type="checkbox" bind:checked={cs.shown[l]} /> {l}</label>
      {/each}
    </div>
    {#if cs.ref.text && cs.page}
      {@const page = cs.ref.text}
      <div class="grid" style:grid-template-columns="2.2rem repeat({langs.length}, minmax(12rem, 1fr))">
        <span></span>
        {#each langs as l (l)}<span class="col">{names[l]}{l === "en" ? " (the template)" : ""}</span>{/each}
        {#each cs.page as s, i (i)}
          <div class="ops">
            <span class="n">{i + 1}</span>
            <button class="mini" title="move it earlier" disabled={i === 0} onclick={() => cs.apply("Move a string", (st) => M.moveString(st, page, i, i - 1))}>↑</button>
            <button class="mini" title="move it later" disabled={i === cs.page.length - 1} onclick={() => cs.apply("Move a string", (st) => M.moveString(st, page, i, i + 1))}>↓</button>
            <button class="mini" title="a new string after it (shown after a key press)" onclick={() => cs.apply("Add a string", (st) => M.addString(st, page, i + 1))}>+</button>
            <button class="mini" title="delete the string" disabled={cs.page.length < 2} onclick={() => cs.apply("Delete a string", (st) => M.deleteString(st, page, i))}>×</button>
          </div>
          {#each langs as l (l)}
            {@const info = cs.info(s[l])}
            {@const bad = info?.problems.some((p) => p.severity === "error")}
            <div class="cell" class:sel={cs.string === i && cs.language === l}>
              <textarea
                rows="3"
                class:bad
                spellcheck="false"
                lang={l}
                value={s[l]}
                onfocus={() => focus(i, l)}
                oninput={(e) => {
                  focus(i, l);
                  edit(page, i, l, e.currentTarget.value);
                }}
              ></textarea>
              {#if info}
                <div class="meta">
                  <span title="key presses: each screen is three lines of the box">{info.screens} screen{info.screens === 1 ? "" : "s"}</span>
                  <span title="the widest line, of the box's {cs.view.box.w} pixels">{info.widest}/{cs.view.box.w} px</span>
                  {#if l !== "en" && s[l] === s.en}<span class="same" title="the same as the English">= en</span>{/if}
                </div>
                {#each info.problems as p, k (k)}<div class="problem {p.severity}">{p.message}</div>{/each}
              {/if}
            </div>
          {/each}
        {/each}
      </div>
    {:else}
      <p class="hint">This slide shows its picture only. Tick "Text with it" on the right to give it a page.</p>
    {/if}
  </div>
{/if}

<style>
  .text {
    padding: 0.4rem 0.5rem;
    font-size: 0.85rem;
  }
  .head {
    display: flex;
    gap: 0.5rem;
    align-items: baseline;
    flex-wrap: wrap;
    margin-bottom: 0.35rem;
  }
  .spacer {
    flex: 1;
  }
  .lang {
    font-size: 0.78rem;
  }
  .hint {
    color: var(--muted);
    font-size: 0.78rem;
  }
  .grid {
    display: grid;
    gap: 0.3rem 0.4rem;
    overflow-x: auto;
  }
  .col {
    font-size: 0.75rem;
    color: var(--muted);
  }
  .ops {
    display: flex;
    flex-direction: column;
    gap: 2px;
    align-items: center;
  }
  .n {
    font-weight: 600;
  }
  .mini {
    padding: 0 0.3rem;
    font-size: 0.75rem;
    line-height: 1.2;
  }
  .cell {
    border-radius: 4px;
    padding: 2px;
  }
  .cell.sel {
    outline: 2px solid var(--accent);
  }
  textarea {
    width: 100%;
    font: inherit;
    font-size: 0.82rem;
    resize: vertical;
    background: var(--input);
    color: var(--text);
    border: 1px solid var(--line);
    border-radius: 3px;
  }
  textarea.bad {
    border-color: var(--error);
    background: var(--error-bg);
  }
  .meta {
    display: flex;
    gap: 0.6rem;
    font-size: 0.72rem;
    color: var(--muted);
    font-variant-numeric: tabular-nums;
  }
  .same {
    font-style: italic;
  }
  .problem {
    font-size: 0.72rem;
  }
  .problem.error {
    color: var(--error);
  }
  .problem.warning {
    color: var(--warning);
  }
</style>

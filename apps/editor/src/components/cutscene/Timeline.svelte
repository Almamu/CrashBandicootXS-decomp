<script lang="ts">
  // The cutscene's slides in order: a thumbnail each (its picture, its
  // first screen of text), dragged or moved to reorder; add, duplicate,
  // delete. A record or page other slides show too is marked.
  import { cutscenes as cs } from "../../lib/cutscene/state.svelte.ts";
  import * as M from "../../lib/cutscene/model.ts";
  import { encode, screens } from "../../lib/cutscene/text.ts";
  import Screen from "./Screen.svelte";

  let dragFrom = $state<number | null>(null);
  let dropAt = $state<number | null>(null);

  function first(ref: M.SlideRef) {
    const v = cs.view;
    const page = ref.text ? cs.state.pages[ref.text] : undefined;
    if (!v || !page?.[0]) return [];
    return screens(encode(page[0][cs.language]).bytes, v.font, v.box)[0] ?? [];
  }

  function move(from: number, to: number) {
    if (cs.apply("Move a slide", (s) => M.moveSlide(s, cs.cutscene, from, to))) cs.selectSlide(to);
  }

  function add() {
    const at = cs.slide + 1;
    if (cs.apply("Add a slide", (s) => M.addSlide(s, cs.cutscene, at, { like: cs.record, text: true }))) cs.selectSlide(at);
  }

  function duplicate() {
    const at = cs.slide + 1;
    if (cs.apply("Duplicate a slide", (s) => M.duplicateSlide(s, cs.cutscene, cs.slide))) cs.selectSlide(at);
  }

  function remove() {
    cs.apply("Delete a slide", (s) => M.deleteSlide(s, cs.cutscene, cs.slide));
  }

  function drop(e: DragEvent, i: number) {
    e.preventDefault();
    if (dragFrom !== null && dragFrom !== i) move(dragFrom, i);
    dragFrom = dropAt = null;
  }
</script>

{#if cs.current && cs.view}
  {@const v = cs.view}
  <div class="timeline">
    <ol aria-label="the slides">
      {#each cs.current.slides as ref, si (si)}
        {@const rec = M.record(cs.doc, ref.slide)}
        {@const shared = M.recordUsers(cs.doc, ref.slide).length > 1}
        {@const sharedPage = ref.text ? M.pageUsers(cs.doc, ref.text).length > 1 : false}
        <li class:drop={dropAt === si && dragFrom !== si}>
          <button
            class="slide"
            class:on={si === cs.slide}
            draggable="true"
            ondragstart={() => (dragFrom = si)}
            ondragover={(e) => {
              e.preventDefault();
              dropAt = si;
            }}
            ondragleave={() => dropAt === si && (dropAt = null)}
            ondrop={(e) => drop(e, si)}
            ondragend={() => (dragFrom = dropAt = null)}
            onclick={() => cs.selectSlide(si)}
            title="{ref.slide}{ref.text ? ` — ${ref.text}` : ' — no text'}{shared ? ' (record shared)' : ''}{sharedPage ? ' (page shared)' : ''}"
          >
            <Screen picture={cs.picture(rec?.picture)} font={v.font} glyphs={first(ref)} width={120} label="slide {si + 1}" />
            <span class="cap">
              <span class="n">{si + 1}</span>
              <span class="name">{ref.slide}</span>
              {#if rec && rec.wait > 0}<span class="badge" title="waits {rec.wait} frames">{(rec.wait / 60).toFixed(1)}s</span>{/if}
              {#if shared}<span class="badge" title="other slides show this record">⇄</span>{/if}
              {#if !ref.text}<span class="badge" title="no text">∅</span>{/if}
            </span>
          </button>
        </li>
      {/each}
    </ol>
    <div class="ops">
      <button title="move the slide earlier" disabled={cs.slide === 0} onclick={() => move(cs.slide, cs.slide - 1)}>◀</button>
      <button title="move the slide later" disabled={cs.slide >= cs.current.slides.length - 1} onclick={() => move(cs.slide, cs.slide + 1)}>▶</button>
      <button title="a new slide after this one: a copy of its record, with a new page of text" onclick={add}>Add</button>
      <button title="the same record (shared) and a copy of its text, after it" onclick={duplicate}>Duplicate</button>
      <button title="delete the slide (its record and page go too if no other slide shows them)" disabled={cs.current.slides.length < 2} onclick={remove}>Delete</button>
    </div>
  </div>
{/if}

<style>
  .timeline {
    display: flex;
    align-items: stretch;
    gap: 0.5rem;
    border-bottom: 1px solid var(--line);
    background: var(--panel);
    padding: 0.4rem 0.5rem;
    min-width: 0;
  }
  ol {
    display: flex;
    gap: 0.4rem;
    list-style: none;
    margin: 0;
    padding: 0 0 0.2rem;
    overflow-x: auto;
    flex: 1;
    min-width: 0;
  }
  li {
    border-left: 3px solid transparent;
  }
  li.drop {
    border-left-color: var(--accent);
  }
  .slide {
    padding: 3px;
    border: 2px solid transparent;
    background: var(--bg);
  }
  .slide.on {
    border-color: var(--accent);
  }
  .cap {
    display: flex;
    gap: 0.25rem;
    align-items: center;
    font-size: 0.72rem;
    max-width: 120px;
    margin-top: 2px;
  }
  .name {
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    flex: 1;
    text-align: left;
  }
  .n {
    font-weight: 600;
  }
  .badge {
    color: var(--muted);
  }
  .ops {
    display: flex;
    flex-direction: column;
    gap: 0.25rem;
    font-size: 0.85rem;
  }
</style>

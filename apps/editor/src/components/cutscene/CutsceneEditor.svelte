<script lang="ts">
  // The cutscene editor (docs/editors.md, "The cutscene editor"): the
  // cutscenes on the left, the selected one's slides along the top, the slide
  // as the game shows it (its picture and a screen of its text) in the
  // middle with its text in the six languages under it, and the slide's
  // record on the right.
  import { onMount } from "svelte";
  import { editor } from "../../lib/state.svelte.ts";
  import { cutscenes as cs } from "../../lib/cutscene/state.svelte.ts";
  import * as M from "../../lib/cutscene/model.ts";
  import CutsceneList from "./CutsceneList.svelte";
  import Screen from "./Screen.svelte";
  import SlidePanel from "./SlidePanel.svelte";
  import TextPanel from "./TextPanel.svelte";
  import Timeline from "./Timeline.svelte";

  $effect(() => {
    void cs.syncProject(editor.projectId, !!editor.project?.ready);
  });

  const steps = $derived(cs.steps);
  const at = $derived(Math.max(0, steps.findIndex((s) => s.string === cs.string && s.screen === cs.screen)));
  const step = $derived(steps[at] ?? steps[0]!);

  function go(d: number) {
    const k = at + d;
    if (k >= 0 && k < steps.length) {
      cs.string = steps[k]!.string;
      cs.screen = steps[k]!.screen;
      return;
    }
    // past the slide's text: the next or previous slide, as a key press goes on
    const n = cs.current?.slides.length ?? 0;
    const si = cs.slide + (d > 0 ? 1 : -1);
    if (si < 0 || si >= n) return;
    cs.selectSlide(si);
    if (d < 0) {
      const last = cs.steps[cs.steps.length - 1]!;
      cs.string = last.string;
      cs.screen = last.screen;
    }
  }

  function onkeydown(e: KeyboardEvent) {
    if (!cs.view) return;
    const t = e.target;
    const typing = t instanceof HTMLInputElement || t instanceof HTMLTextAreaElement || t instanceof HTMLSelectElement;
    const mod = e.ctrlKey || e.metaKey;
    if (mod && e.key.toLowerCase() === "s") {
      e.preventDefault();
      if (cs.dirty) void cs.save();
    } else if (mod && !typing && e.key.toLowerCase() === "z") {
      e.preventDefault();
      if (e.shiftKey) cs.redo();
      else cs.undo();
    } else if (mod && !typing && e.key.toLowerCase() === "y") {
      e.preventDefault();
      cs.redo();
    } else if (!mod && !typing && (e.key === "ArrowRight" || e.key === "ArrowLeft")) {
      e.preventDefault();
      go(e.key === "ArrowRight" ? 1 : -1);
    }
  }

  onMount(() => {
    const before = (e: BeforeUnloadEvent) => {
      if (cs.dirty) e.preventDefault();
    };
    window.addEventListener("beforeunload", before);
    return () => window.removeEventListener("beforeunload", before);
  });
</script>

<svelte:window {onkeydown} />

{#if cs.unavailable}
  <div class="empty">
    <p>{cs.unavailable}.</p>
    <p>The cutscene editor edits Crash Bandicoot XS's cutscenes (games/xs/data/cutscenes.json) and a mod's. N-Tranced's cutscenes are still in its ROM data (docs/n_tranced.md).</p>
  </div>
{:else if !cs.view}
  <div class="empty"><p>{cs.loading ? "Reading the cutscenes…" : editor.project?.ready ? "" : "Apply the mod first."}</p></div>
{:else}
  {@const v = cs.view}
  <div class="ce">
    <aside class="left"><CutsceneList /></aside>
    <section class="middle">
      <div class="toolbar">
        <strong>{cs.current?.constant}</strong>
        <span class="hint">{cs.current?.slides.length} slides, {cs.cutsceneSteps} key presses in {cs.language}</span>
        <span class="spacer"></span>
        <button disabled={!cs.canUndo || !!editor.busy} title="undo {cs.undoLabel} (ctrl+z)" onclick={() => cs.undo()}>Undo</button>
        <button disabled={!cs.canRedo || !!editor.busy} title="redo {cs.redoLabel} (ctrl+shift+z)" onclick={() => cs.redo()}>Redo</button>
        <button disabled={cs.play?.running} title="run the PC port: it plays this cutscene at boot, in {cs.language} (the saved data)" onclick={() => cs.runPreview()}>Play this cutscene</button>
        <button disabled={!cs.dirty || !!editor.busy} onclick={() => cs.revert()}>Revert</button>
        <button class="primary" disabled={!cs.dirty || !!editor.busy} onclick={() => cs.save()} title="save (ctrl+s): cutscenes.json, the text and the pictures, checked like any save (and the text's checks), all or nothing">Save</button>
      </div>
      <Timeline />
      <div class="stage">
        <div class="view">
          <Screen picture={cs.picture(cs.record?.picture)} font={v.font} glyphs={step.glyphs} width={240 * cs.zoom} label="slide {cs.slide + 1}, as the game shows it" />
          <div class="nav">
            <button title="the previous screen (←)" onclick={() => go(-1)}>◀</button>
            <span>
              slide {cs.slide + 1}/{cs.current?.slides.length}
              {#if cs.page}— string {step.string + 1}/{cs.page.length}, screen {step.screen + 1} ({at + 1}/{steps.length} on this slide){:else}— no text{/if}
            </span>
            <button title="the next screen, as A does (→)" onclick={() => go(1)}>▶</button>
            <select bind:value={cs.language} title="the text's language">
              {#each v.languages as l (l.code)}<option value={l.code}>{l.name}</option>{/each}
            </select>
            <select bind:value={cs.zoom} title="zoom">
              <option value={1}>1x</option>
              <option value={2}>2x</option>
              <option value={3}>3x</option>
            </select>
          </div>
          {#if cs.record}
            {@const r = cs.record}
            <p class="hint">
              {r.picture.replace(/^cutscenes\//, "")} · fades in from {r.fade === 1 ? "white" : r.fade === 0 ? "black" : `flags ${r.fade}`}{r.fade_after === -1 ? "" : `, out to ${r.fade_after === 1 ? "white" : r.fade_after === 0 ? "black" : `flags ${r.fade_after}`}`}
              · {r.wait ? `${r.wait} frames` : r.buttons ? "waits for A" : "no wait"} · {r.song.replace(/^SONG_/, "").toLowerCase()}{r.sfx !== "SFX_NONE" ? `, ${r.sfx.replace(/^SFX_/, "").toLowerCase()}` : ""}
              {#if M.recordUsers(cs.doc, r.name).length > 1}· record shared{/if}
            </p>
          {/if}
        </div>
        <TextPanel />
      </div>
    </section>
    <aside class="right"><SlidePanel /></aside>
  </div>
{/if}

{#if cs.play}
  <div class="backdrop" role="presentation" onclick={(e) => e.target === e.currentTarget && !cs.play?.running && (cs.play = null)}>
    <div class="dialog" role="dialog" aria-modal="true" aria-label="Play this cutscene">
      <h2>Play {cs.current?.constant}</h2>
      <pre class:bad={!cs.play.ok}>{cs.play.text}</pre>
      <div class="shots">
        {#each cs.play.images as src, k (src)}<img {src} alt="key press {k + 1}, from the PC port" />{/each}
      </div>
      <div class="actions"><button disabled={cs.play.running} onclick={() => (cs.play = null)}>Close</button></div>
    </div>
  </div>
{/if}

<style>
  .ce {
    display: grid;
    grid-template-columns: minmax(12rem, 16rem) 1fr minmax(18rem, 22rem);
    height: 100%;
    min-height: 0;
  }
  .left,
  .right {
    overflow: auto;
    min-height: 0;
  }
  .left {
    border-right: 1px solid var(--line);
  }
  .right {
    border-left: 1px solid var(--line);
  }
  .middle {
    display: grid;
    grid-template-rows: auto auto 1fr;
    min-width: 0;
    min-height: 0;
  }
  .toolbar {
    display: flex;
    flex-wrap: wrap;
    gap: 0.35rem;
    align-items: center;
    padding: 0.3rem 0.5rem;
    border-bottom: 1px solid var(--line);
    background: var(--panel);
    font-size: 0.85rem;
  }
  .spacer {
    flex: 1;
  }
  .hint {
    color: var(--muted);
    font-size: 0.8rem;
  }
  .stage {
    overflow: auto;
    min-height: 0;
  }
  .view {
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 0.6rem;
    gap: 0.35rem;
  }
  .nav {
    display: flex;
    gap: 0.4rem;
    align-items: center;
    font-size: 0.85rem;
    font-variant-numeric: tabular-nums;
  }
  .empty {
    padding: 2rem;
    max-width: 40rem;
    color: var(--muted);
  }
  .backdrop {
    position: fixed;
    inset: 0;
    background: rgb(0 0 0 / 40%);
    display: grid;
    place-items: center;
    z-index: 10;
  }
  .dialog {
    background: var(--bg);
    border: 1px solid var(--line);
    border-radius: 6px;
    padding: 1rem 1.25rem;
    width: min(60rem, 94vw);
    max-height: 92vh;
    overflow: auto;
  }
  h2 {
    margin: 0 0 0.4rem;
    font-size: 1.05rem;
  }
  pre {
    white-space: pre-wrap;
    font-size: 0.8rem;
    background: var(--panel);
    padding: 0.5rem;
    border-radius: 4px;
  }
  pre.bad {
    color: var(--error);
  }
  .shots {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(240px, 1fr));
    gap: 0.4rem;
  }
  .shots img {
    image-rendering: pixelated;
    width: 100%;
  }
  .actions {
    display: flex;
    justify-content: flex-end;
    margin-top: 0.5rem;
  }
</style>

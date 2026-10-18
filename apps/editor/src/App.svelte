<script lang="ts">
  import { onMount } from "svelte";
  import Diagnostics from "./components/Diagnostics.svelte";
  import FileBrowser from "./components/FileBrowser.svelte";
  import FileEditor from "./components/FileEditor.svelte";
  import CutsceneEditor from "./components/cutscene/CutsceneEditor.svelte";
  import LevelEditor from "./components/level/LevelEditor.svelte";
  import MusicEditor from "./components/music/MusicEditor.svelte";
  import Preview from "./components/Preview.svelte";
  import ProjectPicker from "./components/ProjectPicker.svelte";
  import { desktop, hashWith } from "./lib/api.ts";
  import { editor } from "./lib/state.svelte.ts";

  let previewOpen = $state(false);
  let checkText = $state(true);
  // the level editor, the cutscene editor, the music editor, or the data files' forms
  type Mode = "levels" | "cutscenes" | "music" | "files";
  const asked = new URLSearchParams(location.hash.slice(1)).get("mode");
  let mode = $state<Mode>(asked === "files" || asked === "cutscenes" || asked === "music" ? asked : "levels");

  function setMode(m: Mode) {
    mode = m;
    history.replaceState(null, "", hashWith("mode", m));
  }

  onMount(() => {
    void editor.init();
    const beforeUnload = (e: BeforeUnloadEvent) => {
      if (editor.dirty) e.preventDefault();
    };
    window.addEventListener("beforeunload", beforeUnload);
    return () => window.removeEventListener("beforeunload", beforeUnload);
  });
</script>

<div class="app">
  <header>
    <h1>Crash data editor</h1>
    <ProjectPicker />
    <div class="modes" role="tablist" aria-label="editor">
      <button role="tab" aria-selected={mode === "levels"} class:on={mode === "levels"} onclick={() => setMode("levels")}>Level editor</button>
      <button role="tab" aria-selected={mode === "cutscenes"} class:on={mode === "cutscenes"} onclick={() => setMode("cutscenes")}>Cutscenes</button>
      <button role="tab" aria-selected={mode === "music"} class:on={mode === "music"} onclick={() => setMode("music")}>Music</button>
      <button role="tab" aria-selected={mode === "files"} class:on={mode === "files"} onclick={() => setMode("files")}>Data files</button>
    </div>
    <div class="spacer"></div>
    {#if editor.busy}<span class="busy" role="status">{editor.busy}…</span>{/if}
    <label class="check-text" title="also check the translations (needs gettext's msgfmt and msgcmp)">
      <input type="checkbox" bind:checked={checkText} /> text
    </label>
    <button disabled={!editor.project?.ready || !!editor.busy} onclick={() => editor.check(checkText)}>Check the data</button>
    <button disabled={!editor.project?.ready} onclick={() => (previewOpen = true)}>Preview…</button>
    {#if desktop?.checkout}<span class="checkout" title="the checkout this desktop app edits">{desktop.checkout}</span>{/if}
  </header>
  <div class="error-slot">
    {#if editor.error}
      <div class="error-bar" role="alert">
        {editor.error}
        <button class="link" onclick={() => (editor.error = null)}>dismiss</button>
      </div>
    {/if}
  </div>
  {#if mode === "levels"}
    <main class="levels"><LevelEditor /></main>
  {:else if mode === "cutscenes"}
    <main class="levels"><CutsceneEditor /></main>
  {:else if mode === "music"}
    <main class="levels"><MusicEditor /></main>
  {:else}
    <main>
      <aside><FileBrowser /></aside>
      <section class="editor"><FileEditor /></section>
    </main>
  {/if}
  <footer><Diagnostics /></footer>
</div>

{#if previewOpen}
  <Preview onclose={() => (previewOpen = false)} />
{/if}

<style>
  .app {
    display: grid;
    grid-template-rows: auto auto 1fr minmax(6rem, 30vh);
    height: 100vh;
  }
  header {
    display: flex;
    align-items: center;
    gap: 0.75rem;
    padding: 0.4rem 0.75rem;
    border-bottom: 1px solid var(--line);
    background: var(--panel);
  }
  h1 {
    font-size: 1rem;
    margin: 0;
    white-space: nowrap;
  }
  .spacer {
    flex: 1;
  }
  .busy {
    color: var(--muted);
    font-size: 0.85rem;
  }
  .check-text {
    font-size: 0.85rem;
    color: var(--muted);
  }
  .checkout {
    font-size: 0.75rem;
    color: var(--muted);
    max-width: 20rem;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  .error-bar {
    background: var(--error-bg);
    color: var(--error);
    padding: 0.3rem 0.75rem;
    font-size: 0.9rem;
  }
  main {
    display: grid;
    grid-template-columns: minmax(14rem, 22rem) 1fr;
    min-height: 0;
  }
  main.levels {
    display: block;
  }
  .modes {
    display: flex;
  }
  .modes button {
    border-radius: 0;
    margin-left: -1px;
  }
  .modes button:first-child {
    border-radius: 4px 0 0 4px;
  }
  .modes button:last-child {
    border-radius: 0 4px 4px 0;
  }
  .modes button.on {
    background: var(--accent);
    color: var(--accent-text);
    border-color: var(--accent);
  }
  aside {
    border-right: 1px solid var(--line);
    overflow: auto;
    min-height: 0;
  }
  .editor {
    overflow: auto;
    min-height: 0;
  }
  footer {
    border-top: 1px solid var(--line);
    overflow: auto;
    min-height: 0;
    background: var(--panel);
  }
</style>

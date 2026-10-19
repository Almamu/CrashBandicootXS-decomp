<script lang="ts">
  // The music editor (docs/editors.md, "The music editor"): the songs, the
  // sound effects and the samples on the left; the selected song's
  // patterns (or the sound effect's, the sample's sound) in the middle,
  // played by the editor's own GAX2 (lib/music/gax.ts); what it is and
  // where it plays on the right.
  import { onMount } from "svelte";
  import { editor } from "../../lib/state.svelte.ts";
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import MusicList from "./MusicList.svelte";
  import PatternView from "./PatternView.svelte";
  import SongPanel from "./SongPanel.svelte";
  import SfxPanel from "./SfxPanel.svelte";
  import SamplePanel from "./SamplePanel.svelte";
  import InstrumentPanel from "./InstrumentPanel.svelte";

  $effect(() => {
    void mu.syncProject(editor.projectId, !!editor.project?.ready);
  });

  $effect(() => {
    if (mu.view && mu.song) void mu.measure(mu.song);
  });

  const len = $derived(mu.songLength[mu.lengthKey(mu.song)]);
  let exportSeconds = $state(60);

  async function exportWav() {
    const b = await mu.exportWav(exportSeconds);
    if (!b) return;
    const a = document.createElement("a");
    a.href = URL.createObjectURL(b);
    a.download = `${mu.song}.wav`;
    a.click();
    setTimeout(() => URL.revokeObjectURL(a.href), 1000);
  }

  function onkeydown(e: KeyboardEvent) {
    if (!mu.view) return;
    const t = e.target;
    const typing = t instanceof HTMLInputElement || t instanceof HTMLTextAreaElement || t instanceof HTMLSelectElement;
    const mod = e.ctrlKey || e.metaKey;
    if (mod && e.key.toLowerCase() === "s") {
      e.preventDefault();
      if (mu.dirty) void mu.save();
    } else if (mod && !typing && e.key.toLowerCase() === "z") {
      e.preventDefault();
      if (e.shiftKey) mu.redo();
      else mu.undo();
    } else if (mod && !typing && e.key.toLowerCase() === "y") {
      e.preventDefault();
      mu.redo();
    } else if (!mod && !typing && e.key === " ") {
      e.preventDefault();
      if (mu.playing) mu.stop();
      else if (mu.kind === "sfx") void mu.playSfx();
      else if (mu.kind === "instrument") void mu.playInstrument(mu.instrument, 49, 16);
      else void mu.playSong();
    }
  }

  onMount(() => {
    const before = (e: BeforeUnloadEvent) => {
      if (mu.dirty) e.preventDefault();
    };
    window.addEventListener("beforeunload", before);
    return () => {
      window.removeEventListener("beforeunload", before);
      mu.stop();
    };
  });
</script>

<svelte:window {onkeydown} />

{#if mu.unavailable}
  <div class="empty">
    <p>{mu.unavailable}.</p>
    <p>The music editor edits Crash Bandicoot XS's sound (games/xs/sound/) and a mod's. N-Tranced's songs and sound effects are still in its ROM data (docs/n_tranced.md).</p>
  </div>
{:else if !mu.view}
  <div class="empty"><p>{mu.loading ? "Reading the music…" : editor.project?.ready ? "" : "Apply the mod first."}</p></div>
{:else}
  <div class="me">
    <aside class="left"><MusicList /></aside>
    <section class="middle">
      <div class="toolbar">
        {#if mu.kind === "song"}
          {@const info = mu.songInfo}
          {@const sv = mu.songView}
          <strong>{mu.song}</strong>
          <span class="hint">
            {#if sv}{sv.p.channels} channels{/if}{#if info}, {info.mixRate} Hz{/if}{#if mu.state.structures[mu.song]} · structure changed{/if}
            {#if len}· loops to order {len.order} after {M.frameTime(len.loop)}{/if}
          </span>
          <span class="sep"></span>
          {#if mu.playing === "song"}
            <button class="primary" onclick={() => mu.stop()} title="stop (space)">■ Stop</button>
          {:else}
            <button class="primary" disabled={!sv} onclick={() => mu.playSong(0)} title="play the song from its start, as GAX2 mixes it (space)">▶ Play</button>
            <button disabled={!sv} onclick={() => mu.playSong(mu.order)} title="play from the order position shown">▶ From {mu.order}</button>
          {/if}
          <label class="hint" title="GAX_set_music_volume: every channel's volume (AudioContext's duck volume; 255 is full)">
            volume <input type="range" min="0" max="255" value={mu.musicVolume} onchange={(e) => mu.setMusicVolume(Number(e.currentTarget.value))} />
          </label>
          <label class="hint"><input type="checkbox" bind:checked={mu.follow} /> follow</label>
          <span class="pos">
            {#if mu.playing === "song" && mu.position}order {mu.position.order}, row {mu.position.row} · {M.frameTime(mu.position.frame)}{/if}
          </span>
          <span class="spacer"></span>
          <label class="hint" title="the song as the PC port's --wav writes it: 8-bit mono at its mix rate">
            <input type="number" min="1" max="600" bind:value={exportSeconds} style="width: 4rem" /> s
            <button disabled={!sv} onclick={exportWav}>Save as WAV</button>
          </label>
        {:else}
          <strong>{mu.kind === "sfx" ? "Sound effects" : mu.kind === "instrument" ? "Instruments" : "Samples"}</strong>
          <span class="spacer"></span>
        {/if}
        <button disabled={!mu.canUndo || !!editor.busy} title="undo {mu.undoLabel} (ctrl+z)" onclick={() => mu.undo()}>Undo</button>
        <button disabled={!mu.canRedo || !!editor.busy} title="redo {mu.redoLabel} (ctrl+shift+z)" onclick={() => mu.redo()}>Redo</button>
        <button disabled={!mu.dirty || !!editor.busy} onclick={() => mu.revert()}>Revert</button>
        <button class="primary" disabled={!mu.dirty || !!editor.busy} onclick={() => mu.save()} title="save (ctrl+s): the tables, the songs' XMs, the samples; checked like any save and built (data-check's music-build), all or nothing">Save</button>
      </div>
      <div class="stage">
        {#if mu.kind === "song"}
          <PatternView />
        {:else if mu.kind === "sfx"}
          <SfxPanel part="sound" />
        {:else}
          {#if mu.kind === "instrument"}<InstrumentPanel part="sound" />{:else}<SamplePanel part="sound" />{/if}
        {/if}
      </div>
    </section>
    <aside class="right">
      {#if mu.kind === "song"}
        <SongPanel />
      {:else if mu.kind === "sfx"}
        <SfxPanel part="record" />
      {:else}
        {#if mu.kind === "instrument"}<InstrumentPanel part="record" />{:else}<SamplePanel part="record" />{/if}
      {/if}
    </aside>
  </div>
{/if}

<style>
  .me {
    display: grid;
    grid-template-columns: minmax(13rem, 17rem) 1fr minmax(18rem, 23rem);
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
    grid-template-rows: auto 1fr;
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
  .sep {
    width: 0.5rem;
  }
  .hint {
    color: var(--muted);
    font-size: 0.8rem;
  }
  .pos {
    font-variant-numeric: tabular-nums;
    font-size: 0.8rem;
    min-width: 11rem;
  }
  .stage {
    overflow: auto;
    min-height: 0;
  }
  .empty {
    padding: 2rem;
    max-width: 40rem;
    color: var(--muted);
  }
</style>

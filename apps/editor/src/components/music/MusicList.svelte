<script lang="ts">
  // The songs (in song id order, each with what plays it), the sound
  // effects (in id order) and the samples; a new song from an XM, a new
  // sound effect (on a slot of the set, or with its own sample from a WAV).
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";

  let adding = $state<"song" | "sfx" | null>(null);
  let key = $state("");
  let comment = $state("");
  let sfxName = $state("");
  let file = $state<File | null>(null);

  const keyOk = $derived(M.KEY.test(key) && !mu.songKeys.includes(key));
  const sfxNameOk = $derived(M.SFX_NAME.test(sfxName) && !mu.state.sfx.entries.some((e) => e.name === sfxName));
  const instrumentName = $derived(sfxName.toLowerCase());
  const instrumentOk = $derived(M.KEY.test(instrumentName) && !(instrumentName in mu.state.newInstruments) && !mu.view?.instruments.some((i) => i.name === instrumentName));

  function slotOf(e: M.SfxEntry): string {
    const x = e as M.SfxEntry & { slot_id?: number; instrument?: string };
    return x.instrument ? x.instrument : String(x.slot_id);
  }

  async function addSong() {
    if (!file || !keyOk) return;
    await mu.addSong(file, key, comment || `a new song (${file.name})`);
    adding = null;
    key = comment = "";
    file = null;
  }

  async function addSfx() {
    if (!sfxNameOk) return;
    if (file) {
      if (!instrumentOk) return;
      const s = await mu.importWav(file);
      if (!s) return;
      mu.newSfxWithSample(instrumentName, sfxName, s, comment || `its own sample, from ${file.name}`);
    } else {
      mu.addSfx({ slot_id: 1, pitch_offset: 0, volume: 256, name: sfxName, comment: comment || "a new sound effect" } as unknown as M.SfxEntry);
    }
    adding = null;
    sfxName = comment = "";
    file = null;
  }
</script>

<nav aria-label="music">
  <div class="tabs" role="tablist">
    <button role="tab" aria-selected={mu.kind === "song"} class:on={mu.kind === "song"} onclick={() => (mu.kind = "song")}>Songs</button>
    <button role="tab" aria-selected={mu.kind === "sfx"} class:on={mu.kind === "sfx"} onclick={() => (mu.kind = "sfx")}>Sound effects</button>
    <button role="tab" aria-selected={mu.kind === "sample"} class:on={mu.kind === "sample"} onclick={() => (mu.kind = "sample")}>Samples</button>
  </div>

  {#if mu.kind === "song"}
    <ol>
      {#each mu.songs as s (s.key)}
        {@const uses = mu.usesOf(s.key)}
        <li>
          <button class:on={s.key === mu.song} onclick={() => mu.selectSong(s.key)} title={uses.length ? `played by ${uses.join(", ")}` : "nothing plays it"}>
            <span class="id">{s.id.toString(16).padStart(2, "0")}</span>
            <span class="name">{s.key}</span>
            <span class="count">{s.info?.xm ? `${s.info.xm.channels}ch` : s.added ? "new" : "?"}</span>
            <span class="about">{uses.length ? uses.slice(0, 3).join(", ") + (uses.length > 3 ? ", …" : "") : "nothing plays it"}</span>
          </button>
        </li>
      {/each}
    </ol>
    {#if adding === "song"}
      <div class="new">
        <label>XM <input type="file" accept=".xm" onchange={(e) => (file = e.currentTarget.files?.[0] ?? null)} /></label>
        <label>Key <input bind:value={key} spellcheck="false" placeholder="my_song" class:invalid={!!key && !keyOk} /></label>
        <label>Comment <input bind:value={comment} placeholder="where it plays" /></label>
        <p class="hint">Saved as songs/{key || "<key>"}.xm and SONG_{(key || "<key>").toUpperCase()}, id {mu.songs.length} (docs/audio.md, "Adding a song"). Its instruments: "Instrument N" plays the pool's N, any other name its own sample. The save builds it, so an XM GAX2 can't play is refused with the reason. A theme or a screen then picks it.</p>
        <div class="row">
          <button class="primary" disabled={!file || !keyOk} onclick={addSong}>Add</button>
          <button onclick={() => (adding = null)}>Cancel</button>
        </div>
      </div>
    {:else}
      <div class="row"><button onclick={() => (adding = "song")}>Import an XM as a new song…</button></div>
    {/if}
  {:else if mu.kind === "sfx"}
    <ol>
      {#each mu.state.sfx.entries as e, i (i)}
        {@const uses = mu.view?.sfx[i]?.name === e.name ? (mu.view?.sfx[i]?.uses ?? []) : []}
        <li>
          <button class:on={i === mu.sfx} onclick={() => (mu.sfx = i)} title={uses.length ? `named by ${uses.join(", ")}` : "nothing names it"}>
            <span class="id">{i.toString(16).padStart(2, "0")}</span>
            <span class="name">{e.name ?? "(unnamed)"}</span>
            <span class="count">{slotOf(e)}</span>
          </button>
        </li>
      {/each}
    </ol>
    {#if adding === "sfx"}
      <div class="new">
        <label>Name <input bind:value={sfxName} spellcheck="false" placeholder="MY_SOUND" class:invalid={!!sfxName && !sfxNameOk} /></label>
        <label>Comment <input bind:value={comment} placeholder="what plays it" /></label>
        <label>Its own sample (optional) <input type="file" accept=".wav" onchange={(e) => (file = e.currentTarget.files?.[0] ?? null)} /></label>
        <p class="hint">
          SFX_{sfxName || "<NAME>"}, id 0x{mu.state.sfx.entries.length.toString(16)}; SFX_NONE moves one up. Without a sample it plays slot 1 (pick another after); with one, the WAV becomes
          instruments/{instrumentName || "<name>"}.json and sfx_samples/{instrumentName || "<name>"}.wav (8-bit mono, at the WAV's rate). The code's PlaySfx calls are code: a new sound effect is for data that names it, a cutscene slide's.
        </p>
        <div class="row">
          <button class="primary" disabled={!sfxNameOk || (!!file && !instrumentOk)} onclick={addSfx}>Add</button>
          <button onclick={() => (adding = null)}>Cancel</button>
        </div>
      </div>
    {:else}
      {@const why = mu.view ? M.cannotDeleteLast(mu.view, mu.state) : null}
      <div class="row">
        <button onclick={() => ((adding = "sfx"), (sfxName = M.freeName(mu.state.sfx.entries)))}>New sound effect…</button>
        <button disabled={!!why} title={why ?? "delete the last entry (a sound effect's id is its place)"} onclick={() => mu.deleteLastSfx()}>Delete the last</button>
      </div>
    {/if}
  {:else}
    <h3>The music pool</h3>
    <ol>
      {#each mu.view?.samples ?? [] as s (s.path)}
        <li>
          <button class:on={mu.sample === s.path} onclick={() => (mu.sample = s.path)} title="{s.instruments.length} instruments, {s.songs.length} songs">
            <span class="id">{String(s.index).padStart(2, "0")}</span>
            <span class="name">{s.path}{mu.state.samples[s.path] ? " *" : ""}</span>
            <span class="count">{s.length}</span>
          </button>
        </li>
      {/each}
    </ol>
    <h3>The sound-effect set</h3>
    <ol>
      {#each mu.view?.sfxSlots ?? [] as s (s.path)}
        <li>
          <button class:on={mu.sample === s.path} onclick={() => (mu.sample = s.path)} title="slot {s.slot}, at {s.rate} Hz">
            <span class="id">{String(s.slot).padStart(2, "0")}</span>
            <span class="name">{s.path}{mu.state.samples[s.path] ? " *" : ""}</span>
            <span class="count">{s.length}</span>
          </button>
        </li>
      {/each}
    </ol>
  {/if}
</nav>

<style>
  nav {
    padding: 0.5rem;
  }
  .tabs {
    display: flex;
    margin-bottom: 0.4rem;
  }
  .tabs button {
    flex: 1;
    border-radius: 0;
    margin-left: -1px;
    font-size: 0.8rem;
    padding: 0.2rem 0.3rem;
  }
  .tabs button:first-child {
    border-radius: 4px 0 0 4px;
  }
  .tabs button:last-child {
    border-radius: 0 4px 4px 0;
  }
  .tabs button.on {
    background: var(--accent);
    color: var(--accent-text);
    border-color: var(--accent);
  }
  h3 {
    font-size: 0.8rem;
    margin: 0.6rem 0 0.2rem;
    color: var(--muted);
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
    padding: 0.2rem 0.35rem;
    gap: 0 0.3rem;
    font-size: 0.85rem;
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
    font-family: ui-monospace, monospace;
    font-size: 0.78rem;
  }
  .name {
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  .about {
    grid-column: 2 / 4;
    font-size: 0.72rem;
    opacity: 0.75;
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

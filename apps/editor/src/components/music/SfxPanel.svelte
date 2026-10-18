<script lang="ts">
  // The selected sound effect: in the middle its sample's sound and how it
  // plays (as PlaySfx plays it, over a song, through the editor's GAX2), on
  // the right its table entry (sfx_table.json: name, slot or instrument,
  // priority, volume, comment) and what names it.
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  type Entry = M.SfxEntry & { slot_id?: number; instrument?: string };
  const e = $derived(mu.state.sfx.entries[mu.sfx] as Entry | undefined);
  const old = $derived(mu.view?.sfx[mu.sfx]);
  const uses = $derived(old && e && old.name === e.name ? old.uses : []);
  const slot = $derived(e?.slot_id !== undefined ? mu.view?.sfxSlots.find((s) => s.slot === e.slot_id) : undefined);
  const path = $derived(slot ? slot.path : e?.instrument ? `sfx_samples/${e.instrument}.wav` : "");
  const sound = $derived(path ? mu.sound(path) : null);
  const newInstrument = $derived(e?.instrument ? mu.state.newInstruments[e.instrument] : undefined);
  const savedInstrument = $derived(e?.instrument ? mu.view?.instruments.find((i) => i.name === e.instrument) : undefined);
  let fileInput = $state<HTMLInputElement>();

  $effect(() => {
    if (path && !newInstrument) void mu.loadWav(path);
  });

  function num(field: "pitch_offset" | "volume", s: string) {
    const v = Number(s);
    if (!Number.isInteger(v) || v < -0x80000000 || v > 0x7fffffff) return;
    mu.setSfx(mu.sfx, { [field]: v }, field);
  }

  async function replace(ev: Event) {
    const f = (ev.currentTarget as HTMLInputElement).files?.[0];
    if (fileInput) fileInput.value = "";
    if (!f || !e) return;
    if (slot) {
      const s = await mu.importWav(f, Math.round(slot.rate));
      if (s) mu.replaceSample(slot.path, s);
    } else if (newInstrument && e.instrument) {
      const s = await mu.importWav(f);
      if (s) mu.apply(`Replace ${e.instrument}'s sample`, (st) => ({ ...st, newInstruments: { ...st.newInstruments, [e.instrument!]: { ...newInstrument, ...s } } }));
    }
  }
</script>

{#if e}
  {#if part === "sound"}
    <div class="sound">
      <h2>SFX_{e.name ?? "?"} <span class="hint">id {mu.sfx} (0x{mu.sfx.toString(16)})</span></h2>
      <Waveform data={sound?.data ?? null} label="{path}'s waveform" />
      <p class="hint">
        {#if slot}slot {slot.slot}: {slot.path}, {sound?.data.length ?? slot.length} samples, played at {slot.rate} Hz (gGaxPeriodTable[{Math.round(384 * Math.log2(slot.rate / M.PERIOD_0_HZ))}] / 2048){mu.state.samples[slot.path] ? " — replaced, not saved" : ""}{/if}
        {#if e.instrument}its own instrument {e.instrument}: {path}{newInstrument ? `, ${newInstrument.length} samples at ${newInstrument.rate} Hz — new, not saved` : savedInstrument ? "" : " — no such instrument"}{/if}
      </p>
      <div class="row">
        {#if mu.playing === "sfx"}
          <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
        {:else}
          <button class="primary" onclick={() => mu.playSfx()} title="as PlaySfx plays it: GAX_fx_ex on voice 0 with its priority, its volume (space)">▶ Play</button>
        {/if}
        <label>
          over
          <select bind:value={mu.sfxSong} title="the song playing: its mix rate, and whether the voices mix at half rate (the level songs)">
            {#each mu.view?.songs ?? [] as s (s.key)}<option value={s.key}>{s.key} ({s.mixRate} Hz{s.halfRateFx ? ", half rate" : ""})</option>{/each}
          </select>
        </label>
        <label><input type="checkbox" bind:checked={mu.sfxWithMusic} /> with its music</label>
        <button disabled={!path} onclick={() => mu.playWav(path)} title="the WAV as it is, at its own rate (not through GAX2)">Play the WAV</button>
      </div>
      <p class="hint">
        The game plays it through GAX2 on a sound-effect voice, mixed with the song's channels: at the song's mix rate, a level song's at half that (the resampler's mode 2, docs/audio.md). The editor mixes it the same way, the song's music muted unless “with its music”.
      </p>
    </div>
  {:else}
    <div class="panel">
      <section>
        <h3>The table's entry <span class="hint">sfx_table.json</span></h3>
        <label class="row">
          Name
          <input
            value={e.name ?? ""}
            spellcheck="false"
            disabled={uses.length > 0}
            class:invalid={!!e.name && !M.SFX_NAME.test(e.name)}
            title={uses.length ? "the code or a cutscene names SFX_" + e.name + ": it keeps its name" : "SFX_<NAME>: upper case, digits and _"}
            oninput={(ev) => mu.setSfx(mu.sfx, { name: ev.currentTarget.value }, "name")}
          />
        </label>
        <label class="row">
          Plays
          <select
            value={e.instrument ? `i:${e.instrument}` : `s:${e.slot_id}`}
            onchange={(ev) => {
              const v = ev.currentTarget.value;
              mu.setSfx(mu.sfx, v.startsWith("i:") ? { instrument: v.slice(2) } : { slot_id: Number(v.slice(2)) });
            }}
          >
            {#each mu.view?.sfxSlots ?? [] as s (s.slot)}<option value="s:{s.slot}">slot {s.slot} ({s.path})</option>{/each}
            {#each [...new Set([...(mu.view?.instruments.map((i) => i.name) ?? []), ...Object.keys(mu.state.newInstruments)])] as n (n)}<option value="i:{n}">instrument {n}</option>{/each}
          </select>
        </label>
        <label class="row" title="GAX_fx_ex's priority (the table's pitch_offset): a voice playing a higher one isn't taken">
          Priority
          <input type="number" value={e.pitch_offset} oninput={(ev) => num("pitch_offset", ev.currentTarget.value)} />
        </label>
        <label class="row" title="8.8 fixed point (256 is 1.0); PlaySfx gives the voice min(255, volume)">
          Volume
          <input type="number" min="0" max="1024" value={e.volume} class:invalid={e.volume < 0} oninput={(ev) => num("volume", ev.currentTarget.value)} />
        </label>
        <label class="wide">
          Comment
          <textarea rows="2" value={e.comment ?? ""} oninput={(ev) => mu.setSfx(mu.sfx, { comment: ev.currentTarget.value }, "comment")}></textarea>
        </label>
        <p class="hint">Named by: {uses.length ? uses.join(", ") : "nothing"}.</p>
      </section>
      <section>
        <h3>Its sample</h3>
        {#if slot || newInstrument}
          <label class="import">
            Replace it with a WAV {slot ? `(resampled to its ${Math.round(slot.rate)} Hz)` : "(at the WAV's rate)"}
            <input bind:this={fileInput} type="file" accept=".wav" onchange={replace} />
          </label>
          {#if slot}
            <p class="hint">Every sound effect on slot {slot.slot} plays it: {mu.state.sfx.entries.filter((x) => (x as Entry).slot_id === slot.slot).map((x) => x.name).join(", ")}.</p>
          {/if}
        {:else}
          <p class="hint">An instrument saved earlier: its JSON is instruments/{e.instrument}.json (the Data files tab).</p>
        {/if}
      </section>
    </div>
  {/if}
{/if}

<style>
  .sound {
    padding: 0.6rem 0.8rem;
    display: grid;
    gap: 0.4rem;
    max-width: 60rem;
  }
  h2 {
    font-size: 1rem;
    margin: 0;
  }
  .panel {
    padding: 0.5rem 0.6rem;
    font-size: 0.85rem;
  }
  section {
    border-bottom: 1px solid var(--line);
    padding-bottom: 0.5rem;
    margin-bottom: 0.5rem;
  }
  h3 {
    font-size: 0.85rem;
    margin: 0.2rem 0 0.35rem;
  }
  .row {
    display: grid;
    grid-template-columns: 4.5rem 1fr;
    gap: 0.35rem;
    align-items: center;
    margin: 0.2rem 0;
  }
  .row input,
  .row select {
    min-width: 0;
    width: 100%;
  }
  div.row {
    display: flex;
    flex-wrap: wrap;
    gap: 0.5rem;
    align-items: center;
    font-size: 0.85rem;
  }
  .wide {
    display: block;
  }
  .wide textarea {
    width: 100%;
  }
  .import {
    display: grid;
    gap: 0.2rem;
    font-size: 0.8rem;
  }
  .hint {
    color: var(--muted);
    font-size: 0.78rem;
    margin: 0.2rem 0;
    font-weight: normal;
    overflow-wrap: anywhere;
  }
  div.row label {
    display: inline-flex;
    gap: 0.3rem;
    align-items: center;
  }
</style>

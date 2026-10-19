<script lang="ts">
  // The selected sample (the music pool's samples/NN.wav or the
  // sound-effect set's sfx_samples/NN.wav): its sound, what plays it, and a
  // new sound for it from a WAV (made 8-bit mono at a rate by the backend).
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  const pool = $derived(mu.view?.samples.find((s) => s.path === mu.sample));
  const added = $derived(mu.poolSamples.find((s) => s.path === mu.sample && s.added) ?? mu.sfxSamples.find((s) => s.path === mu.sample && s.added));
  const sfxSample = $derived(mu.view?.sfxSamples.find((s) => s.path === mu.sample));
  const slot = $derived(mu.sfxSlotList.find((s) => s.path === mu.sample));
  const sound = $derived(mu.sample ? mu.sound(mu.sample) : null);
  const minLength = $derived(pool?.minLength ?? sfxSample?.minLength ?? 1);
  const defaultRate = $derived(slot ? Math.round(slot.rate) : (pool?.rate ?? 15769));
  let rate = $derived(defaultRate);
  let fileInput = $state<HTMLInputElement>();
  $effect(() => {
    if (mu.sample) void mu.loadWav(mu.sample);
  });

  async function replace(ev: Event) {
    const f = (ev.currentTarget as HTMLInputElement).files?.[0];
    if (fileInput) fileInput.value = "";
    if (!f || !mu.sample) return;
    const s = await mu.importWav(f, rate);
    if (s) mu.replaceSample(mu.sample, s);
  }
</script>

{#if !mu.sample}
  <div class="empty"><p>Pick a sample on the left.</p></div>
{:else if part === "sound"}
  <div class="sound">
    <h2>{mu.sample}{mu.state.samples[mu.sample] ? " (replaced, not saved)" : added ? " (new, not saved)" : ""}</h2>
    <Waveform data={sound?.data ?? null} label="{mu.sample}'s waveform" />
    <p class="hint">{sound ? `${sound.data.length} samples, the WAV declares ${sound.rate} Hz` : "reading…"}</p>
    <div class="row">
      {#if mu.playing === "sample"}
        <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
      {:else}
        <button class="primary" onclick={() => mu.playWav(mu.sample)} title="the WAV as it is, at the rate it declares (not through GAX2)">▶ Play the WAV</button>
      {/if}
    </div>
  </div>
{:else}
  <div class="panel">
    <section>
      {#if pool || (added && mu.sample.startsWith("samples/"))}
        {@const idx = pool?.index ?? added!.index}
        {@const who = mu.sampleUsers(idx)}
        <h3>Sample {idx} of the music pool{added ? " (new, not saved)" : ""}</h3>
        <p class="hint">
          Played by instruments {who.join(", ") || "none"}{pool ? ` in ${pool.songs.join(", ") || "no song"}` : ""}. Its WAV's rate is cosmetic (the build doesn't read it): an instrument plays it at the note's pitch, with its own tune (the Instruments tab gives a wave row its sample).
        </p>
        <button disabled={who.length > 0} title={who.length ? `instruments ${who.join(", ")} play it` : "take it out of the music pool (samples/NN.wav and the manifest's entry)"} onclick={() => mu.deletePoolSample(idx)}>Delete this sample</button>
      {:else if sfxSample || added}
        {@const idx = sfxSample?.index ?? added!.index}
        {@const who = mu.sfxSampleUsers(idx)}
        <h3>Sample {idx} of the sound-effect set{added ? " (new, not saved)" : ""}</h3>
        <p class="hint">
          Played by instruments {who.join(", ") || "none"} of the set{slot ? `; GAX2 plays it at one rate, ${slot.rate} Hz (slot ${slot.slot}'s fixed pitch), for ${mu.slotUsers(slot.slot).join(", ") || "no sound effect"}` : ""}. The Instruments tab gives a new instrument of the set its sample.
        </p>
        <button disabled={who.length > 0} title={who.length ? `instruments ${who.join(", ")} play it` : "take it out of the sound-effect set (sfx_samples/NN.wav and the manifest's entry)"} onclick={() => mu.deleteSfxSample(idx)}>Delete this sample</button>
      {/if}
      <p class="hint">Its instruments' starts and loops reach sample {minLength}: a new sound must be at least that long.</p>
    </section>
    {#if !added}
    <section>
      <h3>A new sound for it</h3>
      <label class="row" title="the WAV is resampled to this rate: a sound effect's slot plays at its rate, so the sound keeps its pitch">
        Rate <input type="number" min="1000" max="200000" value={rate} oninput={(e) => (rate = Number(e.currentTarget.value))} /> Hz
      </label>
      <label class="import">
        A WAV (any PCM, or 32-bit float; mixed to mono, 8 bits)
        <input bind:this={fileInput} type="file" accept=".wav" onchange={replace} />
      </label>
      {#if mu.state.samples[mu.sample]}
        {@const s = mu.state.samples[mu.sample]!}
        <p class="hint">Replaced: {s.length} samples at {s.rate} Hz.</p>
        {#if s.length < minLength}<p class="warn">It is shorter than {minLength}: the save refuses it.</p>{/if}
        <button onclick={() => mu.apply(`Put ${mu.sample} back`, (st) => { const samples = { ...st.samples }; delete samples[mu.sample]; return { ...st, samples }; })}>Put the saved one back</button>
      {/if}
    </section>
    {/if}
  </div>
{/if}

<style>
  .empty {
    padding: 2rem;
    color: var(--muted);
  }
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
    display: flex;
    gap: 0.35rem;
    align-items: center;
    margin: 0.2rem 0;
  }
  .row input {
    width: 7rem;
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
  }
  .warn {
    color: var(--warning);
    font-size: 0.78rem;
  }
</style>

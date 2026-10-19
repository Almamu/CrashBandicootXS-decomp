<script lang="ts">
  // A song's own XM instrument (not "Instrument N", not an
  // instruments/<name>.json's): the build makes it a new pool instrument
  // from its sample, loop, volume, relative note and finetune, its volume
  // envelope and auto-vibrato (crashdata.audio.gax.xm_instrument_spec),
  // and those are what this edits, written in the song's XM
  // (crashdata.audio.edit.xm_with_own_instrument). In the middle its
  // envelope, drawn and edited, its sample with the loop, and a note played
  // through GAX2 as the song plays it (the backend builds the pool with the
  // unsaved edits); on the right its fields and what GAX2 makes of them.
  import { editor } from "../../lib/state.svelte.ts";
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import EnvelopeGraph from "./EnvelopeGraph.svelte";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  type Spec = M.XmInstrumentEdit["spec"];
  type Env = NonNullable<Spec["envelope"]>;
  const song = $derived(mu.xmSong);
  const n = $derived(mu.instrument);
  const o = $derived(mu.own(song, n));
  const x = $derived(o?.spec ?? null);
  const name = $derived(mu.patterns[song]?.p.instruments[n - 1]?.name ?? "");
  const gax = $derived(x ? M.xmInstrumentGax(x) : null);
  const problem = $derived(x && o ? M.xmInstrumentProblem(x, o.sound.length) : null);
  const points = $derived((x?.envelope?.points ?? []).map(([px, py]) => ({ x: px, y: py })));
  let note = $state(49);
  let hold = $state(16);
  let fileInput = $state<HTMLInputElement>();

  function set(label: string, f: (s: Spec) => Spec, key: string | null = null) {
    mu.setOwn(song, n, label, (e) => ({ ...e, spec: f(e.spec) }), key);
  }

  function setEnv(label: string, f: (e: Env) => Env) {
    set(label, (s) => (s.envelope ? { ...s, envelope: f(s.envelope) } : s));
  }

  /** the envelope's points changed: the sustain and loop follow their points (one added or taken out moves them) */
  function envelope(pts: M.EnvPoint[], label: string) {
    const env = x?.envelope;
    if (!env) return;
    if (pts.length > M.XM_ENVELOPE_POINTS) {
      editor.error = `an XM envelope has at most ${M.XM_ENVELOPE_POINTS} points`;
      return;
    }
    const old = points;
    let at = 0;
    while (at < Math.min(old.length, pts.length) && old[at]!.x === pts[at]!.x) at++;
    const move = (v: number | null): number | null => {
      if (v === null) return null;
      if (pts.length > old.length) return v >= at ? v + 1 : v;
      if (pts.length < old.length) return v === at ? null : v > at ? v - 1 : v;
      return v;
    };
    let loopStart = move(env.loop_start);
    let loopEnd = move(env.loop_end);
    if (loopStart === null || loopEnd === null) loopStart = loopEnd = null;
    setEnv(label, () => ({ points: pts.map((p) => [p.x, Math.min(64, p.y)] as [number, number]), sustain: move(env.sustain), loop_start: loopStart, loop_end: loopEnd }));
  }

  function setMark(which: "sustain" | "loop_start" | "loop_end", v: number | null) {
    setEnv(`envelope ${which.replace("_", " ")}`, (e) => {
      const m = { ...e, [which]: v };
      if (which !== "sustain" && (m.loop_start === null) !== (m.loop_end === null)) {
        if (m.loop_start === null) m.loop_end = null;
        else m.loop_end = m.loop_end ?? m.loop_start;
      }
      if (m.loop_start !== null && m.loop_end !== null && m.loop_start > m.loop_end) m.loop_end = m.loop_start;
      return m;
    });
  }

  async function replace(ev: Event) {
    const f = (ev.currentTarget as HTMLInputElement).files?.[0];
    if (fileInput) fileInput.value = "";
    if (!f) return;
    const s = await mu.importWav(f);
    if (!s) return;
    // C-4 plays it at the WAV's rate; a loop past its end is taken in
    mu.setOwn(song, n, "a new sound", (e) => {
      const loop = e.spec.loop !== "none" && e.spec.loop_end > s.length ? { loop_start: Math.min(e.spec.loop_start, s.length - 1), loop_end: s.length } : {};
      return { spec: { ...e.spec, ...M.xmTuneForRate(s.rate), ...loop }, wav: s };
    });
  }

  const num = (v: string) => (v.trim() === "" ? NaN : Number(v));
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
  const noteNames = Array.from({ length: 96 }, (_, k) => k + 1);
</script>

{#if !o || !x || !gax}
  <div class="empty"><p>{mu.patterns[song] ? "Pick an instrument on the left." : "Reading the song…"}</p></div>
{:else if part === "sound"}
  <div class="sound">
    <h2>{song}'s instrument {n}: {name || "(no name)"}{o.edited ? " (edited, not saved)" : ""}</h2>
    <p class="hint">The song's own instrument: the build makes it a new pool instrument from its XM sample and envelopes (docs/audio.md, "From the XM"). Only {song} plays it.</p>
    {#if o.saved.plays.length > 1}<p class="warn">Its key map plays samples {o.saved.plays.join(", ")} on the song's notes: a GAX2 instrument has one sample (the build refuses it; split it in a tracker).</p>{/if}
    <h3>Volume envelope</h3>
    {#if x.envelope}
      <EnvelopeGraph {points} sustain={x.envelope.sustain} loopStart={x.envelope.loop_start} loopEnd={x.envelope.loop_end} onchange={envelope} top={64} scale="XM's: GAX2's is 4 times it" />
    {:else}
      <p class="hint">Off: full volume, cut at the key off.</p>
    {/if}
    <h3>Its sample (XM sample {x.sample} of {o.saved.samples})</h3>
    <Waveform data={o.sound} height={90} label="the sample's waveform" />
    <p class="hint">
      {o.sound.length} samples{o.saved.bits === 16 && !mu.state.xmInstruments[song]?.[String(n)]?.wav ? " (16-bit, read as 8)" : ""} · {x.loop === "none" ? "no loop: it plays to its end" : `${x.loop} loop ${x.loop_start}-${x.loop_end} (${x.loop_end - x.loop_start} samples)`} · C-4 plays it at {Math.round(8363 * 2 ** ((x.relative_note + x.finetune / 128) / 12))} Hz
    </p>
    {#if problem}<p class="warn">{problem}: the save refuses it.</p>{/if}
    <div class="row">
      <label>Note <select bind:value={note}>{#each noteNames as k (k)}<option value={k}>{M.noteName(k)}</option>{/each}</select></label>
      <label title="the rows (at speed 6, 6 ticks each) before the key off">Key off after <input type="number" min="1" max="200" bind:value={hold} style="width: 4rem" /> rows</label>
      {#if mu.playing === "instrument"}
        <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
      {:else}
        <button class="primary" disabled={!!problem} onclick={() => mu.playOwn(song, n, note, Math.max(1, hold))} title="the note on one channel, as a song plays it (GAX2, the pool the build makes with the unsaved edits)">▶ Play the note</button>
      {/if}
    </div>
  </div>
{:else}
  <div class="panel">
    <section>
      <h3>Instrument {n} <span class="hint">songs/{song}.xm's</span></h3>
      {#if o.edited}<button onclick={() => mu.revertOwn(song, n)}>Put the saved one back</button>{/if}
      <div class="grid3">
        <label title="the sample's volume, 0-64 (GAX2's: the sequence's set-volume step, 4 times it)">Volume <input type="number" min="0" max="64" value={x.volume} oninput={(e) => int(num(e.currentTarget.value), 0, 64) && set("volume", (s) => ({ ...s, volume: num(e.currentTarget.value) }), "vol")} /></label>
        <label title="semitones from the note">Relative note <input type="number" min="-128" max="127" value={x.relative_note} onchange={(e) => int(num(e.currentTarget.value), -128, 127) && set("relative note", (s) => ({ ...s, relative_note: num(e.currentTarget.value) }))} /></label>
        <label title="128ths of a semitone">Finetune <input type="number" min="-128" max="127" value={x.finetune} onchange={(e) => int(num(e.currentTarget.value), -128, 127) && set("finetune", (s) => ({ ...s, finetune: num(e.currentTarget.value) }))} /></label>
        <label>
          Loop
          <select value={x.loop} onchange={(e) => set("loop", (s) => ({ ...s, loop: e.currentTarget.value as Spec["loop"], ...(s.loop_start < s.loop_end && s.loop_end <= o.sound.length ? {} : { loop_start: 0, loop_end: o.sound.length }) }))}>
            <option value="none">none</option><option value="forward">forward</option><option value="pingpong">ping-pong</option>
          </select>
        </label>
        {#if x.loop !== "none"}
          <label>Loop start <input type="number" min="0" value={x.loop_start} onchange={(e) => int(num(e.currentTarget.value), 0, o.sound.length - 1) && set("loop start", (s) => ({ ...s, loop_start: num(e.currentTarget.value), loop_end: Math.max(num(e.currentTarget.value) + 1, s.loop_end) }))} /></label>
          <label title="its length: the loop ends at start + length">Loop length <input type="number" min="1" value={x.loop_end - x.loop_start} onchange={(e) => int(num(e.currentTarget.value), 1, o.sound.length) && set("loop length", (s) => ({ ...s, loop_end: s.loop_start + num(e.currentTarget.value) }))} /></label>
        {/if}
      </div>
      <p class="hint">GAX2's wave row: tune {gax.tune} (1/32 semitones of gGaxPeriodTable from the note), {x.loop === "none" ? "no loop" : `${x.loop} loop`}; volume {Math.min(255, gax.volume)}{x.volume >= 64 ? " (full: no set-volume step)" : ""}.</p>
      <label class="import">
        Replace its sound with a WAV (any PCM, or 32-bit float; mixed to mono, 8 bits, at the WAV's rate: the tune set for C-4 to play it there)
        <input bind:this={fileInput} type="file" accept=".wav" onchange={replace} />
      </label>
    </section>

    <section>
      <h3>Volume envelope <label class="hint"><input type="checkbox" checked={!!x.envelope} onchange={(e) => set(e.currentTarget.checked ? "the volume envelope on" : "the volume envelope off", (s) => ({ ...s, envelope: e.currentTarget.checked ? { points: [[0, 64]], sustain: 0, loop_start: null, loop_end: null } : null }))} /> on</label></h3>
      {#if x.envelope}
        {@const env = x.envelope}
        <table class="pts">
          <thead><tr><th>#</th><th>tick</th><th>volume</th><th></th></tr></thead>
          <tbody>
            {#each points as p, i (i)}
              <tr>
                <td>{i}</td>
                <td><input type="number" min="0" max="65535" value={p.x} disabled={i === 0} onchange={(e) => envelope(points.map((q, k) => (k === i ? { ...q, x: num(e.currentTarget.value) } : q)), `envelope point ${i}'s tick`)} /></td>
                <td><input type="number" min="0" max="64" value={p.y} onchange={(e) => envelope(points.map((q, k) => (k === i ? { ...q, y: num(e.currentTarget.value) } : q)), `envelope point ${i}'s volume`)} /></td>
                <td><button disabled={i === 0} onclick={() => envelope(points.filter((_, k) => k !== i), `envelope point ${i} out`)}>−</button></td>
              </tr>
            {/each}
          </tbody>
        </table>
        <button disabled={points.length >= M.XM_ENVELOPE_POINTS} onclick={() => envelope([...points, { x: (points[points.length - 1]?.x ?? 0) + 8, y: 0 }], "an envelope point")} title="an XM envelope has at most {M.XM_ENVELOPE_POINTS} points">+ point</button>
        <div class="grid3">
          {#each [["sustain", "Sustain"], ["loop_start", "Loop from"], ["loop_end", "to"]] as [k, label] (k)}
            <label>
              {label}
              <select value={env[k as "sustain"] ?? ""} onchange={(e) => setMark(k as "sustain", e.currentTarget.value === "" ? null : Number(e.currentTarget.value))}>
                <option value="">none</option>
                {#each points as _, i (i)}<option value={i}>point {i}</option>{/each}
              </select>
            </label>
          {/each}
        </div>
        <p class="hint">GAX2's points are 4 times XM's volumes ({gax.envelope?.join(", ")}); without a sustain point the note-off doesn't cut the note, as in XM. GAX2 runs the loop only until the key off, and has no fadeout: the save writes the fadeout as 0.</p>
      {/if}
    </section>

    <section>
      <h3>Auto-vibrato</h3>
      <div class="grid3">
        <label title="XM's depth: GAX2's the same (depth/64 semitones)">Depth <input type="number" min="0" max="255" value={x.vibrato.depth} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato depth", (s) => ({ ...s, vibrato: { ...s.vibrato, depth: num(e.currentTarget.value) } }), "vd")} /></label>
        <label title="XM's rate (256 steps a cycle; GAX2's speed is a quarter of it, of 64)">Rate <input type="number" min="0" max="255" value={x.vibrato.rate} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato rate", (s) => ({ ...s, vibrato: { ...s.vibrato, rate: num(e.currentTarget.value) } }), "vr")} /></label>
      </div>
      <p class="hint">GAX2's vibrato: {gax.vibrato.depth ? `depth ${gax.vibrato.depth}, speed ${gax.vibrato.speed}` : "none (a depth and a rate make one)"}. It is a sine that starts at full depth: the save makes the XM's a sine with no sweep. GAX2 mixes mono: no panning envelope.</p>
    </section>
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
  h3 {
    font-size: 0.85rem;
    margin: 0.3rem 0 0.2rem;
  }
  .panel {
    padding: 0.5rem 0.6rem;
    font-size: 0.82rem;
  }
  section {
    border-bottom: 1px solid var(--line);
    padding-bottom: 0.5rem;
    margin-bottom: 0.5rem;
  }
  .grid3 {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 0.3rem 0.4rem;
  }
  .grid3 label {
    display: grid;
    font-size: 0.75rem;
  }
  .grid3 input,
  .grid3 select {
    min-width: 0;
  }
  .row {
    display: flex;
    gap: 0.5rem;
    align-items: center;
    flex-wrap: wrap;
  }
  .pts {
    border-collapse: collapse;
    font-size: 0.75rem;
  }
  .pts td,
  .pts th {
    padding: 0.05rem 0.2rem;
  }
  .pts input[type="number"] {
    width: 3.6rem;
  }
  .import {
    display: grid;
    gap: 0.2rem;
    font-size: 0.75rem;
    margin-top: 0.3rem;
  }
  .hint {
    color: var(--muted);
    font-size: 0.75rem;
    margin: 0.2rem 0;
    font-weight: normal;
  }
  .warn {
    color: var(--warning);
    font-size: 0.75rem;
  }
</style>

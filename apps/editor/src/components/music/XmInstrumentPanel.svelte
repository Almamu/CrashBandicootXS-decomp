<script lang="ts">
  // A song's own XM instrument (not "Instrument N", not an
  // instruments/<name>.json's): the build makes it a new pool instrument
  // from the sample its key map plays on the song's notes, that sample's
  // loop, volume, relative note and finetune, its volume envelope and
  // auto-vibrato (crashdata.audio.gax.xm_instrument_spec), and those are
  // what this edits, with its name, its key map and its other samples,
  // written in the song's XM (crashdata.audio.edit.xm_with_own_instrument).
  // A GAX2 instrument has one sample for every note, so a key map that
  // plays two of them on the song's notes is the build's to refuse, and
  // the panel says so. In the middle its envelope, drawn and edited, its
  // key map, the sample selected with its loop, and a note played through
  // GAX2 as the song plays it (the backend builds the pool with the
  // unsaved edits); on the right its fields and what GAX2 makes of them.
  import { editor } from "../../lib/state.svelte.ts";
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import EnvelopeGraph from "./EnvelopeGraph.svelte";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  type Env = NonNullable<M.XmSpec["envelope"]>;
  const song = $derived(mu.xmSong);
  const n = $derived(mu.instrument);
  const o = $derived(mu.own(song, n));
  const x = $derived(o?.spec ?? null);
  /** the sample the panel shows and edits */
  const k = $derived(x && mu.ownSample < x.samples.length ? mu.ownSample : 0);
  const smp = $derived(x?.samples[k] ?? null);
  const sound = $derived(o?.sounds[k] ?? null);
  const lengths = $derived(o ? o.sounds.map((s) => s?.length ?? 0) : []);
  /** the sample the build makes the instrument from: the one its key map plays on the song's notes */
  const built = $derived(o?.plays[0] ?? 0);
  const gax = $derived(x ? M.xmInstrumentGax(x, built) : null);
  const problem = $derived(x ? (M.xmNameProblem(x.name, x.samples.length, mu.jsonNames) ?? M.xmInstrumentProblem(x, lengths)) : null);
  const points = $derived((x?.envelope?.points ?? []).map(([px, py]) => ({ x: px, y: py })));
  let note = $state(49);
  let hold = $state(16);
  let from = $state(1);
  let to = $state(96);
  let fileInput = $state<HTMLInputElement>();
  let addInput = $state<HTMLInputElement>();
  const noteNames = Array.from({ length: 96 }, (_, i) => i + 1);
  const octaves = Array.from({ length: 8 }, (_, i) => i);

  function set(label: string, f: (s: M.XmSpec) => M.XmSpec, key: string | null = null) {
    mu.setOwn(song, n, label, (e) => ({ ...e, spec: f(e.spec) }), key);
  }

  function setSample(label: string, f: (s: M.XmSample) => M.XmSample, key: string | null = null) {
    const at = k;
    set(x && x.samples.length > 1 ? `sample ${at}'s ${label}` : label, (s) => ({ ...s, samples: s.samples.map((v, i) => (i === at ? f(v) : v)) }), key ? `${at}:${key}` : null);
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

  async function wavOf(ev: Event, input: HTMLInputElement | undefined): Promise<M.SampleEdit | undefined> {
    const f = (ev.currentTarget as HTMLInputElement).files?.[0];
    if (input) input.value = "";
    if (!f) return undefined;
    return mu.importWav(f);
  }

  /** a new sound for the sample selected: C-4 plays it at the WAV's rate; a loop past its end is taken in */
  async function replace(ev: Event) {
    const s = await wavOf(ev, fileInput);
    if (!s) return;
    const at = k;
    mu.setOwn(song, n, x && x.samples.length > 1 ? `sample ${at}'s new sound` : "a new sound", (e) => {
      const samples = e.spec.samples.map((v, i) => {
        if (i !== at) return v;
        const loop = v.loop !== "none" && v.loop_end > s.length ? { loop_start: Math.min(v.loop_start, s.length - 1), loop_end: s.length } : {};
        return { ...v, ...M.xmTuneForRate(s.rate), ...loop };
      });
      return { ...e, spec: { ...e.spec, samples }, sounds: { ...e.sounds, [String(at)]: s } };
    });
  }

  /** a sample more, from a WAV (the instrument's first: every note plays it) */
  async function addSample(ev: Event) {
    const s = await wavOf(ev, addInput);
    if (!s || !x) return;
    if (x.samples.length >= M.XM_SAMPLES) {
      editor.error = `an XM instrument has at most ${M.XM_SAMPLES} samples`;
      return;
    }
    const at = x.samples.length;
    mu.setOwn(song, n, at ? `sample ${at}, a new one` : "its sound", (e) => ({ ...e, spec: { ...e.spec, samples: [...e.spec.samples, { ...M.XM_NEW_SAMPLE, ...M.xmTuneForRate(s.rate) }] }, sounds: { ...e.sounds, [String(at)]: s } }));
    mu.ownSample = at;
  }

  /** notes `lo`-`hi` (1-96) play sample `s` */
  function keys(lo: number, hi: number, s: number) {
    const [a, b] = lo <= hi ? [lo, hi] : [hi, lo];
    set(a === 1 && b === 96 ? `every note plays sample ${s}` : a === b ? `${M.noteName(a)} plays sample ${s}` : `${M.noteName(a)}-${M.noteName(b)} play sample ${s}`, (sp) => ({ ...sp, keymap: sp.keymap.map((v, i) => (i + 1 >= a && i + 1 <= b ? s : v)) }));
  }

  function rename(v: string) {
    if (!x || v === x.name) return;
    const why = M.xmNameProblem(v, x.samples.length, mu.jsonNames);
    if (why) {
      editor.error = why;
      return;
    }
    set("its name", (s) => ({ ...s, name: v }));
  }

  const num = (v: string) => (v.trim() === "" ? NaN : Number(v));
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
  const rate = (s: M.XmSample) => Math.round(8363 * 2 ** ((s.relative_note + s.finetune / 128) / 12));
</script>

{#if !o || !x || !gax}
  <div class="empty"><p>{mu.patterns[song] ? "Pick an instrument on the left." : "Reading the song…"}</p></div>
{:else if part === "sound"}
  <div class="sound">
    <h2>{song}'s instrument {n}: {x.name || "(no name)"}{o.added ? " (new, not saved)" : o.edited ? " (edited, not saved)" : ""}</h2>
    <p class="hint">The song's own instrument: the build makes it a new pool instrument from its XM sample and envelopes (docs/audio.md, "From the XM"). Only {song} plays it.</p>
    {#if !x.samples.length}
      <p class="warn">No sound yet: give it one from a WAV (on the right). The build refuses a cell that plays an instrument with no sample.</p>
    {:else if o.plays.length > 1}
      <p class="warn">Its key map plays samples {o.plays.join(", ")} on the song's notes: a GAX2 instrument has one sample for every note, so the build refuses it. Make the key map play one of them on those notes, or play the other notes on another instrument (a copy, its key map playing the other sample).</p>
    {/if}
    <h3>Volume envelope</h3>
    {#if x.envelope}
      <EnvelopeGraph {points} sustain={x.envelope.sustain} loopStart={x.envelope.loop_start} loopEnd={x.envelope.loop_end} onchange={envelope} top={64} scale="XM's: GAX2's is 4 times it" />
    {:else}
      <p class="hint">Off: full volume, cut at the key off.</p>
    {/if}
    {#if x.samples.length}
      <h3>Key map <span class="hint">the sample each note plays (a click: sample {k} on that note); the song's notes are marked</span></h3>
      <table class="keys">
        <tbody>
          {#each octaves as oct (oct)}
            <tr>
              <th>{oct}</th>
              {#each Array.from({ length: 12 }, (_, i) => oct * 12 + i + 1) as key (key)}
                {@const s = x.keymap[key - 1] ?? 0}
                <td>
                  <button class:played={o.notes.includes(key)} class:mine={s === k} title="{M.noteName(key)} plays sample {s}{o.notes.includes(key) ? ' (the song plays it)' : ''}" onclick={() => keys(key, key, k)}>{s}</button>
                </td>
              {/each}
            </tr>
          {/each}
        </tbody>
      </table>
      <p class="hint">
        {o.notes.length ? `The song plays ${o.notes.map(M.noteName).join(" ")} on it` : "No cell of the song plays it (C-4's sample stands for it)"}: the build makes GAX2's instrument from sample {built}{x.samples.length > 1 ? `, and its other samples aren't heard` : ""}.
      </p>
      <h3>Sample {k}{x.samples.length > 1 ? ` of ${x.samples.length}` : ""}{k === built ? " (the one GAX2 plays)" : ""}</h3>
      {#if sound}
        <Waveform data={sound} height={90} label="the sample's waveform" />
        <p class="hint">
          {sound.length} samples{o.saved?.sounds[k]?.bits === 16 && !mu.state.xmInstruments[song]?.[String(n)]?.sounds?.[String(k)] ? " (16-bit, read as 8)" : ""} · {smp!.loop === "none" ? "no loop: it plays to its end" : `${smp!.loop} loop ${smp!.loop_start}-${smp!.loop_end} (${smp!.loop_end - smp!.loop_start} samples)`} · C-4 plays it at {rate(smp!)} Hz
        </p>
      {:else}
        <p class="warn">{o.problems[k] ?? "no sound"}: give it a new sound from a WAV.</p>
      {/if}
    {/if}
    {#if problem}<p class="warn">{problem}: the save refuses it.</p>{/if}
    <div class="row">
      <label>Note <select bind:value={note}>{#each noteNames as key (key)}<option value={key}>{M.noteName(key)}</option>{/each}</select></label>
      <label title="the rows (at speed 6, 6 ticks each) before the key off">Key off after <input type="number" min="1" max="200" bind:value={hold} style="width: 4rem" /> rows</label>
      {#if mu.playing === "instrument"}
        <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
      {:else}
        <button class="primary" disabled={!!problem || !x.samples.length} onclick={() => mu.playOwn(song, n, note, Math.max(1, hold))} title="the note on one channel, as a song plays it (GAX2, the pool the build makes with the unsaved edits: sample {built} on every note)">▶ Play the note</button>
      {/if}
    </div>
  </div>
{:else}
  {@const cant = mu.cannotDeleteOwn(song, n)}
  <div class="panel">
    <section>
      <h3>Instrument {n} <span class="hint">songs/{song}.xm's</span></h3>
      <label class="name" title="at most {M.XM_NAME} ASCII characters; not &quot;Instrument N&quot; (the pool's), nor a JSON instrument's (that one plays then)">Name <input type="text" maxlength={M.XM_NAME} value={x.name} onchange={(e) => rename(e.currentTarget.value)} /></label>
      <div class="row">
        {#if o.edited && !o.added}<button onclick={() => mu.revertOwn(song, n)}>Put the saved one back</button>{/if}
        {#if o.added}<button disabled={!!cant} title={cant ?? "take it out of the song's XM (it isn't saved yet)"} onclick={() => mu.deleteOwn(song, n)}>Delete</button>{/if}
      </div>
    </section>

    <section>
      <h3>Samples</h3>
      {#if x.samples.length}
        <div class="row">
          {#each x.samples as _, i (i)}
            <button class:on={i === k} onclick={() => (mu.ownSample = i)} title={i === built ? "the sample the build makes GAX2's instrument from" : "not heard: the key map doesn't play it on the song's notes"}>{i}{i === built ? " ♪" : ""}{mu.state.xmInstruments[song]?.[String(n)]?.sounds?.[String(i)] ? " *" : ""}</button>
          {/each}
        </div>
      {/if}
      <label class="import" title="an XM instrument has at most {M.XM_SAMPLES} samples">
        {x.samples.length ? "A sample more from a WAV" : "Its sound from a WAV"} (any PCM, or 32-bit float; mixed to mono, 8 bits, at the WAV's rate: the tune set for C-4 to play it there)
        <input bind:this={addInput} type="file" accept=".wav" disabled={x.samples.length >= M.XM_SAMPLES} onchange={addSample} />
      </label>
      {#if smp && x.samples.length}
        <h3>Sample {k}</h3>
        <div class="grid3">
          <label title="the sample's volume, 0-64 (GAX2's: the sequence's set-volume step, 4 times it)">Volume <input type="number" min="0" max="64" value={smp.volume} oninput={(e) => int(num(e.currentTarget.value), 0, 64) && setSample("volume", (s) => ({ ...s, volume: num(e.currentTarget.value) }), "vol")} /></label>
          <label title="semitones from the note">Relative note <input type="number" min="-128" max="127" value={smp.relative_note} onchange={(e) => int(num(e.currentTarget.value), -128, 127) && setSample("relative note", (s) => ({ ...s, relative_note: num(e.currentTarget.value) }))} /></label>
          <label title="128ths of a semitone">Finetune <input type="number" min="-128" max="127" value={smp.finetune} onchange={(e) => int(num(e.currentTarget.value), -128, 127) && setSample("finetune", (s) => ({ ...s, finetune: num(e.currentTarget.value) }))} /></label>
          <label>
            Loop
            <select value={smp.loop} onchange={(e) => setSample("loop", (s) => ({ ...s, loop: e.currentTarget.value as M.XmSample["loop"], ...(s.loop_start < s.loop_end && s.loop_end <= lengths[k]! ? {} : { loop_start: 0, loop_end: lengths[k]! }) }))}>
              <option value="none">none</option><option value="forward">forward</option><option value="pingpong">ping-pong</option>
            </select>
          </label>
          {#if smp.loop !== "none"}
            <label>Loop start <input type="number" min="0" value={smp.loop_start} onchange={(e) => int(num(e.currentTarget.value), 0, lengths[k]! - 1) && setSample("loop start", (s) => ({ ...s, loop_start: num(e.currentTarget.value), loop_end: Math.max(num(e.currentTarget.value) + 1, s.loop_end) }))} /></label>
            <label title="its length: the loop ends at start + length">Loop length <input type="number" min="1" value={smp.loop_end - smp.loop_start} onchange={(e) => int(num(e.currentTarget.value), 1, lengths[k]!) && setSample("loop length", (s) => ({ ...s, loop_end: s.loop_start + num(e.currentTarget.value) }))} /></label>
          {/if}
        </div>
        {#if k === built}
          <p class="hint">GAX2's wave row: tune {gax.tune} (1/32 semitones of gGaxPeriodTable from the note), {smp.loop === "none" ? "no loop" : `${smp.loop} loop`}; volume {Math.min(255, gax.volume)}{smp.volume >= 64 ? " (full: no set-volume step)" : ""}.</p>
        {:else}
          <p class="hint">The key map doesn't play it on the song's notes: GAX2's instrument is made from sample {built}, and this one isn't heard.</p>
        {/if}
        <label class="import">
          Replace sample {k}'s sound with a WAV (at the WAV's rate: the tune set for C-4 to play it there)
          <input bind:this={fileInput} type="file" accept=".wav" onchange={replace} />
        </label>
      {/if}
    </section>

    {#if x.samples.length > 1}
      <section>
        <h3>Key map</h3>
        <div class="row">
          <button onclick={() => keys(1, 96, k)}>Every note plays sample {k}</button>
        </div>
        <div class="row">
          <label>From <select bind:value={from}>{#each noteNames as key (key)}<option value={key}>{M.noteName(key)}</option>{/each}</select></label>
          <label>to <select bind:value={to}>{#each noteNames as key (key)}<option value={key}>{M.noteName(key)}</option>{/each}</select></label>
          <button onclick={() => keys(from, to, k)}>play sample {k}</button>
        </div>
        <p class="hint">A GAX2 instrument has one sample (its sequence picks a wave row by step, not by note): the build makes this one from the sample the key map plays on the song's notes, and refuses two. The key map is kept in the XM as a tracker plays it.</p>
      </section>
    {/if}

    <section>
      <h3>Volume envelope <label class="hint"><input type="checkbox" checked={!!x.envelope} disabled={!x.samples.length} onchange={(e) => set(e.currentTarget.checked ? "the volume envelope on" : "the volume envelope off", (s) => ({ ...s, envelope: e.currentTarget.checked ? { points: [[0, 64]], sustain: 0, loop_start: null, loop_end: null } : null }))} /> on</label></h3>
      {#if x.envelope}
        {@const env = x.envelope}
        <table class="pts">
          <thead><tr><th>#</th><th>tick</th><th>volume</th><th></th></tr></thead>
          <tbody>
            {#each points as p, i (i)}
              <tr>
                <td>{i}</td>
                <td><input type="number" min="0" max="65535" value={p.x} disabled={i === 0} onchange={(e) => envelope(points.map((q, j) => (j === i ? { ...q, x: num(e.currentTarget.value) } : q)), `envelope point ${i}'s tick`)} /></td>
                <td><input type="number" min="0" max="64" value={p.y} onchange={(e) => envelope(points.map((q, j) => (j === i ? { ...q, y: num(e.currentTarget.value) } : q)), `envelope point ${i}'s volume`)} /></td>
                <td><button disabled={i === 0} onclick={() => envelope(points.filter((_, j) => j !== i), `envelope point ${i} out`)}>−</button></td>
              </tr>
            {/each}
          </tbody>
        </table>
        <button disabled={points.length >= M.XM_ENVELOPE_POINTS} onclick={() => envelope([...points, { x: (points[points.length - 1]?.x ?? 0) + 8, y: 0 }], "an envelope point")} title="an XM envelope has at most {M.XM_ENVELOPE_POINTS} points">+ point</button>
        <div class="grid3">
          {#each [["sustain", "Sustain"], ["loop_start", "Loop from"], ["loop_end", "to"]] as [key, label] (key)}
            <label>
              {label}
              <select value={env[key as "sustain"] ?? ""} onchange={(e) => setMark(key as "sustain", e.currentTarget.value === "" ? null : Number(e.currentTarget.value))}>
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
        <label title="XM's depth: GAX2's the same (depth/64 semitones)">Depth <input type="number" min="0" max="255" value={x.vibrato.depth} disabled={!x.samples.length} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato depth", (s) => ({ ...s, vibrato: { ...s.vibrato, depth: num(e.currentTarget.value) } }), "vd")} /></label>
        <label title="XM's rate (256 steps a cycle; GAX2's speed is a quarter of it, of 64)">Rate <input type="number" min="0" max="255" value={x.vibrato.rate} disabled={!x.samples.length} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato rate", (s) => ({ ...s, vibrato: { ...s.vibrato, rate: num(e.currentTarget.value) } }), "vr")} /></label>
      </div>
      <p class="hint">GAX2's vibrato: {gax.vibrato.depth ? `depth ${gax.vibrato.depth}, speed ${gax.vibrato.speed}` : "none (a depth and a rate make one)"}. It is a sine that starts at full depth: the save makes the XM's a sine with no sweep. GAX2 mixes mono: no panning envelope, and a sample's panning isn't read.</p>
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
  .grid3 label,
  .name {
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
    margin: 0.2rem 0;
  }
  .row button.on {
    outline: 2px solid var(--accent);
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
  .keys {
    border-collapse: collapse;
    font-size: 0.7rem;
  }
  .keys th {
    color: var(--muted);
    font-weight: normal;
    padding-right: 0.3rem;
  }
  .keys td {
    padding: 1px;
  }
  .keys button {
    width: 1.6rem;
    padding: 0;
    font-size: 0.7rem;
    font-family: var(--mono);
    color: var(--muted);
  }
  .keys button.mine {
    color: inherit;
    font-weight: bold;
  }
  .keys button.played {
    outline: 2px solid var(--accent);
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

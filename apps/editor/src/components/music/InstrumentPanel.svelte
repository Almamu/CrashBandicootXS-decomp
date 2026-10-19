<script lang="ts">
  // A music pool instrument (gax_manifest.json's record, struct
  // GaxChannelInstrument): in the middle its volume envelope, drawn and
  // edited, its first wave row's sample with the loop marked, and a note
  // played through GAX2 as a song plays it (the backend builds the pool
  // with the unsaved edits); on the right its record: the vibrato, the
  // wave rows (sample, tune as a relative note and finetune, start, loop
  // or sweep), the sequence (its speed, each step's note, wave row and
  // two effects) and the envelope's points, sustain and loop. GAX2 mixes
  // mono: an instrument has no panning envelope (the record's second
  // envelope, `unknown12`, is one the engine never reads).
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import EnvelopeGraph from "./EnvelopeGraph.svelte";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  const n = $derived(mu.instrument);
  const pool = $derived(mu.instrumentPool);
  const sfx = $derived(pool === "sfx");
  const rec = $derived(mu.record(n, pool));
  const listed = $derived(sfx ? undefined : mu.poolList.find((i) => i.index === n));
  const edited = $derived(!!(sfx ? mu.state.sfxInstruments : mu.state.instruments)[String(n)]);
  const songs = $derived(sfx ? [] : mu.instrumentSongs(n));
  const sfxNames = $derived(sfx ? mu.state.sfx.entries.filter((e) => (e as { slot_id?: number }).slot_id === n).map((e) => `SFX_${e.name ?? "?"}`) : []);
  const samples = $derived(sfx ? mu.sfxSamples : mu.poolSamples);
  const marks = $derived(rec ? M.envMarks(rec) : { sustain: null, loopStart: null, loopEnd: null });
  const points = $derived(rec ? M.envPoints(rec) : []);
  const row0 = $derived(rec ? rec.samples[0] : null);
  const wave0 = $derived(rec ? rec.sample_indices[0] : 0);
  const wavePath = $derived(wave0 ? (samples.find((s) => s.index === wave0)?.path ?? "") : "");
  const sound = $derived(wavePath ? mu.sound(wavePath) : null);
  let note = $state(49);
  let hold = $state(16);
  $effect(() => {
    if (wavePath) void mu.loadWav(wavePath);
  });

  const lengthOf = (idx: number) => samples.find((s) => s.index === idx)?.length ?? 0;

  function set(label: string, f: (r: M.InstrumentRecord) => M.InstrumentRecord, key: string | null = null) {
    mu.setRecord(n, label, f, key, pool);
  }

  function setRow(r: number, label: string, f: (w: M.WaveRow) => M.WaveRow, key: string | null = null) {
    set(`wave row ${r + 1}: ${label}`, (x) => ({ ...x, samples: x.samples.map((w, k) => (k === r ? f(w) : w)) }), key ? `row${r}:${key}` : null);
  }

  function setStep(i: number, label: string, f: (s: M.SeqStep) => M.SeqStep, key: string | null = null) {
    set(`step ${i}: ${label}`, (x) => ({ ...x, rows: x.rows.map((s, k) => (k === i ? f(s) : s)) as M.InstrumentRecord["rows"] }), key ? `step${i}:${key}` : null);
  }

  /** the envelope's points changed: the sustain and loop follow their points (one added or taken out moves them) */
  function envelope(pts: M.EnvPoint[], label: string) {
    if (!rec) return;
    const old = points;
    let at = 0;
    while (at < Math.min(old.length, pts.length) && old[at]!.x === pts[at]!.x) at++;
    const move = (v: number | null): number | null => {
      if (v === null) return null;
      if (pts.length > old.length) return v >= at ? v + 1 : v;
      if (pts.length < old.length) return v === at ? null : v > at ? v - 1 : v;
      return v;
    };
    let { sustain, loopStart, loopEnd } = { sustain: move(marks.sustain), loopStart: move(marks.loopStart), loopEnd: move(marks.loopEnd) };
    if (loopStart === null || loopEnd === null) loopStart = loopEnd = null;
    set(label, (x) => M.withEnvelope(x, pts, sustain, loopStart, loopEnd));
  }

  function setMark(which: "sustain" | "loopStart" | "loopEnd", v: number | null) {
    const m = { ...marks, [which]: v };
    if (which !== "sustain" && (m.loopStart === null) !== (m.loopEnd === null)) {
      if (m.loopStart === null) m.loopEnd = null;
      else m.loopEnd = m.loopEnd ?? m.loopStart;
    }
    if (m.loopStart !== null && m.loopEnd !== null && m.loopStart > m.loopEnd) m.loopEnd = m.loopStart;
    set(`envelope ${which}`, (x) => M.withEnvelope(x, points, m.sustain, m.loopStart, m.loopEnd));
  }

  const num = (v: string) => (v.trim() === "" ? NaN : Number(v));
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
  const problem = $derived(M.envelopeProblem(points));
  const rowProblem = (w: M.WaveRow, idx: number) => (idx && M.waveRowNeeds(w) > lengthOf(idx) ? `it reaches sample ${M.waveRowNeeds(w)}, but sample ${idx} has ${lengthOf(idx)}` : null);
  const noteNames = Array.from({ length: 96 }, (_, k) => k + 1);
</script>

{#if !rec}
  <div class="empty"><p>Pick an instrument on the left.</p></div>
{:else if part === "sound"}
  <div class="sound">
    <h2>{sfx ? "Sound-effect instrument" : "Instrument"} {n}{listed?.added ? " (new, not saved)" : listed?.deleted ? " (taken out, not saved)" : edited ? " (edited, not saved)" : ""}</h2>
    {#if sfx}
      <p class="hint">Slot {n} of the sound-effect set: {sfxNames.length ? `played by ${sfxNames.join(", ")}` : "no entry of the table plays it"}. A sound effect plays it at its fixed pitch on a voice, as PlaySfx does.</p>
    {:else}
      <p class="hint">{listed?.deleted ? "Taken out: it becomes the empty instrument (the pool's last ones leave the pool)." : rec.byte_00 ? "The empty instrument: a note on it plays nothing." : `Named "Instrument ${n}" in ${songs.length ? songs.join(", ") : "no song's XM"}.`}</p>
    {/if}
    <h3>Volume envelope</h3>
    {#if rec.envelope}
      <EnvelopeGraph {points} sustain={marks.sustain} loopStart={marks.loopStart} loopEnd={marks.loopEnd} onchange={envelope} />
      {#if problem}<p class="warn">{problem}</p>{/if}
    {:else}
      <p class="hint">No envelope: full volume, cut at the key off.</p>
    {/if}
    <h3>Wave row 1: sample {wave0 || "none"}</h3>
    {#if sound && row0}
      <Waveform data={sound.data} height={90} label="the sample's waveform" />
      <p class="hint">
        {sound.data.length} samples · start {row0.start_position} · {M.loopKind(row0) === "none" ? "no loop: it plays to its end" : M.loopKind(row0) === "sweep" ? `a sweep of ${row0.int_10} samples between ${row0.loop_start} and ${row0.loop_end}` : `${M.loopKind(row0)} loop ${row0.loop_start}-${row0.loop_end} (${row0.loop_end - row0.loop_start} samples)`}
      </p>
    {/if}
    <div class="row">
      {#if !sfx}
        <label>Note <select bind:value={note}>{#each noteNames as k (k)}<option value={k}>{M.noteName(k)}</option>{/each}</select></label>
        <label title="the rows (at speed 6, 6 ticks each) before the key off">Key off after <input type="number" min="1" max="200" bind:value={hold} style="width: 4rem" /> rows</label>
      {/if}
      {#if mu.playing === "instrument"}
        <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
      {:else if sfx}
        <button class="primary" disabled={!!problem} onclick={() => mu.playSfxInstrument(n)} title="as PlaySfx plays a sound effect on it: GAX_fx_ex on voice 0, priority 0, volume 1.0, over the song chosen in the Sound effects tab ({mu.sfxSong})">▶ Play it</button>
      {:else}
        <button class="primary" disabled={!!problem || !!listed?.deleted} onclick={() => mu.playInstrument(n, note, Math.max(1, hold))} title="the note on one channel, as a song plays it (GAX2, the pool with the unsaved edits)">▶ Play the note</button>
      {/if}
    </div>
  </div>
{:else}
  <div class="panel">
    <section>
      <h3>Instrument {n} <span class="hint">{sfx ? "gax_sfx_manifest.json" : "gax_manifest.json"}'s instruments/{n}</span></h3>
      {#if edited && !listed?.added}<button onclick={() => mu.revertRecord(n, pool)}>Put the saved one back</button>{/if}
      <div class="grid3">
        <label title="ticks before the vibrato starts">Vibrato delay <input type="number" min="0" max="255" value={rec.vibrato_delay} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato delay", (x) => ({ ...x, vibrato_delay: num(e.currentTarget.value) }), "vd")} /></label>
        <label title="its depth: depth/64 semitones (0: none)">depth <input type="number" min="0" max="255" value={rec.vibrato_depth} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato depth", (x) => ({ ...x, vibrato_depth: num(e.currentTarget.value) }), "vdp")} /></label>
        <label title="its speed: of GAX2's 64 steps a cycle, a tick">speed <input type="number" min="0" max="255" value={rec.vibrato_speed} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato speed", (x) => ({ ...x, vibrato_speed: num(e.currentTarget.value) }), "vs")} /></label>
      </div>
      <p class="hint">GAX2 mixes mono: no panning envelope (the record's second envelope, unknown12, is one the engine never reads).</p>
    </section>

    {#if rec.envelope}
      <section>
        <h3>Envelope points</h3>
        <table class="pts">
          <thead><tr><th>#</th><th>tick</th><th>volume</th><th></th></tr></thead>
          <tbody>
            {#each points as p, i (i)}
              <tr>
                <td>{i}</td>
                <td><input type="number" min="0" max="65535" value={p.x} disabled={i === 0} onchange={(e) => envelope(points.map((q, k) => (k === i ? { ...q, x: num(e.currentTarget.value) } : q)), `envelope point ${i}'s tick`)} /></td>
                <td><input type="number" min="0" max="255" value={p.y} onchange={(e) => envelope(points.map((q, k) => (k === i ? { ...q, y: num(e.currentTarget.value) } : q)), `envelope point ${i}'s volume`)} /></td>
                <td><button disabled={i === 0} onclick={() => envelope(points.filter((_, k) => k !== i), `envelope point ${i} out`)}>−</button></td>
              </tr>
            {/each}
          </tbody>
        </table>
        <button onclick={() => envelope([...points, { x: (points[points.length - 1]?.x ?? 0) + 8, y: 0 }], "an envelope point")}>+ point</button>
        <div class="grid3">
          {#each [["sustain", "Sustain"], ["loopStart", "Loop from"], ["loopEnd", "to"]] as [k, label] (k)}
            <label>
              {label}
              <select value={marks[k as "sustain"] ?? ""} onchange={(e) => setMark(k as "sustain", e.currentTarget.value === "" ? null : Number(e.currentTarget.value))}>
                <option value="">none</option>
                {#each points as _, i (i)}<option value={i}>point {i}</option>{/each}
              </select>
            </label>
          {/each}
        </div>
      </section>
    {/if}

    <section>
      <h3>Wave rows</h3>
      {#each rec.samples as w, r (r)}
        {@const idx = rec.sample_indices[r]!}
        {@const kind = M.loopKind(w)}
        {@const t = M.tuneParts(w.pitch)}
        {@const bad = rowProblem(w, idx)}
        <fieldset class:unused={!idx}>
          <legend>Row {r + 1}</legend>
          <div class="grid3">
            <label>
              Sample
              <select value={idx} onchange={(e) => set(`wave row ${r + 1}'s sample`, (x) => ({ ...x, sample_indices: x.sample_indices.map((v, k) => (k === r ? Number(e.currentTarget.value) : v)) as M.InstrumentRecord["sample_indices"] }))}>
                <option value={0}>none</option>
                {#each samples as s (s.index)}<option value={s.index}>{s.index}{s.added ? " (new)" : ""} · {s.length}</option>{/each}
              </select>
            </label>
            <label title="the tune: semitones from the note (GAX2's tune / 32)">Relative note <input type="number" min="-1024" max="1023" value={t.note} onchange={(e) => int(num(e.currentTarget.value), -1024, 1023) && setRow(r, "tune", (x) => ({ ...x, pitch: num(e.currentTarget.value) * 32 + t.fine }))} /></label>
            <label title="the tune's 32nds of a semitone (0-31)">Finetune <input type="number" min="0" max="31" value={t.fine} onchange={(e) => int(num(e.currentTarget.value), 0, 31) && setRow(r, "tune", (x) => ({ ...x, pitch: t.note * 32 + num(e.currentTarget.value) }))} /></label>
            <label title="the sample position it starts at">Start <input type="number" min="0" value={w.start_position} onchange={(e) => int(num(e.currentTarget.value), 0, 0x7fffffff) && setRow(r, "start", (x) => ({ ...x, start_position: num(e.currentTarget.value) }))} /></label>
            <label>
              Loop
              <select value={kind} disabled={kind === "sweep"} onchange={(e) => setRow(r, "loop", (x) => M.withLoop(x, e.currentTarget.value as "none", x.loop_start < x.loop_end ? x.loop_start : 0, x.loop_start < x.loop_end ? x.loop_end : Math.max(1, lengthOf(idx))))}>
                <option value="none">none</option><option value="forward">forward</option><option value="pingpong">ping-pong</option>
                {#if kind === "sweep"}<option value="sweep">sweep</option>{/if}
              </select>
            </label>
            {#if kind !== "none"}
              <label>Loop start <input type="number" min="0" value={w.loop_start} onchange={(e) => int(num(e.currentTarget.value), 0, 0x7fffffff) && setRow(r, "loop start", (x) => ({ ...x, loop_start: num(e.currentTarget.value), loop_end: Math.max(num(e.currentTarget.value) + 1, x.loop_end) }))} /></label>
              <label title="its length: the loop ends at start + length">Loop length <input type="number" min="1" value={w.loop_end - w.loop_start} onchange={(e) => int(num(e.currentTarget.value), 1, 0x7fffffff) && setRow(r, "loop length", (x) => ({ ...x, loop_end: x.loop_start + num(e.currentTarget.value) }))} /></label>
            {/if}
            {#if kind === "sweep"}
              <label title="the window's length (int_10)">Sweep length <input type="number" min="0" value={w.int_10} onchange={(e) => int(num(e.currentTarget.value), 0, 0x7fffffff) && setRow(r, "sweep length", (x) => ({ ...x, int_10: num(e.currentTarget.value) }))} /></label>
              <label title="the samples it moves a step (ushort_14)">Sweep step <input type="number" min="0" max="65535" value={w.ushort_14} onchange={(e) => int(num(e.currentTarget.value), 0, 0xffff) && setRow(r, "sweep step", (x) => ({ ...x, ushort_14: num(e.currentTarget.value) }))} /></label>
              <label title="ticks between steps (ushort_16)">Sweep rate <input type="number" min="0" max="65535" value={w.ushort_16} onchange={(e) => int(num(e.currentTarget.value), 0, 0xffff) && setRow(r, "sweep rate", (x) => ({ ...x, ushort_16: num(e.currentTarget.value) }))} /></label>
            {/if}
          </div>
          {#if bad}<p class="warn">{bad}: the save refuses it.</p>{/if}
        </fieldset>
      {/each}
    </section>

    <section>
      <h3>Sequence <span class="hint">a step every</span> <input type="number" min="0" max="255" value={rec.row_speed} style="width: 3.5rem" onchange={(e) => int(num(e.currentTarget.value), 0, 255) && set("sequence speed", (x) => ({ ...x, row_speed: num(e.currentTarget.value) }))} /> <span class="hint">ticks</span></h3>
      <table class="pts">
        <thead><tr><th>#</th><th title="the note: 2 plays the pattern's note (relative), with fixed pitch an absolute one">note</th><th>fixed</th><th title="the wave row it starts (1-4; 0: none)">row</th><th>effect 1</th><th>effect 2</th><th></th></tr></thead>
        <tbody>
          {#each rec.rows as s, i (i)}
            <tr>
              <td>{i}</td>
              <td><input type="number" min="-128" max="255" value={s.relative_note_number} onchange={(e) => int(num(e.currentTarget.value), -128, 255) && setStep(i, "note", (x) => ({ ...x, relative_note_number: num(e.currentTarget.value) }))} /></td>
              <td><input type="checkbox" checked={s.dont_use_note_pitch} onchange={(e) => setStep(i, "fixed pitch", (x) => ({ ...x, dont_use_note_pitch: e.currentTarget.checked }))} /></td>
              <td><input type="number" min="0" max="4" value={s.sample_index} onchange={(e) => int(num(e.currentTarget.value), 0, 4) && setStep(i, "wave row", (x) => ({ ...x, sample_index: num(e.currentTarget.value) }))} /></td>
              {#each [0, 1] as k (k)}
                <td class="fx">
                  <input type="number" min="0" max="255" value={s.effects[k]!.type} title="command" onchange={(e) => int(num(e.currentTarget.value), 0, 255) && setStep(i, `effect ${k + 1}`, (x) => ({ ...x, effects: x.effects.map((f, j) => (j === k ? { ...f, type: num(e.currentTarget.value) } : f)) as M.SeqStep["effects"] }))} />
                  <input type="number" min="0" max="255" value={s.effects[k]!.param} title="parameter" onchange={(e) => int(num(e.currentTarget.value), 0, 255) && setStep(i, `effect ${k + 1}`, (x) => ({ ...x, effects: x.effects.map((f, j) => (j === k ? { ...f, param: num(e.currentTarget.value) } : f)) as M.SeqStep["effects"] }))} />
                </td>
              {/each}
              <td><button disabled={rec.rows.length <= 1} onclick={() => set(`step ${i} out`, (x) => ({ ...x, rows: x.rows.filter((_, k) => k !== i) as M.InstrumentRecord["rows"] }))}>−</button></td>
            </tr>
          {/each}
        </tbody>
      </table>
      <button disabled={rec.rows.length >= 255} onclick={() => set("a sequence step", (x) => ({ ...x, rows: [...x.rows, { ...x.rows[x.rows.length - 1]!, effects: [{ type: 0, param: 0 }, { type: 0, param: 0 }] }] as M.InstrumentRecord["rows"] }))}>+ step</button>
      <p class="hint">Each step (GaxChannelStepInstrumentSeq) can set the note and the wave row, and runs two effects (the pattern's commands: 12 sets the volume, 1/2 slide the pitch, ...); the first step starts the note.</p>
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
  fieldset {
    border: 1px solid var(--line);
    border-radius: 4px;
    margin: 0.3rem 0;
    padding: 0.3rem 0.4rem;
  }
  fieldset.unused {
    opacity: 0.65;
  }
  legend {
    font-size: 0.75rem;
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
  .fx input[type="number"] {
    width: 2.8rem;
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

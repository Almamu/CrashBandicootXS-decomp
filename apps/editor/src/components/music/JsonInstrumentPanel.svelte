<script lang="ts">
  // An instruments/<name>.json instrument (docs/audio.md, "From a WAV"):
  // a WAV under games/xs/sound/ with its rate, loop, volume, volume
  // envelope (XM's volumes, 0-64) and GAX2's own vibrato, which an XM
  // instrument of that name plays and a sound effect names. The build
  // makes a pool instrument of it (crashdata.audio.gax.json_instrument_spec)
  // in each pool that plays it; the save writes its file as edited and its
  // WAV's new sound (crashdata.audio.edit.apply_json_instruments). In the
  // middle its envelope and its sound, played as a song (or a sound
  // effect) plays it; on the right its fields.
  import type { Instrument } from "@crash/schema-types";
  import { editor } from "../../lib/state.svelte.ts";
  import { music as mu } from "../../lib/music/state.svelte.ts";
  import * as M from "../../lib/music/model.ts";
  import EnvelopeGraph from "./EnvelopeGraph.svelte";
  import Waveform from "./Waveform.svelte";

  let { part }: { part: "sound" | "record" } = $props();

  type Env = NonNullable<Instrument["envelope"]>;
  const name = $derived(mu.jsonName);
  const j = $derived(mu.json(name));
  const x = $derived(j?.json ?? null);
  const length = $derived(j?.sound?.data.length ?? 0);
  const rate = $derived(x?.rate ?? j?.sound?.rate ?? 8363);
  const problem = $derived(x ? (j?.problem ?? M.jsonInstrumentProblem(x, length)) : null);
  const points = $derived((x?.envelope?.points ?? []).map(([px, py]) => ({ x: px, y: py })));
  /** the WAV is shared (a pool's numbered sample, or another JSON's): a new sound for it isn't given here */
  const shared = $derived(x ? /^(sfx_)?samples\/\d{2,}\.wav$/.test(x.sample) || (mu.view?.instruments ?? []).some((i) => i.name !== name && i.data.sample === x.sample) : false);
  let note = $state(49);
  let hold = $state(16);
  let song = $state("");
  let fileInput = $state<HTMLInputElement>();
  const noteNames = Array.from({ length: 96 }, (_, i) => i + 1);
  /** the songs that can play it: not a ROM song built with its recorded scaffold (which plays only the pool's) */
  const playable = $derived(mu.songs.filter((s) => !s.info?.rom || mu.state.structures[s.key] || mu.patterns[s.key]?.p.scaffold === false).map((s) => s.key));

  function set(label: string, f: (j: Instrument) => Instrument, key: string | null = null) {
    mu.setJson(name, label, (e) => ({ ...e, json: f(e.json) }), key);
  }

  /** a field set, or taken out at its default (the build's) */
  function field<K extends keyof Instrument>(k: K, v: Instrument[K] | undefined, label: string, key: string | null = null) {
    set(
      label,
      (s) => {
        const out = { ...s };
        if (v === undefined) delete out[k];
        else out[k] = v;
        return out;
      },
      key,
    );
  }

  /** its loop: none (the fields go), or forward or ping-pong over what it was (all of the sample, if nothing) */
  function setLoop(v: string) {
    set("loop", (s) => {
      const out = { ...s };
      delete out.loop;
      delete out.loop_start;
      delete out.loop_end;
      return v === "none" ? out : { ...out, loop: v as Instrument["loop"], loop_start: s.loop_start ?? 0, loop_end: s.loop_end ?? length };
    });
  }

  function setEnv(label: string, f: (e: Env) => Env) {
    set(label, (s) => (s.envelope ? { ...s, envelope: f(s.envelope) } : s));
  }

  /** the envelope's points changed: the sustain and loop follow their points (one added or taken out moves them) */
  function envelope(pts: M.EnvPoint[], label: string) {
    const env = x?.envelope;
    if (!env) return;
    if (pts.length > M.JSON_ENVELOPE_POINTS) {
      editor.error = `an envelope has at most ${M.JSON_ENVELOPE_POINTS} points`;
      return;
    }
    const old = points;
    let at = 0;
    while (at < Math.min(old.length, pts.length) && old[at]!.x === pts[at]!.x) at++;
    const move = (v: number | undefined): number | undefined => {
      if (v === undefined) return undefined;
      if (pts.length > old.length) return v >= at ? v + 1 : v;
      if (pts.length < old.length) return v === at ? undefined : v > at ? v - 1 : v;
      return v;
    };
    const out: Env = { points: pts.map((p) => [p.x, Math.min(64, p.y)] as [number, number]) as Env["points"] };
    const sustain = move(env.sustain);
    const ls = move(env.loop_start);
    const le = move(env.loop_end);
    if (sustain !== undefined) out.sustain = sustain;
    if (ls !== undefined && le !== undefined) Object.assign(out, { loop_start: ls, loop_end: le });
    setEnv(label, () => out);
  }

  function setMark(which: "sustain" | "loop_start" | "loop_end", v: number | null) {
    setEnv(`envelope ${which.replace("_", " ")}`, (e) => {
      const m: Env = { ...e };
      if (v === null) delete m[which];
      else m[which] = v;
      if (which !== "sustain") {
        if (m.loop_start === undefined || v === null) {
          delete m.loop_start;
          delete m.loop_end;
        } else {
          m.loop_end = Math.max(m.loop_end ?? m.loop_start, m.loop_start);
        }
      }
      return m;
    });
  }

  /** a new sound for its WAV, at the WAV's rate: the rate field (if any) goes, and a loop past its end is taken in */
  async function replace(ev: Event) {
    const f = (ev.currentTarget as HTMLInputElement).files?.[0];
    if (fileInput) fileInput.value = "";
    if (!f) return;
    const s = await mu.importWav(f);
    if (!s) return;
    mu.setJson(name, "a new sound", (e) => {
      const json = { ...e.json };
      delete json.rate;
      if ((json.loop ?? "none") !== "none" && (json.loop_end ?? s.length) > s.length) Object.assign(json, { loop_start: Math.min(json.loop_start ?? 0, s.length - 1), loop_end: s.length });
      return { ...e, json, wav: s };
    });
  }

  const num = (v: string) => (v.trim() === "" ? NaN : Number(v));
  const int = (v: number, lo: number, hi: number) => Number.isInteger(v) && v >= lo && v <= hi;
</script>

{#if !j || !x}
  <div class="empty"><p>Pick an instrument on the left.</p></div>
{:else if part === "sound"}
  <div class="sound">
    <h2>instruments/{name}.json{j.added ? " (new, not saved)" : j.edited ? " (edited, not saved)" : ""}</h2>
    <p class="hint">
      An instrument from a WAV (docs/audio.md, "From a WAV"): an XM instrument named {name} plays it, a sound effect names it. {j.songs.length ? `Played by ${j.songs.join(", ")}` : "No song's XM names it"}{j.sfx.length ? `; ${j.sfx.join(", ")} name it` : ""}.
    </p>
    <h3>Volume envelope</h3>
    {#if x.envelope}
      <EnvelopeGraph {points} sustain={x.envelope.sustain ?? null} loopStart={x.envelope.loop_start ?? null} loopEnd={x.envelope.loop_end ?? null} onchange={envelope} top={64} scale="XM's: GAX2's is 4 times it" />
    {:else}
      <p class="hint">None: full volume, cut at the key off.</p>
    {/if}
    <h3>Its sound <span class="hint">{x.sample}</span></h3>
    {#if j.sound}
      <Waveform data={j.sound.data} height={90} label="the sample's waveform" />
      <p class="hint">{length} samples (8-bit, as the build reads it) · {(x.loop ?? "none") === "none" ? "no loop: it plays to its end" : `${x.loop} loop ${x.loop_start ?? 0}-${x.loop_end ?? length}`} · {x.rate === undefined ? `the WAV's rate, ${rate} Hz` : `${rate} Hz`} on C-4 (a sound effect: always)</p>
    {:else}
      <p class="warn">{j.problem}</p>
    {/if}
    {#if problem}<p class="warn">{problem}: the save refuses it.</p>{/if}
    <div class="row">
      <label>Note <select bind:value={note}>{#each noteNames as k (k)}<option value={k}>{M.noteName(k)}</option>{/each}</select></label>
      <label title="the rows (at speed 6, 6 ticks each) before the key off">Key off after <input type="number" min="1" max="200" bind:value={hold} style="width: 4rem" /> rows</label>
      {#if mu.playing === "instrument"}
        <button class="primary" onclick={() => mu.stop()}>■ Stop</button>
      {:else}
        <button class="primary" disabled={!!problem || !(j.songs.length || j.sfx.length)} onclick={() => mu.playJson(name, note, Math.max(1, hold))} title={j.songs.length ? "the note on one channel, as a song plays it (the pool the build makes with the unsaved edits)" : j.sfx.length ? "as a sound effect plays it, over the Sound effects tab's song" : "nothing plays it: the build doesn't add it to a pool (a song can play it: on the right)"}>▶ Play {j.songs.length ? "the note" : "it"}</button>
      {/if}
    </div>
  </div>
{:else}
  <div class="panel">
    <section>
      <h3>instruments/{name}.json</h3>
      {#if j.edited && !j.added}<button onclick={() => mu.revertJson(name)}>Put the saved one back</button>{/if}
      <label class="wide">Comment <input type="text" value={x.comment ?? ""} onchange={(e) => field("comment", e.currentTarget.value || undefined, "comment")} /></label>
      <div class="grid3">
        <label title="the rate C-4 plays the sample at (a song's), or it always plays at (a sound effect's, a fixed pitch); empty: the WAV's">Rate (Hz) <input type="number" min="1" placeholder={String(j.sound?.rate ?? "")} value={x.rate ?? ""} onchange={(e) => (e.currentTarget.value.trim() === "" ? field("rate", undefined, "rate") : num(e.currentTarget.value) > 0 && field("rate", num(e.currentTarget.value), "rate"))} /></label>
        <label title="the sample's volume, 0-64, as XM's (GAX2's: the sequence's set-volume step, 4 times it)">Volume <input type="number" min="0" max="64" value={x.volume ?? 64} oninput={(e) => int(num(e.currentTarget.value), 0, 64) && field("volume", num(e.currentTarget.value), "volume", "vol")} /></label>
        <label>
          Loop
          <select value={x.loop ?? "none"} onchange={(e) => setLoop(e.currentTarget.value)}>
            <option value="none">none</option><option value="forward">forward</option><option value="pingpong">ping-pong</option>
          </select>
        </label>
        {#if (x.loop ?? "none") !== "none"}
          <label>Loop start <input type="number" min="0" value={x.loop_start ?? 0} onchange={(e) => int(num(e.currentTarget.value), 0, length - 1) && set("loop start", (s) => ({ ...s, loop_start: num(e.currentTarget.value), loop_end: Math.max(num(e.currentTarget.value) + 1, s.loop_end ?? length) }))} /></label>
          <label title="the end, exclusive">Loop end <input type="number" min="1" value={x.loop_end ?? length} onchange={(e) => int(num(e.currentTarget.value), (x.loop_start ?? 0) + 1, length) && field("loop_end", num(e.currentTarget.value), "loop end")} /></label>
        {/if}
      </div>
      <p class="hint">GAX2's wave row: tune {M.rateTune(rate)} for a song (1/32 semitones from the note: C-4 at {rate} Hz).</p>
      {#if shared}
        <p class="hint">{x.sample} is a pool's sample or another instrument's: its sound is given in the Samples tab, or in that instrument.</p>
      {:else}
        <label class="import">
          Replace its sound with a WAV ({x.sample}; any PCM, or 32-bit float; mixed to mono, 8 bits, at the WAV's rate)
          <input bind:this={fileInput} type="file" accept=".wav" onchange={replace} />
        </label>
      {/if}
    </section>

    <section>
      <h3>Volume envelope <label class="hint"><input type="checkbox" checked={!!x.envelope} onchange={(e) => field("envelope", e.currentTarget.checked ? { points: [[0, 64]], sustain: 0 } : undefined, e.currentTarget.checked ? "the volume envelope on" : "the volume envelope off")} /> on</label></h3>
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
        <button disabled={points.length >= M.JSON_ENVELOPE_POINTS} onclick={() => envelope([...points, { x: (points[points.length - 1]?.x ?? 0) + 8, y: 0 }], "an envelope point")}>+ point</button>
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
        <p class="hint">XM's volumes: GAX2's points are 4 times them. Without a sustain point the note-off doesn't cut the note; GAX2 runs the loop only until the key off.</p>
      {/if}
    </section>

    <section>
      <h3>Vibrato <span class="hint">GAX2's own</span></h3>
      <div class="grid3">
        <label title="ticks before it starts, 0-255">Delay <input type="number" min="0" max="255" value={x.vibrato?.delay ?? 0} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato delay", (s) => ({ ...s, vibrato: { ...s.vibrato, delay: num(e.currentTarget.value) } }), "vdl")} /></label>
        <label title="0-255: depth/64 semitones">Depth <input type="number" min="0" max="255" value={x.vibrato?.depth ?? 0} oninput={(e) => int(num(e.currentTarget.value), 0, 255) && set("vibrato depth", (s) => ({ ...s, vibrato: { ...s.vibrato, depth: num(e.currentTarget.value) } }), "vd")} /></label>
        <label title="0-63, of 64 steps a cycle">Speed <input type="number" min="0" max="63" value={x.vibrato?.speed ?? 0} oninput={(e) => int(num(e.currentTarget.value), 0, 63) && set("vibrato speed", (s) => ({ ...s, vibrato: { ...s.vibrato, speed: num(e.currentTarget.value) } }), "vs")} /></label>
      </div>
      <p class="hint">GAX2 mixes mono: an instrument has no panning.</p>
    </section>

    <section>
      <h3>A song plays it</h3>
      <div class="row">
        <select bind:value={song}>
          <option value="">a song…</option>
          {#each playable as k (k)}<option value={k}>{k}</option>{/each}
        </select>
        <button disabled={!song} onclick={() => mu.newOwn(song, { json: name })} title="an instrument named {name} added at the end of the song's XM (a name the build reads: it plays this one), which its cells then play">Add it to the song</button>
      </div>
      <p class="hint">A song plays it as an XM instrument named {name} (a name with no sample): this adds one to the song's instruments, for its cells to play. A ROM song built with its recorded scaffold plays only the pool's.</p>
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
  .wide {
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

<script lang="ts">
  // The selected slide's record (struct cutscene_slide): its picture, timing,
  // fades, music cue and sound effect; a record other slides show is edited
  // for all of them, unless the slide gets its own copy first. A picture
  // imported from a PNG replaces a picture or is a new one.
  import type { CutsceneImport } from "@crash/schema-types";
  import { cutscenes as cs } from "../../lib/cutscene/state.svelte.ts";
  import * as M from "../../lib/cutscene/model.ts";
  import Screen from "./Screen.svelte";

  let pending = $state<{ file: string; result: CutsceneImport; pixels: Uint8Array } | null>(null);
  let mapMode = $state<"own" | "picture">("own");
  let newName = $state("");
  let fileInput = $state<HTMLInputElement>();

  const rec = $derived(cs.record);
  const users = $derived(rec ? M.recordUsers(cs.doc, rec.name) : []);
  const pageShared = $derived(cs.ref?.text ? M.pageUsers(cs.doc, cs.ref.text).length > 1 : false);
  const range = (k: string): [number, number] => (cs.view?.numberFields[k] as [number, number]) ?? [-0x80000000, 0x7fffffff];

  function where(w: M.Where): string {
    return `${cs.doc.cutscenes[w.ci]?.constant ?? w.ci}'s slide ${w.si + 1}`;
  }

  function set(patch: Partial<M.SlideRecord>, key: string | null = null) {
    const r = rec;
    if (!r) return;
    cs.apply(`Edit ${r.name}`, (s) => M.setRecord(s, r.name, patch), key ? `${r.name}:${key}` : null);
  }

  function num(field: keyof M.SlideRecord, value: string) {
    const v = Number(value);
    const [lo, hi] = range(field);
    if (!Number.isInteger(v) || v < lo || v > hi) return;
    set({ [field]: v } as Partial<M.SlideRecord>, field);
  }

  async function pick(e: Event) {
    const f = (e.currentTarget as HTMLInputElement).files?.[0];
    if (!f) return;
    const pal = mapMode === "picture" ? cs.picture(rec?.picture)?.palette : undefined;
    const r = await cs.importFile(f, pal);
    if (r) {
      pending = { file: f.name, result: r, pixels: M.fromBase64(r.pixels) };
      newName = f.name.replace(/\.[^.]*$/, "");
    }
    if (fileInput) fileInput.value = "";
  }

  function replace() {
    if (!pending || !rec) return;
    cs.replacePicture(rec.picture, { palette: pending.result.palette, pixels: pending.pixels });
    pending = null;
  }

  function asNew() {
    if (!pending) return;
    cs.addPicture(newName || "picture", `${newName || "a picture"} (the cutscene editor's, from ${pending.file})`, { palette: pending.result.palette, pixels: pending.pixels });
    pending = null;
  }

  const pictureUsers = $derived(rec ? cs.doc.slides.filter((s) => s.picture === rec.picture).flatMap((s) => M.recordUsers(cs.doc, s.name)) : []);
</script>

{#if rec && cs.view && cs.ref}
  {@const v = cs.view}
  <div class="panel">
    <section>
      <h3>Slide {cs.slide + 1} of {cs.current?.constant}</h3>
      <label class="row">
        Record
        <select value={rec.name} onchange={(e) => cs.apply("Show another record", (s) => M.setSlideRecord(s, cs.cutscene, cs.slide, e.currentTarget.value))} title="the slide record (gCutsceneSlideNN) it shows">
          {#each cs.doc.slides as s (s.name)}<option value={s.name}>{s.name}</option>{/each}
        </select>
      </label>
      <label class="row">
        Name
        <input
          value={rec.name}
          spellcheck="false"
          onchange={(e) => {
            const name = e.currentTarget.value.trim();
            if (name && name !== rec.name) set({ name });
            else e.currentTarget.value = rec.name;
          }}
        />
      </label>
      {#if users.length > 1}
        <p class="note">
          Shown by {users.length} slides ({users.map(where).join(", ")}): a change here changes them all.
          <button class="link" onclick={() => cs.apply("Give a slide its own record", (s) => M.ownRecord(s, cs.cutscene, cs.slide))}>Give this one its own copy</button>
        </p>
      {/if}
    </section>

    <section>
      <h3>Picture</h3>
      <select class="wide" value={rec.picture} onchange={(e) => set({ picture: e.currentTarget.value })}>
        {#each cs.picturePaths as p (p)}<option value={p}>{p.replace(/^cutscenes\//, "")}{cs.state.newPictures[p] && !v.pictures.some((x) => x.path === p) ? " (new)" : ""}</option>{/each}
      </select>
      {#if pictureUsers.length > 1}<p class="hint">This picture is shown by {pictureUsers.length} slides.</p>{/if}
      <div class="import">
        <label title="each pixel the nearest of the picture's 256 colours, or the PNG's own (made into 256 GBA colours)">
          Import a PNG (240x160) on
          <select bind:value={mapMode}>
            <option value="own">its own colours</option>
            <option value="picture">this picture's colours</option>
          </select>
        </label>
        <input bind:this={fileInput} type="file" accept="image/png,image/*" onchange={pick} />
      </div>
      {#if pending}
        <div class="pending">
          <Screen picture={{ palette: pending.result.palette, pixels: pending.pixels }} font={v.font} width={240} label="the imported picture" />
          <p class="hint">{pending.file}: {pending.result.how}; {pending.result.colours} colours{pending.result.exact ? "" : " (approximated)"}.</p>
          <button onclick={replace} title="every slide that shows {rec.picture} gets it">Replace {rec.picture.replace(/^cutscenes\//, "")}</button>
          <div class="row">
            <input bind:value={newName} placeholder="name" spellcheck="false" />
            <button onclick={asNew} title="a new picture of the manifest (cutscenes/NN_name.png), shown by this slide">As a new picture</button>
          </div>
          <button class="link" onclick={() => (pending = null)}>Discard</button>
        </div>
      {/if}
    </section>

    <section>
      <h3>Timing</h3>
      <label class="row" title="WaitForKeyPress's frame count: 0 waits for a key (with the buttons on), else the slide goes on after that many frames (60 a second)">
        Wait
        <input type="number" min="0" max={range("wait")[1]} value={rec.wait} onchange={(e) => num("wait", e.currentTarget.value)} />
        <span class="hint">{rec.wait ? `${(rec.wait / 60).toFixed(2)} s` : rec.buttons ? "until A" : "no wait"}</span>
      </label>
      <label class="check" title="buttons = 1: A goes on, Start skips the slides that have it too (SkipSlides); 0: the keys do nothing">
        <input type="checkbox" checked={rec.buttons === 1} onchange={(e) => set({ buttons: e.currentTarget.checked ? 1 : 0 })} />
        A goes on, Start skips
      </label>
      {#if rec.buttons !== 1 && rec.buttons !== 0}<p class="hint">buttons is {rec.buttons}</p>{/if}
      {#if !rec.wait && !rec.buttons}<p class="warn">No wait and no keys: the slide is gone at once.</p>{/if}
    </section>

    <section>
      <h3>Transitions</h3>
      <label class="row" title="BrightnessFade::Start's flags as the slide starts (fading in)">
        Fade in
        <select value={String(rec.fade)} onchange={(e) => set({ fade: Number(e.currentTarget.value) })}>
          <option value="0">from black</option>
          <option value="1">from white</option>
          {#if rec.fade !== 0 && rec.fade !== 1}<option value={String(rec.fade)}>flags {rec.fade}</option>{/if}
        </select>
      </label>
      <label class="row" title="BrightnessFade::Start's flags after the slide (fading out), or none">
        Fade out
        <select value={String(rec.fade_after)} onchange={(e) => set({ fade_after: Number(e.currentTarget.value) })}>
          <option value="-1">none</option>
          <option value="0">to black</option>
          <option value="1">to white</option>
          {#if rec.fade_after > 1 || rec.fade_after < -1}<option value={String(rec.fade_after)}>flags {rec.fade_after}</option>{/if}
        </select>
      </label>
    </section>

    <section>
      <h3>Sound</h3>
      <label class="row" title="the music cue (PlaySong): a song of the song table">
        Music
        <select value={rec.song} onchange={(e) => set({ song: e.currentTarget.value })}>
          {#each v.songs as s (s.name)}<option value={s.name} title={s.comment}>{s.name.replace(/^SONG_/, "").toLowerCase()}</option>{/each}
          {#if !v.songs.some((s) => s.name === rec.song)}<option value={rec.song}>{rec.song} (unknown)</option>{/if}
        </select>
      </label>
      <label class="check" title="duck_music: FadeOutMusic after the slide">
        <input type="checkbox" checked={!!rec.duck_music} onchange={(e) => set({ duck_music: e.currentTarget.checked ? 1 : 0 })} />
        Fade the music out after it
      </label>
      <label class="row" title="the sound effect played as the slide starts">
        Sound
        <select value={rec.sfx} onchange={(e) => set({ sfx: e.currentTarget.value })}>
          {#each v.sfx as s (s.name)}<option value={s.name} title={s.comment}>{s.name.replace(/^SFX_/, "").toLowerCase()}</option>{/each}
          {#if !v.sfx.some((s) => s.name === rec.sfx)}<option value={rec.sfx}>{rec.sfx} (unknown)</option>{/if}
        </select>
      </label>
      <label class="check" title="rearm_sfx: StopSfx on the sound effect after the slide (EndSlide)">
        <input type="checkbox" checked={!!rec.rearm_sfx} onchange={(e) => set({ rearm_sfx: e.currentTarget.checked ? 1 : 0 })} />
        Stop the sound after it
      </label>
    </section>

    <section>
      <h3>Text</h3>
      <label class="check">
        <input type="checkbox" checked={!!cs.ref.text} onchange={(e) => cs.apply(e.currentTarget.checked ? "Give a slide text" : "Take a slide's text away", (s) => M.setHasText(s, cs.cutscene, cs.slide, e.currentTarget.checked))} />
        Text with it {cs.ref.text ? `(${cs.ref.text})` : ""}
      </label>
      {#if pageShared && cs.ref.text}
        <p class="note">
          Other slides show this page too ({M.pageUsers(cs.doc, cs.ref.text).map(where).join(", ")}).
          <button class="link" onclick={() => cs.apply("Give a slide its own page", (s) => M.ownPage(s, cs.cutscene, cs.slide))}>Give this one its own copy</button>
        </p>
      {/if}
    </section>
  </div>
{/if}

<style>
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
    grid-template-columns: 4.5rem 1fr auto;
    gap: 0.35rem;
    align-items: center;
    margin: 0.2rem 0;
  }
  .row input,
  .row select,
  .wide {
    min-width: 0;
    width: 100%;
  }
  .check {
    display: block;
    margin: 0.25rem 0;
  }
  .hint {
    color: var(--muted);
    font-size: 0.78rem;
    margin: 0.2rem 0;
  }
  .note {
    background: var(--warning-bg);
    color: var(--warning);
    padding: 0.3rem 0.4rem;
    border-radius: 4px;
    font-size: 0.78rem;
  }
  .warn {
    color: var(--warning);
    font-size: 0.78rem;
  }
  .import {
    margin-top: 0.4rem;
    display: grid;
    gap: 0.3rem;
  }
  .import input[type="file"] {
    font-size: 0.78rem;
  }
  .pending {
    margin-top: 0.4rem;
    display: grid;
    gap: 0.3rem;
  }
  .pending .row {
    grid-template-columns: 1fr auto;
  }
</style>

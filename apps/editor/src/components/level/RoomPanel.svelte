<script lang="ts">
  // The room's record in the level table (room.json's "record", docs/levels.md
  // "The level table"): its kind, which picks the player's controller, and its
  // colour effect, which SetupRoomBlend turns into BLDCNT/BLDALPHA (BG3 the
  // first target, BG0-2 and the sprites the second). The view draws the blend
  // as the PPU does ("blend" in the layer bar).
  import { BLEND_EFFECTS, KINDS, startMarkerFor } from "../../lib/level/room.ts";
  import { level } from "../../lib/level/state.svelte.ts";

  const record = $derived(level.room?.record);
  const blend = $derived({ effect: record?.blend?.effect ?? 0, eva: record?.blend?.eva ?? 0, evb: record?.blend?.evb ?? 0 });
  const marker = $derived(startMarkerFor(level.room, level.view?.entityTypes ?? []));

  function setBlend(k: "effect" | "eva" | "evb", s: string) {
    const n = Number(s);
    const max = k === "effect" ? 3 : 31;
    if (!Number.isInteger(n) || n < 0 || n > max) return;
    level.setRoomRecord({ blend: { ...blend, [k]: n } });
  }
</script>

{#if record}
  <div class="panel">
    <div class="muted small">{record.symbol}</div>
    <label class="row">Kind
      <select value={record.kind} onchange={(e) => level.setRoomRecord({ kind: e.currentTarget.value as typeof record.kind })}>
        {#each KINDS as k (k.id)}<option value={k.id}>{k.id}</option>{/each}
      </select>
    </label>
    <p class="muted small">{KINDS.find((k) => k.id === record.kind)?.about}</p>
    {#if marker && marker.want && marker.have !== marker.want}
      <p class="warn small">The room's start marker is {marker.have ?? "missing"}; a {record.kind} room places {marker.want} (docs/levels.md, "Entity types").</p>
    {/if}

    <h3>Blend</h3>
    <label class="row">Effect
      <select value={blend.effect & 3} onchange={(e) => setBlend("effect", e.currentTarget.value)}>
        {#each BLEND_EFFECTS as b, i (i)}<option value={i}>{i}: {b.name}</option>{/each}
      </select>
    </label>
    <p class="muted small">{BLEND_EFFECTS[blend.effect & 3]?.about}</p>
    <label class="row" title="BLDALPHA's EVA: BG3's weight, in 16ths (16 and up: all of it)">EVA (BG3)
      <input type="range" min="0" max="16" value={Math.min(blend.eva, 16)} oninput={(e) => setBlend("eva", e.currentTarget.value)} />
      <input type="number" min="0" max="31" value={blend.eva} onchange={(e) => setBlend("eva", e.currentTarget.value)} />
    </label>
    <label class="row" title="BLDALPHA's EVB: the weight of what is under BG3 (BG0-2, the sprites), in 16ths">EVB (under)
      <input type="range" min="0" max="16" value={Math.min(blend.evb, 16)} oninput={(e) => setBlend("evb", e.currentTarget.value)} />
      <input type="number" min="0" max="31" value={blend.evb} onchange={(e) => setBlend("evb", e.currentTarget.value)} />
    </label>
    <p class="muted small">
      A pixel of BG3 in front of BG0-2 or a sprite is (BG3 × EVA + under × EVB) / 16 a channel, at most full (the PPU's weights stop at
      16). Where BG3 is over the backdrop alone it isn't blended. The ROM's blended rooms use 4 and 16: BG3 a quarter as bright, added.
      {#if record.kind === "UNDERWATER" && blend.effect !== 0}With a blend, an underwater room also raises the sprites' priority (SetupRoomBlend).{/if}
    </p>
    <label><input type="checkbox" bind:checked={level.show.blend} /> draw the blend</label>
  </div>
{/if}

<style>
  .panel {
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
    padding: 0.5rem;
    font-size: 0.85rem;
  }
  h3 {
    margin: 0.5rem 0 0;
    font-size: 0.9rem;
  }
  .row {
    display: flex;
    align-items: center;
    gap: 0.4rem;
  }
  .row input[type="number"] {
    width: 4rem;
  }
  .row input[type="range"] {
    flex: 1;
  }
  p {
    margin: 0;
  }
  .muted {
    color: var(--muted);
  }
  .small {
    font-size: 0.78rem;
  }
  .warn {
    color: var(--error);
  }
</style>

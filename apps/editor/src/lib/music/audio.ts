// The music editor's sound output: a GaxPlayer's 8-bit blocks, a frame at
// a time, queued on the browser's audio (Web Audio) a little ahead, at
// the song's mix rate (the browser resamples to its device's rate), with
// the song position of each frame for the pattern view to follow. Like
// the PC port's window, it plays what GAX2 mixed; it doesn't pace a
// game.
import type { GaxPlayer } from "./gax.ts";

export interface Position {
  frame: number;
  order: number;
  row: number;
}

/** A frame's samples as floats (-1..1), as Direct Sound's 8-bit FIFO plays them. */
export function toFloat(block: Int8Array, out: Float32Array, at = 0): void {
  for (let i = 0; i < block.length; i++) out[at + i] = block[i]! / 128;
}

const AHEAD = 0.35; // seconds queued
const CHUNK = 6; // frames a buffer

export class Output {
  private ctx: AudioContext | null = null;
  private timer: ReturnType<typeof setInterval> | null = null;
  private raf = 0;
  private next = 0;
  private frame = 0;
  private positions: { time: number; pos: Position }[] = [];
  private gain: GainNode | null = null;
  playing = false;

  constructor(
    private readonly onPosition: (p: Position) => void,
    private readonly onEnd: () => void,
  ) {}

  /** Plays `player` from its state now; `before(frame)` runs before each frame (a sound effect's trigger),
   * and the output stops after `frames` frames if given. */
  start(player: GaxPlayer, opts: { before?: (frame: number, p: GaxPlayer) => void; frames?: number; volume?: number } = {}): void {
    this.stop();
    const Ctx = window.AudioContext ?? (window as unknown as { webkitAudioContext: typeof AudioContext }).webkitAudioContext;
    const ctx = new Ctx();
    this.ctx = ctx;
    this.gain = ctx.createGain();
    this.gain.gain.value = opts.volume ?? 1;
    this.gain.connect(ctx.destination);
    this.next = ctx.currentTime + 0.05;
    this.frame = 0;
    this.positions = [];
    this.playing = true;
    const block = new Int8Array(player.frames);
    const pump = () => {
      if (!this.ctx) return;
      while (this.next - ctx.currentTime < AHEAD) {
        if (opts.frames !== undefined && this.frame >= opts.frames) {
          if (ctx.currentTime > this.next) this.finish();
          return;
        }
        const n = opts.frames !== undefined ? Math.min(CHUNK, opts.frames - this.frame) : CHUNK;
        const buf = ctx.createBuffer(1, n * player.frames, player.mixRate);
        const data = buf.getChannelData(0);
        for (let k = 0; k < n; k++) {
          opts.before?.(this.frame, player);
          this.positions.push({ time: this.next + (k * player.frames) / player.mixRate, pos: { frame: this.frame, order: player.info.orderPos, row: player.info.row } });
          player.frame(block);
          toFloat(block, data, k * player.frames);
          this.frame++;
        }
        const src = ctx.createBufferSource();
        src.buffer = buf;
        src.connect(this.gain!);
        src.start(this.next);
        this.next += (n * player.frames) / player.mixRate;
      }
    };
    pump();
    this.timer = setInterval(pump, 40);
    const tick = () => {
      if (!this.ctx) return;
      const t = this.ctx.currentTime;
      let last: Position | null = null;
      while (this.positions.length && this.positions[0]!.time <= t) last = this.positions.shift()!.pos;
      if (last) this.onPosition(last);
      this.raf = requestAnimationFrame(tick);
    };
    this.raf = requestAnimationFrame(tick);
  }

  setVolume(v: number): void {
    if (this.gain) this.gain.gain.value = v;
  }

  private finish(): void {
    this.stop();
    this.onEnd();
  }

  stop(): void {
    if (this.timer) clearInterval(this.timer);
    this.timer = null;
    cancelAnimationFrame(this.raf);
    if (this.ctx) void this.ctx.close();
    this.ctx = null;
    this.gain = null;
    this.playing = false;
  }
}

/** Plays a sample's 8-bit data as it is, at `rate` (not through GAX2: a new sound, before it is saved). */
export async function playSample(data: Int8Array, rate: number): Promise<void> {
  const ctx = new AudioContext();
  const buf = ctx.createBuffer(1, Math.max(1, data.length), rate);
  toFloat(data, buf.getChannelData(0));
  const src = ctx.createBufferSource();
  src.buffer = buf;
  src.connect(ctx.destination);
  await new Promise<void>((resolve) => {
    src.onended = () => resolve();
    src.start();
  });
  await ctx.close();
}

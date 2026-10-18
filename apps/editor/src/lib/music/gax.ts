// GAX2 in TypeScript: the music editor's player (docs/editors.md, "The
// music editor"). It is lib/gax's replay and mixer, function for function
// (each one below names its C original), with the ARM routines as
// platform/pc/src/gax_dsp.c writes them in C: the same integer widths, the
// same truncations, the same order of the channels and of the echo, so a
// song or a sound effect comes out as the game mixes it, an 8-bit block a
// frame (GaxMixFrame). It plays what crashdata.audio.edit gives it: the
// song as tools/gax_audio.py builds it (its GaxSongData, its channels'
// order lists, the packed patterns, the mixer's echo taps) and the two
// pools of instruments and samples, from the data, not from the ROM.
//
// What it leaves out: GAX2_jingle (the game doesn't play a jingle), the
// low-pass filter (GaxSongHeader.flags bit 2, which the game never sets),
// the fxEcho path (flags bit 1, never set), and work RAM left over from an
// earlier song (every song starts from zeroed state, as the PC port's first).

import type { MusicEnvelope, MusicInstrument, MusicPool, MusicPoolData, MusicSong, MusicTables } from "@crash/schema-types";

export type GaxTables = MusicTables;
export type GaxInstrument = MusicInstrument;
export type GaxEnvelope = MusicEnvelope;
export type GaxPoolData = MusicPoolData;
/** A song as crashdata.audio.edit.player_song gives it (GaxSongData and the channel types' data). */
export type GaxSongData = Omit<MusicSong, "key" | "id"> & { key?: string; id?: number };
export type GaxPlayerData = MusicPool;

/** A sample (GaxWave): `length` signed 8-bit samples at `offset` in `data`.
 * `data` is the pool's samples laid out as the data set lays them out (the
 * resampler can read a few samples past a sample's end, into the next one,
 * as the GBA reads on in ROM), or a sample of its own. */
export interface GaxWave {
  data: Int8Array;
  offset: number;
  length: number;
}

/** A pool decoded: its instruments and samples by number. */
export interface GaxPool {
  instruments: (GaxInstrument | null)[];
  waves: Map<number, GaxWave>;
}

export function decodeBase64(b64: string): Uint8Array {
  if (typeof atob === "function") {
    const s = atob(b64);
    const out = new Uint8Array(s.length);
    for (let i = 0; i < s.length; i++) out[i] = s.charCodeAt(i);
    return out;
  }
  return new Uint8Array(Buffer.from(b64, "base64"));
}

export function decodePool(p: GaxPoolData): GaxPool {
  const bytes = decodeBase64(p.samples);
  const all = new Int8Array(bytes.buffer, bytes.byteOffset, bytes.length);
  const waves = new Map<number, GaxWave>();
  for (const [k, [offset, length]] of Object.entries(p.waves)) waves.set(Number(k), { data: all, offset, length });
  return { instruments: [...p.instruments], waves };
}

// the C types' wrap-arounds
const u8 = (v: number) => v & 0xff;
const s8 = (v: number) => (v << 24) >> 24;
const u16 = (v: number) => v & 0xffff;
const s16 = (v: number) => (v << 16) >> 16;
const u32 = (v: number) => v >>> 0;

const NO_NOTE = s16(0x8ad0);
const LOWEST_PRIORITY = -0x80000000;

/** GaxFindMixRate: the first of the table's rates at least `rate`, or the last. */
export function findMixRate(rates: number[], rate: number): number {
  for (const r of rates) if (r >= rate) return r;
  return rates[rates.length - 1]!;
}

/** The samples GAX2 mixes a frame at `mixRate` (GAX2_init: mixRate * 1000 / 59727). */
export function frameSamples(mixRate: number): number {
  return Math.floor((mixRate * 1000) / 0xe94f);
}

/** The song position every channel shares: struct GaxInfoHandler. */
export class GaxInfo {
  lastTick = 0;
  firstTick = 0;
  orderPos = 0;
  row = 0;
  speed = 0;
  playing = 0;
  muteTicks = 0;
  tickCounter = 0;
  newRow = 0;
  newOrder = 0;
  volume = 0;
  stopAtEnd = 0;
  songEnded = 0;
  patternBreak = 0;
  breakRow = 0;

  constructor(readonly song: GaxSongData) {}

  /** GaxInfoResetPosition */
  resetPosition(): void {
    this.orderPos = s16(0xffff);
    this.row = 0x4e20;
    this.tickCounter = 0;
    this.speed = 6;
    this.volume = 0xff;
    this.newRow = 0;
    this.newOrder = 0;
    this.patternBreak = 0;
    this.breakRow = 0;
  }

  /** GaxInfoInit */
  init(): void {
    this.firstTick = 0;
    this.lastTick = 0;
    this.playing = 0;
    this.muteTicks = 0;
    this.stopAtEnd = 0;
    this.songEnded = 0;
    this.resetPosition();
  }

  /** GaxInfoPlay */
  play(tick: number): void {
    if (this.lastTick === tick) return;
    if (this.playing === 0) return;
    const song = this.song;
    if ((this.speed & 0xff) !== 0 && this.tickCounter === 0) {
      if (this.patternBreak !== 0) {
        this.patternBreak = 0;
        this.row = s16(song.patternRows);
      } else {
        this.row = s16(this.row + 1);
      }
      if (this.speed >> 8) this.speed = u16((this.speed >> 8) | ((this.speed & 0xff) << 8));
      this.tickCounter = u8(u8(this.speed) - 1);
      if (this.row >= song.patternRows) {
        this.row = 0;
        this.breakRow = 0;
        this.newOrder = 1;
        this.orderPos = s16(this.orderPos + 1);
        if (this.orderPos >= song.orderCount) {
          if (this.stopAtEnd !== 0) {
            this.playing = 0;
            this.speed = 0;
          }
          this.songEnded = 1;
          this.orderPos = s16(song.loopOrder);
        }
      } else {
        this.newOrder = 0;
      }
      this.newRow = 1;
    } else {
      this.tickCounter = u8(this.tickCounter - 1);
      this.newRow = 0;
    }
    if ((this.speed & 0xff) === 0) this.songEnded = 1;
    this.lastTick = tick;
    if (this.firstTick === 0) this.firstTick = tick;
    if (this.muteTicks !== 0) this.muteTicks--;
  }
}

/** gGaxArmResample's work item (GaxChannelMix's struct GaxMixItem). */
interface MixItem {
  src: GaxWave;
  pos: number;
  end: number;
  frames: number;
  done: number;
  volume: number;
  step: number;
  mode: number;
  loopLen: number;
}

/** A tracker channel or a sound-effect voice: struct GaxChannelState. */
export class GaxChannel {
  muted = 0;
  isFirst = 0;
  emptyPattern = 0;
  rowSkip = 0;
  row = 0;
  direction = 0;
  sweepOn = 0;
  sweepDir = 0;
  sweepTimer = 0;
  vol15 = 0;
  envOut = 0;
  vol17 = 0;
  volume = 0;
  volStep15 = 0;
  volStep17 = 0;
  cutDelay = 0;
  cutTimer = 0;
  seqLoopCount = 0;
  fixedPitch = 0;
  released = 0;
  vibratoDelay = 0;
  pendingNote = 0;
  pendingInstrument = 0;
  pitch = 0;
  pitchStep = 0;
  note = 0;
  noteStep = 0;
  vibratoOffset = 0;
  slideTarget = 0;
  slideRate = 0;
  retriggerDelay = 0;
  seqPos = 0;
  envPos = 0;
  vibratoPhase = 0;
  instrument: GaxInstrument | null = null;
  /** the instrument's number, for the editor's display */
  instrumentNumber = 0;
  patternPtr = 0;
  samplePos = 0;
  sweepPos = 0;
  priority = 0;
  delayedNote = 0;
  delayedInstrument = 0;
  mixMode = 0;
  index = 0;

  constructor(
    readonly player: GaxPlayer,
    readonly info: GaxInfo,
    /** the song's instruments and samples (a channel's) or the sound-effect set's (a voice's) */
    readonly pool: GaxPool,
    /** a channel's order list (its type's data); null for a voice */
    readonly orders: [number, number][] | null,
  ) {}

  /** GaxChannelInit (a channel's; its child, the Info handler, too) */
  initChannel(): void {
    this.samplePos = 0;
    this.row = 0;
    this.instrument = null;
    this.note = NO_NOTE;
    this.direction = 1;
    this.vol15 = 0xff;
    this.volume = -1;
    this.muted = 0;
    this.sweepOn = 0;
    this.isFirst = 0;
    this.rowSkip = 0;
    this.emptyPattern = 0;
    this.pendingNote = 0;
    this.pendingInstrument = 0;
    this.retriggerDelay = 0;
    this.slideRate = 0;
    this.slideTarget = 0;
    this.mixMode = 1;
    this.info.init();
  }

  /** GaxFxChannelInit (a voice's) */
  initFx(): void {
    this.samplePos = 0;
    this.row = 0;
    this.instrument = null;
    this.note = NO_NOTE;
    this.direction = 1;
    this.vol15 = 0xff;
    this.volume = -1;
    this.muted = 0;
    this.sweepOn = 0;
    this.isFirst = 0;
    this.pendingNote = 0;
    this.pendingInstrument = 0;
    this.priority = LOWEST_PRIORITY;
    this.mixMode = this.player.fullResampler ? 2 : 1;
  }

  /** GaxChannelPlay */
  playChannel(buf: Int16Array, arg: number): number {
    const info = this.info;
    info.play(arg);
    if (info.muteTicks !== 0) this.instrument = null;
    if (info.playing !== 0) {
      if (this.retriggerDelay !== 0) {
        this.retriggerDelay = s16(this.retriggerDelay - 1);
        if (this.retriggerDelay === 0) this.decodeRow(1);
      }
      if (info.speed !== 0 && info.newRow !== 0) this.decodeRow(0);
    }
    this.stepSequence();
    this.tick();
    return this.muted === 0 ? u8(this.mix(buf, 0)) : 0;
  }

  /** GaxFxChannelPlay */
  playFx(buf: Int16Array): number {
    const info = this.info;
    if (info.muteTicks !== 0) this.instrument = null;
    if (info.playing !== 0) {
      if ((this.pendingNote === 1 || (this.pendingNote !== 0 && this.pendingInstrument !== 0)) && info.muteTicks === 0) {
        this.setNote(this.pendingNote);
        this.setInstrument(this.pendingInstrument);
        this.volStep15 = 0;
        this.pitchStep = 0;
        this.pendingInstrument = 0;
        this.pendingNote = 0;
      }
    }
    this.stepSequence();
    this.tick();
    return this.muted === 0 ? u8(this.mix(buf, 1)) : 0;
  }

  /** the note-cut countdown both play functions share */
  private stepSequence(): void {
    if (this.instrument !== null && this.cutTimer === 0) {
      if (this.cutDelay !== 0) {
        this.stepInstrumentSeq();
        this.cutTimer = u8(this.cutDelay - 1);
      }
    } else {
      this.cutTimer = u8(this.cutTimer - 1);
    }
  }

  /** GaxChannelDecodeRow */
  decodeRow(retrigger: number): void {
    const info = this.info;
    const pat = this.player.patterns;
    let note: number, instrument: number, cmd: number, param: number;
    this.volStep15 = 0;
    this.pitchStep = 0;
    this.retriggerDelay = 0;
    if (retrigger === 0) {
      if (info.newOrder !== 0) {
        this.patternPtr = this.orders![info.orderPos]![0];
        this.rowSkip = 0;
        this.emptyPattern = pat[this.patternPtr++]!;
      }
      if (this.emptyPattern !== 0) return;
      if (this.rowSkip !== 0) {
        this.rowSkip--;
        return;
      }
      const p = this.patternPtr;
      const b0 = pat[p]!;
      let next: number;
      if (b0 === 0xff) {
        this.rowSkip = u8(pat[p + 1]! - 1);
        this.patternPtr = p + 2;
        return;
      }
      if (b0 & 0x80) {
        const packed = b0 & 0x7f;
        if (packed === 0) {
          this.patternPtr = p + 1;
          return;
        }
        if (packed <= 121) {
          note = packed;
          instrument = pat[p + 1]!;
          param = 0;
          cmd = 0;
          next = p + 2;
        } else {
          note = 0;
          instrument = 0;
          cmd = pat[p + 1]!;
          param = pat[p + 2]!;
          next = p + 3;
        }
      } else {
        note = b0;
        instrument = pat[p + 1]!;
        cmd = pat[p + 2]!;
        param = pat[p + 3]!;
        next = p + 4;
      }
      this.patternPtr = next;
      if (cmd === 14 && param >> 4 === 13) {
        this.retriggerDelay = param & 15;
        this.delayedNote = note;
        this.delayedInstrument = instrument;
        return;
      }
    } else {
      note = this.delayedNote;
      instrument = this.delayedInstrument;
      param = 0;
      cmd = 0;
    }
    if (cmd !== 3) this.setNote(note);
    this.setInstrument(instrument);
    switch (cmd) {
      case 1:
        this.pitchStep = s16(param);
        break;
      case 2:
        this.pitchStep = s16(-param);
        break;
      case 3:
        if (param !== 0) {
          this.slideTarget = s16((note - 2) << 5);
          this.slideRate = s16(Math.trunc((this.slideTarget - this.pitch) / param));
        }
        break;
      case 7:
        info.speed = u16((param >> 4) | ((param << 8) & 0xf00));
        info.tickCounter = u8(info.speed - 1);
        break;
      case 10:
        this.volStep15 = s16(param);
        break;
      case 11:
        this.volStep15 = s16(-param);
        break;
      case 12:
        this.vol15 = u8(param);
        break;
      case 13:
        info.patternBreak = 1;
        info.breakRow = param;
        break;
      case 15:
        info.speed = param;
        info.tickCounter = u8(param - 1);
        break;
    }
  }

  /** GaxChannelSetNote */
  setNote(cmd: number): void {
    if (cmd === 1) {
      const inst = this.instrument;
      if (inst !== null && inst.envelope !== null && inst.envelope.sustain === 0xff) {
        this.note = NO_NOTE;
        this.noteStep = 0;
        this.priority = LOWEST_PRIORITY;
      }
      this.released = 1;
    }
    if (cmd > 1) {
      this.pitch = s16((cmd - 2) << 5);
      this.released = 0;
    }
  }

  /** GaxChannelSetInstrument */
  setInstrument(cmd: number): void {
    if (cmd === 0) return;
    const inst = this.pool.instruments[cmd] ?? null;
    this.envPos = 0;
    this.released = 0;
    this.vibratoPhase = 0;
    this.vibratoDelay = inst ? inst.vibratoDelay : 0;
    this.seqPos = 0;
    this.cutTimer = 0;
    this.seqLoopCount = 0;
    this.vol15 = 0xff;
    this.cutDelay = inst ? inst.seqSpeed : 0;
    this.slideRate = 0;
    this.slideTarget = 0;
    this.instrument = inst !== null && inst.empty === 0 ? inst : null;
    if (this.instrument !== null) this.instrumentNumber = cmd;
  }

  /** GaxChannelStepInstrumentSeq */
  stepInstrumentSeq(): void {
    const inst = this.instrument!;
    if (this.seqPos >= inst.seq.length) {
      this.cutDelay = 0;
      return;
    }
    const e = inst.seq[this.seqPos]!;
    this.seqPos = u16(this.seqPos + 1);
    if (e.note !== 0) {
      this.note = s16((e.note - 2) << 5);
      this.fixedPitch = e.fixedPitch;
      if (e.wave !== 0) {
        this.row = u8(e.wave - 1);
        this.samplePos = 0;
        this.direction = 1;
        this.vol17 = 0xff;
        this.sweepOn = 0;
        const r = inst.rows[this.row]!;
        if (r.sweep !== 0 && r.sweepMin < r.sweepMax && r.sweepLen > 0 && r.sweepRate !== 0 && r.sweepStep > 0) {
          this.sweepOn = 1;
          this.sweepPos = r.start;
          this.samplePos = (this.sweepPos << 11) | 0;
          this.sweepTimer = u8(r.sweepRate);
          this.sweepDir = 1;
          if (this.sweepPos + r.sweepLen > r.sweepMax) this.sweepDir = -1;
        } else {
          this.samplePos = (r.start << 11) | 0;
        }
      }
    }
    this.volStep17 = 0;
    this.noteStep = 0;
    for (const fx of e.fx) {
      const cmd = fx >> 8;
      const param = fx & 0xff;
      switch (cmd) {
        case 1:
          this.noteStep = s16(param);
          break;
        case 2:
          this.noteStep = s16(-param);
          break;
        case 5:
          if (this.seqLoopCount === 0) {
            this.seqPos = param;
          } else {
            this.seqLoopCount = u8(this.seqLoopCount - 1);
            if (this.seqLoopCount !== 0) this.seqPos = param;
          }
          break;
        case 6:
          if (this.seqLoopCount === 0) this.seqLoopCount = u8(param ? param + 1 : 0);
          break;
        case 10:
          this.volStep17 = s16(param);
          break;
        case 11:
          this.volStep17 = s16(-param);
          break;
        case 12:
          this.vol17 = u8(param);
          break;
        case 15:
          this.cutDelay = u8(param);
          break;
      }
    }
  }

  /** GaxChannelTick */
  tick(): void {
    if (this.instrument !== null) {
      this.envOut = this.envelopeTick(this.instrument.envelope!);
      this.tickVibrato();
      this.tickSweep();
    }
    let v = this.volStep15 + this.vol15;
    this.vol15 = v > 0xff ? 0xff : v < 0 ? 0 : v;
    v = this.volStep17 + this.vol17;
    this.vol17 = v > 0xff ? 0xff : v < 0 ? 0 : v;
    this.pitch = s16(this.pitch + this.pitchStep);
    this.note = s16(this.note + this.noteStep);
    if (this.slideRate !== 0) {
      const before = (this.slideTarget - this.pitch) < 0;
      this.pitch = s16(this.pitch + this.slideRate);
      if (before !== (this.slideTarget - this.pitch) < 0) {
        this.slideRate = 0;
        this.pitch = this.slideTarget;
        this.slideTarget = 0;
      }
    }
  }

  /** GaxEnvelopeTick, on envPos */
  envelopeTick(env: GaxEnvelope): number {
    const pts = env.points;
    const pos = this.envPos;
    this.envPos = u16(pos + 1);
    if (env.sustain !== 0xff && this.released === 0 && pos === pts[env.sustain]!.pos) this.envPos = pos;
    const last = pts.length - 1;
    if (pos >= pts[last]!.pos) {
      if (pts[last]!.value === 0 && (env.loopEnd === 0xff || env.loopEnd < last)) {
        this.note = NO_NOTE;
        this.noteStep = 0;
        this.priority = LOWEST_PRIORITY;
      }
      this.envPos = pos;
    }
    if (this.released === 0 && env.loopStart !== 0xff && env.loopEnd !== 0xff && pos === pts[env.loopEnd]!.pos) {
      this.envPos = pts[env.loopStart]!.pos;
    }
    let i = 0;
    // past the last point the ROM reads on; the data never gets there
    while (i < last && pts[i]!.pos < pos) i++;
    if (pos === pts[i]!.pos) return pts[i]!.value;
    const slope = pts[i]!.slope;
    i--;
    return u8((((pos - pts[i]!.pos) * slope) >> 8) + pts[i]!.value);
  }

  /** GaxChannelTickVibrato */
  tickVibrato(): void {
    const inst = this.instrument!;
    let result = inst.vibratoDepth;
    if (result !== 0) {
      if (this.vibratoDelay === 0) this.vibratoPhase = (this.vibratoPhase + inst.vibratoSpeed) & 0x3f;
      else this.vibratoDelay = u8(this.vibratoDelay - 1);
      result = (this.player.tables.vibrato[this.vibratoPhase]! * inst.vibratoDepth) >> 8;
    }
    this.vibratoOffset = s16(result);
  }

  /** GaxChannelTickSweep */
  tickSweep(): void {
    if (this.sweepOn === 0) return;
    this.sweepTimer = u8(this.sweepTimer - 1);
    if (this.sweepTimer !== 0) return;
    const old = this.sweepPos;
    const r = this.instrument!.rows[this.row]!;
    this.sweepTimer = u8(r.sweepRate);
    if (this.sweepDir > 0) {
      this.sweepPos = (old + r.sweepStep) | 0;
      if (this.sweepPos + r.sweepLen > r.sweepMax) {
        this.sweepPos = (this.sweepPos - r.sweepStep * 2) | 0;
        this.sweepDir = -1;
      }
    } else {
      this.sweepPos = (old - r.sweepStep) | 0;
      if (this.sweepPos < r.sweepMin) {
        this.sweepPos = (this.sweepPos + r.sweepStep * 2) | 0;
        this.sweepDir = 1;
      }
    }
    this.samplePos = (this.samplePos - (old << 11) + (this.sweepPos << 11)) | 0;
  }

  /** GaxChannelMix: flag 0 for a song's channel, 1 for a voice */
  mix(buf: Int16Array, flag: number): number {
    const inst = this.instrument;
    if (inst === null || this.note === NO_NOTE || this.row > 3) return 0;
    const wave = this.pool.waves.get(inst.waveIdx[this.row]!);
    if (wave === undefined || wave.length === 0) return 0;
    const player = this.player;
    const info = this.info;
    const r = inst.rows[this.row]!;
    let pitch = this.note + this.vibratoOffset;
    if (this.fixedPitch === 0) {
      pitch += this.pitch;
      if (flag === 0) pitch += this.orders![info.orderPos]![1] << 5;
    }
    let idx = u32(pitch + r.tune);
    if (idx > 0xef3) idx = 0xef3;
    const period = player.tables.period[idx]!;
    let vol = this.envOut !== 0xff ? this.envOut : 0x100;
    if (this.vol17 !== 0xff) vol = (vol * this.vol17) >>> 8;
    if (this.vol15 !== 0xff) vol = (vol * this.vol15) >>> 8;
    if (u8(this.volume) !== 0xff) vol = (vol * u8(this.volume)) >>> 8;
    if (info.volume !== 0xff) vol = (vol * info.volume) >>> 8;
    if (flag === 0) vol = u32(vol * player.song.volume) >>> 8;
    // (s64)period * gGaxMixRateReciprocal >> 32: under 2^53, exact as a double
    const step = Math.floor((period * player.reciprocal) / 4294967296);
    const pingpong = r.sweep === 0 && r.sweepMin < r.sweepMax;
    const frames = player.frames;
    const item: MixItem = {
      src: wave,
      pos: this.samplePos,
      end: (wave.length << 11) | 0,
      frames,
      done: 0,
      volume: vol,
      step,
      mode: 0,
      loopLen: this.sweepOn ? (r.sweepLen << 11) | 0 : 0,
    };
    while (item.done < frames) {
      let backward: boolean;
      if (this.direction > 0) {
        if (pingpong) item.end = (r.sweepMax << 11) | 0;
        else if (this.sweepOn) item.end = ((this.sweepPos + r.sweepLen) << 11) | 0;
        else item.end = (wave.length << 11) | 0;
        item.mode = this.isFirst ? 0 : this.mixMode;
        backward = false;
      } else {
        item.end = (r.sweepMin << 11) | 0;
        item.mode = this.isFirst ? 0 : 1;
        backward = true;
      }
      resample(item, buf, backward);
      if (item.done === frames) break;
      if (pingpong) {
        if (r.pingPong !== 0) {
          if (this.direction > 0) item.pos = (item.pos - step * 2) | 0;
          else item.pos = (item.pos + step * 2) | 0;
          this.direction = s8(~this.direction);
        } else {
          item.pos = (item.pos - ((r.sweepMax - r.sweepMin) << 11)) | 0;
        }
      } else if (this.sweepOn) {
        item.pos = (item.pos - (r.sweepLen << 11)) | 0;
      } else {
        if (this.isFirst) buf.fill(0, item.done, frames + 1);
        this.note = NO_NOTE;
        this.noteStep = 0;
        this.priority = LOWEST_PRIORITY;
        break;
      }
    }
    this.samplePos = item.pos;
    return 1;
  }
}

/** gGaxArmResample (gax_dsp.c's HostGaxResample): mode 0 stores, 1 adds,
 * 2 adds at half rate; `backward` is how GaxChannelMix patched the store
 * and add loops. Positions are Q11; `buf` is the 16-bit mix buffer. */
function resample(item: MixItem, buf: Int16Array, backward: boolean): void {
  const volume = item.volume | 0;
  const step = item.step | 0;
  const outEnd = item.frames;
  let out = item.done;
  let pos = item.pos | 0;
  const end = item.end | 0;
  const loopLen = item.loopLen | 0;
  const src = item.src.data;
  const base = item.src.offset;
  if (item.mode === 0 || item.mode === 1) {
    const add = item.mode === 1;
    for (;;) {
      if (out >= outEnd) break;
      if (backward ? !(pos > end) : !(pos < end)) {
        if (loopLen === 0) break;
        pos = (pos - loopLen) | 0;
      }
      const s = Math.imul(src[base + (pos >> 11)] ?? 0, volume) >> 8;
      buf[out] = add ? s + buf[out]! : s;
      out++;
      pos = backward ? (pos - step) | 0 : (pos + step) | 0;
    }
  } else {
    // the half-rate loop: a word a step, two outputs; an odd halfword is
    // the ARM7TDMI's rotated load and aligned-down store of its word
    const u = new Uint16Array(buf.buffer, buf.byteOffset, buf.length);
    while (pos < end && out < outEnd) {
      const s = Math.imul(src[base + (pos >> 11)] ?? 0, volume) >> 8;
      const both = u32((s << 16) | (s & 0xffff));
      const lo = out & ~1;
      const word = out & 1 ? u[lo + 1]! | (u[lo]! << 16) : u[lo]! | (u[lo + 1]! << 16);
      const sum = u32(both + u32(word));
      u[lo] = sum & 0xffff;
      u[lo + 1] = sum >>> 16;
      out += 2;
      pos = (pos + (step << 1)) | 0;
    }
    if (pos > end || out > outEnd) {
      out -= 1;
      pos = (pos - step) | 0;
    }
  }
  item.pos = pos;
  item.done = out;
}

export interface GaxPlayerOptions {
  /** GaxSongHeader.numSfx: the sound-effect voices (StartSong's 3) */
  numSfx?: number;
  /** GaxSongHeader.mixRate, 0xffff for the song's (StartSong's) */
  mixRate?: number;
  /** GaxSongHeader.flags (StartSong leaves 0); bit 5 mixes the voices at half rate */
  flags?: number;
}

/** A GAX2 player of one song: GAX2_init, then frame() for each GAX_play. */
export class GaxPlayer {
  readonly mixRate: number;
  readonly frames: number;
  readonly reciprocal: number;
  readonly fullResampler: boolean;
  readonly patterns: Uint8Array;
  readonly info: GaxInfo;
  readonly channels: GaxChannel[];
  readonly voices: GaxChannel[];
  /** GaxSongHeader.volume, which GAX_play copies into the Info handler (AudioContext's music fades) */
  headerVolume = 0xff;
  private pos = 1;
  private readonly mixWords: Uint32Array;
  readonly mixBuf: Int16Array;
  private readonly flags: number;
  // the echo (GaxCreateHandlers' taps, the delay line, gax_dsp.c's sEchoPos)
  private readonly echoTaps: number[] | null = null;
  private readonly echoBuf: Int16Array | null = null;
  private echoPos = 0;

  constructor(
    readonly tables: GaxTables,
    readonly music: GaxPool,
    readonly sfx: GaxPool,
    readonly song: GaxSongData,
    opts: GaxPlayerOptions = {},
  ) {
    const numSfx = opts.numSfx ?? 3;
    this.flags = opts.flags ?? 0;
    const want = opts.mixRate === undefined || opts.mixRate === 0xffff ? song.mixRate : opts.mixRate;
    this.mixRate = findMixRate(tables.mixRates, want);
    this.frames = frameSamples(this.mixRate);
    // GaxChannelInit: 2^32 / mixRate (__udivdi3)
    this.reciprocal = Math.floor(4294967296 / this.mixRate);
    this.fullResampler = song.halfRateFx !== 0 || (this.flags & 0x20) !== 0;
    this.patterns = decodeBase64(song.patterns);
    // the mix buffer: (frames + 4) halfwords, word aligned
    const words = Math.ceil(((this.frames + 4) * 2) / 4);
    this.mixWords = new Uint32Array(words);
    this.mixBuf = new Int16Array(this.mixWords.buffer);
    const dsp = song.dsp;
    const maxRate = Math.max(dsp[1]!, dsp[3]!, dsp[5]!);
    if (maxRate !== 0) {
      const len = u32(Math.floor((maxRate * this.mixRate) / 1000) * 2);
      this.echoBuf = new Int16Array(len >> 1);
      this.echoTaps = [0, 1, 2].flatMap((i) => [u32(Math.floor((dsp[i * 2 + 1]! * this.mixRate) / 1000) * 2), dsp[i * 2 + 2] ?? 0]);
    }
    this.info = new GaxInfo(song);
    this.channels = song.orders.map((orders, i) => {
      const c = new GaxChannel(this, this.info, music, orders);
      c.index = i;
      return c;
    });
    this.voices = Array.from({ length: numSfx }, () => new GaxChannel(this, this.info, sfx, null));
    // GaxMixerInit: every child's init
    for (const c of this.channels) c.initChannel();
    for (const v of this.voices) v.initFx();
    this.info.stopAtEnd = (this.flags >> 3) & 1;
  }

  /** GAX_fx_ex: queues sound-effect instrument `instrument` on voice `channel` (-1: the lowest priority's) */
  fx(instrument: number, channel: number, priority: number, pitch = -1): number {
    let sel = -1;
    let best = priority;
    if (channel === -1) {
      this.voices.forEach((v, i) => {
        if (v.priority <= best) {
          sel = i;
          best = v.priority;
        }
      });
    } else {
      sel = channel;
      if (u32(sel) >= this.voices.length) sel = -1;
      if (sel !== -1 && priority < this.voices[sel]!.priority) sel = -1;
    }
    // the ROM's test is against 0x0fffffff, so a refused voice (-1) is written too: past the voices, nothing here
    const v = this.voices[sel];
    if (v !== undefined) {
      v.pendingNote = u8(pitch !== -1 ? (pitch >> 5) + 2 : 8);
      v.pendingInstrument = u8(instrument);
      v.priority = priority | 0;
    }
    return sel;
  }

  /** GAX_stop_fx */
  stopFx(channel: number): void {
    if (channel === -1) for (const v of this.voices) v.pendingNote = 1;
    else if (channel >= 0 && channel < this.voices.length) this.voices[channel]!.pendingNote = 1;
  }

  /** GAX_set_music_volume */
  setMusicVolume(channel: number, volume: number): void {
    const vol = s8(Math.min(volume, 0xff));
    if (channel === -1) for (const c of this.channels) c.volume = vol;
    else if (channel > -2 && channel < this.channels.length) this.channels[channel]!.volume = vol;
  }

  /** GAX_set_fx_volume */
  setFxVolume(channel: number, volume: number): void {
    const vol = s8(Math.min(volume, 0xff));
    if (channel === -1) for (const v of this.voices) v.volume = vol;
    else if (channel > -2 && channel < this.voices.length) this.voices[channel]!.volume = vol;
  }

  /** GAX_play: the next frame's 8-bit block (frames samples at mixRate) */
  frame(out: Int8Array = new Int8Array(this.frames)): Int8Array {
    this.info.volume = Math.min(this.headerVolume, 0xff);
    this.info.playing = 1;
    this.mixFrame(out);
    return out;
  }

  /** frames() blocks one after the other */
  render(count: number): Int8Array {
    const out = new Int8Array(count * this.frames);
    for (let i = 0; i < count; i++) this.frame(out.subarray(i * this.frames, (i + 1) * this.frames));
    return out;
  }

  /** GaxMixFrame */
  private mixFrame(out: Int8Array): void {
    const n = this.channels.length + this.voices.length;
    const gain = n !== 1 ? (n + 8) << 6 : 0x400;
    const samples = this.frames;
    if (this.mixerPlay(this.pos++)) {
      // gGaxArmDownmix (HostGaxDownmix): at least once
      let i = 0;
      do {
        let v = Math.imul(this.mixBuf[i]!, gain) >> 10;
        if (v < -128) v = -128;
        if (v > 127) v = 127;
        out[i] = v;
        i++;
      } while (i < samples);
    } else {
      out.fill(0);
    }
  }

  /** GAX_MIXER_CLEAR: frames halfwords, a word at a time */
  private clear(): void {
    this.mixWords.fill(0, 0, Math.ceil((this.frames * 2) / 4));
  }

  /** GaxMixerPlay (fxEcho and the filter left out: the game sets neither) */
  private mixerPlay(arg: number): number {
    const buf = this.mixBuf;
    const children = [...this.channels, ...this.voices];
    const count = this.channels.length;
    const primary = this.song.dsp[0]!;
    let result = 0;
    const play = (i: number) => {
      const c = children[i]!;
      c.isFirst = result === 0 ? 1 : 0;
      result |= i < count ? c.playChannel(buf, arg) : c.playFx(buf);
    };
    for (let i = 0; i < primary; i++) play(i);
    if (this.song.dsp[1] !== 0) {
      if (result === 0) {
        this.clear();
        result = 1;
      }
      let allMuted = true;
      if (this.info.volume !== 0) {
        for (let i = 0; i < count && allMuted; i++) if (u8(this.channels[i]!.volume) !== 0) allMuted = false;
      }
      if (!allMuted) this.echo();
    }
    for (let i = primary; i < count; i++) play(i);
    if (result === 0 && this.fullResampler) {
      this.clear();
      result = 1;
    }
    for (let i = count; i < count + this.voices.length; i++) play(i);
    return result;
  }

  /** gGaxArmEcho (HostGaxEcho) over the mix buffer */
  private echo(): void {
    const taps = this.echoTaps!;
    const echo = this.echoBuf!;
    const len = echo.length * 2;
    const buf = this.mixBuf;
    const tap = (delay: number) => {
      let at = (this.echoPos + len - delay) | 0;
      if (at >= len) at -= len;
      return echo[at >> 1]!;
    };
    let i = 0;
    do {
      if (this.echoPos === len) this.echoPos = 0;
      let acc = Math.imul(tap(taps[0]!), taps[1]!) >> 4;
      if (taps[2] !== 0) {
        acc += Math.imul(tap(taps[2]!), taps[3]!) >> 4;
        if (taps[4] !== 0) acc += tap(taps[4]!) >> 4;
      }
      const v = buf[i]! + acc;
      buf[i] = v;
      echo[this.echoPos >> 1] = v;
      i++;
      this.echoPos += 2;
    } while (i < this.frames);
  }

  /** where the song is: GaxInfoHandler's order position and row */
  get position(): { order: number; row: number; ended: boolean } {
    return { order: this.info.orderPos, row: this.info.row, ended: this.info.songEnded !== 0 };
  }
}

#include "core.h"

/* GitHub issue #39: 0x08024810-0x08024E68 (game_loop) - the remainder of
 * the UpdateGameFrame-MainLoop cluster between the sound-channel-handle
 * family (game_loop37.c/game_loop38.c) and the terrain-tile decode cache
 * (game_loop3.c, GitHub issue #40). docs/rom_map.md's "A new find: a
 * custom RLE/delta token-stream decoder" and its two follow-up sections
 * ("Follow-up: resolved the semantics by tracing callers", "The
 * background streamer's missing 'level load' half") already
 * characterized this whole cluster from a read-only pass - this is
 * where it gets turned into (attempted) byte-exact C.
 *
 * Two systems share this address range:
 *
 * - `sub_8024810`/`sub_8024820`/`sub_802493C`/`sub_8024948` extend
 *   `struct SoundChannelList` (game_loop37.c/game_loop38.c) with more
 *   fields: `sub_8024804` (already matched, game_loop20.c) sets
 *   `+0xc` (the VRAM-bank toggle) to 1 - `sub_8024948` extends that same
 *   constructor to also zero two new fields, `+0x10`/`+0x14`.
 *   `sub_8024820` is a second per-frame driver loop over the same
 *   `+0/+4` items/count pair `sub_8024640` (game_loop37.c) already
 *   drives, but interleaved with an explicit OAM-shadow-buffer flush
 *   (`sub_8006A90`/`sub_8006A48`/`sub_80006A8`/`sub_8006AAC` on
 *   `gUnknown_03001300`, the same "HUD-icon-plus-number renderer" OAM
 *   pacing pattern docs/matching.md documents elsewhere) and a nested
 *   text-paging loop through a second per-item record array at `+0x10`
 *   (each record `{void **strings; s32 count;}`), rendering each string
 *   via `sub_8000EE4` (text_layout.c) against an `icon_manager *` at
 *   `+0x14` and a 2-word "box" at `+0x18`/`+0x1c`, continuing to the
 *   next string in the current record while a held-input mask (9,
 *   versus `sub_8024640`'s 8) stays set. `+0x24` feeds `sub_8037E54`
 *   (value/divisor) to compute the per-call text-wrap `limit`.
 *   `sub_802493C` is a plain two-argument forwarding trampoline to
 *   `sub_80247EC` (game_loop20.c).
 *
 * - `sub_8024960` through `sub_8024E24` are the "visual scrolling
 *   background streamer" docs/rom_map.md names: a circular 4x4-block
 *   (64 halfword columns x 32 rows, 0x1000 bytes total) ring-buffer
 *   tilemap fed by the same custom RLE/delta token-stream decoder
 *   (`sub_8024960`) the terrain-tile cache's `sub_8025334`
 *   (game_loop3.c) also uses, just writing into a 2D buffer (row
 *   stride 64 halfwords) instead of a flat one. `sub_8024AA0` is the
 *   per-frame driver: it right-shifts the world position by 7/6 (128/64
 *   px tile granularity), and on each axis the camera crosses a tile
 *   boundary, streams in exactly the newly-exposed row (`sub_8024BAC`)
 *   or column (`sub_8024C08`) via `sub_8024960`, while a mod-4
 *   "sub-block" accumulator (`+0x14`/`+0x15`) tracks which of the ring
 *   buffer's 4 blocks is now the logical edge. `sub_8024C64` is the
 *   "level load" half - seeds the ring buffer's *entire* initial
 *   contents the same way, from a fresh camera position. `sub_8024B18`/
 *   `sub_8024B48`/`sub_8024B78` convert a world pixel position into the
 *   ring buffer's wrapped (col, row) address; `sub_8024B78` combines
 *   both and returns the decoded halfword value directly.
 *   `sub_8024CF0`/`sub_8024D0C`/`sub_8024D38`/`sub_8024D58`/
 *   `sub_8024D5C`/`sub_8024D60`/`sub_8024D6C`/`sub_8024D74`/
 *   `sub_8024DAC`/`sub_8024DCC`/`sub_8024DE0`/`sub_8024DFC`/
 *   `sub_8024E24` round out the streamer object's own construction
 *   (allocates the 0x1000-byte ring buffer, wires up two
 *   `sub_803AD80`-style interworking-trampoline tables -
 *   `gStaticData_087E4BDC`/`gStaticData_087E4BEC` - for notifying a
 *   parent object of size/position changes), plain position/clamp
 *   accessors, and the Q8 scale/accumulate step
 *   (`sub_8024DFC`/`sub_8024E24`) game_loop3.c's `sub_8024E68`/
 *   `sub_8024E90` already call into.
 *
 * None of this cluster's fields are given a named struct here, for the
 * same reason game_loop3.c's own viewport/parallax-layer object isn't:
 * several fields are only ever written/read by functions on both sides
 * of this ROM range (this file, game_loop3.c, and still-raw neighbors),
 * so committing a struct now risks getting a field wrong that only
 * surfaces once the rest is matched. */

extern void sub_8024804(void *self);
extern void sub_80247EC(void *self, s32 flags);

/* Trivial wrapper: runs `sub_8024804`'s reset, then returns `self`
 * unchanged (a "chained constructor" idiom this project sees a lot of -
 * see e.g. `sub_8024D38` below for another instance). */
void *sub_8024810(void *self)
{
    sub_8024804(self);
    return self;
}

/* Per-frame driver loop over `self`'s `+0/+4` item list (the same
 * `struct SoundChannelList` shape `sub_8024640` (game_loop37.c) drives),
 * interleaved with an explicit OAM-shadow-buffer flush and a nested
 * text-paging walk through a second per-item record array at `+0x10`.
 * See this file's header comment for the full shape.
 *
 * A first-pass plain-C reconstruction reproduced this control flow
 * exactly (confirmed against the ROM disassembly instruction-by-
 * instruction), but this compiler never assigns `self`/the running
 * item index/the held-input flag byte to the ROM's own `r4`/`r8`/`r7`
 * triple simultaneously live across the many calls this loop makes
 * (`sub_8024708`, the four-call OAM-shadow flush, `sub_8024590`,
 * `sub_8000EE4`, `sub_80010E0`, `sub_8024790`, `sub_80246D8`) - the
 * same register-allocation-permutation gap this file's background-
 * streamer functions hit, just with more live values at once. Closed as
 * a NAKED transcription instead, hand-transcribed instruction-for-
 * instruction from the ROM disassembly, including its own mid-function
 * literal pool (the `gUnknown_03001300` address, loaded three times
 * from the one pool slot placed right where the ROM's own conditional
 * branch jumps over it). */
NAKED void sub_8024820(void *self, void *box)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "ldr r2, [r4, #0x14]\n\t"
        "mov r3, #0x8c\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r2, r3\n\t"
        "str r1, [r0]\n\t"
        "ldr r0, [r4, #0x24]\n\t"
        "add r3, r3, #4\n\t"
        "add r1, r2, r3\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_8037E54\n\t"
        "str r0, [sp, #4]\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "b 9f\n\t"
    "1:\n\t"
        "mov r7, #1\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_8024708\n\t"
        "ldr r1, 2f\n\t"
        "ldr r0, [r1]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r2, 2f\n\t"
        "ldr r0, [r2]\n\t"
        "bl sub_8006A48\n\t"
        "bl sub_80006A8\n\t"
        "ldr r3, 2f\n\t"
        "ldr r0, [r3]\n\t"
        "bl sub_8006AAC\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_8024590\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "mov r2, r8\n\t"
        "lsl r1, r2, #3\n\t"
        "add r0, r1, r0\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov sb, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r1, [r4]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "ldrb r1, [r1, #0x10]\n\t"
        "mov r2, #9\n\t"
        "bl sub_80010E0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r7, r0, #0x18\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_03001300\n"
    "3:\n\t"
        "mov r2, #0\n\t"
        "cmp r2, r0\n\t"
        "bge 8f\n\t"
    "4:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, sb\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r5, [r0]\n\t"
        "mov r6, #0\n\t"
        "ldrb r0, [r5]\n\t"
        "add r2, r2, #1\n\t"
        "mov sl, r2\n\t"
        "b 6f\n\t"
    "5:\n\t"
        "add r0, r5, r6\n\t"
        "ldr r1, [r4, #0x14]\n\t"
        "mov r2, #1\n\t"
        "str r2, [sp]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, r2, #0x18\n\t"
        "ldr r3, [sp, #4]\n\t"
        "bl sub_8000EE4\n\t"
        "add r6, r6, r0\n\t"
        "ldr r1, [r4]\n\t"
        "mov r3, r8\n\t"
        "lsl r0, r3, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "ldrb r1, [r1, #0x10]\n\t"
        "mov r2, #9\n\t"
        "bl sub_80010E0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r7, r0, #0x18\n\t"
        "add r0, r5, r6\n\t"
        "ldrb r0, [r0]\n\t"
    "6:\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "cmp r7, #1\n\t"
        "beq 5b\n\t"
    "7:\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, sb\n\t"
        "ldr r0, [r0, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 8f\n\t"
        "cmp r7, #1\n\t"
        "beq 4b\n\t"
    "8:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_8024790\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, r8\n\t"
        "add r2, r7, #0\n\t"
        "bl sub_80246D8\n\t"
        "mov r8, r0\n\t"
        "mov r0, #1\n\t"
        "add r8, r0\n\t"
    "9:\n\t"
        "ldr r0, [r4, #4]\n\t"
        "cmp r8, r0\n\t"
        "blt 1b\n\t"
        "add sp, #8\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* Plain two-argument forwarding trampoline to `sub_80247EC`
 * (game_loop20.c) - a same-shaped alias for a different call site
 * (matches this project's other trivial-wrapper aliases, e.g.
 * `sub_802425C`/`sub_80247EC` themselves). */
void sub_802493C(void *self, s32 flags)
{
    sub_80247EC(self, flags);
}

/* Extends `sub_8024810`'s reset with two more fields this cluster
 * introduces: `+0x10`/`+0x14` (the text-paging record array/its count,
 * per `sub_8024820` above) both start zeroed. */
void *sub_8024948(void *self0)
{
    u8 *self = (u8 *)self0;

    sub_8024810(self);
    *(s32 *)(self + 0x10) = 0;
    *(s32 *)(self + 0x14) = 0;
    return self;
}

/* The custom RLE/delta token-stream decoder (docs/rom_map.md's "A new
 * find" section): looks up a base pointer via `self+4` indexed by
 * `recordId`'s halfword table entry, then decodes a token stream,
 * budget-limited to 0x7f halfwords, with the same three run modes
 * (literal-fill, signed-delta-accumulate, raw-copy) as the terrain-tile
 * cache's `sub_8025334` (game_loop3.c) - just writing into a 2D buffer
 * (row = idx>>4, 64-halfword row stride) instead of a flat one.
 *
 * Same class of gap as `sub_8025334`/`sub_8024F24`/`sub_80250BC`/
 * `sub_8025130`/`sub_8025228` (game_loop3.c, GitHub issue #40): decode
 * mechanics and total size confirmed correct in an isolated `#if
 * NON_MATCHING` C reconstruction, but the ROM keeps several
 * "bytes-written so far" and mask constants alive in specific fixed
 * registers (`r6`/`r7` across the whole loop, `r8`/`ip`/`sb` for the
 * budget counter and repeated `0xf` masks) that this compiler's
 * allocator never reproduces - closed as a NAKED transcription instead,
 * the same escape hatch already used for that whole sibling decoder
 * family. Hand-transcribed instruction-for-instruction from the ROM
 * disassembly (`0x08024960`-`0x08024AA0`, formerly
 * `asm/code_3_2_17_24810.s`). */
NAKED void sub_8024960(void *self, s32 recordId, void *dest)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "add r7, r2, #0\n\t"
        "ldr r4, [r0, #4]\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r4\n\t"
        "ldrh r1, [r1]\n\t"
        "lsl r0, r1, #2\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x7f\n\t"
        "mov r8, r0\n\t"
        "mov r6, #0\n\t"
    "1:\n\t"
        "ldrh r1, [r4]\n\t"
        "ldrb r3, [r4]\n\t"
        "add r4, r4, #2\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #8\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "ldrh r2, [r4]\n\t"
        "add r4, r4, #2\n\t"
        "mov r0, r8\n\t"
        "sub r0, r0, r3\n\t"
        "mov r8, r0\n\t"
        "mov r5, #0xf\n\t"
    "3:\n\t"
        "asr r0, r6, #4\n\t"
        "add r1, r6, #0\n\t"
        "and r1, r5\n\t"
        "lsl r0, r0, #6\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r7\n\t"
        "strh r2, [r0]\n\t"
        "add r6, r6, #1\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #0\n\t"
        "bne 3b\n\t"
        "b 4f\n\t"
    "2:\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #7\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "beq 6f\n\t"
        "mov r2, r8\n\t"
        "sub r2, r2, r3\n\t"
        "mov r8, r2\n\t"
        "ldrh r5, [r4]\n\t"
        "add r4, r4, #2\n\t"
        "asr r0, r6, #4\n\t"
        "mov r1, #0xf\n\t"
        "and r1, r6\n\t"
        "lsl r0, r0, #6\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r7\n\t"
        "strh r5, [r0]\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "add r6, r6, #1\n\t"
        "mov r0, #0xf\n\t"
        "mov ip, r0\n\t"
    "5:\n\t"
        "ldrh r2, [r4]\n\t"
        "add r4, r4, #2\n\t"
        "lsl r1, r2, #0x18\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "asr r1, r1, #0x18\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r5, r0, #0x10\n\t"
        "asr r0, r6, #4\n\t"
        "mov sb, r0\n\t"
        "add r1, r6, #0\n\t"
        "mov r0, ip\n\t"
        "and r1, r0\n\t"
        "mov r0, sb\n\t"
        "lsl r0, r0, #6\n\t"
        "mov sb, r0\n\t"
        "add r1, sb\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r7\n\t"
        "strh r5, [r0]\n\t"
        "add r6, r6, #1\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "asr r2, r2, #0x18\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r5, r0, #0x10\n\t"
        "asr r0, r6, #4\n\t"
        "add r1, r6, #0\n\t"
        "mov r2, ip\n\t"
        "and r1, r2\n\t"
        "lsl r0, r0, #6\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r7\n\t"
        "strh r5, [r0]\n\t"
        "add r6, r6, #1\n\t"
        "sub r0, r3, #2\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #1\n\t"
        "bhi 5b\n\t"
        "cmp r3, #0\n\t"
        "beq 4f\n\t"
        "ldrh r0, [r4]\n\t"
        "add r4, r4, #2\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsl r2, r5, #0x10\n\t"
        "asr r2, r2, #0x10\n\t"
        "asr r0, r0, #0x18\n\t"
        "add r2, r2, r0\n\t"
        "asr r0, r6, #4\n\t"
        "mov r1, #0xf\n\t"
        "and r1, r6\n\t"
        "lsl r0, r0, #6\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r7\n\t"
        "strh r2, [r0]\n\t"
        "add r6, r6, #1\n\t"
        "b 4f\n\t"
    "6:\n\t"
        "mov r0, r8\n\t"
        "sub r0, r0, r3\n\t"
        "mov r8, r0\n\t"
        "mov r2, #0xf\n\t"
    "7:\n\t"
        "asr r0, r6, #4\n\t"
        "lsl r1, r0, #6\n\t"
        "add r0, r6, #0\n\t"
        "and r0, r2\n\t"
        "add r0, r1, r0\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r7\n\t"
        "ldrh r1, [r4]\n\t"
        "strh r1, [r0]\n\t"
        "add r4, r4, #2\n\t"
        "add r6, r6, #1\n\t"
        "sub r0, r3, #1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r3, r0, #0x10\n\t"
        "cmp r3, #0\n\t"
        "bne 7b\n\t"
    "4:\n\t"
        "mov r2, r8\n\t"
        "cmp r2, #0\n\t"
        "blt 8f\n\t"
        "b 1b\n\t"
    "8:\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

extern void sub_8024C08(void *self, s32 col);
extern void sub_8024BAC(void *self, s32 row);

/* Per-frame background-streamer driver (docs/rom_map.md: "Visual
 * scrolling background streamer"): right-shifts the world position by
 * 7/6 (128/64px tile granularity) and, on each axis the camera has
 * crossed a tile boundary since last call, streams in exactly the
 * newly-exposed column (`sub_8024C08`) or row (`sub_8024BAC`) - the
 * tile 4 ahead of the old edge when scrolling forward, or the tile
 * directly behind when scrolling back - while advancing the mod-4
 * sub-block accumulator (`self+0x14`/`self+0x15`) that feeds
 * `sub_8024B18`/`sub_8024B48`/`sub_8024B78`'s wrapped addressing. */
void sub_8024AA0(void *self0, void *worldpos0)
{
    u8 *self = (u8 *)self0;
    s32 *worldpos = (s32 *)worldpos0;
    s32 tileX = worldpos[0] >> 7;
    s32 tileY = worldpos[1] >> 6;
    s32 oldX = *(s32 *)(self + 0xc);
    s32 oldY = *(s32 *)(self + 0x10);

    if (tileX > oldX) {
        sub_8024C08(self, oldX + 4);
        self[0x14] = (self[0x14] + 1) & 3;
    } else if (tileX < oldX) {
        self[0x14] = (self[0x14] - 1) & 3;
        sub_8024C08(self, oldX - 1);
    }
    *(s32 *)(self + 0xc) = tileX;

    if (tileY > oldY) {
        sub_8024BAC(self, oldY + 4);
        self[0x15] = (self[0x15] + 1) & 3;
    } else if (tileY < oldY) {
        self[0x15] = (self[0x15] - 1) & 3;
        sub_8024BAC(self, oldY - 1);
    }
    *(s32 *)(self + 0x10) = tileY;
}

/* Converts a world pixel `(x, y)` into the background streamer's
 * circular ring-buffer address (see this file's header comment):
 * `self+0xc`/`self+0x10` are the last-known tile coordinates,
 * `self+0x14`/`self+0x15` the mod-4 sub-block accumulator
 * `sub_8024AA0` advances. Returns the column-wrapped halfword pointer
 * (col wrapped mod 0x40) and writes the row (wrapped mod 0x20) out
 * through `rowOut`. */
void *sub_8024B18(void *self0, s32 x, s32 y, s32 *rowOut)
{
    u8 *self = (u8 *)self0;
    s32 col = x - *(s32 *)(self + 0xc) * 16;
    s32 row = y - *(s32 *)(self + 0x10) * 8;
    register u8 subX asm("r5");
    register u8 subY asm("r5");
    register s32 shifted asm("r4");
    u8 *base;
    void *ret;

    subX = self[0x14];
    shifted = subX * 16;
    col += shifted;
    subY = self[0x15];
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = *(u8 **)(self + 8);
    col <<= 1;
    ret = base + col;
    *rowOut = row;
    return ret;
}

/* Sibling of `sub_8024B18`: same wrapped `(col, row)` computation, but
 * returns the row-wrapped pointer (row stride 0x80 bytes = 0x40
 * halfwords, matching `sub_8024960`'s own row stride) and writes the
 * column out through `colOut` instead. */
void *sub_8024B48(void *self0, s32 x, s32 y, s32 *colOut)
{
    u8 *self = (u8 *)self0;
    s32 col = x - *(s32 *)(self + 0xc) * 16;
    s32 row = y - *(s32 *)(self + 0x10) * 8;
    register u8 subX asm("r5");
    register u8 subY asm("r5");
    register s32 shifted asm("r4");
    u8 *base;
    void *ret;

    subX = self[0x14];
    shifted = subX * 16;
    col += shifted;
    subY = self[0x15];
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = *(u8 **)(self + 8);
    row <<= 7;
    ret = base + row;
    *colOut = col;
    return ret;
}

/* Combines `sub_8024B18`/`sub_8024B48`'s address computation and
 * returns the decoded halfword value directly, with no out-param. */
u16 sub_8024B78(void *self0, s32 x, s32 y)
{
    u8 *self = (u8 *)self0;
    s32 col = x - *(s32 *)(self + 0xc) * 16;
    s32 row = y - *(s32 *)(self + 0x10) * 8;
    register u8 subX asm("r4");
    register u8 subY asm("r4");
    register s32 shifted asm("r3");
    u8 *base;
    s32 idx;

    subX = self[0x14];
    shifted = subX * 16;
    col += shifted;
    subY = self[0x15];
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = *(u8 **)(self + 8);
    idx = row << 6;
    idx += col;
    idx <<= 1;
    return *(u16 *)(base + idx);
}

/* `sub_8024AA0`'s Y-axis incremental streamer: for the newly-exposed
 * row `row` (bounds-checked against `self+0`'s source descriptor
 * `+0x18` height), streams in up to 4 tiles (the ring buffer's block
 * width) by decoding each via `sub_8024960` into the block-addressed
 * slot `self+8 + ((self+0x15+4)&3)<<10 + ((self+0x14+col+4)&3)<<5`.
 *
 * Same register-allocation-permutation gap as `sub_8024960` above (the
 * ROM packs `self`/the running linear tile index/the loop counter/a
 * constant `3` mask into `r4`/`r5`/`r6`/`r7` simultaneously across a
 * call to `sub_8024960`) - closed the same way: hand-transcribed
 * instruction-for-instruction from the ROM disassembly. */
NAKED void sub_8024BAC(void *self, s32 row)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r2, [r0, #0x18]\n\t"
        "cmp r1, r2\n\t"
        "bge 3f\n\t"
        "ldrh r0, [r0, #0x16]\n\t"
        "add r5, r0, #0\n\t"
        "mul r5, r5, r1\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r5, r5, r0\n\t"
        "mov r6, #0\n\t"
        "mov r7, #3\n\t"
    "1:\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r4]\n\t"
        "ldrh r3, [r1, #0x16]\n\t"
        "cmp r0, r3\n\t"
        "bge 2f\n\t"
        "ldr r2, [r4, #8]\n\t"
        "ldrb r0, [r4, #0x15]\n\t"
        "add r0, r0, #4\n\t"
        "and r0, r7\n\t"
        "lsl r0, r0, #0xa\n\t"
        "add r2, r2, r0\n\t"
        "ldrb r3, [r4, #0x14]\n\t"
        "add r0, r3, r6\n\t"
        "add r0, r0, #4\n\t"
        "and r0, r7\n\t"
        "lsl r0, r0, #5\n\t"
        "add r2, r2, r0\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r0, r5, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "add r5, r5, #1\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8024960\n\t"
    "2:\n\t"
        "add r6, r6, #1\n\t"
        "cmp r6, #3\n\t"
        "ble 1b\n\t"
    "3:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* Sibling of `sub_8024BAC` above: same shape, X axis (`self+0`'s
 * `+0x16` width bound, `self+0x10`'s running tile-row base, `self+0x14`
 * as the per-column sub-block instead of `self+0x15`). Same
 * register-allocation-permutation gap, closed the same way. */
NAKED void sub_8024C08(void *self, s32 col)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r2, [r0, #0x16]\n\t"
        "cmp r1, r2\n\t"
        "bge 3f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r5, r0, #0\n\t"
        "mul r5, r5, r2\n\t"
        "add r5, r5, r1\n\t"
        "mov r6, #0\n\t"
        "mov r7, #3\n\t"
    "1:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r3, [r4]\n\t"
        "ldrh r1, [r3, #0x18]\n\t"
        "cmp r0, r1\n\t"
        "bge 2f\n\t"
        "ldr r2, [r4, #8]\n\t"
        "ldrb r1, [r4, #0x15]\n\t"
        "add r0, r1, r6\n\t"
        "add r0, r0, #4\n\t"
        "and r0, r7\n\t"
        "lsl r0, r0, #0xa\n\t"
        "add r2, r2, r0\n\t"
        "ldrb r0, [r4, #0x14]\n\t"
        "add r0, r0, #4\n\t"
        "and r0, r7\n\t"
        "lsl r0, r0, #5\n\t"
        "add r2, r2, r0\n\t"
        "ldr r1, [r3]\n\t"
        "lsl r0, r5, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "ldrh r3, [r3, #0x16]\n\t"
        "add r5, r3, r5\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8024960\n\t"
    "2:\n\t"
        "add r6, r6, #1\n\t"
        "cmp r6, #3\n\t"
        "ble 1b\n\t"
    "3:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

/* "Level load" constructor half of the background streamer
 * (docs/rom_map.md: "The background streamer's missing 'level load'
 * half"): zeroes the mod-4 sub-block accumulator, seeds
 * `self+0xc`/`self+0x10` from `source`'s world position (the exact same
 * `>>7`/`>>6` shift constants `sub_8024AA0` uses per-frame), then loops
 * the full 4x4 block grid (unlike `sub_8024BAC`/`sub_8024C08`'s single
 * row/column) decoding every tile via `sub_8024960` to seed the ring
 * buffer's entire initial contents.
 *
 * Same register-allocation-permutation gap as `sub_8024960`/
 * `sub_8024BAC`/`sub_8024C08` above (`self`/both loop counters/two
 * block-stride constants packed into `r4`/`r6`/`r7`/`sb`/`sl` across a
 * nested loop and a call to `sub_8024960`) - closed the same way:
 * hand-transcribed instruction-for-instruction from the ROM
 * disassembly. */
NAKED void sub_8024C64(void *self, void *source)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x40\n\t"
        "mov sl, r0\n\t"
        "mov r2, #0x20\n\t"
        "mov sb, r2\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x14]\n\t"
        "strb r0, [r4, #0x15]\n\t"
        "ldr r0, [r1]\n\t"
        "asr r0, r0, #7\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "asr r0, r0, #6\n\t"
        "str r0, [r4, #0x10]\n\t"
        "mov r6, #0\n\t"
    "1:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r4]\n\t"
        "add r3, r6, #1\n\t"
        "mov r8, r3\n\t"
        "ldrh r1, [r1, #0x18]\n\t"
        "cmp r0, r1\n\t"
        "bge 2f\n\t"
        "mov r5, #0\n\t"
        "lsl r0, r6, #3\n\t"
        "mov r7, sl\n\t"
        "mul r7, r7, r0\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "add r3, r5, r0\n\t"
        "ldr r1, [r4]\n\t"
        "ldrh r2, [r1, #0x16]\n\t"
        "cmp r3, r2\n\t"
        "bge 4f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, r0, r6\n\t"
        "mul r0, r0, r2\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrh r1, [r0]\n\t"
        "ldr r2, [r4, #8]\n\t"
        "mov r3, sb\n\t"
        "asr r0, r3, #1\n\t"
        "mul r0, r0, r5\n\t"
        "add r0, r7, r0\n\t"
        "lsl r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8024960\n\t"
    "4:\n\t"
        "add r5, r5, #1\n\t"
        "cmp r5, #3\n\t"
        "ble 3b\n\t"
    "2:\n\t"
        "mov r6, r8\n\t"
        "cmp r6, #3\n\t"
        "ble 1b\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

extern void *gUnknown_03001308;

/* Stores `source` (the room/level descriptor - see this file's header
 * comment) into `self+0`, caches its `+0x1a`/`+0x1c` pixel dimensions at
 * `self+0x18`/`self+0x1c`, and derives `self+4` from
 * `gUnknown_03001308`'s own `+0x24` field plus `source+4` - the same
 * "camera offset + source field" shape as the terrain-tile cache's
 * `decodeBase` (game_loop3.c's `struct tile_cache`). */
void sub_8024CF0(void *self0, void *source0)
{
    u8 *self = (u8 *)self0;
    u8 *source = (u8 *)source0;
    s32 w;
    s32 h;

    *(void **)self = source;
    w = *(u16 *)(source + 0x1a);
    h = *(u16 *)(source + 0x1c);
    *(s32 *)(self + 0x18) = w;
    *(s32 *)(self + 0x1c) = h;
    *(u8 **)(self + 4) = *(u8 **)((u8 *)gUnknown_03001308 + 0x24) + *(s32 *)(source + 4);
}

extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *self);
extern void *sub_8026EC0(u32 size);
extern void *sub_8026EDC(s32 size);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern u8 gStaticData_087E4BDC[];
extern u8 gStaticData_087E4BEC[];

/* Wires up `self+0x20`'s `sub_803AD80`-style interworking-trampoline
 * table (a fixed `gStaticData_087E4BDC`), then tears down `self+8`'s
 * ring buffer (if already allocated - `sub_8024D38` below is the
 * matching constructor) and/or notifies via `sub_8026ED0` if bit 0 of
 * `flags` is set - the same conditional-teardown shape this project
 * sees a lot of (e.g. `sub_80247EC`, game_loop20.c). */
void sub_8024D0C(void *self0, s32 flags)
{
    u8 *self = (u8 *)self0;

    *(u8 **)(self + 0x20) = gStaticData_087E4BDC;
    if (*(void **)(self + 8) != NULL) {
        sub_8026EB4(*(void **)(self + 8));
    }
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Constructor: wires up the same `gStaticData_087E4BDC` trampoline
 * table as `sub_8024D0C` above, allocates the streamer's 0x1000-byte
 * ring buffer (64 halfword columns x 32 rows, per this file's header
 * comment), and returns `self`. */
void *sub_8024D38(void *self0)
{
    u8 *self = (u8 *)self0;

    *(u8 **)(self + 0x20) = gStaticData_087E4BDC;
    *(void **)(self + 8) = sub_8026EC0(0x1000);
    return self;
}

/* Plain accessor: returns `self+0x1c`. */
s32 sub_8024D58(void *self0)
{
    return *(s32 *)((u8 *)self0 + 0x1c);
}

/* Plain accessor: returns `self+0x18`. */
s32 sub_8024D5C(void *self0)
{
    return *(s32 *)((u8 *)self0 + 0x18);
}

/* Plain setter: copies `vec[0]/vec[1]` into `self+0x18`/`self+0x1c`. */
void sub_8024D60(void *self0, void *vec0)
{
    u8 *self = (u8 *)self0;
    s32 *vec = (s32 *)vec0;
    s32 y = vec[1];
    s32 x = vec[0];

    *(s32 *)(self + 0x18) = x;
    *(s32 *)(self + 0x1c) = y;
}

/* Plain setter: stores `x`/`y` into `self+0x18`/`self+0x1c` directly
 * (same fields as `sub_8024D60` above, caller-supplied scalars instead
 * of a vector). */
void sub_8024D6C(void *self0, s32 x, s32 y)
{
    u8 *self = (u8 *)self0;

    *(s32 *)(self + 0x18) = x;
    *(s32 *)(self + 0x1c) = y;
}

/* Wires up `self+0x30`'s second `sub_803AD80`-style trampoline table
 * (`gStaticData_087E4BEC`, a different fixed table from
 * `sub_8024D0C`'s), then - if `self+0x2c`'s child object is already
 * set (per `sub_8024DAC` below) - notifies it via its own `+0x20`
 * trampoline table with a fixed action code `3`, before the same
 * conditional `sub_8026ED0` teardown notify `sub_8024D0C` has. */
void sub_8024D74(void *self0, s32 flags)
{
    u8 *self = (u8 *)self0;
    u8 *child;

    *(u8 **)(self + 0x30) = gStaticData_087E4BEC;
    child = *(u8 **)(self + 0x2c);

    if (child != NULL) {
        u8 *mgr = *(u8 **)(child + 0x20);

        sub_803AD80(child + *(s16 *)(mgr + 8), (void *)3, *(void **)(mgr + 0xc));
    }

    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Constructor: wires up the `gStaticData_087E4BEC` trampoline table
 * (same as `sub_8024D74` above), allocates a 0x24-byte child object and
 * runs `sub_8024D38` on it (the ring-buffer-owning object those
 * `self+0x20`-rooted trampolines above notify), storing the result at
 * `self+0x2c`. */
void *sub_8024DAC(void *self0)
{
    u8 *self = (u8 *)self0;

    *(u8 **)(self + 0x30) = gStaticData_087E4BEC;
    *(void **)(self + 0x2c) = sub_8024D38(sub_8026EDC(0x24));
    return self;
}

/* Clamps `value` to `[-0x10, 0x10]` - `self` (the first argument) is
 * unused. */
s32 sub_8024DCC(void *self0, s32 value)
{
    if (value < -0x10) {
        value = -0x10;
    }
    if (value > 0x10) {
        value = 0x10;
    }
    return value;
}

/* Clamps `out[0]`/`out[1]` against `self+8`/`self+0xc` (an upper bound
 * pair), writing the smaller of each back into `out`. */
void sub_8024DE0(void *self0, s32 *out)
{
    u8 *self = (u8 *)self0;
    s32 a = *(s32 *)(self + 8);

    if (a > out[0]) {
        a = out[0];
    }
    out[0] = a;

    {
        s32 b = *(s32 *)(self + 0xc);

        if (b > out[1]) {
            b = out[1];
        }
        out[1] = b;
    }
}

/* Applies the layer's per-axis scale (`self+0x20`/`self+0x24`) to
 * `vec2`, floor-dividing the Q8 product by 256 (the `+0xff` bias before
 * the arithmetic shift rounds negative products toward negative
 * infinity, matching a true floor division rather than C's
 * truncate-toward-zero `>>`). Called by game_loop3.c's `sub_8024E68`/
 * `sub_8024E90`. */
void sub_8024DFC(void *self0, void *vec20)
{
    u8 *self = (u8 *)self0;
    s32 *vec2 = (s32 *)vec20;
    s32 v;

    v = vec2[0] * *(s32 *)(self + 0x20);
    if (v >= 0) {
        v = v >> 8;
    } else {
        v = (v + 0xff) >> 8;
    }
    vec2[0] = v;

    v = vec2[1] * *(s32 *)(self + 0x24);
    if (v >= 0) {
        v = v >> 8;
    } else {
        v = (v + 0xff) >> 8;
    }
    vec2[1] = v;
}

/* Two-line text draw / position notify (docs/rom_map.md: "a two-line
 * text draw, same family as the icon-renderer shapes"): for each axis,
 * forwards `delta - self`'s own position through `self+0x30`'s
 * trampoline table (action = the position delta itself, per the same
 * `sub_803AD80`-style convention `sub_8024D74`/`sub_8024DAC` wire up),
 * then accumulates both trampoline results back into `self`'s own
 * position. */
void sub_8024E24(void *self0, void *delta0)
{
    u8 *self = (u8 *)self0;
    s32 *delta = (s32 *)delta0;
    u8 *mgr;
    s32 dx, dy;

    mgr = *(u8 **)(self + 0x30);
    dx = sub_803AD80(self + *(s16 *)(mgr + 0x20),
                      (void *)(delta[0] - *(s32 *)self),
                      *(void **)(mgr + 0x24));

    mgr = *(u8 **)(self + 0x30);
    dy = sub_803AD80(self + *(s16 *)(mgr + 0x20),
                      (void *)(delta[1] - *(s32 *)(self + 4)),
                      *(void **)(mgr + 0x24));

    *(s32 *)self += dx;
    *(s32 *)(self + 4) += dy;
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

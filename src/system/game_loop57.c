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
 * The streamer functions use `struct bg_streamer` below; the rest still
 * take raw pointers. Built with old_agbcc - see
 * docs/matching/game-loop-old-agbcc.md. */

/* The room descriptor the streamer reads its tile map from. */
struct stream_source
{
    u16 *map;                   // 0x00 - width x height tile ids
    u8 unk_04[0x12];
    u16 width;                  // 0x16 - in tiles
    u16 height;                 // 0x18
};

/* The ring-buffer background streamer (see this file's header comment). */
struct bg_streamer
{
    struct stream_source *source; // 0x00
    u16 *records;               // 0x04 - decoder record table
    u8 *ring;                   // 0x08 - 0x1000-byte 4x4-block ring buffer
    s32 tileX;                  // 0x0C
    s32 tileY;                  // 0x10
    u8 subX;                    // 0x14 - mod-4 block of the left edge
    u8 subY;                    // 0x15 - mod-4 block of the top edge
};

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
 * NAKED: plain C under old_agbcc is 77 halfwords off. The ROM reloads
 * `&gUnknown_03001300` from the literal pool at each of the three OAM
 * flushes, leaving r4 free for `self`; old_agbcc CSEs the address into
 * r4 instead (its loop pass declines the hoist), shifting every other
 * value one register. */
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
 * NAKED: plain C under old_agbcc is 13 halfwords off. Registers and
 * control flow match; the ROM computes the accumulator's sign extension
 * between the two shifts that sign-extend the delta byte (3 places), and
 * builds the copy loop's cell index in the other order. */
NAKED void sub_8024960(struct bg_streamer *self, s32 recordId, void *dest)
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

extern void sub_8024C08(struct bg_streamer *self, s32 col);
extern void sub_8024BAC(struct bg_streamer *self, s32 row);

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
        sub_8024C08((struct bg_streamer *)self, oldX + 4);
        self[0x14] = (self[0x14] + 1) & 3;
    } else if (tileX < oldX) {
        self[0x14] = (self[0x14] - 1) & 3;
        sub_8024C08((struct bg_streamer *)self, oldX - 1);
    }
    *(s32 *)(self + 0xc) = tileX;

    if (tileY > oldY) {
        sub_8024BAC((struct bg_streamer *)self, oldY + 4);
        self[0x15] = (self[0x15] + 1) & 3;
    } else if (tileY < oldY) {
        self[0x15] = (self[0x15] - 1) & 3;
        sub_8024BAC((struct bg_streamer *)self, oldY - 1);
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
    u8 subX;
    u8 subY;
    s32 shifted;
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
    u8 subX;
    u8 subY;
    s32 shifted;
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
    u8 subX;
    u8 subY;
    s32 shifted;
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

/* Streams in the newly exposed tile row `row`: decodes each of its up to
 * 4 in-bounds tiles into the row's ring-buffer blocks. */
void sub_8024BAC(struct bg_streamer *self, s32 row)
{
    s32 idx;
    s32 i;

    if (row < self->source->height)
    {
        idx = self->source->width;
        idx *= row;
        idx += self->tileX;
        for (i = 0; i <= 3; i++)
        {
            if (i + self->tileX < self->source->width)
            {
                u8 *dest = self->ring;
                dest += ((self->subY + 4) & 3) << 10;
                dest += ((self->subX + i + 4) & 3) << 5;
                sub_8024960(self, self->source->map[idx++], dest);
            }
        }
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* Streams in the newly exposed tile column `col`: decodes each of its up
 * to 4 in-bounds tiles into the column's ring-buffer blocks. */
void sub_8024C08(struct bg_streamer *self, s32 col)
{
    s32 idx;
    s32 i;

    if (col < self->source->width)
    {
        idx = self->tileY * self->source->width;
        idx += col;
        for (i = 0; i <= 3; i++)
        {
            struct stream_source *src;

            if (i + self->tileY < (src = self->source)->height)
            {
                u8 *dest = self->ring;
                u16 id;

                dest += ((self->subY + i + 4) & 3) << 10;
                dest += ((self->subX + 4) & 3) << 5;
                id = src->map[idx];
                idx += src->width;
                sub_8024960(self, id, dest);
            }
        }
    }
}

/* Seeds the whole ring buffer for a fresh camera position `pos` (Q8
 * world x/y): resets the sub-block origin, derives the top-left tile
 * (128x64 px tiles) and decodes all 4x4 in-bounds tiles. The row stride
 * and block height stay in registers across the loops, as in the ROM. */
void sub_8024C64(struct bg_streamer *self, s32 *pos)
{
    s32 rowLen = 0x40;
    s32 blockH = 0x20;
    s32 j;
    s32 i;

    self->subX = 0;
    self->subY = 0;
    self->tileX = pos[0] >> 7;
    self->tileY = pos[1] >> 6;
    for (j = 0; j <= 3; j++)
    {
        if (j + self->tileY < self->source->height)
        {
            s32 rowBase;

            for (i = 0, rowBase = rowLen * (j << 3); i <= 3; i++)
            {
                s32 x = i + self->tileX;
                struct stream_source *src;

                if (x < (src = self->source)->width)
                {
                    u16 id = src->map[(self->tileY + j) * src->width + x];
                    u16 *ring = (u16 *)self->ring;

                    sub_8024960(self, id, &ring[rowBase + (blockH >> 1) * i]);
                }
            }
        }
    }
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

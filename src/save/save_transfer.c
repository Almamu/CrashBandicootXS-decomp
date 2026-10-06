#include "core.h"
#include "match.h"
#include "link.h"
#include "save.h"

void SetSaveFlags(struct save_data *self, u8 flags)
{
    MATCH_HOLD_REG(u8, loaded, r3);
    MATCH_HOLD_REG(u8, v, r1);

    loaded = self->flags;
    v = loaded | flags;
    self->flags = v;
    UpdateSaveChecksum(self);
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead - force zero
 * padding to match (see docs/matching.md's alignment-padding gotcha /
 * the matching_decomp_alignment_fix convention). */
asm(".align 2, 0");

/* Drains up to 0x60 bytes per call from `self->cursor` (streaming a
 * save_data out of `self->tmpl`) into the SIO session's
 * outgoing ring, once the previous batch has been taken (`ring.count`
 * back to 0). Marks `sendDone` once `remaining` is fully drained. The
 * channel pointer has to be its own local: written as `s->ring.`
 * throughout, gcc keeps the first `&count` computation alive for both
 * fill loops instead of recomputing it as the ROM does. */
void SendSaveTransferChunk(struct settings_sync_pump *self)
{
    if (self->remaining != 0) {
        struct link_session *s = gLinkSession;
        struct link_ring *ch = &s->ring;

        if (ch->count == 0) {
            s32 n = self->remaining;
            u8 *src;
            s32 i;

            if (n > 0x60)
                n = 0x60;
            src = self->cursor;
            if (ch->writePos < 0x80 - n) {
                for (i = n - 1; i != -1; i--) {
                    ch->writePos++;
                    ch->count++;
                    ch->buf[ch->writePos] = *src++;
                }
            } else {
                for (i = n - 1; i != -1; i--) {
                    u8 b = *src++;

                    ch->writePos = ch->writePos == 0x7f ? 0 : ch->writePos + 1;
                    ch->count++;
                    ch->buf[ch->writePos] = b;
                }
            }
            self->cursor += n;
            self->remaining -= n;
        }
    } else if (gLinkSession->ring.count == 0) {
        self->sendDone = 1;
    }
}

/* Counterpart to SendSaveTransferChunk above: drains whatever's available from
 * `playerIndex`'s incoming channel (`gLinkSession->players[playerIndex].ring`)
 * into `self->data` via `self->writePtr`, and marks `receiveDone` once
 * `totalReceived` reaches a full record's worth.
 *
 * Matched in the last-eight pass (docs/matching/archive/last-eight-naked-retry.md).
 * The ROM computes `playerIndex * 0xc8 + s` twice, the second time
 * multiplying straight into the 0xc8 register (`muls r2, r1`), hence
 * the pinned `c`. The channel pointer comes out of an asm with a plain
 * `"r"` input, so it has no copy preference for r2; with the old
 * `"+r"` escape the ring pointer inherited that preference and pushed
 * the wrap loop's `old` out of r2. The wrap loop's count pointer is
 * pinned to r1 (the ROM's register), and the loop is an explicit
 * `if` + `do`/`while` so the pin is set after the zero-trip test. */
void ReceiveSaveTransferChunk(struct settings_sync_pump *self, s32 playerIndex)
{
    struct link_session *s = gLinkSession;
    s32 pi = playerIndex;
    /* One 0xc8 register for both products: the second multiplies
     * straight into it (`muls r2, r1`), and it then becomes the channel
     * pointer. */
    MATCH_HOLD_REG(s32, c, r2) = 0xc8;
    s32 n;

    /* players[pi].ring.count, with `players[pi]` as `s` moved on by
     * pi * 0xc8 (the product lands in `c`). */
    n = ((struct link_session *)(pi * c + (s32)s))->players[0].ring.count;
    if (n != 0) {
        u8 *dst;
        struct link_ring *ch;
        s32 *rd;
        s32 i;

        {
            u8 **wp = &self->writePtr;

            c = c * pi + (s32)s;
            /* &players[pi].ring (0xd0 + 0x38). No code: keeps the +0x108
             * out of the field offsets, and the
             * plain "r" input gives `ch` no copy preference for r2. */
            asm volatile("" : "=r"(ch) : "r"(c + 0x108));
            dst = *wp;
        }
        rd = &ch->readPos;
        if (*rd < 0x80 - n) {
            for (i = n - 1; i != -1; i--) {
                *dst++ = ch->buf[ch->readPos];
                ch->readPos++;
                ch->count--;
            }
        } else {
            i = n - 1;
            if (i != -1) {
                /* The ROM keeps the count pointer in r1, which leaves r2
                 * for `old`. */
                MATCH_HOLD_REG(s32 *, cnt, r1) = &ch->count;

                do {
                    s32 old = *rd;
                    s32 nw = 0;

                    if (old != 0x7f)
                        nw = old + 1;
                    *rd = nw;
                    (*cnt)--;
                    *dst++ = ch->buf[old];
                } while (--i != -1);
            }
        }
        self->writePtr += n;
        self->totalReceived += n;
    } else if (self->totalReceived == 0x200) {
        self->receiveDone = 1;
    }
}

#include "save_data.hpp"
#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "link.h"
#include "save.h"
#include "math_util.h"
}

/* Drains up to 0x60 bytes per call from `self->cursor` (streaming a
 * save_data out of `self->tmpl`) into the SIO session's
 * outgoing ring (LinkRing::Push), once the previous batch has been
 * taken (`ring.count` back to 0). Marks `sendDone` once `remaining` is
 * fully drained. */
void SaveTransfer::SendChunk()
{
    if (remaining != 0) {
        LinkSession *s = gLinkSession;

        if (s->ring.count == 0) {
            s32 n = remaining;

            LIMIT_MAX(n, 0x60);
            s->ring.Push(cursor, n);
            cursor += n;
            remaining -= n;
        }
    } else if (gLinkSession->ring.count == 0) {
        sendDone = 1;
    }
}

/* Counterpart to SaveTransfer::SendChunk above: drains whatever's available from
 * `playerIndex`'s incoming channel (`gLinkSession->players[playerIndex].ring`)
 * into `data` via `writePtr`, and marks `receiveDone` once
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
 * `if` + `do`/`while` so the pin is set after the zero-trip test.
 * #662 round 2: written as `s->players[playerIndex].ring.count` and an
 * inline LinkRing pop (SendChunk's LinkRing::Push in reverse, with the
 * wrap loop's `next = 0; if (old != 0x7f) ...`), both loops come out as
 * the ROM's, but gcc computes `playerIndex * 0xc8 + s` once and reuses
 * it for the ring pointer, where the ROM multiplies twice.
 * #662 round 3: it is cse1 that merges the two products (the -da dumps
 * have both `mult`s up to the cse pass), in every spelling tried: the
 * ring or player pointer taken first, a `LinkPlayer *`/reference local,
 * `players + playerIndex`, a byte offset, a count pointer, an early
 * `return` or the empty case first, with -fno-cse-follow-jumps/
 * -fno-cse-skip-blocks/-fno-gcse and the other per-object flags, and
 * under agbcp. The ROM's second `muls r2, r1` into the 0xc8 register
 * says cse never saw the first product with the second one, and its
 * reuse of r2 is reload_cse's; the natural code also builds the
 * addresses as `(i * 0xc8 + 0xd0) + s` (g++'s pointer arithmetic for
 * `&players[i]`) where the ROM adds `s` first, as the cast through a
 * moved `LinkSession *` here does. */
void SaveTransfer::ReceiveChunk(s32 playerIndex)
{
    LinkSession *s = gLinkSession;
    s32 pi = playerIndex;
    /* One 0xc8 register for both products: the second multiplies
     * straight into it (`muls r2, r1`), and it then becomes the channel
     * pointer. */
    MATCH_HOLD_REG(s32, c, r2) = 0xc8;
    s32 n;

    /* players[pi].ring.count, with `players[pi]` as `s` moved on by
     * pi * 0xc8 (the product lands in `c`). */
    n = ((LinkSession *)(pi * c + (s32)s))->players[0].ring.count;
    if (n != 0) {
        u8 *dst;
        LinkRing *ch;
        s32 *rd;
        s32 i;

        {
            u8 **wp = &writePtr;

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
        writePtr += n;
        totalReceived += n;
    } else if (totalReceived == 0x200) {
        receiveDone = 1;
    }
}

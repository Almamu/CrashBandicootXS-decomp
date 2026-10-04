#include "core.h"
#include "settings_sync.h"

extern void UpdateSaveChecksum(void *arg0);

void SetSaveFlags(struct settings_sync_record *self, u8 flags)
{
    register u8 loaded asm("r3");
    register u8 v asm("r1");

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

/* One direction of the SIO session's byte transport (0xc8 bytes): a
 * 0x80-byte ring plus its pending count and read/write positions. */
struct sio_channel
{
    u32 unk_00;
    u8 ring[0x80];      /* 0x04 */
    s32 count;          /* 0x84 */
    s32 readPos;        /* 0x88 */
    s32 writePos;       /* 0x8c */
    u8 unk_90[0x38];
};

/* The SIO session object gLinkSession points at: the outgoing
 * channel at +0x40 and one incoming channel per player from +0x108. */
struct sio_session
{
    u8 unk_00[0x40];
    struct sio_channel tx;      /* 0x040 */
    struct sio_channel rx[4];   /* 0x108 */
};

extern struct sio_session *gLinkSession;

/* Drains up to 0x60 bytes per call from `self->cursor` (streaming a
 * settings_sync_record out of `self->tmpl`) into the SIO session's
 * outgoing ring, once the previous batch has been taken (`tx.count`
 * back to 0). Marks `field_214` once `remaining` is fully drained. The
 * channel pointer has to be its own local: written as `s->tx.`
 * throughout, gcc keeps the first `&count` computation alive for both
 * fill loops instead of recomputing it as the ROM does. */
void sub_8002D44(struct settings_sync_pump *self)
{
    if (self->remaining != 0)
    {
        struct sio_session *s = gLinkSession;
        struct sio_channel *ch = &s->tx;

        if (ch->count == 0)
        {
            s32 n = self->remaining;
            u8 *src;
            s32 i;

            if (n > 0x60)
                n = 0x60;
            src = self->cursor;
            if (ch->writePos < 0x80 - n)
            {
                for (i = n - 1; i != -1; i--)
                {
                    ch->writePos++;
                    ch->count++;
                    ch->ring[ch->writePos] = *src++;
                }
            }
            else
            {
                for (i = n - 1; i != -1; i--)
                {
                    u8 b = *src++;

                    ch->writePos = ch->writePos == 0x7f ? 0 : ch->writePos + 1;
                    ch->count++;
                    ch->ring[ch->writePos] = b;
                }
            }
            self->cursor += n;
            self->remaining -= n;
        }
    }
    else if (gLinkSession->tx.count == 0)
    {
        self->field_214 = 1;
    }
}

/* Counterpart to sub_8002D44 above: drains whatever's available from
 * `playerIndex`'s incoming channel (`gLinkSession->rx[playerIndex]`)
 * into `self->data` via `self->writePtr`, and marks `field_218` once
 * `totalReceived` reaches a full record's worth.
 *
 * Matched in the last-eight pass (docs/matching/last-eight-naked-retry.md).
 * The ROM computes `playerIndex * 0xc8 + s` twice, the second time
 * multiplying straight into the 0xc8 register (`muls r2, r1`), hence
 * the pinned `c`. The channel pointer comes out of an asm with a plain
 * `"r"` input, so it has no copy preference for r2; with the old
 * `"+r"` escape the ring pointer inherited that preference and pushed
 * the wrap loop's `old` out of r2. The wrap loop's count pointer is
 * pinned to r1 (the ROM's register), and the loop is an explicit
 * `if` + `do`/`while` so the pin is set after the zero-trip test. */
void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex)
{
    struct sio_session *s = gLinkSession;
    s32 pi = playerIndex;
    /* One 0xc8 register for both products: the second multiplies
     * straight into it (`muls r2, r1`), and it then becomes the channel
     * pointer. */
    register s32 c asm("r2") = 0xc8;
    s32 n;

    n = *(s32 *)((u8 *)(pi * c + (s32)s) + 0x18c);
    if (n != 0)
    {
        u8 *dst;
        struct sio_channel *ch;
        s32 *rd;
        s32 i;

        {
            u8 **wp = &self->writePtr;

            c = c * pi + (s32)s;
            /* No code: keeps the +0x108 out of the field offsets, and the
             * plain "r" input gives `ch` no copy preference for r2. */
            asm volatile("" : "=r"(ch) : "r"(c + 0x108));
            dst = *wp;
        }
        rd = &ch->readPos;
        if (*rd < 0x80 - n)
        {
            for (i = n - 1; i != -1; i--)
            {
                *dst++ = ch->ring[ch->readPos];
                ch->readPos++;
                ch->count--;
            }
        }
        else
        {
            i = n - 1;
            if (i != -1)
            {
                /* The ROM keeps the count pointer in r1, which leaves r2
                 * for `old`. */
                register s32 *cnt asm("r1") = &ch->count;

                do
                {
                    s32 old = *rd;
                    s32 nw = 0;

                    if (old != 0x7f)
                        nw = old + 1;
                    *rd = nw;
                    (*cnt)--;
                    *dst++ = ch->ring[old];
                } while (--i != -1);
            }
        }
        self->writePtr += n;
        self->totalReceived += n;
    }
    else if (self->totalReceived == 0x200)
    {
        self->field_218 = 1;
    }
}

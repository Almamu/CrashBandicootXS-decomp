#include "core.h"
#include "match.h"
#include "link.h"

/* Link-session reset/init - see docs/rom_map.md's SIO/link-cable
 * section. Sets the link-active flag (`gLinkSessionReset`), resets a
 * handful of session-header fields, seeds a per-session 8-byte
 * handshake id via `MakeLinkHandshakeId` (self+0x30), mirrors that id into two
 * more session-header slots and every one of the 4 per-player 0xc8-byte
 * sub-records (self+playerIndex*0xc8), resets each per-player
 * sub-record's own RX-ring bookkeeping, writes the literal `0x1234`
 * link sync/ready magic into each player's data-exchange field
 * (self+playerIndex*0xc8+0xd6/0xd8), and finally programs
 * SIOMLT_SEND from the session header's own copy of the id.
 *
 * Built with old_agbcc plus `-fno-rerun-loop-opt` (this object is on
 * both OLD_AGBCC_OBJS and NO_RERUN_LOOP_OPT_OBJS in the Makefile; the
 * flag would change the matching HandleLinkSerial, hence the split). See
 * docs/matching/archive/last-ten-naked-retry.md and
 * docs/matching/archive/last-eleven-naked-retry.md. */
static inline void ring_reset(struct link_ring *r)
{
    r->count = 0;
    r->readPos = 0;
    r->writePos = 0x7f;
}

/* How the shape is reproduced:
 * - The single `-fno-rerun-loop-opt` loop pass keeps the inner copy loop
 *   counting up; the first loop still reverses because it is written
 *   over one pointer (`p[9]`/`p[8]` read, `p[0]`/`p[1]` written) with its
 *   own counter `k` (sharing `i` with the outer loop moved every
 *   register).
 * - `id` is a local passed to MakeLinkHandshakeId and copied into `src` inside
 *   the outer loop; loop motion moves that copy out, which is the ROM's
 *   `str r4, [sp, #4]`.
 * - field_30 goes through a `f30` base plus an `s32 t` offset, so the
 *   base is hoisted and the add is `f30 + t`.
 * - The inner destination is `((struct link_player *)(self + 8))[i + 1]`,
 *   which gives the ROM's `i + 1` precompute and per-pass `self + 0xd0`.
 * - The nibble address is `self + t` with `t = i * 0xc8` in a local, so
 *   the add is built as (plus self t): the ROM's `adds r0, r5, r2`.
 *   `&self->players[i]` expands to (plus (mult i 200) self) instead.
 * - `magic` holds 0x1234 in a function-scope local. Its set is then
 *   outside the loop, so its pseudo lives across the whole loop, loses
 *   global allocation and is rematerialized at each use by reload
 *   (REG_EQUIV constant): `ldr r1, =0x1234` for the first store, and a
 *   second reload that reload_cse turns into `adds r2, r1, #0`. That
 *   extra reload also moves the reload-register rotation to the ROM's
 *   in the tail.
 * - The tail stores field_20 into field_400 and reads it back through a
 *   plain `u16 *` into `v`.
 * The asm statements emit no code:
 * - 13 references on `id` lift its global-alloc priority (15 refs over
 *   41 insns) just above `self`'s (34 over 151), so `id` gets r4 and
 *   `self` r5 as in the ROM.
 * - The MATCH_BARRIER() after the nibble decrement lengthens `self + i *
 *   0xc8`'s life by one insn; that breaks its priority tie with the
 *   nibble pointer, so the pointer gets r4 and the base ip, as in the
 *   ROM. */
s32 ResetLinkSessionState(struct link_session *self)
{
    s32 i, j;
    s32 k;
    u32 magic = 0x1234;
    u8 *id;

    self->field_6 = 0;
    self->field_8 = 0;
    self->field_7 = 0;
    gLinkSessionReset = 1;
    self->field_4 = 0;
    self->field_1c = -1;
    self->field_3fc = -1;
    ring_reset(&self->ring);
    id = self->id;
    MakeLinkHandshakeId(id);
    /* No code: 13 extra references on `id` (see above). */
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    MATCH_USE(id);
    {
        u8 *p = self->field_28;

        for (k = 0; k <= 3; k++) {
            u32 v = (p[9] << 8) | p[8];
            u32 lo = v & 0xff;

            p[0] = lo;
            p[1] = v >> 8;
            p += 2;
        }
    }
    self->field_c = 0;
    self->field_24 = 0;
    for (i = 0; i <= 3; i++) {
        ring_reset(&self->players[i].ring);
        {
            s32 *f30 = &self->players[0].field_30;
            s32 t = i * 0xc8;

            *(s32 *)((u8 *)f30 + t) = 0;
        }
        {
            u8 *src = id;

            for (j = 0; j <= 3; j++) {
                u32 w = (src[j * 2 + 1] << 8) | src[j * 2];
                u32 lo = w & 0xff;

                ((struct link_player *)((u8 *)self + 8))[i + 1].id[j * 2] = lo;
                ((struct link_player *)((u8 *)self + 8))[i + 1].id[j * 2 + 1] = w >> 8;
            }
        }
        {
            s32 t = i * 0xc8;

            /* players[i].id[1]'s low nibble */
            ((struct nibble_pair *)((u8 *)self + t + 0xd1))->lo--;
        }
        /* No code: one insn of padding (see above). */
        MATCH_BARRIER();
        self->players[i].field_34 = 0;
        self->players[i].field_2c = 0;
        self->players[i].field_8 = (self->players[i].field_6 = magic);
    }
    self->field_3f0 = 0;
    self->field_3f4 = 0;
    self->field_3f8 = 0;
    self->field_38 = 0;
    self->field_3c = 0;
    self->field_20.hi = 0xF0B;
    self->field_20.lo = 0;
    {
        u16 *p400 = &self->field_400;
        u16 v;

        *p400 = *(u16 *)&self->field_20;
        v = *p400;
        REG_SIOMLT_SEND = v;
    }
    return 0;
}

#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "link.h"
}

/* Link-session reset/init - see docs/rom_map.md's SIO/link-cable
 * section. Sets the link-active flag (`gLinkSessionReset`), resets a
 * handful of session-header fields, seeds a per-session 8-byte
 * handshake id via `MakeLinkHandshakeId` (`id`, self+0x30), mirrors that id into
 * `prevPacket` and every one of the 4 per-player 0xc8-byte
 * records (self+0xd0+playerIndex*0xc8), resets each per-player
 * sub-record's own RX-ring bookkeeping, writes the literal `0x1234`
 * link sync/ready magic into each player's data-exchange field
 * (self+playerIndex*0xc8+0xd6/0xd8), and finally programs
 * SIOMLT_SEND from the session header's own copy of the id.
 *
 * Built with old_agbcc plus `-fno-rerun-loop-opt` (this object is on
 * both OLD_AGBCC_OBJS and NO_RERUN_LOOP_OPT_OBJS in the Makefile; the
 * flag would change the matching LinkSession::HandleSerial, hence the split). See
 * docs/matching/archive/last-ten-naked-retry.md and
 * docs/matching/archive/last-eleven-naked-retry.md. */
/* How the shape is reproduced:
 * - The single `-fno-rerun-loop-opt` loop pass keeps the inner copy loop
 *   counting up; the first loop still reverses because it is written
 *   over one pointer (`p[9]`/`p[8]` read, `p[0]`/`p[1]` written) with its
 *   own counter `k` (sharing `i` with the outer loop moved every
 *   register).
 * - `id` is a local passed to MakeLinkHandshakeId and copied into `src` inside
 *   the outer loop; loop motion moves that copy out, which is the ROM's
 *   `str r4, [sp, #4]`.
 * - rxSeq goes through a `f30` base plus an `s32 t` offset, so the
 *   base is hoisted and the add is `f30 + t`.
 * - The inner destination is `((LinkPlayer *)(self + 8))[i + 1]`,
 *   which gives the ROM's `i + 1` precompute and per-pass `self + 0xd0`.
 * - The nibble address is `self + t` with `t = i * 0xc8` in a local, so
 *   the add is built as (plus self t): the ROM's `adds r0, r5, r2`.
 *   `&self->players[i]` expands to (plus (mult i 200) self) instead.
 *   The address goes into its own pointer `nb` before the decrement
 *   (#662 round 3): that pseudo gives the pointer r4 and the base ip, as
 *   in the ROM; decremented through the cast expression, the two tied in
 *   global-alloc priority and a MATCH_BARRIER() of insn padding broke
 *   the tie.
 * - `magic` holds 0x1234 in a function-scope local. Its set is then
 *   outside the loop, so its pseudo lives across the whole loop, loses
 *   global allocation and is rematerialized at each use by reload
 *   (REG_EQUIV constant): `ldr r1, =0x1234` for the first store, and a
 *   second reload that reload_cse turns into `adds r2, r1, #0`. That
 *   extra reload also moves the reload-register rotation to the ROM's
 *   in the tail.
 * - The tail stores handshakeWord into sendWord and reads it back through a
 *   plain `u16 *` into `v`.
 * The asm statements emit no code: 13 references on `id` lift its
 * global-alloc priority (15 refs over 41 insns) just above `self`'s (34
 * over 151), so `id` gets r4 and `self` r5 as in the ROM.
 * #662 round 3 diagnosis: global-alloc decides it. Unpinned, `id` has 3
 * (loop-weighted) refs over 29 insns against `self`'s 34 over 140, so
 * `self` is allocated first and takes r4, the inner counter r5 and `id`
 * r8; fewer than 13 uses still leave `self` ahead. No flag of the
 * family list (-fno-gcse ... -fno-regmove), agbcp instead of old_agbcp,
 * an `id` read as `this->id` at the call and the copy, `id` declared at
 * the top, or an inline CopyPacket (bytes or packed halfwords) for the
 * inner copy changes the order; the ROM's `id` is referenced only at the
 * call and the copy into the inner loop's spilled source, so nothing
 * natural raises its priority that far.
 * #662 round 4, the exact priorities (global.c's allocno_compare,
 * floor_log2(refs) * refs / live length; refs and lengths are weighted
 * by loop depth): `self` 5 * 34 / 140 = 1.21, `id` 1 * 3 / 29 = 0.10.
 * `id` lives from its set to the copy into `src` that loop.c hoists in
 * front of the outer loop, across the first loop, so its 29 can't
 * shrink; it would need 12 references (3 * 12 / 29 = 1.24) where the
 * code has 3 (the set, MakeLinkHandshakeId's argument, the hoisted
 * copy). The other way to the ROM's registers, `self` allocated first
 * but r4 already taken by something ranked above it that conflicts
 * with `self` and not with `id`, has no candidate: everything ranked
 * above `self` is a short-lived value that finds r0-r3 free, and what
 * the ROM has in r4 later (the inner counter, 3 * 11 / 44 = 0.75, and
 * the nibble pointer, 2 * 6 / 12 = 1.0) ranks below it; extra
 * references on the nibble pointer still leave `self` in r4. The
 * decomp-permuter on a C port (45 minutes) found only junk (a variable
 * holding the shift count 8). So the references stay.
 * #662 round 2: the copy loops are halfword copies through a packed
 * `struct { u16 v; }` (that reproduces their ldrb/orr/and/strb exactly,
 * the same as the explicit `lo = w & 0xff`), and written naturally that
 * way, with the player fields by name, `self` takes r4 and `id` is
 * recomputed. With the copies in an inline CopyPacket `id` gets r4 and
 * `self` r5 as in the ROM, but the first loop then walks `id` instead of
 * reading `prevPacket + 8`, and the rxSeq/nibble addresses differ; the
 * permuter (45 minutes) got its score from 3340 to 2110 only.
 * #662 round 5: the inner copy as an inline CopyPacket(dst, src) taking
 * `id` or `this->id` (with or without the `id` local, the call taking
 * `this->id`) is 338-350 lines off against 366 for the plain code without
 * the references; `self` still takes r4 in all of them.
 * #662 round 7: a private old_agbcp whose allocno_compare ranks by refs /
 * length, (log2 + 1) * refs / length, log2 * refs or refs * refs /
 * length leaves the plain code 78, 146, 220 and 162 lines off (146 with
 * the stock formula); the same variants are further off for
 * HandleSerial's `n` and GAX2_init's two uses, so no other priority rule
 * explains these references.
 * #662 round 8 (tools/natural_enum.py): without the references, `i`,
 * `j`, `k` and `magic` through all six integer types (1296 variants)
 * stay 134 lines off, and a 12000-variant sample that also retypes the
 * copy loops' `v`/`w`/`lo` and the `t` offsets gets to 82 with `self`
 * still in r4.
 * #662 round 9, the header's types: every LinkSession, LinkPlayer and
 * LinkRing member and gLinkSessionReset in their same-size alternatives
 * (signedness, bool, volatile, the nibble and id-word bitfields' base
 * types), two at a time and then a beam search (2400 variants, scored
 * with Update and HandleSerial), without the references: the nearest
 * (LinkPlayer::hash as `s16`) is 122 lines off against 140. */
s32 LinkSession::ResetState()
{
    s32 i, j;
    s32 k;
    u32 magic = 0x1234;
    u8 *id;

    sioConfigured = 0;
    started = 0;
    connected = 0;
    gLinkSessionReset = 1;
    inSerialIrq = 0;
    playerCount = -1;
    playerId = -1;
    ring.Reset();
    id = this->id;
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
        u8 *p = prevPacket;

        for (k = 0; k <= 3; k++) {
            u32 v = (p[9] << 8) | p[8];
            u32 lo = v & 0xff;

            p[0] = lo;
            p[1] = v >> 8;
            p += 2;
        }
    }
    connectCounter = 0;
    totalSent = 0;
    for (i = 0; i <= 3; i++) {
        players[i].ring.Reset();
        {
            s32 *f30 = &players[0].rxSeq;
            s32 t = i * sizeof(LinkPlayer);

            *(s32 *)((u8 *)f30 + t) = 0;
        }
        {
            u8 *src = id;

            for (j = 0; j <= 3; j++) {
                u32 w = (src[j * 2 + 1] << 8) | src[j * 2];
                u32 lo = w & 0xff;

                ((LinkPlayer *)((u8 *)this + 8))[i + 1].id[j * 2] = lo;
                ((LinkPlayer *)((u8 *)this + 8))[i + 1].id[j * 2 + 1] = w >> 8;
            }
        }
        {
            s32 t = i * sizeof(LinkPlayer);
            struct nibble_pair *nb = (struct nibble_pair *)((u8 *)this + t + 0xd1);

            /* players[i].id[1]'s low nibble */
            nb->lo--;
        }
        players[i].totalReceived = 0;
        players[i].rxCount = 0;
        players[i].prevHash = (players[i].hash = magic);
    }
    ackedMask = 0;
    receivedMask = 0;
    peerMask = 0;
    sendWordIndex = 0;
    sendRound = 0;
    handshakeWord.hi = 0xF0B;
    handshakeWord.lo = 0;
    {
        u16 *p400 = &sendWord;
        u16 v;

        *p400 = *(u16 *)&handshakeWord;
        v = *p400;
        REG_SIOMLT_SEND = v;
    }
    return 0;
}

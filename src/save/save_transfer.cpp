#include "save_data.hpp"
#include "link_session.hpp"

extern "C" {
#include "core.h"
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
 * into `data` via `writePtr` (LinkRing::Pop), and marks `receiveDone` once
 * `totalReceived` reaches a full record's worth.
 *
 * Matched in the last-eight pass (docs/matching/archive/last-eight-naked-retry.md)
 * with a pinned 0xc8 register and an empty asm; #662 round 5 found the
 * source. The ROM computes `playerIndex * 0xc8` twice, sharing only the
 * constant: the count is read through a byte offset built in two
 * statements, `offset = playerIndex * size; offset = offset + base;` (the
 * shape of SaveData::ReadSlot and WriteSlot). The second statement
 * overwrites the product's register, so cse1 has nothing left to reuse
 * when `&s->players[playerIndex].ring` multiplies again; written as one
 * statement, or as `s->players[playerIndex].ring.count`, the product is
 * shared. The pop is LinkRing's inline Pop, the reverse of the Push that
 * SendChunk uses: as a method its `this` keeps the ring pointer apart
 * from the field offsets. */
void SaveTransfer::ReceiveChunk(s32 playerIndex)
{
    LinkSession *s = gLinkSession;
    s32 offset;
    s32 n;

    offset = playerIndex * sizeof(LinkPlayer);
    offset = offset + (s32)s;
    n = ((LinkSession *)offset)->players[0].ring.count;
    if (n != 0) {
        s->players[playerIndex].ring.Pop(writePtr, n);
        writePtr += n;
        totalReceived += n;
    } else if (totalReceived == 0x200) {
        receiveDone = 1;
    }
}

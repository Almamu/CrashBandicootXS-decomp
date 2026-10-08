extern "C" {
#include "core.h"
#include "link.h"
#include "save.h"
}

/* Polls the SIO-handshake spinner's transfer state once per frame: if
 * the session (*gLinkSession, byte +7 = "connected") isn't
 * connected, just tracks completion/reset of `self` and returns
 * 1 (reset)/0 (still finishing). If connected, picks a player slot from
 * the session's `playerId`, pumps RX (ReceiveSaveTransferChunk) and TX
 * (SendSaveTransferChunk) at most once each per call, and once both sides report
 * complete, waits ~0x1e extra polls before finally returning 0
 * ("settled"). Returns 2 if `playerId` is neither 0 nor
 * 1 (unrecognised role).
 *
 * Was an instruction-for-instruction asm transcription (the r7 note of
 * docs/matching/archive/issue-5-overlay-ui-sync.md); plain C++ since
 * the C++ conversion. The ROM reads the session's `playerId` twice,
 * which is what `volatile` on the field gives (the serial IRQ writes it,
 * link_session.h). */
s32 PollSaveTransfer(struct save_transfer *self)
{
    struct link_session *session = gLinkSession;
    s32 slot;
    s32 settled;

    if (!session->connected) {
        if (self->receiveDone != 0 && self->sendDone != 0)
            return 0;
        self->remaining = sizeof(self->data);
        self->totalReceived = 0;
        self->cursor = (u8 *)self->tmpl;
        self->writePtr = self->data;
        self->sendDone = 0;
        self->receiveDone = 0;
        self->settleTimer = 0;
        return 1;
    }
    if (session->playerId == 0)
        slot = 1;
    else if (session->playerId == 1)
        slot = 0;
    else
        return 2;
    if (self->receiveDone == 0)
        ReceiveSaveTransferChunk(self, slot);
    if (self->sendDone == 0)
        SendSaveTransferChunk(self);
    settled = 0;
    if (self->receiveDone != 0 && self->sendDone != 0 && (s32)self->settleTimer++ > 0x1e)
        settled = 1;
    if (settled)
        return 0;
    return 1;
}

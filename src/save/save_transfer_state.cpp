/* The save transfer's state: Poll, then SetRecord, GetData and Reset (those
 * three moved from save_menu_input.cpp, #767; this file was
 * save_transfer_poll.cpp; both default flags). */

#include "save_data.hpp"
#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "link.h"
#include "save.h"
}

/* Polls the SIO-handshake spinner's transfer state once per frame: if
 * the session (*gLinkSession, byte +7 = "connected") isn't
 * connected, just tracks completion/reset of `self` and returns
 * 1 (reset)/0 (still finishing). If connected, picks a player slot from
 * the session's `playerId`, pumps RX (SaveTransfer::ReceiveChunk) and TX
 * (SaveTransfer::SendChunk) at most once each per call, and once both sides report
 * complete, waits ~0x1e extra polls before finally returning 0
 * ("settled"). Returns 2 if `playerId` is neither 0 nor
 * 1 (unrecognised role).
 *
 * Was an instruction-for-instruction asm transcription (the r7 note of
 * docs/matching/archive/issue-5-overlay-ui-sync.md); plain C++ since
 * the C++ conversion. The ROM reads the session's `playerId` twice,
 * which is what `volatile` on the field gives (the serial IRQ writes it,
 * link_session.hpp). */
s32 SaveTransfer::Poll()
{
    LinkSession *session = gLinkSession;
    s32 slot;
    s32 settled;

    if (!session->connected) {
        if (receiveDone != 0 && sendDone != 0)
            return 0;
        remaining = sizeof(data);
        totalReceived = 0;
        cursor = (u8 *)tmpl;
        writePtr = data;
        sendDone = 0;
        receiveDone = 0;
        settleTimer = 0;
        return 1;
    }
    if (session->playerId == 0)
        slot = 1;
    else if (session->playerId == 1)
        slot = 0;
    else
        return 2;
    if (receiveDone == 0)
        ReceiveChunk(slot);
    if (sendDone == 0)
        SendChunk();
    settled = 0;
    if (receiveDone != 0 && sendDone != 0 && (s32)settleTimer++ > 0x1e)
        settled = 1;
    if (settled)
        return 0;
    return 1;
}

void SaveTransfer::SetRecord(SaveData *record)
{
    tmpl = record;
    cursor = (u8 *)record;
}

void *SaveTransfer::GetData()
{
    return data;
}

void SaveTransfer::Reset()
{
    remaining = sizeof(data);
    totalReceived = 0;
    cursor = (u8 *)tmpl;
    writePtr = data;
    sendDone = 0;
    receiveDone = 0;
    settleTimer = 0;
}

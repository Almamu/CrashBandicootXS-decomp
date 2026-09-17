#include "core.h"
#include "settings_sync.h"

extern void sub_8002B70(void *arg0);

void sub_8002D28(struct settings_sync_record *self, u8 flags)
{
    register u8 loaded asm("r3");
    register u8 v asm("r1");

    loaded = self->flags;
    v = loaded | flags;
    self->flags = v;
    sub_8002B70(self);
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead - force zero
 * padding to match (see docs/matching.md's alignment-padding gotcha /
 * the matching_decomp_alignment_fix convention). */
asm(".align 2, 0");

extern void *gUnknown_03000804;

#if NON_MATCHING
/* The three functions below (sub_8002D44/sub_8002E20/sub_8002EFC) are
 * reconstructed (semantics fully understood - see docs/matching/
 * issue-5-overlay-ui-sync.md for the whole SIO send/receive-pump
 * protocol write-up) but NOT YET BYTE-MATCHING: every technique this
 * project documents was tried (down-counting `for`/`do-while` loops
 * matching the ROM's `n != -1` sentinel idiom, swapping wrap/non-wrap
 * branch order to match the ROM's fallthrough side, explicit
 * register-variable pins on `self`/`remaining`/`session`/a `p` alias
 * for the final field-store pair - landed sub_8002D44 at the ROM's
 * exact byte *size* but not byte-for-byte content) - the remaining gap
 * is pure gcc-2.9 scratch-register nondeterminism, the same
 * unresolved class sub_8006600 (src/graphics/oam_count.c) and
 * sub_80049CC (src/graphics/settings_menu.c) document at length. Real
 * bytes stay in asm/code_3_1_10_3_2d44.s, wrapped `.if NON_MATCHING ==
 * 0`. */

/* Drains up to 0x60 bytes per call from `self->cursor` (streaming a
 * settings_sync_record out of `self->tmpl`) into the SIO session
 * object's outgoing ring buffer, tracked by that session's own
 * still-uncharacterized fields at +0xc4 (pending-byte count) and +0xcc
 * (ring write position, wrapping a 0x80-byte ring at +0x44). Marks
 * `field_214` once `remaining` is fully drained. */
void sub_8002D44(struct settings_sync_pump *self)
{
    u8 *session;
    u32 remaining = self->remaining;

    if (remaining == 0) {
        session = gUnknown_03000804;
        if (*(u32 *)(session + 0xc4) != 0) {
            return;
        }
        self->field_214 = 1;
        return;
    }

    session = gUnknown_03000804;
    if (*(u32 *)(session + 0xc4) != 0) {
        return;
    }

    {
        s32 sendLen = remaining;
        s32 *posPtr = (s32 *)(session + 0xcc);
        u32 *pendingPtr = (u32 *)(session + 0xc4);
        u8 *ringBase = session + 0x44;
        u8 *cursor = self->cursor;
        s32 n;

        if (sendLen > 0x60) {
            sendLen = 0x60;
        }

        if (*posPtr < (0x80 - sendLen)) {
            for (n = sendLen - 1; n != -1; n--) {
                *posPtr += 1;
                *pendingPtr += 1;
                ringBase[*posPtr] = *cursor;
                cursor++;
            }
        } else {
            for (n = sendLen - 1; n != -1; n--) {
                u8 byte = *cursor++;
                s32 pos = *posPtr;
                s32 newPos;
                if (pos == 0x7f) {
                    newPos = 0;
                } else {
                    newPos = pos + 1;
                }
                *posPtr = newPos;
                *pendingPtr += 1;
                ringBase[*posPtr] = byte;
            }
        }

        self->cursor = self->cursor + sendLen;
        self->remaining = self->remaining - sendLen;
    }
}

/* Counterpart to sub_8002D44 above: drains whatever's available from
 * `playerIndex`'s per-player incoming ring buffer (session base +
 * playerIndex*0xc8, a still-uncharacterized per-player sub-record) into
 * `self->data` via `self->writePtr`. Marks `field_218` once
 * `totalReceived` reaches a full record's worth. */
void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex)
{
    u8 *session = gUnknown_03000804;
    u8 *playerBase = session + playerIndex * 0xc8;
    u32 availCount = *(u32 *)(playerBase + 0x18c);

    if (availCount == 0) {
        if (self->totalReceived == sizeof(self->data)) {
            self->field_218 = 1;
        }
        return;
    }

    {
        u8 *writePtr = self->writePtr;
        u8 *dataBase = playerBase + 0x10c;
        s32 *posPtr = (s32 *)(playerBase + 0x190);
        u32 *availPtr = (u32 *)(playerBase + 0x18c);
        s32 n;

        if (*posPtr < (0x80 - (s32)availCount)) {
            for (n = (s32)availCount - 1; n != -1; n--) {
                *writePtr++ = dataBase[*posPtr];
                *posPtr = *posPtr + 1;
                *availPtr -= 1;
            }
        } else {
            for (n = (s32)availCount - 1; n != -1; n--) {
                s32 pos = *posPtr;
                s32 newPos;
                if (pos == 0x7f) {
                    newPos = 0;
                } else {
                    newPos = pos + 1;
                }
                *posPtr = newPos;
                *availPtr -= 1;
                *writePtr++ = dataBase[pos];
            }
        }
    }

    self->writePtr = self->writePtr + availCount;
    self->totalReceived = self->totalReceived + availCount;
}

/* Polls the SIO-handshake spinner's transfer state once per frame: if
 * the session (*gUnknown_03000804, byte +7 = "connected") isn't
 * connected, just tracks completion/reset of `self` and returns
 * 1 (reset)/0 (still finishing). If connected, picks a player slot from
 * the session's +0x3fc negotiation value, pumps RX (sub_8002E20) and TX
 * (sub_8002D44) at most once each per call, and once both sides report
 * complete, waits ~0x1e extra polls before finally returning 0
 * ("settled"). Returns 2 if the session's +0x3fc value is neither 0 nor
 * 1 (unrecognised role). */
s32 sub_8002EFC(struct settings_sync_pump *self)
{
    u8 *session = gUnknown_03000804;
    s32 mode;

    if (session[7] == 0) {
        if (self->field_218 == 0) {
            goto doReset;
        }
        if (self->field_214 != 0) {
            return 0;
        }
    doReset:
        self->remaining = sizeof(self->data);
        self->totalReceived = 0;
        self->cursor = (u8 *)self->tmpl;
        self->writePtr = self->data;
        self->field_214 = 0;
        self->field_218 = 0;
        self->field_21c = 0;
        return 1;
    }

    if (*(s32 *)(session + 0x3fc) == 0) {
        mode = 1;
    } else if (*(s32 *)(session + 0x3fc) == 1) {
        mode = 0;
    } else {
        return 2;
    }

    if (self->field_218 == 0) {
        sub_8002E20(self, mode);
    }
    if (self->field_214 == 0) {
        sub_8002D44(self);
    }
    if (self->field_218 != 0 && self->field_214 != 0) {
        s32 old = self->field_21c;
        self->field_21c = old + 1;
        if (old > 0x1e) {
            return 0;
        }
    }
    return 1;
}
#endif /* NON_MATCHING */

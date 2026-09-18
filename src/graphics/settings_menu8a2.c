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

/* sub_8002EFC (below) is the third of a trio with sub_8002D44/
 * sub_8002E20 - see docs/matching/issue-5-overlay-ui-sync.md for the
 * whole SIO send/receive-pump write-up. sub_8002D44/sub_8002E20
 * themselves stay parked: both need `r7` as a genuinely
 * register-allocated scratch (matching the ROM's own `sendLen`/
 * sentinel usage), and this exact agbcc build never includes r7 in a
 * function's automatic callee-save push/pop - confirmed by direct
 * reproduction (a function that only ever touches r7, whether via a
 * plain asm clobber, an explicit `register T x asm("r7")` pin used
 * across a real call, or a real C-level variable forced into r7 under
 * heavy register pressure, still comes back with no r7 in the push/pop
 * list every single time). This is the same categorical r7-pin
 * limitation already documented for `sub_8007DBC` (actor_part2.c),
 * `sub_802D3A8` (actor_part62.c) and others - see
 * matching_decomp_register_pinning memory point 10. sub_8002EFC itself
 * doesn't touch r7 at all, so it isn't affected and is matched via the
 * same instruction-for-instruction asm transcription technique. */

#if NON_MATCHING
/* Reconstructed (semantics fully understood - see docs/matching/
 * issue-5-overlay-ui-sync.md) but NOT YET BYTE-MATCHING: the r7
 * limitation described above. Real bytes stay in
 * asm/code_3_1_10_3_2d44.s, wrapped `.if NON_MATCHING == 0`.
 *
 * Drains up to 0x60 bytes per call from `self->cursor` (streaming a
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
#endif /* NON_MATCHING */

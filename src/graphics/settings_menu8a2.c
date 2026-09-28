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

/* The SIO session object gUnknown_03000804 points at: the outgoing
 * channel at +0x40 and one incoming channel per player from +0x108. */
struct sio_session
{
    u8 unk_00[0x40];
    struct sio_channel tx;      /* 0x040 */
    struct sio_channel rx[4];   /* 0x108 */
};

extern struct sio_session *gUnknown_03000804;

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
        struct sio_session *s = gUnknown_03000804;
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
    else if (gUnknown_03000804->tx.count == 0)
    {
        self->field_214 = 1;
    }
}

/* Counterpart to sub_8002D44 above: drains whatever's available from
 * `playerIndex`'s incoming channel (`gUnknown_03000804->rx[playerIndex]`)
 * into `self->data` via `self->writePtr`, and marks `field_218` once
 * `totalReceived` reaches a full record's worth.
 *
 * Still NAKED. The draft below keeps the ROM's `n - 1 != -1` loop
 * tests (the old note said C folds them to `n != 0`; it doesn't) but is
 * still about 100 halfwords off under both compilers: the ROM computes
 * `playerIndex * 0xc8 + s` twice (once for the count test, once for
 * the channel pointer, which then carries its own +0x108), and gcc
 * CSEs the second into the first and folds the field offsets. Per-file
 * CSE flags (-fno-cse-follow-jumps/-skip-blocks, -fno-rerun-cse-after-
 * loop) don't split it. The old "r7 can't be pushed" reason was wrong:
 * sub_8002D44 gets its r7/r8/sb prologue from plain C. */
#if NON_MATCHING
void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex)
{
    struct sio_session *s = gUnknown_03000804;
    s32 n = s->rx[playerIndex].count;

    if (n != 0)
    {
        u8 *dst = self->writePtr;
        struct sio_channel *ch = &s->rx[playerIndex];
        s32 i;

        if (ch->readPos < 0x80 - n)
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
            for (i = n - 1; i != -1; i--)
            {
                s32 p = ch->readPos;

                ch->readPos = p == 0x7f ? 0 : p + 1;
                ch->count--;
                *dst++ = ch->ring[p];
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
#else
NAKED void sub_8002E20(struct settings_sync_pump *self, s32 playerIndex)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "mov ip, r0\n\t"
        "ldr r0, 2f\n\t"
        "ldr r3, [r0]\n\t"
        "mov r2, #0xc8\n\t"
        "add r0, r1, #0\n\t"
        "mul r0, r2, r0\n\t"
        "add r0, r0, r3\n\t"
        "mov r4, #0xc6\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r0, r4\n\t"
        "ldr r6, [r0]\n\t"
        "cmp r6, #0\n\t"
        "beq 7f\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "mul r2, r1, r2\n\t"
        "add r2, r2, r3\n\t"
        "mov r1, #0x84\n\t"
        "lsl r1, r1, #1\n\t"
        "add r2, r2, r1\n\t"
        "ldr r4, [r0]\n\t"
        "add r5, r2, #0\n\t"
        "add r5, #0x88\n\t"
        "mov r0, #0x80\n\t"
        "sub r0, r0, r6\n\t"
        "ldr r1, [r5]\n\t"
        "cmp r1, r0\n\t"
        "bge 3f\n\t"
        "sub r3, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r3, r0\n\t"
        "beq 6f\n\t"
        "add r1, r5, #0\n\t"
        "add r5, r2, #4\n\t"
        "add r2, #0x84\n\t"
        "add r7, r0, #0\n\t"
    "1:\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r4]\n\t"
        "add r4, #1\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r2]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r2]\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r7\n\t"
        "bne 1b\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_03000804\n"
    "3:\n\t"
        "sub r3, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r3, r0\n\t"
        "beq 6f\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x84\n\t"
        "add r7, r2, #4\n\t"
        "mov r8, r0\n\t"
    "4:\n\t"
        "ldr r2, [r5]\n\t"
        "mov r0, #0\n\t"
        "cmp r2, #0x7f\n\t"
        "beq 5f\n\t"
        "add r0, r2, #1\n\t"
    "5:\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r1]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r1]\n\t"
        "add r0, r7, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r4]\n\t"
        "add r4, #1\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r8\n\t"
        "bne 4b\n\t"
    "6:\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, ip\n\t"
        "ldr r1, [r0]\n\t"
        "add r1, r1, r6\n\t"
        "str r1, [r0]\n\t"
        "mov r4, ip\n\t"
        "ldr r0, [r4, #4]\n\t"
        "add r0, r0, r6\n\t"
        "str r0, [r4, #4]\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "mov r0, ip\n\t"
        "ldr r1, [r0, #4]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #2\n\t"
        "cmp r1, r0\n\t"
        "bne 8f\n\t"
        "mov r1, #0x86\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, ip\n\t"
        "mov r0, #1\n\t"
        "str r0, [r1]\n\t"
    "8:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
#endif

#include "core.h"
#include "link_session.h"

extern void sub_8001CB8(u8 *self);
extern u8 gUnknown_03000800;

/* Link-session reset/init - see docs/rom_map.md's SIO/link-cable
 * section. Sets the link-active flag (`gUnknown_03000800`), resets a
 * handful of session-header fields, seeds a per-session 8-byte
 * handshake id via `sub_8001CB8` (self+0x30), mirrors that id into two
 * more session-header slots and every one of the 4 per-player 0xc8-byte
 * sub-records (self+playerIndex*0xc8), resets each per-player
 * sub-record's own RX-ring bookkeeping, writes the literal `0x1234`
 * link sync/ready magic into each player's data-exchange field
 * (self+playerIndex*0xc8+0xd6/0xd8), and finally programs
 * SIOMLT_SEND from the session header's own copy of the id.
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * is semantically confirmed against the ROM (the paragraph above is
 * that derivation), but the ROM keeps `self` in r5 and threads
 * `r8`/`sb`/`sl`/`ip` plus four cached pointers on the stack through
 * its 4-player loop - this project's usual gcc-2.9 scratch-register
 * nondeterminism (see `sub_8006600`/`sub_80049CC`, and `sub_8001CB8`
 * above for the same fight in a smaller function from this same file),
 * compounded here by the function's size (400 B, a 4-player nested
 * loop) making per-register archaeology impractical. Full NAKED
 * transcription instead, like `sub_8001CB8` above and this project's
 * other hard-compiler-limitation cases.
 *
 * Still NAKED. It lives in its own object because the C draft under
 * NON_MATCHING needs `-fno-rerun-loop-opt` (see NO_RERUN_LOOP_OPT_OBJS in
 * the Makefile), which changes the already-matching sub_8002114. With
 * old_agbcc and that flag the draft is 51 halfwords off, same size (it
 * was 136); see docs/matching/last-ten-naked-retry.md. */
#if NON_MATCHING
static inline void ring_reset(struct link_ring *r)
{
    r->field_84 = 0;
    r->field_88 = 0;
    r->field_8c = 0x7f;
}

/* 51 halfwords off under old_agbcc with -fno-rerun-loop-opt (same size;
 * the instruction shapes match except for three spots). Left:
 * - `self + i * 0xc8` for the nibble/magic block is built as
 *   `(i * 0xc8) + self` (`adds r0, r2, r5`); the ROM has `self` first.
 * - The ROM reloads 0x1234 separately for the second store
 *   (`adds r2, r1, #0`), so its constant had no register; here one
 *   pseudo keeps it in r1.
 * - The tail's reload registers (r1/r2/r3 for r3/r7/r2) follow from the
 *   two above.
 * What got it from 136 to 51:
 * - The single `-fno-rerun-loop-opt` pass keeps the inner copy loop
 *   counting up; the first loop still reverses because it is written
 *   over one pointer (`p[9]`/`p[8]` read, `p[0]`/`p[1]` written) with its
 *   own counter `k` (sharing `i` with the outer loop moved every
 *   register).
 * - `id` is a local passed to sub_8001CB8 and copied into `src` inside
 *   the outer loop; loop motion moves that copy out, which is the ROM's
 *   `str r4, [sp, #4]`.
 * - field_30 goes through a `f30` base plus an `s32 t` offset, so the
 *   base is hoisted and the add is `f30 + t`.
 * - The inner destination is `((struct link_player *)(self + 8))[i + 1]`,
 *   which gives the ROM's `i + 1` precompute and per-pass `self + 0xd0`.
 * - The tail re-reads field_400 through a `vu16` read of a pointer.
 * The asm statements emit no code:
 * - 13 references on `id` lift its global-alloc priority (15 refs over
 *   41 insns) just above `self`'s (34 over 151), so `id` gets r4 and
 *   `self` r5 as in the ROM.
 * - The bare asm("") after the nibble decrement lengthens `self + i *
 *   0xc8`'s life by one insn; that breaks its priority tie with the
 *   nibble pointer (8 refs/24 vs 6/12), so the pointer gets r4 and the
 *   base ip, as in the ROM. */
s32 sub_8001DB4(struct link_session *self)
{
    s32 i, j;
    s32 k;
    u8 *id;

    self->field_6 = 0;
    self->field_8 = 0;
    self->field_7 = 0;
    gUnknown_03000800 = 1;
    self->field_4 = 0;
    self->field_1c = -1;
    self->field_3fc = -1;
    ring_reset(&self->ring);
    id = self->id;
    sub_8001CB8(id);
    /* No code: 13 extra references on `id` (see above). */
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
    asm("" : : "r"(id));
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
        ((struct nibble_pair *)&self->players[i].id[1])->lo--;
        /* No code: one insn of padding (see above). */
        asm("");
        self->players[i].field_34 = 0;
        self->players[i].field_2c = 0;
        self->players[i].field_8 = (self->players[i].field_6 = 0x1234);
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
        v = *(vu16 *)p400;
        REG_SIOMLT_SEND = v;
    }
    return 0;
}
#else
NAKED void sub_8001DB4(u8 *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x10\n\t"
        "add r5, r0, #0\n\t"
        "mov r2, #0\n\t"
        "strb r2, [r5, #6]\n\t"
        "strb r2, [r5, #8]\n\t"
        "strb r2, [r5, #7]\n\t"
        "ldr r1, 4f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "strb r2, [r5, #4]\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "str r1, [r5, #0x1c]\n\t"
        "mov r3, #0xff\n\t"
        "lsl r3, r3, #2\n\t"
        "add r0, r5, r3\n\t"
        "str r1, [r0]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0xc4\n\t"
        "str r2, [r0]\n\t"
        "add r0, #4\n\t"
        "str r2, [r0]\n\t"
        "add r1, r5, #0\n\t"
        "add r1, #0xcc\n\t"
        "mov r0, #0x7f\n\t"
        "str r0, [r1]\n\t"
        "add r4, r5, #0\n\t"
        "add r4, #0x30\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8001CB8\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x28\n\t"
        "mov r6, #0xff\n\t"
        "mov r3, #3\n\t"
    "1:\n\t"
        "ldrb r7, [r2, #9]\n\t"
        "lsl r1, r7, #8\n\t"
        "ldrb r0, [r2, #8]\n\t"
        "orr r1, r0\n\t"
        "add r0, r1, #0\n\t"
        "and r0, r6\n\t"
        "strb r0, [r2]\n\t"
        "lsr r1, r1, #8\n\t"
        "strb r1, [r2, #1]\n\t"
        "add r2, #2\n\t"
        "sub r3, #1\n\t"
        "cmp r3, #0\n\t"
        "bge 1b\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0xc]\n\t"
        "str r0, [r5, #0x24]\n\t"
        "mov r6, #0\n\t"
        "add r1, r5, #0\n\t"
        "add r1, #0xfc\n\t"
        "str r1, [sp, #8]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x20\n\t"
        "str r2, [sp, #0xc]\n\t"
        "mov r3, #0xc8\n\t"
        "mov sl, r3\n\t"
        "mov r7, #0x80\n\t"
        "lsl r7, r7, #1\n\t"
        "add r7, r5, r7\n\t"
        "str r7, [sp]\n\t"
        "str r4, [sp, #4]\n\t"
    "2:\n\t"
        "mov r0, sl\n\t"
        "mul r0, r6, r0\n\t"
        "add r0, r0, r5\n\t"
        "mov r1, #0x84\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x84\n\t"
        "mov r2, #0\n\t"
        "str r2, [r1]\n\t"
        "add r1, #4\n\t"
        "str r2, [r1]\n\t"
        "add r0, #0x8c\n\t"
        "mov r1, #0x7f\n\t"
        "str r1, [r0]\n\t"
        "mov r0, sl\n\t"
        "mul r0, r6, r0\n\t"
        "ldr r3, [sp]\n\t"
        "add r0, r3, r0\n\t"
        "str r2, [r0]\n\t"
        "mov r4, #0\n\t"
        "add r7, r6, #1\n\t"
        "mov r8, r7\n\t"
        "mov r0, #0xc8\n\t"
        "add r1, r6, #0\n\t"
        "mul r1, r0, r1\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0xd0\n\t"
        "add r2, r1, r0\n\t"
        "ldr r3, [sp, #4]\n\t"
    "3:\n\t"
        "ldrb r0, [r3, #1]\n\t"
        "lsl r1, r0, #8\n\t"
        "ldrb r7, [r3]\n\t"
        "orr r1, r7\n\t"
        "add r0, r1, #0\n\t"
        "mov r7, #0xff\n\t"
        "and r0, r7\n\t"
        "strb r0, [r2]\n\t"
        "lsr r1, r1, #8\n\t"
        "strb r1, [r2, #1]\n\t"
        "add r2, #2\n\t"
        "add r3, #2\n\t"
        "add r4, #1\n\t"
        "cmp r4, #3\n\t"
        "ble 3b\n\t"
        "mov r2, sl\n\t"
        "mul r2, r6, r2\n\t"
        "add r0, r5, r2\n\t"
        "mov ip, r0\n\t"
        "mov r4, ip\n\t"
        "add r4, #0xd1\n\t"
        "ldrb r3, [r4]\n\t"
        "lsl r1, r3, #0x1c\n\t"
        "lsr r1, r1, #0x1c\n\t"
        "sub r1, #1\n\t"
        "mov r0, #0xf\n\t"
        "and r1, r0\n\t"
        "mov r7, #0x10\n\t"
        "neg r7, r7\n\t"
        "mov sb, r7\n\t"
        "mov r0, sb\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4]\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r5, r1\n\t"
        "add r0, r0, r2\n\t"
        "mov r3, #0\n\t"
        "str r3, [r0]\n\t"
        "ldr r7, [sp, #8]\n\t"
        "add r2, r7, r2\n\t"
        "str r3, [r2]\n\t"
        "mov r0, ip\n\t"
        "add r0, #0xd6\n\t"
        "ldr r1, 5f\n\t"
        "strh r1, [r0]\n\t"
        "add r0, #2\n\t"
        "add r2, r1, #0\n\t"
        "strh r2, [r0]\n\t"
        "mov r6, r8\n\t"
        "cmp r6, #3\n\t"
        "ble 2b\n\t"
        "mov r3, #0xfc\n\t"
        "lsl r3, r3, #2\n\t"
        "add r0, r5, r3\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "mov r7, #0xfd\n\t"
        "lsl r7, r7, #2\n\t"
        "add r0, r5, r7\n\t"
        "str r1, [r0]\n\t"
        "mov r2, #0xfe\n\t"
        "lsl r2, r2, #2\n\t"
        "add r0, r5, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, [r5, #0x38]\n\t"
        "str r1, [r5, #0x3c]\n\t"
        "mov r0, #0xf\n\t"
        "ldrh r3, [r5, #0x20]\n\t"
        "and r0, r3\n\t"
        "ldr r7, 6f\n\t"
        "add r1, r7, #0\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r5, #0x20]\n\t"
        "mov r0, sb\n\t"
        "ldr r1, [sp, #0xc]\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "strb r0, [r2]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #3\n\t"
        "add r1, r5, r3\n\t"
        "ldrh r0, [r5, #0x20]\n\t"
        "strh r0, [r1]\n\t"
        "ldrh r1, [r1]\n\t"
        "ldr r0, 7f\n\t"
        "strh r1, [r0]\n\t"
        "mov r0, #0\n\t"
        "add sp, #0x10\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "4: .4byte gUnknown_03000800\n"
    "5: .4byte 0x1234\n"
    "6: .4byte 0xF0B0\n"
    "7: .4byte 0x0400012A\n"
    );
}
#endif

#include "core.h"
#include "irq.h"

/* The GBA multiplayer link-cable/SIO transport - see docs/rom_map.md's
 * SIO/link-cable section. `sub_8001DB4` resets a per-session object at
 * `gUnknown_03000804` (still uncharacterized beyond the offsets touched
 * here and in src/graphics/settings_menu8a2.c/settings_menu.c); 4
 * per-player 0xc8-byte sub-records live at session+playerIndex*0xc8. */

extern void sub_8000544(s32 interruptIndex);
extern u16 gStaticData_0816AF10[];
extern void sub_8001CB8(u8 *self);
extern u8 gUnknown_03000800;
extern void sub_80005A0(s32 interruptIndex, irq_handler_t *fn);
extern void sub_8002830(void);
extern void sub_8002848(void);

/* The layouts below are what the NON_MATCHING drafts of sub_8001CB8/
 * sub_8001DB4/sub_8001F50 establish (docs/matching/
 * issue-4-6-8-naked-retry.md). */
struct nibble_pair {
    u8 lo:4;
    u8 hi:4;
} __attribute__((packed));

/* A 0x90-byte receive ring (the session header has one at +0x40, each
 * player record one at +0x38). */
struct link_ring {
    u8 unused_00[4];
    u8 buf[0x80];       /* 0x04 - written/read at field_8c/field_88 */
    s32 field_84;
    s32 field_88;
    s32 field_8c;       /* reset to 0x7f */
};

/* One per-player 0xc8-byte record of the link session. */
struct link_player {
    u8 id[6];           /* 0x00 - copy of the session handshake id */
    u16 field_6;        /* 0x06 - overwrites the id's hash with 0x1234 */
    u16 field_8;        /* 0x08 */
    u16 rx[16];         /* 0x0a - last 16 received words, at field_2c */
    u8 unused_2a[2];
    s32 field_2c;       /* 0x2c */
    s32 field_30;       /* 0x30 */
    s32 field_34;       /* 0x34 */
    struct link_ring ring; /* 0x38 */
};

struct link_id_word {
    u16 lo:4;
    u16 hi:12;
} __attribute__((packed));

/* The link session object (`*gUnknown_03000804`). */
struct link_session {
    u8 unused_00[4];
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 unused_09[3];
    s32 field_c;
    s32 field_10;
    s32 field_14;
    u8 field_18;
    u8 unused_19[3];
    s32 field_1c;
    struct link_id_word field_20;
    u8 unused_22[2];
    s32 field_24;
    u8 field_28[8];
    u8 id[8];           /* 0x30 - sub_8001CB8's handshake id */
    s32 field_38;
    s32 field_3c;
    struct link_ring ring;         /* 0x40 */
    struct link_player players[4]; /* 0xd0 */
    s32 field_3f0;
    s32 field_3f4;
    s32 field_3f8;
    s32 field_3fc;
    u16 field_400;
    u8 unused_402[2];
    s32 field_404;
};
COMPILE_TIME_ASSERT(sizeof(struct link_player) == 0xc8);
COMPILE_TIME_ASSERT(sizeof(struct link_session) == 0x408);

/* Fills `self`'s first 8 bytes with a fixed 0xEC pattern (byte 0 masked
 * to its low nibble, byte 1 zeroed), then hashes bytes 1-5 with a
 * CRC-16-style table walk seeded at 0x1234 (`gStaticData_0816AF10`,
 * also used by the still-raw `sub_8002114`), stores the 16-bit result
 * into bytes 6-7, and folds its low nibble into byte 0's low nibble
 * (preserving byte 0's high nibble; that high nibble is always 0 at
 * this point from the earlier mask, which is why the ROM's own
 * `(self[0]>>4) + hash` tail - reconstructing `hash` from the just-
 * stored bytes 6/7 rather than reusing the register - reduces to plain
 * `hash & 0xF`). Called by `sub_8001DB4` on each per-player 8-byte
 * sub-record - reads as generating a deterministic per-slot
 * handshake/session id.
 *
 * Once a NAKED transcription; it matches as plain C under old_agbcc
 * (link_cable.o is on the Makefile's OLD_AGBCC_OBJS) with no pins
 * (docs/matching/early-rom-naked-retry-2.md). The fill loop is written
 * over an integer address so the compare is the ROM's signed
 * `cmp; bge` (the ROM got it from strength-reducing `self[i]`, which
 * gcc here declines: "giv not worth while"); `c` ahead of it puts the
 * 0xec load first. `hash` is a u32 truncated with a `(u16)` cast each
 * step, and read back as `(u16)hash >> 8` into a u32 `hi`: with a u16
 * `hash`, CSE folds `hash >> 8` into `(x << 16) >> 24` of the loop's
 * zero-extend temporary. The new low nibble is masked in SImode
 * (`w`) before the bitfield store, so the 15 is an SImode constant and
 * reload's move2add derives the -16 from it (`sub r0, #0x1f`); a QImode
 * 15 gives `mov #16; neg`. */
void sub_8001CB8(u8 *self)
{
    u8 *p;
    u32 hash;
    s32 i;

    {
        u8 c = 0xec;
        s32 a = (s32)self + 7;

        do {
            *(u8 *)a = c;
        } while (--a >= (s32)self);
    }
    self[0] &= 0xf;
    self[1] = 0;
    hash = 0x1234;
    p = self + 1;
    for (i = 4; i != -1; i--) {
        hash = (u16)(gStaticData_0816AF10[((hash >> 8) ^ *p) & 0xff] ^ (hash << 8));
        p++;
    }
    {
        u32 hi;
        u32 v, n;
        s32 w;

        self[6] = hash;
        self[7] = hi = (u16)hash >> 8;
        n = ((struct nibble_pair *)self)->hi;
        v = (hi << 8) | self[6];
        w = (n + v) & 0xf;
        ((struct nibble_pair *)self)->lo = w;
    }
}

/* "Stop" step of the link session: disables the Serial and Timer3 IRQ
 * lines (each individually IME-guarded), clears their installed
 * handlers, restores IME, resets RCNT to general-purpose mode, sets
 * SIOCNT to a fixed idle value, reloads Timer3 (stopped) with 0xBBBC,
 * and acknowledges both IRQ flags in IF. Always returns 0. Its callers
 * in this file pass the session in r0 (`self` is unused), which the
 * NON_MATCHING drafts below reproduce by passing it. */
s32 sub_8001D30(struct link_session *self)
{
    u16 savedIme;

    REG_IME = 0;
    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x80;
    REG_IME = savedIme;

    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x40;
    REG_IME = savedIme;

    sub_8000544(INTR_INDEX_SERIAL);
    sub_8000544(INTR_INDEX_TIMER3);

    REG_IME = 1;

    REG_RCNT = 0;
    REG_SIOCNT = 0x3000;
    REG_TM3CNT = 0xBBBC;

    REG_IF |= 0x80;
    REG_IF |= 0x40;

    return 0;
}

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
 * other hard-compiler-limitation cases. */
#if NON_MATCHING
static inline void ring_reset(struct link_ring *r)
{
    r->field_84 = 0;
    r->field_88 = 0;
    r->field_8c = 0x7f;
}

/* 136 halfwords off under old_agbcc (same size as the ROM). The
 * `link_ring` resets, the id copies and the tail match in shape; the
 * per-player loop allocates differently: the ROM recomputes `i * 0xc8`
 * for each field, precomputes `i + 1` before the inner copy loop and
 * keeps that loop counting up. Later pass: do/while, goto, `continue`
 * and explicit src/dst pointer forms of the inner loop all still get
 * reversed by old_agbcc (-fno-strength-reduce is worse: the ROM's first
 * id loop *is* reversed and pointer-reduced). The ROM also doesn't hoist
 * `self + 0xd0` out of the outer loop (it builds the inner dst giv as
 * `i * 0xc8 + (self + 0xd0)` each pass) and re-reads `field_400` after
 * storing it. Second pass (docs/matching/early-rom-naked-retry-2.md): an
 * inner `for` over explicit src/dst pointers with `asm("" : "+r"(j))`
 * in the body keeps the counter counting up with the ROM's in-loop
 * `mov #0xff` (a goto loop also counts up, but combine then folds
 * `w & 0xff` into a second `ldrb`); the outer loop then hoists 0x1234,
 * 0 and `self + 0x104` where the ROM hoists only 200, `self + 0x100`,
 * `self + 0xfc`, `self + 0x20` and the id pointer (163 halfwords, so
 * not adopted). Third pass (docs/matching/last-four-naked-retry.md):
 * escaped `self`/`players` pointers for the inner dst, a pointer-form
 * first loop and re-read tails did not beat 136. */
s32 sub_8001DB4(struct link_session *self)
{
    s32 i, j;

    self->field_6 = 0;
    self->field_8 = 0;
    self->field_7 = 0;
    gUnknown_03000800 = 1;
    self->field_4 = 0;
    self->field_1c = -1;
    self->field_3fc = -1;
    ring_reset(&self->ring);
    sub_8001CB8(self->id);
    for (i = 0; i <= 3; i++) {
        u32 v = (self->id[i * 2 + 1] << 8) | self->id[i * 2];
        u32 lo = v & 0xff;

        self->field_28[i * 2] = lo;
        self->field_28[i * 2 + 1] = v >> 8;
    }
    self->field_c = 0;
    self->field_24 = 0;
    for (i = 0; i <= 3; i++) {
        ring_reset(&self->players[i].ring);
        self->players[i].field_30 = 0;
        for (j = 0; j <= 3; j++) {
            u32 w = (self->id[j * 2 + 1] << 8) | self->id[j * 2];
            u32 lo = w & 0xff;

            self->players[i].id[j * 2] = lo;
            self->players[i].id[j * 2 + 1] = w >> 8;
        }
        ((struct nibble_pair *)&self->players[i].id[1])->lo--;
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
    self->field_400 = *(u16 *)&self->field_20;
    REG_SIOMLT_SEND = self->field_400;
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

/* Link-connection/handshake driver - see docs/rom_map.md's SIO/link-
 * cable section. Called repeatedly (once per frame) until the link is
 * established or times out. `self+5` gates a one-time SIOCNT setup
 * (multiplayer mode, `0x4003`); `self+6` gates a second one-time
 * SIOCNT poke once `self+8` (the "IRQs installed" flag) is still
 * clear. Once `self+8` is clear, checks `REG_SIOCNT` bit 3 (the
 * multi-player "ready" bit): if not ready yet, resets the session
 * (`sub_8001D30`) and re-primes SIOCNT/SIODATA32_H, returning 0
 * ("still connecting"). If ready, flips `self+8`, derives an "is
 * player 0 / arm3" flag from `REG_SIOCNT` bits, resets `REG_SIODATA32`
 * (via `REG_TM3CNT`/`0x04000208` toggling, matching
 * `sub_80026E4`-family's RCNT/SIOCNT reset shape in
 * src/system/link_cable2.c), installs the Serial IRQ handler
 * (`sub_8002830`) always and the Timer3 IRQ handler (`sub_8002848`)
 * only when the "arm3" flag is set, programs Timer3 as a running
 * countdown timeout (`0x00C0BBBC`) in that case, and resets several
 * per-session timeout/retry fields (`self+0x1c`=-1, `self+0x14`,
 * `self+0x404`=0, `self+0x18`=0). From there (and on every call once
 * `self+8` is already set), it advances a handshake-timeout counter at
 * `self+0x404`: past 15 it forces `self+0xc`=-15 and rearms
 * `self+0x14` to a large timeout constant (0x708); once `self+7`
 * (retry-exhausted flag) is still clear, it walks a retry-backoff
 * counter at `self+0xc` (incrementing or decrementing depending on
 * whether `self+0xfc`'s stored `s32` is negative), clamping it into
 * `self+7`/giving up (return 0, via the same early-exit path as the
 * top-of-function "still initializing" case) once it exceeds -15, or
 * resetting the session (`sub_8001D30`+`sub_8001DB4`) and retrying
 * once it exceeds 14; either way it refreshes `self+0x10`/`self+0x14`
 * from each other (keeping the larger, with `self+0x18` forcing a
 * reset to 0) and, past a 0x1d threshold, resets the session again.
 * Finally increments `self+0xc` and returns 1 ("handshake in
 * progress/succeeded").
 *
 * Matched in the second near-miss sweep (37 halfwords before). The ROM
 * materializes two separate 1s after reading SIOCNT: r2 (copied to sb)
 * for `field_8`/IME and r1 for the arm3 flag. Three things reproduce
 * that:
 * - `asm("" : "+r"(one1))` keeps the flag's 1 from being merged into
 *   `one`.
 * - The ready test uses a literal 1, so `one` is a copy of that
 *   constant's register.
 * - `asm("" : "+r"(arm3))` between the eor and the and stops combine
 *   from folding `(x ^ 1) & 1` into a `bic`, which the ROM doesn't have.
 * Both asm statements emit no code. Matches under both compilers. */
s32 sub_8001F50(struct link_session *self)
{
    s32 arm3;
    u16 saved;
    s32 one;

    if (!self->field_5)
        return 0;
    if (!self->field_6) {
        REG_RCNT = 0;
        REG_SIOCNT = 0x2000;
        REG_SIOCNT |= 0x4003;
        self->field_6 = 1;
    }
    if (!self->field_8) {
        s32 one1;
        u32 v = REG_SIOCNT >> 3;

        one1 = 1;
        asm("" : "+r"(one1)); /* keep the flag's own 1 (r1) */
        one = 1;
        if (!(v & 1)) {
            sub_8001D30(self);
            REG_RCNT = 0;
            REG_SIOCNT = 0x2000;
            REG_SIOCNT |= 0x4003;
            return 0;
        }
        self->field_8 = one;
        arm3 = (REG_SIOCNT >> 2) ^ one1;
        asm("" : "+r"(arm3)); /* keep eor/and, not bic */
        arm3 &= one1;
        REG_IME = 0;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~0x80;
        REG_IME = saved;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~0x40;
        REG_IME = saved;
        sub_8000544(INTR_INDEX_TIMER3);
        sub_80005A0(INTR_INDEX_SERIAL, (irq_handler_t *)sub_8002830);
        REG_IE |= 0x80;
        if (arm3) {
            sub_80005A0(INTR_INDEX_TIMER3, (irq_handler_t *)sub_8002848);
            REG_IE |= 0x40;
            REG_TM3CNT = 0x00C0BBBC;
        }
        REG_IME = one;
        self->field_3fc = -1;
        self->field_14 = 0;
        self->field_404 = 0;
        self->field_18 = 0;
    }
    if (self->field_404 > 15) {
        self->field_c = -15;
        self->field_14 = 0x708;
    }
    self->field_404++;
    if (!self->field_7) {
        if (self->field_3fc < 0)
            self->field_c = self->field_c - 1;
        else
            self->field_c = self->field_c + 1;
        if (self->field_c > 14) {
            self->field_7 = 1;
            self->field_14 = 0;
            self->field_10 = 0;
        } else if (self->field_c > -15) {
            return 0;
        } else {
            sub_8001D30(self);
            sub_8001DB4(self);
        }
    }
    {
        s32 a = self->field_10;
        s32 b = self->field_14;

        if (a < b)
            a = b;
        self->field_10 = a;
        b = self->field_18 ? 0 : b + 1;
        self->field_14 = b;
        self->field_18 = 0;
        if (b > 0x1d) {
            sub_8001D30(self);
            sub_8001DB4(self);
        }
    }
    self->field_c++;
    return 1;
}

/* Per-frame SIO data-exchange pump - see docs/rom_map.md's SIO/link-
 * cable section (called from the Serial IRQ handler `sub_8002830` in
 * src/system/link_cable2.c, with the session object and SIODATA32's
 * low half as the two arguments). `docs/rom_map.md` confirms the
 * high-level shape: manipulates `REG_SIOCNT`/`SIODATA8`
 * (`0x04000128`/`0x0400012A`), checking specific control bits before
 * touching per-object state - it was "not read to completion" there.
 * This pass read it further without reaching full per-branch
 * confidence, so it's recorded here as a best-effort guide for whoever
 * attempts a real C reconstruction next, not as a verified spec: if
 * `self+4` (a "connected" flag also touched by `sub_8001F50` above) is
 * already set, it just mirrors the outgoing word into SIOMLT_SEND and
 * returns; otherwise, on the first call it seeds SIOMLT_SEND from
 * `self+0x20` and sets that flag, and on every call after that it
 * inspects `REG_SIOCNT`'s per-slot bits against the incoming word to
 * work out which of the 4 multiplayer slots are actually present,
 * latching player-count-derived fields once that stabilizes across a
 * few frames (`self+0x1c`) and marking the link "ready" (`self+7`).
 * Past that point each call walks the 4 player sub-records
 * (`self+i*0xc8`, i=0..3, same region as `sub_8001DB4`'s struct above)
 * and, once every 4 samples, runs the exact same CRC-16 table walk
 * `sub_8001CB8` uses (`gStaticData_0816AF10`) over each slot's 8-byte
 * handshake-id mirror - this part is a confident read, since the loop
 * body is textually identical to `sub_8001CB8`'s - feeding the result
 * into per-slot bookkeeping this pass did not fully trace (fields
 * around `self+0x30`/`self+0xc4`/`self+0xbc`/`self+0x36`/`self+0x38`/
 * `self+0x3c`), before deciding what to send out next over SIOMLT_SEND.
 *
 * Written as NAKED asm, not plain C: this project's biggest raw
 * function by a wide margin (1488 B) - `ip`/`r8`/`sb`/`sl` are all
 * live simultaneously across a 0x2c-byte stack frame with deep nested
 * branching (60 branch targets), left completely untouched by the
 * previous matching pass rather than risk a low-confidence C
 * reconstruction. Like `sub_8001F50` above, this is a mechanical,
 * byte-verified transcription of the ROM's own instructions (checked
 * instruction-by-instruction against the ROM disassembly via
 * `objdump` before being counted as matched), not an inferred
 * C control-flow guess - unlike the functions above, this one's
 * semantics are NOT fully confirmed (see the paragraph above), only
 * its bytes; a future pass with more time should re-derive the real
 * field names/struct layout and replace this transcription with
 * genuine C, using this comment as a starting point rather than a
 * finished answer. */
#if NON_MATCHING
/* C draft (docs/matching/big-naked-retry-3.md,
 * docs/matching/early-rom-naked-retry-2.md,
 * docs/matching/last-four-naked-retry.md,
 * docs/matching/last-six-naked-retry.md): same size as the ROM
 * (1488 bytes) under old_agbcc, 6 halfwords off (was 16). Control
 * flow, block order and the stack frame match. What is left is the
 * first receive loop only: the ROM's `data[i]` load giv is in r1 and
 * the 0xffff test giv in r3 (here swapped), and its spilled counter
 * reloads into r0 (here r7). Taking the test address first
 * (`u16 *t = &d2[i]`) fixes the load/test pair but puts the `w[i]`
 * giv first; see the doc for the giv-order analysis.
 * `data` is SIOMULTI0-3 (link_cable2.c passes 0x04000120). */
/* A received SIOMULTI word, read back from a stack copy. */
struct link_rx_word {
    u32 lo:4;
    u32 hi:12;
    u32 unused_10:16;
};

#define LINK_NIB(p) (*(struct nibble_pair *)(p))

/* The CRC-16 walk sub_8001CB8 also uses, over bytes 1-5 of an id. */
#define LINK_HASH(hash, p)                                                     \
    {                                                                          \
        s32 _k;                                                                \
        u8 *_p = (p);                                                          \
                                                                               \
        for (_k = 4; _k != -1; _k--) {                                         \
            hash = gStaticData_0816AF10[((hash >> 8) ^ *_p) & 0xff] ^ (hash << 8); \
            _p++;                                                              \
        }                                                                      \
    }

/* Copies an 8-byte id as four byte-assembled halfwords. */
#define LINK_COPY_ID(dst, src)                                                 \
    {                                                                          \
        s32 _j;                                                                \
                                                                               \
        for (_j = 0; _j <= 3; _j++) {                                          \
            u32 _v = ((src)[_j * 2 + 1] << 8) | (src)[_j * 2];                 \
            u32 _lo = _v & 0xff;                                               \
                                                                               \
            (dst)[_j * 2] = _lo;                                               \
            (dst)[_j * 2 + 1] = _v >> 8;                                       \
        }                                                                      \
    }

/* The session ring pop. `rf` is a second copy of the ring pointer for
 * the fast loop: the ROM builds that loop's field addresses from a copy
 * made right after the id copy (`adds r4, r7, #0`), and the wrap loop's
 * from the original. The bounds test goes through `rd`, the caller's
 * `&ring.field_88`. */
static inline void LinkRingPop(struct link_ring *r, struct link_ring *rf, u8 *dst, s32 n, s32 *rd)
{
    s32 k;

    if (*rd < 0x80 - n) {
        for (k = n - 1; k != -1; k--) {
            *dst++ = rf->buf[rf->field_88];
            rf->field_88++;
            rf->field_84--;
        }
    } else {
        for (k = n - 1; k != -1; k--) {
            s32 old = r->field_88;
            s32 nw = 0;

            if (old != 0x7f)
                nw = old + 1;
            r->field_88 = nw;
            r->field_84--;
            *dst++ = r->buf[old];
        }
    }
}

void sub_8002114(struct link_session *self, u16 *data)
{
    struct link_rx_word w[4];
    s32 i;
    s32 changed;
    s32 nib;
    u16 siocnt;
    u32 one;

    /* Instruction-count padding (no code): shifts gcc's temporary
     * numbering so two stack slots come out in the ROM's order. */
    asm("");
    self->field_404 = 0;
    if (self->field_4) {
        u16 v = self->field_400;

        REG_SIOMLT_SEND = v;
        return;
    }
    one = 1;
    self->field_4 = one;
    siocnt = REG_SIOCNT;
    changed = 0;
    nib = LINK_NIB(&self->id[1]).lo + 1;
    nib &= 0xf;
    if ((siocnt >> 6) & one)
        goto send;
    if (!self->field_7) {
        s32 nId, nFree, same;

        if (!((siocnt >> 3) & one))
            goto send;
        nId = 0;
        nFree = 0;
        {
            u16 *d2 = data;

            for (i = 0; i <= 3; i++) {
                w[i] = *(struct link_rx_word *)&data[i];
                if (w[i].hi == 0xF0B)
                    nId++;
                if (d2[i] == 0xffff)
                    nFree++;
            }
        }
        same = 1;
        for (i = 0; i < nId; i++) {
            if (w[i].lo != nId)
                same = 0;
        }
        if (nFree + nId == 4 && same && nId > 1) {
            self->field_1c = nId;
            {
                s32 me = (REG_SIOCNT & 0x30) >> 4;

                self->field_3fc = me;
            }
            self->field_3f8 = ~(-1 << nId);
            self->field_3f8 &= ~(1 << self->field_3fc);
        }
        self->field_20.lo = nId;
        {
            u16 *dst = &self->field_400;

            *dst = *(u16 *)&self->field_20;
        }
        goto send;
    }

    for (i = 0; i < self->field_1c; i++) {
        struct link_player *p;
        u8 *q;
        s32 ok;

        if (i == self->field_3fc)
            continue;
        p = &self->players[i];
        p->rx[p->field_2c] = data[i];
        p->field_2c = (p->field_2c + 1) & 0xf;
        if (p->field_2c <= 3)
            continue;
        q = (u8 *)&p->rx[p->field_2c - 4];
        ok = 0;
        if (LINK_NIB(&q[1]).lo == LINK_NIB(&q[0]).hi
         || LINK_NIB(&q[1]).lo == ((LINK_NIB(&q[0]).hi - 1) & 0xf)) {
            if (LINK_NIB(&q[1]).hi <= 4) {
                /* A u8 against a u16: the compare is done in HImode, so
                 * `lo`'s zero-extension is emitted at the compare (the
                 * ROM's split lsls/lsrs #28 pair). */
                u8 lo = LINK_NIB(&q[0]).lo;
                u16 sum = (u16)(LINK_NIB(&q[0]).hi + ((q[7] << 8) | q[6])) % 16;

                if (lo == sum)
                    ok = 1;
            }
        }
        if (!ok)
            continue;
        {
            if ((q[1] & 0xf) == (p->id[1] & 0xf)) {
                u16 want = (q[7] << 8) | q[6];
                u16 hash = p->field_8;

                LINK_HASH(hash, &q[1]);
                if (want == hash)
                    goto copy;
                continue;
            } else {
                u16 want;
                u16 hash;
                s32 n, k;
                u8 *src;

                if (LINK_NIB(&q[1]).lo != p->field_30)
                    continue;
                want = (q[7] << 8) | q[6];
                hash = p->field_6;
                LINK_HASH(hash, &q[1]);
                if (want != hash)
                    continue;
                n = p->id[1] >> 4;
                /* Extra reference (no code): raises `n`'s priority so it
                 * gets its own register (r7) instead of reusing the
                 * id-byte one. */
                asm("" : : "r"(n));
                src = &p->id[2];
                /* The bounds test reaches the ring through an escaped copy
                 * of `p` (no code), so CSE doesn't share its address with
                 * the loop pre-headers, which recompute it as the ROM does. */
                if (({ struct link_player *_q = p; asm("" : "+r"(_q)); _q; })->ring.field_8c < 0x80 - n) {
                    for (k = n - 1; k != -1; k--) {
                        p->ring.field_8c++;
                        p->ring.field_84++;
                        p->ring.buf[p->ring.field_8c] = *src++;
                    }
                } else {
                    for (k = n - 1; k != -1; k--) {
                        u8 b = *src++;

                        p->ring.field_8c = p->ring.field_8c == 0x7f ? 0 : p->ring.field_8c + 1;
                        p->ring.field_84++;
                        p->ring.buf[p->ring.field_8c] = b;
                    }
                }
                p->field_34 += n;
                p->field_8 = p->field_6;
                p->field_30 = (p->field_30 + 1) & 0xf;
                self->field_3f4 |= 1 << i;
            }
        }
    copy:
        LINK_COPY_ID(p->id, q);
        if (LINK_NIB(&p->id[0]).hi == nib)
            self->field_3f0 |= 1 << i;
        p->field_2c = 0;
        changed = 1;
    }

    if (changed) {
        changed = 0;
        if (self->field_3f0 == self->field_3f8) {
            s32 n, k;
            u8 *dst;
            u16 hash;
            u8 *id;
            struct link_ring *ring;
            s32 *cnt;
            s32 *rd;
            struct link_ring *rf;

            self->field_3f0 = 0;
            id = self->id;
            ring = &self->ring;
            cnt = &self->ring.field_84;
            dst = &self->id[2];
            rd = &self->ring.field_88;
            {
                u8 *d = self->field_28;
                u8 *s = id;

                for (k = 0; k <= 3; k++) {
                    u32 v = (s[1] << 8) | s[0];
                    u32 lo = v & 0xff;

                    d[0] = lo;
                    d[1] = v >> 8;
                    d += 2;
                    s += 2;
                }
            }
            rf = ring;
            /* A distinct copy of the ring pointer (no code), taken here
             * like the ROM's `adds r4, r7, #0`. */
            asm("" : "+r"(rf));
            n = *cnt;
            if (n > 4)
                n = 4;
            LINK_NIB(&id[1]).hi = n;
            LinkRingPop(ring, rf, dst, n, rd);
            self->field_24 += n;
            {
                /* A signed QImode read-modify-write: the ROM's mask is
                 * -16 and `nib` (already masked) is not masked again. */
                s8 *b = (s8 *)&self->id[1];

                *b = (*b & ~0xf) | nib;
            }
            hash = *(u16 *)&self->id[6];
            LINK_HASH(hash, &self->id[1]);
            {
                /* The ROM sets this 0 in r0 before the hash store. */
                register s32 z asm("r0") = 0;

                *(u16 *)&self->id[6] = hash;
                self->field_3c = z;
            }
            changed = 1;
        }
        if (self->field_3f4 == self->field_3f8) {
            self->field_3f4 = 0;
            LINK_NIB(&self->id[0]).hi = (LINK_NIB(&self->id[0]).hi + 1) & 0xf;
            changed = 1;
        }
        if (changed) {
            self->field_38 = 0;
            self->field_18 = 1;
            {
                u8 *id = self->id;
                u32 n = LINK_NIB(&id[0]).hi;
                u32 v = (id[7] << 8) | id[6];
                s32 w = (n + v) & 0xf;

                LINK_NIB(&id[0]).lo = w;
            }
        }
    }
    {
        u8 *lo, *hi;

        if ((self->field_3c & 3) == 3) {
            u8 *b = (u8 *)self + self->field_38 * 2;

            lo = b + 0x28;
            hi = b + 0x29;
        } else {
            u8 *b = (u8 *)self + self->field_38 * 2;

            lo = b + 0x30;
            hi = b + 0x31;
        }
        self->field_400 = (*hi << 8) | *lo;
    }
    if (++self->field_38 == 4) {
        self->field_38 = 0;
        self->field_3c++;
    }
send:
    {
        u16 v = self->field_400;

        REG_SIOMLT_SEND = v;
    }
    self->field_4 = 0;
}
#else
NAKED void sub_8002114(void *self, u16 data)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x2c\n\t"
        "mov sl, r0\n\t"
        "str r1, [sp, #0x10]\n\t"
        "ldr r0, 1f\n\t"
        "add r0, sl\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "mov r1, sl\n\t"
        "ldrb r0, [r1, #4]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, sl\n\t"
        "ldrh r1, [r0]\n\t"
        "ldr r0, 2f\n\t"
        "strh r1, [r0]\n\t"
        "b 59f\n\t"
        ".align 2, 0\n"
        "1: .4byte 0x00000404\n"
        "2: .4byte 0x0400012A\n"
        "3:\n\t"
        "mov r2, #1\n\t"
        "mov r3, sl\n\t"
        "strb r2, [r3, #4]\n\t"
        "ldr r0, 13f\n\t"
        "ldrh r0, [r0]\n\t"
        "lsl r3, r0, #0x10\n\t"
        "mov r4, #0\n\t"
        "str r4, [sp, #0x18]\n\t"
        "mov r1, sl\n\t"
        "add r1, #0x31\n\t"
        "ldrb r5, [r1]\n\t"
        "lsl r0, r5, #0x1c\n\t"
        "lsr r0, r0, #0x1c\n\t"
        "add r0, #1\n\t"
        "str r0, [sp, #0x1c]\n\t"
        "mov r0, #0xf\n\t"
        "ldr r7, [sp, #0x1c]\n\t"
        "and r7, r0\n\t"
        "str r7, [sp, #0x1c]\n\t"
        "lsr r0, r3, #0x16\n\t"
        "and r0, r2\n\t"
        "str r1, [sp, #0x28]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "b 58f\n\t"
        "4:\n\t"
        "mov r1, sl\n\t"
        "ldrb r0, [r1, #7]\n\t"
        "cmp r0, #0\n\t"
        "bne 16f\n\t"
        "lsr r0, r3, #0x13\n\t"
        "and r0, r2\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "b 58f\n\t"
        "5:\n\t"
        "mov r5, #0\n\t"
        "mov r2, #0x20\n\t"
        "add r2, sl\n\t"
        "mov r8, r2\n\t"
        "ldr r3, 14f\n\t"
        "mov sb, r3\n\t"
        "ldr r6, 15f\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "mov r2, sp\n\t"
        "add r3, r1, #0\n\t"
        "mov r7, #3\n\t"
        "str r7, [sp, #0x14]\n\t"
        "6:\n\t"
        "ldr r0, [r1]\n\t"
        "str r0, [r2]\n\t"
        "ldrh r7, [r2]\n\t"
        "lsr r0, r7, #4\n\t"
        "cmp r0, sb\n\t"
        "bne 7f\n\t"
        "add r4, #1\n\t"
        "7:\n\t"
        "ldrh r0, [r3]\n\t"
        "cmp r0, r6\n\t"
        "bne 8f\n\t"
        "add r5, #1\n\t"
        "8:\n\t"
        "add r1, #2\n\t"
        "add r2, #4\n\t"
        "add r3, #2\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "sub r0, #1\n\t"
        "str r0, [sp, #0x14]\n\t"
        "cmp r0, #0\n\t"
        "bge 6b\n\t"
        "mov r2, #1\n\t"
        "add r3, r5, r4\n\t"
        "cmp r4, #0\n\t"
        "ble 11f\n\t"
        "mov r1, sp\n\t"
        "str r4, [sp, #0x14]\n\t"
        "9:\n\t"
        "ldrb r5, [r1]\n\t"
        "lsl r0, r5, #0x1c\n\t"
        "lsr r0, r0, #0x1c\n\t"
        "cmp r0, r4\n\t"
        "beq 10f\n\t"
        "mov r2, #0\n\t"
        "10:\n\t"
        "add r1, #4\n\t"
        "ldr r7, [sp, #0x14]\n\t"
        "sub r7, #1\n\t"
        "str r7, [sp, #0x14]\n\t"
        "cmp r7, #0\n\t"
        "bne 9b\n\t"
        "11:\n\t"
        "cmp r3, #4\n\t"
        "bne 12f\n\t"
        "cmp r2, #0\n\t"
        "beq 12f\n\t"
        "cmp r4, #1\n\t"
        "ble 12f\n\t"
        "mov r0, sl\n\t"
        "str r4, [r0, #0x1c]\n\t"
        "ldr r0, 13f\n\t"
        "ldrh r1, [r0]\n\t"
        "mov r0, #0x30\n\t"
        "and r0, r1\n\t"
        "lsr r0, r0, #4\n\t"
        "mov r2, #0xff\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, sl\n\t"
        "str r0, [r2]\n\t"
        "mov r3, #0xfe\n\t"
        "lsl r3, r3, #2\n\t"
        "add r3, sl\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "lsl r1, r4\n\t"
        "mvn r1, r1\n\t"
        "str r1, [r3]\n\t"
        "ldr r2, [r2]\n\t"
        "mov r0, #1\n\t"
        "lsl r0, r2\n\t"
        "bic r1, r0\n\t"
        "str r1, [r3]\n\t"
        "12:\n\t"
        "mov r0, #0xf\n\t"
        "and r4, r0\n\t"
        "mov r0, #0x10\n\t"
        "neg r0, r0\n\t"
        "mov r1, r8\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "orr r0, r4\n\t"
        "mov r2, r8\n\t"
        "strb r0, [r2]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #3\n\t"
        "add r1, sl\n\t"
        "mov r3, sl\n\t"
        "ldrh r0, [r3, #0x20]\n\t"
        "strh r0, [r1]\n\t"
        "b 58f\n\t"
        ".align 2, 0\n"
        "13: .4byte 0x04000128\n"
        "14: .4byte 0x00000F0B\n"
        "15: .4byte 0x0000FFFF\n"
        "16:\n\t"
        "mov r4, #0\n\t"
        "str r4, [sp, #0x14]\n\t"
        "mov r5, sl\n\t"
        "ldr r0, [r5, #0x1c]\n\t"
        "ldr r7, [sp, #0x18]\n\t"
        "cmp r7, r0\n\t"
        "blt 17f\n\t"
        "b 41f\n\t"
        "17:\n\t"
        "mov r0, #0xfd\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, sl\n\t"
        "str r0, [sp, #0x20]\n\t"
        "18:\n\t"
        "mov r0, #0xff\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, sl\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "add r1, #1\n\t"
        "str r1, [sp, #0x24]\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "cmp r2, r0\n\t"
        "bne 19f\n\t"
        "b 40f\n\t"
        "19:\n\t"
        "mov r0, #0xc8\n\t"
        "mul r0, r2, r0\n\t"
        "add r0, #0xd0\n\t"
        "add r0, sl\n\t"
        "mov ip, r0\n\t"
        "ldr r0, [r0, #0x2c]\n\t"
        "lsl r0, r0, #1\n\t"
        "mov r1, ip\n\t"
        "add r1, #0xa\n\t"
        "add r1, r1, r0\n\t"
        "lsl r0, r2, #1\n\t"
        "ldr r3, [sp, #0x10]\n\t"
        "add r0, r0, r3\n\t"
        "ldrh r0, [r0]\n\t"
        "strh r0, [r1]\n\t"
        "mov r4, ip\n\t"
        "ldr r0, [r4, #0x2c]\n\t"
        "add r0, #1\n\t"
        "mov r6, #0xf\n\t"
        "and r0, r6\n\t"
        "str r0, [r4, #0x2c]\n\t"
        "cmp r0, #3\n\t"
        "bgt 20f\n\t"
        "b 40f\n\t"
        "20:\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, #2\n\t"
        "add r0, ip\n\t"
        "mov sb, r0\n\t"
        "mov r7, #0\n\t"
        "ldrb r5, [r0, #1]\n\t"
        "lsl r3, r5, #0x1c\n\t"
        "ldrb r2, [r0]\n\t"
        "lsl r4, r2, #0x18\n\t"
        "lsr r1, r3, #0x1c\n\t"
        "lsr r0, r4, #0x1c\n\t"
        "cmp r1, r0\n\t"
        "beq 21f\n\t"
        "sub r0, #1\n\t"
        "and r0, r6\n\t"
        "cmp r1, r0\n\t"
        "bne 22f\n\t"
        "21:\n\t"
        "lsr r0, r5, #4\n\t"
        "cmp r0, #4\n\t"
        "bhi 22f\n\t"
        "lsl r2, r2, #0x1c\n\t"
        "lsr r1, r4, #0x1c\n\t"
        "mov r5, sb\n\t"
        "ldrb r5, [r5, #7]\n\t"
        "lsl r0, r5, #8\n\t"
        "mov r3, sb\n\t"
        "ldrb r3, [r3, #6]\n\t"
        "orr r0, r3\n\t"
        "add r1, r1, r0\n\t"
        "and r1, r6\n\t"
        "lsr r2, r2, #0x1c\n\t"
        "cmp r2, r1\n\t"
        "bne 22f\n\t"
        "mov r7, #1\n\t"
        "22:\n\t"
        "ldr r4, [sp, #0x14]\n\t"
        "add r4, #1\n\t"
        "str r4, [sp, #0x24]\n\t"
        "cmp r7, #0\n\t"
        "bne 23f\n\t"
        "b 40f\n\t"
        "23:\n\t"
        "mov r5, sb\n\t"
        "ldrb r3, [r5, #1]\n\t"
        "mov r0, #0xf\n\t"
        "mov r7, ip\n\t"
        "ldrb r2, [r7, #1]\n\t"
        "add r1, r0, #0\n\t"
        "and r1, r3\n\t"
        "and r0, r2\n\t"
        "mov r8, r2\n\t"
        "cmp r1, r0\n\t"
        "bne 27f\n\t"
        "ldrb r0, [r5, #7]\n\t"
        "lsl r3, r0, #8\n\t"
        "ldrb r1, [r5, #6]\n\t"
        "orr r3, r1\n\t"
        "ldrh r0, [r7, #8]\n\t"
        "mov r2, sb\n\t"
        "add r2, #1\n\t"
        "mov r4, #4\n\t"
        "ldr r5, 26f\n\t"
        "mov r8, r5\n\t"
        "mov r6, #0xff\n\t"
        "mov r5, #1\n\t"
        "neg r5, r5\n\t"
        "24:\n\t"
        "lsr r1, r0, #8\n\t"
        "ldrb r7, [r2]\n\t"
        "eor r1, r7\n\t"
        "and r1, r6\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r8\n\t"
        "lsl r0, r0, #8\n\t"
        "ldrh r1, [r1]\n\t"
        "eor r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r2, #1\n\t"
        "sub r4, #1\n\t"
        "cmp r4, r5\n\t"
        "bne 24b\n\t"
        "cmp r3, r0\n\t"
        "bne 25f\n\t"
        "b 37f\n\t"
        "25:\n\t"
        "b 40f\n\t"
        ".align 2, 0\n"
        "26: .4byte gStaticData_0816AF10\n"
        "27:\n\t"
        "lsl r0, r3, #0x1c\n\t"
        "lsr r0, r0, #0x1c\n\t"
        "mov r2, ip\n\t"
        "ldr r1, [r2, #0x30]\n\t"
        "cmp r0, r1\n\t"
        "beq 28f\n\t"
        "b 40f\n\t"
        "28:\n\t"
        "mov r3, sb\n\t"
        "ldrb r3, [r3, #7]\n\t"
        "lsl r4, r3, #8\n\t"
        "mov r5, sb\n\t"
        "ldrb r5, [r5, #6]\n\t"
        "orr r4, r5\n\t"
        "ldrh r0, [r2, #6]\n\t"
        "mov r3, sb\n\t"
        "add r3, #1\n\t"
        "mov r5, #4\n\t"
        "ldr r2, 32f\n\t"
        "mov r6, #1\n\t"
        "neg r6, r6\n\t"
        "29:\n\t"
        "lsr r1, r0, #8\n\t"
        "ldrb r7, [r3]\n\t"
        "eor r1, r7\n\t"
        "mov r7, #0xff\n\t"
        "and r1, r7\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r0, r0, #8\n\t"
        "ldrh r1, [r1]\n\t"
        "eor r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, #1\n\t"
        "sub r5, #1\n\t"
        "cmp r5, r6\n\t"
        "bne 29b\n\t"
        "cmp r4, r0\n\t"
        "beq 30f\n\t"
        "b 40f\n\t"
        "30:\n\t"
        "mov r0, r8\n\t"
        "lsr r7, r0, #4\n\t"
        "mov r4, ip\n\t"
        "add r4, #2\n\t"
        "mov r0, ip\n\t"
        "add r0, #0xc4\n\t"
        "mov r1, #0x80\n\t"
        "sub r1, r1, r7\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, r1\n\t"
        "bge 33f\n\t"
        "sub r3, r7, #1\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "cmp r3, r1\n\t"
        "beq 36f\n\t"
        "mov r2, ip\n\t"
        "add r2, #0xc4\n\t"
        "mov r5, ip\n\t"
        "add r5, #0xbc\n\t"
        "mov r6, ip\n\t"
        "add r6, #0x3c\n\t"
        "31:\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #1\n\t"
        "str r0, [r2]\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, #1\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldrb r1, [r4]\n\t"
        "strb r1, [r0]\n\t"
        "add r4, #1\n\t"
        "sub r3, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r3, r0\n\t"
        "bne 31b\n\t"
        "b 36f\n\t"
        ".align 2, 0\n"
        "32: .4byte gStaticData_0816AF10\n"
        "33:\n\t"
        "sub r3, r7, #1\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "cmp r3, r1\n\t"
        "beq 36f\n\t"
        "mov r2, ip\n\t"
        "add r2, #0xc4\n\t"
        "mov r5, ip\n\t"
        "add r5, #0xbc\n\t"
        "mov r0, #0x3c\n\t"
        "add r0, ip\n\t"
        "mov r8, r0\n\t"
        "34:\n\t"
        "ldrb r6, [r4]\n\t"
        "add r4, #1\n\t"
        "ldr r0, [r2]\n\t"
        "mov r1, #0\n\t"
        "cmp r0, #0x7f\n\t"
        "beq 35f\n\t"
        "add r1, r0, #1\n\t"
        "35:\n\t"
        "str r1, [r2]\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, #1\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, r8\n\t"
        "strb r6, [r0]\n\t"
        "sub r3, #1\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "cmp r3, r1\n\t"
        "bne 34b\n\t"
        "36:\n\t"
        "mov r2, ip\n\t"
        "ldr r0, [r2, #0x34]\n\t"
        "add r0, r0, r7\n\t"
        "str r0, [r2, #0x34]\n\t"
        "ldrh r0, [r2, #6]\n\t"
        "strh r0, [r2, #8]\n\t"
        "ldr r0, [r2, #0x30]\n\t"
        "add r0, #1\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "str r0, [r2, #0x30]\n\t"
        "mov r1, #1\n\t"
        "ldr r3, [sp, #0x14]\n\t"
        "lsl r1, r3\n\t"
        "ldr r4, [sp, #0x20]\n\t"
        "ldr r0, [r4]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r4]\n\t"
        "37:\n\t"
        "mov r5, #0xff\n\t"
        "mov r3, ip\n\t"
        "mov r2, sb\n\t"
        "mov r4, #3\n\t"
        "38:\n\t"
        "ldrb r7, [r2, #1]\n\t"
        "lsl r1, r7, #8\n\t"
        "ldrb r0, [r2]\n\t"
        "orr r1, r0\n\t"
        "add r0, r1, #0\n\t"
        "and r0, r5\n\t"
        "strb r0, [r3]\n\t"
        "lsr r1, r1, #8\n\t"
        "strb r1, [r3, #1]\n\t"
        "add r3, #2\n\t"
        "add r2, #2\n\t"
        "sub r4, #1\n\t"
        "cmp r4, #0\n\t"
        "bge 38b\n\t"
        "mov r1, ip\n\t"
        "ldrb r1, [r1]\n\t"
        "lsr r0, r1, #4\n\t"
        "ldr r2, [sp, #0x1c]\n\t"
        "cmp r0, r2\n\t"
        "bne 39f\n\t"
        "mov r2, #0xfc\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, sl\n\t"
        "mov r1, #1\n\t"
        "ldr r3, [sp, #0x14]\n\t"
        "lsl r1, r3\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "39:\n\t"
        "mov r0, #0\n\t"
        "mov r4, ip\n\t"
        "str r0, [r4, #0x2c]\n\t"
        "mov r5, #1\n\t"
        "str r5, [sp, #0x18]\n\t"
        "40:\n\t"
        "ldr r7, [sp, #0x24]\n\t"
        "str r7, [sp, #0x14]\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1, #0x1c]\n\t"
        "add r2, r7, #0\n\t"
        "cmp r2, r0\n\t"
        "bge 41f\n\t"
        "b 18b\n\t"
        "41:\n\t"
        "ldr r3, [sp, #0x18]\n\t"
        "cmp r3, #0\n\t"
        "bne 42f\n\t"
        "b 54f\n\t"
        "42:\n\t"
        "mov r4, #0\n\t"
        "str r4, [sp, #0x18]\n\t"
        "mov r2, #0xfc\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, sl\n\t"
        "mov r0, #0xfe\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, sl\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r1, r0\n\t"
        "beq 43f\n\t"
        "b 52f\n\t"
        "43:\n\t"
        "str r4, [r2]\n\t"
        "mov r5, sl\n\t"
        "add r5, #0x30\n\t"
        "mov r7, sl\n\t"
        "add r7, #0x40\n\t"
        "mov r0, #0xc4\n\t"
        "add r0, sl\n\t"
        "mov r8, r0\n\t"
        "mov r1, #0x32\n\t"
        "add r1, sl\n\t"
        "mov sb, r1\n\t"
        "mov r2, #0xc8\n\t"
        "add r2, sl\n\t"
        "mov ip, r2\n\t"
        "mov r3, sl\n\t"
        "add r3, #0x28\n\t"
        "add r2, r5, #0\n\t"
        "mov r6, #0xff\n\t"
        "mov r4, #3\n\t"
        "44:\n\t"
        "ldrb r0, [r2, #1]\n\t"
        "lsl r1, r0, #8\n\t"
        "ldrb r0, [r2]\n\t"
        "orr r1, r0\n\t"
        "add r0, r1, #0\n\t"
        "and r0, r6\n\t"
        "strb r0, [r3]\n\t"
        "lsr r1, r1, #8\n\t"
        "strb r1, [r3, #1]\n\t"
        "add r3, #2\n\t"
        "add r2, #2\n\t"
        "sub r4, #1\n\t"
        "cmp r4, #0\n\t"
        "bge 44b\n\t"
        "add r4, r7, #0\n\t"
        "mov r1, r8\n\t"
        "ldr r6, [r1]\n\t"
        "cmp r6, #4\n\t"
        "ble 45f\n\t"
        "mov r6, #4\n\t"
        "45:\n\t"
        "lsl r1, r6, #4\n\t"
        "mov r0, #0xf\n\t"
        "ldrb r2, [r5, #1]\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r5, #1]\n\t"
        "mov r3, sb\n\t"
        "mov r0, #0x80\n\t"
        "sub r0, r0, r6\n\t"
        "mov r5, ip\n\t"
        "ldr r1, [r5]\n\t"
        "cmp r1, r0\n\t"
        "bge 47f\n\t"
        "sub r2, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r2, r0\n\t"
        "beq 50f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x88\n\t"
        "add r5, r4, #4\n\t"
        "add r4, #0x84\n\t"
        "add r7, r0, #0\n\t"
        "46:\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r3]\n\t"
        "add r3, #1\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r4]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r4]\n\t"
        "sub r2, #1\n\t"
        "cmp r2, r7\n\t"
        "bne 46b\n\t"
        "b 50f\n\t"
        "47:\n\t"
        "sub r2, r6, #1\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r2, r0\n\t"
        "beq 50f\n\t"
        "add r5, r7, #0\n\t"
        "add r5, #0x88\n\t"
        "add r4, r7, #0\n\t"
        "add r4, #0x84\n\t"
        "add r7, #4\n\t"
        "mov r8, r0\n\t"
        "48:\n\t"
        "ldr r1, [r5]\n\t"
        "mov r0, #0\n\t"
        "cmp r1, #0x7f\n\t"
        "beq 49f\n\t"
        "add r0, r1, #1\n\t"
        "49:\n\t"
        "str r0, [r5]\n\t"
        "ldr r0, [r4]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r4]\n\t"
        "add r0, r7, r1\n\t"
        "ldrb r0, [r0]\n\t"
        "strb r0, [r3]\n\t"
        "add r3, #1\n\t"
        "sub r2, #1\n\t"
        "cmp r2, r8\n\t"
        "bne 48b\n\t"
        "50:\n\t"
        "mov r7, sl\n\t"
        "ldr r0, [r7, #0x24]\n\t"
        "add r0, r0, r6\n\t"
        "str r0, [r7, #0x24]\n\t"
        "mov r0, #0x10\n\t"
        "neg r0, r0\n\t"
        "ldr r1, [sp, #0x28]\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "ldr r2, [sp, #0x1c]\n\t"
        "orr r0, r2\n\t"
        "ldr r3, [sp, #0x28]\n\t"
        "strb r0, [r3]\n\t"
        "ldrh r1, [r7, #0x36]\n\t"
        "ldr r2, [sp, #0x28]\n\t"
        "mov r3, #4\n\t"
        "ldr r6, 55f\n\t"
        "mov r5, #0xff\n\t"
        "mov r4, #1\n\t"
        "neg r4, r4\n\t"
        "51:\n\t"
        "lsr r0, r1, #8\n\t"
        "ldrb r7, [r2]\n\t"
        "eor r0, r7\n\t"
        "and r0, r5\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r6\n\t"
        "lsl r1, r1, #8\n\t"
        "ldrh r0, [r0]\n\t"
        "eor r1, r0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "add r2, #1\n\t"
        "sub r3, #1\n\t"
        "cmp r3, r4\n\t"
        "bne 51b\n\t"
        "mov r0, #0\n\t"
        "mov r2, sl\n\t"
        "strh r1, [r2, #0x36]\n\t"
        "str r0, [r2, #0x3c]\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp, #0x18]\n\t"
        "52:\n\t"
        "mov r2, #0xfd\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, sl\n\t"
        "mov r0, #0xfe\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, sl\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r1, r0\n\t"
        "bne 53f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r2]\n\t"
        "mov r3, sl\n\t"
        "add r3, #0x30\n\t"
        "ldrb r2, [r3]\n\t"
        "lsr r1, r2, #4\n\t"
        "add r1, #1\n\t"
        "mov r0, #0xf\n\t"
        "and r1, r0\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r0, #0xf\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "mov r4, #1\n\t"
        "str r4, [sp, #0x18]\n\t"
        "53:\n\t"
        "ldr r5, [sp, #0x18]\n\t"
        "cmp r5, #0\n\t"
        "beq 54f\n\t"
        "mov r0, #0\n\t"
        "mov r7, sl\n\t"
        "str r0, [r7, #0x38]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r7, #0x18]\n\t"
        "mov r2, sl\n\t"
        "add r2, #0x30\n\t"
        "ldrb r3, [r2]\n\t"
        "lsr r1, r3, #4\n\t"
        "ldrb r4, [r2, #7]\n\t"
        "lsl r0, r4, #8\n\t"
        "ldrb r5, [r2, #6]\n\t"
        "orr r0, r5\n\t"
        "add r1, r1, r0\n\t"
        "mov r0, #0xf\n\t"
        "and r1, r0\n\t"
        "sub r0, #0x1f\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "54:\n\t"
        "mov r7, sl\n\t"
        "ldr r0, [r7, #0x3c]\n\t"
        "mov r1, #3\n\t"
        "and r0, r1\n\t"
        "cmp r0, #3\n\t"
        "bne 56f\n\t"
        "ldr r0, [r7, #0x38]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, sl\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x28\n\t"
        "add r0, #0x29\n\t"
        "b 57f\n\t"
        ".align 2, 0\n"
        "55: .4byte gStaticData_0816AF10\n"
        "56:\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1, #0x38]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, sl\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x30\n\t"
        "add r0, #0x31\n\t"
        "57:\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldrb r1, [r1]\n\t"
        "orr r0, r1\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #3\n\t"
        "add r1, sl\n\t"
        "strh r0, [r1]\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2, #0x38]\n\t"
        "add r0, #1\n\t"
        "str r0, [r2, #0x38]\n\t"
        "cmp r0, #4\n\t"
        "bne 58f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r2, #0x38]\n\t"
        "ldr r0, [r2, #0x3c]\n\t"
        "add r0, #1\n\t"
        "str r0, [r2, #0x3c]\n\t"
        "58:\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, sl\n\t"
        "ldrh r1, [r0]\n\t"
        "ldr r0, 60f\n\t"
        "strh r1, [r0]\n\t"
        "mov r0, #0\n\t"
        "mov r3, sl\n\t"
        "strb r0, [r3, #4]\n\t"
        "59:\n\t"
        "add sp, #0x2c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "60: .4byte 0x0400012A\n"
    );
}
#endif

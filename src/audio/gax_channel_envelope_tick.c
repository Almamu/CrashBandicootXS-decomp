#include "core.h"

/* Per-tick per-channel envelope/portamento update: if a voice entry is
 * still bound (`self->0x3c`), advances the pattern-note scheduler
 * (`sub_8039F30`, storing the returned pitch offset at `self->0x16`),
 * then the effect-table tick (`sub_8039FFC`) and a third still-raw
 * per-tick step (`sub_803A03C`). Independently of that, ramps two 8-bit
 * envelope fields (`self->0x15`/`self->0x17`) by their own signed 16-bit
 * per-tick deltas (`self->0x1a`/`self->0x1c`), clamped to 0-0xff, then
 * advances two 16-bit accumulators (`self->0x26`/`self->0x2a`) by their
 * own per-tick step fields (`self->0x28`/`self->0x2c`) - a portamento
 * pitch slide. If a slide rate (`self->0x32`) is armed, applies it to
 * `self->0x26` as well, but only keeps the result if doing so didn't
 * change which side of the target (`self->0x30`) it's on relative to
 * before the step (a standard "don't overshoot" portamento-arrival
 * check) - overshooting snaps `self->0x26` to the target and disarms the
 * slide. Channel/voice object shape not modeled yet - kept as raw
 * offsets, same as the other GAX2 engine internals in this directory.
 *
 * Written as NAKED asm, not plain C: a 99.7%-matching C reconstruction
 * is kept below under `#if NON_MATCHING` - see
 * docs/matching/naked-sub_8039aa4-matched.md for the derivation and the
 * two-instruction residual (a pair of `ldrsh`-with-register-offset
 * reads in the portamento tail whose own register pins independently
 * reconfirmed the original parking finding: pinning either one ripples
 * backward and perturbs the earlier, already-correct clamp blocks'
 * register choices - gcc-2.9's hard-register variable reservations
 * aren't scoped as tightly as their C block, so this looks like a
 * genuine compiler limitation, not a phrasing gap). Mechanical, not an
 * inferred control-flow guess. */
#if NON_MATCHING
/* NOT YET BYTE-MATCHING - 99.7% instruction match (two small,
 * isolated register-choice residuals left - see the doc comment above
 * and docs/matching/naked-sub_8039aa4-matched.md); compiled only under
 * `make NON_MATCHING=1`, the NAKED version below is used otherwise. */
extern u8 sub_8039F30(void *self, void *table, u16 *out);
extern void sub_8039FFC(void *self);
extern void sub_803A03C(void *self);

void sub_8039AA4(void *self)
{
    register u8 *s asm("r4") = (u8 *)self;
    register s32 zero asm("r6");
    void *voice;
    s32 sum1, sum2, sum26;

    voice = *(void **)(s + 0x3c);
    if (voice != NULL) {
        void *table = *(void **)((u8 *)voice + 0x7c);
        s[0x16] = sub_8039F30(s, table, (u16 *)(s + 0x38));
        sub_8039FFC(s);
        sub_803A03C(s);
    }

    sum1 = *(s16 *)(s + 0x1a) + s[0x15];
    if (sum1 > 0xff) {
        sum1 = 0xff;
    }
    if (sum1 < 0) {
        sum1 = 0;
    }
    zero = 0;
    s[0x15] = sum1;

    sum2 = *(s16 *)(s + 0x1c) + s[0x17];
    if (sum2 > 0xff) {
        sum2 = 0xff;
    }
    if (sum2 < 0) {
        sum2 = 0;
    }
    s[0x17] = sum2;

    { register s32 v28 asm("r0") = *(u16 *)(s + 0x28);
      register s32 v26 asm("r1") = *(u16 *)(s + 0x26);
      sum26 = v28 + v26; }
    *(u16 *)(s + 0x26) = sum26;
    { register s32 v2c asm("r0") = *(u16 *)(s + 0x2c);
      register s32 v2a asm("r2") = *(u16 *)(s + 0x2a);
      *(u16 *)(s + 0x2a) = v2c + v2a; }

    { register s32 slideRate asm("r1") = *(u16 *)(s + 0x32);
    if ((s16)slideRate != 0) {
        s32 diffBefore = (*(s16 *)(s + 0x30) - *(s16 *)(s + 0x26)) & 0x80000000;
        { register s32 newVal asm("r0");
          asm volatile("add %0, %1, %2" : "=r"(newVal) : "r"(sum26), "r"(slideRate));
          *(u16 *)(s + 0x26) = newVal; }
        {
            s32 diffAfter = (*(s16 *)(s + 0x30) - *(s16 *)(s + 0x26)) & 0x80000000;
            if (diffBefore != diffAfter) {
                *(u16 *)(s + 0x32) = zero;
                *(u16 *)(s + 0x26) = *(u16 *)(s + 0x30);
                *(u16 *)(s + 0x30) = zero;
            }
        }
    } }
}
#else /* !NON_MATCHING */
NAKED void sub_8039AA4(void *self)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x3c]\n\t"
        "cmp r0, #0\n\t"
        "beq L9AA4_0\n\t"
        "ldr r1, [r0, #0x7c]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x38\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8039F30\n\t"
        "strb r0, [r4, #0x16]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8039FFC\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803A03C\n\t"
        "L9AA4_0:\n\t"
        "movs r1, #0x1a\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "ldrb r2, [r4, #0x15]\n\t"
        "add r0, r0, r2\n\t"
        "cmp r0, #0xff\n\t"
        "ble L9AA4_1\n\t"
        "movs r0, #0xff\n\t"
        "L9AA4_1:\n\t"
        "cmp r0, #0\n\t"
        "bge L9AA4_2\n\t"
        "movs r0, #0\n\t"
        "L9AA4_2:\n\t"
        "movs r6, #0\n\t"
        "strb r0, [r4, #0x15]\n\t"
        "movs r3, #0x1c\n\t"
        "ldrsh r0, [r4, r3]\n\t"
        "ldrb r5, [r4, #0x17]\n\t"
        "add r0, r0, r5\n\t"
        "cmp r0, #0xff\n\t"
        "ble L9AA4_3\n\t"
        "movs r0, #0xff\n\t"
        "L9AA4_3:\n\t"
        "cmp r0, #0\n\t"
        "bge L9AA4_4\n\t"
        "movs r0, #0\n\t"
        "L9AA4_4:\n\t"
        "strb r0, [r4, #0x17]\n\t"
        "ldrh r0, [r4, #0x28]\n\t"
        "ldrh r1, [r4, #0x26]\n\t"
        "add r5, r0, r1\n\t"
        "strh r5, [r4, #0x26]\n\t"
        "ldrh r0, [r4, #0x2c]\n\t"
        "ldrh r2, [r4, #0x2a]\n\t"
        "add r0, r0, r2\n\t"
        "strh r0, [r4, #0x2a]\n\t"
        "ldrh r1, [r4, #0x32]\n\t"
        "movs r3, #0x32\n\t"
        "ldrsh r0, [r4, r3]\n\t"
        "cmp r0, #0\n\t"
        "beq L9AA4_5\n\t"
        "movs r0, #0x30\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "movs r3, #0x26\n\t"
        "ldrsh r0, [r4, r3]\n\t"
        "sub r2, r2, r0\n\t"
        "movs r3, #0x80\n\t"
        "lsl r3, r3, #0x18\n\t"
        "and r2, r3\n\t"
        "add r0, r5, r1\n\t"
        "strh r0, [r4, #0x26]\n\t"
        "movs r5, #0x30\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "movs r5, #0x26\n\t"
        "ldrsh r1, [r4, r5]\n\t"
        "sub r0, r0, r1\n\t"
        "and r0, r3\n\t"
        "cmp r2, r0\n\t"
        "beq L9AA4_5\n\t"
        "strh r6, [r4, #0x32]\n\t"
        "ldrh r0, [r4, #0x30]\n\t"
        "strh r0, [r4, #0x26]\n\t"
        "strh r6, [r4, #0x30]\n\t"
        "L9AA4_5:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    );
}
#endif /* NON_MATCHING */

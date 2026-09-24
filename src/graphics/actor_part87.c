#include "core.h"
#include "gba/io_reg.h"
#include "graphics_package.h"
#include "icon_manager.h"
#include "vram_pool.h"

/* GitHub issue #63's final remaining raw span, right after the
 * three-kind `InitActorPart` cluster (`actor_part63.c`-`actor_part73.c`)
 * - see docs/matching/issue-63-0x08033ef4-actor.md. This "self" object
 * is *not* part of that `InitActorPart` family (no `InitActorPart` call,
 * and its field layout doesn't match): it's a standalone fade/overlay
 * controller that owns three small BG scratch buffers plus a combined
 * BLDCNT/BLDALPHA mirror, driving a full-screen alpha-blend effect. */
struct fade_overlay {
    u8 *bg1Buf;   /* 0x00 - LoadGraphicsPackage "self" scratch for BG1 */
    u8 *bg0Buf;   /* 0x04 - ditto for BG0 */
    u8 *bg2Buf;   /* 0x08 - ditto for BG2 */
    u16 dispcnt;  /* 0x0c - written as one halfword to REG_DISPCNT; bytes
                   * accessed individually below via DISPCNT_LO/DISPCNT_HI */
    u8 unused_0e[2];
    union {
        u32 word;    /* 0x10 - written as one word to REG_BLDCNT/BLDALPHA */
        struct {
            u8 bldcntLo;
            u8 bldcntHi;
            u8 bldalphaLo;
            u8 bldalphaHi;
        } b;
    } blend;
    u8 unused_14[4]; /* 0x14 - never referenced by this file's functions */
    struct icon_manager *icons; /* 0x18 */
    u32 unused_1c;
    u32 flag_20; /* 0x20 - which of two alternating cue sfx last fired */
};

extern void *sub_8026EDC(s32 size);
extern void sub_801E644(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg5);
extern u16 sub_801E640(u8 *self);
extern void LoadGraphicsPackage(u8 *selfArg, struct bg_package *pkgArg);
extern u8 gStaticData_0817C5BC[];
extern u8 gStaticData_0817C594[];
extern u8 gStaticData_0817C5A8[];
extern struct AudioContext *gUnknown_030012BC;
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern struct fade_overlay *sub_803487C(struct fade_overlay *self);

/* Allocates and initializes the fade overlay's three BG scratch buffers
 * (BG1 priority 3/bgcnt 0x1e, BG0 bgcnt 0x1f/slot 3, BG2 priority
 * 1/bgcnt 0x1d/slot 2 - see `sub_801E644`), loads their graphics
 * packages, clears palette entry 0, builds a DISPCNT value enabling
 * BG0/BG1/BG2 in BG2-priority-preserving mode 0 (dropping the "forced
 * blank" bit), hands off to `sub_803487C` for the HUD/level-object
 * setup half, then builds a fixed BLDCNT/BLDALPHA alpha-blend value
 * (BG2 -> BG0, mode 1, EVA=8/16 EVB=16/16), applies every register,
 * zeroes two more fields, and finally ducks the audio context out via
 * `sub_8001AC4`. Returns `self`.
 *
 * Matched, byte-exact real C. The remaining BLDCNT/BLDALPHA byte-packing
 * gap this function was previously parked over (see
 * docs/matching/issue-63-final-raw-actor.md and
 * docs/matching/issue-30-graphics-loading.md's LoadGraphicsPackage entry
 * for the established "handful of register choices" pattern this hit
 * too) turned out to be two narrow, closable spots, not a genuine
 * compiler limitation:
 *  - `one |= self->blend.b.bldcntHi;`'s reload was left to this
 *    compiler's free register choice (it picked r0), where the ROM
 *    reuses r1 (the register `mask` occupies a few lines later) -
 *    closed with a `register u8 hi asm("r1")` pin on just that reload.
 *  - The `mask`/`tmp` BLDALPHA pair: this compiler elides the `tmp =
 *    mask` copy entirely and ANDs the freshly-loaded byte directly
 *    against `mask`'s own register instead (2 bytes shorter than the
 *    ROM's real "copy mask into tmp, then AND against a separately
 *    reloaded byte" sequence) - closed with an empty `asm("" :
 *    "+r"(tmp))` compiler barrier right after `tmp`'s initializer,
 *    the same "stop the materialize-then-copy fold" technique
 *    documented for `sub_801E8F8`/`sub_801E96C`
 *    (docs/matching/issue-30-graphics-loading.md). Both are ordinary
 *    register-pin/barrier fixes, not opaque `asm volatile` islands -
 *    the "handful of accumulator/temp-register choices" this function
 *    was previously parked over. The real bytes used to live in
 *    `asm/code_3_2_20_28568_c99c_31784_33ef4_3472c.s` under a
 *    `.if NON_MATCHING == 0` guard; that block is now removed since
 *    this function always compiles to the ROM's exact bytes. */
void *sub_803472C(void *selfArg)
{
    /* Register-pinned to match the ROM's own allocation
     * (matching_decomp_register_pinning memory): `self` occupies r5 for
     * the whole function (it's still needed for the final return value),
     * which leaves no free low register for the two constants (`0`,
     * `0x40`) that stay live across the `sub_803487C` call below, so the
     * ROM's own build pushes them into r8/sb instead - reproduced here
     * the same way. */
    register struct fade_overlay *self asm("r5") = selfArg;
    register u8 *buf asm("r0");

    buf = sub_8026EDC(0x10);
    sub_801E644(buf, 0, 0x1f, 0, 3);
    self->bg0Buf = buf;

    buf = sub_8026EDC(0x10);
    sub_801E644(buf, 3, 0x1e, 0, 1);
    self->bg1Buf = buf;

    buf = sub_8026EDC(0x10);
    sub_801E644(buf, 2, 0x1d, 1, 2);
    self->bg2Buf = buf;

    buf = self->bg1Buf;
    LoadGraphicsPackage(buf, (struct bg_package *)gStaticData_0817C5BC);
    LoadGraphicsPackage(self->bg0Buf, (struct bg_package *)gStaticData_0817C594);
    LoadGraphicsPackage(self->bg2Buf, (struct bg_package *)gStaticData_0817C5A8);

    *(vu16 *)PLTT = 0;

    {
        /* Each constant is declared right where the ROM first
         * materializes it (matching_decomp_register_pinning memory) -
         * `zero`/`c0x40` land in r8/sb since `self` (r5) and `buf`'s
         * former home (r0) leave no free low register for either, and
         * `one`/`four` (r6/r4) stay resident across the `sub_803487C`
         * call below and get reused for the BLDCNT bit-2/bit-0 sets
         * afterward instead of being re-materialized as fresh
         * immediates. */
        register u32 zero asm("r8") = 0;

        /* Inline-asm anchor (docs/workflow.md step 3): the ROM's own
         * build never takes the shortcut of reusing the low-register
         * copy still sitting around from materializing `zero` into r8
         * above - it always re-derives a fresh low-register copy from r8
         * itself, here and at every later use. A plain `self->dispcnt =
         * zero;` compiles one instruction shorter (this compiler *does*
         * take that shortcut), so this one store is transcribed
         * directly. */
        asm volatile("mov r1, r8\n\tstrh r1, [r5, #0xc]" ::: "r1");
        {
            /* ROM materializes 0x40 via r2 (not the just-freed r0) before
             * copying it up into sb - another instance of the same
             * "never take the free-register shortcut" pattern above. */
            register u32 c0x40 asm("sb");
            asm volatile("mov r2, #0x40\n\tmov sb, r2" : "=r" (c0x40) :: "r2");

            /* Same anchor reason as above, for the ROM's `mov r0, sb`
             * hop plus the negative-constant bit-clear idiom
             * (matching_decomp_register_pinning memory) clearing
             * DISPCNT's low 3 (mode) bits. */
            asm volatile(
                "mov r0, sb\n\t"
                "ldrb r1, [r5, #0xc]\n\t"
                "orr r0, r1\n\t"
                "movs r1, #8\n\t"
                "neg r1, r1\n\t"
                "and r0, r1\n\t"
                "strb r0, [r5, #0xc]"
                ::: "r0", "r1"
            );
            {
                register u32 one asm("r6") = 1;

                ((u8 *)&self->dispcnt)[1] |= one;
                ((u8 *)&self->dispcnt)[1] |= 2;
                {
                    register u32 four asm("r4") = 4;

                    ((u8 *)&self->dispcnt)[1] |= four;

                    sub_803487C(self);

                    /* Each of these zero-stores gets its own fresh
                     * low-register copy-down from r8 (matching_decomp_
                     * register_pinning memory) rather than one kept alive
                     * across the `sub_801E640` calls below - the ROM's
                     * own build re-derives it every time except where two
                     * stores sit back-to-back with no call between them
                     * (BG2HOFS and the two field zeros at the very end,
                     * which really do share one copy in r2). */
                    {
                        register u32 z2 asm("r2") = zero;
                        self->blend.word = z2;
                    }
                    four |= self->blend.b.bldcntLo;
                    {
                        /* Reload pinned to r1 (rather than the freely-
                         * chosen scratch register this compiler otherwise
                         * picks) so it lands in the same register the
                         * ROM's own build reuses right below - see the
                         * doc comment above. */
                        register u8 hi asm("r1") = self->blend.b.bldcntHi;
                        one |= hi;
                    }
                    self->blend.b.bldcntHi = one;
                    {
                        register s32 mask asm("r1") = -0x20;
                        register s32 tmp asm("r0") = mask;

                        /* Compiler barrier (matching_decomp_register_
                         * pinning memory): without it, this compiler
                         * elides the `tmp = mask` copy entirely and ANDs
                         * the freshly-loaded byte directly against
                         * `mask`'s own register instead, 2 bytes shorter
                         * than the ROM's real "copy mask into tmp, then
                         * AND against a separately reloaded byte"
                         * sequence. */
                        asm("" : "+r" (tmp));
                        tmp &= self->blend.b.bldalphaLo;
                        tmp |= 8;
                        self->blend.b.bldalphaLo = tmp;
                        mask &= self->blend.b.bldalphaHi;
                        mask |= 0x10;
                        self->blend.b.bldalphaHi = mask;
                    }
                    four &= 0x3f;
                    /* Inline-asm anchor (docs/workflow.md step 3): a
                     * plain `four |= c0x40;` picks r0 for the low-reg
                     * copy-down of `c0x40` here, where the ROM's own
                     * build picks r1. */
                    asm volatile(
                        "mov r1, sb\n\t"
                        "orr r4, r1\n\t"
                        "strb r4, [r5, #0x10]"
                        :: "r" (c0x40), "r" (four) : "r1"
                    );

                    REG_BG0CNT = sub_801E640(self->bg0Buf);
                    {
                        vu32 *addr = (vu32 *)REG_ADDR_BG0HOFS;
                        register u32 z2 asm("r2") = zero;
                        *addr = z2;
                    }
                    REG_BG1CNT = sub_801E640(self->bg1Buf);
                    {
                        vu32 *addr = (vu32 *)REG_ADDR_BG1HOFS;
                        register u32 z1 asm("r1") = zero;
                        *addr = z1;
                    }
                    REG_BG2CNT = sub_801E640(self->bg2Buf);
                    {
                        vu32 *addr = (vu32 *)REG_ADDR_BG2HOFS;
                        register u32 z2 asm("r2") = zero;

                        *addr = z2;

                        *(vu16 *)REG_ADDR_DISPCNT = self->dispcnt;
                        *(vu32 *)REG_ADDR_BLDCNT = self->blend.word;
                        self->unused_1c = z2;
                        self->flag_20 = z2;
                    }
                }
            }
        }
    }

    sub_8001AC4(gUnknown_030012BC, 0);

    return self;
}

asm(".align 2, 0");

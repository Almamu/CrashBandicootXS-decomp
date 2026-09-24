#include "core.h"
#include "actor.h"

extern void *gUnknown_030012D8;
extern void *gUnknown_030012D4;
extern void *gUnknown_030012C8;
extern void *gUnknown_03001300;
extern void *gUnknown_03001308;
extern void *gUnknown_03001318;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F4;
extern void *gUnknown_030012F8;
extern void *gUnknown_030012EC;
extern void *gUnknown_0300130C;
extern u8 gUnknown_03001280[4];
extern struct tile_asset_cache *gUnknown_030012B8;

extern void sub_8006DC8(struct tile_asset_cache *self);
extern void sub_8026E6C(void *self);
extern void sub_802692C(void *self);
extern void sub_8026F54(void *self);
extern void sub_80274EC(void *self);
extern void sub_8008DC0(struct dual_array_manager *manager);
extern void *sub_803AD7C(void *arg0, void *arg1);
extern void sub_800944C(void *managerArg);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_80268F8(void *self);
extern void FlushVramDmaQueue(void);

/* Runs the DMA3/`sub_8006DC8`+`sub_8026984` refresh pass over every
 * currently-active dual-array manager, then flushes the VRAM DMA
 * queue - only while `self->0x0` is still within the "near start of
 * level" range (`<= 0x1000`), otherwise this is a no-op. */
void sub_802400C(void *self)
{
    sub_8006DC8(gUnknown_030012B8);
    sub_8026E6C(gUnknown_030012D4);
    sub_802692C(gUnknown_03001308);
    sub_8026F54(gUnknown_030012C8);

    if (*(s32 *)self <= 0x1000) {
        sub_80274EC(gUnknown_03001318);
        sub_8008DC0(gUnknown_030012F4);

        {
            register struct actor *p asm("r0") = (struct actor *)gUnknown_030012D8;
            void *tbl = p->table;
            if ((u8)(s32)sub_803AD7C((u8 *)p + *(s16 *)((u8 *)tbl + 0x28),
                                      *(void **)((u8 *)tbl + 0x2c)) != 0) {
                register struct actor *p2 asm("r0") = (struct actor *)gUnknown_030012D8;
                void *tbl2 = p2->table;
                sub_803AD7C((u8 *)p2 + *(s16 *)((u8 *)tbl2 + 0x20),
                            *(void **)((u8 *)tbl2 + 0x24));
            }
        }

        sub_8008DC0(gUnknown_030012F0);
        sub_8008DC0(gUnknown_030012EC);
        sub_800944C(gUnknown_0300130C);
        sub_8008DC0(gUnknown_030012F8);

        sub_8006A48(gUnknown_03001300);
        sub_80006A8();
        sub_8006AAC(gUnknown_03001300);
        sub_80268F8(gUnknown_03001308);
        FlushVramDmaQueue();
    }
}

/* Rebuilds the `gUnknown_03001280` `REG_BLDCNT`/`REG_BLDALPHA` shadow
 * word (see `src/graphics/aabb_util.c`'s `sub_8001624`, which commits
 * this same shadow to hardware) from `self->0x18`'s (a level object)
 * raster-mode fields, and sets `gUnknown_03001308`'s `+0x2b` flag when
 * that level's mode is 1. When the level has no raster mode at all
 * (`+0x10` halfword is 0), the blend word instead gets a fixed
 * "disabled" pattern.
 *
 * Matched as NAKED, transcribed instruction-for-instruction from the
 * ROM disassembly, rather than plain C or an ordinary `asm volatile`
 * island inside a normal C function: every field/mask/shift/branch in
 * the dense byte-level bitfield packing below (~40 AND/OR/shift/mask
 * instructions) is semantically straightforward, but this compiler's
 * natural register allocation for it picks a different register for
 * nearly every intermediate than the ROM throughout (which keeps
 * `self->0x18` in r4, the shadow-word base in r6, and threads
 * mask/value pairs through r0-r3/r7 in a specific reused order) -
 * plausible but impractical to hand-pin individually. Wrapping the
 * whole sequence in one `asm volatile` block inside an ordinary
 * (non-NAKED) function - the technique `AllocVramTileBlock`
 * (`src/graphics/sprite_frame_queue.c`) and `sub_802AB58`
 * (`src/graphics/actor_part53.c`) established for this same kind of
 * "impractical to hand-pin" case - gets every instruction byte-exact
 * except the prologue/epilogue: the ROM's `push {r4, r5, r6, r7, lr}` /
 * `pop {r4, r5, r6, r7}` needs r7 callee-saved too, but r7 only ever
 * appears in the asm block's clobber list (nothing here gives it a
 * live C-level value), and this compiler's auto-generated prologue
 * only saves callee-saved registers it can see a live use for - it
 * silently drops r7 from the push/pop list even though the asm clobber
 * list names it (confirmed with a minimal isolated-compile repro:
 * `push {r4, r5, r6, lr}` / `pop {r4, r5, r6}`, r7 missing from both).
 * This is the same "gcc-2.9 r7-pin bug" already documented project-wide
 * (e.g. `src/graphics/oam_count.c`'s `sub_8006600`,
 * `src/graphics/graphics_loading_21280.c`'s `sub_8021280`) - an
 * explicit `register T x asm("r7")` pin doesn't reliably survive either
 * - so this function uses that same project-wide escape hatch instead:
 * NAKED with the compiler-generated prologue/epilogue replaced by a
 * literal, hand-written `push`/`pop` covering all four registers in one
 * instruction, matching the ROM exactly. The ROM's suffixed Thumb
 * mnemonics (`movs`/`ands`/`orrs`/`lsls`/`strb`/`ldrb`) are written in
 * their suffix-less forms here (`mov`/`and`/`orr`/`lsl`/`strb`/`ldrb`),
 * which `arm-none-eabi-as` accepts identically in this project's
 * Thumb16 mode; `rsbs r1, r1, #0` becomes `neg r1, r1`, since a
 * suffixed `rsbs`/`rsb` is rejected here ("cannot honor width suffix")
 * but `neg` assembles to the identical encoding. `.pool` right after
 * the `if`-branch's trailing `b 3f` forces the `gUnknown_03001280`/
 * `gUnknown_03001308` literals (loaded via the assembler's own
 * `=symbol` syntax) to group in the same ROM-matching mid-function gap
 * the ROM's own `.align 2, 0` + two `.4byte` entries occupy, right
 * before the `else`-branch, instead of at the function's end. */
NAKED void sub_80240E4(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r2, r0, #0\n\t"
        "ldr r6, =gUnknown_03001280\n\t"
        "mov r0, #0\n\t"
        "str r0, [r6]\n\t"
        "ldr r3, =gUnknown_03001308\n\t"
        "ldr r1, [r3]\n\t"
        "add r1, r1, #0x2b\n\t"
        "strb r0, [r1]\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "ldrh r0, [r1, #0x10]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "ldr r1, [r1, #8]\n\t"
        "cmp r1, #1\n\t"
        "bne 1f\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, r0, #0x2b\n\t"
        "strb r1, [r0]\n\t"
        "1:\n\t"
        "ldr r4, [r2, #0x18]\n\t"
        "ldrb r1, [r4, #0x10]\n\t"
        "lsl r0, r1, #6\n\t"
        "mov r2, #0x3f\n\t"
        "ldrb r3, [r6]\n\t"
        "and r2, r3\n\t"
        "orr r2, r0\n\t"
        "strb r2, [r6]\n\t"
        "mov r3, #0x1f\n\t"
        "ldrb r5, [r4, #0x12]\n\t"
        "and r5, r3\n\t"
        "mov r1, #0x20\n\t"
        "neg r1, r1\n\t"
        "add r0, r1, #0\n\t"
        "ldrb r7, [r6, #2]\n\t"
        "and r0, r7\n\t"
        "orr r0, r5\n\t"
        "strb r0, [r6, #2]\n\t"
        "ldrb r4, [r4, #0x13]\n\t"
        "and r3, r4\n\t"
        "ldrb r0, [r6, #3]\n\t"
        "and r1, r0\n\t"
        "orr r1, r3\n\t"
        "strb r1, [r6, #3]\n\t"
        "mov r0, #8\n\t"
        "orr r2, r0\n\t"
        "strb r2, [r6]\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r6, #1]\n\t"
        "orr r0, r1\n\t"
        "mov r1, #2\n\t"
        "orr r0, r1\n\t"
        "mov r1, #4\n\t"
        "orr r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "orr r0, r1\n\t"
        "b 3f\n\t"
        ".pool\n\t"
        "2:\n\t"
        "mov r2, #0x3f\n\t"
        "ldrb r3, [r6]\n\t"
        "and r2, r3\n\t"
        "mov r0, #0x20\n\t"
        "neg r0, r0\n\t"
        "add r1, r0, #0\n\t"
        "ldrb r7, [r6, #2]\n\t"
        "and r1, r7\n\t"
        "mov r3, #0x10\n\t"
        "orr r1, r3\n\t"
        "strb r1, [r6, #2]\n\t"
        "ldrb r1, [r6, #3]\n\t"
        "and r0, r1\n\t"
        "orr r0, r3\n\t"
        "strb r0, [r6, #3]\n\t"
        "mov r0, #8\n\t"
        "orr r2, r0\n\t"
        "strb r2, [r6]\n\t"
        "mov r0, #1\n\t"
        "ldrb r7, [r6, #1]\n\t"
        "orr r0, r7\n\t"
        "mov r1, #2\n\t"
        "orr r0, r1\n\t"
        "mov r1, #4\n\t"
        "orr r0, r1\n\t"
        "orr r0, r3\n\t"
        "3:\n\t"
        "strb r0, [r6, #1]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

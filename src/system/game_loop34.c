#include "core.h"
#include "actor.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010914`/
 * `sub_801095C`/`sub_80109A4` right after this function are matched in
 * game_loop30.c. */

extern void *gUnknown_030012BC;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012E4;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_802599C(void *self, s32 n);
extern void sub_80259D4(void *self, s32 n);
/* sub_8025A64 is parked (NON_MATCHING) as of game_loop14.c. This call
 * site's own arguments are spelled out entirely in inline asm below -
 * see the comment right above that block for why. */
extern struct actor *sub_8025A64(void *unused0, s32 x, s32 y, u8 p3, u32 p5, u8 flag6);

/* Plays cue-3 SFX, then - unless `self->field_08` is the sentinel
 * `0xffff` - consumes a slot from the per-record bit-grid
 * (`gUnknown_030012B4`, the same `sub_802599C`/`sub_80259D4` accessor
 * pair game_loop12.c/game_loop13.c already establish) keyed by
 * `self->field_08`, setting the bit only if it wasn't already set.
 * Finally spawns a part object (`sub_8025A64`) three tiles below
 * `self`'s own position, tagged with the caller's own byte argument. */
void sub_801089C(struct actor *self, u32 arg1)
{
    u8 flag6 = (u8)arg1;

    PlaySfx(gUnknown_030012BC, 3, 0x100);

    if (self->field_08 != 0xFFFF) {
        if ((u8)sub_802599C(gUnknown_030012B4, self->field_08) == 0) {
            sub_80259D4(gUnknown_030012B4, self->field_08);
        }
    }

    /* The ROM stores this call's stack-passed 6th argument (`flag6`, a
     * plain u8) through a computed `add r3, sp, #4`/`strb r6, [r3]`
     * pair - Thumb1 has no sp-relative byte-store encoding, so the
     * byte has to go through a register base - whereas this compiler
     * always emits a direct word-sized `str` for a stack argument
     * regardless of the parameter's declared width (same gap already
     * closed for `sub_8003A60`'s own `sub_8003F30` call in
     * settings_menu8c.c - see docs/matching/issue-5-overlay-ui-sync.md).
     * The whole call is spelled out in asm to match; a dummy 2-word
     * local's address is taken as an unused input operand purely to
     * make this compiler reserve the same 8-byte outgoing-argument
     * stack slot pair (5th arg `p5`'s word, 6th arg `flag6`'s byte)
     * the ROM's own `sub sp, #8`/`add sp, #8` frame does. */
    {
        u32 dummy[2];
        asm volatile(
            "ldr r1, [%0]\n"
            "asr r1, r1, #8\n"
            "ldr r2, [%0, #4]\n"
            "asr r2, r2, #8\n"
            "add r2, r2, #3\n"
            "ldr r0, =gUnknown_030012E4\n"
            "ldr r0, [r0]\n"
            "mov r3, #3\n"
            "str r3, [sp]\n"
            "add r3, sp, #4\n"
            "strb %1, [r3]\n"
            "mov r3, #0\n"
            "bl sub_8025A64\n"
            :
            : "r" (self), "r" (flag6), "r" (dummy)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );
    }
}

#include "core.h"

/* GitHub issue #9/#10: `sub_800C40C` and `sub_800C5D4`, the last two of
 * the four `self+0x68`-dispatching siblings flagged in
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md - `sub_800C40C` is
 * the specific function that doc's own Phase 1 pass already flagged as
 * "the exact function docs/rom_map.md ties to sharing sub_800B8DC's own
 * self+0x68 field" (called from sub_800B8DC's states 4, 13, 14, 16);
 * `sub_800C5D4` is called from state 3 (and 13's fallthrough).
 *
 * `sub_800C40C`: a 6-case dispatcher (modes 0, 3, 4, 5; anything else,
 * including 1/2, is a no-op). Modes 0 and 4 share an "impact
 * distance" gate - `sub_803AE4C` division/remainder-style scalar check
 * against a `gUnknown_0300082C`-relative table lookup indexed by
 * `self->0x30`/`self->0x34`/`self->0x38` (the same "close enough"
 * primitive `docs/rom_map.md` already ties to hud_counter.c/
 * hud_stat_widget3.c) - only proceeding when the check passes, then
 * consulting `self->0x84`'s pointed record (`+0xc` for mode 0, `+0x14`
 * for mode 4) against a constant `8` to pick between two
 * `sub_800C8CC` trigger constants. Mode 0 additionally clears bit 3 of
 * `owner->0xd` when `self->0x6c` is `0xf`/`0x12`/`0x1a` and re-triggers
 * `sub_800C8BC(self,0)`, or when `self->0x6c` is `0x12`/`0x1a`. Mode 3
 * is the largest case: if `owner->0x38` is set, triggers
 * `sub_800C8CC(self,4)`, then on `self->0x6c` `0x12`/`0x1a` sets bit 3
 * of `owner->0xd` and plays SFX `0x26`, or on `self->0x6c == 0xf` plays
 * SFX `9`; then unconditionally, if `self->0x6c == 0x17` and
 * `owner->0x30 == 9` and `owner->0x34 == 0`, spawns a part via
 * `sub_8025B0C(gUnknown_030012E4, 0x17, 4, -0x2d, 2, owner)` (the same
 * `sub_8025B0C(pool, kind, ..., z, src)` shape `game_loop14.c`
 * documents), tags the new part's `+0xc`/`+0xa` fields, and plays SFX
 * `0x1e`. Mode 5 mirrors mode 3's `owner->0x38` gate but triggers
 * `sub_800C8CC(self,0)` and, only for `self->0x6c==0xf`, additionally
 * `sub_800C8BC(self,1)`; a shared tail (also reached directly when
 * `owner->0x38` was clear) then plays SFX `0x23` when `self->0x6c==0xf`
 * and `owner->0x30==8` and `owner->0x34==0`.
 *
 * `sub_800C5D4`: a 3-case dispatcher (modes 0, 2; anything else falls
 * to a shared tail). Unconditional prelude: if `self->0x6c==0xb` and
 * `owner->4 < self->0x64`, latches `owner->4 = self->0x64` and fires
 * `sub_800C8AC(self,0)`. Mode 0 builds an AABB at `owner`'s position
 * offset by `self->0x20`/`self->0x24` sized by `self->0x28-0x20`/
 * `self->0x2c-0x24` (via `sub_803AFE4`/`sub_803AFDC`, the same
 * `struct aabb` shape `actor_part4.c`/`actor_part15.c` already use),
 * mirrors it per `owner->0x28` bit 4, then tests it against the player
 * (`gUnknown_030012D8`) via `sub_800B37C` - on overlap, triggers
 * `sub_800C8CC(self,2)` and, if `self->0x6c==0xb`, seeds `owner`'s
 * `0x48`-`0x64` velocity-target fields with fixed constants (a
 * "knockback impulse" shape, same family as `sub_800B8DC` state 17's
 * own jump-impulse). Mode 2 triggers `sub_800C8CC(self,0)` only when
 * `owner->0x38` is set.
 *
 * Both NAKED transcriptions, not real C: the same `self`/`owner`
 * multi-field-liveness shape already established as resistant
 * throughout this ROM neighborhood (docs/matching/issue-9-10-0x0800b8dc-graphics.md's
 * "Matching" section) - transcribed directly from the confirmed-correct
 * ROM disassembly, including `sub_800C40C`'s own multi-arg
 * `sub_8025B0C` call site (register/stack setup transcribed exactly as
 * the ROM emits it). Confirmed byte-exact via the isolated
 * cpp/agbcc/as + objcopy pipeline (the only differences from a direct
 * ROM slice are the `bl` and `.word` relocation sites) and a full clean
 * `make compare`. `sub_800C5D4`'s own trailing byte count needed the
 * `matching_decomp_alignment_fix` trailing `asm(".align 2, 0")` idiom
 * (the ROM zero-pads its last 2 bytes to the next 4-byte boundary). */

extern void sub_800C8AC(void *self, s32 mode);
extern void sub_800C8BC(void *self, s32 mode);
extern void sub_800C8CC(void *self, s32 mode);
extern s32 sub_803AE4C(s32 a, s32 b);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *sub_8025B0C(void *arg0, void *arg1, void *arg2, s32 margin, s32 z, void *src);
extern u8 sub_800B37C(void *selfArg, void *buf);
extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern void *gUnknown_0300082C;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012E4;
extern void *gUnknown_030012D8;

NAKED void sub_800C40C(void *selfArg)
{
    asm(
        "push {r4, lr}\n\t"
        "sub sp, #0xc\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x68]\n\t"
        "cmp r0, #3\n\t"
        "beq 14f\n\t"
        "cmp r0, #3\n\t"
        "bgt 1f\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "b 9f\n\t"
    "1:\n\t"
        "cmp r0, #4\n\t"
        "beq 10f\n\t"
        "cmp r0, #5\n\t"
        "bne 21f\n\t"
        "b 18f\n\t"
    "21:\n\t"
        "b 9f\n\t"
    "2:\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "cmp r1, #0\n\t"
        "bgt 3f\n\t"
        "b 9f\n\t"
    "3:\n\t"
        "ldr r0, =gUnknown_0300082C\n\t"
        "ldr r3, [r4, #0x30]\n\t"
        "add r1, r3, r1\n\t"
        "lsl r2, r1, #1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r2, [r4, #0x38]\n\t"
        "sub r0, r0, r2\n\t"
        "sub r0, r0, r3\n\t"
        "bl sub_803AE4C\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "b 9f\n\t"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x84\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "cmp r0, #8\n\t"
        "beq 5f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #3\n\t"
        "bl sub_800C8CC\n\t"
        "b 6f\n\t"
        ".pool\n\t"
    "5:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #4\n\t"
        "bl sub_800C8CC\n\t"
    "6:\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0xf\n\t"
        "bne 7f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8BC\n\t"
        "b 9f\n\t"
    "7:\n\t"
        "cmp r0, #0x12\n\t"
        "beq 8f\n\t"
        "cmp r0, #0x1a\n\t"
        "beq 8f\n\t"
        "b 9f\n\t"
    "8:\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "mov r0, #9\n\t"
        "neg r0, r0\n\t"
        "ldrb r2, [r1, #0xd]\n\t"
        "and r0, r0, r2\n\t"
        "strb r0, [r1, #0xd]\n\t"
        "b 9f\n\t"
    "10:\n\t"
        "ldr r3, [r4, #0x30]\n\t"
        "cmp r3, #0\n\t"
        "bgt 11f\n\t"
        "b 9f\n\t"
    "11:\n\t"
        "ldr r0, =gUnknown_0300082C\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r4, #0x38]\n\t"
        "sub r0, r0, r2\n\t"
        "add r1, r3, r1\n\t"
        "bl sub_803AE4C\n\t"
        "cmp r0, #0\n\t"
        "beq 12f\n\t"
        "b 9f\n\t"
    "12:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x84\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "cmp r0, #8\n\t"
        "beq 13f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #5\n\t"
        "bl sub_800C8CC\n\t"
        "b 9f\n\t"
        ".pool\n\t"
    "13:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
        "b 9f\n\t"
    "14:\n\t"
        "ldr r0, [r4, #0x70]\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 17f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #4\n\t"
        "bl sub_800C8CC\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0x12\n\t"
        "beq 15f\n\t"
        "cmp r0, #0x1a\n\t"
        "bne 16f\n\t"
    "15:\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "mov r0, #8\n\t"
        "ldrb r2, [r1, #0xd]\n\t"
        "orr r0, r0, r2\n\t"
        "strb r0, [r1, #0xd]\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x26\n\t"
        "bl PlaySfx\n\t"
        "b 17f\n\t"
        ".pool\n\t"
    "16:\n\t"
        "cmp r0, #0xf\n\t"
        "bne 17f\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #9\n\t"
        "bl PlaySfx\n\t"
    "17:\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0x17\n\t"
        "bne 9f\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "ldr r0, [r1, #0x30]\n\t"
        "cmp r0, #9\n\t"
        "bne 9f\n\t"
        "ldr r2, [r1, #0x34]\n\t"
        "cmp r2, #0\n\t"
        "bne 9f\n\t"
        "mov r4, #2\n\t"
        "ldr r0, =gUnknown_030012E4\n\t"
        "ldr r0, [r0]\n\t"
        "str r4, [sp]\n\t"
        "str r2, [sp, #4]\n\t"
        "str r1, [sp, #8]\n\t"
        "mov r1, #0x17\n\t"
        "mov r2, #4\n\t"
        "mov r3, #0x2d\n\t"
        "neg r3, r3\n\t"
        "bl sub_8025B0C\n\t"
        "mov r1, #4\n\t"
        "ldrb r2, [r0, #0xc]\n\t"
        "orr r1, r1, r2\n\t"
        "mov r2, #0x41\n\t"
        "neg r2, r2\n\t"
        "and r1, r1, r2\n\t"
        "strb r1, [r0, #0xc]\n\t"
        "strb r4, [r0, #0xa]\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x1e\n\t"
        "bl PlaySfx\n\t"
        "b 9f\n\t"
        ".pool\n\t"
    "18:\n\t"
        "ldr r0, [r4, #0x70]\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 19f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0xf\n\t"
        "bne 9f\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800C8BC\n\t"
    "19:\n\t"
        "ldr r0, [r4, #0x6c]\n\t"
        "cmp r0, #0xf\n\t"
        "bne 9f\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "ldr r0, [r1, #0x30]\n\t"
        "cmp r0, #8\n\t"
        "bne 9f\n\t"
        "ldr r0, [r1, #0x34]\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "ldr r0, =gUnknown_030012BC\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x23\n\t"
        "bl PlaySfx\n\t"
    "9:\n\t"
        "add sp, #0xc\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool\n\t"
    );
}

NAKED void sub_800C5D4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #0x10\n\t"
        "add r6, r0, #0\n\t"
        "ldr r0, [r6, #0x6c]\n\t"
        "cmp r0, #0xb\n\t"
        "bne 1f\n\t"
        "ldr r1, [r6, #0x70]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "ldr r2, [r6, #0x64]\n\t"
        "cmp r0, r2\n\t"
        "bge 1f\n\t"
        "str r2, [r1, #4]\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8AC\n\t"
    "1:\n\t"
        "ldr r7, [r6, #0x68]\n\t"
        "cmp r7, #0\n\t"
        "beq 2f\n\t"
        "cmp r7, #2\n\t"
        "beq 3f\n\t"
        "b 4f\n\t"
    "2:\n\t"
        "ldr r0, [r6, #0x70]\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r0, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r5, [r6, #0x28]\n\t"
        "ldr r3, [r6, #0x20]\n\t"
        "sub r5, r5, r3\n\t"
        "ldr r4, [r6, #0x2c]\n\t"
        "ldr r0, [r6, #0x24]\n\t"
        "sub r4, r4, r0\n\t"
        "add r1, r1, r3\n\t"
        "add r2, r2, r0\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r4, #0\n\t"
        "bl sub_803AFDC\n\t"
        "ldr r1, [r6, #0x70]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 5f\n\t"
        "ldr r0, [r1]\n\t"
        "asr r0, r0, #8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "5:\n\t"
        "ldr r0, =gUnknown_030012D8\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, sp\n\t"
        "bl sub_800B37C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_800C8CC\n\t"
        "ldr r0, [r6, #0x6c]\n\t"
        "cmp r0, #0xb\n\t"
        "bne 4f\n\t"
        "ldr r0, [r6, #0x70]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #2\n\t"
        "mov r2, #0x20\n\t"
        "str r1, [r0, #0x64]\n\t"
        "str r1, [r0, #0x54]\n\t"
        "str r2, [r0, #0x58]\n\t"
        "str r7, [r0, #0x5c]\n\t"
        "ldr r1, =0xFFFFFE00\n\t"
        "str r7, [r0, #0x60]\n\t"
        "str r7, [r0, #0x48]\n\t"
        "str r2, [r0, #0x4c]\n\t"
        "str r1, [r0, #0x50]\n\t"
        "b 4f\n\t"
        ".pool\n\t"
    "3:\n\t"
        "ldr r0, [r6, #0x70]\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800C8CC\n\t"
    "4:\n\t"
        "add sp, #0x10\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool\n\t"
    );
}
asm(".align 2, 0");

#include "core.h"

/* Same "self" object family as actor_part59.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-63-0x08033ef4-actor.md,
 * "Parked: sub_8034058" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s) is used otherwise.
 * Constructor: health defaults to `0x10`, or `0x18` if the
 * `gUnknown_030015AC` singleton hasn't been constructed yet
 * (`sub_80338DC() == 0`). Forwards to `InitActorPart`, sets the event
 * table (`+0x50=&gStaticData_087E5554`), caches the constructor's 6th
 * (byte, stack-passed) argument at `+0x59`, selects table-index 0 or 1
 * depending on whether that byte is set, resets the usual state/frame-
 * counter/anim fields, clears the death flag (`+0x58=0`), seeds the
 * "spawn/orbit" record (`+0x5c` from the `+0x59` byte, `+0x60=0xa00`,
 * `+0x64=-1`), clears the one-shot flag (`+0x2c=0`), caches the
 * singleton table's `+4` field at `+0x68`, and clears `+0x6c`. Returns
 * `self`. Semantics fully understood and every field/call confirmed
 * correct; parked because this compiler reads the 6th (stack-passed,
 * byte-sized) constructor argument as a full word shifted/masked down
 * to its low byte, where the ROM's own build addresses that stack slot
 * directly with a plain `ldrb` - the same trailing-byte-stack-argument
 * gap already parked for `sub_8025A64` in game_loop14.c. */
extern void *sub_80338DC(void);
extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E5554[];
extern void *sub_80338C4(void);

void *sub_8034058(void *selfArg, void *part, s32 b, s32 cParam, s32 d, u8 eByte)
{
    u8 *self = selfArg;
    s32 health;
    s32 idx;

    health = (sub_80338DC() != 0) ? 0x10 : 0x18;

    InitActorPart(self, part, b, cParam, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E5554;
    self[0x59] = eByte;
    idx = (self[0x59] != 0) ? 0 : 1;
    *(s32 *)(self + 0x28) = 0;
    *(s32 *)(self + 0x44) = 0;
    *(s32 *)(self + 0xc) = idx;
    *(u16 *)(self + 0x10) = *(u16 *)(*(u8 **)self + idx * 12);
    self[0x12] = 0;
    *(s32 *)(self + 8) = 0;
    self[0x58] = 0;
    *(s32 *)(self + 0x5c) = (self[0x59] != 0) ? 0xFFFFBF00 : 0x8400;
    *(s32 *)(self + 0x60) = 0xa00;
    *(s32 *)(self + 0x64) = -1;
    self[0x2c] = 0;
    *(s32 *)(self + 0x68) = *(s32 *)((u8 *)sub_80338C4() + 4);
    *(s32 *)(self + 0x6c) = 0;

    return self;
}
#endif /* NON_MATCHING */

asm(".align 2, 0");

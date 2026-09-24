#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E517C[];

/* An `InitActorPart`-based constructor for this cluster's `self` object:
 * forwards its first three real arguments plus one stack argument
 * straight to `InitActorPart`, then marks `self+0x54` = 1, sets
 * `self+0x50`'s event/trampoline table to `gStaticData_087E517C`, and
 * stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`.
 * The same 7-argument `InitActorPart`-wrapper shape as `sub_80305F8`
 * (docs/matching/issue-58-0x08030334-actor.md). A plain (unpinned) local
 * for the `1` constant, declared and assigned *before* the
 * `InitActorPart` call, is what gets this compiler to keep it live in a
 * register across the call and push/pop the full `r4-r7` set the ROM
 * does - an explicit `register ... asm("r7")` pin for `f` compiles and
 * puts `f` in `r7` at every use, but (matching the categorical `r7`-pin
 * quirk documented elsewhere in this codebase) never makes it into this
 * compiler's own `push`/`pop` list, leaving `r7` unsaved across the call
 * even though the ROM saves it. Letting the natural, unforced register
 * pressure from four call-spanning values (`self`, the `1` constant,
 * `e`, `f`) decide is what reproduces the ROM's own `r4=self,r5=1,r6=e,
 * r7=f` allocation, including saving `r7`. */
void *sub_802FA04(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    u8 *self = selfArg;
    s32 health = 1;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E517C;
    *(s32 *)(self + 0x58) = e;
    *(s32 *)(self + 0x5c) = f;
    return self;
}

asm(".align 2, 0");

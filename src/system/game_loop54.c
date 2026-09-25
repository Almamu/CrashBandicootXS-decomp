#include "core.h"

/* GitHub issue #12/#14 Phase 2 mop-up: the last 5 raw functions of the
 * still-large 24-function tail past `sub_8010D54`
 * (`asm/code_3_2_17_e560_10d54.s`) - see
 * docs/matching/issue-14-0x08010d54-physics-apply.md's "Not integrated
 * this pass" section for the individual draft characterizations this
 * file finishes integrating: `sub_8010E34`, `sub_8010EAC`, `sub_8010F8C`,
 * `sub_8011114`, `sub_80111B8`. All 5 operate on the same still-unnamed
 * "part" object `src/system/game_loop52.c`/`game_loop53.c` already
 * document (raw `u8 *`/offset convention, matching those two sibling
 * files - no named struct exists yet for this object). This closes out
 * the entire `0x08010D54` chunk (issue #12/#14): every function between
 * `sub_8010D54` and the already-matched `src/graphics/actor_part39.c`
 * (`sub_80119A8`) is now matched. */

extern void *gUnknown_030012D8;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012EC;
extern void *gUnknown_03001318;

extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *sub_8007B98(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern s32 sub_80008F0(s32 arg0, s32 arg1);
extern s32 sub_80008FC(s32 a, s32 b);
extern s32 sub_8023464(void *self);
extern void sub_80284A4(void *state);
extern void *sub_8026EDC(s32 size);
extern struct actor *sub_80084A4(struct actor *self);
extern void sub_8008E94(void *manager, void *value);
extern void sub_8011308(void *self);
extern void sub_8011248(void *self);
extern void sub_8008364(struct actor *part);
extern s16 gStaticData_0816A820[];
extern u8 gStaticData_087E40DC[];
extern s32 rand(void);

void sub_8010EAC(void *selfArg, u8 randomize);

/* Called from `sub_8010EAC` below only (the "randomized-behavior"
 * family's own bounds-check gate). No existing cross-reference
 * elsewhere in the codebase. Gated: does nothing unless the orbit is
 * active (`self+0x4a != 0`) and either its phase hasn't wrapped past
 * `0x16` yet or the player (`gUnknown_030012D8`) is in state
 * `+0x88 == 3` - the exact same opening gate `sub_8011390`
 * (`game_loop52.c`) already uses. Once past that gate, proceeds only
 * when flags bit 3 is clear and flags bit 2 is set (same bit-test idiom
 * as `sub_8011390`), builds both `self`'s and the player's AABB via
 * `sub_8007B98` (unlike `sub_8011390`, always the "secondary" AABB
 * build for both sides - no `player+0xa == 0x13` branch here), and on
 * overlap sets flags bit 3 and calls `sub_8010EAC(self, 0)` (the fixed,
 * non-randomized despawn-offset path). */
void sub_8010E34(void *selfArg)
{
    u8 *self = selfArg;
    u8 selfBox[16];
    u8 playerBox[16];
    u8 *player;

    if (self[0x4a] != 0 && self[0x4b] <= 0x16) {
        if (((u8 *)gUnknown_030012D8)[0x88] != 3) {
            return;
        }
    }

    {
        u8 flags = self[0xc];
        u32 shifted = (u32)flags << 0x18;

        if ((shifted >> 0x1b) & 1) {
            return;
        }
        if (!((shifted >> 0x1a) & 1)) {
            return;
        }
    }

    sub_8007B98(selfBox, self);
    player = gUnknown_030012D8;
    sub_8007B98(playerBox, player);

    if (sub_8001688(playerBox, selfBox)) {
        register s32 bit asm("r0") = 8;

        bit |= self[0xc];
        self[0xc] = bit;
        sub_8010EAC(self, 0);
    }
}

/* `docs/rom_map.md`: part of "the randomized-behavior... famil[y]"
 * alongside `sub_8016048`. Plays a hit SFX, sets `self+0x3c` (a
 * timer/animation field) to `0xa0`, then either derives a randomized
 * `(dx,dy)` offset pair from `rand()` (`randomize` nonzero -
 * `self->0x49` tags which of three `rand()`-driven bands the x-offset
 * came from, `self->0x48 = 2`) or uses a fixed `(0xb400,0xc00)` offset
 * and fires `sub_80284A4(gUnknown_03001318)` (`self->0x48 = 1`). Either
 * way: `self->0xc |= 0x10`, `self->0x25 = 1`, then calls
 * `sub_8007174(self, self->x>>8, self->y>>8, &outX, &outY)` and
 * re-derives `self->x`/`self->y` plus `self->0x40`/`self->0x44` (a
 * "distance to travel" pair, `-sub_80008F0(newPos<<8 - offset, 0x1400)`)
 * from the results - the exact same tail shape `sub_8011448`/
 * `sub_80111B8`/`sub_8011870` (`game_loop53.c`) all share. */
void sub_8010EAC(void *selfArg, u8 randomize)
{
    u8 *self = selfArg;
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    asm volatile("" : "+r"(self));

    PlaySfx(gUnknown_030012BC, 7, 0x100);
    *(u16 *)(self + 0x3c) = 0xa0;

    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        self[0x49] = lowbit;
        if (lowbit) {
            if (rv & 2) {
                dx = ((rv & 0x3f) + 5) << 8;
            } else {
                dx = (0xeb - (rv & 0x3f)) << 8;
            }
        } else {
            dx = ((rv & 0x7f) + 0x24) << 8;
        }
        dy = ((rv & 0x1f) + 0x10) << 8;
        self[0x48] = 2;
    } else {
        dx = 0xb400;
        dy = 0xc00;
        self[0x48] = 1;
        sub_80284A4(gUnknown_03001318);
    }

    {
        register s32 mask asm("r0") = 0x10;

        mask |= self[0xc];
        self[0xc] = mask;
    }
    {
        register u8 one asm("r0") = 1;

        self[0x25] = one;
    }

    sub_8007174(self, *(s32 *)self >> 8, *(s32 *)(self + 4) >> 8, &outX, &outY);

    newX = outX << 8;
    *(s32 *)self = newX;
    *(s32 *)(self + 0x40) = -sub_80008F0(newX - dx, 0x1400);

    newY = outY << 8;
    *(s32 *)(self + 4) = newY;
    *(s32 *)(self + 0x44) = -sub_80008F0(newY - dy, 0x1400);
}

/* `docs/rom_map.md`: "a bounds-checked, mode-selected object state
 * machine that self-destructs off-screen" - a rotating/orbiting
 * hazard/projectile behavior, uses the shared sine table
 * `gStaticData_0816A820` this whole neighborhood references. Modes
 * 1/2 integrate `self->x`/`self->y` by `self->0x40`/`self->0x44` and,
 * on reaching an on-screen "arrival" bound (mode 1) or a wrapping
 * `self->0x3c` timer threshold (mode 2), fire a hit SFX,
 * `sub_8023464(gUnknown_030012C0)`, set flags bit 0, and (unless
 * `self->8 == 0xffff`) set `self->8`'s bit in the
 * `gUnknown_030012B4+0x108` collision bitmap - the same inline idiom
 * `sub_8011548` (`game_loop53.c`) also duplicates per mode. Any other
 * mode (0, or 3+): gated by `self->0x4a`, increments `self->0x49` or
 * `self->0x4b` (wrapping the gate off after 32 ticks). The shared tail:
 * unless `self->0x48 != 0`, either computes an orbit step via
 * `gStaticData_0816A820[(self->0x49 & 0x7f)]` and `sub_80008FC` added
 * into `self->0x50`, storing to `self->y` (when `self->0x4a` is clear),
 * or calls `sub_8011248` (`game_loop52.c`'s own orbit-position updater)
 * when `self->0x4a` is set - then always tail-calls `sub_8008364`
 * (already matched, `actor_part5.c`).
 *
 * NAKED, not plain C: this function's two independently-inlined
 * "collision bitmap" tails (mode 1's and mode 2's) each recompute their
 * own address/shift chain with a different register allocation
 * depending on which registers survive from that mode's own preceding
 * branch - notably mode 1's tail opportunistically reuses `r5` (still
 * holding the just-tested `self->0x48 == 1` mode value) as the literal
 * `1` for its `1 << bit` shift, something a natural plain-C compile of
 * the same logic can't be coaxed into reproducing (the two tails are
 * textually identical C but the ROM's own register choice differs
 * between them) - the same "shared tail duplicated near-identically
 * with different register survivors" shape already documented for
 * `sub_8011548` in this exact neighborhood (`game_loop53.c`).
 * Transcribed straight from the confirmed-correct ROM disassembly. */
NAKED void sub_8010F8C(void *self)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "add r4, r0, #0\n\t"
        "add r0, #0x48\n\t"
        "ldrb r5, [r0]\n\t"
        "cmp r5, #1\n\t"
        "bne 3f\n\t"
        "ldr r1, [r4]\n\t"
        "ldr r3, [r4, #0x40]\n\t"
        "add r0, r1, r3\n\t"
        "str r0, [r4]\n\t"
        "ldr r2, [r4, #4]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r0, r2, r0\n\t"
        "str r0, [r4, #4]\n\t"
        "add r1, r1, r3\n\t"
        "asr r1, r1, #8\n\t"
        "cmp r1, #0xb4\n\t"
        "ble 1f\n\t"
        "b 9f\n\t"
    "1:\n\t"
        "asr r0, r0, #8\n\t"
        "cmp r0, #0xc\n\t"
        "ble 2f\n\t"
        "b 9f\n\t"
    "2:\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0xe\n\t"
        "bl PlaySfx\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8023464\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 22f\n\t"
        "ldrh r2, [r4, #8]\n\t"
        "cmp r2, r0\n\t"
        "beq 9f\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r2, r0, #2\n\t"
        "mov r6, #0x84\n\t"
        "lsl r6, r6, #1\n\t"
        "add r1, r1, r6\n\t"
        "add r1, r1, r2\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "lsl r5, r0\n\t"
        "ldr r0, [r1]\n\t"
        "orr r0, r5\n\t"
        "str r0, [r1]\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012BC\n"
    "21: .4byte gUnknown_030012C0\n"
    "22: .4byte 0x0000FFFF\n"
    "23: .4byte gUnknown_030012B4\n"
    "3:\n\t"
        "cmp r5, #2\n\t"
        "bne 7f\n\t"
        "ldr r1, [r4]\n\t"
        "ldr r0, [r4, #0x40]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r4]\n\t"
        "ldr r1, [r4, #4]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r4, #4]\n\t"
        "mov r1, #0\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x49\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "ldrh r0, [r4, #0x3c]\n\t"
        "sub r0, #4\n\t"
        "strh r0, [r4, #0x3c]\n\t"
        "cmp r0, #0x3f\n\t"
        "bgt 5f\n\t"
        "b 6f\n\t"
    "4:\n\t"
        "ldrh r0, [r4, #0x3c]\n\t"
        "add r0, #0xc\n\t"
        "strh r0, [r4, #0x3c]\n\t"
        "mov r0, #0xd8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldrh r2, [r4, #0x3c]\n\t"
        "cmp r2, r0\n\t"
        "ble 5f\n\t"
        "mov r1, #1\n\t"
    "5:\n\t"
        "cmp r1, #0\n\t"
        "beq 9f\n\t"
    "6:\n\t"
        "mov r0, #1\n\t"
        "ldrb r5, [r4, #0xc]\n\t"
        "orr r0, r5\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 24f\n\t"
        "ldrh r6, [r4, #8]\n\t"
        "cmp r6, r0\n\t"
        "beq 9f\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, 25f\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r5, #0x84\n\t"
        "lsl r5, r5, #1\n\t"
        "add r2, r2, r5\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, #1\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "24: .4byte 0x0000FFFF\n"
    "25: .4byte gUnknown_030012B4\n"
    "7:\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x4a\n\t"
        "ldrb r0, [r2]\n\t"
        "cmp r0, #0\n\t"
        "bne 8f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x49\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "b 9f\n\t"
    "8:\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4b\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x1f\n\t"
        "bls 9f\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r2]\n\t"
    "9:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x48\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 11f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4a\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 10f\n\t"
        "ldr r1, 26f\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x49\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r2, [r2]\n\t"
        "and r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "mov r6, #0\n\t"
        "ldrsh r2, [r0, r6]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_80008FC\n\t"
        "add r2, r0, #0\n\t"
        "ldr r0, [r4, #0x50]\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r4, #4]\n\t"
        "b 11f\n\t"
        ".align 2, 0\n"
    "26: .4byte gStaticData_0816A820\n"
    "10:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8011248\n\t"
    "11:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8008364\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* `struct actor *sub_8011114(u16 arg0, u16 arg1, u16 arg2, s32 arg3)` -
 * spawns a part-object; extern already declared in `game_loop29.c`.
 * `arg3` is never actually read (the ROM hardcodes the field it would
 * feed - `self+0x29`/`+0x2a`/`+0x2b` - to a compile-time `0`
 * regardless), matching the extern's own always-`0` call sites.
 * Allocates a `0x54`-byte object (`sub_8026EDC`), re-initializes it
 * (`sub_80084A4`), repoints `self->table` (`self+0x18`) at
 * `gStaticData_087E40DC`, clears the "spawned/active" gate byte via
 * `sub_8011308` (`game_loop52.c`), stores `arg0` at `self+8` and
 * `arg1`/`arg2` (Q8-scaled) at `self+0`/`self+4`, mirrored into
 * `self+0x4c`/`self+0x50` (the orbit anchor `sub_8011364`/
 * `sub_8011248` also use), joins the `gUnknown_030012EC`
 * `dual_array_manager` list (`sub_8008E94`), derives `self+0x30` from
 * the same `table[self->0x2d]->+0x16` clamp idiom `sub_8011448`/
 * `sub_8011870` (`game_loop53.c`) use, clears bits 0/5 of `self+0x28`,
 * and always tags `self+0x29`/`+0x2a`/`+0x2b` all `0`, returning the
 * new part.
 *
 * NAKED, not plain C: needs the `0` sentinel alive in `r8` across the
 * `sub_8026EDC`/`sub_80084A4`/`sub_8011308` call sequence purely so it
 * can later be spilled back out for the three trailing `self+0x29`/
 * `+0x2a`/`+0x2b` byte stores - the same confirmed
 * `mov r_lo,r_hi`/`push {r_lo,...}` high-register save/restore dance
 * this compiler only reproduces when its own *unforced* allocator picks
 * those registers itself, already documented at length for this exact
 * object family's other spawn helpers (`sub_801173C`/`sub_8011548`,
 * `game_loop53.c`). Transcribed straight from the confirmed-correct ROM
 * disassembly. */
NAKED struct actor *sub_8011114(u16 arg0, u16 arg1, u16 arg2, s32 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r6, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "add r5, r2, #0\n\t"
        "lsl r6, r6, #0x10\n\t"
        "lsr r6, r6, #0x10\n\t"
        "lsl r4, r4, #0x10\n\t"
        "lsr r4, r4, #0x10\n\t"
        "lsl r5, r5, #0x10\n\t"
        "lsr r5, r5, #0x10\n\t"
        "mov r0, #0x54\n\t"
        "bl sub_8026EDC\n\t"
        "add r7, r0, #0\n\t"
        "bl sub_80084A4\n\t"
        "ldr r0, 2f\n\t"
        "str r0, [r7, #0x18]\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8011308\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "strh r6, [r7, #8]\n\t"
        "lsl r4, r4, #8\n\t"
        "str r4, [r7]\n\t"
        "lsl r5, r5, #8\n\t"
        "str r5, [r7, #4]\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r7, #4]\n\t"
        "str r0, [r7, #0x4c]\n\t"
        "str r1, [r7, #0x50]\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8008E94\n\t"
        "mov r3, #0\n\t"
        "ldr r0, [r7, #0x20]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r4, [r2]\n\t"
        "lsl r0, r4, #3\n\t"
        "sub r0, r0, r4\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r3, r0\n\t"
        "blt 1f\n\t"
        "sub r3, r0, #1\n\t"
    "1:\n\t"
        "str r3, [r7, #0x30]\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x28\n\t"
        "mov r1, #0x11\n\t"
        "neg r1, r1\n\t"
        "ldrb r2, [r0]\n\t"
        "and r1, r2\n\t"
        "mov r2, #0x21\n\t"
        "neg r2, r2\n\t"
        "and r1, r2\n\t"
        "strb r1, [r0]\n\t"
        "add r0, #0x21\n\t"
        "mov r4, r8\n\t"
        "strb r4, [r0]\n\t"
        "add r0, #1\n\t"
        "strb r4, [r0]\n\t"
        "add r0, #1\n\t"
        "strb r4, [r0]\n\t"
        "add r0, r7, #0\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "2: .4byte gStaticData_087E40DC\n"
    "3: .4byte gUnknown_030012EC\n"
    );
}

/* `void sub_80111B8(void *part)` - extern already declared in
 * `game_loop29.c`; the documented "mutually exclusive alternative" is
 * `sub_8011870` (`game_loop53.c`, already matched). Plays a hit SFX,
 * sets `self->0x48 = 1`, nudges `self->x -= self->0x4a<<8`, sets
 * `self->0x25 = 1`, calls `sub_8007174(self, x>>8, y>>8, &outX, &outY)`
 * and re-derives `self->x`/`self->y` plus `self->0x40`/`self->0x44`
 * (the same `-sub_80008F0(newPos<<8 - offset, 0x1400)` "distance to
 * travel" idiom `sub_8010EAC`/`sub_8011448`/`sub_8011870` all share),
 * with fixed `0xb400`/`0xc00` offsets on x/y respectively, then fires
 * `sub_80284A4(gUnknown_03001318)` - unlike `sub_8011870`'s
 * `sub_80284D4`. Notably simpler than its `sub_8011870` sibling: no
 * `self->0x3c`/`self->0x30` table-lookup-clamp setup here at all. */
void sub_80111B8(void *selfArg)
{
    u8 *self = selfArg;
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gUnknown_030012BC, 7, 0x100);
    self[0x48] = 1;
    {
        register s32 off asm("r0") = self[0x4a];
        register s32 shifted asm("r1") = off << 8;

        *(s32 *)self -= shifted;
    }
    self[0x25] = 1;

    sub_8007174(self, *(s32 *)self >> 8, *(s32 *)(self + 4) >> 8, &outX, &outY);

    newX = outX << 8;
    *(s32 *)self = newX;
    *(s32 *)(self + 0x40) = -sub_80008F0(newX - 0xb400, 0x1400);

    newY = outY << 8;
    *(s32 *)(self + 4) = newY;
    *(s32 *)(self + 0x44) = -sub_80008F0(newY - 0xc00, 0x1400);

    sub_80284A4(gUnknown_03001318);
}

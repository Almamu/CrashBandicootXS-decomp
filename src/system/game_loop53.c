#include "core.h"
#include "actor.h"
#include "orbit_part.h"

/* GitHub issue #12/#14 Phase 2, second parallel slice: the tail 6
 * functions of the still-large 24-function chunk past sub_8010D54
 * (asm/code_3_2_17_e560_10d54.s) - see
 * docs/matching/issue-14-0x08010d54-physics-apply.md's Phase 2 planning
 * section for the full function/size list. This file carves out only
 * sub_8011448-sub_801192C (the chunk's last 6 functions, contiguous
 * through to the already-matched src/graphics/actor_part39.c at
 * 0x080119A8) - a clean single trim point at the tail of the asm file,
 * chosen specifically because sub_8011114 and the sub_8011248-
 * sub_8011390 "no cross-reference" accessor cluster in between this
 * file's own functions and the ones a sibling parallel session is
 * working on are interleaved with several *other* individually-
 * characterized functions (sub_8010E34/sub_8010EAC/sub_8010F8C/
 * sub_80111B8) this pass also read and verified in isolation but did
 * NOT integrate here - splitting the asm file at more than one point to
 * reach them would need a second new C file, which this session's
 * parallel-agent convention reserves collision-avoidance for a single
 * name (game_loop53.c) - see the issue doc's own follow-up note. */

extern void *gUnknown_030012BC;
extern void *gUnknown_03001318;
extern void *gUnknown_030012C0;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern s32 sub_80008F0(s32 arg0, s32 arg1);
extern s32 sub_80008FC(s32 a, s32 b);
extern void sub_80284D4(void *state);
extern s16 gStaticData_0816A820[];
extern s32 rand(void);

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15
 * NAKED retry: sub_8011448 and sub_801192C match only under it, and the
 * rest of the file compiles identically under either compiler. */

/* `frame = min(0, frameCount - 1)` against the part's current animation
 * record - the clamp every spawner/launcher in this family repeats. */
static inline void OrbitClampFrame(struct orbit_part *self)
{
    s32 frame = 0;
    s32 count = self->bank->records[self->tag].frameCount;

    if (frame >= count)
        frame = count - 1;
    self->frame = frame;
}

/* sub_8011448: "randomized-position spawn/despawn picker" (docs/rom_map.md),
 * called as `sub_8011448(entry, 1)`/`(other, 1)` from game_loop40.c/
 * game_loop49.c for despawn. PlaySfx(gUnknown_030012BC, 8, 0x100), then
 * either derives a randomized (dx,dy) offset pair from rand() (arg1
 * nonzero - self->0x48 = 2, self->0x49 tags which of three rand()-driven
 * bands was picked) or uses a fixed (0x1000,0x1000) offset and fires
 * sub_80284D4(gUnknown_03001318) (self->0x48 = 1). Either way: self->0x3c
 * = 0xa0, self->0x30 clamped from a self->0x20 table lookup at
 * self->0x2d*0x1c+0x16 (same "table[tag]->field 0x16, clamp against a
 * zero floor" idiom sub_8010F8C/sub_8011870 also use), self->0x25 = 1,
 * self->0xc |= 0x10, then calls sub_8007174(self, self->x>>8, self->y>>8,
 * &outX, &outY) and re-derives self->x/self->y plus self->0x40/self->0x44
 * (a "distance to travel" pair, via -sub_80008F0(newPos<<8 - offset,
 * 0x1400)) from the results - the exact same tail shape sub_8010EAC/
 * sub_80111B8/sub_8011870 all share in this subsystem.
 *
 * The `self->0x25 = 1` store goes through a `u8` local so old_agbcc
 * materializes the 1 before the field address, as the ROM does. (Earlier
 * notes blamed an r7 allocation gap; under old_agbcc the tag lands in r7
 * on its own.) */
void sub_8011448(struct orbit_part *self, u8 randomize)
{
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gUnknown_030012BC, 8, 0x100);
    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        self->counter = lowbit;
        if (lowbit) {
            if (rv & 2)
                dx = ((rv & 0x3f) + 5) << 8;
            else
                dx = (0xeb - (rv & 0x3f)) << 8;
        } else {
            dx = ((rv & 0x7f) + 0x24) << 8;
        }
        dy = ((rv & 0x1f) + 0x10) << 8;
        self->state = 2;
    } else {
        dx = dy = 0x1000;
        self->state = 1;
        sub_80284D4(gUnknown_03001318);
    }
    self->timer = 0xa0;
    OrbitClampFrame(self);
    {
        u8 one = 1;

        self->unk_25 = one;
    }
    self->base.flags |= 0x10;

    sub_8007174(self, self->base.x >> 8, self->base.y >> 8, &outX, &outY);

    newX = outX << 8;
    self->base.x = newX;
    self->velX = -sub_80008F0(newX - dx, 0x1400);
    newY = outY << 8;
    self->base.y = newY;
    self->velY = -sub_80008F0(newY - dy, 0x1400);
}

/* sub_8011548: "entity-vtable-dispatched velocity integrator" (docs/
 * rom_map.md). Dispatches on self->0x48 (0-3):
 *  - mode 1: integrates self->x/self->y by self->0x40/self->0x44 (the
 *    "distance to travel" pair sub_8011448/sub_8010EAC/etc. compute),
 *    wraps self->0x3c by +/-4 (mode-gated by self->0x49) each frame in
 *    [0,0x140], and once self->x>>8/self->y>>8 both fall within a small
 *    box (|x|<=0x10, |y|<=0x10) fires PlaySfx(gUnknown_030012BC,0xe,
 *    0x100), calls sub_8023430(gUnknown_030012C0) (a scoring/counter
 *    candidate per docs/rom_map.md), sets self->0xc bit 0, and - unless
 *    self->8 == 0xffff - sets self->8's bit in the gUnknown_030012B4+
 *    0x108 collision bitmap (the same inline idiom sub_80072D8/
 *    sub_8025A64 use on a struct actor).
 *  - mode 2: same integrate step, then wraps self->0x3c similarly but
 *    with different thresholds/direction, and on wrap-triggered falls
 *    into the same "set self->0xc bit 0 + collision-bitmap" tail as
 *    mode 1.
 *  - mode 3: increments self->0x49 each frame; every 11th frame resets
 *    it and calls sub_8025CA4(gUnknown_030012E4, self->x>>8, self->y>>8,
 *    0, 1, 0) (a NAKED part-object spawner already matched in
 *    game_loop14.c) - then increments self->0x4b every frame too; every
 *    10th frame falls into the same collision-bitmap tail as modes 1/2.
 *  - mode 0 (default, self->0x4a-gated): increments self->0x49 or
 *    self->0x4b depending on self->0x4a, wrapping self->0x4a's own
 *    gate off after 32 self->0x4b ticks.
 * All four modes converge on a shared tail: if self->0x48 == 0, reads
 * self->0x4a again - if clear, computes a velocity step via
 * gStaticData_0816A820[self->0x49 & 0x7f] and sub_80008FC, added to
 * self->0x50 and stored into self->y (a "rotate self->y around a fixed
 * center by a table-driven step" idiom, same table/shape as
 * sub_8010F8C's own default-mode branch); if set, calls sub_801192C
 * (the small self->0x4b/self->0x4a-driven table helper above) instead.
 * If self->0x48 == 3 specifically, self->x/self->y are instead reset to
 * gUnknown_030012D8's own position minus a small fixed offset
 * (0xFFFFFC00/0xFFFFF200, i.e. -0x400/-0xe00 in Q8). Every path ends
 * with a tail call to sub_8008364(self) (already matched elsewhere,
 * src/graphics/actor_part5.c).
 *
 * NAKED, not plain C: this is the largest function in this file (500
 * bytes) and, per this subsystem's own established pattern, hits the
 * same class of gcc-2.9 register-pressure mismatches already documented
 * at length for its neighbors (sub_800D040/sub_0800D18C/sub_8010B6C/
 * sub_8011448 above) - specifically the shared "self->0xc bit 0 +
 * collision-bitmap" tail duplicated after modes 1/2/3 each recomputes
 * its own address/shift chain slightly differently depending on which
 * registers are still live from that mode's own preceding branch, a
 * shape a natural compile collapses into one shared subroutine instead
 * of the ROM's own three near-identical inlined copies. Transcribed
 * straight from the confirmed-correct ROM disassembly - every field
 * offset, branch, and call argument (including the mode-3 spawn call's
 * exact fixed args) was cross-referenced against the semantic read
 * above before transcription. */
#if NON_MATCHING
/* First C draft (old_agbcc): control flow and every call are right,
 * including the mode-3 spawn's stack arguments (the game_loop48.c
 * argP4/argP5 trick), but it is 84 bytes too long. In the ROM,
 * cross-jumping merged the three "flags |= 1; set the id bit" tails
 * (mode 1 jumps into the bit-set part, modes 2 and 3 share everything
 * from the `orr`); here their register allocations differ, so the
 * copies stay separate. The mode-3 `++phase > 9` test also uses
 * `ands` with a hoisted 0xff instead of the ROM's `lsl`/`lsr`.
 *
 * Second pass (docs/matching/big-naked-retry-3.md): size-exact, 21
 * halfwords off. The tails now merge: the spawn's byte argument is a
 * plain `*(volatile u8 *)&argP5 = 1`, so its 1 is a QImode constant
 * that cse reuses for mode 3's `flags |= 1` but not for the SImode
 * `1 << bit`; the phase test spells out the zero-extension as shifts
 * (a QImode 0xff register was otherwise shared across the call); the
 * timer re-reads go through an `s32` inline so the compare stays the
 * ROM's signed `ble` after a fresh `ldrh`; the state is re-read for
 * each test. Left: modes 1/2 load x/y into r0 and the velocity into r1
 * (the ROM has them the other way round), the spawn's `movs r5, #1`
 * comes one instruction before `add r3, sp, #4`, and the state-3 tail
 * stores x before computing y. */
extern void *gUnknown_030012B4;
extern void *gUnknown_030012E4;
extern struct orbit_part *gUnknown_030012D8;
extern void sub_8023430(void *state);
extern struct actor *sub_8025CA4(void *unused0, u16 x, u16 y, u8 p3, u8 p4, u8 p5);
typedef struct actor *(*OrbitSpawn4)(void *pool, s32 x, s32 y, u8 p3);

extern void sub_801192C(struct orbit_part *self);
extern void sub_8008364(struct actor *self);

/* flags |= 1 and, unless the id is 0xffff, the id's bit in the
 * collision bitmap. The three copies are merged by cross-jumping. */
#define ORBIT_MARK_GONE(self, one)                                             \
    if (1)                                                                     \
    {                                                                          \
        (self)->base.flags |= (one);                                           \
        if ((self)->base.field_08 != 0xffff) {                                 \
            ORBIT_SET_ID_BIT((self)->base.field_08, 1);                        \
        }                                                                      \
    } else (void)0

#define ORBIT_SET_ID_BIT(idExpr, one)                                          \
    if (1)                                                                     \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gUnknown_030012B4;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= (one) << (_id - _word * 32);                                 \
    } else (void)0

/* An `s32` view of the timer: keeps the compare signed after the
 * ROM's fresh `ldrh`. */
static inline s32 OrbitTimer(struct orbit_part *self)
{
    return self->timer;
}

void sub_8011548(struct orbit_part *self)
{
    s32 argP4;
    u32 argP5;

    if (self->state == 1) {
        self->base.x += self->velX;
        self->base.y += self->velY;
        if (self->timer != 0) {
            self->timer += 4;
            if (OrbitTimer(self) > 0x100)
                self->timer = 0;
        }
        if (self->base.x >> 8 <= 0x10 && self->base.y >> 8 <= 0x10) {
            PlaySfx(gUnknown_030012BC, 0xe, 0x100);
            sub_8023430(gUnknown_030012C0);
            ORBIT_MARK_GONE(self, 1);
        }
    } else if (self->state == 2) {
        s32 fire;

        self->base.x += self->velX;
        self->base.y += self->velY;
        fire = 0;
        if (self->counter == 0) {
            s32 t = self->timer - 4;

            self->timer = t;
            if (t < 0x40)
                fire = 1;
        } else {
            self->timer += 0xc;
            if (OrbitTimer(self) > 0x1b0)
                fire = 1;
        }
        if (fire) {
            ORBIT_MARK_GONE(self, 1);
        }
    } else if (self->state == 3) {
        if (++self->counter > 10) {
            self->counter = 0;
            {
                s32 sx = self->base.x >> 8;
                s32 sy = self->base.y >> 8;

                ((OrbitSpawn4)sub_8025CA4)(gUnknown_030012E4, sx, sy,
                    (*(volatile s32 *)&argP4 = 0,
                     *(volatile u8 *)&argP5 = 1, 0));
            }
            {
                u32 ph = self->phase + 1;

                self->phase = ph;
                if ((ph << 24) >> 24 > 9) {
                    ORBIT_MARK_GONE(self, 1);
                }
            }
        }
    } else {
        if (self->mode == 0)
            self->counter++;
        else if (++self->phase > 0x1f)
            self->mode = 0;
    }

    if (self->state == 0) {
        if (self->mode == 0) {
            s32 sn = gStaticData_0816A820[(self->counter & 0x7f) * 2];

            sn = sub_80008FC(sn, 0x280);
            self->base.y = self->anchor.y + sn;
        } else {
            sub_801192C(self);
        }
    } else if (self->state == 3) {
        struct orbit_part *p = gUnknown_030012D8;
        s32 px = p->base.x, py = p->base.y;

        self->base.x = px - 0x400;
        self->base.y = py - 0xe00;
    }
    sub_8008364(&self->base);
}
#else
NAKED void sub_8011548(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "add r0, #0x48\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #1\n\t"
        "bne 2f\n\t"
        "ldr r1, [r4]\n\t"
        "ldr r0, [r4, #0x40]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r4]\n\t"
        "ldr r1, [r4, #4]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r4, #4]\n\t"
        "ldrh r0, [r4, #0x3c]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "add r0, #4\n\t"
        "strh r0, [r4, #0x3c]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #1\n\t"
        "ldrh r1, [r4, #0x3c]\n\t"
        "cmp r1, r0\n\t"
        "ble 3f\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r4, #0x3c]\n\t"
    "3:\n\t"
        "ldr r0, [r4]\n\t"
        "asr r0, r0, #8\n\t"
        "cmp r0, #0x10\n\t"
        "ble 4f\n\t"
        "b 15f\n\t"
    "4:\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "cmp r0, #0x10\n\t"
        "ble 5f\n\t"
        "b 15f\n\t"
    "5:\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0xe\n\t"
        "bl PlaySfx\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8023430\n\t"
        "mov r0, #1\n\t"
        "ldrb r2, [r4, #0xc]\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 22f\n\t"
        "ldrh r5, [r4, #8]\n\t"
        "cmp r5, r0\n\t"
        "bne 6f\n\t"
        "b 15f\n\t"
    "6:\n\t"
        "b 14f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012BC\n"
    "21: .4byte gUnknown_030012C0\n"
    "22: .4byte 0x0000FFFF\n"
    "2:\n\t"
        "cmp r0, #2\n\t"
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
        "bne 8f\n\t"
        "ldrh r0, [r4, #0x3c]\n\t"
        "sub r0, #4\n\t"
        "strh r0, [r4, #0x3c]\n\t"
        "cmp r0, #0x3f\n\t"
        "bgt 9f\n\t"
        "b 10f\n\t"
    "8:\n\t"
        "ldrh r0, [r4, #0x3c]\n\t"
        "add r0, #0xc\n\t"
        "strh r0, [r4, #0x3c]\n\t"
        "mov r0, #0xd8\n\t"
        "lsl r0, r0, #1\n\t"
        "ldrh r2, [r4, #0x3c]\n\t"
        "cmp r2, r0\n\t"
        "ble 9f\n\t"
        "mov r1, #1\n\t"
    "9:\n\t"
        "cmp r1, #0\n\t"
        "beq 15f\n\t"
    "10:\n\t"
        "mov r0, #1\n\t"
        "ldrb r5, [r4, #0xc]\n\t"
        "b 13f\n\t"
    "7:\n\t"
        "cmp r0, #3\n\t"
        "bne 16f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x49\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "mov r3, #0\n\t"
        "strb r0, [r1]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0xa\n\t"
        "bls 15f\n\t"
        "strb r3, [r1]\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "mov r5, #1\n\t"
        "strb r5, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4b\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #9\n\t"
        "bls 15f\n\t"
        "ldrb r0, [r4, #0xc]\n\t"
    "13:\n\t"
        "orr r0, r5\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 24f\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "cmp r1, r0\n\t"
        "beq 15f\n\t"
    "14:\n\t"
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
        "b 15f\n\t"
        ".align 2, 0\n"
    "23: .4byte gUnknown_030012E4\n"
    "24: .4byte 0x0000FFFF\n"
    "25: .4byte gUnknown_030012B4\n"
    "16:\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x4a\n\t"
        "ldrb r0, [r2]\n\t"
        "cmp r0, #0\n\t"
        "bne 17f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x49\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "b 15f\n\t"
    "17:\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4b\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x1f\n\t"
        "bls 15f\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r2]\n\t"
    "15:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x48\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 11f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4a\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 18f\n\t"
        "ldr r1, 26f\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x49\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r2, [r2]\n\t"
        "and r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, #0\n\t"
        "ldrsh r2, [r0, r1]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_80008FC\n\t"
        "add r2, r0, #0\n\t"
        "ldr r0, [r4, #0x50]\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r4, #4]\n\t"
        "b 19f\n\t"
        ".align 2, 0\n"
    "26: .4byte gStaticData_0816A820\n"
    "18:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801192C\n\t"
        "b 19f\n\t"
    "11:\n\t"
        "cmp r0, #3\n\t"
        "bne 19f\n\t"
        "ldr r0, 27f\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r1, #4]\n\t"
        "ldr r2, 28f\n\t"
        "add r0, r0, r2\n\t"
        "ldr r5, 29f\n\t"
        "add r1, r1, r5\n\t"
        "str r0, [r4]\n\t"
        "str r1, [r4, #4]\n\t"
    "19:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8008364\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "27: .4byte gUnknown_030012D8\n"
    "28: .4byte 0xFFFFFC00\n"
    "29: .4byte 0xFFFFF200\n"
    );
}
#endif

/* sub_801173C: the achievement/unlock-icon spawn helper (docs/rom_map.md),
 * extern-declared as `void sub_801173C(u16 arg0)` in
 * src/graphics/graphics_loading_21d80.c (that call site only ever reads
 * `arg0`, per its own doc comment - the other three args below are real
 * per this function's own body, just unused/garbage at that particular
 * call site) and called with all four real arguments from
 * sub_8025CA4 (game_loop14.c, NAKED, already matched): `sub_801173C(id,
 * x, y, special)` where `special` is `0xFFFF` or `0` selecting which of
 * two dual_array_manager lists (`gUnknown_030012F4` vs `gUnknown_030012EC`)
 * the newly spawned part joins. Allocates a new 0x54-byte object
 * (`sub_8026EDC`), re-initializes it (`sub_80084A4`), points its vtable
 * at `gStaticData_087E414C`, re-initializes via `sub_80119EC` (actor_part39.c,
 * already matched), stores `id` at `+8` and `x`/`y` (Q8-shifted) at `+0`/
 * `+4` - mirrored into `+0x4c`/`+0x50` as a "home position" pair the same
 * way sub_8011548's mode-3 branch reads it back - joins the
 * `special`-selected list, points `+0x20` at `gUnknown_030012D0`'s shared
 * resource table (fixed slot `0xd2*2`, per the same `sub_8025A64`/
 * `sub_8025CA4` convention), tags `+0x2d = 1`, builds the OAM/keyframe
 * trio (`sub_80087C0`/`sub_80087B4`/`sub_800872C`), derives `+0x30` from
 * the same `table[tag]->+0x16` clamp idiom as sub_8011448/sub_8011870,
 * clears bits 0/5 of `+0x28`, clears `+0x49`, tags `+0x4a`/`+0x4b` both 0
 * (always - `r7`/`r6` are hardcoded 0 locals, not passed through from any
 * argument), and - since that tag is always 0, never 0xff - never fires
 * the `sub_801191C` special-case call the ROM's own dead `cmp r7,#0xff`
 * still checks for. Finishes with the same `+0x29` nibble-from-
 * `sub_8006DF8` bitfield combine `sub_8025A64`/`sub_8025CA4` already use,
 * then returns the new part.
 *
 * NAKED, not plain C: identical shape to `sub_8025A64`/`sub_8025CA4`
 * (game_loop29.c/game_loop14.c, both already matched NAKED for exactly
 * this reason) - `x`/`y`/`special` have to stay truncated-and-live in
 * `r8`/`sb` across the `sub_8026EDC`/`sub_80084A4`/`sub_80119EC` call
 * sequence, the confirmed `mov r_lo,r_hi`/`push {r_lo,...}` high-register
 * save dance this compiler only reproduces when the *unforced* allocator
 * picks those registers itself - transcribed straight from the
 * confirmed-correct ROM disassembly instead of fighting it. */
#if NON_MATCHING
/* Near miss under old_agbcc: same instructions, but the ROM keeps `id` in
 * r8, `special` in sb and `mode` in r7, and shares the +0x4B zero with the
 * frame clamp's compare; old_agbcc assigns those the other way round.
 * Later pass: here x/y/id/special live only in the first basic block, so
 * local-alloc takes r4-r6/r8 for them before global-alloc places `self`
 * (r7) and `mode` (sb). The ROM's layout (self r4, mode r7) is what you
 * get when all of them go through global-alloc; extra references after
 * the list `if`/`else` get that (self r4, &tag r5, phase r6), but change
 * the parameter copies. Passing `phase` into the clamp as its start value
 * (`if (start >= count)`) reproduces the +0x49 fresh zero / +0x4B shared
 * zero split. `mode` needs `asm("" : "=r"(mode) : "0"(0))` to be
 * referenced at all. Best combination found was still ~110 halfwords
 * off. */
extern void *gUnknown_030012EC;
extern void *gUnknown_030012F4;
extern void ***gUnknown_030012D0;
extern void *gUnknown_030012B8;
extern u8 gStaticData_087E414C[];
extern void *sub_8026EDC(s32 size);
extern struct actor *sub_80084A4(struct actor *self);
extern void sub_8008E94(void *manager, void *value);
extern void sub_80119EC(struct orbit_part *self);
extern void sub_801191C(struct actor *self);
extern void sub_80087C0(struct orbit_part *part);
extern void sub_80087B4(struct orbit_part *part);
extern void sub_800872C(struct orbit_part *part, u8 val);
extern u8 sub_8006DF8(void *cache, u8 record);

struct orbit_part *sub_801173C(u16 id, u16 x, u16 y, u16 special)
{
    struct orbit_part *self;
    u8 mode = 0;
    u8 phase = 0;

    self = sub_8026EDC(0x54);
    sub_80084A4(&self->base);
    self->base.table = gStaticData_087E414C;
    sub_80119EC(self);
    self->base.field_08 = id;
    self->base.x = x << 8;
    self->base.y = y << 8;
    self->anchor = ORBIT_POS(self);
    if (special == 0xffff)
        sub_8008E94(gUnknown_030012F4, self);
    else
        sub_8008E94(gUnknown_030012EC, self);
    self->bank = (struct act_anim_bank *)((u8 *)**gUnknown_030012D0 + 0xd2 * 2);
    self->tag = 1;
    sub_80087C0(self);
    sub_80087B4(self);
    sub_800872C(self, 0);
    OrbitClampFrame(self);
    self->flipX = 0;
    self->flipY = 0;
    self->counter = 0;
    self->mode = mode;
    self->phase = phase;
    if (mode == 0xff)
        sub_801191C(&self->base);
    self->slotNibble = sub_8006DF8(gUnknown_030012B8, self->bank->records->unk_14);
    return self;
}
#else
NAKED struct actor *sub_801173C(u16 id, u16 x, u16 y, u16 special)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "mov r8, r0\n\t"
        "add r5, r1, #0\n\t"
        "add r6, r2, #0\n\t"
        "mov sb, r3\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r8, r0\n\t"
        "lsl r5, r5, #0x10\n\t"
        "lsr r5, r5, #0x10\n\t"
        "lsl r6, r6, #0x10\n\t"
        "lsr r6, r6, #0x10\n\t"
        "mov r1, sb\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "mov sb, r1\n\t"
        "mov r7, #0\n\t"
        "mov r0, #0x54\n\t"
        "bl sub_8026EDC\n\t"
        "add r4, r0, #0\n\t"
        "bl sub_80084A4\n\t"
        "ldr r0, 1f\n\t"
        "str r0, [r4, #0x18]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80119EC\n\t"
        "mov r3, r8\n\t"
        "strh r3, [r4, #8]\n\t"
        "lsl r5, r5, #8\n\t"
        "str r5, [r4]\n\t"
        "lsl r6, r6, #8\n\t"
        "str r6, [r4, #4]\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r4, #4]\n\t"
        "str r0, [r4, #0x4c]\n\t"
        "str r1, [r4, #0x50]\n\t"
        "ldr r0, 2f\n\t"
        "cmp sb, r0\n\t"
        "bne 4f\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_087E414C\n"
    "2: .4byte 0x0000FFFF\n"
    "3: .4byte gUnknown_030012F4\n"
    "4:\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
    "5:\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xd2\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "mov r0, #1\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x2d\n\t"
        "mov r6, #0\n\t"
        "strb r0, [r5]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "mov r2, #0\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r3, [r5]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r6, r0\n\t"
        "blt 6f\n\t"
        "sub r2, r0, #1\n\t"
    "6:\n\t"
        "str r2, [r4, #0x30]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x28\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r2]\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x21\n\t"
        "neg r1, r1\n\t"
        "and r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x49\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4a\n\t"
        "strb r7, [r0]\n\t"
        "add r0, #1\n\t"
        "strb r6, [r0]\n\t"
        "cmp r7, #0xff\n\t"
        "bne 7f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801191C\n\t"
    "7:\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r4, #0x20]\n\t"
        "ldr r1, [r1]\n\t"
        "ldrb r1, [r1, #0x14]\n\t"
        "bl sub_8006DF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "add r0, r4, #0\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_030012EC\n"
    "10: .4byte gUnknown_030012D0\n"
    "11: .4byte gUnknown_030012B8\n"
    );
}
#endif

/* sub_8011870: alternative to sub_80111B8 (game_loop29.c), called from
 * game_loop14.c "instead of sub_80111B8" per that file's own doc
 * comment. Same shape as sub_80111B8/sub_8011448's tail: PlaySfx(
 * gUnknown_030012BC, 8, 0x100), self->0x48 = 1, self->x -= self->0x4a<<8
 * (a fixed-offset nudge), self->0x3c = 0xa0, self->0x30 clamped from the
 * same self->0x20/self->0x2d table-lookup idiom, self->0x25 = 1, calls
 * sub_8007174(self, x>>8, y>>8, &outX, &outY) and re-derives self->x/
 * self->y plus self->0x40/self->0x44 the same way, with a fixed
 * 0xFFFFF000 (-0x1000) offset on both axes instead of a randomized one -
 * then, unlike sub_8011448/sub_80111B8, finishes with
 * sub_80284D4(gUnknown_03001318) instead of sub_80284A4.
 *
 * The fixed -0x1000 offsets go through `OrbitOffset` (an inline taking
 * the offset as a parameter): that is what makes old_agbcc reload the
 * 0xFFFFF000 constant from the pool for each axis instead of keeping one
 * copy across the call, as the ROM does. (The earlier "r7 hazard" note
 * was a symptom of compiling with the wrong compiler.) */
static inline s32 OrbitOffset(s32 pos, s32 off)
{
    return pos - off;
}

void sub_8011870(struct orbit_part *self)
{
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gUnknown_030012BC, 8, 0x100);
    self->state = 1;
    self->base.x -= self->mode << 8;
    self->timer = 0xa0;
    OrbitClampFrame(self);
    self->unk_25 = 1;

    sub_8007174(self, self->base.x >> 8, self->base.y >> 8, &outX, &outY);

    newX = outX << 8;
    self->base.x = newX;
    self->velX = -sub_80008F0(OrbitOffset(newX, 0x1000), 0x1400);
    newY = outY << 8;
    self->base.y = newY;
    self->velY = -sub_80008F0(OrbitOffset(newY, 0x1000), 0x1400);
    sub_80284D4(gUnknown_03001318);
}

/* sub_801191C: sibling of sub_8011870 above - sets self->0x48 = 3 (mode)
 * and self->0x49 = 0xa (a fixed countdown), no other side effects.
 * Already extern-declared as `void sub_801191C(struct actor *self)` in
 * src/graphics/actor_part39.c. Matched: trivial leaf, no push/pop, plain
 * field stores. */
void sub_801191C(struct actor *self)
{
    *((u8 *)self + 0x48) = 3;
    *((u8 *)self + 0x49) = 0xa;
}

/* sub_801192C: address-adjacent to sub_801191C, a small self->0x4b/
 * self->0x4a-driven table helper - copies a fixed 3-word table
 * (gStaticData_0816BF14) onto the stack, computes self->y from a
 * gStaticData_0816A820 (shared trig-ish table, see sub_8010F8C's own doc
 * comment) lookup at self->0x4b*4 scaled by sub_80008FC(...,0x3000)
 * against self->0x50 (the "home Y" sub_801173C/sub_8011548 both write),
 * then computes self->x from a second gStaticData_0816A820 lookup at
 * self->0x4b*2 scaled by sub_80008FC against the stack copy indexed by
 * self->0x4a-1, added to or subtracted from self->0x4c (the "home X")
 * depending on whether self->0x4a is 1, 2, or anything else (unchanged).
 * Called from sub_8011548's own default-mode tail above when
 * self->0x4a is nonzero.
 *
 * Same shape as sub_8011248 (game_loop52.c) with a 0x3000 y-scale: the
 * sine sample goes through one reused local, which old_agbcc keeps in r2
 * across both calls exactly like the ROM. */
struct three_words {
    s32 a[3];
};

extern struct three_words gStaticData_0816BF14;

void sub_801192C(struct orbit_part *self)
{
    struct three_words scales = gStaticData_0816BF14;
    s32 dy;
    s32 sn;

    sn = gStaticData_0816A820[self->phase * 4];
    dy = sub_80008FC(sn, 0x3000);
    self->base.y = self->anchor.y - dy;
    sn = gStaticData_0816A820[self->phase * 2];
    sn = sub_80008FC(sn, scales.a[self->mode - 1]);
    if (self->mode == 1)
        self->base.x = self->anchor.x - sn;
    else if (self->mode == 2)
        self->base.x = self->anchor.x + sn;
    else
        self->base.x = self->anchor.x;
}

/* The ROM pads this function to the next word with zeros, not a nop. */
asm(".align 2, 0");

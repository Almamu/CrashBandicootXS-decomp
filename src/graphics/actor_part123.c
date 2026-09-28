#include "core.h"

/* GitHub issue #9/#10, tail of the 0x0800B8DC-0x0800D040 cluster (see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the last raw file
 * in the cluster, `asm/code_3_2_17_cbf4.s` - `sub_800CBF4`,
 * `nullsub_15`, `nullsub_3`, `sub_800CCCC`, `sub_800CCE0`, ROM
 * 0x0800CBF4-0x0800CD00 (contiguous, no gap on either side -
 * `actor_part117.o`'s `sub_800CBD4` ends exactly where this file
 * starts, and `actor_part109.o`'s already-matched `sub_800CD00`
 * begins exactly where this file ends). Closes out the entire
 * 43-function cluster investigation that began with `sub_800B8DC`/
 * `sub_800BD48`. */

extern void *gUnknown_030012B4;
extern void *sub_803AD7C(void *arg0, void *fn);

/* `other` (the second argument - `self`, the first, is never read)
 * shares `struct actor`'s leading header layout (id @8, flags @0xc,
 * table @0x18, same `other->table`-relative `{s16 offset, void *fn}`
 * pair at +0x28/+0x2c that `src/system/game_loop8.c`'s `sub_802400C`
 * already reads via an identical `sub_803AD7C` hit-probe call), but
 * is read at +0x38 too - bigger than the 0x1c-byte `struct actor`, so
 * it gets its own `struct cbf4_other` below.
 *
 * Runs the "flag active + bitmap-set" idiom (`other->0xc |= 1`, then,
 * unless `other`'s id sentinel-checks as `0xFFFF`, sets bit
 * `other->8 & 0x1f` of word `other->8 >> 5` in the
 * `gUnknown_030012B4+0x108` bitmap) up to three times, independently
 * gated: once when the `sub_803AD7C` hit-probe against `other->table`'s
 * own +0x28/+0x2c pair reports *no* hit, once when `other->0xc` bit 3
 * is already set, and once when `other->0x38` is nonzero. This is the
 * exact idiom `actor_part27c.c`'s `sub_8018884` already matches as
 * real C (its own doc comment: "needs several `register asm` pins ...
 * without them this compiler ... folds the ROM's shift-setup pair ...
 * and CSEs away the ROM's second, seemingly redundant reload") - here
 * inlined three times over (the ROM has no `bl` to a shared helper).
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the whole file
 * matches under it, so actor_part123.o is in OLD_AGBCC_OBJS - the
 * constant-before-`ldrb` flag ORs are old_agbcc's tell). `MarkGone` is
 * an inline with the do/while(0) `SET_ID_BIT` that
 * actor_part_16048.c uses (its loop notes give the id reload). The
 * gone bit is set through a bitfield view of +0x0C and bit 3 is tested
 * through the byte view: that is what makes the second copy reuse the
 * tested byte and its `1` for the OR, in the ROM's registers. */
struct cbf4_other {
    u8 unk_00[8];
    u16 id;             // 0x08
    u8 unk_0A[2];
    union {
        u8 flags;       // 0x0C
        struct {
            u8 gone:1;
            u8 unk_1:7;
        } b;            // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    } f;
    u8 unk_10[8];
    u8 *table;          // 0x18
    u8 unk_1C[0x1C];
    u8 unk_38;          // 0x38
};

#define SET_ID_BIT(idExpr)                                                     \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gUnknown_030012B4;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= 1 << (_id - _word * 32);                                     \
    } while (0)

static inline void MarkGone(struct cbf4_other *t)
{
    t->f.b.gone = 1;
    if (t->id != 0xFFFF)
        SET_ID_BIT(t->id);
}

void sub_800CBF4(void *self, struct cbf4_other *other)
{
    if (!(u8)(s32)sub_803AD7C((u8 *)other + *(s16 *)(other->table + 0x28), *(void **)(other->table + 0x2c)))
        MarkGone(other);
    if ((other->f.flags >> 3) & 1)
        MarkGone(other);
    if (other->unk_38)
        MarkGone(other);
}

/* Genuine empty stubs (`bx lr`). */
void nullsub_15(void *self)
{
}

void nullsub_3(void *self)
{
}

extern u8 gStaticData_087E400C[];
extern void sub_800B8A8(void *self, s32 flags);

/* Sets `self+0xc`'s table pointer to `gStaticData_087E400C`, then
 * tail-calls `sub_800B8A8` - same double-set pattern as
 * `sub_8018858`/`sub_8017A78`/`sub_8017FD4`. */
void sub_800CCCC(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E400C;
    sub_800B8A8(self, flags);
}

extern void sub_800B8C8(void *self);

/* Resets via `sub_800B8C8`, re-points `self+0xc`'s table pointer at
 * `gStaticData_087E400C`, and runs `nullsub_3(self)` - the same
 * "reset via `sub_800B8C8`, re-point `self+0xc`, return `self`"
 * constructor shape already matched for `sub_801886C`/`sub_8018858`/
 * `sub_800CBD4`. */
void *sub_800CCE0(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E400C;
    nullsub_3(self);
    return self;
}

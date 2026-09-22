#include "core.h"
#include "actor.h"

extern void ***gUnknown_030012D0;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012E8;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F8;

extern struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern struct actor *sub_800CB40(void);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern u8 sub_8006DF8(void *cache, s32 recordId);
extern void *sub_8026EDC(s32 size);
extern void *sub_8017FE8(void *selfArg);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8008E94(void *manager, void *value);
extern u8 sub_80232A0(void *self);
extern s32 sub_801A878(u16 x, u16 y, u16 w, u16 h, s32 id);
extern void sub_80234F4(void *self, s32 value);
extern s32 sub_801B984(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_801F680(void);
extern void *sub_800FF0C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u8 type);

/* Sets `part->field_29`'s low nibble to `sub_800815C(part)`'s result,
 * keeping the high nibble - same idiom as `UPDATE_PART_FRAME_NIBBLE` in
 * src/graphics/graphics_loading_21d80.c/trigger_effect.c (not shared via
 * a header since each file only needs it locally, per those files' own
 * comment). */
#define UPDATE_PART_FRAME_NIBBLE(partPtr) \
    do { \
        register s32 _ret asm("r0") = sub_800815C(partPtr); \
        register u8 *_addr asm("r2") = (u8 *)(partPtr) + 0x29; \
        register s32 _mask asm("r1"); \
        register u8 _byte asm("r3"); \
        _mask = 0xf; \
        _ret &= _mask; \
        asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r" (_mask)); \
        _byte = *_addr; \
        _mask &= _byte; \
        _mask |= _ret; \
        *_addr = _mask; \
    } while (0)

/* `part->flags = (part->flags & 0x7f) & -5;` written as two separate
 * `asm volatile`-anchored AND steps instead of plain C - this compiler's
 * optimizer otherwise constant-folds two sequential AND-by-immediate
 * operations into a single `and`/`0x7b` (`0x7f & -5 == 0x7b`), which the
 * ROM's own codegen never does (it keeps them as two distinct `ands`
 * against freshly computed `-5`, plus a persistent `r5`/`r6`-cached `0`
 * for the two write-sites this pattern's callers also share). */
#define CLEAR_FLAGS_7F_AND_NEG5(partPtr) \
    do { \
        register s32 _acc asm("r0") = 0x7f; \
        register u8 _flags asm("r1") = (partPtr)->flags; \
        register s32 _mask2 asm("r1"); \
        _acc &= _flags; \
        asm volatile("mov r1, #5\n\tneg r1, r1\n\t" : "=r" (_mask2)); \
        _acc &= _mask2; \
        (partPtr)->flags = _acc; \
    } while (0)

/* One of the "two-line text popup" family (docs/rom_map.md, the
 * `sub_801FDEC`-adjacent siblings documented in
 * docs/matching/issue-31-graphics-loading.md), but with a variant tail:
 * instead of the `gUnknown_030012B4`-rooted "collected bits" pack this
 * family's other instances use, this one builds the part via the
 * standard `sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio, looks up
 * a record id through its own `+0x20` table pointer (double-dereferenced:
 * `*(*(part->0x20))` at offset `0x14`) to call `sub_8006DF8` against
 * `gUnknown_030012B8`'s tile-asset cache for the frame-nibble value,
 * unconditionally clears bits 4/5 of `part->0x28` (no OR - unlike the
 * lookup-table pack `sub_801FDEC` uses), fires a single `sub_803AD80`
 * animation-table trampoline (not twice), and finishes with a
 * three-step flags mask (`(((flags & 0x7f) & -5) & -0x41) | 0x10`).
 * `arg0` stays `u32` per the family's usual deferred-truncation
 * convention (truncated only at the `sub_8009ED0` call site). Every
 * `asm volatile` block below spells out a two-step mask/negative-
 * constant idiom this compiler's optimizer would otherwise fold or
 * reorder - same technique catalogued in
 * docs/matching/issue-31-graphics-loading.md for `sub_801FDEC`. */
void sub_8021668(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct actor *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    void *tableBase = **gUnknown_030012D0;

    *(void **)((u8 *)part + 0x20) = (u8 *)tableBase + 0x168;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    {
        register u8 zeroVal asm("r0") = 0;
        register u8 *addr2d asm("r1") = (u8 *)part + 0x2d;

        *addr2d = zeroVal;
    }
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);

    {
        register void *cache asm("r0") = gUnknown_030012B8;
        register void *tmp asm("r1") = *(void **)((u8 *)part + 0x20);
        register u8 recordId asm("r1");
        register s32 ret asm("r0");
        register u8 *addr asm("r2");
        register s32 mask asm("r1");
        register u8 byte asm("r3");

        tmp = *(void **)tmp;
        recordId = *((u8 *)tmp + 0x14);
        ret = sub_8006DF8(cache, recordId);
        addr = (u8 *)part + 0x29;
        mask = 0xf;
        ret &= mask;
        asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r" (mask));
        byte = *addr;
        mask &= byte;
        mask |= ret;
        *addr = mask;

        {
            register u8 *addr28 asm("r2") = addr - 1;
            register s32 acc asm("r0") = -0x11;
            register u8 byte28 asm("r1");
            register s32 val asm("r1");

            byte28 = *addr28;
            acc &= byte28;
            asm volatile("mov r1, #0x21\n\tneg r1, r1\n\t" : "=r" (val));
            acc &= val;
            *addr28 = acc;
        }
    }

    {
        void *hdr = sub_8017FE8(sub_8026EDC(0x24));
        u8 *table;

        *(void **)((u8 *)part + 0x44) = hdr;
        table = *(u8 **)((u8 *)hdr + 0xc);
        sub_803AD80((u8 *)hdr + *(s16 *)(table + 0x18), part, *(void **)(table + 0x1c));
    }

    *((u8 *)part + 0xa) = 1;
    {
        register s32 acc asm("r0") = 0x7f;
        register u8 flagsByte asm("r1") = part->flags;
        register s32 mask2 asm("r1");

        acc &= flagsByte;
        asm volatile("mov r1, #5\n\tneg r1, r1\n\t" : "=r" (mask2));
        acc &= mask2;
        asm volatile("sub r1, #0x3c\n\t" : "+r" (mask2));
        acc &= mask2;
        acc |= 0x10;
        part->flags = acc;
    }
    sub_8008E94(gUnknown_030012F0, part);
}

/* Same overall shape as `sub_8021D80`'s family
 * (src/graphics/graphics_loading_21d80.c) - `sub_8008434` constructor,
 * table offset (here `0x21c`), OAM trio, frame-nibble update - but
 * registers into `gUnknown_030012F8`'s manager instead of `EC`, and adds
 * a `part->flags = (flags & 0x7f) & -5;` step
 * (`CLEAR_FLAGS_7F_AND_NEG5` above) not present in that family. The `0`
 * tag/`field_0A` value is cached once in `r5` (a genuine
 * `register u8 zero asm("r5")`) and reused for both the `+0x2d` and
 * `+0xa` writes, matching the ROM's own register reuse - assigning it
 * only after the table-offset store (not at declaration) is what keeps
 * the truncation-prologue instruction order matching the ROM's. */
void sub_8021748(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 zero asm("r5");
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x21c;
    zero = 0;
    *((u8 *)part + 0x2d) = zero;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    CLEAR_FLAGS_7F_AND_NEG5(part);
    *((u8 *)part + 0xa) = zero;
    sub_8008E94(gUnknown_030012F8, part);
}

/* Same shape as `sub_8021748` above but skips the OAM trio entirely (no
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C`, no `+0x2d` write) and
 * writes a fresh `0` (not cached) to `+0xa`, since it's the field's only
 * write in this variant. */
void sub_80217D0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x21c;
    UPDATE_PART_FRAME_NIBBLE(part);
    CLEAR_FLAGS_7F_AND_NEG5(part);
    *((u8 *)part + 0xa) = 0;
    sub_8008E94(gUnknown_030012F8, part);
}

/* Same shape as `sub_8021748` above (OAM trio, cached-zero `r5`), table
 * offset `0x210` instead of `0x21c`. */
void sub_802183C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 zero asm("r5");
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x210;
    zero = 0;
    *((u8 *)part + 0x2d) = zero;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    CLEAR_FLAGS_7F_AND_NEG5(part);
    *((u8 *)part + 0xa) = zero;
    sub_8008E94(gUnknown_030012F8, part);
}

/* Plain `sub_801A878` trampoline (docs/rom_map.md; same callee as
 * trigger_effect.c's twin family), id `8`. */
void sub_80218C4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801A878(arg0, arg1, arg2, arg3, 8);
}

/* Plain `sub_801A878` trampoline, id `6`. */
void sub_80218E8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801A878(arg0, arg1, arg2, arg3, 6);
}

/* Gated `sub_801A878` dispatcher: id `7` if
 * `sub_80232A0(gUnknown_030012C0)` is true or `gUnknown_030012C0+0x8c`
 * is nonzero, else id `5` - same OR-gated shape as trigger_effect.c's
 * twin family's sound-id choice, but feeding `sub_80234F4` (the
 * `self->0x1b8` setter, src/system/game_loop10.c) with the result
 * instead of `sub_80234E8`. Parked as NAKED asm: semantics fully
 * understood (matches the twin family's gating idiom exactly), but the
 * `arg0`-`arg3` parameter-home registers (`r5`-`r8`, mixed
 * deferred/immediate truncation) and the `id` register's exact
 * scheduling relative to the stack-argument store never converged
 * through plain C or register pins - same class of gcc-2.9
 * register-allocation gap documented for the twin family in
 * docs/matching/issue-31-graphics-loading.md (parking rather than
 * spending unlimited iteration on a single-instruction scheduling
 * gap). */
NAKED void sub_802190C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r3, r3, #0x10\n\t"
        "mov r8, r3\n\t"
        "ldr r4, 1f\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_80232A0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "ldr r0, [r4]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
    "2:\n\t"
        "lsl r0, r5, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #7\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012C0\n"
    "3:\n\t"
        "lsl r0, r5, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #5\n\t"
    "4:\n\t"
        "str r1, [sp]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "mov r3, r8\n\t"
        "bl sub_801A878\n\t"
        "add r1, r0, #0\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80234F4\n\t"
        "add sp, #4\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012C0\n"
    );
}

/* Plain `sub_801A878` trampoline, id `2`. */
void sub_8021974(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801A878(arg0, arg1, arg2, arg3, 2);
}

/* Plain `sub_801A878` trampoline, id `1`. */
void sub_8021998(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801A878(arg0, arg1, arg2, arg3, 1);
}

/* Plain `sub_801A878` trampoline, id `0`. */
void sub_80219BC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801A878(arg0, arg1, arg2, arg3, 0);
}

/* Plain tail-call trampoline to `sub_801B984` (still raw). */
void sub_80219E0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801B984(arg0, arg1, arg2, arg3);
}

/* Empty stub. */
void nullsub_21(void)
{
}

/* Allocates an 0x28-byte block via `sub_8026EDC` purely for a side
 * effect (its return value is discarded - the ROM never saves it before
 * the next call, same "call purely for a side effect" idiom
 * docs/naming.md documents), then builds the real object via
 * `sub_800CB40` (a different constructor than every other sibling in
 * this chunk - no `sub_8009ED0`/`sub_8008434`). Wires a fixed
 * `sub_801F680` callback into `+0x1c`, `+0x20 = 0x78`, `+0x24 = 0`
 * (cached in `r2`, computed right after the object pointer but stored
 * last - matches the ROM's own scheduling), writes a Q8.8 `{x, y}` pair,
 * sets flag bit 4, and registers into `gUnknown_030012E8`'s manager. */
void sub_8021A00(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct actor *obj;
    register s32 zero asm("r2");

    sub_8026EDC(0x28);
    obj = sub_800CB40();
    zero = 0;
    *(void (**)(void))((u8 *)obj + 0x1c) = sub_801F680;
    *(s32 *)((u8 *)obj + 0x20) = 0x78;
    *(s32 *)((u8 *)obj + 0x24) = zero;
    obj->x = (s32)arg1 << 8;
    obj->y = (s32)arg2 << 8;
    {
        register s32 mask asm("r0") = 0x10;
        register u8 old asm("r2") = obj->flags;

        mask |= old;
        obj->flags = mask;
    }
    sub_8008E94(gUnknown_030012E8, obj);
}

/* Plain `sub_800FF0C` entity-constructor trampoline (docs/rom_map.md;
 * same dispatch family as src/graphics/graphics_loading_21bfc.c's
 * types 1-7), type `0x12`. */
void sub_8021A4C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0x12);
}

/* Plain `sub_800FF0C` trampoline, type `0x11`. */
void sub_8021A70(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0x11);
}

/* Plain `sub_800FF0C` trampoline, type `0x10`. */
void sub_8021A94(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0x10);
}

/* Plain `sub_800FF0C` trampoline, type `0xf`. */
void sub_8021AB8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xf);
}

/* Plain `sub_800FF0C` trampoline, type `0xe`. */
void sub_8021ADC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xe);
}

/* Plain `sub_800FF0C` trampoline, type `0xd`. */
void sub_8021B00(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xd);
}

/* Plain `sub_800FF0C` trampoline, type `0xc`. */
void sub_8021B24(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xc);
}

/* Plain `sub_800FF0C` trampoline, type `0xb`. */
void sub_8021B48(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xb);
}

/* Plain `sub_800FF0C` trampoline, type `0xa`. */
void sub_8021B6C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 0xa);
}

/* Plain `sub_800FF0C` trampoline, type `9`. */
void sub_8021B90(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 9);
}

/* Plain `sub_800FF0C` trampoline, type `8`. */
void sub_8021BB4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 8);
}

/* Plain `sub_800FF0C` trampoline, type `7`. Last function in this ROM
 * region - `asm/code_3_2_17_21280.s` (still-raw text past this point
 * used to continue here) now ends right before this file's span, at
 * `sub_802155C`'s literal pool. */
void sub_8021BD8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_800FF0C(arg0, arg1, arg2, arg3, 7);
}

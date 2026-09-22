#include "core.h"
#include "memory.h"

/* gUnknown_03001400[] - the category's "sub_effect_table" runtime array,
 * category_descriptor.sub_effect_table (include/actor_anim.h, offset
 * 0x14), stride 0x14 bytes - see docs/rom_map.md's "sub_802A5xx
 * siblings pin down sub_effect_table's runtime shape" finding (a real
 * ROM data dump confirmed record 0 doubles as a combined header+entry,
 * its own field_04 holding the entry count). Set up by
 * SelectActorCategory (still raw).
 *
 * sub_802A504/sub_802A51C both read one record *past* the index they're
 * given (their address math works out to base+idx*0x14+0x18 and
 * +0x14 respectively, i.e. `&record[idx+1].field_04`/`&record[idx+1].
 * field_00` - not a distinct field of record[idx] itself, confirmed by
 * matching the exact ROM instruction order: this compiler computes the
 * base+constant step before the base+idx*stride step for these two
 * specifically, unlike every plain record[idx].field access elsewhere
 * in this file, which folds its constant straight into the load). */
struct sub_effect_record {
    s32 field_00;   /* 0x00 */
    s32 field_04;   /* 0x04 */
    u8 variantA;    /* 0x08 */
    u8 variantB;    /* 0x09 */
    u8 variantC;    /* 0x0a */
    u8 pad_0b;
    s32 offsetX;    /* 0x0c - stored raw, <<8 by the accessors that read it */
    s32 offsetY;    /* 0x10 - stored raw, <<8 by the accessors that read it */
};
COMPILE_TIME_ASSERT(sizeof(struct sub_effect_record) == 0x14);

extern struct sub_effect_record *gUnknown_03001400;
extern s32 gUnknown_03001420;
extern s32 gUnknown_03001424;
extern s32 gUnknown_03001410;
extern u8 gUnknown_0300141C;
extern u8 gUnknown_03001414;
extern void *gUnknown_030012C0;
extern void *gUnknown_03000884;
/* The category vtable object (include/actor_anim.h's 13-fn-pointer
 * `struct category_vtable`) only has slots 0-6 confirmed as real
 * per-type behavior entries (docs/rom_map.md) - slots past that
 * (byte offsets 0x10/0x14/0x18/0x2c/0x30, i.e. indices 4/5/6/11/12)
 * are read here as plain data values passed straight into
 * `sub_803AD78`, never called, and their real role/type isn't pinned
 * down yet - kept as raw-offset `void *` accesses rather than forcing
 * them into the `fn[13]` function-pointer typing, per docs/workflow.md
 * step 7 (don't force a struct/field guess where the shape isn't
 * actually known). Set by SelectActorCategory (still raw). */
extern void *gUnknown_03001418;
extern void **gUnknown_03001408;

extern s32 sub_803AD78(void *arg);
extern void sub_803AD80(s32 x, s32 mode, s32 color);
extern void sub_802F4C0(void *arg);
extern void sub_802BD18(void *arg);

/* Trivial getter - the big loading-loop call counter set by
 * InitActorCategory's own loop tail (still raw). */
s32 sub_802A4D4(void)
{
    return gUnknown_03001424;
}

/* Trivial getter, Q8.8-converted. */
s32 sub_802A4E0(void)
{
    return gUnknown_03001420 << 8;
}

void sub_802A4EC(void)
{
    gUnknown_0300141C = 0;
}

void sub_802A4F8(void)
{
    gUnknown_0300141C = 1;
}

/* Reads the *next* record's field_04 (see struct comment above). Written
 * as "cache base pointer, then compute idx*stride, then add the
 * constant record-boundary offset to the base before combining" to
 * match this compiler's exact instruction order for this specific
 * shape - see struct comment. */
s32 sub_802A504(s32 idx)
{
    u8 *base = (u8 *)gUnknown_03001400;
    s32 off = idx * 0x14;

    base = base + 0x18;
    return *(s32 *)(base + off);
}

/* Reads the *next* record's field_00, offset by gUnknown_03001420,
 * Q8.8-converted. Same instruction-order shape as sub_802A504. */
s32 sub_802A51C(s32 idx)
{
    u8 *base = (u8 *)gUnknown_03001400;
    s32 off = idx * 0x14;

    base = base + 0x14;
    return (*(s32 *)(base + off) + gUnknown_03001420) << 8;
}

s32 sub_802A540(s32 idx)
{
    u8 *base = (u8 *)gUnknown_03001400;
    s32 off = idx * 0x14;

    base = base + 0x10;
    return *(s32 *)(base + off) << 8;
}

s32 sub_802A558(s32 idx)
{
    u8 *base = (u8 *)gUnknown_03001400;
    s32 off = idx * 0x14;

    base = base + 0xc;
    return *(s32 *)(base + off) << 8;
}

/* Mode-selects one of the record's three variant bytes (normal /
 * "paused" via gUnknown_030012C0+0x8c / gUnknown_03001414), then
 * subtracts 0x20 - see docs/rom_map.md's "sub_802A5xx siblings" entry.
 * This one *is* record[idx] itself (not idx+1) and its fields all fold
 * straight into the load/ldrb offsets, matching the ROM exactly. */
s32 sub_802A570(s32 idx)
{
    struct sub_effect_record *base = gUnknown_03001400;
    s32 off = idx * 0x14;
    struct sub_effect_record *record = (struct sub_effect_record *)((u8 *)base + off);
    u8 v;

    v = record->variantA;
    if (*((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
        v = record->variantB;
    } else if (gUnknown_03001414 != 0) {
        v = record->variantC;
    }
    return v - 0x20;
}

/* The natural `sub_803AD78(...) ^ 1` C phrasing has this compiler
 * materialize the constant `1` into the destination register and the
 * call's result into a second register (an extra `adds r1, r0, #0`
 * copy the ROM doesn't have) - the ROM instead reuses r0 (the call
 * result) directly as the EOR destination. Anchored with inline asm
 * for the exact two-instruction ROM sequence, per docs/workflow.md
 * step 3. */
s32 sub_802A5AC(void)
{
    s32 result = sub_803AD78(*(void **)((u8 *)gUnknown_03001418 + 0x30));
    asm volatile("movs r1, #1\n\teor r0, r1" : "+r"(result) :: "r1");
    return result;
}

void sub_802A5C4(void)
{
    sub_803AD78(*(void **)((u8 *)gUnknown_03001418 + 0x18));
    sub_803AD78(*(void **)((u8 *)gUnknown_03001418 + 0x2c));
}

/* Draws every actor's marker via its shared self+0x50 record's `+8`
 * (s16 x-offset)/`+0xc` (color) fields through sub_803AD80, walking the
 * gUnknown_03000884 circular list starting at head->next and handling
 * the head itself last, then releases the temporary pointer array this
 * category's loading loop built (gUnknown_03001408).
 *
 * The address of gUnknown_03000884 is cached across the whole function
 * (survives the loop) while its *value* is deliberately re-read fresh
 * both for the loop's own exit test and again after the loop - the
 * ROM's own register allocation keeps a callee-saved copy of the
 * address (r5) but always reloads the value through it, rather than
 * caching the value itself. A plain C `void *head = gUnknown_03000884;`
 * lets this compiler fold the address-load straight into r5 in one
 * instruction; the ROM instead loads it into r0 first and copies it to
 * r5 as a second step (visible in the extra `adds r5, r0, #0`) -
 * anchored with inline asm for that exact three-instruction prologue,
 * per docs/workflow.md step 3. */
void sub_802A5E4(void)
{
    register void **headAddr asm("r5");
    register void *head asm("r1");
    void *cur;
    void *next;

    if (*(void **)((u8 *)gUnknown_03001418 + 0x14) != NULL) {
        sub_803AD78(*(void **)((u8 *)gUnknown_03001418 + 0x14));
    }

    {
        /* The address is fed in as a plain input operand (rather than
         * an `ldr rX, =gUnknown_03000884` pseudo-op inside the asm
         * text) so it reuses this compiler's own literal-pool tracking
         * instead of GAS's separate automatic literal pool - the
         * latter doesn't know this is the same constant gcc's own pool
         * already emits for the loop-tail reload below, and silently
         * duplicates it as 4 extra bytes. `headAddr`'s own copy is
         * deferred until *after* `cur` is computed below - the ROM
         * doesn't copy `addr` into its callee-saved register until
         * right before the loop condition, not immediately after the
         * load. */
        register void *addr asm("r0") = &gUnknown_03000884;
        asm volatile(
            "ldr %0, [%1]\n\t"
            : "=r"(head)
            : "r"(addr)
        );

        cur = *(void **)((u8 *)head + 0x4c);
        asm volatile(
            "add %0, %1, #0\n\t"
            : "=r"(headAddr)
            : "r"(addr)
        );
    }
    if (cur != head) {
        do {
            next = *(void **)((u8 *)cur + 0x4c);
            if (cur != NULL) {
                void *rec = *(void **)((u8 *)cur + 0x50);
                s32 x = (s32)cur + *(s16 *)((u8 *)rec + 8);
                s32 color = *(s32 *)((u8 *)rec + 0xc);
                sub_803AD80(x, 3, color);
            }
            cur = next;
        } while (cur != gUnknown_03000884);
    }

    cur = *headAddr;
    if (cur != NULL) {
        void *rec = *(void **)((u8 *)cur + 0x50);
        s32 x = (s32)cur + *(s16 *)((u8 *)rec + 8);
        s32 color = *(s32 *)((u8 *)rec + 0xc);
        sub_803AD80(x, 3, color);
    }

    mem_free((u8 *)gUnknown_03001408);
}

void sub_802A650(void)
{
    if (*(void **)((u8 *)gUnknown_03001418 + 0x10) != NULL) {
        sub_803AD78(*(void **)((u8 *)gUnknown_03001418 + 0x10));
    }
}

void sub_802A668(s32 arg0)
{
    gUnknown_03001410 = arg0;
}

/* NAKED - plain trampolines identical in shape to already-matched
 * siblings elsewhere in this project (e.g. actor_part50.c's
 * sub_802A69C), but this compiler's epilogue register allocator picks
 * `r1` for these two specific functions' `pop`/`bx` pair instead of the
 * usual `r0` - a parity-like quirk tied to this translation unit's
 * cumulative pseudo-register count rather than anything controllable
 * per-function (confirmed: register-pinning the argument, and other
 * plain-C phrasings, all still compile `pop {r0}`/`bx r0`). Anchored as
 * NAKED rather than chasing the exact trigger further. */
NAKED void sub_802A674(void)
{
    asm(
        "push {lr}\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_802F4C0\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_03000884\n"
    );
}

/* NAKED - see sub_802A674 above for why. */
NAKED void sub_802A688(void)
{
    asm(
        "push {lr}\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_802BD18\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_03000884\n"
    );
}

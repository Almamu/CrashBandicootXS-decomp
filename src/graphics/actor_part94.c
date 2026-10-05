#include "core.h"
#include "memory.h"

/* gActorSpawnTable[] - the category's "spawnTable" runtime array,
 * category_descriptor.spawnTable (include/actor_anim.h, offset
 * 0x14), stride 0x14 bytes - see docs/rom_map.md's "sub_802A5xx
 * siblings pin down spawnTable's runtime shape" finding (a real
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
    u8 kind;    /* 0x08 */
    u8 altKind;    /* 0x09 */
    u8 bonusKind;    /* 0x0a */
    u8 pad_0b;
    s32 offsetX;    /* 0x0c - stored raw, <<8 by the accessors that read it */
    s32 offsetY;    /* 0x10 - stored raw, <<8 by the accessors that read it */
};
COMPILE_TIME_ASSERT(sizeof(struct sub_effect_record) == 0x14);

/* Not at the top: COMPILE_TIME_ASSERT names its typedef after __LINE__,
 * and moving the assert above to line 31 clashes with memory.h's. */
#include "actor_self.h"

extern struct sub_effect_record *gActorSpawnTable;
extern s32 gActorSpawnOffset;
extern s32 gActorCategoryFrameCount;
extern s32 gActorCategoryExitStatus;
extern u8 gActorSpawnsPaused;
extern u8 gUnknown_03001414;
extern void *gLevelState;
extern void *gActorList;
/* The category vtable object (include/actor_anim.h's 13-fn-pointer
 * `struct category_vtable`) only has slots 0-6 confirmed as real
 * per-type behavior entries (docs/rom_map.md) - slots past that
 * (byte offsets 0x10/0x14/0x18/0x2c/0x30, i.e. indices 4/5/6/11/12)
 * are read here as plain data values passed straight into
 * `_call_via_r0`, never called, and their real role/type isn't pinned
 * down yet - kept as raw-offset `void *` accesses rather than forcing
 * them into the `fn[13]` function-pointer typing, per docs/workflow.md
 * step 7 (don't force a struct/field guess where the shape isn't
 * actually known). Set by SelectActorCategory (still raw). */
extern void *gActorCategoryVtable;
extern void **gActorDrawList;

extern s32 _call_via_r0(void *arg);
extern void _call_via_r2(s32 self, s32 arg, s32 fn);
extern s32 IsJetpackPauseLocked(void *arg);
extern s32 IsPolarPauseLocked(void *arg);

/* Trivial getter - the big loading-loop call counter set by
 * InitActorCategory's own loop tail (still raw). */
s32 GetActorCategoryFrameCount(void)
{
    return gActorCategoryFrameCount;
}

/* Trivial getter, Q8.8-converted. */
s32 GetActorSpawnOffset(void)
{
    return gActorSpawnOffset << 8;
}

void ResumeActorSpawns(void)
{
    gActorSpawnsPaused = 0;
}

void PauseActorSpawns(void)
{
    gActorSpawnsPaused = 1;
}

/* Reads the *next* record's field_04 (see struct comment above). Written
 * as "cache base pointer, then compute idx*stride, then add the
 * constant record-boundary offset to the base before combining" to
 * match this compiler's exact instruction order for this specific
 * shape - see struct comment. */
s32 sub_802A504(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x18;
    return *(s32 *)(base + off);
}

/* Reads the *next* record's field_00, offset by gActorSpawnOffset,
 * Q8.8-converted. Same instruction-order shape as sub_802A504. */
s32 sub_802A51C(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x14;
    return (*(s32 *)(base + off) + gActorSpawnOffset) << 8;
}

s32 sub_802A540(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0x10;
    return *(s32 *)(base + off) << 8;
}

s32 sub_802A558(s32 idx)
{
    u8 *base = (u8 *)gActorSpawnTable;
    s32 off = idx * 0x14;

    base = base + 0xc;
    return *(s32 *)(base + off) << 8;
}

/* Mode-selects one of the record's three variant bytes (normal /
 * "paused" via gLevelState+0x8c / gUnknown_03001414), then
 * subtracts 0x20 - see docs/rom_map.md's "sub_802A5xx siblings" entry.
 * This one *is* record[idx] itself (not idx+1) and its fields all fold
 * straight into the load/ldrb offsets, matching the ROM exactly. */
s32 sub_802A570(s32 idx)
{
    struct sub_effect_record *base = gActorSpawnTable;
    s32 off = idx * 0x14;
    struct sub_effect_record *record = (struct sub_effect_record *)((u8 *)base + off);
    u8 v;

    v = record->kind;
    if (*((u8 *)gLevelState + 0x8c) != 0) {
        v = record->altKind;
    } else if (gUnknown_03001414 != 0) {
        v = record->bonusKind;
    }
    return v - 0x20;
}

/* The natural `_call_via_r0(...) ^ 1` C phrasing has this compiler
 * materialize the constant `1` into the destination register and the
 * call's result into a second register (an extra `adds r1, r0, #0`
 * copy the ROM doesn't have) - the ROM instead reuses r0 (the call
 * result) directly as the EOR destination. Anchored with inline asm
 * for the exact two-instruction ROM sequence, per docs/workflow.md
 * step 3. */
s32 CanPauseActorCategory(void)
{
    s32 result = _call_via_r0(*(void **)((u8 *)gActorCategoryVtable + 0x30));
    asm volatile("movs r1, #1\n\teor r0, r1" : "+r"(result) :: "r1");
    return result;
}

void ReloadActorCategoryGraphics(void)
{
    _call_via_r0(*(void **)((u8 *)gActorCategoryVtable + 0x18));
    _call_via_r0(*(void **)((u8 *)gActorCategoryVtable + 0x2c));
}

/* Calls every actor's `destroy` virtual method with 3 (the "destroy" call,
 * through the _call_via_r2 call-via-r2 thunk), walking the
 * gActorList circular list along its +0x4C links starting after
 * the head and handling the head itself last, then releases the
 * temporary pointer array this category's loading loop built
 * (gActorDrawList).
 *
 * The address of gActorList is cached across the whole function
 * (survives the loop) while its *value* is deliberately re-read fresh
 * both for the loop's own exit test and again after the loop - the
 * ROM's own register allocation keeps a callee-saved copy of the
 * address (r5) but always reloads the value through it, rather than
 * caching the value itself. A plain C `void *head = gActorList;`
 * lets this compiler fold the address-load straight into r5 in one
 * instruction; the ROM instead loads it into r0 first and copies it to
 * r5 as a second step (visible in the extra `adds r5, r0, #0`) -
 * anchored with inline asm for that exact three-instruction prologue,
 * per docs/workflow.md step 3. */
void DestroyAllActors(void)
{
    register void **headAddr asm("r5");
    register void *head asm("r1");
    struct actor_self *cur;
    struct actor_self *next;

    if (*(void **)((u8 *)gActorCategoryVtable + 0x14) != NULL) {
        _call_via_r0(*(void **)((u8 *)gActorCategoryVtable + 0x14));
    }

    {
        /* The address is fed in as a plain input operand (rather than
         * an `ldr rX, =gActorList` pseudo-op inside the asm
         * text) so it reuses this compiler's own literal-pool tracking
         * instead of GAS's separate automatic literal pool - the
         * latter doesn't know this is the same constant gcc's own pool
         * already emits for the loop-tail reload below, and silently
         * duplicates it as 4 extra bytes. `headAddr`'s own copy is
         * deferred until *after* `cur` is computed below - the ROM
         * doesn't copy `addr` into its callee-saved register until
         * right before the loop condition, not immediately after the
         * load. */
        register void *addr asm("r0") = &gActorList;
        asm volatile(
            "ldr %0, [%1]\n\t"
            : "=r"(head)
            : "r"(addr)
        );

        cur = ACTOR_LINK_NEXT((struct actor_self *)head);
        asm volatile(
            "add %0, %1, #0\n\t"
            : "=r"(headAddr)
            : "r"(addr)
        );
    }
    if (cur != head) {
        do {
            next = ACTOR_LINK_NEXT(cur);
            if (cur != NULL) {
                struct actor_vtable *rec = cur->vtable;
                s32 x = (s32)cur + rec->destroy.thisOffset;
                s32 fn = (s32)rec->destroy.fn;
                _call_via_r2(x, 3, fn);
            }
            cur = next;
        } while (cur != gActorList);
    }

    cur = *headAddr;
    if (cur != NULL) {
        struct actor_vtable *rec = cur->vtable;
        s32 x = (s32)cur + rec->destroy.thisOffset;
        s32 fn = (s32)rec->destroy.fn;
        _call_via_r2(x, 3, fn);
    }

    mem_free((u8 *)gActorDrawList);
}

void UpdateActorCategoryBg2(void)
{
    if (*(void **)((u8 *)gActorCategoryVtable + 0x10) != NULL) {
        _call_via_r0(*(void **)((u8 *)gActorCategoryVtable + 0x10));
    }
}

void SetActorCategoryExitStatus(s32 arg0)
{
    gActorCategoryExitStatus = arg0;
}

/* Both forward the callee's result untouched: the callees are `u8`
 * (actor_part44.c / actor_part107.c), but this file's source saw them
 * returning `int`, so there is no re-narrowing and the epilogue returns
 * through `pop {r1}`. The old NAKED note blamed a TU-wide allocator
 * quirk; it was just the missing return value. */
s32 JetpackIsPauseLocked(void)
{
    return IsJetpackPauseLocked(gActorList);
}

s32 PolarIsPauseLocked(void)
{
    return IsPolarPauseLocked(gActorList);
}

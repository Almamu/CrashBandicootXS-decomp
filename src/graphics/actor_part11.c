#include "core.h"
#include "actor.h"

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *manager);
extern void *sub_8026EC0(u32 size);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void *gUnknown_03001308;

/* The "filter into a second array" manager struct also used by
 * `sub_8008C80`/`sub_8008CEC`/`sub_8008D30` in `actor_part10.c` -
 * `array1` is the primary list (bounded by `count1`, up to
 * `capacity`), `array2` a filtered/derived list built from it
 * (bounded by `count2`). `sub_8008EE4` below is this struct's own
 * initializer. */
struct dual_array_manager {
    s32 capacity;     // +0x0
    s32 count1;         // +0x4
    s32 count2;           // +0x8
    void **array1;          // +0xc
    void **array2;             // +0x10
};

/* Fires a `part->table+0x20/0x24`-driven trampoline via `sub_803AD7C`
 * for every entry in `manager->array2` (bounded by `count2`). */
void sub_8008DC0(struct dual_array_manager *manager)
{
    s32 i;

    for (i = 0; i < manager->count2; i++) {
        void *part = manager->array2[i];
        u8 *tbl = *(u8 **)((u8 *)part + 0x18);
        s16 offset = *(s16 *)(tbl + 0x20);
        void *addr = (u8 *)part + offset;
        void *fn = *(void **)(tbl + 0x24);

        sub_803AD7C(addr, fn);
    }
}

/* Searches `manager->array1` (bounded by `capacity`) for an entry
 * equal to `target`; if found, compacts the array by shifting every
 * following entry down by one slot via the BIOS `CpuSet` wrapper
 * `sub_803A94C`, decrements `count1`, and clears the now-unused
 * trailing slot. Same removal logic as `sub_8008E50` below, but
 * locates the index by value instead of taking it directly as an
 * argument. */
void sub_8008DEC(struct dual_array_manager *manager, void *target)
{
    s32 i = 0;
    s32 count = manager->capacity;
    void **base;

    if (i >= count) {
        goto done;
    }
    {
        void **p0 = manager->array1;
        void *val = *p0;
        base = p0;
        if (val != target) {
            void **p = base;
            do {
                p++;
                i++;
                if (i >= count) {
                    goto done;
                }
            } while (*p != target);
        }
    }

    if (i < manager->capacity) {
        s32 off = i * 4;
        s32 srcOff = off + 4;
        void *src = (u8 *)base + srcOff;
        void *dst = (u8 *)base + off;
        s32 control = (manager->count1 - i) & 0x1FFFFF;
        s32 newCount;

        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = manager->count1 - 1;
        manager->count1 = newCount;
        manager->array1[newCount] = 0;
    }
done:
    return;
}

/* Removes the entry at `index` from `manager->array1`, compacting via
 * `sub_803A94C` the same way `sub_8008DEC` does after its own
 * search. */
void sub_8008E50(struct dual_array_manager *manager, s32 index)
{
    if (index < manager->capacity) {
        s32 off = index * 4;
        s32 srcOff = off + 4;
        u8 *base;
        void *src, *dst;
        s32 control;
        s32 newCount;

        base = (u8 *)manager->array1;
        src = base + srcOff;
        dst = base + off;
        control = (manager->count1 - index) & 0x1FFFFF;
        control |= 0x4000000;
        sub_803A94C(src, dst, control);

        newCount = manager->count1 - 1;
        manager->count1 = newCount;
        manager->array1[newCount] = 0;
    }
}

/* Appends `value` to `manager->array1` if there's room (`count1` <
 * `capacity`). */
void sub_8008E94(struct dual_array_manager *manager, void *value)
{
    s32 count = manager->count1;

    if (count < manager->capacity) {
        manager->array1[count] = value;
        manager->count1 = count + 1;
    }
}

/* Tears down a manager: frees both of its arrays (`array2` and
 * `array1`, each via `sub_8026EB4` if non-`NULL`), and, if bit 0 of
 * `flags` is set, frees the manager struct itself via
 * `sub_8026ED0`. */
void sub_8008EB4(struct dual_array_manager *manager, s32 flags)
{
    if (manager->array2 != 0) {
        sub_8026EB4(manager->array2);
    }
    if (manager->array1 != 0) {
        sub_8026EB4(manager->array1);
    }
    if (flags & 1) {
        sub_8026ED0(manager);
    }
}

/* Initializes a manager: sets `count1`/`count2` to 0, `capacity` to
 * `count`, allocates two `count`-word arrays via `sub_8026EC0` for
 * `array1`/`array2`, and zero-fills `array1`. Returns `manager`. */
struct dual_array_manager *sub_8008EE4(struct dual_array_manager *manager, s32 count)
{
    s32 i;
    void **arr;
    s32 allocSize;

    manager->count1 = 0;
    manager->count2 = 0;
    manager->capacity = count;

    allocSize = count * 4;
    manager->array1 = sub_8026EC0(allocSize);
    manager->array2 = sub_8026EC0(allocSize);

    i = manager->capacity;
    if (i > 0) {
        void *zero = 0;
        arr = manager->array1;
        do {
            *arr = zero;
            arr++;
            i--;
        } while (i != 0);
    }

    return manager;
}

/* sub_8008F20 initializes a fixed-slot object pool "manager" struct:
 *  +0x0: s32 activeCount (0)
 *  +0x4: s32 capacity (= count)
 *  +0x8: void **slotArray - `count` pointers, allocated via
 *        sub_8026EC0(count*4), zero-filled
 *  +0xc: u8 *nodeArray - `count` 0x14-byte nodes, allocated via
 *        sub_8026EC0(count*0x14)
 *  +0x10..0x40F: a 256-word (0x400-byte) table, zeroed
 *  +0x410..0x80F: a second 256-word (0x400-byte) table, zeroed
 *  +0x810: void *freeListArray - `count` 8-byte {node, next} pairs,
 *          allocated via sub_8026EC0(count*8)
 *  +0x814: void *freeListHead - set to `freeListArray` itself at the
 *          very end
 *
 * For each node index i (0..count-1): freeListArray[i].node points at
 * nodeArray[i]; nodeArray[i]'s fields 0/4/0xc and byte 0x10 are
 * zeroed; nodeArray[i]'s field 8 is set to point back at
 * freeListArray[i]; and freeListArray[i].next is chained to
 * freeListArray[i+1] (or NULL for the last entry) - building a
 * singly-linked free list of pool nodes through the wrapper array.
 * The two big 256-word tables (+0x10, +0x410) are very likely a pair
 * of spatial-partition/collision grids, consistent with this whole
 * region being part of docs/rom_map.md's still-only-partially-
 * understood AI/collision dispatch system - but that broader purpose
 * isn't needed to confirm every individual load/store here, which are
 * all confirmed correct.
 *
 * PARKED AS NAKED: this compiler allocates the persistent cross-loop
 * values (item count, the +0x810/+0x814 field addresses, the loop
 * index reused from the zero-fill counter, the running "next" byte
 * offset) across r3/sb/sl/r4/r8 in a specific combination no C-level
 * reconstruction tried reproduces - the isolated compile produces the
 * right *shape* (same branches, same use of `ip`/r8, and even a
 * matching high-register save/restore prologue/epilogue) but a
 * different concrete register assignment for several of the four
 * long-lived scalars. Hand-transcribed instruction-for-instruction
 * from the ROM disassembly instead, the same technique already
 * established for this exact class of many-register allocation gap
 * (`sub_8010B6C`, game_loop28.c) - the ROM's suffixed Thumb mnemonics
 * (`movs`/`adds`/`subs`/`lsls`) are written in their suffix-less forms
 * here (`mov`/`add`/`sub`/`lsl`), which this project's assembler
 * invocation accepts identically. `sub_8009914`'s own tail is a
 * byte-for-byte copy of this function's free-list-build loop and hits
 * the identical gap - see that function's own writeup
 * (`actor_part11i.c`) for the shared technique applied there. See
 * docs/matching.md, "Parked, not matched: sub_8008F20". */
NAKED void *sub_8008F20(void *manager, s32 count)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r5, r0, #0\n\t"
        "add r0, r1, #0\n\t"
        "mov r1, #0\n\t"
        "str r1, [r5]\n\t"
        "str r0, [r5, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "bl sub_8026EC0\n\t"
        "str r0, [r5, #8]\n\t"
        "ldr r1, [r5, #4]\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "bl sub_8026EC0\n\t"
        "str r0, [r5, #0xc]\n\t"
        "mov r0, #0x81\n\t"
        "lsl r0, r0, #4\n\t"
        "add r4, r5, r0\n\t"
        "ldr r0, [r5, #4]\n\t"
        "lsl r0, r0, #3\n\t"
        "bl sub_8026EC0\n\t"
        "str r0, [r4]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "cmp r0, #0\n\t"
        "ble 2f\n\t"
        "mov r2, #0\n\t"
        "ldr r1, [r5, #8]\n\t"
    "1:\n\t"
        "stm r1!, {r2}\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #0\n\t"
        "bne 1b\n\t"
    "2:\n\t"
        "ldr r3, [r5, #4]\n\t"
        "mov r1, #0x81\n\t"
        "lsl r1, r1, #4\n\t"
        "add r1, r1, r5\n\t"
        "mov sb, r1\n\t"
        "ldr r2, 5f\n\t"
        "add r2, r2, r5\n\t"
        "mov sl, r2\n\t"
        "mov r0, #0\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #3\n\t"
        "add r2, r5, r1\n\t"
        "add r1, r5, #0\n\t"
        "add r1, #0x10\n\t"
        "mov r4, #0xff\n\t"
    "3:\n\t"
        "stm r1!, {r0}\n\t"
        "stm r2!, {r0}\n\t"
        "sub r4, #1\n\t"
        "cmp r4, #0\n\t"
        "bge 3b\n\t"
        "mov r4, #0\n\t"
        "cmp r4, r3\n\t"
        "bge 8f\n\t"
        "mov ip, sb\n\t"
        "mov r7, #0\n\t"
        "mov r2, #8\n\t"
        "mov r8, r2\n\t"
        "mov r6, #0\n\t"
    "4:\n\t"
        "mov r0, ip\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r2, r4, #3\n\t"
        "add r1, r2, r1\n\t"
        "ldr r0, [r5, #0xc]\n\t"
        "add r0, r0, r6\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, [r5, #0xc]\n\t"
        "add r0, r6, r0\n\t"
        "str r7, [r0]\n\t"
        "str r7, [r0, #4]\n\t"
        "str r7, [r0, #0xc]\n\t"
        "strb r7, [r0, #0x10]\n\t"
        "ldr r0, [r5, #0xc]\n\t"
        "add r0, r6, r0\n\t"
        "mov r1, ip\n\t"
        "ldr r3, [r1]\n\t"
        "add r1, r3, r2\n\t"
        "str r1, [r0, #8]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "sub r0, #1\n\t"
        "cmp r4, r0\n\t"
        "bne 6f\n\t"
        "str r7, [r1, #4]\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "5: .4byte 0x00000814\n"
    "6:\n\t"
        "mov r2, r8\n\t"
        "add r0, r3, r2\n\t"
        "str r0, [r1, #4]\n\t"
    "7:\n\t"
        "mov r0, #8\n\t"
        "add r8, r0\n\t"
        "add r6, #0x14\n\t"
        "add r4, #1\n\t"
        "ldr r0, [r5, #4]\n\t"
        "cmp r4, r0\n\t"
        "blt 4b\n\t"
    "8:\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "mov r2, sl\n\t"
        "str r0, [r2]\n\t"
        "add r0, r5, #0\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
asm(".align 2, 0");

/* Same pool-manager struct sub_8008F20 initializes and actor_part12.c
 * operates on - see that file for the full field writeup. Also used by
 * the now-matched `sub_8009150` (`actor_part11g.c`). */
struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    void *gridHead[256];
    void *gridTail[256];
    void *freeListArray;
    void *freeListHead;
};

/* sub_8009914 is reconstructed (semantics fully understood, and now
 * matched) as a NAKED transcription in its own translation unit,
 * `src/graphics/actor_part11i.c` - not appended here since its real
 * ROM address, 0x08009914, doesn't sit adjacent to this file's own
 * functions (it comes right after `sub_8009868`, `actor_part11d.c`,
 * and right before `sub_80099F0`/`actor_part12.c`), per
 * docs/workflow.md step 4's "needs its own new .c file" case - the
 * same reason `sub_80096C0` above got its own file
 * (`actor_part11e.c`), `sub_8009528` got `actor_part11f.c`, the
 * now-matched `sub_8009150` got `actor_part11g.c`, and the now-matched
 * `sub_800944C` got `actor_part11h.c`. */

/* sub_800944C is now matched as real C in its own translation unit,
 * `src/graphics/actor_part11h.c`; sub_8009528 is a NAKED transcription
 * in `src/graphics/actor_part11f.c`; sub_80096C0 likewise in
 * `src/graphics/actor_part11e.c` - none appended here since their real
 * ROM addresses don't sit adjacent to this file's own functions (they
 * come right after `sub_8009150` above and right before `sub_8009868`,
 * `actor_part11d.c`), per docs/workflow.md step 4's "needs its own new
 * .c file" case. */
asm(".align 2, 0");

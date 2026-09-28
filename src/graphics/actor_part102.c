#include "core.h"

/* NAKED - sets up the currently-selected category's runtime state:
 * `gUnknown_03001418` = `&gStaticData_081756C4[type]` (the category's
 * shared vtable, `type` being the first argument), `gUnknown_03001414`
 * = the 4th argument (a variant-selector byte), `gUnknown_03001400` =
 * the 2nd argument (the category's `sub_effect_table` array pointer,
 * see `struct sub_effect_record` in actor_part94.c/include/actor_anim.h),
 * resets `gUnknown_03001404`/`gUnknown_0300141C`/`gUnknown_03001420` to
 * 0, draws the vtable's slot-0 function pointer via `sub_803AD84`
 * (arg2/arg3 as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * `gUnknown_03001400[]` array comparing each entry's threshold field
 * (see `struct sub_effect_record.field_00`) plus `gUnknown_03001420`'s
 * offset against `gUnknown_03001418->fn[8]` (the vtable's own +0x20
 * slot, reinterpreted as a threshold): the first pass finds the last
 * entry whose adjusted threshold the vtable slot still exceeds
 * (`gUnknown_03001404`, an index into the table), the second draws
 * every entry from index 0 up to that point via `sub_803AD84`, mem_
 * alloc's a 0xc8-byte scratch buffer (`gUnknown_03001408`), and fires
 * one more `sub_803AD7C` visibility check if the vtable's own +8 slot
 * is set, before resetting `gUnknown_03001424`'s call counter to 0.
 *
 * The ROM passes a 6th argument: `[sp, #0x1c]` is the 5th (handed to
 * vtable slot 2), `[sp, #0x20]` the 6th (`y`).
 *
 * The draft below has the ROM's instruction sequence throughout
 * (NextThreshold is actor_part94.c's `sub_802A51C` address shape,
 * returned as a pointer so the load lands after the limit), but
 * `&gUnknown_03001404` and `base` swap `r7`/`r8` (125 halfwords, 4 bytes
 * long from the extra `mov`s). In the -dg dump `base` (refs 7, live
 * 105) outranks the `&gUnknown_03001404` pseudo (refs 4, live 84);
 * goto/for(;;) loop forms did not change that. Same under both
 * compilers. */
#if NON_MATCHING
#include "memory.h"
#include "actor_anim.h"

asm(".set _call_via_r1, sub_803AD7C\n"
    ".set _call_via_r3, sub_803AD84");

extern struct category_vtable *gUnknown_03001418;
extern u8 gUnknown_03001414;
extern struct sub_effect_record *gUnknown_03001400;
extern s32 gUnknown_03001404;
extern u8 gUnknown_0300141C;
extern s32 gUnknown_03001420;
extern u8 *gUnknown_03001408;
extern s32 gUnknown_03001424;
extern s32 sub_8029B2C(void);

/* `table[idx + 1].field_00`, with the record-boundary constant added
 * to the base before the index (same shape as actor_part94.c's
 * `sub_802A51C`). */
static inline s32 *NextThreshold(struct sub_effect_record *table, s32 idx)
{
    u8 *b = (u8 *)table;
    s32 off = idx * 0x14;

    b = b + 0x14;
    return (s32 *)(b + off);
}

void SelectActorCategory(s32 type, struct sub_effect_record *table, s32 x, u8 variant, s32 arg4, s32 y)
{
    struct sub_effect_record *t;
    u8 **buf;
    s32 base;

    gUnknown_03001418 = &gStaticData_081756C4[type];
    gUnknown_03001414 = variant;
    gUnknown_03001400 = table;
    gUnknown_03001404 = 0;
    gUnknown_0300141C = 0;
    gUnknown_03001420 = 0;
    ((void (*)(s32, s32))gUnknown_03001418->fn[0])(x, y);
    base = sub_8029B2C();
    t = gUnknown_03001400;
    while (gUnknown_03001404 < t->field_04
           && *NextThreshold(t, gUnknown_03001404) < (s32)gUnknown_03001418->fn[8] + base)
        gUnknown_03001404++;
    buf = &gUnknown_03001408;
    *buf = mem_alloc(0xc8, 0x80000000);
    if (gUnknown_03001418->fn[2] != NULL)
        ((void (*)(s32))gUnknown_03001418->fn[2])(arg4);
    while (gUnknown_03001404 < gUnknown_03001400->field_04
           && *NextThreshold(gUnknown_03001400, gUnknown_03001404) <= (s32)gUnknown_03001418->fn[7] + base) {
        ((void (*)(void *, s32, s32))gUnknown_03001418->fn[1])((u8 *)gUnknown_03001400 + (gUnknown_03001404 * 0x14 + 8),
                                                               gUnknown_03001414, 0);
        gUnknown_03001404++;
    }
    gUnknown_03001424 = 0;
}
#else
NAKED void SelectActorCategory(s32 type, void *subEffectTable, s32 x, s32 variantByte, s32 y)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "ldr r6, [sp, #0x20]\n\t"
        "ldr r4, 1f\n\t"
        "mov sb, r4\n\t"
        "mov r4, #0x34\n\t"
        "mul r4, r0, r4\n\t"
        "ldr r0, 2f\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, sb\n\t"
        "str r4, [r0]\n\t"
        "ldr r0, 3f\n\t"
        "strb r3, [r0]\n\t"
        "ldr r5, 4f\n\t"
        "str r1, [r5]\n\t"
        "ldr r7, 5f\n\t"
        "mov r1, #0\n\t"
        "str r1, [r7]\n\t"
        "ldr r0, 6f\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, 7f\n\t"
        "str r1, [r0]\n\t"
        "ldr r3, [r4]\n\t"
        "add r0, r2, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_803AD84\n\t"
        "bl sub_8029B2C\n\t"
        "mov r8, r0\n\t"
        "ldr r5, [r5]\n\t"
        "ldr r1, [r7]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "cmp r1, r0\n\t"
        "bge 8f\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x14\n\t"
        "add r0, r2, r0\n\t"
        "mov r3, sb\n\t"
        "ldr r1, [r3]\n\t"
        "ldr r1, [r1, #0x20]\n\t"
        "add r1, r8\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, r1\n\t"
        "bge 8f\n\t"
        "add r3, r7, #0\n\t"
        "mov r4, sb\n\t"
        "9:\n\t"
        "ldr r0, [r3]\n\t"
        "add r1, r0, #1\n\t"
        "str r1, [r3]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "cmp r1, r0\n\t"
        "bge 8f\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r2, r0\n\t"
        "ldr r1, [r4]\n\t"
        "ldr r1, [r1, #0x20]\n\t"
        "add r1, r8\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, r1\n\t"
        "blt 9b\n\t"
        "8:\n\t"
        "ldr r4, 10f\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #0x18\n\t"
        "mov r0, #0xc8\n\t"
        "bl mem_alloc\n\t"
        "str r0, [r4]\n\t"
        "ldr r4, 1f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "cmp r1, #0\n\t"
        "beq 11f\n\t"
        "ldr r0, [sp, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
        "11:\n\t"
        "ldr r7, 5f\n\t"
        "ldr r3, 4f\n\t"
        "ldr r1, [r3]\n\t"
        "ldr r2, [r7]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 12f\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, #0x14\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "add r0, r8\n\t"
        "ldr r1, [r1]\n\t"
        "cmp r1, r0\n\t"
        "bgt 12f\n\t"
        "add r6, r4, #0\n\t"
        "add r5, r3, #0\n\t"
        "add r4, r7, #0\n\t"
        "13:\n\t"
        "ldr r2, [r6]\n\t"
        "ldr r0, [r4]\n\t"
        "lsl r1, r0, #2\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, #8\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, 3f\n\t"
        "ldrb r1, [r1]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r4]\n\t"
        "add r2, r0, #1\n\t"
        "str r2, [r4]\n\t"
        "ldr r1, [r5]\n\t"
        "ldr r0, [r1, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 12f\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, #0x14\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "add r0, r8\n\t"
        "ldr r1, [r1]\n\t"
        "cmp r1, r0\n\t"
        "ble 13b\n\t"
        "12:\n\t"
        "ldr r1, 14f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1]\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "1: .4byte gUnknown_03001418\n"
        "2: .4byte gStaticData_081756C4\n"
        "3: .4byte gUnknown_03001414\n"
        "4: .4byte gUnknown_03001400\n"
        "5: .4byte gUnknown_03001404\n"
        "6: .4byte gUnknown_0300141C\n"
        "7: .4byte gUnknown_03001420\n"
        "10: .4byte gUnknown_03001408\n"
        "14: .4byte gUnknown_03001424\n"
    );
}
#endif

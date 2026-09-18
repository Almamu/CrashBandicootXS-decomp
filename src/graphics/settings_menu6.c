#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8008904(struct actor *part);
extern void sub_800737C(struct actor *self, s32 arg1, s32 arg2);
extern s32 sub_800695C(void *arg0);
extern s32 sub_80060AC(s32 value, void *dest);
extern void ***gUnknown_030012D0;
extern struct icon_pos gStaticData_0816B1E4;

/* Constructs the single icon at `field_88`: positions it from the fixed
 * `gStaticData_0816B1E4` pair, picks its starting keyframe-table entry
 * from a shared table (`gUnknown_030012D0`'s triple-indirected base,
 * offset `0xde<<1`), and formats two small numbers - a row-stats
 * derived count into `buf2c` and the constant `0x14` into `buf46` -
 * as decimal strings. */
void sub_8005A78(struct pause_screen_results *self)
{
    struct settings_icon_actor **dest = &self->field_88;

    *dest = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    (*dest)->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xde << 1));
    sub_800737C(&(*dest)->base, gStaticData_0816B1E4.x, gStaticData_0816B1E4.y);
    UPDATE_ICON_FRAME_NIBBLE(*dest);

    sub_80060AC(sub_800695C(self->field_10), self->buf2c);
    sub_80060AC(0x14, (u8 *)self + 0x46);
}

extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);
extern u32 gStaticData_0816B20C[];
extern struct icon_pos gStaticData_0816B1EC[];

/* Builds the 4-icon array at `icons8c`: one per `gStaticData_0816B1EC`
 * position entry, keyframe-table base `0xe4<<1` off the same shared
 * table `sub_8005A78` uses, frame index from `gStaticData_0816B20C`,
 * then the standard sub-counter/frame-counter/"done"-flag reset trio.
 *
 * Written as NAKED asm, not plain C: this project's usual gcc-2.9
 * register-allocation difficulty already documented at length for
 * `sub_8006600` (src/graphics/oam_count.c) - the loop/self pointer
 * never ended up in `r8` the way the ROM's does, no matter how the
 * source was rephrased. Every instruction below is transcribed directly
 * from and checked against the ROM's own disassembly. */
NAKED void sub_8005AE8(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "mov r8, r0\n\t"
    "mov r7, #0\n\t"
    "1:\n\t"
    "lsl r5, r7, #2\n\t"
    "mov r6, r8\n\t"
    "add r6, #0x8c\n\t"
    "add r6, r6, r5\n\t"
    "mov r0, #0x40\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8008904\n\t"
    "add r4, r0, #0\n\t"
    "str r4, [r6]\n\t"
    "ldr r0, 2f\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0xe4\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "str r0, [r4, #0x20]\n\t"
    "ldr r0, 3f\n\t"
    "add r5, r5, r0\n\t"
    "ldr r0, [r5]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "ldr r0, [r6]\n\t"
    "lsl r2, r7, #3\n\t"
    "ldr r1, 4f\n\t"
    "add r2, r2, r1\n\t"
    "ldr r1, [r2]\n\t"
    "ldr r2, [r2, #4]\n\t"
    "bl sub_800737C\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_800815C\n\t"
    "ldr r2, [r6]\n\t"
    "add r2, #0x29\n\t"
    "mov r1, #0xf\n\t"
    "and r0, r1\n\t"
    "mov r3, #0x10\n\t"
    "neg r3, r3\n\t"
    "add r1, r3, #0\n\t"
    "ldrb r3, [r2]\n\t"
    "and r1, r3\n\t"
    "orr r1, r0\n\t"
    "strb r1, [r2]\n\t"
    "add r7, #1\n\t"
    "cmp r7, #3\n\t"
    "ble 1b\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_030012D0\n"
    "3: .4byte gStaticData_0816B20C\n"
    "4: .4byte gStaticData_0816B1EC\n"
    );
}

extern s32 sub_8006920(void *arg0);
extern s32 sub_80068CC(void *arg0);
extern u32 gStaticData_0816B244[];
extern struct icon_pos gStaticData_0816B21C[];

/* Same shape as sub_8005AE8 above for the 5-icon array at `icons9c`
 * (keyframe-table base `0xc0<<1`, positions/frame indices from
 * `gStaticData_0816B21C`/`gStaticData_0816B244`), plus each icon's
 * `field_3c = 0x80`. After the loop, formats two more row-stats
 * derived numbers (`sub_8006920`/`sub_80068CC` on `field_10`) into
 * `buf2f`/`buf32`, and the constant `0x1c` into `buf49`.
 *
 * Written as NAKED asm, not plain C: same register-pressure class of
 * difficulty as `sub_8005AE8` above. Every instruction below is
 * transcribed directly from and checked against the ROM's own
 * disassembly. */
NAKED void sub_8005B80(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "add r7, r0, #0\n\t"
    "mov r0, #0\n\t"
    "mov r8, r0\n\t"
    "1:\n\t"
    "mov r1, r8\n\t"
    "lsl r5, r1, #2\n\t"
    "add r6, r7, #0\n\t"
    "add r6, #0x9c\n\t"
    "add r6, r6, r5\n\t"
    "mov r0, #0x40\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8008904\n\t"
    "add r4, r0, #0\n\t"
    "str r4, [r6]\n\t"
    "ldr r0, 2f\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "mov r3, #0xc0\n\t"
    "lsl r3, r3, #1\n\t"
    "add r0, r0, r3\n\t"
    "str r0, [r4, #0x20]\n\t"
    "ldr r0, 3f\n\t"
    "add r5, r5, r0\n\t"
    "ldr r0, [r5]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "ldr r0, [r6]\n\t"
    "mov r1, r8\n\t"
    "lsl r2, r1, #3\n\t"
    "ldr r1, 4f\n\t"
    "add r2, r2, r1\n\t"
    "ldr r1, [r2]\n\t"
    "ldr r2, [r2, #4]\n\t"
    "bl sub_800737C\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_800815C\n\t"
    "ldr r2, [r6]\n\t"
    "add r2, #0x29\n\t"
    "mov r1, #0xf\n\t"
    "and r0, r1\n\t"
    "mov r3, #0x10\n\t"
    "neg r3, r3\n\t"
    "add r1, r3, #0\n\t"
    "ldrb r3, [r2]\n\t"
    "and r1, r3\n\t"
    "orr r1, r0\n\t"
    "strb r1, [r2]\n\t"
    "ldr r1, [r6]\n\t"
    "mov r0, #0x80\n\t"
    "strh r0, [r1, #0x3c]\n\t"
    "mov r0, #1\n\t"
    "add r8, r0\n\t"
    "mov r1, r8\n\t"
    "cmp r1, #4\n\t"
    "ble 1b\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_8006920\n\t"
    "add r4, r0, #0\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_80068CC\n\t"
    "add r5, r0, #0\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x2f\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80060AC\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x32\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80060AC\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x49\n\t"
    "mov r0, #0x1c\n\t"
    "bl sub_80060AC\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_030012D0\n"
    "3: .4byte gStaticData_0816B244\n"
    "4: .4byte gStaticData_0816B21C\n"
    );
}

extern s32 sub_8006864(void *arg0);
extern s32 sub_8006820(void *arg0);
extern s32 sub_80067EC(void *arg0);
extern s32 sub_80068A8(void *arg0);
extern u32 gStaticData_0816B270[];
extern struct icon_pos gStaticData_0816B258[];

/* Same shape as sub_8005AE8/B80 above for the 3-icon array at
 * `iconsB0` (keyframe-table base `0xc6<<1`, positions/frame indices
 * from `gStaticData_0816B258`/`gStaticData_0816B270`), each icon's
 * `field_3c = 0x80`. After the loop, formats four category counts
 * (`sub_8006864`/`sub_8006820`/`sub_80067EC`/`sub_80068A8` on
 * `field_10` - the same four functions src/graphics/oam_count.c
 * documents) into `buf38`/`buf3b`/`buf3e`/`buf35`, and the constant
 * `0x14` into `buf4c`.
 *
 * Written as NAKED asm, not plain C: same register-pressure class of
 * difficulty as `sub_8005AE8`/`sub_8005B80` above. Every instruction
 * below is transcribed directly from and checked against the ROM's own
 * disassembly. */
NAKED void sub_8005C58(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "add r7, r0, #0\n\t"
    "mov r0, #0\n\t"
    "mov r8, r0\n\t"
    "1:\n\t"
    "mov r1, r8\n\t"
    "lsl r5, r1, #2\n\t"
    "add r6, r7, #0\n\t"
    "add r6, #0xb0\n\t"
    "add r6, r6, r5\n\t"
    "mov r0, #0x40\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8008904\n\t"
    "add r4, r0, #0\n\t"
    "str r4, [r6]\n\t"
    "ldr r0, 2f\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0]\n\t"
    "mov r3, #0xc6\n\t"
    "lsl r3, r3, #1\n\t"
    "add r0, r0, r3\n\t"
    "str r0, [r4, #0x20]\n\t"
    "ldr r0, 3f\n\t"
    "add r5, r5, r0\n\t"
    "ldr r0, [r5]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "ldr r0, [r6]\n\t"
    "mov r1, r8\n\t"
    "lsl r2, r1, #3\n\t"
    "ldr r1, 4f\n\t"
    "add r2, r2, r1\n\t"
    "ldr r1, [r2]\n\t"
    "ldr r2, [r2, #4]\n\t"
    "bl sub_800737C\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_800815C\n\t"
    "ldr r2, [r6]\n\t"
    "add r2, #0x29\n\t"
    "mov r1, #0xf\n\t"
    "and r0, r1\n\t"
    "mov r3, #0x10\n\t"
    "neg r3, r3\n\t"
    "add r1, r3, #0\n\t"
    "ldrb r3, [r2]\n\t"
    "and r1, r3\n\t"
    "orr r1, r0\n\t"
    "strb r1, [r2]\n\t"
    "ldr r1, [r6]\n\t"
    "mov r0, #0x80\n\t"
    "strh r0, [r1, #0x3c]\n\t"
    "mov r0, #1\n\t"
    "add r8, r0\n\t"
    "mov r1, r8\n\t"
    "cmp r1, #2\n\t"
    "ble 1b\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_8006864\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x38\n\t"
    "bl sub_80060AC\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_8006820\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x3b\n\t"
    "bl sub_80060AC\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_80067EC\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x3e\n\t"
    "bl sub_80060AC\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "bl sub_80068A8\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x35\n\t"
    "bl sub_80060AC\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x4c\n\t"
    "mov r0, #0x14\n\t"
    "bl sub_80060AC\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_030012D0\n"
    "3: .4byte gStaticData_0816B270\n"
    "4: .4byte gStaticData_0816B258\n"
    );
}

extern void *gUnknown_030012C0;
extern s32 sub_802332C(void *arg0);
extern void FormatCentiseconds(s32 value, u8 *buf);

/* Same per-level bronze/silver/gold threshold table src/graphics/oam_count.c's
 * `struct threshold_table_entry`/`gStaticData_0816C86C` already document -
 * duplicated locally (rather than shared via a header) per that file's
 * own comment on the type, matching this project's minimal-local-type
 * convention. */
struct threshold_table_entry {
    u8 unused_00[8];
    u32 threshold_08;
    u32 threshold_0C;
    u32 threshold_10;
    u8 unused_14[0x24 - 0x14];
};
COMPILE_TIME_ASSERT(sizeof(struct threshold_table_entry) == 0x24);

extern struct threshold_table_entry gStaticData_0816C86C[];
extern struct icon_pos gStaticData_0816B27C;

/* The medal/rank award widget (see docs/rom_map.md's overlay_ui
 * "Correction" section): reads the current level's completion time
 * (`self->field_10[levelIdx+1]`, a raw record whose low bits are a
 * centiseconds time), formats it via FormatCentiseconds, then compares
 * it against `gStaticData_0816C86C[levelIdx]`'s bronze/silver/gold
 * thresholds and constructs the icon at `field_bc` tagged with the
 * matching medal (from `gStaticData_0816B270`, the same table
 * sub_8005C58 uses) only if a threshold was actually met - otherwise
 * leaves `field_6c` (an "earned" flag) clear and the icon untagged.
 *
 * Written as NAKED asm, not plain C: same register-pressure class of
 * difficulty as `sub_8005AE8`/`sub_8005B80`/`sub_8005C58` above. Every
 * instruction below is transcribed directly from and checked against
 * the ROM's own disassembly. */
NAKED void sub_8005D44(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "add r6, r0, #0\n\t"
    "ldr r0, 6f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_802332C\n\t"
    "add r4, r0, #0\n\t"
    "lsl r1, r4, #2\n\t"
    "add r1, #4\n\t"
    "ldr r0, [r6, #0x10]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "lsl r0, r0, #0x10\n\t"
    "lsr r5, r0, #0x13\n\t"
    "add r1, r6, #0\n\t"
    "add r1, #0x7c\n\t"
    "add r0, r5, #0\n\t"
    "bl FormatCentiseconds\n\t"
    "lsl r0, r4, #3\n\t"
    "add r0, r0, r4\n\t"
    "lsl r0, r0, #2\n\t"
    "ldr r1, 7f\n\t"
    "add r7, r0, r1\n\t"
    "mov r1, #0\n\t"
    "cmp r5, #0\n\t"
    "beq 1f\n\t"
    "ldr r0, [r7, #8]\n\t"
    "cmp r5, r0\n\t"
    "bhi 1f\n\t"
    "mov r1, #1\n\t"
    "1:\n\t"
    "add r0, r6, #0\n\t"
    "add r0, #0x6c\n\t"
    "strb r1, [r0]\n\t"
    "add r6, #0xbc\n\t"
    "mov r0, #0x40\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8008904\n\t"
    "str r0, [r6]\n\t"
    "ldr r1, 8f\n\t"
    "ldr r1, [r1]\n\t"
    "ldr r1, [r1]\n\t"
    "ldr r1, [r1]\n\t"
    "mov r2, #0xc6\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "str r1, [r0, #0x20]\n\t"
    "ldr r2, 9f\n\t"
    "ldr r1, [r2]\n\t"
    "ldr r2, [r2, #4]\n\t"
    "bl sub_800737C\n\t"
    "cmp r5, #0\n\t"
    "beq 5f\n\t"
    "ldr r0, [r7, #8]\n\t"
    "cmp r5, r0\n\t"
    "bhi 2f\n\t"
    "ldr r0, 10f\n\t"
    "ldr r4, [r6]\n\t"
    "ldr r0, [r0, #8]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "2:\n\t"
    "ldr r0, [r7, #0xc]\n\t"
    "cmp r5, r0\n\t"
    "bhi 3f\n\t"
    "ldr r0, 10f\n\t"
    "ldr r4, [r6]\n\t"
    "ldr r0, [r0, #4]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "3:\n\t"
    "ldr r0, [r7, #0x10]\n\t"
    "cmp r5, r0\n\t"
    "bhi 4f\n\t"
    "ldr r0, 10f\n\t"
    "ldr r4, [r6]\n\t"
    "ldr r0, [r0]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2d\n\t"
    "strb r0, [r1]\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087C0\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_80087B4\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_800872C\n\t"
    "4:\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_800815C\n\t"
    "ldr r2, [r6]\n\t"
    "add r2, #0x29\n\t"
    "mov r1, #0xf\n\t"
    "and r0, r1\n\t"
    "mov r1, #0x10\n\t"
    "neg r1, r1\n\t"
    "ldrb r3, [r2]\n\t"
    "and r1, r3\n\t"
    "orr r1, r0\n\t"
    "strb r1, [r2]\n\t"
    "5:\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "6: .4byte gUnknown_030012C0\n"
    "7: .4byte gStaticData_0816C86C\n"
    "8: .4byte gUnknown_030012D0\n"
    "9: .4byte gStaticData_0816B27C\n"
    "10: .4byte gStaticData_0816B270\n"
    );
}

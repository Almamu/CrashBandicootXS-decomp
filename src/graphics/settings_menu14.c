#include "core.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "memory.h"

/* `sub_80062A8` (GitHub issue #8) - the higher-level dialog spawner:
 * resets palette color 0 and `REG_DISPCNT`, re-initializes the popup-
 * text system's `gUnknown_030012DC`/`030012E0` icon managers (the same
 * `sub_8028A40` init call the between-level map screen's `sub_8034CEC`
 * uses), resets the shared VRAM upload cursor `gUnknown_030012FC`,
 * fires each icon manager's `record->slots[6]` trampoline via
 * `sub_803AD7C` and reserves its `field_12c<<5` bytes of VRAM via
 * `sub_8006C58` (copying `gUnknown_030012DC`'s `field_12c` into
 * `gUnknown_030012E0`'s `field_108` in between - meaning not otherwise
 * established), then allocates the dialog object (`sub_8026EDC(0x2c)`,
 * exactly `src/graphics/settings_menu13.c`'s `struct
 * sub_8006700_actor`'s own size) and builds it via `sub_80063D8`
 * (matched, same file) before running its fade-in/wait-for-confirm/
 * fade-out lifecycle via `sub_8006518` and, if the confirm button was
 * pressed (a non-NULL result), `sub_8006770(dialog, 3)`.
 *
 * Has no `bl` caller in raw asm - it's called from the four
 * already-matched `sub_80067A4`-`D4` wrappers in
 * `src/graphics/oam_count.c`, each with a fixed `(label1, label2,
 * type)` triple.
 *
 * Written as NAKED asm, not plain C: every load, store, and call here
 * was confirmed against the ROM, but what resisted matching in C was
 * the register choreography around the two cached-global-address
 * locals (`gUnknown_030012DC`/`030012E0`, held in `r5`/`r8` in the ROM)
 * and the `record`/`field_12c` field reads threaded through them,
 * including the exact constant-reuse trick the ROM uses to derive
 * `gUnknown_030012E0`'s `field_108` offset (`0x108`) by subtracting
 * `0x24` from the already-loaded `field_12c` offset constant (`0x12c`)
 * rather than loading a fresh literal - the same class of "last mile"
 * gcc-2.9 register/constant-reuse nondeterminism `sub_8006600`/
 * `sub_80049CC` document at length. An explicit `asm("r7")` pin for
 * `type` was tried and discarded in an earlier pass - it produced a
 * genuine correctness bug (the natural allocator reused r7 for an
 * unrelated cached address partway through the function, silently
 * clobbering the pinned value before its final use at the
 * `sub_80063D8` call site), the project's documented categorical
 * r7-pin limitation (docs/matching/naked-sub_8007dbc.md) - this NAKED
 * transcription sidesteps that class of bug entirely by pinning nothing
 * and instead reproducing the ROM's own register choices verbatim.
 * Every instruction below is transcribed directly from and checked
 * against the ROM's own disassembly. */

/* Same struct sub_8006700_actor shape src/graphics/settings_menu13.c
 * documents (redeclared locally per this project's convention). */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void *sub_8026EDC(s32 size);
extern s32 sub_8026F38(s32 arg0);
extern void sub_80006A8(void);
extern s32 mem_free_bytes(s32 flags);
extern void sub_8006EA8(struct tile_asset_cache *self);
extern void sub_8028A40(struct icon_manager *self, u32 unused);
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern s32 sub_8006C58(struct vram_upload_cursor *self, s32 size);
extern void sub_8006C30(struct vram_upload_cursor *self);
extern void sub_803AD7C(void *addr, void *fn);
extern void sub_8006770(struct sub_8006700_actor *self, u32 flags);
extern void sub_8006518(struct sub_8006700_actor *self);
extern struct sub_8006700_actor *sub_80063D8(struct sub_8006700_actor *self, s32 label1, s32 label2, s32 type);

extern struct tile_asset_cache *gUnknown_030012B8;
extern struct vram_upload_cursor *gUnknown_030012FC;
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;

NAKED void sub_80062A8(s32 label1, s32 label2, s32 type)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sl\n\t"
    "mov r6, sb\n\t"
    "mov r5, r8\n\t"
    "push {r5, r6, r7}\n\t"
    "mov sb, r0\n\t"
    "mov sl, r1\n\t"
    "add r7, r2, #0\n\t"
    "mov r0, #0xc0\n\t"
    "lsl r0, r0, #0x18\n\t"
    "bl mem_free_bytes\n\t"
    "bl sub_80006A8\n\t"
    "mov r0, #0xa0\n\t"
    "lsl r0, r0, #0x13\n\t"
    "mov r1, #0\n\t"
    "strh r1, [r0]\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #0x13\n\t"
    "strh r1, [r0]\n\t"
    "ldr r1, 2f\n\t"
    "ldr r0, [r1]\n\t"
    "bl sub_8006EA8\n\t"
    "ldr r5, 3f\n\t"
    "ldr r0, [r5]\n\t"
    "bl sub_8028A40\n\t"
    "ldr r2, 4f\n\t"
    "mov r8, r2\n\t"
    "ldr r0, [r2]\n\t"
    "bl sub_8028A40\n\t"
    "ldr r6, 5f\n\t"
    "ldr r0, [r6]\n\t"
    "mov r4, #0\n\t"
    "str r4, [r0, #8]\n\t"
    "bl sub_8006C4C\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_8006C4C\n\t"
    "ldr r0, [r5]\n\t"
    "mov r3, #0x84\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "str r4, [r1]\n\t"
    "mov r2, #0x98\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "ldr r1, [r1]\n\t"
    "add r1, #0x40\n\t"
    "mov r3, #0\n\t"
    "ldrsh r2, [r1, r3]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r1, [r1, #4]\n\t"
    "bl sub_803AD7C\n\t"
    "ldr r0, [r6]\n\t"
    "ldr r1, [r5]\n\t"
    "mov r2, #0x96\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "ldr r1, [r1]\n\t"
    "lsl r1, r1, #5\n\t"
    "bl sub_8006C58\n\t"
    "ldr r0, [r5]\n\t"
    "mov r3, #0x96\n\t"
    "lsl r3, r3, #1\n\t"
    "add r0, r0, r3\n\t"
    "ldr r2, [r0]\n\t"
    "mov r1, r8\n\t"
    "ldr r0, [r1]\n\t"
    "sub r3, #0x24\n\t"
    "add r1, r0, r3\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x98\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "ldr r1, [r1]\n\t"
    "add r1, #0x40\n\t"
    "mov r3, #0\n\t"
    "ldrsh r2, [r1, r3]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r1, [r1, #4]\n\t"
    "bl sub_803AD7C\n\t"
    "ldr r0, [r6]\n\t"
    "mov r2, r8\n\t"
    "ldr r1, [r2]\n\t"
    "mov r3, #0x96\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r1, r3\n\t"
    "ldr r1, [r1]\n\t"
    "lsl r1, r1, #5\n\t"
    "bl sub_8006C58\n\t"
    "ldr r0, [r6]\n\t"
    "bl sub_8006C30\n\t"
    "mov r0, #0x2c\n\t"
    "bl sub_8026EDC\n\t"
    "add r5, r0, #0\n\t"
    "mov r0, sb\n\t"
    "bl sub_8026F38\n\t"
    "add r4, r0, #0\n\t"
    "mov r0, sl\n\t"
    "bl sub_8026F38\n\t"
    "add r2, r0, #0\n\t"
    "add r0, r5, #0\n\t"
    "add r1, r4, #0\n\t"
    "add r3, r7, #0\n\t"
    "bl sub_80063D8\n\t"
    "add r4, r0, #0\n\t"
    "bl sub_8006518\n\t"
    "cmp r4, #0\n\t"
    "beq 1f\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #3\n\t"
    "bl sub_8006770\n\t"
    "1:\n\t"
    "ldr r1, 2f\n\t"
    "ldr r0, [r1]\n\t"
    "bl sub_8006EA8\n\t"
    "mov r0, #0xc0\n\t"
    "lsl r0, r0, #0x18\n\t"
    "bl mem_free_bytes\n\t"
    "pop {r3, r4, r5}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "mov sl, r5\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_030012B8\n"
    "3: .4byte gUnknown_030012DC\n"
    "4: .4byte gUnknown_030012E0\n"
    "5: .4byte gUnknown_030012FC\n"
    );
}

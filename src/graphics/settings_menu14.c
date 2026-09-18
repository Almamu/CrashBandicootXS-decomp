#include "core.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "memory.h"

#if NON_MATCHING
/* `sub_80062A8` (GitHub issue #8) - reconstructed (semantics fully
 * traced and cross-checked against docs/rom_map.md's "`sub_80063D8`
 * builds a two-string dialog/message box" section) but NOT YET
 * BYTE-MATCHING - parked the same way this ROM region's other
 * icon-manager-heavy constructors are (`sub_8006600` in
 * src/graphics/oam_count.c; the `sub_8006124`/`sub_800619C`/
 * `sub_80061E8` trio and `sub_8006518` in src/graphics/
 * settings_menu10.c/11.c). The higher-level dialog spawner: resets
 * palette color 0 and `REG_DISPCNT`, re-initializes the popup-text
 * system's `gUnknown_030012DC`/`030012E0` icon managers (the same
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
 * Every load, store, and call here is confirmed against the ROM - what
 * resists matching is the register choreography around the two
 * cached-global-address locals (`gUnknown_030012DC`/`030012E0`, held
 * in `r5`/`r8` in the ROM) and the `record`/`field_12c` field reads
 * threaded through them: the exact constant-reuse trick the ROM uses
 * to derive `gUnknown_030012E0`'s `field_108` offset (`0x108`) by
 * subtracting `0x24` from the already-loaded `field_12c` offset
 * constant (`0x12c`) rather than loading a fresh literal - the same
 * class of "last mile" gcc-2.9 register/constant-reuse nondeterminism
 * `sub_8006600`/`sub_80049CC` document at length - wasn't closed with
 * the pin techniques tried here (explicit `register ... asm("r5")`/
 * `asm("r8")`/`asm("r6")` pins for the three cached addresses,
 * matching `sub_80063D8`'s own successful pins one scope up - these do
 * get the three addresses into the right registers, but the field
 * reads/offset arithmetic around them still diverge). An explicit
 * `asm("r7")` pin for `type` was tried and discarded - it produced a
 * genuine correctness bug (the natural allocator reused r7 for an
 * unrelated cached address partway through the function, silently
 * clobbering the pinned value before its final use at the
 * `sub_80063D8` call site) - this is the project's documented
 * categorical r7-pin limitation
 * (docs/matching/naked-sub_8007dbc.md), not something safe to keep
 * pushing on for a single call site. */

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

void sub_80062A8(s32 label1, s32 label2, s32 type)
{
    struct icon_manager *mgr;
    struct icon_slot *slot;
    struct sub_8006700_actor *dialog;
    s32 label1Str;
    s32 label2Str;

    mem_free_bytes(MEM_HEAP_BOTH);
    sub_80006A8();

    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    sub_8006EA8(gUnknown_030012B8);
    sub_8028A40(gUnknown_030012DC, 0);
    sub_8028A40(gUnknown_030012E0, 0);

    gUnknown_030012FC->field_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);

    mgr = gUnknown_030012DC;
    mgr->field_108 = 0;
    slot = &mgr->record->slots[6];
    sub_803AD7C((u8 *)mgr + slot->offset, slot->ptr);
    sub_8006C58(gUnknown_030012FC, gUnknown_030012DC->field_12c << 5);

    gUnknown_030012E0->field_108 = gUnknown_030012DC->field_12c;
    mgr = gUnknown_030012E0;
    slot = &mgr->record->slots[6];
    sub_803AD7C((u8 *)mgr + slot->offset, slot->ptr);
    sub_8006C58(gUnknown_030012FC, gUnknown_030012E0->field_12c << 5);

    sub_8006C30(gUnknown_030012FC);

    dialog = (struct sub_8006700_actor *)sub_8026EDC(0x2c);
    label1Str = sub_8026F38(label1);
    label2Str = sub_8026F38(label2);
    dialog = sub_80063D8(dialog, label1Str, label2Str, type);
    sub_8006518(dialog);
    if (dialog != NULL) {
        sub_8006770(dialog, 3);
    }

    sub_8006EA8(gUnknown_030012B8);
    mem_free_bytes(MEM_HEAP_BOTH);
}
#endif /* NON_MATCHING */

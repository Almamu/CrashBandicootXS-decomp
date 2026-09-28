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
 * Once a NAKED transcription. It matches as plain C under both
 * compilers: the icon-manager set-up is the same `IconSetup`/
 * `IconReserve` inline-helper sequence `sub_801BAF0`
 * (src/graphics/actor_part_1b85c.c) uses, and `sub_8028A40` takes one
 * argument. See docs/matching/issue-4-6-8-naked-retry.md. */

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
extern void sub_8028A40(struct icon_manager *self);
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

static inline void IconSetup(struct icon_manager *m, u32 v)
{
    struct icon_slot *slot;

    m->field_108 = v;
    slot = &m->record->slots[6];
    sub_803AD7C((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct icon_manager **m)
{
    struct vram_upload_cursor *c = gUnknown_030012FC;

    sub_8006C58(c, (*m)->field_12c << 5);
}

void sub_80062A8(s32 label1, s32 label2, s32 type)
{
    struct sub_8006700_actor *dialog;

    mem_free_bytes(0xC0000000);
    sub_80006A8();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    sub_8006EA8(gUnknown_030012B8);
    sub_8028A40(gUnknown_030012DC);
    sub_8028A40(gUnknown_030012E0);
    gUnknown_030012FC->field_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);
    IconSetup(gUnknown_030012DC, 0);
    IconReserve(&gUnknown_030012DC);
    {
        u32 v = gUnknown_030012DC->field_12c;

        IconSetup(gUnknown_030012E0, v);
    }
    IconReserve(&gUnknown_030012E0);
    sub_8006C30(gUnknown_030012FC);
    dialog = sub_80063D8(sub_8026EDC(0x2c), sub_8026F38(label1), sub_8026F38(label2), type);
    sub_8006518(dialog);
    if (dialog != NULL)
        sub_8006770(dialog, 3);
    sub_8006EA8(gUnknown_030012B8);
    mem_free_bytes(0xC0000000);
}

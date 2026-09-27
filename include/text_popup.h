#ifndef GUARD_TEXT_POPUP_H
#define GUARD_TEXT_POPUP_H

/* Shared by the "two-line text popup" spawners in
 * src/graphics/graphics_loading_1ef0c.c, graphics_loading_1fdec.c,
 * graphics_loading_1feec.c, graphics_loading_21280.c and
 * graphics_loading_21668.c (ROM 0x0801EF0C-0x08021BFC). Each builds a
 * sprite part with sub_8009ED0, attaches a freshly constructed popup
 * header (sub_800CA74) to it, and fills the part's two "collected" bits
 * from the level's record table. This ROM region was built with
 * old_agbcc (see docs/matching/old-agbcc-retry.md). */

#include "actor.h"

/* The sprite part sub_8009ED0 returns. Same layout as actor_part_188d0.c's
 * `struct gfx_part`. The +0x28 bits are declared on a 32-bit base type:
 * with `u8` bitfields the shared `1` constant is a QImode pseudo that CSE
 * merges with `field_0A = 1`, and the allocator no longer matches. */
struct popup_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    void *anim;                 // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;             // 0x28
    u32 flipX:1;
    u32 unk_28_5:1;
    u32 unk_28_6:2;
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                     // 0x2D
    u8 unk_2E[0x16];
    struct popup_hdr *hdr;      // 0x44
};

/* One level record, at `bytes + offsets[id]` in gUnknown_030012B4's
 * table. */
struct level_record
{
    u8 flags;                   // bit 1: not yet collected, bit 2: ?
    u8 unk_01[3];
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
};

struct level_record_table
{
    u8 unk_00[8];
    u16 *offsets;
    u8 *bytes;
};

/* A C++-style method slot: `this` adjustment plus function pointer. */
struct popup_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct popup_vtable
{
    u8 unk_00[0x18];
    struct popup_method attach; // 0x18
};

/* The popup header sub_800CA74 constructs. */
struct popup_hdr
{
    u8 unk_00[0xC];
    struct popup_vtable *vtable; // 0x0C
    u8 unk_10[0x10];
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3C;
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    s32 unk_4C;
    u8 unk_50[0x1C];
    s32 tag;                    // 0x6C
    u8 unk_70[0x14];
    void *gfx;                  // 0x84
};

extern void ***gUnknown_030012D0;
extern struct level_record_table **gUnknown_030012B4;
extern void *gUnknown_030012F0;

extern struct popup_part *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern s32 sub_800815C(struct popup_part *part);
extern void *sub_8026EDC(s32 size);
extern struct popup_hdr *sub_800CA74(void);
extern s32 sub_803AD80(void *self, void *arg, void *fn);
extern void sub_8008E94(void *manager, void *value);
extern void sub_800C6A8(struct popup_hdr *hdr, s32 arg1);
extern void sub_800C860(struct popup_hdr *hdr, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800C87C(struct popup_hdr *hdr, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800C898(struct popup_hdr *hdr, s32 arg1);
extern void sub_80087C0(struct popup_part *part);
extern void sub_80087B4(struct popup_part *part);
extern void sub_800872C(struct popup_part *part, s32 arg);

#define POPUP_ANIM(offset) ((void *)((u8 *)**gUnknown_030012D0 + (offset)))

/* hdr->attach(part), through sub_803AD80 (`_call_via_r2`). */
#define POPUP_ATTACH(hdr, part)                                                \
    sub_803AD80((u8 *)(hdr) + (hdr)->vtable->attach.thisOffset, (part),       \
                (hdr)->vtable->attach.fn)

#define LEVEL_RECORD(id)                                                       \
    ((struct level_record *)((*gUnknown_030012B4)->bytes +                     \
                             (*gUnknown_030012B4)->offsets[id]))

/* The setters below are inline because old_agbcc schedules a store's
 * value before its address only when the value arrives as an inline
 * helper's parameter; written in place, the order flips. */
static inline void SetPartField0A(struct popup_part *part, s32 value)
{
    part->base.field_0A = value;
}

static inline void SetPopupGfx(struct popup_hdr *hdr, void *gfx)
{
    hdr->gfx = gfx;
}

static inline void SetPartTag(struct popup_part *part, s32 tag)
{
    part->tag = tag;
}

/* The mask arrives as an `s32` so old_agbcc derives it from a still-live
 * constant (`subs r0, #0x43`) the way the ROM does. */
static inline void AndPartFlags(struct popup_part *part, s32 mask)
{
    part->base.flags &= mask;
}

/* Hands the part animation `anim` and restarts it. */
static inline void SetPartAnim(struct popup_part *part, s32 anim)
{
    part->tag = anim;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
}

/* The multi-field setters load every value before storing any, as the ROM
 * does; written as separate statements, each load/store pair interleaves. */
static inline void SetPopupRect(struct popup_hdr *hdr, s32 x, s32 y, s32 w, s32 h)
{
    hdr->unk_20 = x;
    hdr->unk_28 = w;
    hdr->unk_24 = y;
    hdr->unk_2C = h;
}

static inline void SetPopupSpan(struct popup_hdr *hdr, s32 a, s32 b, s32 c)
{
    hdr->unk_30 = a;
    hdr->unk_34 = b;
    hdr->unk_38 = c;
}

static inline void SetPopupBox(struct popup_hdr *hdr, s32 a, s32 b, s32 c)
{
    hdr->unk_3C = a;
    hdr->unk_40 = b;
    hdr->unk_44 = c;
}

#endif /* GUARD_TEXT_POPUP_H */

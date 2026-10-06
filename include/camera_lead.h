#ifndef GUARD_CAMERA_LEAD_H
#define GUARD_CAMERA_LEAD_H

#include "gba/types.h"
#include "vtable.h"

/*
 * The camera lead (`struct follow_child`, 0x80 bytes, method table
 * gCameraLeadVtable): the moving sprite InputCtrlStateStart (input_ctrl.c)
 * spawns for the input controller (CreateCameraLead, level_select.c). It
 * trails the player at a horizontal offset that eases 2 px per frame toward
 * a clamped target, and registers itself as gCamera's follow target while
 * alive.
 *
 * level_select.c's definition and input_ctrl.c's `struct ctrl_child`
 * (`gone` is `flags.bits.gone`, `unk_78` is `targetOffset`) were merged
 * here (#574, batch 9e).
 */
struct follow_child {
    s32 x;        // 0x00
    s32 y;        // 0x04
    u16 field_08; // 0x08 - bitmap id (MARK_GONE in input_ctrl.c)
    u8 unk_0A[2];
    /* The flags byte: level_select.c ORs it whole, input_ctrl.c sets
     * `gone`. `packed` keeps the union one byte. */
    union {
        u8 all;
        struct {
            u8 gone:1; // bit 0: removed
            u8 unk_1:7;
        } __attribute__((packed)) bits;
    } __attribute__((packed)) flags; // 0x0C
    /* ResetCameraLead reads the byte whole and sets `visible`. */
    union {
        u8 all;
        struct {
            u8 bit0_1:2;
            u8 visible:1;
            u8 bit3_7:5;
        } __attribute__((packed)) bits;
    } __attribute__((packed)) state; // 0x0D
    u8 unk_0E[0x0A];
    const struct vtable_slot *vtable; // 0x18 - gCameraLeadVtable
    u8 unk_1C[8];
    u8 moveAxes; // 0x24
    u8 unk_25[0x0D];
    u8 unk_32; // 0x32
    u8 unk_33[0x2D];
    s32 speedX; // 0x60 - copied from the player every frame
    u8 unk_64[4];
    u8 hitAxes; // 0x68
    u8 unk_69[0x0F];
    s32 targetOffset; // 0x78 - Q8 x offset from the player, 0xA00-0x3200
    s32 offset;       // 0x7C - eases toward targetOffset
};

#endif /* GUARD_CAMERA_LEAD_H */

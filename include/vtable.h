#ifndef GUARD_VTABLE_H
#define GUARD_VTABLE_H

/*
 * The game is gcc 2.x C++ built without -fvtable-thunks, so a virtual
 * table is an array of 8-byte slots: a `this` adjustment, an index the
 * ROM always leaves 0, and the (Thumb) code address. Slot 0 is the
 * type-info slot, empty (all zero) everywhere - the game has no RTTI.
 *
 * This is the layout the code reads through `struct actor_method`
 * (actor_self.h; gobj_1a794.h's and level_menu.h's `struct method` and the
 * file-local `*_method` copies were merged into it, #574 batch 9e); this
 * type is for defining the tables themselves (src/data, see docs/data.md).
 */
struct vtable_slot {
    s16 delta; // 0x00 - added to `this` before the call
    s16 index; // 0x02 - always 0
    void *fn;  // 0x04 - NULL only in slot 0
};

#define VTABLE_SLOT(func) { 0, 0, (void *)(func) }

#endif /* !GUARD_VTABLE_H */

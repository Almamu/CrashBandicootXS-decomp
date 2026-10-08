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
 * type is for defining the tables that are still C data
 * (src/data/entity_vtables_7e3bec.c, see docs/data.md). g++ emits the
 * others itself, in this same layout (docs/cplusplus.md, "Emitting the
 * vtables").
 */
struct vtable_slot {
    s16 delta; // 0x00 - added to `this` before the call
    s16 index; // 0x02 - always 0
    void *fn;  // 0x04 - NULL only in slot 0
};

#define VTABLE_SLOT(func) { 0, 0, (void *)(func) }

/* Puts a C table in a section of its own, `.rodata.<name>`, so that
 * ldscript.txt can place each table at its ROM address, between the
 * tables g++ emits (`.gnu.linkonce.d._vt.<class>` in the class's
 * key-method object: docs/cplusplus.md, "Emitting the vtables"). */
#define VTABLE_SECTION(name) __attribute__((section(".rodata." #name)))

#endif /* !GUARD_VTABLE_H */

#ifndef __ICON_MANAGER_H__
#define __ICON_MANAGER_H__

/* One (OAM-slot-offset, pointer) pair, as used by sub_803AD80/
 * sub_803AD84 to draw a single OAM entry. `struct icon_record` is an
 * array of these, 8 bytes apart, starting at offset 0x10 - sub_8006600
 * (src/oam_count.c, parked) uses slots 0 and 2 (a wide icon spanning
 * two OAM entries); sub_8000EE4 (src/text_layout.c, parked) uses slots
 * 1, 3, and 5 (per-glyph and newline-marker OAM entries). */
struct icon_slot {
    s16 offset;
    u8 unused_2[2];
    void *ptr;
};

struct icon_record {
    u8 unused_00[0x10];
    struct icon_slot slots[6];
};

/* An OAM "icon" positioner: screen X/Y for the icon, then a pointer to
 * a small record describing which OAM slot(s) to draw it into.
 * gUnknown_030012E0/gUnknown_030012DC (src/oam_count.c) are two
 * instances of this, used for a left/right icon pair flanking a number
 * in sub_8006600; sub_8000EE4 takes one as its render-target object. */
struct icon_manager {
    u8 unused_00[0x110];
    u32 posX;
    u32 posY;
    u8 unused_118[0x130 - 0x118];
    struct icon_record *record;
};

#endif /* __ICON_MANAGER_H__ */

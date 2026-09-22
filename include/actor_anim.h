#ifndef __ACTOR_ANIM_H__
#define __ACTOR_ANIM_H__

/*
 * The category-descriptor -> vtable -> animation-table -> keyframe ->
 * frame system used to render rotating pickup/hazard sprites (crates,
 * fruit, balloons, checkpoint text, ...). See docs/graphics.md, "Found
 * the real per-actor animation-frame system" onward, for how each of
 * these was reversed, and each graphics/unknown/<sheet>/entities.json
 * for the actual data (every category, every animation-table record, every
 * keyframe, resolved to the exact ROM address or extracted frame PNG it
 * uses).
 *
 * None of the *code* behind this system (SelectActorCategory, InitActorPart,
 * GetAnimFrameData, the per-category vtable functions, ...) has been
 * reversed to C yet - these structs are only the data layout, written
 * ahead of that so the two can be matched up directly once it is.
 */

/* gStaticData_081796CC (categories 0-2, 41 slots) and gStaticData_0817B2A4
 * (categories 3-6, 47 slots) - one record per animation clip. Several
 * slots in each table are exact duplicates of an earlier slot's
 * table_A/table_B pair (see "duplicate_of_slot" in entities.json) -
 * still real, valid slots, just aliasing another one's data rather than
 * a copy. */
struct anim_table_record {
    u32 index;                 // 0x00 - equals the record's own slot number in every valid record observed
    struct keyframe_entry *table_A; // 0x04 - keyframe/timing sequence, see below
    u32 *table_B;               // 0x08 - frame address/offset array, see the comment above struct sprite_frame
    u8 header_byte;              // 0x0C - copied into the runtime per-part instance at offset +0x18 by InitActorPart; role beyond that not traced
    u8 pad_0D[3];
    u8 unknown_10[0x18];         // 0x10 - always 0 in every record observed
}; // 0x28
COMPILE_TIME_ASSERT(sizeof(struct anim_table_record) == 0x28);

/* table_A: one entry per keyframe in an animation clip, terminated
 * implicitly (there's no end marker - a record's real keyframe count is
 * however many entries have a table_B_index that's actually in-bounds
 * for that record's own table_B array before the data stops looking
 * like a valid entry; see table_b_real_length() in tools/dump_entities.py). */
struct keyframe_entry {
    u8 unknown_00;    // 0x00 - alternates between two values (seen 0x80/0x40) across entries in every record sampled; role unclear
    u8 unknown_01;    // 0x01 - 0 in every entry sampled
    s16 table_B_index; // 0x02 - signed index into this record's table_B array - the one confirmed field, both by code (GetAnimFrameData) and by exhaustive empirical resolution against every record's real frame data
    u32 unknown_04;    // 0x04 - a small integer, or two packed 16-bit sub-values a fixed distance apart; role unclear
    u32 unknown_08;    // 0x08 - 0 in every entry sampled
}; // 0xC
COMPILE_TIME_ASSERT(sizeof(struct keyframe_entry) == 0xC);

/*
 * table_B (see struct anim_table_record above) is an array of raw u32
 * entries, in one of two addressing modes - fixed per record, never
 * mixed within one record:
 *
 *   - "absolute_rom": each entry is a real ROM pointer straight to a
 *     struct sprite_frame. Used only by the "mask"-style records (index
 *     0 of both animation tables) - these frames deliberately overlap
 *     byte-for-byte with their neighbors (a rotation-strip compression
 *     trick, see docs/graphics.md) rather than living in the category's
 *     sprite sheet at all, so there's no extracted PNG for them.
 *
 *   - "pool_offset": each entry is a byte offset from the start of the
 *     category family's decompressed sprite sheet
 *     (gStaticData_080B2120 for categories 0-2, gStaticData_0814174C for
 *     3-6) to a struct sprite_frame. These frames *are* extracted - see
 *     graphics/unknown/<sheet>/<entity>/NN.png, one file per frame.
 *
 * entities.json's "addressing_mode" field records which mode each
 * record uses; "frames" resolves every keyframe-used table_B entry to
 * either {rom_address, w_tiles, h_tiles} or {asset_file, w_tiles, h_tiles}
 * accordingly.
 */

/* One animation frame's raw pixel data, exactly as DMA'd to VRAM with no
 * reformatting (confirmed via the DMA3 register writes in the queued
 * transfer flush routine, FlushVramDmaQueue) - so this is also exactly what
 * graphics/unknown/<sheet>/<entity>/NN.png round-trips to/from (that
 * tool strips/reinserts this same 4-byte header - see tools/framed_gfx.py). */
struct sprite_frame {
    u8 width_tiles;   // 0x00 - always 1, 2, 4, or 8 in every frame observed
    u8 height_tiles;  // 0x01 - always 1, 2, 4, or 8 in every frame observed
    u8 pad2;          // 0x02 - always 0x10 in both category families' sheets
    u8 pad3;          // 0x03 - always 0x00
    u8 tile_data[0];  // 0x04 - width_tiles*height_tiles*32 bytes, standard swizzled 4bpp tile data
};

/* gStaticData_08175558 - 7 entries (categories 0-2 use the family rooted
 * at gStaticData_081796CC, 3-6 the one at gStaticData_0817B2A4). Each
 * category's `type` field (0/1/2, see below) - NOT the category index
 * itself - selects a shared vtable: traced the one real call site, and
 * the caller reads gStaticData_08175558[category].type into the
 * register it passes as SelectActorCategory's first argument, which
 * then computes gStaticData_081756C4 + type*0x34 (confirmed against
 * SelectActorCategory's own disassembly). Categories 0/1/2 all have
 * type 0 and share one vtable; 3/4/5 all have type 1 and share another;
 * 6 is type 2, on its own - three real vtables total, not seven, so
 * gStaticData_081756C4 below is declared [3]. Whatever data actually
 * follows those three at gStaticData_081756C4+0x9C (ROM 0x08175760) is
 * unrelated - it doesn't decode as a 4th/5th/6th/7th vtable no matter
 * how it's sliced (verified directly against the raw bytes), so
 * there's no "categories 3-6 vtable" to go looking for. */
/* category_descriptor.sub_effect_table's record layout - resolved from
 * `sub_802968C` (counts matching `variantA` entries), `SelectActorCategory`
 * (which stores this pointer directly into `gUnknown_03001400`, indexing
 * with `idx*0x14`), and the `sub_802A504`/`51C`/`540`/`558`/`570`
 * per-index accessor family (see docs/rom_map.md's "The sub_802A5xx
 * siblings pin down sub_effect_table's runtime shape"). Record 0 doubles
 * as a combined header+entry: `field_04` there is the table's real entry
 * count (read by `sub_802968C`/`SelectActorCategory`), while every
 * record's own `field_00`/`field_04` otherwise serve as the *next*
 * record's threshold/opaque-accessor fields for `sub_802A51C`/
 * `sub_802A504` (`idx*0x14+0x14 == (idx+1)*0x14+0x0`, i.e. those two
 * accessors are reading one record ahead, not an oversized record). */
struct sub_effect_record {
    s32 field_00;   // 0x00 - selection threshold value (record 0: unused as a threshold, see above)
    s32 field_04;   // 0x04 - record 0 only: the table's real entry count
    u8 variantA;    // 0x08 - "kind"/kind byte, gated by category type in sub_802968C
    u8 variantB;    // 0x09 - alternate kind byte, selected by sub_802A570 when gUnknown_030012C0+0x8c is set
    u8 variantC;    // 0x0a - alternate kind byte, selected by sub_802A570 when gUnknown_03001414 is set
    u8 pad_0b;
    s32 offsetX;    // 0x0c - Q8.8 after sub_802A558's <<8
    s32 offsetY;    // 0x10 - Q8.8 after sub_802A540's <<8
}; // 0x14
COMPILE_TIME_ASSERT(sizeof(struct sub_effect_record) == 0x14);

struct category_descriptor {
    u32 type;                       // 0x00 - 0 for categories 0-2, 1 for 3-5, 2 for 6 - selects the shared vtable, see above
    void *family_shared_04;         // 0x04 - constant across all categories in one family; pointer-shaped, role unknown
    u32 family_shared_08;           // 0x08 - constant across all categories in one family; role unknown
    void *conditional_ptr_0C;       // 0x0C - NULL for categories 0-2 (type 0); a real pointer for every category in the 3-6 family (5 and 6 alias the exact same pointer) - a family-2-only extra graphics blob, not a per-category flag. When non-NULL, sub_802F7B0 (not reversed) DMAs a small header-prefixed tile blob from it to VRAM once during category init - see docs/graphics.md
    const u16 *palette;             // 0x10 - raw 16-color RGB555 palette, DMA'd to OBJ palette RAM (InitActorCategory)
    struct sub_effect_record *sub_effect_table; // 0x14 - a second per-category table (threshold-triggered sub-effects/spawns via vtable slot 1); see struct sub_effect_record above
    struct anim_table_record *anim_table; // 0x18 - this category's animation table base (gStaticData_081796CC or gStaticData_0817B2A4)
    const u8 *sprite_sheet;         // 0x1C - this category family's LZ77-compressed sprite sheet
    u32 unknown_20;                 // 0x20
    u32 active_count_threshold;     // 0x24 - compared against a running "how many of this category are active" counter (gUnknown_03001384) to gate spawning an extra sub-effect instance
    u32 unknown_28;                 // 0x28
    u32 position_offset_flag;       // 0x2C - zero/nonzero selects between two fixed position-offset constants (0xFFFFB000 / 0x2800) applied to a spawned part's vertical anchor
    u32 unknown_30;                 // 0x30
}; // 0x34
COMPILE_TIME_ASSERT(sizeof(struct category_descriptor) == 0x34);

/* gStaticData_081756C4 - 3 entries, type-indexed (see category_descriptor
 * above), not category-indexed - categories that share a type share one
 * of these. Exact signatures unknown (none of these functions have been
 * reversed to C yet); slot 0 is confirmed to be the constructor,
 * ConstructAnimTableState (receives the animation table base and the
 * descriptor's position_offset_flag) for type 0, but a different,
 * still-unnamed function for types 1/2 (which share it - constructor
 * logic splits by sprite-sheet family, not by type individually; see
 * docs/rom_map.md's "confirmed: mostly actor per-type behavior" section
 * for the raw addresses of all 39 slots and which ROM region each type's
 * slots 2-6 land in). A couple of slots (7 and 8, at least for type 0)
 * hold obviously-invalid addresses and appear to simply be unused for
 * that type. Placeholder names for the per-type functions once matched
 * (slots 2-6, the ones that actually differ between types):
 * "Actor0_", "Actor1_", "Actor2_" prefixes rather than a guessed
 * real-world name - see docs/naming.md. */
struct category_vtable {
    void (*fn[13])(void);
}; // 0x34
COMPILE_TIME_ASSERT(sizeof(struct category_vtable) == 0x34);

/* gStaticData_08175558 is already split out at exactly this size in
 * data/data.s (7*0x34 = 0x16C bytes before gStaticData_081756C4 starts).
 * gStaticData_081756C4 is only labeled for its first 3 entries there,
 * which is also all of it - there is no 4th/5th/6th/7th vtable to find,
 * see the comment on category_descriptor.type above. Whatever real data
 * follows it at gStaticData_081756C4+0x9C (ROM 0x08175760, currently
 * inside the still-generic gStaticData_08175760 label) is unrelated. */
extern struct category_descriptor gStaticData_08175558[7];
extern struct category_vtable gStaticData_081756C4[3];

/* Not split out as their own labels in data/data.s yet - the bytes exist
 * at these ROM addresses (currently inside larger unlabeled incbin
 * blocks), so these are the correct extern declarations for whenever
 * that's done, not yet linkable today. */
extern struct anim_table_record gStaticData_081796CC[41]; // 0x081796CC, categories 0-2
extern struct anim_table_record gStaticData_0817B2A4[47]; // 0x0817B2A4, categories 3-6

#endif /* !__ACTOR_ANIM_H__ */

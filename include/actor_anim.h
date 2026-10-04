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

/* A box in the actors' 16-bit world units: position then size. The code
 * reads it through several local views (actor_part74.c's `struct box16`,
 * actor_part24b.c's `struct box3`, actor_part126.c's `struct box12`,
 * ...). The anim_table_record's box_14 and several small src/data tables
 * are this. */
struct anim_box {
    s16 x, y, z;
    s16 w, h, d;
}; // 0xC
COMPILE_TIME_ASSERT(sizeof(struct anim_box) == 0xC);

struct anim_frame_record; /* actor_self.h */

/* gStaticData_081796CC (categories 0-2, 41 slots) and gStaticData_0817B2A4
 * (categories 3-6, 47 slots) - one record per animation clip, defined in
 * src/data/anim_family_178f80.c / anim_family_17aa6c.c. Several slots
 * share another slot's table_A/table_B arrays (see "duplicate_of_slot"
 * in entities.json), and slots 32-34 of the first table are all zero. */
struct anim_table_record {
    u32 index;                 // 0x00 - equals the record's own slot number in every valid record observed
    struct anim_frame_record *table_A; // 0x04 - the clip's keyframes (struct anim_frame_record, actor_self.h), no count stored
    u32 *table_B;               // 0x08 - frame address/offset array, see the comment above struct sprite_frame
    u8 header_byte;              // 0x0C - copied into the runtime per-part instance at offset +0x18 by InitActorPart; role beyond that not traced
    u8 pad_0D[3];
    s32 baseDepth;               // 0x10 - 0 or 0x260C-0x36D5: the depth at which the part draws unscaled (UpdateAnimatedActorPart divides by it)
    struct anim_box box_14;      // 0x14 - copied into the runtime per-part instance at +0x38 by InitActorPart; zero in some records
    s32 spawnX;                  // 0x20 - added to the spawn X by sub_802AC28 (the per-kind actor factory)
    s32 spawnY;                  // 0x24 - added to the spawn Y by sub_802AC28
}; // 0x28
COMPILE_TIME_ASSERT(sizeof(struct anim_table_record) == 0x28);

/* table_A is an array of struct anim_frame_record (actor_self.h): one
 * entry per keyframe, {duration, frameIndex (into table_B), loop
 * threshold, loop base, OAM attribute bits, 2 unknown bytes}. There's no
 * end marker or count; the actor's state code knows how many keyframes
 * each clip has. */

/*
 * table_B (see struct anim_table_record above) is an array of raw u32
 * entries, in one of two addressing modes - fixed per record, never
 * mixed within one record:
 *
 *   - "absolute_rom": each entry is a real ROM pointer straight to a
 *     zero-run-compressed frame ({w, h, 0x30, 0} + u16 run stream,
 *     unpacked by the gUnpackRleSpriteFrameFunc IWRAM hook). Used only by record
 *     0 of both animation tables (and the sub_802DFDC singleton's table):
 *     the frame sets are rle_sprites_0c2758.c / rle_sprites_15a050.c,
 *     built from graphics/rle_sprites/ (docs/data.md, "Compressed sprite
 *     frames").
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
    u8 variantB;    // 0x09 - alternate kind byte, selected by sub_802A570 when gLevelState+0x8c is set
    u8 variantC;    // 0x0a - alternate kind byte, selected by sub_802A570 when gUnknown_03001414 is set
    u8 pad_0b;
    s32 offsetX;    // 0x0c - Q8.8 after sub_802A558's <<8
    s32 offsetY;    // 0x10 - Q8.8 after sub_802A540's <<8
}; // 0x14
COMPILE_TIME_ASSERT(sizeof(struct sub_effect_record) == 0x14);

/* The 12 bytes after a table's last record: the first three words of a
 * record that isn't there, which the one-record-ahead accessors
 * (`sub_802A51C`/`sub_802A504`) read for the last record. `field_04` is
 * -1 like every real record's but record 0's. */
struct sub_effect_table_end {
    s32 field_00;   // 0x00 - the final threshold
    s32 field_04;   // 0x04 - always -1
    u8 variantA;    // 0x08 - no real record's bytes: zero in three of the seven tables, arbitrary in the rest
    u8 variantB;
    u8 variantC;
    u8 pad_0b;
}; // 0xC
COMPILE_TIME_ASSERT(sizeof(struct sub_effect_table_end) == 0xC);

/* A whole sub_effect_table as the ROM stores it (src/data/). */
#define SUB_EFFECT_TABLE(n) struct { struct sub_effect_record records[n]; struct sub_effect_table_end end; }

/* The start of a BG0 cell animation (category_descriptor.family_shared_04,
 * read by sub_8029890/sub_802996C/sub_80297C8 in actor_part95.c): a
 * 256-colour palette DMA'd whole to BG palette RAM, the grid size in 8x8
 * cells, then the frames, each `cols * rows` 4bpp tiles in row-major cell
 * order (plus, for type-0 categories, one 4-bit palette bank per cell
 * padded to a multiple of 4 bytes, handed to the gDrawMirroredTilemapFunc map
 * callback). family_shared_08 is the whole record's size. */
struct cell_anim_header {
    u16 palette[256];   // 0x000
    s16 cols;           // 0x200
    s16 rows;           // 0x202
}; // 0x204, the frames follow

/* The start of a BG1 picture (category_descriptor.conditional_ptr_0C,
 * read by sub_802F7B0 in actor_part45d.c): a 256-colour palette, the map
 * size, the tile count, then `u16 map[cols * rows]` (padded to a multiple
 * of 4 bytes), `tileCount` 4bpp tiles, and one 4-bit palette bank per map
 * entry, low nibble first. */
struct bg_picture_header {
    u16 palette[256];   // 0x000
    s16 cols;           // 0x200
    s16 rows;           // 0x202
    u32 tileCount;      // 0x204
}; // 0x208, the map follows

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

/* Both tables are defined in src/data/actor_category_175558.c (7*0x34 =
 * 0x16C bytes of descriptors, then gStaticData_081756C4). The latter
 * has 3 entries, which is also all of it - there is no 4th/5th/6th/7th
 * vtable to find,
 * see the comment on category_descriptor.type above. What follows it at
 * gStaticData_081756C4+0x9C (ROM 0x08175760) is unrelated: the BG
 * palette-cycle frames of src/data/palette_cycle_175760.c. */
extern const struct category_descriptor gStaticData_08175558[7];
extern const struct category_vtable gStaticData_081756C4[3];

/* Defined in src/data/anim_family_178f80.c and anim_family_17aa6c.c. */
extern const struct anim_table_record gStaticData_081796CC[41]; // 0x081796CC, categories 0-2
extern const struct anim_table_record gStaticData_0817B2A4[47]; // 0x0817B2A4, categories 3-6

#endif /* !__ACTOR_ANIM_H__ */

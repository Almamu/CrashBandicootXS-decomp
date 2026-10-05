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

/* A box in the actors' 16-bit world units: position then size. The
 * anim_table_record's box_14, `actor_self.box` and several small src/data
 * tables are this. A fixed box is copied into an actor as three words
 * (`struct vec3_words`, vehicle.h) for the ROM's `ldm`/`stm`. */
struct anim_box {
    s16 x, y, z;
    s16 w, h, d;
}; // 0xC
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct anim_box) == 0xC);

struct anim_frame_record; /* actor_self.h */

/* gCategoryFamily0AnimTable (categories 0-2, 41 slots) and gCategoryFamily1AnimTable
 * (categories 3-6, 47 slots) - one record per animation clip, defined in
 * src/data/anim_family_178f80.c / anim_family_17aa6c.c. Several slots
 * share another slot's table_A/table_B arrays (see "duplicate_of_slot"
 * in entities.json), and slots 32-34 of the first table are all zero. */
struct anim_table_record {
    u32 index;                 // 0x00 - equals the record's own slot number in every valid record observed
    struct anim_frame_record *table_A; // 0x04 - the clip's keyframes (struct anim_frame_record, actor_self.h), no count stored
    u32 *table_B;               // 0x08 - frame address/offset array, see the comment above struct sprite_frame
    u8 palette;                  // 0x0C - OBJ palette bank; InitActorPart copies it to actor_self.palette (+0x18), which DrawActor puts in OAM attr 2 (<< 12)
    u8 pad_0D[3];
    s32 baseDepth;               // 0x10 - 0 or 0x260C-0x36D5: the depth at which the part draws unscaled (DrawActor divides by it)
    struct anim_box box_14;      // 0x14 - copied into the runtime per-part instance at +0x38 by InitActorPart; zero in some records
    s32 spawnX;                  // 0x20 - added to the spawn X by CreateActor (the per-kind actor factory)
    s32 spawnY;                  // 0x24 - added to the spawn Y by CreateActor
}; // 0x28
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct anim_table_record) == 0x28);

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
 *     0 of both animation tables (and the CreateYeti singleton's table):
 *     the frame sets are rle_sprites_0c2758.c / rle_sprites_15a050.c,
 *     built from graphics/rle_sprites/ (docs/data.md, "Compressed sprite
 *     frames").
 *
 *   - "pool_offset": each entry is a byte offset from the start of the
 *     category family's decompressed sprite sheet
 *     (gPolarSpriteSheet for categories 0-2, gJetpackSpriteSheet for
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

/* gActorCategories - 7 entries (categories 0-2 use the family rooted
 * at gCategoryFamily0AnimTable, 3-6 the one at gCategoryFamily1AnimTable). Each
 * category's `type` field (0/1/2, see below) - NOT the category index
 * itself - selects a shared vtable: traced the one real call site, and
 * the caller reads gActorCategories[category].type into the
 * register it passes as SelectActorCategory's first argument, which
 * then computes gActorCategoryVtables + type*0x34 (confirmed against
 * SelectActorCategory's own disassembly). Categories 0/1/2 all have
 * type 0 and share one vtable; 3/4/5 all have type 1 and share another;
 * 6 is type 2, on its own - three real vtables total, not seven, so
 * gActorCategoryVtables below is declared [3]. Whatever data actually
 * follows those three at gActorCategoryVtables+0x9C (ROM 0x08175760) is
 * unrelated - it doesn't decode as a 4th/5th/6th/7th vtable no matter
 * how it's sliced (verified directly against the raw bytes), so
 * there's no "categories 3-6 vtable" to go looking for. */
/* category_descriptor.spawnTable's record layout: the stage's actor spawn
 * list, ordered by `field_00` (the depth at which the next record is due).
 * RunActorCategoryFrame hands `&record.kind` of each due record to the
 * category vtable's slot 1 (SpawnActor for type 0), which reads it as
 * `{kind, altKind, bonusKind, pad, x, y, z}` - so a spawn's own depth is
 * the following record's `field_00`. Resolved from
 * `CountCategoryCrates` (counts matching `kind` entries), `SelectActorCategory`
 * (which stores this pointer directly into `gActorSpawnTable`, indexing
 * with `idx*0x14`), and the `sub_802A504`/`51C`/`540`/`558`/`570`
 * per-index accessor family (see docs/rom_map.md's "The sub_802A5xx
 * siblings pin down spawnTable's runtime shape"). Record 0 doubles
 * as a combined header+entry: `field_04` there is the table's real entry
 * count (read by `CountCategoryCrates`/`SelectActorCategory`), while every
 * record's own `field_00`/`field_04` otherwise serve as the *next*
 * record's threshold/opaque-accessor fields for `sub_802A51C`/
 * `sub_802A504` (`idx*0x14+0x14 == (idx+1)*0x14+0x0`, i.e. those two
 * accessors are reading one record ahead, not an oversized record). */
struct sub_effect_record {
    s32 field_00;   // 0x00 - selection threshold value (record 0: unused as a threshold, see above)
    s32 field_04;   // 0x04 - record 0 only: the table's real entry count
    u8 kind;        // 0x08 - the actor kind CreateActor builds; counted by CountCategoryCrates
    u8 altKind;     // 0x09 - the kind used instead when gLevelState+0x8c is set (SpawnActor, sub_802A570)
    u8 bonusKind;   // 0x0a - the kind used instead when gUnknown_03001414 is set (SpawnActor's useBonus, sub_802A570)
    u8 pad_0b;
    s32 offsetX;    // 0x0c - Q8.8 after sub_802A558's <<8
    s32 offsetY;    // 0x10 - Q8.8 after sub_802A540's <<8
}; // 0x14
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct sub_effect_record) == 0x14);

/* The 12 bytes after a table's last record: the first three words of a
 * record that isn't there, which the one-record-ahead accessors
 * (`sub_802A51C`/`sub_802A504`) read for the last record. `field_04` is
 * -1 like every real record's but record 0's. */
struct sub_effect_table_end {
    s32 field_00;   // 0x00 - the final threshold
    s32 field_04;   // 0x04 - always -1
    u8 kind;    // 0x08 - no real record's bytes: zero in three of the seven tables, arbitrary in the rest
    u8 altKind;
    u8 bonusKind;
    u8 pad_0b;
}; // 0xC
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct sub_effect_table_end) == 0xC);

/* A whole spawnTable as the ROM stores it (src/data/). */
#define SUB_EFFECT_TABLE(n) struct { struct sub_effect_record records[n]; struct sub_effect_table_end end; }

/* The start of a BG0 cell animation (category_descriptor.cellAnim,
 * read by InitCellAnim/ResetCellAnimBg/UploadCellAnimFrame in cell_anim.c): a
 * 256-colour palette DMA'd whole to BG palette RAM, the grid size in 8x8
 * cells, then the frames, each `cols * rows` 4bpp tiles in row-major cell
 * order (plus, for type-0 categories, one 4-bit palette bank per cell
 * padded to a multiple of 4 bytes, handed to the gDrawMirroredTilemapFunc map
 * callback). cellAnimSize is the whole record's size. */
struct cell_anim_header {
    u16 palette[256];   // 0x000
    s16 cols;           // 0x200
    s16 rows;           // 0x202
}; // 0x204, the frames follow

/* The start of a BG1 picture (category_descriptor.bgPicture,
 * read by LoadBgPicture in bg_picture.c): a 256-colour palette, the map
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
    void *cellAnim;                 // 0x04 - the BG0 cell animation (gCategoryFamily0CellAnim/gCategoryFamily1CellAnim), played by InitCellAnim
    u32 cellAnimSize;               // 0x08 - its size in bytes
    void *bgPicture;                // 0x0C - the BG1 picture LoadBgPicture shows (gCategoryNBgPicture); NULL for type-0 categories (0-2), 5 and 6 share one
    const u16 *palette;             // 0x10 - raw 16-color RGB555 palette, DMA'd to OBJ palette RAM (InitActorCategory)
    struct sub_effect_record *spawnTable; // 0x14 - the stage's actor spawn list (gCategoryNSpawnTable), see struct sub_effect_record above
    struct anim_table_record *anim_table; // 0x18 - this category's animation table base (gCategoryFamily0AnimTable or gCategoryFamily1AnimTable)
    const u8 *sprite_sheet;         // 0x1C - this category family's LZ77-compressed sprite sheet
    u32 unknown_20;                 // 0x20
    u32 active_count_threshold;     // 0x24 - compared against a running "how many of this category are active" counter (gActorCategoryDeaths) to gate spawning an extra sub-effect instance
    u32 unknown_28;                 // 0x28
    u32 position_offset_flag;       // 0x2C - zero/nonzero selects between two fixed position-offset constants (0xFFFFB000 / 0x2800) applied to a spawned part's vertical anchor
    u32 unknown_30;                 // 0x30
}; // 0x34
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct category_descriptor) == 0x34);

/* gActorCategoryVtables - 3 entries, type-indexed (see category_descriptor
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
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct category_vtable) == 0x34);

/* Both tables are defined in src/data/actor_category_175558.c (7*0x34 =
 * 0x16C bytes of descriptors, then gActorCategoryVtables). The latter
 * has 3 entries, which is also all of it - there is no 4th/5th/6th/7th
 * vtable to find,
 * see the comment on category_descriptor.type above. What follows it at
 * gActorCategoryVtables+0x9C (ROM 0x08175760) is unrelated: the BG
 * palette-cycle frames of src/data/palette_cycle_175760.c. */
extern const struct category_descriptor gActorCategories[7];
extern const struct category_vtable gActorCategoryVtables[3];

/* Defined in src/data/anim_family_178f80.c and anim_family_17aa6c.c. */
extern const struct anim_table_record gCategoryFamily0AnimTable[41]; // 0x081796CC, categories 0-2
extern const struct anim_table_record gCategoryFamily1AnimTable[47]; // 0x0817B2A4, categories 3-6

#endif /* !__ACTOR_ANIM_H__ */

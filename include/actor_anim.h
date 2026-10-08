#ifndef __ACTOR_ANIM_H__
#define __ACTOR_ANIM_H__

#include "constants/categories.h"
#include "actor_self.h"

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

/* struct anim_box, the actors' boxes, is actor_self.h's. */

struct anim_frame_record; /* actor_self.h */

/* gCategoryFamily0AnimTable (categories 0-2, 41 slots) and gCategoryFamily1AnimTable
 * (categories 3-6, 47 slots) - one record per animation clip, defined in
 * src/data/anim_family_178f80.c / anim_family_17aa6c.c. Several slots
 * share another slot's table_A/table_B arrays (see "duplicate_of_slot"
 * in entities.json), and slots 32-34 of the first table are all zero. */
struct anim_table_record {
    u32 index; // 0x00 - equals the record's own slot number in every valid record observed
    // 0x04 - the clip's keyframes (struct anim_frame_record, actor_self.h), no count stored
    struct anim_frame_record *table_A;
    u32 *table_B; // 0x08 - frame address/offset array, see the comment below
    // 0x0C - OBJ palette bank; InitActorPart copies it to actor_self.palette (+0x18), which
    // DrawActor puts in OAM attr 2 (<< 12)
    u8 palette;
    u8 pad_0D[3];
    // 0x10 - 0 or 0x260C-0x36D5: the depth at which the
    // part draws unscaled (DrawActor divides by it)
    s32 baseDepth;
    // 0x14 - copied into the runtime per-part instance
    // at +0x38 by InitActorPart; zero in some records
    struct anim_box box_14;
    s32 spawnX; // 0x20 - added to the spawn X by CreateActor (the per-kind actor factory)
    s32 spawnY; // 0x24 - added to the spawn Y by CreateActor
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
 *     3-6) to a struct actor_frame_pixels. These frames *are* extracted - see
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
struct actor_frame_pixels {
    u8 width_tiles;  // 0x00 - always 1, 2, 4, or 8 in every frame observed
    u8 height_tiles; // 0x01 - always 1, 2, 4, or 8 in every frame observed
    u8 pad2;         // 0x02 - always 0x10 in both category families' sheets
    u8 pad3;         // 0x03 - always 0x00
    u8 tile_data[0]; // 0x04 - width_tiles*height_tiles*32 bytes, standard swizzled 4bpp tile data
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
 * list, ordered by `depth`. A spawn's data straddles two records:
 * RunActorCategoryFrame hands `&record[i].kind` of each due record to the
 * category vtable's slot 1 (SpawnActor for type 0), which reads it as
 * `{kind, altKind, bonusKind, pad, x, y, depth, link}` - so a spawn's own
 * depth (the course distance at which it is due) and link are the
 * *following* record's `depth`/`link`. GetActorSpawnZ and
 * GetActorSpawnNextTarget read them that way (`idx*0x14+0x14 ==
 * (idx+1)*0x14+0x0`: one record ahead, not an oversized record), and
 * SelectActorCategory/RunActorCategoryFrame's due tests do the same.
 * Record 0 doubles as the table header: its `depth` is the course length
 * (past it RunActorCategoryFrame calls the vtable's slot 10, the "reach
 * course end" function) and its `link` the entry count
 * (CountCategoryCrates/SelectActorCategory). Resolved from
 * `CountCategoryCrates` (counts matching `kind` entries), `SelectActorCategory`
 * (which stores this pointer directly into `gActorSpawnTable`, indexing
 * with `idx*0x14`), and the GetActorSpawnX/Y/Z/KindIndex/NextTarget
 * per-index accessor family (see docs/rom_map.md's "The sub_802A5xx
 * siblings pin down spawnTable's runtime shape"). */
struct sub_effect_record {
    // 0x00 - the previous record's spawn depth, in course distance units
    // (GetActorSpawnZ: Q8 after <<8); record 0: the course length
    s32 depth;
    // 0x04 - the previous record's next target: the spawn index a jetpack plane or
    // polar penguin homes in on after this one (GetActorSpawnNextTarget), -1: none;
    // record 0: the table's entry count
    s32 link;
    u8 kind; // 0x08 - the actor kind CreateActor builds; counted by CountCategoryCrates
    // 0x09 - the kind used instead when gLevelState+0x8c is set (SpawnActor, GetActorSpawnKindIndex)
    u8 altKind;
    // 0x0a - the kind used instead when gActorSpawnUseBonus
    // is set (SpawnActor's useBonus, GetActorSpawnKindIndex)
    u8 bonusKind;
    u8 pad_0b;
    s32 offsetX; // 0x0c - Q8.8 after GetActorSpawnX's <<8
    s32 offsetY; // 0x10 - Q8.8 after GetActorSpawnY's <<8
}; // 0x14
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct sub_effect_record) == 0x14);

/* The 12 bytes after a table's last record: the first three words of a
 * record that isn't there, which the one-record-ahead accessors
 * (`GetActorSpawnZ`/`GetActorSpawnNextTarget`) read for the last record. */
struct sub_effect_table_end {
    s32 depth; // 0x00 - the last spawn's depth
    s32 link;  // 0x04 - the last spawn's next target (always -1)
    // 0x08 - no real record's bytes: zero in three of the seven tables, arbitrary in the rest
    u8 kind;
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
    u16 palette[256]; // 0x000
    s16 cols;         // 0x200
    s16 rows;         // 0x202
}; // 0x204, the frames follow

/* The start of a BG1 picture (category_descriptor.bgPicture,
 * read by LoadBgPicture in bg_picture.c): a 256-colour palette, the map
 * size, the tile count, then `u16 map[cols * rows]` (padded to a multiple
 * of 4 bytes), `tileCount` 4bpp tiles, and one 4-bit palette bank per map
 * entry, low nibble first. */
struct bg_picture_header {
    u16 palette[256]; // 0x000
    s16 cols;         // 0x200
    s16 rows;         // 0x202
    u32 tileCount;    // 0x204
}; // 0x208, the map follows

struct category_descriptor {
    // 0x00 - 0 for categories 0-2, 1 for 3-5, 2 for 6 - selects the shared vtable, see above
    u32 type;
    // 0x04 - the BG0 cell animation
    // (gCategoryFamily0CellAnim/gCategoryFamily1CellAnim), played by InitCellAnim
    void *cellAnim;
    u32 cellAnimSize; // 0x08 - its size in bytes
    // 0x0C - the BG1 picture LoadBgPicture shows (gCategoryNBgPicture);
    // NULL for type-0 categories (0-2), 5 and 6 share one
    void *bgPicture;
    // 0x10 - raw 16-color RGB555 palette, DMA'd to OBJ palette RAM (InitActorCategory)
    const u16 *palette;
    // 0x14 - the stage's actor spawn list
    // (gCategoryNSpawnTable), see struct sub_effect_record above
    struct sub_effect_record *spawnTable;
    // 0x18 - this category's animation table base
    // (gCategoryFamily0AnimTable or gCategoryFamily1AnimTable)
    struct anim_table_record *anim_table;
    const u8 *sprite_sheet; // 0x1C - this category family's LZ77-compressed sprite sheet
    // 0x20 - deaths since the checkpoint (gActorCategoryDeaths) after which
    // IsActorMaskAssistDue reports true
    u32 maskAssistDeaths;
    // 0x24 - deaths since the checkpoint (gActorCategoryDeaths) after which the
    // spawns use their `bonusKind` (InitActorCategory -> gActorSpawnUseBonus)
    u32 bonusKindDeaths;
    // 0x28 - boss deaths (gActorCategoryBossDeaths) after which the boss is
    // created with `retryBossLevel` instead of `bossLevel`
    u32 retryBossDeaths;
    // 0x2C - the argument InitActorCategory passes to the boss's constructor
    // (category vtable slot 2: CreateYeti's gYetiParamsIndex,
    // CreateAirship's gAirshipLevel, CreateHovercraft's gHovercraftLevel)
    u32 bossLevel;
    u32 retryBossLevel; // 0x30 - the same, once retryBossDeaths is reached
}; // 0x34
COMPILE_TIME_ASSERT(actor_anim_h, sizeof(struct category_descriptor) == 0x34);

/* gActorCategoryVtables - 3 entries, type-indexed (see category_descriptor
 * above), not category-indexed - categories that share a type share one
 * of these. Exact signatures unknown (none of these functions have been
 * reversed to C yet); slot 0 is confirmed to be the constructor,
 * ConstructAnimTableState (receives the animation table base and the
 * checkpoint) for type 0, but a different,
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
extern const struct category_descriptor gActorCategories[CATEGORY_COUNT];
extern const struct category_vtable gActorCategoryVtables[CATEGORY_TYPE_COUNT];

/* Defined in src/data/anim_family_178f80.c and anim_family_17aa6c.c. */
extern const struct anim_table_record gCategoryFamily0AnimTable[41]; // 0x081796CC, categories 0-2
extern const struct anim_table_record gCategoryFamily1AnimTable[47]; // 0x0817B2A4, categories 3-6

#endif /* !__ACTOR_ANIM_H__ */

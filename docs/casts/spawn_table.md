# The actor spawn tables (#855)

`gActorSpawnTable` and `category_descriptor.spawnTable` were a `struct
sub_effect_record *`, an array of 0x14-byte records
`{depth, link, kind, altKind, bonusKind, pad, offsetX, offsetY}`. A
spawn's data straddled two records: SpawnActor was handed `&record[i].kind`
through a `(struct actor_spawn *)` byte-offset cast, and the spawn's own
depth and next target were the *next* record's `depth` and `link`. Record 0's
`depth` and `link` were the table's header, and 12 bytes followed the last
record (`struct sub_effect_table_end`). The readers cast the table to `u8 *`
to build those addresses: 16 casts, in actor_spawn.cpp's accessors,
SelectActorCategory and RunActorCategoryFrame.

## The table as the ROM reads it

The table is a header, then the spawns (actor.h):

```c
struct actor_spawn_entry {
    struct actor_spawn spawn; // 0x00 - kinds, x, y, z (the depth)
    s32 next;                 // 0x10 - the next target, -1: none
};
struct actor_spawn_table {
    s32 length;                          // 0x00 - the course length
    s32 count;                           // 0x04 - the number of spawns
    struct actor_spawn_entry entries[0]; // 0x08
};
```

Every reader is now a field access:

| reader | was | is |
|---|---|---|
| GetActorSpawnNextTarget | `*(s32 *)((u8 *)table + 0x14 + 4 + idx * 0x14)` | `table->entries[idx].next` |
| GetActorSpawnZ | `*(s32 *)((u8 *)table + 0x14 + idx * 0x14)` | `table->entries[idx].spawn.z` |
| GetActorSpawnX / Y | `*(s32 *)((u8 *)table + 0xC/0x10 + idx * 0x14)` | `table->entries[idx].spawn.x` / `.y` |
| GetActorSpawnKindIndex | `table[idx].kind` | `table->entries[idx].spawn.kind` |
| SelectActorCategory, RunActorCategoryFrame | `(struct actor_spawn *)((u8 *)table + i * 0x14 + 8)` | `&table->entries[i].spawn` |
| their due tests | `off`/`tb` locals, NextThreshold | `table->entries[i].spawn.z` |
| CountCategoryCrates | `table[i].kind`, `table[0].link` | `table->entries[i].spawn.kind`, `table->count` |

The accessors' code is what the layout predicts: for
`table->entries[idx].next` gcc adds `entries`' offset and the field's
(8 + 0x10) to the table pointer, then the scaled index (`adds r2, #24;
adds r2, r2, r1; ldr r0, [r2]`). The old byte-offset spelling was there
to give exactly that order. The two-locals comma expression and the
NextThreshold inline in SelectActorCategory's loops aren't needed
either.

Two places keep a shape for the match, without casts:
- GetActorSpawnKindIndex reads `table->entries[idx].spawn.kind` (and
  `altKind`, `bonusKind`) each time. Through a pointer to the entry,
  gcc adds `entries`' offset first.
- CountCategoryCrates' jetpack branch counts `total2` down beside its
  index. gcc then walks the table pointer itself (`adds r3, #0x14`,
  `ldrb [r3, #8]`).

## The data

The seven tables (src/game/vehicles/polar/data/polar_cell_anim.cpp, polar_spawn_tables.cpp,
jetpack_cell_anim.cpp, jetpack_sky_pictures.cpp) are now
`ACTOR_SPAWN_TABLE(n)`: the course length, the count, `n` entries
`{ { kind, altKind, bonusKind, unk_03, x, y, depth }, next }`, and the
4 bytes after the last entry (`end`, zero in three tables). Each spawn's
depth and next target are now on its own line. Every data object is the
same.

## Left

- vehicle.hpp's `struct spawn_arg` (the planes' and penguins'
  constructors) is the same record as `struct actor_spawn_entry`, and
  CreateActor/CreateJetpackActor cast the `actor_spawn *` they are
  handed to it. Merging the two changes the constructors' mangled names.
- actor_categories.cpp declares the tables as `const u8 []` and
  casts them in gActorCategories' initializers. That is src/*/data.

# The room's entity parameter records (#855)

## level_entity_list::params is const u8 *

`params` (level_data.h, 0x0C) points at the room's parameter records.
They have different lengths, and `paramOffsets[id]` gives each one's
offset in **bytes**. The field was a `const u32 *`, and all 7 reads cast
it to `u8 *` before adding the offset. It is now a `const u8 *`, and the
`(u8 *)` casts go:

- `EntityParams` (spawners.hpp);
- Crate::Create's `PLACEMENT` (crate_create.cpp);
- LevelEntityFlags::CountCrateEntities (entity_flags.cpp);
- DingodileCtrl::SpawnShieldOrRocket: its `bits` is a `const u8 *` now.

tools/levels.py builds the room entity lists (`build/.../level_rooms_*.inc`)
with `(const u8 *)&gRoomNParams` instead of `(const u32 *)`. The data
objects are the same.

Two casts stay, and they are real conversions: the record at a byte
offset, viewed as its record type. These are EntityParams'
`(const struct entity_params *)` and CountCrateEntities' `(const s16 *)`,
which reads the outline crate's `solidKind` at +8.

## Platform::Create reads struct entity_params

Platform::Create read its record through gobj_1a794.h's `struct
spawn_rec`, with `(struct spawn_rec *)((const u8 *)lvl->params +
offsets[index])`. The layout is entity_params' layout: the flags byte, then
the per-type words from 0x04. It is now entity_params' `p.platform` view
(`type`, `distX`, `distY`, `dirX`, `dirY`, `flag`), read through
`EntityParams(index)`, and `struct spawn_rec` is gone. platform_create.o
is the same.

crate.hpp's `struct crate_placement` stays its own view. It uses byte 1
(`options`), which is padding in entity_params.

## CrateList::nodes stays

CrateList::ResetGrid zeroes the nodes through `((CrateGridNodeInit
*)nodes)[i]` (5 casts). The view's fields are `void *`. Through the
real `CrateGridNode *` fields, gcc takes each zeroing store as a possible
write to `nodes` (a `CrateGridNode *` too) and reloads it, and the ROM
doesn't. It is the codegen view crate_list.hpp describes.

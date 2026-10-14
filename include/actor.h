#ifndef __ACTOR_H__
#define __ACTOR_H__

/* Two things share this header (docs/headers.md):
 *
 * - the entity's flags (`union EntityFlags`), class Entity's
 *   (include/entity.hpp);
 * - the actor subsystem (src/actor/): every function it defines, and the
 *   globals and data tables its files use. The prototypes are copied from
 *   the definitions. A .c file that needs a different local declaration
 *   for codegen keeps it as an asm-label alias with a `codegen:` comment.
 *
 * The bosses and vehicles built on ActorSelf (actor_self.hpp) are
 * declared in bosses.h and vehicle.hpp. */

#include "core.h"
#include "constants/categories.h"

/* The entity flags byte at +0x0C and the one after it, +0x0D (Entity's
 * `f`). */
union EntityFlags {
    u8 flags; // 0x0C
    struct {
        u8 gone:1;       // removed (SetGone); the lists drop it
        u8 attacks:1;    // its attack box hits the collidables (Player::TouchPlayer);
                         // only Player::Reset sets it (Get/Set/ClearFlag1)
        u8 visible:1;    // in contact with the player (Is/Enable/DisableContact)
        u8 bit3:1;       // touched by the player or another object; SetTargetAnim clears it
        u8 active:1;     // always active: skips the camera tests (updated off screen too)
        u8 unk_5:1;      // Get/Set/ClearSpriteObjFlag5
        u8 vulnerable:1; // the player's attacks hit it
        u8 collides:1;   // Is/Enable/DisableCollision
        u8 floorProbe:1; // 0x0D - a ground sprite probes the floor (Enable/DisableFloorProbe)
        u8 grounded:1;   // a ground sprite stands on the floor (ProbeFloor)
        u8 blink:1;      // hidden this frame (a blinking part; Is/ToggleHidden)
        u8 solid:1;      // pushes the player out (Is/Set/ClearSolid)
        u8 exitMirror:1; // a bonus platform's exit facing (Platform::Get/SetExitMirror)
    } b;                 // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    struct {
        u8 flags;  // 0x0C
        u8 flags2; // 0x0D
    } bytes;
};

/* The bits of the two bytes (#843, docs/flags/entity.md), for the code
 * that reads or writes the whole byte rather than a bitfield of `b`:
 * ENTITY_BIT_* / ENTITY_BIT2_* is a bit's number in `f.flags` (+0x0C) /
 * `f.bytes.flags2` (+0x0D), ENTITY_FLAG_* / ENTITY_FLAG2_* its mask. The
 * names follow the accessors (entity.hpp, sprite_obj.hpp); the bitfield
 * names above are older (`visible` is the contact bit, `bit3` touched,
 * `active` always active, `blink` hidden). */
#define ENTITY_BIT_GONE 0          // removed (MarkGone); the part and crate lists drop it
#define ENTITY_BIT_ATTACKS 1       // its attack box hits (Player::Reset; Get/Set/ClearFlag1)
#define ENTITY_BIT_CONTACT 2       // in contact with the player (Is/Enable/DisableContact)
#define ENTITY_BIT_TOUCHED 3       // touched by the player or an object (IsTouched)
#define ENTITY_BIT_ALWAYS_ACTIVE 4 // updated off screen too (IsAlwaysActive; the spawners)
// Set by GroundSprite::CheckPlayerContact when its floor hit has no edge
// terrain under it (it walked off a ledge), cleared on each floor hit; only
// GetFlag5 reads it, so what it is for isn't known.
#define ENTITY_BIT_UNK5 5
#define ENTITY_BIT_VULNERABLE 6 // the player's attacks hit it (Sprite::IsVulnerable)
#define ENTITY_BIT_COLLIDES 7   // probes the terrain (Is/Enable/DisableCollision)

#define ENTITY_FLAG_GONE (1 << ENTITY_BIT_GONE)
#define ENTITY_FLAG_ATTACKS (1 << ENTITY_BIT_ATTACKS)
#define ENTITY_FLAG_CONTACT (1 << ENTITY_BIT_CONTACT)
#define ENTITY_FLAG_TOUCHED (1 << ENTITY_BIT_TOUCHED)
#define ENTITY_FLAG_ALWAYS_ACTIVE (1 << ENTITY_BIT_ALWAYS_ACTIVE)
#define ENTITY_FLAG_UNK5 (1 << ENTITY_BIT_UNK5)
#define ENTITY_FLAG_VULNERABLE (1 << ENTITY_BIT_VULNERABLE)
#define ENTITY_FLAG_COLLIDES (1 << ENTITY_BIT_COLLIDES)

#define ENTITY_BIT2_FLOOR_PROBE 0 // a ground sprite probes the floor (Enable/DisableFloorProbe)
#define ENTITY_BIT2_GROUNDED 1    // on the floor (ProbeFloor; IsGrounded)
#define ENTITY_BIT2_HIDDEN 2      // hidden this frame, a blinking sprite (Is/ToggleHidden)
#define ENTITY_BIT2_SOLID 3       // pushes the player out (Is/Set/ClearSolid)
#define ENTITY_BIT2_EXIT_MIRROR 4 // a bonus platform's exit facing (Get/SetExitMirror)

#define ENTITY_FLAG2_FLOOR_PROBE (1 << ENTITY_BIT2_FLOOR_PROBE)
#define ENTITY_FLAG2_GROUNDED (1 << ENTITY_BIT2_GROUNDED)
#define ENTITY_FLAG2_HIDDEN (1 << ENTITY_BIT2_HIDDEN)
#define ENTITY_FLAG2_SOLID (1 << ENTITY_BIT2_SOLID)
#define ENTITY_FLAG2_EXIT_MIRROR (1 << ENTITY_BIT2_EXIT_MIRROR)

/* actor_anim.h has the full definitions; it can't be included here,
 * since several includers of this header define their own `struct
 * anim_box`/`struct sprite_frame` (#574). */
struct anim_table_record;
struct category_vtable;
struct actor_spawn_table;

/* A level spawn record as PolarPlayer::SpawnActor and SpawnJetpackActor (vehicle.hpp)
 * take it: the actor kinds, then the position in tile units (<< 8 to Q8)
 * and the depth. It is an entry of a spawn table (struct
 * actor_spawn_table below; actor_factory.cpp's and jetpack_spawn.cpp's
 * copies were merged here, #656). */
struct actor_spawn {
    u8 kind;      // 0x00 - the normal kind
    u8 altKind;   // 0x01 - in time trial mode
    u8 bonusKind; // 0x02 - when the bonus kinds are on (`useBonus`, `alt`)
    // 0x03 - not padding: the spawn tables (src/data/) hold 0 in most
    //        records and 1, 3, 17, 33, 53, 108, 125, 138, 155, 157, 161
    //        or 191 in others; nothing reads it
    u8 unk_03;
    s32 x; // 0x04 - Q8.8 after ActorSpawns::GetX's <<8
    s32 y; // 0x08 - Q8.8 after ActorSpawns::GetY's <<8
    // 0x0C - the course distance at which the spawn is due (ActorSpawns::GetZ:
    //        Q8 after <<8; ActorCategory::Select, ActorCategory::RunFrame)
    s32 z;
};

/* One spawn of a spawn table: the spawn record PolarPlayer::SpawnActor is handed, and
 * the spawn index a jetpack plane or polar penguin homes in on after this
 * one (ActorSpawns::GetNextTarget; -1: none). vehicle.hpp's struct spawn_arg
 * is the same record, as the planes' and the penguins' constructors take
 * it. */
struct actor_spawn_entry {
    struct actor_spawn spawn; // 0x00
    s32 next;                 // 0x10
}; // 0x14
COMPILE_TIME_ASSERT(actor_h, sizeof(struct actor_spawn_entry) == 0x14);

/* A stage's actor spawn list (category_descriptor.spawnTable, actor_anim.h;
 * ActorSpawns::table), ordered by depth: the course length (past it
 * ActorCategory::RunFrame calls the vtable's reachCourseEnd), the number of
 * spawns, then the spawns. The accessors (actor_spawn.cpp) take a spawn by
 * its index. Every access is `table->entries[i].field`, whose address
 * gcc builds as the ROM does: `entries`' offset and the field's are added
 * to the table pointer, then the index (ActorSpawns::GetX's
 * `adds r2, #12; adds r2, r2, r1`). */
struct actor_spawn_table {
    s32 length;                          // 0x00 - the course length, in course distance units
    s32 count;                           // 0x04 - the number of spawns
    struct actor_spawn_entry entries[0]; // 0x08
};

/* A whole spawn table as the ROM stores it (src/data/): `n` spawns, then 4
 * bytes that no spawn reads (zero in three of the seven tables). */
#define ACTOR_SPAWN_TABLE(n) \
    struct { s32 length; s32 count; struct actor_spawn_entry entries[n]; u8 end[4]; }

/* The actor zone (src/actor/): the 3D actor object (ActorSelf),
 * its animation, spawning, category frame and backgrounds. */

/* ActorSelf's own tag (actor_self.hpp): an incomplete struct to C, the
 * class to C++. */
struct ActorSelf;

/* src/actor/actor_bg.cpp (the rest is class ActorBg, actor_bg.hpp) */
extern void ActorCategoryEndStub(void);

/* src/actor/bg_picture.cpp */
extern void LoadBgPicture(const u8 *pic);
extern void FillBgPictureMap(const u8 *nib, const u16 *map, s32 cols, s32 rows);

/* src/actor/cell_anim.cpp (the rest is class CellAnim, cell_anim.hpp) */
extern void ActorCategoryAttemptEndStub(void);

/* The actor zone's globals (sym_iwram.txt). */
extern struct anim_table_record *gActorAnimTable;
extern s32 gActorFarClipDepth;
extern s32 gActorNearClipDepth;
/* The projection distance (Q8): DrawActor scales x/y by
 * gActorFocalLength / depth, and InitCellAnim starts the cell animation
 * this far from the checkpoint. Set by ActorBg::InitScroll: 47 px in the
 * polar levels, 28 px in the others. */
extern s32 gActorFocalLength;
#define ACTOR_FOCAL_LENGTH_POLAR INT_TO_Q8(47)
#define ACTOR_FOCAL_LENGTH       INT_TO_Q8(28)
/* The Q12 projection of the 3D levels with the non-polar focal length
 * (the jetpack player, the airship and the hovercraft): ACTOR_PROJECTION
 * / depth (a Q8 depth) is the Q12 scale that Q12_MUL applies to a Q8
 * position, as ActorSelf::Draw's INT_TO_Q12(gActorFocalLength) / depth. */
#define ACTOR_PROJECTION INT_TO_Q12(ACTOR_FOCAL_LENGTH)

/* src/iwram/iwram_data.cpp */
extern void (*gDrawMirroredTilemapFunc)(u8 *pal, s32 lowBlock, s32 w, s32 h);
#ifdef __cplusplus
extern void (*gHeapSortActorsByKeyFunc)(s32 n, class ActorSelf **list);
#endif

#endif /* !__ACTOR_H__ */

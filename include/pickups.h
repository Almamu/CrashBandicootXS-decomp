#ifndef GUARD_PICKUPS_H
#define GUARD_PICKUPS_H

/* The pickups subsystem (src/pickups/): the wumpa fruit, the extra life
 * and the time-trial stopwatch. Wumpas and extra lives are `struct
 * orbit_part`s (include/orbit_part.h); the stopwatch is a plain 0x40-byte
 * sprite object.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * `ResetActionCtrl` (src/pickups/wumpa.c) only shares the file's ROM
 * range; it is in player.h. */

#include "core.h"
#include "actor.h"
#include "orbit_part.h"
#include "vtable.h"

/* A copy of one of the hop width tables, as the struct UpdateExtraLifeHop
 * and UpdateWumpaHop copy onto the stack in one go. */
struct three_words {
    s32 a[3];
};

/* The method tables (src/data/entity_vtables_7e3bec.c). */
extern const struct vtable_slot gExtraLifeVtable[14];
extern const struct vtable_slot gWumpaVtable[14];
extern const struct vtable_slot gStopwatchVtable[13];

/* The x offset scale of each hop mode (1-3) of UpdateExtraLifeHop and
 * UpdateWumpaHop (src/data/object_tables_16bb6c.c). */
extern const s32 gExtraLifeHopWidths[3];
extern const s32 gWumpaHopWidths[3];

/* src/pickups/extra_life.c */
extern void CheckExtraLifePickup(struct orbit_part *self);
extern void PickUpExtraLife(struct orbit_part *self, u8 randomize);
extern void UpdateExtraLife(struct orbit_part *self);
extern struct orbit_part *CreateExtraLife(u16 id, u16 x, u16 y, s32 unused);
extern void SendExtraLifeToHud(struct orbit_part *self);
extern void UpdateExtraLifeHop(struct orbit_part *self);
extern void DrawExtraLife(struct orbit_part *self);
extern s32 GetExtraLifeClassId(void);
extern void DestroyExtraLife(struct orbit_part *self, u32 flags);
extern void ResetExtraLifePickup(struct orbit_part *self);
extern struct orbit_part *InitExtraLife(struct orbit_part *self);
extern s32 CollideExtraLife(struct orbit_part *self);
extern void SetExtraLifePos(struct orbit_part *self, s32 x, s32 y);
extern void SetExtraLifeHop(struct orbit_part *self, u8 mode);
extern void SetExtraLifeCounter(struct orbit_part *self, u8 val);
extern void CheckWumpaPickup(struct orbit_part *self);

/* src/pickups/wumpa_update.c */
extern void PickUpWumpa(struct orbit_part *self, u8 randomize);
extern void UpdateWumpa(struct orbit_part *self);
extern struct orbit_part *CreateWumpa(u16 id, u16 x, u16 y, u16 special);
extern void SendWumpaToHud(struct orbit_part *self);
extern void StartWumpaPayout(struct orbit_part *self);
extern void UpdateWumpaHop(struct orbit_part *self);

/* src/pickups/wumpa.c */
extern void DrawWumpa(struct orbit_part *self);
extern s32 GetWumpaClassId(void);
extern void DestroyWumpa(struct orbit_part *self, u32 flags);
extern void ResetWumpaPickup(struct orbit_part *self);
extern struct orbit_part *InitWumpa(struct orbit_part *self);
extern s32 CollideWumpa(struct orbit_part *self);
extern void SetWumpaPos(struct orbit_part *self, s32 x, s32 y);
extern void SetWumpaHop(struct orbit_part *self, s32 mode);
extern void SetWumpaCounter(struct orbit_part *self, u8 value);
extern void UpdateStopwatch(struct actor *self);
extern struct actor *CreateStopwatch(u16 id, u16 x, u16 y, u16 unused);
extern void ResetStopwatch(struct actor *self);
extern void DestroyStopwatch(struct actor *self, u32 flags);
extern struct actor *InitStopwatch(struct actor *self);

#endif /* GUARD_PICKUPS_H */

#ifndef GUARD_CONSTANTS_ROOMS_H
#define GUARD_CONSTANTS_ROOMS_H

/*
 * `struct level_room.kind`: how a room is played. PlayRoom attaches the
 * player controller of kinds 0-2; UpdateGameFrame hands a
 * ROOM_KIND_CATEGORY stage to InitActorCategory instead. Each room places
 * the start marker of its kind (docs/levels.md).
 */
#define ROOM_KIND_ON_FOOT 0    /* action controller (InitActionCtrl) */
#define ROOM_KIND_UNDERWATER 1 /* swim controller (InitPlayerCtrl) */
#define ROOM_KIND_HOVER 2      /* hover-vehicle controller (CreateInputCtrl); room 16 only */
#define ROOM_KIND_CATEGORY 3   /* no room data: a stage played in actor category `catIndex` */

#endif /* GUARD_CONSTANTS_ROOMS_H */

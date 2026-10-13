#ifndef GUARD_CONSTANTS_ROOMS_H
#define GUARD_CONSTANTS_ROOMS_H

/*
 * `struct level_room.kind`: how a room is played. PlayRoom attaches the
 * player controller of kinds 0-2; UpdateGameFrame hands a
 * ROOM_KIND_CATEGORY stage to ActorCategory::Init instead. Each room places
 * the start marker of its kind (docs/levels.md).
 */
#define ROOM_KIND_ON_FOOT 0    /* action controller (InitActionCtrl) */
#define ROOM_KIND_UNDERWATER 1 /* swim controller (InitSwimCtrl) */
#define ROOM_KIND_HOVER 2      /* hover-vehicle controller (CreateInputCtrl); room 16 only */
#define ROOM_KIND_CATEGORY 3   /* no room data: a stage played in actor category `catIndex` */

/*
 * How a room ended (#843, docs/flags/room_exit.md): what PlayRoom
 * (RunRoom) and ActorCategory::Init return to UpdateGameFrame's `status`.
 * Not the CATEGORY_EXIT_* values (categories.h), which are a category
 * frame's status inside ActorCategory::Init's loop.
 */
#define ROOM_EXIT_CLEARED 0 /* the room's exit reached (LevelProgress::RequestRoomExit), a category stage finished: the next room */
#define ROOM_EXIT_RETRY 1   /* the player is gone (died), or the pause menu's restart: back to the checkpoint */
#define ROOM_EXIT_QUIT 2    /* the pause menu's quit: back to the level select */

#endif /* GUARD_CONSTANTS_ROOMS_H */

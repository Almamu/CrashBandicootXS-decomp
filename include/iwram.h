#ifndef __IWRAM_H__
#define __IWRAM_H__

/* The ARM routines of the IWRAM image (src/iwram/sprite_arm.cpp,
 * src/iwram/string_arm.cpp), with the prototypes of their definitions
 * (docs/headers_plan.md). The Thumb code doesn't call them directly: it
 * goes through the hooks in src/iwram/iwram_data.cpp, declared with their
 * users (gfx.h, actor.h, vehicle.h). */

#include "core.h"

struct actor_self;

/* A zero-run-compressed OBJ frame (docs/data.md "Compressed sprite
 * frames"): the size in tiles, then u16 counts alternating between a run
 * of zero halfwords and a run of literal halfwords that follow it. */
struct rle_frame {
    u8 w;
    u8 h;
    u8 unk_2; // 0x30 in every frame; nothing reads it
    u16 data[0];
};

/* src/iwram/string_arm.cpp (UNUSED, see the file comment) */
extern s32 strlen_arm(u8 *s);
extern void strcpy_arm(u8 *dst, u8 *src);
extern void strncpy_arm(u8 *dst, u8 *src, s32 n);
extern void strcat_arm(u8 *dst, u8 *src);
extern s32 itoa_arm(s32 value, u8 *buf, s32 base);

/* src/iwram/sprite_arm.cpp */
extern void UnpackNibbleTiles(u16 *src, s32 lowBlock);
extern void DrawMirroredTilemap(u8 *pal, s32 lowBlock, s32 w, s32 h);
/* C++ sees the list as ActorSelf's (actor_self.hpp), as actor.h's
 * gHeapSortActorsByKeyFunc. */
#ifdef __cplusplus
extern void HeapSortActorsByKey(s32 n, class ActorSelf **list);
#else
extern void HeapSortActorsByKey(s32 n, struct actor_self **list);
#endif
extern void UnpackRleSpriteFrame(u16 *dst, struct rle_frame *frame);
extern s32 LookupSpriteFrameCache(u8 *frame);

#endif /* __IWRAM_H__ */

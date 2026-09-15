#ifndef __ACTOR_H__
#define __ACTOR_H__

/* A small, moving on-screen object: position, a handful of flag bits, a
 * width/height pair (both raw and pre-halved/negated for centering), and
 * a pointer to a per-category data table (offset/text record pairs read
 * at several different fixed offsets by src/graphics/graphics.c's
 * sub_8006FE4/sub_8007048/sub_8007114/sub_80070D4/sub_8007230/etc. and
 * by oam_count.c's sub_8006770 - none of that table's own shape is
 * understood yet, so it stays a raw `void *` here). Exactly 0x1c bytes -
 * confirmed by sub_80071E4's `sub_8026EDC(0x1c)` allocation. Several
 * fields (0x0A, 0x0B, 0x0D-0x0F, 0x16-0x17) are read/written but not
 * understood beyond their offset yet - named `unusedNN`/`fieldNN`
 * rather than guessed. `struct sub_8006700_actor.field_18` (in
 * oam_count.c) points at one of these. */
struct actor {
    s32 x;             // 0x00 - Q8 fixed-point screen position
    s32 y;              // 0x04 - Q8 fixed-point screen position
    u16 field_08;         // 0x08 - set from a constructor argument, role unclear
    u8 field_0A;            // 0x0A - passed through to sub_803AD88 by sub_8007048
    u8 unused_0B;             // 0x0B
    u8 flags;                  // 0x0C - individual bits tested/set by several functions
    u8 unused_0D[3];             // 0x0D-0x0F
    s16 halfW;                     // 0x10 - -rawW/2, set by sub_80070EC/sub_8007230
    s16 halfH;                       // 0x12 - -rawH/2, set by sub_80070EC/sub_8007230
    u8 rawW;                           // 0x14
    u8 rawH;                            // 0x15
    u8 unused_16[2];                      // 0x16-0x17
    void *table;                            // 0x18 - per-category data table, shape not yet known
};




COMPILE_TIME_ASSERT(sizeof(struct actor) == 0x1c);

#endif /* !__ACTOR_H__ */

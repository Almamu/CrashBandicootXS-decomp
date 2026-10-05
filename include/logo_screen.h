#ifndef GUARD_LOGO_SCREEN_H
#define GUARD_LOGO_SCREEN_H

/* The 0x44c-byte block ShowCompanyLogos (level_state.c) allocates for the
 * 20-slot object subsystem RunCompanyLogos drives
 * (src/frontend/title_screen.c / company_logos.c):
 * 20 `struct logo_piece` records, then a small header. Only the fields
 * matched code reads are named. */

/* One 0x34-byte slot of the 20-slot array `InitVvLogoPieces` seeds. */
struct logo_piece
{
    u8 active;          // 0x00
    u8 pad_01[3];
    s32 countdown;      // 0x04
    union {
        s32 q;          // 0x08 - Q16.16 x
        struct { u16 frac; s16 i; } h;
    } posA;
    union {
        s32 q;          // 0x0c - Q16.16 y
        struct { u16 frac; s16 i; } h;
    } posB;
    s32 posC;           // 0x10
    s32 velA;           // 0x14 - x scale
    s32 velB;           // 0x18 - y scale
    u8 pad_1c[0x18];
};

struct logo_screen
{
    struct logo_piece slots[20];  // 0x000
    u8 sfxPending[0x12];        // 0x410 - per-slot "play the cue once" flags
    u8 pad_422[2];
    u32 tilesA;                 // 0x424 - OBJ VRAM tile block (0x1200 bytes)
    u32 tilesB;                 // 0x428 - OBJ VRAM tile block (0x400 bytes)
    u32 tilesC;                 // 0x42C - OBJ VRAM tile block (0x1000 bytes)
    u8 *frames;                 // 0x430 - unpacked frame strip, 0xa00 bytes a frame
    u8 *scratch;                // 0x434 - 0x1000-byte frame build buffer
    s32 frame;                  // 0x438 - index into `frames` (0-9)
    s32 loops;                  // 0x43C - `frames` passes played, stops at 2
    s32 frameTick;              // 0x440 - ticks on the current frame (0-3)
    s32 fade;                   // 0x444 - fade/zoom counter, -1 when idle
    s32 timer;                  // 0x448 - -1 while the slots move, then
                                //         the outro countdown
};

COMPILE_TIME_ASSERT(sizeof(struct logo_piece) == 0x34);
COMPILE_TIME_ASSERT(sizeof(struct logo_screen) == 0x44c);

#endif // GUARD_LOGO_SCREEN_H

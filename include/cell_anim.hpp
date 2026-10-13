#ifndef GUARD_CELL_ANIM_HPP
#define GUARD_CELL_ANIM_HPP

/* The 3D stages' BG0 cell animation (src/actor/cell_anim.cpp, #915): the
 * category's cell record (a `struct cell_anim_header`, actor_anim.h)
 * played frame by frame as the course goes by. Its course distance is the
 * depth every actor is placed and drawn against.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The cell animation as an all-static class"): its variables are
 * separate IWRAM globals (0x03001394-0x030013BF, read by no other
 * object), and the ROM loads each one's own address, even for the two
 * adjacent bytes `page` and `uploaded`. Static data members compile like
 * those globals and static member functions like plain functions, so the
 * code is byte-identical to the C.
 *
 * The data members are only declared: cxx_symbols.txt (block "#915
 * CellAnim") maps each one to its C name (`_8CellAnim.cols` is
 * gCellAnimCols, ...), whose address is in sym_iwram.txt, and the
 * functions keep their C names the same way (`Advance__8CellAnim` is
 * AdvanceCellAnim).
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

class CellAnim
{
public:
    /* InitCellAnim: the category's cell record, from `checkpoint` on */
    static void Init(s32 type, void *cellAnim, u32 animSize, s32 checkpoint);
    static void ResetBg();           // ResetCellAnimBg
    static s32 GetFreeTile();        // GetCellAnimFreeTile
    static void FlipPage();          // FlipCellAnimPage
    static s32 GetDistance();        // GetCellAnimDistance
    static void Advance();           // AdvanceCellAnim
    static s32 GetFrameStep();       // GetCellAnimFrameStep
    static s32 GetSpeed();           // GetCellAnimSpeed
    static void SetSpeed(s32 value); // SetCellAnimSpeed

private:
    static void UploadFrame();                         // UploadCellAnimFrame
    static void FillTilemap(s32 screen, s32 w, s32 h); // FillCellAnimTilemap: UNUSED

    /* IWRAM (sym_iwram.txt) */
    static void *data;    // gCellAnim - the cell record (a struct cell_anim_header)
    static s32 cols;      // gCellAnimCols - the cells across
    static s32 rows;      // gCellAnimRows - the cells down
    static s32 tileBytes; // gCellAnimTileBytes - a frame's tiles, cols * rows * 0x20
    static s32 frameSize; // gCellAnimFrameSize - a frame's bytes (tiles, then the bank map)
    static s32 distance;  // gCellAnimDistance - the course distance: one per frame played
    static s32 length;    // gCellAnimLength - the animation's length (Q8 frames)
    static s32 time;      // gCellAnimTime - the current frame (Q8)
    static s32 speed;     // gCellAnimSpeed - frames per Advance (Q8)
    static u8 hasBanks;   // gCellAnimHasBanks - the frames carry a bank map (type 0)
    static u8 page;       // gCellAnimPage - the BG0 tile half being written
    static u8 uploaded;   // gCellAnimUploaded - a frame was uploaded since FlipPage
    static s32 frameStep; // gCellAnimFrameStep - the whole frames of the last Advance
};

#endif // GUARD_CELL_ANIM_HPP

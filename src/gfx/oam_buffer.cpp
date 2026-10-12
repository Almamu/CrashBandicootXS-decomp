/* The OAM shadow buffer (OamBuffer, gOamBuffer; include/sprite_obj.hpp).
 * Split from gfx/graphics.cpp (#767), same flags (old_agbcc). */

#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "globals.h"
}

/* Writes `n` affine matrices' scales: each matrix's pa/pd from the next
 * two of `scales`, its pb/pc 0. */
void OamBuffer::SetAffineScales(u16 *scales, s32 n)
{
    s32 i;
    u16 zero;

    if (n <= 0)
        return;
    zero = 0;
    i = 0;
    do {
        table[i++].attr[3] = scales[0];
        table[i++].attr[3] = zero;
        table[i++].attr[3] = zero;
        table[i++].attr[3] = scales[1];
        scales += 2;
        n--;
    } while (n != 0);
}

/* Appends `n` entries (DMA). */
void OamBuffer::Append(void *entries, s32 n)
{
    if (n == 0)
        return;
    DmaCopy32(3, entries, &table[count], n * sizeof(table[0]));
    count += n;
}

/* Hides the entries from `count` on (the OBJ disable bit of attr0). */
void OamBuffer::HideUnused()
{
    s32 i = count;

    if (i > OAM_ENTRY_COUNT - 1)
        return;
    do {
        table[i].attr0.affineMode = OAM_AFFINE_MODE_HIDDEN;
        i++;
    } while (i <= OAM_ENTRY_COUNT - 1);
}

void OamBuffer::Rewind()
{
    count = base;
    matrixCount = 0;
}

void OamBuffer::MarkBase()
{
    base = count;
    matrixCount = 0;
}

void OamBuffer::Reset()
{
    count = 0;
    matrixCount = 0;
    MarkBase();
    Rewind();
}

/* Copies the shadow table to OAM (DMA). */
void OamBuffer::Commit()
{
    DmaCopy32(3, table, OAM, sizeof(table));
}

/* Adds one entry, keeping the affine parameter (attr[3]) of the slot it
 * goes in: that belongs to a matrix. */
void OamBuffer::Add(const void *entry)
{
    s32 n = count;

    if (n > OAM_ENTRY_COUNT - 1)
        return;
    /* `saved` is needed: inlined, the instructions are reordered */
    u16 saved = table[n].attr[3];

    table[n] = *(const union oam_shadow_entry *)entry;
    table[count++].attr[3] = saved;
}

OamBuffer::~OamBuffer()
{
}

OamBuffer::OamBuffer()
{
    Reset();
}

#ifndef GUARD_ENTITY_BITS_H
#define GUARD_ENTITY_BITS_H

#include "match.h"
#include "level.h"
#include "globals.h"

/* The entity "gone" bitmap helpers (#667): MarkEntityGone's sequence
 * (graphics.c), which many objects inline instead of calling it. An
 * entity that is collected, broken or killed sets its own `gone` flag
 * and, unless its id is ENTITY_ID_NONE, bit `id` of
 * `gEntityFlags->bits0Copy` (struct entity_flags, level.h).
 * entity_flags.c has the out-of-line accessors for the other bitmaps.
 *
 * Like the rest of the project's helpers, each macro expands to exactly
 * the code the matched copies spelled out, statement for statement, so
 * a site converted to one compiles to the same bytes (#667 checked every
 * converted object against the old build):
 *
 * - ENTITY_SET_GONE_BIT's do/while(0) is load-bearing. gcc 2.9 puts loop
 *   notes around it, and they stop CSE from reusing an id already loaded
 *   for the caller's ENTITY_ID_NONE test, which gives the ROM's reload of
 *   the id. crate_break.c's BreakCrate spells the same body out in a
 *   plain `{ }` block, which compiles differently there, so don't rewrite
 *   the wrapper.
 * - The word index is a signed division (`_id` is an s32), so the ROM's
 *   copy, `asr #5` and subtract come out; `_id - _word * 32` is the bit.
 * - The address is formed in two steps (the bitmap, then the word's byte
 *   offset), which is what the matched copies did.
 *
 * Copies that are still spelled out, each with a comment saying why:
 * graphics.c's MarkEntityGone itself, wumpa.c's UpdateStopwatch and
 * crate_break.c (other asm/pins or wrapper), and enemy_ctrl_update.cpp's
 * MarkGoneFreshBit (a MATCH_CONST inside the sequence). */

/* An entity id that has no bit in the bitmaps (actor.h's `id`). */
#define ENTITY_ID_NONE 0xFFFF

/* Sets bit `idExpr` of gEntityFlags->bits0Copy to `bit` (the matched
 * copies pass 1, or a local already holding 1 so that gcc shares the
 * register). */
#define ENTITY_SET_GONE_BIT_OF(idExpr, bit)                                    \
    do {                                                                       \
        s32 _id = (idExpr);                                                    \
        struct entity_flags *_base = gEntityFlags;                             \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = _base->bits0Copy;                                         \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= (bit) << (_id - _word * 32);                                 \
    } while (0)

/* Sets bit `idExpr` of gEntityFlags->bits0Copy. */
#define ENTITY_SET_GONE_BIT(idExpr) ENTITY_SET_GONE_BIT_OF(idExpr, 1)

/* MarkEntityGone inlined: sets the object's `gone` flag (an lvalue: a
 * bitfield, or the flags byte's bit 0 through a view) and, unless `id`
 * is ENTITY_ID_NONE, its bit in the bitmap. `id` is read twice, once for
 * the test and once in ENTITY_SET_GONE_BIT, as in the ROM. */
#define ENTITY_MARK_GONE(gone, id)                                             \
    {                                                                          \
        (gone) = 1;                                                            \
        if ((id) != ENTITY_ID_NONE)                                            \
            ENTITY_SET_GONE_BIT(id);                                           \
    }

/* The register-pinned copy (cortex.c's three, platform.c's): unless
 * `t->id` is ENTITY_ID_NONE, sets its bit in the bitmap. The ROM tests
 * the id in a register of its own (R_CUR) against an r0 0xFFFF, re-reads
 * it (`volatile`) into r3 after the test, and divides that zero-extended
 * value *signed* (r0: copy, `asr #5`, subtract); the bitmap base is in
 * R_BASE. R_CUR and R_BASE are bare register names (r2), as for
 * MATCH_HOLD_REG: the pins differ per site and are load-bearing
 * (docs/matching_techniques.md, "Register pins"). */
#define ENTITY_SET_GONE_BIT_PINNED(t, R_CUR, R_BASE)                           \
    ENTITY_SET_GONE_BIT_PINNED_SLOT(t, R_CUR, R_BASE, ENTITY_GONE_SLOT)

/* The statement that points `slot` at the bitmap, for
 * ENTITY_SET_GONE_BIT_PINNED_SLOT: a site that needs the 0x108 offset
 * in a register of its own passes its own (cortex.c's GONE_SLOT_R4). */
#define ENTITY_GONE_SLOT(slot, base) slot = (u32 *)((base) + 0x108)

#define ENTITY_SET_GONE_BIT_PINNED_SLOT(t, R_CUR, R_BASE, SLOT_STMT)           \
    do {                                                                       \
        {                                                                      \
            MATCH_HOLD_REG(s32, _none, r0) = ENTITY_ID_NONE;                   \
            MATCH_HOLD_REG(u32, _cur, R_CUR) = (t)->id;                        \
                                                                               \
            if (_cur != _none) {                                               \
                MATCH_HOLD_REG(s32, _id, r3) = *(vu16 *)&(t)->id;              \
                MATCH_HOLD_REG(u8 *, _base, R_BASE) = (u8 *)gEntityFlags;      \
                MATCH_HOLD_REG(s32, _word, r0) = _id;                          \
                s32 _off;                                                      \
                u32 *_slot;                                                    \
                                                                               \
                _word /= 32;                                                   \
                _off = _word * 4;                                              \
                SLOT_STMT(_slot, _base);                                       \
                _slot = (u32 *)((u8 *)_slot + _off);                           \
                _word = _id - _word * 32;                                      \
                *_slot |= 1 << _word;                                          \
            }                                                                  \
        }                                                                      \
    } while (0)

#endif // GUARD_ENTITY_BITS_H

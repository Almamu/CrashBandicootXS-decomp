#include "core.h"
#include "actor.h"

/* GitHub issue #9/#10: 0x0800B3F0 - a part-object constructor on the
 * same big, still-unnamed "part" object (at least 0x108 bytes)
 * documented in `actor_part15.c`'s file header - raw offset casts are
 * used throughout for the same reason that file gives: most individual
 * fields' meaning isn't confirmed beyond "a byte/word at this offset". */

extern struct actor *InitGroundSprite(struct actor *self);
extern void ResetCollisionQueue(void *arg0);
extern struct actor *CreateSpriteObj(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void ResetSpriteFrameTimer(void *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteAnimDone(void *part, u8 val);
extern void ResetPlayer(void *selfArg);
extern u8 gPlayerVtable[];
extern void ***gSpriteBankSet;

/* Re-initializes `self` (via `InitGroundSprite`, already matched in
 * `actor_part14.c`), then overwrites its table with
 * `gPlayerVtable` and clears its trailing `+0x108`/`+0x10c`
 * fields via `ResetCollisionQueue` (still raw - a two-field, 4-byte-plus-byte
 * clear). Allocates a fresh `struct actor`-shaped child object
 * (`CreateSpriteObj(0, 0, 0, 0)`, the same allocator `actor_part6.c`'s
 * `CreateSpriteObj` is - called here with an extra, unused 4th zero
 * argument, the same calling convention already used by
 * `graphics_loading_21d80.c`'s own callers of it) and hooks it up at
 * `self+0xb0`: points its own `+0x20` table-entry pointer at the
 * `gSpriteBankSet` shared table's `(0xcc << 1)` slot (the same
 * idiom `graphics_loading_21d80.c` uses throughout), clears its
 * `+0x2d` byte, and builds it via the standard `ResetSpriteFrameTimer`/
 * `ResetSpriteFrameIndex`/`SetSpriteAnimDone` OAM trio. Clears `self+0xb4`, then
 * calls `ResetPlayer` (matched in `actor_part48.c`) to finish resetting
 * `self`'s velocity/state fields and hook the `self+0xb0` child up via
 * its own `GetSpriteAnimPaletteSlot` call. Finally sets `self+8`'s `field_08` and
 * the Q8 `x`/`y` position from the three `u16` arguments - the same
 * tail `CreateGroundSprite` (`actor_part14.c`) uses for its own, smaller
 * `struct actor` - and returns `self`. */
void *InitPlayer(void *selfArg, u16 arg1, u16 arg2, u16 arg3)
{
    u8 *self = selfArg;
    struct actor *child;

    InitGroundSprite((struct actor *)self);
    *(u8 **)(self + 0x18) = gPlayerVtable;
    ResetCollisionQueue(self + 0x108);

    child = CreateSpriteObj(0, 0, 0, 0);
    *(struct actor **)(self + 0xb0) = child;
    *(void **)((u8 *)child + 0x20) = (u8 *)(**gSpriteBankSet) + (0xcc << 1);

    /* Register-pinned: the ROM keeps this `0` constant alive in `sl`
     * across all three `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` calls
     * (a callee-saved register survives a `bl`) so it can reuse the
     * same value for `self+0xb4` afterward, instead of reloading a
     * fresh `0` there - a plain `0` literal for both stores lets this
     * compiler fold each into its own cheap `movs`/immediate instead,
     * needing only 2 (not 3) high registers preserved across the whole
     * function. `SetSpriteAnimDone`'s own `0` argument is a separate, fresh
     * literal in the ROM too (`movs r1, #0`), not sourced from `sl`. */
    {
        register s32 zero asm("sl") = 0;

        *((u8 *)child + 0x2d) = zero;
        ResetSpriteFrameTimer(child);
        ResetSpriteFrameIndex(child);
        SetSpriteAnimDone(child, 0);

        *(u32 *)(self + 0xb4) = zero;
    }
    ResetPlayer(self);

    *(u16 *)(self + 8) = arg1;
    *(s32 *)self = (s32)arg2 << 8;
    *(s32 *)(self + 4) = (s32)arg3 << 8;

    return self;
}

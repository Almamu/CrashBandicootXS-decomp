#include "core.h"

/* GitHub issue #22, ROM 0x08017A44-0x08017AAC. `self` is the same
 * large per-level "player/action" object documented in
 * actor_part18.c's top-of-file comment - not the small (0x1c-byte)
 * `struct actor` from include/actor.h. `self+0xc` is the per-category
 * table pointer that convention documents; `self+0x10` is the "part"
 * sub-object pointer. `self+0x14`/`self+0x18` are each a packed 4-byte
 * field written both as a whole word (`sub_8017A70`) and as individual
 * bytes elsewhere in this group - the exact sub-byte meanings aren't
 * pinned down yet, so every access here stays a raw offset rather than
 * a guessed struct, matching the rest of this object family. */

extern u8 gStaticData_087E435C[];
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);

/* `self+0x17` byte getter. */
u8 sub_8017A44(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x17];
}

/* Sets the `self+0x18`-word's byte0/byte2 flags and the `self+0x14`-
 * word's byte1 to `val`. */
void sub_8017A48(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x1a] = 1;
    self[0x18] = 1;
    self[0x15] = val;
}

/* Sets the `self+0x18`-word's byte1 flag, the `self+0x14`-word's
 * byte3 flag, and the `self+0x14`-word's byte0 to `val`. */
void sub_8017A54(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x19] = 1;
    self[0x17] = 1;
    self[0x14] = val;
}

/* Single-flag version of `sub_8017A48`: sets `self+0x18`'s byte0 flag
 * and `self+0x14`'s byte1 to `val`. */
void sub_8017A60(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x18] = 1;
    self[0x15] = val;
}

/* Single-flag version of `sub_8017A54`: sets `self+0x14`'s byte3 flag
 * and `self+0x14`'s byte0 to `val`. */
void sub_8017A68(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x17] = 1;
    self[0x14] = val;
}

/* Raw whole-word setter for the two packed fields `sub_8017A48`/
 * `sub_8017A54`/`sub_8017A60`/`sub_8017A68` otherwise update one byte
 * at a time - `arg1` is taken but unused (register-only pass-through,
 * confirmed unread anywhere in the ROM body). */
void sub_8017A70(void *selfArg, s32 arg1, s32 a, s32 b)
{
    u8 *self = selfArg;

    (void)arg1;
    *(s32 *)(self + 0x14) = a;
    *(s32 *)(self + 0x18) = b;
}

/* Sets `self+0xc`'s table pointer to `gStaticData_087E435C`, then
 * tail-calls `sub_800B8A8(self, flags)` - which promptly resets it
 * back to `gStaticData_087E3E7C` (see actor_part17.c) and, if
 * `flags` bit 0 is set, fires `sub_8026ED0`. */
void sub_8017A78(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E435C;
    sub_800B8A8(self, flags);
}

/* Resets via `sub_800B8C8` (table pointer to `gStaticData_087E3E7C`,
 * `self+8` cleared), then re-points the table at `gStaticData_087E435C`
 * and zeroes `self+0x10`/`self+0x14`/`self+0x18`. Returns `self`. */
void *sub_8017A8C(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E435C;
    *(s32 *)(self + 0x10) = 0;
    *(s32 *)(self + 0x14) = 0;
    *(s32 *)(self + 0x18) = 0;
    return self;
}

/* `self+0x10` pointer getter - the "part" sub-object. */
void *sub_8017AAC(void *selfArg)
{
    return *(void **)((u8 *)selfArg + 0x10);
}

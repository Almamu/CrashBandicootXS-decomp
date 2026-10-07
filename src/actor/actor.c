#include "core.h"
#include "math_util.h"
#include "match.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"

/* The `InitActorPart`/`gActorList`-rooted "self" object family
 * already documented in ctrl.cpp/action_ctrl_states.cpp/polar_player_actions.c/
 * hovercraft_parts.c/hovercraft_cannon.c: a "part table" pointer at `self+0`
 * (copied from the constructor's `part` argument's own `+4` field), a
 * table-index/"kind" field at `self+0xc`, an anim-frame halfword/byte
 * pair at `self+0x10`/`self+0x12`, an accumulator at `self+8`, state at
 * `self+0x28`, a frame counter at `self+0x44`, a `+0x50`-rooted event/
 * trampoline table fed through `_call_via_r2`, and the `+0x48`(prev)/
 * `+0x4c`(next) circular doubly-linked list rooted at the player-pointer
 * global `gActorList`. This file additionally pins down
 * `InitActorPart` itself (the constructor every other actor file
 * already forward-declares and calls) plus a handful of new fields it
 * introduces: the constructor's raw `part`/`b`/`c`/`d` arguments cached
 * at `self+0x30`/`self+0x1c`/`self+0x20`/`self+0x24`, a "movement"
 * threshold pair at `self+0x14`/`self+0x34` (an absolute-value/packed-
 * bitfield distance metric compared against `gActorNearClipDepth`/
 * `gActorFarClipDepth`, gating whether the object fires its `+0x50`
 * table's slot-3 trampoline instead of animating), a one-shot byte flag
 * at `self+0x2c`, and a 12-byte little vector block at `self+0x38`
 * (copied from `part+0x14..0x20`) whose first three `s16` slots are a
 * position `DrawActor`'s sibling `GetActorWorldBox` offsets by
 * a per-axis velocity into. The object is `struct actor_self`
 * (actor_self.h: `x`/`y`/`z` are the cached `b`/`c`/`d`, `depth` and
 * `sortKey` the threshold pair) and the constructor's `part` a `struct
 * anim_table_record` (actor_anim.h). See
 * docs/matching/archive/issue-50-actor-2a69c.md. */

/* Trivial forwarder - ignores its own argument and calls
 * `AllocJetpackPlayerTiles(gActorList)` (the player object), discarding its
 * return value. Same shape as `GivePolarPlayerLife` in polar_player_actions.c. */
void JetpackReloadPlayerTiles(void *arg0)
{
    AllocJetpackPlayerTiles(gActorList);
}

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `AllocPolarPlayerTiles` instead. */
void PolarReloadPlayerTiles(void *arg0)
{
    AllocPolarPlayerTiles(gActorList);
}

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `FinishJetpackRun` instead. */
void JetpackReachCourseEnd(void *arg0)
{
    FinishJetpackRun(gActorList);
}

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `FinishPolarRun` (already
 * matched as a no-argument function in polar_player_actions.c) with the player
 * pointer anyway - the callee simply ignores it. */
void PolarReachCourseEnd(void *arg0)
{
    FinishPolarRun(gActorList);
}

extern s32 _call_via_r1(void *arg0, void *fn);

/* Passes its own `self` argument through to `_call_via_r1`, alongside
 * slot 9 (`+0x24`) of the selected category's vtable. */
s32 IsTouchingPlayer(void *self)
{
    return _call_via_r1(self, gActorCategoryVtable->fn[9]);
}

/* The constructor every other actor file already forward-
 * declares: seeds `self`'s part-table pointer (`+0`/`+4`, copied from
 * `part+4`/`part+8`) and header byte (`+0x18`, from `part+0xc`), resets
 * it via `SetActorAnim`, marks it "dead" (`+0x50 = gActorVtable`)
 * until a real event table is assigned later, caches the constructor's
 * own `part`/`b`/`c`/`d` arguments (`+0x30`/`+0x1c`/`+0x20`/`+0x24`),
 * copies a 12-byte vector block from `part+0x14..0x20` to `self+0x38`,
 * resets state/counter (`+0x28`/`+0x44 = 0`) and the one-shot flag
 * (`+0x2c = 1`), computes the movement-threshold pair (`+0x34`/`+0x14`,
 * see this file's header comment), and links `self` into the circular
 * `+0x48`/`+0x4c` list rooted at the player pointer `gActorList`
 * (or self-links it if that list is still empty). Returns `self`. */
void *InitActorPart(void *selfArg, void *partArg, s32 b, s32 c, s32 d)
{
    struct actor_self *self = selfArg;
    MATCH_HOLD_REG(struct anim_table_record *, part, r4) = partArg;
    MATCH_HOLD_REG(s32, bReg, r5) = b;
    MATCH_HOLD_REG(s32, cReg, r6) = c;

    {
        u8 vC = part->palette;
        struct anim_frame_record *v4 = part->table_A;
        u32 *v8 = part->table_B;

        self->anims = v4;
        self->frameOffsets = v8;
        self->palette = vC;
    }

    SetActorAnim(self, 0);

    self->vtable = (struct actor_vtable *)gActorVtable;
    self->x = bReg;
    self->y = cReg;
    self->z = d;
    ACTOR_RECORD(self) = part;
    *(struct anim_box *)self->box = part->box_14;

    self->state = 0;
    self->stateTime = 0;
    self->visible = 1;

    {
        MATCH_HOLD_REG(s32, value, r2) = self->z - INT_TO_Q8(GetCellAnimDistance());
        s32 sign;

        MAKE_ABS_BRANCHLESS(value, sign);
        self->depth = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = self->y;
            s32 cSign;
            s32 b, bSign;

            MAKE_ABS_BRANCHLESS(c, cSign);
            b = self->x;
            MAKE_ABS_BRANCHLESS(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        self->sortKey = value;

        if (self->depth > GetActorBgLayerDepth()) {
            self->sortKey |= SORT_KEY_FLAG_BEHIND_BG;
        }
    }

    {
        struct actor_self *head = gActorList;

        if (head != NULL) {
            ACTOR_LINK_NEXT(self) = head;
            ACTOR_LINK_PREV(self) = ACTOR_LINK_PREV(head);
            ACTOR_LINK_PREV(head) = self;
            ACTOR_LINK_NEXT(ACTOR_LINK_PREV(self)) = self;
        } else {
            ACTOR_LINK_NEXT(self) = self;
            ACTOR_LINK_PREV(self) = self;
        }
    }

    return self;
}

extern s32 _call_via_r2(void *arg0, s32 arg1, void *fn);

/* Recomputes `self`'s movement-threshold pair (`+0x34`/`+0x14`, same
 * formula as `InitActorPart`, using `self`'s own already-stored `+0x24`
 * in place of the constructor's `d` argument). If the result falls
 * outside `[gActorNearClipDepth-0x200, gActorFarClipDepth+0x200]`, fires
 * the `+0x50` event table's slot-3 trampoline instead of animating;
 * otherwise advances the frame counter (`+0x44`), applies the current
 * anim-frame delta (`+0x10`) to the position accumulator (`+8`), and -
 * once the animation's base offset reaches the current keyframe's `+4`
 * threshold - rewinds `+8` by the keyframe's `+4`/`+6` delta and marks
 * `+0x12` "done". */
void UpdateActor(void *selfArg)
{
    struct actor_self *self = selfArg;

    {
        MATCH_HOLD_REG(s32, value, r2) = self->z - INT_TO_Q8(GetCellAnimDistance());
        s32 sign;

        MAKE_ABS_BRANCHLESS(value, sign);
        self->depth = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = self->y;
            s32 cSign;
            s32 b, bSign;

            MAKE_ABS_BRANCHLESS(c, cSign);
            b = self->x;
            MAKE_ABS_BRANCHLESS(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        self->sortKey = value;

        if (self->depth > GetActorBgLayerDepth()) {
            self->sortKey |= SORT_KEY_FLAG_BEHIND_BG;
        }
    }

    if (self->depth > gActorFarClipDepth + 0x200 || self->depth < gActorNearClipDepth - 0x200) {
        if (self != NULL) {
            struct actor_vtable *table = self->vtable;
            s32 offset = table->destroy.thisOffset;
            u8 *addr = (u8 *)self + offset;
            void *fn = table->destroy.fn;

            _call_via_r2(addr, 3, fn);
        }
        return;
    }

    self->stateTime += 1;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        s32 idx = self->animIndex;
        u8 *table = (u8 *)self->anims;
        s32 recordAddr = idx * 0xc;
        MATCH_HOLD_REG(struct anim_frame_record *, record, r1);

        recordAddr += (s32)table;
        record = (struct anim_frame_record *)recordAddr;

        {
            s32 v4 = record->loopThreshold;

            if (frame >= v4) {
                s32 v6 = record->loopBase;

                self->animTime -= INT_TO_Q8(v4 - v6);
                self->animDone = 1;
            }
        }
    }
}

/* Same "self" object family as above - see this file's header
 * comment and docs/matching/archive/issue-50-actor-2a69c.md. */

/* Computes an OBJ scale factor from `self->depth` and its animation
 * record's `baseDepth` (via `__divsi3`), then a second
 * scale from `gActorFocalLength` (via the same helper) used to project
 * `self`'s x/y position through `GetActorBgCenterY`/`GetActorBgCenterX`'s
 * screen-space offsets into on-screen X/Y. Fetches the current anim
 * frame (`GetAnimFrameData`), centers it (frame's own width/height
 * bytes, doubled if the first scale factor exceeds `0xff`), culls if
 * fully off-screen, and - if visible - builds the OAM attribute word
 * (position, `GetAnimFrameAttr`'s flag byte, an oversize-scale bit, and a
 * priority/palette nibble from `self->palette`/`self->sortKey`) and calls
 * `SetupSpriteFrameOam` with the first scale factor as its OBJ-affine
 * "priority" argument. See docs/matching/archive/issue-50-actor-2a69c.md for
 * the two register-pinning gaps this needed to close (the `frame[1]`
 * read reusing GetAnimFrameData's still-live `r0` return instead of the
 * `r7` copy used for `frame[0]`, and the `flag` spill-across-call
 * around `GetAnimFrameAttr` needing to be written out explicitly since it's
 * pinned to a caller-saved register). */
void DrawActor(void *selfArg)
{
    MATCH_HOLD_REG(struct actor_self *, self, r6) = selfArg;
    MATCH_HOLD_REG(s32, scale, r8);
    s32 dist = self->depth;
    MATCH_HOLD_REG(s32, scaleY, r5);
    s32 posX;
    s32 posY;
    /* The ROM keeps GetAnimFrameData's raw return value alive in r0
     * (unclobbered by the flag computation below) and reads frame[1]
     * through it directly, while frame[0] is read twice through r7 - an
     * explicit copy (`adds r7, r0, #0`) made right after the call. A
     * single plain `frame` local always collapses back to one canonical
     * register for every access, so this pins the call result to r0 and
     * makes the r7 copy an explicit second variable, matching the ROM's
     * own register choice for each of the three reads instead of gcc's
     * single-register default. */
    MATCH_HOLD_REG(u8 *, frame, r0);
    u8 *frameCopy;
    MATCH_HOLD_REG(u32, flag, r2);
    MATCH_HOLD_REG(s32, delta0, r1);
    s32 delta1;

    scale = __divsi3(dist << 8, ACTOR_RECORD(self)->baseDepth);
    scaleY = __divsi3(gActorFocalLength << 0xc, dist);

    {
        s32 off = GetActorBgCenterY();
        MATCH_HOLD_REG(s32, tmp, r1) = self->y;

        posX = tmp * scaleY;
        posX >>= 0xc;
        posX += off;
        posX = Q8_TO_INT(posX);
    }

    {
        s32 off = GetActorBgCenterX();
        MATCH_HOLD_REG(s32, tmp, r1) = self->x;

        tmp = tmp * scaleY;
        tmp >>= 0xc;
        tmp += off;
        posY = Q8_TO_INT(tmp);
    }

    frame = GetAnimFrameData(self);
    frameCopy = frame;

    flag = 0;
    {
        MATCH_HOLD_REG(s32, scaleCmp, r1);

        asm("mov %0, r8" : "=r"(scaleCmp));
        if (scaleCmp <= 0xff) {
            flag = 0x200;
        }
    }

    if (flag != 0) {
        MATCH_HOLD_REG(u8, b, r3) = frameCopy[0];
        delta0 = b << 3;
    } else {
        MATCH_HOLD_REG(u8, b, r3) = frameCopy[0];
        delta0 = b << 2;
    }

    if (flag != 0) {
        delta1 = frame[1] << 3;
    } else {
        delta1 = frame[1] << 2;
    }

    posY -= delta0;
    posX -= delta1;

    if (posX > 0x9f)
        return;
    if (posX + delta1 * 2 < 0)
        return;
    if (posY > 0xef)
        return;
    {
        MATCH_HOLD_REG(s32, shifted, r0) = delta0 << 1;

        if (posY + shifted < 0)
            return;
    }

    if (scale != 0x100) {
        flag |= 0x100;
    }

    {
        /* `flag` is pinned to r2 (matching the ROM's own choice, needed
         * to reproduce the r7/r0 split above), but a plain
         * `MATCH_HOLD_REG(..., r2)` variable is the caller's own
         * responsibility across a call - gcc, unlike with an ordinary
         * pseudo-register it owns, won't insert protective spill code
         * for it automatically the way it does for a normal local stuck
         * in a caller-saved register. The ROM's real compiler still
         * needed r2 preserved here (no free callee-saved register left -
         * r4-r8 are already dist/scaleY/self/frame/scale) and spilled it
         * to a dedicated stack word around this one call; reproduce that
         * explicitly (same technique as DivMod's r2-across-SWI
         * save/restore, see docs/matching.md) rather than relying on the
         * compiler to notice on its own. */
        u32 flagStack[1];
        s32 attr;
        MATCH_HOLD_REG(void *, callArg, r0) = self;

        asm volatile("str %1, %0" : "=m"(flagStack[0]) : "r"(flag));
        attr = GetAnimFrameAttr(callArg);

        {
            u32 packed;
            u32 v;
            u32 pre;
            MATCH_HOLD_REG(u32, shifted, r0);
            u32 attr2;

            packed = posX & 0xff;
            packed |= ((u32)posY & 0x1ff) << 16;
            packed |= attr;
            /* Reload right before its one remaining use, matching the
             * ROM's own late `ldr r2, [sp]` placement (immediately
             * before the final `orrs r3, r2`) rather than eagerly right
             * after the call. */
            asm volatile("ldr %0, %1" : "=r"(flag) : "m"(flagStack[0]));
            packed |= flag;

            v = self->palette;
            pre = v << 0xc;

            if (self->sortKey & SORT_KEY_FLAG_BEHIND_BG) {
                shifted = (pre | 0x800) << 0x10;
            } else {
                shifted = v << 0x1c;
            }
            attr2 = shifted >> 0x10;

            SetupSpriteFrameOam(frameCopy, packed, attr2, scale);
        }
    }
}

/* Same "self" object family as above - see this file's header
 * comment and docs/matching/archive/issue-50-actor-2a69c.md. (This was a
 * separate file while `DrawActor`, above, was still raw.) */

/* Same movement-threshold computation as `InitActorPart`/`UpdateActor`
 * (see this file's header comment), but with no trampoline-fire/frame-
 * update tail - just refreshes `depth`/`sortKey`. */
void UpdateActorDepth(struct actor_self *self)
{
    MATCH_HOLD_REG(s32, value, r2) = self->z - INT_TO_Q8(GetCellAnimDistance());
    s32 sign;

    MAKE_ABS_BRANCHLESS(value, sign);
    self->depth = value;
    value = (value >> 1) & 0x7f80;

    {
        s32 c = self->y;
        s32 cSign;
        s32 b, bSign;

        MAKE_ABS_BRANCHLESS(c, cSign);
        b = self->x;
        MAKE_ABS_BRANCHLESS(b, bSign);
        c = c + b;
        c >>= 0xb;
        c &= 0x7f;
        value |= c;
    }

    self->sortKey = value;

    if (self->depth > GetActorBgLayerDepth()) {
        self->sortKey |= SORT_KEY_FLAG_BEHIND_BG;
    }
}

/* Trivial getter: the `index` of `self`'s animation record (`+0x30`,
 * InitActorPart's `part`), read as a byte. */
u8 GetActorRecordIndex(struct actor_self *self)
{
    return *(u8 *)&ACTOR_RECORD(self)->index;
}

/* State/table-index/anim-frame reset, the same idiom already documented
 * for the boss cluster's `DamageAirshipFireball`/`AirshipStateFall` (see
 * docs/matching/archive/issue-58-0x08030334-actor.md): sets `self+0x28`/
 * `self+0xc` from its own arguments, resets the frame counter
 * (`+0x44`)/accumulator (`+8`), and seeds the anim-frame halfword/byte
 * pair (`+0x10`/`+0x12`) from `self`'s part-table's `kind`th record. The
 * `*(T *)&self->...` stores keep gcc from treating them as struct-member
 * accesses, which changes where the byte zero is built. */
void SetActorState(struct actor_self *self, s32 a, s32 kind)
{
    MATCH_HOLD_REG(s32, zero, r4);

    self->state = a;
    zero = 0;
    self->stateTime = zero;
    self->animIndex = kind;
    {
        MATCH_HOLD_REG(struct anim_frame_record *, table, r3) = self->anims;
        MATCH_HOLD_REG(u16, anim, r1) = table[kind].duration;
        MATCH_HOLD_REG(u8, zero2, r2) = 0;

        *(u16 *)&self->animTimer = anim;
        *(u8 *)&self->animDone = zero2;
    }
    self->animTime = zero;
}

/* Trivial getter: `self->z` (the constructor's `d` argument). */
s32 GetActorZ(struct actor_self *self)
{
    return self->z;
}

/* Trivial getter: `self->y` (the constructor's `c` argument). */
s32 GetActorY(struct actor_self *self)
{
    return self->y;
}

/* Trivial getter: `self->x` (the constructor's `b` argument). */
s32 GetActorX(struct actor_self *self)
{
    return self->x;
}

/* Same "self" object family as above - see this file's header
 * comment and
 * docs/matching/archive/issue-50-actor-2a69c.md. */

/* The actor's collision box in world space: copies `box` (`self+0x38`,
 * the record's box InitActorPart copied) into `*out`, adding the actor's
 * position (`x`/`y`/`z`, `self+0x1c`/`0x20`/`0x24`, each `>>8`'d to whole
 * units) to its first three `s16` slots (the corner); the size slots pass
 * through unchanged. UNUSED - no caller anywhere in the ROM (no `bl` in
 * src/, no Thumb pointer to it in baserom.gba). See docs/matching/archive/issue-50-actor-2a69c.md for the small
 * inline-asm islands this needed (the compiler's own list scheduler
 * reorders the three per-axis load/shift pairs and the RMW halfword
 * updates' register reuse differently from the ROM's literal order no
 * matter how the surrounding C is phrased). Declared returning `void *`
 * (not `void`) because the ROM's own epilogue physically returns
 * `outArg` unchanged: `r0` (the parameter) is never touched again after
 * being copied to `r2` for the final block copy, and the ROM's real
 * compiler used that still-live value to pick `r1` (not `r0`) for its
 * "pop a register, branch to it" epilogue step - the same
 * return-type-shapes-epilogue-register-choice gotcha already documented
 * for `GetCompletionPercent`/`DrawWrappedTextInBox` in docs/matching.md. No caller of
 * this function has been matched yet to say whether the return value is
 * actually used. */
void *GetActorWorldBox(void *outArg, void *selfArg)
{
    struct blob0xc {
        u32 w0, w1, w2;
    };

    struct actor_self *self = selfArg;
    struct blob0xc buf = *(struct blob0xc *)self->box;
    MATCH_HOLD_REG(s32, d0, r3);
    MATCH_HOLD_REG(s32, d1, r5);
    MATCH_HOLD_REG(s32, d2, r4);

    /* The ROM computes all three per-axis deltas up front (a strict
     * load/shift, load/shift, load/shift run) before touching any of
     * `buf`'s halfwords; this compiler's own instruction scheduler
     * always interleaves the three loads/shifts differently (hoisting
     * loads ahead of unrelated shifts to hide latency) regardless of
     * how the source groups or pins them - confirmed with several
     * different phrasings. A small inline-asm island, matching the
     * ROM's literal instruction order, sidesteps the scheduler for just
     * this one sequence while leaving the rest of the function real C. */
    // clang-format off
    asm("ldr %0, [%3, #0x1c]\n"
        "asr %0, %0, #8\n"
        "ldr %1, [%3, #0x20]\n"
        "asr %1, %1, #8\n"
        "ldr %2, [%3, #0x24]\n"
        "asr %2, %2, #8"
        : "=r" (d0), "=r" (d1), "=r" (d2)
        : "r" (self));
    // clang-format on

    {
        /* The ROM's three read-modify-write halfword updates reuse
         * registers in an ad hoc pattern that plain C never reproduces:
         * the first and third overwrite the just-used delta's own
         * register (r3, r4) with the sum, while the middle one leaves
         * the sum in the freshly loaded value's register (r1) instead -
         * not a consistent rule a normal expression/assignment phrasing
         * can steer this compiler's allocator into. Thumb `ldrh`/`strh`
         * also have no SP-relative form, so the ROM materializes `sp`
         * into a plain register (r2) first - a bare "r" input constraint
         * here would let gcc hand back `sp` itself as that register
         * (address-legal for a `ldr`, but out of range for `ldrh`'s
         * 3-bit base-register field), so this pins the copy to r2
         * explicitly instead of trusting the constraint to materialize
         * one. */
        MATCH_HOLD_REG(s16 *, sp2, r2) = (s16 *)&buf;

        // clang-format off
        asm volatile(
            "ldrh r1, [r2]\n"
            "add r3, r1, r3\n"
            "strh r3, [r2]\n"
            "ldrh r1, [r2, #2]\n"
            "add r1, r1, r5\n"
            "strh r1, [r2, #2]\n"
            "ldrh r3, [r2, #4]\n"
            "add r4, r3, r4\n"
            "strh r4, [r2, #4]"
            :
            : "r" (sp2), "r" (d1), "r" (d0), "r" (d2)
            : "r1", "r3", "r4", "memory");
        // clang-format on
    }

    {
        /* Plain `*(struct blob0xc *)outArg = buf;` picks a different
         * scratch-register triple here than the ROM's own re-use of
         * r4-r6 (the same registers the initial self->buf copy used,
         * long dead by this point) - register-pinning both ends forces
         * the same choice back. */
        MATCH_HOLD_REG(void *, dst, r2) = outArg;
        MATCH_HOLD_REG(s16 *, src, r1) = (s16 *)&buf;

        // clang-format off
        asm volatile(
            "ldmia r1!, {r4, r5, r6}\n"
            "stmia r2!, {r4, r5, r6}"
            :
            : "r" (dst), "r" (src)
            : "r4", "r5", "r6", "memory");
        // clang-format on
    }

    return outArg;
}

/* Same "self" object family as above - see this file's header
 * comment and docs/matching/archive/issue-50-actor-2a69c.md. (This was a
 * separate file while `GetActorWorldBox`, above, was still raw.) */

/* Trivial getter for `visible`. */
u8 IsActorVisible(void *selfArg)
{
    return ((struct actor_self *)selfArg)->visible;
}

/* Teardown: marks `self` "dead" (`vtable = gActorVtable`), unlinks
 * it from the circular `prev`/`next` list, and frees it
 * when `flags & 1`. Same shape as `DestroyPolarPlayer`'s unlink sequence in
 * polar_player_actions.c. */
void DestroyActor(void *selfArg, s32 flags)
{
    struct actor_self *self = selfArg;

    self->vtable = (struct actor_vtable *)gActorVtable;

    {
        struct actor_self *next = self->next;
        struct actor_self *prev = self->prev;
        next->prev = prev;
    }
    {
        struct actor_self *prev = self->prev;
        struct actor_self *next = self->next;
        prev->next = next;
    }

    if (flags & 1) {
        mem_free(self);
    }
}

/* Linear-searches `gCollectedSpawns`'s first `gCollectedSpawnCount`
 * entries for `self`, returning whether it's present. */
s32 IsSpawnCollected(void *selfArg)
{
    u8 *self = selfArg;
    MATCH_HOLD_REG(s32, i, r2) = 0;
    s32 count = gCollectedSpawnCount;

    if (i < count) {
        s32 n = count;
        MATCH_HOLD_REG(void **, p, r1) = gCollectedSpawns;

        do {
            if (*p == self) {
                return 1;
            }
            p++;
            i++;
        } while (i < n);
    }
    return 0;
}

/* Appends `self` to `gCollectedSpawns` (capped at 15 entries), unless
 * it's `NULL`, the array is already full, or it's already present. */
void MarkSpawnCollected(void *selfArg)
{
    MATCH_HOLD_REG(u8 *, self, r3) = selfArg;
    s32 count = gCollectedSpawnCount;

    if (count == 0xf || self == NULL) {
        return;
    }

    {
        s32 i = 0;

        if (i < count) {
            s32 n = count;
            void **p = gCollectedSpawns;

            do {
                if (*p == self) {
                    return;
                }
                p++;
                i++;
            } while (i < n);
        }
    }

    {
        s32 freshCount = gCollectedSpawnCount;

        gCollectedSpawns[freshCount] = self;
        gCollectedSpawnCount = freshCount + 1;
    }
}

/* Clears `gCollectedSpawns`'s entry count. */
void ClearCollectedSpawns(void)
{
    gCollectedSpawnCount = 0;
}

/* Loads the palette-cycle cursor/bound pair (`gActorPaletteCycleFrame`/
 * `gActorPaletteCycleTarget`, see `UpdateActorPaletteCycle` below) from their saved
 * counterparts (`gSavedActorPaletteCycleFrame`/`gSavedActorPaletteCycleTarget`) and resets the
 * DMA-refresh counter `gActorPaletteCycleTimer`. */
void RestoreActorPaletteCycle(void)
{
    gActorPaletteCycleFrame = gSavedActorPaletteCycleFrame;
    gActorPaletteCycleTarget = gSavedActorPaletteCycleTarget;
    gActorPaletteCycleTimer = 0;
}

/* The inverse of `RestoreActorPaletteCycle`: saves the current cursor/bound pair back
 * into `gSavedActorPaletteCycleFrame`/`gSavedActorPaletteCycleTarget`. */
void SaveActorPaletteCycle(void)
{
    gSavedActorPaletteCycleFrame = gActorPaletteCycleFrame;
    gSavedActorPaletteCycleTarget = gActorPaletteCycleTarget;
}

/* Same palette-cycle cluster as the functions around it - see
 * docs/matching/archive/issue-50-actor-2a69c.md. */

/* Per-frame palette-cycle DMA: while `gActorPaletteCycleEnabled` is set, DMAs one
 * `0x1c0`-byte palette-animation "frame" (`gActorPaletteCycleFrames +
 * cursor*0x1c0`) to BG palette RAM. Every `0x24` calls (the
 * `gActorPaletteCycleTimer` counter), advances the cursor (`gActorPaletteCycleFrame`)
 * toward `gActorPaletteCycleTarget`: increments while still below the bound,
 * decrements once past it, and holds steady exactly at the bound -
 * `SaveActorPaletteCycle`/`SetActorPaletteCycle` flip which end is "the bound" to make this
 * ping-pong.
 *
 * Two gaps this needed, see docs/matching/archive/issue-50-actor-2a69c.md:
 * - The cursor-advance tail: plain if/else-if/else (and every other
 *   C-level phrasing tried - goto-linearized with an explicit `result`
 *   copy, cached-address locals, register-pinned address locals) lets
 *   this compiler speculatively compute the decrement (`idx - 1`) ahead
 *   of the branch that decides whether it's needed, folding away the
 *   ROM's own redundant unconditional jump on the increment path.
 * - The literal pool: the ROM splits this function's 5-word pool right
 *   after that same tail's unconditional `b`, mid-function - but this
 *   compiler's own plain-C-driven pool placement (used for a normal
 *   `extern` global access) always dumps everything at the function's
 *   very end and completely ignores an `asm(".pool")` marker placed
 *   around it (confirmed: neither embedding one inside the tail's own
 *   asm block, nor a separate standalone `asm volatile(".pool");`
 *   statement between two otherwise-plain-C-interspersed asm blocks,
 *   moved anything). A real, respected `.pool` split only works for
 *   symbols whose literal load is itself written directly in inline-asm
 *   text using the assembler's own `=symbol` pseudo-op (opaque to this
 *   compiler's own pool bookkeeping, so it's the real GNU `as` that
 *   manages - and obeys `.pool` for - that queue), so every global
 *   access in this function had to move into one continuous asm island
 *   to land all five words in the ROM's one mid-function group; the
 *   `if`/`return` control flow stays real (GCC's own shared-epilogue
 *   return point, reached by both this asm's early `beq`/`ble` and by
 *   falling off the end), so the function signature/prologue/epilogue
 *   are still fully compiler-generated. */
void UpdateActorPaletteCycle(void)
{
    // clang-format off
    asm volatile(
        "ldr r0, =gActorPaletteCycleEnabled\n"
        "ldrb r0, [r0]\n"
        "cmp r0, #0\n"
        "beq .Lsub802AB58_end\n"
        "ldr r4, =gActorPaletteCycleFrame\n"
        "ldr r1, [r4]\n"
        "lsl r0, r1, #3\n"
        "sub r0, r0, r1\n"
        "lsl r0, r0, #6\n"
        "ldr r1, =gActorPaletteCycleFrames\n"
        "add r0, r0, r1\n"
        "mov r1, #0xa0\n"
        "lsl r1, r1, #19\n"
        "mov r2, #0xe0\n"
        "lsl r2, r2, #1\n"
        "mov r3, #0x10\n"
        "bl QueueVramDmaTransfer\n"
        "ldr r1, =gActorPaletteCycleTimer\n"
        "ldr r0, [r1]\n"
        "add r0, r0, #1\n"
        "str r0, [r1]\n"
        "cmp r0, #0x23\n"
        "ble .Lsub802AB58_end\n"
        "mov r0, #0\n"
        "str r0, [r1]\n"
        "add r1, r4, #0\n"
        "ldr r0, =gActorPaletteCycleTarget\n"
        "ldr r3, [r0]\n"
        "ldr r2, [r1]\n"
        "sub r0, r3, r2\n"
        "cmp r0, #0\n"
        "blt .Lsub802AB58_dec\n"
        "add r0, r2, #0\n"
        "cmp r3, r0\n"
        "beq .Lsub802AB58_store\n"
        "add r0, r0, #1\n"
        "b .Lsub802AB58_store\n"
        ".pool\n"
        ".Lsub802AB58_dec:\n"
        "sub r0, r2, #1\n"
        ".Lsub802AB58_store:\n"
        "str r0, [r1]\n"
        ".Lsub802AB58_end:"
        :
        :
        : "r0", "r1", "r2", "r3", "r4", "lr", "cc", "memory");
    // clang-format on
}

/* Same palette-cycle cluster as `RestoreActorPaletteCycle`/
 * `SaveActorPaletteCycle` above - see docs/matching/archive/issue-50-actor-2a69c.md.
 * (This was a separate file while `UpdateActorPaletteCycle`, above, was
 * still raw.) */

/* Seeds the palette-cycle cursor/bound pair from a per-category table
 * (`gActorPaletteCycleStartFrames`/`gActorPaletteCycleTargetFrames`, indexed by `idx`) and
 * resets the DMA-refresh counter. */
void SetActorPaletteCycle(s32 idx)
{
    gActorPaletteCycleFrame = gActorPaletteCycleStartFrames[idx];
    gActorPaletteCycleTarget = gActorPaletteCycleTargetFrames[idx];
    gActorPaletteCycleTimer = 0;
}

/* Arms/disarms the palette-cycle system (`gActorPaletteCycleEnabled`), resets
 * the cursor/bound/DMA-refresh counter, and saves that reset state back
 * via `SaveActorPaletteCycle`. */
void EnableActorPaletteCycle(u8 flag)
{
    gActorPaletteCycleEnabled = flag;
    gActorPaletteCycleFrame = 0;
    gActorPaletteCycleTarget = 0;
    gActorPaletteCycleTimer = 0;
    SaveActorPaletteCycle();
}

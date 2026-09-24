#include "core.h"
#include "actor.h"

/* GitHub issue #9/#10: 0x0800A884 - the same big, still-unnamed "part"
 * object family as `actor_part15.c`/`actor_part48.c`; raw offset casts
 * throughout for the same reason those files give. */

extern void *sub_803AD7C(void *arg0, void *fn);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800A0FC(void *self);
extern void sub_80231EC(void *arg0, s32 arg1);
extern void *sub_80083B8(void *part);
extern s32 sub_8026BC0(void *arg0, s32 x, s32 y);
extern void *gUnknown_03001308;
extern void *gUnknown_030012C0;
extern u8 gStaticData_0816B300[];

/* A per-frame "reentrancy guard"-shaped wrapper (only runs if
 * `self+0xc` bit 7 is set): fires `self->table+0x70`'s trampoline via
 * `sub_803AD7C`, then calls `sub_800A0FC` (still raw) with the global
 * `gUnknown_03001308+0x2a` flag held set for the duration. If
 * `self+0xac` (a pointer, cleared here) was non-null, sets `self+0x68`
 * bit 3 and clears the `+0x100`/`+0x102`/`+0x103` flag bytes. Then
 * dispatches on `gUnknown_03001308+0x29` (a pending-action "kind"
 * byte, cleared back to 0 by every path here): kind 0 additionally
 * resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` is exactly 8; kinds
 * 1/5/7/9 (`gStaticData_0816B300`'s index scheme - see the `case`
 * labels below) are no-ops beyond the shared reset; kind 1 also sets
 * `self+0xc` bit 6, clears `+0x8c`, calls `sub_80231EC`, and fires the
 * `self->table+0x68` trampoline (arg 1); kind 5 sets the `+0x100`
 * flag; kind 7 sets `+0x102`; kind 10 sets `+0x103`. Finally, looks up
 * the current keyframe record (`sub_80083B8`, already parked in
 * `actor_part5.c`) and picks a `{s16 x, s16 y}` offset table off its
 * `+4` byte's upper nibble - the exact same `sub_80084C4`
 * (`actor_part6.c`) case-to-block mapping (0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else -> the fixed fallback
 * `gStaticData_0816B300`) - applies it (mirrored by `self+0x28` bit 4)
 * to `self`'s de-Q8'd position, and probes the result via
 * `sub_8026BC0` (still raw). A hit (code 6) snaps `self`'s Y position
 * down to the next multiple of 8 (unless `+0x101` is already set) and
 * fires the table+0x68 trampoline with code `0x17`; any other code
 * fires the same trampoline with code `0x18` if `+0x101` is set.
 * Returns the (possibly just-updated) `self+0x68` state byte.
 *
 * The three `sub_803AD88` calls each also load (but never pass through
 * r0-r3) the table's own `+4` function-pointer field right next to the
 * `+0` offset they do use - a "dead read" the ROM performs anyway,
 * same established idiom as `sub_80096C0`'s own `sub_803AD88` calls in
 * `actor_part11.c` (`register void *deadRead asm("r4") = *(void
 * *volatile *)(...)`).
 *
 * PARKED, NOT BYTE-MATCHING (but far closer than before - see below):
 * every load, store, branch and call in this reconstruction is
 * confirmed correct against the ROM - both jump-table dispatches
 * reproduce the ROM's own tables exactly (matching `sub_80084C4`'s
 * established case-to-block mapping for the second one, needing the
 * same non-monotonic case-scattering trick - case 3/4, then case 1/2,
 * then case 5 - to make this compiler choose a jump table over a
 * compare-chain), and the shared `_0800A9F4`-style tail store merges
 * via the `goto storeAndDispatch` pair the same way the ROM merges
 * those two blocks into one physical store. The leading ~40
 * instructions (through the `sub_800A0FC` call and the `+0xac`
 * conditional block) are register-for-register byte-exact, confirmed
 * again this session against the *whole* function (not just in
 * isolation - the earlier isolated match didn't survive once the rest
 * of the function was added, see "Real gotchas" below). The 10-way
 * "kind" dispatch (including two case bodies - kind 7/kind 10 - that
 * needed hand-written `asm volatile` islands to reproduce the ROM's
 * own cross-case tail-merge exactly, `case 4`'s own distinct
 * non-merged tail closed with a plain register pin instead) and the
 * keyframe-lookup/camera-probe tail (including the `self+0x28` bit-4
 * test, which needed the same shift-vs-mask rewrite as the leading
 * block's own bit-7 test) are now ALSO byte-for-byte matched. Only two
 * narrow, purely register-*choice* gaps remain, identified precisely:
 *
 * 1. The `self+0x105` clear's address computation loads the raw
 *    `0x105` immediate from the literal pool into `r0` here, where the
 *    ROM uses `r2` for that one transient scratch value (the
 *    *destination* address still correctly lands in `r6` either way,
 *    matching the ROM's own later reuse of `r6` for `self+0x105`'s
 *    address across the `sub_803AD7C` call). Every register-pin
 *    variation tried for this single scratch temp (pinning it
 *    directly, hoisting the address into its own persistent pointer
 *    variable, reordering the two leading clears) either left this
 *    register choice unchanged or reshuffled unrelated *later*
 *    register choices instead (the `+0xac` block's own register plan,
 *    the type-switch's jump-table register choices) - a instance of
 *    this session's general "ripple" lesson (see below) with no
 *    register-pin fix found that doesn't trade this one gap for a
 *    worse one elsewhere.
 * 2. `kindZero`'s `self+0x68 == 8` test loads the byte through `r7`
 *    (the same register already holding `self+0x68`'s address) into a
 *    *fresh* register (`r0`) here, where the ROM overwrites `r7`
 *    itself with the loaded byte (`ldrb r7, [r7], an idiom to a
 *    location) - both this project's usual `register T v asm("r7")`
 *    pin and an explicit `asm volatile("ldrb r7, [r7, #0]")` island
 *    (matching the `case 4`/kind-7/kind-10 islands' own established
 *    style) were tried; both compile and assemble correctly and this
 *    *one* instruction pair does reproduce (`ldrb r7, [r7]; cmp r7,
 *    #8`), but introducing either one perturbs the *upstream* `+0xac`
 *    block's own already-matching register choices (more registers
 *    pinned earlier in the function shifts this compiler's -O2
 *    register-coloring plan for unrelated code, the same "ripple"
 *    effect, just running backward through the function instead of
 *    forward this time) - reverting back to the plain, unpinned
 *    `if (*p68 != 8)` C phrasing keeps the rest of the function
 *    (everything else, including `case 4`/the type-switch/the camera
 *    probe) byte-exact and limits this single register-choice gap to
 *    a self-contained two-instruction pair, rather than trading it for
 *    a worse, wider mismatch.
 *
 * Neither gap changes program behavior or even instruction *count* -
 * both are the compiler choosing a different (but equally valid)
 * scratch register for a value that's dead moments later. See
 * docs/matching/issue-9-10-0x0800a884-graphics.md.
 *
 * Real gotchas found this session (in addition to the leading block's
 * own, already-documented ones):
 *
 * 1. **A leading-block isolated match doesn't survive whole-function
 *    compilation.** The doc's earlier claim that the leading ~40
 *    instructions were "confirmed" came from compiling just that
 *    prefix in isolation; adding the rest of the function back
 *    reshuffled several of those *same* register choices (the
 *    `self+0x105` scratch register among them) purely due to
 *    increased register pressure/allocation-order changes elsewhere -
 *    a full-function recompile is required after *every* later-block
 *    change, not just a diff of the block just edited.
 * 2. **`kind - 1` must stay a plain (wide) integer, never a `u8`
 *    local.** Storing the switch index in a `u8 idx` forces this
 *    compiler to truncate the subtraction result back to 8 bits
 *    (`lsls`/`lsrs #24` pair) before the `cmp #9`/`bhi` range check -
 *    the ROM never does this (its `subs r0, r1, #1` result feeds the
 *    unsigned `cmp` directly, relying on `kind == 0` already having
 *    been excluded by an earlier branch). Fixed by keeping `idx`
 *    (and the `(u32)idx <= 9` cast) as `s32`.
 * 3. **Physical block *order*, not just goto targets, has to mirror
 *    the ROM's own layout for this compiler's `if`-with-`goto`
 *    lowering.** Writing `if (kind == 0) { <kindZero body>; goto X; }
 *    <switch>` (the `kindZero` body physically first, matching a
 *    literal reading of the C) makes this compiler lay out the
 *    kind-zero body *inline* and skip *around* it via a `bne` when
 *    `kind != 0` - the ROM instead lays the switch out first and
 *    reaches the (later, separately labeled) kind-zero body via a
 *    direct `beq`. Fixed by writing `if (kind == 0) goto kindZero;
 *    <switch>; ...; kindZero: ...` - i.e. moving the kind-zero body's
 *    *source position* to after the switch, matching the ROM's own
 *    block order, not just its jump targets.
 * 4. **This compiler's own cross-jump/tail-merge pass will merge a
 *    case that the ROM does *not* merge, unless the two are
 *    register-distinguishable.** `case 4`'s own trailing store and
 *    the shared `case 6`/`case 10` merge point are structurally
 *    identical C (`self[N] = 1;` after two `self[M] = 0;` clears), and
 *    a plain unpinned compile merges all three into one shared tail -
 *    but the ROM's own `case 4` swaps which register holds the store
 *    *address* vs. *value* for its final `strb` relative to the
 *    `case 6`/`10` pair, which is what keeps this compiler from
 *    merging it in the ROM build. Reproduced by explicitly pinning
 *    `case 4`'s final store's address/value registers (`r0`/`r1`) to
 *    the ROM's own (swapped) roles; `case 6`/`case 10` themselves
 *    needed a genuine `asm volatile` island each (sharing a
 *    `.Lcase69_merge` local label between the two, the same
 *    "continuous asm island" technique as `AllocVramTileBlock`) since
 *    no plain-C phrasing reproduced the ROM's own specific
 *    register-threading (`r2` incrementing `0x100`->`0x102`->`0x103`
 *    across the two cases) for that shared tail.
 * 5. **A bit-4 test (`self[0x28] & 0x10`) needs the same
 *    shift-not-mask rewrite as the leading block's own bit-7 test**,
 *    just shifted to put the target bit in the sign position instead
 *    of the top: `(s32)(self[0x28] << 27) < 0`, not `& 0x10` - this
 *    compiler doesn't normalize between the two phrasings, and only
 *    the shift form reproduces the ROM's own `lsls #0x1b`/`cmp
 *    #0`/`bge`.
 * 6. **The "ripple" effect runs in both directions.** Every fix
 *    attempted for either of the two remaining gaps above (register
 *    pins, `asm volatile` islands, restructured pointer variables)
 *    reliably reproduced the *targeted* instruction(s) but just as
 *    reliably reshuffled register choices in unrelated, already-
 *    matching code elsewhere in the function - sometimes upstream of
 *    the change, sometimes downstream, sometimes both. The dense
 *    10-way switch and the shared literal pool for the whole function
 *    appear to make this compiler's -O2 register allocator especially
 *    sensitive to any change in the total number of pinned/asm-
 *    referenced registers, not just their positions.
 *
 * The next session picking this up should treat the two remaining
 * gaps as a genuine compiler-fragility floor for this specific
 * function shape, not an unexplored lead - reasonable variations of
 * every technique in this project's toolbox (plain pins, scoped
 * pins, `asm volatile` islands with shared local labels, explicit
 * `.pool`/`.align` placement) were tried for both gaps this session. */
#if NON_MATCHING
u8 sub_800A884(void *selfArg)
{
    u8 *self = selfArg;
    u8 *p68;
    register u8 *storeAddr asm("r0");
    register u8 storeVal asm("r1");
    u8 kind;

    {
        register u8 byte asm("r1") = self[0xc];
        register u32 result asm("r0") = byte >> 7;
        if (!result) {
            goto end;
        }
    }

    {
        register s32 zero asm("r4") = 0;

        p68 = self + 0x68;
        *p68 = zero;
        self[0x105] = zero;
        {
            u8 *tbl = *(u8 **)(self + 0x18) + 0x70;
            s16 offset = *(s16 *)tbl;
            void *addr = self + offset;
            void *fn = *(void **)(tbl + 4);
            sub_803AD7C(addr, fn);
        }
        self[0x105] = 1;

        *((u8 *)gUnknown_03001308 + 0x2a) = 1;
        sub_800A0FC(self);
        *((u8 *)gUnknown_03001308 + 0x2a) = zero;

        if (*(void **)(self + 0xac) != 0) {
            {
                register u8 mask asm("r0") = 8;
                register u8 old asm("r2") = *p68;
                mask |= old;
                *p68 = mask;
            }
            *(s32 *)(self + 0xac) = zero;
            self[0x100] = zero;
            self[0x102] = zero;
            self[0x103] = zero;
        }
    }

    kind = *((u8 *)gUnknown_03001308 + 0x29);
    if (kind == 0) {
        goto kindZero;
    }

    {
        s32 idx = kind - 1;

        if ((u32)idx <= 9) {
            switch (idx) {
            case 0:
                {
                    register u8 mask asm("r0") = 0x40;
                    register u8 old asm("r2") = self[0xc];
                    mask |= old;
                    self[0xc] = mask;
                }
                *(s32 *)(self + 0x8c) = 0;
                sub_80231EC(gUnknown_030012C0, 0);
                {
                    u8 *tbl = *(u8 **)(self + 0x18) + 0x68;
                    s16 offset = *(s16 *)tbl;
                    void *addr = self + offset;
                    register void *deadRead asm("r4") = *(void *volatile *)(tbl + 4);
                    (void)deadRead;
                    sub_803AD88(addr, 0, 1, 0);
                }
                break;
            case 1:
            case 2:
            case 3:
            case 5:
            case 7:
            case 8:
                break;
            case 4:
                self[0x102] = 0;
                self[0x103] = 0;
                {
                    register u8 one asm("r1") = 1;
                    register u8 *addr100 asm("r0") = self + 0x100;
                    *addr100 = one;
                }
                break;
            case 6:
                asm volatile(
                    "ldr r0, =0x103\n\t"
                    "add r1, %0, r0\n\t"
                    "mov r0, #0\n\t"
                    "strb r0, [r1, #0]\n\t"
                    "mov r2, #0x80\n\t"
                    "lsl r2, r2, #1\n\t"
                    "add r1, %0, r2\n\t"
                    "strb r0, [r1, #0]\n\t"
                    "mov r0, #1\n\t"
                    "add r2, r2, #2\n\t"
                    "b .Lcase69_merge\n\t"
                    :
                    : "r"(self)
                    : "r0", "r1", "r2", "cc", "memory"
                );
            case 9:
                asm volatile(
                    "mov r0, #0x81\n\t"
                    "lsl r0, r0, #1\n\t"
                    "add r1, %0, r0\n\t"
                    "mov r0, #0\n\t"
                    "strb r0, [r1, #0]\n\t"
                    "mov r2, #0x80\n\t"
                    "lsl r2, r2, #1\n\t"
                    "add r1, %0, r2\n\t"
                    "strb r0, [r1, #0]\n\t"
                    "mov r0, #1\n\t"
                    "add r2, r2, #3\n\t"
                    ".Lcase69_merge:\n\t"
                    "add r1, %0, r2\n\t"
                    "strb r0, [r1, #0]\n\t"
                    :
                    : "r"(self)
                    : "r0", "r1", "r2", "cc", "memory"
                );
            }
        }
    }
    storeAddr = (u8 *)gUnknown_03001308 + 0x29;
    storeVal = 0;
    goto storeAndDispatch;

kindZero:
    if (*p68 != 8) {
        goto dispatch;
    }
    self[0x102] = 0;
    self[0x103] = 0;
    storeAddr = self + 0x100;
    storeVal = kind;

storeAndDispatch:
    *storeAddr = storeVal;

dispatch:
    {
        void *info = sub_80083B8(self);
        u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
        void *tbl;

        switch (type) {
        case 0:
            tbl = (u8 *)info + 0x24;
            break;
        case 3:
        case 4:
            tbl = gStaticData_0816B300;
            break;
        case 1:
        case 2:
            tbl = gStaticData_0816B300;
            break;
        case 5:
            tbl = gStaticData_0816B300;
            break;
        case 6:
            tbl = (u8 *)info + 0x14;
            break;
        default:
            tbl = gStaticData_0816B300;
            break;
        }

        {
            s32 x = *(s32 *)self >> 8;
            s32 y = *(s32 *)(self + 4) >> 8;
            s32 code;

            if ((s32)(self[0x28] << 27) < 0) {
                x -= *(s16 *)tbl;
            } else {
                x += *(s16 *)tbl;
            }
            y += *(s16 *)((u8 *)tbl + 2);

            code = sub_8026BC0(gUnknown_03001308, x, y);
            if (code == 6) {
                if (self[0x101] == 0) {
                    s32 snap = (((u32)y & 0x00FFFFF8) + 7) - y;
                    *(s32 *)(self + 4) += snap << 8;
                    {
                        u8 *tbl2 = *(u8 **)(self + 0x18) + 0x68;
                        s16 offset = *(s16 *)tbl2;
                        void *addr = self + offset;
                        register void *deadRead asm("r4") = *(void *volatile *)(tbl2 + 4);
                        (void)deadRead;
                        sub_803AD88(addr, 0, 0x17, 0);
                    }
                }
            } else {
                if (self[0x101] != 0) {
                    u8 *tbl2 = *(u8 **)(self + 0x18) + 0x68;
                    s16 offset = *(s16 *)tbl2;
                    void *addr = self + offset;
                    register void *deadRead asm("r4") = *(void *volatile *)(tbl2 + 4);
                    (void)deadRead;
                    sub_803AD88(addr, 0, 0x18, 0);
                }
            }
        }
    }

end:
    return self[0x68];
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

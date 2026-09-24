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
 * conditional block) are register-for-register byte-exact. The 10-way
 * "kind" dispatch (including two case bodies - kind 7/kind 10 - that
 * needed hand-written `asm volatile` islands to reproduce the ROM's
 * own cross-case tail-merge exactly, `case 4`'s own distinct
 * non-merged tail closed with a plain register pin instead) and the
 * keyframe-lookup/camera-probe tail (including the `self+0x28` bit-4
 * test, which needed the same shift-vs-mask rewrite as the leading
 * block's own bit-7 test) are ALSO byte-for-byte matched, including
 * the `snap` computation feeding the Y-position rounding (see "Real
 * gotchas" point 6 below - this one was found and closed this
 * follow-up session; it was NOT one of the two gaps the previous
 * session's doc comment described as remaining, meaning that claim of
 * "only two gaps left" was itself slightly stale). `kindZero`'s own
 * `self+0x68 == 8` test and its three-clear tail (`self+0x102`,
 * `self+0x103`, `self+0x100`, all sharing the `storeAndDispatch` merge
 * point with the switch path) are now ALSO byte-for-byte matched -
 * see "Real gotchas" points 7-8 for how the two previously-parked
 * `kindZero` gaps (the `ldrb r7, [r7]` self-overwrite, and the third
 * clear's offset-walk register) were finally closed this session.
 * Only one narrow, purely register-*choice* gap remains, plus one
 * single extra instruction that appeared as a side effect of closing
 * the `kindZero` gaps (a strict net improvement over both - see
 * below):
 *
 * 1. The `self+0x105` clear's address computation loads the raw
 *    `0x105` immediate from the literal pool into `r0` here, where the
 *    ROM uses `r2` for that one transient scratch value (the
 *    *destination* address still correctly lands in `r6` either way,
 *    matching the ROM's own later reuse of `r6` for `self+0x105`'s
 *    address across the `sub_803AD7C` call). Every register-pin
 *    variation tried across two separate sessions for this single
 *    scratch temp (pinning it directly, hoisting the address into its
 *    own persistent pointer variable, an opaque `asm volatile`
 *    barrier forcing the pin to "stick", a fully opaque asm island
 *    computing the address outright, reordering the two leading
 *    clears) either left this register choice unchanged (the pin
 *    silently ignored, since the offset is a compile-time constant
 *    the optimizer refuses to route through a hinted-but-unbarriered
 *    register) or reshuffled `self` itself out of `r5` into `r6` for
 *    the *entire rest of the function* the moment the pin was made to
 *    "stick" via any barrier or asm island (confirmed both with a
 *    barriered register variable and with a fully opaque asm block
 *    hardcoding `r5`/`r6` - both produced the identical `self`-moves-
 *    to-`r6` ripple). Introducing a *new*, previously-unforced hard-
 *    register requirement this early in the function's body appears
 *    to be what triggers this specific ripple (contrast with gap
 *    2 below, closed this session precisely because its fix did
 *    *not* introduce a new hard-register requirement - it only
 *    reused a register, `r7`, already forced there by `p68`'s own
 *    natural allocation). No register-pin fix was found this session
 *    either that doesn't trade this one gap for a worse one
 *    elsewhere; left parked as the sole remaining register-choice
 *    gap.
 * 2. `kindZero`'s three-clear tail (`self+0x102`, `self+0x103`,
 *    `self+0x100`, `storeAndDispatch`'s shared final `strb`) now
 *    matches the ROM's own `r2` offset-walk (`+1`, then `-3`) exactly,
 *    reusing `kind`'s own residual `r1` register as the "already
 *    zero" store value across all three clears just like the ROM
 *    does - **except for a single extra `movs r1, #0` instruction**
 *    that appears right after the `cmp r7, #8` branch, immediately
 *    before the `r2` offset walk starts. This is a compiler-driven
 *    constant-propagation artifact: this compiler already proves
 *    `kind == 0` on this path (from the earlier `if (kind == 0) goto
 *    kindZero;` guard) and schedules a fresh, redundant `r1 = 0`
 *    materialization into the first free slot after the branch,
 *    regardless of where the C source's own `storeVal = kind;`
 *    assignment physically sits (tried at the very end, right after
 *    the offset walk, and via an explicit `asm volatile` barrier at
 *    several different points in between - every placement produced
 *    the *exact same* single extra `movs r1, #0`, always scheduled
 *    right after the branch, never eliminated outright). This is a
 *    strict, verified improvement over the previous session's
 *    two-gap state: the `ldrb r7, [r7]` self-overwrite and the wrong-
 *    register offset walk (worth 4 mismatched instructions between
 *    them) are now fully closed, at the cost of this single new
 *    2-byte `movs r1, #0` - net fewer mismatched bytes overall, with
 *    zero regressions anywhere else in the function (re-verified via
 *    full-function diff after every change, per this project's
 *    workflow).
 *
 * See docs/matching/issue-9-10-0x0800a884-graphics.md for the full
 * accounting of both gaps.
 *
 * Real gotchas found across both sessions:
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
 * 6. **A `(masked + 7) - y` expression can get algebraically
 *    re-associated into `masked - (y - 7)` at -O2**, changing which
 *    register holds the intermediate and introducing a spurious `sub`
 *    against a second register the ROM never uses - even though both
 *    forms are the same value. The ROM computes this Y-snap purely in
 *    one register (`ands`/`adds #7`/`subs`), never touching a second
 *    one. Fixed by forcing the whole 3-step computation through a
 *    single pinned accumulator: `register s32 acc asm("r0") = (u32)y &
 *    0x00FFFFF8; acc = acc + 7; acc = acc - y;` - written as three
 *    separate statements on the *same* pinned variable so this
 *    compiler has no freedom to re-associate the arithmetic into a
 *    different register pairing. Confirmed via full-function diff to
 *    have zero effect on any other register choice in the function -
 *    a clean, isolated fix.
 * 7. **A self-overwriting `ldrb rN, [rN]` (destination register same
 *    as the address register) *can* be reproduced without a whole-
 *    function ripple, but only via a matching-constraint asm operand
 *    against a variable *already* forced into that register by
 *    something else - not via a freshly `register T v asm("rN")`-
 *    declared variable on its own.** `kindZero`'s `self+0x68` byte
 *    load needed exactly `ldrb r7, [r7]`, and `p68` (`self+0x68`'s
 *    address) is *already* naturally allocated to `r7` throughout this
 *    function (nothing forces this - it's this compiler's own
 *    unforced choice, matching the ROM's). The fix: `register s32
 *    r7byte asm("r7"); asm volatile("ldrb r7, [r7]" : "=r"(r7byte) :
 *    "0"(p68));` - the `"0"` matching constraint ties the *input*
 *    register (wherever `p68` already lives) to the *output* register,
 *    rather than declaring a brand-new, independently-pinned `r7`
 *    variable. Since `p68` already lives in `r7` with no forcing
 *    needed, this introduces *zero new hard-register requirements*
 *    into the function's register-allocation graph - it only asserts
 *    "reuse whatever register this value's already in," which is
 *    fundamentally different from asking the allocator to conjure a
 *    *new* `r7` binding from scratch. This is why it didn't ripple,
 *    where the earlier (previous session's) `register T v asm("r7")`
 *    pin and freestanding `asm volatile("ldrb r7, [r7, #0]")` island
 *    both did - those introduced `r7` as an independent constraint on
 *    top of `p68`'s own, rather than aliasing the same one. Also note:
 *    the output type must be `s32` (a full register width), not `u8` -
 *    a `u8`-typed register-asm output variable used directly in a
 *    comparison made this compiler emit a bizarre spurious `mov r1,
 *    sp` / shift-pair "truncation" sequence instead of a plain `cmp`;
 *    reading the raw `s32` register value and comparing it directly
 *    avoided that entirely.
 * 8. **The matching-constraint technique from point 7 does *not*
 *    generalize to introducing a genuinely new scratch register that
 *    wasn't already forced somewhere else.** The `self+0x105` gap
 *    (gap 1 above) needs a *new* `r2` binding for a value (the literal
 *    `0x105`) that has no pre-existing forced home - unlike `p68`,
 *    which already lived in `r7` for unrelated reasons. Every attempt
 *    to introduce that binding (plain pin, barriered pin, matching-
 *    constraint asm, fully opaque asm island) rippled `self` itself
 *    out of `r5`. The distinguishing factor between the two gaps
 *    this session found: reusing an *already-forced* register's
 *    binding via a matching constraint is ripple-free; asking the
 *    allocator to forge a *new* one is not, at least not anywhere
 *    this early in the function's body.
 * 9. **The "ripple" effect runs in both directions and is a stable,
 *    reproducible property of this function's shape**, not a one-off:
 *    every fix attempted for the `self+0x105` gap across *two*
 *    separate sessions (register pins, `asm volatile` islands,
 *    restructured pointer variables, matching constraints) reliably
 *    reproduced the *targeted* instruction(s) but just as reliably
 *    reshuffled register choices in unrelated, already-matching code
 *    elsewhere in the function - confirmed again this session with
 *    two more independent technique variations, both producing the
 *    identical `self`-moves-to-`r6` ripple. The dense 10-way switch
 *    and the shared literal pool for the whole function appear to
 *    make this compiler's -O2 register allocator especially sensitive
 *    to *new* hard-register requirements introduced early in the
 *    function body specifically - point 7's success shows this is not
 *    a blanket "any asm ripples" rule, but a "asking for a genuinely
 *    new binding ripples, reusing an existing one does not" rule.
 *
 * The next session picking this up should treat the `self+0x105` gap
 * as a genuine compiler-fragility floor for this specific function
 * shape, not an unexplored lead - reasonable variations of every
 * technique in this project's toolbox, including the matching-
 * constraint trick that closed the `kindZero` gap, were tried for it
 * across two sessions now. The one remaining extra `movs r1, #0` (gap
 * 2 above) is a much smaller, single-instruction, non-rippling
 * artifact and may be worth one more look with a fresh technique, but
 * is not a priority given how narrow it already is. */
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
    {
        register s32 r7byte asm("r7");
        asm volatile("ldrb r7, [r7]" : "=r"(r7byte) : "0"(p68));
        if (r7byte != 8) {
            goto dispatch;
        }
    }
    asm volatile("" : "+r"(kind));
    {
        register s32 off asm("r2") = 0x102;
        self[off] = kind;
        off += 1;
        self[off] = kind;
        off -= 3;
        storeAddr = self + off;
    }
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
                    s32 snap;
                    {
                        register s32 acc asm("r0") = (u32)y & 0x00FFFFF8;
                        acc = acc + 7;
                        acc = acc - y;
                        snap = acc;
                    }
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

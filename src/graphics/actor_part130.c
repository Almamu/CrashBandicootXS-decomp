#include "core.h"
#include "actor_self.h"

/* Second half of issue #59's Phase 2 gap (`sub_80326E4`-`nullsub_35`,
 * the tail of `asm/code_3_2_20_28568_c99c_31784_31a6c.s`) - see
 * docs/matching/issue-59-0x08031784-actor.md and
 * docs/matching/issue-60-61-gap-31a6c-part2.md for the full writeup. A
 * sibling pass (`src/graphics/actor_part129.c`) covers the first half
 * of the same file (`sub_8031A6C`-`sub_8032688`).
 *
 * Two threads converge in this range:
 *
 * 1. The same per-instance boss-weapon/tracker "self" object family
 *    documented since issue #58 (`actor_part20.c`-`actor_part26.c`):
 *    state at `self+0x28`, table-index/"kind" at `self+0xc`, an
 *    anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 *    accumulator at `self+8`, a "part table" pointer at `self+0`, and
 *    an event/trampoline table pointer at `self+0x50` - none of these
 *    objects' full shapes are pinned down yet project-wide, so accesses
 *    stay raw offsets with doc comments, matching every neighboring
 *    `actor_part*.c` file's own convention.
 *
 * 2. The `gUnknown_030015AC` singleton system first constructed by
 *    `sub_80331BC` (this file) and already partially characterized by
 *    the LATER `actor_part28.c`-`actor_part37.c` (issue #62,
 *    `0x08033804`+): a second, independent "unique object" cluster,
 *    structurally parallel to the boss's own patrol/BG2-affine/tile
 *    machinery (issue #58) but on a completely separate global family
 *    (`gUnknown_030015A0`-`030015FF`, plus the P1/P2-mirror pair
 *    `gUnknown_030008B4`/`030008B8` and the row-pointer array
 *    `gUnknown_03001600`). Every global in that family already has a
 *    real name from `actor_part28.c`'s own extern block where this
 *    file's functions are the ones that *first* reference it in ROM
 *    order - reused verbatim here for consistency rather than
 *    reinvented. Per that file's own precedent (flat, independently
 *    linked BSS symbols, not fields of one struct reached through a
 *    common base pointer), this file does the same rather than
 *    introducing a struct wrapper that wouldn't match the actual link
 *    layout - see the writeup doc for the fuller rationale. */

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 GetAnimFrameBaseOffset(void *self);
extern u8 *GetAnimFrameData(void *self);
extern s32 sub_803B060(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);
extern void sub_803B0A8(void *self, s32 idx);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern void sub_802A7B8(void *self);
extern s32 sub_803ADB4(s32 dividend, s32 divisor);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void sub_8029E34(s32 arg0);
extern s32 sub_8029B2C(void);
extern u8 sub_802A6EC(void *self);
extern void sub_802A4EC(void);
extern void sub_802A4F8(void);
extern void sub_802F0DC(void *arg0);
extern void sub_8023430(void *self);
extern u8 *mem_alloc(u32 size, s32 flags);
extern struct actor_pmf gStaticData_0817C450[];
extern void mem_free(void *ptr);
extern void sub_8032AF8(void);
extern void sub_8032B6C(void);
extern void sub_803AD78(void *fn);
extern void sub_80330FC(void *tileRow);
extern void sub_8033550(void);
extern void sub_8033604(void);
extern void sub_80336CC(void);
extern void sub_802E538(s32 a, s32 b, s32 c, s32 d);
extern void sub_802E57C(s32 a, s32 b, s32 c);
extern void sub_802E5B0(s32 a, s32 b, s32 c);

extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03000884;
extern void *gUnknown_030008B4;
extern void *gUnknown_030008B8;
extern u16 gUnknown_03001590;
extern s32 gUnknown_03001594;

/* The `gUnknown_030015AC` singleton system's own globals - names as
 * already established by `actor_part28.c` (issue #62) for the ones it
 * also touches; the rest are new (first referenced anywhere in ROM
 * order by this file's functions). */
extern void *gUnknown_030015AC;
extern s32 gUnknown_030015A0;
extern s32 gUnknown_030015A4;
extern s32 gUnknown_030015A8;
extern void *gUnknown_03001600[];
extern s32 gUnknown_030015B0;
extern s32 gUnknown_030015B4;
extern s32 gUnknown_030015B8;
extern s32 gUnknown_030015BC;
extern s32 gUnknown_030015C0;
extern s32 gUnknown_030015C4;
extern s32 gUnknown_030015C8;
extern s32 gUnknown_030015CC;
extern s32 gUnknown_030015D0;
extern s32 gUnknown_030015D4;
extern void *gUnknown_030015D8;
extern void *gUnknown_030015DC;
extern s32 gUnknown_030015E0;
extern s32 gUnknown_030015E4;
extern s32 gUnknown_030015E8;
extern s32 gUnknown_030015EC;
extern s32 gUnknown_030015F0;
extern s32 gUnknown_030015F4;
extern s32 gUnknown_030015F8;
extern s16 gUnknown_030015FC;
extern u8 gUnknown_030015FE;
extern u8 gUnknown_030015FF;
extern u8 gUnknown_0300159C;
extern s32 gUnknown_03001598;

extern u8 gStaticData_087E543C[];
extern u8 gStaticData_087E5474[];
extern u8 gStaticData_087E4DF4[];
extern u8 gStaticData_087E54AC[];
extern u16 gStaticData_08169AE8[];
extern u8 gStaticData_08169CE8[];
extern u8 gStaticData_0817C4BC[];
extern void *gStaticData_0817C4C8[];

ACTOR_CALL_VIA_ALIASES

/* The homing spawn effect advanced by sub_8032718. */
struct actor_2718 {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 velX;           // 0x58
    s32 velY;           // 0x5C
};

/* An `InitActorPart`-based constructor: forwards its 4 real arguments
 * straight through (the 5th, `d`, is itself stack-passed), marks
 * `self+0x54 = 1` (health-like), sets `self+0x50`'s event/trampoline
 * table, and clears the `self+0x58` byte. Same shape as the
 * already-matched `sub_80305F8` (issue #58, `actor_part20d.c`), minus
 * that function's extra `b`/`c` re-stash into `self+0x58`/`self+0x5c`. */
void *sub_80326E4(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 dReg asm("r0") = d;
    register s32 one asm("r5") = 1;

    InitActorPart(self, a, b, c, dReg);
    *(s32 *)(self + 0x54) = one;
    *(void **)(self + 0x50) = gStaticData_087E543C;
    self[0x58] = 0;

    return self;
}

/* Trivial constant getter - always "true"/"ready". */
s32 sub_8032714(void *self)
{
    return 1;
}

/* Homing spawn effect step: advances the position by the velocity at
 * `+0x58`/`+0x5c`; once it leaves the positive quadrant past 0x1000,
 * plays sound 0xE and destroys itself, otherwise runs the shared
 * anim-frame-advance-and-clamp idiom. The idiom's `#4`/`#6` scheduling
 * gap closes by re-indexing `anims[animIndex]` for each field instead
 * of through a record pointer (docs/matching/pmf-dispatch-retry.md). */
void sub_8032718(void *selfArg)
{
    struct actor_2718 *self = selfArg;
    s32 one = 1;
    s32 x, y;
    s32 base;

    self->base.visible = one;
    x = self->base.x += self->velX;
    y = self->base.y += self->velY;
    if (x <= 0x1000 || y <= 0x1000) {
        PlaySfx(gUnknown_030012BC, 0xE, 0x100);
        if (self != NULL) {
            ACTOR_VCALL(&self->base, m08, 3);
        }
        return;
    }
    self->base.animTime += *(s16 *)&self->base.animTimer;
    self->base.animDone = 0;
    base = GetAnimFrameBaseOffset(self);
    if (base >= self->base.anims[self->base.animIndex].loopThreshold) {
        self->base.animTime -= (self->base.anims[self->base.animIndex].loopThreshold
                                - self->base.anims[self->base.animIndex].loopBase) << 8;
        self->base.animDone = one;
    }
}

/* Draws `self`'s current anim frame at its Q8 position, centered on the
 * frame's width/height bytes, unless it is entirely off screen. Same
 * shape as UpdateAnimatedActorPart (actor_part55.c) with the scale
 * doubling fixed off: `scaled` starts at 0 (halving nothing) and becomes
 * the 0x100 OBJ-affine bit once the sprite is known to be visible. The
 * third OAM word takes `self->unk_18` as its priority nibble, plus 0x800
 * when bit 15 of `self->visible` is set. */
void sub_80327A4(void *selfArg)
{
    struct actor_self *self = selfArg;
    s32 x;
    s32 y;
    u8 *frame;
    u32 scaled;
    s32 halfW;
    s32 halfH;
    u32 attr;
    u32 attr2;
    u16 attr2Out;
    s32 oamPriority = 0x180;
    u32 highBit = 0x800;

    {
        s32 qx = self->x, qy = self->y;

        x = qx >> 8;
        y = qy >> 8;
    }
    frame = GetAnimFrameData(self);
    scaled = 0;
    halfW = scaled ? frame[0] << 3 : frame[0] << 2;
    halfH = scaled ? frame[1] << 3 : frame[1] << 2;
    x -= halfW;
    y -= halfH;
    if (y > 0x9f)
        return;
    if (y + halfH * 2 < 0)
        return;
    if (x > 0xef)
        return;
    if (x + halfW * 2 < 0)
        return;

    scaled |= 0x100;
    attr = (y & 0xff) | ((x & 0x1ff) << 16) | sub_803B060(self) | scaled;
    x = self->unk_18;
    attr2 = x << 12;
    if (self->visible & 0x8000)
        attr2Out = attr2 | highBit;
    else
        attr2Out = attr2;
    SetupSpriteFrameOam(frame, attr, attr2Out, oamPriority);
}

/* Destructor: dispenses `reward` score increments via `sub_8023430`
 * while temporarily wearing `gStaticData_087E5474` (`sub_8032890`'s
 * constructor table), then runs the same teardown as `sub_803B0C4`
 * (actor_anim.c): retag to the shared "dead" table, unlink from the
 * `+0x48`/`+0x4c` circular list, and free `self` if `flags & 1`. The
 * parameter-copy order the earlier NAKED note blamed on the compiler
 * comes out as in the ROM from this plain form
 * (docs/matching/pmf-dispatch-retry.md). */
struct actor_283c {
    u8 unk_00[0x48];
    struct actor_283c *l48;
    struct actor_283c *l4c;
    void *vtable;
    u8 unk_54[0xc];
    s32 reward;
};

void sub_803283C(struct actor_283c *self, u32 flags)
{
    s32 i;

    self->vtable = gStaticData_087E5474;
    for (i = 0; i < self->reward; i++) {
        sub_8023430(gUnknown_030012C0);
    }
    self->vtable = gStaticData_087E4DF4;
    self->l4c->l48 = self->l48;
    self->l48->l4c = self->l4c;
    if (flags & 1) {
        mem_free(self);
    }
}

/* "Spawn effect type N" homing/seek-toward-point constructor - a
 * byte-for-byte twin of the already-matched `sub_802C3E8` (issue #52,
 * `actor_part19c2.c`/`actor_part19g.c`), per `docs/rom_map.md`'s
 * "Two small follow-ups round out the picture further" finding. Field
 * offsets differ slightly from that twin: this object keeps a fixed
 * `self+0x54 = 1` health field separate from the computed velocity
 * (stored at `self+0x58`/`self+0x5c` here, vs. `self+0x54`/`self+0x58`
 * there), and stashes `spawnParam` at `self+0x60` rather than `0x5c`. */
void *sub_8032890(void *selfArg, s32 a, s32 b, s32 c, s32 spawnParam)
{
    u8 *self = selfArg;
    register s32 dy asm("r3");
    s32 sum;
    s32 q;

    InitActorPart(self, a, b, c, 1);
    *(s32 *)(self + 0x54) = 1;
    *(void **)(self + 0x50) = gStaticData_087E5474;
    *(s32 *)(self + 0x60) = spawnParam;

    *(s32 *)(self + 0x20) += sub_8029E98();

    {
        register s32 ebResult asm("r0") = sub_8029EB4();
        register s32 old asm("r1") = *(s32 *)(self + 0x1c);
        dy = old + ebResult;
    }
    *(s32 *)(self + 0x1c) = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = (a1 ^ (a1 >> 31)) - (a1 >> 31);
        s32 dx = *(s32 *)(self + 0x20);
        s32 b1 = dx - 0x1000;
        s32 b2 = (b1 ^ (b1 >> 31)) - (b1 >> 31);

        sum = a2 + b2;
        if (sum < 0) {
            sum += 0x7ff;
        }
        q = sum >> 0xb;

        *(s32 *)(self + 0x58) = sub_803ADB4(0x1000 - dy, q);
        *(s32 *)(self + 0x5c) = sub_803ADB4(0x1000 - dx, q);
    }

    return self;
}

/* Trivial constant getter - always "true"/"ready", same shape as
 * `sub_8032714` above. */
s32 sub_803290C(void *self)
{
    return 1;
}

/* Damage/health countdown: subtracts `delta` from `self+0x54`, and once
 * it drops to zero (or below) plays the death sound and runs the full
 * state/accumulator/anim-frame reset idiom already matched for
 * `sub_80318B4` (issue #59, `actor_part125.c`)/`sub_8033AE0` (issue
 * #62, `actor_part30.c`). */
void sub_8032910(void *selfArg, s32 delta)
{
    u8 *self = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    *(s32 *)(self + 0x18) = 4;
    PlaySfx(gUnknown_030012BC, 4, 0x100);
    {
        register s32 one asm("r0") = 1;

        *(s32 *)(self + 0x28) = one;
        {
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = one;
            {
                u16 anim = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
        }
    }
}

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817C450
 * [this->state])()` (see `ACTOR_PMF_CALL`), then "destroy" once the
 * state-1 animation has played through, else the standard sub_802A7B8
 * step. */
void sub_8032950(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gStaticData_0817C450);

    if (self->state == 1 && self->animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(self, m08, 3);
        }
    } else {
        sub_802A7B8(self);
    }
}

/* Same shared shape as `sub_80329D4`/`sub_8033BB8` (issue #62,
 * `actor_part32.c`): forwards `a`/`b`/`c` (the last two re-stashed into
 * `self+0x58`/`self+0x5c`, `c` pinned to the high register `r8`
 * matching `sub_8033BB8`'s own documented gap - this compiler's
 * allocator always prefers the low registers when they fit, so `c`
 * needs the explicit pin to reproduce the ROM's choice), `d` stack-
 * passed straight through to `InitActorPart`, plus the fixed
 * `self+0x54 = 2`/`self+0x50` event table/`self+0x64 = 0`/
 * `self+0x60 = 0x95`/`self+0x68 (byte) = 0` initialization already
 * matched verbatim for `sub_80305F8` (issue #58, `actor_part20d.c`). */
void *sub_80329D4(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 cReg asm("r8") = c;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 2;

    InitActorPart(self, a, b, c, dReg);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E54AC;
    *(s32 *)(self + 0x58) = bReg;
    *(s32 *)(self + 0x5c) = cReg;
    *(s32 *)(self + 0x64) = 0;
    *(s32 *)(self + 0x60) = 0x95;
    self[0x68] = 0;

    return self;
}

/* Trivial `self+0x68` byte setter. */
void sub_8032A1C(void *selfArg)
{
    u8 *self = selfArg;
    self[0x68] = 1;
}

/* Patrol-speed decay plus a death transition: advances `self+0x24` by
 * `self+0x60`, decays `self+0x60` by 5 (floored at 0x14). If
 * `sub_802A6EC(self)` fires, draws a text popup on the orbiting
 * companion object (`gUnknown_03000884`, via its own `+0x50`
 * trampoline-table pointer, same `{s16 offset; ...; void *arg}`
 * convention already documented at `self+0x50` elsewhere in this
 * cluster), then runs the exact same state/anim-frame reset tail as
 * `sub_8032910` above. */
void sub_8032A24(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;
    s32 sum = *(s32 *)(self + 0x24);
    s32 delta = *(s32 *)(self + 0x60);

    sum += delta;
    *(s32 *)(self + 0x24) = sum;
    delta -= 5;
    *(s32 *)(self + 0x60) = delta;
    if (delta <= 0x13) {
        *(s32 *)(self + 0x60) = 0x14;
    }

    if (sub_802A6EC(self)) {
        u8 *player = gUnknown_03000884;
        u8 *table = *(u8 **)(player + 0x50);

        sub_803AD80(player + *(s16 *)(table + 0x20), 6, *(void **)(table + 0x24));

        *(s32 *)(self + 0x18) = 4;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        {
            register s32 one asm("r0") = 1;

            *(s32 *)(self + 0x28) = one;
            {
                register s32 zero asm("r2") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = one;
                {
                    u16 anim = *(u16 *)(*(u8 **)self + 0xc);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = zero;
            }
        }
    }
}

/* `sub_8032950`'s dispatch without its tail: `(this->*gStaticData_
 * 0817C450[this->state])()` (see `ACTOR_PMF_CALL`). */
void sub_8032A94(void *selfArg)
{
    struct actor_self *self = selfArg;

    ACTOR_PMF_CALL(self, gStaticData_0817C450);
}

/* Trivial `self+0x68` byte getter. */
u8 sub_8032AF0(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x68];
}

/* Palette blink/flash effect for the P2 VRAM fill-level meter, gated by
 * the one-shot latch `sub_8033804` (issue #62, `actor_part28.c`) arms
 * (`gUnknown_030015FC`/`030015FE`): once armed, advances the counter
 * every call, flips the toggle byte every 4th call, wraps the counter
 * past 0xb, then rewrites the meter's 16-halfword palette strip
 * (`0x05000020`) either solid white (`0x7fff`, while the toggle is set)
 * or restored from `gStaticData_08169AE8`'s own first 16 halfwords -
 * the same per-level table `sub_80336CC` below reads for the meter's
 * fill-level heights. */
void sub_8032AF8(void)
{
    if (gUnknown_030015FC == 0) {
        return;
    }

    gUnknown_030015FC += 1;
    {
        register s32 three asm("r0") = 3;
        register s32 cur asm("r1") = *(vu16 *)&gUnknown_030015FC;
        register s32 result asm("r0");

        result = three & cur;
        if (result == 0) {
            register u8 *addr asm("r1") = &gUnknown_030015FE;
            register u8 one asm("r0") = 1;
            register u8 old asm("r3") = *addr;

            one ^= old;
            *addr = one;
        }
    }

    if (gUnknown_030015FC > 0xb) {
        gUnknown_030015FC = 0;
    }

    {
        register u8 *flagAddr asm("r5") = &gUnknown_030015FE;
        register u16 white asm("r4") = 0x7fff;
        u16 *src = gStaticData_08169AE8;
        vu16 *dst = (vu16 *)0x05000020;
        vu16 *end = (vu16 *)((u8 *)dst + 0x1e);

        do {
            if (*flagAddr != 0) {
                *dst = white;
            } else {
                *dst = *src;
            }
            src++;
            dst++;
        } while ((s32)dst <= (s32)end);
    }
}

/* A frame counter (`gUnknown_030015F4`) drives the same P1/P2-mirrored
 * speed-override toggle already matched for `sub_8033828` (issue #62,
 * `actor_part28.c`) - inlined twice here (once forcing "max speed"
 * every 16th frame, once restoring the cached normal speed every
 * 8th-but-not-16th frame) rather than calling that function, matching
 * the ROM exactly (the ROM's own build never emits a `bl sub_8033828`
 * here, so the original source duplicated the logic rather than
 * sharing it). `sub_8033828`'s own body confirms the same "reload
 * `gUnknown_030008B4`'s pointer value through a register-pinned `p`,
 * assign the register-pinned `val` in its own statement" idiom closes
 * the exact register split this compiler otherwise collapses (loading
 * a >255 constant like `0x7FFF` straight into a pre-existing
 * register-pinned variable forces this compiler to materialize it in
 * a fresh register first, then copy - the `asm("" : "+r"(p))` barrier
 * between the pointer reload and the value assignment keeps that
 * reload from being folded into the shared tail the two call sites
 * below happen to converge on). Tail: runs `sub_8032AF8` (the meter
 * blink above) then dispatches the singleton's current animation
 * "kind" through the third table family (`gStaticData_0817C4C8`), the
 * same convention already documented for the entity/actor category
 * vtables. */
static inline void CommitSpeed(u8 *p, u16 val)
{
    *(u16 *)(p + 0x1e) = val;
    *(u16 *)((u8 *)gUnknown_030008B8 + 0x1e) = val;
}

void sub_8032B6C(void)
{
    s32 counter = gUnknown_030015F4 + 1;
    gUnknown_030015F4 = counter;

    if ((counter & 0xf) == 0)
    {
        if (gUnknown_03001594 == 0)
        {
            gUnknown_03001590 = *(u16 *)((u8 *)gUnknown_030008B4 + 0x1e);
            gUnknown_03001594 = 1;
        }
        {
            register u8 *p asm("r0") = gUnknown_030008B4;
            register u16 val asm("r1");

            asm("" : "+r"(p));
            val = 0x7FFF;
            CommitSpeed(p, val);
        }
    }
    else if ((counter & 7) == 0)
    {
        if (gUnknown_03001594 == 0)
        {
            gUnknown_03001590 = *(u16 *)((u8 *)gUnknown_030008B4 + 0x1e);
            gUnknown_03001594 = 1;
        }
        {
            register u8 *p asm("r0") = gUnknown_030008B4;
            register u16 val asm("r1");

            asm("" : "+r"(p));
            val = gUnknown_03001590;
            CommitSpeed(p, val);
        }
    }

    sub_8032AF8();
    sub_803AD78(gStaticData_0817C4C8[gUnknown_030015B0]);
}

/* NAKED transcription: opens the singleton's own camera-follow/scroll-
 * velocity smoothing computation (`gUnknown_030015B4`-`030015EC`,
 * mirroring `gUnknown_03000884`'s screen position against a trig-table
 * lookup at `gStaticData_0816A820` - the same table already tied to the
 * orbiting-companion object, `sub_8030334`). Genuinely resists a plain-C
 * reconstruction: the ROM keeps four scratch values alive simultaneously
 * across this function's straight-line body (`ip`, `sb`, `r8` in
 * addition to `r4`-`r7`), the same many-high-register allocation gcc-2.9
 * difficulty already documented project-wide for functions like
 * `sub_8031604`/`sub_80372BC`. Mechanical, byte-verified transcription
 * (unified-to-divided Thumb syntax only, no logic changes). */
NAKED void sub_8032C0C(void)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sb\n\t"
    "mov r6, r8\n\t"
    "push {r6, r7}\n\t"
    "ldr r0, c0c__08032C34\n\t"
    "ldr r3, c0c__08032C38\n\t"
    "ldr r1, [r0]\n\t"
    "ldr r2, [r3]\n\t"
    "add r1, r1, r2\n\t"
    "str r1, [r0]\n\t"
    "ldr r0, c0c__08032C3C\n\t"
    "ldr r1, [r0]\n\t"
    "add r4, r0, #0\n\t"
    "cmp r1, #0\n\t"
    "bne c0c__08032C40\n\t"
    "cmp r2, #0x98\n\t"
    "bgt c0c__08032C5E\n\t"
    "add r0, r2, #1\n\t"
    "b c0c__08032C60\n\t"
    ".align 2, 0\n"
    "c0c__08032C34: .4byte gUnknown_030015BC\n"
    "c0c__08032C38: .4byte gUnknown_030015D4\n"
    "c0c__08032C3C: .4byte gUnknown_030015EC\n"
    "c0c__08032C40:\n\t"
    "cmp r1, #1\n\t"
    "bne c0c__08032C52\n\t"
    "cmp r2, #0x3f\n\t"
    "bgt c0c__08032C4C\n\t"
    "add r0, r2, #1\n\t"
    "b c0c__08032C60\n\t"
    "c0c__08032C4C:\n\t"
    "cmp r2, #0x40\n\t"
    "ble c0c__08032C62\n\t"
    "b c0c__08032C5E\n\t"
    "c0c__08032C52:\n\t"
    "cmp r2, #0x69\n\t"
    "bgt c0c__08032C5A\n\t"
    "add r0, r2, #1\n\t"
    "b c0c__08032C60\n\t"
    "c0c__08032C5A:\n\t"
    "cmp r2, #0x6a\n\t"
    "ble c0c__08032C62\n\t"
    "c0c__08032C5E:\n\t"
    "sub r0, r2, #1\n\t"
    "c0c__08032C60:\n\t"
    "str r0, [r3]\n\t"
    "c0c__08032C62:\n\t"
    "ldr r0, [r4]\n\t"
    "cmp r0, #0\n\t"
    "beq c0c__08032C6A\n\t"
    "b c0c__08032D6C\n\t"
    "c0c__08032C6A:\n\t"
    "ldr r1, c0c__08032D38\n\t"
    "ldr r0, c0c__08032D3C\n\t"
    "mov ip, r0\n\t"
    "ldr r0, [r1]\n\t"
    "mov r2, ip\n\t"
    "ldr r5, [r2]\n\t"
    "add r0, r0, r5\n\t"
    "str r0, [r1]\n\t"
    "ldr r1, c0c__08032D40\n\t"
    "ldr r3, c0c__08032D44\n\t"
    "mov sb, r3\n\t"
    "ldr r0, [r1]\n\t"
    "ldr r4, [r3]\n\t"
    "mov r8, r4\n\t"
    "add r0, r8\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, c0c__08032D48\n\t"
    "ldr r6, [r0]\n\t"
    "ldr r2, [r6, #0x1c]\n\t"
    "ldr r0, c0c__08032D4C\n\t"
    "ldr r7, [r0]\n\t"
    "ldr r1, c0c__08032D50\n\t"
    "add r0, r7, r1\n\t"
    "sub r2, r2, r0\n\t"
    "ldr r4, c0c__08032D54\n\t"
    "mov r0, #0\n\t"
    "ldrsh r3, [r4, r0]\n\t"
    "mov r1, #6\n\t"
    "ldrsh r0, [r4, r1]\n\t"
    "lsr r1, r0, #0x1f\n\t"
    "add r0, r0, r1\n\t"
    "asr r0, r0, #1\n\t"
    "add r3, r3, r0\n\t"
    "sub r2, r2, r3\n\t"
    "asr r2, r2, #0xc\n\t"
    "sub r5, r5, r2\n\t"
    "mov r2, ip\n\t"
    "str r5, [r2]\n\t"
    "ldr r2, [r6, #0x20]\n\t"
    "ldr r0, c0c__08032D58\n\t"
    "ldr r6, [r0]\n\t"
    "mov r3, #0xc0\n\t"
    "lsl r3, r3, #5\n\t"
    "add r0, r6, r3\n\t"
    "sub r2, r2, r0\n\t"
    "mov r0, #2\n\t"
    "ldrsh r3, [r4, r0]\n\t"
    "mov r1, #8\n\t"
    "ldrsh r0, [r4, r1]\n\t"
    "lsr r1, r0, #0x1f\n\t"
    "add r0, r0, r1\n\t"
    "asr r0, r0, #1\n\t"
    "add r3, r3, r0\n\t"
    "sub r2, r2, r3\n\t"
    "asr r2, r2, #0xc\n\t"
    "mov r3, r8\n\t"
    "sub r0, r3, r2\n\t"
    "mov r4, sb\n\t"
    "str r0, [r4]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #2\n\t"
    "cmp r5, r2\n\t"
    "ble c0c__08032CEA\n\t"
    "add r5, r2, #0\n\t"
    "c0c__08032CEA:\n\t"
    "mov r1, ip\n\t"
    "str r5, [r1]\n\t"
    "ldr r1, c0c__08032D5C\n\t"
    "cmp r5, r1\n\t"
    "bge c0c__08032CF6\n\t"
    "add r5, r1, #0\n\t"
    "c0c__08032CF6:\n\t"
    "mov r3, ip\n\t"
    "str r5, [r3]\n\t"
    "cmp r0, r2\n\t"
    "ble c0c__08032D00\n\t"
    "add r0, r2, #0\n\t"
    "c0c__08032D00:\n\t"
    "mov r4, sb\n\t"
    "str r0, [r4]\n\t"
    "cmp r0, r1\n\t"
    "bge c0c__08032D0A\n\t"
    "add r0, r1, #0\n\t"
    "c0c__08032D0A:\n\t"
    "mov r3, sb\n\t"
    "str r0, [r3]\n\t"
    "cmp r7, #0\n\t"
    "bgt c0c__08032D16\n\t"
    "mov r4, ip\n\t"
    "str r2, [r4]\n\t"
    "c0c__08032D16:\n\t"
    "ldr r0, c0c__08032D60\n\t"
    "cmp r7, r0\n\t"
    "ble c0c__08032D20\n\t"
    "mov r0, ip\n\t"
    "str r1, [r0]\n\t"
    "c0c__08032D20:\n\t"
    "ldr r0, c0c__08032D64\n\t"
    "cmp r6, r0\n\t"
    "bgt c0c__08032D2A\n\t"
    "mov r3, sb\n\t"
    "str r2, [r3]\n\t"
    "c0c__08032D2A:\n\t"
    "ldr r0, c0c__08032D68\n\t"
    "cmp r6, r0\n\t"
    "ble c0c__08032E04\n\t"
    "mov r4, sb\n\t"
    "str r1, [r4]\n\t"
    "b c0c__08032E04\n\t"
    ".align 2, 0\n"
    "c0c__08032D38: .4byte gUnknown_030015B4\n"
    "c0c__08032D3C: .4byte gUnknown_030015CC\n"
    "c0c__08032D40: .4byte gUnknown_030015B8\n"
    "c0c__08032D44: .4byte gUnknown_030015D0\n"
    "c0c__08032D48: .4byte gUnknown_03000884\n"
    "c0c__08032D4C: .4byte gUnknown_030015C0\n"
    "c0c__08032D50: .4byte 0xFFFFEE00\n"
    "c0c__08032D54: .4byte gStaticData_0817C4B0\n"
    "c0c__08032D58: .4byte gUnknown_030015C4\n"
    "c0c__08032D5C: .4byte 0xFFFFFE00\n"
    "c0c__08032D60: .4byte 0x000063FF\n"
    "c0c__08032D64: .4byte 0xFFFFC400\n"
    "c0c__08032D68: .4byte 0x00002BFF\n"
    "c0c__08032D6C:\n\t"
    "cmp r0, #1\n\t"
    "bne c0c__08032DB4\n\t"
    "ldr r1, c0c__08032D8C\n\t"
    "ldr r3, c0c__08032D90\n\t"
    "ldr r0, [r1]\n\t"
    "ldr r4, [r3]\n\t"
    "add r0, r0, r4\n\t"
    "str r0, [r1]\n\t"
    "ldr r2, c0c__08032D94\n\t"
    "cmp r0, r2\n\t"
    "ble c0c__08032D9C\n\t"
    "cmp r4, #0\n\t"
    "ble c0c__08032D9C\n\t"
    "ldr r0, c0c__08032D98\n\t"
    "b c0c__08032E02\n\t"
    ".align 2, 0\n"
    "c0c__08032D8C: .4byte gUnknown_030015B4\n"
    "c0c__08032D90: .4byte gUnknown_030015CC\n"
    "c0c__08032D94: .4byte 0x0000FFFF\n"
    "c0c__08032D98: .4byte 0xFFFFFC00\n"
    "c0c__08032D9C:\n\t"
    "ldr r1, [r1]\n\t"
    "ldr r0, c0c__08032DB0\n\t"
    "cmp r1, r0\n\t"
    "bgt c0c__08032E04\n\t"
    "ldr r0, [r3]\n\t"
    "cmp r0, #0\n\t"
    "bge c0c__08032E04\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #3\n\t"
    "b c0c__08032E02\n\t"
    ".align 2, 0\n"
    "c0c__08032DB0: .4byte 0xFFFF0000\n"
    "c0c__08032DB4:\n\t"
    "ldr r5, c0c__08032E68\n\t"
    "ldr r0, [r5]\n\t"
    "mov r1, #0xc0\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "str r0, [r5]\n\t"
    "mov r1, #0x80\n\t"
    "lsl r1, r1, #8\n\t"
    "cmp r0, r1\n\t"
    "ble c0c__08032DCA\n\t"
    "str r1, [r5]\n\t"
    "c0c__08032DCA:\n\t"
    "ldr r3, c0c__08032E6C\n\t"
    "ldr r4, c0c__08032E70\n\t"
    "ldr r0, c0c__08032E74\n\t"
    "ldr r0, [r0]\n\t"
    "lsl r1, r0, #4\n\t"
    "sub r1, r1, r0\n\t"
    "lsl r1, r1, #1\n\t"
    "asr r1, r1, #4\n\t"
    "mov r2, #0xff\n\t"
    "and r1, r2\n\t"
    "add r0, r1, #0\n\t"
    "add r0, #0x40\n\t"
    "and r0, r2\n\t"
    "lsl r0, r0, #1\n\t"
    "add r0, r0, r4\n\t"
    "mov r2, #0\n\t"
    "ldrsh r0, [r0, r2]\n\t"
    "ldr r2, [r5]\n\t"
    "mul r0, r2, r0\n\t"
    "asr r0, r0, #8\n\t"
    "str r0, [r3]\n\t"
    "ldr r3, c0c__08032E78\n\t"
    "lsl r1, r1, #1\n\t"
    "add r1, r1, r4\n\t"
    "mov r4, #0\n\t"
    "ldrsh r0, [r1, r4]\n\t"
    "mul r0, r2, r0\n\t"
    "asr r0, r0, #8\n\t"
    "c0c__08032E02:\n\t"
    "str r0, [r3]\n\t"
    "c0c__08032E04:\n\t"
    "ldr r0, c0c__08032E7C\n\t"
    "ldr r1, [r0]\n\t"
    "ldr r0, c0c__08032E80\n\t"
    "cmp r1, r0\n\t"
    "bgt c0c__08032E5A\n\t"
    "ldr r1, c0c__08032E84\n\t"
    "ldr r0, c0c__08032E88\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0, #0x10]\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, c0c__08032E8C\n\t"
    "mov r5, #0\n\t"
    "str r5, [r0]\n\t"
    "mov r1, #3\n\t"
    "ldr r0, c0c__08032E90\n\t"
    "str r1, [r0]\n\t"
    "ldr r0, c0c__08032E94\n\t"
    "ldr r4, [r0]\n\t"
    "str r5, [r4, #0xc]\n\t"
    "ldr r0, [r4]\n\t"
    "ldrh r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "strh r0, [r4, #0x10]\n\t"
    "strb r1, [r4, #0x12]\n\t"
    "add r0, r4, #0\n\t"
    "bl GetAnimFrameBaseOffset\n\t"
    "ldr r2, [r4, #0xc]\n\t"
    "ldr r3, [r4]\n\t"
    "lsl r1, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "lsl r1, r1, #2\n\t"
    "add r1, r1, r3\n\t"
    "mov r2, #4\n\t"
    "ldrsh r1, [r1, r2]\n\t"
    "cmp r0, r1\n\t"
    "blt c0c__08032E50\n\t"
    "str r5, [r4, #8]\n\t"
    "c0c__08032E50:\n\t"
    "ldr r0, c0c__08032E98\n\t"
    "str r5, [r0]\n\t"
    "ldr r1, c0c__08032E9C\n\t"
    "mov r0, #0xae\n\t"
    "str r0, [r1]\n\t"
    "c0c__08032E5A:\n\t"
    "pop {r3, r4}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "c0c__08032E68: .4byte gUnknown_030015F0\n"
    "c0c__08032E6C: .4byte gUnknown_030015B4\n"
    "c0c__08032E70: .4byte gStaticData_0816A820\n"
    "c0c__08032E74: .4byte gUnknown_030015F4\n"
    "c0c__08032E78: .4byte gUnknown_030015B8\n"
    "c0c__08032E7C: .4byte gUnknown_030015C8\n"
    "c0c__08032E80: .4byte 0x000027FF\n"
    "c0c__08032E84: .4byte gUnknown_030015E4\n"
    "c0c__08032E88: .4byte gUnknown_030015DC\n"
    "c0c__08032E8C: .4byte gUnknown_030015E8\n"
    "c0c__08032E90: .4byte gUnknown_030015B0\n"
    "c0c__08032E94: .4byte gUnknown_030015AC\n"
    "c0c__08032E98: .4byte gUnknown_030015EC\n"
    "c0c__08032E9C: .4byte gUnknown_030015D4\n"
    );
}

/* NAKED transcription: `sub_8032C0C`'s sibling half of the same
 * camera-follow/scroll-velocity smoothing computation - same
 * many-high-register (`ip`/`sb`/`r8`) allocation gap. Mechanical,
 * byte-verified transcription. */
NAKED void sub_8032EA0(void)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "ldr r3, ea0__08032ED4\n\t"
    "ldr r5, ea0__08032ED8\n\t"
    "ldr r0, [r3]\n\t"
    "ldr r1, [r5]\n\t"
    "add r0, r0, r1\n\t"
    "str r0, [r3]\n\t"
    "ldr r2, ea0__08032EDC\n\t"
    "ldr r7, ea0__08032EE0\n\t"
    "ldr r1, [r2]\n\t"
    "ldr r0, [r7]\n\t"
    "add r6, r1, r0\n\t"
    "str r6, [r2]\n\t"
    "ldr r2, ea0__08032EE4\n\t"
    "ldr r4, ea0__08032EE8\n\t"
    "ldr r0, [r2]\n\t"
    "ldr r1, [r4]\n\t"
    "add r0, r0, r1\n\t"
    "str r0, [r2]\n\t"
    "cmp r6, #0\n\t"
    "ble ea0__08032EF0\n\t"
    "ldr r0, ea0__08032EEC\n\t"
    "b ea0__08032EFC\n\t"
    ".align 2, 0\n"
    "ea0__08032ED4: .4byte gUnknown_030015B4\n"
    "ea0__08032ED8: .4byte gUnknown_030015CC\n"
    "ea0__08032EDC: .4byte gUnknown_030015B8\n"
    "ea0__08032EE0: .4byte gUnknown_030015D0\n"
    "ea0__08032EE4: .4byte gUnknown_030015BC\n"
    "ea0__08032EE8: .4byte gUnknown_030015D4\n"
    "ea0__08032EEC: .4byte 0xFFFFFF00\n"
    "ea0__08032EF0:\n\t"
    "cmp r6, #0\n\t"
    "bge ea0__08032EFA\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #1\n\t"
    "b ea0__08032EFC\n\t"
    "ea0__08032EFA:\n\t"
    "mov r0, #0\n\t"
    "ea0__08032EFC:\n\t"
    "str r0, [r7]\n\t"
    "ldr r0, ea0__08032F14\n\t"
    "ldr r1, [r0]\n\t"
    "add r6, r0, #0\n\t"
    "cmp r1, #3\n\t"
    "bgt ea0__08032F5C\n\t"
    "ldr r0, [r4]\n\t"
    "cmp r0, #0xad\n\t"
    "bgt ea0__08032F18\n\t"
    "add r0, #1\n\t"
    "b ea0__08032F1E\n\t"
    ".align 2, 0\n"
    "ea0__08032F14: .4byte gUnknown_030015EC\n"
    "ea0__08032F18:\n\t"
    "cmp r0, #0xae\n\t"
    "ble ea0__08032F20\n\t"
    "sub r0, #1\n\t"
    "ea0__08032F1E:\n\t"
    "str r0, [r4]\n\t"
    "ea0__08032F20:\n\t"
    "ldr r1, [r3]\n\t"
    "ldr r0, ea0__08032F34\n\t"
    "cmp r1, r0\n\t"
    "ble ea0__08032F3C\n\t"
    "ldr r0, [r5]\n\t"
    "cmp r0, #0\n\t"
    "ble ea0__08032F3C\n\t"
    "ldr r0, ea0__08032F38\n\t"
    "b ea0__08032F4E\n\t"
    ".align 2, 0\n"
    "ea0__08032F34: .4byte 0x00007FFF\n"
    "ea0__08032F38: .4byte 0xFFFFFE00\n"
    "ea0__08032F3C:\n\t"
    "ldr r1, [r3]\n\t"
    "ldr r0, ea0__08032F58\n\t"
    "cmp r1, r0\n\t"
    "bgt ea0__08032F8A\n\t"
    "ldr r0, [r5]\n\t"
    "cmp r0, #0\n\t"
    "bge ea0__08032F8A\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #2\n\t"
    "ea0__08032F4E:\n\t"
    "str r0, [r5]\n\t"
    "ldr r0, [r6]\n\t"
    "add r0, #1\n\t"
    "str r0, [r6]\n\t"
    "b ea0__08032F8A\n\t"
    ".align 2, 0\n"
    "ea0__08032F58: .4byte 0xFFFF8000\n"
    "ea0__08032F5C:\n\t"
    "ldr r1, [r4]\n\t"
    "mov r0, #0xea\n\t"
    "lsl r0, r0, #1\n\t"
    "cmp r1, r0\n\t"
    "bgt ea0__08032F6A\n\t"
    "add r0, r1, #1\n\t"
    "b ea0__08032F6C\n\t"
    "ea0__08032F6A:\n\t"
    "sub r0, r1, #1\n\t"
    "ea0__08032F6C:\n\t"
    "str r0, [r4]\n\t"
    "ldr r1, [r3]\n\t"
    "cmp r1, #0\n\t"
    "ble ea0__08032F7C\n\t"
    "ldr r0, ea0__08032F78\n\t"
    "b ea0__08032F88\n\t"
    ".align 2, 0\n"
    "ea0__08032F78: .4byte 0xFFFFFE00\n"
    "ea0__08032F7C:\n\t"
    "cmp r1, #0\n\t"
    "bge ea0__08032F86\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #2\n\t"
    "b ea0__08032F88\n\t"
    "ea0__08032F86:\n\t"
    "mov r0, #0\n\t"
    "ea0__08032F88:\n\t"
    "str r0, [r5]\n\t"
    "ea0__08032F8A:\n\t"
    "mov r8, r6\n\t"
    "ldr r0, [r6]\n\t"
    "cmp r0, #3\n\t"
    "ble ea0__08033032\n\t"
    "ldr r0, ea0__08032FFC\n\t"
    "ldr r1, [r0]\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #8\n\t"
    "cmp r1, r0\n\t"
    "ble ea0__08033032\n\t"
    "ldr r0, ea0__08033000\n\t"
    "mov r5, #0\n\t"
    "str r5, [r0]\n\t"
    "ldr r0, ea0__08033004\n\t"
    "str r5, [r0]\n\t"
    "ldr r1, ea0__08033008\n\t"
    "ldr r0, ea0__0803300C\n\t"
    "ldr r0, [r0]\n\t"
    "ldr r0, [r0, #0x10]\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, ea0__08033010\n\t"
    "str r5, [r0]\n\t"
    "mov r7, #2\n\t"
    "ldr r0, ea0__08033014\n\t"
    "str r7, [r0]\n\t"
    "ldr r0, ea0__08033018\n\t"
    "ldr r4, [r0]\n\t"
    "str r5, [r4, #0xc]\n\t"
    "ldr r0, [r4]\n\t"
    "ldrh r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "strh r0, [r4, #0x10]\n\t"
    "strb r1, [r4, #0x12]\n\t"
    "add r0, r4, #0\n\t"
    "bl GetAnimFrameBaseOffset\n\t"
    "ldr r2, [r4, #0xc]\n\t"
    "ldr r3, [r4]\n\t"
    "lsl r1, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "lsl r1, r1, #2\n\t"
    "add r1, r1, r3\n\t"
    "mov r2, #4\n\t"
    "ldrsh r1, [r1, r2]\n\t"
    "cmp r0, r1\n\t"
    "blt ea0__08032FE8\n\t"
    "str r5, [r4, #8]\n\t"
    "ea0__08032FE8:\n\t"
    "ldr r0, ea0__0803301C\n\t"
    "ldr r0, [r0]\n\t"
    "cmp r0, #2\n\t"
    "ble ea0__08033024\n\t"
    "mov r0, #1\n\t"
    "mov r1, r8\n\t"
    "str r0, [r1]\n\t"
    "ldr r1, ea0__08033020\n\t"
    "mov r0, #0x40\n\t"
    "b ea0__0803302A\n\t"
    ".align 2, 0\n"
    "ea0__08032FFC: .4byte gUnknown_030015C8\n"
    "ea0__08033000: .4byte gUnknown_030015F4\n"
    "ea0__08033004: .4byte gUnknown_030015F0\n"
    "ea0__08033008: .4byte gUnknown_030015E4\n"
    "ea0__0803300C: .4byte gUnknown_030015DC\n"
    "ea0__08033010: .4byte gUnknown_030015E8\n"
    "ea0__08033014: .4byte gUnknown_030015B0\n"
    "ea0__08033018: .4byte gUnknown_030015AC\n"
    "ea0__0803301C: .4byte gUnknown_030015F8\n"
    "ea0__08033020: .4byte gUnknown_030015D4\n"
    "ea0__08033024:\n\t"
    "str r7, [r6]\n\t"
    "ldr r1, ea0__0803303C\n\t"
    "mov r0, #0x6a\n\t"
    "ea0__0803302A:\n\t"
    "str r0, [r1]\n\t"
    "ldr r1, ea0__08033040\n\t"
    "ldr r0, ea0__08033044\n\t"
    "str r0, [r1]\n\t"
    "ea0__08033032:\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "ea0__0803303C: .4byte gUnknown_030015D4\n"
    "ea0__08033040: .4byte gUnknown_030015CC\n"
    "ea0__08033044: .4byte 0xFFFFF600\n"
    );
}

/* Patrol/oscillation driver, structurally parallel to the boss
 * cluster's own `sub_8030734` (issue #58) - a bounded oscillator on
 * `gUnknown_030015D4` (converging on 0x99) and `gUnknown_030015D0`
 * (bouncing 0-0x100), applied to the singleton's own position
 * (`gUnknown_030015BC`/`030015B8`), with a reward trigger
 * (`sub_802A4EC`/`sub_802F0DC`) once `gUnknown_030015B8` crosses
 * 0x4b00, and a DISPCNT window/mosaic-bit clear once
 * `gUnknown_030015C8` drops below 0x1500 (setting the "dead" flag
 * `gUnknown_030015FF`). */
void sub_8033048(void)
{
    if (gUnknown_030015FF == 0) {
        s32 v = gUnknown_030015D4;
        s32 d;

        if (v <= 0x98) {
            gUnknown_030015D4 = v + 1;
        } else if (v > 0x99) {
            gUnknown_030015D4 = v - 1;
        }

        d = gUnknown_030015D0;
        if (d <= 0xff) {
            gUnknown_030015D0 = d + 0x100;
        } else if (d > 0x100) {
            gUnknown_030015D0 = d - 0x100;
        }

        gUnknown_030015BC += gUnknown_030015D4;
        gUnknown_030015B8 += gUnknown_030015D0;

        if (gUnknown_030015B8 > 0x4b00) {
            sub_802A4EC();
            sub_802F0DC(gUnknown_03000884);
        }
    }

    if (gUnknown_030015C8 <= 0x14ff) {
        REG_DISPCNT &= 0xfbff;
        gUnknown_030015FF = 1;
    }
}

/* NAKED transcription: the singleton's own BG-tilemap-blit tile
 * consumer, confirmed by `docs/rom_map.md` as the same mechanics as the
 * boss cluster's `sub_8030D48` (issue #58) but on the singleton's own
 * separate global cluster (`gUnknown_030015A0`-family, not
 * `gUnknown_03001520`-family) - the same many-high-register
 * (`sl`/`sb`/`r8`) allocation gap as `sub_8032C0C`/`sub_8032EA0` above.
 * Mechanical, byte-verified transcription. */
NAKED void sub_80330FC(void *tileRow)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sl\n\t"
    "mov r6, sb\n\t"
    "mov r5, r8\n\t"
    "push {r5, r6, r7}\n\t"
    "add r5, r0, #0\n\t"
    "ldr r0, f0c__080331AC\n\t"
    "ldr r0, [r0]\n\t"
    "add r0, #0x18\n\t"
    "lsl r2, r0, #0xb\n\t"
    "ldr r7, f0c__080331B0\n\t"
    "ldr r0, [r7]\n\t"
    "mov r6, #0x20\n\t"
    "sub r1, r6, r0\n\t"
    "cmp r1, #0\n\t"
    "bge f0c__0803311E\n\t"
    "add r1, #3\n\t"
    "f0c__0803311E:\n\t"
    "asr r1, r1, #2\n\t"
    "lsl r1, r1, #1\n\t"
    "mov r0, #0xc0\n\t"
    "lsl r0, r0, #0x13\n\t"
    "add r1, r1, r0\n\t"
    "add r1, r2, r1\n\t"
    "ldr r3, f0c__080331B4\n\t"
    "ldr r4, [r3]\n\t"
    "sub r0, r6, r4\n\t"
    "lsr r2, r0, #0x1f\n\t"
    "add r0, r0, r2\n\t"
    "asr r0, r0, #1\n\t"
    "lsl r0, r0, #5\n\t"
    "add r0, #2\n\t"
    "add r6, r1, r0\n\t"
    "mov r2, #0\n\t"
    "mov sl, r3\n\t"
    "cmp r2, r4\n\t"
    "bge f0c__0803319E\n\t"
    "mov r8, r7\n\t"
    "ldr r1, f0c__080331B8\n\t"
    "mov sb, r1\n\t"
    "f0c__0803314A:\n\t"
    "mov r4, #0\n\t"
    "mov r0, r8\n\t"
    "ldr r1, [r0]\n\t"
    "lsr r0, r1, #0x1f\n\t"
    "add r1, r1, r0\n\t"
    "asr r1, r1, #1\n\t"
    "mov r0, #0x20\n\t"
    "add r0, r0, r6\n\t"
    "mov ip, r0\n\t"
    "add r7, r2, #1\n\t"
    "cmp r4, r1\n\t"
    "bge f0c__08033192\n\t"
    "mov r3, sb\n\t"
    "add r2, r6, #0\n\t"
    "f0c__08033166:\n\t"
    "ldrb r0, [r3]\n\t"
    "ldrh r6, [r5]\n\t"
    "add r1, r6, r0\n\t"
    "lsl r1, r1, #0x10\n\t"
    "lsr r1, r1, #0x10\n\t"
    "add r5, #2\n\t"
    "ldrh r6, [r5]\n\t"
    "add r0, r6, r0\n\t"
    "lsl r0, r0, #0x10\n\t"
    "add r5, #2\n\t"
    "lsr r0, r0, #8\n\t"
    "orr r1, r0\n\t"
    "strh r1, [r2]\n\t"
    "add r2, #2\n\t"
    "add r4, #1\n\t"
    "mov r1, r8\n\t"
    "ldr r0, [r1]\n\t"
    "lsr r1, r0, #0x1f\n\t"
    "add r0, r0, r1\n\t"
    "asr r0, r0, #1\n\t"
    "cmp r4, r0\n\t"
    "blt f0c__08033166\n\t"
    "f0c__08033192:\n\t"
    "mov r6, ip\n\t"
    "add r2, r7, #0\n\t"
    "mov r1, sl\n\t"
    "ldr r0, [r1]\n\t"
    "cmp r2, r0\n\t"
    "blt f0c__0803314A\n\t"
    "f0c__0803319E:\n\t"
    "pop {r3, r4, r5}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "mov sl, r5\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "f0c__080331AC: .4byte gUnknown_03001598\n"
    "f0c__080331B0: .4byte gUnknown_030015A0\n"
    "f0c__080331B4: .4byte gUnknown_030015A4\n"
    "f0c__080331B8: .4byte gUnknown_030015A8\n"
    );
}

/* NAKED transcription: the missing constructor for the whole singleton
 * system - see `docs/rom_map.md`'s "`sub_80331BC` closes a long-open
 * question" section. Caches its own incoming argument into
 * `gUnknown_030015D8`, seeds the P2-meter-shaped row/column counts
 * (`gUnknown_030015A0`/`030015A4`) from a per-level table
 * (`gStaticData_08169CE8`, same family shape as the meter-twins' own
 * tables), allocates the singleton object itself (part table
 * `gStaticData_0817C4BC`, "frame offsets" field re-using the
 * row-pointer array `gUnknown_03001600`), selects keyframe 0, stores
 * the new object into `gUnknown_030015AC` - the pointer everything else
 * in this thread reads - resets its state/anim-frame fields, fires the
 * per-frame update driver (`sub_8033604`) once, and finishes by
 * clearing the "apply now" BG2 latch and setting the lifetime counter
 * `gUnknown_030015F8 = 4` (the exact counter `sub_803388C`, issue #62,
 * decrements toward "dead"). A first plain-C attempt kept the freshly
 * allocated pointer and `&gUnknown_030015AC` in the same register
 * (collapsing the ROM's separate `r4`(address)/`r5`(allocation) split)
 * and only used one "zero" register where the ROM keeps two (`r4`
 * reused as a zero literal after the address store, plus a second,
 * independent `r6` zero for the `self+0x12` byte) - four bytes short of
 * the ROM's own build once linked. Transcribed directly instead. */
NAKED void sub_80331BC(void *arg0)
{
    asm(
    "push {r4, r5, r6, lr}\n\t"
    "ldr r1, bc__0803323C\n\t"
    "str r0, [r1]\n\t"
    "ldr r1, bc__08033240\n\t"
    "ldr r2, bc__08033244\n\t"
    "mov r3, #0\n\t"
    "ldrsh r0, [r2, r3]\n\t"
    "str r0, [r1]\n\t"
    "ldr r1, bc__08033248\n\t"
    "mov r3, #2\n\t"
    "ldrsh r0, [r2, r3]\n\t"
    "str r0, [r1]\n\t"
    "ldr r4, bc__0803324C\n\t"
    "mov r0, #0x1c\n\t"
    "mov r1, #0x80\n\t"
    "lsl r1, r1, #0x18\n\t"
    "bl mem_alloc\n\t"
    "add r5, r0, #0\n\t"
    "ldr r0, bc__08033250\n\t"
    "ldr r1, bc__08033254\n\t"
    "mov r2, #1\n\t"
    "str r0, [r5]\n\t"
    "str r1, [r5, #4]\n\t"
    "str r2, [r5, #0x18]\n\t"
    "add r0, r5, #0\n\t"
    "mov r1, #0\n\t"
    "bl sub_803B0A8\n\t"
    "str r5, [r4]\n\t"
    "mov r4, #0\n\t"
    "ldr r0, bc__08033258\n\t"
    "str r4, [r0]\n\t"
    "str r4, [r5, #0xc]\n\t"
    "ldr r0, [r5]\n\t"
    "ldrh r0, [r0]\n\t"
    "mov r6, #0\n\t"
    "strh r0, [r5, #0x10]\n\t"
    "strb r6, [r5, #0x12]\n\t"
    "add r0, r5, #0\n\t"
    "bl GetAnimFrameBaseOffset\n\t"
    "ldr r2, [r5, #0xc]\n\t"
    "ldr r3, [r5]\n\t"
    "lsl r1, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "lsl r1, r1, #2\n\t"
    "add r1, r1, r3\n\t"
    "mov r2, #4\n\t"
    "ldrsh r1, [r1, r2]\n\t"
    "cmp r0, r1\n\t"
    "blt bc__08033226\n\t"
    "str r4, [r5, #8]\n\t"
    "bc__08033226:\n\t"
    "bl sub_8033604\n\t"
    "ldr r0, bc__0803325C\n\t"
    "strb r6, [r0]\n\t"
    "ldr r1, bc__08033260\n\t"
    "mov r0, #4\n\t"
    "str r0, [r1]\n\t"
    "pop {r4, r5, r6}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "bc__0803323C: .4byte gUnknown_030015D8\n"
    "bc__08033240: .4byte gUnknown_030015A0\n"
    "bc__08033244: .4byte gStaticData_08169CE8\n"
    "bc__08033248: .4byte gUnknown_030015A4\n"
    "bc__0803324C: .4byte gUnknown_030015AC\n"
    "bc__08033250: .4byte gStaticData_0817C4BC\n"
    "bc__08033254: .4byte gUnknown_03001600\n"
    "bc__08033258: .4byte gUnknown_030015B0\n"
    "bc__0803325C: .4byte gUnknown_0300159C\n"
    "bc__08033260: .4byte gUnknown_030015F8\n"
    );
}

/* NAKED transcription: the animation-system-wired constructor/init step
 * for the same `gUnknown_030015Bx` object - resets the patrol
 * oscillator (`gUnknown_030015D4 = 0x66`), selects animation "kind" 1,
 * runs the standard anim-frame-reset sequence, seeds position from its
 * arguments, looks up the object's own trampoline record through the
 * third table family (`gStaticData_0817C460`, stride 0x28, indexed by
 * both the singleton pointer and the incoming "kind" argument -
 * `gUnknown_030015DC` caching the result for later accessors, same
 * convention as `sub_8033264`'s siblings), sets DISPCNT's window bit,
 * resets every timing/lifetime field for a fresh spawn, and finishes by
 * spawning two pairs of small effect objects (`sub_802E538` x2,
 * `sub_802E5B0`, `sub_802E57C`) positioned relative to the singleton's
 * own coordinates - the "burst spawn... clustered around the
 * singleton" `docs/rom_map.md` already documents. Not attempted as
 * plain C: six live scratch values simultaneously (`sb`, `sl`, `r8` in
 * addition to `r4`-`r7`), the same many-high-register allocation gap as
 * the rest of this file's heavier functions. Mechanical, byte-verified
 * transcription. */
NAKED void sub_8033264(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sl\n\t"
    "mov r6, sb\n\t"
    "mov r5, r8\n\t"
    "push {r5, r6, r7}\n\t"
    "mov sb, r0\n\t"
    "add r5, r1, #0\n\t"
    "add r6, r2, #0\n\t"
    "mov r8, r3\n\t"
    "ldr r1, t264__08033408\n\t"
    "mov r0, #0x66\n\t"
    "str r0, [r1]\n\t"
    "mov r7, #0\n\t"
    "ldr r0, t264__0803340C\n\t"
    "mov r1, #1\n\t"
    "str r1, [r0]\n\t"
    "ldr r2, t264__08033410\n\t"
    "ldr r4, [r2]\n\t"
    "str r7, [r4, #0xc]\n\t"
    "ldr r0, [r4]\n\t"
    "ldrh r0, [r0]\n\t"
    "strh r0, [r4, #0x10]\n\t"
    "mov r3, #0\n\t"
    "strb r3, [r4, #0x12]\n\t"
    "add r0, r4, #0\n\t"
    "bl GetAnimFrameBaseOffset\n\t"
    "ldr r2, [r4, #0xc]\n\t"
    "ldr r3, [r4]\n\t"
    "lsl r1, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "lsl r1, r1, #2\n\t"
    "add r1, r1, r3\n\t"
    "mov r2, #4\n\t"
    "ldrsh r1, [r1, r2]\n\t"
    "cmp r0, r1\n\t"
    "blt t264__080332B0\n\t"
    "str r7, [r4, #8]\n\t"
    "t264__080332B0:\n\t"
    "lsl r0, r5, #2\n\t"
    "add r0, r0, r5\n\t"
    "ldr r3, t264__08033414\n\t"
    "str r0, [r3]\n\t"
    "ldr r0, t264__08033418\n\t"
    "mov sl, r0\n\t"
    "lsl r0, r6, #1\n\t"
    "add r0, r0, r6\n\t"
    "mov r1, sl\n\t"
    "str r0, [r1]\n\t"
    "ldr r5, t264__0803341C\n\t"
    "mov r2, r8\n\t"
    "str r2, [r5]\n\t"
    "ldr r3, t264__08033420\n\t"
    "ldr r0, t264__08033424\n\t"
    "ldr r0, [r0]\n\t"
    "lsl r1, r0, #2\n\t"
    "add r1, r1, r0\n\t"
    "lsl r1, r1, #3\n\t"
    "mov r2, sb\n\t"
    "lsl r0, r2, #2\n\t"
    "add r0, sb\n\t"
    "lsl r0, r0, #3\n\t"
    "ldr r2, t264__08033428\n\t"
    "add r0, r0, r2\n\t"
    "add r1, r1, r0\n\t"
    "str r1, [r3]\n\t"
    "ldr r2, t264__0803342C\n\t"
    "ldr r0, [r1, #0xc]\n\t"
    "str r0, [r2]\n\t"
    "ldr r2, t264__08033430\n\t"
    "ldr r0, [r1]\n\t"
    "str r0, [r2]\n\t"
    "ldr r0, t264__08033434\n\t"
    "str r7, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #0x13\n\t"
    "ldrh r0, [r2]\n\t"
    "mov r3, #0x80\n\t"
    "lsl r3, r3, #3\n\t"
    "add r1, r3, #0\n\t"
    "orr r0, r1\n\t"
    "strh r0, [r2]\n\t"
    "ldr r0, t264__08033438\n\t"
    "mov r1, #1\n\t"
    "strb r1, [r0]\n\t"
    "ldr r0, t264__0803343C\n\t"
    "str r7, [r0]\n\t"
    "ldr r0, t264__08033440\n\t"
    "str r7, [r0]\n\t"
    "ldr r0, t264__08033444\n\t"
    "str r7, [r0]\n\t"
    "ldr r0, t264__08033448\n\t"
    "str r7, [r0]\n\t"
    "ldr r1, t264__0803344C\n\t"
    "mov r0, #4\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, t264__08033450\n\t"
    "strh r7, [r0]\n\t"
    "ldr r0, t264__08033454\n\t"
    "mov r2, #0\n\t"
    "strb r2, [r0]\n\t"
    "ldr r4, t264__08033458\n\t"
    "bl sub_8029B2C\n\t"
    "lsl r0, r0, #8\n\t"
    "ldr r1, [r5]\n\t"
    "sub r1, r1, r0\n\t"
    "str r1, [r4]\n\t"
    "mov r0, #0xe0\n\t"
    "lsl r0, r0, #0x11\n\t"
    "bl sub_803ADB4\n\t"
    "ldr r2, t264__0803345C\n\t"
    "ldr r3, t264__08033414\n\t"
    "ldr r1, [r3]\n\t"
    "mul r1, r0, r1\n\t"
    "asr r1, r1, #0xc\n\t"
    "str r1, [r2]\n\t"
    "ldr r2, t264__08033460\n\t"
    "mov r3, sl\n\t"
    "ldr r1, [r3]\n\t"
    "mul r0, r1, r0\n\t"
    "asr r0, r0, #0xc\n\t"
    "str r0, [r2]\n\t"
    "ldr r0, [r4]\n\t"
    "bl sub_8029E34\n\t"
    "ldr r0, t264__08033410\n\t"
    "ldr r2, [r0]\n\t"
    "ldr r3, [r2, #8]\n\t"
    "asr r3, r3, #8\n\t"
    "ldr r1, [r2, #0xc]\n\t"
    "ldr r4, [r2]\n\t"
    "lsl r0, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r4\n\t"
    "mov r1, #2\n\t"
    "ldrsh r0, [r0, r1]\n\t"
    "add r0, r0, r3\n\t"
    "ldr r1, [r2, #4]\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80330FC\n\t"
    "ldr r2, t264__08033414\n\t"
    "ldr r0, [r2]\n\t"
    "mov r3, #0x80\n\t"
    "lsl r3, r3, #6\n\t"
    "add r0, r0, r3\n\t"
    "mov r2, sl\n\t"
    "ldr r1, [r2]\n\t"
    "mov r3, #0xc0\n\t"
    "lsl r3, r3, #6\n\t"
    "add r1, r1, r3\n\t"
    "ldr r2, [r5]\n\t"
    "ldr r4, t264__08033464\n\t"
    "add r2, r2, r4\n\t"
    "bl sub_802E5B0\n\t"
    "ldr r1, t264__08033414\n\t"
    "ldr r0, [r1]\n\t"
    "mov r2, #0xf0\n\t"
    "lsl r2, r2, #5\n\t"
    "add r0, r0, r2\n\t"
    "mov r3, sl\n\t"
    "ldr r1, [r3]\n\t"
    "ldr r2, t264__08033468\n\t"
    "add r1, r1, r2\n\t"
    "ldr r2, [r5]\n\t"
    "add r2, r2, r4\n\t"
    "bl sub_802E57C\n\t"
    "ldr r3, t264__08033414\n\t"
    "ldr r0, [r3]\n\t"
    "ldr r1, t264__0803346C\n\t"
    "add r0, r0, r1\n\t"
    "mov r2, sl\n\t"
    "ldr r1, [r2]\n\t"
    "mov r4, #0xa0\n\t"
    "lsl r4, r4, #4\n\t"
    "add r1, r1, r4\n\t"
    "ldr r2, [r5]\n\t"
    "sub r2, #1\n\t"
    "mov r3, #1\n\t"
    "bl sub_802E538\n\t"
    "ldr r3, t264__08033414\n\t"
    "ldr r0, [r3]\n\t"
    "mov r1, #0x84\n\t"
    "lsl r1, r1, #8\n\t"
    "add r0, r0, r1\n\t"
    "mov r2, sl\n\t"
    "ldr r1, [r2]\n\t"
    "add r1, r1, r4\n\t"
    "ldr r2, [r5]\n\t"
    "sub r2, #1\n\t"
    "mov r3, #0\n\t"
    "bl sub_802E538\n\t"
    "bl sub_802A4F8\n\t"
    "pop {r3, r4, r5}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "mov sl, r5\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "t264__08033408: .4byte gUnknown_030015D4\n"
    "t264__0803340C: .4byte gUnknown_030015B0\n"
    "t264__08033410: .4byte gUnknown_030015AC\n"
    "t264__08033414: .4byte gUnknown_030015B4\n"
    "t264__08033418: .4byte gUnknown_030015B8\n"
    "t264__0803341C: .4byte gUnknown_030015BC\n"
    "t264__08033420: .4byte gUnknown_030015DC\n"
    "t264__08033424: .4byte gUnknown_030015D8\n"
    "t264__08033428: .4byte gStaticData_0817C460\n"
    "t264__0803342C: .4byte gUnknown_030015E4\n"
    "t264__08033430: .4byte gUnknown_030015E0\n"
    "t264__08033434: .4byte gUnknown_030015E8\n"
    "t264__08033438: .4byte gUnknown_0300159C\n"
    "t264__0803343C: .4byte gUnknown_03001598\n"
    "t264__08033440: .4byte gUnknown_030015EC\n"
    "t264__08033444: .4byte gUnknown_030015F4\n"
    "t264__08033448: .4byte gUnknown_030015F0\n"
    "t264__0803344C: .4byte gUnknown_030015F8\n"
    "t264__08033450: .4byte gUnknown_030015FC\n"
    "t264__08033454: .4byte gUnknown_030015FE\n"
    "t264__08033458: .4byte gUnknown_030015C8\n"
    "t264__0803345C: .4byte gUnknown_030015C0\n"
    "t264__08033460: .4byte gUnknown_030015C4\n"
    "t264__08033464: .4byte 0xFFFFFF00\n"
    "t264__08033468: .4byte 0xFFFFD000\n"
    "t264__0803346C: .4byte 0xFFFFBF00\n"
    );
}

/* NAKED transcription: per-frame animate+project+tile-stream update
 * driver, structurally parallel to the boss cluster's `sub_8031504`
 * (issue #58): runs the P1/P2 speed-toggle dispatcher (`sub_8032B6C`),
 * and while the singleton's animation "kind" (`gUnknown_030015B0`) is
 * active, the usual anim-frame-advance-and-clamp idiom; then
 * recomputes the projection scale (`sub_803ADB4`) and BG2-space offsets
 * (`gUnknown_030015C0`/`030015C4`) from the current position and
 * `gUnknown_030015C8`, calling `sub_8029E34` on the result; finally, if
 * the (Q8.8-truncated) frame index changed this tick, streams the new
 * tile row through `sub_80330FC` and arms the "apply now" BG2 latch
 * (`gUnknown_0300159C`). A first plain-C attempt (mirroring the anim
 * idiom already matched for `sub_8032910` et al.) produced 4 *extra*
 * bytes once linked - undetectable from the isolated compile alone,
 * only surfacing via the map-file address-shift check `docs/workflow.md`
 * describes. Transcribed directly instead. */
NAKED void sub_8033470(void)
{
    asm(
    "push {r4, r5, r6, lr}\n\t"
    "ldr r5, t470__0803352C\n\t"
    "ldr r0, [r5]\n\t"
    "ldr r0, [r0, #8]\n\t"
    "asr r6, r0, #8\n\t"
    "bl sub_8032B6C\n\t"
    "ldr r0, t470__08033530\n\t"
    "ldr r0, [r0]\n\t"
    "cmp r0, #0\n\t"
    "beq t470__08033526\n\t"
    "ldr r4, [r5]\n\t"
    "mov r0, #0x10\n\t"
    "ldrsh r1, [r4, r0]\n\t"
    "ldr r0, [r4, #8]\n\t"
    "add r0, r0, r1\n\t"
    "str r0, [r4, #8]\n\t"
    "mov r0, #0\n\t"
    "strb r0, [r4, #0x12]\n\t"
    "add r0, r4, #0\n\t"
    "bl GetAnimFrameBaseOffset\n\t"
    "ldr r2, [r4, #0xc]\n\t"
    "ldr r3, [r4]\n\t"
    "lsl r1, r2, #1\n\t"
    "add r1, r1, r2\n\t"
    "lsl r1, r1, #2\n\t"
    "add r1, r1, r3\n\t"
    "mov r3, #4\n\t"
    "ldrsh r2, [r1, r3]\n\t"
    "cmp r0, r2\n\t"
    "blt t470__080334C2\n\t"
    "mov r3, #6\n\t"
    "ldrsh r0, [r1, r3]\n\t"
    "sub r0, r2, r0\n\t"
    "lsl r0, r0, #8\n\t"
    "ldr r1, [r4, #8]\n\t"
    "sub r1, r1, r0\n\t"
    "str r1, [r4, #8]\n\t"
    "mov r0, #1\n\t"
    "strb r0, [r4, #0x12]\n\t"
    "t470__080334C2:\n\t"
    "ldr r4, t470__08033534\n\t"
    "bl sub_8029B2C\n\t"
    "ldr r1, t470__08033538\n\t"
    "lsl r0, r0, #8\n\t"
    "ldr r1, [r1]\n\t"
    "sub r1, r1, r0\n\t"
    "str r1, [r4]\n\t"
    "mov r0, #0xe0\n\t"
    "lsl r0, r0, #0x11\n\t"
    "bl sub_803ADB4\n\t"
    "ldr r2, t470__0803353C\n\t"
    "ldr r1, t470__08033540\n\t"
    "ldr r1, [r1]\n\t"
    "mul r1, r0, r1\n\t"
    "asr r1, r1, #0xc\n\t"
    "str r1, [r2]\n\t"
    "ldr r2, t470__08033544\n\t"
    "ldr r1, t470__08033548\n\t"
    "ldr r1, [r1]\n\t"
    "mul r0, r1, r0\n\t"
    "asr r0, r0, #0xc\n\t"
    "str r0, [r2]\n\t"
    "ldr r0, [r4]\n\t"
    "bl sub_8029E34\n\t"
    "ldr r3, [r5]\n\t"
    "ldr r0, [r3, #8]\n\t"
    "asr r4, r0, #8\n\t"
    "cmp r6, r4\n\t"
    "beq t470__08033526\n\t"
    "ldr r1, [r3, #0xc]\n\t"
    "ldr r2, [r3]\n\t"
    "lsl r0, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r2\n\t"
    "mov r1, #2\n\t"
    "ldrsh r0, [r0, r1]\n\t"
    "add r0, r0, r4\n\t"
    "ldr r1, [r3, #4]\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80330FC\n\t"
    "ldr r1, t470__0803354C\n\t"
    "mov r0, #1\n\t"
    "strb r0, [r1]\n\t"
    "t470__08033526:\n\t"
    "pop {r4, r5, r6}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "t470__0803352C: .4byte gUnknown_030015AC\n"
    "t470__08033530: .4byte gUnknown_030015B0\n"
    "t470__08033534: .4byte gUnknown_030015C8\n"
    "t470__08033538: .4byte gUnknown_030015BC\n"
    "t470__0803353C: .4byte gUnknown_030015C0\n"
    "t470__08033540: .4byte gUnknown_030015B4\n"
    "t470__08033544: .4byte gUnknown_030015C4\n"
    "t470__08033548: .4byte gUnknown_030015B8\n"
    "t470__0803354C: .4byte gUnknown_0300159C\n"
    );
}

/* The singleton's own BG2 affine-matrix committer, structurally
 * parallel to the boss cluster's `sub_80312C4` (issue #58) - pure
 * scale, no rotation (`BG2PB`/`BG2PC = 0`), per `docs/rom_map.md`'s
 * confirmation of this exact reuse pattern. */
void sub_8033550(void)
{
    s32 scale;
    s32 dy;
    s32 dx;

    if (gUnknown_0300159C != 0) {
        if (gUnknown_03001598 == 0) {
            REG_BG2CNT = 0x5809;
        } else {
            REG_BG2CNT = 0x5909;
        }
        gUnknown_0300159C = 0;
        gUnknown_03001598 ^= 1;
    }

    scale = sub_803ADB4(gUnknown_030015C8 << 8, 0x3c00);
    dy = gUnknown_030015C0 + sub_8029EB4();
    dx = gUnknown_030015C4 + sub_8029E98();

    REG_BG2X = 0x8000 - ((dy * scale) >> 8);
    REG_BG2Y = 0x8000 - ((dx * scale) >> 8);
    REG_BG2PA = scale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
}

/* NAKED transcription: top-level per-frame driver for the whole
 * singleton system - fired once by the constructor (`sub_80331BC`) and,
 * per `docs/rom_map.md`, confirmed as the per-frame step
 * `sub_8033048`/`sub_8033550` connect to. DMA-clears the tile just
 * before BG char block 3 and fills block 3 itself with a blank/
 * transparent tile (the exact same idiom the boss cluster's
 * `sub_8031504` uses ahead of its own meter-generator call), runs the
 * P2 VRAM fill-level meter (`sub_80336CC`), and - while the singleton's
 * animation "kind" is active - re-arms the "apply now" BG2 latch,
 * streams the current tile row through `sub_80330FC`, sets DISPCNT's
 * window/mosaic bit, and commits the BG2 affine matrix (`sub_8033550`).
 * A first plain-C attempt using the `DmaSet()` macro produced 24 bytes
 * *less* than the ROM's own build (this compiler folds the two DMA
 * setups' shared literal-pool addresses/instruction sequence more
 * aggressively than the ROM's build did) - only caught by the map-file
 * address-shift check, not the isolated compile. Transcribed directly
 * instead. */
NAKED void sub_8033604(void)
{
    asm(
    "push {r4, lr}\n\t"
    "sub sp, #4\n\t"
    "ldr r1, t604__0803369C\n\t"
    "ldr r0, t604__080336A0\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, t604__080336A4\n\t"
    "str r0, [r1, #4]\n\t"
    "ldr r0, t604__080336A8\n\t"
    "str r0, [r1, #8]\n\t"
    "ldr r0, [r1, #8]\n\t"
    "ldr r1, t604__080336AC\n\t"
    "mov r2, #0\n\t"
    "add r0, r1, #0\n\t"
    "add r0, #0x3c\n\t"
    "t604__08033620:\n\t"
    "str r2, [r0]\n\t"
    "sub r0, #4\n\t"
    "cmp r0, r1\n\t"
    "bge t604__08033620\n\t"
    "mov r1, sp\n\t"
    "ldr r2, t604__080336B0\n\t"
    "add r0, r2, #0\n\t"
    "strh r0, [r1]\n\t"
    "ldr r1, t604__0803369C\n\t"
    "mov r3, sp\n\t"
    "str r3, [r1]\n\t"
    "ldr r0, t604__080336B4\n\t"
    "str r0, [r1, #4]\n\t"
    "ldr r0, t604__080336B8\n\t"
    "str r0, [r1, #8]\n\t"
    "ldr r0, [r1, #8]\n\t"
    "bl sub_80336CC\n\t"
    "ldr r0, t604__080336BC\n\t"
    "ldr r0, [r0]\n\t"
    "cmp r0, #0\n\t"
    "beq t604__08033692\n\t"
    "ldr r1, t604__080336C0\n\t"
    "mov r0, #1\n\t"
    "strb r0, [r1]\n\t"
    "ldr r1, t604__080336C4\n\t"
    "mov r0, #0\n\t"
    "str r0, [r1]\n\t"
    "ldr r0, t604__080336C8\n\t"
    "ldr r2, [r0]\n\t"
    "ldr r3, [r2, #8]\n\t"
    "asr r3, r3, #8\n\t"
    "ldr r1, [r2, #0xc]\n\t"
    "ldr r4, [r2]\n\t"
    "lsl r0, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r4\n\t"
    "mov r1, #2\n\t"
    "ldrsh r0, [r0, r1]\n\t"
    "add r0, r0, r3\n\t"
    "ldr r1, [r2, #4]\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80330FC\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #0x13\n\t"
    "ldrh r0, [r2]\n\t"
    "mov r3, #0x80\n\t"
    "lsl r3, r3, #3\n\t"
    "add r1, r3, #0\n\t"
    "orr r0, r1\n\t"
    "strh r0, [r2]\n\t"
    "bl sub_8033550\n\t"
    "t604__08033692:\n\t"
    "add sp, #4\n\t"
    "pop {r4}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "t604__0803369C: .4byte 0x040000D4\n"
    "t604__080336A0: .4byte gStaticData_08169AE8\n"
    "t604__080336A4: .4byte 0x05000020\n"
    "t604__080336A8: .4byte 0x80000010\n"
    "t604__080336AC: .4byte 0x0600BFC0\n"
    "t604__080336B0: .4byte 0x0000FFFF\n"
    "t604__080336B4: .4byte 0x0600C000\n"
    "t604__080336B8: .4byte 0x81000800\n"
    "t604__080336BC: .4byte gUnknown_030015B0\n"
    "t604__080336C0: .4byte gUnknown_0300159C\n"
    "t604__080336C4: .4byte gUnknown_03001598\n"
    "t604__080336C8: .4byte gUnknown_030015AC\n"
    );
}

/* NAKED transcription: the P2-side VRAM fill-level meter, a
 * near-identical twin of the already-matched `sub_8031604` (issue #58,
 * `actor_part26c.c`) - same many-high-register (`sl`/`sb`/`r8`)
 * allocation gap documented there in full (the inner nibble-packing
 * loop holds four scratch values live simultaneously through its own
 * internal branches), applied to this cluster's own per-level table
 * (`gStaticData_08169AE8`) and row-pointer array (`gUnknown_03001600`).
 * Mechanical, byte-verified transcription. */
NAKED void sub_80336CC(void)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sl\n\t"
    "mov r6, sb\n\t"
    "mov r5, r8\n\t"
    "push {r5, r6, r7}\n\t"
    "sub sp, #8\n\t"
    "mov r5, #0\n\t"
    "mov r3, #0x81\n\t"
    "lsl r3, r3, #2\n\t"
    "ldr r0, t6cc__080337CC\n\t"
    "ldr r1, t6cc__080337D0\n\t"
    "ldr r2, [r0]\n\t"
    "ldr r0, [r1]\n\t"
    "mul r0, r2, r0\n\t"
    "add r0, #1\n\t"
    "lsr r0, r0, #1\n\t"
    "lsl r0, r0, #2\n\t"
    "str r0, [sp, #4]\n\t"
    "ldr r7, t6cc__080337D4\n\t"
    "ldr r4, t6cc__080337D8\n\t"
    "mov r1, sp\n\t"
    "ldr r6, t6cc__080337DC\n\t"
    "add r2, r5, #0\n\t"
    "t6cc__080336FA:\n\t"
    "add r0, r3, r4\n\t"
    "ldr r0, [r0]\n\t"
    "str r0, [r1]\n\t"
    "add r5, r5, r0\n\t"
    "add r3, #4\n\t"
    "add r0, r3, r4\n\t"
    "stm r6!, {r0}\n\t"
    "ldr r0, [sp, #4]\n\t"
    "add r3, r3, r0\n\t"
    "ldm r1!, {r0}\n\t"
    "lsl r0, r0, #5\n\t"
    "add r3, r3, r0\n\t"
    "sub r2, #1\n\t"
    "cmp r2, #0\n\t"
    "bge t6cc__080336FA\n\t"
    "mov r0, #0xff\n\t"
    "sub r0, r0, r5\n\t"
    "str r0, [r7]\n\t"
    "lsl r0, r0, #6\n\t"
    "ldr r1, t6cc__080337E0\n\t"
    "add r3, r0, r1\n\t"
    "mov r2, #0\n\t"
    "t6cc__08033726:\n\t"
    "lsl r1, r2, #2\n\t"
    "ldr r4, t6cc__080337DC\n\t"
    "add r0, r1, r4\n\t"
    "ldr r0, [r0]\n\t"
    "add r1, sp\n\t"
    "mov sb, r3\n\t"
    "ldr r3, [sp, #4]\n\t"
    "add r5, r0, r3\n\t"
    "ldr r1, [r1]\n\t"
    "mov r8, r1\n\t"
    "mov r4, #0\n\t"
    "mov ip, r4\n\t"
    "lsl r0, r1, #4\n\t"
    "add r2, #1\n\t"
    "mov sl, r2\n\t"
    "cmp ip, r0\n\t"
    "bge t6cc__080337B4\n\t"
    "mov r6, #0xf\n\t"
    "mov r7, #0x10\n\t"
    "t6cc__0803374C:\n\t"
    "ldrb r0, [r5]\n\t"
    "add r4, r6, #0\n\t"
    "and r4, r0\n\t"
    "mov r1, #0\n\t"
    "cmp r4, #0\n\t"
    "beq t6cc__0803375C\n\t"
    "add r1, r7, #0\n\t"
    "orr r1, r4\n\t"
    "t6cc__0803375C:\n\t"
    "add r4, r1, #0\n\t"
    "lsr r0, r0, #4\n\t"
    "and r0, r6\n\t"
    "add r5, #1\n\t"
    "mov r1, #0\n\t"
    "cmp r0, #0\n\t"
    "beq t6cc__0803376E\n\t"
    "add r1, r7, #0\n\t"
    "orr r1, r0\n\t"
    "t6cc__0803376E:\n\t"
    "add r0, r1, #0\n\t"
    "ldrb r3, [r5]\n\t"
    "add r1, r6, #0\n\t"
    "and r1, r3\n\t"
    "mov r2, #0\n\t"
    "cmp r1, #0\n\t"
    "beq t6cc__08033780\n\t"
    "add r2, r7, #0\n\t"
    "orr r2, r1\n\t"
    "t6cc__08033780:\n\t"
    "add r1, r2, #0\n\t"
    "lsr r3, r3, #4\n\t"
    "and r3, r6\n\t"
    "add r5, #1\n\t"
    "mov r2, #0\n\t"
    "cmp r3, #0\n\t"
    "beq t6cc__08033792\n\t"
    "add r2, r7, #0\n\t"
    "orr r2, r3\n\t"
    "t6cc__08033792:\n\t"
    "lsl r0, r0, #8\n\t"
    "orr r0, r4\n\t"
    "lsl r1, r1, #0x10\n\t"
    "orr r1, r0\n\t"
    "lsl r0, r2, #0x18\n\t"
    "orr r0, r1\n\t"
    "mov r1, sb\n\t"
    "add r1, #4\n\t"
    "mov sb, r1\n\t"
    "sub r1, #4\n\t"
    "stm r1!, {r0}\n\t"
    "mov r3, #1\n\t"
    "add ip, r3\n\t"
    "mov r4, r8\n\t"
    "lsl r0, r4, #4\n\t"
    "cmp ip, r0\n\t"
    "blt t6cc__0803374C\n\t"
    "t6cc__080337B4:\n\t"
    "mov r3, sb\n\t"
    "mov r2, sl\n\t"
    "cmp r2, #0\n\t"
    "ble t6cc__08033726\n\t"
    "add sp, #8\n\t"
    "pop {r3, r4, r5}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "mov sl, r5\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "t6cc__080337CC: .4byte gUnknown_030015A0\n"
    "t6cc__080337D0: .4byte gUnknown_030015A4\n"
    "t6cc__080337D4: .4byte gUnknown_030015A8\n"
    "t6cc__080337D8: .4byte gStaticData_08169AE8\n"
    "t6cc__080337DC: .4byte gUnknown_03001600\n"
    "t6cc__080337E0: .4byte 0x06008000\n"
    );
}

/* Destructor: frees the singleton object. */
void sub_80337E4(void)
{
    mem_free(gUnknown_030015AC);
}

/* No-op stub. */
void nullsub_34(void)
{
}

/* Trivial constant getter - always "false"/0. */
s32 sub_80337FC(void)
{
    return 0;
}

/* No-op stub. */
void nullsub_35(void)
{
}

asm(".align 2, 0");

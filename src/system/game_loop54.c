#include "core.h"
#include "orbit_part.h"

/* GitHub issue #12/#14 Phase 2 mop-up: the last 5 raw functions of the
 * still-large 24-function tail past `AddCollisionCandidate`
 * (`asm/code_3_2_17_e560_10d54.s`) - see
 * docs/matching/issue-14-0x08010d54-physics-apply.md's "Not integrated
 * this pass" section for the individual draft characterizations this
 * file finishes integrating: `CheckExtraLifePickup`, `PickUpExtraLife`, `UpdateExtraLife`,
 * `CreateExtraLife`, `SendExtraLifeToHud`. All 5 operate on the same still-unnamed
 * "part" object `src/system/game_loop52.c`/`game_loop53.c` already
 * document (the older functions use raw `u8 *` offsets; the ones
 * turned into C by the issue #15 NAKED retry use `struct orbit_part`,
 * include/orbit_part.h). This closes out
 * the entire `0x08010D54` chunk (issue #12/#14): every function between
 * `AddCollisionCandidate` and the already-matched `src/graphics/actor_part39.c`
 * (`DrawWumpa`) is now matched. */

extern void *gPlayer;
extern void *gAudioContext;
extern void *gLevelState;
extern void *gEntityFlags;
extern void *gUnknown_030012EC;
extern void *gHud;

extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *GetSpriteHitbox(void *dest, void *pt);
extern u8 AabbOverlaps(void *buf1, void *buf2);
extern void WorldToScreen(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern s32 FixedDiv(s32 arg0, s32 arg1);
extern s32 FixedMul(s32 a, s32 b);
extern s32 AddLife(void *self);
extern void ShowHudLives(void *state);
extern void *OperatorNew(s32 size);
extern struct actor *InitSpriteObj(struct actor *self);
extern void AddToPartList(void *manager, void *value);
extern void sub_8011308(void *self);
extern void UpdateExtraLifeHop(void *self);
extern void UpdateSpriteObj(struct actor *part);
extern s16 gSineTable[];
extern u8 gExtraLifeVtable[];

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15
 * NAKED retry: CreateExtraLife matches only under it, and the rest of the
 * file compiles identically under either compiler. */

/* `frame = min(0, frameCount - 1)` against the part's current animation
 * record (same clamp as game_loop53.c's copy). */
static inline void OrbitClampFrame(struct orbit_part *self)
{
    s32 frame = 0;
    s32 count = self->bank->records[self->tag].frameCount;

    if (frame >= count)
        frame = count - 1;
    self->frame = frame;
}
extern s32 rand(void);

void PickUpExtraLife(void *selfArg, u8 randomize);

/* Called from `PickUpExtraLife` below only (the "randomized-behavior"
 * family's own bounds-check gate). No existing cross-reference
 * elsewhere in the codebase. Gated: does nothing unless the orbit is
 * active (`self+0x4a != 0`) and either its phase hasn't wrapped past
 * `0x16` yet or the player (`gPlayer`) is in state
 * `+0x88 == 3` - the exact same opening gate `CheckWumpaPickup`
 * (`game_loop52.c`) already uses. Once past that gate, proceeds only
 * when flags bit 3 is clear and flags bit 2 is set (same bit-test idiom
 * as `CheckWumpaPickup`), builds both `self`'s and the player's AABB via
 * `GetSpriteHitbox` (unlike `CheckWumpaPickup`, always the "secondary" AABB
 * build for both sides - no `player+0xa == 0x13` branch here), and on
 * overlap sets flags bit 3 and calls `PickUpExtraLife(self, 0)` (the fixed,
 * non-randomized despawn-offset path). */
void CheckExtraLifePickup(void *selfArg)
{
    u8 *self = selfArg;
    u8 selfBox[16];
    u8 playerBox[16];
    u8 *player;

    if (self[0x4a] != 0 && self[0x4b] <= 0x16) {
        if (((u8 *)gPlayer)[0x88] != 3) {
            return;
        }
    }

    {
        u8 flags = self[0xc];
        u32 shifted = (u32)flags << 0x18;

        if ((shifted >> 0x1b) & 1) {
            return;
        }
        if (!((shifted >> 0x1a) & 1)) {
            return;
        }
    }

    GetSpriteHitbox(selfBox, self);
    player = gPlayer;
    GetSpriteHitbox(playerBox, player);

    if (AabbOverlaps(playerBox, selfBox)) {
        register s32 bit asm("r0") = 8;

        bit |= self[0xc];
        self[0xc] = bit;
        PickUpExtraLife(self, 0);
    }
}

/* `docs/rom_map.md`: part of "the randomized-behavior... famil[y]"
 * alongside `sub_8016048`. Plays a hit SFX, sets `self+0x3c` (a
 * timer/animation field) to `0xa0`, then either derives a randomized
 * `(dx,dy)` offset pair from `rand()` (`randomize` nonzero -
 * `self->0x49` tags which of three `rand()`-driven bands the x-offset
 * came from, `self->0x48 = 2`) or uses a fixed `(0xb400,0xc00)` offset
 * and fires `ShowHudLives(gHud)` (`self->0x48 = 1`). Either
 * way: `self->0xc |= 0x10`, `self->0x25 = 1`, then calls
 * `WorldToScreen(self, self->x>>8, self->y>>8, &outX, &outY)` and
 * re-derives `self->x`/`self->y` plus `self->0x40`/`self->0x44` (a
 * "distance to travel" pair, `-FixedDiv(newPos<<8 - offset, 0x1400)`)
 * from the results - the exact same tail shape `PickUpWumpa`/
 * `SendExtraLifeToHud`/`SendWumpaToHud` (`game_loop53.c`) all share. */
void PickUpExtraLife(void *selfArg, u8 randomize)
{
    u8 *self = selfArg;
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    asm volatile("" : "+r"(self));

    PlaySfx(gAudioContext, 7, 0x100);
    *(u16 *)(self + 0x3c) = 0xa0;

    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        self[0x49] = lowbit;
        if (lowbit) {
            if (rv & 2) {
                dx = ((rv & 0x3f) + 5) << 8;
            } else {
                dx = (0xeb - (rv & 0x3f)) << 8;
            }
        } else {
            dx = ((rv & 0x7f) + 0x24) << 8;
        }
        dy = ((rv & 0x1f) + 0x10) << 8;
        self[0x48] = 2;
    } else {
        dx = 0xb400;
        dy = 0xc00;
        self[0x48] = 1;
        ShowHudLives(gHud);
    }

    {
        register s32 mask asm("r0") = 0x10;

        mask |= self[0xc];
        self[0xc] = mask;
    }
    {
        register u8 one asm("r0") = 1;

        self[0x25] = one;
    }

    WorldToScreen(self, *(s32 *)self >> 8, *(s32 *)(self + 4) >> 8, &outX, &outY);

    newX = outX << 8;
    *(s32 *)self = newX;
    *(s32 *)(self + 0x40) = -FixedDiv(newX - dx, 0x1400);

    newY = outY << 8;
    *(s32 *)(self + 4) = newY;
    *(s32 *)(self + 0x44) = -FixedDiv(newY - dy, 0x1400);
}

/* `docs/rom_map.md`: "a bounds-checked, mode-selected object state
 * machine that self-destructs off-screen" - a rotating/orbiting
 * hazard/projectile behavior, uses the shared sine table
 * `gSineTable` this whole neighborhood references. Modes
 * 1/2 integrate `self->x`/`self->y` by `self->0x40`/`self->0x44` and,
 * on reaching an on-screen "arrival" bound (mode 1) or a wrapping
 * `self->0x3c` timer threshold (mode 2), fire a hit SFX,
 * `AddLife(gLevelState)`, set flags bit 0, and (unless
 * `self->8 == 0xffff`) set `self->8`'s bit in the
 * `gEntityFlags+0x108` collision bitmap - the same inline idiom
 * `UpdateWumpa` (`game_loop53.c`) also duplicates per mode. Any other
 * mode (0, or 3+): gated by `self->0x4a`, increments `self->0x49` or
 * `self->0x4b` (wrapping the gate off after 32 ticks). The shared tail:
 * unless `self->0x48 != 0`, either computes an orbit step via
 * `gSineTable[(self->0x49 & 0x7f)]` and `FixedMul` added
 * into `self->0x50`, storing to `self->y` (when `self->0x4a` is clear),
 * or calls `UpdateExtraLifeHop` (`game_loop52.c`'s own orbit-position updater)
 * when `self->0x4a` is set - then always tail-calls `UpdateSpriteObj`
 * (already matched, `actor_part5.c`).
 *
 *
 * old_agbcc. The `"+r"` copy in mode 1 reproduces its recomputed
 * `x + velX`. In mode 2 the timer is re-read through `self` after the
 * store and the id compared without a local, so both become the ROM's
 * reloads from memory (`ldrh` at the compare); two extra references on
 * each velocity give it r0 and the position r1 (third near-miss sweep;
 * the draft was 22 halfwords off). */
#define SET_ID_BIT(idExpr, one)                                                \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gEntityFlags;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= one << (_id - _word * 32);                                     \
    } while (0)

void UpdateExtraLife(struct orbit_part *self)
{
    u8 state = self->state;

    if (state == 1) {
        s32 x = self->base.x, vx = self->velX, y;

        self->base.x = x + vx;
        /* Hides that `vx` is unchanged, so the bound check below
         * recomputes `x + vx` as the ROM does instead of reusing the
         * stored sum. */
        asm("" : "+r"(vx));
        y = self->base.y + self->velY;
        self->base.y = y;
        if ((x + vx) >> 8 <= 0xb4 && y >> 8 <= 0xc) {
            /* Extra reference: puts velX in r3 and y in r2, as in the
             * ROM. */
            asm("" : : "r"(vx));
            PlaySfx(gAudioContext, 0xe, 0x100);
            AddLife(gLevelState);
            self->base.flags |= 1;
            if (self->base.field_08 != 0xffff)
                SET_ID_BIT(self->base.field_08, state);
        }
    } else if (state == 2) {
        s32 fire;

        /* Two extra references on each velocity: it wins r0 and the
         * position/sum r1, as in the ROM. */
        {
            s32 x = self->base.x;
            s32 v = self->velX;

            asm("" : : "r"(v));
            asm("" : : "r"(v));
            self->base.x = x + v;
        }
        {
            s32 y = self->base.y;
            s32 v = self->velY;

            asm("" : : "r"(v));
            asm("" : : "r"(v));
            self->base.y = y + v;
        }
        fire = 0;
        if (self->counter == 0) {
            s32 t = self->timer - 4;

            self->timer = t;
            if (t < 0x40)
                fire = 1;
        } else {
            s32 t;

            self->timer = self->timer + 0xc;
            t = self->timer;
            if (t > 0x1b0)
                fire = 1;
        }
        if (fire) {
            self->base.flags |= 1;
            if (self->base.field_08 != 0xffff)
                SET_ID_BIT(self->base.field_08, 1);
        }
    } else {
        if (self->mode == 0)
            self->counter++;
        else if (++self->phase > 0x1f)
            self->mode = 0;
    }

    if (self->state == 0) {
        if (self->mode == 0) {
            s32 sn = gSineTable[(self->counter & 0x7f) * 2];
            sn = FixedMul(sn, 0x280);
            self->base.y = self->anchor.y + sn;
        } else {
            UpdateExtraLifeHop(self);
        }
    }
    UpdateSpriteObj(&self->base);
}

/* `struct actor *CreateExtraLife(u16 arg0, u16 arg1, u16 arg2, s32 arg3)` -
 * spawns a part-object; extern already declared in `game_loop29.c`.
 * `arg3` is never actually read (the ROM hardcodes the field it would
 * feed - `self+0x29`/`+0x2a`/`+0x2b` - to a compile-time `0`
 * regardless), matching the extern's own always-`0` call sites.
 * Allocates a `0x54`-byte object (`OperatorNew`), re-initializes it
 * (`InitSpriteObj`), repoints `self->table` (`self+0x18`) at
 * `gExtraLifeVtable`, clears the "spawned/active" gate byte via
 * `sub_8011308` (`game_loop52.c`), stores `arg0` at `self+8` and
 * `arg1`/`arg2` (Q8-scaled) at `self+0`/`self+4`, mirrored into
 * `self+0x4c`/`self+0x50` (the orbit anchor `SetExtraLifePos`/
 * `UpdateExtraLifeHop` also use), joins the `gUnknown_030012EC`
 * `dual_array_manager` list (`AddToPartList`), derives `self+0x30` from
 * the same `table[self->0x2d]->+0x16` clamp idiom `PickUpWumpa`/
 * `SendWumpaToHud` (`game_loop53.c`) use, clears bits 0/5 of `self+0x28`,
 * and always tags `self+0x29`/`+0x2a`/`+0x2b` all `0`, returning the
 * new part.
 *
 * Under old_agbcc this is plain C: the `0` sentinel held in `r8` across
 * AddToPartList is just the `zero` local below, and the anchor copy is a
 * struct copy of the head x/y pair (`ORBIT_POS`). */
struct orbit_part *CreateExtraLife(u16 id, u16 x, u16 y, s32 unused)
{
    struct orbit_part *self;
    u8 zero;

    self = OperatorNew(0x54);
    InitSpriteObj(&self->base);
    self->base.table = gExtraLifeVtable;
    sub_8011308(self);
    zero = 0;
    self->base.field_08 = id;
    self->base.x = x << 8;
    self->base.y = y << 8;
    self->anchor = ORBIT_POS(self);
    AddToPartList(gUnknown_030012EC, self);
    OrbitClampFrame(self);
    self->flipX = 0;
    self->flipY = 0;
    self->counter = zero;
    self->mode = zero;
    self->phase = zero;
    return self;
}

/* `void SendExtraLifeToHud(void *part)` - extern already declared in
 * `game_loop29.c`; the documented "mutually exclusive alternative" is
 * `SendWumpaToHud` (`game_loop53.c`, already matched). Plays a hit SFX,
 * sets `self->0x48 = 1`, nudges `self->x -= self->0x4a<<8`, sets
 * `self->0x25 = 1`, calls `WorldToScreen(self, x>>8, y>>8, &outX, &outY)`
 * and re-derives `self->x`/`self->y` plus `self->0x40`/`self->0x44`
 * (the same `-FixedDiv(newPos<<8 - offset, 0x1400)` "distance to
 * travel" idiom `PickUpExtraLife`/`PickUpWumpa`/`SendWumpaToHud` all share),
 * with fixed `0xb400`/`0xc00` offsets on x/y respectively, then fires
 * `ShowHudLives(gHud)` - unlike `SendWumpaToHud`'s
 * `ShowHudWumpa`. Notably simpler than its `SendWumpaToHud` sibling: no
 * `self->0x3c`/`self->0x30` table-lookup-clamp setup here at all. */
void SendExtraLifeToHud(void *selfArg)
{
    u8 *self = selfArg;
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gAudioContext, 7, 0x100);
    self[0x48] = 1;
    {
        register s32 off asm("r0") = self[0x4a];
        register s32 shifted asm("r1") = off << 8;

        *(s32 *)self -= shifted;
    }
    self[0x25] = 1;

    WorldToScreen(self, *(s32 *)self >> 8, *(s32 *)(self + 4) >> 8, &outX, &outY);

    newX = outX << 8;
    *(s32 *)self = newX;
    *(s32 *)(self + 0x40) = -FixedDiv(newX - 0xb400, 0x1400);

    newY = outY << 8;
    *(s32 *)(self + 4) = newY;
    *(s32 *)(self + 0x44) = -FixedDiv(newY - 0xc00, 0x1400);

    ShowHudLives(gHud);
}

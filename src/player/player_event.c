#include "core.h"
#include "match.h"
#include "actor.h"
#include "actor_self.h"
#include "box_part.h"
#include "hud.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "crates.h"
#include "player.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #9/#10, ROM 0x0800AB9C-0x0800AC2C (details in
 * docs/matching/archive/issue-9-10-0x0800ab9c-graphics.md). Built with old_agbcc
 * (Makefile OLD_AGBCC_OBJS).
 *
 * The player object's (`struct player`, player.h) collision pass, event
 * handler and draw method. CollidePlayerWithObjects is a two-flag-gated
 * teardown/notification step guarded by the `cleared` (+0x105) latch. */

/* GetSpriteAttackBox's result and the copy of it that goes to CollidePartList. As two
 * members of one frame object, the copy is addressed as a frame offset, so
 * its by-value words load straight from sp (a separate `struct aabb`
 * local's address is kept in a callee-saved register instead). */
struct aabb_copy {
    struct aabb src;
    struct aabb copy;
};

/* Bit 1 of +0x0C: builds the object's AABB (GetSpriteAttackBox), copies it
 * (MemCopy32, a CpuSet memcpy) and hands the copy to CollidePartList by value
 * - three words in r1-r3, the fourth on the stack, which is what gives the
 * ROM's stack-argument order (6th, 7th, then the box's last word). Bit 7:
 * clears +0x108/+0x10C with the latch's 0 and fires three teardown
 * notifications (CollidePlayerWithCrates never reads its second argument; the ROM
 * still loads it). */
void CollidePlayerWithObjects(struct player *self)
{
    u32 cleared = self->cleared;

    if (cleared != 0)
        return;

    if ((self->flags.all >> 1) & 1) {
        struct aabb_copy b;
        void *manager;

        GetSpriteAttackBox(&b.src, self);
        manager = gCollidableList;
        MemCopy32(&b.copy, &b.src, sizeof(b.src));
        CollidePartList(manager, b.copy, self->dir, (struct box_part *)self);
    }

    if (self->flags.all >> 7) {
        struct collision_queue *link = &self->collisionQueue;

        link->count = cleared;
        link->posCommitted = cleared;
        CollidePlayerWithCrates(gCrateList, 3);
        CollidePartsOfClass(gTouchableList, 4);
        ResolvePlayerCollisions();
    }
}
asm(".align 2, 0");

/* GitHub issue #9/#10, dedicated deep-investigation session:
 * `DrawPlayer` (0x0800AFF4-0x0800B270), the second function in the
 * `PlayerHandleEvent`-through-`DrawPlayer` raw span `tools/report_units.py`
 * tracked as parked (`base_object=None`). `PlayerHandleEvent` (the 38-case
 * event dispatcher right before it) was moved here from
 * `asm/code_3_2_16_ac2c.s` as real C in the issue #9 raw-asm pass; see
 * its own comment below.
 *
 * `docs/rom_map.md`'s "eight more core reads" passage had already
 * flagged this function's shape from one angle ("reaches [the 28-byte-
 * record table] through a *child* object's `+0x20` field and reads a
 * third offset, byte `+0x16` this time, clamping the result into
 * `self+0x30`" - actually the *child's* own `+0x30`, not `self`'s; see
 * below) - reading the raw bytes directly confirms and extends that.
 *
 * `self` is the same wide, still-unnamed "big object" struct
 * (0x108+ bytes) referenced by raw offset throughout this ROM
 * neighborhood (`player_flags.c`/`kill_player.c`/`player_anim_room.c`
 * etc.) - `self+0xc` (flags byte), `self+0x18` (per-category
 * `{s16 offset; void *fn}` trampoline table pointer, the
 * `_call_via_r1` convention `player_anim_room.c` already established),
 * `self+0x20` (per-tag 28-byte-record table pointer,
 * `*(self+0x20) + tag*0x1c`, the exact convention `kill_player.c`
 * documents from a sibling call site), `self+0x28` bit 4 (the
 * mirror-flag bit `player_flags.c`/`ctrl.c`/`player_anim_room.c`
 * already read), `self+0x2d` (per-tag selector byte), and `self+0x8c`
 * (a `gRoomFrameCount`-relative deadline - the exact
 * `IsTimerArmed`/`SetTimer` convention `player_flags.c` names:
 * `*(u32 *)(self+0x8c) > gRoomFrameCount` means "still armed").
 * `self+0xb0` is a pointer to a single "child" companion object (the
 * same object across every use in this function); `self+0xb4` is a
 * write index into an 8-slot circular buffer of `self`'s own recent
 * `{x, y}` Q8 positions at `self+0xb8` (`struct { s32 x, y; }
 * posHistory[8]`, 8-byte stride); `self+0x38` is a one-shot flag
 * consumed at the very end.
 *
 * Read together, this is the per-frame update for a "stars orbiting a
 * dizzy head" companion effect, gated on `gLevelState+0x78`
 * (the central game-state "mode" field several other functions in
 * this doc already gate on):
 *
 * - **mode == 3** ("just got hit" / stun-entry): every ~8 frames
 *   (`gRoomFrameCount & 7 == 0`) re-rolls `gAkuAkuInvincibleFrame` to
 *   `(u16)RandRange(2) + 2 - (mirrored ? 2 : 0)` (0/1 if mirrored,
 *   2/3 otherwise - which side the effect "starts" from, based on
 *   facing). Clamps that value against the child's own hitbox/variant
 *   record's `+0x16` byte (via the child's own `+0x20`-table,
 *   `+0x2d`-tag convention) and stores the clamp into the child's own
 *   `+0x30`. Repositions the child directly next to `self`'s own
 *   *current* position (`self.x +/- 0x600` depending on the mirror
 *   flag, `self.y - 0x1300`, both Q8 - a fixed offset near the head).
 *   Toggles the child's own `+0x2d` tag between `1`/`2` on a 4-frame
 *   parity of `gRoomFrameCount` (a flicker), then fires the child's
 *   own `+0x18`-table `+0x20`/`+0x24` trampoline (the `_call_via_r1`
 *   refresh/notify convention). Also (regardless of the mode-3 gate,
 *   using `self`'s own blink deadline at `self+0x8c`) draws `self`
 *   itself via `DrawSprite(gSpriteRenderer, self)` (matched,
 *   `sprite.c` - queues `self`'s own OAM using its own Q8
 *   position) either unconditionally (mode == 3, or the deadline has
 *   expired) or, while the deadline is still armed, only on the same
 *   4-frame parity - a standard hit-invincibility blink. Once that
 *   deadline is no longer armed while mode == 3, calls
 *   `SetMaskLevel(gLevelState, 2)` (matched pattern,
 *   `action_ctrl_update.c`/`polar_aku_aku.c` - a mode-transition/"state
 *   close" call) - ends the stun state, transitioning mode 3 -> 2.
 *
 * - Unconditionally (any mode): pushes `self`'s own current `{x, y}`
 *   into the 8-slot `self+0xb8` position-history ring buffer at
 *   `self+0xb4`, advancing the index mod 8.
 *
 * - **mode == 1 or mode == 2** (ongoing idle-orbit): every ~8 frames,
 *   random-walks `gAkuAkuFollowFrame` by `RandRange(3) - 1` (-1/0/+1),
 *   clamped to `[0, 3]`. Clamps that value against the same child
 *   record's `+0x16` byte and stores it into the child's own `+0x30`
 *   (same clamp-and-store idiom as the mode-3 branch, different source
 *   counter). Reads the *oldest* surviving entry of the position-
 *   history ring buffer (the slot about to be overwritten next frame -
 *   effectively `self`'s own position from up to 8 frames ago, a
 *   fixed trailing delay) and adds a rotating offset built from the
 *   shared 256-entry sine-ish table `gSineTable` (already
 *   confirmed `extern s16 gSineTable[];`,
 *   `starfield.c`): `child.x = oldX + (table[frame & 0xff] << 4)`,
 *   `child.y = oldY + (table[(frame >> 1) & 0xff] << 3) - 0x1800`
 *   (Q8 `-24.0`) - two different angular speeds (full-speed X,
 *   half-speed Y) around a point 24 px above the trailed position, the
 *   classic elliptical "orbiting stars" motion. Sets the child's own
 *   `+0x2d` tag to `mode - 1` (`0`/`1`) and fires the same
 *   `+0x18`-table trampoline refresh.
 *
 * - Finally, if `self+0x38` is nonzero, clears bit 3 (`0x08`) of
 *   `self+0xc` - a one-shot "orbit effect (re)armed" flag consumed
 *   once.
 *
 * **Matching**: not attempted as a C reconstruction. This is the
 * exact register-pressure shape this session's own risk flag named up
 * front (`sb`/`sl`/`r8`/`ip` all simultaneously live - the ring-buffer
 * base pointers `sb`/`sl` and the saved-mode/saved-child-address
 * `r8`/`ip` all stay live across the trig-table lookups and the two
 * child-record clamp blocks) and the same shape this project has
 * already proven resistant to gcc 2.9 reconstruction on several other
 * functions this session (`PlayerAnimWouldTouchCrate`, `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`,
 * `ProbeTerrainX`/`ProbeTerrainY`) - transcribed directly as byte-exact
 * NAKED asm instead, verified instruction-for-instruction against the
 * ROM disassembly (`asm/code_3_2_16_ac2c.s`'s own former content at
 * this address) and confirmed byte-exact via the isolated
 * `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pipeline against `baserom.gba`
 * at `0x0800AFF4`-`0x0800B270` (the only differences at relocation
 * sites - `bl` calls and `.4byte` literals - which resolve correctly
 * once linked). See docs/matching/archive/issue-9-10-0x0800aff4-graphics.md
 * for the full write-up. */

struct ac2c_listener {
    u8 unk_00[0xc];
    u8 *vtable; // 0x0C
};


typedef void (*ac2c_fn3)(void *self, s32 a, s32 b, s32 c);

#define NOTIFY(self, a, b, c)                                                  \
    if (1) {                                                                   \
        struct ac2c_listener *_l = (self)->ctrl;                               \
        struct actor_method *_m = (struct actor_method *)(_l->vtable + 0x10);    \
        ((ac2c_fn3)_m->fn)((u8 *)_l + _m->thisOffset, (a), (b), (c));          \
    } else (void)0

static inline s32 Ac2cArmed(struct player *self)
{
    s32 armed = 0;

    if (self->deadline > gRoomFrameCount)
        armed = 1;
    return armed;
}

/* Event handler of the player-side object: `code` selects the event
 * (1-38). Hits (1-10) start a 90-frame invulnerability window, drop the
 * game mode by one, play two sounds, forward event 0xB to `ctrl`
 * and spawn a star burst at the child; events 29-34 play sfx 0x1F and
 * set a bit in the game's flag bytes; 26 resets the position history
 * and may call RaiseMaskLevel; the rest forward `code` to `ctrl`'s
 * method at vtable+0x10, some after clearing +0x54..+0x5C.
 *
 * Real C under old_agbcc (issue #9 raw-asm pass). The case bodies are in
 * the ROM's block order. Mode 0 sits in the else branch so it is laid
 * out last. The star-burst arguments go through locals so the pool
 * pointer is loaded after them. The ROM reloads the game mode after the
 * listener call and never uses it; only a volatile read reproduces that
 * load. */
void PlayerHandleEvent(struct player *self, s32 a, s32 code, s32 c)
{
    switch (code) {
    case 27:
        *GetCurrentLevelFlags(gLevelState) |= 1;
        PlaySfx(gAudioContext, 0x1c, 0x100);
        break;
    case 18:
        RequestRoomExit();
        ShowHudCounters(gHud);
        break;
    case 17:
        {
            struct level_state *game = gLevelState;

            if (game->timeTrial)
                FreezeLevelClock((struct level_state *)game, 100);
        }
        NOTIFY(self, a, code, c);
        ShowHudCounters(gHud);
        break;
    case 15:
        RequestBonusRound(gLevelState);
        NOTIFY(self, a, code, c);
        break;
    case 16:
        RequestGemPath(gLevelState);
        NOTIFY(self, a, code, c);
        break;
    case 28:
        if (gLevelState->maskLevel == 3)
            self->deadline = 0;
        PlaySfx(gAudioContext, 0x18, 0x100);
        StartTimeTrial(gLevelState);
        break;
    case 29:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        *GetCurrentLevelFlags(gLevelState) |= 2;
        break;
    case 30:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        *GetCurrentLevelFlags(gLevelState) |= 4;
        break;
    case 34:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags |= 2;
        break;
    case 32:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags |= 4;
        break;
    case 31:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags |= 1;
        break;
    case 33:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags |= 8;
        break;
    case 35:
    case 36:
    case 37:
    case 38:
        RequestRoomExit();
        break;
    case 26:
        if (gLevelState->maskLevel == 0) {
            struct player_pos *h = self->maskTrail;
            s32 i;

            for (i = 7; i >= 0; i--)
                *h++ = *(struct player_pos *)self;
        }
        {
            s32 mode = gLevelState->maskLevel;

            if ((mode <= 2 && gPlayer->ctrlMode != 1) || mode <= 1)
                RaiseMaskLevel(gLevelState);
        }
        if (gLevelState->maskLevel == 3)
            self->deadline = gRoomFrameCount + 1200;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        if ((self->flags.all >> 6) & 1) {
            if (!Ac2cArmed(self)) {
                struct level_state *game = gLevelState;

                if (game->maskLevel != 0) {
                    if (game->maskLevel <= 2) {
                        struct box_part *child;
                        s32 x, y, m;

                        self->deadline = gRoomFrameCount + 90;
                        SetMaskLevel(game, game->maskLevel - 1);
                        PlaySfx(gAudioContext, 0, 0x100);
                        PlaySfx(gAudioContext, 0x1b, 0x100);
                        NOTIFY(self, a, 0xb, c);
                        /* The ROM reloads the mode here and never uses it. */
                        (void)*(volatile s32 *)&gLevelState->maskLevel;
                        child = self->child;
                        x = child->x >> 8;
                        y = child->y >> 8;
                        m = child->mirrorX;
                        SpawnEffectPart(gEntitySpawner, 0x22, 3, x, y, m);
                    }
                } else {
                    AddDeath((struct level_state *)game);
                    NOTIFY(self, a, code, c);
                }
            }
        }
        break;
    case 23:
    case 24:
        self->rampY.start = 0;
        self->rampY.step = 0;
        self->rampY.target = 0;
        NOTIFY(self, a, code, c);
        break;
    case 12:
        NOTIFY(self, a, code, c);
        break;
    case 13:
    case 14:
    case 25:
        self->rampY.start = 0;
        self->rampY.step = 0;
        self->rampY.target = 0;
        NOTIFY(self, a, code, c);
        break;
    }
}

extern s32 _call_via_r1(void *addr, void *fn);

static inline s32 BlinkArmed(struct player *self)
{
    s32 armed = 0;

    if (self->deadline > gRoomFrameCount)
        armed = 1;
    return armed;
}

static inline void ClampTick(struct box_part *child, s32 v)
{
    s32 n = (*child->keyframes)[child->frame].steps;

    if (v >= n)
        v = n - 1;
    child->tick = v;
}

static inline void SetPos(s32 x, s32 y, struct box_part *child, s32 dx, s32 dy)
{
    x += dx;
    y += dy;
    child->x = x;
    child->y = y;
}

/* The orbit tail passes its two sums straight in as arguments. gcc 2.x
 * expands all of an inline call's arguments (a sum is left unforced)
 * before it copies them into the parameters. So the history addresses
 * and table reads come first, in argument order, and the shifts,
 * history loads and adds come after the `child` load, as in the ROM.
 * The same sums in locals, or with `(tbl << 4)` or `tbl * 16 + maskTrail`,
 * come out in statement order instead. */
static inline void SetChildPos(struct box_part *child, s32 x, s32 y)
{
    child->x = x;
    child->y = y;
}

static inline void SetFrame(struct box_part *child, u8 frame)
{
    child->frame = frame;
}

static inline void RefreshChild(struct box_part *child)
{
    struct part_method *m = PART_METHOD(child, 0x20);

    _call_via_r1((u8 *)child + m->thisOffset, m->fn);
}

void DrawPlayer(struct player *self)
{
    MATCH_HOLD_REG(s32, hold, r6);

    if (gLevelState->maskLevel == 3) {
        if (!(gRoomFrameCount & 7))
            /* One expression, so the store address is loaded before
             * the call; the locals keep `+ 2` from being folded into
             * the mirror term and load the mirror bit before the u16
             * mask, as in the ROM. */
            gAkuAkuInvincibleFrame = ({
                s32 r = RandRange(2);
                s32 m = self->mirror.bits.flipX;
                s32 v = (u16)r + 2;

                v - m * 2;
            });
        ClampTick(self->child, gAkuAkuInvincibleFrame);
        if (self->mirror.bits.flipX)
            SetPos(self->x, self->y, self->child, -0x600, -0x1300);
        else
            SetPos(self->x, self->y, self->child, 0x600, -0x1300);
        if (gRoomFrameCount & 4)
            SetFrame(self->child, 1);
        else
            SetFrame(self->child, 2);
        RefreshChild(self->child);
    }
    /* Hard-register hold (emits no code): r6 live across the blink
     * call keeps `self` out of r6, so it gets r7 as in the ROM. */
    MATCH_HOLD(hold);
    if (gLevelState->maskLevel == 3 || !BlinkArmed(self) || (gRoomFrameCount & 4))
        DrawSprite(gSpriteRenderer, self);
    /* End of the hold above (emits no code). */
    MATCH_USE(hold);
    {
        struct level_state *game = gLevelState;

        if (game->maskLevel == 3 && !BlinkArmed(self))
            SetMaskLevel(game, 2);
    }
    {
        s32 x = self->x;

        self->maskTrail[self->maskTrailIdx].x = x;
    }
    {
        s32 y = self->y;

        self->maskTrail[self->maskTrailIdx].y = y;
    }
    self->maskTrailIdx = (self->maskTrailIdx + 1) % 8;
    {
        s32 mode = gLevelState->maskLevel;

        if ((u32)(mode - 1) <= 1) {
            if (!(gRoomFrameCount & 7)) {
                s32 v;

                gAkuAkuFollowFrame = gAkuAkuFollowFrame + (u16)RandRange(3) - 1;
                v = gAkuAkuFollowFrame;
                if (v > 3)
                    v = 3;
                if (v < 0)
                    v = 0;
                gAkuAkuFollowFrame = v;
            }
            ClampTick(self->child, gAkuAkuFollowFrame);
            {
                s32 idx = self->maskTrailIdx;

                // clang-format off
                SetChildPos(self->child,
                            self->maskTrail[idx].x + gSineTable[gRoomFrameCount & 0xff] * 16,
                            self->maskTrail[idx].y +
                                gSineTable[(gRoomFrameCount >> 1) & 0xff] * 8 - 0x1800);
                // clang-format on
            }
            /* Hard-register hold (emits no code): r6 live here keeps
             * `&self->child` out of r6 (it goes to ip), which leaves r6
             * for `&maskTrailIdx` in global-alloc; reload then evicts it to
             * [sp], giving the ROM's single spill and sb/sl/r8 layout. */
            MATCH_HOLD(hold);
            self->child->frame = mode - 1;
            RefreshChild(self->child);
            /* End of the hold above (emits no code). */
            MATCH_USE(hold);
        }
    }
    if (self->animDone)
        self->flags.bits.hit = 0;
}

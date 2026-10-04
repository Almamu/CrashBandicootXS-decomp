#include "core.h"

/* GitHub issue #9/#10, dedicated deep-investigation session:
 * `sub_800AFF4` (0x0800AFF4-0x0800B270), the second function in the
 * `sub_800AC2C`-through-`sub_800AFF4` raw span `tools/report_units.py`
 * tracked as parked (`base_object=None`). `sub_800AC2C` (the 38-case
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
 * neighborhood (`actor_part16.c`/`actor_part79.c`/`actor_part108.c`
 * etc.) - `self+0xc` (flags byte), `self+0x18` (per-category
 * `{s16 offset; void *fn}` trampoline table pointer, the
 * `_call_via_r1` convention `actor_part108.c` already established),
 * `self+0x20` (per-tag 28-byte-record table pointer,
 * `*(self+0x20) + tag*0x1c`, the exact convention `actor_part79.c`
 * documents from a sibling call site), `self+0x28` bit 4 (the
 * mirror-flag bit `actor_part16.c`/`actor_part17.c`/`actor_part108.c`
 * already read), `self+0x2d` (per-tag selector byte), and `self+0x8c`
 * (a `gUnknown_0300082C`-relative deadline - the exact
 * `IsTimerArmed`/`SetTimer` convention `actor_part16.c` names:
 * `*(u32 *)(self+0x8c) > gUnknown_0300082C` means "still armed").
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
 *   (`gUnknown_0300082C & 7 == 0`) re-rolls `gUnknown_03000818` to
 *   `(u16)RandRange(2) + 2 - (mirrored ? 2 : 0)` (0/1 if mirrored,
 *   2/3 otherwise - which side the effect "starts" from, based on
 *   facing). Clamps that value against the child's own hitbox/variant
 *   record's `+0x16` byte (via the child's own `+0x20`-table,
 *   `+0x2d`-tag convention) and stores the clamp into the child's own
 *   `+0x30`. Repositions the child directly next to `self`'s own
 *   *current* position (`self.x +/- 0x600` depending on the mirror
 *   flag, `self.y - 0x1300`, both Q8 - a fixed offset near the head).
 *   Toggles the child's own `+0x2d` tag between `1`/`2` on a 4-frame
 *   parity of `gUnknown_0300082C` (a flicker), then fires the child's
 *   own `+0x18`-table `+0x20`/`+0x24` trampoline (the `_call_via_r1`
 *   refresh/notify convention). Also (regardless of the mode-3 gate,
 *   using `self`'s own blink deadline at `self+0x8c`) draws `self`
 *   itself via `sub_8007A84(gUnknown_030012CC, self)` (matched,
 *   `actor_part.c` - queues `self`'s own OAM using its own Q8
 *   position) either unconditionally (mode == 3, or the deadline has
 *   expired) or, while the deadline is still armed, only on the same
 *   4-frame parity - a standard hit-invincibility blink. Once that
 *   deadline is no longer armed while mode == 3, calls
 *   `SetMaskLevel(gLevelState, 2)` (matched pattern,
 *   `actor_part84.c`/`actor_part58.c` - a mode-transition/"state
 *   close" call) - ends the stun state, transitioning mode 3 -> 2.
 *
 * - Unconditionally (any mode): pushes `self`'s own current `{x, y}`
 *   into the 8-slot `self+0xb8` position-history ring buffer at
 *   `self+0xb4`, advancing the index mod 8.
 *
 * - **mode == 1 or mode == 2** (ongoing idle-orbit): every ~8 frames,
 *   random-walks `gUnknown_0300081C` by `RandRange(3) - 1` (-1/0/+1),
 *   clamped to `[0, 3]`. Clamps that value against the same child
 *   record's `+0x16` byte and stores it into the child's own `+0x30`
 *   (same clamp-and-store idiom as the mode-3 branch, different source
 *   counter). Reads the *oldest* surviving entry of the position-
 *   history ring buffer (the slot about to be overwritten next frame -
 *   effectively `self`'s own position from up to 8 frames ago, a
 *   fixed trailing delay) and adds a rotating offset built from the
 *   shared 256-entry sine-ish table `gStaticData_0816A820` (already
 *   confirmed `extern s16 gStaticData_0816A820[];`,
 *   `actor_part72.c`): `child.x = oldX + (table[frame & 0xff] << 4)`,
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
 * functions this session (`sub_800CD00`, `sub_800A178`/`sub_800A420`,
 * `sub_8026AE8`/`sub_8026A18`) - transcribed directly as byte-exact
 * NAKED asm instead, verified instruction-for-instruction against the
 * ROM disassembly (`asm/code_3_2_16_ac2c.s`'s own former content at
 * this address) and confirmed byte-exact via the isolated
 * `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pipeline against `baserom.gba`
 * at `0x0800AFF4`-`0x0800B270` (the only differences at relocation
 * sites - `bl` calls and `.4byte` literals - which resolve correctly
 * once linked). See docs/matching/issue-9-10-0x0800aff4-graphics.md
 * for the full write-up. */

#include "actor_self.h"

struct ac2c_pos {
    s32 x;
    s32 y;
};

struct ac2c_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

struct ac2c_listener {
    u8 unk_00[0xc];
    u8 *vtable;                 // 0x0C
};

struct ac2c_child {
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[0x20];
    u32 unk_28_0:4;             // 0x28
    u32 mirrorX:1;
    u32 unk_28_5:3;
};

struct ac2c_self {
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[4];
    u8 flags;                   // 0x0C - bit 6: can be hit
    u8 unk_0D[0x37];
    struct ac2c_listener *listener; // 0x44
    u8 unk_48[0xc];
    s32 unk_54;                 // 0x54
    s32 unk_58;                 // 0x58
    s32 unk_5c;                 // 0x5C
    u8 unk_60[0x2c];
    u32 deadline;               // 0x8C
    u8 unk_90[0x20];
    struct ac2c_child *child;   // 0xB0
    s32 histIdx;                // 0xB4
    struct ac2c_pos hist[8];    // 0xB8
};

struct orbit_game {
    u8 unk_00[2];
    u8 flags2;                  // 0x02
    u8 unk_03[0x75];
    s32 mode;                   // 0x78
    u8 unk_7C[0x10];
    u8 unk_8c;                  // 0x8C
};

struct ac2c_player {
    u8 unk_00[0x88];
    u8 unk_88;                  // 0x88
};

typedef void (*ac2c_fn3)(void *self, s32 a, s32 b, s32 c);

extern struct orbit_game *gLevelState;
extern void *gAudioContext;
extern struct ac2c_player *gUnknown_030012D8;
extern void *gEntitySpawner;
extern void *gHud;
extern u32 gUnknown_0300082C;
extern u8 *GetCurrentLevelFlags(void *game);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void sub_80241A4(void);
extern void ShowHudCounters(void *arg0);
extern void FreezeLevelClock(void *game, s32 n);
extern void sub_802352C(void *game);
extern void sub_8023510(void *game);
extern void sub_8022D50(void *game);
extern void RaiseMaskLevel(void *game);
extern void sub_80232E4(void *game);
extern void SetMaskLevel(void *game, s32 mode);
extern void *sub_8025BAC(void *pool, s32 a, s32 kind, s32 x, s32 y, s32 mirror);

#define NOTIFY(self, a, b, c)                                                  \
    if (1) {                                                                   \
        struct ac2c_listener *_l = (self)->listener;                           \
        struct ac2c_method *_m = (struct ac2c_method *)(_l->vtable + 0x10);    \
        ((ac2c_fn3)_m->fn)((u8 *)_l + _m->thisOffset, (a), (b), (c));          \
    } else (void)0

static inline s32 Ac2cArmed(struct ac2c_self *self)
{
    s32 armed = 0;

    if (self->deadline > gUnknown_0300082C)
        armed = 1;
    return armed;
}

/* Event handler of the player-side object: `code` selects the event
 * (1-38). Hits (1-10) start a 90-frame invulnerability window, drop the
 * game mode by one, play two sounds, forward event 0xB to `listener`
 * and spawn a star burst at the child; events 29-34 play sfx 0x1F and
 * set a bit in the game's flag bytes; 26 resets the position history
 * and may call RaiseMaskLevel; the rest forward `code` to `listener`'s
 * method at vtable+0x10, some after clearing +0x54..+0x5C.
 *
 * Real C under old_agbcc (issue #9 raw-asm pass). The case bodies are in
 * the ROM's block order. Mode 0 sits in the else branch so it is laid
 * out last. The star-burst arguments go through locals so the pool
 * pointer is loaded after them. The ROM reloads the game mode after the
 * listener call and never uses it; only a volatile read reproduces that
 * load. */
void sub_800AC2C(struct ac2c_self *self, s32 a, s32 code, s32 c)
{
    switch (code) {
    case 27:
        *GetCurrentLevelFlags(gLevelState) |= 1;
        PlaySfx(gAudioContext, 0x1c, 0x100);
        break;
    case 18:
        sub_80241A4();
        ShowHudCounters(gHud);
        break;
    case 17:
        {
            struct orbit_game *game = gLevelState;

            if (game->unk_8c)
                FreezeLevelClock(game, 100);
        }
        NOTIFY(self, a, code, c);
        ShowHudCounters(gHud);
        break;
    case 15:
        sub_802352C(gLevelState);
        NOTIFY(self, a, code, c);
        break;
    case 16:
        sub_8023510(gLevelState);
        NOTIFY(self, a, code, c);
        break;
    case 28:
        if (gLevelState->mode == 3)
            self->deadline = 0;
        PlaySfx(gAudioContext, 0x18, 0x100);
        sub_8022D50(gLevelState);
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
        gLevelState->flags2 |= 2;
        break;
    case 32:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags2 |= 4;
        break;
    case 31:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags2 |= 1;
        break;
    case 33:
        PlaySfx(gAudioContext, 0x1f, 0x100);
        gLevelState->flags2 |= 8;
        break;
    case 35:
    case 36:
    case 37:
    case 38:
        sub_80241A4();
        break;
    case 26:
        if (gLevelState->mode == 0) {
            struct ac2c_pos *h = self->hist;
            s32 i;

            for (i = 7; i >= 0; i--)
                *h++ = *(struct ac2c_pos *)self;
        }
        {
            s32 mode = gLevelState->mode;

            if ((mode <= 2 && gUnknown_030012D8->unk_88 != 1) || mode <= 1)
                RaiseMaskLevel(gLevelState);
        }
        if (gLevelState->mode == 3)
            self->deadline = gUnknown_0300082C + 1200;
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
        if ((self->flags >> 6) & 1) {
            if (!Ac2cArmed(self)) {
                struct orbit_game *game = gLevelState;

                if (game->mode != 0) {
                    if (game->mode <= 2) {
                        struct ac2c_child *child;
                        s32 x, y, m;

                        self->deadline = gUnknown_0300082C + 90;
                        SetMaskLevel(game, game->mode - 1);
                        PlaySfx(gAudioContext, 0, 0x100);
                        PlaySfx(gAudioContext, 0x1b, 0x100);
                        NOTIFY(self, a, 0xb, c);
                        /* The ROM reloads the mode here and never uses it. */
                        (void)*(volatile s32 *)&gLevelState->mode;
                        child = self->child;
                        x = child->x >> 8;
                        y = child->y >> 8;
                        m = child->mirrorX;
                        sub_8025BAC(gEntitySpawner, 0x22, 3, x, y, m);
                    }
                } else {
                    sub_80232E4(game);
                    NOTIFY(self, a, code, c);
                }
            }
        }
        break;
    case 23:
    case 24:
        self->unk_54 = 0;
        self->unk_58 = 0;
        self->unk_5c = 0;
        NOTIFY(self, a, code, c);
        break;
    case 12:
        NOTIFY(self, a, code, c);
        break;
    case 13:
    case 14:
    case 25:
        self->unk_54 = 0;
        self->unk_58 = 0;
        self->unk_5c = 0;
        NOTIFY(self, a, code, c);
        break;
    }
}

#include "box_part.h"

struct orbit_pos {
    s32 x;
    s32 y;
};

struct orbit_self {
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[4];
    u8 flags_0:3;               // 0x0C
    u8 flag3:1;                 // bit 3 cleared once +0x38 is set
    u8 flags_4:4;
    u8 unk_0D[0x1b];
    u32 unk_28_0:4;             // 0x28
    u32 mirrorX:1;
    u32 unk_28_5:3;
    u8 unk_29[0xf];
    u8 unk_38;                  // 0x38
    u8 unk_39[0x53];
    u32 blinkDeadline;          // 0x8C
    u8 unk_90[0x20];
    struct box_part *child;     // 0xB0
    s32 histIdx;                // 0xB4
    struct orbit_pos hist[8];   // 0xB8
};

extern s32 gUnknown_03000818;
extern s32 gUnknown_0300081C;
extern void *gUnknown_030012CC;
extern s16 gStaticData_0816A820[];
extern s32 RandRange(s32 max);
extern void sub_8007A84(void *self, void *part);
extern s32 _call_via_r1(void *addr, void *fn);

static inline s32 BlinkArmed(struct orbit_self *self)
{
    s32 armed = 0;

    if (self->blinkDeadline > gUnknown_0300082C)
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
 * The same sums in locals, or with `(tbl << 4)` or `tbl * 16 + hist`,
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

void sub_800AFF4(struct orbit_self *self)
{
    register s32 hold asm("r6");

    if (gLevelState->mode == 3) {
        if (!(gUnknown_0300082C & 7))
            /* One expression, so the store address is loaded before
             * the call; the locals keep `+ 2` from being folded into
             * the mirror term and load the mirror bit before the u16
             * mask, as in the ROM. */
            gUnknown_03000818 = ({
                s32 r = RandRange(2);
                s32 m = self->mirrorX;
                s32 v = (u16)r + 2;

                v - m * 2;
            });
        ClampTick(self->child, gUnknown_03000818);
        if (self->mirrorX)
            SetPos(self->x, self->y, self->child, -0x600, -0x1300);
        else
            SetPos(self->x, self->y, self->child, 0x600, -0x1300);
        if (gUnknown_0300082C & 4)
            SetFrame(self->child, 1);
        else
            SetFrame(self->child, 2);
        RefreshChild(self->child);
    }
    /* Hard-register hold (emits no code): r6 live across the blink
     * call keeps `self` out of r6, so it gets r7 as in the ROM. */
    asm("" : "=r"(hold));
    if (gLevelState->mode == 3 || !BlinkArmed(self) || (gUnknown_0300082C & 4))
        sub_8007A84(gUnknown_030012CC, self);
    /* End of the hold above (emits no code). */
    asm("" : : "r"(hold));
    {
        struct orbit_game *game = gLevelState;

        if (game->mode == 3 && !BlinkArmed(self))
            SetMaskLevel(game, 2);
    }
    {
        s32 x = self->x;

        self->hist[self->histIdx].x = x;
    }
    {
        s32 y = self->y;

        self->hist[self->histIdx].y = y;
    }
    self->histIdx = (self->histIdx + 1) % 8;
    {
        s32 mode = gLevelState->mode;

        if ((u32)(mode - 1) <= 1) {
            if (!(gUnknown_0300082C & 7)) {
                s32 v;

                gUnknown_0300081C = gUnknown_0300081C + (u16)RandRange(3) - 1;
                v = gUnknown_0300081C;
                if (v > 3)
                    v = 3;
                if (v < 0)
                    v = 0;
                gUnknown_0300081C = v;
            }
            ClampTick(self->child, gUnknown_0300081C);
            {
                s32 idx = self->histIdx;

                SetChildPos(self->child,
                            self->hist[idx].x + gStaticData_0816A820[gUnknown_0300082C & 0xff] * 16,
                            self->hist[idx].y + gStaticData_0816A820[(gUnknown_0300082C >> 1) & 0xff] * 8 - 0x1800);
            }
            /* Hard-register hold (emits no code): r6 live here keeps
             * `&self->child` out of r6 (it goes to ip), which leaves r6
             * for `&histIdx` in global-alloc; reload then evicts it to
             * [sp], giving the ROM's single spill and sb/sl/r8 layout. */
            asm("" : "=r"(hold));
            self->child->frame = mode - 1;
            RefreshChild(self->child);
            /* End of the hold above (emits no code). */
            asm("" : : "r"(hold));
        }
    }
    if (self->unk_38)
        self->flag3 = 0;
}

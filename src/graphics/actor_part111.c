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
 * `sub_803AD7C` convention `actor_part108.c` already established),
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
 * dizzy head" companion effect, gated on `gUnknown_030012C0+0x78`
 * (the central game-state "mode" field several other functions in
 * this doc already gate on):
 *
 * - **mode == 3** ("just got hit" / stun-entry): every ~8 frames
 *   (`gUnknown_0300082C & 7 == 0`) re-rolls `gUnknown_03000818` to
 *   `(u16)sub_8000E1C(2) + 2 - (mirrored ? 2 : 0)` (0/1 if mirrored,
 *   2/3 otherwise - which side the effect "starts" from, based on
 *   facing). Clamps that value against the child's own hitbox/variant
 *   record's `+0x16` byte (via the child's own `+0x20`-table,
 *   `+0x2d`-tag convention) and stores the clamp into the child's own
 *   `+0x30`. Repositions the child directly next to `self`'s own
 *   *current* position (`self.x +/- 0x600` depending on the mirror
 *   flag, `self.y - 0x1300`, both Q8 - a fixed offset near the head).
 *   Toggles the child's own `+0x2d` tag between `1`/`2` on a 4-frame
 *   parity of `gUnknown_0300082C` (a flicker), then fires the child's
 *   own `+0x18`-table `+0x20`/`+0x24` trampoline (the `sub_803AD7C`
 *   refresh/notify convention). Also (regardless of the mode-3 gate,
 *   using `self`'s own blink deadline at `self+0x8c`) draws `self`
 *   itself via `sub_8007A84(gUnknown_030012CC, self)` (matched,
 *   `actor_part.c` - queues `self`'s own OAM using its own Q8
 *   position) either unconditionally (mode == 3, or the deadline has
 *   expired) or, while the deadline is still armed, only on the same
 *   4-frame parity - a standard hit-invincibility blink. Once that
 *   deadline is no longer armed while mode == 3, calls
 *   `sub_80231EC(gUnknown_030012C0, 2)` (matched pattern,
 *   `actor_part84.c`/`actor_part58.c` - a mode-transition/"state
 *   close" call) - ends the stun state, transitioning mode 3 -> 2.
 *
 * - Unconditionally (any mode): pushes `self`'s own current `{x, y}`
 *   into the 8-slot `self+0xb8` position-history ring buffer at
 *   `self+0xb4`, advancing the index mod 8.
 *
 * - **mode == 1 or mode == 2** (ongoing idle-orbit): every ~8 frames,
 *   random-walks `gUnknown_0300081C` by `sub_8000E1C(3) - 1` (-1/0/+1),
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

ACTOR_CALL_VIA_ALIASES

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

extern struct orbit_game *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern struct ac2c_player *gUnknown_030012D8;
extern void *gUnknown_030012E4;
extern void *gUnknown_03001318;
extern u32 gUnknown_0300082C;
extern u8 *sub_8023404(void *game);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void sub_80241A4(void);
extern void sub_8028504(void *arg0);
extern void sub_8022EA8(void *game, s32 n);
extern void sub_802352C(void *game);
extern void sub_8023510(void *game);
extern void sub_8022D50(void *game);
extern void sub_8023224(void *game);
extern void sub_80232E4(void *game);
extern void sub_80231EC(void *game, s32 mode);
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
 * and may call sub_8023224; the rest forward `code` to `listener`'s
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
        *sub_8023404(gUnknown_030012C0) |= 1;
        PlaySfx(gUnknown_030012BC, 0x1c, 0x100);
        break;
    case 18:
        sub_80241A4();
        sub_8028504(gUnknown_03001318);
        break;
    case 17:
        {
            struct orbit_game *game = gUnknown_030012C0;

            if (game->unk_8c)
                sub_8022EA8(game, 100);
        }
        NOTIFY(self, a, code, c);
        sub_8028504(gUnknown_03001318);
        break;
    case 15:
        sub_802352C(gUnknown_030012C0);
        NOTIFY(self, a, code, c);
        break;
    case 16:
        sub_8023510(gUnknown_030012C0);
        NOTIFY(self, a, code, c);
        break;
    case 28:
        if (gUnknown_030012C0->mode == 3)
            self->deadline = 0;
        PlaySfx(gUnknown_030012BC, 0x18, 0x100);
        sub_8022D50(gUnknown_030012C0);
        break;
    case 29:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        *sub_8023404(gUnknown_030012C0) |= 2;
        break;
    case 30:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        *sub_8023404(gUnknown_030012C0) |= 4;
        break;
    case 34:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        gUnknown_030012C0->flags2 |= 2;
        break;
    case 32:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        gUnknown_030012C0->flags2 |= 4;
        break;
    case 31:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        gUnknown_030012C0->flags2 |= 1;
        break;
    case 33:
        PlaySfx(gUnknown_030012BC, 0x1f, 0x100);
        gUnknown_030012C0->flags2 |= 8;
        break;
    case 35:
    case 36:
    case 37:
    case 38:
        sub_80241A4();
        break;
    case 26:
        if (gUnknown_030012C0->mode == 0) {
            struct ac2c_pos *h = self->hist;
            s32 i;

            for (i = 7; i >= 0; i--)
                *h++ = *(struct ac2c_pos *)self;
        }
        {
            s32 mode = gUnknown_030012C0->mode;

            if ((mode <= 2 && gUnknown_030012D8->unk_88 != 1) || mode <= 1)
                sub_8023224(gUnknown_030012C0);
        }
        if (gUnknown_030012C0->mode == 3)
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
                struct orbit_game *game = gUnknown_030012C0;

                if (game->mode != 0) {
                    if (game->mode <= 2) {
                        struct ac2c_child *child;
                        s32 x, y, m;

                        self->deadline = gUnknown_0300082C + 90;
                        sub_80231EC(game, game->mode - 1);
                        PlaySfx(gUnknown_030012BC, 0, 0x100);
                        PlaySfx(gUnknown_030012BC, 0x1b, 0x100);
                        NOTIFY(self, a, 0xb, c);
                        /* The ROM reloads the mode here and never uses it. */
                        (void)*(volatile s32 *)&gUnknown_030012C0->mode;
                        child = self->child;
                        x = child->x >> 8;
                        y = child->y >> 8;
                        m = child->mirrorX;
                        sub_8025BAC(gUnknown_030012E4, 0x22, 3, x, y, m);
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

#if NON_MATCHING
/* C draft (issue #9-#11 NAKED retry, old_agbcc): same shape as the ROM
 * but not converged (624 bytes vs 636) - the ROM keeps `self` in r7
 * and needs only one spill slot (`&histIdx` at [sp]); this draft puts
 * `self` in r6 and spills twice. The child repositioning goes through
 * an inline whose argument order (x, y, child) gives the ROM's load
 * order. */
#include "box_part.h"

struct orbit_pos {
    s32 x;
    s32 y;
};

struct orbit_self {
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[4];
    u8 flags;                   // 0x0C - bit 3 cleared once +0x38 is set
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
extern s32 sub_8000E1C(s32 max);
extern void sub_8007A84(void *self, void *part);
extern s32 sub_803AD7C(void *addr, void *fn);

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
    child->x = x + dx;
    child->y = y + dy;
}

static inline void SetFrame(struct box_part *child, u8 frame)
{
    child->frame = frame;
}

static inline void RefreshChild(struct box_part *child)
{
    struct part_method *m = PART_METHOD(child, 0x20);

    sub_803AD7C((u8 *)child + m->thisOffset, m->fn);
}

void sub_800AFF4(struct orbit_self *self)
{
    if (gUnknown_030012C0->mode == 3) {
        if (!(gUnknown_0300082C & 7))
            gUnknown_03000818 = (u16)sub_8000E1C(2) + 2 - self->mirrorX * 2;
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
    if (gUnknown_030012C0->mode == 3 || !BlinkArmed(self) || (gUnknown_0300082C & 4))
        sub_8007A84(gUnknown_030012CC, self);
    {
        struct orbit_game *game = gUnknown_030012C0;

        if (game->mode == 3 && !BlinkArmed(self))
            sub_80231EC(game, 2);
    }
    self->hist[self->histIdx].x = self->x;
    self->hist[self->histIdx].y = self->y;
    self->histIdx = (self->histIdx + 1) % 8;
    {
        s32 mode = gUnknown_030012C0->mode;

        if ((u32)(mode - 1) <= 1) {
            if (!(gUnknown_0300082C & 7)) {
                s32 v = gUnknown_0300081C + (u16)sub_8000E1C(3) - 1;

                if (v > 3)
                    v = 3;
                if (v < 0)
                    v = 0;
                gUnknown_0300081C = v;
            }
            ClampTick(self->child, gUnknown_0300081C);
            {
                u32 t = gUnknown_0300082C;
                s32 a = gStaticData_0816A820[t & 0xff];
                s32 b = gStaticData_0816A820[(t >> 1) & 0xff];
                struct box_part *child = self->child;

                child->x = (a << 4) + self->hist[self->histIdx].x;
                child->y = (b << 3) + self->hist[self->histIdx].y + (s32)0xFFFFE800;
            }
            self->child->frame = mode - 1;
            RefreshChild(self->child);
        }
    }
    if (self->unk_38)
        self->flags &= ~8;
}
#else
NAKED void sub_800AFF4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r7, r0, #0\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "bne 7f\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #7\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x28\n\t"
        "add r1, r1, r7\n\t"
        "mov r8, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "ldr r4, 22f\n\t"
        "mov r0, #2\n\t"
        "bl sub_8000E1C\n\t"
        "mov r2, r8\n\t"
        "ldrb r2, [r2]\n\t"
        "lsl r1, r2, #0x1b\n\t"
        "lsr r1, r1, #0x1f\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r0, #2\n\t"
        "lsl r1, r1, #1\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r4]\n\t"
    "1:\n\t"
        "ldr r0, 22f\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0xb0\n\t"
        "ldr r5, [r2]\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r0, [r5, #0x20]\n\t"
        "add r3, r5, #0\n\t"
        "add r3, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r6, [r3]\n\t"
        "lsl r0, r6, #3\n\t"
        "sub r0, r0, r6\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "add r3, r2, #0\n\t"
        "cmp r4, r0\n\t"
        "blt 2f\n\t"
        "sub r4, r0, #1\n\t"
    "2:\n\t"
        "str r4, [r5, #0x30]\n\t"
        "mov r1, r8\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r7, #4]\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r4, 23f\n\t"
        "add r0, r0, r4\n\t"
        "ldr r5, 24f\n\t"
        "add r1, r1, r5\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "21: .4byte gUnknown_0300082C\n"
    "22: .4byte gUnknown_03000818\n"
    "23: .4byte 0xFFFFFA00\n"
    "24: .4byte 0xFFFFED00\n"
    "3:\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r7, #4]\n\t"
        "ldr r2, [r3]\n\t"
        "mov r6, #0xc0\n\t"
        "lsl r6, r6, #3\n\t"
        "add r0, r0, r6\n\t"
        "ldr r4, 25f\n\t"
        "add r1, r1, r4\n\t"
    "4:\n\t"
        "str r0, [r2]\n\t"
        "str r1, [r2, #4]\n\t"
        "ldr r0, 26f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #4\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "ldr r0, [r3]\n\t"
        "mov r1, #1\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "25: .4byte 0xFFFFED00\n"
    "26: .4byte gUnknown_0300082C\n"
    "5:\n\t"
        "ldr r0, [r3]\n\t"
        "mov r1, #2\n\t"
    "6:\n\t"
        "add r0, #0x2d\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r5, #0x20\n\t"
        "ldrsh r1, [r2, r5]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x24]\n\t"
        "bl sub_803AD7C\n\t"
    "7:\n\t"
        "ldr r0, 27f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "beq 9f\n\t"
        "mov r2, #0\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldr r1, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r1]\n\t"
        "cmp r0, r1\n\t"
        "bls 8f\n\t"
        "mov r2, #1\n\t"
    "8:\n\t"
        "cmp r2, #0\n\t"
        "beq 9f\n\t"
        "mov r0, #4\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "beq 10f\n\t"
    "9:\n\t"
        "ldr r0, 29f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8007A84\n\t"
    "10:\n\t"
        "ldr r0, 27f\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r0, [r3, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "bne 12f\n\t"
        "mov r4, #0\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldr r1, 28f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r2, r0\n\t"
        "bls 11f\n\t"
        "mov r4, #1\n\t"
    "11:\n\t"
        "cmp r4, #0\n\t"
        "bne 12f\n\t"
        "add r0, r3, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_80231EC\n\t"
    "12:\n\t"
        "ldr r2, [r7]\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0xb4\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r4, r7, #0\n\t"
        "add r4, #0xb8\n\t"
        "add r0, r4, r0\n\t"
        "str r2, [r0]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0xbc\n\t"
        "add r0, r3, r0\n\t"
        "str r2, [r0]\n\t"
        "ldr r5, [r1]\n\t"
        "add r2, r5, #1\n\t"
        "add r0, r2, #0\n\t"
        "str r1, [sp]\n\t"
        "mov sb, r4\n\t"
        "mov sl, r3\n\t"
        "cmp r2, #0\n\t"
        "bge 13f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #8\n\t"
    "13:\n\t"
        "asr r0, r0, #3\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r2, r0\n\t"
        "ldr r6, [sp]\n\t"
        "str r0, [r6]\n\t"
        "ldr r0, 27f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "mov r8, r0\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #1\n\t"
        "bhi 18f\n\t"
        "ldr r0, 28f\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #7\n\t"
        "and r1, r2\n\t"
        "add r4, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 16f\n\t"
        "ldr r5, 30f\n\t"
        "mov r0, #3\n\t"
        "bl sub_8000E1C\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "ldr r1, [r5]\n\t"
        "add r1, r1, r0\n\t"
        "sub r1, #1\n\t"
        "str r1, [r5]\n\t"
        "cmp r1, #3\n\t"
        "ble 14f\n\t"
        "mov r1, #3\n\t"
    "14:\n\t"
        "cmp r1, #0\n\t"
        "bge 15f\n\t"
        "mov r1, #0\n\t"
    "15:\n\t"
        "str r1, [r5]\n\t"
    "16:\n\t"
        "ldr r0, 30f\n\t"
        "mov r1, #0xb0\n\t"
        "add r1, r1, r7\n\t"
        "mov ip, r1\n\t"
        "ldr r5, [r1]\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r0, [r5, #0x20]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r6, [r2]\n\t"
        "lsl r0, r6, #3\n\t"
        "sub r0, r0, r6\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r3, r0\n\t"
        "blt 17f\n\t"
        "sub r3, r0, #1\n\t"
    "17:\n\t"
        "str r3, [r5, #0x30]\n\t"
        "ldr r0, [sp]\n\t"
        "ldr r3, [r0]\n\t"
        "lsl r3, r3, #3\n\t"
        "add sb, r3\n\t"
        "ldr r5, 31f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r4, #0xff\n\t"
        "add r0, r1, #0\n\t"
        "and r0, r4\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r5\n\t"
        "mov r6, #0\n\t"
        "ldrsh r2, [r0, r6]\n\t"
        "add r3, sl\n\t"
        "lsr r1, r1, #1\n\t"
        "and r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r5\n\t"
        "mov r4, #0\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "mov r5, ip\n\t"
        "ldr r4, [r5]\n\t"
        "lsl r2, r2, #4\n\t"
        "mov r6, sb\n\t"
        "ldr r1, [r6]\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #3\n\t"
        "ldr r1, [r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, 32f\n\t"
        "add r0, r0, r1\n\t"
        "str r2, [r4]\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, [r5]\n\t"
        "mov r1, r8\n\t"
        "sub r1, #1\n\t"
        "add r0, #0x2d\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x24]\n\t"
        "bl sub_803AD7C\n\t"
    "18:\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 19f\n\t"
        "mov r0, #9\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [r7, #0xc]\n\t"
        "and r0, r4\n\t"
        "strb r0, [r7, #0xc]\n\t"
    "19:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "27: .4byte gUnknown_030012C0\n"
    "28: .4byte gUnknown_0300082C\n"
    "29: .4byte gUnknown_030012CC\n"
    "30: .4byte gUnknown_0300081C\n"
    "31: .4byte gStaticData_0816A820\n"
    "32: .4byte 0xFFFFE800\n"
    );
}
#endif

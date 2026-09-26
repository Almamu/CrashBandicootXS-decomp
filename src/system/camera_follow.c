#include "core.h"
#include "memory.h"

/* GitHub issue #44: the `gUnknown_030012D4` camera-follow block (the
 * "generic 0x18-byte block" docs/matching/issue-37-game-loop-2375c.md
 * saw `sub_802375C`'s tail flush via `sub_8026DFC`), plus two identical
 * EWRAM `mem_free`/`mem_alloc` wrapper pairs that follow it in ROM.
 *
 * `struct camera` holds a Q8 position (`x`/`y`), a Q8 look-ahead offset
 * (`vx`/`vy`), the followed object (`target`) and a `mode`. Every
 * per-frame update eases the position a quarter of the way toward
 * `target + look-ahead` (`x += (goal - x) / 4`), and both publishers
 * (`sub_8026DFC`/`sub_8026E6C`) hand `(x - (120 << 8), y - (80 << 8))`
 * - the position offset by half the 240x160 screen - to the still-raw
 * `sub_80268D0` on `gUnknown_03001308`, which clamps it to `>= 0`,
 * converts Q8 to whole pixels, caps it at that object's own `+0x0`/`+0x4`
 * limits, and stores the result at its `+0x8`/`+0xC`. That is what makes
 * this a camera: the target ends up centered on screen, clamped to
 * the level bounds.
 *
 * - `sub_8026C90` (mode 2): `target+0x24` direction bits steer the
 *   look-ahead in 0x100 steps (bit 0/1 = +x/-x up to +0x27FF/-0x2800,
 *   bit 2/3 = -y/+y up to -0x1AAA/+0x1AA9); an axis with neither of its
 *   bits set decays back toward 0 by the same step. What writes
 *   `target+0x24` is not traced here.
 * - `sub_8026D8C` (mode 1): horizontal look-ahead grows toward -0x1276
 *   or +0x1276 depending on `target+0x28` bit 4 (the mirror flag several
 *   actor-side functions already document at that offset), vertical
 *   look-ahead fixed at -0x1000.
 * - `sub_8026DFC`: snaps straight to the target (no easing), seeding the
 *   mode-1 look-ahead at its limit (or zero for any other mode), then
 *   publishes. Called from `sub_80241BC`'s teardown/refresh pass
 *   (`game_loop9.c`) and `sub_802375C`'s shared tail (`game_loop56.c`).
 * - `sub_8026E6C`: the per-frame update, dispatching on `mode`, then
 *   publishing. Called from `sub_802400C` (`game_loop8.c`).
 *
 * Matching notes: `tx`/`ty` are pinned to r2/r3 in both easing
 * functions - left to itself this compiler gives them r3/r4 (or r4/r5)
 * and moves `cam` down into the low register the ROM uses for `tx`;
 * pinning `cam` to r4 instead breaks the shared +-0x100 tail. The
 * easing tail in `sub_8026C90` also needs an r4-pinned `cur` temp and a
 * separate `n` result so the add lands as `adds r0, r4, r0`. The empty
 * `case 3` in `sub_8026E6C` has no behavior; it reproduces the ROM's
 * switch decision tree (`cmp #2 / beq`, `bgt`, `cmp #1 / bne`), which
 * a two-case switch compiles to a flat compare chain instead. See
 * docs/matching/issue-44-camera-follow.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_26bf8.s`. */

struct camera_target
{
    s32 x;           // 0x00 - Q8
    s32 y;           // 0x04 - Q8
    u8 unk_08[0x1C]; // 0x08-0x23
    u8 dirFlags;     // 0x24 - bit 0/1 = +x/-x, bit 2/3 = -y/+y (mode 2 look-ahead)
    u8 unk_25[3];    // 0x25-0x27
    u8 flags;        // 0x28 - bit 4 is the mirror flag
};

struct camera
{
    s32 x;                        // 0x00 - Q8
    s32 y;                        // 0x04 - Q8
    s32 vx;                       // 0x08 - Q8 look-ahead
    s32 vy;                       // 0x0C - Q8 look-ahead
    struct camera_target *target; // 0x10
    s32 mode;                     // 0x14 - 1/2 select sub_8026D8C/sub_8026C90
};

extern void *gUnknown_03001308;
extern void sub_80268D0(void *self, s32 x, s32 y);

void sub_8026C90(struct camera *cam)
{
    // r2/r3 pins are load-bearing (see docs/workflow.md step 7 and the file comment)
    register s32 tx asm("r2") = cam->target->x;
    register s32 ty asm("r3") = cam->target->y;
    u8 dir = cam->target->dirFlags;

    if (dir != 0)
    {
        if ((dir & 4) && cam->vy > -0x1AAA)
            cam->vy -= 0x100;
        else if ((dir & 8) && cam->vy <= 0x1AA9)
            cam->vy += 0x100;

        if ((dir & 2) && cam->vx > -0x2800)
            cam->vx -= 0x100;
        else if ((dir & 1) && cam->vx <= 0x27FF)
            cam->vx += 0x100;

        if (!(dir & 3))
        {
            if (cam->vx > 0)
                cam->vx -= 0x100;
            else if (cam->vx < 0)
                cam->vx += 0x100;
        }

        if (!(dir & 0xC))
        {
            if (cam->vy > 0)
                cam->vy -= 0x100;
            else if (cam->vy < 0)
                cam->vy += 0x100;
        }
    }

    tx += cam->vx;
    ty += cam->vy;
    {
        // r4 pin and separate `n` are load-bearing (see the file comment)
        register s32 cur asm("r4") = cam->x;
        s32 n = cur + (tx - cur) / 4;
        cam->x = n;
        cur = cam->y;
        n = cur + (ty - cur) / 4;
        cam->y = n;
    }
}

void sub_8026D8C(struct camera *cam)
{
    // r2/r3 pins are load-bearing (see docs/workflow.md step 7 and the file comment)
    register s32 tx asm("r2") = cam->target->x;
    register s32 ty asm("r3") = cam->target->y;

    if ((cam->target->flags << 27) < 0)
    {
        if (cam->vx > -0x1276)
            cam->vx -= 0x100;
    }
    else
    {
        if (cam->vx <= 0x1275)
            cam->vx += 0x100;
    }

    cam->vy = -0x1000;
    tx += cam->vx;
    ty += cam->vy;
    cam->x += (tx - cam->x) / 4;
    cam->y += (ty - cam->y) / 4;
}

void sub_8026DFC(struct camera *cam)
{
    struct camera_target *target = cam->target;

    cam->x = target->x;
    cam->y = target->y;

    if (cam->mode == 1)
    {
        if ((target->flags << 27) < 0)
            cam->vx = -0x1276;
        else
            cam->vx = 0x1276;
        cam->vy = -0x1000;
    }
    else
    {
        cam->vx = 0;
        cam->vy = 0;
    }

    cam->x += cam->vx;
    cam->y += cam->vy;
    sub_80268D0(gUnknown_03001308, cam->x - (120 << 8), cam->y - (80 << 8));
}

void sub_8026E6C(struct camera *cam)
{
    switch (cam->mode)
    {
    case 1:
        sub_8026D8C(cam);
        break;
    case 2:
        sub_8026C90(cam);
        break;
    case 3: // no behavior - needed for the ROM's switch shape (see the file comment)
        break;
    }

    sub_80268D0(gUnknown_03001308, cam->x - (120 << 8), cam->y - (80 << 8));
}

void sub_8026EB4(u8 *ptr)
{
    mem_free(ptr);
}

u8 *sub_8026EC0(u32 size)
{
    return mem_alloc(size, MEM_HEAP_EWRAM);
}

void sub_8026ED0(u8 *ptr)
{
    mem_free(ptr);
}

u8 *sub_8026EDC(u32 size)
{
    return mem_alloc(size, MEM_HEAP_EWRAM);
}

asm(".align 2, 0");

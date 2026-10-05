#include "core.h"
#include "actor.h"

struct probe_pos {
    s32 x;
    s32 y;
};

extern s32 sub_8008278(void *posQ8, s32 kind, void *rec);
extern u8 ProbeTerrain(void *player, s32 mode, void *posInt, s32 span, void *outY);
extern void *gLevelLayers;

/* A physics/collision "step probe": makes a working copy of `self`'s
 * position (`self->x`/`self->y`), runs it through `sub_8008278`
 * (still unexamined - some kind of movement/gravity step, taking the
 * position pointer and `arg1` alongside it), converts the result from
 * Q8 fixed-point to plain integers, resets `self+0x69` (an attempt
 * counter) to 0, then probes the position via `ProbeTerrain` (also
 * still unexamined - takes `gLevelLayers`, `arg1`, the working
 * integer position, `arg2` - `*(u8 *)(arg2+4)` - and a pointer to
 * `self`'s original Q8 `y`).
 *
 * If the first probe succeeds: restores `self->y` to its original
 * value (undoing whatever `sub_8008278` mutated) and returns `1`.
 *
 * Otherwise: clears `gLevelLayers`'s `+0x2a` flag byte (saving
 * its old value) and retries the probe up to 3 more times, nudging the
 * working Y position down by `8` (Q8, i.e. `1/32` of a pixel-ish unit)
 * each attempt and incrementing `self+0x69`'s attempt counter; whether
 * a retry succeeds or all 4 attempts are exhausted, restores
 * `gLevelLayers`'s `+0x2a` byte to its saved value and returns
 * `0` either way - only the very first, un-nudged probe returning
 * success is distinguished by this function's return value.
 *
 * Real C (issue #9-#11 NAKED retry; matches under both compilers).
 * The "keeps `self+0x69`'s address in r6 for the whole retry loop and
 * reuses it for the termination test" shape is a second pointer: the
 * loop increments through a `t2 = tries` copy and tests through
 * `tries` (pinned to r6, which otherwise swaps with the saved flag
 * byte), and bumps `pos.y` through a `&pos` pointer. The first probe's
 * `u8` result is kept in a local (its 0 is what the ROM stores into
 * `+0x2a`), position and `origY` are one frame struct, and a hit inside
 * the loop restores the flag and returns on its own - that copy reloads
 * `gLevelLayers` from the literal pool, the loop-exit copy uses
 * the cached address, and cross-jumping shares their `strb`. */
#include "box_part.h"

struct probe_world {
    u8 unk_00[0x2a];
    u8 probeFlag;       // 0x2A
};

s32 sub_8009BE0(struct box_part *self, s32 mode, struct part_box *quad)
{
    struct {
        s32 x;
        s32 y;
        s32 origY;
    } f;
    u8 span = quad->w;
    register u8 *tries asm("r6");
    u8 hit;

    f.origY = self->y;
    *(struct probe_pos *)&f = *(struct probe_pos *)self;
    sub_8008278(&f, mode, quad);
    f.x >>= 8;
    f.y >>= 8;
    tries = &self->probeTries;
    *tries = 0;
    hit = ProbeTerrain(gLevelLayers, mode, &f, span, &f.origY);
    if (hit) {
        self->y = f.origY;
        return 1;
    }
    {
        u8 saved = ((struct probe_world *)gLevelLayers)->probeFlag;
        u8 *t2;
        struct probe_pos *pp;

        ((struct probe_world *)gLevelLayers)->probeFlag = 0;
        t2 = tries;
        pp = (struct probe_pos *)&f;
        do {
            (*t2)++;
            pp->y += 8;
            if (ProbeTerrain(gLevelLayers, mode, &f, span, &f.origY)) {
                ((struct probe_world *)gLevelLayers)->probeFlag = saved;
                return 0;
            }
        } while (*tries <= 2);
        ((struct probe_world *)gLevelLayers)->probeFlag = saved;
    }
    return 0;
}

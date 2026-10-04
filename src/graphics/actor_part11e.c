#include "core.h"
#include "actor_self.h"
#include "box_part.h"

typedef void (*part_method3_fn)(void *self, s32 a, s32 b, s32 c);

struct game_state {
    u8 unk_00[0x78];
    s32 maskLevel;      // 0x78 - the Aku Aku mask level (0-3)
};

extern struct game_state *gLevelState;
extern struct box_part *gPlayer;
extern void *gAudioContext;
extern s32 sub_8009FF4(struct box_part *part, struct part_aabb *box);
extern struct part_aabb sub_8007B98(struct box_part *part);
extern struct part_aabb sub_8007CF8(struct box_part *part);
extern u8 sub_8001688(struct part_aabb *a, struct part_aabb *b);
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);

/* obj->vtable[0x68](a, b, c) - the part's "hit" method. */
#define CALL_HIT(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        struct part_method *_m = PART_METHOD(obj, 0x68);                       \
        ((part_method3_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c)); \
    } else (void)0

/* `sub_8008AD8`'s twin, operating in this spatial-hash-grid cluster:
 * byte-identical collision-hit resolution logic (mode dispatch via
 * `gLevelState`, the AABB push-out via `sub_8007B98`/
 * `sub_8007CF8`/`sub_8001688`, and the "hit" method calls) - see
 * `sub_8008AD8`'s own writeup in `actor_part7.c` for the branch-by-branch
 * semantics, identical here. `list` itself is never read.
 *
 * Matches under old_agbcc with the box passed by value, the same C as
 * sub_8008AD8 (see docs/matching/issue-9-naked-retry.md). Kept in its own
 * translation unit since its ROM address, 0x080096C0, sits between
 * `sub_8009528` (`actor_part11f.c`) and `sub_8009868` (`actor_part11d.c`)
 * in ROM order. */
void sub_80096C0(struct part_list *list, struct part_aabb box, struct box_part *part)
{
    if (gLevelState->maskLevel == 3) {
        if (!sub_8009FF4(part, &box))
            return;
        CALL_HIT(part, 1, gPlayer->kind, 0);
    } else if ((part->flags2 >> 3) & 1) {
        struct part_aabb a, b;
        s32 px;

        a = sub_8007B98(gPlayer);
        b = sub_8007CF8(part);
        if (!sub_8001688(&a, &b))
            return;
        px = part->x;
        if (px < gPlayer->x) {
            gPlayer->x = px + ((b.w + a.w) << 7);
            CALL_HIT(gPlayer, 0, 0xc, 2);
        } else {
            gPlayer->x = px - ((b.w + a.w) << 7);
            CALL_HIT(gPlayer, 0, 0xc, 1);
        }
    } else {
        u8 kind;

        switch (sub_8009FF4(part, &box)) {
        case 0:
            break;
        case 1:
        {
            u8 *flags = &gPlayer->flags;
            *flags |= 8;
        }
            kind = gPlayer->kind;
            if (kind == 1) {
                if (gPlayer->unk_64 > 0) {
                    CALL_HIT(part, 1, 1, 0);
                    CALL_HIT(gPlayer, 0, 0xd, 0);
                    PlaySfx(gAudioContext, 0x21, 0x100);
                }
            } else {
                CALL_HIT(part, 1, kind, 0);
            }
            break;
        case 2:
            part->flags |= 8;
            if (gLevelState->maskLevel) {
                CALL_HIT(part, 1, 1, 0);
            }
            CALL_HIT(gPlayer, 1, part->kind, 0);
            break;
        }
    }
}
asm(".align 2, 0");

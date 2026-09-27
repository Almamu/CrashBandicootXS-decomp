#include "core.h"
#include "actor.h"

struct grid_node {
    void *data;
    struct grid_node *next;
};

struct pool_manager {
    s32 activeCount;
    s32 capacity;
    void **slotArray;
    void *nodeArray;
    struct grid_node *gridHead[256];
};

struct camera_pos {
    s32 x;
    s32 y;
};

struct viewport {
    u8 unk_00[0x10];
    struct camera_pos *camera; // 0x10
};

struct ctrl {
    u8 unk_00[8];
    s32 mode;
};

struct player {
    s32 x;
    s32 y;
    u8 unk_08[2];
    u8 kind;            // 0x0A
    u8 unk_0B[0x39];
    struct ctrl *ctrl;  // 0x44
    u8 unk_48[0x40];
    u8 state;           // 0x88
};

extern void sub_800D040(void *part);
extern void sub_80109A4(void *part, s32 mode, s32 playerX, s32 playerY);
extern struct viewport *gUnknown_03001308;
extern struct player *gUnknown_030012D8;

/* Another per-frame spatial-hash-grid pass over `manager`, scoped to
 * the same 3-bucket window `[baseIdx, baseIdx+2]` (`baseIdx` computed
 * the same way as `sub_80091D4`'s: `max(gUnknown_03001308`'s
 * sub-object's own `x >> 8`, `0)`), reading the player
 * (`gUnknown_030012D8`) rather than writing to the grid.
 *
 * If the player's `+0x88` byte is `3`: for every windowed node, calls
 * `sub_800D040(part)`.
 *
 * Otherwise: computes a dispatch value from the player (`*(void **)
 * (player+0x44) + 8`'s pointee by default; `0` if the player's `+0x88`
 * byte is `1`, further overridden to `0xd` if that byte is also `1`
 * *and* the player's `+0xa` byte is `0x13`), then for every windowed
 * node calls `sub_80109A4(part, dispatchValue, player->x, player->y)`.
 *
 * Matches under old_agbcc (see docs/matching/issue-9-naked-retry.md):
 * the "cross-branch register-role gap" this was parked for was the
 * newer compiler; the only shaping detail is reading the camera x
 * before the `>> 8`. */
void sub_8009868(struct pool_manager *m)
{
    s32 lo = gUnknown_03001308->camera->x;
    s32 i;
    struct player *p;
    u8 state;

    lo >>= 8;
    if (lo < 0)
        lo = 0;
    i = lo + 2;
    p = gUnknown_030012D8;
    state = p->state;
    if (state == 3) {
        do {
            struct grid_node *node;
            for (node = m->gridHead[i]; node != NULL; node = node->next)
                sub_800D040(node->data);
            i--;
        } while (i >= lo);
    } else {
        s32 mode = p->ctrl->mode;
        s32 px = p->x;
        s32 py = p->y;

        if (state == 1) {
            mode = 0;
            if (p->kind == 0x13)
                mode = 0xd;
        }
        do {
            struct grid_node *node;
            for (node = m->gridHead[i]; node != NULL; node = node->next)
                sub_80109A4(node->data, mode, px, py);
            i--;
        } while (i >= lo);
    }
}

asm(".align 2, 0");

#include "core.h"
#include "actor.h"
#include "hud.h"

/* NOT YET BYTE-MATCHING - see docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md for the full account;
 * compiled only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_2757c.s) is used otherwise. */
#if NON_MATCHING

extern s32 gUnknown_0300086C;
extern void *gUnknown_030012C0;
extern void sub_80270E0(struct hud_digit_part *part, s32 arg1, s32 arg2);
extern s32 sub_8023378(void *self);
extern s32 sub_8023270(void *self);
extern s32 sub_8023268(void *self);
extern s32 sub_8023260(void *self);
extern s32 sub_8037E54(s32 dividend, s32 divisor);
extern s32 sub_803AF1C(u32 a, u32 b);

struct icon_pos {
    s32 x;
    s32 y;
};

extern struct icon_pos gStaticData_08174C6C[];

/* Icon-indicator widget (lives display) - see
 * docs/matching/issue-45-hud-stat-widget-dispatcher.md. Positions and
 * clamps the primary icon slot (parts[22]) unconditionally, then a
 * second icon (parts[23]) only when sub_8023378's count exceeds 1.
 *
 * Residual gap: the ROM keeps this slot's `anim_index` byte in r7 right
 * up to using it as the `records[]` subscript - pinning it there
 * (`register u8 index asm("r7")`) reliably miscompiles regardless of
 * how the subsequent multiply-by-28/array-index step is phrased (plain
 * `records[index]`, or the manual shift-and-subtract-plus-asm-forced-add
 * idiom `sub_8027838`/`sub_802757C`'s second icon use elsewhere): the
 * compiler spills it to the stack and reads it back through a bogus
 * `mov r0, sp` / shift-mask sequence instead of the real value, on the
 * very next statement, call or no call in between. Every other register
 * in this function (self=r6, table=r5, first icon's slot=r3/frame=r4,
 * second icon's frame=r3/slot=r4/index=unpinned) does match by
 * construction; only this one r7 use resists. */
void sub_802757C(struct hud_counter *self)
{
    struct hud_digit_part *slot;
    struct actor *actor;
    s32 frame;
    struct hud_anim_record *record;

    gUnknown_0300086C = 0;

    slot = &self->parts[22];
    actor = (struct actor *)slot;
    {
        s32 x = gStaticData_08174C6C[22].x;
        s32 y = gStaticData_08174C6C[22].y;
        actor->x = x << 8;
        actor->y = y << 8;
    }

    frame = 0;
    {
        /* Matches sub_8027138/sub_802763C's own raw form for this same
         * per-slot byte field (`self + 0x5AD` rather than
         * `slot->anim_index`) - see docs/workflow.md step 7. anim_data
         * is read before the index byte, matching the ROM's order. */
        struct hud_anim_data *anim_data = slot->anim_data;
        u8 index = *((u8 *)self + 0x5ad);
        record = &anim_data->records[index];
    }
    if (frame >= record->frame_count) {
        frame = record->frame_count - 1;
    }
    slot->frame_index = frame;

    sub_80270E0(slot, 0, 0);

    {
        register s32 frame2 asm("r3");
        frame2 = sub_8023378(gUnknown_030012C0);

        if (frame2 > 0) {
            register struct hud_digit_part *slot2 asm("r4");

            slot2 = &self->parts[23];
            {
                s32 x = gStaticData_08174C6C[23].x;
                s32 y = gStaticData_08174C6C[23].y;
                ((struct actor *)slot2)->x = x << 8;
                ((struct actor *)slot2)->y = y << 8;
            }

            frame2 -= 1;
            {
                struct hud_anim_data *anim_data = slot2->anim_data;
                u8 index = *((u8 *)self + 0x5ed);
                record = &anim_data->records[index];
            }
            if (frame2 >= record->frame_count) {
                frame2 = record->frame_count - 1;
            }
            slot2->frame_index = frame2;

            sub_80270E0(slot2, 0, 0);
        }
    }
}

/* Three more digit/icon widgets, gated by their own change-detection
 * caches (`sync_value_a`/`b`/`c`, `include/hud.h`) against
 * `sub_8023270`/`sub_8023268`/`sub_8023260`. The first two split their
 * value into tens/ones digits (`sub_8037E54`/`sub_803AF1C`, div/mod by
 * 10) across a slot pair each (14/15, 17/18); the third does not split
 * at all - slot 20 gets the raw value as its desired frame, slot 21
 * always gets a fixed desired frame of 0 (a single-frame icon, not a
 * digit). All six (plus slots 16/19 from sub_8027138's own setup) get
 * redrawn unconditionally afterward via sub_80270E0 - slot 21 appears
 * twice in that list, matching the ROM exactly. */
static void hud_clamp_frame_index(struct hud_digit_part *part, s32 desired)
{
    struct hud_anim_record *record = &part->anim_data->records[part->anim_index];
    if (desired >= record->frame_count) {
        desired = record->frame_count - 1;
    }
    part->frame_index = desired;
}

void sub_802763C(struct hud_counter *self)
{
    gUnknown_0300086C = 0;

    if (self->sync_value_a != sub_8023270(gUnknown_030012C0)) {
        s32 value = sub_8023270(gUnknown_030012C0);
        self->sync_value_a = value;
        hud_clamp_frame_index(&self->parts[14], sub_8037E54(value, 10));
        hud_clamp_frame_index(&self->parts[15], sub_803AF1C(value, 10));
    }

    if (self->sync_value_b != sub_8023268(gUnknown_030012C0)) {
        s32 value = sub_8023268(gUnknown_030012C0);
        self->sync_value_b = value;
        hud_clamp_frame_index(&self->parts[17], sub_8037E54(value, 10));
        hud_clamp_frame_index(&self->parts[18], sub_803AF1C(value, 10));
    }

    if (self->sync_value_c != sub_8023260(gUnknown_030012C0)) {
        s32 value = sub_8023260(gUnknown_030012C0);
        self->sync_value_c = value;
        hud_clamp_frame_index(&self->parts[20], value);
        hud_clamp_frame_index(&self->parts[21], 0);
    }

    sub_80270E0(&self->parts[14], 0, 0);
    sub_80270E0(&self->parts[15], 0, 0);
    sub_80270E0(&self->parts[17], 0, 0);
    sub_80270E0(&self->parts[18], 0, 0);
    sub_80270E0(&self->parts[20], 0, 0);
    sub_80270E0(&self->parts[21], 0, 0);
    sub_80270E0(&self->parts[16], 0, 0);
    sub_80270E0(&self->parts[19], 0, 0);
    sub_80270E0(&self->parts[21], 0, 0);
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

#include "core.h"
#include "hud.h"

extern void *gUnknown_030012C0;
extern s32 gUnknown_0300086C;

extern void sub_80270E0(struct hud_digit_part *part, s32 x, s32 y);
extern s32 sub_803ADB4(s32 dividend, s32 divisor);
extern s32 sub_803AE4C(s32 dividend, s32 divisor);
extern s32 sub_803AFEC(void *state);

void sub_8027838(struct hud_counter *counter)
{
    /* Register pins preserve the ROM allocation after plain struct C
     * reordered the clamp loads; see docs/workflow.md step 7. */
    register struct hud_counter *self asm("r5") = counter;
    register struct hud_digit_part *parts asm("r4");

    if (self->mode == 0) {
        return;
    }

    {
        register void **state_slot asm("r4") = &gUnknown_030012C0;
        register s32 value asm("r0");

        if (sub_803AFEC(*state_slot) > 0) {
            value = sub_803AFEC(*state_slot);
        } else {
            value = 0;
        }
        self->value = value;
    }

    {
        register s32 mode asm("r0") = self->mode;

        if (mode == 1 || mode == 3) {
            gUnknown_0300086C = self->layout_value * 2 - 0x28;
        } else {
            gUnknown_0300086C = 0;
        }
    }

    {
        register s32 current asm("r1") = self->value;
        register s32 previous asm("r0") = self->previous_value;
        parts = self->parts;

        if (current != previous) {
            if (current > 9) {
                {
                    register s32 frame asm("r3") = sub_803ADB4(current, 10);
                    register struct hud_anim_data *anim_data asm("r0");
                    register u8 *index_addr asm("r2");
                    register struct hud_anim_record *records asm("r1");
                    register u32 index asm("r6");
                    register u32 record_offset asm("r0");
                    register struct hud_anim_record *record asm("r0");
                    register s32 frame_count asm("r0");

                    anim_data = parts[0].anim_data;
                    index_addr = &parts[0].anim_index;
                    records = anim_data->records;
                    index = *index_addr;
                    record_offset = index * sizeof(*records);
                    /* Plain C reverses this commutative ADD's operands. */
                    asm volatile("add %0, %0, %1" : "+r"(record_offset) : "r"(records));
                    record = (struct hud_anim_record *)record_offset;
                    frame_count = record->frame_count;
                    if (frame >= frame_count) {
                        frame = frame_count - 1;
                    }
                    parts[0].frame_index = frame;
                }

                {
                    register s32 result asm("r0") = sub_803AE4C(self->value, 10);
                    register struct hud_digit_part *part asm("r6") = &parts[1];
                    register s32 frame asm("r3") = result;
                    register struct hud_anim_data *anim_data asm("r0");
                    register u8 *index_addr asm("r2");
                    register struct hud_anim_record *records asm("r1");
                    u32 second_index;
                    register u32 record_offset asm("r0");
                    register struct hud_anim_record *record asm("r0");
                    register s32 frame_count asm("r0");

                    anim_data = part->anim_data;
                    index_addr = &parts[1].anim_index;
                    records = anim_data->records;
                    second_index = *index_addr;
                    /* Keeping index_addr live here reproduces the ROM's r7/r2
                     * allocation; the equivalent expression otherwise uses r2. */
                    asm volatile(
                        "lsl r0, %1, #3\n\t"
                        "add %0, %1, #0\n\t"
                        "sub r0, r0, %0\n\t"
                        "lsl r0, r0, #2"
                        : "+r"(index_addr)
                        : "l"(second_index)
                        : "r0");
                    /* Exposes the r0 result of the allocation anchor above. */
                    asm volatile("" : "=r"(record_offset));
                    /* Plain C reverses this commutative ADD's operands. */
                    asm volatile("add %0, %0, %1" : "+r"(record_offset) : "r"(records));
                    record = (struct hud_anim_record *)record_offset;
                    frame_count = record->frame_count;
                    if (frame >= frame_count) {
                        frame = frame_count - 1;
                    }
                    part->frame_index = frame;
                }
            } else {
                register s32 frame asm("r3") = current;
                register struct hud_anim_data *anim_data asm("r0");
                register u8 *index_addr asm("r2");
                register struct hud_anim_record *records asm("r1");
                register u32 index asm("r6");
                register u32 record_offset asm("r0");
                register struct hud_anim_record *record asm("r0");
                register s32 frame_count asm("r0");
                register struct hud_digit_part *part asm("r3");
                register s32 hidden_frame asm("r1");

                anim_data = parts[0].anim_data;
                index_addr = &parts[0].anim_index;
                records = anim_data->records;
                index = *index_addr;
                record_offset = index * sizeof(*records);
                /* Plain C reverses this commutative ADD's operands. */
                asm volatile("add %0, %0, %1" : "+r"(record_offset) : "r"(records));
                record = (struct hud_anim_record *)record_offset;
                frame_count = record->frame_count;
                if (frame >= frame_count) {
                    frame = frame_count - 1;
                }
                parts[0].frame_index = frame;

                part = &parts[1];
                index_addr = &parts[1].anim_index;
                /* The ROM retains this otherwise-dead read before hiding digit 2. */
                asm volatile("ldrb r7, [%0]" : : "r"(index_addr));
                hidden_frame = -1;
                part->frame_index = hidden_frame;
            }
        }
    }

    sub_80270E0(&parts[2], 0, 0);
    sub_80270E0(&self->parts[0], 0, 0);
    sub_80270E0(&self->parts[1], 0, 0);
    self->previous_value = self->value;
}

/* Match the ROM's zero halfword padding rather than Thumb NOP padding. */
asm(".align 2, 0");

#include "core.h"

struct sub_8008618_anim_record {
    u8 unknown_00[0x16];
    u8 frame_count;
    u8 unknown_17[5];
};

struct sub_8008618_anim_data {
    struct sub_8008618_anim_record *records;
};

struct sub_8008618_part {
    u8 unknown_00[0x20];
    struct sub_8008618_anim_data *anim_data;
    u8 unknown_24[9];
    u8 anim_index;
    u8 unknown_2E[2];
    s32 frame_index;
};

void sub_8008618(struct sub_8008618_part *part, s32 frame_index)
{
    /* Register pins preserve the ROM's otherwise-unreachable allocation;
     * see docs/workflow.md step 7 and docs/matching.md. */
    register struct sub_8008618_part *self asm("r4") = part;
    register s32 frame asm("r3") = frame_index;
    register struct sub_8008618_anim_data *anim_data asm("r0");
    register u8 *index_addr asm("r2");
    register struct sub_8008618_anim_record *records asm("r1");
    register u32 index asm("r5");
    register u32 record_offset asm("r0");
    register struct sub_8008618_anim_record *record asm("r0");
    register s32 frame_count asm("r0");

    anim_data = self->anim_data;
    index_addr = &self->anim_index;
    records = anim_data->records;
    index = *index_addr;
    record_offset = index * sizeof(*records);
    /* Plain C reverses the two ADD source operands; see workflow step 7. */
    asm volatile("add %0, %0, %1" : "+r"(record_offset) : "r"(records));
    record = (struct sub_8008618_anim_record *)record_offset;
    frame_count = record->frame_count;
    if (frame >= frame_count) {
        frame = frame_count - 1;
    }
    self->frame_index = frame;
}

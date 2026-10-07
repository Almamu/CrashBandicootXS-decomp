#include "core.h"
#include "match.h"
#include "hud.h"
#include "util.h"
#include <libgcc.h>
#include "globals.h"

void UpdateHudLives(struct hud_counter *counter)
{
    /* Register pins preserve the ROM allocation after plain struct C
     * reordered the clamp loads; see docs/workflow.md step 7. */
    MATCH_HOLD_REG(struct hud_counter *, self, r5) = counter;
    MATCH_HOLD_REG(struct hud_digit_part *, parts, r4);

    if (self->livesSlide == 0) {
        return;
    }

    {
        MATCH_HOLD_REG(struct level_state **, state_slot, r4) = &gLevelState;
        MATCH_HOLD_REG(s32, value, r0);

        if (GetLives(*state_slot) > 0) {
            value = GetLives(*state_slot);
        } else {
            value = 0;
        }
        self->lives = value;
    }

    {
        MATCH_HOLD_REG(s32, mode, r0) = self->livesSlide;

        if (mode == 1 || mode == 3) {
            gHudSlideOffset = self->livesSlideTimer * 2 - 0x28;
        } else {
            gHudSlideOffset = 0;
        }
    }

    {
        MATCH_HOLD_REG(s32, current, r1) = self->lives;
        MATCH_HOLD_REG(s32, previous, r0) = self->shownLives;
        parts = self->parts;

        if (current != previous) {
            if (current > 9) {
                {
                    MATCH_HOLD_REG(s32, frame, r3) = __divsi3(current, 10);
                    MATCH_HOLD_REG(struct hud_anim_data *, anim_data, r0);
                    MATCH_HOLD_REG(u8 *, index_addr, r2);
                    MATCH_HOLD_REG(struct hud_anim_record *, records, r1);
                    MATCH_HOLD_REG(u32, index, r6);
                    MATCH_HOLD_REG(u32, record_offset, r0);
                    MATCH_HOLD_REG(struct hud_anim_record *, record, r0);
                    MATCH_HOLD_REG(s32, frame_count, r0);

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
                    MATCH_HOLD_REG(s32, result, r0) = __modsi3(self->lives, 10);
                    MATCH_HOLD_REG(struct hud_digit_part *, part, r6) = &parts[1];
                    MATCH_HOLD_REG(s32, frame, r3) = result;
                    MATCH_HOLD_REG(struct hud_anim_data *, anim_data, r0);
                    MATCH_HOLD_REG(u8 *, index_addr, r2);
                    MATCH_HOLD_REG(struct hud_anim_record *, records, r1);
                    u32 second_index;
                    MATCH_HOLD_REG(u32, record_offset, r0);
                    MATCH_HOLD_REG(struct hud_anim_record *, record, r0);
                    MATCH_HOLD_REG(s32, frame_count, r0);

                    anim_data = part->anim_data;
                    index_addr = &parts[1].anim_index;
                    records = anim_data->records;
                    second_index = *index_addr;
                    /* Keeping index_addr live here reproduces the ROM's r7/r2
                     * allocation; the equivalent expression otherwise uses r2. */
                    // clang-format off
                    asm volatile(
                        "lsl r0, %1, #3\n\t"
                        "add %0, %1, #0\n\t"
                        "sub r0, r0, %0\n\t"
                        "lsl r0, r0, #2"
                        : "+r"(index_addr)
                        : "l"(second_index)
                        : "r0");
                    // clang-format on
                    /* Exposes the r0 result of the allocation anchor above. */
                    MATCH_HOLD_VOLATILE(record_offset);
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
                MATCH_HOLD_REG(s32, frame, r3) = current;
                MATCH_HOLD_REG(struct hud_anim_data *, anim_data, r0);
                MATCH_HOLD_REG(u8 *, index_addr, r2);
                MATCH_HOLD_REG(struct hud_anim_record *, records, r1);
                MATCH_HOLD_REG(u32, index, r6);
                MATCH_HOLD_REG(u32, record_offset, r0);
                MATCH_HOLD_REG(struct hud_anim_record *, record, r0);
                MATCH_HOLD_REG(s32, frame_count, r0);
                MATCH_HOLD_REG(struct hud_digit_part *, part, r3);
                MATCH_HOLD_REG(s32, hidden_frame, r1);

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

    DrawHudPart(&parts[2], 0, 0);
    DrawHudPart(&self->parts[0], 0, 0);
    DrawHudPart(&self->parts[1], 0, 0);
    self->shownLives = self->lives;
}

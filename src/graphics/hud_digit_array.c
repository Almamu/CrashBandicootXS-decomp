#include "core.h"
#include "hud.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

/* Local views of the slot and record fields hud.h doesn't name yet. */
struct hud_slot
{
    u8 unk_00[0x20];
    struct hud_anim_data *anim_data;  // 0x20
    u8 unk_24[5];
    u8 palette:4;                     // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 anim_index;                    // 0x2D
    u8 unk_2E[2];
    s32 frame_index;                  // 0x30
    u8 unk_34[0xC];
};

struct hud_record
{
    u8 unk_00[0x14];
    u8 tile_record;                   // 0x14
    u8 unk_15;
    u8 frame_count;                   // 0x16
    u8 unk_17[5];
};

struct hud_pos
{
    s32 x;
    s32 y;
};

extern void ***gUnknown_030012D0;
extern void *gUnknown_030012C0;
extern u8 *gUnknown_030012B8;
extern u32 gStaticData_08174BE0[];
extern struct hud_pos gStaticData_08174C6C[];

extern void *sub_8026EC0(s32 size);
extern void sub_8027120(struct hud_slot *slot);
extern void sub_80088D8(struct hud_slot *slot, s32 value);
extern s32 sub_80233B4(void *self);
extern void sub_80087C0(struct hud_slot *slot);
extern void sub_80087B4(struct hud_slot *slot);
extern void sub_800872C(struct hud_slot *slot, s32 arg);
extern void sub_800737C(struct hud_slot *slot, s32 x, s32 y);
extern u8 sub_8006DF8(u8 *cache, s32 recordId);
extern void sub_802732C(struct hud_counter *self, u8 iconFlag);

#define HUD_ANIM(offset) ((struct hud_anim_data *)((u8 *)**gUnknown_030012D0 + (offset)))
#define SLOT_RECORD(s) (((struct hud_record *)(s)->anim_data->records)[(s)->anim_index])

static inline void RestartSlot(struct hud_slot *slot)
{
    sub_80087C0(slot);
    sub_80087B4(slot);
    sub_800872C(slot, 0);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetSlotFrame(struct hud_slot *slot, s32 frame)
{
    s32 n = SLOT_RECORD(slot).frame_count;
    if (frame >= n)
        frame = n - 1;
    slot->frame_index = frame;
}

#define SLOTS(self) ((struct hud_slot *)(self)->parts)

static inline void SetSlotPos(struct hud_slot *slot, struct hud_pos *pos)
{
    sub_800737C(slot, pos->x, pos->y);
}

/* Builds self->parts: a counted array of 35 HUD slots, each given the
 * shared HUD animation table, its frame (slot 22 shows the current mode's
 * life icon) and its position. Slot 13 gets the second table plus its
 * tile record's palette, slots 16/19/21 their starting frames, and
 * sub_802732C does the rest. */
struct hud_counter *sub_8027138(struct hud_counter *self)
{
    s32 i;

    {
        s32 *mem = sub_8026EC0(0x8C4);
        struct hud_slot *slots;
        struct hud_slot *slot;
        s32 n;

        *mem++ = 0x23;
        slots = (struct hud_slot *)mem;
        for (slot = slots, n = 0x22; n != -1; slot++, n--)
            sub_8027120(slot);
        self->parts = (struct hud_digit_part *)slots;
    }
    self->mode = 0;
    *(s32 *)&self->unknown_0c[4] = 0;
    self->field_08 = 0;
    self->layout_value = 0;
    *(s32 *)&self->unknown_0c[8] = 0;
    *(s32 *)&self->unknown_0c[0] = 0;

    for (i = 0; i <= 0x22; i++)
    {
        struct hud_slot *slot;

        sub_80088D8(&SLOTS(self)[i], 0);
        {
            struct hud_anim_data *anim = HUD_ANIM(0x234);

            slot = (struct hud_slot *)(i * sizeof(struct hud_slot) + (u32)SLOTS(self));
            slot->anim_data = anim;
        }
        if (i == 0x16)
        {
            s32 life = sub_80233B4(gUnknown_030012C0);

            slot = &SLOTS(self)[i];
            SLOTS(self)[0x16].anim_index = life + 6;
            RestartSlot(slot);
        }
        else
        {
            slot->anim_index = gStaticData_08174BE0[i];
            RestartSlot(slot);
        }
        SetSlotPos(&SLOTS(self)[i], &gStaticData_08174C6C[i]);
    }

    {
        struct hud_anim_data *anim;
        struct hud_slot *slot;

        i = 13;
        anim = HUD_ANIM(0x1A4);
        slot = &SLOTS(self)[i];
        slot->anim_data = anim;
        SLOTS(self)[13].anim_index = gStaticData_08174BE0[13];
        RestartSlot(slot);
    }
    {
        struct hud_record *records = (struct hud_record *)SLOTS(self)[13].anim_data->records;
        struct hud_record *rec = &records[SLOTS(self)[13].anim_index];
        s32 palette = sub_8006DF8(gUnknown_030012B8, rec->tile_record);

        SLOTS(self)[13].palette = palette;
    }

    SetSlotFrame(&SLOTS(self)[16], 10);
    SetSlotFrame(&SLOTS(self)[19], 11);
    SetSlotFrame(&SLOTS(self)[21], 0);
    sub_802732C(self, 0);
    return self;
}

/* `sub_8027138`'s own tail: stores `iconFlag` into `self->icon_flag`,
 * finishes the two slots `sub_8027138` set up part of already (a
 * position/frame-index pair from a shared table, then the same
 * `field_29`-low-nibble update `settings_menu6.c`'s
 * `UPDATE_ICON_FRAME_NIBBLE` macro names for the unrelated
 * `struct settings_icon_actor` family - `sub_800815C`'s result feeds the
 * same low-nibble-preserving update here too), then loops over the
 * remaining slots (index 0-34 again) repositioning/re-clamping a
 * handful of specific ones (13, 22, 29 - byte offsets `0x340`/`0x580`/
 * `0x740` off `self->parts`) depending on the current level/game-mode
 * (`sub_80233B4`) and `self->icon_flag`, before DMA-filling nine words
 * at `self+0x40` with `-1` (a raw `REG_DMA3SAD`/`DAD`/`CNT` poke, the
 * same low-level idiom `settings_menu8e.c`'s `sub_8002AA4` and
 * `link_cable.c` already document for this ROM).
 *
 * NAKED: plain C under old_agbcc is 32 bytes short. Everything up to the
 * per-slot palette stores matches, but the ROM keeps three separate
 * copies of the "palette = frame" nibble insert (sharing only the final
 * `orr`/`strb`), which this compile merges into one. */
NAKED void sub_802732C(struct hud_counter *self, u8 iconFlag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r6, r0, #0\n\t"
        "strb r1, [r6, #0x18]\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xd2\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "mov r5, #0xd0\n\t"
        "lsl r5, r5, #2\n\t"
        "ldr r1, [r6, #0x64]\n\t"
        "add r4, r1, r5\n\t"
        "str r0, [r4, #0x20]\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0, #0x34]\n\t"
        "ldr r2, 3f\n\t"
        "add r1, r1, r2\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, [r6, #0x64]\n\t"
        "add r0, r0, r5\n\t"
        "bl sub_800815C\n\t"
        "mov sl, r0\n\t"
        "ldr r2, [r6, #0x64]\n\t"
        "ldr r3, 4f\n\t"
        "add r2, r2, r3\n\t"
        "mov r0, #0xf\n\t"
        "mov r1, sl\n\t"
        "and r1, r0\n\t"
        "mov r3, #0x10\n\t"
        "neg r3, r3\n\t"
        "add r0, r3, #0\n\t"
        "ldrb r4, [r2]\n\t"
        "and r0, r4\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "mov r7, #0\n\t"
        "mov sb, r3\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
    "5:\n\t"
        "lsl r5, r7, #6\n\t"
        "cmp r7, #0x16\n\t"
        "bne 6f\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80233B4\n\t"
        "ldr r1, [r6, #0x64]\n\t"
        "mov r2, r8\n\t"
        "add r4, r1, r2\n\t"
        "add r0, #6\n\t"
        "ldr r3, 8f\n\t"
        "add r1, r1, r3\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
    "6:\n\t"
        "mov r4, #0\n\t"
        "cmp r7, #0x16\n\t"
        "blt 9f\n\t"
        "cmp r7, #0x17\n\t"
        "ble 10f\n\t"
        "cmp r7, #0x1d\n\t"
        "beq 11f\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012D0\n"
    "2: .4byte gStaticData_08174BE0\n"
    "3: .4byte 0x0000036D\n"
    "4: .4byte 0x00000369\n"
    "7: .4byte gUnknown_030012C0\n"
    "8: .4byte 0x000005AD\n"
    "10:\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80233B4\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "cmp r0, r1\n\t"
        "beq 13f\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "12: .4byte gUnknown_030012C0\n"
    "11:\n\t"
        "ldrb r0, [r6, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "ldr r0, [r6, #0x64]\n\t"
        "add r0, r0, r5\n\t"
        "b 14f\n\t"
    "9:\n\t"
        "ldr r0, [r6, #0x64]\n\t"
        "add r0, r8\n\t"
    "14:\n\t"
        "bl sub_800815C\n\t"
        "add r4, r0, #0\n\t"
    "13:\n\t"
        "ldr r0, 15f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80233B4\n\t"
        "mov r1, #1\n\t"
        "neg r1, r1\n\t"
        "cmp r0, r1\n\t"
        "bne 16f\n\t"
        "ldrb r0, [r6, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "beq 16f\n\t"
        "cmp r7, #0x15\n\t"
        "bgt 17f\n\t"
        "cmp r7, #0xe\n\t"
        "blt 17f\n\t"
        "ldr r1, 18f\n\t"
        "lsl r0, r7, #3\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r6, #0x64]\n\t"
        "add r1, r8\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #8\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0xa0\n\t"
        "lsl r0, r0, #5\n\t"
        "str r0, [r1, #4]\n\t"
        "add r1, #0x29\n\t"
        "mov r0, #0xf\n\t"
        "and r4, r0\n\t"
        "mov r0, sb\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "b 19f\n\t"
        ".align 2, 0\n"
    "15: .4byte gUnknown_030012C0\n"
    "18: .4byte gStaticData_08174C6C\n"
    "17:\n\t"
        "cmp r4, sl\n\t"
        "bne 20f\n\t"
        "ldr r0, [r6, #0x64]\n\t"
        "add r0, r8\n\t"
        "add r0, #0x29\n\t"
        "mov r2, #0xa\n\t"
        "mov r1, sb\n\t"
        "ldrb r3, [r0]\n\t"
        "and r1, r3\n\t"
        "orr r1, r2\n\t"
        "strb r1, [r0]\n\t"
        "b 21f\n\t"
    "20:\n\t"
        "ldr r1, [r6, #0x64]\n\t"
        "add r1, r8\n\t"
        "add r1, #0x29\n\t"
        "mov r0, #0xf\n\t"
        "and r4, r0\n\t"
        "mov r0, sb\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "b 19f\n\t"
    "16:\n\t"
        "ldr r1, [r6, #0x64]\n\t"
        "add r1, r8\n\t"
        "add r1, #0x29\n\t"
        "mov r0, #0xf\n\t"
        "and r4, r0\n\t"
        "mov r0, sb\n\t"
        "ldrb r3, [r1]\n\t"
        "and r0, r3\n\t"
    "19:\n\t"
        "orr r0, r4\n\t"
        "strb r0, [r1]\n\t"
    "21:\n\t"
        "mov r4, #0x40\n\t"
        "add r8, r4\n\t"
        "add r7, #1\n\t"
        "cmp r7, #0x22\n\t"
        "bgt 22f\n\t"
        "b 5b\n\t"
    "22:\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "str r0, [sp]\n\t"
        "ldr r1, 23f\n\t"
        "mov r0, sp\n\t"
        "str r0, [r1]\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x40\n\t"
        "str r0, [r1, #4]\n\t"
        "ldr r0, 24f\n\t"
        "str r0, [r1, #8]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "23: .4byte 0x040000D4\n"
    "24: .4byte 0x85000009\n"
    );
}

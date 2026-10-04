#include "core.h"
#include "text_popup.h"

/* "Two-line text popup" spawners, ROM 0x0801EF0C-0x0801FDEC. Each builds
 * a sub_8009ED0 part, attaches a CreateEnemyCtrl popup header and fills the
 * part's collected bits from its level record; the tails differ. Built
 * with old_agbcc; see include/text_popup.h. */

extern u8 gEnemyDefaultAnimMap[];
extern u8 gPenguinAnimMap[];
extern u8 gPufferfishAnimMap[];
extern u8 gBlowgunTribesmanAnimMap[];
extern u8 gVenusFlytrapAnimMap[];
extern u8 gVultureAnimMap[];
extern u8 gSharkAnimMap[];
extern u8 gElectricEelAnimMap[];
extern void *gAudioContext;

extern struct popup_part *sub_800A604(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void PlaySfx(void *bank, s32 sfxId, s32 volume);

/* Inline so the lookup's result gets its own register copy, as the ROM
 * does. */
static inline struct level_record *GetLevelRecord(u16 id)
{
    return LEVEL_RECORD(id);
}

static inline void SetPopupCenter(struct enemy_ctrl *hdr, s32 x, s32 y)
{
    hdr->unk_48 = x;
    hdr->unk_4C = y;
}

/* The u8 parameter keeps the toggled bit's truncation where the ROM
 * has it. */
static inline void SetPartFlipX(struct popup_part *part, u8 value)
{
    part->flipX = value;
}

/* Text popup, tag 0xD. Restarts the part's animation (tag 0) and hands
 * the level record's +4 word to the header through SetEnemyRangeX. */
void sub_801EF0C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x9c);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0xd;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->tag = 0;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->unk_04);
}

/* Text popup, tag 0xB. Draws the header with gEnemyDefaultAnimMap,
 * then switches it to gVultureAnimMap and gives it a fixed
 * 100x50 box at the origin. */
void SpawnVulture(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x84);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0xb;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetEnemyState(hdr, 0x11);
    hdr->animMap = gVultureAnimMap;
    SetPopupRect(hdr, 0, 0, 100, 50);
}

/* Text popup, tag 0xA. Restarts the part's animation as tag 1, sets
 * field_0A to 6, then switches the header to gVenusFlytrapAnimMap with
 * a 45x20 box raised 20 pixels. Still in its agbcc-era pinned form: the
 * plain-C version is 5 halfwords off under old_agbcc, loading the three
 * constants after the second attach as 0->sl, 1->r8, 1->r9 where the ROM
 * loads 1->r9, 0->sl, 1->r8. */
void SpawnVenusFlytrap(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct popup_part *part asm("r4");
    register s32 oneSb asm("sb");
    register s32 zeroSl asm("sl");
    register s32 oneR8 asm("r8");
    void *p2;
    void *p3;
    register struct enemy_ctrl *hdr asm("r6");
    struct popup_vtable *table;

  {
    register s32 idx asm("r5");

    asm volatile(
        "add r5, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "lsl r5, r5, #0x10\n\t"
        "lsr r5, r5, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8009ED0\n\t"
        "add r4, r0, #0\n\t"
        : "=r" (part), "=r" (idx)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;
    *(void **)((u8 *)part + 0x20) = (u8 *)p3 + 0x78;

    {
        register s32 result asm("r0") = sub_800815C(part);
        register u8 *addr asm("r2") = (u8 *)part + 0x29;
        register s32 acc asm("r1");
        result &= 0xf;
        asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r" (acc));
        acc &= *addr;
        acc |= result;
        *addr = acc;
    }

    sub_8026EDC(0x8c);

    hdr = CreateEnemyCtrl();
    table = hdr->vtable;
    _call_via_r2((u8 *)hdr + table->attach.thisOffset, part, table->attach.fn);
    {
        register s32 tagVal asm("r0") = 0xa;
        hdr->kind = tagVal;
    }
    part->hdr = hdr;
    table = hdr->vtable;
    {
        /* A bare `register s32 off asm("r3") = 0x18;` pin is silently
         * ignored by this compiler for a simple constant initializer
         * (lands the two-step mov/lsl synthesis in whatever register it
         * likes, not the ROM's r3) - the same gotcha
         * docs/matching/issue-31-graphics-loading.md documents for
         * sub_8021388's own `+0x20` table-offset constant. Spelled out
         * as a full hand-written trampoline call instead. */
        register void *tbl asm("r1") = table;
        asm volatile(
            "mov r3, #0x18\n\t"
            "ldrsh r0, [r1, r3]\n\t"
            "add r0, r6, r0\n\t"
            "ldr r2, [r1, #0x1c]\n\t"
            "add r1, r4, #0\n\t"
            "bl _call_via_r2\n\t"
            :
            : "r" (tbl), "r" (hdr), "r" (part)
            : "r0", "r1", "r2", "r3", "lr", "memory");
    }

    asm volatile(
        "mov r0, #1\n\t"
        "mov sb, r0\n\t"
        "mov r1, #0\n\t"
        "mov sl, r1\n\t"
        "mov r2, #1\n\t"
        "mov r8, r2\n\t"
        "mov r3, sb\n\t"
        "strb r3, [r4, #0xa]\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        : "=r" (oneSb), "=r" (zeroSl), "=r" (oneR8)
        : "r" (part)
        : "r0", "r1", "r2", "r3", "memory");

    {
        register void *gAddr asm("r0") = &gEntityFlags;

        asm volatile(
            "ldr r0, [r0]\n\t"
            "ldr r1, [r0]\n\t"
            "ldr r0, [r1, #8]\n\t"
            "lsl r5, r5, #1\n\t"
            "add r5, r5, r0\n\t"
            "ldr r2, [r1, #0xc]\n\t"
            "ldrh r5, [r5]\n\t"
            "add r2, r5, r2\n\t"
            "ldrb r3, [r2]\n\t"
            "lsr r0, r3, #1\n\t"
            "mov r5, r8\n\t"
            "eor r0, r0, r5\n\t"
            "and r0, r0, r5\n\t"
            "add r3, r4, #0\n\t"
            "add r3, r3, #0x28\n\t"
            "and r0, r0, r5\n\t"
            "lsl r0, r0, #4\n\t"
            "mov r1, #0x11\n\t"
            "neg r1, r1\n\t"
            "ldrb r5, [r3]\n\t"
            "and r1, r1, r5\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            "ldrb r2, [r2]\n\t"
            "lsr r0, r2, #2\n\t"
            "mov r2, r8\n\t"
            "and r0, r0, r2\n\t"
            "and r0, r0, r2\n\t"
            "lsl r0, r0, #5\n\t"
            "mov r2, #0x21\n\t"
            "neg r2, r2\n\t"
            "and r1, r1, r2\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            : "+r" (idx)
            : "r" (part), "r" (gAddr), "r" (oneR8)
            : "r0", "r1", "r2", "r3", "r5", "memory");
    }
  }

    sub_8008E94(gUnknown_030012F0, part);

    {
        register void *val asm("r0") = gEnemyDefaultAnimMap;
        register void *statAddr asm("r5") = (u8 *)hdr + 0x84;
        *(void **)statAddr = val;

        {
            register u8 *addr2d asm("r0") = (u8 *)part + 0x2d;
            register s32 tagVal2 asm("r3") = oneSb;
            *addr2d = tagVal2;
        }

        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);

        {
            register s32 six asm("r0") = 6;
            *(u8 *)((u8 *)part + 0xa) = six;
        }

        SetEnemyState(hdr, 3);

        *(void **)statAddr = gVenusFlytrapAnimMap;
    }

    asm volatile(
        "mov r2, #0x14\n\t"
        "neg r2, r2\n\t"
        "mov r0, #0x2d\n\t"
        "mov r1, #0x14\n\t"
        "mov r5, sl\n\t"
        "str r5, [r6, #0x20]\n\t"
        "str r0, [r6, #0x28]\n\t"
        "str r2, [r6, #0x24]\n\t"
        "str r1, [r6, #0x2c]\n\t"
        :
        : "r" (hdr), "r" (zeroSl)
        : "r0", "r1", "r2", "r5", "memory");
}

/* Text popup, tag 0xE. Same as sub_801EF0C without the animation
 * restart: draws the header with gEnemyDefaultAnimMap and hands it the
 * level record's +4 word through SetEnemyRangeX. */
void sub_801F2BC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xa8);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0xe;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->unk_04);
}

/* Text popup, tag 0xC. Copies the level record's +4/+8/+0xC words into
 * the header, then centres on them: +0x48 is the midpoint of +4 and +8,
 * +0x4C is +0xC plus a quarter of that midpoint. */
void SpawnBlowgunTribesman(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;
    s32 mid;

    part->anim = POPUP_ANIM(0x90);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0xc;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = GetLevelRecord(arg3);
    SetEnemyAnimMap(hdr, gBlowgunTribesmanAnimMap);
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    mid = (rec2->unk_04 + rec2->unk_08) / 2;
    SetPopupCenter(hdr, mid, rec2->unk_0C + mid / 4);
    SetEnemyState(hdr, 0x10);
}

/* Text popup, tag 0xF. Restarts the part's animation (tag 0), switches
 * the header to gPenguinAnimMap and copies the level record's
 * +0xC/+8/+0x10 words into it before SetEnemyRangeX gets the +4 word. */
void SpawnPenguin(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xb4);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0xf;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->tag = 0;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    SetEnemyAnimMap(hdr, gPenguinAnimMap);
    SetPopupSpan(hdr, rec2->unk_0C, rec2->unk_08, rec2->unk_10);
    SetEnemyRangeX(hdr, rec2->unk_04);
    SetEnemyState(hdr, 0xd);
}

/* Popup spawner, tag 0x11, built with sub_800A604 instead of
 * sub_8009ED0. Resets the part's animation (tag 0 plus the OAM trio)
 * before attaching it, sets flag bit 4, and plays sound 0x27. */
void SpawnSeal(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_800A604(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0xcc);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x11;
    SetPartAnim(part, 0);
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetEnemyState(hdr, 5);
    PlaySfx(gAudioContext, 0x27, 0x100);
}

/* Popup spawner, tag 0x10. After registering the part it switches
 * part->field_0A to 6 and feeds the level record's +4 word to the
 * header via SetEnemyRangeX. */
void SpawnPolarBear(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xc0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x10;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 6;
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->unk_04);
}

/* Popup spawner, tag 5. Restarts the part on animation 2, sets
 * part->field_0A to 5, swaps the header's graphics to
 * gPufferfishAnimMap and copies six of the level record's words into
 * the header before SetEnemyState(hdr, 14). */
void SpawnPufferfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x3c);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 5;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetPartAnim(part, 2);
    part->base.field_0A = 5;
    SetEnemyAnimMap(hdr, gPufferfishAnimMap);
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    SetPopupBox(hdr, rec2->unk_14, rec2->unk_18, rec2->unk_10);
    SetEnemyState(hdr, 14);
}

/* Popup spawner, tag 4. Sets part->field_0A to 6, swaps the header's
 * graphics to gSharkAnimMap, and passes the level record's
 * +4..+0x10 words straight to SetEnemyRangeX/SetEnemyRangeYSpeed. */
void SpawnShark(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x30);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 4;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 6;
    SetEnemyAnimMap(hdr, gSharkAnimMap);
    SetEnemyState(hdr, 15);
    SetEnemyRangeX(hdr, rec2->unk_04);
    SetEnemyRangeYSpeed(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_10);
}

/* Popup spawner, tag 3. Restarts the part on animation 0, inverts the
 * flipX bit the collected-bits pack just wrote, and sets part->field_0A
 * to 6 before SetEnemyState(hdr, 1). */
void SpawnMorayEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x24);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 3;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetPartAnim(part, 0);
    {
        s32 f = part->flipX;
        SetPartFlipX(part, !f);
    }
    part->base.field_0A = 6;
    SetEnemyState(hdr, 1);
}

/* Popup spawner, tag 8. Sets part->field_0A to 3, swaps the header's
 * graphics to gElectricEelAnimMap, copies the level record's
 * +8/+0xC/+0x10 words into the header and hands its +4 word to
 * SetEnemyRangeX before SetEnemyState(hdr, 13). */
void SpawnElectricEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x60);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 8;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 3;
    SetEnemyAnimMap(hdr, gElectricEelAnimMap);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_10);
    SetEnemyRangeX(hdr, rec2->unk_04);
    SetEnemyState(hdr, 13);
}

#include "core.h"
#include "text_popup.h"

/* "Two-line text popup" spawners, ROM 0x0801FEEC-0x08020E84 - the
 * continuation of graphics_loading_1ef0c.c. Built with old_agbcc; see
 * include/text_popup.h. */

extern u8 gEnemyDefaultAnimMap[];
extern u8 gFlamethrowerLabAssistantAnimMap[];
extern u8 gStationarySpaceEnemyAnimMap[];
extern u8 gPatrollingSpaceEnemyAnimMap[];
extern u8 gPatrollingSewerEnemyAnimMap[];
extern u8 gCrusherAnimMap[];
extern u8 gSaucerLabAssistantAnimMap[];

/* Text popup, tag 9. Shows the header with gEnemyDefaultAnimMap and
 * style 6, then copies the level record's +8/+0xc/+4 words into
 * header+0x3c/0x40/0x44. */
void SpawnJellyfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x6c);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 9;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 6);
    SetEnemyWave(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_04);
}

/* Text popup, tag 0x19, anim +0x12c. Flips the part's flipX, sets
 * field_0A to 2, clears flag bit 6 and shows the header with style 1. */
void SpawnLaserBarrier(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x12c);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x19;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    {
        s32 f = part->flipX;
        part->flipX = f == 0;
    }
    SetPartField0A(part, 2);
    AndPartFlags(part, ~0x40);
    SetEnemyState(hdr, 1);
}

/* Entity type 0x38: an enemy on sprite bank 27 (a shell that shoots out
 * flaming spikes) that attacks in place (state 4, UpdateEnemyAttackCycle);
 * placed only in space rooms. Species not identified.
 *
 * Text popup, tag 0x1b, anim +0x144. After registering the part it
 * switches the header's graphics to gStationarySpaceEnemyAnimMap, copies the
 * level record's +8/+4/+0xc fields into header+0x30/0x34/0x38 and shows
 * it with style 4. */
void SpawnStationarySpaceEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x144);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x1b;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gStationarySpaceEnemyAnimMap);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_04, rec2->unk_0C);
    SetEnemyState(hdr, 4);
}

/* Entity type 0x39: an enemy on sprite bank 24 that patrols and attacks
 * (state 13); placed only in space rooms. Species not identified.
 *
 * Text popup, tag 0x18, anim +0x120. After registering the part it
 * switches the header's graphics to gPatrollingSpaceEnemyAnimMap, copies the
 * level record's +8/+0xc/+0x10 fields into header+0x30/0x34/0x38, passes
 * the record's +4 to SetEnemyRangeX and shows the header with style 0xd. */
void SpawnPatrollingSpaceEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x120);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x18;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gPatrollingSpaceEnemyAnimMap);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_0C, rec2->unk_10);
    SetEnemyRangeX(hdr, rec2->unk_04);
    SetEnemyState(hdr, 0xd);
}

/* Text popup, tag 0x1d, anim +0x15c, spawned 0x28 pixels above arg2.
 * After registering the part it switches the header's graphics to
 * gSaucerLabAssistantAnimMap, sets header+0x30/0x34/0x38 to {0x78, 0x5a,
 * record+0xc}, calls SetEnemyRangeX(hdr, 0x28) and shows it with style 0x12. */
void SpawnSaucerLabAssistant(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u16 y = arg2 - 0x28;
    struct popup_part *part = CreateMovingSprite(arg0, arg1, y, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x15c);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x1d;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gSaucerLabAssistantAnimMap);
    SetPopupSpan(hdr, 0x78, 0x5a, rec2->unk_0C);
    SetEnemyRangeX(hdr, 0x28);
    SetEnemyState(hdr, 0x12);
}

/* Text popup, tag 0x1a, anim +0x138. After registering the part it
 * switches it to mode 0xa with flags bit 6 cleared, points the header at
 * gCrusherAnimMap, copies the level record's +4/+8/+0xc fields into
 * header+0x30/0x34/0x38 and shows it with style 4. */
void SpawnPistonCrusher(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x138);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x1a;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetPartField0A(part, 0xa);
    AndPartFlags(part, ~0x40);
    SetEnemyAnimMap(hdr, gCrusherAnimMap);
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    SetEnemyState(hdr, 4);
}

/* Text popup, tag 0x17, anim +0x114. Flips the part's flipX bit, points
 * the header at gFlamethrowerLabAssistantAnimMap, copies the level record's
 * +8/+4/+0xc words into header+0x30/0x34/0x38 and shows it with style 4.
 * Register allocation took several passes (see
 * docs/matching/last-eleven-naked-retry.md and the passes it links).
 * The ROM keeps arg3 in r4, part+0x28 in r3 across AddToPartList through a
 * stack slot (`str r3, [sp]` after the argument setup, `ldr r3, [sp]`
 * before the second flip), &gEntityFlags in sb and -0x11 in sl.
 * The second flip reads part+0x28 back from `q2`, a stack-resident copy
 * (an `"m"` asm operand), so the first flip's pointer is block-local (r3)
 * and arg3/hdr+0x84 get r4/r5. */

/* The part's +0x28 bitfield byte seen through its own pointer. Padded
 * past a word so the fields are read with `ldrb` (a 4-byte struct is
 * read as a whole word). */
struct popup_bits
{
    u32 unk_28_0:4;
    u32 flipX:1;
    u32 unk_28_5:1;
    u32 unk_28_6:2;
    u8 unk_29[0x1f];
};

void SpawnFlamethrowerLabAssistant(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;
    struct popup_bits *q2;
    register s32 h3 asm("r3");

    part->anim = POPUP_ANIM(0x114);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x17;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    /* The `q2` store sits inside the second argument, after a copy of
     * `part` that the empty asm (no code) keeps as its own pseudo. That
     * copy is tied to r1, so the ROM's order comes out: `adds r1, r7, #0`,
     * then `str r3, [sp]`, then the call. With `(q2 = ..., part)` the
     * store comes before the r1 move. */
    AddToPartList(gCollidableList, ({
        struct popup_part *t = part;

        asm("" : "+r"(t));
        q2 = (struct popup_bits *)((u8 *)part + 0x28);
        t;
    }));
    /* No code: the "m" operand keeps `q2` in a stack slot, the ROM's
     * `str r3, [sp]` / `ldr r3, [sp]` pair around the call. */
    asm("" : : "m"(q2));
    /* No code: hold r3 over the gfx store and the rec2 lookup. The ROM
     * keeps r3 free there, so reloading &gEntityFlags out of sb
     * uses r1 (`mov r1, sb`), not r3. */
    asm("" : "=r"(h3));
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    /* No code: end of the r3 hold. */
    asm("" : : "r"(h3));
    /* No code: four extra references lift rec2's allocation priority
     * above the reloaded q2 pointer's, so rec2 keeps r2 and q2 gets r3. */
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    asm("" : : "r"(rec2));
    {
        struct popup_bits *p = q2;
        s32 f;

        /* No code: one extra reference puts the reloaded q2 pointer
         * ahead of the flag byte, so the pointer gets r3 and the byte
         * r4. */
        asm("" : : "r"(p));
        f = p->flipX;
        p->flipX = f == 0;
    }
    SetPartField0A(part, 1);
    SetEnemyAnimMap(hdr, gFlamethrowerLabAssistantAnimMap);
    SetPopupSpan(hdr, rec2->unk_08, rec2->unk_04, rec2->unk_0C);
    SetEnemyState(hdr, 4);
}

/* Entity type 0x41: an enemy on sprite bank 22 that follows the player's
 * X within its range (state 9, UpdateEnemyHomingX/UpdateEnemyOscillateX);
 * placed only in sewer rooms. Species not identified.
 *
 * Text popup, tag 0x16, anim +0x108. After registering the part it shows
 * the header with style 9, passes the level record's +4/+8/+0xc fields
 * to SetEnemyRangeXSpeed and sets header+0x3c/0x40/0x44 to {0x80, 0, 0x14}. */
void SpawnHomingSewerEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x108);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x16;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 9);
    SetEnemyRangeXSpeed(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    SetEnemyWave(hdr, 0x80, 0, 0x14);
}

/* Entity type 0x42: an enemy on sprite bank 20 that patrols (state 2);
 * placed only in sewer rooms. Species not identified.
 *
 * Text popup, tag 0x14, anim +0xf0. After registering the part it points
 * the header at gPatrollingSewerEnemyAnimMap, shows it with style 2 and passes
 * the level record's +4 field to SetEnemyRangeX. */
void SpawnPatrollingSewerEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xf0);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x14;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gPatrollingSewerEnemyAnimMap);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->unk_04);
}

/* "Two-line text popup" spawner, tag 0x15. After the shared setup it
 * points the header at gEnemyDefaultAnimMap, shows it with
 * SetEnemyState(hdr, 2) and passes the level record's +4 word to
 * SetEnemyRangeX. */
void SpawnRat(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xfc);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x15;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->unk_04);
}

/* "Two-line text popup" spawner, tag 0x13. The tail points the header at
 * gEnemyDefaultAnimMap, switches the part's field_0A from 1 to 7, then
 * re-points the header at gPatrollingSewerEnemyAnimMap before showing it with
 * SetEnemyState(hdr, 8). */
void SpawnFrog(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0xe4);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x13;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    part->base.field_0A = 7;
    hdr->animMap = gPatrollingSewerEnemyAnimMap;
    SetEnemyState(hdr, 8);
}

/* "Two-line text popup" spawner, tag 6. The tail sets the part's field_0A
 * to 4, shows the header with SetEnemyState(hdr, 0xb), then hands it the
 * level record's X bounds (+0x10..+0x18, SetEnemyRangeXSpeed) and Y bounds
 * (+0x4..+0xc, SetEnemyRangeYSpeed). */
void SpawnSeaMine(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0x48);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 6;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 4;
    SetEnemyState(hdr, 0xb);
    SetEnemyRangeXSpeed(hdr, rec2->unk_10, rec2->unk_14, rec2->unk_18);
    SetEnemyRangeYSpeed(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
}

/* "Two-line text popup" spawner, tag 0x12. The tail sets the part's
 * field_0A to 0xa and clears flag bit 6, re-points the header from
 * gEnemyDefaultAnimMap to gCrusherAnimMap, copies the level
 * record's +4/+8/+0xc words into the header's +0x30 box, and shows it
 * with SetEnemyState(hdr, 4). */
void SpawnWoodenCrusher(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;
    struct level_record *rec2;

    part->anim = POPUP_ANIM(0xd8);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 0x12;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.field_0A = 0xa;
    AndPartFlags(part, ~0x40);
    hdr->animMap = gCrusherAnimMap;
    SetPopupSpan(hdr, rec2->unk_04, rec2->unk_08, rec2->unk_0C);
    SetEnemyState(hdr, 4);
}

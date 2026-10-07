#include "core.h"
#include "match.h"
#include "text_popup.h"
#include "audio.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* codegen: CreateEnemyCtrl takes the 0x8C-byte block OperatorNew
 * returns (enemies.h). In 11 of the 26 spawners below the registers only
 * match with the two calls as separate statements, the block passed on
 * in r0 without a C argument; the other 15 call
 * `CreateEnemyCtrl(OperatorNew(0x8c))`. docs/headers_plan.md */
extern struct part_ctrl *CreateEnemyCtrl_r0(void) asm("CreateEnemyCtrl");

/* "Two-line text popup" spawners, ROM 0x0801EF0C-0x0801FDEC. Each builds
 * a CreateMovingSprite part, attaches a CreateEnemyCtrl popup header and fills the
 * part's collected bits from its level record; the tails differ. Built
 * with old_agbcc; see include/text_popup.h. */

/* Inline so the lookup's result gets its own register copy, as the ROM
 * does. */
static inline struct entity_params *GetLevelRecord(u16 id)
{
    return LEVEL_RECORD(id);
}

static inline void SetEnemyShotTiming(struct part_ctrl *hdr, s32 period, s32 phase)
{
    hdr->shotPeriod = period;
    hdr->shotPhase = phase;
}

/* The u8 parameter keeps the toggled bit's truncation where the ROM
 * has it. */
static inline void SetPartFlipX(struct popup_part *part, u8 value)
{
    part->flipX = value;
}

/* Text popup, tag 0xD. Restarts the part's animation (tag 0) and hands
 * the level record's +4 word to the header through SetEnemyRangeX. */
void SpawnLizard(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x9c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_LIZARD;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->tag = 0;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->p.patrol.rangeX);
}

/* Text popup, tag 0xB. Draws the header with gEnemyDefaultAnimMap,
 * then switches it to gVultureAnimMap and gives it a fixed
 * 100x50 box at the origin. */
void SpawnVulture(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0x84);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_VULTURE;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetEnemyState(hdr, 0x11);
    hdr->anims = gVultureAnimMap;
    SetEnemyHitBox(hdr, 0, 0, 100, 50);
}

/* Text popup, tag 0xA. Restarts the part's animation as tag 1, sets
 * field_0A to 6, then switches the header to gVenusFlytrapAnimMap with
 * a 45x20 box raised 20 pixels. Still in its agbcc-era pinned form: the
 * plain-C version is 5 halfwords off under old_agbcc, loading the three
 * constants after the second attach as 0->sl, 1->r8, 1->r9 where the ROM
 * loads 1->r9, 0->sl, 1->r8. */
void SpawnVenusFlytrap(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    MATCH_HOLD_REG(u32, raw0, r0) = arg0;
    MATCH_HOLD_REG(u32, raw1, r1) = arg1;
    MATCH_HOLD_REG(u32, raw2, r2) = arg2;
    MATCH_HOLD_REG(u32, raw3, r3) = arg3;
    MATCH_HOLD_REG(struct popup_part *, part, r4);
    MATCH_HOLD_REG(s32, oneSb, sb);
    MATCH_HOLD_REG(s32, zeroSl, sl);
    MATCH_HOLD_REG(s32, oneR8, r8);
    void *p2;
    void *p3;
    MATCH_HOLD_REG(struct part_ctrl *, hdr, r6);
    struct ctrl_anchor *table;

    {
        MATCH_HOLD_REG(s32, idx, r5);

        // clang-format off
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
            "bl CreateMovingSprite\n\t"
            "add r4, r0, #0\n\t"
            : "=r" (part), "=r" (idx)
            : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
            : "r1", "r2", "r3", "lr", "memory");
        // clang-format on

        p2 = (void *)gSpriteBankSet->table;
        p3 = *(void **)p2;
        part->anim = (u8 *)p3 + 0x78; /* sprite bank 10 */

        {
            MATCH_HOLD_REG(s32, result, r0) = GetSpriteAnimPaletteSlot((struct actor *)part);
            MATCH_HOLD_REG(u8 *, addr, r2) = (u8 *)part + 0x29;
            MATCH_HOLD_REG(s32, acc, r1);
            result &= 0xf;
            asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r"(acc));
            acc &= *addr;
            acc |= result;
            *addr = acc;
        }

        hdr = CreateEnemyCtrl(OperatorNew(0x8c));
        table = hdr->anchor;
        _call_via_r2((u8 *)hdr + table->attach.thisOffset, part, table->attach.fn);
        {
            MATCH_HOLD_REG(s32, tagVal, r0) = 0xa;
            hdr->kind = tagVal;
        }
        part->hdr = hdr;
        table = hdr->anchor;
        {
            /* A bare `MATCH_HOLD_REG(s32, off, r3) = 0x18;` pin is silently
             * ignored by this compiler for a simple constant initializer
             * (lands the two-step mov/lsl synthesis in whatever register it
             * likes, not the ROM's r3) - the same gotcha
             * docs/matching/archive/issue-31-graphics-loading.md documents for
             * SpawnDingodile's own `+0x20` table-offset constant. Spelled out
             * as a full hand-written trampoline call instead. */
            MATCH_HOLD_REG(void *, tbl, r1) = table;
            // clang-format off
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
            // clang-format on
        }

        // clang-format off
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
        // clang-format on

        {
            MATCH_HOLD_REG(void *, gAddr, r0) = &gEntityFlags;

            // clang-format off
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
            // clang-format on
        }
    }

    AddToPartList(gCollidableList, part);

    {
        MATCH_HOLD_REG(const void *, val, r0) = gEnemyDefaultAnimMap;
        MATCH_HOLD_REG(const s32 **, statAddr, r5) = &hdr->anims;
        *statAddr = val;

        {
            MATCH_HOLD_REG(u8 *, addr2d, r0) = &part->tag;
            MATCH_HOLD_REG(s32, tagVal2, r3) = oneSb;
            *addr2d = tagVal2;
        }

        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);

        {
            MATCH_HOLD_REG(s32, six, r0) = 6;
            part->base.kind = six;
        }

        SetEnemyState(hdr, 3);

        *(const void **)statAddr = gVenusFlytrapAnimMap;
    }

    // clang-format off
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
    // clang-format on
}

/* Entity type 0x2B: an enemy on sprite bank 14 that patrols (state 2,
 * UpdateEnemyPatrol); placed only in the jungle rooms 15, 27 and 32. The
 * species isn't identified, so the name is generic.
 *
 * Text popup, tag 0xE. Same as SpawnLizard without the animation
 * restart: draws the header with gEnemyDefaultAnimMap and hands it the
 * level record's +4 word through SetEnemyRangeX. */
void SpawnPatrollingJungleEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xa8);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PATROLLING_JUNGLE_ENEMY;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->p.patrol.rangeX);
}

/* Text popup, tag 0xC. Copies the level record's +4/+8/+0xC words into
 * the header, then centres on them: +0x48 is the midpoint of +4 and +8,
 * +0x4C is +0xC plus a quarter of that midpoint. */
void SpawnBlowgunTribesman(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;
    s32 mid;

    part->anim = POPUP_ANIM(0x90);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_BLOWGUN_TRIBESMAN;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = GetLevelRecord(arg3);
    SetEnemyAnimMap(hdr, gBlowgunTribesmanAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                        rec2->p.cycle.cycleOffset);
    mid = (rec2->p.cycle.idleTime + rec2->p.cycle.attackTime) / 2;
    SetEnemyShotTiming(hdr, mid, rec2->p.cycle.cycleOffset + mid / 4);
    SetEnemyState(hdr, 0x10);
}

/* Text popup, tag 0xF. Restarts the part's animation (tag 0), switches
 * the header to gPenguinAnimMap and copies the level record's
 * +0xC/+8/+0x10 words into it before SetEnemyRangeX gets the +4 word. */
void SpawnPenguin(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xb4);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PENGUIN;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->tag = 0;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    SetEnemyAnimMap(hdr, gPenguinAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.rangeAttackFirst.idleTime, rec2->p.rangeAttackFirst.attackTime,
                        rec2->p.rangeAttackFirst.cycleOffset);
    SetEnemyRangeX(hdr, rec2->p.rangeAttackFirst.rangeX);
    SetEnemyState(hdr, 0xd);
}

/* Popup spawner, tag 0x11, built with CreateGroundSprite instead of
 * CreateMovingSprite. Resets the part's animation (tag 0 plus the OAM trio)
 * before attaching it, sets flag bit 4, and plays SFX_SEAL_SPAWN. */
void SpawnSeal(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateGroundSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0xcc);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_SEAL;
    SetPartAnim(part, 0);
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetEnemyState(hdr, 5);
    PlaySfx(gAudioContext, SFX_SEAL_SPAWN, 0x100);
}

/* Popup spawner, tag 0x10. After registering the part it switches
 * part->field_0A to 6 and feeds the level record's +4 word to the
 * header via SetEnemyRangeX. */
void SpawnPolarBear(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xc0);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_POLAR_BEAR;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.kind = 6;
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->p.patrol.rangeX);
}

/* Popup spawner, tag 5. Restarts the part on animation 2, sets
 * part->field_0A to 5, swaps the header's graphics to
 * gPufferfishAnimMap and copies six of the level record's words into
 * the header before SetEnemyState(hdr, 14). */
void SpawnPufferfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x3c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PUFFERFISH;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetPartAnim(part, 2);
    part->base.kind = 5;
    SetEnemyAnimMap(hdr, gPufferfishAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                        rec2->p.cycle.cycleOffset);
    SetEnemyWave(hdr, rec2->p.cycle.wavePeriod, rec2->p.cycle.wavePhase,
                 rec2->p.cycle.waveAmplitude);
    SetEnemyState(hdr, 14);
}

/* Popup spawner, tag 4. Sets part->field_0A to 6, swaps the header's
 * graphics to gSharkAnimMap, and passes the level record's
 * +4..+0x10 words straight to SetEnemyRangeX/SetEnemyRangeYSpeed. */
void SpawnShark(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x30);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_SHARK;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.kind = 6;
    SetEnemyAnimMap(hdr, gSharkAnimMap);
    SetEnemyState(hdr, 15);
    SetEnemyRangeX(hdr, rec2->p.rangeHomingY.rangeX);
    SetEnemyRangeYSpeed(hdr, rec2->p.rangeHomingY.rangeY, rec2->p.rangeHomingY.speedY,
                        rec2->p.rangeHomingY.accelY);
}

/* Popup spawner, tag 3. Restarts the part on animation 0, inverts the
 * flipX bit the collected-bits pack just wrote, and sets part->field_0A
 * to 6 before SetEnemyState(hdr, 1). */
void SpawnMorayEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0x24);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_MORAY_EEL;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    SetPartAnim(part, 0);
    {
        s32 f = part->flipX;
        SetPartFlipX(part, !f);
    }
    part->base.kind = 6;
    SetEnemyState(hdr, 1);
}

/* Popup spawner, tag 8. Sets part->field_0A to 3, swaps the header's
 * graphics to gElectricEelAnimMap, copies the level record's
 * +8/+0xC/+0x10 words into the header and hands its +4 word to
 * SetEnemyRangeX before SetEnemyState(hdr, 13). */
void SpawnElectricEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x60);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_ELECTRIC_EEL;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.kind = 3;
    SetEnemyAnimMap(hdr, gElectricEelAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.rangeCycle.idleTime, rec2->p.rangeCycle.attackTime,
                        rec2->p.rangeCycle.cycleOffset);
    SetEnemyRangeX(hdr, rec2->p.rangeCycle.rangeX);
    SetEnemyState(hdr, 13);
}

/* "Two-line text popup" spawner. Built with old_agbcc; see
 * include/text_popup.h. */

/* Text popup, tag 7: the plainest member of the family. Shows the header
 * with gSquidAnimMap and style 7. */
void SpawnSquid(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0x54);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_SQUID;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    hdr->anims = gSquidAnimMap;
    SetEnemyState(hdr, 7);
}

/* "Two-line text popup" spawners, ROM 0x0801FEEC-0x08020E84 - the
 * continuation of the code above. Built with old_agbcc; see
 * include/text_popup.h. */

/* Text popup, tag 9. Shows the header with gEnemyDefaultAnimMap and
 * style 6, then copies the level record's +8/+0xc/+4 words into
 * header+0x3c/0x40/0x44. */
void SpawnJellyfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x6c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_JELLYFISH;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 6);
    SetEnemyWave(hdr, rec2->p.wave.period, rec2->p.wave.phase, rec2->p.wave.amplitude);
}

/* Text popup, tag 0x19, anim +0x12c. Flips the part's flipX, sets
 * field_0A to 2, clears flag bit 6 and shows the header with style 1. */
void SpawnLaserBarrier(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0x12c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_LASER_BARRIER;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    {
        s32 f = part->flipX;
        part->flipX = f == 0;
    }
    SetPartKind(part, 2);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x144);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_STATIONARY_SPACE_ENEMY;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gStationarySpaceEnemyAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.attackFirst.idleTime, rec2->p.attackFirst.attackTime,
                        rec2->p.attackFirst.cycleOffset);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x120);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PATROLLING_SPACE_ENEMY;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gPatrollingSpaceEnemyAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.rangeCycle.idleTime, rec2->p.rangeCycle.attackTime,
                        rec2->p.rangeCycle.cycleOffset);
    SetEnemyRangeX(hdr, rec2->p.rangeCycle.rangeX);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x15c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_SAUCER_LAB_ASSISTANT;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gSaucerLabAssistantAnimMap);
    SetEnemyAttackCycle(hdr, 0x78, 0x5a, rec2->p.cycle.cycleOffset);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x138);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PISTON_CRUSHER;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetPartKind(part, 0xa);
    AndPartFlags(part, ~0x40);
    SetEnemyAnimMap(hdr, gCrusherAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                        rec2->p.cycle.cycleOffset);
    SetEnemyState(hdr, 4);
}

/* Text popup, tag 0x17, anim +0x114. Flips the part's flipX bit, points
 * the header at gFlamethrowerLabAssistantAnimMap, copies the level record's
 * +8/+4/+0xc words into header+0x30/0x34/0x38 and shows it with style 4.
 * Register allocation took several passes (see
 * docs/matching/archive/last-eleven-naked-retry.md and the passes it links).
 * The ROM keeps arg3 in r4, part+0x28 in r3 across AddToPartList through a
 * stack slot (`str r3, [sp]` after the argument setup, `ldr r3, [sp]`
 * before the second flip), &gEntityFlags in sb and -0x11 in sl.
 * The second flip reads part+0x28 back from `q2`, a stack-resident copy
 * (an `"m"` asm operand), so the first flip's pointer is block-local (r3)
 * and arg3/hdr+0x84 get r4/r5. */

/* The part's +0x28 bitfield byte seen through its own pointer. Padded
 * past a word so the fields are read with `ldrb` (a 4-byte struct is
 * read as a whole word). */
struct popup_bits {
    u32 unk_28_0:4;
    u32 flipX:1;
    u32 unk_28_5:1;
    u32 unk_28_6:2;
    u8 unk_29[0x1f];
};

void SpawnFlamethrowerLabAssistant(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;
    struct popup_bits *q2;
    MATCH_HOLD_REG(s32, h3, r3);

    part->anim = POPUP_ANIM(0x114);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_FLAMETHROWER_LAB_ASSISTANT;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    /* The `q2` store sits inside the second argument, after a copy of
     * `part` that the empty asm (no code) keeps as its own pseudo. That
     * copy is tied to r1, so the ROM's order comes out: `adds r1, r7, #0`,
     * then `str r3, [sp]`, then the call. With `(q2 = ..., part)` the
     * store comes before the r1 move. */
    // clang-format off
    AddToPartList(gCollidableList, ({
        struct popup_part *t = part;

        MATCH_KEEP(t);
        q2 = (struct popup_bits *)((u8 *)part + 0x28);
        t;
    }));
    // clang-format on
    /* No code: the "m" operand keeps `q2` in a stack slot, the ROM's
     * `str r3, [sp]` / `ldr r3, [sp]` pair around the call. */
    MATCH_USE_MEM(q2);
    /* No code: hold r3 over the gfx store and the rec2 lookup. The ROM
     * keeps r3 free there, so reloading &gEntityFlags out of sb
     * uses r1 (`mov r1, sb`), not r3. */
    MATCH_HOLD(h3);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    /* No code: end of the r3 hold. */
    MATCH_USE(h3);
    /* No code: four extra references lift rec2's allocation priority
     * above the reloaded q2 pointer's, so rec2 keeps r2 and q2 gets r3. */
    MATCH_USE(rec2);
    MATCH_USE(rec2);
    MATCH_USE(rec2);
    MATCH_USE(rec2);
    {
        struct popup_bits *p = q2;
        s32 f;

        /* No code: one extra reference puts the reloaded q2 pointer
         * ahead of the flag byte, so the pointer gets r3 and the byte
         * r4. */
        MATCH_USE(p);
        f = p->flipX;
        p->flipX = f == 0;
    }
    SetPartKind(part, 1);
    SetEnemyAnimMap(hdr, gFlamethrowerLabAssistantAnimMap);
    SetEnemyAttackCycle(hdr, rec2->p.attackFirst.idleTime, rec2->p.attackFirst.attackTime,
                        rec2->p.attackFirst.cycleOffset);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x108);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_HOMING_SEWER_ENEMY;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 9);
    SetEnemyRangeXSpeed(hdr, rec2->p.homingX.rangeX, rec2->p.homingX.speedX,
                        rec2->p.homingX.accelX);
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
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xf0);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_PATROLLING_SEWER_ENEMY;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyAnimMap(hdr, gPatrollingSewerEnemyAnimMap);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->p.patrol.rangeX);
}

/* "Two-line text popup" spawner, tag 0x15. After the shared setup it
 * points the header at gEnemyDefaultAnimMap, shows it with
 * SetEnemyState(hdr, 2) and passes the level record's +4 word to
 * SetEnemyRangeX. */
void SpawnRat(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xfc);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_RAT;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    SetEnemyState(hdr, 2);
    SetEnemyRangeX(hdr, rec2->p.patrol.rangeX);
}

/* "Two-line text popup" spawner, tag 0x13. The tail points the header at
 * gEnemyDefaultAnimMap, switches the part's field_0A from 1 to 7, then
 * re-points the header at gPatrollingSewerEnemyAnimMap before showing it with
 * SetEnemyState(hdr, 8). */
void SpawnFrog(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;

    part->anim = POPUP_ANIM(0xe4);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl_r0();
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_FROG;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    part->base.kind = 7;
    hdr->anims = gPatrollingSewerEnemyAnimMap;
    SetEnemyState(hdr, 8);
}

/* "Two-line text popup" spawner, tag 6. The tail sets the part's field_0A
 * to 4, shows the header with SetEnemyState(hdr, 0xb), then hands it the
 * level record's X bounds (+0x10..+0x18, SetEnemyRangeXSpeed) and Y bounds
 * (+0x4..+0xc, SetEnemyRangeYSpeed). */
void SpawnSeaMine(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0x48);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_SEA_MINE;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.kind = 4;
    SetEnemyState(hdr, 0xb);
    SetEnemyRangeXSpeed(hdr, rec2->p.homingXY.rangeX, rec2->p.homingXY.speedX,
                        rec2->p.homingXY.accelX);
    SetEnemyRangeYSpeed(hdr, rec2->p.homingXY.rangeY, rec2->p.homingXY.speedY,
                        rec2->p.homingXY.accelY);
}

/* "Two-line text popup" spawner, tag 0x12. The tail sets the part's
 * field_0A to 0xa and clears flag bit 6, re-points the header from
 * gEnemyDefaultAnimMap to gCrusherAnimMap, copies the level
 * record's +4/+8/+0xc words into the header's +0x30 box, and shows it
 * with SetEnemyState(hdr, 4). */
void SpawnWoodenCrusher(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;
    struct entity_params *rec;
    struct entity_params *rec2;

    part->anim = POPUP_ANIM(0xd8);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    hdr = CreateEnemyCtrl(OperatorNew(0x8c));
    POPUP_ATTACH(hdr, part);
    hdr->kind = ENEMY_KIND_WOODEN_CRUSHER;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartKind(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    SetEnemyAnimMap(hdr, gEnemyDefaultAnimMap);
    rec2 = LEVEL_RECORD(arg3);
    part->base.kind = 0xa;
    AndPartFlags(part, ~0x40);
    hdr->anims = gCrusherAnimMap;
    SetEnemyAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                        rec2->p.cycle.cycleOffset);
    SetEnemyState(hdr, 4);
}

#include "core.h"
#include "text_popup.h"

/* "Two-line text popup" spawner. Built with old_agbcc; see
 * include/text_popup.h. */

extern u8 gSquidAnimMap[];

/* Text popup, tag 7: the plainest member of the family. Shows the header
 * with gSquidAnimMap and style 7. */
void SpawnSquid(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x54);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x8c);
    hdr = CreateEnemyCtrl();
    POPUP_ATTACH(hdr, part);
    hdr->kind = 7;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    AddToPartList(gUnknown_030012F0, part);
    hdr->animMap = gSquidAnimMap;
    SetEnemyState(hdr, 7);
}

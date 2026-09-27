#include "core.h"
#include "text_popup.h"

/* "Two-line text popup" spawner. Built with old_agbcc; see
 * include/text_popup.h. */

extern u8 gStaticData_0816BA6C[];

/* Text popup, tag 7: the plainest member of the family. Shows the header
 * with gStaticData_0816BA6C and style 7. */
void sub_801FDEC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct popup_hdr *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x54);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x8c);
    hdr = sub_800CA74();
    POPUP_ATTACH(hdr, part);
    hdr->tag = 7;
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetPartField0A(part, 1);
    part->base.flags &= 0x7f;
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    hdr->gfx = gStaticData_0816BA6C;
    sub_800C6A8(hdr, 7);
}

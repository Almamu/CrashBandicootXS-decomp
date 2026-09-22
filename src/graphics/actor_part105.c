#include "core.h"

/* A frame-tick counter, incremented every category-load-loop tick
 * (see InitActorCategory, still raw) and snapshotted into
 * gUnknown_0300138C at the top of SelectActorCategory (also still raw)
 * for the "how long has this sub-effect run" bookkeeping the
 * gUnknown_03001400 sub_effect_table accessors use. */
extern s32 gUnknown_0300138C;
extern s32 gUnknown_03000878;

void sub_8029720(void)
{
    gUnknown_0300138C++;
}

s32 sub_8029730(void)
{
    return gUnknown_0300138C;
}

s32 sub_802973C(void)
{
    return gUnknown_03000878;
}

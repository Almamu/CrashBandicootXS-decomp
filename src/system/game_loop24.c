#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_801071C`/
 * `sub_801075C`/the two Bresenham-line helpers `sub_8010784`/
 * `sub_80107C4` right before `sub_8010804` are left untouched raw. */

struct actor_list {
    s32 count;
    s32 unused_4;
    void **items;
};

extern struct actor_list *gUnknown_0300130C;
extern s32 sub_803AD7C(void *addr, void *fn);
extern void sub_800F5B8(void *self);

/* Walks the `gUnknown_0300130C` object list (the same list/table
 * layout `sub_800F1B8`/`sub_800F258` elsewhere in this raw region
 * read); for each entry whose own `+0x18`-table `+0x48` trampoline
 * (`sub_803AD7C`) reports state `3` and whose `+0x54` countdown isn't
 * disabled (`-1`), truncates that countdown into `+0x48` and fires
 * `sub_800F5B8` on it - a "state-3 countdown expiry" sweep. */
void sub_8010804(void)
{
    s32 i = 0;

    if (i < gUnknown_0300130C->count) {
        struct actor_list **listAddr = &gUnknown_0300130C;
        do {
            u8 *e = (*listAddr)->items[i];
            u8 *rec = *(u8 **)(e + 0x18) + 0x48;
            s16 offset = *(s16 *)rec;
            void *addr = e + offset;
            void *fn = *(void **)(rec + 4);

            if (sub_803AD7C(addr, fn) == 3) {
                s32 v = *(s32 *)(e + 0x54);
                if (v != -1) {
                    *(u32 *)(e + 0x48) = (u8)v;
                    sub_800F5B8(e);
                }
            }
            i++;
        } while (i < (*listAddr)->count);
    }
}

extern void *gUnknown_030012D8;
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* If the viewport's `+0xc` bit 7 flag is set, fires its own `+0x18`
 * table's `+0x68` trampoline pair (`sub_803AD88`, action `0x1a`) and
 * plays cue 1 - the same `+0x18`-table/trampoline-pair convention
 * `sub_801085C`'s sibling functions in this subsystem use throughout. */
void sub_801085C(void)
{
    u8 *self = (u8 *)gUnknown_030012D8;
    register u8 flags asm("r1") = self[0xc];
    register u32 bit asm("r0");

    /* Inline-asm-anchored: this compiler always shifts in place
     * (`lsrs r1,r1,#7`) regardless of C phrasing, while the ROM keeps
     * the loaded byte in r1 and the shifted bit in a separate r0 -
     * see the same gap in sub_800FEB0 (game_loop17.c). */
    asm volatile("lsr r0, r1, #7" : "=r"(bit) : "r"(flags));

    if (bit != 0) {
        u8 *rec = *(u8 **)(self + 0x18) + 0x68;
        s16 offset = *(s16 *)rec;
        void *addr = self + offset;
        register void *fn asm("r4") = *(void *volatile *)(rec + 4);

        sub_803AD88(addr, 0, 0x1a, 0);
        (void)fn;
        PlaySfx(gUnknown_030012BC, 1, 0x100);
    }
}

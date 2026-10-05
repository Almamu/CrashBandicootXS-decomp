#include "core.h"
#include "crate.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `DestroyCrate`/
 * `InitCrate`/the two Bresenham-line helpers `FindLineCrossingYMajor`/
 * `FindLineCrossingXMajor` right before `ConvertCratesForTimeTrial` are left untouched raw. */

extern struct crate_list *gCrateList;
extern s32 _call_via_r1(void *addr, void *fn);
extern void SolidifyOutlineCrate(void *self);

/* Walks the `gCrateList` object list (the same list/table
 * layout `UpdateCrates`/`DetonateNitroCrates` elsewhere in this raw region
 * read); for each box (vtable `m48`, the class id, reports `3`) whose
 * `unk_54` countdown isn't disabled (`-1`), truncates that countdown
 * into `u48` and fires `SolidifyOutlineCrate` on it - a "box countdown expiry"
 * sweep. */
void ConvertCratesForTimeTrial(void)
{
    s32 i = 0;

    if (i < gCrateList->count) {
        struct crate_list **listAddr = &gCrateList;
        do {
            struct crate *e = (*listAddr)->items[i];
            struct method *rec = &e->vtable->m48;
            s16 offset = rec->thisOffset;
            void *addr = (u8 *)e + offset;
            void *fn = rec->fn;

            if (_call_via_r1(addr, fn) == 3) {
                s32 v = e->trialKind;
                if (v != -1) {
                    e->u48.solidKind = (u8)v;
                    SolidifyOutlineCrate(e);
                }
            }
            i++;
        } while (i < (*listAddr)->count);
    }
}

extern void *gAudioContext;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* If the viewport's `+0xc` bit 7 flag is set, fires its own `+0x18`
 * table's `+0x68` trampoline pair (`_call_via_r4`, action `0x1a`) and
 * plays cue 1 - the same `+0x18`-table/trampoline-pair convention
 * `OpenAkuAkuCrate`'s sibling functions in this subsystem use throughout. */
void OpenAkuAkuCrate(void)
{
    u8 *self = (u8 *)gPlayer;
    register u8 flags asm("r1") = self[0xc];
    register u32 bit asm("r0");

    /* Inline-asm-anchored: this compiler always shifts in place
     * (`lsrs r1,r1,#7`) regardless of C phrasing, while the ROM keeps
     * the loaded byte in r1 and the shifted bit in a separate r0 -
     * see the same gap in ResetCrate (crate_reset.c). */
    asm volatile("lsr r0, r1, #7" : "=r"(bit) : "r"(flags));

    if (bit != 0) {
        u8 *rec = *(u8 **)(self + 0x18) + 0x68;
        s16 offset = *(s16 *)rec;
        void *addr = self + offset;
        register void *fn asm("r4") = *(void *volatile *)(rec + 4);

        _call_via_r4(addr, 0, 0x1a, 0);
        (void)fn;
        PlaySfx(gAudioContext, 1, 0x100);
    }
}

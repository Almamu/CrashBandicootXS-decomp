#include "core.h"
#include "gba/io_reg.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;
extern void sub_80392E0(const u8 *msg1, const u8 *msg2);
extern u8 gStaticData_085A6214[];
extern u8 gStaticData_085A621C[];

/* GAX2's per-frame DMA1/Timer0 direct-sound-output follow-up to
 * sub_8038538's play-start: once a song is loaded (magic == "GAX2") and
 * `state` is non-zero, a fresh `state == 1` (just-started) primes
 * SOUNDCNT_X, advances `state` to 2, and reloads Timer0 from
 * `field_34`'s per-song sample-rate divisor; if the song header asks for
 * fatal errors to be shown (`showErrors`) and one hasn't already been
 * reported (`field_43`), shows GAX2's fatal-error screen (sub_80392E0);
 * finally, when `field_2c == 1`, re-arms DMA1 for Direct Sound A output
 * from `field_18` (same DMA1CNT_H settle-delay quirk as sub_80384DC) and
 * clears the `field_43` flag. */
void sub_8038B68(void)
{
    if (gUnknown_03001630->magic != 0x47415832) {
        return;
    }
    if (gUnknown_03001630->state == 0) {
        return;
    }
    /* Volatile-forced re-read: the ROM re-loads `state` a second time here
     * (rather than reusing the register from the `== 0` check above) - a
     * gcc-2.9 CSE difference this scoped volatile cast reproduces without
     * marking the field volatile project-wide (matching_decomp_register_
     * pinning memory's "scoped-volatile casts" technique). */
    if (*(vu32 *)&gUnknown_03001630->state == 1) {
        struct GaxPlayerState *p = gUnknown_03001630;

        REG_SOUNDCNT_X = 0x80;
        p->state = 2;
        REG_TM0CNT_H = 0;
        REG_TM0CNT = (0x10000 - p->field_34) | 0x00C00000;
    }

    {
        struct GaxSongHeader *songData = gUnknown_03001630->songPtr;

        if (songData->showErrors != 0 && gUnknown_03001630->field_43 == 0) {
            sub_80392E0(gStaticData_085A6214, gStaticData_085A621C);
        }
    }

    if (gUnknown_03001630->field_2c == 1) {
        struct GaxPlayerState *p = gUnknown_03001630;

        REG_DMA1CNT_H = 0x8640;
        /* Real hardware settle delay, not padding - see sub_80384DC's
         * doc comment in gax_hw_reset.c for why this can't be written
         * as plain "adds r3, r3, #0" text. */
        asm(".byte 0x1b, 0x1c\n\t"
            "mov r8, r8\n\t"
            "mov r8, r8\n\t"
            "mov r8, r8");
        REG_DMA1CNT_H = 0xc8 << 3;
        REG_DMA1SAD = p->field_18;
        REG_DMA1CNT_H = 0xB660;
    }

    gUnknown_03001630->field_43 = 0;
}

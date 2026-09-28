#include "core.h"
#include "audio.h"

/* sub_803AD7C is libgcc's `_call_via_r1` (the mixer's `type->init`). */
asm(".set _call_via_r1, sub_803AD7C\n");

/* GAX2's fatal-error screen messages. */
extern const char gStaticData_085A61EC[];
extern const char gStaticData_085A61F8[];
extern const char gStaticData_085A61D0[];
extern const char gStaticData_085A61DC[];
extern void sub_80392E0(const char *a, const char *b);
extern void sub_8037F3C(void *dest, s32 count);
extern u8 sub_8038240(struct GaxHandlerLayout *layout, struct GaxHandlerType **sfx, u32 numSfx, u8 **bufp,
                      u32 *sizep);

/* Builds player 1 (the sound-effect player) out of the caller-supplied
 * work buffer (`gUnknown_03001630->workBuf`/`workSize`): refuses (with
 * GAX2's fatal-error screen, if the song asks for it) when the song's
 * flag 0x10 is set; otherwise clears the buffer, carves player 1's
 * handler array plus every handler out of it (`sub_8038240`), links
 * the song's channels in as the mixer's SFX voices, and runs the
 * mixer's `init`. Returns 1 on success, 0 (again with the optional
 * fatal-error screen) if the buffer is too small.
 *
 * Was NAKED ("r8/sb allocation ceiling"); written plainly against the
 * player/handler structs in include/audio.h it matches outright - the
 * one subtlety is writing the "0" stores as constants (the ROM reuses
 * the register holding the known-zero flag test for them) - see
 * docs/matching/gax-toolchain-retry.md. */
u32 sub_8038A1C(struct GaxHandlerLayout *layout)
{
    u8 *buf = gUnknown_03001630->workBuf;
    u32 size = gUnknown_03001630->workSize;
    u16 busy = GAX_SONG()->flags & 0x10;
    u32 n;
    struct GaxHandlerType **sfx;
    struct GaxSongHeader *song;

    if (busy) {
        if (GAX_SONG()->showErrors)
            sub_80392E0(gStaticData_085A61EC, gStaticData_085A61F8);
        return 0;
    }
    sub_8037F3C(buf, size);
    gUnknown_03001630->state = 0;
    gUnknown_03001630->curChannelIdx = 1;
    n = layout->count;
    song = GAX_SONG();
    sfx = song->sfxTypes;
    if (sfx)
        n += song->numSfx;
    gUnknown_03001630->channels[1] = buf;
    n *= 4;
    if (size < n) {
        gUnknown_03001630->state = 1;
        gUnknown_03001630->curChannelIdx = 0;
    } else {
        buf += n;
        size -= n;
        if (sub_8038240(layout, sfx, song->numSfx, &buf, &size)) {
            GAX_MIXER()->extraChildren = GAX_SONG()->numSfx;
            GAX_MIXER()->field_10 = gUnknown_03001630->field_1c;
            GAX_MIXER()->type->init(GAX_MIXER());
            GAX_INFO()->field_20 = 1;
            GAX_SONG()->field_39 = 0;
            GAX_SONG()->field_3a = 0;
            gUnknown_03001630->field_41 = 1;
            gUnknown_03001630->state = 1;
            return 1;
        }
        gUnknown_03001630->state = 1;
        gUnknown_03001630->curChannelIdx = 0;
    }
    if (GAX_SONG()->showErrors)
        sub_80392E0(gStaticData_085A61D0, gStaticData_085A61DC);
    return 0;
}

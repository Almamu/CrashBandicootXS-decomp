#include "core.h"

/* GAX2_SoundHandler "Channel" type's unknown_fn (ROM 0x080395A1, see
 * docs/audio.md's per-type function-pointer table) - a no-op stub, same
 * as the "Info" type's unknown_fn (nullsub_39, gax_sound_handler_info.c).
 * The Channel type's init_fn (sub_8039518) and play_fn (sub_80395A4) are
 * still raw. */
void nullsub_40(void)
{
}
asm(".align 2, 0");

#include "core.h"

/* Called with a small command value (`cmd`, 0-3 seen at the call site in
 * the still-raw `sub_8039658`) against a per-channel voice object
 * (`self`). `cmd == 1` is the "note cut" case: if the voice's currently
 * bound instrument (`+0x3c`) has its second byte (`+0x7d`) set to the
 * sentinel `0xff`, arms a fixed-pitch/zero-volume note-off envelope
 * (`+0x2a`/`+0x2c`/`+0x4c`) before setting the "stopped" flag (`+0x22`);
 * otherwise the fixed note-off envelope is skipped but the flag still
 * gets set. `cmd > 1` additionally derives a pattern-note-looking value
 * from `cmd` into `+0x26` and clears the flag again - this project
 * hasn't nailed down the exact channel/voice object shape yet (same
 * situation as the neighboring GAX2 engine internals in
 * gax_note_param.c/gax_sound_handler_info.c), so every field stays a raw
 * offset rather than a guessed struct. */
void sub_8039818(void *self, u32 cmd)
{
    register u32 v asm("r3") = cmd;

    if (v == 1) {
        u8 *inst = *(u8 **)((u8 *)self + 0x3c);
        if (inst != NULL) {
            u8 *inner = *(u8 **)(inst + 0x7c);
            if (inner[1] == 0xff) {
                u16 zero = 0;
                u16 val = 0x8AD0;

                *(u16 *)((u8 *)self + 0x2a) = val;
                *(u16 *)((u8 *)self + 0x2c) = zero;
                *(u32 *)((u8 *)self + 0x4c) = 0x80000000;
            }
        }
        *((u8 *)self + 0x22) = 1;
    }
    if (v > 1) {
        register u32 tmp asm("r0") = v - 2;
        u16 shifted = tmp << 5;
        u8 zero = 0;

        *(u16 *)((u8 *)self + 0x26) = shifted;
        *((u8 *)self + 0x22) = zero;
    }
}

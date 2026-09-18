#include "core.h"
#include "audio.h"

/* `sub_80019F8` sits right after the matched `sub_80019E8`
 * (src/audio/sfx_ambient.c) and before the matched functions this
 * file holds - the rest of the `AudioContext` accessor/state-machine
 * cluster (play/pause/stop, the two fade-envelope arm/setter pairs,
 * the constructor). */

extern u32 gUnknown_0300082C;
extern s32 sub_8038E74(u32 handle, s32 channel, s32 pitchOffset, s32 priority);
extern void sub_80390F8(s32 channel, u32 volume);

/* Requests the ambient-sfx channel play `id` for `frameOffset` frames
 * (relative to `gUnknown_0300082C`) at a volume derived from
 * `volumeMul`/the table's own base volume/`sfxVolume`, same formula as
 * `PlaySfx`. If nothing is currently active, starts it immediately;
 * if something louder-or-equal is already active with the same `id`,
 * refreshes its deadline/volume (optionally retriggering the voice via
 * `forceFlag`); if a different, louder request comes in, queues it as
 * `pendingSfx` and forces the current record to expire on the very
 * next tick instead of playing over it.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) closed one gap (the ROM's `add rX,sp,#0x14; ldrb
 * rX,[rX]` stack-byte-parameter read, via an inline-asm anchor) but
 * left one open - the ROM recomputes
 * `&gStaticData_0816AA6C[id].baseVolume` fully from `tableBase`+
 * offset+8 for the volume read, even though the identical address was
 * already computed for the `slotId` read a few instructions earlier
 * and is still live; this compiler's CSE always reuses that live
 * address instead (fewer instructions), and once folded, the following
 * `baseVolume * volumeMul` multiply's register-copy step also lands on
 * this compiler's generic "mov Rd,Rs" encoding rather than the ROM's
 * "adds Rd,Rs,#0" form. Raw-offset casts, memory-clobber barriers,
 * operand reordering, and pinning the loaded value straight into a
 * register all failed to close it (see git history for the blow-by-
 * blow). Every instruction below is confirmed byte-identical to the
 * ROM - full NAKED transcription, like this project's other
 * hard-compiler-limitation cases (see `src/util/printf_util.c`'s
 * `sub_8000CBC` for the established pattern), is more honest than
 * continuing to chase this one CSE decision through plain C. */
NAKED void sub_80019F8(struct AudioContext *self, u32 id, u32 frameOffset, s32 volumeMul, u8 forceFlag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "add r6, r1, #0\n\t"
        "add r7, r2, #0\n\t"
        "add r0, sp, #0x14\n\t"
        "ldrb r0, [r0]\n\t"
        "mov ip, r0\n\t"
        "ldr r5, 2f\n\t"
        "lsl r0, r6, #1\n\t"
        "add r0, r0, r6\n\t"
        "lsl r1, r0, #2\n\t"
        "add r0, r1, r5\n\t"
        "ldr r2, [r0]\n\t"
        "cmp r2, #0\n\t"
        "beq 1f\n\t"
        "cmp r3, #0\n\t"
        "ble 1f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #8\n\t"
        "add r0, r1, r0\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r0, #0\n\t"
        "mul r1, r3, r1\n\t"
        "ldr r0, [r4, #0x2c]\n\t"
        "mul r0, r1, r0\n\t"
        "lsr r5, r0, #0x10\n\t"
        "ldr r1, [r4, #0x38]\n\t"
        "cmp r1, #0x63\n\t"
        "bne 4f\n\t"
        "mov r3, #1\n\t"
        "neg r3, r3\n\t"
        "add r0, r2, #0\n\t"
        "mov r1, #2\n\t"
        "mov r2, #0\n\t"
        "bl sub_8038E74\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "mov r0, #2\n\t"
        "bl sub_80390F8\n\t"
        "str r6, [r4, #0x38]\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r7\n\t"
        "str r0, [r4, #0x3c]\n\t"
        "str r5, [r4, #0x40]\n\t"
        "b 1f\n\t"
        ".align 2, 0\n\t"
    "2: .4byte gStaticData_0816AA6C\n\t"
    "3: .4byte gUnknown_0300082C\n\t"
    "4:\n\t"
        "ldr r0, [r4, #0x40]\n\t"
        "cmp r5, r0\n\t"
        "blt 1f\n\t"
        "cmp r1, r6\n\t"
        "bne 7f\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r7\n\t"
        "str r0, [r4, #0x3c]\n\t"
        "str r5, [r4, #0x40]\n\t"
        "mov r0, ip\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "mov r3, #1\n\t"
        "neg r3, r3\n\t"
        "add r0, r2, #0\n\t"
        "mov r1, #2\n\t"
        "mov r2, #0\n\t"
        "bl sub_8038E74\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "mov r0, #2\n\t"
        "bl sub_80390F8\n\t"
    "6:\n\t"
        "mov r0, #0x63\n\t"
        "str r0, [r4, #0x44]\n\t"
        "mov r0, #0\n\t"
        "str r0, [r4, #0x4c]\n\t"
        "b 1f\n\t"
        ".align 2, 0\n\t"
    "5: .4byte gUnknown_0300082C\n\t"
    "7:\n\t"
        "str r6, [r4, #0x44]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r1, r7\n\t"
        "str r0, [r4, #0x48]\n\t"
        "str r5, [r4, #0x4c]\n\t"
        "str r1, [r4, #0x3c]\n\t"
    "1:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    "8: .4byte gUnknown_0300082C\n\t"
    );
}

extern void sub_8039064(s32 channel, u32 volume);
extern void sub_8039198(void);
extern u8 gUnknown_030007DD;
extern void sub_8038C50(void);
extern void sub_8038C88(void);
extern void sub_8038C28(void);
extern void sub_80006A8(void);
extern void sub_80016D0(u8 *address);
extern void sub_8000558(s32 interruptIndex);
extern void sub_80017BC(struct AudioContext *self, u32 songIndex);

/* currentSong getter. */
u32 sub_8001AB8(struct AudioContext *self)
{
    return self->currentSong;
}

/* sfxVolume getter. */
u32 sub_8001ABC(struct AudioContext *self)
{
    return self->sfxVolume;
}

/* duckVolDefault getter. */
u32 sub_8001AC0(struct AudioContext *self)
{
    return self->duckVolDefault;
}

/* Arms a duck-out: sets the ducking fade target and the "fade down"
 * direction flag. */
void sub_8001AC4(struct AudioContext *self, u32 value)
{
    self->duckVolTarget = value;
    self->duckVolFadeUpArmed = 0;
    self->duckVolFadeDownArmed = 1;
}

/* Arms a duck-in back to `duckVolDefault` (the last value explicitly
 * set via `sub_8001B30`). */
void sub_8001AD8(struct AudioContext *self)
{
    self->duckVolTarget = self->duckVolDefault;
    self->duckVolFadeUpArmed = 1;
    self->duckVolFadeDownArmed = 0;
}

/* Arms a fade-out on the independent `musicVolCurrent`/`Target` pair. */
void sub_8001AEC(struct AudioContext *self, u32 value)
{
    self->musicVolTarget = value;
    self->musicVolFadeUpArmed = 0;
    self->musicVolFadeDownArmed = 1;
}

/* Arms a fade-in on the independent `musicVolCurrent`/`Target` pair. */
void sub_8001B00(struct AudioContext *self, u32 value)
{
    self->musicVolTarget = value;
    self->musicVolFadeUpArmed = 1;
    self->musicVolFadeDownArmed = 0;
}

/* Setter for `field_30`, hardware-mirrored (truncated) into the
 * embedded GAX object's own field at `self+0x62` while a song is
 * playing - see include/audio.h. */
void sub_8001B14(struct AudioContext *self, u32 value)
{
    s32 isPlaying;

    self->field_30 = value;
    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        *(vu16 *)((u8 *)self + 0x62) = value;
    }
}

/* Immediate (non-fading) music-volume setter - also becomes the new
 * `duckVolDefault` a later `sub_8001AD8` duck-in restores to. */
void sub_8001B30(struct AudioContext *self, u32 value)
{
    s32 isPlaying;

    self->duckVolCurrent = value;
    self->duckVolDefault = value;
    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        sub_8039064(-1, value);
    }
}

/* sfxVolume setter. */
void sub_8001B50(struct AudioContext *self, u32 value)
{
    self->sfxVolume = value;
}

/* Requests song `id` to play: if nothing is playing yet, starts it
 * immediately (`sub_80017BC`); otherwise, if it's not already the
 * current or already-queued song, queues it as `pendingSong` and arms
 * a duck-out (`sub_8001AC4`) - `sub_80016EC`'s per-tick update starts
 * the queued song once the duck-out fade completes. */
void sub_8001B54(struct AudioContext *self, u32 id)
{
    s32 isPlaying = 0;

    if (self->state == 1) {
        isPlaying = 1;
    }
    if (!isPlaying) {
        sub_80017BC(self, id);
        return;
    }
    if (id == self->currentSong) {
        return;
    }
    if (id == self->pendingSong) {
        return;
    }
    self->pendingSong = id;
    sub_8001AC4(self, 0);
}

/* Resumes a paused song. */
void sub_8001B88(struct AudioContext *self)
{
    s32 isPaused;

    isPaused = 0;
    if (self->state == 2) {
        isPaused = 1;
    }
    if (isPaused) {
        sub_80006A8();
        sub_8038C50();
        self->state = 1;
    }
}

/* Pauses a playing song. */
void sub_8001BAC(struct AudioContext *self)
{
    s32 isPlaying;

    isPlaying = 0;
    if (self->state == 1) {
        isPlaying = 1;
    }
    if (isPlaying) {
        self->state = 2;
        sub_80006A8();
        sub_8038C88();
        sub_8038C28();
    }
}

/* Stops the currently playing/paused song (a no-op if already
 * stopped) and disarms the per-tick GAX2 IRQ update. */
void sub_8001BD4(struct AudioContext *self)
{
    register s32 isStopped asm("r2");
    register s32 zero asm("r4");

    isStopped = 0;
    if (self->state == 0) {
        isStopped = 1;
    }
    zero = isStopped;
    if (zero == 0) {
        self->pendingSong = 0x13;
        self->currentSong = 0x13;
        self->state = zero;
        sub_8039198();
        gUnknown_030007DD = zero;
    }
}

/* Stops the song, and if `flags` bit 0 is set, also frees `self`. */
void sub_8001C04(struct AudioContext *self, u32 flags)
{
    sub_8001BD4(self);
    gUnknown_030007DD = 0;
    if (flags & 1) {
        sub_80016D0((u8 *)self);
    }
}

/* Constructor: resets every field to its idle default (both volume
 * pairs to `0x100` = 1.0 in Q8.8, both song slots to the `0x13` "none"
 * sentinel, every fade-direction flag cleared) and returns `self`. */
struct AudioContext *sub_8001C2C(struct AudioContext *self)
{
    self->state = 0;
    self->pendingSong = 0x13;
    self->currentSong = 0x13;
    self->musicVolCurrent = 0x100;
    self->duckVolCurrent = 0x100;
    self->duckVolDefault = 0x100;
    self->sfxVolume = 0x100;
    self->field_34 = 0;
    self->activeSfx.id = 0x63;
    self->musicVolFadeDownArmed = 0;
    self->musicVolFadeUpArmed = 0;
    self->duckVolFadeDownArmed = 0;
    self->duckVolFadeUpArmed = 0;
    self->field_54 = 0;
    self->field_30 = 0;
    return self;
}

/* Disables the GBA's V-Count interrupt - a counterpart to
 * `sub_8000654` (VBlank) in src/system/irq.c. */
void sub_8001C64(void)
{
    register vu8 *dispstat asm("r1") = REG_ADDR_DISPSTAT;
    u8 tmp = DISPSTAT_VCOUNT_INTR;

    *dispstat &= ~tmp;
    sub_8000558(INTR_INDEX_VCOUNT);
}
asm(".align 2, 0");

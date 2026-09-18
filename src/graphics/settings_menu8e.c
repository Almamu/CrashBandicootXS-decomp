#include "core.h"
#include "audio.h"
#include "settings_sync.h"

extern void sub_800014C(void *dst, void *src, s32 len);
extern void sub_8002C6C(struct settings_sync_record *self, s32 row);
extern void sub_8002B70(struct settings_sync_record *self);

/* Validates the record's checksum (inline word-sum, same shape as
 * `sub_8002B44` below but not a call to it - the ROM genuinely inlines
 * this one) and, if it fails, repairs the record in place: DMA-fills
 * the whole 0x200 bytes with 0 (raw DMA3 register pokes rather than a
 * `DmaFill16` call - a different, earlier style than
 * `src/graphics/settings_menu8.c`'s `sub_8002C84` uses for the same
 * "reset to blank" operation), marks every row selected
 * (`sub_8002C6C`), re-stamps the two marker bytes, clears
 * `flags`/`field_1fb`, and refreshes the checksum (`sub_8002B70`).
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * is semantically confirmed against the ROM (the paragraph above is
 * that derivation) - the ROM caches 4 field addresses
 * (self+0x1f8/0x1f9/0x1fa/0x1fb) into r6/sb/r7/r8 ahead of the row loop,
 * which explicit register pins reproduced exactly for the loop body and
 * post-loop field writes, but the *prologue*'s `sb`/`r8`-via-`r7`/`r6`
 * push shuffle never came out right (this compiler kept settling on a
 * smaller push list, treating r7 as not needing protection across
 * `sub_8002C6C`'s calls unlike the ROM) - the same unresolved gcc-2.9
 * scratch/callee-saved-register-choice class this project documents at
 * length elsewhere. Full NAKED transcription like
 * `sub_8001CB8`/`sub_8001DB4`/`sub_8002868`/`sub_8002938`. */
NAKED void sub_8002AA4(struct settings_sync_record *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "add r3, r5, #0\n\t"
        "mov r2, #0\n\t"
        "mov r1, #0x7e\n\t"
    "1:\n\t"
        "ldm r3!, {r0}\n\t"
        "add r2, r2, r0\n\t"
        "sub r1, #1\n\t"
        "cmp r1, #0\n\t"
        "bge 1b\n\t"
        "mov r1, #0\n\t"
        "mov r3, #0xfe\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r5, r3\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r2, r0\n\t"
        "bne 2f\n\t"
        "mov r1, #1\n\t"
    "2:\n\t"
        "cmp r1, #0\n\t"
        "bne 3f\n\t"
        "mov r0, sp\n\t"
        "strh r1, [r0]\n\t"
        "ldr r0, 5f\n\t"
        "mov r1, sp\n\t"
        "str r1, [r0]\n\t"
        "str r5, [r0, #4]\n\t"
        "ldr r1, 6f\n\t"
        "str r1, [r0, #8]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "mov r4, #0\n\t"
        "mov r2, #0xfc\n\t"
        "lsl r2, r2, #1\n\t"
        "add r6, r5, r2\n\t"
        "ldr r3, 7f\n\t"
        "add r3, r3, r5\n\t"
        "mov sb, r3\n\t"
        "mov r0, #0xfd\n\t"
        "lsl r0, r0, #1\n\t"
        "add r7, r5, r0\n\t"
        "ldr r1, 8f\n\t"
        "add r1, r1, r5\n\t"
        "mov r8, r1\n\t"
    "4:\n\t"
        "add r0, r5, #0\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8002C6C\n\t"
        "add r4, #1\n\t"
        "cmp r4, #3\n\t"
        "ble 4b\n\t"
        "mov r0, #0\n\t"
        "mov r1, #0x43\n\t"
        "strb r1, [r6]\n\t"
        "mov r1, #0x12\n\t"
        "mov r2, sb\n\t"
        "strb r1, [r2]\n\t"
        "strb r0, [r7]\n\t"
        "mov r3, r8\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8002B70\n\t"
    "3:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "5: .4byte 0x040000D4\n"
    "6: .4byte 0x81000100\n"
    "7: .4byte 0x000001F9\n"
    "8: .4byte 0x000001FB\n"
    );
}

/* Recomputes this record's additive word-sum checksum over its first
 * 0x1fc bytes (127 words) and compares it against the stored
 * `checksum` field, returning 1 on match. Counterpart to `sub_8002B70`
 * below, which stores instead of comparing. */
u32 sub_8002B44(struct settings_sync_record *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;
    register u32 result asm("r1");

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }

    result = 0;
    if (sum == self->checksum) {
        result = 1;
    }
    return result;
}

/* Recomputes and stores this record's additive word-sum checksum over
 * its first 0x1fc bytes (127 words) into `checksum`. Every function
 * that mutates `flags`/`rowSelected` calls this afterward to keep the
 * checksum in sync - see src/graphics/settings_menu8.c's header
 * comment, which already anticipated this function (it was matched
 * from a later chunk, issue #5, before this one). */
void sub_8002B70(struct settings_sync_record *self)
{
    u32 *p = (u32 *)self;
    u32 sum = 0;
    s32 i;

    for (i = 0x7e; i >= 0; i--) {
        sum += *p++;
    }
    self->checksum = sum;
}

/* `versionNibble`'s high-nibble accessor. */
u32 sub_8002B94(struct settings_sync_record *self)
{
    return self->versionNibble >> 4;
}

extern struct AudioContext *gUnknown_030012BC;
extern u32 sub_8001AB8(struct AudioContext *self);
extern void sub_8001BD4(struct AudioContext *self);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern s32 sub_8002938(void *self, s32 len);

/* Saves the settings record to EEPROM (`sub_8002938`, retried up to 5
 * times), muting the music player across the transfer the same way
 * `sub_8002A08` (src/graphics/settings_menu8d.c) does (checksum
 * refreshed first via `sub_8002B70`, before the mute). Returns 4
 * (EEPROM write failed after retries) or 0 (success). */
s32 sub_8002BA4(struct settings_sync_record *self)
{
    struct AudioContext *audio;
    s32 flag;
    s32 wasPlaying;
    u32 savedSong;
    s32 i;
    s32 result;

    audio = gUnknown_030012BC;
    flag = 0;
    if (audio->state == 1) {
        flag = 1;
    }
    wasPlaying = flag;

    savedSong = sub_8001AB8(audio);
    sub_8002B70(self);

    if (wasPlaying) {
        sub_8001BD4(gUnknown_030012BC);
    }

    i = 0;
    do {
        result = sub_8002938(self, 0x200);
        i++;
    } while (i <= 4 && result != 0);

    if (wasPlaying) {
        sub_8001B54(gUnknown_030012BC, savedSong);
    }

    if (result != 0) {
        return 4;
    }
    return 0;
}

/* Copies row `row`'s 0x70-byte slot out to `dst`, but only if the row
 * hasn't been explicitly selected/edited (`rowSelected[row] == 0`) -
 * refreshes a caller-side scratch copy with the stored default/synced
 * value while leaving a user-edited row alone. */
void sub_8002C14(struct settings_sync_record *self, s32 row, void *dst)
{
    if (self->rowSelected[row] == 0) {
        register s32 offset asm("r1");

        offset = row * 0x70;
        offset = offset + (s32)self;
        sub_800014C(dst, (void *)offset, 0x70);
    }
}

/* Force-writes `src` into row `row`'s 0x70-byte slot, clears the row's
 * `rowSelected` flag (marking it "not explicitly edited" again), and
 * refreshes the checksum. */
void sub_8002C40(struct settings_sync_record *self, s32 row, void *src)
{
    register s32 offset asm("r0");

    self->rowSelected[row] = 0;
    offset = row * 0x70;
    offset = offset + (s32)self;
    sub_800014C((void *)offset, src, 0x70);
    sub_8002B70(self);
}

/* Marks row `row` as explicitly selected/edited and refreshes the
 * checksum. See `include/settings_sync.h`'s `rowSelected` field
 * comment, which already anticipated this function (matched from a
 * later chunk, issue #5, before this one). */
void sub_8002C6C(struct settings_sync_record *self, s32 row)
{
    self->rowSelected[row] = 1;
    sub_8002B70(self);
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead (see
 * docs/matching.md's alignment-padding gotcha / the
 * matching_decomp_alignment_fix convention). */
asm(".align 2, 0");

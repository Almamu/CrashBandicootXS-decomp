#include "core.h"
#include "audio.h"
#include "settings_sync.h"

extern void sub_800014C(void *dst, void *src, s32 len);
extern void sub_8002C6C(struct settings_sync_record *self, s32 row);
extern void sub_8002B70(struct settings_sync_record *self);

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: semantics fully understood. Validates the
 * record's checksum (inline word-sum, same shape as `sub_8002B44`
 * below but not a call to it - the ROM genuinely inlines this one)
 * and, if it fails, repairs the record in place: DMA-fills the whole
 * 0x200 bytes with 0 (raw DMA3 register pokes rather than a
 * `DmaFill16` call - a different, earlier style than
 * `src/graphics/settings_menu8.c`'s `sub_8002C84` uses for the same
 * "reset to blank" operation), marks every row selected
 * (`sub_8002C6C`), re-stamps the two marker bytes, clears
 * `flags`/`field_1fb`, and refreshes the checksum (`sub_8002B70`).
 * Every load/store, branch and call is semantically confirmed - the
 * ROM caches 4 field addresses (self+0x1f8/0x1f9/0x1fa/0x1fb) into
 * r6/sb/r7/r8 ahead of the row loop and this reconstruction does the
 * same via explicit register pins, which got the loop body and the
 * post-loop field writes to match exactly, but the *prologue* still
 * differs: the ROM's push list is `{r4,r5,r6,r7,lr}` then a
 * `sb`/`r8`-via-`r7`/`r6` shuffle-push, while this compiler settles on
 * a smaller `{r4,r5,r6,lr}` push shuffled via `r5`/`r6` instead (r7
 * apparently doesn't need protecting across `sub_8002C6C`'s calls in
 * this compiler's allocation, unlike the ROM's) - the same
 * unresolved gcc-2.9 scratch/callee-saved-register-choice class this
 * project documents at length elsewhere. Real bytes stay in
 * `asm/code_3_1_10_3_2aa4.s`, wrapped `.if NON_MATCHING == 0`. */
void sub_8002AA4(struct settings_sync_record *arg0)
{
    register struct settings_sync_record *self asm("r5");
    register u32 *p asm("r3");
    register u32 sum asm("r2");
    register s32 i asm("r1");
    register u32 matched asm("r1");
    u16 zero;
    register u8 *field1f8Addr asm("r6");
    register u8 *versionAddr asm("sb");
    register u8 *flagsAddr asm("r7");
    register u8 *field1fbAddr asm("r8");
    register s32 j asm("r4");

    self = arg0;
    p = (u32 *)self;
    sum = 0;
    i = 0x7e;
    do {
        sum += *p;
        p++;
        i--;
    } while (i >= 0);

    matched = 0;
    if (sum == self->checksum) {
        matched = 1;
    }

    if (matched == 0) {
        zero = matched;
        REG_DMA3SAD = (u32)&zero;
        REG_DMA3DAD = (u32)self;
        REG_DMA3CNT = 0x81000100;
        (void)REG_DMA3CNT;

        j = 0;
        field1f8Addr = &self->field_1f8;
        versionAddr = &self->versionNibble;
        flagsAddr = &self->flags;
        field1fbAddr = &self->field_1fb;
        do {
            sub_8002C6C(self, j);
            j++;
        } while (j <= 3);

        *field1f8Addr = 0x43;
        *versionAddr = 0x12;
        *flagsAddr = 0;
        *field1fbAddr = 0;
        sub_8002B70(self);
    }
}
#endif /* NON_MATCHING */

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

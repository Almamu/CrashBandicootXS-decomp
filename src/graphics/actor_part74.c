#include "core.h"
#include "actor_self.h"

/* Sits right after actor_part58.c's `sub_802D764` and before
 * actor_part59.c's `sub_802DB2C`/`sub_802DCC0` - the whole contiguous
 * range that used to be `asm/code_3_2_20_28568_c99c_d7b0.s`. All three
 * functions here operate on the `gUnknown_030014BC`-rooted "position-
 * tracking object with tier-threshold sound cues" documented in
 * actor_part59.c's header comment and docs/matching/issue-54-actor-d3a8.md
 * (the "third RAM-struct family" from docs/rom_map.md). See that issue
 * doc's "Second pass" section for how the 12-byte AABB-record layout
 * used here and by `sub_802DD9C` (actor_part75.c) was finally pinned
 * down.
 *
 * Built with old_agbcc: `sub_802DA68` only matches under it, and
 * `sub_802D9A8` matches under both. */

extern s32 gUnknown_030014D0;
extern s32 gUnknown_030014C4;
extern struct actor_self *gUnknown_03000884;
extern struct actor_self *gUnknown_030014BC;
extern s32 GetAnimFrameBaseOffset(void *self);
extern void (*gStaticData_0817A840[])(void);
extern void sub_803AD78(void *fn);
extern void (*gUnknown_03000898)(void *frame, s32 arg);
extern void sub_803AD80(void *arg0, s32 arg1, void *fn);
extern u8 gUnknown_030014C0;
extern u8 gUnknown_030014C1;
extern s32 gUnknown_030014CC;
extern void sub_8029E34(s32 arg0);
extern void sub_802D9A8(void);
extern u8 gStaticData_0817AA98[];
extern s32 gUnknown_030014C8;
extern u8 gUnknown_030014A0;
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_802C018(void *self);
extern void sub_8029BAC(s32 arg0);
extern u16 gStaticData_0817AA6C[];
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);

asm(".set __divsi3, sub_803ADB4\n"
    ".set _call_via_r0, sub_803AD78");
ACTOR_CALL_VIA_ALIASES

/* One of two confirmed slots (index 3, dispatched via
 * `gStaticData_0817A840[gUnknown_030014D0]`) of the type-0
 * `category_vtable` (`gStaticData_081756C4[0]`, `include/actor_anim.h`)
 * - `UpdateGameFrame`'s own direct top-level callee for this object, per
 * docs/rom_map.md's "Two new type-0 vtable slots confirmed" section.
 *
 * Unless `gUnknown_030014D0 == 3`, first eases `gUnknown_030014C4`
 * toward the player's cached X position (`gUnknown_03000884->+0x1c`,
 * divisor 32 - the same rsb/lsr/add/asr round-toward-zero idiom as
 * `sub_802D3A8`/`sub_80070EC`). Then advances the object's own anim
 * frame (`+8` accumulator by the `+0x10` per-frame increment,
 * `GetAnimFrameBaseOffset` against the current part-table record's
 * `+4`/`+6` timing fields, latching the `+0x12` done flag and correcting
 * the accumulator on overrun), fires the current
 * `gStaticData_0817A840[gUnknown_030014D0]` vtable slot via
 * `sub_803AD78`, and - only when the accumulator's `>>8` value actually
 * changed this frame - fires a `sub_803AD80` trampoline from the part
 * table's own `+2`-offset record (latching `gUnknown_030014C1`).
 *
 * Finally, while `gUnknown_030014D0 <= 1`, runs a 3-axis AABB overlap
 * test between two 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` records
 * (axes compared Z, then Y, then X - matching the ROM's own instruction
 * order, not storage order) built the same way both times: a
 * `gStaticData_0817AA98`-rooted static record with `gUnknown_030014C4`/
 * `030014C8` (both `>>8`) added into its `x`/`z` fields only (this
 * object tracks no Y), against the player's own `+0x38` 12-byte vector
 * with the player's `+0x1c`/`0x20`/`0x24` position (all `>>8`) added
 * into all three of `x`/`y`/`z`. The second record is then run through
 * `sub_800014C` - a real, byte-verified `memcpy(box, box, 0xc)`
 * self-copy (`sub_800014C`'s own definition, `src/system/boot_util.c`,
 * confirmed a plain `memcpy`-style `CpuSet` wrapper) - a genuine no-op
 * kept byte-faithful since a shared "copy src into a working buffer,
 * then test" helper is being called here with a buffer that already
 * *is* its own source, not a disassembly artifact. On overlap, arms
 * `gUnknown_030014D0 = 2`, resets the object's kind/anim state to the
 * part table's `+0x18` record, and refreshes the player via
 * `sub_802C018`/`sub_8029BAC(0)`; skipped once `gUnknown_030014A0` (an
 * already-consumed one-shot flag elsewhere in this ROM region) is set.
 *
 * Written as NAKED asm, not plain C: this function's heavy stack-buffer
 * use (two 12-byte scratch AABB records sharing one 0x24-byte frame,
 * built with raw `ldm`/`stm` block copies) and register reuse (`r5`
 * holds the `gUnknown_030014BC` pointer early on, then gets clobbered
 * with an unrelated accumulator delta later; `r7` similarly holds the
 * vtable dispatch index then the "is `030014D0` <= 1" comparison
 * operand) make this a poor match for gcc 2.9's register allocator
 * without extensive per-register archaeology - each individual
 * instruction's operation/operand/order was fully confirmed against the
 * ROM disassembly (`expected/code_3.s`) first, so this is a mechanical,
 * byte-verified transcription (translated from the disassembler's
 * unified syntax to this project's established NAKED plain/divided
 * syntax, `adds`->`add`/`ands`->`and`/etc, local labels renumbered per
 * docs/matching/issue-4-sio-settings-sync.md's convention), not an
 * inferred control-flow guess.
 *
 * NON_MATCHING draft (issue #51/#54 retry, see
 * docs/matching/issue-51-54-naked-retry.md): right size, ~52 halfwords
 * off under old_agbcc - `&b` (sp+0xc) is computed once into a
 * callee-saved register before the player box is built instead of
 * after the copy, which pushes r5/r6 roles around for the rest of the
 * function. Same wall as actor_part24b.c's `sub_8031378`. */
#if NON_MATCHING
struct box16 {
    s16 x, y, z;
    s16 w, h, d;
};

static inline void BoxMove(struct box16 *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

static inline struct box16 ActorBox(struct actor_self *obj)
{
    struct box16 t = *(struct box16 *)obj->unk_38;
    s32 px = obj->x >> 8;
    s32 py = obj->y >> 8;
    s32 pz = obj->z >> 8;

    BoxMove(&t, px, py, pz);
    return t;
}

static inline u8 BoxOverlap(struct box16 *b, struct box16 *a)
{
    if (b->z < a->z + a->d && b->z + b->d > a->z
        && b->y < a->y + a->h && b->y + b->h > a->y
        && b->x < a->x + a->w && b->x + b->w > a->x)
        goto hit;
    return 0;
hit:
    return 1;
}

void sub_802D7B0(void)
{
    struct box16 a, b;
    struct actor_self *obj;
    s32 old, cur;

    if (gUnknown_030014D0 != 3)
        gUnknown_030014C4 += (((struct actor_self *)gUnknown_03000884)->x - gUnknown_030014C4) / 32;
    obj = gUnknown_030014BC;
    old = obj->animTime >> 8;
    obj->animTime += *(s16 *)&obj->animTimer;
    obj->animDone = 0;
    if (GetAnimFrameBaseOffset(obj) >= obj->anims[obj->animIndex].loopThreshold) {
        obj->animTime -= (obj->anims[obj->animIndex].loopThreshold
                          - obj->anims[obj->animIndex].loopBase) << 8;
        obj->animDone = 1;
    }
    gStaticData_0817A840[gUnknown_030014D0]();
    obj = gUnknown_030014BC;
    cur = obj->animTime >> 8;
    if (old != cur) {
        gUnknown_03000898((u8 *)obj->frameOffsets[obj->anims[obj->animIndex].frameIndex + cur] + 4,
                          gUnknown_030014C0);
        gUnknown_030014C1 = 1;
    }
    sub_8029E34(gUnknown_030014CC);
    sub_802D9A8();
    a = *(struct box16 *)gStaticData_0817AA98;
    BoxMove(&a, gUnknown_030014C4 >> 8, 0, gUnknown_030014C8 >> 8);
    if ((u32)gUnknown_030014D0 <= 1) {
        struct actor_self **playerAddr = &gUnknown_03000884;

        if (gUnknown_030014A0 != 0)
            return;
        b = ActorBox(*playerAddr);
        sub_800014C(&b, &b, sizeof(b));
        if (BoxOverlap(&b, &a)) {
            gUnknown_030014D0 = 2;
            obj = gUnknown_030014BC;
            obj->animIndex = 2;
            obj->animTimer = obj->anims[2].duration;
            obj->animDone = 0;
            obj->animTime = 0;
            sub_802C018(gUnknown_03000884);
            sub_8029BAC(0);
        }
    }
}
#else
NAKED void sub_802D7B0(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #0x24\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r0, #3\n\t"
        "beq 2f\n\t"
        "ldr r2, 8f\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "ldr r1, [r2]\n\t"
        "sub r0, r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "add r0, #0x1f\n\t"
        "1:\n\t"
        "asr r0, r0, #5\n\t"
        "add r0, r1, r0\n\t"
        "str r0, [r2]\n\t"
        "2:\n\t"
        "ldr r5, 10f\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "asr r6, r0, #8\n\t"
        "mov r2, #0x10\n\t"
        "ldrsh r1, [r4, r2]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r3, #4\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "cmp r0, r2\n\t"
        "blt 3f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "3:\n\t"
        "ldr r1, 11f\n\t"
        "ldr r7, 7f\n\t"
        "ldr r0, [r7]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_803AD78\n\t"
        "ldr r4, [r5]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "asr r5, r0, #8\n\t"
        "cmp r6, r5\n\t"
        "beq 4f\n\t"
        "ldr r3, 12f\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "ldr r2, [r4]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r0, r5\n\t"
        "ldr r1, [r4, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #4\n\t"
        "ldr r1, 13f\n\t"
        "ldrb r1, [r1]\n\t"
        "ldr r2, [r3]\n\t"
        "bl sub_803AD80\n\t"
        "ldr r1, 14f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "4:\n\t"
        "ldr r0, 15f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8029E34\n\t"
        "bl sub_802D9A8\n\t"
        "mov r1, sp\n\t"
        "ldr r0, 16f\n\t"
        "ldm r0!, {r2, r3, r4}\n\t"
        "stm r1!, {r2, r3, r4}\n\t"
        "ldr r0, 8f\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 17f\n\t"
        "ldr r2, [r0]\n\t"
        "asr r2, r2, #8\n\t"
        "mov r0, sp\n\t"
        "ldrh r5, [r0]\n\t"
        "add r1, r5, r1\n\t"
        "strh r1, [r0]\n\t"
        "ldrh r1, [r0, #4]\n\t"
        "add r2, r1, r2\n\t"
        "strh r2, [r0, #4]\n\t"
        "ldr r0, [r7]\n\t"
        "cmp r0, #1\n\t"
        "bls 5f\n\t"
        "b 21f\n\t"
        "5:\n\t"
        "ldr r2, 9f\n\t"
        "ldr r0, 18f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 21f\n\t"
        "ldr r2, [r2]\n\t"
        "add r1, sp, #0x18\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x38\n\t"
        "ldm r0!, {r3, r4, r5}\n\t"
        "stm r1!, {r3, r4, r5}\n\t"
        "ldr r0, [r2, #0x1c]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r3, [r2, #0x20]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "asr r2, r2, #8\n\t"
        "add r1, sp, #0x18\n\t"
        "ldrh r4, [r1]\n\t"
        "add r0, r4, r0\n\t"
        "strh r0, [r1]\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "add r0, r0, r3\n\t"
        "strh r0, [r1, #2]\n\t"
        "ldrh r5, [r1, #4]\n\t"
        "add r2, r5, r2\n\t"
        "strh r2, [r1, #4]\n\t"
        "add r0, sp, #0xc\n\t"
        "ldm r1!, {r2, r3, r4}\n\t"
        "stm r0!, {r2, r3, r4}\n\t"
        "add r4, sp, #0xc\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r4, #0\n\t"
        "mov r2, #0xc\n\t"
        "bl sub_800014C\n\t"
        "mov r1, sp\n\t"
        "mov r5, #4\n\t"
        "ldrsh r2, [r4, r5]\n\t"
        "mov r0, #4\n\t"
        "ldrsh r3, [r1, r0]\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r5, #0xa\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 6f\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r5, #2\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r5, #8\n\t"
        "ldrsh r0, [r4, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 6f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r4, r0]\n\t"
        "mov r5, #0\n\t"
        "ldrsh r3, [r1, r5]\n\t"
        "mov r5, #6\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 6f\n\t"
        "mov r1, #6\n\t"
        "ldrsh r0, [r4, r1]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "bgt 19f\n\t"
        "6:\n\t"
        "mov r0, #0\n\t"
        "b 20f\n\t"
        ".align 2, 0\n"
        "7: .4byte gUnknown_030014D0\n"
        "8: .4byte gUnknown_030014C4\n"
        "9: .4byte gUnknown_03000884\n"
        "10: .4byte gUnknown_030014BC\n"
        "11: .4byte gStaticData_0817A840\n"
        "12: .4byte gUnknown_03000898\n"
        "13: .4byte gUnknown_030014C0\n"
        "14: .4byte gUnknown_030014C1\n"
        "15: .4byte gUnknown_030014CC\n"
        "16: .4byte gStaticData_0817AA98\n"
        "17: .4byte gUnknown_030014C8\n"
        "18: .4byte gUnknown_030014A0\n"
        "19:\n\t"
        "mov r0, #1\n\t"
        "20:\n\t"
        "cmp r0, #0\n\t"
        "beq 21f\n\t"
        "ldr r0, 22f\n\t"
        "mov r2, #2\n\t"
        "str r2, [r0]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r1, [r0]\n\t"
        "str r2, [r1, #0xc]\n\t"
        "ldr r0, [r1]\n\t"
        "ldrh r0, [r0, #0x18]\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "strh r0, [r1, #0x10]\n\t"
        "strb r2, [r1, #0x12]\n\t"
        "str r3, [r1, #8]\n\t"
        "ldr r0, 24f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_802C018\n\t"
        "mov r0, #0\n\t"
        "bl sub_8029BAC\n\t"
        "21:\n\t"
        "add sp, #0x24\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "22: .4byte gUnknown_030014D0\n"
        "23: .4byte gUnknown_030014BC\n"
        "24: .4byte gUnknown_03000884\n"
    );
}
#endif

/* Palette-gradient cursor for the `gUnknown_030014BC` "gauge" object.
 * Below `0x5000`, DMAs a fixed 16-color gradient (`gStaticData_0817AA6C`)
 * straight into BG palette RAM at `0x050001E0` (`REG_DMA3` at
 * `0x040000D4`). Above `0xBE00`, DMAs a single zeroed halfword instead
 * (blanking the gradient). In between, computes a `sub_803ADB4`-scaled
 * factor from how far `gUnknown_030014CC` sits into that `[0x5000,
 * 0xBE00]` range, then directly writes 16 colors: for each
 * `gStaticData_0817AA6C` source halfword (a packed BGR555 color), its
 * 5-bit R and G channels are scaled by that factor (`>>8` after the
 * multiply) and repacked as `R | (G<<5) | (G<<10)` - the ROM really
 * reuses the scaled green for blue - into BG palette RAM at
 * `0x050001E0` onward, a manual brightness ramp rather than a second DMA.
 *
 * The two `0x1f` masks are separate locals: the ROM keeps one in `ip`
 * (set before the pointers) and one in `r7` (set after them). */
void sub_802D9A8(void)
{
    s32 v = gUnknown_030014CC;

    if (v <= 0x4fff) {
        DmaCopy16(3, gStaticData_0817AA6C, (void *)0x050001E0, 0x20);
    } else if (v > 0xbdff) {
        DmaFill16(3, 0, (void *)0x050001E0, 0x20);
    } else {
        s32 f = ((0xbe00 - v) << 8) / 0x6e00;
        s32 mask = 0x1f;
        u16 *dst = (u16 *)0x050001E0;
        u16 *src = gStaticData_0817AA6C;
        s32 mask2 = 0x1f;
        s32 i;

        for (i = 15; i >= 0; i--) {
            u16 c = *src;
            s32 r = ((mask2 & c) * f) >> 8;
            s32 g = (((c >> 5) & mask) * f) >> 8;

            *dst = r | (g << 5) | (g << 10);
            dst++;
            src++;
        }
    }
}

/* Companion to `sub_802D9A8` above: the gauge's affine BG2 setup. When
 * `gUnknown_030014C1` is set, flips `REG_BG2CNT` (`0x0400000C`) between
 * two screen-base words according to `gUnknown_030014C0`, clears `C1`
 * and toggles `C0`. Then derives a zoom factor from `gUnknown_030014CC`
 * (`/0x5500`), writes `REG_BG2X` (`0x04000028`) from
 * `gUnknown_030014C4` and `sub_8029EB4()`, `REG_BG2Y` (`0x0400002C`)
 * from `sub_8029E98()`, and the `PA`/`PB`/`PC`/`PD` matrix at
 * `0x04000020` as `scale, 0, 0, scale`.
 *
 * Matches under old_agbcc. The flag addresses are copied into their own
 * locals after the load (the ROM's `ldrb r1, [r0]; adds r3, r0, #0`),
 * with the loaded flag pinned to r1. */
void sub_802DA68(void)
{
    s32 scale, base, t;
    u8 *p = &gUnknown_030014C1;
    register s32 v asm("r1") = *p;
    u8 *changed = p;

    if (v != 0) {
        u8 *alt;

        p = &gUnknown_030014C0;
        v = *p;
        alt = p;
        if (v != 0)
            *(vu16 *)0x0400000C = 0x1a09;
        else
            *(vu16 *)0x0400000C = 0x1b09;
        *changed = 0;
        *alt ^= 1;
    }
    scale = (gUnknown_030014CC << 8) / 0x5500;
    base = sub_8029EB4();
    t = (gUnknown_030014C4 * 47 << 8) / gUnknown_030014CC + base;
    *(vs32 *)0x04000028 = 0x4000 - ((t * scale) >> 8);
    *(vs32 *)0x0400002C = 0x4400 - ((sub_8029E98() * scale) >> 8);
    {
        vu16 *pa = (vu16 *)0x04000020;

        *pa++ = scale;
        *pa++ = 0;
        *pa++ = 0;
        *pa = scale;
    }
}

asm(".align 2, 0");

#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012D0;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F4;

extern struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern s32 sub_800815C(struct actor *part);
extern void *sub_8026EDC(s32 size);
extern void *sub_801A838(void *block, u32 arg1, u32 arg2);
extern void *sub_80189EC(void);
extern void *sub_80197DC(void);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8008E94(void *manager, void *value);
extern void sub_8023318(void *self, void *hdr);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);

/* Sets `part->field_29`'s low nibble to `sub_800815C(part)`'s result,
 * keeping the high nibble - same idiom as `UPDATE_PART_FRAME_NIBBLE` in
 * src/graphics/graphics_loading_21668.c/graphics_loading_21d80.c (not
 * shared via a header since each file only needs it locally, per those
 * files' own comment). */
#define UPDATE_PART_FRAME_NIBBLE(partPtr) \
    do { \
        register s32 _ret asm("r0") = sub_800815C(partPtr); \
        register u8 *_addr asm("r2") = (u8 *)(partPtr) + 0x29; \
        register s32 _mask asm("r1"); \
        register u8 _byte asm("r3"); \
        _mask = 0xf; \
        _ret &= _mask; \
        asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r" (_mask)); \
        _byte = *_addr; \
        _mask &= _byte; \
        _mask |= _ret; \
        *_addr = _mask; \
    } while (0)

/* Not part of the "two-line text popup" family the rest of this ROM region
 * belongs to (docs/matching/issue-31-graphics-loading.md) - a three-way
 * dispatcher gated by a `gStaticData_0816C86C`-indexed guard check. If
 * `sub_8023290`/`sub_80232B8`/`sub_8023324` (all three, `gUnknown_030012C0`)
 * say "no" and the current level's threshold-table entry's guard field
 * (offset `+4` off `gStaticData_0816C86C[idx]`, meaning not otherwise
 * understood) is zero, spawns a `sub_80071E4`-built part sized `0x64`x`0x64`
 * and tags it `0x12`, registering into `gUnknown_030012E8`. Otherwise, if
 * the byte at `gUnknown_030012D8 + 0x88` is zero, probes a position via
 * `sub_801A878(..., id=4)` (which, unlike its other callers in this ROM
 * region, returns a pointer whose first two Q8.8 fields line up with
 * `struct actor`'s own `x`/`y`) and feeds `sub_8023500` an
 * `{x - 2, y - 0x1e}` offset pair. Otherwise, falls through to the same
 * `sub_80071E4` spawn as the first arm, just sized `0x28`x`0x28` instead of
 * `0x64`x`0x64`. Every `sub_80071E4`/`sub_801A878` call still marshals
 * `arg3` into `r3` even though neither function's real body reads a 4th
 * argument - the same "pass everything, callee ignores the rest" calling
 * convention this whole ROM region's `sub_8009ED0` callers already
 * establish (docs/matching/issue-31-graphics-loading.md).
 *
 * Parked as NAKED: semantics are fully understood and a plain-C
 * reconstruction reproduces every instruction's operation (confirmed via
 * isolated compile), but `arg1`'s truncated-u16 home register (`r7`,
 * matching `sub_80071E4`/`sub_801A878`'s `adds r1, r7, #0` argument
 * marshalling at all three call sites) never converges through any C-level
 * technique tried - an explicit `register u16 asm("r7")` pin either gets
 * silently dropped from the prologue's push list or, worse, gets
 * reassigned mid-function to an unrelated address computation (confirmed
 * with a minimal repro: pinning `r7` here produced a bogus `add r2, sp,
 * #4`-derived value at the call site instead of `arg1`'s truncated value),
 * the same "r7 is never usable for an explicit register-variable pin in
 * this toolchain" gotcha already documented for `sub_8007114`
 * (src/graphics/graphics.c) and the class of gcc-2.9 register-allocation
 * gap this whole ROM region's other NAKED entries hit (see
 * docs/matching/issue-31-graphics-loading.md's "third pass"/"fourth pass",
 * `sub_802190C`). Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used throughout this
 * project. */
NAKED void sub_8021280(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "add r6, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r7, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r8, r2\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r3, r3, #0x10\n\t"
        "mov sb, r3\n\t"
        "ldr r5, 1f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023290\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023324\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_802332C\n\t"
        "lsl r1, r0, #3\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r4, #4\n\t"
        "add r1, r1, r4\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_80071E4\n\t"
        "add r4, r0, #0\n\t"
        "mov r1, #0x64\n\t"
        "mov r2, #0x64\n\t"
        "bl sub_80070EC\n\t"
        "mov r0, #0x12\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012C0\n"
    "2: .4byte gStaticData_0816C86C\n"
    "4: .4byte gUnknown_030012E8\n"
    "3:\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x88\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #4\n\t"
        "str r1, [sp]\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_801A878\n\t"
        "ldr r1, [r0]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r2, r1, #2\n\t"
        "ldr r0, [r0, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "add r3, r0, #0\n\t"
        "sub r3, #0x1e\n\t"
        "str r2, [sp, #4]\n\t"
        "str r3, [sp, #8]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #4\n\t"
        "bl sub_8023500\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012D8\n"
    "8: .4byte gUnknown_030012C0\n"
    "6:\n\t"
        "lsl r0, r6, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "mov r3, sb\n\t"
        "bl sub_80071E4\n\t"
        "add r4, r0, #0\n\t"
        "mov r1, #0x28\n\t"
        "mov r2, #0x28\n\t"
        "bl sub_80070EC\n\t"
        "mov r0, #0x12\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
    "7:\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_030012E8\n"
    );
}

/* One more instance of the "two-line text popup" spawner family
 * `sub_801FDEC` (src/graphics/graphics_loading_1fdec.c) belongs to - same
 * `sub_8009ED0` constructor, `gUnknown_030012D0`-rooted `+0x20` table
 * offset (`0x288` here), `sub_800815C`/`UPDATE_PART_FRAME_NIBBLE` frame
 * nibble, `gUnknown_030012B4` two-bit "collected" pack into `+0x28`, and
 * `gUnknown_030012F0` manager registration - but with a different tail:
 * instead of a `sub_800CA74`-obtained header and a `sub_803AD80`
 * trampoline fired *twice* (like `sub_801FDEC`), this one builds its own
 * header via `sub_801A838(sub_8026EDC(0x30), arg1, arg2)` (the freshly
 * allocated block's pointer feeds `sub_801A838`'s first argument directly,
 * its return value never stored anywhere in between), fires the
 * `sub_803AD80` trampoline once, and closes with `sub_8023318(header)`
 * instead of `sub_800C6A8`. `arg1`/`arg2` need durable homes (`r8`/`sb`)
 * across the intervening `sub_8009ED0`/`sub_800815C`/`sub_8026EDC` calls
 * since `sub_801A838` needs them again at the very end. */
void sub_8021388(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct actor *part asm("r6");
    register u32 a1 asm("r8");
    register u32 a2 asm("sb");
    register u32 a3trunc asm("r4");
    void *p2;
    void *p3;
    void *hdr;
    u8 *table;

    /* Reproduces the ROM's exact prologue: `arg1`/`arg2` get stored into
     * their durable `r8`/`sb` homes *twice* - once with their raw,
     * untruncated incoming values, then again after truncating (needed
     * again much later for `sub_801A838`) - while `arg3` truncates once
     * into `r4` and gets copied to `r3` for the `sub_8009ED0` call. No
     * plain-C phrasing (including separately-pinned locals initialized
     * directly from the parameters) reproduces the first, redundant store
     * - this compiler's dead-store elimination always collapses it down
     * to a single store once it can see both writes want the same final
     * truncated value, unlike the ROM's own (seemingly wasteful) codegen -
     * see docs/matching/issue-31-graphics-loading.md, which flagged this
     * exact gap after a prior pass. */
    asm volatile(
        "mov r8, r1\n\t"
        "mov sb, r2\n\t"
        "add r4, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "mov r8, r1\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov sb, r2\n\t"
        "lsl r4, r4, #0x10\n\t"
        "lsr r4, r4, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, r4, #0\n\t"
        "bl sub_8009ED0\n\t"
        "add r6, r0, #0\n\t"
        : "=r" (part), "=r" (a1), "=r" (a2), "=r" (a3trunc)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;

    {
        register void *addr asm("r0") = p3;

        /* p3 + 0x288 - written via asm volatile since a plain-C `off`
         * register pin (asm("r3")) is silently ignored by this compiler
         * for a simple constant initializer, landing the two-step
         * mov/lsl constant synthesis in r1 instead of matching the ROM's
         * r3. */
        asm volatile(
            "mov r3, #0xa2\n\t"
            "lsl r3, r3, #0x2\n\t"
            "add r0, r0, r3\n\t"
            : "+r" (addr)
            :
            : "r3", "memory");
        *(void **)((u8 *)part + 0x20) = addr;
    }

    UPDATE_PART_FRAME_NIBBLE(part);

    {
        register s32 mask asm("r0") = 0x10;
        register u8 old asm("r1") = part->flags;

        mask |= old;
        part->flags = mask;
    }

    {
        register s32 one asm("r5");
        register s32 arg3R asm("r4") = a3trunc;

        /* part->field_0A = 1; one = 1; - written via asm volatile (the
         * same idiom sub_801FDEC's own version of this pack uses) since
         * plain C schedules the two independent constant-1 writes in
         * whatever order is convenient, not the ROM's "both constants
         * materialized before the store" order. */
        asm volatile(
            "mov r0, #1\n\t"
            "mov r5, #1\n\t"
            "strb r0, [r6, #0xa]\n\t"
            : "=r" (one)
            : "r" (part)
            : "r0", "memory");

        {
            register void *gAddr asm("r0") = &gUnknown_030012B4;

            /* Packs the two "collected" bits (gUnknown_030012B4's
             * {offsets[], bytes[]} pair, indexed by arg3) into
             * part->field_0x28 bits 4/5 - same idiom as
             * src/graphics/graphics_loading_1fdec.c's sub_801FDEC, just
             * with `part` in r6/the cached `1` in r5 instead of r5/r6. */
            asm volatile(
                "ldr r0, [r0]\n\t"
                "ldr r1, [r0]\n\t"
                "ldr r0, [r1, #8]\n\t"
                "lsl r4, r4, #1\n\t"
                "add r4, r4, r0\n\t"
                "ldr r2, [r1, #0xc]\n\t"
                "ldrh r4, [r4]\n\t"
                "add r2, r4, r2\n\t"
                "ldrb r3, [r2]\n\t"
                "lsr r0, r3, #1\n\t"
                "eor r0, r0, r5\n\t"
                "and r0, r0, r5\n\t"
                "add r3, r6, #0\n\t"
                "add r3, r3, #0x28\n\t"
                "and r0, r0, r5\n\t"
                "lsl r0, r0, #4\n\t"
                "mov r1, #0x11\n\t"
                "neg r1, r1\n\t"
                "ldrb r4, [r3]\n\t"
                "and r1, r1, r4\n\t"
                "orr r1, r1, r0\n\t"
                "strb r1, [r3]\n\t"
                "ldrb r2, [r2]\n\t"
                "lsr r0, r2, #2\n\t"
                "and r0, r0, r5\n\t"
                "and r0, r0, r5\n\t"
                "lsl r0, r0, #5\n\t"
                "mov r2, #0x21\n\t"
                "neg r2, r2\n\t"
                "and r1, r1, r2\n\t"
                "orr r1, r1, r0\n\t"
                "strb r1, [r3]\n\t"
                :
                : "r" (part), "r" (arg3R), "r" (gAddr), "r" (one)
                : "r0", "r1", "r2", "r3", "r4", "memory");
        }
    }

    sub_8008E94(gUnknown_030012F0, part);

    hdr = sub_801A838(sub_8026EDC(0x30), a1, a2);
    *(void **)((u8 *)part + 0x44) = hdr;
    table = *(u8 **)((u8 *)hdr + 0xc);
    sub_803AD80((u8 *)hdr + *(s16 *)(table + 0x18), part, *(void **)(table + 0x1c));

    sub_8023318(gUnknown_030012C0, hdr);
}

/* Same "two-line text popup" family shape as `sub_8021388` above, but a
 * simpler tail: `arg1`/`arg2` aren't needed again after the `sub_8009ED0`
 * call (no `sub_801A838` here), so the prologue only truncates each
 * argument once - no `r8`/`sb` double-store quirk. The header comes from
 * a bare `sub_80189EC()` call (no arguments) instead of
 * `sub_801A838(block, arg1, arg2)`, and the `flags |= 0x10` step moves to
 * the very end (right before the manager registration) instead of
 * right after the `+0x29` nibble update.
 *
 * Parked as NAKED: semantics are fully understood and a plain-C
 * reconstruction reproduces every instruction's *content* (confirmed via
 * isolated compile - the collected-bits pack's mask-byte reload uses `r7`
 * as scratch, matching the ROM exactly), but the ROM's `push {r4, r5, r6,
 * r7, lr}` callee-saved set never converges: this compiler only adds a
 * hard-pinned register to a function's push/pop list when it tracks that
 * register as holding a value genuinely live across a wider span, and
 * `r7` here is used only transiently inside one inline-asm block - no
 * plain-C technique tried (an unused pinned `register ... asm("r7")`
 * local, capturing it as an asm output, or forcing a trailing "keep it
 * alive" read at the end of the function) gets this compiler to include
 * `r7` in the prologue, the same "r7 is never usable for an explicit
 * register-variable pin in this toolchain" gotcha `sub_8021280` above and
 * `sub_8007114` (src/graphics/graphics.c) already hit. Transcribed
 * instruction-for-instruction from the ROM disassembly instead. */
NAKED void sub_8021480(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "lsl r4, r4, #0x10\n\t"
        "lsr r4, r4, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, r4, #0\n\t"
        "bl sub_8009ED0\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xa5\n\t"
        "lsl r1, r1, #0x2\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r5, #0x20]\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r5, #0\n\t"
        "add r2, r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r1, r3\n\t"
        "orr r1, r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r0, #0x4c\n\t"
        "bl sub_8026EDC\n\t"
        "bl sub_80189EC\n\t"
        "add r6, r0, #0\n\t"
        "str r6, [r5, #0x44]\n\t"
        "ldr r1, [r6, #0xc]\n\t"
        "mov r7, #0x18\n\t"
        "ldrsh r0, [r1, r7]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r2, [r1, #0x1c]\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_803AD80\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #8]\n\t"
        "lsl r4, r4, #1\n\t"
        "add r4, r4, r0\n\t"
        "ldr r3, [r1, #0xc]\n\t"
        "ldrh r4, [r4]\n\t"
        "add r3, r4, r3\n\t"
        "ldrb r1, [r3]\n\t"
        "lsr r0, r1, #1\n\t"
        "mov r2, #1\n\t"
        "eor r0, r0, r2\n\t"
        "and r0, r0, r2\n\t"
        "add r4, r5, #0\n\t"
        "add r4, r4, #0x28\n\t"
        "and r0, r0, r2\n\t"
        "lsl r0, r0, #4\n\t"
        "mov r1, #0x11\n\t"
        "neg r1, r1\n\t"
        "ldrb r7, [r4]\n\t"
        "and r1, r1, r7\n\t"
        "orr r1, r1, r0\n\t"
        "strb r1, [r4]\n\t"
        "ldrb r3, [r3]\n\t"
        "lsr r0, r3, #2\n\t"
        "and r0, r0, r2\n\t"
        "and r0, r0, r2\n\t"
        "lsl r0, r0, #5\n\t"
        "mov r2, #0x21\n\t"
        "neg r2, r2\n\t"
        "and r1, r1, r2\n\t"
        "orr r1, r1, r0\n\t"
        "strb r1, [r4]\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8008E94\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8023318\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012D0\n"
    "2: .4byte gUnknown_030012B4\n"
    "3: .4byte gUnknown_030012F0\n"
    "4: .4byte gUnknown_030012C0\n"
    );
}

/* Last of the four instances the still-raw tail of
 * asm/code_3_2_17_21280.s held (docs/matching/issue-31-graphics-loading.md,
 * "Left raw") - the OAM-trio tail variant, like `sub_8021668`
 * (src/graphics/graphics_loading_21668.c) rather than the "collected bits"
 * lookup-table pack `sub_8021388`/`sub_8021480` use for their frame value.
 * `arg3`'s truncated home is `r5` for the whole function (needed again by
 * the collected-bits pack much later), `part` lives in `r4`, and `hdr`
 * (from a bare `sub_80197DC()` call) lives in `r8` - the same "durable
 * header pointer" idiom `sub_801FDEC` (src/graphics/graphics_loading_1fdec.c)
 * established. Registers into `gUnknown_030012F4`'s manager (not `F0`/`EC`
 * like its siblings), and its `+0x2d` field is set to `1` here (not `0`
 * like `sub_8021668`'s OAM-trio variant) - the same store also caches a
 * `0` into `sb` (used far later for a `part->field_2c = 0;` write after
 * `part`'s own `r4` home gets bumped by `0x2c` in place) and a `1` into
 * `r6` (reused by the collected-bits pack, same shape as
 * `sub_8021388`/`sub_8021480`'s cached `1`). */
void sub_802155C(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct actor *part asm("r4");
    register u32 a3trunc asm("r5");
    register u32 zeroSb asm("sb");
    register s32 oneR6 asm("r6");
    register void *hdr asm("r8");
    void *p2;
    void *p3;

    /* Same shape as sub_8021480's prologue (single truncation pass, no
     * r8/sb double-store) - `part`/`a3trunc` end up in r4/r5 instead of
     * r5/r4, matching this instance's own register choice. */
    asm volatile(
        "add r5, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "lsl r5, r5, #0x10\n\t"
        "lsr r5, r5, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8009ED0\n\t"
        "add r4, r0, #0\n\t"
        : "=r" (part), "=r" (a3trunc)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;
    *(void **)((u8 *)part + 0x20) = (u8 *)p3 + 0x27c;

    /* part->field_2d = 1; a durable 0 cached into sb (part->field_2c's
     * value, written much later) and 1 into r6 (the collected-bits pack's
     * constant) - all computed before the store itself, matching the
     * ROM's exact scheduling. */
    asm volatile(
        "mov r0, #1\n\t"
        "add r1, r4, #0\n\t"
        "add r1, r1, #0x2d\n\t"
        "mov r2, #0\n\t"
        "mov sb, r2\n\t"
        "mov r6, #1\n\t"
        "strb r0, [r1]\n\t"
        : "=r" (zeroSb), "=r" (oneR6)
        : "r" (part)
        : "r0", "r1", "r2", "memory");

    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);

    UPDATE_PART_FRAME_NIBBLE(part);

    sub_8026EDC(0x24);
    /* Unlike sub_801FDEC's equivalent (where `header` is already r8 by
     * the time its own +0xc table dereference runs), this instance reads
     * `table` from `sub_80197DC`'s fresh r0 return value *before* it gets
     * aliased into r8 - r8 can't be an immediate-offset load's base
     * register in Thumb, and copying it into a lo register first (the
     * fix sub_801FDEC needed for its store pair) would add an instruction
     * the ROM doesn't have here. `hdr` is recovered from r8 only after
     * r0 gets clobbered by the `ldrsh` below. */
    asm volatile(
        "bl sub_80197DC\n\t"
        "mov r8, r0\n\t"
        "str r0, [r4, #0x44]\n\t"
        "ldr r1, [r0, #0xc]\n\t"
        "mov r2, #0x18\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r8\n\t"
        "ldr r2, [r1, #0x1c]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        : "=r" (hdr)
        : "r" (part)
        : "r0", "r1", "r2", "r3", "lr", "memory");

    {
        register void *gAddr asm("r0") = &gUnknown_030012B4;

        /* Same "collected bits" pack as sub_8021388/sub_8021480, using
         * the `1` cached in r6 above instead of a fresh one, and
         * `part`(r4)/`arg3`(r5) in their own homes for this instance. */
        asm volatile(
            "ldr r0, [r0]\n\t"
            "ldr r1, [r0]\n\t"
            "ldr r0, [r1, #8]\n\t"
            "lsl r5, r5, #1\n\t"
            "add r5, r5, r0\n\t"
            "ldr r2, [r1, #0xc]\n\t"
            "ldrh r5, [r5]\n\t"
            "add r2, r5, r2\n\t"
            "ldrb r3, [r2]\n\t"
            "lsr r0, r3, #1\n\t"
            "eor r0, r0, r6\n\t"
            "and r0, r0, r6\n\t"
            "add r3, r4, #0\n\t"
            "add r3, r3, #0x28\n\t"
            "and r0, r0, r6\n\t"
            "lsl r0, r0, #4\n\t"
            "mov r1, #0x11\n\t"
            "neg r1, r1\n\t"
            "ldrb r5, [r3]\n\t"
            "and r1, r1, r5\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            "ldrb r2, [r2]\n\t"
            "lsr r0, r2, #2\n\t"
            "and r0, r0, r6\n\t"
            "and r0, r0, r6\n\t"
            "lsl r0, r0, #5\n\t"
            "mov r2, #0x21\n\t"
            "neg r2, r2\n\t"
            "and r1, r1, r2\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            :
            : "r" (part), "r" (a3trunc), "r" (gAddr), "r" (oneR6)
            : "r0", "r1", "r2", "r3", "r5", "memory");
    }

    {
        register s32 mask asm("r0") = 0x10;
        register u8 old asm("r1") = part->flags;

        mask |= old;
        part->flags = mask;
    }

    sub_8008E94(gUnknown_030012F4, part);

    {
        register u8 *addr2c asm("r4") = (u8 *)part + 0x2c;
        register u32 zero asm("r2") = zeroSb;

        *addr2c = zero;
    }

    sub_8023318(gUnknown_030012C0, hdr);
}

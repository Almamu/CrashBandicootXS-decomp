#include "core.h"
#include "actor.h"

extern void *gUnknown_030012D0;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012F0;
extern u8 gStaticData_0816BA6C[];

extern struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern s32 sub_800815C(struct actor *part);
extern void *sub_8026EDC(s32 size);
extern void *sub_800CA74(void);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8008E94(void *manager, void *value);
extern void sub_800C6A8(void *hdr, s32 arg1);

/* One of the many near-identical "two-line text popup" spawners in the
 * `gStaticData_0816C7D8`-adjacent dispatch region (docs/rom_map.md, "A
 * family of 'trigger effect type N' functions" and the "richer spawn"
 * shape `sub_8020D4C` was first characterized with) - unlike the
 * sound-or-effect twin family in trigger_effect.c, this whole sibling
 * group always spawns a part-object via `sub_8009ED0`, hooks its `+0x20`
 * table pointer at a fixed offset (here `0x54`) into the record reached
 * through `gUnknown_030012D0`'s double pointer-to-pointer, sets its
 * `+0x29` bitfield nibble from `sub_800815C`, fires a `sub_803AD80`
 * animation-table trampoline twice (tagging the header's own `+0x6c`
 * slot `7` and wiring `part+0x44` back to the header in between), marks
 * itself active, clears its own top flag bit, then packs two "collected"
 * bits looked up via a `gUnknown_030012B4`-rooted `{offsets[], bytes[]}`
 * pair (indexed by `arg3`, the caller's fourth argument) into the
 * `+0x28` bitfield's bits 4/5, registers itself into
 * `gUnknown_030012F0`'s manager, and finally overwrites the header's own
 * `+0x84` table pointer with `gStaticData_0816BA6C` before calling
 * `sub_800C6A8(header, 7)`.
 *
 * `arg0` has to stay `u32` (matching every sibling in this family and
 * the "trigger effect type N" twins next door) - the ROM only truncates
 * it at its single call site inside `sub_8009ED0`'s argument marshalling,
 * not up front in the prologue.
 *
 * Three spots needed inline asm rather than plain C to reproduce the
 * ROM exactly, all confirmed necessary by direct isolated-compile diff
 * against the ROM's disassembly:
 *   - The header pointer lives in `r8` (a genuine `register void *hdr
 *     asm("r8")`) for the whole function - forced explicitly since a
 *     plain local lets gcc pick a lo register instead, which changes the
 *     prologue/epilogue's callee-saved set (no `mov r6, r8`/`push {r6}`
 *     pair) and shortens the function by 4 bytes.
 *   - The bitfield-packing block (the two "collected" bits) reproduces a
 *     literal-constant-in-register idiom this compiler won't reach via
 *     plain C: the ROM caches `1` in `r6` *once* (a second, independent
 *     `mov r6, #1` right next to the `part->field_0A = 1` store, not
 *     reused from it) and then re-ANDs with that cached `1` twice per
 *     bit even when nothing in between could have clobbered the result -
 *     for the first bit this second AND straddles an unrelated `+0x28`
 *     address computation, but for the second bit both ANDs are
 *     adjacent, which gcc's own optimizer folds into a single
 *     instruction if phrased as plain C (`x &= 1; x &= 1;` collapses).
 *     Written as `asm volatile` blocks that spell out the exact ROM
 *     instructions instead of fighting the optimizer.
 *   - The `header + 0x6c = 7; part->field_44 = header;` writes need the
 *     header pointer copied into a plain (lo-register) local *before*
 *     either store - `r8` can't be the base register for an immediate-
 *     offset `strb`/`str` in Thumb, so the ROM does one `mov` into a lo
 *     register (`r3`) and then two offset-form stores, rather than the
 *     "compute each full address, then store with no offset" shape a
 *     bare `(u8 *)hdr + 0x6c` cast naturally produces.
 */
void sub_801FDEC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register struct actor *part asm("r5") = sub_8009ED0(arg0, arg1, arg2, arg3);
    void *p2 = *(void **)gUnknown_030012D0;
    void *p3 = *(void **)p2;
    u8 *table;
    register void *hdr asm("r8");

    *(void **)((u8 *)part + 0x20) = (u8 *)p3 + 0x54;
    {
        register s32 result asm("r0") = sub_800815C(part);
        register u8 *addr asm("r2") = (u8 *)part + 0x29;
        register s32 acc asm("r1");

        result &= 0xf;
        /* acc = -0x10; - written via inline asm: this compiler
         * otherwise synthesizes -0x10 from the still-live r1=0xf mask
         * constant above (`sub r1, r1, #0x1f`), a one-instruction
         * shortcut the ROM doesn't take (it reloads a fresh `mov r1,
         * #0x10` / `neg r1, r1` pair instead). */
        asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r" (acc));
        acc &= *addr;
        acc |= result;
        *addr = acc;
    }

    sub_8026EDC(0x8c);

    hdr = sub_800CA74();
    table = *(u8 **)((u8 *)hdr + 0xc);
    sub_803AD80((u8 *)hdr + *(s16 *)(table + 0x18), part, *(void **)(table + 0x1c));
    {
        register s32 tagVal asm("r0") = 7;
        register u8 *h asm("r3") = (u8 *)hdr;

        *(s32 *)(h + 0x6c) = tagVal;
        *(void **)((u8 *)part + 0x44) = h;
        table = *(u8 **)(h + 0xc);
    }
    sub_803AD80((u8 *)hdr + *(s16 *)(table + 0x18), part, *(void **)(table + 0x1c));

    {
        register s32 arg3R asm("r4") = arg3;

        /* part->field_0A = 1; part->flags &= 0x7f; */
        asm volatile(
            "mov r0, #1\n\t"
            "mov r6, #1\n\t"
            "strb r0, [r5, #0xa]\n\t"
            "mov r0, #0x7f\n\t"
            "ldrb r3, [r5, #0xc]\n\t"
            "and r0, r0, r3\n\t"
            "strb r0, [r5, #0xc]\n\t"
            :
            : "r" (part)
            : "r0", "r3", "r6", "memory");

        {
            register void *gAddr asm("r0") = &gUnknown_030012B4;

            /* Packs the two "collected" bits (gUnknown_030012B4's
             * {offsets[], bytes[]} pair, indexed by arg3) into
             * part->field_0x28 bits 4/5. */
            asm volatile(
                "ldr r0, [r0]\n\t"
                "ldr r1, [r0]\n\t"
                "ldr r0, [r1, #8]\n\t"
                "lsl r4, r4, #1\n\t"
                "add r4, r4, r0\n\t"
                "ldr r2, [r1, #0xc]\n\t"
                "ldrh r4, [r4]\n\t"
                "add r2, r4, r2\n\t"
                "ldrb r4, [r2]\n\t"
                "lsr r0, r4, #1\n\t"
                "eor r0, r0, r6\n\t"
                "and r0, r0, r6\n\t"
                "add r3, r5, #0\n\t"
                "add r3, r3, #0x28\n\t"
                "and r0, r0, r6\n\t"
                "lsl r0, r0, #4\n\t"
                "mov r1, #0x11\n\t"
                "neg r1, r1\n\t"
                "ldrb r4, [r3]\n\t"
                "and r1, r1, r4\n\t"
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
                : "r" (part), "r" (arg3R), "r" (gAddr)
                : "r0", "r1", "r2", "r3", "r4", "r6", "memory");
        }
    }

    sub_8008E94(gUnknown_030012F0, part);

    *(void **)((u8 *)hdr + 0x84) = gStaticData_0816BA6C;
    sub_800C6A8(hdr, 7);
}

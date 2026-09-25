#include "core.h"
#include "actor.h"

/* GitHub issue #31 - the remainder of the "two-line text popup" spawner
 * family `sub_801FDEC` (src/graphics/graphics_loading_1fdec.c) and
 * `sub_8021388`/`sub_802155C` (src/graphics/graphics_loading_21280.c)
 * already established: `sub_8009ED0`/`sub_800A604`-built part object,
 * `gUnknown_030012D0`-rooted `+0x20` table offset, `sub_800815C` frame
 * nibble update, `gUnknown_030012B4` two-bit "collected" pack into
 * `+0x28`, `gUnknown_030012F0` manager registration, and a header
 * (`sub_800CA74`) with one or two `sub_803AD80` trampoline calls. Every
 * instance in this file shares that skeleton, varying only the embedded
 * offsets/constants and tail shape (see docs/matching/issue-31-graphics-loading.md
 * for the numbered breakdown this whole cluster follows). Three of the
 * twelve (`sub_801F050`, `sub_801F170`, `sub_801F680`) reconstruct as
 * real C; the other nine hit the same "r7 only ever lives inside a
 * single inline-asm block" toolchain gap already documented for
 * `sub_8021280`/`sub_8021480`/`sub_802190C` and are transcribed
 * instruction-for-instruction from the ROM disassembly instead. */

extern void *gUnknown_030012D0;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012BC;
extern u8 gStaticData_0816B98C[];
extern u8 gStaticData_0816BA2C[];
extern u8 gStaticData_0816BA0C[];
extern u8 gStaticData_0816B9EC[];
extern u8 gStaticData_0816B9AC[];
extern u8 gStaticData_0816B9CC[];
extern u8 gStaticData_0816BA4C[];
extern u8 gStaticData_0816BA8C[];

extern struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern struct actor *sub_800A604(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern s32 sub_800815C(struct actor *part);
extern void *sub_8026EDC(s32 size);
extern void *sub_800CA74(void);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8008E94(void *manager, void *value);
extern void sub_800C6A8(void *hdr, s32 arg1);
extern void sub_800C898(void *hdr, s32 arg1);
extern void sub_800C87C(void *hdr, s32 arg1, s32 arg2, s32 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern void PlaySfx(void *bank, s32 sfxId, s32 volume);

/* Same shape as `sub_801FDEC`'s tail, plus the record re-lookup
 * (`gUnknown_030012B4`'s `{offsets[], bytes[]}` pair, indexed by
 * `arg3`) that several siblings in this cluster reuse to feed a second
 * pair of helper calls (`sub_800C6A8`/`sub_800C898`) after the OAM trio
 * and a `part->field_2d = 0` write. Parked as NAKED - every instruction's
 * operation matches the ROM (confirmed via isolated compile), but the
 * ROM's `push {r4, r5, r6, r7, lr}` callee-saved set (three extra high
 * registers - `sl`/`sb`/`r8` - shadowed through `r5`/`r6`/`r7`) and the
 * `+0x29` nibble reload's own `r7` scratch choice never converge through
 * plain C or register pins - the same "r7 is never usable for an
 * explicit register-variable pin, and only emerges from this compiler's
 * own natural allocator under register pressure this reconstruction
 * doesn't reproduce" gotcha `sub_8021280`/`sub_8021480`/`sub_802190C`
 * (graphics_loading_21280.c/graphics_loading_21668.c) already hit. */
NAKED void sub_801EF0C(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r4, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x9c\n"
        "\tstr r0, [r4, #0x20]\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r4, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0xd\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r4, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r7, #0\n"
        "\tmov sl, r7\n"
        "\tmov r5, #1\n"
        "\tstrb r0, [r4, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r1, [r4, #0xc]\n"
        "\tand r0, r1\n"
        "\tstrb r0, [r4, #0xc]\n"
        "\tldr r2, 2f @ =gUnknown_030012B4\n"
        "\tmov sb, r2\n"
        "\tldr r0, [r2]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r3, r8\n"
        "\tlsl r3, r3, #1\n"
        "\tmov r8, r3\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r7, [r2]\n"
        "\tlsr r0, r7, #1\n"
        "\teor r0, r5\n"
        "\tand r0, r5\n"
        "\tadd r3, r4, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r5\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r1, 4f @ =gStaticData_0816B98C\n"
        "\tadd r0, r6, #0\n"
        "\tadd r0, #0x84\n"
        "\tstr r1, [r0]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r5, [r1, #0xc]\n"
        "\tmov r2, r8\n"
        "\tldrh r2, [r2]\n"
        "\tadd r5, r2, r5\n"
        "\tadd r0, r4, #0\n"
        "\tadd r0, #0x2d\n"
        "\tmov r3, sl\n"
        "\tstrb r3, [r0]\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_80087C0\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_80087B4\n"
        "\tadd r0, r4, #0\n"
        "\tmov r1, #0\n"
        "\tbl sub_800872C\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #2\n"
        "\tbl sub_800C6A8\n"
        "\tldr r1, [r5, #4]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C898\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
    );
}

/* One more `sub_801FDEC`-family instance, but `part` lands in `r8` (the
 * "header pinned in a high register" idiom `sub_801FDEC` itself already
 * established, just for `part` instead of `hdr` here) and `hdr`/`arg3`'s
 * scratch home in `r4`/`r5`. Ends with a "second `header->0x84` rewrite
 * plus literal-constant struct-field copy" tail (into `hdr`'s own
 * `+0x20`/`+0x24`/`+0x28`/`+0x2c`, not a record read) - the variant
 * `docs/matching/issue-31-graphics-loading.md`'s earlier passes flagged
 * but never worked through. Every `mov rX, r8`/value-then-address
 * ordering below matches the ROM's own scheduling; this compiler
 * otherwise reorders them (computing the destination address before the
 * value, or vice versa) or silently ignores a bare-constant register
 * pin (see the `off18`-style spots elsewhere in this cluster) - both
 * confirmed by isolated-compile diff during this pass. */
void sub_801F050(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct actor *part asm("r8");
    register s32 idx asm("r5");
    register s32 zero asm("sb");
    void *p2;
    void *p3;
    register void *hdr asm("r4");
    u8 *table;

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
        "mov r8, r0\n\t"
        : "=r" (part), "=r" (idx)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;

    {
        register void *addr asm("r0") = (u8 *)p3 + 0x84;
        register void *pv asm("r1") = part;
        *(void **)((u8 *)pv + 0x20) = addr;
    }

    {
        register s32 result asm("r0") = sub_800815C(part);
        register u8 *addr asm("r2") = (u8 *)part + 0x29;
        register s32 acc asm("r1");
        result &= 0xf;
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
        register s32 tagVal asm("r0") = 0xb;
        *(s32 *)((u8 *)hdr + 0x6c) = tagVal;
    }
    {
        register void *pv asm("r3") = part;
        *(void **)((u8 *)pv + 0x44) = hdr;
    }
    table = *(u8 **)((u8 *)hdr + 0xc);
    sub_803AD80((u8 *)hdr + *(s16 *)(table + 0x18), part, *(void **)(table + 0x1c));

    {
        register s32 one asm("r6");

        asm volatile(
            "mov r0, #1\n\t"
            "mov r3, #0\n\t"
            "mov sb, r3\n\t"
            "mov r6, #1\n\t"
            "mov r1, r8\n\t"
            "strb r0, [r1, #0xa]\n\t"
            "mov r0, #0x7f\n\t"
            "ldrb r2, [r1, #0xc]\n\t"
            "and r0, r0, r2\n\t"
            "strb r0, [r1, #0xc]\n\t"
            : "=r" (zero), "=r" (one)
            : "r" (part)
            : "r0", "r1", "r2", "r3", "memory");

        {
            register void *gAddr asm("r0") = &gUnknown_030012B4;

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
                "mov r3, r8\n\t"
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
                : "+r" (idx)
                : "r" (part), "r" (gAddr), "r" (one)
                : "r0", "r1", "r2", "r3", "r5", "memory");
        }

        sub_8008E94(gUnknown_030012F0, part);
    }

    {
        register void *val asm("r0") = gStaticData_0816B98C;
        register void *statAddr asm("r5") = (u8 *)hdr + 0x84;
        *(void **)statAddr = val;
        sub_800C6A8(hdr, 0x11);
        *(void **)statAddr = gStaticData_0816BA2C;
    }

    {
        register s32 w asm("r0") = 0x64;
        register s32 h asm("r1") = 0x32;
        register s32 zero2 asm("r2") = zero;

        *(s32 *)((u8 *)hdr + 0x20) = zero2;
        *(s32 *)((u8 *)hdr + 0x28) = w;
        *(s32 *)((u8 *)hdr + 0x24) = zero2;
        *(s32 *)((u8 *)hdr + 0x2c) = h;
    }
}

/* Same family, `part` back in a lo register (`r4`) this time so its own
 * offset writes need no lo-register copy, `hdr` in `r6`, `arg3`'s home
 * in `r5` (reused as collected-pack scratch after). Ends with the same
 * "second `header->0x84` rewrite plus literal struct-field copy" shape
 * as `sub_801F050`, plus an OAM trio and a `part->field_0A` overwrite
 * `sub_801F050` doesn't have. The `+0x2d`/collected-pack `1` cache (`sb`)
 * and the `+0x20`/`+0x24`/`+0x28`/`+0x2c` zero/`sl` cache both need to
 * stay live across the whole function, same as `sub_801F050`'s `sb`
 * zero cache. */
void sub_801F170(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct actor *part asm("r4");
    register s32 oneSb asm("sb");
    register s32 zeroSl asm("sl");
    register s32 oneR8 asm("r8");
    void *p2;
    void *p3;
    register void *hdr asm("r6");
    u8 *table;

  {
    register s32 idx asm("r5");

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
        : "=r" (part), "=r" (idx)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;
    *(void **)((u8 *)part + 0x20) = (u8 *)p3 + 0x78;

    {
        register s32 result asm("r0") = sub_800815C(part);
        register u8 *addr asm("r2") = (u8 *)part + 0x29;
        register s32 acc asm("r1");
        result &= 0xf;
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
        register s32 tagVal asm("r0") = 0xa;
        *(s32 *)((u8 *)hdr + 0x6c) = tagVal;
    }
    *(void **)((u8 *)part + 0x44) = hdr;
    table = *(u8 **)((u8 *)hdr + 0xc);
    {
        /* A bare `register s32 off asm("r3") = 0x18;` pin is silently
         * ignored by this compiler for a simple constant initializer
         * (lands the two-step mov/lsl synthesis in whatever register it
         * likes, not the ROM's r3) - the same gotcha
         * docs/matching/issue-31-graphics-loading.md documents for
         * sub_8021388's own `+0x20` table-offset constant. Spelled out
         * as a full hand-written trampoline call instead. */
        register void *tbl asm("r1") = table;
        asm volatile(
            "mov r3, #0x18\n\t"
            "ldrsh r0, [r1, r3]\n\t"
            "add r0, r6, r0\n\t"
            "ldr r2, [r1, #0x1c]\n\t"
            "add r1, r4, #0\n\t"
            "bl sub_803AD80\n\t"
            :
            : "r" (tbl), "r" (hdr), "r" (part)
            : "r0", "r1", "r2", "r3", "lr", "memory");
    }

    asm volatile(
        "mov r0, #1\n\t"
        "mov sb, r0\n\t"
        "mov r1, #0\n\t"
        "mov sl, r1\n\t"
        "mov r2, #1\n\t"
        "mov r8, r2\n\t"
        "mov r3, sb\n\t"
        "strb r3, [r4, #0xa]\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        : "=r" (oneSb), "=r" (zeroSl), "=r" (oneR8)
        : "r" (part)
        : "r0", "r1", "r2", "r3", "memory");

    {
        register void *gAddr asm("r0") = &gUnknown_030012B4;

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
            "mov r5, r8\n\t"
            "eor r0, r0, r5\n\t"
            "and r0, r0, r5\n\t"
            "add r3, r4, #0\n\t"
            "add r3, r3, #0x28\n\t"
            "and r0, r0, r5\n\t"
            "lsl r0, r0, #4\n\t"
            "mov r1, #0x11\n\t"
            "neg r1, r1\n\t"
            "ldrb r5, [r3]\n\t"
            "and r1, r1, r5\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            "ldrb r2, [r2]\n\t"
            "lsr r0, r2, #2\n\t"
            "mov r2, r8\n\t"
            "and r0, r0, r2\n\t"
            "and r0, r0, r2\n\t"
            "lsl r0, r0, #5\n\t"
            "mov r2, #0x21\n\t"
            "neg r2, r2\n\t"
            "and r1, r1, r2\n\t"
            "orr r1, r1, r0\n\t"
            "strb r1, [r3]\n\t"
            : "+r" (idx)
            : "r" (part), "r" (gAddr), "r" (oneR8)
            : "r0", "r1", "r2", "r3", "r5", "memory");
    }
  }

    sub_8008E94(gUnknown_030012F0, part);

    {
        register void *val asm("r0") = gStaticData_0816B98C;
        register void *statAddr asm("r5") = (u8 *)hdr + 0x84;
        *(void **)statAddr = val;

        {
            register u8 *addr2d asm("r0") = (u8 *)part + 0x2d;
            register s32 tagVal2 asm("r3") = oneSb;
            *addr2d = tagVal2;
        }

        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);

        {
            register s32 six asm("r0") = 6;
            *(u8 *)((u8 *)part + 0xa) = six;
        }

        sub_800C6A8(hdr, 3);

        *(void **)statAddr = gStaticData_0816BA0C;
    }

    asm volatile(
        "mov r2, #0x14\n\t"
        "neg r2, r2\n\t"
        "mov r0, #0x2d\n\t"
        "mov r1, #0x14\n\t"
        "mov r5, sl\n\t"
        "str r5, [r6, #0x20]\n\t"
        "str r0, [r6, #0x28]\n\t"
        "str r2, [r6, #0x24]\n\t"
        "str r1, [r6, #0x2c]\n\t"
        :
        : "r" (hdr), "r" (zeroSl)
        : "r0", "r1", "r2", "r5", "memory");
}

/* Same family shape as `sub_801EF0C`, tag `0xe`, no OAM trio, no
 * `part->field_2d` write - `sub_800C6A8(hdr, 2)`/`sub_800C898` fire
 * straight off the manager register/header-table writes. Parked as
 * NAKED for the same `r7`-in-the-callee-saved-set gap `sub_801EF0C`
 * hits - confirmed by isolated compile that every instruction's
 * operation already matches. */
NAKED void sub_801F2BC(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r4, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0xa8\n"
        "\tstr r0, [r4, #0x20]\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r4, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0xe\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r4, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r5, #1\n"
        "\tstrb r0, [r4, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r7, [r4, #0xc]\n"
        "\tand r0, r7\n"
        "\tstrb r0, [r4, #0xc]\n"
        "\tldr r0, 2f @ =gUnknown_030012B4\n"
        "\tmov sb, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r2, r8\n"
        "\tlsl r2, r2, #1\n"
        "\tmov r8, r2\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r5\n"
        "\tand r0, r5\n"
        "\tadd r3, r4, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r5\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r1, 4f @ =gStaticData_0816B98C\n"
        "\tadd r0, r6, #0\n"
        "\tadd r0, #0x84\n"
        "\tstr r1, [r0]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r4, [r1, #0xc]\n"
        "\tmov r2, r8\n"
        "\tldrh r2, [r2]\n"
        "\tadd r4, r2, r4\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #2\n"
        "\tbl sub_800C6A8\n"
        "\tldr r1, [r4, #4]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C898\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
    );
}

/* Same family, tag `0xc`. Ends with a "second `header->0x84` rewrite
 * plus a `{x, y, w}`-shaped struct-field copy into `hdr`'s own
 * `+0x30`/`+0x34`/`+0x38`" from the collected-bits record's own
 * `+4`/`+8`/`+0xc` fields, then a genuine average/quarter computation
 * (`(record.f4 + record.f8) / 2`, then that result `/ 4`, both signed
 * divisions by a power of 2 - this compiler's own division-by-power-
 * of-2 idiom, confirmed instruction-for-instruction) written into
 * `hdr->0x48`/`hdr->0x4c`. Parked as NAKED for the same `r7`
 * callee-saved-set gap as the rest of this cluster's NAKED entries. */
NAKED void sub_801F3DC(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tadd r6, r3, #0\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r6, r6, #0x10\n"
        "\tlsr r6, r6, #0x10\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tadd r3, r6, #0\n"
        "\tbl sub_8009ED0\n"
        "\tadd r4, r0, #0\n"
        "\tldr r0, 2f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x90\n"
        "\tstr r0, [r4, #0x20]\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r4, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r3, [r2]\n"
        "\tand r1, r3\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r7, r0, #0\n"
        "\tldr r1, [r7, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r7, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0xc\n"
        "\tstr r0, [r7, #0x6c]\n"
        "\tstr r7, [r4, #0x44]\n"
        "\tldr r1, [r7, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r7, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r5, #1\n"
        "\tstrb r0, [r4, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r1, [r4, #0xc]\n"
        "\tand r0, r1\n"
        "\tstrb r0, [r4, #0xc]\n"
        "\tldr r2, 3f @ =gUnknown_030012B4\n"
        "\tmov r8, r2\n"
        "\tldr r0, [r2]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tlsl r6, r6, #1\n"
        "\tmov sb, r6\n"
        "\tadd r0, sb\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r5\n"
        "\tand r0, r5\n"
        "\tadd r3, r4, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r6, [r3]\n"
        "\tand r1, r6\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r5\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 4f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r4, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 5f @ =gStaticData_0816B98C\n"
        "\tadd r2, r7, #0\n"
        "\tadd r2, #0x84\n"
        "\tstr r0, [r2]\n"
        "\tmov r1, r8\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r3, sb\n"
        "\tadd r6, r3, r0\n"
        "\tldr r0, [r1, #0xc]\n"
        "\tldrh r6, [r6]\n"
        "\tadd r0, r6, r0\n"
        "\tadd r3, r0, #0\n"
        "\tldr r0, 6f @ =gStaticData_0816B9EC\n"
        "\tstr r0, [r2]\n"
        "\tldr r0, [r3, #4]\n"
        "\tldr r1, [r3, #8]\n"
        "\tldr r2, [r3, #0xc]\n"
        "\tstr r0, [r7, #0x30]\n"
        "\tstr r1, [r7, #0x34]\n"
        "\tstr r2, [r7, #0x38]\n"
        "\tldr r0, [r3, #4]\n"
        "\tldr r1, [r3, #8]\n"
        "\tadd r0, r0, r1\n"
        "\tlsr r1, r0, #0x1f\n"
        "\tadd r0, r0, r1\n"
        "\tasr r2, r0, #1\n"
        "\tadd r1, r2, #0\n"
        "\tcmp r2, #0\n"
        "\tbge 1f\n"
        "\tadd r1, r2, #3\n"
        "1:\n"
        "\tasr r1, r1, #2\n"
        "\tldr r0, [r3, #0xc]\n"
        "\tadd r0, r0, r1\n"
        "\tstr r2, [r7, #0x48]\n"
        "\tstr r0, [r7, #0x4c]\n"
        "\tadd r0, r7, #0\n"
        "\tmov r1, #0x10\n"
        "\tbl sub_800C6A8\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "2: .4byte gUnknown_030012D0\n"
        "3: .4byte gUnknown_030012B4\n"
        "4: .4byte gUnknown_030012F0\n"
        "5: .4byte gStaticData_0816B98C\n"
        "6: .4byte gStaticData_0816B9EC\n"
    );
}

/* Same family, tag `0xf`. Ends with the "second `header->0x84` rewrite
 * plus record-field struct copy" shape too, but with the record's own
 * `+8`/`+0xc`/`+0x10` fields (not `+4`/`+8`/`+0xc`) landing in
 * `hdr->0x30`/`0x34`/`0x38` (shuffled relative to how `sub_801FCB4`
 * below orders the same three fields) and a trailing `sub_800C898`
 * call reading the record's own `+4` field. Parked as NAKED for the
 * same `r7` gap. */
NAKED void sub_801F528(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r5, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0xb4\n"
        "\tstr r0, [r5, #0x20]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r5, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0xf\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r5, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r4, #1\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r7, [r5, #0xc]\n"
        "\tand r0, r7\n"
        "\tstrb r0, [r5, #0xc]\n"
        "\tldr r0, 2f @ =gUnknown_030012B4\n"
        "\tmov sl, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r2, r8\n"
        "\tlsl r2, r2, #1\n"
        "\tmov r8, r2\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r4\n"
        "\tand r0, r4\n"
        "\tadd r3, r5, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r4\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 4f @ =gStaticData_0816B98C\n"
        "\tmov r1, #0x84\n"
        "\tadd r1, r1, r6\n"
        "\tmov sb, r1\n"
        "\tstr r0, [r1]\n"
        "\tmov r2, sl\n"
        "\tldr r0, [r2]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r4, [r1, #0xc]\n"
        "\tmov r3, r8\n"
        "\tldrh r3, [r3]\n"
        "\tadd r4, r3, r4\n"
        "\tadd r0, r5, #0\n"
        "\tadd r0, #0x2d\n"
        "\tmov r7, #0\n"
        "\tstrb r7, [r0]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_80087C0\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_80087B4\n"
        "\tadd r0, r5, #0\n"
        "\tmov r1, #0\n"
        "\tbl sub_800872C\n"
        "\tldr r0, 5f @ =gStaticData_0816B9AC\n"
        "\tmov r1, sb\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r4, #0xc]\n"
        "\tldr r1, [r4, #8]\n"
        "\tldr r2, [r4, #0x10]\n"
        "\tstr r0, [r6, #0x30]\n"
        "\tstr r1, [r6, #0x34]\n"
        "\tstr r2, [r6, #0x38]\n"
        "\tldr r1, [r4, #4]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C898\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #0xd\n"
        "\tbl sub_800C6A8\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
        "5: .4byte gStaticData_0816B9AC\n"
    );
}

/* The one instance in this cluster built via `sub_800A604` instead of
 * `sub_8009ED0` (a bigger, self-initializing part object - see
 * `sub_800A604`'s own definition in `src/graphics/actor_part14.c`),
 * with `part` in `r4` and `hdr` in `r8` (the same "header pinned high,
 * dereferenced through its own fresh low-register return value before
 * getting aliased" idiom `sub_802155C`, graphics_loading_21280.c,
 * already established). Ends with the standard OAM trio, a plain
 * `flags |= 0x10`, and a `PlaySfx` call instead of a record re-lookup -
 * the same tail shape `sub_8021748`/`sub_802183C`
 * (graphics_loading_21668.c) use for their own `gStaticData_084A5600`
 * family. `arg3` stays live in `r6` for the collected-bits pack (no
 * `sb` cache this time - the lookup table's address is reloaded fresh). */
void sub_801F680(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 raw0 asm("r0") = arg0;
    register u32 raw1 asm("r1") = arg1;
    register u32 raw2 asm("r2") = arg2;
    register u32 raw3 asm("r3") = arg3;
    register struct actor *part asm("r4");
    register s32 idx asm("r6");
    void *p2;
    void *p3;
    register void *hdr asm("r8");
    u8 *table;

    asm volatile(
        "add r6, r3, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "lsl r6, r6, #0x10\n\t"
        "lsr r6, r6, #0x10\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r3, r6, #0\n\t"
        "bl sub_800A604\n\t"
        "add r4, r0, #0\n\t"
        : "=r" (part), "=r" (idx)
        : "r" (raw0), "r" (raw1), "r" (raw2), "r" (raw3)
        : "r1", "r2", "r3", "lr", "memory");

    p2 = *(void **)gUnknown_030012D0;
    p3 = *(void **)p2;
    *(void **)((u8 *)part + 0x20) = (u8 *)p3 + 0xcc;

    {
        register s32 result asm("r0") = sub_800815C(part);
        register u8 *addr asm("r2") = (u8 *)part + 0x29;
        register s32 acc asm("r1");
        result &= 0xf;
        asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r" (acc));
        acc &= *addr;
        acc |= result;
        *addr = acc;
    }

    sub_8026EDC(0x8c);

    {
        register void *hv asm("r0") = sub_800CA74();
        hdr = hv;
        table = *(u8 **)((u8 *)hv + 0xc);
    }
    {
        register void *tbl asm("r1") = table;
        asm volatile(
            "mov r2, #0x18\n\t"
            "ldrsh r0, [r1, r2]\n\t"
            "add r0, r8\n\t"
            "ldr r2, [r1, #0x1c]\n\t"
            "add r1, r4, #0\n\t"
            "bl sub_803AD80\n\t"
            :
            : "r" (tbl), "r" (hdr), "r" (part)
            : "r0", "r1", "r2", "lr", "memory");
    }

    {
        register s32 tagVal asm("r0") = 0x11;
        register void *hv2 asm("r3") = hdr;
        *(s32 *)((u8 *)hv2 + 0x6c) = tagVal;
    }
    {
        register s32 zero asm("r0") = 0;
        register u8 *addr2d asm("r1") = (u8 *)part + 0x2d;
        *addr2d = zero;
    }

    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);

    {
        register void *hv3 asm("r0") = hdr;
        *(void **)((u8 *)part + 0x44) = hv3;
        table = *(u8 **)((u8 *)hv3 + 0xc);
        {
            register void *tbl2 asm("r1") = table;
            asm volatile(
                "mov r2, #0x18\n\t"
                "ldrsh r0, [r1, r2]\n\t"
                "add r0, r8\n\t"
                "ldr r2, [r1, #0x1c]\n\t"
                "add r1, r4, #0\n\t"
                "bl sub_803AD80\n\t"
                :
                : "r" (tbl2), "r" (hdr), "r" (part)
                : "r0", "r1", "r2", "lr", "memory");
        }
    }

    {
        register s32 one asm("r5");

        asm volatile(
            "mov r0, #1\n\t"
            "mov r5, #1\n\t"
            "strb r0, [r4, #0xa]\n\t"
            "mov r0, #0x7f\n\t"
            "ldrb r3, [r4, #0xc]\n\t"
            "and r0, r0, r3\n\t"
            "strb r0, [r4, #0xc]\n\t"
            : "=r" (one)
            : "r" (part)
            : "r0", "r3", "memory");

        {
            register void *gAddr asm("r0") = &gUnknown_030012B4;

            asm volatile(
                "ldr r0, [r0]\n\t"
                "ldr r1, [r0]\n\t"
                "ldr r0, [r1, #8]\n\t"
                "lsl r6, r6, #1\n\t"
                "add r6, r6, r0\n\t"
                "ldr r2, [r1, #0xc]\n\t"
                "ldrh r6, [r6]\n\t"
                "add r2, r6, r2\n\t"
                "ldrb r6, [r2]\n\t"
                "lsr r0, r6, #1\n\t"
                "eor r0, r0, r5\n\t"
                "and r0, r0, r5\n\t"
                "add r3, r4, #0\n\t"
                "add r3, r3, #0x28\n\t"
                "and r0, r0, r5\n\t"
                "lsl r0, r0, #4\n\t"
                "mov r1, #0x11\n\t"
                "neg r1, r1\n\t"
                "ldrb r6, [r3]\n\t"
                "and r1, r1, r6\n\t"
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
                : "+r" (idx)
                : "r" (part), "r" (gAddr), "r" (one)
                : "r0", "r1", "r2", "r3", "r6", "memory");
        }
    }

    {
        register s32 mask asm("r0") = 0x10;
        register u8 old asm("r1") = part->flags;

        mask |= old;
        part->flags = mask;
    }

    sub_8008E94(gUnknown_030012F0, part);

    {
        register void *val asm("r1") = gStaticData_0816B98C;
        register void *addr84 asm("r0") = (u8 *)hdr + 0x84;
        *(void **)addr84 = val;
    }

    sub_800C6A8(hdr, 5);

    {
        register void *bank asm("r0") = gUnknown_030012BC;
        register s32 vol asm("r2") = 0x100;
        register s32 id asm("r1") = 0x27;
        PlaySfx(bank, id, vol);
    }
}

/* Same family, tag `0x10`, single `hdr->0x84` write, followed by a
 * `part->field_0A` overwrite (to `6`) and the record-relookup tail
 * (`sub_800C6A8(hdr, 2)` + `sub_800C898`) `sub_801F2BC` above also has,
 * minus that function's `r7`-in-`push` set - here it's `sb`/`r8`
 * (shadowed through `r6`/`r7`) that need the extra callee-saved slot,
 * and the nibble reload also lands in `r7`, so this is parked NAKED for
 * the same reason. */
NAKED void sub_801F7B8(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r5, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0xc0\n"
        "\tstr r0, [r5, #0x20]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r5, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #0x10\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r5, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r4, #1\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r7, [r5, #0xc]\n"
        "\tand r0, r7\n"
        "\tstrb r0, [r5, #0xc]\n"
        "\tldr r0, 2f @ =gUnknown_030012B4\n"
        "\tmov sb, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r2, r8\n"
        "\tlsl r2, r2, #1\n"
        "\tmov r8, r2\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r4\n"
        "\tand r0, r4\n"
        "\tadd r3, r5, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r4\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r1, 4f @ =gStaticData_0816B98C\n"
        "\tadd r0, r6, #0\n"
        "\tadd r0, #0x84\n"
        "\tstr r1, [r0]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r4, [r1, #0xc]\n"
        "\tmov r2, r8\n"
        "\tldrh r2, [r2]\n"
        "\tadd r4, r2, r4\n"
        "\tmov r0, #6\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #2\n"
        "\tbl sub_800C6A8\n"
        "\tldr r1, [r4, #4]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C898\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
    );
}

/* Same family, tag `5`. Ends with the "second `header->0x84` rewrite
 * plus record-field struct copy" shape again - this time all six of the
 * record's `+4`/`+8`/`+0xc`/`+0x10`/`+0x14`/`+0x18` fields land in
 * `hdr->0x30`/`0x34`/`0x38`/`0x3c`/`0x40`/`0x44` (with `+0x10` and
 * `+0x14`/`+0x18` read out of address order - `+0x14`/`+0x18` first,
 * `+0x10` last - matching the ROM's own instruction order exactly).
 * Parked NAKED for the same `r7` gap. */
NAKED void sub_801F8DC(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r5, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x3c\n"
        "\tstr r0, [r5, #0x20]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r5, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r3, #5\n"
        "\tstr r3, [r6, #0x6c]\n"
        "\tstr r6, [r5, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r7, #0x18\n"
        "\tldrsh r0, [r1, r7]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r4, #1\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r1, [r5, #0xc]\n"
        "\tand r0, r1\n"
        "\tstrb r0, [r5, #0xc]\n"
        "\tldr r2, 2f @ =gUnknown_030012B4\n"
        "\tmov sl, r2\n"
        "\tldr r0, [r2]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r3, r8\n"
        "\tlsl r3, r3, #1\n"
        "\tmov r8, r3\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r7, [r2]\n"
        "\tlsr r0, r7, #1\n"
        "\teor r0, r4\n"
        "\tand r0, r4\n"
        "\tadd r3, r5, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r4\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 4f @ =gStaticData_0816B98C\n"
        "\tmov r1, #0x84\n"
        "\tadd r1, r1, r6\n"
        "\tmov sb, r1\n"
        "\tstr r0, [r1]\n"
        "\tmov r2, sl\n"
        "\tldr r0, [r2]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r4, [r1, #0xc]\n"
        "\tmov r3, r8\n"
        "\tldrh r3, [r3]\n"
        "\tadd r4, r3, r4\n"
        "\tmov r0, #2\n"
        "\tadd r1, r5, #0\n"
        "\tadd r1, #0x2d\n"
        "\tstrb r0, [r1]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_80087C0\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_80087B4\n"
        "\tadd r0, r5, #0\n"
        "\tmov r1, #0\n"
        "\tbl sub_800872C\n"
        "\tmov r7, #5\n"
        "\tstrb r7, [r5, #0xa]\n"
        "\tldr r0, 5f @ =gStaticData_0816B9CC\n"
        "\tmov r1, sb\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r4, #4]\n"
        "\tldr r1, [r4, #8]\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tstr r0, [r6, #0x30]\n"
        "\tstr r1, [r6, #0x34]\n"
        "\tstr r2, [r6, #0x38]\n"
        "\tldr r0, [r4, #0x14]\n"
        "\tldr r1, [r4, #0x18]\n"
        "\tldr r2, [r4, #0x10]\n"
        "\tstr r0, [r6, #0x3c]\n"
        "\tstr r1, [r6, #0x40]\n"
        "\tstr r2, [r6, #0x44]\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #0xe\n"
        "\tbl sub_800C6A8\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
        "5: .4byte gStaticData_0816B9CC\n"
    );
}

/* Same family, tag `4`. Ends with the "second `header->0x84` rewrite"
 * shape, a `part->field_0A` overwrite (to `6`), and both
 * `sub_800C6A8(hdr, 0xf)` *and* `sub_800C898`/`sub_800C87C` (passing the
 * record's own `+4`/`+8`/`+0xc`/`+0x10` fields straight through as
 * arguments rather than copying them into `hdr` struct fields - unlike
 * every other "record relookup" sibling in this cluster). Parked NAKED
 * for the same `r7` gap. */
NAKED void sub_801FA3C(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r5, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x30\n"
        "\tstr r0, [r5, #0x20]\n"
        "\tadd r0, r5, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r5, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r6, r0, #0\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #4\n"
        "\tstr r0, [r6, #0x6c]\n"
        "\tstr r6, [r5, #0x44]\n"
        "\tldr r1, [r6, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r6, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r4, #1\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r7, [r5, #0xc]\n"
        "\tand r0, r7\n"
        "\tstrb r0, [r5, #0xc]\n"
        "\tldr r0, 2f @ =gUnknown_030012B4\n"
        "\tmov sb, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r2, r8\n"
        "\tlsl r2, r2, #1\n"
        "\tmov r8, r2\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r4\n"
        "\tand r0, r4\n"
        "\tadd r3, r5, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r4\n"
        "\tand r0, r4\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r5, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 4f @ =gStaticData_0816B98C\n"
        "\tadd r2, r6, #0\n"
        "\tadd r2, #0x84\n"
        "\tstr r0, [r2]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r4, [r1, #0xc]\n"
        "\tmov r3, r8\n"
        "\tldrh r3, [r3]\n"
        "\tadd r4, r3, r4\n"
        "\tmov r0, #6\n"
        "\tstrb r0, [r5, #0xa]\n"
        "\tldr r0, 5f @ =gStaticData_0816BA4C\n"
        "\tstr r0, [r2]\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #0xf\n"
        "\tbl sub_800C6A8\n"
        "\tldr r1, [r4, #4]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C898\n"
        "\tldr r1, [r4, #8]\n"
        "\tldr r2, [r4, #0xc]\n"
        "\tldr r3, [r4, #0x10]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800C87C\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
        "5: .4byte gStaticData_0816BA4C\n"
    );
}

/* Same family, tag `3`. This is the "bit-27 test on `part->field_28`
 * gating a different tag value" variant `docs/matching/issue-31-graphics-loading.md`'s
 * second pass flagged but never worked through: after the OAM trio, it
 * re-tests bit 4 of the same `+0x28` byte the collected-bits pack just
 * wrote (`lsl r0, r2, #0x1b` puts that bit at the sign position) and
 * re-toggles it based on the result before the final `sub_800C6A8`
 * call. No OAM-standard `header->0x84` rewrite or record relookup here.
 * Parked NAKED for the same `r7` gap as this cluster's other entries. */
NAKED void sub_801FB74(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tadd r4, r3, #0\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r4, r4, #0x10\n"
        "\tlsr r4, r4, #0x10\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tadd r3, r4, #0\n"
        "\tbl sub_8009ED0\n"
        "\tadd r6, r0, #0\n"
        "\tldr r0, 2f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x24\n"
        "\tstr r0, [r6, #0x20]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r6, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r3, [r2]\n"
        "\tand r1, r3\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r7, r0, #0\n"
        "\tldr r1, [r7, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r7, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #3\n"
        "\tstr r0, [r7, #0x6c]\n"
        "\tstr r7, [r6, #0x44]\n"
        "\tldr r1, [r7, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r7, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r1, #0\n"
        "\tmov sb, r1\n"
        "\tmov r5, #1\n"
        "\tstrb r0, [r6, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r2, [r6, #0xc]\n"
        "\tand r0, r2\n"
        "\tstrb r0, [r6, #0xc]\n"
        "\tldr r0, 3f @ =gUnknown_030012B4\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tlsl r4, r4, #1\n"
        "\tadd r4, r4, r0\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r4, [r4]\n"
        "\tadd r2, r4, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r5\n"
        "\tand r0, r5\n"
        "\tadd r4, r6, #0\n"
        "\tadd r4, #0x28\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tmov r8, r1\n"
        "\tldrb r3, [r4]\n"
        "\tand r1, r3\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r4]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r5\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r4]\n"
        "\tldr r0, 4f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r1, 5f @ =gStaticData_0816B98C\n"
        "\tadd r0, r7, #0\n"
        "\tadd r0, #0x84\n"
        "\tstr r1, [r0]\n"
        "\tadd r0, r6, #0\n"
        "\tadd r0, #0x2d\n"
        "\tmov r1, sb\n"
        "\tstrb r1, [r0]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_80087C0\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_80087B4\n"
        "\tadd r0, r6, #0\n"
        "\tmov r1, #0\n"
        "\tbl sub_800872C\n"
        "\tldrb r2, [r4]\n"
        "\tlsl r0, r2, #0x1b\n"
        "\tmov r1, #0\n"
        "\tcmp r0, #0\n"
        "\tblt 1f\n"
        "\tmov r1, #1\n"
        "1:\n"
        "\tadd r0, r5, #0\n"
        "\tand r0, r1\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, r8\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r4]\n"
        "\tmov r0, #6\n"
        "\tstrb r0, [r6, #0xa]\n"
        "\tadd r0, r7, #0\n"
        "\tmov r1, #1\n"
        "\tbl sub_800C6A8\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "2: .4byte gUnknown_030012D0\n"
        "3: .4byte gUnknown_030012B4\n"
        "4: .4byte gUnknown_030012F0\n"
        "5: .4byte gStaticData_0816B98C\n"
    );
}

/* Same family, tag `8`. Ends with the "second `header->0x84` rewrite
 * plus record-field struct copy" shape, record's own `+8`/`+0xc`/`+0x10`
 * fields into `hdr->0x30`/`0x34`/`0x38` (matching `sub_801F3DC`'s field
 * choice, not `sub_801F528`'s shuffled one), then `sub_800C898` reading
 * the record's own `+4` field. Parked NAKED for the same `r7` gap. This
 * is the last function in this file - `sub_801FDEC`
 * (src/graphics/graphics_loading_1fdec.c) picks up immediately after in
 * ROM order. */
NAKED void sub_801FCB4(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sb\n"
        "\tmov r6, r8\n"
        "\tpush {r6, r7}\n"
        "\tmov r8, r3\n"
        "\tlsl r1, r1, #0x10\n"
        "\tlsr r1, r1, #0x10\n"
        "\tlsl r2, r2, #0x10\n"
        "\tlsr r2, r2, #0x10\n"
        "\tlsl r3, r3, #0x10\n"
        "\tlsr r3, r3, #0x10\n"
        "\tmov r8, r3\n"
        "\tlsl r0, r0, #0x10\n"
        "\tlsr r0, r0, #0x10\n"
        "\tbl sub_8009ED0\n"
        "\tadd r6, r0, #0\n"
        "\tldr r0, 1f @ =gUnknown_030012D0\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tldr r0, [r0]\n"
        "\tadd r0, #0x60\n"
        "\tstr r0, [r6, #0x20]\n"
        "\tadd r0, r6, #0\n"
        "\tbl sub_800815C\n"
        "\tadd r2, r6, #0\n"
        "\tadd r2, #0x29\n"
        "\tmov r1, #0xf\n"
        "\tand r0, r1\n"
        "\tmov r1, #0x10\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r2]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r2]\n"
        "\tmov r0, #0x8c\n"
        "\tbl sub_8026EDC\n"
        "\tbl sub_800CA74\n"
        "\tadd r4, r0, #0\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmov r2, #0x18\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadd r0, r4, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #8\n"
        "\tstr r0, [r4, #0x6c]\n"
        "\tstr r4, [r6, #0x44]\n"
        "\tldr r1, [r4, #0xc]\n"
        "\tmov r3, #0x18\n"
        "\tldrsh r0, [r1, r3]\n"
        "\tadd r0, r4, r0\n"
        "\tldr r2, [r1, #0x1c]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_803AD80\n"
        "\tmov r0, #1\n"
        "\tmov r5, #1\n"
        "\tstrb r0, [r6, #0xa]\n"
        "\tmov r0, #0x7f\n"
        "\tldrb r7, [r6, #0xc]\n"
        "\tand r0, r7\n"
        "\tstrb r0, [r6, #0xc]\n"
        "\tldr r0, 2f @ =gUnknown_030012B4\n"
        "\tmov sb, r0\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tmov r2, r8\n"
        "\tlsl r2, r2, #1\n"
        "\tmov r8, r2\n"
        "\tadd r0, r8\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldrh r0, [r0]\n"
        "\tadd r2, r0, r2\n"
        "\tldrb r3, [r2]\n"
        "\tlsr r0, r3, #1\n"
        "\teor r0, r5\n"
        "\tand r0, r5\n"
        "\tadd r3, r6, #0\n"
        "\tadd r3, #0x28\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #4\n"
        "\tmov r1, #0x11\n"
        "\tneg r1, r1\n"
        "\tldrb r7, [r3]\n"
        "\tand r1, r7\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldrb r2, [r2]\n"
        "\tlsr r0, r2, #2\n"
        "\tand r0, r5\n"
        "\tand r0, r5\n"
        "\tlsl r0, r0, #5\n"
        "\tmov r2, #0x21\n"
        "\tneg r2, r2\n"
        "\tand r1, r2\n"
        "\torr r1, r0\n"
        "\tstrb r1, [r3]\n"
        "\tldr r0, 3f @ =gUnknown_030012F0\n"
        "\tldr r0, [r0]\n"
        "\tadd r1, r6, #0\n"
        "\tbl sub_8008E94\n"
        "\tldr r0, 4f @ =gStaticData_0816B98C\n"
        "\tadd r2, r4, #0\n"
        "\tadd r2, #0x84\n"
        "\tstr r0, [r2]\n"
        "\tmov r1, sb\n"
        "\tldr r0, [r1]\n"
        "\tldr r1, [r0]\n"
        "\tldr r0, [r1, #8]\n"
        "\tadd r8, r0\n"
        "\tldr r1, [r1, #0xc]\n"
        "\tmov r3, r8\n"
        "\tldrh r3, [r3]\n"
        "\tadd r1, r3, r1\n"
        "\tmov r0, #3\n"
        "\tstrb r0, [r6, #0xa]\n"
        "\tldr r0, 5f @ =gStaticData_0816BA8C\n"
        "\tstr r0, [r2]\n"
        "\tldr r0, [r1, #8]\n"
        "\tldr r2, [r1, #0xc]\n"
        "\tldr r3, [r1, #0x10]\n"
        "\tstr r0, [r4, #0x30]\n"
        "\tstr r2, [r4, #0x34]\n"
        "\tstr r3, [r4, #0x38]\n"
        "\tldr r1, [r1, #4]\n"
        "\tadd r0, r4, #0\n"
        "\tbl sub_800C898\n"
        "\tadd r0, r4, #0\n"
        "\tmov r1, #0xd\n"
        "\tbl sub_800C6A8\n"
        "\tpop {r3, r4}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "1: .4byte gUnknown_030012D0\n"
        "2: .4byte gUnknown_030012B4\n"
        "3: .4byte gUnknown_030012F0\n"
        "4: .4byte gStaticData_0816B98C\n"
        "5: .4byte gStaticData_0816BA8C\n"
    );
}

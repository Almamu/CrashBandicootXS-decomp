#include "core.h"
#include "actor.h"
#include "actor_self.h"

/* GitHub issue #9/#10: 0x0800A884 - the same big, still-unnamed "part"
 * object family as `actor_part15.c`/`actor_part48.c`; raw offset casts
 * throughout for the same reason those files give. */

extern void sub_800A0FC(void *self);
extern void sub_80231EC(void *arg0, s32 arg1);
extern void *sub_80083B8(void *part);
extern s32 sub_8026BC0(void *arg0, s32 x, s32 y);
extern void *gUnknown_03001308;
extern void *gUnknown_030012C0;
extern u8 gStaticData_0816B300[];

struct a884_game {
    u8 unk_00[0x29];
    u8 kind;            // 0x29
    u8 busy;            // 0x2A
};

struct a884_part {
    s32 x;              // 0x00
    s32 y;              // 0x04
    u8 unk_08[4];
    u8 flags;           // 0x0C - bit 7: active, bit 6: set by kind 1
    u8 unk_0d[0xb];
    u8 *vtable;         // 0x18
    u8 unk_1c[0xc];
    u32 unk_28_0:4;     // 0x28
    u32 mirrorX:1;
    u32 unk_28_5:27;
    u8 unk_2c[0x3c];
    u8 hitAxes;         // 0x68
    u8 unk_69[0x23];
    s32 unk_8c;         // 0x8C
    u8 unk_90[0x1c];
    s32 unk_ac;         // 0xAC
    u8 unk_b0[0x50];
    u8 f100;            // 0x100
    u8 f101;            // 0x101
    u8 f102;            // 0x102
    u8 f103;            // 0x103
    u8 unk_104;
    u8 f105;            // 0x105
};

struct a884_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

/* A per-frame "reentrancy guard"-shaped wrapper (only runs if
 * `self+0xc` bit 7 is set): fires `self->table+0x70`'s trampoline via
 * `sub_803AD7C`, then calls `sub_800A0FC` (still raw) with the global
 * `gUnknown_03001308+0x2a` flag held set for the duration. If
 * `self+0xac` (a pointer, cleared here) was non-null, sets `self+0x68`
 * bit 3 and clears the `+0x100`/`+0x102`/`+0x103` flag bytes. Then
 * dispatches on `gUnknown_03001308+0x29` (a pending-action "kind"
 * byte, cleared back to 0 by every path here): kind 0 additionally
 * resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` is exactly 8; kinds
 * 1/5/7/9 (`gStaticData_0816B300`'s index scheme - see the `case`
 * labels below) are no-ops beyond the shared reset; kind 1 also sets
 * `self+0xc` bit 6, clears `+0x8c`, calls `sub_80231EC`, and fires the
 * `self->table+0x68` trampoline (arg 1); kind 5 sets the `+0x100`
 * flag; kind 7 sets `+0x102`; kind 10 sets `+0x103`. Finally, looks up
 * the current keyframe record (`sub_80083B8`, already parked in
 * `actor_part5.c`) and picks a `{s16 x, s16 y}` offset table off its
 * `+4` byte's upper nibble - the exact same `sub_80084C4`
 * (`actor_part6.c`) case-to-block mapping (0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else -> the fixed fallback
 * `gStaticData_0816B300`) - applies it (mirrored by `self+0x28` bit 4)
 * to `self`'s de-Q8'd position, and probes the result via
 * `sub_8026BC0` (still raw). A hit (code 6) snaps `self`'s Y position
 * down to the next multiple of 8 (unless `+0x101` is already set) and
 * fires the table+0x68 trampoline with code `0x17`; any other code
 * fires the same trampoline with code `0x18` if `+0x101` is set.
 * Returns the (possibly just-updated) `self+0x68` state byte.
 *
 * Moved here from asm/code_3_2_16_a884.s as NAKED (issue #9 raw-asm
 * pass). The NON_MATCHING draft below replaced an older one built from
 * register pins and asm islands; it is plain C and closer (127 halfwords
 * off under old_agbcc, 180 under agbcc, against the old draft's 137 under
 * both). The `movs #0x40`/`movs #8` before their `ldrb` show this is
 * old_agbcc code. The methods are gcc 2.x virtual calls through
 * `self+0x18` (`_call_via_r1`/`_call_via_r4`), and the offset-table
 * switch is `sub_80084C4` (actor_part6.c) inlined. What's left: the ROM
 * builds the 0x100/0x102/0x103 flag offsets by walking one register
 * (`adds r1, #3`, `subs r2, #3`), while the draft gives each constant its
 * own register or a literal-pool load. That also makes gcc cross-jump
 * the kind-5 case tail into the kind-7/kind-10 one. */
#if NON_MATCHING
ACTOR_CALL_VIA_ALIASES

#define A884_METHOD(obj, off) ((struct a884_method *)((obj)->vtable + (off)))
typedef void (*a884_fn0)(void *self);
typedef void (*a884_fn3)(void *self, s32 a, s32 b, s32 c);

#define CALL_M70(obj)                                                          \
    if (1) {                                                                   \
        struct a884_method *_m = A884_METHOD(obj, 0x70);                       \
        ((a884_fn0)_m->fn)((u8 *)(obj) + _m->thisOffset);                      \
    } else (void)0

#define CALL_M68(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        struct a884_method *_m = A884_METHOD(obj, 0x68);                       \
        ((a884_fn3)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c));       \
    } else (void)0

static inline s16 *A884Offset(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    s16 *result;

    switch (type) {
    case 0:
        result = (s16 *)((u8 *)info + 0x24);
        break;
    case 3:
    case 4:
        result = (s16 *)gStaticData_0816B300;
        break;
    case 1:
    case 2:
        result = (s16 *)gStaticData_0816B300;
        break;
    case 5:
        result = (s16 *)gStaticData_0816B300;
        break;
    case 6:
        result = (s16 *)((u8 *)info + 0x14);
        break;
    default:
        result = (s16 *)gStaticData_0816B300;
        break;
    }
    return result;
}

u8 sub_800A884(struct a884_part *self)
{
    if (self->flags >> 7) {
        u8 kind;
        s16 *off;
        s32 x, y;
        s32 zero;
        asm("" : "=r"(zero) : "0"(0));
        self->hitAxes = zero;
        self->f105 = zero;
        CALL_M70(self);
        self->f105 = 1;
        ((struct a884_game *)gUnknown_03001308)->busy = 1;
        sub_800A0FC(self);
        ((struct a884_game *)gUnknown_03001308)->busy = zero;
        if (self->unk_ac != 0) {
            self->hitAxes |= 8;
            self->unk_ac = zero;
            self->f100 = zero;
            self->f102 = zero;
            self->f103 = zero;
        }
        kind = ((struct a884_game *)gUnknown_03001308)->kind;
        if (kind != 0) {
            switch (kind) {
            case 1:
                self->flags |= 0x40;
                self->unk_8c = 0;
                sub_80231EC(gUnknown_030012C0, 0);
                CALL_M68(self, 0, 1, 0);
                break;
            case 2:
            case 3:
            case 4:
                break;
            case 5:
                self->f102 = 0;
                self->f103 = 0;
                self->f100 = 1;
                break;
            case 7:
                self->f103 = 0;
                self->f100 = 0;
                self->f102 = 1;
                break;
            case 6:
            case 8:
            case 9:
                break;
            case 10:
                self->f102 = 0;
                self->f100 = 0;
                self->f103 = 1;
                break;
            }
            ((struct a884_game *)gUnknown_03001308)->kind = 0;
        } else if (self->hitAxes == 8) {
                self->f102 = 0;
                self->f103 = 0;
                self->f100 = 0;
        }

        off = A884Offset(self);
        x = self->x >> 8;
        y = self->y >> 8;
        if (self->mirrorX)
            x -= off[0];
        else
            x += off[0];
        y += off[1];
        if (sub_8026BC0(gUnknown_03001308, x, y) == 6) {
            if (self->f101 == 0) {
                s32 snap = (y & 0x00FFFFF8) + 7;

                snap -= y;
                self->y += snap << 8;
                CALL_M68(self, 0, 0x17, 0);
            }
        } else if (self->f101 != 0) {
            CALL_M68(self, 0, 0x18, 0);
        }
    }
    return self->hitAxes;
}
#else
NAKED u8 sub_800A884(struct a884_part *self)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tadds r5, r0, #0\n"
        "\tldrb r1, [r5, #0xc]\n"
        "\tlsrs r0, r1, #7\n"
        "\tcmp r0, #0\n"
        "\tbne _0800A892\n"
        "\tb _0800AADC\n"
        "_0800A892:\n"
        "\tmovs r4, #0\n"
        "\tadds r7, r5, #0\n"
        "\tadds r7, #0x68\n"
        "\tstrb r4, [r7]\n"
        "\tldr r2, _0800A90C\n"
        "\tadds r6, r5, r2\n"
        "\tstrb r4, [r6]\n"
        "\tldr r1, [r5, #0x18]\n"
        "\tadds r1, #0x70\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r5, r0\n"
        "\tldr r1, [r1, #4]\n"
        "\tbl sub_803AD7C\n"
        "\tmovs r1, #1\n"
        "\tstrb r1, [r6]\n"
        "\tldr r6, _0800A910\n"
        "\tldr r0, [r6]\n"
        "\tadds r0, #0x2a\n"
        "\tstrb r1, [r0]\n"
        "\tadds r0, r5, #0\n"
        "\tbl sub_800A0FC\n"
        "\tldr r0, [r6]\n"
        "\tadds r0, #0x2a\n"
        "\tstrb r4, [r0]\n"
        "\tadds r1, r5, #0\n"
        "\tadds r1, #0xac\n"
        "\tldr r0, [r1]\n"
        "\tcmp r0, #0\n"
        "\tbeq _0800A8F2\n"
        "\tmovs r0, #8\n"
        "\tldrb r2, [r7]\n"
        "\torrs r0, r2\n"
        "\tstrb r0, [r7]\n"
        "\tstr r4, [r1]\n"
        "\tmovs r1, #0x80\n"
        "\tlsls r1, r1, #1\n"
        "\tadds r0, r5, r1\n"
        "\tstrb r4, [r0]\n"
        "\tmovs r2, #0x81\n"
        "\tlsls r2, r2, #1\n"
        "\tadds r0, r5, r2\n"
        "\tstrb r4, [r0]\n"
        "\tadds r1, #3\n"
        "\tadds r0, r5, r1\n"
        "\tstrb r4, [r0]\n"
        "_0800A8F2:\n"
        "\tldr r0, [r6]\n"
        "\tadds r0, #0x29\n"
        "\tldrb r1, [r0]\n"
        "\tcmp r1, #0\n"
        "\tbeq _0800A9DC\n"
        "\tsubs r0, r1, #1\n"
        "\tcmp r0, #9\n"
        "\tbhi _0800A9CE\n"
        "\tlsls r0, r0, #2\n"
        "\tldr r1, _0800A914\n"
        "\tadds r0, r0, r1\n"
        "\tldr r0, [r0]\n"
        "\tmov pc, r0\n"
        "\t.align 2, 0\n"
        "_0800A90C: .4byte 0x00000105\n"
        "_0800A910: .4byte gUnknown_03001308\n"
        "_0800A914: .4byte _0800A918\n"
        "_0800A918:\n"
        "\t.4byte _0800A940\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A978\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A998\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A9CE\n"
        "\t.4byte _0800A9B4\n"
        "_0800A940:\n"
        "\tmovs r0, #0x40\n"
        "\tldrb r2, [r5, #0xc]\n"
        "\torrs r0, r2\n"
        "\tstrb r0, [r5, #0xc]\n"
        "\tadds r1, r5, #0\n"
        "\tadds r1, #0x8c\n"
        "\tmovs r0, #0\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, _0800A974\n"
        "\tldr r0, [r0]\n"
        "\tmovs r1, #0\n"
        "\tbl sub_80231EC\n"
        "\tldr r1, [r5, #0x18]\n"
        "\tadds r1, #0x68\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r5, r0\n"
        "\tldr r4, [r1, #4]\n"
        "\tmovs r1, #0\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #0\n"
        "\tbl sub_803AD88\n"
        "\tb _0800A9CE\n"
        "\t.align 2, 0\n"
        "_0800A974: .4byte gUnknown_030012C0\n"
        "_0800A978:\n"
        "\tmovs r0, #0x81\n"
        "\tlsls r0, r0, #1\n"
        "\tadds r1, r5, r0\n"
        "\tmovs r0, #0\n"
        "\tstrb r0, [r1]\n"
        "\tldr r2, _0800A994\n"
        "\tadds r1, r5, r2\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r1, #1\n"
        "\tsubs r2, #3\n"
        "\tadds r0, r5, r2\n"
        "\tstrb r1, [r0]\n"
        "\tb _0800A9CE\n"
        "\t.align 2, 0\n"
        "_0800A994: .4byte 0x00000103\n"
        "_0800A998:\n"
        "\tldr r0, _0800A9B0\n"
        "\tadds r1, r5, r0\n"
        "\tmovs r0, #0\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tadds r1, r5, r2\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r0, #1\n"
        "\tadds r2, #2\n"
        "\tb _0800A9CA\n"
        "\t.align 2, 0\n"
        "_0800A9B0: .4byte 0x00000103\n"
        "_0800A9B4:\n"
        "\tmovs r0, #0x81\n"
        "\tlsls r0, r0, #1\n"
        "\tadds r1, r5, r0\n"
        "\tmovs r0, #0\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tadds r1, r5, r2\n"
        "\tstrb r0, [r1]\n"
        "\tmovs r0, #1\n"
        "\tadds r2, #3\n"
        "_0800A9CA:\n"
        "\tadds r1, r5, r2\n"
        "\tstrb r0, [r1]\n"
        "_0800A9CE:\n"
        "\tldr r0, _0800A9D8\n"
        "\tldr r0, [r0]\n"
        "\tadds r0, #0x29\n"
        "\tmovs r1, #0\n"
        "\tb _0800A9F4\n"
        "\t.align 2, 0\n"
        "_0800A9D8: .4byte gUnknown_03001308\n"
        "_0800A9DC:\n"
        "\tldrb r7, [r7]\n"
        "\tcmp r7, #8\n"
        "\tbne _0800A9F6\n"
        "\tmovs r2, #0x81\n"
        "\tlsls r2, r2, #1\n"
        "\tadds r0, r5, r2\n"
        "\tstrb r1, [r0]\n"
        "\tadds r2, #1\n"
        "\tadds r0, r5, r2\n"
        "\tstrb r1, [r0]\n"
        "\tsubs r2, #3\n"
        "\tadds r0, r5, r2\n"
        "_0800A9F4:\n"
        "\tstrb r1, [r0]\n"
        "_0800A9F6:\n"
        "\tadds r0, r5, #0\n"
        "\tbl sub_80083B8\n"
        "\tadds r2, r0, #0\n"
        "\tldr r0, [r2, #4]\n"
        "\tldrb r0, [r0]\n"
        "\tlsrs r0, r0, #4\n"
        "\tcmp r0, #6\n"
        "\tbhi _0800AA40\n"
        "\tlsls r0, r0, #2\n"
        "\tldr r1, _0800AA14\n"
        "\tadds r0, r0, r1\n"
        "\tldr r0, [r0]\n"
        "\tmov pc, r0\n"
        "\t.align 2, 0\n"
        "_0800AA14: .4byte _0800AA18\n"
        "_0800AA18:\n"
        "\t.4byte _0800AA34\n"
        "\t.4byte _0800AA40\n"
        "\t.4byte _0800AA40\n"
        "\t.4byte _0800AA40\n"
        "\t.4byte _0800AA40\n"
        "\t.4byte _0800AA40\n"
        "\t.4byte _0800AA3A\n"
        "_0800AA34:\n"
        "\tadds r3, r2, #0\n"
        "\tadds r3, #0x24\n"
        "\tb _0800AA42\n"
        "_0800AA3A:\n"
        "\tadds r3, r2, #0\n"
        "\tadds r3, #0x14\n"
        "\tb _0800AA42\n"
        "_0800AA40:\n"
        "\tldr r3, _0800AA60\n"
        "_0800AA42:\n"
        "\tldr r0, [r5]\n"
        "\tasrs r1, r0, #8\n"
        "\tldr r0, [r5, #4]\n"
        "\tasrs r4, r0, #8\n"
        "\tadds r0, r5, #0\n"
        "\tadds r0, #0x28\n"
        "\tldrb r0, [r0]\n"
        "\tlsls r0, r0, #0x1b\n"
        "\tcmp r0, #0\n"
        "\tbge _0800AA64\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r3, r2]\n"
        "\tsubs r1, r1, r0\n"
        "\tb _0800AA6A\n"
        "\t.align 2, 0\n"
        "_0800AA60: .4byte gStaticData_0816B300\n"
        "_0800AA64:\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r3, r2]\n"
        "\tadds r1, r1, r0\n"
        "_0800AA6A:\n"
        "\tmovs r2, #2\n"
        "\tldrsh r0, [r3, r2]\n"
        "\tadds r4, r4, r0\n"
        "\tldr r0, _0800AAB0\n"
        "\tldr r0, [r0]\n"
        "\tadds r2, r4, #0\n"
        "\tbl sub_8026BC0\n"
        "\tcmp r0, #6\n"
        "\tbne _0800AABC\n"
        "\tldr r1, _0800AAB4\n"
        "\tadds r0, r5, r1\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0\n"
        "\tbne _0800AADC\n"
        "\tldr r0, _0800AAB8\n"
        "\tands r0, r4\n"
        "\tadds r0, #7\n"
        "\tsubs r0, r0, r4\n"
        "\tlsls r0, r0, #8\n"
        "\tldr r1, [r5, #4]\n"
        "\tadds r1, r1, r0\n"
        "\tstr r1, [r5, #4]\n"
        "\tldr r1, [r5, #0x18]\n"
        "\tadds r1, #0x68\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r5, r0\n"
        "\tldr r4, [r1, #4]\n"
        "\tmovs r1, #0\n"
        "\tmovs r2, #0x17\n"
        "\tmovs r3, #0\n"
        "\tbl sub_803AD88\n"
        "\tb _0800AADC\n"
        "\t.align 2, 0\n"
        "_0800AAB0: .4byte gUnknown_03001308\n"
        "_0800AAB4: .4byte 0x00000101\n"
        "_0800AAB8: .4byte 0x00FFFFF8\n"
        "_0800AABC:\n"
        "\tldr r1, _0800AAE8\n"
        "\tadds r0, r5, r1\n"
        "\tldrb r0, [r0]\n"
        "\tcmp r0, #0\n"
        "\tbeq _0800AADC\n"
        "\tldr r1, [r5, #0x18]\n"
        "\tadds r1, #0x68\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tadds r0, r5, r0\n"
        "\tldr r4, [r1, #4]\n"
        "\tmovs r1, #0\n"
        "\tmovs r2, #0x18\n"
        "\tmovs r3, #0\n"
        "\tbl sub_803AD88\n"
        "_0800AADC:\n"
        "\tadds r0, r5, #0\n"
        "\tadds r0, #0x68\n"
        "\tldrb r0, [r0]\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r1}\n"
        "\tbx r1\n"
        "\t.align 2, 0\n"
        "_0800AAE8: .4byte 0x00000101\n"
        ".syntax divided\n");
}

#endif /* NON_MATCHING */
asm(".align 2, 0");

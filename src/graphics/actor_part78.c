#include "core.h"
#include "actor.h"
#include "actor_self.h"

/* GitHub issue #9/#10: 0x0800A884 - the same big, still-unnamed "part"
 * object family as `actor_part15.c`/`actor_part48.c`; raw offset casts
 * throughout for the same reason those files give. */

extern void sub_800A0FC(void *self);
extern void SetMaskLevel(void *arg0, s32 arg1);
extern void *sub_80083B8(void *part);
extern s32 sub_8026BC0(void *arg0, s32 x, s32 y);
extern void *gLevelLayers;
extern void *gLevelState;
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
 * `_call_via_r1`, then calls `sub_800A0FC` (still raw) with the global
 * `gLevelLayers+0x2a` flag held set for the duration. If
 * `self+0xac` (a pointer, cleared here) was non-null, sets `self+0x68`
 * bit 3 and clears the `+0x100`/`+0x102`/`+0x103` flag bytes. Then
 * dispatches on `gLevelLayers+0x29` (a pending-action "kind"
 * byte, cleared back to 0 by every path here): kind 0 additionally
 * resets `+0x100`/`+0x102`/`+0x103` if `self+0x68` is exactly 8; kinds
 * 1/5/7/9 (`gStaticData_0816B300`'s index scheme - see the `case`
 * labels below) are no-ops beyond the shared reset; kind 1 also sets
 * `self+0xc` bit 6, clears `+0x8c`, calls `SetMaskLevel`, and fires the
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
 * pass); matched in a later pass. Built with old_agbcc (the `movs
 * #0x40`/`movs #8` before their `ldrb`). The methods are gcc 2.x virtual
 * calls through `self+0x18` (`_call_via_r1`/`_call_via_r4`), and the
 * offset-table switch is `sub_80084C4` (actor_part6.c) inlined. The
 * ROM's "walking" flag offsets (`adds r1, #3`, `subs r2, #3`) are
 * reload's move2add reusing a reload register; they come from r3 holds
 * (no code) that keep reload rotating through r0-r2 only. */

#define A884_METHOD(obj, off) ((struct a884_method *)((obj)->vtable + (off)))
typedef void (*a884_fn0)(void *self);
typedef void (*a884_fn3)(void *self, s32 a, s32 b, s32 c);

#define CALL_M68(obj, a, b, c)                                                 \
    if (1) {                                                                   \
        struct a884_method *_m = A884_METHOD(obj, 0x68);                       \
        ((a884_fn3)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c));       \
    } else (void)0

/* The same dispatch with r3 held (no code) from before the method
 * lookup to after the `this` adjustment: reload skips a live hard
 * register, so the `ldrsh` index reload takes r2 as in the ROM. */
#define CALL_M70H(obj)                                                         \
    if (1) {                                                                   \
        register s32 _h asm("r3");                                             \
        struct a884_method *_m;                                                \
        void *_t;                                                              \
        asm("" : "=r"(_h)); /* r3 hold starts: no code */                      \
        _m = A884_METHOD(obj, 0x70);                                           \
        _t = (u8 *)(obj) + _m->thisOffset;                                     \
        asm("" : : "r"(_h)); /* r3 hold ends: no code */                       \
        ((a884_fn0)_m->fn)(_t);                                                \
    } else (void)0

#define CALL_M68H(obj, a, b, c)                                                \
    if (1) {                                                                   \
        register s32 _h asm("r3");                                             \
        struct a884_method *_m;                                                \
        void *_t;                                                              \
        asm("" : "=r"(_h)); /* r3 hold starts: no code */                      \
        _m = A884_METHOD(obj, 0x68);                                           \
        _t = (u8 *)(obj) + _m->thisOffset;                                     \
        asm("" : : "r"(_h)); /* r3 hold ends: no code */                       \
        ((a884_fn3)_m->fn)(_t, (a), (b), (c));                                 \
    } else (void)0

static inline s16 *A884Offset(void *part)
{
    void *info = sub_80083B8(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    register s16 *result asm("r3"); /* the ROM builds it in r3 */

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
        register s32 hold asm("r3");

        /* Constant-init without live-range doubling (no code). */
        asm("" : "=r"(zero) : "0"(0));
        self->hitAxes = zero;
        self->f105 = zero;
        CALL_M70H(self);
        self->f105 = 1;
        ((struct a884_game *)gLevelLayers)->busy = 1;
        sub_800A0FC(self);
        /* r3 hold (no code) over the flag resets and the kind switch:
         * the ROM's reloads rotate through r0-r2 only, so the flag
         * offsets reuse one register (`adds r1, #3`, `subs r2, #3`). */
        asm("" : "=r"(hold));
        ((struct a884_game *)gLevelLayers)->busy = zero;
        if (self->unk_ac != 0) {
            self->hitAxes |= 8;
            self->unk_ac = zero;
            self->f100 = zero;
            self->f102 = zero;
            self->f103 = zero;
        }
        kind = ((struct a884_game *)gLevelLayers)->kind;
        if (kind != 0) {
            switch (kind) {
            case 1:
                self->flags |= 0x40;
                {
                    /* The ROM stores a fresh 0 from r0 (address in r1). */
                    s32 *_p = &self->unk_8c;
                    register s32 _z asm("r0") = 0;
                    *_p = _z;
                }
                SetMaskLevel(gLevelState, 0);
                CALL_M68H(self, 0, 1, 0);
                break;
            case 2:
            case 3:
            case 4:
                break;
            case 5:
                self->f102 = 0;
                self->f103 = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    asm("" : "=r"(_one) : "0"(1));
                    self->f100 = _one;
                }
                break;
            case 7:
                self->f103 = 0;
                self->f100 = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    asm("" : "=r"(_one) : "0"(1));
                    self->f102 = _one;
                }
                break;
            case 6:
            case 8:
            case 9:
                break;
            case 10:
                self->f102 = 0;
                self->f100 = 0;
                {
                    /* Constant-init (no code): the 1 is set before the
                     * address, as in the ROM, which keeps the kind-5 tail
                     * from being cross-jumped. */
                    s32 _one;
                    asm("" : "=r"(_one) : "0"(1));
                    self->f103 = _one;
                }
                break;
            }
            ((struct a884_game *)gLevelLayers)->kind = 0;
        } else if (self->hitAxes == 8) {
            self->f102 = 0;
            self->f103 = 0;
            self->f100 = 0;
        }

        asm("" : : "r"(hold)); /* r3 hold ends: no code */
        off = A884Offset(self);
        x = self->x >> 8;
        y = self->y >> 8;
        if (self->mirrorX)
            x -= off[0];
        else
            x += off[0];
        y += off[1];
        if (sub_8026BC0(gLevelLayers, x, y) == 6) {
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
asm(".align 2, 0");

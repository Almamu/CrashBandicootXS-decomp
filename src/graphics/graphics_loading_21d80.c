#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;
extern void ***gUnknown_030012D0;
extern void *gUnknown_030012EC;
extern void *gUnknown_030012E4;

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void sub_8008E94(void *manager, void *value);

/* Sets `part->field_29`'s low nibble to `sub_800815C(part)`'s result,
 * keeping the high nibble - same idiom as `UPDATE_ICON_FRAME_NIBBLE`
 * (src/graphics/settings_menu6.c, confirmed matching for `sub_8005A78`),
 * adapted for a raw-offset `struct actor *` instead of a named
 * `field_29`, since this object's tail past `struct actor`'s 0x1c
 * bytes isn't its own named struct here (see `trigger_effect.c`'s same
 * caveat). Same macro as src/graphics/graphics_loading_21bfc.c - not
 * shared via a header since both files only need it locally. */
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

/* Spawns a full visual effect via `sub_8008434`: points its `+0x20`
 * table pointer at `gStaticData_084A5600`'s master 12-byte record 38
 * (`table_base + 0x1c8` - the same record `overlay_ui`'s
 * `sub_80063D8` dialog-box spawner uses, see docs/rom_map.md's
 * "`gStaticData_084A5600` record-indexed" writeup), tags it (`+0x2d =
 * 1`), builds it via the standard `sub_80087C0`/`sub_80087B4`/
 * `sub_800872C` OAM trio, sets its `+0x29` bitfield via
 * `sub_800815C`/`UPDATE_PART_FRAME_NIBBLE`, sets `+0xa` to the fixed
 * `0x25`, then registers it into `gUnknown_030012EC`'s manager via
 * `sub_8008E94`. One of four near-identical siblings in this chunk
 * (`sub_8021DFC`/`sub_8021E78`/`sub_8021EF4`), differing only in the
 * `+0x2d`/`+0xa` constants. */
void sub_8021D80(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 1;
    register u8 field0A asm("r6") = 0x25;
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    sub_8008E94(gUnknown_030012EC, part);
}

/* Same shape as `sub_8021D80` above, tag `0`, `+0xa = 0x24`. */
void sub_8021DFC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 0;
    register u8 field0A asm("r6") = 0x24;
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    sub_8008E94(gUnknown_030012EC, part);
}

/* Same shape as `sub_8021D80` above, tag `2`, `+0xa = 0x23`. */
void sub_8021E78(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 2;
    register u8 field0A asm("r6") = 0x23;
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    sub_8008E94(gUnknown_030012EC, part);
}

/* Same shape as `sub_8021D80` above, tag `3`, `+0xa = 0x26`. */
void sub_8021EF4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 3;
    register u8 field0A asm("r6") = 0x26;
    struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    sub_8008E94(gUnknown_030012EC, part);
}

extern u8 sub_8023418(void *self);
extern struct actor *sub_8011B0C(u16 arg0, u16 arg1, u16 arg2, u16 arg3);

/* Gated spawn (see `sub_802200C` below for the sibling shape), but
 * built via `sub_8011B0C` instead of `sub_8008434`, gated by
 * `sub_8023418(gUnknown_030012C0)` being true instead of a flag-bit
 * test, table offset `table_base + 0x1b0`, tag `0`, `+0xa = 0x1c`, and
 * an extra `flags |= 0x10` on the constructed object before
 * registering it. */
void sub_8021F70(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (sub_8023418(gUnknown_030012C0)) {
        register struct actor *part asm("r4") = sub_8011B0C(arg0, arg1, arg2, arg3);

        *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x1b0;
        {
            register u8 tag asm("r0") = 0;
            register u8 *addr asm("r1") = (u8 *)part + 0x2d;
            *addr = tag;
        }
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        UPDATE_PART_FRAME_NIBBLE(part);
        *((u8 *)part + 0xa) = 0x1c;
        {
            register u8 mask asm("r0") = 0x10;
            register u8 old asm("r1") = part->flags;

            mask |= old;
            part->flags = mask;
        }
        sub_8008E94(gUnknown_030012EC, part);
    }
}

/* Same spawn shape as `sub_8021D80`'s family above (record 32 -
 * `table_base + 0x180`, tag `4`, `+0xa = 0x21`), but gated: does
 * nothing at all unless bit 3 of `gUnknown_030012C0+2` is clear. Does
 * not return the spawned object (the ROM's shared exit pops straight
 * into `r0` from the stack, discarding whatever was last computed
 * there - matches a `void` return exactly). */
void sub_802200C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5");
    register u8 field0A asm("r6");
    struct actor *part;
    register u8 *gv asm("r1") = gUnknown_030012C0;
    register s32 mask asm("r0") = 8;
    register u8 byte asm("r1");

    byte = gv[2];
    if (mask & byte) {
        return;
    }
    tag = 4;
    field0A = 0x21;
    part = sub_8008434(arg0, arg1, arg2, arg3);
    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + 0x180;
    *((u8 *)part + 0x2d) = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    sub_8008E94(gUnknown_030012EC, part);
}

extern void sub_8023500(void *self, s32 *point);

/* New shape (docs/rom_map.md's 15-slot dispatch table, slot 12): a
 * plain state-write, no sound/spawn - never reads `arg0`/`arg3` at all
 * (matches the ROM, which never touches r0/r3), packs `arg1`/`arg2`
 * into a stack `{x, y}` pair and calls `sub_8023500` (already matched
 * in game_loop10.c), which just stores them into
 * `gUnknown_030012C0->0x1c0`/`->0x1c4`. The ROM truncates both u16
 * args in one batch (`lsl r1,r1 / lsl r2,r2` then `lsr r3,r1 / lsr
 * r4,r2`) landing the truncated values in different registers (r1->r3,
 * r2->r4) than plain C produces here - gcc instead coalesces the
 * truncated value right back into r1/r2 (the same register it was
 * already in) since nothing else forces it into r3/r4 first. `arg1`/
 * `arg2` are kept `u32` (deferred truncation, same idiom used
 * elsewhere in this file) and the exact two-instruction-pair truncation
 * is spelled out via inline asm instead, forcing the ROM's register
 * choice directly. */
void sub_802209C(u32 arg0, u32 arg1, u32 arg2, u16 arg3)
{
    register u32 rx asm("r1") = arg1;
    register u32 ry asm("r2") = arg2;
    register s32 x asm("r3");
    register s32 y asm("r4");
    s32 point[2];

    asm volatile(
        "lsl %2, %2, #0x10\n\t"
        "lsl %3, %3, #0x10\n\t"
        "lsr %0, %2, #0x10\n\t"
        "lsr %1, %3, #0x10"
        : "=r" (x), "=r" (y), "+r" (rx), "+r" (ry));
    point[0] = x;
    point[1] = y;
    sub_8023500(gUnknown_030012C0, point);
}

/* Same overall spawn shape as `sub_8021D80`'s family above, but with
 * the master-table record index (`index`), tag (`+0x2d`) and `+0xa`
 * field all taken as *runtime* parameters instead of fixed constants
 * (matches `sub_8025BAC`'s already-documented `param1*12` runtime-
 * indexed access to `gStaticData_084A5600`'s record array, docs/
 * rom_map.md). */
void *sub_80220C4(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch)
{
    struct actor *part = sub_8008434(cx, cy, cw, ch);

    *(void **)((u8 *)part + 0x20) = (u8 *)(**gUnknown_030012D0) + index * 12;
    *((u8 *)part + 0x2d) = (u8)tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = (u8)field0A;
    sub_8008E94(gUnknown_030012EC, part);
    return part;
}

extern void sub_801173C(u16 arg0);

/* Only conditionally calls `sub_801173C(arg0)` (the achievement/
 * unlock-icon family spawner, docs/rom_map.md) when
 * `gUnknown_030012C0+0x8c` is clear - `arg1`/`arg2`/`arg3` are
 * truncated (matching every other 4-arg dispatch-table slot in this
 * chunk) but never read. */
void sub_8022158(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm volatile("" :: "r" (arg1), "r" (arg2), "r" (arg3));
    if (*((u8 *)gUnknown_030012C0 + 0x8c) == 0) {
        sub_801173C(arg0);
    }
}

/* Empty stub. */
void nullsub_22(void)
{
}

extern void *sub_801E990(u32 arg0, u16 arg1, u16 arg2, u16 arg3);
extern struct actor *gUnknown_030012D8;

/* Plain tail-call trampoline to `sub_801E990` - still raw in this same
 * file (top of asm/code_3_2_17_1e990.s, out of this chunk's scope), a
 * position/state-write slot on the hot camera/viewport struct
 * (docs/rom_map.md's "unified ~92-slot table" writeup). */
void sub_802218C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801E990(arg0, arg1, arg2, arg3);
}

/* Writes a Q8.8 `{x, y}` position straight into `gUnknown_030012D8`
 * (the hot camera/viewport struct's own `x`/`y` fields) - ignores
 * `arg0`/`arg3` entirely, matching the ROM (a leaf function, no
 * `push`/`pop` at all). */
void sub_80221A4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gUnknown_030012D8->x = (s32)arg1 << 8;
    gUnknown_030012D8->y = (s32)arg2 << 8;
}

/* Same trampoline as `sub_802218C` above. */
void sub_80221BC(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801E990(arg0, arg1, arg2, arg3);
}

/* Same shape as `sub_80221A4` above. */
void sub_80221D4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gUnknown_030012D8->x = (s32)arg1 << 8;
    gUnknown_030012D8->y = (s32)arg2 << 8;
}

/* Empty stub. */
void nullsub_23(void)
{
}

extern void sub_8025D54(void *self, u32 flags);

/* Constructor/consumer pair (docs/rom_map.md): frees `gUnknown_030012E4`
 * (via `sub_8025D54`'s conditional `sub_8026ED0`, gated bit 0) if
 * already allocated. */
void sub_80221F0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gUnknown_030012E4 != 0) {
        sub_8025D54(gUnknown_030012E4, 3);
    }
}

extern void sub_8025D6C(void *self);
extern void sub_8025D4C(void *self, void *base, s32 count);
extern u8 gStaticData_0816C6A4[];

/* Allocates an 8-byte `{table_base, count}` descriptor
 * (docs/rom_map.md disproves the earlier "local vtable copy"
 * hypothesis - it's a generic pair, nothing table-specific) pointing
 * at the unified 92-slot dispatch array this whole chunk lives
 * inside. Ignores all its own parameters (matches the ROM, a
 * `push {r4, lr}` prologue with no truncation at all). Like
 * `sub_8022230`'s `nullsub_2`/`nullsub_1` calls, `sub_8025D6C` is
 * void and the ROM leaves the freshly-allocated pointer in `r0`
 * across the call rather than saving it - same inline-asm technique. */
void sub_8022208(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    void **addr = &gUnknown_030012E4;
    register void *obj asm("r0") = sub_8026EDC(8);

    asm volatile("bl sub_8025D6C" : "+r" (obj) :: "r1", "r2", "r3", "lr", "cc");
    *addr = obj;
    sub_8025D4C(obj, gStaticData_0816C6A4, 0x5c);
}

extern void *sub_80016DC(u32 size);
extern struct AudioContext *sub_8001C2C(struct AudioContext *self);
extern void sub_8001C80(void);
extern void sub_8001B50(void *arg0, u16 arg1);
extern void sub_8001B30(void *arg0, u16 arg1);
extern void *gUnknown_030012BC;
extern void *gUnknown_030012CC;
extern void *gUnknown_030012C8;
extern struct tile_asset_cache *gUnknown_030012B8;
extern void sub_8006EF0(struct tile_asset_cache *self, u16 count, const u8 *records);
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *InitHudIconWidgetA(struct icon_manager *self);
extern struct icon_manager *InitHudIconWidgetB(struct icon_manager *self);
extern s32 AllocVramDmaQueue(void);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern struct oam_shadow_buffer *sub_8006B0C(struct oam_shadow_buffer *arg0);
extern struct vram_upload_cursor *gUnknown_030012FC;
extern struct vram_upload_cursor *sub_8006CE8(struct vram_upload_cursor *self, s32 count);
extern void *gUnknown_03001304;
extern struct hud_fx_queue *sub_80270C0(struct hud_fx_queue *self);
extern u8 gUnknown_03001288[2];
extern void sub_8001604(void);
extern void sub_8001614(void);
extern u8 gStaticData_084A5600[];

/* `sub_8022230` (docs/rom_map.md, "Found the origin point"): the
 * function `sub_8023738` calls once at the top of the game loop to
 * construct essentially every hot IWRAM global this whole ROM region
 * references - `gUnknown_030012BC` (an 8340-byte `AudioContext`
 * allocation), `030012CC`/`D0`/`B8`/`DC`/`E0`/`03001300`/`FC`/
 * `03001304`/`030012B4`/`C8`, clears `gUnknown_03001288`'s mode byte,
 * and zeroes `self+0xc0` before returning `self` unchanged. `gUnknown_
 * 030012D0` gets pointed at a freshly-allocated 4-byte pointer cell
 * which itself is set to `&gStaticData_084A5600` (the 729 KB master
 * asset index, resolved separately in docs/rom_map.md).
 *
 * Several of these constructions call a *void*-returning helper
 * (`nullsub_2`, `nullsub_1`, `sub_8006FB4`, `sub_80007DC`,
 * `sub_8025A5C`) immediately after allocating the block, then store
 * *that same allocation* without reloading it - relying on the real
 * ROM function leaving the allocated pointer in `r0` untouched (true
 * of each one's real body, which never writes r0 for anything else).
 * A plain C call can't assume that (any call conservatively clobbers
 * r0-r3), so each is spelled with the pointer pinned to r0 across an
 * inline-asm `bl`, the same technique used for `sub_8023674`'s
 * `nullsub_7` call (docs/matching/issue-37-game-loop-234e8.md). */
void *sub_8022230(void *self)
{
    {
        void **addr = (void **)&gUnknown_030012BC;
        register void *audio asm("r0") = sub_80016DC(0x2094);

        asm volatile("bl sub_8001C2C" : "+r" (audio) :: "r1", "r2", "r3", "lr", "cc");
        *addr = audio;
    }
    sub_8001C80();
    sub_8001B50(gUnknown_030012BC, 0xc0);
    sub_8001B30(gUnknown_030012BC, 0xc0);

    {
        void **addr = (void **)&gUnknown_030012CC;
        register void *tmp asm("r0") = sub_8026EDC(4);

        asm volatile("bl nullsub_2" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        void ****addr = &gUnknown_030012D0;
        register void *tmp asm("r0") = sub_8026EDC(4);

        asm volatile("bl nullsub_1" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = (void ***)tmp;
        *(u8 **)tmp = gStaticData_084A5600;
    }
    {
        struct tile_asset_cache **addr = &gUnknown_030012B8;
        register struct tile_asset_cache *cache asm("r0") = sub_8026EDC(0x8c << 2);

        asm volatile("bl sub_8006FB4" : "+r" (cache) :: "r1", "r2", "r3", "lr", "cc");
        *addr = cache;
        {
            register u16 count asm("r1") = *(u16 *)(gStaticData_084A5600 + 0xe);
            register const u8 *records asm("r2") = *(const u8 **)(gStaticData_084A5600 + 8);

            sub_8006EF0(cache, count, records);
        }
    }
    {
        struct icon_manager **addr = &gUnknown_030012DC;
        s32 size = 0x9a << 1;

        *addr = InitHudIconWidgetA(sub_8026EDC(size));
        addr = &gUnknown_030012E0;
        *addr = InitHudIconWidgetB(sub_8026EDC(size));
    }
    AllocVramDmaQueue();
    {
        struct oam_shadow_buffer **addr = &gUnknown_03001300;

        *addr = sub_8006B0C(sub_8026EDC(0x40c));
    }
    {
        struct vram_upload_cursor **addr = &gUnknown_030012FC;

        *addr = sub_8006CE8(sub_8026EDC(0xc), 0);
    }
    {
        void **addr = (void **)&gUnknown_03001304;
        register void *tmp asm("r0") = sub_8026EDC(4);

        asm volatile("bl sub_80007DC" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        void **addr = (void **)&gUnknown_030012B4;
        register void *tmp asm("r0") = sub_8026EDC(0x81 << 3);

        asm volatile("bl sub_8025A5C" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        void **addr = (void **)&gUnknown_030012C8;

        *addr = sub_80270C0(sub_8026EDC(0x48));
    }
    {
        register u8 *addr asm("r0") = gUnknown_03001288;
        register u16 zero asm("r4") = 0;

        *(u16 *)addr = zero;
        sub_8001604();
        sub_8001614();
        {
            register u8 *addr2 asm("r0") = (u8 *)self + 0xc0;

            asm volatile("str %1, [%0]" :: "r" (addr2), "r" (zero) : "memory");
        }
    }
    return self;
}

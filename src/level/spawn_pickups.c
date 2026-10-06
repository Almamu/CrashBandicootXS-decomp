#include "core.h"
#include "actor.h"
#include "sprite_bank.h"
#include "text.h"
#include "pickups.h"
#include "util.h"
#include "audio.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* Sets `part->field_29`'s low nibble to `GetSpriteAnimPaletteSlot(part)`'s result,
 * keeping the high nibble - same idiom as `UPDATE_ICON_FRAME_NIBBLE`
 * (src/menus/pause_menu_pages_init.c, confirmed matching for `InitPauseCrystalsPage`),
 * adapted for a raw-offset `struct actor *` instead of a named
 * `field_29`, since this object's tail past `struct actor`'s 0x1c
 * bytes isn't its own named struct here (see `spawn_gem_platforms.c`'s same
 * caveat). Same macro as src/level/spawn_crates.c - not
 * shared via a header since both files only need it locally. */
#define UPDATE_PART_FRAME_NIBBLE(partPtr) \
    do { \
        register s32 _ret asm("r0") = GetSpriteAnimPaletteSlot(partPtr); \
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

/* Spawns a full visual effect via `CreateSpriteObj`: points its `+0x20`
 * table pointer at `gSpriteBankTable`'s master 12-byte record 38
 * (`table_base + 0x1c8` - the same record `overlay_ui`'s
 * `InitPowerDialog` dialog-box spawner uses, see docs/rom_map.md's
 * "`gSpriteBankTable` record-indexed" writeup), tags it (`+0x2d =
 * 1`), builds it via the standard `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/
 * `SetSpriteAnimDone` OAM trio, sets its `+0x29` bitfield via
 * `GetSpriteAnimPaletteSlot`/`UPDATE_PART_FRAME_NIBBLE`, sets `+0xa` to the fixed
 * `0x25`, then registers it into `gUnknown_030012EC`'s manager via
 * `AddToPartList`. One of four near-identical siblings in this chunk
 * (`SpawnTornadoSpinPower`/`SpawnDoubleJumpPower`/`SpawnTurboRunPower`), differing only in the
 * `+0x2d`/`+0xa` constants. */
void SpawnBodySlamPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 1;
    register u8 field0A asm("r6") = 0x25;
    struct actor *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    AddToPartList(gUnknown_030012EC, part);
}

/* Same shape as `SpawnBodySlamPower` above, tag `0`, `+0xa = 0x24`. */
void SpawnTornadoSpinPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 0;
    register u8 field0A asm("r6") = 0x24;
    struct actor *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    AddToPartList(gUnknown_030012EC, part);
}

/* Same shape as `SpawnBodySlamPower` above, tag `2`, `+0xa = 0x23`. */
void SpawnDoubleJumpPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 2;
    register u8 field0A asm("r6") = 0x23;
    struct actor *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    AddToPartList(gUnknown_030012EC, part);
}

/* Same shape as `SpawnBodySlamPower` above, tag `3`, `+0xa = 0x26`. */
void SpawnTurboRunPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5") = 3;
    register u8 field0A asm("r6") = 0x26;
    struct actor *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x1c8;
    *((u8 *)part + 0x2d) = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    AddToPartList(gUnknown_030012EC, part);
}

/* Gated spawn (see `SpawnBlueGem` below for the sibling shape), but
 * built via `CreateStopwatch` instead of `CreateSpriteObj`, gated by
 * `IsCrystalSaved(gLevelState)` being true instead of a flag-bit
 * test, table offset `table_base + 0x1b0`, tag `0`, `+0xa = 0x1c`, and
 * an extra `flags |= 0x10` on the constructed object before
 * registering it. */
void SpawnStopwatch(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if ((u8)IsCrystalSaved(gLevelState)) {
        register struct actor *part asm("r4") = CreateStopwatch(arg0, arg1, arg2, arg3);

        *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x1b0;
        {
            register u8 tag asm("r0") = 0;
            register u8 *addr asm("r1") = (u8 *)part + 0x2d;
            *addr = tag;
        }
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        UPDATE_PART_FRAME_NIBBLE(part);
        *((u8 *)part + 0xa) = 0x1c;
        {
            register u8 mask asm("r0") = 0x10;
            register u8 old asm("r1") = part->flags;

            mask |= old;
            part->flags = mask;
        }
        AddToPartList(gUnknown_030012EC, part);
    }
}

/* Same spawn shape as `SpawnBodySlamPower`'s family above (record 32 -
 * `table_base + 0x180`, tag `4`, `+0xa = 0x21`), but gated: does
 * nothing at all unless bit 3 of `gLevelState+2` is clear. Does
 * not return the spawned object (the ROM's shared exit pops straight
 * into `r0` from the stack, discarding whatever was last computed
 * there - matches a `void` return exactly). */
void SpawnBlueGem(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    register u8 tag asm("r5");
    register u8 field0A asm("r6");
    struct actor *part;
    register struct level_state *gv asm("r1") = gLevelState;
    register s32 mask asm("r0") = 8;
    register u8 byte asm("r1");

    byte = gv->flags;
    if (mask & byte) {
        return;
    }
    tag = 4;
    field0A = 0x21;
    part = CreateSpriteObj(arg0, arg1, arg2, arg3);
    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + 0x180;
    *((u8 *)part + 0x2d) = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = field0A;
    AddToPartList(gUnknown_030012EC, part);
}

/* New shape (docs/rom_map.md's 15-slot dispatch table, slot 12): a
 * plain state-write, no sound/spawn - never reads `arg0`/`arg3` at all
 * (matches the ROM, which never touches r0/r3), packs `arg1`/`arg2`
 * into a stack `{x, y}` pair and calls `SetCrateGemPos` (already matched
 * in level_state.c), which just stores them into
 * `gLevelState->0x1c0`/`->0x1c4`. The ROM truncates both u16
 * args in one batch (`lsl r1,r1 / lsl r2,r2` then `lsr r3,r1 / lsr
 * r4,r2`) landing the truncated values in different registers (r1->r3,
 * r2->r4) than plain C produces here - gcc instead coalesces the
 * truncated value right back into r1/r2 (the same register it was
 * already in) since nothing else forces it into r3/r4 first. `arg1`/
 * `arg2` are kept `u32` (deferred truncation, same idiom used
 * elsewhere in this file) and the exact two-instruction-pair truncation
 * is spelled out via inline asm instead, forcing the ROM's register
 * choice directly. */
void SpawnCrateGemMarker(u32 arg0, u32 arg1, u32 arg2, u16 arg3)
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
    SetCrateGemPos(gLevelState, point);
}

/* Same overall spawn shape as `SpawnBodySlamPower`'s family above, but with
 * the master-table record index (`index`), tag (`+0x2d`) and `+0xa`
 * field all taken as *runtime* parameters instead of fixed constants
 * (matches `SpawnEffectPart`'s already-documented `param1*12` runtime-
 * indexed access to `gSpriteBankTable`'s record array, docs/
 * rom_map.md). */
void *sub_80220C4(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch)
{
    struct actor *part = CreateSpriteObj(cx, cy, cw, ch);

    *(void **)((u8 *)part + 0x20) = SPRITE_BANK_BASE + index * 12;
    *((u8 *)part + 0x2d) = (u8)tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    UPDATE_PART_FRAME_NIBBLE(part);
    *((u8 *)part + 0xa) = (u8)field0A;
    AddToPartList(gUnknown_030012EC, part);
    return part;
}

/* Spawns a wumpa (`CreateWumpa`, wumpa_update.c) with this slot's four
 * arguments, unless `gLevelState+0x8c` (time trial) is set. */
void SpawnWumpa(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->timeTrial == 0) {
        CreateWumpa(arg0, arg1, arg2, arg3);
    }
}

/* Empty stub. */
void nullsub_22(void)
{
}

extern struct actor *gPlayer;

/* Entity type 0x04: the player start of the kind-2 (hover vehicle) room
 * 16, the only room that places it. Forwards to `SpawnStartMarker`
 * (type 0x00, the start of the kind-0 rooms). */
void SpawnHoverStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Writes a Q8.8 `{x, y}` position straight into `gPlayer`
 * (the hot camera/viewport struct's own `x`/`y` fields) - ignores
 * `arg0`/`arg3` entirely, matching the ROM (a leaf function, no
 * `push`/`pop` at all). */
void sub_80221A4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = (s32)arg1 << 8;
    gPlayer->y = (s32)arg2 << 8;
}

/* Entity type 0x02: the player start of the kind-1 (underwater) rooms;
 * every kind-1 room places one and no other room does. Same trampoline
 * as `SpawnHoverStartMarker` above. */
void SpawnUnderwaterStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Same shape as `sub_80221A4` above. */
void sub_80221D4(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = (s32)arg1 << 8;
    gPlayer->y = (s32)arg2 << 8;
}

/* Empty stub. */
void nullsub_23(void)
{
}

/* Constructor/consumer pair (docs/rom_map.md): frees `gEntitySpawner`
 * (via `sub_8025D54`'s conditional `OperatorDelete`, gated bit 0) if
 * already allocated. */
void DestroyEntitySpawner(void)
{
    if (gEntitySpawner != 0) {
        sub_8025D54(gEntitySpawner, 3);
    }
}

/* Allocates an 8-byte `{table_base, count}` descriptor
 * (docs/rom_map.md disproves the earlier "local vtable copy"
 * hypothesis - it's a generic pair, nothing table-specific) pointing
 * at the unified 92-slot dispatch array this whole chunk lives
 * inside. Ignores all its own parameters (matches the ROM, a
 * `push {r4, lr}` prologue with no truncation at all). Like
 * `InitLevelState`'s `nullsub_2`/`nullsub_1` calls, `InitEntitySpawner` is
 * void and the ROM leaves the freshly-allocated pointer in `r0`
 * across the call rather than saving it - same inline-asm technique. */
void CreateEntitySpawner(void)
{
    void **addr = &gEntitySpawner;
    register void *obj asm("r0") = OperatorNew(8);

    asm volatile("bl InitEntitySpawner" : "+r" (obj) :: "r1", "r2", "r3", "lr", "cc");
    *addr = obj;
    SetEntitySpawnerTable(obj, gEntitySpawnFuncs, 0x5c);
}

/* `InitLevelState` (docs/rom_map.md, "Found the origin point"): the
 * function `GetLevelState` calls once at the top of the game loop to
 * construct essentially every hot IWRAM global this whole ROM region
 * references - `gAudioContext` (an 8340-byte `AudioContext`
 * allocation), `030012CC`/`D0`/`B8`/`DC`/`E0`/`03001300`/`FC`/
 * `03001304`/`030012B4`/`C8`, clears `gDispcnt`'s mode byte,
 * and zeroes `self+0xc0` before returning `self` unchanged. `gUnknown_
 * 030012D0` gets pointed at a freshly-allocated 4-byte pointer cell
 * which itself is set to `&gSpriteBankTable` (the 729 KB master
 * asset index, resolved separately in docs/rom_map.md).
 *
 * Several of these constructions call a *void*-returning helper
 * (`nullsub_2`, `nullsub_1`, `InitPaletteCache`, `ClearKeys`,
 * `InitEntityFlags`) immediately after allocating the block, then store
 * *that same allocation* without reloading it - relying on the real
 * ROM function leaving the allocated pointer in `r0` untouched (true
 * of each one's real body, which never writes r0 for anything else).
 * A plain C call can't assume that (any call conservatively clobbers
 * r0-r3), so each is spelled with the pointer pinned to r0 across an
 * inline-asm `bl`, the same technique used for `ShowCompanyLogos`'s
 * `nullsub_7` call (docs/matching/issue-37-game-loop-234e8.md). */
void *InitLevelState(void *self)
{
    {
        void **addr = (void **)&gAudioContext;
        register void *audio asm("r0") = IwramAlloc(0x2094);

        asm volatile("bl InitAudioContext" : "+r" (audio) :: "r1", "r2", "r3", "lr", "cc");
        *addr = audio;
    }
    EnableMusicVCountIrq();
    SetSfxVolume(gAudioContext, 0xc0);
    SetMusicVolume(gAudioContext, 0xc0);

    {
        void **addr = (void **)&gSpriteRenderer;
        register void *tmp asm("r0") = OperatorNew(4);

        asm volatile("bl nullsub_2" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        struct sprite_bank_set **addr = &gSpriteBankSet;
        register void *tmp asm("r0") = OperatorNew(4);

        asm volatile("bl nullsub_1" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
        *(const void **)tmp = &gSpriteBankTable;
    }
    {
        struct palette_cache **addr = &gPaletteCache;
        register struct palette_cache *cache asm("r0") = OperatorNew(0x8c << 2);

        asm volatile("bl InitPaletteCache" : "+r" (cache) :: "r1", "r2", "r3", "lr", "cc");
        *addr = cache;
        {
            register u16 count asm("r1") =
                gSpriteBankTable.paletteCount;
            register const u8 *records asm("r2") =
                gSpriteBankTable.palettes;

            SetPaletteCacheSource(cache, count, records);
        }
    }
    {
        struct bitmap_font **addr = &gSmallFont;
        s32 size = 0x9a << 1;

        *addr = InitSmallFont(OperatorNew(size));
        addr = &gLargeFont;
        *addr = InitLargeFont(OperatorNew(size));
    }
    AllocVramDmaQueue();
    {
        struct oam_shadow_buffer **addr = &gOamBuffer;

        *addr = InitOamBuffer(OperatorNew(0x40c));
    }
    {
        struct vram_upload_cursor **addr = &gObjVramCursor;

        *addr = InitObjVramCursor(OperatorNew(0xc), 0);
    }
    {
        void **addr = (void **)&gInput;
        register void *tmp asm("r0") = OperatorNew(4);

        asm volatile("bl ClearKeys" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        struct entity_flags **addr = &gEntityFlags;
        register void *tmp asm("r0") = OperatorNew(0x81 << 3);

        asm volatile("bl InitEntityFlags" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        *addr = tmp;
    }
    {
        void **addr = (void **)&gPaletteCycles;

        *addr = InitPaletteCycles(OperatorNew(0x48));
    }
    {
        register u8 *addr asm("r0") = gDispcnt;
        register u16 zero asm("r4") = 0;

        *(u16 *)addr = zero;
        SetObjMapping1D();
        CommitDispcnt();
        {
            register u8 *addr2 asm("r0") = (u8 *)self + 0xc0;

            asm volatile("str %1, [%0]" :: "r" (addr2), "r" (zero) : "memory");
        }
    }
    return self;
}

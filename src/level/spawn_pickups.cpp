#include "spawners.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "part_list.hpp"
#include "font.hpp"
#include "level_state.hpp"

extern "C" {
#include "audio.h"
#include "util.h"
#include "text.h"
#include "gfx.h"
#include "memory.h"
#include "system.h"
#include "match.h"
#include "level.h"
#include "globals.h"
}

/* The pickup spawners, the player starts, the entity spawner's
 * constructor and destructor calls and InitLevelState (#664,
 * include/spawners.hpp). */

/* `t` is an s32: the constant is loaded before the tag's address. */
static inline void SetTag(Sprite *part, s32 t)
{
    part->tag = t;
}

/* A sprite of bank `bank` (an offset into the sprite bank table),
 * animation `tag`, kind `kind`, in the touchable list. The callers pass
 * `tag` and `kind` in variables: the ROM loads both into callee-saved
 * registers before the Sprite::Create call and stores them from there
 * (a literal 0 is loaded again at the store). */
static inline Sprite *SpawnTouchable(u32 a0, u16 a1, u16 a2, u16 a3, s32 bank, u8 tag, u8 kind)
{
    Sprite *part = Sprite::Create(a0, a1, a2, a3);

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + bank);
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->kind = kind;
    TouchableList()->Add(part);
    return part;
}

/* The four powers (sprite bank record 38, `+0x1C8`, the one
 * InitPowerDialog's dialog box uses): body slam (tag 1, kind 0x25),
 * tornado spin (0, 0x24), double jump (2, 0x23) and turbo run (3,
 * 0x26). */
void SpawnBodySlamPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 1;
    u8 kind = 0x25;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnTornadoSpinPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 0;
    u8 kind = 0x24;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnDoubleJumpPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 2;
    u8 kind = 0x23;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnTurboRunPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 3;
    u8 kind = 0x26;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

/* The time trial's stopwatch (bank `+0x1B0`, kind 0x1C, always active),
 * once the level's crystal is saved. */
void SpawnStopwatch(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if ((u8)IsCrystalSaved(gLevelState)) {
        Stopwatch *part = Stopwatch::Create(arg0, arg1, arg2, arg3);

        part->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x1b0);
        SetTag(part, 0);
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = 0x1c;
        part->f.flags |= 0x10;
        TouchableList()->Add(part);
    }
}

/* The blue gem (bank record 32, `+0x180`, tag 4, kind 0x21), unless
 * collected (bit 3 of the level state's gem flags). */
void SpawnBlueGem(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->progress.flags & 8)
        return;
    u8 tag = 4;
    u8 kind = 0x21;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x180, tag, kind);
}

/* Entity type 0x0C (the crate gem's marker): where the crate gem appears
 * (SetCrateGemPos) once every crate is broken.
 *
 * Kept from the C: the truncation of `arg1`/`arg2` in inline asm. The
 * ROM truncates both in one batch (`lsl r1; lsl r2; lsr r3, r1; lsr r4,
 * r2`) into fresh registers, one of them callee-saved; g++ truncates them
 * in place, with every spelling tried (u16 parameters, locals, a point
 * class with a constructor), the same gap as SpawnRoomExit
 * (spawn_bosses.cpp). */
void SpawnCrateGemMarker(u32 arg0, u32 arg1, u32 arg2, u16 arg3)
{
    MATCH_HOLD_REG(u32, rx, r1) = arg1;
    MATCH_HOLD_REG(u32, ry, r2) = arg2;
    MATCH_HOLD_REG(s32, x, r3);
    MATCH_HOLD_REG(s32, y, r4);
    s32 point[2];

    // clang-format off
    asm volatile(
        "lsl %2, %2, #0x10\n\t"
        "lsl %3, %3, #0x10\n\t"
        "lsr %0, %2, #0x10\n\t"
        "lsr %1, %3, #0x10"
        : "=r" (x), "=r" (y), "+r" (rx), "+r" (ry));
    // clang-format on
    point[0] = x;
    point[1] = y;
    SetCrateGemPos(gLevelState, point);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). Builds a CreateSpriteObj part and adds it to gTouchableList,
 * like the pickup spawners above.
 *
 * Same overall spawn shape as `SpawnBodySlamPower`'s family above, but with
 * the master-table record index (`index`), `tag` and `kind`
 * field all taken as *runtime* parameters instead of fixed constants
 * (matches `SpawnEffectPart`'s already-documented `param1*12` runtime-
 * indexed access to `gSpriteBankTable`'s record array, docs/
 * rom_map.md). */
void *CreateTouchableSprite(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch)
{
    Sprite *part = Sprite::Create(cx, cy, cw, ch);

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + index * 12);
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->kind = field0A;
    TouchableList()->Add(part);
    return part;
}

/* Spawns a wumpa (`CreateWumpa`, wumpa_update.cpp) with this slot's four
 * arguments, unless `gLevelState+0x8c` (time trial) is set. */
void SpawnWumpa(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->timeTrial == 0)
        Wumpa::Create(arg0, arg1, arg2, arg3);
}

/* Entity type 0x05: does nothing. Types 0x01/0x03 (SpawnPlayerPosition,
 * SpawnUnderwaterPlayerPosition) only move the player to the entity, and
 * the three pair up with the start markers of the room kinds by slot (0x00
 * normal, 0x02 underwater, 0x04 hover; the ROM holds these five functions
 * in descending type order); for the hover room this one is empty. No
 * level places types 0x01, 0x03 or 0x05 (docs/levels.md). */
void SpawnHoverPlayerPosition(void)
{
}

/* Entity type 0x04: the player start of the kind-2 (hover vehicle) room
 * 16, the only room that places it. Forwards to `SpawnStartMarker`
 * (type 0x00, the start of the kind-0 rooms). */
void SpawnHoverStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Entity type 0x03: moves the player to the entity's position (Q8),
 * without a start marker; the underwater counterpart of type 0x01 (see
 * SpawnHoverPlayerPosition). Ignores `arg0`/`arg3` entirely, matching the
 * ROM (a leaf function, no `push`/`pop` at all). */
void SpawnUnderwaterPlayerPosition(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = INT_TO_Q8((s32)arg1);
    gPlayer->y = INT_TO_Q8((s32)arg2);
}

/* Entity type 0x02: the player start of the kind-1 (underwater) rooms;
 * every kind-1 room places one and no other room does. Same trampoline
 * as `SpawnHoverStartMarker` above. */
void SpawnUnderwaterStartMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnStartMarker(arg0, arg1, arg2, arg3);
}

/* Entity type 0x01: the same as `SpawnUnderwaterPlayerPosition` above, for
 * the normal rooms (see SpawnHoverPlayerPosition). */
void SpawnPlayerPosition(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    gPlayer->x = INT_TO_Q8((s32)arg1);
    gPlayer->y = INT_TO_Q8((s32)arg2);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). The spawners in this stretch of ROM run in descending type order
 * (SpawnWumpa 0x06, SpawnHoverPlayerPosition 0x05 ... SpawnPlayerPosition
 * 0x01), so this empty stub sits where type 0x00's spawner would; the
 * table's type 0x00 is SpawnStartMarker (spawn_start_marker.cpp, 0x0801E990)
 * instead. */
void SpawnStartMarkerStub(void)
{
}

/* `delete gEntitySpawner`: the null test, then the destructor
 * (DestroyEntitySpawnerObj) with 3. */
void DestroyEntitySpawner(void)
{
    delete gEntitySpawner;
}

/* gEntitySpawner, over the 92 spawn functions of gEntitySpawnFuncs. */
void CreateEntitySpawner(void)
{
    gEntitySpawner = new EntitySpawner;
    gEntitySpawner->SetTable(gEntitySpawnFuncs, ENTITY_COUNT);
}

/* The level state's constructor (GetLevelState calls it once, at the top
 * of the game loop): builds the hot IWRAM globals - the audio context
 * (0x2094 bytes of IWRAM), the sprite renderer, the sprite bank set
 * (pointed at gSpriteBankTable), the palette cache (over the table's
 * palettes), the two fonts, the VRAM DMA queue, the OAM buffer, the OBJ
 * VRAM cursor, the key input, the entity flags and the palette cycles -
 * clears the display control and the level state's unused flags.
 *
 * It is LevelState's constructor (include/level_state.hpp). The globals
 * keep their C types (the C files use them), so the new objects of the
 * C++ classes are stored through their C views (the fonts are `Font *`s
 * to C++, text.h). The audio context is still C, built by its C
 * constructor. */
LevelState::LevelState()
{
    {
        struct AudioContext **audio = &gAudioContext;

        *audio = InitAudioContext((struct AudioContext *)IwramAlloc(0x2094));
        EnableMusicVCountIrq();
        SetSfxVolume(*audio, 0xc0);
        SetMusicVolume(*audio, 0xc0);
    }
    gSpriteRenderer = new SpriteRenderer;
    {
        SpriteBankSet *banks;

        gSpriteBankSet = (banks = new SpriteBankSet);
        banks->table = &gSpriteBankTable;
    }
    {
        PaletteCache *cache;

        gPaletteCache = (cache = new PaletteCache);
        cache->SetSource(gSpriteBankTable.paletteCount, gSpriteBankTable.palettes);
    }
    {
        Font **font = &gSmallFont;

        *font = new SmallFont;
        font = &gLargeFont;
        *font = new LargeFont;
    }
    AllocVramDmaQueue();
    gOamBuffer = new OamBuffer;
    gObjVramCursor = new ObjVramCursor(0);
    gInput = new KeyInput;
    gEntityFlags = new LevelEntityFlags;
    gPaletteCycles = new PaletteCycles;
    {
        u16 *dispcnt = (u16 *)gDispcnt;
        u16 zero = 0;

        *dispcnt = zero;
        SetObjMapping1D();
        CommitDispcnt();
        unusedFlags = zero;
    }
}

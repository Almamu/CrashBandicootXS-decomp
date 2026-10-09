#ifndef GUARD_SPRITE_OBJ_HPP
#define GUARD_SPRITE_OBJ_HPP

/* The entity and sprite object classes (#664, docs/cplusplus.md): the
 * classes behind gEntityVtable, gSpriteObjVtable, gMovingSpriteVtable and
 * gGroundSpriteVtable, and the subclasses built on them.
 *
 *   Entity        0x1C  gEntityVtable        (11 slots) src/objects/entity.cpp
 *   Sprite        0x40  gSpriteObjVtable     (13 slots) src/objects/sprite*.cpp
 *   UiSprite      0x40  gUiSpriteObjVtable              src/objects/sprite_anim.cpp
 *   MovingSprite  0x78  gMovingSpriteVtable  (15 slots) src/objects/moving_sprite*.cpp
 *   GroundSprite  0x80  gGroundSpriteVtable  (15 slots) src/objects/ground_sprite*.cpp
 *
 * The sizes are the ROM's: CreateEntity allocates 0x1C bytes,
 * CreateSpriteObj 0x40, CreateMovingSprite 0x78 and CreateGroundSprite
 * 0x80. The C names of Sprite's methods say "SpriteObj"
 * (InitSpriteObj, DestroySpriteObj, ...); cxx_symbols.txt maps them.
 *
 * A controller (ctrl.hpp and the rest) drives a MovingSprite: its `mover`
 * calls the controller's methods with itself (`P12MovingSprite` in their
 * mangled names, cxx_symbols.txt).
 *
 * The fields keep their offsets in comments. No C file reads these
 * objects, and they have no C view: level.h's `struct camera_target` (the
 * camera's followed Sprite) and player.h's `struct player` went in #754.
 *
 * No `#pragma interface`: g++ emits the vtables of Sprite (sprite.cpp),
 * UiSprite (sprite_anim.cpp), MovingSprite (moving_sprite_collide.cpp) and
 * GroundSprite (ground_sprite_collide.cpp), their key-method objects (see
 * ctrl.hpp). */

#include "entity.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "aabb.h"
#include "hitbox_quad.h"
#include "sprite_bank.h"
#include "box_part.h"
#include "gfx_part.h"
#include "objects.h"
#include "gobj_1a794.h"
#include "gfx.h"
#include "globals.h"
#include "level.h"
}

class Ctrl;

/* The sprite object (src/objects/sprite_obj.cpp, sprite.cpp,
 * sprite_anim.cpp; gSpriteObjVtable): an entity with a sprite bank
 * animation. `bank` is the sprite bank (sprite_bank.h), `tag` the
 * animation in it, `frame` the step within the animation. */
class Sprite : public Entity
{
public:
    void *lastHitbox; // 0x1C - the last hitbox record (AnchorGroundSpriteHitbox)
    /* 0x20 - the sprite bank. A union of one: gcc gives every access to a
     * union member alias set 0, and the ROM's code was compiled with that
     * (a store to it doesn't kill other loads; see hud.hpp's
     * SET_PART_BANK). */
    union {
        const struct sprite_bank *bank;
    };
    u8 dir; // 0x24 - motion direction bits: 1 right, 2 left, 4 up, 8 down
    // 0x25 - 1: x/y are screen coordinates (DrawAt skips WorldToScreen;
    //        always counts as on screen)
    u8 screenSpace;
    u8 unk_26[2];
    /* 0x28: the OBJ mode, mosaic, 8bpp, mirror and priority bits of the
     * sprite's OAM entries, as a byte or as bitfields. */
    union {
        u8 mirror; // bit 4: X mirrored, bit 5: Y mirrored
        struct MirrorBits {
            u8 gfxMode:2; // OBJ mode (Get/SetSpriteGfxMode); 1: semi-transparent
            u8 mosaic:1;
            u8 colorMode:1; // 8bpp
            s32 flipX:1;    // signed: test them with `< 0` (`lsl #27`, a sign test)
            s32 flipY:1;
            u8 priority:2; // OBJ priority (Get/SetSpritePriority)
        } __attribute__((packed)) mirrorBits;
        /* The same bits unsigned: read as
         * values (`lsl #27; lsr #31`), not tested. */
        struct MirrorFlags {
            u32 gfxMode:2;
            u32 mosaic:1;
            u32 colorMode:1;
            u32 mirrorX:1;
            u32 mirrorY:1;
            u32 priority:2;
        } __attribute__((packed)) mirrorFlags;
    } __attribute__((packed)); // one byte, not the 4 of an ARM union
    u8 palette:4;              // 0x29 - low nibble: the OBJ palette slot
    u32:20;                    // 0x29 bit 4-0x2B: the unused rest of the 0x28 word
    u8 animating;              // 0x2C - nonzero while the animation timer runs
    u8 tag;                    // 0x2D - the animation (SetTargetAnim)
    s32 frame;                 // 0x30 - the step within the animation
    s32 stepTimer;             // 0x34 - ticks spent on the current step
    u8 animDone;               // 0x38 - set once a non-looping animation ends
    u8 unk_39[3];
    u16 affine; // 0x3C - nonzero: DrawWithOffset draws the affine pieces

    Sprite();                                      // InitSpriteObj
    virtual s32 CheckPlayerContact();              // 1 CheckSpritePickup
    virtual const struct hitbox_quad *GetBounds(); // 2 GetSpriteObjHitbox
    virtual void Update();                         // 3 UpdateSpriteObj
    virtual void Draw();                           // 4 DrawSpriteObj
    virtual u8 IsOnScreen();                       // 5 IsSpriteObjOnScreen
    virtual s32 OverlapsRect(struct aabb *region); // 6 SpriteObjOverlapsRect
    virtual u8 IsNearCamera();                     // 7 IsSpriteObjNearCamera
    virtual s32 IsInsideRect(struct aabb *box);    // 8 IsSpriteObjInsideRect
    virtual s32 GetClassId();                      // 9 GetSpriteObjClassId
    virtual ~Sprite();                             // 10 DestroySpriteObj
    virtual s32 GetPriority();                     // 11 GetSpriteObjPriority
    virtual s32 ApplyVelocity();                   // 12 ApplySpriteObjVelocity

    /* CreateSpriteObj's `new Sprite(id, x, y)`: the constructor inlined,
     * then the spawn's id and position. */
    Sprite(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8((s32)px);
        y = INT_TO_Q8((s32)py);
    }
    static Sprite *Create(u16 id, u16 x, u16 y, u16 unused); // CreateSpriteObj

    void Reset();                            // ResetSpriteObj
    struct aabb GetAnimBounds();             // GetSpriteBounds: the animation's box[1]
    struct aabb GetAnimHitbox();             // GetSpriteHitbox: the animation's box[0]
    struct aabb GetAttackBox();              // GetSpriteAttackBox: the frame's
    struct aabb GetBodyBox();                // GetSpriteBodyBox: the frame's
    void AdvanceAnim();                      // AdvanceSpriteAnim
    s32 HitboxOverlaps(struct aabb *region); // SpriteHitboxOverlaps
    s32 GetAnimPaletteSlot();
    s32 GetTileBase();
    const struct sprite_frame *GetFrame(); // GetSpriteFrame
    const struct sprite_point *GetFrameAnchor();
    const struct hitbox_quad *GetFrameThirdBox();
    const struct hitbox_quad *GetFrameAttackBox();
    const struct hitbox_quad *GetFrameBodyBox();
    const struct sprite_anim *GetAnim(); // GetSpriteAnimRecord
    void SetFrameIndex(s32 step);        // clamped to the animation's last step
    u8 GetScreenSpace();
    void SetScreenSpace(u8 value);
    s32 IsHidden(); // `blink`
    void ToggleHidden();
    s32 IsSolid(); // IsPartSolid, ...
    void ClearSolid();
    void SetSolid();
    s32 IsVulnerable(); // IsSpriteObjVulnerable, ...
    void ClearVulnerable();
    void SetVulnerable();
    void ResetAnimIndex(); // `tag`
    s32 IsCollisionEnabled();
    void DisableCollision();
    void EnableCollision();
    u8 GetAnimating();
    void SetAnimating(u8 value);
    void SetFlipX(u8 value);
    void SetFlipY(u8 value);
    void SetAnimDone(u8 value);
    u8 GetAnimPaletteId();
    s32 GetPalette();
    void SetPalette(s32 value);
    void SetBank(void *bank); // SetSpriteAnimTable
    void *GetBank();          // GetSpriteAnimTable
    u8 IsAnimLooping();
    u8 GetAnimFrameCount();
    u8 GetAnimDuration();
    void ResetFrameIndex();
    void SetFrameTimer(s32 value);
    void ResetFrameTimer();
    void SetAnimIndex(u8 value);
    void SetAnim(u8 index); // SetSpriteAnim: the animation, from its start
    void IncFrameIndex();
    void IncFrameTimer();
    void SetMoveAxes(u8 value); // `dir`
    u8 GetMoveAxes();
    s32 GetFrameIndex();
    s32 GetFrameTimer();
    u8 GetAnimIndex(); // GetSpriteAnim
    s32 GetGfxMode();
    void SetGfxMode(s32 value);
    s32 GetFlipX();
    s32 GetFlipY();
    u8 GetAnimDone();
    s32 GetMosaic();
    s32 GetOamPalette();
    s32 GetColorMode();
    u16 GetAffine();
    void SetAffine(u16 value);
    void DrawWithOffset(s32 dx, s32 dy);
    void SetPriority(s32 value);

    /* The position as a vector: a copy of it is a block copy (both
     * loads, then both stores; the camera's goal, MovingSprite's). */
    struct vec2 &Pos()
    {
        return *(struct vec2 *)&x;
    }
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(Sprite) == 0x40);

/* A sprite on the HUD or a menu (gUiSpriteObjVtable;
 * src/objects/sprite_anim.cpp): its OBJ priority is its own. */
class UiSprite : public Sprite
{
public:
    UiSprite();                // InitUiSpriteObj
    virtual ~UiSprite();       // DestroyUiSpriteObj
    virtual s32 GetPriority(); // GetSpritePriority: the mirror byte's
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(UiSprite) == 0x40);

/* The moving sprite (src/objects/moving_sprite.cpp, moving_sprite_collide.cpp,
 * moving_sprite_contact.cpp, moving_sprite_probe.cpp; gMovingSpriteVtable): a sprite with a
 * controller (`mover`), per-axis speeds and their ramps, and the terrain
 * probe's state. Its update runs the controller's, its events go to the
 * controller, and its slot 14 is the contact with the player. */
class MovingSprite : public Sprite
{
public:
    s32 unk_40;              // 0x40 - ResetMovingSprite clears it; nothing reads it
    Ctrl *mover;             // 0x44 - its controller
    struct speed_ramp rampX; // 0x48 - speedX's ramp
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64
    u8 hitAxes;              // 0x68 - the collision axes the terrain probe resolved
    u8 probeTries;           // 0x69
    s32 prevX;               // 0x6C - the previous position (Q8), cached by ApplyVelocity
    s32 prevY;               // 0x70
    s32 hitMask;             // 0x74 - the probe axes hit this frame

    MovingSprite();                                         // InitMovingSprite
    virtual s32 CheckPlayerContact();                       // 1 CollideMovingSprite
    virtual void Update();                                  // 3 UpdateMovingSprite
    virtual s32 GetClassId();                               // 9 GetMovingSpriteClassId
    virtual ~MovingSprite();                                // 10 DestroyMovingSprite
    virtual s32 ApplyVelocity();                            // 12 ApplySpriteVelocity
    virtual void HandleEvent(s32 from, s32 event, s32 arg); // 13 HitMovingSprite
    virtual void TouchPlayer();                             // 14 CheckPlayerContact

    /* CreateMovingSprite's `new MovingSprite(id, x, y)`: the constructor
     * inlined (Sprite's out of line), then the spawn's id and position. */
    MovingSprite(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8((s32)px);
        y = INT_TO_Q8((s32)py);
    }
    static MovingSprite *Create(u16 id, u16 x, u16 y, u16 unused); // CreateMovingSprite

    /* The previous position as a vector, as Sprite's Pos(): a copy of
     * it is a block copy (both loads, then both stores). */
    struct vec2 &PrevPos()
    {
        return *(struct vec2 *)&prevX;
    }

    void Reset(); // ResetMovingSprite
    void SetPrevPos(s32 px, s32 py);
    struct vec2 GetPrevPos();
    s32 GetPrevY(); // in pixels
    s32 GetPrevX();
    s32 ClassifyContact(struct aabb *region); // ClassifySpriteContact
    s32 GetHitMask();                         // GetGroundSpriteHitMask, ...
    s32 HasHitMask();
    void ClearHitMask();
    void AddHitMask(s32 mask);
    void SetHitAxes(u8 value); // SetGroundSpriteHitAxes, ...
    u8 GetHitAxes();
    void SetSpeedY(s32 value); // SetSpriteSpeedY, ...
    void SetSpeedX(s32 value);
    s32 GetSpeedX();
    s32 GetSpeedY();
    Ctrl *GetCtrl();             // GetSpriteCtrl
    void AttachCtrl(Ctrl *ctrl); // AttachSpriteCtrl: `mover`, then the controller's Attach
    /* StartSpriteMotionY, ...: the speed and its ramp (Start), or the
     * ramp alone (Set). */
    void StartMotionY(s32 speed, s32 step, s32 target);
    void SetMotionY(s32 start, s32 step, s32 target);
    void StartMotionX(s32 speed, s32 step, s32 target);
    void SetMotionX(s32 start, s32 step, s32 target);
    u8 GetProbeTries(); // GetGroundSpriteProbeTries
    void ResolvePlayerContact();
    /* ProbeHitboxEdgeTerrain: ProbeTerrain along the edge of `quad` that
     * faces `mode`, retried lower (src/objects/moving_sprite_probe.cpp). */
    s32 ProbeEdgeTerrain(s32 mode, const struct hitbox_quad *quad);
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(MovingSprite) == 0x78);

/* The ground sprite (src/objects/ground_sprite.cpp, ground_sprite_collide.cpp,
 * ground_sprite_update.cpp; gGroundSpriteVtable): a moving sprite that
 * probes the terrain under it (slot 1) and keeps its hitbox anchored to
 * the floor or the ceiling (slot 3). */
class GroundSprite : public MovingSprite
{
public:
    s32 type; // 0x78 - the platform type (CreatePlatform)
    u8 unk_7C[4];

    GroundSprite();                   // InitGroundSprite
    virtual s32 CheckPlayerContact(); // 1 CollideGroundSprite
    virtual void Update();            // 3 UpdateGroundSprite
    virtual void Draw();              // 4 DrawGroundSprite
    virtual s32 GetClassId();         // 9 GetGroundSpriteClassId
    virtual ~GroundSprite();          // 10 DestroyGroundSprite

    /* CreateGroundSprite's `new GroundSprite(id, x, y)`: the constructor
     * inlined (MovingSprite's out of line), then the id and position. */
    GroundSprite(u16 id, u16 px, u16 py)
    {
        Reset();
        this->id = id;
        x = INT_TO_Q8((s32)px);
        y = INT_TO_Q8((s32)py);
    }
    static GroundSprite *Create(u16 id, u16 x, u16 y, u16 unused); // CreateGroundSprite

    void Reset();    // ResetGroundSprite
    u8 IsGrounded(); // IsGroundSpriteGrounded, ...: `grounded`
    void ClearGrounded();
    void SetGrounded();
    u8 IsFloorProbeEnabled(); // IsGroundSpriteFloorProbeEnabled, ...: `floorProbe`
    void DisableFloorProbe();
    void EnableFloorProbe();
    void ClearFlag5(); // ClearSpriteObjFlag5, ...
    void SetFlag5();
    u8 GetFlag5();
    Ctrl *GetMover(); // GetMovingSpriteCtrl: GetCtrl's twin
    /* ProbeGroundSpriteTerrain: the floor, then the terrain along each
     * axis it moves on; returns the axes it hit. */
    s32 ProbeTerrainAxes();
    u8 ProbeFloor(const struct hitbox_quad *quad, u8 *outFlag); // ProbeGroundSpriteFloor
    void AnchorHitbox();                                        // AnchorGroundSpriteHitbox
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(GroundSprite) == 0x80);

/* A list of sprite objects (the room's part lists, globals.h):
 * Update compacts `items` and fills `visible`, the parts on screen, which
 * Collide walks. Its methods are C++: Update, Collide and CollideWithPlayer
 * in src/objects/sprite_anim.cpp, the rest in part_list.cpp,
 * part_list_cull.cpp and part_collide.cpp (part 7c). The items are
 * Sprites: gTouchableList holds pickups, platforms and Tiny's hop pads,
 * gCollidableList and gForegroundList moving sprites and effect parts.
 * Collide hands on only the parts whose class id is above 4, the moving
 * sprites (MovingSprite 5, GroundSprite 6), which are what CollideWithPlayer
 * and CollideWithObject take. */
class PartList
{
public:
    s32 capacity;     // 0x00
    s32 count;        // 0x04
    s32 visibleCount; // 0x08
    Sprite **items;   // 0x0C
    Sprite **visible; // 0x10

    void Update();                                                  // UpdatePartList
    void Collide(struct aabb box, s32 unused, MovingSprite *other); // CollidePartList
    void CollideWithPlayer(struct aabb box, MovingSprite *part);    // CollidePartWithPlayer
    void CollideWithObject(struct aabb box, MovingSprite *part,
                           MovingSprite *other); // CollidePartWithObject
    /* part 7c (include/part_list.hpp; src/objects/part_list.cpp,
     * part_list_cull.cpp) */
    PartList(s32 capacity);         // InitPartList
    ~PartList();                    // DestroyPartList
    void Draw();                    // DrawPartList
    void Remove(Sprite *part);      // RemoveFromPartList
    void RemoveAt(s32 index);       // RemovePartListAt
    void Add(Sprite *part);         // AddToPartList
    void Cull();                    // CullPartList
    void Clear();                   // ClearPartList
    void CollideClass(s32 classId); // CollidePartsOfClass
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(PartList) == 0x14);

/* The room's part lists (globals.h), as Crates() is the crate list
 * (crate_list.hpp). */
static inline PartList *TouchableList()
{
    return gTouchableList;
}

static inline PartList *CollidableList()
{
    return gCollidableList;
}

static inline PartList *ForegroundList()
{
    return gForegroundList;
}

/* The sprite graphics managers (src/gfx/oam_buffer.cpp, obj_vram_cursor.cpp,
 * palette_cache.cpp, sprite_bank_set.cpp). */

/* The OAM shadow buffer (gOamBuffer): a shadow copy of the 128-entry hardware OAM
 * table. `count` entries of it are in use, `base` of them kept from frame
 * to frame (MarkBase, Rewind), and `matrixCount` affine matrices handed
 * out this frame. Matrix `m`'s pa/pb/pc/pd are the affine parameters of
 * entries 4m..4m+3 (`table[4 * m + n].attr[3]`). */
class OamBuffer
{
public:
    s32 count;                          // 0x00
    s32 base;                           // 0x04
    s32 matrixCount;                    // 0x08
    union oam_shadow_entry table[0x80]; // 0x0C

    OamBuffer();                              // InitOamBuffer
    ~OamBuffer();                             // DestroyOamBuffer
    void SetAffineScales(u16 *scales, s32 n); // SetOamAffineScales
    void Append(void *entries, s32 n);        // AppendOamEntries
    void HideUnused();                        // HideUnusedOamEntries
    void Rewind();
    void MarkBase();
    void Reset();
    void Commit();
    void Add(const void *entry); // AddOamEntry
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(OamBuffer) == 0x40C);

/* The OBJ VRAM upload cursor (gObjVramCursor): a bump allocator over OBJ tile VRAM
 * (OBJ_VRAM0, OBJ_VRAM0_SIZE bytes) for the tile data uploaded through
 * the VRAM DMA queue. `offset` is the next free byte, bumped by Reserve
 * and Upload; `mark` is the checkpoint Mark saves and Rewind restores.
 * `baseTile` is the number of tiles kept below the allocator; Reset
 * starts both cursors there. */
class ObjVramCursor
{
public:
    u32 mark;     // 0x00
    u32 offset;   // 0x04
    s32 baseTile; // 0x08

    ObjVramCursor(s32 baseTile); // InitObjVramCursor
    ~ObjVramCursor();            // DestroyObjVramCursor
    void Rewind();
    void Mark();
    s32 GetFreeBytes();
    s32 GetTile();
    void Reset();
    s32 Reserve(s32 size);
    s32 Upload(void *src, s32 size);
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(ObjVramCursor) == 0xC);

/* The OBJ palette cache (gPaletteCache): maps a ROM table of `count` 16-colour OBJ palettes
 * (`palettes`, 32 bytes each: the sprite bank table's 125 palettes) onto
 * the 16 OBJ palette banks. `slotOf[id]` is the bank palette `id` is
 * loaded in, or 0xFF; `slots` holds each bank's colours, uploaded to
 * OBJ_PLTT by UploadSlot/Upload. `isFree[bank]` marks a bank available to
 * GetSlot; `locked[bank]` keeps a bank from being reclaimed by
 * FreeUnlockedSlots (Lock/Unlock). */
class PaletteCache
{
public:
    u16 count;                    // 0x00
    const u8 *palettes;           // 0x04
    u8 *slotOf;                   // 0x08 - the bank palette `id` is in, or 0xFF
    u8 isFree[16];                // 0x0C
    u8 locked[16];                // 0x1C
    u8 slots[16][TILE_SIZE_4BPP]; // 0x2C - each bank's colours
    u8 dirty;                     // 0x22C

    PaletteCache();  // InitPaletteCache
    ~PaletteCache(); // DestroyPaletteCache
    void LoadSlot(s32 slot, s32 id);
    void BindSlot(s32 slot, s32 id);
    s32 ClaimSlot(s32 slot);
    void Unlock(s32 id);
    void Lock(s32 id);
    void UploadSlot(s32 slot);
    void Upload();
    u8 GetSlot(s32 id); // GetPaletteSlot
    s32 FreeSlot(s32 slot);
    void FreeUnlockedSlots();
    void SetSource(u16 count, const u8 *palettes);
    void Clear();
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(PaletteCache) == 0x230);

/* The sprite bank set (gSpriteBankSet, 4 bytes): InitLevelState points it
 * at gSpriteBankTable, the table the level's sprites come from. Its users
 * take the first bank's animations (`table->banks`, globals.h's
 * SPRITE_BANK_BASE) as the base of their byte offsets. */
class SpriteBankSet
{
public:
    const struct sprite_bank_table *table;

    /* Bank `bank`'s animations. FreezeLevelClock and TickLevelClock
     * read bank 47 twice around a call, and the ROM rebuilds its offset
     * (`movs #0x8d; lsls #2`) at each read: through this inline the
     * offset is a constant only after inlining, so gcc doesn't keep it
     * in a register across the call as it does for `banks[47]`. */
    const struct sprite_anim *Anims(s32 bank) const
    {
        return table->banks[bank].anims;
    }

    SpriteBankSet();  // InitSpriteBankSet
    ~SpriteBankSet(); // DestroySpriteBankSet
};

COMPILE_TIME_ASSERT(sprite_obj_hpp, sizeof(SpriteBankSet) == 4);

/* The sprite renderer (gSpriteRenderer, an empty object InitLevelState
 * allocates): draws a sprite's OAM pieces (DrawPieces in
 * src/gfx/sprite_pieces.cpp, DrawAffinePieces in affine_sprite_pieces.cpp). */
class SpriteRenderer
{
public:
    SpriteRenderer();  // InitSpriteRenderer
    ~SpriteRenderer(); // DestroySpriteRenderer
    void DrawAt(Sprite *part, s32 x, s32 y);
    void Draw(Sprite *part);
    void DrawPieces(Sprite *part, s32 *pos);       // DrawSpritePieces
    void DrawAffinePieces(Sprite *part, s32 *pos); // DrawAffineSpritePieces
};

#endif /* !GUARD_SPRITE_OBJ_HPP */
